#ifndef CPP_DETECTOR_H
#define CPP_DETECTOR_H


#include <vector>
#include "Detection.h"

class Detector {
public:
    Detector();

    Detector(const Detector &) = delete;

    Detector(const Detector &&) = delete;

    Detector &operator=(const Detector &) = delete;

    Detector &operator=(const Detector &&) = delete;

    virtual ~Detector();
    
    virtual std::vector<Detection> detect() const = 0;
};


#endif //CPP_DETECTOR_H