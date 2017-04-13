#include "../tracker/Tracker.h"
#include "../tracker/mcsort/MCSORT.h"
#include "../util/DetectionFileParser.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <chrono>

const char *USAGE_MESSAGE = "Usage: %s [-g] [-s sequencePath] [-f configFile]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";

std::chrono::duration<double, std::milli> track(const boost::filesystem::path &inputPath,
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
    for (auto const &detMap : DetectionFileParser::parseMOTFile(inputStream)) {

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
    }
    outputStream.close();
    return cumulativeDuration;
}

std::chrono::duration<double, std::milli> track(const boost::filesystem::path &datasetsDir,
                                                const boost::filesystem::path &resultsDir,
                                                const std::string &sequenceName,
                                                bool useGroundTruth) {
    boost::filesystem::path inputPath = datasetsDir / sequenceName;
    boost::filesystem::path outputDir = resultsDir;
    if (useGroundTruth) {
        inputPath /= "gt/gt.txt";
        outputDir /= "gt";
    } else {
        inputPath /= "det/det.txt";
        outputDir /= "det";
    }
    if (!boost::filesystem::is_directory(outputDir)) {
        boost::filesystem::create_directories(outputDir);
    }
    boost::filesystem::path outputPath = outputDir / (sequenceName + ".txt");
    return track(inputPath, outputPath);
}

int main(int argc, char **argv) {
    const boost::filesystem::path dataDir = boost::filesystem::current_path().parent_path() / "data";
    bool useGroundTruth = false;
    int opt;
    std::string sequenceName;
    std::string configFileName;
    while ((opt = getopt(argc, argv, "gs:f:")) != -1) {
        switch (opt) {
            case 'g':
                useGroundTruth = true;
                break;
            case 's':
                sequenceName = optarg;
                break;
            case 'f':
                configFileName = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (sequenceName != "" && configFileName != "") {
        fprintf(stderr, "You can't specify both -s and -f\n");
    } else if (sequenceName != "") {
        track(sequenceName, boost::filesystem::current_path() / "result.txt");
        exit(EXIT_SUCCESS);
    } else if (configFileName != "") {
        boost::filesystem::path configFilePath = dataDir / configFileName;
        std::ifstream configFile(configFilePath.string());
        if (configFile.is_open()) {
            std::string line;
            getline(configFile, line);
            boost::filesystem::path sequencesDir = dataDir / line;
            getline(configFile, line);
            boost::filesystem::path resultsDir = dataDir / line;

            std::chrono::duration<double, std::milli> duration;
            std::chrono::duration<double, std::milli> cumulativeDuration;
            while (getline(configFile, sequenceName)) {
                std::cout << "Sequence: " << sequenceName << std::endl;
                duration = track(sequencesDir, resultsDir, sequenceName, useGroundTruth);
                std::cout << "Duration: "
                          << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
                          << "ms\n";
                cumulativeDuration += duration;
            }
            configFile.close();
            std::cout << "Total duration: "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(cumulativeDuration).count()
                      << "ms\n";
            exit(EXIT_SUCCESS);
        }
        fprintf(stderr, OPEN_FILE_MESSAGE, configFileName.c_str());
    }

    fprintf(stderr, USAGE_MESSAGE, argv[0]);
    exit(EXIT_FAILURE);
}