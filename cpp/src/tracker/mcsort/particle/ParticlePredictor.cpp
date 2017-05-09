#include "ParticlePredictor.h"

// Constructors

ParticlePredictor::ParticlePredictor(const Detection &initialState, int ID)
        : Predictor(initialState.label, ID), filter(nullptr) {
    filter = std::make_shared<ParticleFilter>(initialState.bb);
}

ParticlePredictor::ParticlePredictor(ParticlePredictor &&rhs)
        : Predictor(std::move(rhs)), filter(std::move(rhs.filter)) {}

ParticlePredictor &ParticlePredictor::operator=(ParticlePredictor &&rhs) {
    Predictor::operator=(std::move(rhs));
    filter = std::move(rhs.filter);
    return *this;
}

// Methods

void ParticlePredictor::update() {
    Predictor::update();
    filter->update();
}

void ParticlePredictor::update(const Detection &det) {
    Predictor::update(det);
    filter->update(det.bb);
}

// Getters

Detection ParticlePredictor::getPredictedNextDetection() const {
    return Detection(getLabel(), 1, filter->getPrediction());
}

Tracking ParticlePredictor::getTracking() const {
    return Tracking(getLabel(), getID(), filter->getCurrentEstimate());
}
