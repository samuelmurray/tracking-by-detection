#ifndef CPP_BOUNDINGBOX_H
#define CPP_BOUNDINGBOX_H


#include <ostream>

struct BoundingBox {
    BoundingBox(double cx, double cy, double width, double height);

    const double cx;
    const double cy;
    const double width;
    const double height;

    // (x1,y1) - top left
    // (x2,y2) - bottom right
    inline double x1() const { return cx - width / 2; }
    inline double y1() const { return cy - height / 2; }
    inline double x2() const { return cx + width / 2; }
    inline double y2() const { return cy + height / 2; }
    inline double area() const { return width * height; }
    inline double ratio() const { return width / height; }
};

std::ostream &operator<<(std::ostream &os, const BoundingBox &bb);


#endif //CPP_BOUNDINGBOX_H