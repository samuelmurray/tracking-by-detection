#include "Tracker.h"
#include "BBDetector.h"

// Constructors

Tracker::Tracker() :
        Tracker(new BBDetector) {}

Tracker::Tracker(Detector *detector) :
        detector(detector), state(TrackerState()) {}

Tracker::~Tracker() {
    delete detector;
}

// Methods

std::vector<Detection> Tracker::track() {
    std::vector<Detection> detections = detector->detect();
    std::vector<Detection> associatedDetections = associate(detections);
    state.update(associatedDetections);
    return associatedDetections;
}

std::vector<Detection> Tracker::associate(const std::vector<Detection> newDetections) const {
    std::vector<Detection> associatedDetections;
    int count = 0;
    for (std::vector<Detection>::const_iterator it = newDetections.begin(); it != newDetections.end(); ++it, ++count) {
        Detection d(*it);
        d.ID = count;
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}