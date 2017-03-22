#ifndef CPP_KALMANFILTERER_H
#define CPP_KALMANFILTERER_H


#include <vector>
#include <dlib/matrix.h>
#include <dlib/filtering.h>
#include "BoundingBox.h"
#include "Detection.h"

class KalmanFilterer {
public:
    static constexpr int numStates = 7; // [x, y, area, ratio, vx, vy, area_change]
    static constexpr int numMeas = 4; // [x, y, area, ratio]
    static const double dt;
    static int count; // TODO: This should be a dict from class to count

public:
    KalmanFilterer(BoundingBox initialState);

    virtual ~KalmanFilterer();

    /**
     * Updates the state vector with observed bbox.
     */
    void update(BoundingBox box);

    /**
     * Advances the state vector and returns the predicted bounding box estimate.
     */
    BoundingBox predict();

    /**
     * Returns the current bounding box estimate.
     */
    BoundingBox getState() const;

    Detection detection() const;

    static BoundingBox stateToBoundingBox(const dlib::matrix<double, numStates, 1> &state);

    static dlib::matrix<double, numMeas, 1> boundingBoxToMeas(const BoundingBox &box);

private:
    std::shared_ptr<dlib::kalman_filter<numStates, numMeas>> filter;
    std::vector<BoundingBox> history;
    int ID;
    int timeSinceUpdate = 0;
public:
    int getHitStreak() const;

public:
    int getTimeSinceUpdate() const;

private:
    int hits = 0;
    int hitStreak = 0;
    int age = 0;
};


#endif //CPP_KALMANFILTERER_H