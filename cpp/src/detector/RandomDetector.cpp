#include "RandomDetector.h"

// Methods

std::vector<Detection> RandomDetector::detect(const cv::Mat &image, double confidenceThreshold) {
    std::vector<Detection> detections;
    detections.push_back(Detection(0,
                                   BoundingBox(0.1 * image.cols + rand() % 10 - 5,
                                               0.2 * image.rows + rand() % 10 - 5,
                                               50,
                                               50
                                   )));
    detections.push_back(Detection(0,
                                   BoundingBox(0.4 * image.cols + rand() % 10 - 5,
                                               0.7 * image.rows + rand() % 10 - 5,
                                               40,
                                               100
                                   )));
    detections.push_back(Detection(1,
                                   BoundingBox(0.8 * image.cols + rand() % 10 - 5,
                                               0.8 * image.rows + rand() % 10 - 5,
                                               90,
                                               30
                                   )));
    return detections;
}