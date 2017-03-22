#include "KalmanTracker.h"
#include "NaiveDetector.h"
#include <dlib/optimization.h>

using std::vector;

const int KalmanTracker::maxAge = 1;
const int KalmanTracker::minHits = 3;

// Constructors

KalmanTracker::KalmanTracker()
        : KalmanTracker(std::shared_ptr<NaiveDetector>(new NaiveDetector())) {}

KalmanTracker::KalmanTracker(std::shared_ptr<Detector> detector)
        : Tracker(detector) {}

// Methods

vector<Detection> KalmanTracker::track(cv::Mat mat) {
    frameCount++;
    vector<BoundingBox> boundingBoxes = detector->detect(mat);
    vector<BoundingBox> predictions;
    for (auto filterer : filterers) {
        filterer.predict();
    }
    std::cout << "\nPREDICTED\n";
    Association association = associateDetectionsToTrackers(boundingBoxes);

    std::cout << "\nASSOCIATED\n";

    // Update matched trackers with assigned detections
    for (auto match : association.matches) {
        filterers.at(match.second).update(boundingBoxes.at(match.first));
    }
    std::cout << "\nUPDATED\n";

    // Create and initialise new trackers for unmatched detections
    for (auto detection : association.unmatchedBoxes) {
        KalmanFilterer filterer(detection);
        std::cout << "\nWUT\n";
        filterers.push_back(filterer);
        std::cout << "\nPUSHED\n";
    }
    std::cout << "\nINITIALIZED\n";

    // Remove filterers that have been inactive for too long
    /*
    auto new_end = std::remove_if(filterers.begin(), filterers.end(),
                                  [](const KalmanFilterer &filterer) {
                                      return filterer.getTimeSinceUpdate() > maxAge;
                                  });
    filterers.erase(new_end, filterers.end());
     */

    // Add the tracked objects
    vector<Detection> trackings;
    for (auto filterer : filterers) {
        if (filterer.getTimeSinceUpdate() < 1 &&
            (filterer.getHitStreak() >= minHits || frameCount <= minHits)) {
            trackings.push_back(filterer.detection());
        }
    }

    return trackings;
}

KalmanTracker::Association KalmanTracker::associateDetectionsToTrackers(vector<BoundingBox> detections) {
    double IOU_THRESHOLD = 0.3;
    int doublePrecision = 100;
    // TODO: Implement
    if (filterers.empty()) {
        return KalmanTracker::Association{vector<std::pair<int, int>>(), detections, vector<KalmanFilterer>()};
    }
    dlib::matrix<int> cost(detections.size(), filterers.size());
    for (size_t row = 0; row < detections.size(); ++row) {
        for (size_t col = 0; col < filterers.size(); ++col) {
            cost(row, col) = int(doublePrecision * BoundingBox::iou(detections.at(row), filterers.at(col).getState()));
        }
    }
    if (cost.nr() > cost.nc()) {
        cost = dlib::join_rows(cost, dlib::zeros_matrix<int>(1, cost.nr() - cost.nc()));
    } else if (cost.nc() > cost.nr()) {
        cost = dlib::join_cols(cost, dlib::zeros_matrix<int>(cost.nc() - cost.nr(), 1));
    }

    vector<long> assignment = dlib::max_cost_assignment(cost);
    std::vector<std::pair<int, int>> matches;
    vector<BoundingBox> unmatchedDetections;
    vector<KalmanFilterer> unmatchedFilterers;
    /* Maybe not needed? Matches to "padded" detections/filterers should have value 0
    for (int d = 0; d < assignment.size(); ++d) {
        if (assignment[d] >= filterers.size()) {
            unmatchedDetections.push_back(BoundingBox(detections.at(d)));
        }
        if (d >= detections.size()) {
            unmatchedFilterers.push_back(KalmanFilterer(filterers.at(assignment[d])));
        }
    }
     */
    for (int d = 0; d < assignment.size(); ++d) {
        if (cost(d, assignment[d]) < IOU_THRESHOLD * doublePrecision) {
            unmatchedDetections.push_back(BoundingBox(detections.at(d)));
            unmatchedFilterers.push_back(KalmanFilterer(filterers.at(assignment[d])));
        } else {
            matches.push_back(std::pair<int, int>(d, assignment[d]));
        }
    }
    return KalmanTracker::Association{matches, unmatchedDetections, unmatchedFilterers};
}