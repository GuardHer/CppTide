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
    cv::VideoCapture &get(int index);

    void init(int camera_num = 2);

    void close(int index);

    void closeAll();

    bool isCloseAll();

    void open(int index);

    bool isOpened(int index);

    int getCameraNum() const { return camera_num_; }

private:
    std::unordered_map<int, cv::VideoCapture> cap_map_;
    int camera_num_ { 0 };
};

}// namespace cpptide::YOLO