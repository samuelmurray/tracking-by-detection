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
    x0 << boundingBoxToMeas(initialState), 0, 0, 0;
    filter->init(0, x0);
}

KalmanFilterer::~KalmanFilterer() {
    delete filter;
}

// Methods

void KalmanFilterer::update(BoundingBox box) {
    timeSinceUpdate = 0;
    history = vector<BoundingBox>();
    hits++;
    hitStreak++;
    filter->update(boundingBoxToMeas(box));
}

BoundingBox KalmanFilterer::predict() {
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

BoundingBox KalmanFilterer::getState() const {
    return stateToBoundingBox(filter->state());
}

BoundingBox KalmanFilterer::stateToBoundingBox(const Eigen::VectorXd &state) {
    double width = std::sqrt(state(2) * state(3));
    double height = state(2) / width;
    return BoundingBox("FIXME", int(state(0)), int(state(1)), int(width), int(height));
}

Eigen::VectorXd KalmanFilterer::boundingBoxToMeas(const BoundingBox &box) {
    Eigen::VectorXd z(numMeas);
    z << box.cx, box.cy, box.width * box.height, box.width / box.height;
    return z;
}