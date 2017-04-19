#include "../detector/RandomDetector.h"
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

const char *USAGE_MESSAGE = "Usage: [-d dataConfig] [-m modelConfig]\n";
const char *OPEN_FILE_MESSAGE = "Could not open file %s\n";
const char *OPEN_DIR_MESSAGE = "Could not open directory %s\n";

std::chrono::duration<double, std::milli> detectAndSave(const std::shared_ptr<Detector> &detector,
                                                        const boost::filesystem::path &inputDirPath,
                                                        const boost::filesystem::path &outputPath) {
    if (!boost::filesystem::is_directory(inputDirPath)) {
        fprintf(stderr, OPEN_DIR_MESSAGE, inputDirPath.c_str());
        exit(EXIT_FAILURE);
    }
    std::ofstream outputStream;
    outputStream.open(outputPath.string());
    if (!outputStream.is_open()) {
        fprintf(stderr, OPEN_FILE_MESSAGE, outputPath.c_str());
        exit(EXIT_FAILURE);
    }

    std::vector<Detection> detections;
    std::chrono::duration<double, std::milli> cumulativeDuration = std::chrono::milliseconds::zero();
    int frameCount = 0;
    cv::Mat image;
    std::vector<boost::filesystem::path> imagePaths;
    std::copy(boost::filesystem::directory_iterator(inputDirPath),
              boost::filesystem::directory_iterator(),
              std::back_inserter(imagePaths));

    for (auto imageIt = imagePaths.begin(); imageIt != imagePaths.end(); ++imageIt) {
        std::cout << imageIt->string() << std::endl;
        image = cv::imread(imageIt->string(), 1);

        auto startTime = std::chrono::high_resolution_clock::now();
        detections = detector->detect(image);
        auto endTime = std::chrono::high_resolution_clock::now();

        cumulativeDuration += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                endTime - startTime);

        for (auto detectionIt = detections.begin(); detectionIt != detections.end(); ++detectionIt) {
            outputStream << frameCount << ","
                         << detectionIt->label << ","
                         << "-1,"
                         << detectionIt->bb.x1() << ","
                         << detectionIt->bb.y1() << ","
                         << detectionIt->bb.width << ","
                         << detectionIt->bb.height << ","
                         << detectionIt->confidence << ","
                         << "-1,-1,-1\n";
        }
        ++frameCount;
    }
    outputStream.close();
    return cumulativeDuration;
}

int main(int argc, char **argv) {
    const boost::filesystem::path dataDirPath = boost::filesystem::current_path().parent_path() / "data";
    const boost::filesystem::path modelDirPath = boost::filesystem::current_path().parent_path() / "model";
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
#ifdef USE_CAFFE
        detector = std::make_shared<BBDetector>(modelFile, weightsFile, meanValues);
#else //USE_CAFFE
        detector = std::make_shared<RandomDetector>();
#endif //USE_CAFFE
    } else {
        fprintf(stderr, OPEN_FILE_MESSAGE, modelConfigFileName.c_str());
        exit(EXIT_FAILURE);
    }

    boost::filesystem::path dataConfigFilePath = dataDirPath / "config" / dataConfigFileName;
    std::ifstream dataConfigFile(dataConfigFilePath.string());
    if (dataConfigFile.is_open()) {
        std::string sequencesDir;
        std::getline(dataConfigFile, sequencesDir);
        boost::filesystem::path datasetPath = dataDirPath / sequencesDir;

        std::chrono::duration<double, std::milli> duration;
        std::chrono::duration<double, std::milli> cumulativeDuration;
        std::string sequence;
        boost::filesystem::path sequenceImagesDirPath;
        boost::filesystem::path outputDirPath;
        boost::filesystem::path outputPath;
        while (std::getline(dataConfigFile, sequence)) {
            std::cout << "Sequence: " << sequence << std::endl;

            sequenceImagesDirPath = datasetPath / sequence / "images";
            outputDirPath = datasetPath / sequence / modelType;
            if (!boost::filesystem::is_directory(outputDirPath)) {
                boost::filesystem::create_directories(outputDirPath);
            }
            outputPath = outputDirPath / "det.txt";

            duration = detectAndSave(detector, sequenceImagesDirPath, outputPath);

            std::cout << "Duration: "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
                      << "ms\n";
            cumulativeDuration += duration;
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