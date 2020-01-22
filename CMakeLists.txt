# e.g. cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_JPEGTURBO=OFF -DOPENCV_VER=4.2.0 -DEXPROJ_CV_SOURCE_DIR=/home/username/dev/tests/3rdparty/opencv/

cmake_minimum_required(VERSION 3.1)

project(opencv_decode C CXX)

option(WITH_JPEGTURBO "Build with libjpeg-turbo library" ON)

if (NOT OPENCV_VER)
  set(OPENCV_VER "4.2.0")
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

# CV version greater or equals to 3
if(NOT (${OPENCV_VER} VERSION_LESS 3))
    add_definitions(-DOPENCV_LEGACY_MODE)
endif()

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
if(NOT (${OPENCV_VER} VERSION_LESS 3))
  SET(OPENCV_INC_DIR        "${OPENCV_INSTALL_DIR}/include/opencv4" CACHE PATH "OpenCV include directory." FORCE)
else()
  SET(OPENCV_INC_DIR        "${OPENCV_INSTALL_DIR}/include" CACHE PATH "OpenCV include directory." FORCE)
endif()
SET(OPENCV_LIB_DIR        "${OPENCV_INSTALL_DIR}/lib" CACHE PATH "OpenCV library directory." FORCE)

INCLUDE(ExternalProject)

# Arguments for ExternalProject_Add
list(APPEND CMAKE_ARGS "GIT_TAG:string=\"${OPENCV_VER}\"")
list(APPEND CMAKE_ARGS "PREFIX:string=\"${OPENCV_SOURCES_DIR}\"")
list(APPEND CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:string=\"${OPENCV_INSTALL_DIR}\"")
list(APPEND CMAKE_ARGS "-DCMAKE_BUILD_TYPE:string=\"${CMAKE_BUILD_TYPE}\"")
list(APPEND CMAKE_ARGS "-DINSTALL_C_EXAMPLES:bool=OFF")
list(APPEND CMAKE_ARGS "-DINSTALL_PYTHON_EXAMPLES:bool=OFF")
list(APPEND CMAKE_ARGS "-DWITH_TBB:bool=OFF")
list(APPEND CMAKE_ARGS "-DWITH_V4L:bool=OFF")
list(APPEND CMAKE_ARGS "-DWITH_QT:bool=OFF")
list(APPEND CMAKE_ARGS "-DWITH_OPENGL:bool=OFF")
list(APPEND CMAKE_ARGS "-DBUILD_EXAMPLES:bool=OFF")
list(APPEND CMAKE_ARGS "-DWITH_CUDA:bool=OFF")
list(APPEND CMAKE_ARGS "-DOPENCV_FORCE_3RDPARTY_BUILD:bool=OFF")
list(APPEND CMAKE_ARGS "-DWITH_IPP:bool=OFF")
list(APPEND CMAKE_ARGS "-DENABLE_AVX2:bool=ON")
list(APPEND CMAKE_ARGS "-DWITH_JPEG:bool=ON")
list(APPEND CMAKE_ARGS "-DOPENCV_GENERATE_PKGCONFIG:bool=ON")

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

  list(APPEND CMAKE_ARGS "-DBUILD_JPEG:bool=OFF")
  list(APPEND CMAKE_ARGS "-DJPEG_INCLUDE_DIR:string=\"${LIBJPEGTURBO_INC_DIR}\"")
  list(APPEND CMAKE_ARGS "-DJPEG_LIBRARY:string=\"${LIBJPEGTURBO_LIB_DIR}/${JPEG_OUTPUTNAME}\"")
endif()

if(EXPROJ_CV_SOURCE_DIR)
  #MESSAGE(STATUS "${CMAKE_ARGS}")
  ExternalProject_Add(
    ${OPENCV_PROJECT}
    SOURCE_DIR ${EXPROJ_CV_SOURCE_DIR}
    CMAKE_ARGS "${CMAKE_ARGS}"
  )
else()
  ExternalProject_Add(
    ${OPENCV_PROJECT}
    GIT_REPOSITORY "https://github.com/opencv/opencv.git"
    CMAKE_ARGS     "${CMAKE_ARGS}"
  )
endif()

include_directories(${OPENCV_INC_DIR})
link_directories(${OPENCV_LIB_DIR})

add_executable(${CMAKE_PROJECT_NAME} opencv_decode.cpp)

if(WITH_JPEGTURBO)
  add_dependencies(${CMAKE_PROJECT_NAME} ${LIBJPEGTURBO_PROJECT})
endif()

add_dependencies(${CMAKE_PROJECT_NAME} ${OPENCV_PROJECT})

# CV version greater or equals to 3
if(NOT (${OPENCV_VER} VERSION_LESS 3))
  target_link_libraries(${CMAKE_PROJECT_NAME} pthread opencv_dnn opencv_gapi opencv_photo opencv_stitching opencv_highgui opencv_video opencv_videoio opencv_imgcodecs opencv_ml opencv_objdetect opencv_calib3d opencv_features2d opencv_imgproc opencv_flann opencv_core)
else()
  target_link_libraries(${CMAKE_PROJECT_NAME} pthread opencv_core opencv_highgui opencv_imgproc)
endif()

# Build summary
message(STATUS "")
message(STATUS "============== Build Summary ==============")
message(STATUS "OpenCV version: ${OPENCV_VER}")
message(STATUS "OpenCV src: ${OPENCV_SOURCES_DIR}")
message(STATUS "Use libjpeg-turbo lib: ${WITH_JPEGTURBO}")
message(STATUS "===========================================")
message(STATUS "")
