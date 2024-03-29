#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

namespace cpptide::YOLO
{

class MultiVideoCapture
{
public:
    MultiVideoCapture() {}
    explicit MultiVideoCapture(int camera_num);

    ~MultiVideoCapture();

public:
    /// @brief Get the camera object
    /// @param index Camera index
    /// @return cv::VideoCapture [cv::VideoCapture object &]
    cv::VideoCapture &get(int index);

    /// @brief Initialize the camera
    /// @param camera_num Number of cameras [default: 2]
    void init(int camera_num = 2);

    /// @brief Close the camera
    /// @param index Camera index
    void close(int index);

    /// @brief Close all cameras
    void closeAll();

    /// @brief Check if all cameras are closed
    /// @return true: all closed, false: not all closed
    bool isCloseAll();

    /// @brief Open the camera
    /// @param index Camera index
    void open(int index);

    /// @brief Check if the camera is open
    /// @param index Camera index
    /// @return true: open, false: not open
    bool isOpened(int index);

    /// @brief Get the number of cameras
    /// @return int [Number of cameras]
    int getCameraNum() const { return camera_num_; }

private:
    std::unordered_map<int, cv::VideoCapture> cap_map_;
    int camera_num_ { 0 };
};

}// namespace cpptide::YOLO