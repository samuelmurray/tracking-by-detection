#ifndef CPP_KALMANPREDICTOR_H
#define CPP_KALMANPREDICTOR_H


#include "../Predictor.h"

#include <dlib/filtering.h>

#include <vector>
#include <memory>

class KalmanPredictor : public Predictor {
public:
    KalmanPredictor(const Detection &initialState, int ID);

    KalmanPredictor(KalmanPredictor &&rhs);

    KalmanPredictor &operator=(KalmanPredictor &&rhs);

    /**
     * Advances the state vector using current estimation.
     */
    void update() override;

    /**
     * Updates and advances the state vector using given Detection as observation.
     */
    void update(const Detection &det) override;

    /**
     * Returns the predicted next state as Detection.
     */
    Detection getPredictedNextDetection() const override;

    /**
     * Returns the current state as Tracking.
     */
    Tracking getTracking() const override;

private:
    std::shared_ptr<dlib::kalman_filter<numStates, numObservations>> filter;
};


#endif //CPP_KALMANPREDICTOR_H