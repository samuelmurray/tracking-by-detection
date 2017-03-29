#include "Tracking.h"

// Constructors

Tracking::Tracking(const std::string &className, int ID, const BoundingBox &bb)
        : className(className), ID(ID), bb(bb) {}

// Functions

std::ostream &operator<<(std::ostream &os, const Tracking &t) {
    os << t.className << " ID: " << t.ID << " " << t.bb;
    return os;
}