#include "VideoTracker.h"

VideoTracker::VideoTracker(const std::shared_ptr<Detector> &detector, const std::shared_ptr<Tracker> &tracker)
        : detector(detector), tracker(tracker) {}

std::vector<Tracking> VideoTracker::track(const cv::Mat &image) const {
    return tracker->track(detector->detect(image));
}