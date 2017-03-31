#include "RandomTracker.h"

#include <map>

// Methods

std::vector<Tracking> RandomTracker::track(const std::vector<Detection> &detections) {
    std::vector<Tracking> associatedDetections;
    std::map<std::string, int> classCount;
    for (auto det : detections) {
        Tracking d(det.className, classCount[det.className]++, det.bb);
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}