#ifndef CPP_MCSORT_H
#define CPP_MCSORT_H


#include "Tracker.h"
#include "DetectionPredictor.h"

class MCSORT;

class MCSORT : public Tracker {
    struct Association;

public:
    static const int maxAge;
    static const int minHits;

public:
    MCSORT();

    MCSORT(const std::shared_ptr<Detector> &detector);

    std::vector<Tracking> track(const cv::Mat &image) override;

private:
    std::vector<DetectionPredictor> predictors = std::vector<DetectionPredictor>();
    int frameCount = 1;

    Association associateDetectionsToPredictors(const std::vector<Detection> &detections);

    struct Association {
        std::vector<std::pair<int, int>> matches;
        std::vector<int> unmatchedDetections;
        std::vector<int> unmatchedPredictors;
    };
};


#endif //CPP_MCSORT_H