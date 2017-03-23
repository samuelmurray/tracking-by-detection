#include "Detection.h"

// Constructors

Detection::Detection(const std::string &className, int cx, int cy, int width, int height)
        : className(className), cx(cx), cy(cy), width(width), height(height) {}

// Functions

double Detection::iou(const Detection &a, const Detection &b) {
    int xx1 = std::max(a.x1(), b.x1());
    int yy1 = std::max(a.y1(), b.y1());
    int xx2 = std::min(a.x2(), b.x2());
    int yy2 = std::min(a.y2(), b.y2());
    int width = std::max(0, xx2 - xx1);
    int height = std::max(0, yy2 - yy1);
    double intersection = width * height;
    double uni = a.area() + b.area() - intersection;
    return intersection / uni;
}

std::ostream &operator<<(std::ostream &os, const Detection &d) {
    os << d.className << " (" << d.cx << "," << d.cy << ") - " << d.width << "x" << d.height;
    return os;
}