
#include <vector>
#include "Tracker.h"

int main() {
    Tracker tracker;
    std::vector<Detection> ret = tracker.track();
    for (auto a : ret) {
        std::cout << a << std::endl;
    }

    return 0;
}