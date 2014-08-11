#! /bin/bash
#
# OpenCV
# library of programming functions mainly aimed at real-time computer vision
# http://opencv.org
#
# uses a CMake build system
 
FORMULA_TYPES=( "osx" )
 
# define the version
VER=2.4.9
 
# tools for git use
GIT_URL=https://github.com/Itseez/opencv.git
GIT_TAG=$VER

local LIB_FOLDER="$BUILD_ROOT_DIR/OpenCv"
local LIB_FOLDER32="$LIB_FOLDER-32"
local LIB_FOLDER64="$LIB_FOLDER-64"

 
# download the source code and unpack it into LIB_NAME
function download() {
  curl -Lk https://github.com/Itseez/opencv/archive/$VER.tar.gz -o opencv-$VER.tar.gz
  tar -xf opencv-$VER.tar.gz
  mv opencv-$VER opencv
  rm opencv*.tar.gz
}
 
# prepare the build environment, executed inside the lib src dir
function prepare() {
  : # noop
}
 
function build_osx() {
  
  if [ "$1" == "64" ] ; then
    cmake . -DCMAKE_INSTALL_PREFIX=$LIB_FOLDER64 \
      -DGLFW_BUILD_UNIVERSAL=ON \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7 \
      -DCMAKE_OSX_ARCHITECTURES="x86_64" \
      -DENABLE_FAST_MATH=ON \
      -DCMAKE_CXX_FLAGS="-fvisibility-inlines-hidden -stdlib=libc++ -O3" \
      -DCMAKE_BUILD_TYPE="Release" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_DOCS=OFF \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_FAT_JAVA_LIB=OFF \
      -DBUILD_JASPER=OFF \
      -DBUILD_PACKAGE=OFF \
      -DBUILD_opencv_java=OFF \
      -DBUILD_opencv_python=OFF \
      -DBUILD_opencv_apps=OFF \
      -DWITH_1394=OFF \
      -DWITH_CARBON=OFF \
      -DWITH_FFMPEG=OFF \
      -DWITH_OPENCL=OFF \
      -DWITH_OPENCLAMDBLAS=OFF \
      -DWITH_OPENCLAMDFFT=OFF \
      -DWITH_GIGEAPI=OFF \
      -DWITH_CUDA=OFF \
      -DWITH_CUFFT=OFF \
      -DWITH_JASPER=OFF \
      -DWITH_LIBV4L=OFF \
      -DWITH_IMAGEIO=OFF \
      -DWITH_IPP=OFF \
      -DWITH_OPENNI=OFF \
      -DWITH_QT=OFF \
      -DWITH_QUICKTIME=OFF \
      -DWITH_V4L=OFF \
      -DWITH_PVAPI=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_PERF_TESTS=OFF
  elif [ "$1" == "32" ] ; then
    # NB - using a special BUILD_ROOT_DIR
    cmake . -DCMAKE_INSTALL_PREFIX=$LIB_FOLDER32 \
      -DGLFW_BUILD_UNIVERSAL=ON \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=10.6 \
      -DCMAKE_OSX_ARCHITECTURES="i386" \
      -DENABLE_FAST_MATH=ON \
      -DCMAKE_CXX_FLAGS="-fvisibility-inlines-hidden -stdlib=libstdc++ -O3" \
      -DCMAKE_BUILD_TYPE="Release" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_DOCS=OFF \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_FAT_JAVA_LIB=OFF \
      -DBUILD_JASPER=OFF \
      -DBUILD_PACKAGE=OFF \
      -DBUILD_opencv_java=OFF \
      -DBUILD_opencv_python=OFF \
      -DBUILD_opencv_apps=OFF \
      -DWITH_1394=OFF \
      -DWITH_CARBON=OFF \
      -DWITH_FFMPEG=OFF \
      -DWITH_OPENCL=OFF \
      -DWITH_OPENCLAMDBLAS=OFF \
      -DWITH_OPENCLAMDFFT=OFF \
      -DWITH_GIGEAPI=OFF \
      -DWITH_CUDA=OFF \
      -DWITH_CUFFT=OFF \
      -DWITH_JASPER=OFF \
      -DWITH_LIBV4L=OFF \
      -DWITH_IMAGEIO=OFF \
      -DWITH_IPP=OFF \
      -DWITH_OPENNI=OFF \
      -DWITH_QT=OFF \
      -DWITH_QUICKTIME=OFF \
      -DWITH_V4L=OFF \
      -DWITH_PVAPI=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_PERF_TESTS=OFF
  fi
 
  make clean
  make
  make install
}
 
function make_universal_binary() {
  shopt -s nullglob
  
  libs="$LIB_FOLDER64/lib/libopencv*.a"

  for lib in $libs
  do
    fname=$(basename "$lib"); 
    otherLib="$LIB_FOLDER32/lib/$fname"
    echo "lipoing $fname"
    lipo -create $lib $otherLib -o "$LIB_FOLDER/lib/$fname" || true
  done

  thirdparty="$LIB_FOLDER64/share/OpenCV/3rdparty/lib/*.a"

  for lib in $thirdparty
  do
    fname=$(basename "$lib"); 
    otherLib="$LIB_FOLDER32/share/OpenCV/3rdparty/lib/$fname"
    echo "lipoing $fname"
    lipo -create $lib $otherLib -o "$LIB_FOLDER/lib/$fname" || true
  done

  outputlist="$LIB_FOLDER/lib/lib*.a" 

  libtool -static $outputlist -o "$LIB_FOLDER/lib/opencv.a"
}
 
# executed inside the lib src dir
function build() {
  rm -f CMakeCache.txt
 
  LIB_FOLDER="$BUILD_ROOT_DIR/$TYPE/FAT/OpenCv"
  LIB_FOLDER32="$BUILD_ROOT_DIR/$TYPE/32bit/OpenCv"
  LIB_FOLDER64="$BUILD_ROOT_DIR/$TYPE/64bit/OpenCv"

  if [ "$TYPE" == "osx" ] ; then
    # 64-bit OF transition, build x86-64 and i386 separately
    build_osx "64";
    rm -f CMakeCache.txt
    build_osx "32";

	mkdir -p $LIB_FOLDER/include
	mkdir -p $LIB_FOLDER/lib
 
    # lipo shit together
    make_universal_binary

    # copy headers
    cp -R $LIB_FOLDER64/include/opencv $LIB_FOLDER/include/opencv
    cp -R $LIB_FOLDER64/include/opencv2 $LIB_FOLDER/include/opencv2
  fi
}
 
# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

  LIB_FOLDER="$BUILD_ROOT_DIR/$TYPE/FAT/OpenCv"
  LIB_FOLDER32="$BUILD_ROOT_DIR/$TYPE/32bit/OpenCv"
  LIB_FOLDER64="$BUILD_ROOT_DIR/$TYPE/64bit/OpenCv"

  # prepare headers directory if needed
  mkdir -p $1/include
 
  # prepare libs directory if needed
  mkdir -p $1/lib/$TYPE
 
  if [ "$TYPE" == "osx" ] ; then
    # Standard *nix style copy.
    # copy headers
    cp -R $LIB_FOLDER/include/ $1/include/
 
    # copy lib
    cp -R $LIB_FOLDER/lib/opencv.a $1/lib/$TYPE/
  fi
}
 
# executed inside the lib src dir
function clean() {
  if [ "$TYPE" == "osx" ] ; then
    make clean;
  fi
}