#include "Detection.h"

Detection::Detection(const BoundingBox &boundingBox, int ID)
        : boundingBox(boundingBox), ID(ID) {}

Detection::Detection(const std::string &className, int cx, int cy, int width, int height, int ID)
        : boundingBox(BoundingBox(className, cx, cy, width, height)), ID(ID) {}

std::ostream &operator<<(std::ostream &os, const Detection &d) {
    os << d.boundingBox << " ID: " << d.ID;
    return os;
}