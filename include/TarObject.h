#ifndef TAROBJECT_H
#define TAROBJECT_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <omp.h>

class TarObeject
{
    /**
     * @brief The class is the data of the target object
     * @param _center the geometric center of the object
     * @param _radius the radius of the minist enclosing circle of the object points
     * @param _orient the move orientation of the object, is used to instruct the direction, 0 is for none, 1 for left, 2 for right
     * @param _isBlocked the flag to indicate if the object is blocked to choose the mode of the indicator
    */
    private:
        cv::Point2f _center=cv::Point2f(0, 0);

        bool _isBlocked=false;
        float _radius=0;
        int _orient = 0;

    public:
        TarObeject() = default;

        void set_center(cv::Point2f center);
        /**
         * @brief set the center of the object
         * @param center the center of the object
         */

        cv::Point2f get_center();
        /**
         * @brief get the center of the object
         * @return the center of the object
         */

        void set_radius(float radius);
        /**
         * @brief set the radius of the object
         */

        float get_radius();
        /**
         * @brief get the radius of the object
         * @return the radius of the object
         */

        void set_orient(int orient);
        /**
         * @brief set the orientation of the object
         * @param orient the orientation of the object
        */

        int get_orient();
        /**
         * @brief get the orientation of the object
         * @return the orientation of the object
        */

        void get_MEC1(cv::Mat mask);
        /**
         * @brief get the minimum enclosing circle's info by findContours
         * @param mask the mask of the object, the mask must be binary image
         * @remarks the function will set the center, area of the object
         */
        
        void get_MEC2(cv::Mat mask);
        /**
         * @brief get the minimum enclosing circle's info by connectedComponents
         * @param mask the mask of the object, the mask must be binary image
         * @remarks the function will set the center, area of the object
         */
        
        bool check_blocked(int leastArea);
         /**
          * @brief check if the object is blocked
          * @return true if the object is blocked
          */

         bool check_blocked();
         /**
          * @brief check if the object is blocked
          * @return true if the object is blocked
         */

         TarObeject(const TarObeject &b);

};

#endif