#include "RandomTracker.h"

#include <map>

// Methods

std::vector<Tracking> RandomTracker::track(const std::vector<Detection> &detections) {
    std::vector<Tracking> associatedDetections;
    std::map<int, int> perLabelCount;
    for (const auto &det : detections) {
        Tracking d(det.label, perLabelCount[det.label]++, det.bb);
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}