#ifndef CPP_NAIVETRACKER_H
#define CPP_NAIVETRACKER_H


#include "Tracker.h"

class NaiveTracker : public Tracker {
public:
    NaiveTracker();

    std::vector<Detection> track(cv::Mat mat) override;

private:
    NaiveTracker(Detector *);

    std::vector<Detection> associate(const std::vector<Detection> newDetections) const;

    TrackerState state;
};


#endif //CPP_NAIVETRACKER_H