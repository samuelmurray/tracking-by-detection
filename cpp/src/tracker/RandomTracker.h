#ifndef CPP_RANDOMTRACKER_H
#define CPP_RANDOMTRACKER_H


#include "Tracker.h"
#include "../detector/Detector.h"

class RandomTracker : public Tracker {
public:
    RandomTracker() = default;

    /**
      * Assigns IDs to Detections in the same order as they arrive.
      * Only used as a placeholder for a real Tracker, e.g. for debugging.
      */
    std::vector<Tracking> track(const std::vector<Detection> &detections) override;
};


#endif //CPP_RANDOMTRACKER_H