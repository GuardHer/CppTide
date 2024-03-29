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

/// @brief YOLOv5-lite model
struct Net_config
{
    float confThreshold;    // Confidence threshold
    float nmsThreshold;     // Non-maximum suppression threshold
    float objThreshold;     // Object Confidence threshold
    std::string model_path; // Model path
    std::string classesFile;// Class name file
};

/// @brief Box information structure
typedef struct BoxInfo
{
    float x1;   // x1: top left x
    float y1;   // y1: top left y
    float x2;   // x2: bottom right x
    float y2;   // y2: bottom right y
    float score;// score
    int label;  // label
} BoxInfo;

class V5Lite
{

public:
    V5Lite();
    V5Lite(Net_config config);
    ~V5Lite();

    /// @brief Detection function: detect the input image
    /// @param frame Input image [cv::Mat &]
    void detect(cv::Mat &frame);

    /// @brief Initialize the model
    /// @param config Model configuration [Net_config]
    void init(Net_config config);

    /// @brief Whether the model is initialized
    /// @return true: initialized, false: not initialized
    bool isInit() const { return is_initialized; }

private:
    cv::Mat resize_image(cv::Mat srcimg, int *newh, int *neww, int *top, int *left);
    void normalize_(cv::Mat img);
    void nms(std::vector<BoxInfo> &input_boxes);

private:
    const float anchors[3][6] = { { 10.0, 13.0, 16.0, 30.0, 33.0, 23.0 }, { 30.0, 61.0, 62.0, 45.0, 59.0, 119.0 }, { 116.0, 90.0, 156.0, 198.0, 373.0, 326.0 } };
    const float stride[3]     = { 8.0, 16.0, 32.0 };
    int inpWidth;                                                // Width of network's input image
    int inpHeight;                                               // Height of network's input image
    int num_class;                                               // Number of classes
    float confThreshold;                                         // Confidence threshold
    float nmsThreshold;                                          // Non-maximum suppression threshold
    float objThreshold;                                          // Object Confidence threshold
    bool is_initialized { false };                               // Whether the model is initialized
    const bool keep_ratio { true };                              // Keep the ratio of the image
    Ort::Env env { Ort::Env(ORT_LOGGING_LEVEL_ERROR, "V5Lite") };// Ort::Env
    Ort::Session *ort_session          = nullptr;                // Ort::Session
    Ort::SessionOptions sessionOptions = Ort::SessionOptions();  // Ort::SessionOptions
    std::vector<char *> input_names;                             // input_names
    std::vector<char *> output_names;                            // output_names
    std::vector<float> input_image_;                             // input_image
    std::vector<std::string> class_names;                        // class_names
    std::vector<std::vector<int64_t>> input_node_dims;           // >=1 outputs
    std::vector<std::vector<int64_t>> output_node_dims;          // >=1 outputs
};

}// namespace cpptide::YOLO