#include "ObjectInfo.h"

std::vector<std::vector<int>> ObjectInfo::get_colorHSV()
{
    return this->_colorHSV;
}

void ObjectInfo::set_colorHSV(std::vector<std::vector<int>> colorHSV)
{
    this->_colorHSV = colorHSV;
}

void ObjectInfo::set_leastArea(int leastArea)
{
     this->_leastArea = leastArea;
}

int ObjectInfo::get_leastArea()
{
    return this->_leastArea;
}

void ObjectInfo::append_colorHSV(std::vector<int> colorHSV)
{
    this->_colorHSV.push_back(colorHSV);
}

cv::Mat ObjectInfo::getMask(cv::Mat frame)
{
    cv::Mat imgBlur, imgHSV, mask;

    // Gaussian blur to filter out noises
    int kernelSize = 5;
    cv::GaussianBlur(frame, imgBlur, cv::Size(kernelSize, kernelSize), 3, 0);
    
    // select colors
    cv::cvtColor(imgBlur, imgHSV, cv::COLOR_BGR2HSV);
    for(int i = 0; i < this->_colorHSV.size(); i++)
    {
        cv::Scalar lower(this->_colorHSV[i][0], this->_colorHSV[i][1], this->_colorHSV[i][2]);
        cv::Scalar upper(this->_colorHSV[i][3], this->_colorHSV[i][4], this->_colorHSV[i][5]);
        cv::inRange(imgHSV, lower, upper, mask);
    }
    
    return mask;
}

