#include "TarObject.h"
#include <cmath>
#include <iostream>

#define PI 3.1415926;

void TarObeject::set_center(cv::Point2f center)
{
    this->_center = center;
}

cv::Point2f TarObeject::get_center()
{
    return this->_center;
}

void TarObeject::set_radius(float radius)
{
    this->_radius = radius;
}

float TarObeject::get_radius()
{
    return this->_radius;
}

void TarObeject::set_orient(int orient)
{
    this->_orient = orient;
}

int TarObeject::get_orient()
{
    return this->_orient;
}

void TarObeject::get_MEC1(cv::Mat mask)
{
    if(mask.empty())    {std::cout << "failed to read the mask" << std::endl; return;}

    ////////// get contours area //////////
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    ////////// filter the areas //////////
    std::vector<std::vector<cv::Point>> filteredContours;
    for(int i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        std::cout << "area by contourArea:" << std::endl;
        if(area > 7000) // filter out noises
            filteredContours.push_back(contours[i]);
    }

    if(filteredContours.empty())
    {
        std::cout << "fail to get the object" << std::endl;
        return;
    }
    ////////// get the minist Enclosing circle //////////
    cv::minEnclosingCircle(filteredContours[0], this->_center, this->_radius);
    return;
}

////////////////// Abandoned ////////////////////
void TarObeject::get_MEC2(cv::Mat mask)
{
    if(mask.empty())
        return;

    ////////// get connnected components areas //////////
    cv::Mat labels, stats, centroids;
    int numLables = cv::connectedComponentsWithStats(mask, labels, stats, centroids, 8, CV_32S);
    std::vector<cv::Point2f> objectArea;
    cv::Mat_<uchar>::iterator it;
    cv::Mat_<uchar>::iterator end = mask.end<uchar>();

    ///////// get points //////////
    #pragma omp parallel for
    for(int label = 1; label < numLables; label++)  // iterate over the labels
    {
        
        for(it = mask.begin<uchar>(); it != end; ++it)
        {
            int x = it.pos().x;
            int y = it.pos().y;
            if(labels.at<int>(y, x) == label && stats.at<int>(label, 4) > 7000)
            {
                std::cout << "识别面积" << stats.at<int>(label, 4) << std::endl;
                #pragma omp critical
                objectArea.emplace_back(static_cast<float>(x), static_cast<float>(y));
            }
        }
    }
    
    if(objectArea.empty())
    {
        std::cout << "the object has been blocked" << std::endl;
        return;
    }
        

    ////////// get the minist Enclosing circle //////////
    cv::minEnclosingCircle(objectArea, this->_center, this->_radius);
    
}

bool TarObeject::check_blocked(int leastArea)
{
    std::cout << " 目前半径" << this->_radius << std::endl;
    double area = pow(this->_radius, 2) * PI;
    std::cout << area << std::endl;
    if(area < leastArea)
        this->_isBlocked = true;
    else
        this->_isBlocked = false;
    return _isBlocked;
}

bool TarObeject::check_blocked()
{
    return this->_isBlocked;
}

TarObeject::TarObeject(const TarObeject &b)
{
    this->_center = b._center;
    this->_isBlocked = b._isBlocked;
    this->_radius = b._radius;
    this->_orient = b._orient;
}