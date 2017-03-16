#include "KalmanFilterer.h"

using std::vector;

// Constructors

KalmanFilterer::KalmanFilterer(Detection detection)
        : filter(nullptr), history(vector<Detection>()), ID(count++) {
    Eigen::MatrixXd A(numStates, numStates); // System dynamics matrix
    Eigen::MatrixXd C(numMeas, numStates); // Output matrix
    Eigen::MatrixXd Q(numStates, numStates); // Process noise covariance
    Eigen::MatrixXd R(numMeas, numMeas); // Measurement noise covariance
    Eigen::MatrixXd P(numStates, numStates); // Estimate error covariance

    // define constant velocity model
    A << 1, 0, 0, 0, dt, 0, 0,
            0, 1, 0, 0, 0, dt, 0,
            0, 0, 1, 0, 0, 0, dt,
            0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 1;

    C << 1, 0, 0, 0, 0, 0, 0,
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

    filter = new KalmanFilter(dt, A, C, Q, R, P);
    Eigen::VectorXd x0(numStates);
    // TODO: input the detection to init
    filter->init(0, x0);
}

KalmanFilterer::~KalmanFilterer() {
    delete filter;
}

// Methods

void KalmanFilterer::update(Detection detection) {
    /*
     * Updates the state vector with observed bbox.
     */
    timeSinceUpdate = 0;
    history = vector<Detection>();
    hits++;
    hitStreak++;
    Eigen::VectorXd det(numMeas);
    // TODO: input the detection to update
    filter->update(det);
}

Detection KalmanFilterer::predict() {
    /*
     * Advances the state vector and returns the predicted bounding box estimate.
     */

    // TODO: Convert Python code below to C++

    /*
    if((self.kf.x[6]+self.kf.x[2])<=0):
    self.kf.x[6] *= 0.0
    self.kf.predict()
    self.age += 1
    if(self.time_since_update>0):
      self.hit_streak = 0
    self.time_since_update += 1
    self.history.append(convert_x_to_bbox(self.kf.x))
    return self.history[-1]
     */
    return Detection();
}

Detection KalmanFilterer::get_state() {
    /*
     * Returns the current bounding box estimate.
     */
    // TODO: Return this as Detection
    filter->state();
    return Detection();
}