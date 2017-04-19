#include "../tracker/Tracker.h"
#include "../tracker/mcsort/MCSORT.h"
#include "../util/DetectionFileParser.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <chrono>

const char *USAGE_MESSAGE = "Usage: %s [-d configFile]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";

std::pair<std::chrono::duration<double, std::milli>, int> track(const boost::filesystem::path &inputPath,
                                                                const boost::filesystem::path &outputPath) {
    std::ifstream inputStream(inputPath.string());
    if (!inputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, inputPath.c_str());
        exit(EXIT_FAILURE);
    }
    std::ofstream outputStream;
    outputStream.open(outputPath.string());
    if (!outputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, outputPath.c_str());
        exit(EXIT_FAILURE);
    }

    MCSORT tracker;
    std::vector<Tracking> trackings;
    std::chrono::duration<double, std::milli> cumulativeDuration = std::chrono::milliseconds::zero();
    int frameCount = 0;
    for (const auto &detMap : DetectionFileParser::parseMOTFile(inputStream)) {

        auto startTime = std::chrono::high_resolution_clock::now();
        trackings = tracker.track(detMap.second);
        auto endTime = std::chrono::high_resolution_clock::now();

        cumulativeDuration += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                endTime - startTime);

        for (auto it = trackings.begin(); it != trackings.end(); ++it) {
            outputStream << detMap.first << ","
                         << it->ID << ","
                         << it->bb.x1() << ","
                         << it->bb.y1() << ","
                         << it->bb.width << ","
                         << it->bb.height << ","
                         << "1,-1,-1,-1\n";
        }
        ++frameCount;
    }
    outputStream.close();
    return std::pair<std::chrono::duration<double, std::milli>, int>(cumulativeDuration, frameCount);
}

int main(int argc, char **argv) {
    const boost::filesystem::path dataDirPath = boost::filesystem::current_path().parent_path() / "data";
    int opt;

    std::string dataConfigFileName;

    while ((opt = getopt(argc, argv, "d:")) != -1) {
        switch (opt) {
            case 'd':
                dataConfigFileName = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (dataConfigFileName == "") {
        fprintf(stderr, USAGE_MESSAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    boost::filesystem::path dataConfigFilePath = dataDirPath / "config" / dataConfigFileName;
    std::ifstream dataConfigFile(dataConfigFilePath.string());
    if (dataConfigFile.is_open()) {
        std::string datasetRelPath;
        std::getline(dataConfigFile, datasetRelPath);
        boost::filesystem::path datasetPath = dataDirPath / datasetRelPath;

        std::string sequence;
        boost::filesystem::path outputDirPath;
        boost::filesystem::path inputPath;
        boost::filesystem::path outputPath;
        std::pair<std::chrono::duration<double, std::milli>, int> durationFrameCount;
        std::chrono::duration<double, std::milli> cumulativeDuration;
        while (getline(dataConfigFile, sequence)) {
            std::cout << "Sequence: " << sequence << std::endl;
            inputPath = datasetPath / sequence / "det/det.txt";
            outputDirPath = dataDirPath / "results" / datasetRelPath / sequence / "TEMP";
            if (!boost::filesystem::is_directory(outputDirPath)) {
                boost::filesystem::create_directories(outputDirPath);
            }
            outputPath = outputDirPath / "track.txt";

            durationFrameCount = track(inputPath, outputPath);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(durationFrameCount.first).count();
            std::cout << "Duration: " << duration << "ms (" << double(durationFrameCount.second * 1000) / duration << "fps)\n";
            cumulativeDuration += durationFrameCount.first;
        }
        dataConfigFile.close();
        std::cout << "Total duration: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(cumulativeDuration).count()
                  << "ms\n";
    } else {
        fprintf(stderr, OPEN_FILE_MESSAGE, dataConfigFileName.c_str());
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}