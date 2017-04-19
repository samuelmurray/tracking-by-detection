#ifndef CPP_IMAGETRACKER_H
#define CPP_IMAGETRACKER_H


#include "detector/Detector.h"
#include "tracker/Tracker.h"

#include <memory>

class ImageTracker {
public:
    ImageTracker(const std::shared_ptr<Detector> &detector, const std::shared_ptr<Tracker> &tracker);

    /**
     * For each frame, gets a set of Detections from Detector and tracks them with Tracker.
     * Returns the Trackings.
     */
    std::vector<Tracking> detectAndTrack(const cv::Mat &image) const;

private:
    std::shared_ptr<Detector> detector;
    std::shared_ptr<Tracker> tracker;
};


#endif //CPP_IMAGETRACKER_H