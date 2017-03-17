#include "BoundingBox.h"

BoundingBox::BoundingBox(const std::string &className, int cx, int cy, int width, int height)
        : className(className), cx(cx), cy(cy), width(width), height(height) {}

std::ostream &operator<<(std::ostream &os, const BoundingBox &bb) {
    os << bb.className << " (" << bb.cx << "," << bb.cy << ") - " << bb.width << "x" << bb.height;
    return os;
}