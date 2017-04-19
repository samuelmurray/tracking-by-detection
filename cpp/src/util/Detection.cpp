#include "Detection.h"

// Constructors

Detection::Detection(int label, const BoundingBox &bb)
        : label(label), bb(bb) {}

Detection::Detection(const Detection &rhs)
        : label(rhs.label), bb(rhs.bb) {}

Detection::Detection(Detection &&rhs)
        : label(std::move(rhs.label)), bb(std::move(rhs.bb)) {}

// Functions

std::ostream &operator<<(std::ostream &os, const Detection &d) {
    os << "Label: " << d.label << " " << d.bb;
    return os;
}