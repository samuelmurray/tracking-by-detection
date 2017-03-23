#ifndef CPP_DETECTION_H
#define CPP_DETECTION_H


#include <ostream>
#include "BoundingBox.h"

struct Detection {
    Detection(const std::string &className, const BoundingBox &bb);

    Detection(const Detection &rhs);

    Detection(Detection &&rhs);

    Detection &operator=(const Detection &rhs) = delete;

    Detection &operator=(Detection &&rhs) = delete;

    static double iou(const Detection &a, const Detection &b);

    const std::string className;
    const BoundingBox bb;
};

std::ostream &operator<<(std::ostream &os, const Detection &d);


#endif //CPP_DETECTION_H