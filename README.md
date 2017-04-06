# tracking-by-detection
Connects detections in consecutive frames in a video in a _tracking-by-detection_ way.

## Introduction
TODO

## Dependencies
* Build:
  * C++11
  * make
  * pkg-config
* Track detections:
  * dlib
  * openblas
* Detect objects in images:
  * OpenCV
  * Caffe
  * Cuda
* Run demos:
  * Boost

#### Mac
All dependencies can be installed with [Homebrew](https://brew.sh/) using:
```
$ brew install <package>
```

#### Linux
Hopefully everything can be installed with `apt-get`.

#### Windows
Not tested.

## Configure

#### Directory structure
The following directories should be present in root:
* `config` - `.pc` files for dependencies.
* `cpp` - All source code.
* `data` - Data sets and `config_file.txt` for running the demos. The file structure of MOTChallenge is assumed for the data sets.
* `models` - Caffe models.
* `python` - Extra Python code. Not needed for main project.

#### pkg-config
Make sure `PKG_CONFIG_PATH` includes the paths to the `.pc` files for all dependencies. Alternatively, copy and modify the `.pc.example` files in `config/`. Test if `pkg-config` can find all packages:
```
$ make test
```

#### Caffe
To use Caffe, put models in `models/`. [This](https://github.com/weiliu89/caffe/tree/ssd) version of Caffe is used, which works with SSD models.

#### Data


## Demo
Two example usages are provided. Start by `cd` to `cpp/`
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

**NOTE:** Calling `make <target>` without passing `use_caffe=true` will make the preprocessor remove all code that uses Caffe. This is to enable compilation without Caffe installed. However, this can lead to problems when `make detect use_caffe=true` is called after `make track`, since the files that use Caffe will not be rebuilt. To prevent this, either pass `use_caffe=true` every time you compile, or call `force_rebuild=true` to force all `.o` files to be rebuilt. 

## Integrate in other projects
* To track objects from pre-existing detections, create an instance of `MCSORT`. No code in `detector/` is needed.  
* To detect and track objects from images, create an instance of `VideoTracker`. Requires Caffe.

For both classes, the function `tracker.track(...)` is used.
