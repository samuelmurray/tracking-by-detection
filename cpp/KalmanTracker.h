#ifndef CPP_KALMANTRACKER_H
#define CPP_KALMANTRACKER_H


#include "Tracker.h"
#include "KalmanFilterer.h"

class KalmanTracker;

class KalmanTracker : public Tracker {
    struct Association;

public:
    static const int maxAge;
    static const int minHits;

public:
    KalmanTracker(Detector *);

    std::vector<Detection> track(cv::Mat mat) override;

private:
    std::vector<KalmanFilterer> filterers = std::vector<KalmanFilterer>();
    int frameCount = 1;

    Association associateDetectionsToTrackers(std::vector<BoundingBox> detections);

    struct Association {
        std::vector<std::pair<int, int>> matches;
        std::vector<BoundingBox> unmatchedBoxes;
        std::vector<KalmanFilterer> unmatchedFilterers;
    };
};


#endif //CPP_KALMANTRACKER_H