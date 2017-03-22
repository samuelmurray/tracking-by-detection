#include "Tracker.h"

Tracker::Tracker(std::shared_ptr<Detector> detector)
        : detector(detector) {}

Tracker::~Tracker() {}