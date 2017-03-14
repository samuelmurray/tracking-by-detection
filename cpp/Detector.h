#ifndef CPP_DETECTOR_H
#define CPP_DETECTOR_H


#include <vector>
#include <opencv2/core/mat.hpp>
#include "Detection.h"

class Detector {
public:
    Detector();

    virtual ~Detector();

    Detector(const Detector &) = delete;

    Detector(const Detector &&) = delete;

    Detector &operator=(const Detector &) = delete;

    Detector &operator=(const Detector &&) = delete;

    virtual std::vector<Detection> detect(const cv::Mat &image) = 0;
};


#endif //CPP_DETECTOR_H