from __future__ import print_function

import numpy as np

SEQUENCE = "2_2_1"
PATH_TO_DETECTIONS_FILE = "../data/results/okutama/img1/{}.txt".format(SEQUENCE)
PATH_TO_IMAGES = "../data/okutama/{}/img1/".format(SEQUENCE)
PATH_TO_OUTPUT = "../data/results/okutama/trackings/"
CLASS_MAP = {
            0 : "Background",
            1 : "Walking",
            2 : "Sitting",
            3 : "Standing",
            4 : "Running",
            5 : "Lying",
            6 : "Carrying",
            7 : "Pushing/Pulling",
            8 : "Reading",
            9 : "Drinking",
            10 : "Calling",
            11 : "Hand Shaking",
            12 : "Hugging"
}

def generate_and_save_PIL():
  from PIL import Image, ImageDraw, ImageFont

  detections = np.loadtxt(PATH_TO_DETECTIONS_FILE, delimiter=",")
  colours = np.random.randint(0, 255, (32, 3))

  for frame in range(int(detections[:,0].max())):
    file_name = str(frame).rjust(4, '0') + ".jpg"
    image_path = PATH_TO_IMAGES + file_name
    image = Image.open(image_path)
    draw = ImageDraw.Draw(image, 'RGBA')

    dets = detections[detections[:,0]==frame, 1:7]
    dets[:, 4:6] += dets[:, 2:4] # Convert from [x1, y1, width, height] to [x1, y1, x2, y2]
    if frame%10 == 0 :
      print(frame)
    for d in dets:
      d = d.astype(np.uint32)
      c = tuple(colours[d[0]%32, :])
      draw.rectangle([d[2], d[3], d[4], d[5]], fill=(c + (100,)), outline=c)
      draw.text((d[2], d[3] + 5), CLASS_MAP[d[1]], fill=(255,255,255))
    image.save(PATH_TO_OUTPUT + file_name)


def generate_and_save_MATPLOTLIB():
  import matplotlib.pyplot as plt
  import matplotlib.patches as patches
  import matplotlib.image as mpimg
  from skimage import io

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
  generate_and_save_PIL()
