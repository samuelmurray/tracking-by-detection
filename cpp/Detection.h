#ifndef CPP_DETECTION_H
#define CPP_DETECTION_H


#include <string>
#include <iostream>

struct Detection {
    std::string class_name;
    int cx;
    int cy;
    int width;
    int height;
    int ID;
};

std::ostream &operator<<(std::ostream &os, const Detection &d);


#endif //CPP_DETECTION_H