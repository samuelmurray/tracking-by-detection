#ifndef CPP_RANDOMTRACKER_H
#define CPP_RANDOMTRACKER_H


#include "Tracker.h"
#include "../detector/Detector.h"

class RandomTracker : public Tracker {
public:
    RandomTracker();

    std::vector<Tracking> track(const cv::Mat &image) override;

    std::vector<Tracking> track(const std::vector<Detection> &detections) override;

private:
    const std::shared_ptr<Detector> detector;
};


#endif //CPP_RANDOMTRACKER_H