#include "MCSORT.h"
#include "RandomDetector.h"
#include <dlib/optimization.h>

using std::vector;

const int MCSORT::maxAge = 1;
const int MCSORT::minHits = 3;

// Constructors

MCSORT::MCSORT()
        : Tracker(std::make_shared<RandomDetector>(RandomDetector())) {}

// Methods

vector<Tracking> MCSORT::track(const cv::Mat &image) {
    frameCount++;
    vector<Detection> boundingBoxes = detector->detect(image);
    vector<Detection> predictions;
    for (auto predictor : predictors) {
        predictor.predict();
    }
    std::cout << "\nPREDICTED\n";
    Association association = associateDetectionsToTrackers(boundingBoxes);

    std::cout << "\nASSOCIATED\n";

    // Update matched trackers with assigned detections
    for (auto match : association.matches) {
        predictors.at(match.second).update(boundingBoxes.at(match.first));
    }
    std::cout << "\nUPDATED\n";

    // Create and initialise new trackers for unmatched detections
    for (auto detection : association.unmatchedDetections) {
        DetectionPredictor predictor(detection);
        std::cout << "\nWUT\n";
        predictors.push_back(predictor);
        std::cout << "\nPUSHED\n";
    }
    std::cout << "\nINITIALIZED\n";

    // Remove predictors that have been inactive for too long
    /*
    auto new_end = std::remove_if(predictors.begin(), predictors.end(),
                                  [](const DetectionPredictor &filterer) {
                                      return filterer.getTimeSinceUpdate() > maxAge;
                                  });
    predictors.erase(new_end, predictors.end());
     */

    // Add the tracked objects
    vector<Tracking> trackings;
    for (auto predictor : predictors) {
        if (predictor.getTimeSinceUpdate() < 1 &&
            (predictor.getHitStreak() >= minHits || frameCount <= minHits)) {
            trackings.push_back(predictor.getTracking());
        }
    }

    return trackings;
}

MCSORT::Association MCSORT::associateDetectionsToTrackers(const vector<Detection> &detections) {
    double IOU_THRESHOLD = 0.3;
    int doublePrecision = 100;
    // TODO: Implement
    if (predictors.empty()) {
        return MCSORT::Association{vector<std::pair<int, int>>(), detections, vector<DetectionPredictor>()};
    }
    dlib::matrix<int> cost(detections.size(), predictors.size());
    for (size_t row = 0; row < detections.size(); ++row) {
        for (size_t col = 0; col < predictors.size(); ++col) {
            cost(row, col) = int(doublePrecision * Detection::iou(detections.at(row),
                                                                  predictors.at(col).getCurrentPrediction()));
        }
    }
    if (cost.nr() > cost.nc()) {
        cost = dlib::join_rows(cost, dlib::zeros_matrix<int>(1, cost.nr() - cost.nc()));
    } else if (cost.nc() > cost.nr()) {
        cost = dlib::join_cols(cost, dlib::zeros_matrix<int>(cost.nc() - cost.nr(), 1));
    }

    vector<long> assignment = dlib::max_cost_assignment(cost);
    std::vector<std::pair<int, int>> matches;
    vector<Detection> unmatchedDetections;
    vector<DetectionPredictor> unmatchedPredictors;
    /* Maybe not needed? Matches to "padded" detections/predictors should have value 0
    for (int d = 0; d < assignment.size(); ++d) {
        if (assignment[d] >= predictors.size()) {
            unmatchedDetections.push_back(Detection(detections.at(d)));
        }
        if (d >= detections.size()) {
            unmatchedPredictors.push_back(DetectionPredictor(predictors.at(assignment[d])));
        }
    }
     */
    for (int d = 0; d < assignment.size(); ++d) {
        if (cost(d, assignment[d]) < IOU_THRESHOLD * doublePrecision) {
            unmatchedDetections.push_back(Detection(detections.at(d)));
            unmatchedPredictors.push_back(DetectionPredictor(predictors.at(assignment[d])));
        } else {
            matches.push_back(std::pair<int, int>(d, assignment[d]));
        }
    }
    return MCSORT::Association{matches, unmatchedDetections, unmatchedPredictors};
}