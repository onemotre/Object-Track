#include "LSMPredict.h"
#include <iostream>
#include <cmath>

LSMPredict::LSMPredict(std::vector<float> dataList)
{

    cv::Mat_<float> y(0, 1, CV_64F);
    cv::Mat_<float> A(0, 4, CV_64F);
    
    float i = 1;
    for(auto item : dataList)
    {
        y.push_back(cv::Mat_<float>({item}));
        cv::Mat add = (cv::Mat_<float>(1,4) << pow(i, 3), (i * i), i, 1.0);
        A.push_back(add);
        i += 1.0;
    }
    
    this->_y = y.clone();
    this->_A = A.clone();
    std::cout << "A:"<< _A.size()<< std::endl << std::endl << this->_A << std::endl;
    std::cout << "y:" << _y.size()<< std::endl<< std::endl << this->_y << std::endl;

    this->_size = dataList.size();
}

float LSMPredict::get_nextData()
{
    cv::Mat invA;
    cv::invert(this->_A.t() * _A, invA);
    this->_wlin = invA * _A.t() * _y;

    this->_nextData = this->_wlin.at<float>(0, 0) * pow((_size + 1), 3) + this->_wlin.at<float>(1, 0) * pow((_size + 1), 2) + this->_wlin.at<float>(2, 0) * (_size + 1) + this->_wlin.at<float>(3, 0);
    return this->_nextData;
}