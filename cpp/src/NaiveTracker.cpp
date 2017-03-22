#include "NaiveTracker.h"

#include <map>
#include <string>

#include "NaiveDetector.h"

using std::vector;
using std::map;
using std::string;

// Constructors

NaiveTracker::NaiveTracker()
        : NaiveTracker(std::shared_ptr<NaiveDetector>(new NaiveDetector())) {}

NaiveTracker::NaiveTracker(std::shared_ptr<Detector> detector)
        : Tracker(detector) {}

// Methods

vector<Detection> NaiveTracker::track(cv::Mat image) {
    vector<BoundingBox> boundingBoxes = detector->detect(image);
    vector<Detection> associatedDetections;
    map<string, int> classCount;
    for (auto it : boundingBoxes) { //= boundingBoxes.begin(); it != boundingBoxes.end(); ++it) {
        Detection d(it, classCount[it.className]++);
        associatedDetections.push_back(d);
    }
    return associatedDetections;
}