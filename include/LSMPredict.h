#include <opencv2/imgcodecs.hpp>
#include <vector>

class LSMPredict{

private:
    float _nextData;
    cv::Mat _y;
    cv::Mat _A;
    cv::Mat _wlin;
    int _size;

public:
    LSMPredict() = default;
    LSMPredict(std::vector<float> dataList);

    float get_nextData();
    /**
     * @brief get the next data of the list
     * @return the next data of the list
    */
};