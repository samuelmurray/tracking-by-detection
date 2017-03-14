#include "Detection.h"

std::ostream &operator<<(std::ostream &os, const Detection &d) {
    os << d.className << " " << d.ID << " : ("
       << d.cx << "," << d.cy << ") - "
       << d.width << "x" << d.height;
    return os;
}