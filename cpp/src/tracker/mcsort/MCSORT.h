#ifndef CPP_MCSORT_H
#define CPP_MCSORT_H


#include "../Tracker.h"
#include "KalmanPredictor.h"

class MCSORT : public Tracker {
    struct Association;

public:
    MCSORT();

    MCSORT(int maxAge, int minHits);

    /**
      * Uses a linear velocity Kalman filters to predict locations of objects from previous frame.
      * Associates detections to Kalman filters using an Affinity measure and the Hungarian Algorithm.
      */
    std::vector<Tracking> track(const std::vector<Detection> &detections) override;

private:
    const int maxAge;
    const int minHits;
    std::vector<KalmanPredictor> predictors;
    int frameCount = 0;

    /**
     * Uses an Affinity measure and Hungarian algorithm to determine which detection corresponds to which Kalman filter.
     */
    static Association associateDetectionsToPredictors(
            const std::vector<Detection> &detections,
            const std::vector<KalmanPredictor> &predictors,
            double (*affinityMeasure)(const BoundingBox &a, const BoundingBox &b),
            double threshold = 0.3);

    struct Association {
        std::vector<std::pair<int, int>> matches;
        std::vector<int> unmatchedDetections;
        std::vector<int> unmatchedPredictors;
    };
};


#endif //CPP_MCSORT_H