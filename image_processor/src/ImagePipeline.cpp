#include "ImagePipeline.hpp"
#include <filesystem>
#include <iostream>
#include <thread>

namespace fs = std::filesystem;

ImagePipeline::ImagePipeline(std::string inputDir, std::string outputDir, size_t threadCount)
    : inputDir_(std::move(inputDir)), outputDir_(std::move(outputDir)), threadCount_(threadCount) {
    
    if (!fs::exists(outputDir_)) {
        fs::create_directories(outputDir_);
    }

    
    for (const auto& entry : fs::directory_iterator(inputDir_)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".jpg" || ext == ".png" || ext == ".jpeg") {
                fileQueue_.push_back(entry.path().filename().string());
            }
        }
    }
}

void ImagePipeline::workerThread() {
    while (true) {
        std::string filename;
        {
            
            std::lock_guard<std::mutex> lock(queueMutex_);
            if (fileQueue_.empty()) {
                return;
            }
            filename = fileQueue_.back();
            fileQueue_.pop_back();
        }

        processSingleImage(filename);
    }
}

bool ImagePipeline::processSingleImage(const std::string& filename) {
    std::string inputPath = inputDir_ + "/" + filename;
    std::string outputPath = outputDir_ + "/edge_" + filename;

    
    cv::Mat src = cv::imread(inputPath, cv::IMREAD_COLOR);
    if (src.empty()) {
        std::lock_guard<std::mutex> lock(consoleMutex_);
        std::cerr << "[Error] Failed to load: " << filename << "\n";
        return false;
    }

    
    cv::Mat gray, gradX, gradY, absGradX, absGradY, edges;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    
    cv::Sobel(gray, gradX, CV_16S, 1, 0, 3);
    cv::Sobel(gray, gradY, CV_16S, 0, 1, 3);
    
    cv::convertScaleAbs(gradX, absGradX);
    cv::convertScaleAbs(gradY, absGradY);
    cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, edges);

    cv::imwrite(outputPath, edges);

    {
        std::lock_guard<std::mutex> lock(consoleMutex_);
        std::cout << "[Thread " << std::this_thread::get_id() << "] Processed: " << filename << "\n";
    }

    return true;
}

void ImagePipeline::run() {
    std::vector<std::thread> workers;
    workers.reserve(threadCount_);

    for (size_t i = 0; i < threadCount_; ++i) {
        workers.emplace_back(&ImagePipeline::workerThread, this);
    }

    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}