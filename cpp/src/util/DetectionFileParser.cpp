#include "DetectionFileParser.h"

#include <sstream>

// Functions

std::map<int, std::vector<Detection>> DetectionFileParser::parseFile(std::ifstream &file) {
    return parseFile(file, DetectionFileParser::parseLine);
}

std::map<int, std::vector<Detection>> DetectionFileParser::parseMOTFile(std::ifstream &file) {
    return parseFile(file, DetectionFileParser::parseMOTLine);
}

std::map<int, std::vector<Detection>> DetectionFileParser::parseFile(
        std::ifstream &file,
        std::pair<int, Detection> (*parseLineFunc)(const std::string &)) {
    std::string line;
    std::map<int, std::vector<Detection>> frameToDetections;
    if (file.is_open()) {
        while (getline(file, line)) {
            std::pair<int, Detection> frameDet = parseLineFunc(line);
            frameToDetections[frameDet.first].push_back(frameDet.second);
        }
        file.close();
    }
    return frameToDetections;
}

std::pair<int, Detection> DetectionFileParser::parseLine(const std::string &line) {
    std::istringstream iss(line);
    int frame, cx, cy, width, height;
    std::string className;
    if (!(iss >> frame >> className >> cx >> cy >> width >> height)) {
        throw std::invalid_argument(
                "Each line must be on following format: "
                        "<frame>,<class>,<cx>,<cy>,<width>,<height>");
    }
    return std::pair<int, Detection>(frame, Detection(className, BoundingBox(cx, cy, width, height)));
}

std::pair<int, Detection> DetectionFileParser::parseMOTLine(const std::string &line) {
    std::istringstream is(line);
    int frame, x1, y1, width, height;
    int id, conf, x, y, z; // Unused
    std::string className = "person";
    if (!(is >> frame && is.ignore() &&
            is >> id && is.ignore() &&
            is >> x1 && is.ignore() &&
            is >> y1 && is.ignore() &&
            is >> width && is.ignore() &&
            is >> height && is.ignore() &&
            is >> conf && is.ignore() &&
            is >> x && is.ignore() &&
            is >> y && is.ignore() &&
            is >> z)) {
        throw std::invalid_argument(
                "Each line must be on following format: "
                        "<frame>,<id>,<x_topleft>,<y_topleft>,<width>,<height>,<conf>,<x>,<y>,<z>");
    }
    return std::pair<int, Detection>(frame, Detection(
            className, BoundingBox(x1 + width / 2.0, y1 + height / 2.0, width, height)));
}