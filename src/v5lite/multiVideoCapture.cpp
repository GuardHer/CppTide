#include "src/v5lite/multiVideoCapture.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <trantor/utils/Logger.h>

MultiVideoCapture::MultiVideoCapture(int camera_num)
    : camera_num_(camera_num)
{
    cap_map_.clear();
    init(camera_num);
}

MultiVideoCapture::~MultiVideoCapture()
{
    closeAll();
}

void MultiVideoCapture::init(int camera_num)
{
    for (int i = 0; i < camera_num; i++) {
        cap_map_.insert(std::make_pair(i, cv::VideoCapture()));
    }
}

cv::VideoCapture &MultiVideoCapture::get(int index)
{
    return cap_map_[index];
}

void MultiVideoCapture::close(int index)
{
    /// find the camera and release it
    auto it = cap_map_.find(index);
    if (it != cap_map_.end()) {
        it->second.release();
    }
}

void MultiVideoCapture::closeAll()
{
    for (auto &cap: cap_map_) {
        cap.second.release();
    }
}

bool MultiVideoCapture::isCloseAll()
{
    for (auto &cap: cap_map_) {
        if (cap.second.isOpened()) {
            return false;
        }
    }
    return true;
}

void MultiVideoCapture::open(int index)
{
    auto it = cap_map_.find(index);
    if (it != cap_map_.end()) {
        it->second.open(index);
    }
}

bool MultiVideoCapture::isOpened(int index)
{
    auto it = cap_map_.find(index);
    if (it != cap_map_.end()) {
        return it->second.isOpened();
    }
    return false;
}