#include "DetectionPredictor.h"

using std::vector;

std::map<std::string, int> DetectionPredictor::classCount;

// Constructors

DetectionPredictor::DetectionPredictor(const Detection &initialState)
        : filter(nullptr),
          className(initialState.className),
          ID(++classCount[initialState.className]),
          timeSinceUpdate(0),
          hitStreak(0) {
    dlib::matrix<double, numStates, numStates> F; // System dynamics matrix
    dlib::matrix<double, numMeas, numStates> H; // Output matrix
    dlib::matrix<double, numStates, numStates> Q; // Process noise covariance
    dlib::matrix<double, numMeas, numMeas> R; // Measurement noise covariance
    dlib::matrix<double, numStates, numStates> P; // Estimate error covariance

    // define constant velocity model
    F = 1, 0, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 0, 1, 0,
            0, 0, 1, 0, 0, 0, 1,
            0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 1;

    H = 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0;

    // Covariance values from SORT
    Q = 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, .01, 0, 0,
            0, 0, 0, 0, 0, .01, 0,
            0, 0, 0, 0, 0, 0, .0001;

    R = 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 10, 0,
            0, 0, 0, 10;

    P = 10, 0, 0, 0, 0, 0, 0,
            0, 10, 0, 0, 0, 0, 0,
            0, 0, 10, 0, 0, 0, 0,
            0, 0, 0, 10, 0, 0, 0,
            0, 0, 0, 0, 10000, 0, 0,
            0, 0, 0, 0, 0, 10000, 0,
            0, 0, 0, 0, 0, 0, 10000;

    filter = std::make_shared<dlib::kalman_filter<numStates, numMeas>>(dlib::kalman_filter<numStates, numMeas>());
    filter->set_transition_model(F);
    filter->set_observation_model(H);
    filter->set_process_noise(Q);
    filter->set_measurement_noise(R);
    filter->set_estimation_error_covariance(P);
    dlib::matrix<double, numMeas, 1> x0(boundingBoxToMeas(initialState.bb));
    filter->update(x0);
}

DetectionPredictor::DetectionPredictor(DetectionPredictor &&rhs)
        : filter(std::move(rhs.filter)),
          className(std::move(rhs.className)),
          ID(rhs.ID),
          timeSinceUpdate(rhs.timeSinceUpdate),
          hitStreak(rhs.hitStreak) {}

DetectionPredictor &DetectionPredictor::operator=(DetectionPredictor &&rhs) {
    filter = std::move(rhs.filter);
    className = std::move(rhs.className);
    ID = rhs.ID;
    timeSinceUpdate = rhs.timeSinceUpdate;
    hitStreak = rhs.hitStreak;
    return *this;
}

// Methods

void DetectionPredictor::update() {
    timeSinceUpdate++;
    hitStreak = 0;
    filter->update();
}

void DetectionPredictor::update(const Detection &det) {
    timeSinceUpdate = 0;
    hitStreak++;
    filter->update(boundingBoxToMeas(det.bb));
}

// Getters

Detection DetectionPredictor::getPredictedNextDetection() const {
    return Detection(className, stateToBoundingBox(filter->get_predicted_next_state()));
}

Tracking DetectionPredictor::getTracking() const {
    return Tracking(className, ID, stateToBoundingBox(filter->get_current_state()));
}

int DetectionPredictor::getTimeSinceUpdate() const {
    return timeSinceUpdate;
}

int DetectionPredictor::getHitStreak() const {
    return hitStreak;
}

const int DetectionPredictor::getID() const {
    return ID;
}

// Functions

dlib::matrix<double, DetectionPredictor::numMeas, 1> DetectionPredictor::boundingBoxToMeas(const BoundingBox &bb) {
    dlib::matrix<double, DetectionPredictor::numMeas, 1> z;
    z = bb.cx, bb.cy, bb.area(), bb.ratio();
    return z;
}

BoundingBox DetectionPredictor::stateToBoundingBox(const dlib::matrix<double, numStates, 1> &state) {
    double rectifiedArea = std::max(state(2), 0.);
    double width = std::sqrt(rectifiedArea * state(3));
    double height = rectifiedArea / width;
    return BoundingBox(int(state(0)), int(state(1)), int(width), int(height));
}