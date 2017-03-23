#ifndef CPP_DETECTIONPREDICTOR_H
#define CPP_DETECTIONPREDICTOR_H


#include <vector>
#include <dlib/matrix.h>
#include <dlib/filtering.h>
#include "Detection.h"
#include "Tracking.h"

class DetectionPredictor {
public:
    static constexpr int numStates = 7; // [x, y, area, ratio, vx, vy, area_change]
    static constexpr int numMeas = 4; // [x, y, area, ratio]
    static const double dt;
    static int count; // TODO: This should be a dict from class to count

public:
    DetectionPredictor(const Detection &initialState);

    virtual ~DetectionPredictor();

    /**
     * Updates the state vector with observed bbox.
     */
    void update(const Detection &det);

    /**
     * Advances the state vector and returns the predicted bounding box estimate.
     */
    Detection predict();

    /**
     * Returns the current bounding box estimate.
     */
    Detection getCurrentPrediction() const;

    Tracking getTracking() const;

    int getHitStreak() const;

    int getTimeSinceUpdate() const;

    static BoundingBox stateToBoundingBox(const dlib::matrix<double, numStates, 1> &state);

    static dlib::matrix<double, numMeas, 1> boundingBoxToMeas(const BoundingBox &bb);

private:
    std::shared_ptr<dlib::kalman_filter<numStates, numMeas>> filter;
    std::vector<Detection> history;
    const int ID;
    const std::string className;
    int timeSinceUpdate = 0;
    int hits = 0;
    int hitStreak = 0;
    int age = 0;
};


#endif //CPP_DETECTIONPREDICTOR_H