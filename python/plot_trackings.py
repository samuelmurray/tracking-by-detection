from __future__ import print_function

import os.path
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.image as mpimg
from skimage import io
import time

PATH_TO_DETECTIONS_FILE = "data/detections.txt"
PATH_TO_IMAGES = "../data/okutama/seq-1_1_10/img1/"
PATH_TO_OUTPUT = "data/images/"

def generate_and_save():
  detections = np.loadtxt(PATH_TO_DETECTIONS_FILE, delimiter=",")

  colours = np.random.rand(32,3)
  plt.ion()
  fig = plt.figure() 
  
  for frame in range(int(detections[:,0].max())):
    ax1 = fig.add_subplot(111, aspect="equal")
    file_name = str(frame).rjust(4, '0') + ".jpg"
    image_path = PATH_TO_IMAGES + file_name
    im = io.imread(image_path)
    plt.imshow(im)
    plt.title("Tracked Targets")
    
    dets = detections[detections[:,0]==frame,1:6]
    for d in dets:
      d = d.astype(np.uint32)
      ax1.add_patch(patches.Rectangle((d[1],d[2]),d[3],d[4],fill=False,lw=3,ec=colours[d[0]%32,:]))
      ax1.set_adjustable("box-forced")

    fig.canvas.flush_events()
    plt.draw()
    plt.savefig(PATH_TO_OUTPUT + file_name)
    ax1.cla()

if __name__ == "__main__":
  draw_images()
