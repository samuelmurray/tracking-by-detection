#ifndef CPP_BBDETECTOR_H
#define CPP_BBDETECTOR_H


#include "Detector.h"

class BBDetector : public Detector {
public:
    BBDetector();

    std::vector<Detection> detect(cv::Mat image) const override;

private:
    static std::vector<Detection> parse();
};


#endif //CPP_BBDETECTOR_H