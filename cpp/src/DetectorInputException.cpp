#include "DetectorInputException.h"

DetectorInputException::DetectorInputException()
        : std::logic_error("The detector does not support the given input") {};