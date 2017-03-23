#ifndef CPP_NOASSOCIATEDDETECTOR_H
#define CPP_NOASSOCIATEDDETECTOR_H


#include <stdexcept>

class NoAssociatedDetector : std::logic_error {
public:
    NoAssociatedDetector();
};


#endif //CPP_NOASSOCIATEDDETECTOR_H