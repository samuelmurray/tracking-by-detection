# tracking-by-detection
Connects detections in consecutive frames in a video in a _tracking-by-detection_ way.

## Introduction
TODO

## Dependencies
* Build:
  * C++11
  * make
  * pkg-config
* Tracking:
  * dlib (≥19.4)
* Detection:
  * OpenCV (≥3.2)
  * Caffe<sup>\*</sup>
  * Cuda<sup>\*</sup> (≥7)
* Run demos:
  * Boost (≥1.63)

\* For instructions on how to install `Caffe` and `Cuda`, see **Configure**.

#### Mac
Use [Homebrew](https://brew.sh/) to build packages.

To install `OpenCV`, follow [this tutorial](http://www.pyimagesearch.com/2016/12/19/install-opencv-3-on-macos-with-homebrew-the-easy-way/).

Remaining dependencies can be installed with:
```
$ brew install <package>
```

It might be possible to build all packages yourself, without `Homebrew`. Please refer to the instructions for **Linux** below.

#### Linux
To install `boost`, see the [official documentation](http://www.boost.org/doc/libs/1_63_0/more/getting_started/unix-variants.html).

To install `OpenCV`, see the [official documentation](http://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html).

To install `dlib`, download and unpack it from [here](http://dlib.net/). Build as a shared library with:
```
$ cd dclib/dlib
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

#### Windows
Not tested. Please refer to the instructions for **Linux** above; some packages have similar guides for Windows.

## Configure

#### Directory structure
The following directories should be present in root:
* `config` - `.pc` files for dependencies.
* `cpp` - C++ source code.
* `data` - Data sets and sequence maps for running demos.
* `models` - Caffe models for object detection.
* `python` - Python scripts for generating images.

#### pkg-config
Make sure `PKG_CONFIG_PATH` includes the paths to the `.pc` files for all dependencies. Alternatively, copy and modify the `.pc.example` files in `config/`. Test if `pkg-config` can find `.pc` files for all dependencies, including custom ones in `config/`:
```
$ make test
```

#### Caffe
[This version of Caffe](https://github.com/weiliu89/caffe/tree/ssd) is used, which works with SSD models. Thorough instructions are given there on how to install and configure Caffe. They refer to the [official documentation](http://caffe.berkeleyvision.org/installation.html) on how to install prerequisites.

#### Cuda
See **Caffe** above.

## Demo
#### Directory structure
* `data/` - Put datasets (images and/or detections) here. Images are expected to be in `<path/to/sequence>/images/`, and detections in `<path/to/sequence>/model-type` (this is also where custom detections will be put). Sequence maps should exist in `data/seqmaps` (an example file is given). Tracking output will be in `data/result/<same/structure/as/dataset>/<sequence-name>.txt`.
* `models/` - To use Caffe, put models in `models/`. Config files for each model should exist in `models/config/` (an example file is given).

Three example usages are provided:
1. **Detect** - Detect objects in a provided sequence of images. Objects are detected by a CNN (Caffe).
1. **Track** - Track objects from provided detections. The detections should be on the format used in [MOTChallenge](https://motchallenge.net/), or a custom format used for the Okutama-Action dataset (not yet publicly available).
1. **Detect and Track** - Track objects detected in a provided sequence of images. Objects are detected by a CNN (Caffe).

## Integrate in other projects
All source code is in `cpp/src/`.
* To track objects from pre-existing detections, create an instance of `MCSORT`. No code in `detector/` is needed. Use function `tracker.track(<detections>)`.
* To detect and track objects from images, create an instance of `ImageTracker`. Requires Caffe. Use function `tracker.detectAndTrack(<image>)`.
