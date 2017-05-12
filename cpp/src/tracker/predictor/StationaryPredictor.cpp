#include "StationaryPredictor.h"

// Constructors

StationaryPredictor::StationaryPredictor(const Detection &initialState, int ID)
        : Predictor(initialState.label, ID), state(std::make_shared<BoundingBox>(initialState.bb)) {}

StationaryPredictor::StationaryPredictor(StationaryPredictor &&rhs)
        : Predictor(std::move(rhs)), state(rhs.state) {}

StationaryPredictor &StationaryPredictor::operator=(StationaryPredictor &&rhs) {
    Predictor::operator=(std::move(rhs));
    state = std::move(rhs.state);
    return *this;
}

// Methods

void StationaryPredictor::update() {
    Predictor::update();
}

void StationaryPredictor::update(const Detection &det) {
    Predictor::update(det);
    state = std::make_shared<BoundingBox>(det.bb);
}

// Getters

Detection StationaryPredictor::getPredictedNextDetection() const {
    return Detection(getLabel(), 1, *state);
}

Tracking StationaryPredictor::getTracking() const {
    return Tracking(getLabel(), getID(), *state);
}