#ifndef CPP_NAIVETRACKER_H
#define CPP_NAIVETRACKER_H


#include "Tracker.h"

class NaiveTracker : public Tracker {
public:
    NaiveTracker();

    std::vector<Detection> track(cv::Mat mat) override;

private:
    NaiveTracker(Detector *);
};


#endif //CPP_NAIVETRACKER_H