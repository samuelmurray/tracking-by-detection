#ifndef CPP_DETECTIONPREDICTOR_H
#define CPP_DETECTIONPREDICTOR_H


#include "Detection.h"
#include "Tracking.h"

#include <dlib/matrix.h>
#include <dlib/filtering.h>

#include <vector>

class DetectionPredictor {
public:
    static constexpr int numStates = 7; // [x, y, area, ratio, vx, vy, area_change]
    static constexpr int numMeas = 4; // [x, y, area, ratio]
    static std::map<std::string, int> classCount;

public:
    DetectionPredictor(const Detection &initialState);

    virtual ~DetectionPredictor() = default;

    DetectionPredictor(DetectionPredictor &&rhs);

    DetectionPredictor &operator=(DetectionPredictor &&rhs);

    // Prevent copying
    DetectionPredictor(const DetectionPredictor &) = delete;

    DetectionPredictor &operator=(DetectionPredictor &) = delete;

    /**
     * Advances the state vector.
     */
    void advance();

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


#endif //CPP_DETECTIONPREDICTOR_H