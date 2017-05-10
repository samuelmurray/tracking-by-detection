#ifndef CPP_PARTICLE_H
#define CPP_PARTICLE_H


#include <dlib/matrix.h>

class Particle {
    static constexpr int numStates = 7;
public:
    Particle(const dlib::matrix<double, numStates, 1> &state, double weight);

    virtual ~Particle() = default;

    Particle(const Particle &rhs);

    Particle &operator=(const Particle &rhs);

    // No move
    Particle &operator=(Particle &&rhs) = delete;

    Particle(Particle &&rhs) = delete;

    dlib::matrix<double, numStates, 1> state;
    double weight;
};


#endif //CPP_PARTICLE_H