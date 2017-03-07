#include "TrackerState.h"

void TrackerState::update(std::vector<Detection> newDetections) {
    previousDetections = newDetections;
}