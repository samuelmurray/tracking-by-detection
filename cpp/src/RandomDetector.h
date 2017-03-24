#ifndef CPP_RANDOMDETECTOR_H
#define CPP_RANDOMDETECTOR_H


#include "Detector.h"

class RandomDetector : public Detector {
public:
    RandomDetector();

    RandomDetector(RandomDetector &&rhs);

    RandomDetector &operator=(RandomDetector &&rhs);

    std::vector<Detection> detect(const cv::Mat &image) override;
};


#endif //CPP_RANDOMDETECTOR_H