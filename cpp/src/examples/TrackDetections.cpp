#include "../tracker/Tracker.h"
#include "../tracker/mcsort/MCSORT.h"
#include "../util/DetectionFileParser.h"

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

static bool SHOW = false;
static string datasetsDir = "../data/2DMOT2015/data/train/";
static string resultsDir = "../data/results/";

void track(const string &sequence) {
    string inputFilePath = datasetsDir + sequence + "/gt/gt.txt";
    string outputFilePath = resultsDir + sequence + ".txt";
    ifstream detectionFile(inputFilePath);
    if (!detectionFile.is_open()) {
        cout << "Could not load file " << inputFilePath << endl;
    } else {
        ofstream output;
        output.open(outputFilePath);
        MCSORT tracker;
        vector<Tracking> trackings;
        for (auto const &detMap : DetectionFileParser::parseMOTFile(detectionFile)) {
            cout << "NEW FRAME - " << detMap.first << endl;
            trackings = tracker.track(detMap.second);
            cout << "---TRACKINGS---" << endl;
            for (auto it = trackings.begin(); it != trackings.end(); ++it) {
                cout << *it << endl;
                output << detMap.first << ","
                       << it->ID << ","
                       << it->bb.x1() << ","
                       << it->bb.y1() << ","
                       << it->bb.width << ","
                       << it->bb.height << ","
                       << "1,-1,-1,-1\n";
            }
            cout << endl;
        }
        output.close();
    }
}

int main(int argc, char **argv) {
    int opt;
    string sequence;
    string sequencesFile;
    while ((opt = getopt(argc, argv, "ps:f:")) != -1) {
        switch (opt) {
            case 'p':
                SHOW = true;
                break;
            case 's':
                sequence = optarg;
                break;
            case 'f':
                sequencesFile = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-p] [-s sequence] [-f sequencesFile]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (sequence != "" && sequencesFile != "") {
        fprintf(stderr, "You can't specify both -s and -f\n");
    } else if (sequence != "") {
        track(sequence);
        exit(EXIT_SUCCESS);
    } else if (sequencesFile != "") {
        ifstream detectionFile(sequencesFile);
        if (detectionFile.is_open()) {
            while (getline(detectionFile, sequence)) { track(sequence); }
            detectionFile.close();
            exit(EXIT_SUCCESS);
        }
        fprintf(stderr, "Could not load file %s\n", sequencesFile.c_str());
    }
    fprintf(stderr, "Usage: %s [-p] [-s sequence] [-f sequencesFile]\n", argv[0]);
    exit(EXIT_FAILURE);
}