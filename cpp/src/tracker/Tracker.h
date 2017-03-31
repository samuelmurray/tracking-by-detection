#ifndef CPP_TRACKER_H
#define CPP_TRACKER_H


#include "../util/Detection.h"
#include "../util/Tracking.h"

#include <vector>

class Tracker {
public:
    Tracker() = default;

    virtual ~Tracker() = default;

    // Prevent copying and moving
    Tracker(const Tracker &) = delete;

    Tracker(const Tracker &&) = delete;

    Tracker &operator=(const Tracker &) = delete;

    Tracker &operator=(const Tracker &&) = delete;

    /**
     * For each timestep, tracks the given Detections.
     * Returns the Trackings.
     */
    virtual std::vector<Tracking> track(const std::vector<Detection> &detections) = 0;
};


#endif //CPP_TRACKER_H