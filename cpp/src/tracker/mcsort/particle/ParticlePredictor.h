#ifndef CPP_PARTICLEPREDICTOR_H
#define CPP_PARTICLEPREDICTOR_H


#include "../Predictor.h"
#include "ParticleFilter.h"

#include <memory>

class ParticlePredictor : public Predictor {
public:
    ParticlePredictor(const Detection &initialState, int ID);

    ParticlePredictor(ParticlePredictor &&rhs);

    ParticlePredictor &operator=(ParticlePredictor &&rhs);

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
    std::shared_ptr<ParticleFilter> filter;
};


#endif //CPP_PARTICLEPREDICTOR_H