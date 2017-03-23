#ifndef CPP_TRACKING_H
#define CPP_TRACKING_H


#include <ostream>
#include "BoundingBox.h"

struct Tracking {
    Tracking(const std::string &className, int ID, const BoundingBox &bb);

    const std::string className;
    const int ID;
    const BoundingBox bb;
};

std::ostream &operator<<(std::ostream &os, const Tracking &t);


#endif //CPP_TRACKING_H