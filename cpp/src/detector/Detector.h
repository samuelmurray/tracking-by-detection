#ifndef CPP_DETECTOR_H
#define CPP_DETECTOR_H


#include "../util/Detection.h"

#include <opencv2/core/mat.hpp>

#include <vector>

class Detector {
public:
    Detector() = default;

    virtual ~Detector() = default;

    // Prevent copying and moving
    Detector(const Detector &) = delete;

    Detector(Detector &&rhs) = delete;

    Detector &operator=(const Detector &) = delete;

    Detector &operator=(Detector &&rhs) = delete;

    /**
     * Detects objects in a given image.
     * Returns the Detections.
     */
    virtual std::vector<Detection> detect(const cv::Mat &image) = 0;
};


#endif //CPP_DETECTOR_H