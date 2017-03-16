#ifndef CPP_KALMANFILTERER_H
#define CPP_KALMANFILTERER_H


#include <vector>
#include "KalmanFilter.h"
#include "Detection.h"

class KalmanFilterer {
public:
    static const int numStates = 7; // [x, y, area, ratio, vx, vy, area_change]
    static const int numMeas = 4; // [x, y, area, ratio]
    static const double dt = 1;
    static int count = 1; // TODO: This should be a dict from class to count

public:
    KalmanFilterer(Detection detection);

    virtual ~KalmanFilterer();

    void update(Detection detection);

    Detection predict();

    Detection get_state();

private:
    KalmanFilter *filter;
    std::vector<Detection> history;
    int ID;
    int timeSinceUpdate = 0;
    int hits = 0;
    int hitStreak = 0;
    int age = 0;
};


#endif //CPP_KALMANFILTERER_H