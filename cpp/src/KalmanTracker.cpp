#include "KalmanTracker.h"
#include "NaiveDetector.h"
#include <munkres/matrix.h>
#include <munkres/munkres.h>

using std::vector;

const int KalmanTracker::maxAge = 1;
const int KalmanTracker::minHits = 3;

// Constructors

KalmanTracker::KalmanTracker(Detector *)
        : Tracker(new NaiveDetector) {}

// Methods

vector<Detection> KalmanTracker::track(cv::Mat mat) {
    frameCount++;
    vector<Detection> toReturn;
    vector<BoundingBox> detections = detector->detect(mat);
    vector<BoundingBox> predictions;
    for (auto filterer : filterers) {
        filterer.predict();
    }
    Association association = associateDetectionsToTrackers(detections);



    /*
     * Params:
     *  dets - a numpy array of detections in the format [[x,y,w,h,score],[x,y,w,h,score],...]
     * Requires: this method must be called once for each frame even with empty detections.
     * Returns the a similar array, where the last column is the object ID.
     * NOTE: The number of objects returned may differ from the number of detections provided.
     */

    // TODO: Convert Python code below to C++

    /*
     self.frame_count += 1
     #get predicted locations from existing trackers.
    trks = np.zeros((len(self.trackers),5))
    to_del = []
    ret = []
    for t,trk in enumerate(trks):
      pos = self.trackers[t].predict()[0]
      trk[:] = [pos[0], pos[1], pos[2], pos[3], 0]
      if(np.any(np.isnan(pos))):
        to_del.append(t)
    trks = np.ma.compress_rows(np.ma.masked_invalid(trks))
    for t in reversed(to_del):
      self.trackers.pop(t)
    matched, unmatched_dets, unmatched_trks = associate_detections_to_trackers(dets,trks)

    #update matched trackers with assigned detections
    for t,trk in enumerate(self.trackers):
      if(t not in unmatched_trks):
        d = matched[np.where(matched[:,1]==t)[0],0]
        trk.update(dets[d,:][0])

    #create and initialise new trackers for unmatched detections
    for i in unmatched_dets:
        trk = KalmanBoxTracker(dets[i,:])
        self.trackers.append(trk)
    i = len(self.trackers)
    for trk in reversed(self.trackers):
        d = trk.getState()[0]
        if((trk.time_since_update < 1) and (trk.hit_streak >= self.min_hits or self.frame_count <= self.min_hits)):
          ret.append(np.concatenate((d,[trk.id+1])).reshape(1,-1)) # +1 as MOT benchmark requires positive
        i -= 1
        #remove dead tracklet
        if(trk.time_since_update > self.max_age):
          self.trackers.pop(i)
    if(len(ret)>0):
      return np.concatenate(ret)
    return np.empty((0,5))
     */
    return vector<Detection>();
}

KalmanTracker::Association KalmanTracker::associateDetectionsToTrackers(vector<BoundingBox> detections) {
    double IOU_THRESHOLD = 0.3;
    // TODO: Implement
    if (filterers.empty()) {
        return KalmanTracker::Association{vector<std::pair<int, int>>(), detections, vector<KalmanFilterer>()};
    }
    Matrix<double> iouMatrix(detections.size(), filterers.size());
    for (size_t row = 0; row < iouMatrix.rows(); ++row) {
        for (size_t col = 0; col < iouMatrix.columns(); ++col) {
            iouMatrix(row, col) = BoundingBox::iou(detections.at(row), filterers.at(col).getState());
        }
    }

    Munkres<double> munkres;
    munkres.solve(iouMatrix);

    return KalmanTracker::Association();
}