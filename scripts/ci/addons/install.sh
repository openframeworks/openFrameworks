#!/usr/bin/env bash
set -e

export OF_ROOT=~/openFrameworks

cd $OF_ROOT
if [ -f scripts/ci/$TARGET/install.sh ]; then
    scripts/ci/$TARGET/install.sh;
fi
if [ "$OF_BRANCH" == "master" ]; then
    if [ "$TARGET" == "linux64" ]; then
        scripts/dev/download_libs.sh -a 64;
    elif [ "$TARGET" == "linuxarmv6l" ]; then
        scripts/linux/download_libs.sh -a armv6l;
    elif [ "$TARGET" == "linuxarmv7l" ]; then
        scripts/linux/download_libs.sh -a armv7l;
    elif [ "$TARGET" == "tvos" ]; then
        scripts/ios/download_libs.sh;
    else
        scripts/$TARGET/download_libs.sh;
    fi
fi

cd $TRAVIS_BUILD_DIR
if [ -f scripts/ci/install.sh ]; then
    scripts/ci/install.sh;
fi
if [ -f scripts/ci/$TARGET/install.sh ]; then
    scripts/ci/$TARGET/install.sh;
fi

cd ~
mv $TRAVIS_BUILD_DIR $OF_ROOT/addons/
mkdir -p $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/

cd $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/
if [ "$TARGET" == "android" ]; then
    mkdir armv7;
    mkdir x86;

    cd armv7;
    wget http://ci.openframeworks.cc/openFrameworks_libs/$TARGET/armv7/libopenFrameworksDebug.a;
    cd ../x86;
    wget http://ci.openframeworks.cc/openFrameworks_libs/$TARGET/x86/libopenFrameworksDebug.a;
    cd ..;
elif [ "$TARGET" == "emscripten" ]; then
    wget http://ci.openframeworks.cc/openFrameworks_libs/$TARGET/libopenFrameworksDebug.bc;
else
    wget http://ci.openframeworks.cc/openFrameworks_libs/$TARGET/libopenFrameworksDebug.a;
fi
