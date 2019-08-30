#!/bin/sh
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/kbinias/dev/opencv/build/installation/OpenCV/lib"
/home/kbinias/dev/tests/opencv_decode/opencv_decode 18 '/data/dataset/imagenet/val/*'
