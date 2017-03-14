#include "NaiveTracker.h"

#include <map>
#include <string>

#include "NaiveDetector.h"

// Constructors

NaiveTracker::NaiveTracker()
        : NaiveTracker(new NaiveDetector()) {}

NaiveTracker::NaiveTracker(Detector *detector)
        : Tracker(detector) {}

// Methods

std::vector<Detection> NaiveTracker::track(cv::Mat image) {
    std::vector<Detection> detections = detector->detect(image);
    std::vector<Detection> associatedDetections = associate(detections);
    return associatedDetections;
}

std::vector<Detection> NaiveTracker::associate(const std::vector<Detection> newDetections) const {
    std::vector<Detection> associatedDetections;
    int count = 0;
    std::map<std::string, int> classCount;
    for (std::vector<Detection>::const_iterator it = newDetections.begin(); it != newDetections.end(); ++it, ++count) {
        Detection d(*it);
        d.ID = classCount[d.className]++;
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}