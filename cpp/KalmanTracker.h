#ifndef CPP_KALMANTRACKER_H
#define CPP_KALMANTRACKER_H


#include "Tracker.h"
#include "KalmanFilterer.h"

class KalmanTracker : public Tracker {
public:
    static const int maxAge = 1;
    static const int minHits = 3;

public:
    KalmanTracker(Detector *);

    std::vector<Detection> track(cv::Mat mat) override;

private:
    std::vector<KalmanFilterer> filterers = std::vector<KalmanFilterer>();
    int frameCount = 1;
};


#endif //CPP_KALMANTRACKER_H