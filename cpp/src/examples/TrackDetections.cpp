#include "../tracker/Tracker.h"
#include "../tracker/mcsort/MCSORT.h"
#include "../util/DetectionFileParser.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <chrono>

using namespace std;

const char *USAGE_MESSAGE = "Usage: %s [-g] [-s sequencePath] [-f configFile]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";

int track(const boost::filesystem::path &inputPath,
          const boost::filesystem::path &outputPath) {
    ifstream inputStream(inputPath.string());
    if (!inputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, inputPath.c_str());
        exit(EXIT_FAILURE);
    }
    ofstream outputStream;
    outputStream.open(outputPath.string());
    if (!outputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, outputPath.c_str());
        exit(EXIT_FAILURE);
    }
    int cumulativeDuration = 0;
    MCSORT tracker;
    vector<Tracking> trackings;
    for (auto const &detMap : DetectionFileParser::parseMOTFile(inputStream)) {
        cout << "NEW FRAME - " << detMap.first << endl;
        auto startTime = chrono::steady_clock::now();
        trackings = tracker.track(detMap.second);
        auto duration = chrono::steady_clock::now() - startTime;
        cumulativeDuration += chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout << "---TRACKINGS---" << endl;
        for (auto it = trackings.begin(); it != trackings.end(); ++it) {
            cout << *it << endl;
            outputStream << detMap.first << ","
                         << it->ID << ","
                         << it->bb.x1() << ","
                         << it->bb.y1() << ","
                         << it->bb.width << ","
                         << it->bb.height << ","
                         << "1,-1,-1,-1\n";
        }
        cout << endl;
    }
    outputStream.close();
    return cumulativeDuration;
}

int track(const boost::filesystem::path &datasetsDir,
          const boost::filesystem::path &resultsDir,
          const string &sequenceName,
          bool useGroundTruth) {
    boost::filesystem::path inputPath = datasetsDir / sequenceName;
    boost::filesystem::path outputDir = resultsDir;
    if (useGroundTruth) {
        inputPath /= "gt/gt.txt";
        outputDir /= "gt" ;
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
    boost::filesystem::path DATA_DIR = boost::filesystem::current_path().parent_path() / "data";
    bool useGroundTruth = false;
    int opt;
    string sequencePath;
    string configFile;
    while ((opt = getopt(argc, argv, "gs:f:")) != -1) {
        switch (opt) {
            case 'g':
                useGroundTruth = true;
                break;
            case 's':
                sequencePath = optarg;
                break;
            case 'f':
                configFile = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (sequencePath != "" && configFile != "") {
        fprintf(stderr, "You can't specify both -s and -f\n");
    } else if (sequencePath != "") {
        track(sequencePath, boost::filesystem::current_path() / "result.txt");
        exit(EXIT_SUCCESS);
    } else if (configFile != "") {
        boost::filesystem::path sequencesFilePath = DATA_DIR / configFile;
        ifstream detectionFile(sequencesFilePath.string());
        if (detectionFile.is_open()) {
            string line;
            getline(detectionFile, line);
            boost::filesystem::path datasetsDir = DATA_DIR / line;
            getline(detectionFile, line);
            boost::filesystem::path resultsDir = DATA_DIR / line;

            int cumulativeDuration = 0;
            while (getline(detectionFile, sequencePath)) {
                cumulativeDuration += track(datasetsDir, resultsDir, sequencePath, useGroundTruth);
            }
            detectionFile.close();
            cout << "---FINISHED---" << endl;
            cout << "Total duration: " << cumulativeDuration << "ms" << endl;
            exit(EXIT_SUCCESS);
        }
        fprintf(stderr, OPEN_FILE_MESSAGE, configFile.c_str());
    }

    fprintf(stderr, USAGE_MESSAGE, argv[0]);
    exit(EXIT_FAILURE);
}