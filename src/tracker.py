import numpy as np
import cv2
from munkres import Munkres, print_matrix

from detection import Detection
from detector import Detector
from semantic_segmentation_detector import SemanticSegmentationDetector
from util import distances_between_detections


class Tracker:
    def __init__(self, detector):
        self.detector = detector
        self.state = None
        self.previous_detections = None
        self.m = Munkres()

    def unique_classes(self, predictions):
        classes = set()
        for prediction in predictions:
            classes.add(prediction.class_name)
        return classes

    def track(self, image):
        new_detections = self.detector.detect(image)
        if self.previous_detections:
            classes = self.unique_classes(self.previous_detections + new_detections)
            for class_name in classes:
                print(class_name)
                previous_class_detections = [det for det in self.previous_detections if det.class_name == class_name]
                new_class_detections = [det for det in new_detections if det.class_name == class_name]
                distance_matrix = distances_between_detections(previous_class_detections, new_class_detections)
                print(distance_matrix.shape)
                indices = self.m.compute(distance_matrix)
                print(f"Best indices are {indices}")

                # Very unclear if this works as intended. Needs checking
                new_to_old = {i[1]: i[0] for i in indices if
                              i[0] < len(previous_class_detections) and
                              i[1] < len(new_class_detections)}
                for detection in new_class_detections:
                    nr = detection.obj_id
                    if nr in new_to_old.keys():
                        detection.obj_id = previous_class_detections[new_to_old[nr]].obj_id
                    else:
                        detection.obj_id = 100

        self.previous_detections = new_detections
        return self.previous_detections

    def test(self, input_file, output_file):
        image = cv2.imread(input_file)
        detections = self.track(image)
        for d in detections:
            cv2.putText(image, f"{d.class_name} {d.obj_id}", (d.pos[0], d.pos[1]), fontFace=0, fontScale=1,
                        color=(0, 255, 0), thickness=2)
        cv2.imwrite(output_file, image)


t = Tracker(SemanticSegmentationDetector())
