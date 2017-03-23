#include "Detector.h"
#include "DetectorInputException.h"

// Methods

std::vector<Detection> Detector::detect(const cv::Mat &image) {
    throw DetectorInputException();
}

std::vector<Detection> Detector::detect() {
    throw DetectorInputException();
}