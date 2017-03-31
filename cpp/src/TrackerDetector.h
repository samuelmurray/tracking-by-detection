#ifndef CPP_TRACKERDETECTOR_H
#define CPP_TRACKERDETECTOR_H


#include "detector/Detector.h"
#include "tracker/Tracker.h"

class TrackerDetector {
public:
    TrackerDetector(const std::shared_ptr<Detector> &detector, const std::shared_ptr<Tracker> &tracker);

    std::vector<Tracking> track(const cv::Mat &image) const;

private:
    std::shared_ptr<Detector> detector;
    std::shared_ptr<Tracker> tracker;
};


#endif //CPP_TRACKERDETECTOR_H