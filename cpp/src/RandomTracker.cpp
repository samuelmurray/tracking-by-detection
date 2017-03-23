#include "RandomTracker.h"
#include <map>
#include "RandomDetector.h"

using std::vector;
using std::map;
using std::string;

// Constructors

RandomTracker::RandomTracker()
        : Tracker(std::make_shared<RandomDetector>(RandomDetector())) {}

// Methods

vector<Tracking> RandomTracker::track(const cv::Mat &image) {
    vector<Detection> detections = detector->detect(image);
    vector<Tracking> associatedDetections;
    map<string, int> classCount;
    for (auto det : detections) {
        Tracking d(det.className, classCount[det.className]++, det.bb);
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}