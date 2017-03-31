#ifndef CPP_DETECTION_H
#define CPP_DETECTION_H


#include "BoundingBox.h"

#include <ostream>

struct Detection {
    Detection(const std::string &className, const BoundingBox &bb);

    Detection(const Detection &rhs);

    Detection(Detection &&rhs);

    // Prevent assignment
    Detection &operator=(const Detection &rhs) = delete;

    Detection &operator=(Detection &&rhs) = delete;

    const std::string className;
    const BoundingBox bb;
};

std::ostream &operator<<(std::ostream &os, const Detection &d);


#endif //CPP_DETECTION_H