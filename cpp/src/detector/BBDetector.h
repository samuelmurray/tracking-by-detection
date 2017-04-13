#ifndef CPP_BBDETECTOR_H
#define CPP_BBDETECTOR_H


#include "Detector.h"

#include <opencv2/core/mat.hpp>

#ifdef USE_CAFFE

#include <caffe/caffe.hpp>

#endif //USE_CAFFE

#include <string>
#include <memory>

#ifdef USE_CAFFE

class BBDetector : public Detector {
public:
    BBDetector(const std::string &modelFile,
               const std::string &weightsFile,
               const std::string &meanFile,
               const std::string &meanValue);

    std::vector<Detection> detect(const cv::Mat &image, double confidenceThreshold) override;

private:
    void setMean(const std::string &meanFile, const std::string &meanValue);

    void wrapInputLayer(std::vector<cv::Mat> *inputChannels);

    void preprocess(const cv::Mat &image, std::vector<cv::Mat> *inputChannels);

    std::shared_ptr<caffe::Net<float>> net;
    cv::Size inputGeometry;
    int numChannels;
    cv::Mat mean;
};

#else // USE_CAFFE

class BBDetector : public Detector {
public:
    BBDetector();

    BBDetector(const std::string &model_file,
               const std::string &weights_file,
               const std::string &mean_file,
               const std::string &mean_value);

    std::vector<Detection> detect(const cv::Mat &image, double confidenceThreshold) override;
};

#endif  //USE_CAFFE


#endif //CPP_BBDETECTOR_H