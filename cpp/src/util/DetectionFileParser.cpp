#include "DetectionFileParser.h"

#include <sstream>

// Functions

std::map<int, std::vector<Detection>> DetectionFileParser::parseMOTFile(std::ifstream &file) {
    return parseFile(file, DetectionFileParser::parseMOTLine);
}

std::map<int, std::vector<Detection>> DetectionFileParser::parseOkutamaFile(std::ifstream &file) {
    return parseFile(file, DetectionFileParser::parseOkutamaLine);
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

std::pair<int, Detection> DetectionFileParser::parseMOTLine(const std::string &line) {
    std::istringstream is(line);
    int frame;
    int id; // Unused
    double x1, y1, width, height, confidence;
    double x, y, z; // Unused
    int label = 1;
    if (!(is >> frame && is.ignore() &&
          is >> id && is.ignore() &&
          is >> x1 && is.ignore() &&
          is >> y1 && is.ignore() &&
          is >> width && is.ignore() &&
          is >> height && is.ignore() &&
          is >> confidence && is.ignore() &&
          is >> x && is.ignore() &&
          is >> y && is.ignore() &&
          is >> z)) {
        throw std::invalid_argument(
                "Each line must be on following format: "
                        "<frame>,<id>,<x_topleft>,<y_topleft>,<width>,<height>,<confidence>,<x>,<y>,<z>");
    }
    return std::pair<int, Detection>(frame, Detection(label, confidence,
                                                      BoundingBox(x1 + width / 2.0, y1 + height / 2.0, width, height)));
}

std::pair<int, Detection> DetectionFileParser::parseOkutamaLine(const std::string &line) {
    std::istringstream is(line);
    int frame, label;
    int id; // Unused
    double x1, y1, width, height, confidence;
    double x, y, z; // Unused
    if (!(is >> frame && is.ignore() &&
          is >> label && is.ignore() &&
          is >> id && is.ignore() &&
          is >> x1 && is.ignore() &&
          is >> y1 && is.ignore() &&
          is >> width && is.ignore() &&
          is >> height && is.ignore() &&
          is >> confidence && is.ignore() &&
          is >> x && is.ignore() &&
          is >> y && is.ignore() &&
          is >> z)) {
        throw std::invalid_argument(
                "Each line must be on following format: "
                        "<frame>,<label>,<id>,<x_topleft>,<y_topleft>,<width>,<height>,<confidence>,<x>,<y>,<z>");
    }
    return std::pair<int, Detection>(frame, Detection(label, confidence,
                                                      BoundingBox(x1 + width / 2.0, y1 + height / 2.0, width, height)));
}