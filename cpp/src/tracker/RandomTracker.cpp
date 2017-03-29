#include "RandomTracker.h"

#include "../detector/RandomDetector.h"
#include "NoAssociatedDetector.h"

#include <map>

// Constructors

RandomTracker::RandomTracker()
        : detector(std::make_shared<RandomDetector>(RandomDetector())) {}

// Methods

std::vector<Tracking> RandomTracker::track(const cv::Mat &image) {
    if (!detector) {
        throw NoAssociatedDetector();
    }
    std::vector<Detection> detections = detector->detect(image);
    return track(detections);
}

std::vector<Tracking> RandomTracker::track(const std::vector<Detection> &detections) {
    std::vector<Tracking> associatedDetections;
    std::map<std::string, int> classCount;
    for (auto det : detections) {
        Tracking d(det.className, classCount[det.className]++, det.bb);
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}