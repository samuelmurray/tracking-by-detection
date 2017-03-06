from detection import Detection


class Detector(object):
    def __init__(self):
        self.count = 0
        pass

    def detect(self, image):
        detections = []
        if self.count == 0:
            detections.append(Detection("test", 0, (0, 0)))
            detections.append(Detection("test", 1, (3, 3)))
        elif self.count == 1:
            detections.append(Detection("test", 0, (1, 0)))
            detections.append(Detection("test", 1, (3, 4)))
        elif self.count == 2:
            detections.append(Detection("test", 1, (0, 1)))
            detections.append(Detection("test", 0, (5, 4)))
        self.count += 1
        return detections
