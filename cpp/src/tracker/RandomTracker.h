#ifndef CPP_RANDOMTRACKER_H
#define CPP_RANDOMTRACKER_H


#include "Tracker.h"
#include "../detector/Detector.h"

class RandomTracker : public Tracker {
public:
    RandomTracker() = default;

    std::vector<Tracking> track(const std::vector<Detection> &detections) override;
};


#endif //CPP_RANDOMTRACKER_H