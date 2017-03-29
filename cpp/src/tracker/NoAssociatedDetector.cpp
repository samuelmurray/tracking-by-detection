#include "NoAssociatedDetector.h"

// Constructors

NoAssociatedDetector::NoAssociatedDetector()
        : std::logic_error("The tracker has no associated detector - can't call 'track(image)'") {};