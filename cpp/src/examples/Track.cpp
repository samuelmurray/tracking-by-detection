#include "../tracker/Tracker.h"
#include "../tracker/mcsort/MCSORT.h"
#include "../util/DetectionFileParser.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <chrono>

const boost::filesystem::path dataDirPath = boost::filesystem::current_path().parent_path() / "data";

const char *USAGE_MESSAGE = "Usage: %s "
        "[-s sequencesFile] "
        "[-m modelType] "
        "[-d dataType (default okutama)] "
        "[-i frameInterval (default 1)]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";
const char *FILE_EXISTS_MESSAGE = "Output file %s already exists; won't overwrite\n";

std::pair<std::chrono::duration<double, std::milli>, int> track(const boost::filesystem::path &sequencePath,
                                                                const std::string &modelType,
                                                                const std::string &dataType,
                                                                const int frameInterval) {
    typedef std::chrono::duration<double, std::milli> msduration;

    // Make sure input file exists
    boost::filesystem::path inputPath = dataDirPath / sequencePath / modelType / "det.txt";
    std::ifstream inputStream(inputPath.string());
    if (!inputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, inputPath.c_str());
        exit(EXIT_FAILURE);
    }

    // Create output directory if not exists
    boost::filesystem::path outputDirPath = dataDirPath / "results" / sequencePath.parent_path() / modelType;
    if (!boost::filesystem::is_directory(outputDirPath)) {
        boost::filesystem::create_directories(outputDirPath);
    }

    // Make sure output file does not exist
    boost::filesystem::path outputPath = outputDirPath / (sequencePath.filename().string() + ".txt");
    if (boost::filesystem::exists(outputPath)) {
        fprintf(stderr, FILE_EXISTS_MESSAGE, outputPath.c_str());
        return std::pair<msduration, int>(msduration(0), 0);
    }

    // Make sure output file can be opened
    std::ofstream outputStream;
    outputStream.open(outputPath.string());
    if (!outputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, outputPath.c_str());
        exit(EXIT_FAILURE);
    }

    MCSORT tracker;

    std::map<int, std::vector<Detection>> (*parseFileFunc)(std::ifstream &file);
    if (dataType == "okutama") {
        parseFileFunc = DetectionFileParser::parseOkutamaFile;
    } else if (dataType == "mot") {
        parseFileFunc = DetectionFileParser::parseMOTFile;
    } else {
        throw std::invalid_argument(dataType + " not a valid dataType");
    }
    const auto frameToDetections = parseFileFunc(inputStream);

    msduration cumulativeDuration = std::chrono::milliseconds::zero();
    int frameCount = 0;
    for (int frame = 0; frame < frameToDetections.rbegin()->first; ++frame) {
        if (frame % frameInterval == 0 && frameToDetections.find(frame) != frameToDetections.end()) {
            auto startTime = std::chrono::high_resolution_clock::now();
            std::vector<Tracking> trackings = tracker.track(frameToDetections.at(frame));
            auto endTime = std::chrono::high_resolution_clock::now();

            cumulativeDuration += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                    endTime - startTime);

            for (auto trackingIt = trackings.begin(); trackingIt != trackings.end(); ++trackingIt) {
                outputStream << frame << ","
                             << trackingIt->label << ","
                             << trackingIt->ID << ","
                             << trackingIt->bb.x1() << ","
                             << trackingIt->bb.y1() << ","
                             << trackingIt->bb.width << ","
                             << trackingIt->bb.height << ","
                             << "1,-1,-1,-1\n";
            }
            ++frameCount;
        }
    }
    outputStream.close();
    return std::pair<msduration, int>(cumulativeDuration, frameCount);
}

int main(int argc, char **argv) {

    std::string sequencesFileName;
    std::string modelType;
    std::string dataType = "okutama";
    int frameInterval = 1;

    int opt;
    while ((opt = getopt(argc, argv, "s:m:d:i:")) != -1) {
        switch (opt) {
            case 's':
                sequencesFileName = optarg;
                break;
            case 'm':
                modelType = optarg;
                break;
            case 'd':
                dataType = optarg;
                break;
            case 'i':
                frameInterval = atoi(optarg);
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (sequencesFileName == "" || modelType == "") {
        fprintf(stderr, USAGE_MESSAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    if (frameInterval < 1) {
        fprintf(stderr, "frameInterval must be a positive integer");
        exit(EXIT_FAILURE);
    }

    boost::filesystem::path sequencesFilePath = dataDirPath / "seqmaps" / sequencesFileName;
    std::ifstream sequencesFile(sequencesFilePath.string());
    if (sequencesFile.is_open()) {

        std::chrono::duration<double, std::milli> cumulativeDuration;
        int cumulativeFrameCount = 0;

        std::string sequencePathString;
        while (getline(sequencesFile, sequencePathString)) {
            std::cout << "Sequence: " << sequencePathString << std::endl;
            auto durationFrameCount = track(sequencePathString, modelType, dataType, frameInterval);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(durationFrameCount.first).count();
            std::cout << "Duration: " << duration << "ms"
                      << " (" << double(durationFrameCount.second * 1000) / duration << "fps)\n";
            cumulativeDuration += durationFrameCount.first;
            cumulativeFrameCount += durationFrameCount.second;
        }
        sequencesFile.close();
        auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(cumulativeDuration).count();
        std::cout << "Total duration: " << double(totalDuration) / 1000 << "s"
                  << " (" << double(cumulativeFrameCount * 1000) / totalDuration << "fps)\n";
    } else {
        fprintf(stderr, OPEN_FILE_MESSAGE, sequencesFileName.c_str());
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}