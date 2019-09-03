#!/bin/sh
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${ROOT_DIR}/build/3rdparty/install/opencv/lib"
${ROOT_DIR}/build/opencv_decode 18 '/data/dataset/imagenet/val/*'
