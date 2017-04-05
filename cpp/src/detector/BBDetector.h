#ifndef CPP_BBDETECTOR_H
#define CPP_BBDETECTOR_H


#ifdef USE_CAFFE

#include "Detector.h"

#include <caffe/caffe.hpp>
#include <opencv2/core/mat.hpp>

#include <string>
#include <memory>

class BBDetector : public Detector {
public:
    BBDetector();

    BBDetector(const std::string &model_file,
               const std::string &weights_file,
               const std::string &mean_file,
               const std::string &mean_value);

    std::vector<Detection> detect(const cv::Mat &image) override;

private:
    void setMean(const std::string &mean_file, const std::string &mean_value);

    void wrapInputLayer(std::vector<cv::Mat> *input_channels);

    void preprocess(const cv::Mat &img, std::vector<cv::Mat> *input_channels);

    std::shared_ptr<caffe::Net<float>> net_;
    cv::Size input_geometry_;
    int num_channels_;
    cv::Mat mean_;
};

#endif  //USE_CAFFE


#endif //CPP_BBDETECTOR_H