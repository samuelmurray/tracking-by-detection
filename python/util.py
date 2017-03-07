import numpy as np
import math


def euclidian_distance(p1, p2):
    return math.sqrt(math.pow(p1[0] - p2[0], 2) + math.pow(p1[1] - p2[1], 2))


def pad_to_square(a, pad_value=0):
    m = a.reshape((a.shape[0], -1))
    padded = pad_value * np.ones(2 * [max(m.shape)], dtype=m.dtype)
    padded[0:m.shape[0], 0:m.shape[1]] = m
    return padded


def distances_between_detections(old_detections, new_detections):
    mat = np.zeros((len(old_detections), len(new_detections)))
    for old_id, old_detection in enumerate(old_detections):
        for new_id, new_detection in enumerate(new_detections):
            mat[old_id, new_id] = euclidian_distance(old_detection.pos, new_detection.pos)
    return pad_to_square(mat)
