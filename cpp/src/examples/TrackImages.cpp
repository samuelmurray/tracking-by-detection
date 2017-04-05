#include "../VideoTracker.h"
#include "../detector/RandomDetector.h"
#include "../tracker/mcsort/MCSORT.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <chrono>

const char *USAGE_MESSAGE = "Usage: %s [-g] [-s sequencePath] [-f configFile]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";

int track(const boost::filesystem::path &inputDir,
          const boost::filesystem::path &outputPath) {
    if (!boost::filesystem::is_directory(inputDir)) {
        fprintf(stderr, "TODO: ERROR MESSAGE");
        exit(EXIT_FAILURE);
    }
    std::ofstream outputStream;
    outputStream.open(outputPath.string());
    if (!outputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, outputPath.c_str());
        exit(EXIT_FAILURE);
    }

    int cumulativeDuration = 0;
    VideoTracker tracker(std::make_shared<RandomDetector>(), std::make_shared<MCSORT>());
    std::vector<Tracking> trackings;
    int frameCount = 0;
    cv::Mat image;
    for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(inputDir), {})) {
        image = cv::imread(entry.path().string(), 1);

        auto startTime = std::chrono::steady_clock::now();
        trackings = tracker.track(image);
        auto duration = std::chrono::steady_clock::now() - startTime;

        cumulativeDuration += std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        for (auto it = trackings.begin(); it != trackings.end(); ++it) {
            outputStream << frameCount << ","
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
    return cumulativeDuration;
}

int track(const boost::filesystem::path &datasetsDir,
          const boost::filesystem::path &resultsDir,
          const std::string &sequenceName) {
    boost::filesystem::path inputDir = datasetsDir / sequenceName;
    boost::filesystem::path outputDir = resultsDir;
    inputDir /= "img1";
    outputDir /= "img1";
    if (!boost::filesystem::is_directory(outputDir)) {
        boost::filesystem::create_directories(outputDir);
    }
    boost::filesystem::path outputPath = outputDir / (sequenceName + ".txt");
    return track(inputDir, outputPath);
}

int main(int argc, char **argv) {
    const boost::filesystem::path dataDir = boost::filesystem::current_path().parent_path() / "data";
    int opt;
    std::string sequenceName;
    std::string configFileName;
    while ((opt = getopt(argc, argv, "s:f:")) != -1) {
        switch (opt) {
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
            std::getline(configFile, line);
            boost::filesystem::path sequencesDir = dataDir / line;
            std::getline(configFile, line);
            boost::filesystem::path resultsDir = dataDir / line;

            int duration;
            int cumulativeDuration = 0;
            while (std::getline(configFile, sequenceName)) {
                std::cout << "Sequence: " << sequenceName << std::endl;
                duration = track(sequencesDir, resultsDir, sequenceName);
                std::cout << "Duration: " << duration << "s" << std::endl;
                cumulativeDuration += duration;
            }
            configFile.close();
            std::cout << "Total duration: " << cumulativeDuration << "s" << std::endl;
            exit(EXIT_SUCCESS);
        }
        fprintf(stderr, OPEN_FILE_MESSAGE, configFileName.c_str());
    }

    fprintf(stderr, USAGE_MESSAGE, argv[0]);
    exit(EXIT_FAILURE);
}