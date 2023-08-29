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

    std::string line;
    std::getline(resultFile, line);

    std::istringstream iss(line);
    std::string field;
    std::vector<std::string> fields;

    while(std::getline(iss, field, ','))
    {
        fields.push_back(field);
    }

    center_x = std::stof(fields[0]);
    center_y = std::stof(fields[1]);
    radius = std::stof(fields[2]);
    orient = std::stoi(fields[3]);

    TarObeject result;
    result.set_center(cv::Point2f(center_x, center_y));
    result.set_radius(radius);
    result.set_orient(orient);

    return result;
}