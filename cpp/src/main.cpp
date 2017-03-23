#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <dlib/matrix.h>
#include <dlib/optimization.h>

#include "Tracker.h"
#include "RandomTracker.h"
#include "MCSORT.h"

using std::cout;
using std::endl;

void tracking() {
    using namespace cv;
    Mat image;
    std::string imagePath;

    std::cout << "Give the path to an image, or type '0' to exit\n";
    Tracker *tracker = new MCSORT();
    while (std::cin >> imagePath) {
        if (imagePath == "0") {
            return;
        }

        image = imread(imagePath, 1);
        if (!image.data) {
            std::cout << "Could not load image " << imagePath << std::endl;
        } else {
            std::vector<Tracking> ret = tracker->track(image);
            for (auto a : ret) {
                std::cout << a << std::endl;
            }
        }
        std::cout << "\nGive more images, or type '0' to exit\n";
    }
}

void tracking2() {
    using namespace cv;
    std::string imagePath = "../data/test.png";
    Mat image = imread(imagePath, 1);
    Tracker *tracker = new MCSORT();
    if (!image.data) {
        std::cout << "Could not load image " << imagePath << std::endl;
    } else {
        std::vector<Tracking> trackings;
        for (int i = 0; i < 5; ++i) {
            trackings = tracker->track(image);
            std::cout << "---TRACKINGS---" << std::endl;
            for (auto a : trackings) {
                std::cout << a << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

void bb() {
    BoundingBox a(5, 5, 10, 8);
    std::cout << a.x1() << "," << a.y1() << "-" << a.x2() << "," << a.y2() << std::endl;
    BoundingBox b(5, 6, 9, 2);
    std::cout << BoundingBox::iou(a, b);
}

int main(int argc, char **argv) {
    tracking2();
    return 0;
}