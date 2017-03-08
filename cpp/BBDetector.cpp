#include "BBDetector.h"

BBDetector::BBDetector()
        : Detector() {
    // Load caffe / TF network
}

std::vector<Detection> BBDetector::detect() const {
    std::vector<Detection> detections;
    // ret = network.inference(image);
    // det = parse(ret);
    detections.push_back(Detection{"test", 1, 0, 3, 4});
    detections.push_back(Detection{"test", 4, 3, 4, 2});
    detections.push_back(Detection{"test", 1, 4, 5, 6});
    return detections;
}
