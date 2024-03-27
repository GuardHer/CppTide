#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
// #include <cuda_provider_factory.h>
#include <onnxruntime/onnxruntime_cxx_api.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


namespace cpptide::YOLO
{

struct Net_config
{
    float confThreshold;// Confidence threshold
    float nmsThreshold; // Non-maximum suppression threshold
    float objThreshold; //Object Confidence threshold
    std::string model_path;
    std::string classesFile;
};


typedef struct BoxInfo
{
    float x1;
    float y1;
    float x2;
    float y2;
    float score;
    int label;
} BoxInfo;

class V5Lite
{

public:
    V5Lite();
    V5Lite(Net_config config);
    ~V5Lite();
    void detect(cv::Mat &frame);

    void init(Net_config config);

    bool isInit() const { return is_initialized; }

private:
    bool is_initialized = false;

    const float anchors[3][6] = { { 10.0, 13.0, 16.0, 30.0, 33.0, 23.0 }, { 30.0, 61.0, 62.0, 45.0, 59.0, 119.0 }, { 116.0, 90.0, 156.0, 198.0, 373.0, 326.0 } };
    const float stride[3]     = { 8.0, 16.0, 32.0 };
    int inpWidth;
    int inpHeight;
    std::vector<std::string> class_names;
    int num_class;
    float confThreshold;
    float nmsThreshold;
    float objThreshold;

    cv::Mat resize_image(cv::Mat srcimg, int *newh, int *neww, int *top, int *left);
    std::vector<float> input_image_;
    void normalize_(cv::Mat img);
    void nms(std::vector<BoxInfo> &input_boxes);
    const bool keep_ratio              = true;
    Ort::Env env                       = Ort::Env(ORT_LOGGING_LEVEL_ERROR, "V5Litev5-lite");
    Ort::Session *ort_session          = nullptr;
    Ort::SessionOptions sessionOptions = Ort::SessionOptions();
    std::vector<char *> input_names;
    std::vector<char *> output_names;
    std::vector<std::vector<int64_t>> input_node_dims; // >=1 outputs
    std::vector<std::vector<int64_t>> output_node_dims;// >=1 outputs
};

}// namespace cpptide::YOLO