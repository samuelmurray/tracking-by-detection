#ifndef CPP_BBDETECTOR_H
#define CPP_BBDETECTOR_H


#include "Detector.h"

class BBDetector : public Detector {
public:
    BBDetector();

    std::vector<Detection> detect() const override; // TODO: image as input

private:
    static std::vector<Detection> parse();
};


#endif //CPP_BBDETECTOR_H