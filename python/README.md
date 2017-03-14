# Python
Scripts for separate tasks. Not needed for the C++ part.

## Run

By inputing images to ```semantic_segmentation_detector.py``` as below, you get output images where each found object is highlighted and given an ID.  
**NOTE:** the images are expected to be prepared by semantic segmentation, with colors corresponding to classes as described in ```semantic_segmentation_detector.py```.

```
$ python3 semantic_segmentation_detector.py <path_to_input_image> (<path_to_output_image>)
```

Alternatively, you can run an interactive version of the (naive) tracker as below. If you give consecutive frames of an image, it will preserve IDs of objects. Don't expect it to work when objects enter or leave the scene!

```
$ python3 -i tracker.py
> t.test("<path_to_input_image>", "<path_to_output_image>")
```
