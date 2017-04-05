#include "MCSORT.h"

#include "../Affinity.h"

#include <dlib/optimization.h>

// Constructors

MCSORT::MCSORT() : MCSORT(1, 3) {}

MCSORT::MCSORT(int maxAge, int minHits)
        : maxAge(maxAge), minHits(minHits) {}

MCSORT::~MCSORT() {}

// Methods

std::vector<Tracking> MCSORT::track(const std::vector<Detection> &detections) {
    frameCount++;

    Association association = associateDetectionsToPredictors(detections, predictors, Affinity::iou);

    // Update matched predictors with assigned detections
    for (auto match : association.matches) {
        predictors.at(match.second).update(detections.at(match.first));
    }

    for (auto p : association.unmatchedPredictors) {
        predictors.at(p).update();
    }

    // Create and initialise new predictors for unmatched detections
    for (auto id : association.unmatchedDetections) {
        KalmanPredictor predictor(detections.at(id), ++classCount[detections.at(id).className]);
        predictors.push_back(std::move(predictor));
    }

    // Remove predictors that have been inactive for too long
    predictors.erase(std::remove_if(
            predictors.begin(), predictors.end(),
            [this](const KalmanPredictor &predictor) {
                return predictor.getTimeSinceUpdate() > maxAge;
            }), predictors.end());

    // Return trackings from active predictors
    std::vector<Tracking> trackings;
    for (auto it = predictors.begin(); it != predictors.end(); ++it) {
        if (it->getTimeSinceUpdate() < 1 &&
            (it->getHitStreak() >= minHits || frameCount <= minHits)) {
            trackings.push_back(it->getTracking());
        }
    }
    return trackings;
}

MCSORT::Association MCSORT::associateDetectionsToPredictors(
        const std::vector<Detection> &detections,
        const std::vector<KalmanPredictor> &predictors,
        double (*affinityMeasure)(const BoundingBox &a, const BoundingBox &b),
        double threshold) {

    const int DOUBLE_PRECISION = 100;
    std::vector<std::pair<int, int>> matches;
    std::vector<int> unmatchedDetections;
    std::vector<int> unmatchedPredictors;

    if (predictors.empty()) {
        for (int i = 0; i < detections.size(); ++i)
            unmatchedDetections.push_back(i);
        return MCSORT::Association{matches, unmatchedDetections, unmatchedPredictors};
    }

    dlib::matrix<int> cost(detections.size(), predictors.size());
    for (size_t row = 0; row < detections.size(); ++row) {
        for (size_t col = 0; col < predictors.size(); ++col) {
            cost(row, col) = int(DOUBLE_PRECISION * affinityMeasure(detections.at(row).bb,
                                                                    predictors.at(col).getPredictedNextDetection().bb));
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
        if (cost(d, assignment[d]) < threshold * DOUBLE_PRECISION) {
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
    return MCSORT::Association{matches, unmatchedDetections, unmatchedPredictors};
}