cmake_minimum_required(VERSION 3.1)

project(opencv_decode C CXX)

option(WITH_JPEGTURBO "Build with libjpeg-turbo library" ON)

if (NOT OPENCV_VER)
  set(OPENCV_VER "3.4.7")
endif()

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

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

# Third party modules
set(THIRD_PARTY_PATH "${CMAKE_BINARY_DIR}/3rdparty" CACHE STRING
  "A path setting third party libraries download & build directories.")

if(WITH_JPEGTURBO)
  SET(LIBJPEGTURBO_PROJECT        "libjpeg-turbo")
  SET(LIBJPEGTURBO_SOURCES_DIR    ${THIRD_PARTY_PATH}/${LIBJPEGTURBO_PROJECT})
  SET(LIBJPEGTURBO_INSTALL_DIR    ${THIRD_PARTY_PATH}/install/${LIBJPEGTURBO_PROJECT})
  SET(LIBJPEGTURBO_INC_DIR        "${LIBJPEGTURBO_INSTALL_DIR}/include" CACHE PATH "libjpeg-turbo include directory." FORCE)
  SET(LIBJPEGTURBO_LIB_DIR        "${LIBJPEGTURBO_INSTALL_DIR}/lib" CACHE PATH "libjpeg-turbo library directory." FORCE)
endif()

SET(OPENCV_PROJECT        "opencv")
SET(OPENCV_SOURCES_DIR    ${THIRD_PARTY_PATH}/${OPENCV_PROJECT})
SET(OPENCV_INSTALL_DIR    ${THIRD_PARTY_PATH}/install/${OPENCV_PROJECT})
SET(OPENCV_INC_DIR        "${OPENCV_INSTALL_DIR}/include" CACHE PATH "OpenCV include directory." FORCE)
SET(OPENCV_LIB_DIR        "${OPENCV_INSTALL_DIR}/lib" CACHE PATH "OpenCV library directory." FORCE)

INCLUDE(ExternalProject)

if(WITH_JPEGTURBO)
  ExternalProject_Add(
    ${LIBJPEGTURBO_PROJECT}
    GIT_REPOSITORY      "https://github.com/libjpeg-turbo/libjpeg-turbo.git"
    PREFIX              ${LIBJPEGTURBO_SOURCES_DIR}
    CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${LIBJPEGTURBO_INSTALL_DIR}
    CMAKE_ARGS          -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    CMAKE_ARGS          -DENABLE_STATIC=TRUE
    CMAKE_ARGS          -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    CMAKE_ARGS          -DCMAKE_INSTALL_DEFAULT_LIBDIR=lib
  )

  if(MSVC)
    SET(JPEG_OUTPUTNAME jpeg-static${CMAKE_STATIC_LIBRARY_SUFFIX})
  else()
    SET(JPEG_OUTPUTNAME ${CMAKE_STATIC_LIBRARY_PREFIX}jpeg${CMAKE_STATIC_LIBRARY_SUFFIX})
  endif()

  ExternalProject_Add(
    ${OPENCV_PROJECT}
    GIT_REPOSITORY      "https://github.com/opencv/opencv.git"
    GIT_TAG             ${OPENCV_VER}
    PREFIX              ${OPENCV_SOURCES_DIR}
    CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_DIR}
    CMAKE_ARGS          -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    CMAKE_ARGS          -DINSTALL_C_EXAMPLES=OFF
    CMAKE_ARGS          -DINSTALL_PYTHON_EXAMPLES=OFF
    CMAKE_ARGS          -DWITH_TBB=OFF
    CMAKE_ARGS          -DWITH_V4L=OFF
    CMAKE_ARGS          -DWITH_QT=OFF
    CMAKE_ARGS          -DWITH_OPENGL=OFF
    CMAKE_ARGS          -DBUILD_EXAMPLES=OFF
    CMAKE_ARGS          -DWITH_CUDA=OFF
    CMAKE_ARGS          -DOPENCV_FORCE_3RDPARTY_BUILD=OFF
    CMAKE_ARGS          -DWITH_IPP=OFF
    CMAKE_ARGS          -DENABLE_AVX2=ON
    CMAKE_ARGS          -DWITH_JPEG=ON
    CMAKE_ARGS          -DBUILD_JPEG=OFF
    CMAKE_ARGS          -DJPEG_INCLUDE_DIR=${LIBJPEGTURBO_INC_DIR}
    CMAKE_ARGS          -DJPEG_LIBRARY=${LIBJPEGTURBO_LIB_DIR}/${JPEG_OUTPUTNAME}
  )
else()
  ExternalProject_Add(
    ${OPENCV_PROJECT}
    GIT_REPOSITORY      "https://github.com/opencv/opencv.git"
    GIT_TAG             ${OPENCV_VER}
    PREFIX              ${OPENCV_SOURCES_DIR}
    CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_DIR}
    CMAKE_ARGS          -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    CMAKE_ARGS          -DINSTALL_C_EXAMPLES=OFF
    CMAKE_ARGS          -DINSTALL_PYTHON_EXAMPLES=OFF
    CMAKE_ARGS          -DWITH_TBB=OFF
    CMAKE_ARGS          -DWITH_V4L=OFF
    CMAKE_ARGS          -DWITH_QT=OFF
    CMAKE_ARGS          -DWITH_OPENGL=OFF
    CMAKE_ARGS          -DBUILD_EXAMPLES=OFF
    CMAKE_ARGS          -DWITH_CUDA=OFF
    CMAKE_ARGS          -DOPENCV_FORCE_3RDPARTY_BUILD=OFF
    CMAKE_ARGS          -DWITH_IPP=OFF
    CMAKE_ARGS          -DENABLE_AVX2=ON
    CMAKE_ARGS          -DWITH_JPEG=ON
  )
endif()

include_directories(${OPENCV_INC_DIR})
link_directories(${OPENCV_LIB_DIR})

add_executable(${CMAKE_PROJECT_NAME} opencv_decode.cpp)

if(WITH_JPEGTURBO)
  add_dependencies(${CMAKE_PROJECT_NAME} ${LIBJPEGTURBO_PROJECT})
endif()

add_dependencies(${CMAKE_PROJECT_NAME} ${OPENCV_PROJECT})

target_link_libraries(${CMAKE_PROJECT_NAME} pthread opencv_core opencv_imgcodecs)

# Build summary
message(STATUS "")
message(STATUS "============== Build Summary ==============")
message(STATUS "OpenCV version: ${OPENCV_VER}")
message(STATUS "OpenCV src: ${OPENCV_SOURCES_DIR}")
message(STATUS "Use libjpeg-turbo lib: ${WITH_JPEGTURBO}")
message(STATUS "===========================================")
message(STATUS "")
