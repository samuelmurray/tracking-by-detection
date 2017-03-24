#ifndef CPP_DETECTOR_H
#define CPP_DETECTOR_H


#include "Detection.h"

#include <opencv2/core/mat.hpp>

#include <vector>

class Detector {
public:
    Detector() = default;

    virtual ~Detector() = default;

    Detector(Detector &&rhs) = default;

    Detector &operator=(Detector &&rhs) = default;

    virtual std::vector<Detection> detect(const cv::Mat &image) = 0;

    // Prevent copying
    Detector(const Detector &) = delete;

    Detector &operator=(const Detector &) = delete;
};


#endif //CPP_DETECTOR_H