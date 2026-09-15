#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <opencv2/opencv.hpp>

class ImagePipeline {
public:
    ImagePipeline(std::string inputDir, std::string outputDir, size_t threadCount);
    void run();

private:
    void workerThread();
    bool processSingleImage(const std::string& filename);

    std::string inputDir_;
    std::string outputDir_;
    size_t threadCount_;

    std::vector<std::string> fileQueue_;
    std::mutex queueMutex_;
    std::mutex consoleMutex_;
};