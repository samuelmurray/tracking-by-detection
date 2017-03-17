#ifndef CPP_DETECTION_H
#define CPP_DETECTION_H


#include <string>
#include <ostream>
#include "BoundingBox.h"

struct Detection {
    Detection(const BoundingBox &boundingBox, int ID);

    Detection(const std::string &className, int cx, int cy, int width, int height, int ID);

    BoundingBox boundingBox;
    int ID;
};

std::ostream &operator<<(std::ostream &os, const Detection &d);


#endif //CPP_DETECTION_H