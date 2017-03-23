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
    vector<Detection> boundingBoxes = detector->detect(image);
    vector<Tracking> associatedDetections;
    map<string, int> classCount;
    for (auto it : boundingBoxes) {
        Tracking d(it, classCount[it.className]++);
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}