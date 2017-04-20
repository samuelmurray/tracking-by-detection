#ifndef CPP_DETECTIONFILEPARSER_H
#define CPP_DETECTIONFILEPARSER_H


#include "../detector/Detector.h"

#include <fstream>
#include <map>

class DetectionFileParser {
public:
    static std::map<int, std::vector<Detection>> parseMOTFile(std::ifstream &file);

    static std::map<int, std::vector<Detection>> parseOkutamaFile(std::ifstream &file);

private:
    // Prevent instantiation
    DetectionFileParser() {};

    static std::map<int, std::vector<Detection>> parseFile(
            std::ifstream &file, std::pair<int, Detection> (*parseLineFunc)(const std::string &line));

    static std::pair<int, Detection> parseMOTLine(const std::string &line);

    static std::pair<int, Detection> parseOkutamaLine(const std::string &line);
};


#endif //CPP_DETECTIONFILEPARSER_H