#include "KalmanFilterer.h"

using std::vector;

const int KalmanFilterer::numStates = 7;
const int KalmanFilterer::numMeas = 4;
const double KalmanFilterer::dt = 1;
int KalmanFilterer::count = 1;

// Constructors

KalmanFilterer::KalmanFilterer(BoundingBox initialState)
        : filter(nullptr), history(vector<BoundingBox>()), ID(count++) {
    Eigen::MatrixXd F(numStates, numStates); // System dynamics matrix
    Eigen::MatrixXd H(numMeas, numStates); // Output matrix
    Eigen::MatrixXd Q(numStates, numStates); // Process noise covariance
    Eigen::MatrixXd R(numMeas, numMeas); // Measurement noise covariance
    Eigen::MatrixXd P(numStates, numStates); // Estimate error covariance

    // define constant velocity model
    F << 1, 0, 0, 0, dt, 0, 0,
            0, 1, 0, 0, 0, dt, 0,
            0, 0, 1, 0, 0, 0, dt,
            0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 1;

    H << 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0;

    // Covariance values from SORT
    Q << 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, .0001, 0, 0,
            0, 0, 0, 0, 0, .0001, 0,
            0, 0, 0, 0, 0, 0, .0001;

    R << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 10, 0,
            0, 0, 0, 10;

    P << 10, 0, 0, 0, 0, 0, 0,
            0, 10, 0, 0, 0, 0, 0,
            0, 0, 10, 0, 0, 0, 0,
            0, 0, 0, 10, 0, 0, 0,
            0, 0, 0, 0, 10000, 0, 0,
            0, 0, 0, 0, 0, 10000, 0,
            0, 0, 0, 0, 0, 0, 10000;

    filter = new KalmanFilter(dt, F, H, Q, R, P);
    Eigen::VectorXd x0(numStates);
    // TODO: input the detection to init
    filter->init(0, x0);
}

KalmanFilterer::~KalmanFilterer() {
    delete filter;
}

// Methods

void KalmanFilterer::update(BoundingBox detection) {
    /*
     * Updates the state vector with observed bbox.
     */
    timeSinceUpdate = 0;
    history = vector<BoundingBox>();
    hits++;
    hitStreak++;
    Eigen::VectorXd det(numMeas);
    // TODO: input the detection to update
    filter->update(det);
}

BoundingBox KalmanFilterer::predict() {
    /*
     * Advances the state vector and returns the predicted bounding box estimate.
     */

    // TODO: Convert Python code below to C++

    if (filter->state()(6) + filter->state()(2)) {
        filter->state()(6) = 0;
    }
    filter->predict();
    age++;
    if (timeSinceUpdate > 0) {
        hitStreak = 0;
    }
    timeSinceUpdate++;
    BoundingBox prediction = stateToBoundingBox(filter->state());
    history.push_back(prediction);
    return prediction;
}

BoundingBox KalmanFilterer::get_state() {
    /*
     * Returns the current bounding box estimate.
     */
    // TODO: Return this as Detection
    filter->state();
    return BoundingBox("", 0, 0, 0, 0);
}

BoundingBox KalmanFilterer::stateToBoundingBox(const Eigen::VectorXd &state) {
    return BoundingBox("", 0, 0, 0, 0);
}
