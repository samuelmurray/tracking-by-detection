#include "MCSORT.h"
#include "RandomDetector.h"
#include "NoAssociatedDetector.h"
#include <dlib/optimization.h>

using std::vector;
using std::cout;
using std::endl;

const int MCSORT::maxAge = 1;
const int MCSORT::minHits = 3;

// Constructors

MCSORT::MCSORT()
        : Tracker() {}

MCSORT::MCSORT(const std::shared_ptr<Detector> &detector)
        : Tracker(), detector(detector) {}

// Methods

std::vector<Tracking> MCSORT::track(const cv::Mat &image) {
    if (!detector) {
        throw NoAssociatedDetector();
    }
    vector<Detection> detections = detector->detect(image);
    return track(detections);
}

vector<Tracking> MCSORT::track(const vector<Detection> &detections) {
    frameCount++;

    cout << "---DETECTIONS---" << endl;
    for (auto it = detections.begin(); it != detections.end(); ++it) {
        cout << *it << endl;
    }

    cout << "---PREDICTIONS---" << endl;
    for (auto it = predictors.begin(); it != predictors.end(); ++it) {
        cout << it->getCurrentPrediction() << endl;
    }

    Association association = associateDetectionsToPredictors(detections);
    cout << endl << "Number of matches and unmatched detections and predictors:" << endl;
    cout << association.matches.size() << " " <<
         association.unmatchedDetections.size() << " " <<
         association.unmatchedPredictors.size() << endl;

    // Update matched trackers with assigned detections
    for (auto match : association.matches) {
        predictors.at(match.second).update(detections.at(match.first));
    }

    // Create and initialise new trackers for unmatched detections
    for (auto id : association.unmatchedDetections) {
        DetectionPredictor predictor(detections.at(id));
        predictors.push_back(std::move(predictor));
    }

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
    for (auto it = predictors.begin(); it != predictors.end(); ++it) {
        if (it->getTimeSinceUpdate() < 1 &&
            (it->getHitStreak() >= minHits || frameCount <= minHits)) {
            trackings.push_back(it->getTracking());
        }
    }
    return trackings;
}

MCSORT::Association MCSORT::associateDetectionsToPredictors(const vector<Detection> &detections) {
    double IOU_THRESHOLD = 0.3;
    int doublePrecision = 100;
    std::vector<std::pair<int, int>> matches;
    vector<int> unmatchedDetections;
    vector<int> unmatchedPredictors;

    if (predictors.empty()) {
        cout << "No predictors, creating..." << endl;
        for (int i = 0; i < detections.size(); ++i)
            unmatchedDetections.push_back(i);
        return MCSORT::Association{matches, unmatchedDetections, unmatchedPredictors};
    }
    dlib::matrix<int> cost(detections.size(), predictors.size());
    for (size_t row = 0; row < detections.size(); ++row) {
        for (size_t col = 0; col < predictors.size(); ++col) {
            cost(row, col) = int(doublePrecision *
                                 Detection::iou(detections.at(row), predictors.at(col).getCurrentPrediction()));
        }
    }
    // Pad with 0s to make square
    if (cost.nr() > cost.nc()) {
        cost = dlib::join_rows(cost, dlib::zeros_matrix<int>(1, cost.nr() - cost.nc()));
    } else if (cost.nc() > cost.nr()) {
        cost = dlib::join_cols(cost, dlib::zeros_matrix<int>(cost.nc() - cost.nr(), 1));
    }
    cout << cost;

    vector<long> assignment = dlib::max_cost_assignment(cost);
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
            unmatchedDetections.push_back(d);
            unmatchedPredictors.push_back(int(assignment[d]));
        } else {
            matches.push_back(std::pair<int, int>(d, assignment[d]));
        }
    }
    return MCSORT::Association{matches, unmatchedDetections, unmatchedPredictors};
}