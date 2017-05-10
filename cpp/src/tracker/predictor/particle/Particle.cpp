#include "Particle.h"

Particle::Particle(const dlib::matrix<double, numStates, 1> &state, double weight)
        : state(state), weight(weight) {}

Particle::Particle(const Particle &rhs) {
    state = rhs.state;
    weight = rhs.weight;
}

Particle &Particle::operator=(const Particle &rhs) {
    state = rhs.state;
    weight = rhs.weight;
    return *this;
}
