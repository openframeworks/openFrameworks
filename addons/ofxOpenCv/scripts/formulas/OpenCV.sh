#! /bin/bash
#
# OpenCV
# library of programming functions mainly aimed at real-time computer vision
# http://opencv.org
#
# uses a CMake build system
 
FORMULA_TYPES=( "osx" "ios" )
 
# define the version
VER=2.4.9
 
# tools for git use
GIT_URL=https://github.com/Itseez/opencv.git
GIT_TAG=$VER

# these paths don't really matter - they are set correctly further down
local LIB_FOLDER="$BUILD_ROOT_DIR/OpenCv"
local LIB_FOLDER32="$LIB_FOLDER-32"
local LIB_FOLDER64="$LIB_FOLDER-64"
local LIB_FOLDER_IOS="$LIB_FOLDER-IOS"
local LIB_FOLDER_IOS_SIM="$LIB_FOLDER-IOSIM"

 
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
      -DCMAKE_C_FLAGS="-fvisibility-inlines-hidden -stdlib=libc++ -O3" \
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
      -DCMAKE_C_FLAGS="-fvisibility-inlines-hidden -stdlib=libstdc++ -O3" \
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

function build_ios() {

  iosSDK="7.1"
  iosPlatform="iPhoneOS"
  arch="armv7"
  isSimulator="FALSE";
  effectivePlatforms="-iphoneos;"

  targetFolder=$LIB_FOLDER_IOS

  if [ "$1" == "i386" ] ; then
    arch="i386"
    iosPlatform="iPhoneSimulator"
    targetFolder=$LIB_FOLDER_IOS_SIM
    isSimulator="TRUE"
    effectivePlatforms="-iphonesimulator;"
  fi

  iosPlatSDK="$iosPlatform$iosSDK.sdk"

  #-miphoneos-version-min is REALLY needed to cmake allows you to build for simulator

    cmake . -DCMAKE_INSTALL_PREFIX=$targetFolder \
      -DIOS=1 \
      -DAPPLE=1 \
      -DUNIX=1 \
      -DIPHONESIMULATOR="TRUE" \
      -DCMAKE_CXX_COMPILER_WORKS="TRUE" \
      -DCMAKE_C_COMPILER_WORKS="TRUE" \
      -DSDKVER="$iosSDK" \
      -DCMAKE_IOS_DEVELOPER_ROOT="/Applications/Xcode.app/Contents/Developer/Platforms/$iosPlatform.platform/Developer" \
      -DDEVROOT="/Applications/Xcode.app/Contents/Developer/Platforms/$iosPlatform.platform/Developer" \
      -DSDKROOT="/Applications/Xcode.app/Contents/Developer/Platforms/$iosPlatform.platform/Developer/SDKs/$iosPlatSDK" \
      -DCMAKE_OSX_SYSROOT="/Applications/Xcode.app/Contents/Developer/Platforms/$iosPlatform.platform/Developer/SDKs/$iosPlatSDK" \
      -DCMAKE_OSX_ARCHITECTURES="$arch" \
      -DCMAKE_XCODE_EFFECTIVE_PLATFORMS="$effectivePlatforms" \
      -DGLFW_BUILD_UNIVERSAL=ON \
      -DENABLE_FAST_MATH=ON \
      -DCMAKE_CXX_FLAGS="-stdlib=libc++ -fvisibility=hidden -fvisibility-inlines-hidden -DNDEBUG -O3 -fomit-frame-pointer -ffast-math -miphoneos-version-min=5.0" \
      -DCMAKE_C_FLAGS="-stdlib=libc++ -fvisibility=hidden -fvisibility-inlines-hidden -DNDEBUG -O3 -fomit-frame-pointer -ffast-math -miphoneos-version-min=5.0" \
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

  make clean
  make
  make install
}


function make_universal_binary() {
  shopt -s nullglob

  src1="$1"
  src2="$2"
  dst="$3"

  libs="$src1/lib/libopencv*.a"

  for lib in $libs
  do
    fname=$(basename "$lib"); 
    otherLib="$src2/lib/$fname"
    echo "lipoing $fname"
    lipo -create $lib $otherLib -o "$dst/lib/$fname" || true
  done

  thirdparty="$src1/share/OpenCV/3rdparty/lib/*.a"

  for lib in $thirdparty
  do
    fname=$(basename "$lib"); 
    otherLib="$src2/share/OpenCV/3rdparty/lib/$fname"
    echo "lipoing $fname"
    lipo -create $lib $otherLib -o "$dst/lib/$fname" || true
  done

  outputlist="$dst/lib/lib*.a"

  libtool -static $outputlist -o "$dst/lib/opencv.a"
}

# executed inside the lib src dir
function build() {
  rm -f CMakeCache.txt
 
  LIB_FOLDER="$BUILD_ROOT_DIR/$TYPE/FAT/OpenCv"
  LIB_FOLDER32="$BUILD_ROOT_DIR/$TYPE/32bit/OpenCv"
  LIB_FOLDER64="$BUILD_ROOT_DIR/$TYPE/64bit/OpenCv"

  if [ "$TYPE" == "osx" ] ; then
    # 64-bit OF transition, build x86-64 and i386 separately
    rm -f CMakeCache.txt
    build_osx "64";
    rm -f CMakeCache.txt
    build_osx "32";

	mkdir -p $LIB_FOLDER/include
	mkdir -p $LIB_FOLDER/lib
 
    # lipo shit together
    make_universal_binary "$LIB_FOLDER64" "$LIB_FOLDER32" "$LIB_FOLDER" 

    # copy headers
    cp -R $LIB_FOLDER64/include/opencv $LIB_FOLDER/include/opencv
    cp -R $LIB_FOLDER64/include/opencv2 $LIB_FOLDER/include/opencv2
  fi

  if [ "$TYPE" == "ios" ] ; then

    local LIB_FOLDER_IOS="$BUILD_ROOT_DIR/$TYPE/iOS/OpenCv"
    local LIB_FOLDER_IOS_SIM="$BUILD_ROOT_DIR/$TYPE/iOS_SIMULATOR/OpenCv"

    # 64-bit OF transition, build x86-64 and i386 separately
    rm -f CMakeCache.txt
    build_ios "i386";

    rm -f CMakeCache.txt
    build_ios "armv7";

	mkdir -p $LIB_FOLDER/include
	mkdir -p $LIB_FOLDER/lib

    make_universal_binary "$LIB_FOLDER_IOS" "$LIB_FOLDER_IOS_SIM" "$LIB_FOLDER"

    # copy headers
    cp -R $LIB_FOLDER_IOS/include/opencv $LIB_FOLDER/include/opencv
    cp -R $LIB_FOLDER_IOS/include/opencv2 $LIB_FOLDER/include/opencv2

  fi

}
 
# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

  LIB_FOLDER="$BUILD_ROOT_DIR/$TYPE/FAT/OpenCv"

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

  if [ "$TYPE" == "ios" ] ; then
    # Standard *nix style copy.
    # copy headers
    cp -R $LIB_FOLDER/include/ $1/include/
 
    # copy lib
    cp -R $LIB_FOLDER/lib/opencv.a $1/lib/$TYPE/
  fi

  # copy license files
  rm -rf $1/license # remove any older files if exists
  mkdir -p $1/license
  cp -v LICENSE $1/license/

}
 
# executed inside the lib src dir
function clean() {
  if [ "$TYPE" == "osx" ] ; then
    make clean;
  fi
}
