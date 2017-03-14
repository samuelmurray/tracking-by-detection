# tracking-by-detection

Connects detections in consecutive frames in a video in a _tracking-by-detection_ way. 

## Configure
Modify ```.pc``` files in config/ folder if you have not set ```$PKG_CONFIG_PATH``` to point at them already.  
To use Caffe, put models in models/ folder.

## Run
If you have Caffe, run the full version with
```
make
./main.out
```
Otherwise, run a test version with
```
make nocaffe
./ncmain.out
```
