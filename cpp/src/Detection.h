#ifndef CPP_DETECTION_H
#define CPP_DETECTION_H


#include <ostream>

struct Detection {
    Detection(const std::string &className, int cx, int cy, int width, int height);

    const std::string className;
    const int cx;
    const int cy;
    const int width;
    const int height;

    // (x1,y1) - top left
    // (x2,y2) - bottom right
    inline int x1() const { return cx - width / 2; }
    inline int y1() const { return cy + height / 2; }
    inline int x2() const { return cx + width / 2; }
    inline int y2() const { return cy - height / 2; }
    inline int area() const { return width * height; }

    static double iou(const Detection &a, const Detection &b);
};

std::ostream &operator<<(std::ostream &os, const Detection &d);


#endif //CPP_DETECTION_H