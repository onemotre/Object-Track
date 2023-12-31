#include "ObjectInfo.h"
#include "TarObject.h"
#include "imgDrawer.h"
#include "LSMPredict.h"
#include "dataTransformer.h"

#include <string>
#include <iostream>
#include <cstdlib>

std::string windowName = "Main Window";
std::string path = R"(../resource/orange2.mp4)";
cv::VideoCapture cap(path);
cv::Mat frame;

ObjectInfo orange;

void initObejectInfo()
{
    std::vector<int> colorHSV = {0, 159, 70, 180, 255, 255};
    orange.append_colorHSV(colorHSV);
    orange.set_leastArea(7000);
}

void setOrient(std::vector<TarObeject> dataList, TarObeject &currentOrange)
{
    TarObeject previousOrange;
    if(dataList.size() > 0){
        previousOrange = dataList[dataList.size() - 1];
        int minus = currentOrange.get_center().x - previousOrange.get_center().x;
        if(minus > 0)   currentOrange.set_orient(2);
        else            currentOrange.set_orient(1);
        } else{
            currentOrange.set_orient(0);
        }
    return;
}

TarObeject blockedObject(std::vector<TarObeject> dataList, cv::Mat img)
{
    if(dataList.empty()){
        std::cout << "can't predict with empty datalist!" << std::endl; 
        return TarObeject();
    }

    ///////////// preprogress the data /////////////
    std::vector<float> dataList_X;
    std::vector<float> dataList_Y;
    std::vector<float> dataList_R;
    for(auto item : dataList)
    {
        dataList_X.push_back(item.get_center().x);
        dataList_Y.push_back(item.get_center().y);
        dataList_R.push_back(item.get_radius());
    }

    ///////////// predict x, y, radius /////////////
    LSMPredict predictX(dataList_X);
    LSMPredict predictY(dataList_Y);
    LSMPredict predictR(dataList_R);

    cv::Point2f pre_center(predictX.get_nextData(), predictY.get_nextData());
    float pre_radius = predictR.get_nextData();

    TarObeject predict_ob;
    predict_ob.set_center(pre_center);
    predict_ob.set_radius(pre_radius);
    predict_ob.set_orient(dataList[dataList.size() - 1].get_orient());

    return predict_ob;
}

// predict the position two frames later by kalman
TarObeject kalmanFilter(TarObeject datalist)
{
    // transform the data
    dataTransformer newTransformer;
    newTransformer.setTarObject(datalist);
    newTransformer.sendTarObject();

    // run the kalman filter
    int returnCode = std::system("python3 -u ../scripts/kalman_v2.py");
    if(returnCode == 0) {
        // get the result from the result.csv
        TarObeject result = newTransformer.getResult();
        return result;
    } else {
        std::cout << "Error: can not run the kalman filter" << std::endl;
        exit(1);
    }
}

void predict()
{
    initObejectInfo();
    std::vector<TarObeject> orangeDataList;

    int frameNum = 0;
    for(;; frameNum++)
    {
        cap.read(frame);
        if(frame.empty())   { std::cout << "can't read the vedio" << std::endl; break; }

        ////////// get the mask binary image //////////
        cv::Mat mask;
        mask = orange.getMask(frame).clone();
        cv::imshow("mask test", mask);

        ////////// get the minimum enclosing circle //////////
        TarObeject currentOrange;
        currentOrange.get_MEC1(mask);

        ////////// collect data //////////
        if(!currentOrange.check_blocked(orange.get_leastArea()))  //is not blocked
        {
            std::cout << "collect" << std::endl;
            // draw the circle
            imgDrawer actual_ob = imgDrawer(frame, currentOrange.get_center(), 
                                        currentOrange.get_radius(), cv::Scalar(0, 255, 0));
            actual_ob.draw();
            // set the orient
            setOrient(orangeDataList, currentOrange);

            // update the dataList
            if(orangeDataList.size() > 0)
            {
                if(currentOrange.get_orient() != orangeDataList[orangeDataList.size() - 1].get_orient())
                    orangeDataList.clear();
                orangeDataList.push_back(currentOrange);
            }
            orangeDataList.push_back(currentOrange);
            
        } else {
            /////////// predict the position when obeject is blocked ///////////
            std::cout << "predict" << std::endl;
            TarObeject predict_ob = blockedObject(orangeDataList, frame);
            // draw the circle
            imgDrawer predict_ob_draw = imgDrawer(frame, predict_ob.get_center(), 
                                        predict_ob.get_radius(), cv::Scalar(0, 255, 255));
            predict_ob_draw.draw();
            // update teh dataList
            currentOrange = predict_ob;
            orangeDataList.push_back(predict_ob);
        }

        ///////// predict the position two frames later by kalman /////////
        TarObeject kalman_pre = kalmanFilter(currentOrange);
        // draw the circle
        imgDrawer kalman_ob_draw = imgDrawer(frame, kalman_pre.get_center(), 
                                        kalman_pre.get_radius(), cv::Scalar(255, 0, 0));
        kalman_ob_draw.draw();
        ///////// show the result /////////
        cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
        cv::imshow(windowName, frame);
        if(cv::waitKey(30) == 27) break; 
    }
    std::cout << "total frames is: " << frameNum << endl;
}

int main()
{
    predict();
    return 0;
}