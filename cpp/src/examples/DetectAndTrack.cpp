#include "../ImageTracker.h"
#include "../detector/RandomDetector.h"
#include "../detector/BBDetector.h"
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
#include <memory>

const char *USAGE_MESSAGE = "Usage: %s [-d dataConfigFile] [-m modelConfigFile]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";
const char *OPEN_DIR_MESSAGE = "Could not open directory %s\n";

std::pair<std::chrono::duration<double, std::milli>, int> detectAndTrack(const std::shared_ptr<Detector> &detector,
                                                                         const boost::filesystem::path &inputDir,
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

    ImageTracker tracker(detector, std::make_shared<MCSORT>());
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
        trackings = tracker.detectAndTrack(image);
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
    return std::pair<std::chrono::duration<double, std::milli>, int>(cumulativeDuration, frameCount);
}

#ifdef USE_CAFFE

int main(int argc, char **argv) {
    const boost::filesystem::path dataDirPath = boost::filesystem::current_path().parent_path() / "data";
    const boost::filesystem::path modelDirPath = boost::filesystem::current_path().parent_path() / "models";
    int opt;

    std::string dataConfigFileName;
    std::string modelConfigFileName;
    std::string modelType;
    std::shared_ptr<Detector> detector;

    while ((opt = getopt(argc, argv, "d:m:")) != -1) {
        switch (opt) {
            case 'd':
                dataConfigFileName = optarg;
                break;
            case 'm':
                modelConfigFileName = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (dataConfigFileName == "" || modelConfigFileName == "") {
        fprintf(stderr, USAGE_MESSAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    boost::filesystem::path modelConfigFilePath = modelDirPath / "config" / modelConfigFileName;
    std::ifstream modelConfigFile(modelConfigFilePath.string());
    if (modelConfigFile.is_open()) {
        std::string modelFile;
        std::string weightsFile;
        std::string meanValues;
        std::getline(modelConfigFile, modelType);
        std::getline(modelConfigFile, modelFile);
        std::getline(modelConfigFile, weightsFile);
        std::getline(modelConfigFile, meanValues);
        boost::filesystem::path modelFilePath = modelDirPath / modelFile;
        boost::filesystem::path weightsFilePath = modelDirPath / weightsFile;
        detector = std::make_shared<BBDetector>(modelFilePath.string(), weightsFilePath.string(), meanValues);
    } else {
        fprintf(stderr, OPEN_FILE_MESSAGE, modelConfigFilePath.c_str());
        exit(EXIT_FAILURE);
    }

    boost::filesystem::path dataConfigFilePath = dataDirPath / "config" / dataConfigFileName;
    std::ifstream dataConfigFile(dataConfigFilePath.string());
    if (dataConfigFile.is_open()) {
        std::string datasetRelPath;
        std::getline(dataConfigFile, datasetRelPath);
        boost::filesystem::path datasetPath = dataDirPath / datasetRelPath;

        std::string sequence;
        boost::filesystem::path sequenceImagesDirPath;
        boost::filesystem::path outputDirPath;
        boost::filesystem::path outputPath;
        std::pair<std::chrono::duration<double, std::milli>, int> durationFrameCount;
        std::chrono::duration<double, std::milli> cumulativeDuration;
        while (std::getline(dataConfigFile, sequence)) {
            std::cout << "Sequence: " << sequence << std::endl;
            sequenceImagesDirPath = datasetPath / sequence / "images";
            outputDirPath = dataDirPath / "results" / datasetRelPath / sequence / modelType;
            if (!boost::filesystem::is_directory(outputDirPath)) {
                boost::filesystem::create_directories(outputDirPath);
            }
            outputPath = outputDirPath / "track.txt";

            durationFrameCount = detectAndTrack(detector, sequenceImagesDirPath, outputPath);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(durationFrameCount.first).count();
            std::cout << "Duration: " << duration << "ms (" << double(durationFrameCount.second) / duration << "fps)\n";
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

#else //USE_CAFFE
int main(int argc, char** argv) {
    std::cerr << "This example requires Caffe; compile with USE_CAFFE.\n";
    exit(EXIT_FAILURE);
}
#endif //USE_CAFFE