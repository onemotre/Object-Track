#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <vector>

class imgDrawer
{
    private:
        cv::Point2f _center;
        float _radius;
        cv::Scalar _color = cv::Scalar(0, 255, 0);
        cv::Mat _img;

    public:
        imgDrawer() = default;
        imgDrawer(cv::Mat img, cv::Point2f center, float radius, cv::Scalar color);
        /*
         * @brief Construct a new img Drawer object
         * @param center the center of the circle
         * @param radius the radius of the circle
         * @param img the image to draw the circle on
         */

        cv::Point2f get_center();
        /**
         * @brief Get the center of the circle
         * @return cv::Point2f the center of the circle
        */

        void draw();
        /*
         * @brief draw the circle on the image
         */
};