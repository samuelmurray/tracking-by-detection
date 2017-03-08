#include "Tracker.h"

// Constructors

Tracker::Tracker(Detector *detector) :
        detector(detector) {}

Tracker::~Tracker() {
    delete detector;
}