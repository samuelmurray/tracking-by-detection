#include "TrackerDetector.h"

TrackerDetector::TrackerDetector(const std::shared_ptr<Detector> &detector, const std::shared_ptr<Tracker> &tracker)
        : detector(detector), tracker(tracker) {}

std::vector<Tracking> TrackerDetector::track(const cv::Mat &image) const {
    return tracker->track(detector->detect(image));
}