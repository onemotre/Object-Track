#include "imgDrawer.h"

imgDrawer::imgDrawer(cv::Mat img, cv::Point2f center, float radius, cv::Scalar color)
{
    this->_img = img;
    this->_center = center;
    this->_radius = radius;
    this->_color = color;
}

cv::Point2f imgDrawer::get_center()
{
    return this->_center;
}

void imgDrawer::draw()
{
    cv::circle(this->_img, this->_center, this->_radius, this->_color, 2);
    return;
}