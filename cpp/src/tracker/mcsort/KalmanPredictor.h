#ifndef CPP_DETECTIONPREDICTOR_H
#define CPP_DETECTIONPREDICTOR_H


#include "../../util/Detection.h"
#include "../../util/Tracking.h"

#include <dlib/matrix.h>
#include <dlib/filtering.h>

#include <vector>

class KalmanPredictor {
public:
    static constexpr int numStates = 7; // [x, y, area, ratio, vx, vy, area_change]
    static constexpr int numMeas = 4; // [x, y, area, ratio]
    static std::map<std::string, int> classCount;

public:
    KalmanPredictor(const Detection &initialState);

    virtual ~KalmanPredictor() = default;

    KalmanPredictor(KalmanPredictor &&rhs);

    KalmanPredictor &operator=(KalmanPredictor &&rhs);

    // Prevent copying
    KalmanPredictor(const KalmanPredictor &) = delete;

    KalmanPredictor &operator=(KalmanPredictor &) = delete;

    /**
     * Advances the state vector.
     */
    void update();

    /**
     * Updates the state vector with observed bbox.
     */
    void update(const Detection &det);

    /**
     * Returns the current bounding box estimate.
     */
    Detection getPredictedNextDetection() const;

    Tracking getTracking() const;

    int getHitStreak() const;

    int getTimeSinceUpdate() const;

    const int getID() const;

    static BoundingBox stateToBoundingBox(const dlib::matrix<double, numStates, 1> &state);

    static dlib::matrix<double, numMeas, 1> boundingBoxToMeas(const BoundingBox &bb);

private:
    std::shared_ptr<dlib::kalman_filter<numStates, numMeas>> filter;
    std::string className;
    int ID;
    int timeSinceUpdate;
    int hitStreak;
};

std::ostream &operator<<(std::ostream &os, const KalmanPredictor &kp);


#endif //CPP_DETECTIONPREDICTOR_H