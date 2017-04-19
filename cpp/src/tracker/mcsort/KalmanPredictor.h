#ifndef CPP_KALMANPREDICTOR_H
#define CPP_KALMANPREDICTOR_H


#include "../../util/Detection.h"
#include "../../util/Tracking.h"

#include <dlib/matrix.h>
#include <dlib/filtering.h>

#include <vector>
#include <memory>

class KalmanPredictor {
public:
    static constexpr int numStates = 7; // [x, y, area, ratio, vx, vy, area_change]
    static constexpr int numObservations = 4; // [x, y, area, ratio]

public:
    KalmanPredictor(const Detection &initialState, int ID);

    virtual ~KalmanPredictor() = default;

    KalmanPredictor(KalmanPredictor &&rhs);

    KalmanPredictor &operator=(KalmanPredictor &&rhs);

    // Prevent copying
    KalmanPredictor(const KalmanPredictor &) = delete;

    KalmanPredictor &operator=(KalmanPredictor &) = delete;

    /**
     * Advances the state vector using current estimation.
     */
    void update();

    /**
     * Updates and advances the state vector using given Detection as observation.
     */
    void update(const Detection &det);

    /**
     * Returns the predicted next state as Detection.
     */
    Detection getPredictedNextDetection() const;

    /**
     * Returns the current state as Tracking.
     */
    Tracking getTracking() const;

    int getHitStreak() const;

    int getTimeSinceUpdate() const;

    int getID() const;

    /**
     * Converts a state vector to a BoundingBox.
     */
    static BoundingBox stateToBoundingBox(const dlib::matrix<double, numStates, 1> &state);

    /**
     * Converts a BoundingBox to an observation vector.
     */
    static dlib::matrix<double, numObservations, 1> boundingBoxToMeas(const BoundingBox &bb);

private:
    std::shared_ptr<dlib::kalman_filter<numStates, numObservations>> filter;
    int label;
    int ID;
    int timeSinceUpdate;
    int hitStreak;
};

std::ostream &operator<<(std::ostream &os, const KalmanPredictor &kp);


#endif //CPP_KALMANPREDICTOR_H