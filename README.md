# Multithreaded Image Processing Pipeline 

A high-performance C++ CLI tool designed to batch-process image datasets using Sobel Edge Detection and multi-core CPU parallelism.

## Build Instructions (Linux)
```bash
sudo apt update && sudo apt install -y libopencv-dev build-essential cmake
mkdir build && cd build
cmake ..
make -j$(nproc)
```

