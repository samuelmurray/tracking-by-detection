#include <sstream>
#include <iostream>
#include "DetectionFileParser.h"

using std::vector;
using std::string;

// Constructor

DetectionFileParser::DetectionFileParser(std::ifstream &file)
        : Detector() {
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            std::pair<int, Detection> frameDet = DetectionFileParser::parseLine(line);
            frameToDetections[frameDet.first].push_back(frameDet.second);
        }
        file.close();
    }
}

// Methods

vector<Detection> DetectionFileParser::detect() {
    if (frame > frameToDetections.rbegin()->first) {
        throw "Out of frames";
    }
    while (frameToDetections.find(++frame) == frameToDetections.end()) {
        if (frame > frameToDetections.rbegin()->first) {
            throw "Out of frames";
        }
    }
    return frameToDetections[frame];
}

std::pair<int, Detection> DetectionFileParser::parseLine(const string &line) {
    std::istringstream iss(line);
    int frame, cx, cy, width, height;
    string className;
    iss >> frame >> className >> cx >> cy >> width >> height;
    return std::pair<int, Detection>(frame, Detection(className, BoundingBox(cx, cy, width, height)));
}