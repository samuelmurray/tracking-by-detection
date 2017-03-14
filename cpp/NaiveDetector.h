#ifndef CPP_NAIVEDETECTOR_H
#define CPP_NAIVEDETECTOR_H


#include "Detector.h"

class NaiveDetector : public Detector {
public:
    NaiveDetector();

    std::vector<Detection> detect(const cv::Mat &image) override;
};


#endif //CPP_NAIVEDETECTOR_H