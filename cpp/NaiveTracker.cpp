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
    std::vector<BoundingBox> boundingBoxes = detector->detect(image);
    std::vector<Detection> associatedDetections;
    std::map<std::string, int> classCount;
    for (std::vector<BoundingBox>::const_iterator it = boundingBoxes.begin(); it != boundingBoxes.end(); ++it) {
        Detection d(*it, classCount[it->className]++);
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}