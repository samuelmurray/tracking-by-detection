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

<sup>\*</sup>For instructions on how to install `Caffe` and `Cuda`, see **Configure**.

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
* `cpp` - All source code.
* `data` - Data sets and `config_file.txt` for running the demos. The file structure of MOTChallenge is assumed for the data sets.
* `models` - Caffe models.
* `python` - Extra Python code. Not needed for main project.

#### pkg-config
Make sure `PKG_CONFIG_PATH` includes the paths to the `.pc` files for all dependencies. Alternatively, copy and modify the `.pc.example` files in `config/`. Test if `pkg-config` can find `.pc` files for all dependencies, including custom ones in `config/`:
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
