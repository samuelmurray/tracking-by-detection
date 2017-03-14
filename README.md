# tracking-by-detection
Connects detections in consecutive frames in a video in a _tracking-by-detection_ way.

## Configure
To use Caffe, put models in models/ folder. [This](https://github.com/weiliu89/caffe/tree/ssd) version of Caffe is used, which works with SSD models.

Modify ```.pc``` files in config/ folder if you have not set ```$PKG_CONFIG_PATH``` to point at them already.  


## Run
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
Detection.*
Detector.*
BBDetector.* (If Caffe)
NaiveDetector.* (If not Caffe)
Tracker.*
NaiveTracker.*
```
