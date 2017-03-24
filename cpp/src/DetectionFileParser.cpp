#include "DetectionFileParser.h"

#include <sstream>
#include <iostream>

using std::vector;
using std::string;

// Functions

std::map<int, vector<Detection>> DetectionFileParser::parseFile(std::ifstream &file) {
    string line;
    std::map<int, vector<Detection>> frameToDetections;
    if (file.is_open()) {
        while (getline(file, line)) {
            std::pair<int, Detection> frameDet = DetectionFileParser::parseLine(line);
            frameToDetections[frameDet.first].push_back(frameDet.second);
        }
        file.close();
    }
    return frameToDetections;
}

std::pair<int, Detection> DetectionFileParser::parseLine(const string &line) {
    std::istringstream iss(line);
    int frame, cx, cy, width, height;
    string className;
    if (!(iss >> frame >> className >> cx >> cy >> width >> height)) {
        throw std::invalid_argument("Each line must be on format [frame] [class] [cx] [cy] [width] [height]");
    }
    return std::pair<int, Detection>(frame, Detection(className, BoundingBox(cx, cy, width, height)));
}