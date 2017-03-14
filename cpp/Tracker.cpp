#include "Tracker.h"

Tracker::Tracker(Detector *detector) :
        detector(detector) {}

Tracker::~Tracker() {
    delete detector;
}