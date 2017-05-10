#ifndef CPP_AFFINITY_H
#define CPP_AFFINITY_H


#include "../util/BoundingBox.h"

class Affinity {
public:
    /**
     * Euclidian distance
     * Small value indicates similarity
     */
    static double euclDist(const BoundingBox &a, const BoundingBox &b);

    /**
     * Intersection over union
     * Large value indicates similarity
     * From: Simple Online and Realtime Tracking
     * (Alex Bewley, Zongyuan Ge, Lionel Ott, Fabio Ramos, Ben Upcroft)
     */
    static double iou(const BoundingBox &a, const BoundingBox &b);

    /**
     * Combination of linear similarity in position and shape
     * Small value indicates similarity
     * From: Online multi-target tracking with strong and weak detections
     * (Ricardo Sanchez-Matilla, Fabio Poiesi, Andrea Cavallaro Centre)
     */
    static double linCost(const BoundingBox &a, const BoundingBox &b);

    /**
     * Combination of exponential similarity in position and shape
     * Large value indicates similarity
     * From: POI: Multiple Object Tracking with High Performance Detection and Appearance Feature
     * (Fengwei Yu, Wenbo Li, Quanquan Li, Yu Liu, Xiaohua Shi, Junjie Yan)
     * @param a BoundingBox from Detection
     * @param b BoundingBox from Predictor
     */
    static double expCost(const BoundingBox &a, const BoundingBox &b);

private:
    // Prevent instantiation
    Affinity() {};
};


#endif //CPP_AFFINITY_H