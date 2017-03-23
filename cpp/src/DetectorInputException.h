#ifndef CPP_NOTIMPLEMENTEDEXCEPTION_H
#define CPP_NOTIMPLEMENTEDEXCEPTION_H


#include <stdexcept>

class DetectorInputException : std::logic_error {
public:
    DetectorInputException();
};


#endif //CPP_NOTIMPLEMENTEDEXCEPTION_H