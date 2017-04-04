#include "../tracker/Tracker.h"
#include "../tracker/mcsort/MCSORT.h"
#include "../util/DetectionFileParser.h"

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

static bool USE_GROUND_TRUTH = false;
static string DATASETS_DIR = "../data";
static string RESULTS_DIR = "../data";
const char *USAGE_MESSAGE = "Usage: %s [-g] [-s sequencePath] [-f sequencesFile]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";

void track(const string &inputPath, const string &outputPath) {
    ifstream inputStream(inputPath);
    if (!inputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, inputPath.c_str());
        exit(EXIT_FAILURE);
    }
    ofstream outputStream;
    outputStream.open(outputPath);
    if (!outputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, outputPath.c_str());
        exit(EXIT_FAILURE);
    }
    MCSORT tracker;
    vector<Tracking> trackings;
    for (auto const &detMap : DetectionFileParser::parseMOTFile(inputStream)) {
        cout << "NEW FRAME - " << detMap.first << endl;
        trackings = tracker.track(detMap.second);
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
}

void track(const string &sequenceName) {
    string inputPath = DATASETS_DIR + "/" + sequenceName;
    string outputPath = RESULTS_DIR;
    if (USE_GROUND_TRUTH) {
        inputPath += "/gt/gt.txt";
        outputPath += "/gt/" + sequenceName + ".txt";
    } else {
        inputPath += "/det/det.txt";
        outputPath += "/det/" + sequenceName + ".txt";
    }
    track(inputPath, outputPath);
}


int main(int argc, char **argv) {
    int opt;
    string sequencePath;
    string sequencesFile;
    while ((opt = getopt(argc, argv, "gs:f:")) != -1) {
        switch (opt) {
            case 'g':
                USE_GROUND_TRUTH = true;
                break;
            case 's':
                sequencePath = optarg;
                break;
            case 'f':
                sequencesFile = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (sequencePath != "" && sequencesFile != "") {
        fprintf(stderr, "You can't specify both -s and -f\n");
    } else if (sequencePath != "") {
        track(sequencePath, "./result.txt");
        exit(EXIT_SUCCESS);
    } else if (sequencesFile != "") {
        ifstream detectionFile(sequencesFile);
        if (detectionFile.is_open()) {
            string relInputPath;
            string relOutputPath;
            getline(detectionFile, relInputPath);
            getline(detectionFile, relOutputPath);
            DATASETS_DIR += "/" + relInputPath;
            RESULTS_DIR += "/" + relOutputPath;
            while (getline(detectionFile, sequencePath)) { track(sequencePath); }
            detectionFile.close();
            exit(EXIT_SUCCESS);
        }
        fprintf(stderr, OPEN_FILE_MESSAGE, sequencesFile.c_str());
    }

    fprintf(stderr, USAGE_MESSAGE, argv[0]);
    exit(EXIT_FAILURE);
}