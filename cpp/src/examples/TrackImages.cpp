#include "../tracker/Tracker.h"
#include "../tracker/mcsort/MCSORT.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void tracking1() {
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
            vector<Tracking> trackings = tracker->track(image);
            for (auto it = trackings.begin(); it != trackings.end(); ++it) {
                cout << *it << endl;
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
            for (auto it = trackings.begin(); it != trackings.end(); ++it) {
                cout << *it << endl;
            }
            cout << endl;
        }
    }
}

int main() {
    tracking1();
    return 0;
}