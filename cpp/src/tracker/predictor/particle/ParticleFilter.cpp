#include "ParticleFilter.h"
#include "../Predictor.h"
#include "../../../util/Affinity.h"

#include <random>
#include <algorithm>
#include <chrono>

// Constructors
ParticleFilter::ParticleFilter(const BoundingBox &initialState)
        : particles(std::vector<std::shared_ptr<Particle>>(numParticles)) {
    std::default_random_engine generator;
    std::normal_distribution<double> norm(0, 1);
    for (auto &particle : particles) {
        // TODO: Different noise for different fields
        dlib::matrix<double, numStates, 1> state;
        state = initialState.cx, initialState.cy, initialState.area(), initialState.ratio(), 0, 0, 0;
        dlib::matrix<double, numStates, 1> noise;
        noise = 0, 0, 0, 0, norm(generator), norm(generator), norm(generator);
        state += noise;
        particle = std::make_shared<Particle>(state, 1. / numParticles);
    }
    velocityModel = 1, 0, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 0, 1, 0,
            0, 0, 1, 0, 0, 0, 1,
            0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 0, 1;
}

ParticleFilter::ParticleFilter(ParticleFilter &&rhs)
        : particles(std::move(rhs.particles)),
          velocityModel(std::move(rhs.velocityModel)) {}

ParticleFilter &ParticleFilter::operator=(ParticleFilter &&rhs) {
    particles = std::move(rhs.particles);
    velocityModel = std::move(rhs.velocityModel);
    return *this;
}

// Methods

void ParticleFilter::update() {
    std::default_random_engine generator;
    std::normal_distribution<double> norm(0, 1);
    for (auto &particle : particles) {
        dlib::matrix<double, numStates, 1> noise;
        noise = 0, 0, 0, 0, norm(generator), norm(generator), norm(generator);
        particle->state = velocityModel * (particle->state + noise);
    }
}

void ParticleFilter::update(const BoundingBox &bb) {
    std::default_random_engine generator;
    std::normal_distribution<double> norm(0, 1);
    for (auto &particle : particles) {
        dlib::matrix<double, numStates, 1> noise;
        noise = 0, 0, 0, 0, norm(generator), norm(generator), norm(generator);
        particle->state = velocityModel * (particle->state + noise);
    }
    double weightSum = 0;
    for (int i = 0; i < numParticles; ++i) {
        particles.at(i)->weight = Affinity::iou(bb, Predictor::stateToBoundingBox(particles.at(i)->state));
        weightSum += particles.at(i)->weight;
    }
    if (weightSum == 0) {
        // If not particle is good, restart with a new filter
        ParticleFilter newFilter(bb);
        *this = std::move(newFilter);
        return;
    }
    for (int i = 0; i < numParticles; ++i) {
        particles.at(i)->weight /= weightSum;
    }
    resample();
}

BoundingBox ParticleFilter::getPrediction() const {
    dlib::matrix<double, numStates, 1> meanState = dlib::zeros_matrix<double>(numStates, 1);
    for (const auto &particle : particles) {
        meanState += velocityModel * particle->state;
    }
    meanState /= numParticles;
    return Predictor::stateToBoundingBox(meanState);
}

BoundingBox ParticleFilter::getCurrentEstimate() const {
    dlib::matrix<double, numStates, 1> meanState = dlib::zeros_matrix<double>(numStates, 1);
    for (const auto &particle : particles) {
        meanState += particle->state;
    }
    meanState /= numParticles;
    return Predictor::stateToBoundingBox(meanState);
}

void ParticleFilter::resample() {
    std::vector<std::shared_ptr<Particle>> newParticles(numParticles);
    std::vector<double> weightDistribution;
    double weightSum = 0;
    for (int i = 0; i < numParticles; ++i) {
        weightSum += particles.at(i)->weight;
        weightDistribution.push_back(weightSum);
    }
    double interval = 1. / weightSum;
    int weightIdx = 0;
    for (int i = 0; i < numParticles; ++i) {
        double val = i * interval;
        while (val <= weightSum && val > weightDistribution.at(weightIdx)) {
            ++weightIdx;
        }
        newParticles.at(i) = particles.at(weightIdx);
    }
    particles = newParticles;
    /*
    for (int i = 0; i < numParticles; ++i) {
        double r = ((double) rand() / RAND_MAX);
        std::lower_bound(weightDistribution.begin(), weightDistribution.end(), r);

    }
     */
}