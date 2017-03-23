#ifndef CPP_RANDOMTRACKER_H
#define CPP_RANDOMTRACKER_H


#include "Tracker.h"

class RandomTracker : public Tracker {
public:
    RandomTracker();

    std::vector<Tracking> track(const cv::Mat &image) override;
};


#endif //CPP_RANDOMTRACKER_H