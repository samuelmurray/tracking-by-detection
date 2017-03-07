import sys
import os.path
import numpy as np
import cv2

from detection import Detection
from detector import Detector

"""
All classes and corresponding colours used in the segmented images
classes = Background, Structures, Buildings, Pavement, Non-paved ground, Train tracks, Plants, Vehicles, Water, People
colours = np.array([[0,0,0],[237,237,237],[0,0,181],[135,135,135],[0,107,189],[128,0,128],[22,123,31],[130,0,6],[255,168,0],[0,255,240]])
"""


class SemanticSegmentationDetector(Detector):
    CLASS_AREA_THRESHOLD = {"car": 400,
                            "building": 500,
                            "people": 10}
    CLASS_COLOURS = {"car": [130, 0, 6],
                     "building": [0, 0, 181],
                     "people": [0, 255, 240]}

    def __init__(self):
        super(SemanticSegmentationDetector, self).__init__()

    def find_contours_of_class(self, image, class_name):
        # Create a grayscale copy with only one class left
        filtered_image = image.copy()
        filtered_image[np.where((image != self.CLASS_COLOURS[class_name]).all(axis=2))] = [0, 0, 0]
        filtered_image[np.where((image == self.CLASS_COLOURS[class_name]).all(axis=2))] = [255, 255, 255]
        grayscale_image = cv2.cvtColor(filtered_image, cv2.COLOR_RGB2GRAY)

        # Find contours
        ret, thresh = cv2.threshold(grayscale_image, 254, 255, 0)
        _, contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        contours_filtered_by_area = [cnt for cnt in contours if
                                     cv2.contourArea(cnt) >= self.CLASS_AREA_THRESHOLD[class_name]]
        print(f"Found {len(contours_filtered_by_area)} objects for class \"{class_name}\"" +
              f" of area at least {self.CLASS_AREA_THRESHOLD[class_name]}.")
        return contours_filtered_by_area

    @staticmethod
    def find_center_of_contour(contour):
        moments = cv2.moments(contour)
        if moments['m00'] == 0:
            print("zero moment!")
            return
        x = int(moments['m10'] / moments['m00'])
        y = int(moments['m01'] / moments['m00'])
        return x, y

    def localize_and_highlight_objects(self, image):
        image_with_highlights = image.copy()
        detections = []
        for class_name, colour in self.CLASS_COLOURS.items():
            inverse_colour = (255 - colour[0],
                              255 - colour[1],
                              255 - colour[2])
            contours = self.find_contours_of_class(image, class_name)
            for obj_id, contour in enumerate(contours):
                x_pos, y_pos = self.find_center_of_contour(contour)
                detections.append(Detection(class_name, obj_id, (x_pos, y_pos)))
                cv2.drawContours(image_with_highlights, [contour], 0, color=inverse_colour, thickness=3)
                cv2.circle(image_with_highlights, (x_pos, y_pos), radius=5, color=inverse_colour, thickness=8)
                cv2.putText(image_with_highlights, f"{class_name} {obj_id}", (x_pos - 40, y_pos + 40), fontFace=0,
                            fontScale=1, color=(0, 255, 0), thickness=2)
        return detections, image_with_highlights

    def detect(self, image):
        detections = []
        for class_name, colour in self.CLASS_COLOURS.items():
            contours = self.find_contours_of_class(image, class_name)
            for obj_id, contour in enumerate(contours):
                x_pos, y_pos = self.find_center_of_contour(contour)
                detections.append(Detection(class_name, obj_id, (x_pos, y_pos)))
        return detections


if __name__ == "__main__":
    args = sys.argv
    assert (len(args) > 1), "You need to to pass a path to input image"
    assert (len(args) < 4), "You can at most pass 2 additional arguments"
    assert (os.path.isfile(args[1])), f"Input file \"{args[1]}\" doesn't exist"
    image = cv2.imread(args[1])
    assert (image is not None), f"Input file \"{args[1]}\" can't be read as an image"

    det = SemanticSegmentationDetector()
    if len(args) == 2:
        detections = det.detect(image)
    else:
        output_path = args[2]
        assert (not os.path.isfile(output_path)), f"Output file \"{args[2]}\" already exists. Won't overwrite!"
        detections, output = det.localize_and_highlight_objects(image)
        cv2.imwrite(output_path, output)
    print(detections)
