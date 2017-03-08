#include <vector>
#include "Tracker.h"
#include "NaiveTracker.h"

int main() {
    Tracker *tracker = new NaiveTracker();
    std::vector<Detection> ret = tracker->track();
    for (auto a : ret) {
        std::cout << a << std::endl;
    }

    return 0;
}