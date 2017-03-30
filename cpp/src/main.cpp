#include "tracker/Tracker.h"
#include "tracker/RandomTracker.h"
#include "tracker/mcsort/MCSORT.h"
#include "util/DetectionFileParser.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

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

void tracking3() {
    string filePath = "../data/detections.txt";
    ifstream detectionFile(filePath);
    if (!detectionFile.is_open()) {
        cout << "Could not load file " << filePath << endl;
    } else {
        MCSORT tracker;
        vector<Tracking> trackings;
        for (auto const &detMap : DetectionFileParser::parseFile(detectionFile)) {
            cout << "NEW FRAME - " << detMap.first << endl;
            trackings = tracker.track(detMap.second);
            cout << "---TRACKINGS---" << endl;
            for (auto it = trackings.begin(); it != trackings.end(); ++it) {
                cout << *it << endl;
            }
            cout << endl;
        }
    }
}

void tracking4() {
    //string filePath = "../data/2DMOT2015/data/train/ADL-Rundle-6/gt/gt.txt";
    string filePath = "../data/motdet.txt";
    //string filePath = "../data/mot.txt";
    ifstream detectionFile(filePath);
    if (!detectionFile.is_open()) {
        cout << "Could not load file " << filePath << endl;
    } else {
        ofstream output;
        output.open ("output.txt");
        MCSORT tracker;
        vector<Tracking> trackings;
        for (auto const &detMap : DetectionFileParser::parseMOTFile(detectionFile)) {
            cout << "NEW FRAME - " << detMap.first << endl;
            trackings = tracker.track(detMap.second);
            cout << "---TRACKINGS---" << endl;
            for (auto it = trackings.begin(); it != trackings.end(); ++it) {
                cout << *it << endl;
                output << detMap.first << ","
                        << it->ID << ","
                        << it->bb.x1() << ","
                        << it->bb.y1() << ","
                        << it->bb.width << ","
                        << it->bb.height << ","
                        << "1,-1,-1,-1\n";
            }
            cout << endl;
        }
        output.close();
    }
}


int main(int argc, char **argv) {
    tracking4();
    return 0;
}