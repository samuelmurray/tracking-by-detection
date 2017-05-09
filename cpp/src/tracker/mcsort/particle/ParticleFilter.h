#ifndef CPP_PARTICLEFILTER_H
#define CPP_PARTICLEFILTER_H


#include "Particle.h"
#include "../../../util/BoundingBox.h"

#include <dlib/matrix.h>

class ParticleFilter {
public:
    static constexpr int numStates = 7; // [x, y, area, ratio, vx, vy, area_change]
    static constexpr int numParticles = 2000;
public:
    ParticleFilter(const BoundingBox &initialState);

    virtual ~ParticleFilter() = default;

    ParticleFilter(ParticleFilter &&rhs);

    ParticleFilter &operator=(ParticleFilter &&rhs);

    // Prevent copying
    ParticleFilter(const ParticleFilter &rhs) = delete;

    ParticleFilter &operator=(const ParticleFilter &rhs) = delete;

    /**
     * Move all particles using current velocities.
     */
    void update();

    /**
     * Move all particles using current velocities. Update weights and resample based on observation
     */
    void update(const BoundingBox &bb);

    /**
     * Compute the predicted next state by moving all particles one time step and computing the mean.
     */
    BoundingBox getPrediction() const;

    /**
     * Compute the mean of all particles.
     */
    BoundingBox getCurrentEstimate() const;

private:
    std::vector<std::shared_ptr<Particle>> particles;
    dlib::matrix<double, numStates, numStates> velocityModel;

    void resample();
};


#endif //CPP_PARTICLEFILTER_H