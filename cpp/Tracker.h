#ifndef CPP_TRACKER_H
#define CPP_TRACKER_H

#include <vector>
#include "Detector.h"
#include "Detection.h"
#include "TrackerState.h"

class Tracker {
public:
    Tracker();

    virtual ~Tracker();

    Tracker(const Tracker &) = delete;

    Tracker(const Tracker &&) = delete;

    Tracker &operator=(const Tracker &) = delete;

    Tracker &operator=(const Tracker &&) = delete;

    std::vector<Detection> track(); // TODO: image as input

private:
    std::vector<Detection> associate(const std::vector<Detection> newDetections) const;

    Tracker(Detector *);

    Detector *detector;
    TrackerState state;
};


#endif //CPP_TRACKER_H