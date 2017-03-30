#include "Tracking.h"

// Constructors

Tracking::Tracking(const std::string &className, int ID, const BoundingBox &bb)
        : className(className), ID(ID), bb(bb) {}

Tracking::Tracking(const Tracking &rhs)
        : className(rhs.className), ID(rhs.ID), bb(rhs.bb) {}

Tracking::Tracking(Tracking &&rhs)
        : className(std::move(rhs.className)), ID(rhs.ID), bb(std::move(rhs.bb)) {}

// Functions

std::ostream &operator<<(std::ostream &os, const Tracking &t) {
    os << t.className << " ID: " << t.ID << " " << t.bb;
    return os;
}