#ifndef CPP_VIDEOTRACKER_H
#define CPP_VIDEOTRACKER_H


#include "detector/Detector.h"
#include "tracker/Tracker.h"

#include <memory>

class VideoTracker {
public:
    VideoTracker(const std::shared_ptr<Detector> &detector, const std::shared_ptr<Tracker> &tracker);

    /**
     * For each frame, gets a set of Detections from Detector and tracks them with Tracker.
     * Returns the Trackings.
     */
    std::vector<Tracking> track(const cv::Mat &image) const;

private:
    std::shared_ptr<Detector> detector;
    std::shared_ptr<Tracker> tracker;
};


#endif //CPP_VIDEOTRACKER_H