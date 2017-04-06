# C++
The source code of the project.

## Compile
To compile every file, run
```
$ make compile [use_caffe=true] [force_rebuild=true]
```
**NOTE:** Calling `make <target>` without passing `use_caffe=true` will make the preprocessor remove all code that uses Caffe. This is to enable compilation without Caffe installed. However, this can lead to problems when `make detect use_caffe=true` is called after `make track`, since the files that use Caffe will not be rebuilt. To prevent this, either pass `use_caffe=true` every time you compile, or call `force_rebuild=true` to force all `.o` files to be rebuilt.

## Demo
Two example usages are provided. Note that the following targets will also compile the code, and so the same variables (`use_caffe` and `force_rebuild`) can be set.
1. _Track:_ Track objects from provided detections. The detections should be on the format used in [MOTChallenge](https://motchallenge.net/).  
```
$ make track
$ ./trackApp.out [-g] [-f <config_file>]
```
1. _Detect and Track:_ Track objects detected in a provided sequence of images. Objects are detected by a CNN (Caffe).
```
$ make detect use_caffe=true
$ ./detectApp.out [-f <config_file>]
```

## Integrate in other projects
* To track objects from pre-existing detections, create an instance of `MCSORT`. No code in `detector/` is needed.  
* To detect and track objects from images, create an instance of `VideoTracker`. Requires Caffe.

For both classes, the function `track(...)` is used.
