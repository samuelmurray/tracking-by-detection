#ifndef CPP_PAOT_H
#define CPP_PAOT_H


#include "Tracker.h"
#include "predictor/Predictor.h"
#include "predictor/kalman/KalmanPredictor.h"

#include <memory>
#include <vector>

class PAOT : public Tracker {
    struct Association;

public:
    PAOT() = default;

    virtual ~PAOT() = default;

    /**
      * Uses a linear velocity Kalman filters to predict locations of objects from previous frame.
      * Associates detections to Kalman filters using an Affinity measure and the Hungarian Algorithm.
      */
    std::vector<Tracking> track(const std::vector<Detection> &detections) override;

private:
    const int maxAge = 2; // Original: 1
    const int minHits = 0; // Original: 3
    const double detectionThreshold = 0.4;
    const double affinityThreshold = 0.1;
    std::vector<std::shared_ptr<Predictor>> predictors;
    int trackCount = 0;
    int frameCount = 0;

    /**
     * Uses an Affinity measure and Hungarian algorithm to determine which detection corresponds to which Kalman filter.
     */
    static Association associateDetectionsToPredictors(
            const std::vector<Detection> &detections,
            const std::vector<std::shared_ptr<Predictor>> &predictors,
            double (*affinityMeasure)(const BoundingBox &a, const BoundingBox &b),
            double affinityThreshold);

    struct Association {
        std::vector<std::pair<int, int>> matching;
        std::vector<int> unmatchedDetections;
        std::vector<int> unmatchedPredictors;
    };
};


#endif //CPP_MCSORT_H