#include "Tracker.h"

Tracker::Tracker(const std::shared_ptr<Detector> &detector)
        : detector(std::move(detector)) {}

Tracker::~Tracker() {}