#include "../ImageTracker.h"
#include "../detector/RandomDetector.h"
#include "../detector/SSDDetector.h"
#include "../tracker/PAOT.h"

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

static const boost::filesystem::path dataDirPath = boost::filesystem::current_path().parent_path() / "data";
static const boost::filesystem::path modelDirPath = boost::filesystem::current_path().parent_path() / "models";
static const double originalFrameRate = 30;

const char *USAGE_MESSAGE = "Usage: %s "
        "-s sequenceMap "
        "-m modelConfigFile "
        "[-r]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";
const char *OPEN_DIR_MESSAGE = "Could not open directory %s\n";
const char *FILE_EXISTS_MESSAGE = "Output file %s already exists; don't overwrite\n";

std::pair<std::chrono::duration<double, std::milli>, int> detectAndTrack(const std::shared_ptr<Detector> &detector,
                                                                         const boost::filesystem::path &sequencePath,
                                                                         const std::string &modelType,
                                                                         const bool realTime) {
    typedef std::chrono::duration<double, std::milli> msduration;

    // Make sure input directory exists
    boost::filesystem::path inputDirPath = dataDirPath / sequencePath / "images";
    if (!boost::filesystem::is_directory(inputDirPath)) {
        fprintf(stderr, OPEN_DIR_MESSAGE, sequencePath.c_str());
        exit(EXIT_FAILURE);
    }

    // Create output directory if not exists
    boost::filesystem::path outputDirPath = dataDirPath / "results" / sequencePath.parent_path() / modelType;
    if (!boost::filesystem::is_directory(outputDirPath)) {
        boost::filesystem::create_directories(outputDirPath);
    }

    // Make sure output file does not exist
    boost::filesystem::path outputPath = outputDirPath / (sequencePath.filename().string() + ".txt");
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

    ImageTracker imageTracker(detector, std::make_shared<PAOT>());

    std::vector<boost::filesystem::path> imagePaths;
    std::copy(boost::filesystem::directory_iterator(inputDirPath),
              boost::filesystem::directory_iterator(),
              std::back_inserter(imagePaths));
    std::sort(imagePaths.begin(), imagePaths.end());

    msduration cumulativeDuration = std::chrono::milliseconds::zero();
    int frameCount = 0;
    int frame = 0;
    for (auto imageIt = imagePaths.begin(); imageIt != imagePaths.end(); ++imageIt) {
        std::cout << "Image: " << imageIt->string() << std::endl;
        cv::Mat image = cv::imread(imageIt->string(), 1);

        auto startTime = std::chrono::high_resolution_clock::now();
        std::vector<Tracking> trackings = imageTracker.detectAndTrack(image);
        auto endTime = std::chrono::high_resolution_clock::now();
        msduration duration = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                endTime - startTime);
        cumulativeDuration += duration;

        for (auto trackingIt = trackings.begin(); trackingIt != trackings.end(); ++trackingIt) {
            outputStream << frame << ","
                         << trackingIt->ID << ","
                         << trackingIt->bb.x1() << ","
                         << trackingIt->bb.y1() << ","
                         << trackingIt->bb.width << ","
                         << trackingIt->bb.height << ","
                         << "1" << "," // Confidence
                         << "-1,-1,-1" << "," // Unused
                         << trackingIt->label << "\n";
        }
        if (realTime) {
            int framesToSkip = int((originalFrameRate * duration.count()) / 1000) - 1;
            std::cout << "Skipping " << framesToSkip << " frames" << std::endl;
            for (int i = 0; i < framesToSkip; ++i) {
                if (imageIt == imagePaths.end()) {
                    ++frameCount;
                    outputStream.close();
                    return std::pair<msduration, int>(cumulativeDuration, frameCount);
                }
                ++frame;
                ++imageIt;
            }
        }
        ++frame;
        ++frameCount;
        if (frameCount % 100 == 0) {
            std::cout << "Processed " << frameCount << " images" << std::endl;
        }
    }
    outputStream.close();
    return std::pair<msduration, int>(cumulativeDuration, frameCount);
}

#ifdef USE_CAFFE

int main(int argc, char **argv) {

    std::string sequenceMapName;
    std::string modelConfigFileName;
    std::string modelType;
    bool realTime = false;
    std::shared_ptr<Detector> detector;

    int opt;
    while ((opt = getopt(argc, argv, "s:m:r")) != -1) {
        switch (opt) {
            case 's':
                sequenceMapName = optarg;
                break;
            case 'm':
                modelConfigFileName = optarg;
                break;
            case 'r':
                realTime = true;
                break;
            default:
                fprintf(stderr, USAGE_MESSAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (sequenceMapName == "" || modelConfigFileName == "") {
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
        detector = std::make_shared<SSDDetector>(modelFilePath.string(), weightsFilePath.string(), meanValues);
    } else {
        fprintf(stderr, OPEN_FILE_MESSAGE, modelConfigFilePath.c_str());
        exit(EXIT_FAILURE);
    }

    boost::filesystem::path sequenceMapPath = dataDirPath / "seqmaps" / sequenceMapName;
    std::ifstream sequenceMap(sequenceMapPath.string());
    if (sequenceMap.is_open()) {

        std::chrono::duration<double, std::milli> cumulativeDuration;
        int cumulativeFrameCount = 0;

        std::string sequencePathString;
        while (std::getline(sequenceMap, sequencePathString)) {
            std::cout << "Sequence: " << sequencePathString << std::endl;
            auto durationFrameCount = detectAndTrack(detector, sequencePathString, modelType, realTime);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(durationFrameCount.first).count();
            std::cout << "Duration: " << duration << "ms"
                      << " (" << double(durationFrameCount.second * 1000) / duration << "fps)\n";
            cumulativeDuration += durationFrameCount.first;
            cumulativeFrameCount += durationFrameCount.second;
        }
        sequenceMap.close();
        auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(cumulativeDuration).count();
        std::cout << "Total duration: " << double(totalDuration) / 1000 << "s"
                  << " (" << double(cumulativeFrameCount * 1000) / totalDuration << "fps)\n";
    } else {
        fprintf(stderr, OPEN_FILE_MESSAGE, sequenceMapName.c_str());
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