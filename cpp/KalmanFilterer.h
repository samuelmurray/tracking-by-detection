#ifndef CPP_KALMANFILTERER_H
#define CPP_KALMANFILTERER_H


#include <vector>
#include "KalmanFilter.h"
#include "BoundingBox.h"

class KalmanFilterer {
public:
    static const int numStates; // [x, y, area, ratio, vx, vy, area_change]
    static const int numMeas; // [x, y, area, ratio]
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

    static BoundingBox stateToBoundingBox(const Eigen::VectorXd &state);

    static Eigen::VectorXd boundingBoxToMeas(const BoundingBox &box);

private:
    KalmanFilter *filter;
    std::vector<BoundingBox> history;
    int ID;
    int timeSinceUpdate = 0;
    int hits = 0;
    int hitStreak = 0;
    int age = 0;
};


#endif //CPP_KALMANFILTERER_H