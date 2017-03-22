#include "KalmanFilterer.h"

using std::vector;

//const int KalmanFilterer::numStates = 7;
//const int KalmanFilterer::numMeas = 4;
const double KalmanFilterer::dt = 1;
int KalmanFilterer::count = 1;

// Constructors

KalmanFilterer::KalmanFilterer(BoundingBox initialState)
        : filter(nullptr), history(vector<BoundingBox>()), ID(count++) {
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
    std::cout << "\nCREATED\n";

    filter = std::make_shared<dlib::kalman_filter<numStates, numMeas>>(dlib::kalman_filter<numStates, numMeas>());
    filter->set_measurement_noise(R);
    filter->set_observation_model(H);
    filter->set_estimation_error_covariance(P);
    filter->set_process_noise(Q);
    filter->set_transition_model(F);
    std::cout << "\nSET\n";
    dlib::matrix<double, numMeas, 1> x0(boundingBoxToMeas(initialState));
    std::cout << "\nSTATE\n";
    filter->update(x0);
    std::cout << "\nUPDATED\n";
}

KalmanFilterer::~KalmanFilterer() {}

// Methods

void KalmanFilterer::update(BoundingBox box) {
    timeSinceUpdate = 0;
    history = vector<BoundingBox>();
    hits++;
    hitStreak++;
    filter->update(boundingBoxToMeas(box));
}

BoundingBox KalmanFilterer::predict() {
    if (filter->get_current_state()(6) + filter->get_current_state()(2)) {
        //filter->get_current_state()(6) = 0; // TODO
    }
    filter->get_predicted_next_state();
    age++;
    if (timeSinceUpdate > 0) {
        hitStreak = 0;
    }
    timeSinceUpdate++;
    BoundingBox prediction = stateToBoundingBox(filter->get_current_state());
    history.push_back(prediction);
    return prediction;
}

BoundingBox KalmanFilterer::getState() const {
    return stateToBoundingBox(filter->get_current_state());
}

Detection KalmanFilterer::detection() const {
    return Detection(getState(), 0);
}

BoundingBox KalmanFilterer::stateToBoundingBox(const dlib::matrix<double, KalmanFilterer::numStates, 1> &state) {
    double width = std::sqrt(state(2) * state(3));
    double height = state(2) / width;
    return BoundingBox("FIXME", int(state(0)), int(state(1)), int(width), int(height));
}

dlib::matrix<double, KalmanFilterer::numMeas, 1> KalmanFilterer::boundingBoxToMeas(const BoundingBox &box) {
    dlib::matrix<double, KalmanFilterer::numMeas, 1> z;
    z = box.cx, box.cy, box.width * box.height, box.width / box.height;
    return z;
}

// Getters

int KalmanFilterer::getTimeSinceUpdate() const {
    return timeSinceUpdate;
}

int KalmanFilterer::getHitStreak() const {
    return hitStreak;
}