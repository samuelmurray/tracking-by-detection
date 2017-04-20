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

const char *USAGE_MESSAGE = "Usage: %s [-s sequencesFile] [-m modelType]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";
const char *FILE_EXISTS_MESSAGE = "Output file %s already exists; won't overwrite\n";

std::pair<std::chrono::duration<double, std::milli>, int> track(const boost::filesystem::path &sequencePath,
                                                                const std::string &modelType) {
    typedef std::chrono::duration<double, std::milli> msduration;

    // Make sure input file exists
    boost::filesystem::path inputPath = dataDirPath / sequencePath / modelType / "det.txt";
    std::ifstream inputStream(inputPath.string());
    if (!inputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, inputPath.c_str());
        exit(EXIT_FAILURE);
    }

    // Create output directory if not exists
    boost::filesystem::path outputDirPath = dataDirPath / "results" / sequencePath / modelType;
    if (!boost::filesystem::is_directory(outputDirPath)) {
        boost::filesystem::create_directories(outputDirPath);
    }

    // Make sure output file does not exist
    boost::filesystem::path outputPath = outputDirPath / "track.txt";
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

    msduration cumulativeDuration = std::chrono::milliseconds::zero();
    int frameCount = 0;
    for (const auto &detMap : DetectionFileParser::parseMOTFile(inputStream)) {

        auto startTime = std::chrono::high_resolution_clock::now();
        std::vector<Tracking> trackings = tracker.track(detMap.second);
        auto endTime = std::chrono::high_resolution_clock::now();

        cumulativeDuration += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                endTime - startTime);

        for (auto trackingIt = trackings.begin(); trackingIt != trackings.end(); ++trackingIt) {
            outputStream << detMap.first << ","
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
    outputStream.close();
    return std::pair<msduration, int>(cumulativeDuration, frameCount);
}

int main(int argc, char **argv) {

    std::string sequencesFileName;
    std::string modelType;

    int opt;
    while ((opt = getopt(argc, argv, "s:m:")) != -1) {
        switch (opt) {
            case 's':
                sequencesFileName = optarg;
                break;
            case 'm':
                modelType = optarg;
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

    boost::filesystem::path sequencesFilePath = dataDirPath / "config" / sequencesFileName;
    std::ifstream sequencesFile(sequencesFilePath.string());
    if (sequencesFile.is_open()) {

        std::chrono::duration<double, std::milli> cumulativeDuration;
        int cumulativeFrameCount = 0;

        std::string sequencePathString;
        while (getline(sequencesFile, sequencePathString)) {
            std::cout << "Sequence: " << sequencePathString << std::endl;
            auto durationFrameCount = track(sequencePathString, modelType);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(durationFrameCount.first).count();
            std::cout << "Duration: " << duration << "ms"
                      << " (" << double(durationFrameCount.second * 1000) / duration << "fps)\n";
            cumulativeDuration += durationFrameCount.first;
            cumulativeFrameCount += durationFrameCount.second;
        }
        sequencesFile.close();
        auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(cumulativeDuration).count();
        std::cout << "Total duration: " << totalDuration << "ms"
                  << " (" << double(cumulativeFrameCount * 1000) / totalDuration << "fps)\n";
    } else {
        fprintf(stderr, OPEN_FILE_MESSAGE, sequencesFileName.c_str());
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}