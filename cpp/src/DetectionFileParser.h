#ifndef CPP_DETECTIONFILEPARSER_H
#define CPP_DETECTIONFILEPARSER_H


#include <fstream>
#include <map>
#include "Detector.h"

class DetectionFileParser : public Detector {
public:
    DetectionFileParser(std::ifstream &file);

    std::vector<Detection> detect() override;

private:
    static std::pair<int, Detection> parseLine(const std::string &line);

    std::map<int, std::vector<Detection>> frameToDetections;
    int frame = 0;
};


#endif //CPP_DETECTIONFILEPARSER_H