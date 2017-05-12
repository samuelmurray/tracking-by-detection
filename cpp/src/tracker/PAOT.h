#ifndef CPP_PAOT_H
#define CPP_PAOT_H


#include "Tracker.h"
#include "predictor/Predictor.h"
#include "Affinity.h"

#include <dlib/optimization.h>

#include <memory>
#include <vector>

template<class T>
class PAOT : public Tracker {
    struct Association;

public:
    PAOT(const int maxAge, const int minHits, const double detectionThreshold, const double affinityThreshold,
         double (*affinityMeasureFnc)(const BoundingBox &a, const BoundingBox &b));

    virtual ~PAOT() = default;

    /**
      * Uses a linear velocity Kalman filters to predict locations of objects from previous frame.
      * Associates detections to Kalman filters using an Affinity measure and the Hungarian Algorithm.
      */
    std::vector<Tracking> track(const std::vector<Detection> &detections) override;

private:
    const int maxAge; // Original: 1, Okutama: 2
    const int minHits; // Original: 3, Okutama: 0
    const double detectionThreshold; // 0.4
    const double affinityThreshold; // Original: 0.3;
    double (*affinityMeasure)(const BoundingBox &a, const BoundingBox &b);

    std::vector<std::shared_ptr<T>> predictors;
    int trackCount = 0;
    int frameCount = 0;

    /**
     * Uses an Affinity measure and Hungarian algorithm to determine which detection corresponds to which Kalman filter.
     */
    Association associateDetectionsToPredictors(
            const std::vector<Detection> &detections,
            const std::vector<std::shared_ptr<T>> &predictors);

    struct Association {
        std::vector<std::pair<int, int>> matching;
        std::vector<int> unmatchedDetections;
        std::vector<int> unmatchedPredictors;
    };
};

// Constructors

template<class T>
PAOT<T>::PAOT(const int maxAge, const int minHits, const double detectionThreshold, const double affinityThreshold,
              double (*affinityMeasureFnc)(const BoundingBox &, const BoundingBox &))
        : maxAge(maxAge),
          minHits(minHits),
          detectionThreshold(detectionThreshold),
          affinityThreshold(affinityThreshold),
          affinityMeasure(affinityMeasureFnc),
          predictors(std::vector<std::shared_ptr<T>>()) {}

// Methods

template<class T>
std::vector<Tracking> PAOT<T>::track(const std::vector<Detection> &detections) {
    frameCount++;

    // Filter detections on confidence
    std::vector<Detection> strongDetections;
    for (const auto &detection : detections) {
        if (detection.confidence > detectionThreshold) {
            strongDetections.push_back(detection);
        }
    }

    // Associate detections to predictors
    Association association = associateDetectionsToPredictors(strongDetections, predictors);

    // Update matched predictors with assigned detections
    for (const auto &match : association.matching) {
        predictors.at(match.second)->update(strongDetections.at(match.first));
    }

    // Update unmatched predictors
    for (const auto p : association.unmatchedPredictors) {
        predictors.at(p)->update();
    }

    // Create and initialise new predictors for unmatched detections
    for (const auto id : association.unmatchedDetections) {
        auto predictor = std::make_shared<T>(strongDetections.at(id), ++trackCount);
        predictors.push_back(predictor);
    }

    // Remove predictors that have been inactive for too long
    predictors.erase(std::remove_if(
            predictors.begin(), predictors.end(),
            [this](const std::shared_ptr<T> &predictor) {
                return predictor->getTimeSinceUpdate() > maxAge;
            }), predictors.end());

    // Return trackings from active predictors
    std::vector<Tracking> trackings;
    for (auto it = predictors.begin(); it != predictors.end(); ++it) {
        if ((*it)->getTimeSinceUpdate() < 1 &&
            ((*it)->getHitStreak() >= minHits || frameCount <= minHits)) {
            trackings.push_back((*it)->getTracking());
        }
    }
    return trackings;
}

template<class T>
typename PAOT<T>::Association PAOT<T>::associateDetectionsToPredictors(const std::vector<Detection> &detections,
                                                              const std::vector<std::shared_ptr<T>> &predictors) {
    const int DOUBLE_PRECISION = 100; // Used to convert doubles to ints
    std::vector<std::pair<int, int>> matches;
    std::vector<int> unmatchedDetections;
    std::vector<int> unmatchedPredictors;

    // Handle case where no predictors exist
    if (predictors.empty()) {
        for (int i = 0; i < detections.size(); ++i)
            unmatchedDetections.push_back(i);
        return PAOT::Association{matches, unmatchedDetections, unmatchedPredictors};
    }

    // Create and fill cost matrix with affinity measure
    dlib::matrix<int> cost(detections.size(), predictors.size());
    for (size_t row = 0; row < detections.size(); ++row) {
        for (size_t col = 0; col < predictors.size(); ++col) {
            cost(row, col) = int(DOUBLE_PRECISION * affinityMeasure(
                    detections.at(row).bb,
                    predictors.at(col)->getPredictedNextDetection().bb));
        }
    }

    // Zero-pad to make it square
    if (cost.nr() > cost.nc()) {
        cost = dlib::join_rows(cost, dlib::zeros_matrix<int>(1, cost.nr() - cost.nc()));
    } else if (cost.nc() > cost.nr()) {
        cost = dlib::join_cols(cost, dlib::zeros_matrix<int>(cost.nc() - cost.nr(), 1));
    }

    std::vector<long> assignment = dlib::max_cost_assignment(cost);

    // Filter out matches with low IoU, including those for indices from padding
    for (int d = 0; d < assignment.size(); ++d) {
        if (cost(d, assignment[d]) < affinityThreshold * DOUBLE_PRECISION) {
            if (d < detections.size()) {
                unmatchedDetections.push_back(d);
            }
            if (assignment[d] < predictors.size()) {
                unmatchedPredictors.push_back(int(assignment[d]));
            }
        } else {
            matches.push_back(std::pair<int, int>(d, assignment[d]));
        }
    }
    return PAOT::Association{matches, unmatchedDetections, unmatchedPredictors};
}


#endif //CPP_MCSORT_H