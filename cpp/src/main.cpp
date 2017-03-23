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
#include "DetectionFileParser.h"


using namespace std;

/*
using std::vector
using std::cout;
using std::cin;
using std::endl;
using std::string;
 */

void tracking() {
    using namespace cv;
    Mat image;
    string imagePath;

    cout << "Give the path to an image, or type '0' to exit\n";
    Tracker *tracker = new MCSORT();
    while (cin >> imagePath) {
        if (imagePath == "0") {
            return;
        }

        image = imread(imagePath, 1);
        if (!image.data) {
            cout << "Could not load image " << imagePath << endl;
        } else {
            vector<Tracking> ret = tracker->track(image);
            for (auto a : ret) {
                cout << a << endl;
            }
        }
        cout << "\nGive more images, or type '0' to exit\n";
    }
}

void tracking2() {
    using namespace cv;
    string imagePath = "../data/test.png";
    Mat image = imread(imagePath, 1);
    if (!image.data) {
        cout << "Could not load image " << imagePath << endl;
    } else {
        MCSORT tracker;
        vector<Tracking> trackings;
        for (int i = 0; i < 5; ++i) {
            trackings = tracker.track(image);
            cout << "---TRACKINGS---" << endl;
            for (auto a : trackings) {
                cout << a << endl;
            }
            cout << endl;
        }
    }
}

void tracking3() {
    string filePath = "../data/detections.txt";
    ifstream detectionFile(filePath);
    if (!detectionFile.is_open()) {
        cout << "Could not load file " << filePath << endl;
    } else {
        MCSORT tracker(std::make_shared<Detector>(DetectionFileParser(detectionFile)));
        vector<Tracking> trackings;
        for (int i = 0; i < 5; ++i) {
            trackings = tracker.track(cv::Mat()); // FIXME: We need to input detections here instead
            cout << "---TRACKINGS---" << endl;
            for (auto a : trackings) {
                cout << a << endl;
            }
            cout << endl;
        }
    }
}


int main(int argc, char **argv) {
    tracking3();
    return 0;
}