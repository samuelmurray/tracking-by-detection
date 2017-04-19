#include "Tracking.h"

// Constructors

Tracking::Tracking(int label, int ID, const BoundingBox &bb)
        : label(label), ID(ID), bb(bb) {}

Tracking::Tracking(const Tracking &rhs)
        : label(rhs.label), ID(rhs.ID), bb(rhs.bb) {}

Tracking::Tracking(Tracking &&rhs)
        : label(std::move(rhs.label)), ID(std::move(rhs.ID)), bb(std::move(rhs.bb)) {}

// Functions

std::ostream &operator<<(std::ostream &os, const Tracking &t) {
    os << "Label: " << t.label << " ID: " << t.ID << " " << t.bb;
    return os;
}