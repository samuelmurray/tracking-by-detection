#ifndef CPP_TRACKER_H
#define CPP_TRACKER_H


#include <opencv2/core/core.hpp>
#include <vector>

#include "Detector.h"
#include "Tracking.h"

class Tracker {
public:
    Tracker(const std::shared_ptr<Detector> &detector);

    virtual ~Tracker();

    virtual std::vector<Tracking> track(const cv::Mat &image) = 0;

    // Prevent copying and moving
    Tracker(const Tracker &) = delete;

    Tracker(const Tracker &&) = delete;

    Tracker &operator=(const Tracker &) = delete;

    Tracker &operator=(const Tracker &&) = delete;

protected:
    const std::shared_ptr<Detector> detector;
};


#endif //CPP_TRACKER_H