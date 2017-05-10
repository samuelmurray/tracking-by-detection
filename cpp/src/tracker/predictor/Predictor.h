#ifndef CPP_PREDICTOR_H
#define CPP_PREDICTOR_H


#include "../../util/Detection.h"
#include "../../util/Tracking.h"

#include <dlib/matrix.h>

class Predictor {
public:
    static constexpr int numStates = 7; // [x, y, area, ratio, vx, vy, area_change]
    static constexpr int numObservations = 4; // [x, y, area, ratio]

public:
    Predictor(int label, int ID);

    virtual ~Predictor() = default;

    Predictor(Predictor &&rhs);

    Predictor &operator=(Predictor &&rhs);

    // Prevent copying
    Predictor(const Predictor &) = delete;

    Predictor &operator=(Predictor &) = delete;

    /**
     * Advances the state vector using current estimation.
     */
    virtual void update() = 0;

    /**
     * Updates and advances the state vector using given Detection as observation.
     */
    virtual void update(const Detection &det) = 0;

    /**
     * Returns the predicted next state as Detection.
     */
    virtual Detection getPredictedNextDetection() const = 0;

    /**
     * Returns the current state as Tracking.
     */
    virtual Tracking getTracking() const = 0;

    int getHitStreak() const;

    int getTimeSinceUpdate() const;

    int getLabel() const;

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
    int label;
    int ID;
    int timeSinceUpdate;
    int hitStreak;
};

std::ostream &operator<<(std::ostream &os, const Predictor &kp);


#endif //CPP_PREDICTOR_H