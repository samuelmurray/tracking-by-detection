from __future__ import print_function

import os.path
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.image as mpimg
from skimage import io
import time

def draw_images():
  colours = np.random.rand(32,3) #used only for display
  plt.ion()
  fig = plt.figure() 
  

  #detections = np.loadtxt('../data/results/okutama/img1/seq-1_1_10.txt',delimiter=',') #load detections
  detections = np.loadtxt("data/detections.txt",delimiter=",") #load detections
  
  for frame in range(int(detections[:,0].max())):
    ax1 = fig.add_subplot(111, aspect="equal")
    dets = detections[detections[:,0]==frame,1:6]
    file_name = f"{str(frame).rjust(4, '0')}.jpg"
    image_path = f"../data/okutama/seq-1_1_10/img1/{file_name}"
    im = io.imread(image_path)
    plt.imshow(im)
    plt.title("Tracked Targets")
    
    for d in dets:
      d = d.astype(np.uint32)
      ax1.add_patch(patches.Rectangle((d[1],d[2]),d[3],d[4],fill=False,lw=3,ec=colours[d[0]%32,:]))
      ax1.set_adjustable('box-forced')
    fig.canvas.flush_events()
    #plt.pause(0.001)
    plt.draw()
    plt.savefig(f"data/images/{file_name}")
    ax1.cla()

if __name__ == "__main__":
  draw_images()
