#ifndef CPP_MCSORT_H
#define CPP_MCSORT_H


#include "Tracker.h"
#include "Detector.h"
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

    std::vector<Tracking> track(const std::vector<Detection> &detections) override;

private:
    const std::shared_ptr<Detector> detector;
    std::vector<DetectionPredictor> predictors;
    int frameCount = 0;

    static Association associateDetectionsToPredictors(const std::vector<Detection> &detections,
                                                       const std::vector<DetectionPredictor> &predictors,
                                                       double iouThreshold = 0.3);

    struct Association {
        std::vector<std::pair<int, int>> matches;
        std::vector<int> unmatchedDetections;
        std::vector<int> unmatchedPredictors;
    };
};


#endif //CPP_MCSORT_H