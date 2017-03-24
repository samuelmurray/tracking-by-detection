#include "RandomDetector.h"

using std::vector;

// Constructors

RandomDetector::RandomDetector() : Detector() {}

RandomDetector::RandomDetector(RandomDetector &&rhs) : Detector(std::move(rhs)) {}

RandomDetector &RandomDetector::operator=(RandomDetector &&rhs) {
    Detector::operator=(std::move(rhs));
    return *this;
}

// Methods

vector<Detection> RandomDetector::detect(const cv::Mat &image) {
    vector<Detection> detections;
    detections.push_back(Detection("test_class",
                                   BoundingBox(
                                           int(0.1 * image.cols) + (rand() % 10 - 5),
                                           int(0.2 * image.rows) + (rand() % 10 - 5),
                                           50, 50
                                   )));
    detections.push_back(Detection("test_class",
                                   BoundingBox(
                                           int(0.4 * image.cols) + (rand() % 10 - 5),
                                           int(0.7 * image.rows) + (rand() % 10 - 5),
                                           40, 100
                                   )));
    detections.push_back(Detection("another_class",
                                   BoundingBox(
                                           int(0.8 * image.cols) + (rand() % 10 - 5),
                                           int(0.8 * image.rows) + (rand() % 10 - 5),
                                           90, 30
                                   )));
    return detections;
}