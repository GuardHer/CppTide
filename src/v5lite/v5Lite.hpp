#include <fstream>
#include <iostream>
#include <sstream>
// #include <cuda_provider_factory.h>
#include <onnxruntime_cxx_api.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

struct Net_config
{
	float confThreshold;				 // Confidence threshold (置信度)
	std::wstring modelpath;				 // model path
	std::vector<std::string> classNames; // classNames
};

class v5Lite
{
public:
	v5Lite(Net_config config);
	void detect(cv::Mat &frame);

private:
	cv::Mat letter_(cv::Mat &img);
	void normalize_(const cv::Mat &img);

private:
	int64_t inpWidth, inpHeight;
	int maxSide, Padw, Padh;
	int64_t nout, num_proposal;
	float ratio;
	bool has_postprocess;
	Net_config model_config;
	Ort::Env env = Ort::Env(ORT_LOGGING_LEVEL_ERROR, "v5Lite");
	Ort::Session *ort_session = nullptr;
	Ort::SessionOptions sessionOptions = Ort::SessionOptions();
	std::vector<float> input_image_;
	std::vector<char *> input_names;
	std::vector<char *> output_names;
	std::vector<std::vector<int64_t>> input_node_dims;	// >=1 outputs
	std::vector<std::vector<int64_t>> output_node_dims; // >=1 outputs
};