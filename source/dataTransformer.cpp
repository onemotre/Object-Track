#include "dataTransformer.h"
#include <iostream>

dataTransformer::dataTransformer()
{
    this->_tarObject = TarObeject();
}

void dataTransformer::setTarObject(TarObeject tarObject)
{
    this->_tarObject = tarObject;
}

void dataTransformer::sendTarObject()
{
    std::ofstream csvFile("../tmp/data.csv", std::ios::app);

    if(!csvFile.is_open())
    {
        std::cout << "Error: can not open the file" << std::endl;
        exit(1);
    }
    // write the data to the file
    csvFile << this->_tarObject.get_center().x << ", " 
        << this->_tarObject.get_center().y << ", " 
        << this->_tarObject.get_radius() << ", " 
        << this->_tarObject.get_orient() << std::endl;

    csvFile.close();

    return;
}

bool dataTransformer::isResult()
{
    std::ifstream resultFile("../tmp/result.csv");

    if(!resultFile.is_open())
    {
        std::cout << "Error: can not open the file" << std::endl;
        exit(1);
    }

    std::string result;
    resultFile >> result;

    resultFile.seekg(0, std::ios::beg);
    if(resultFile.tellg() == 0)
    {
        resultFile.close();
        return false;
    }
    else
    {
        resultFile.close();
        return true;
    }
}

TarObeject dataTransformer::getResult()
{
    float center_x, center_y, radius;
    int orient;

    std::ifstream resultFile("../tmp/result.csv");

    if(!resultFile.is_open())
    {
        std::cout << "Error: can not open the file" << std::endl;
        exit(1);
    }

    std::vector<std::vector<std::string>> data;

    std::string line;
    while(std::getline(resultFile, line)) {
        std::vector<std::string> row;
        size_t start = 0;
        size_t end = 0;
        while(end = line.find(",", start), end != std::string::npos) {
            row.push_back(line.substr(start, end - start));
            start = end + 1;
        }
        row.push_back(line.substr(start));
        data.push_back(row);
    }
    resultFile.close();

    std::vector<std::string> lastRow = data[data.size() - 1];

    // get the result
    center_x = std::stof(lastRow[1]);
    center_y = std::stof(lastRow[2]);
    radius = std::stof(lastRow[3]);
    orient = std::stoi(lastRow[4]);
    
    TarObeject result;
    result.set_center(cv::Point2f(center_x, center_y));
    result.set_radius(radius);
    result.set_orient(orient);

    return result;
}