# Python
Scripts for generating images and videos with tracked objects displayed as colored boxes.

## Run
To generate images, run
```
$ python3 generate_images.py -s sequenceMap \
                            -m modelType \
                            -f trackingFormat \
                            [-i frameInterval]
```
The resulting images will be put saved to `output/sequence/`. If `ffmpeg` is installed, the images can be combined to a video by running
```
$ . ../../generate_video.sh [frameRate]
```
in the directory with the images. If the original video has frame rate `X` and frameInterval is `Y`, then frameRate should be `X/Y`.
