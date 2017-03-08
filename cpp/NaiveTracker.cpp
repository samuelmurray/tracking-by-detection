#include "NaiveTracker.h"
#include "BBDetector.h"

// Constructors

NaiveTracker::NaiveTracker()
        : NaiveTracker(new BBDetector()) {}

NaiveTracker::NaiveTracker(Detector *detector)
        : Tracker(detector), state(TrackerState()) {}

// Methods

std::vector<Detection> NaiveTracker::track() {
    std::vector<Detection> detections = detector->detect();
    std::vector<Detection> associatedDetections = associate(detections);
    state.update(associatedDetections);
    return associatedDetections;
}

std::vector<Detection> NaiveTracker::associate(const std::vector<Detection> newDetections) const {
    std::vector<Detection> associatedDetections;
    int count = 0;
    for (std::vector<Detection>::const_iterator it = newDetections.begin(); it != newDetections.end(); ++it, ++count) {
        Detection d(*it);
        d.ID = count;
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}