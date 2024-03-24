#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

class MultiVideoCapture
{
public:
    MultiVideoCapture(int camera_num = 2);

    ~MultiVideoCapture();

public:
    cv::VideoCapture &get(int index);

    void close(int index);

    void closeAll();

    bool isCloseAll();

    void open(int index);

    bool isOpened(int index);

private:
    void init(int camera_num = 2);

private:
    std::unordered_map<int, cv::VideoCapture> cap_map_;
    int camera_num_ { 0 };
};