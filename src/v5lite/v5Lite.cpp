#include "src/v5lite/v5Lite.hpp"

#include <codecvt>
#include <locale>
#include <trantor/utils/Logger.h>
#include <utility>

using namespace YOLO;

v5Lite::v5Lite(Net_config config)
{
    this->confThreshold = config.confThreshold;
    this->nmsThreshold  = config.nmsThreshold;
    this->objThreshold  = config.objThreshold;

    std::string model_path = config.model_path;
    std::wstring widestr   = std::wstring(model_path.begin(), model_path.end());
    // OrtStatus* status = OrtSessionOptionsAppendExecutionProvider_CUDA(sessionOptions, 0);
    sessionOptions.SetGraphOptimizationLevel(ORT_ENABLE_BASIC);
    ort_session           = new Ort::Session(env, widestr.c_str(), sessionOptions);
    size_t numInputNodes  = ort_session->GetInputCount();
    size_t numOutputNodes = ort_session->GetOutputCount();
    Ort::AllocatorWithDefaultOptions allocator;
    for (int i = 0; i < numInputNodes; i++) {
        input_names.push_back(ort_session->GetInputName(i, allocator));
        // auto input_name_Ptr = ort_session->GetInputNameAllocated(i, allocator);
        // input_names.push_back(input_name_Ptr.get());

        Ort::TypeInfo input_type_info = ort_session->GetInputTypeInfo(i);
        auto input_tensor_info        = input_type_info.GetTensorTypeAndShapeInfo();
        auto input_dims               = input_tensor_info.GetShape();
        input_node_dims.push_back(input_dims);
    }
    for (int i = 0; i < numOutputNodes; i++) {
        output_names.push_back(ort_session->GetOutputName(i, allocator));
        // auto output_name_Ptr = ort_session->GetInputNameAllocated(i, allocator);
        // output_names.push_back(output_name_Ptr.get());

        Ort::TypeInfo output_type_info = ort_session->GetOutputTypeInfo(i);
        auto output_tensor_info        = output_type_info.GetTensorTypeAndShapeInfo();
        auto output_dims               = output_tensor_info.GetShape();
        output_node_dims.push_back(output_dims);
    }
    this->inpHeight         = static_cast<int>(input_node_dims[0][2]);
    this->inpWidth          = static_cast<int>(input_node_dims[0][3]);
    std::string classesFile = config.classesFile;
    std::ifstream ifs(classesFile.c_str());
    std::string line;
    while (getline(ifs, line)) this->class_names.push_back(line);
    this->num_class = static_cast<int>(this->class_names.size());
}

cv::Mat v5Lite::resize_image(cv::Mat srcimg, int *newh, int *neww, int *top, int *left)
{
    int srch = srcimg.rows, srcw = srcimg.cols;
    *newh = this->inpHeight;
    *neww = this->inpWidth;
    cv::Mat dstimg;
    if (this->keep_ratio && srch != srcw) {
        float hw_scale = (float) srch / srcw;
        if (hw_scale > 1) {
            *newh = this->inpHeight;
            *neww = int(this->inpWidth / hw_scale);
            resize(srcimg, dstimg, cv::Size(*neww, *newh), cv::INTER_AREA);
            *left = int((this->inpWidth - *neww) * 0.5);
            copyMakeBorder(dstimg, dstimg, 0, 0, *left, this->inpWidth - *neww - *left, cv::BORDER_CONSTANT, 0);
        } else {
            *newh = (int) (this->inpHeight * hw_scale);
            *neww = this->inpWidth;
            resize(srcimg, dstimg, cv::Size(*neww, *newh), cv::INTER_AREA);
            *top = (int) ((this->inpHeight - *newh) * 0.5);
            copyMakeBorder(dstimg, dstimg, *top, this->inpHeight - *newh - *top, 0, 0, cv::BORDER_CONSTANT, 0);
        }
    } else {
        resize(srcimg, dstimg, cv::Size(*neww, *newh), cv::INTER_AREA);
    }
    return dstimg;
}

void v5Lite::normalize_(cv::Mat img)
{
    //    img.convertTo(img, CV_32F);
    int row = img.rows;
    int col = img.cols;
    this->input_image_.resize(row * col * img.channels());
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                float pix                                       = img.ptr<uchar>(i)[j * 3 + 2 - c];
                this->input_image_[c * row * col + i * col + j] = (float) (pix / 255.0);
            }
        }
    }
}

void v5Lite::nms(std::vector<BoxInfo> &input_boxes)
{
    sort(input_boxes.begin(), input_boxes.end(), [](BoxInfo a, BoxInfo b) { return a.score > b.score; });
    std::vector<float> vArea(input_boxes.size());
    for (int i = 0; i < int(input_boxes.size()); ++i) {
        vArea[i] = (input_boxes.at(i).x2 - input_boxes.at(i).x1 + 1) * (input_boxes.at(i).y2 - input_boxes.at(i).y1 + 1);
    }

    std::vector<bool> isSuppressed(input_boxes.size(), false);
    for (int i = 0; i < int(input_boxes.size()); ++i) {
        if (isSuppressed[i]) { continue; }
        for (int j = i + 1; j < int(input_boxes.size()); ++j) {
            if (isSuppressed[j]) { continue; }
            float xx1 = (cv::max)(input_boxes[i].x1, input_boxes[j].x1);
            float yy1 = (cv::max)(input_boxes[i].y1, input_boxes[j].y1);
            float xx2 = (cv::min)(input_boxes[i].x2, input_boxes[j].x2);
            float yy2 = (cv::min)(input_boxes[i].y2, input_boxes[j].y2);

            float w     = (cv::max)(float(0), xx2 - xx1 + 1);
            float h     = (cv::max)(float(0), yy2 - yy1 + 1);
            float inter = w * h;
            float ovr   = inter / (vArea[i] + vArea[j] - inter);

            if (ovr >= this->nmsThreshold) {
                isSuppressed[j] = true;
            }
        }
    }
    // return post_nms;
    int idx_t = 0;
    input_boxes.erase(remove_if(input_boxes.begin(), input_boxes.end(), [&idx_t, &isSuppressed](const BoxInfo &f) { return isSuppressed[idx_t++]; }), input_boxes.end());
}

void v5Lite::detect(cv::Mat &frame)
{
    auto start = std::chrono::high_resolution_clock::now();

    int newh = 0, neww = 0, padh = 0, padw = 0;
    cv::Mat dstimg = this->resize_image(frame, &newh, &neww, &padh, &padw);
    this->normalize_(dstimg);
    std::array<int64_t, 4> input_shape_ { 1, 3, this->inpHeight, this->inpWidth };

    auto allocator_info      = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    Ort::Value input_tensor_ = Ort::Value::CreateTensor<float>(allocator_info, input_image_.data(), input_image_.size(), input_shape_.data(), input_shape_.size());

    // this->output_names[0] = "output";
    // this->input_names[0]  = "images";

    // 开始推理
    std::vector<Ort::Value> ort_outputs = ort_session->Run(Ort::RunOptions { nullptr }, &input_names[0], &input_tensor_, 1, output_names.data(), output_names.size());// 开始推理
    const float *preds                  = ort_outputs[0].GetTensorMutableData<float>();

    /////generate proposals
    std::vector<BoxInfo> generate_boxes;
    float ratioh = (float) frame.rows / newh, ratiow = (float) frame.cols / neww;
    int n = 0, q = 0, i = 0, j = 0, k = 0;///xmin,ymin,xamx,ymax,box_score,class_score
    const int nout = this->num_class + 5;
    for (n = 0; n < 3; n++)///特征图尺度
    {
        int num_grid_x = (int) (this->inpWidth / this->stride[n]);
        int num_grid_y = (int) (this->inpHeight / this->stride[n]);
        for (q = 0; q < 3; q++)///anchor
        {
            const float anchor_w = this->anchors[n][q * 2];
            const float anchor_h = this->anchors[n][q * 2 + 1];
            for (i = 0; i < num_grid_y; i++) {
                for (j = 0; j < num_grid_x; j++) {
                    float box_score = preds[4];
                    if (box_score > this->objThreshold) {
                        float class_score = 0;
                        int class_ind     = 0;
                        for (k = 0; k < this->num_class; k++) {
                            if (preds[k + 5] > class_score) {
                                class_score = preds[k + 5];
                                class_ind   = k;
                            }
                        }
                        //if (class_score > this->confThreshold)
                        //{
                        float cx = (float) (preds[0] * 2.f - 0.5f + j) * this->stride[n];///cx
                        float cy = (float) (preds[1] * 2.f - 0.5f + i) * this->stride[n];///cy
                        float w  = (float) powf(preds[2] * 2.f, 2.f) * anchor_w;         ///w
                        float h  = (float) powf(preds[3] * 2.f, 2.f) * anchor_h;         ///h

                        float xmin = (float) (cx - padw - 0.5 * w) * ratiow;
                        float ymin = (float) (cy - padh - 0.5 * h) * ratioh;
                        float xmax = (float) (cx - padw + 0.5 * w) * ratiow;
                        float ymax = (float) (cy - padh + 0.5 * h) * ratioh;

                        generate_boxes.push_back(BoxInfo { xmin, ymin, xmax, ymax, class_score, class_ind });
                        //}
                    }
                    preds += nout;
                }
            }
        }
    }
    auto end                                     = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;
    double fps                                   = 1.0 / elapsedSeconds.count();
    std::string fps_str                          = std::to_string(fps);

    nms(generate_boxes);
    for (size_t i = 0; i < generate_boxes.size(); ++i) {
        int xmin = int(generate_boxes[i].x1);
        int ymin = int(generate_boxes[i].y1);
        rectangle(frame, cv::Point(xmin, ymin), cv::Point(int(generate_boxes[i].x2), int(generate_boxes[i].y2)), cv::Scalar(0, 0, 255), 2);
        std::string label = cv::format("%.2f", generate_boxes[i].score);
        label             = this->class_names[generate_boxes[i].label] + ":" + label + ", FPS: " + fps_str;
        putText(frame, label, cv::Point(xmin, ymin - 5), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 255, 0), 2);
    }
}
