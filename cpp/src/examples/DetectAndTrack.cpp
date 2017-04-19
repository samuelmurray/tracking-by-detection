#include "../VideoTracker.h"
#include "../detector/RandomDetector.h"
#include "../tracker/mcsort/MCSORT.h"
#include "../detector/BBDetector.h"

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
#include <memory>

const char *USAGE_MESSAGE = "Usage: %s [-s sequencePath] [-f configFile]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";
const char *OPEN_DIR_MESSAGE = "Could not open directory %s\n";

std::chrono::duration<double, std::milli> track(const boost::filesystem::path &inputDir,
                                                const boost::filesystem::path &outputPath) {
    if (!boost::filesystem::is_directory(inputDir)) {
        fprintf(stderr, OPEN_DIR_MESSAGE, inputDir.c_str());
        exit(EXIT_FAILURE);
    }
    std::ofstream outputStream;
    outputStream.open(outputPath.string());
    if (!outputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, outputPath.c_str());
        exit(EXIT_FAILURE);
    }
#ifdef USE_CAFFE
    VideoTracker tracker(std::make_shared<BBDetector>(
            "../models/okutama/action-detection/deploy.prototxt",
            "../models/okutama/action-detection/VGG_okutama_action_SSD_960x540_iter_12000.caffemodel",
            "", "104,117,123"),
                         std::make_shared<MCSORT>());
#else //USE_CAFFE
    VideoTracker tracker(std::make_shared<RandomDetector>(), std::make_shared<MCSORT>());
#endif //USE_CAFFE
    std::vector<Tracking> trackings;
    std::chrono::duration<double, std::milli> cumulativeDuration = std::chrono::milliseconds::zero();
    int frameCount = 0;
    cv::Mat image;
    std::vector<boost::filesystem::path> imagePaths;
    std::copy(boost::filesystem::directory_iterator(inputDir),
              boost::filesystem::directory_iterator(),
              std::back_inserter(imagePaths));
    std::sort(imagePaths.begin(), imagePaths.end());

    for (auto imageIt = imagePaths.begin(); imageIt != imagePaths.end(); ++imageIt) {
        std::cout << imageIt->string() << std::endl;
        image = cv::imread(imageIt->string(), 1);

        auto startTime = std::chrono::high_resolution_clock::now();
        trackings = tracker.track(image);
        auto endTime = std::chrono::high_resolution_clock::now();

        cumulativeDuration += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                endTime - startTime);

        for (auto trackingIt = trackings.begin(); trackingIt != trackings.end(); ++trackingIt) {
            outputStream << frameCount << ","
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
    return cumulativeDuration;
}

std::chrono::duration<double, std::milli> track(const boost::filesystem::path &datasetsDir,
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

            std::chrono::duration<double, std::milli> duration;
            std::chrono::duration<double, std::milli> cumulativeDuration;
            while (std::getline(configFile, sequenceName)) {
                std::cout << "Sequence: " << sequenceName << std::endl;
                duration = track(sequencesDir, resultsDir, sequenceName);
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
