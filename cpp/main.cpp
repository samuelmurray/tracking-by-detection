#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "Tracker.h"
#include "NaiveTracker.h"
#include "tf_demo.h"

void tracking() {
    using namespace cv;
    Mat image;
    std::string imagePath;

    std::cout << "Give the path to an image, or type '0' to exit\n";
    while (std::cin >> imagePath) {
        if (imagePath == "0") {
            return;
        }

        image = imread(imagePath, 1);
        if (!image.data) {
            std::cout << "Could not load image " << imagePath << std::endl;
        } else {
            Tracker *tracker = new NaiveTracker();
            std::vector<Detection> ret = tracker->track(image);
            for (auto a : ret) {
                std::cout << a << std::endl;
            }
        }
        std::cout << "\nGive more images, or type '0' to exit\n";
    }
}

int main(int argc, char **argv) {
    //tracking();
    //return 0;
    return demo();
}