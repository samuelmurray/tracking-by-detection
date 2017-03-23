#include "DetectionPredictor.h"

using std::vector;

const double DetectionPredictor::dt = 1;
int DetectionPredictor::count = 1;

// Constructors

DetectionPredictor::DetectionPredictor(const Detection &initialState)
        : filter(nullptr), history(vector<Detection>()), ID(count++), className(initialState.className) {
    dlib::matrix<double, numStates, numStates> F; // System dynamics matrix
    dlib::matrix<double, numMeas, numStates> H; // Output matrix
    dlib::matrix<double, numStates, numStates> Q; // Process noise covariance
    dlib::matrix<double, numMeas, numMeas> R; // Measurement noise covariance
    dlib::matrix<double, numStates, numStates> P; // Estimate error covariance

    // define constant velocity model
    F = 1, 0, 0, 0, dt, 0, 0,
            0, 1, 0, 0, 0, dt, 0,
            0, 0, 1, 0, 0, 0, dt,
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
            0, 0, 0, 0, .0001, 0, 0,
            0, 0, 0, 0, 0, .0001, 0,
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
    filter->set_measurement_noise(R);
    filter->set_observation_model(H);
    filter->set_estimation_error_covariance(P);
    filter->set_process_noise(Q);
    filter->set_transition_model(F);
    dlib::matrix<double, numMeas, 1> x0(boundingBoxToMeas(initialState.bb));
    filter->update(x0);
}

DetectionPredictor::~DetectionPredictor() {}

// Methods

void DetectionPredictor::update(const Detection &det) {
    timeSinceUpdate = 0;
    history = vector<Detection>();
    hits++;
    hitStreak++;
    filter->update(boundingBoxToMeas(det.bb));
}

Detection DetectionPredictor::predict() {
    if (filter->get_current_state()(6) + filter->get_current_state()(2)) {
        //filter->get_current_state()(6) = 0; // TODO
    }
    filter->get_predicted_next_state();
    age++;
    if (timeSinceUpdate > 0) {
        hitStreak = 0;
    }
    timeSinceUpdate++;
    Detection prediction = getCurrentPrediction();
    history.push_back(prediction);
    return prediction;
}

// Getters

Detection DetectionPredictor::getCurrentPrediction() const {
    return Detection(className, stateToBoundingBox(filter->get_current_state()));
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

// Functions

dlib::matrix<double, DetectionPredictor::numMeas, 1> DetectionPredictor::boundingBoxToMeas(const BoundingBox &bb) {
    dlib::matrix<double, DetectionPredictor::numMeas, 1> z;
    z = bb.cx, bb.cy, bb.width * bb.height, bb.width / bb.height;
    return z;
}

BoundingBox DetectionPredictor::stateToBoundingBox(const dlib::matrix<double, numStates, 1> &state) {
    double width = std::sqrt(state(2) * state(3));
    double height = state(2) / width;
    return BoundingBox(int(state(0)), int(state(1)), int(width), int(height));
}
