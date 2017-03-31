#ifndef CPP_TRACKING_H
#define CPP_TRACKING_H


#include "BoundingBox.h"

#include <ostream>

struct Tracking {
    Tracking(const std::string &className, int ID, const BoundingBox &bb);

    Tracking(const Tracking &rhs);

    Tracking(Tracking &&rhs);

    // Prevent assignment
    Tracking &operator=(const Tracking &rhs) = delete;

    Tracking &operator=(Tracking &&rhs) = delete;

    const std::string className;
    const int ID;
    const BoundingBox bb;
};

std::ostream &operator<<(std::ostream &os, const Tracking &t);


#endif //CPP_TRACKING_H