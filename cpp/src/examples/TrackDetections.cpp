#include "../tracker/Tracker.h"
#include "../tracker/mcsort/MCSORT.h"
#include "../util/DetectionFileParser.h"

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

static bool USE_GROUND_TRUTH = false;
static bool USE_TEST_SET = false;

void track(const string &sequence) {
    string datasetsDir = "../data/2DMOT2015/data/";
    string resultsDir = "../data/results/";
    if (USE_TEST_SET) {
        datasetsDir += "test/";
        resultsDir += "test/";
    } else {
        datasetsDir += "train/";
        resultsDir += "train/";
    }

    string inputFilePath = datasetsDir + sequence + "/";
    string outputFilePath = resultsDir;
    if (USE_GROUND_TRUTH) {
        inputFilePath += "gt/gt.txt";
        outputFilePath += "gt/" + sequence + ".txt";
    } else {
        inputFilePath += "det/det.txt";
        outputFilePath += "det/" + sequence + ".txt";
    }

    ifstream inputStream(inputFilePath);
    if (!inputStream.is_open()) {
        cout << "Could not load file " << inputFilePath << endl;
    } else {
        ofstream outputStream;
        outputStream.open(outputFilePath);
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
}

const char *USAGE_MESSAGE = "Usage: %s [-g] [-s sequence] [-f sequencesFile]\n";

int main(int argc, char **argv) {
    int opt;
    string sequence;
    string sequencesFile;
    while ((opt = getopt(argc, argv, "gs:f:")) != -1) {
        switch (opt) {
            case 'g':
                USE_GROUND_TRUTH = true;
                break;
            case 's':
                sequence = optarg;
                break;
            case 'f':
                sequencesFile = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (sequence != "" && sequencesFile != "") {
        fprintf(stderr, "You can't specify both -s and -f\n");
    } else if (USE_GROUND_TRUTH && USE_TEST_SET) {
        fprintf(stderr, "You can't use both -g and -t");
    } else if (sequence != "") {
        track(sequence);
        exit(EXIT_SUCCESS);
    } else if (sequencesFile != "") {
        ifstream detectionFile(sequencesFile);
        if (detectionFile.is_open()) {
            string firstLine;
            getline(detectionFile, firstLine);
            if (firstLine != "train" && firstLine != "test") {
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
            } else {
                USE_TEST_SET = firstLine == "test";
                while (getline(detectionFile, sequence)) { track(sequence); }
                detectionFile.close();
                exit(EXIT_SUCCESS);
            }
        }
        fprintf(stderr, "Could not load file %s\n", sequencesFile.c_str());
    }

    fprintf(stderr, USAGE_MESSAGE, argv[0]);
    exit(EXIT_FAILURE);
}