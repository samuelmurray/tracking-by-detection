#include "RandomTracker.h"
#include <map>
#include "RandomDetector.h"
#include "NoAssociatedDetector.h"

using std::vector;
using std::map;
using std::string;

// Constructors

RandomTracker::RandomTracker()
        : Tracker(), detector(std::make_shared<RandomDetector>(RandomDetector())) {}

// Methods

vector<Tracking> RandomTracker::track(const cv::Mat &image) {
    if (!detector) {
        throw NoAssociatedDetector();
    }
    vector<Detection> detections = detector->detect(image);
    return track(detections);
}

vector<Tracking> RandomTracker::track(const vector<Detection> &detections) {
    vector<Tracking> associatedDetections;
    map<string, int> classCount;
    for (auto det : detections) {
        Tracking d(det.className, classCount[det.className]++, det.bb);
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}