#ifndef CPP_TRACKING_H
#define CPP_TRACKING_H


#include <string>
#include <ostream>
#include "Detection.h"

struct Tracking {
    Tracking(const Detection &detection, int ID);

    Tracking(const std::string &className, int cx, int cy, int width, int height, int ID);

    Detection detection;
    int ID;
};

std::ostream &operator<<(std::ostream &os, const Tracking &t);


#endif //CPP_TRACKING_H