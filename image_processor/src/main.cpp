#include "ImagePipeline.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./image_processor <input_dir> <output_dir> [threads]\n";
        return 1;
    }

    std::string inDir = argv[1];
    std::string outDir = argv[2];
    size_t threads = (argc >= 4) ? std::stoul(argv[3]) : std::thread::hardware_concurrency();

    std::cout << "Starting processing with " << threads << " worker threads...\n";

    auto start = std::chrono::high_resolution_clock::now();

    ImagePipeline pipeline(inDir, outDir, threads);
    pipeline.run();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Done! Execution time: " << duration.count() << " ms\n";
    return 0;
}