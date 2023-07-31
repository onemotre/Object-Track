#ifndef OBJECTINFO_H
#define OBJECTINFO_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>

using namespace std;

class ObjectInfo
{
    /**
     * @brief The class to store the basic information of the target object
     * @param _colorHSV The HSV color range of the object, to filter out other objects
     * @param _leastArea The least area of the object, to filter out small noises
    */
    private:
        vector<vector<int>> _colorHSV;
        int _leastArea = 7000;

    public:

        ObjectInfo() = default;
        
        void set_colorHSV(vector<vector<int>> colorHSV);

        vector<vector<int>> get_colorHSV();

        void set_leastArea(int leastArea=7000);

        int get_leastArea();
        
        void append_colorHSV(vector<int> colorHSV);
        /**
         * @brief Append a color range to the object
        */
        
        cv::Mat getMask(cv::Mat frame);
        /**
         * @brief Get the mask of the object
         * @param frame The frame to get the mask from
         * @return The mask of a frame
         */


};

#endif