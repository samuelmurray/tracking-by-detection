#include "Predictor.h"

// Constructors

Predictor::Predictor(int label, int ID)
        : label(label), ID(ID), timeSinceUpdate(0), hitStreak(0) {}

Predictor::Predictor(Predictor &&rhs)
        : label(rhs.label), ID(rhs.ID), timeSinceUpdate(rhs.timeSinceUpdate), hitStreak(rhs.hitStreak) {}

Predictor &Predictor::operator=(Predictor &&rhs) {
    label = rhs.label;
    ID = rhs.ID;
    timeSinceUpdate = rhs.timeSinceUpdate;
    hitStreak = rhs.hitStreak;
    return *this;
}

// Methods

void Predictor::update() {
    timeSinceUpdate++;
    hitStreak = 0;
}

void Predictor::update(const Detection &det) {
    timeSinceUpdate = 0;
    hitStreak++;
}

// Getters

int Predictor::getTimeSinceUpdate() const {
    return timeSinceUpdate;
}

int Predictor::getHitStreak() const {
    return hitStreak;
}

int Predictor::getLabel() const {
    return label;
}

int Predictor::getID() const {
    return ID;
}

// Functions

BoundingBox Predictor::stateToBoundingBox(const dlib::matrix<double, numStates, 1> &state) {
    double rectifiedArea = std::max(state(2), 0.);
    double width = std::sqrt(rectifiedArea * state(3));
    double height = rectifiedArea / width;
    return BoundingBox(state(0), state(1), width, height);
}

dlib::matrix<double, Predictor::numObservations, 1> Predictor::boundingBoxToMeas(const BoundingBox &bb) {
    dlib::matrix<double, Predictor::numObservations, 1> z;
    z = bb.cx, bb.cy, bb.area(), bb.ratio();
    return z;
}

std::ostream &operator<<(std::ostream &os, const Predictor &p) {
    os << p.getPredictedNextDetection();
    return os;
}