#ifndef CPP_DETECTION_H
#define CPP_DETECTION_H


#include "BoundingBox.h"

#include <ostream>

struct Detection {
    Detection(int label, double confidence, const BoundingBox &bb);

    Detection(const Detection &rhs);

    Detection(Detection &&rhs);

    // Prevent assignment
    Detection &operator=(const Detection &rhs) = delete;

    Detection &operator=(Detection &&rhs) = delete;

    const int label;
    const double confidence;
    const BoundingBox bb;
};

std::ostream &operator<<(std::ostream &os, const Detection &d);


#endif //CPP_DETECTION_H