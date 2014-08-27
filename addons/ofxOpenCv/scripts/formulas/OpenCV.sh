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
local LIB_FOLDER=""
local LIB_FOLDER_OUT="" 
 
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

  arch="$1"
  targetFolder="$LIB_FOLDER/$arch/"
  libType="libc++" 
  deployTarget="10.7"

  if [ "$arch" == "i386" ] ; then
    libType="libstdc++" 
    deployTarget="10.6" 
  fi
  
    cmake . -DCMAKE_INSTALL_PREFIX="$targetFolder" \
      -DGLFW_BUILD_UNIVERSAL=ON \
      -DCMAKE_OSX_DEPLOYMENT_TARGET="$deployTarget" \
      -DCMAKE_OSX_ARCHITECTURES="$arch" \
      -DENABLE_FAST_MATH=ON \
      -DCMAKE_CXX_FLAGS="-fvisibility-inlines-hidden -stdlib=$libType -O3" \
      -DCMAKE_C_FLAGS="-fvisibility-inlines-hidden -stdlib=$libType -O3" \
      -DCMAKE_BUILD_TYPE="Release" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_DOCS=OFF \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_FAT_JAVA_LIB=OFF \
      -DBUILD_JASPER=OFF \
      -DBUILD_PACKAGE=OFF \
      -DBUILD_JPEG=OFF \
      -DBUILD_TIFF=OFF \
      -DBUILD_PNG=OFF \
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
      -DWITH_JPEG=OFF \
      -DWITH_TIFF=OFF \
      -DWITH_PNG=OFF \
      -DWITH_QUICKTIME=OFF \
      -DWITH_V4L=OFF \
      -DWITH_PVAPI=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_PERF_TESTS=OFF
 
  make clean
  make
  make install

  # copy headers
  cp -R "$targetFolder/include/opencv" $LIB_FOLDER_OUT/include/opencv
  cp -R "$targetFolder/include/opencv2" $LIB_FOLDER_OUT/include/opencv2
}

function build_ios() {

  iosSDK="7.1"
  iosPlatform="iPhoneOS"
  arch="$1"
  isSimulator="FALSE";
  effectivePlatforms="-iphoneos;"
  minOS="5.0"

  targetFolder="$LIB_FOLDER/$arch/"

  if [ "$arch" == "i386" ] || [ "$arch" == "x86_64" ] ; then
    iosPlatform="iPhoneSimulator"
    isSimulator="TRUE"
    effectivePlatforms="-iphonesimulator;"
  fi
  if [ "$arch" == "arm64" ]; then
    minOS="7.0"
  fi

  iosPlatSDK="$iosPlatform$iosSDK.sdk"

  #-miphoneos-version-min in CXX AND C FLAGS REALLY needed to cmake allows you to build for simulator

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
      -DCMAKE_CXX_FLAGS="-stdlib=libc++ -fvisibility=hidden -fvisibility-inlines-hidden -DNDEBUG -O3 -fomit-frame-pointer -ffast-math -miphoneos-version-min=$minOS" \
      -DCMAKE_C_FLAGS="-stdlib=libc++ -fvisibility=hidden -fvisibility-inlines-hidden -DNDEBUG -O3 -fomit-frame-pointer -ffast-math -miphoneos-version-min=$minOS" \
      -DCMAKE_BUILD_TYPE="Release" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_DOCS=OFF \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_FAT_JAVA_LIB=OFF \
      -DBUILD_JASPER=OFF \
      -DBUILD_PACKAGE=OFF \
      -DBUILD_TIFF=OFF \
      -DBUILD_JPEG=OFF \
      -DBUILD_PNG=OFF \
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
      -DWITH_JPEG=OFF \
      -DWITH_TIFF=OFF \
      -DWITH_PNG=OFF \
      -DWITH_QUICKTIME=OFF \
      -DWITH_V4L=OFF \
      -DWITH_PVAPI=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_PERF_TESTS=OFF

  make clean
  make
  make install

  # copy headers
  cp -R "$targetFolder/include/opencv" $LIB_FOLDER_OUT/include/opencv
  cp -R "$targetFolder/include/opencv2" $LIB_FOLDER_OUT/include/opencv2
}

#first argument is the destination
#second argument is a list of src architectures
function make_universal_binary() {
  shopt -s nullglob

  dst="$1"
  shift
  src_archs=("${@}")
  src_list="$LIB_FOLDER/$src_archs"

  libs="$src_list/lib/lib*.a"
  thirdparty="$src_list/share/OpenCV/3rdparty/lib/*.a"

  allArchs=""
  libsToLipo=""

  echo "src_list $libs"

  for arch in "${src_archs[@]}"
  do
    allArchs=""

    for lib in $libs
    do
        fname=$(basename "$lib")
        allArchs="$allArchs$LIB_FOLDER/$arch/lib/$fname "
    done

    for lib in $thirdparty
    do
        fname=$(basename "$lib")
        allArchs="$allArchs$LIB_FOLDER/$arch/share/OpenCV/3rdparty/lib/$fname "
    done

    echo "libtooling $arch"

    command="libtool -static $allArchs -o $LIB_FOLDER/$arch/lib/opencv.a"
    $command || true
    libsToLipo="$libsToLipo$LIB_FOLDER/$arch/lib/opencv.a "
  done

  command="lipo -create $libsToLipo -o $dst/lib/opencv.a"
  $command || true
}

# executed inside the lib src dir
function build() {

  LIB_FOLDER="$BUILD_ROOT_DIR/$TYPE/OpenCv"
  LIB_FOLDER_OUT="$LIB_FOLDER/FAT"

  if [ "$TYPE" == "osx" ] ; then
	mkdir -p $LIB_FOLDER_OUT/include
	mkdir -p $LIB_FOLDER_OUT/lib

    archs=("i386" "x86_64")

    for curArch in "${archs[@]}"
    do
        rm -f CMakeCache.txt
        build_osx "$curArch"
    done

    make_universal_binary "$LIB_FOLDER_OUT" "${archs[@]}"

  fi

  if [ "$TYPE" == "ios" ] ; then

    mkdir -p $LIB_FOLDER_OUT/include
    mkdir -p $LIB_FOLDER_OUT/lib

    archs=("armv7" "armv7s" "arm64" "i386" "x86_64")

    for curArch in "${archs[@]}"
    do
        rm -f CMakeCache.txt
        build_ios "$curArch"
    done

    make_universal_binary "$LIB_FOLDER_OUT" "${archs[@]}"
  fi

}
 
# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

  LIB_FOLDER="$BUILD_ROOT_DIR/$TYPE/OpenCv"
  LIB_FOLDER_OUT="$LIB_FOLDER/FAT"

  # prepare headers directory if needed
  mkdir -p $1/include
 
  # prepare libs directory if needed
  mkdir -p $1/lib/$TYPE
 
  if [ "$TYPE" == "osx" ] || [ "$TYPE" == "ios" ] ; then
    # Standard *nix style copy.
    # copy headers
    cp -R $LIB_FOLDER_OUT/include/ $1/include/
 
    # copy lib
    cp -R $LIB_FOLDER_OUT/lib/ $1/lib/$TYPE/
  fi


}
 
# executed inside the lib src dir
function clean() {
  if [ "$TYPE" == "osx" ] ; then
    make clean;
  fi
}