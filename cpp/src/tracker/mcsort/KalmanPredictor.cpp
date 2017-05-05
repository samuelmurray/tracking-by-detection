#include "KalmanPredictor.h"

// Constructors

KalmanPredictor::KalmanPredictor(const Detection &initialState, int ID)
        : Predictor(initialState.label, ID), filter(nullptr) {
    dlib::matrix<double, numStates, numStates> F; // System dynamics matrix
    dlib::matrix<double, numObservations, numStates> H; // Output matrix
    dlib::matrix<double, numStates, numStates> Q; // Process noise covariance
    dlib::matrix<double, numObservations, numObservations> R; // Measurement noise covariance
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

    filter = std::make_shared<dlib::kalman_filter<numStates, numObservations>>(
            dlib::kalman_filter<numStates, numObservations>());
    filter->set_transition_model(F);
    filter->set_observation_model(H);
    filter->set_process_noise(Q);
    filter->set_measurement_noise(R);
    filter->set_estimation_error_covariance(P);
    dlib::matrix<double, numObservations, 1> x0(boundingBoxToMeas(initialState.bb));
    filter->update(x0);
}

KalmanPredictor::KalmanPredictor(KalmanPredictor &&rhs)
        : Predictor(std::move(rhs)), filter(std::move(rhs.filter)) {}


KalmanPredictor &KalmanPredictor::operator=(KalmanPredictor &&rhs) {
    Predictor::operator=(std::move(rhs));
    filter = std::move(rhs.filter);
    return *this;
}

// Methods

void KalmanPredictor::update() {
    Predictor::update();
    filter->update();
}

void KalmanPredictor::update(const Detection &det) {
    Predictor::update(det);
    filter->update(boundingBoxToMeas(det.bb));
}

// Getters

Detection KalmanPredictor::getPredictedNextDetection() const {
    return Detection(getLabel(), 1, stateToBoundingBox(filter->get_predicted_next_state()));
}

Tracking KalmanPredictor::getTracking() const {
    return Tracking(getLabel(), getID(), stateToBoundingBox(filter->get_current_state()));
}