# tracking-by-detection
Connects detections in consecutive frames in a video in a _tracking-by-detection_ way.

## Configure
To use Caffe, put models in ```models/``` folder. [This](https://github.com/weiliu89/caffe/tree/ssd) version of Caffe is used, which works with SSD models.

Copy and modify ```.pc.example``` files in ```config/``` directory if you have not set ```$PKG_CONFIG_PATH``` to point at already existing ones. You can test of the the path is already set this by:

```
$ pkg-config --cflags <package>
$ pkg-config --libs <package>
```

## Run
If you have Caffe, run the full version with
```
$ make caffe
$ ./main.out
```
Otherwise, run a test version with
```
$ make nocaffe
$ ./main.out
```

## Integrate in other projects
Depending on if you use Caffe or not, you use the following files:
```
BBDetector.* (If Caffe)
RandomDetector.* (If not Caffe)
```
