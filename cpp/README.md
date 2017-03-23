# C++
The main code of the project.

## Run
This provides an environment for testing the object detectors and trackers. The Naive versions output sample bounding boxes with slight noise for each frame, independent of the image. These can be used for easy compiling and debugging of other parts.

The ```main``` function is a loop that at each step takes a path to an image and outputs detections to the terminal.

If you have Caffe, run the full version with
```
$ make
$ ./main.out
```
Otherwise, run a test version with
```
$ make nocaffe
$ ./ncmain.out
```


## Integrate in other projects
To integrate the tracker in other projects, you need the following files:
```
Tracking.*
Detector.*
BBDetector.* (If Caffe)
RandomDetector.* (If not Caffe)
Tracker.*
RandomTracker.*
```
