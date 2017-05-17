#include "../tracker/PAOT.h"
#include "../tracker/predictor/kalman/KalmanPredictor.h"
#include "../tracker/predictor/particle/ParticlePredictor.h"
#include "../tracker/predictor/StationaryPredictor.h"
#include "../util/Affinity.h"
#include "../util/DetectionFileParser.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <chrono>

const boost::filesystem::path dataDirPath = boost::filesystem::current_path().parent_path() / "data";

const char *USAGE_MESSAGE = "Usage: %s "
        "-s sequenceMap "
        "-m modelType "
        "-f detectionFormat (okutama, mot) "
        "-p predictorType (kalman, particle) "
        "[-i frameInterval (default 1)] "
        "[-x (overwrites result)]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";
const char *FILE_EXISTS_MESSAGE = "Output file %s already exists; won't overwrite\n";

std::pair<std::chrono::duration<double, std::milli>, int> track(const boost::filesystem::path &sequencePath,
                                                                const std::string &modelType,
                                                                const std::string &detectionFormat,
                                                                const std::string &predictorType,
                                                                const int frameInterval,
                                                                const bool doOverwriteResult) {
    typedef std::chrono::duration<double, std::milli> msduration;

    // Make sure input file exists
    boost::filesystem::path inputPath = dataDirPath / sequencePath / modelType / "det.txt";
    std::ifstream inputStream(inputPath.string());
    if (!inputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, inputPath.c_str());
        return std::pair<msduration, int>(msduration(0), 0);
    }

    // Create output directory if not exists
    boost::filesystem::path outputDirPath = dataDirPath / "results" / sequencePath.parent_path() / modelType;
    if (!boost::filesystem::is_directory(outputDirPath)) {
        boost::filesystem::create_directories(outputDirPath);
    }

    // Make sure output file does not exist
    boost::filesystem::path outputPath = outputDirPath / (sequencePath.filename().string() + ".txt");
    if (boost::filesystem::exists(outputPath) && !doOverwriteResult) {
        fprintf(stderr, FILE_EXISTS_MESSAGE, outputPath.c_str());
        return std::pair<msduration, int>(msduration(0), 0);
    }

    // Make sure output file can be opened
    std::ofstream outputStream;
    outputStream.open(outputPath.string());
    if (!outputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, outputPath.c_str());
        return std::pair<msduration, int>(msduration(0), 0);
    }

    // Create tracker
    std::shared_ptr<Tracker> tracker;
    if (predictorType == "kalman") {
        tracker = std::make_shared<PAOT<KalmanPredictor>>(2, 0, 0.4, 0.3, Affinity::iou);
    } else if (predictorType == "particle") {
        tracker = std::make_shared<PAOT<ParticlePredictor>>(2, 0, 0.4, 0.3, Affinity::iou);
    } else {
        throw std::invalid_argument(predictorType + " not a valid predictorType");
    }

    // Parse detections
    std::map<int, std::vector<Detection>> (*parseFileFunc)(std::ifstream &file);
    if (detectionFormat == "okutama") {
        parseFileFunc = DetectionFileParser::parseOkutamaFile;
    } else if (detectionFormat == "mot") {
        parseFileFunc = DetectionFileParser::parseMOTFile;
    } else {
        throw std::invalid_argument(detectionFormat + " not a valid detectionFormat");
    }
    const auto frameToDetections = parseFileFunc(inputStream);

    // Loop through all frames
    msduration cumulativeDuration = std::chrono::milliseconds::zero();
    int frameCount = 0;
    for (int frame = 0; frame < frameToDetections.rbegin()->first; ++frame) {
        if (frame % frameInterval == 0 && frameToDetections.find(frame) != frameToDetections.end()) {
            auto startTime = std::chrono::high_resolution_clock::now();
            std::vector<Tracking> trackings = tracker->track(frameToDetections.at(frame));
            auto endTime = std::chrono::high_resolution_clock::now();

            cumulativeDuration += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                    endTime - startTime);

            for (auto trackingIt = trackings.begin(); trackingIt != trackings.end(); ++trackingIt) {
                outputStream << frame << ","
                             << trackingIt->ID << ","
                             << trackingIt->bb.x1() << ","
                             << trackingIt->bb.y1() << ","
                             << trackingIt->bb.width << ","
                             << trackingIt->bb.height << ","
                             << "1" << "," // Confidence
                             << "-1,-1,-1" << "," // Unused
                             << trackingIt->label << "\n";
            }
            ++frameCount;
        }
    }
    outputStream.close();
    return std::pair<msduration, int>(cumulativeDuration, frameCount);
}

int main(int argc, char **argv) {

    std::string sequenceMapName;
    std::string modelType;
    std::string detectionFormat;
    std::string predictorType;
    int frameInterval = 1;
    bool doOverwriteResult = false;

    int opt;
    while ((opt = getopt(argc, argv, "s:m:f:p:i:x")) != -1) {
        switch (opt) {
            case 's':
                sequenceMapName = optarg;
                break;
            case 'm':
                modelType = optarg;
                break;
            case 'f':
                detectionFormat = optarg;
                break;
            case 'p':
                predictorType = optarg;
                break;
            case 'i':
                frameInterval = atoi(optarg);
                break;
            case 'x':
                doOverwriteResult = true;
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (sequenceMapName == "" || modelType == "" || detectionFormat == "" || predictorType == "") {
        fprintf(stderr, USAGE_MESSAGE, argv[0]);
        exit(EXIT_FAILURE);
    }
    if (frameInterval < 1) {
        fprintf(stderr, "frameInterval must be a positive integer");
        exit(EXIT_FAILURE);
    }

    boost::filesystem::path sequenceMapPath = dataDirPath / "seqmaps" / sequenceMapName;
    std::ifstream sequenceMap(sequenceMapPath.string());
    if (sequenceMap.is_open()) {

        std::chrono::duration<double, std::milli> cumulativeDuration;
        int cumulativeFrameCount = 0;

        std::string sequencePathString;
        while (getline(sequenceMap, sequencePathString)) {
            std::cout << "Sequence: " << sequencePathString << std::endl;
            auto durationFrameCount = track(sequencePathString, modelType, detectionFormat, predictorType,
                                            frameInterval, doOverwriteResult);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(durationFrameCount.first).count();
            std::cout << "Duration: " << duration << "ms"
                      << " (" << double(durationFrameCount.second * 1000) / duration << "fps)\n";
            cumulativeDuration += durationFrameCount.first;
            cumulativeFrameCount += durationFrameCount.second;
        }
        sequenceMap.close();
        auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(cumulativeDuration).count();
        std::cout << "Total duration: " << double(totalDuration) / 1000 << "s"
                  << " (" << double(cumulativeFrameCount * 1000) / totalDuration << "fps)\n";
    } else {
        fprintf(stderr, OPEN_FILE_MESSAGE, sequenceMapName.c_str());
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}