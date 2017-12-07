# tracking-by-detection
Code for my master thesis, titled "Real-Time Multiple Object Tracking: A Study on the Importance of Speed".

## Introduction
In this project, we implement a multiple object tracker, following the _tracking-by-detection_ paradigm, as an extension of an existing method. It works by modelling the movement of objects by solving the filtering problem, and associating detections with predicted new locations in new frames using the Hungarian algorithm. Three different similarity measures are used, which use the location and shape of the bounding boxes. Compared to other trackers on the [MOTChallenge](https://motchallenge.net/) leaderboard, our method, referred to as _C++SORT_, is the fastest non-anonymous submission, while also achieving decent score on other metrics. By running our model on the [Okutama-Action](http://okutama-action.org/) dataset, sampled at different frame-rates, we show that the performance is greatly reduced when running the model - including detecting objects - in real-time. In most metrics, the score is reduced by 50%, but in certain cases as much as 90%. We argue that this indicates that other, slower methods could not be used for tracking in real-time, but that more research is required specifically on this.

This work constitutes my master thesis. The thesis is published [on a university portal](http://www.diva-portal.org/smash/record.jsf?pid=diva2:1146388), but is also made avalable [on arXiv](https://arxiv.org/abs/1709.03572).

## Citing
If you find the thesis or this repository useful in your research, please consider citing the report:
```
@article{murray2017real,
  title={Real-Time Multiple Object Tracking - A Study on the Importance of Speed},
  author={Murray, Samuel},
  journal={arXiv preprint arXiv:1709.03572},
  year = {2017}
}
```

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
$ cd dlib-<version>
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
1. **Track** - Track objects from provided detections. The detections should be on the format used in [MOTChallenge](https://motchallenge.net/).
1. **Detect and Track** - Track objects detected in a provided sequence of images. Objects are detected by a CNN (Caffe).

## Integrate in other projects
All source code is in `cpp/src/`.
* To track objects from pre-existing detections, create an instance of `MCSORT`. No code in `detector/` is needed. Use function `tracker.track(<detections>)`.
* To detect and track objects from images, create an instance of `ImageTracker`. Requires Caffe. Use function `tracker.detectAndTrack(<image>)`.
