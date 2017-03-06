import sys
import os.path
import cv2

args = sys.argv
assert (len(args) > 1), "You need to to pass a path to input image"
assert (os.path.isfile(args[1])), f"Input file \"{args[1]}\" doesn't exist"
image = cv2.imread(args[1])
assert (image is not None), f"Input file \"{args[1]}\" can't be read as an image"

width = image.shape[1]
cropped_width = width / 2
step_size = width / 15

for i in range(3):
    cv2.imwrite(f"crop_{i}.png", image[:, int(i * step_size): int(cropped_width + i * step_size)])
