#ifndef CPP_SSDDETECTOR_H
#define CPP_SSDDETECTOR_H


#include "Detector.h"

#include <opencv2/core/mat.hpp>

#ifdef USE_CAFFE

#include <caffe/caffe.hpp>

#endif //USE_CAFFE

#include <string>
#include <memory>

#ifdef USE_CAFFE

class SSDDetector : public Detector {
public:
    SSDDetector(const std::string &modelFile, const std::string &weightsFile, const std::string &meanValue);

    std::vector<Detection> detect(const cv::Mat &image) override;

private:
    void setMean(const std::string &meanValue);

    /**
     * Wrap the input layer of the network in separate cv::Mat objects
     * (one per channel). This way we save one memcpy operation and we
     * don't need to rely on cudaMemcpy2D. The last preprocessing operation
     * will write the separate channels directly to the input layer.
     */
    void wrapInputLayer(std::vector<cv::Mat> *inputChannels);

    void preprocess(const cv::Mat &image, std::vector<cv::Mat> *inputChannels);

    std::shared_ptr<caffe::Net<float>> net;
    cv::Size inputGeometry;
    int numChannels;
    cv::Mat mean;
};

#else // USE_CAFFE

class SSDDetector : public Detector {
public:
    SSDDetector();

    SSDDetector(const std::string &model_file, const std::string &weights_file, const std::string &mean_value);

    std::vector<Detection> detect(const cv::Mat &image) override;
};

#endif  //USE_CAFFE


#endif //CPP_SSDDETECTOR_H