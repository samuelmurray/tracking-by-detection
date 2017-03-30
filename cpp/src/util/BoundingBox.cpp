#include "BoundingBox.h"

// Constructors

BoundingBox::BoundingBox(double cx, double cy, double width, double height)
        : cx(cx), cy(cy), width(width), height(height) {}

// Functions

std::ostream &operator<<(std::ostream &os, const BoundingBox &bb) {
    os << "(" << bb.cx << "," << bb.cy << ") - " << bb.width << "x" << bb.height;
    return os;
}