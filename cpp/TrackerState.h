#ifndef CPP_TRACKERSTATE_H
#define CPP_TRACKERSTATE_H


#include <vector>
#include "Detection.h"

class TrackerState {
public:
    void update(std::vector<Detection>);

private:
    std::vector<Detection> previousDetections;
};


#endif //CPP_TRACKERSTATE_H