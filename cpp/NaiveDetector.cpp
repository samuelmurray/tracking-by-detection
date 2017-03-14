#include "NaiveDetector.h"

// Constructors

NaiveDetector::NaiveDetector() : Detector() {}

// Methods

std::vector<Detection> NaiveDetector::detect(const cv::Mat &image) {
    std::vector<Detection> detections;
    detections.push_back(Detection{"test_class",
                                   int(0.1 * image.cols) + (rand() % 10 - 5),
                                   int(0.2 * image.rows) + (rand() % 10 - 5),
                                   50, 50});
    detections.push_back(Detection{"test_class",
                                   int(0.4 * image.cols) + (rand() % 10 - 5),
                                   int(0.7 * image.rows) + (rand() % 10 - 5),
                                   40, 100});
    detections.push_back(Detection{"another_class",
                                   int(0.8 * image.cols) + (rand() % 10 - 5),
                                   int(0.8 * image.rows) + (rand() % 10 - 5),
                                   90, 30});
    return detections;
}