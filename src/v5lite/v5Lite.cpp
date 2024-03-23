#include "src/v5lite/v5Lite.hpp"

#include <locale>
#include <codecvt>

v5Lite::v5Lite(Net_config config)
	: model_config(config)
{
	std::wstring model_path = config.modelpath;

	// std::wstring widestr = std::wstring(model_path.begin(), model_path.end());
	// OrtStatus* status = OrtSessionOptionsAppendExecutionProvider_CUDA(sessionOptions, 0);
	// ort_session = new Session(env, widestr.c_str(), sessionOptions);
	// window type
	sessionOptions.SetGraphOptimizationLevel(ORT_ENABLE_BASIC);
	// OrtCUDAProviderOptions cuda_options{};
	// sessionOptions.AppendExecutionProvider_CUDA(cuda_options);
	ort_session = new Ort::Session(env, model_path.c_str(), sessionOptions);
	// linux type

	size_t numInputNodes = ort_session->GetInputCount();
	size_t numOutputNodes = ort_session->GetOutputCount();
	Ort::AllocatorWithDefaultOptions allocator;
	for (int i = 0; i < numInputNodes; i++)
	{
		// input_names.push_back(ort_session->GetInputName(i, allocator)); (已弃用)
		auto input_name_Ptr = ort_session->GetInputNameAllocated(i, allocator);
		input_names.push_back(input_name_Ptr.get());

		Ort::TypeInfo input_type_info = ort_session->GetInputTypeInfo(i);
		auto input_tensor_info = input_type_info.GetTensorTypeAndShapeInfo();
		auto input_dims = input_tensor_info.GetShape();
		input_node_dims.push_back(input_dims);
	}
	for (int i = 0; i < numOutputNodes; i++)
	{
		// output_names.push_back(ort_session->GetOutputName(i, allocator)); (已弃用)
		auto output_name_Ptr = ort_session->GetInputNameAllocated(i, allocator);
		output_names.push_back(output_name_Ptr.get());

		Ort::TypeInfo output_type_info = ort_session->GetOutputTypeInfo(i);
		auto output_tensor_info = output_type_info.GetTensorTypeAndShapeInfo();
		auto output_dims = output_tensor_info.GetShape();
		output_node_dims.push_back(output_dims);
	}
	this->inpHeight = input_node_dims[0][2];
	this->inpWidth = input_node_dims[0][3];
}

void v5Lite::normalize_(const cv::Mat &img)
{
	int row = img.rows;
	int col = img.cols;
	this->input_image_.resize(row * col * img.channels());
	for (int c = 0; c < 3; c++)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				float pix = img.ptr<uchar>(i)[j * 3 + 2 - c];
				this->input_image_[c * row * col + i * col + j] = static_cast<float>(pix / 255.0);
			}
		}
	}
}

cv::Mat v5Lite::letter_(cv::Mat &img)
{
	cv::Mat dstimg;
	this->maxSide = img.rows > img.cols ? img.rows : img.cols;
	this->ratio = this->inpWidth / float(this->maxSide);
	int fx = static_cast<int>(img.cols * ratio);
	int fy = static_cast<int>(img.rows * ratio);
	this->Padw = static_cast<int>((this->inpWidth - fx) * 0.5);
	this->Padh = static_cast<int>((this->inpHeight - fy) * 0.5);
	resize(img, dstimg, cv::Size(fx, fy));
	copyMakeBorder(dstimg, dstimg, Padh, Padh, Padw, Padw, cv::BORDER_CONSTANT, cv::Scalar::all(127));
	return dstimg;
}

void v5Lite::detect(cv::Mat &frame)
{
	cv::Mat dstimg = this->letter_(frame);
	this->normalize_(dstimg);
	std::array<int64_t, 4> input_shape_{1, 3, this->inpHeight, this->inpWidth};

	auto allocator_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
	Ort::Value input_tensor_ = Ort::Value::CreateTensor<float>(allocator_info, input_image_.data(), input_image_.size(), input_shape_.data(), input_shape_.size());
	std::vector<Ort::Value> ort_outputs = ort_session->Run(Ort::RunOptions{nullptr}, &input_names[0], &input_tensor_, 1, output_names.data(), output_names.size());

	Ort::Value &predictions = ort_outputs.at(0);
	auto pred_dims = predictions.GetTensorTypeAndShapeInfo().GetShape();
	num_proposal = pred_dims.at(0);
	nout = pred_dims.at(1);
	const float *pdata = predictions.GetTensorMutableData<float>();

	for (int n = 0; n < this->num_proposal; n++)
	{
		float box_score = pdata[4 + nout * n];
		if (box_score > model_config.confThreshold)
		{
			int xmin = static_cast<int>((pdata[n * nout] - this->Padw) * (1.0 / this->ratio));
			int ymin = static_cast<int>((pdata[n * nout + 1] - this->Padh) * (1.0 / this->ratio));
			int xmax = static_cast<int>((pdata[n * nout + 2] - this->Padw) * (1.0 / this->ratio));
			int ymax = static_cast<int>((pdata[n * nout + 3] - this->Padh) * (1.0 / this->ratio));
			int label = static_cast<int>(pdata[n * nout + 5]);

			// printf("Class: %s result is %d %d %d %d %f\n", class_names[label], xmin, ymin, xmax, ymax, box_score);
			char text[256];
			sprintf_s(text, "%s:%.3f", model_config.classNames[label].c_str(), box_score);
			putText(frame, text, cv::Point(xmin, ymin - 5), cv::FONT_HERSHEY_TRIPLEX, 0.75, cv::Scalar(0, 255, 0), 1);
			rectangle(frame, cv::Point(xmin, ymin), cv::Point(xmax, ymax), cv::Scalar(0, 0, 255), 2);
		}
	}
}