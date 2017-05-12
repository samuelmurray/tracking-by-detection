#ifndef CPP_STATIONARYPREDICTOR_H
#define CPP_STATIONARYPREDICTOR_H


#include "Predictor.h"

#include <memory>

class StationaryPredictor : public Predictor {
public:
    StationaryPredictor(const Detection &initialState, int ID);

    StationaryPredictor(StationaryPredictor &&rhs);

    StationaryPredictor &operator=(StationaryPredictor &&rhs);

    /**
     * Does nothing except calling Predictor::update().
     */
    void update() override;

    /**
     * Sets state to match the Detection.
     */
    void update(const Detection &det) override;

    /**
     * Returns state as Detection.
     */
    Detection getPredictedNextDetection() const override;

    /**
     * Returns state as Tracking.
     */
    Tracking getTracking() const override;

private:
    std::shared_ptr<BoundingBox> state;
};


#endif //CPP_STATIONARYPREDICTOR_H