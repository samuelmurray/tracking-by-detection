#include "Tracking.h"

Tracking::Tracking(const Detection &detection, int ID)
        : detection(detection), ID(ID) {}

Tracking::Tracking(const std::string &className, int cx, int cy, int width, int height, int ID)
        : detection(Detection(className, cx, cy, width, height)), ID(ID) {}

std::ostream &operator<<(std::ostream &os, const Tracking &t) {
    os << t.detection << " ID: " << t.ID;
    return os;
}