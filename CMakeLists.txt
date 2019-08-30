cmake_minimum_required(VERSION 3.0)

project(opencv_decode C CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
        "Choose the type of build, options are: Debug, Release, RelWithDebInfo and MinSizeRel"
        FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED PATHS "/home/kbinias/dev/opencv/build/installation/OpenCV" COMPONENTS opencv_core opencv_imgcodecs)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

add_executable(opencv_decode opencv_decode.cpp)

target_link_libraries(opencv_decode ${OpenCV_LIBS})
