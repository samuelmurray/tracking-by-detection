#ifndef CPP_TRACKER_H
#define CPP_TRACKER_H


#include <opencv2/core/core.hpp>
#include <vector>

#include "Detector.h"
#include "Detection.h"

class Tracker {
public:
    Tracker(std::shared_ptr<Detector>);

    virtual ~Tracker();

    Tracker(const Tracker &) = delete;

    Tracker(const Tracker &&) = delete;

    Tracker &operator=(const Tracker &) = delete;

    Tracker &operator=(const Tracker &&) = delete;

    virtual std::vector<Detection> track(cv::Mat mat) = 0;

protected:
    std::shared_ptr<Detector> detector;
};


#endif //CPP_TRACKER_H