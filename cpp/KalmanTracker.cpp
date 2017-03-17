#include "KalmanTracker.h"
#include "NaiveDetector.h"

using std::vector;

// Constructors

KalmanTracker::KalmanTracker(Detector *)
        : Tracker(new NaiveDetector) {}

// Methods

vector<Detection> KalmanTracker::track(cv::Mat mat) {
    vector<BoundingBox> bboxes = detector->detect(mat);
    frameCount++;

    /*
     * Params:
     *  dets - a numpy array of detections in the format [[x,y,w,h,score],[x,y,w,h,score],...]
     * Requires: this method must be called once for each frame even with empty detections.
     * Returns the a similar array, where the last column is the object ID.
     * NOTE: The number of objects returned may differ from the number of detections provided.
     */

    // TODO: Convert Python code below to C++

    /*
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
        d = trk.get_state()[0]
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