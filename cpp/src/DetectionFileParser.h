#ifndef CPP_DETECTIONFILEPARSER_H
#define CPP_DETECTIONFILEPARSER_H


#include "Detector.h"

#include <fstream>
#include <map>

class DetectionFileParser {
public:
    static std::map<int, std::vector<Detection>> parseFile(std::ifstream &file);

    static std::pair<int, Detection> parseLine(const std::string &line);

private:
    // Prevent instantiation
    DetectionFileParser() {};
};


#endif //CPP_DETECTIONFILEPARSER_H