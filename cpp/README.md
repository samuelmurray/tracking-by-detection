# C++
The source code of the project. Code is located in `src/`.

## Compile
Compile everything:
```
$ make compile [use_caffe=true] [force_rebuild=true]
```

Remove all created files:
```
$ make clean
```

Check the value of a makefile variable:
```
$ make print-<variable>
```

**NOTE:** Calling `make <target>` without passing `use_caffe=true` will make the preprocessor remove all code that uses Caffe. This is to enable compilation without Caffe installed. However, this can lead to problems when _e.g._ `make detect use_caffe=true` is called after `make track`, since the files that use Caffe will not be rebuilt. To prevent this, either pass `use_caffe=true` every time you compile, or call `force_rebuild=true` to force all `.o` files to be rebuilt.

## Demo
Two example usages are provided. Note that the following targets will also compile the code, and so the same variables (`use_caffe` and `force_rebuild`) are used.  
Three example usages are provided:
1. **Detect** - Detect objects in a provided sequence of images. Objects are detected by a CNN (Caffe).
```
$ make detect
$ ./detectApp.out [-s sequencesFile] [-m modelConfigFile]
```
2. **Track** - Track objects from provided detections. The detections should be on the format used in [MOTChallenge](https://motchallenge.net/).
```
$ make track
$ ./trackApp.out [-s sequencesFile] [-m modelType] \
                    [-d detectionFormat] [-i frameInterval]
```

3. **Detect and Track** - Track objects detected in a provided sequence of images. Objects are detected by a CNN (Caffe).
```
$ make detectAndTrack
$ ./trackApp.out [-s sequencesFile] [-m modelConfigFile]
```

## Integrate in other projects
* To detect objects from images, create an instance of `SSDDetector`. Requires Caffe. Use function `detector.detect(<image>)`.
* To track objects from pre-existing detections, create an instance of `PAOT`. No code in `detector/` is needed. Use function `tracker.track(<detections>)`.
* To detect and track objects from images, create an instance of `ImageTracker`. Requires Caffe. Use function `tracker.detectAndTrack(<image>)`.
