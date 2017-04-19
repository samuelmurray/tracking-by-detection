#include "Detection.h"

// Constructors

Detection::Detection(int label, double confidence, const BoundingBox &bb)
        : label(label), confidence(confidence), bb(bb) {}

Detection::Detection(const Detection &rhs)
        : label(rhs.label), confidence(rhs.confidence), bb(rhs.bb) {}

Detection::Detection(Detection &&rhs)
        : label(rhs.label), confidence(rhs.confidence), bb(std::move(rhs.bb)) {}

// Functions

std::ostream &operator<<(std::ostream &os, const Detection &d) {
    os << "Label: " << d.label << " Confidence: " << d.confidence << " " << d.bb;
    return os;
}