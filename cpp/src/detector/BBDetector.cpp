#include "BBDetector.h"

#ifdef USE_CAFFE

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace caffe;

// Constructors

BBDetector::BBDetector(const std::string &modelFile,
                       const std::string &weightsFile,
                       const std::string &meanFile,
                       const std::string &meanValue)
        : Detector() {
    Caffe::set_mode(Caffe::GPU);

    /* Load the network. */
    net.reset(new Net<float>(modelFile, caffe::TEST));
    net->CopyTrainedLayersFrom(weightsFile);

    //CHECK_EQ(net->num_inputs(), 1) << "Network should have exactly one input.";
    //CHECK_EQ(net->num_outputs(), 1) << "Network should have exactly one output.";

    Blob<float> *inputLayer = net->input_blobs()[0];
    numChannels = inputLayer->channels();
    //CHECK(numChannels == 3 || numChannels == 1)
    //        << "Input layer should have 1 or 3 channels.";
    inputGeometry = cv::Size(inputLayer->width(), inputLayer->height());

    /* Load the binaryproto mean file. */
    setMean(meanFile, meanValue);
}

// Methods

std::vector<Detection> BBDetector::detect(const cv::Mat &image, double confidenceThreshold) {
    Blob<float> *inputLayer = net->input_blobs()[0];
    inputLayer->Reshape(1, numChannels,
                        inputGeometry.height, inputGeometry.width);
    /* Forward dimension change to all layers. */
    net->Reshape();

    std::vector<cv::Mat> inputChannels;
    wrapInputLayer(&inputChannels);

    preprocess(image, &inputChannels);

    net->Forward();

    /* Copy the output layer to a vector */
    Blob<float> *resultBlob = net->output_blobs()[0];
    const float *result = resultBlob->cpu_data();
    const int numDet = resultBlob->height();
    std::vector<Detection> detections;
    for (int k = 0; k < numDet; ++k) {
        if (result[0] == -1 || result[2] < confidenceThreshold) {
            // Skip invalid detection.
            result += 7;
            continue;
        }
        std::vector<float> det(result, result + 7);

        Detection detection("test_class",
                            BoundingBox((det[3] + det[5]) / 2 * image.cols,
                                        (det[4] + det[6]) / 2 * image.rows,
                                        (det[5] - det[3]) * image.cols,
                                        (det[6] - det[4]) * image.rows
                            ));

        detections.push_back(detection);
        result += 7;
    }

    return detections;
}


/* Load the mean file in binaryproto format. */
void BBDetector::setMean(const std::string &meanFile, const std::string &meanValue) {
    cv::Scalar channelMean;
    if (!meanFile.empty()) {

        BlobProto blobProto;
        ReadProtoFromBinaryFileOrDie(meanFile.c_str(), &blobProto);

        /* Convert from BlobProto to Blob<float> */
        Blob<float> meanBlob;
        meanBlob.FromProto(blobProto);
        /*
        CHECK_EQ(meanBlob.channels(), numChannels)
                << "Number of channels of mean file doesn't match input layer.";
        */

        /* The format of the mean file is planar 32-bit float BGR or grayscale. */
        std::vector<cv::Mat> channels;
        float *data = meanBlob.mutable_cpu_data();
        for (int i = 0; i < numChannels; ++i) {
            /* Extract an individual channel. */
            cv::Mat channel(meanBlob.height(), meanBlob.width(), CV_32FC1, data);
            channels.push_back(channel);
            data += meanBlob.height() * meanBlob.width();
        }

        /* Merge the separate channels into a single image. */
        cv::Mat mean;
        cv::merge(channels, mean);

        /* Compute the global mean pixel value and create a mean image
         * filled with this value. */
        channelMean = cv::mean(mean);
        mean = cv::Mat(inputGeometry, mean.type(), channelMean);
    }
    if (!meanValue.empty()) {
        /*
        CHECK(meanFile.empty()) <<
                                 "Cannot specify meanFile and meanValue at the same time";
        */
        std::stringstream ss(meanValue);
        std::vector<double> values;
        std::string item;
        while (getline(ss, item, ',')) {
            double value = std::atof(item.c_str());
            values.push_back(value);
        }
        /*
        CHECK(values.size() == 1 || values.size() == numChannels) <<
                                                                    "Specify either 1 meanValue or as many as channels: "
                                                                    << numChannels;
        */
        std::vector<cv::Mat> channels;
        for (int i = 0; i < numChannels; ++i) {
            /* Extract an individual channel. */
            cv::Mat channel(inputGeometry.height, inputGeometry.width, CV_32FC1,
                            cv::Scalar(values[i]));
            channels.push_back(channel);
        }
        cv::merge(channels, mean);
    }
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void BBDetector::wrapInputLayer(std::vector<cv::Mat> *inputChannels) {
    Blob<float> *inputLayer = net->input_blobs()[0];

    int width = inputLayer->width();
    int height = inputLayer->height();
    float *inputData = inputLayer->mutable_cpu_data();
    for (int i = 0; i < inputLayer->channels(); ++i) {
        cv::Mat channel(height, width, CV_32FC1, inputData);
        inputChannels->push_back(channel);
        inputData += width * height;
    }
}

void BBDetector::preprocess(const cv::Mat &image,
                            std::vector<cv::Mat> *inputChannels) {
    /* Convert the input image to the input image format of the network. */
    cv::Mat sample;
    if (image.channels() == 3 && numChannels == 1)
        cv::cvtColor(image, sample, cv::COLOR_BGR2GRAY);
    else if (image.channels() == 4 && numChannels == 1)
        cv::cvtColor(image, sample, cv::COLOR_BGRA2GRAY);
    else if (image.channels() == 4 && numChannels == 3)
        cv::cvtColor(image, sample, cv::COLOR_BGRA2BGR);
    else if (image.channels() == 1 && numChannels == 3)
        cv::cvtColor(image, sample, cv::COLOR_GRAY2BGR);
    else
        sample = image;

    cv::Mat sampleResized;
    if (sample.size() != inputGeometry)
        cv::resize(sample, sampleResized, inputGeometry);
    else
        sampleResized = sample;

    cv::Mat sampleFloat;
    if (numChannels == 3)
        sampleResized.convertTo(sampleFloat, CV_32FC3);
    else
        sampleResized.convertTo(sampleFloat, CV_32FC1);

    cv::Mat sampleNormalized;
    cv::subtract(sampleFloat, mean, sampleNormalized);

    /* This operation will write the separate BGR planes directly to the
     * input layer of the network because it is wrapped by the cv::Mat
     * objects in inputChannels. */
    cv::split(sampleNormalized, *inputChannels);

    /*
    CHECK(reinterpret_cast<float *>(inputChannels->at(0).data)
          == net->input_blobs()[0]->cpu_data())
            << "Input channels are not wrapping the input layer of the network.";
    */
}

#else //USE_CAFFE

#include <stdexcept>
#include <iostream>
BBDetector::BBDetector() {
        std::cerr << "Use of BBDetector requires Caffe; compile with USE_CAFFE.\n";
        throw std::runtime_error("Use of BBDetector requires Caffe; compile with USE_CAFFE.");
    }

BBDetector::BBDetector(const std::string &model_file,
               const std::string &weights_file,
               const std::string &mean_file,
               const std::string &mean_value) : BBDetector() {}
std::vector<Detection> BBDetector::detect(const cv::Mat &image, double confidenceThreshold) {
    throw std::runtime_error("Use of BBDetector requires Caffe; compile with USE_CAFFE.");
}

#endif //USE_CAFFE