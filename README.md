## Build instructions

Clone application to your directory:

`git clone https://github.com/kbinias/opencv_decode.git`

`export ROOT_DIR="your_directory/opencv_decode"`

e.g. `export ROOT_DIR="${HOME}/dev/opencv_decode"`

### With CMake

`mkdir build`
`cd build`
`cmake .. -DCMAKE_BUILD_TYPE=Release`
`make -j`

### With Make

* Build the latest version of libjpeg-turbo

`sudo apt install nasm`

`git clone https://github.com/libjpeg-turbo/libjpeg-turbo.git $ROOT_DIR/build/3rdparty/libjpeg-turbo`

`mkdir $ROOT_DIR/build/3rdparty/libjpeg-turbo/build`

`cmake -B${ROOT_DIR}/build/3rdparty/libjpeg-turbo/build -H${ROOT_DIR}/build/3rdparty/libjpeg-turbo -DCMAKE_INSTALL_PREFIX=${ROOT_DIR}/build/3rdparty/install/libjpeg-turbo -DCMAKE_BUILD_TYPE=Release -DENABLE_STATIC=TRUE -DCMAKE_INSTALL_DEFAULT_LIBDIR=lib -DCMAKE_POSITION_INDEPENDENT_CODE=ON`

`make -j -C ${ROOT_DIR}/build/3rdparty/libjpeg-turbo/build`

`make install -C ${ROOT_DIR}/build/3rdparty/libjpeg-turbo/build`

* Build OpenCV 3.4.7

`git clone --branch 3.4.7 https://github.com/opencv/opencv.git $ROOT_DIR/build/3rdparty/opencv`

`mkdir $ROOT_DIR/build/3rdparty/opencv/build`

`cmake -DCMAKE_BUILD_TYPE=Release -B${ROOT_DIR}/build/3rdparty/opencv/build -H${ROOT_DIR}/build/3rdparty/opencv -DCMAKE_INSTALL_PREFIX=${ROOT_DIR}/build/3rdparty/install/opencv -DINSTALL_C_EXAMPLES=OFF -DINSTALL_PYTHON_EXAMPLES=OFF -DWITH_TBB=ON -DWITH_V4L=ON -DOPENCV_PYTHON3_INSTALL_PATH=$cwd/OpenCV-$cvVersion-py3/lib/python3.5/site-packages -DWITH_QT=OFF -DWITH_OPENGL=OFF -DBUILD_EXAMPLES=OFF -DWITH_CUDA=OFF -DOPENCV_FORCE_3RDPARTY_BUILD=OFF -DWITH_IPP=ON -DENABLE_AVX2=ON -DWITH_JPEG=ON -DBUILD_JPEG=OFF -DJPEG_INCLUDE_DIR=${ROOT_DIR}/build/3rdparty/install/libjpeg-turbo/include/ -DJPEG_LIBRARY=${ROOT_DIR}/build/3rdparty/install/libjpeg-turbo/lib/libjpeg.a`

`make -j -C ${ROOT_DIR}/build/3rdparty/opencv/build`

`make install -C ${ROOT_DIR}/build/3rdparty/opencv/build`

* Build opencv_decode

`cd opencv_decode`

`g++ -std=c++11 -Wall opencv_decode.cpp -o build/opencv_decode -pthread -lopencv_core -lopencv_imgcodecs -I${ROOT_DIR}/build/3rdparty/install/opencv/include -L${ROOT_DIR}/build/3rdparty/install/opencv/lib`

### Test application
`LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${ROOT_DIR}/build/3rdparty/install/opencv/lib ./build/opencv_decode 18 '/data/dataset/imagenet/val/*'`
