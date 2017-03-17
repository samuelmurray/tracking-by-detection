#ifndef CPP_DETECTOR_H
#define CPP_DETECTOR_H


#include <opencv2/core/core.hpp>
#include <vector>
#include "BoundingBox.h"

class Detector {
public:
    Detector();

    virtual ~Detector();

    Detector(const Detector &) = delete;

    Detector(const Detector &&) = delete;

    Detector &operator=(const Detector &) = delete;

    Detector &operator=(const Detector &&) = delete;

    virtual std::vector<BoundingBox> detect(const cv::Mat &image) = 0;
};


#endif //CPP_DETECTOR_H