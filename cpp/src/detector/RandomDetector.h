#ifndef CPP_RANDOMDETECTOR_H
#define CPP_RANDOMDETECTOR_H


#include "Detector.h"

class RandomDetector : public Detector {
public:
    RandomDetector() = default;

    /**
      * Returns a set of (semi) random Detections, independent of the image.
      * Only used as a placeholder for a real Detector, e.g. for debugging.
      */
    std::vector<Detection> detect(const cv::Mat &image) override;
};


#endif //CPP_RANDOMDETECTOR_H