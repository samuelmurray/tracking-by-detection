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

    void update(BoundingBox detection);

    BoundingBox predict();

    BoundingBox get_state();

    static BoundingBox stateToBoundingBox(const Eigen::VectorXd &state);

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