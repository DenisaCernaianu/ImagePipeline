cmake_minimum_required(VERSION 3.16)
project(ImageProcessor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

include_directories(include ${OpenCV_INCLUDE_DIRS})

add_executable(image_processor 
    src/main.cpp 
    src/ImagePipeline.cpp
)

target_link_libraries(image_processor 
    PRIVATE 
    ${OpenCV_LIBS} 
    Threads::Threads
)