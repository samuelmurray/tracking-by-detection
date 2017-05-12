from __future__ import print_function

import numpy as np
import os
import argparse

LABEL_ACTION_MAP = {
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


def generate(sequence_path, model_type, frame_interval, confidence_threshold):
	from PIL import Image, ImageDraw, ImageFont
	path_until_sequence, sequence_name = os.path.split(sequence_path)
	tracking_file_path = "../data/results/{}/{}/{}.txt".format(path_until_sequence, model_type, sequence_name)
	image_dir_path = "../data/{}/images".format(sequence_path)
	output_dir_path = "output/{}".format(sequence_name)
	if not os.path.exists(output_dir_path):
		print("Creating output directory {}".format(output_dir_path))
		os.makedirs(output_dir_path)

	detections = np.loadtxt(os.path.abspath(tracking_file_path), delimiter=",")
	colours = np.random.randint(0, 255, (32, 3))

	for frame in range(0, int(detections[:,0].max()), frame_interval):
		image_name = str(frame).rjust(4, '0') + ".jpg"
		image_path = "{}/{}".format(image_dir_path, image_name)

		with Image.open(image_path) as image:
			draw = ImageDraw.Draw(image, 'RGBA')
			# Format: [frame, ID, x1, y1, width, height, confidence, _, _, _, (label)]
			dets = detections[detections[:,0]==frame, 1:7]
			labels = None
			if model_type == "action-detection":
				labels = detections[detections[:,0]==frame, 1]
			dets[:, 3:5] += dets[:, 1:3] # Convert from [x1, y1, width, height] to [x1, y1, x2, y2]
			for i, d in enumerate(dets):
			    if d[5] < CONFIDENCE_THRESHOLD:
			      	continue
			    d = d.astype(np.int32)
			    c = tuple(colours[d[0]%32, :])
			    draw.rectangle([d[1], d[2], d[3], d[4]], fill=(c + (100,)), outline=c)
			    if model_type == "action-detection":
		    		draw.text((d[1], d[2] + 4), LABEL_ACTION_MAP[labels[i]], fill=(255,255,255))
			image.save("{}/{}".format(output_dir_path, image_name))


if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('-s', '--sequenceMap', metavar='sequenceMap', required=True,
						help='a textfile specifying paths to sequences')
	parser.add_argument('-m', '--modelType', metavar='modelType', required=True,
						help='name of the type of model')
	parser.add_argument('-i', '--frameInterval', metavar='frameInterval', required=False, type=int, default=1,
						help='number of frames to skip to skip between each produced frame')
	parser.add_argument('-c', '--confidenceThreshold', metavar='confidenceThreshold', required=False, type=float, default=0.5,
						help='don\'t display detections with confidence below this value')
	args = parser.parse_args()
	with open("../data/seqmaps/{}".format(args.sequenceMap)) as f:
		for sequence_path in f:
			generate(sequence_path, args.modelType, args.frameInterval, args.confidenceThreshold)
