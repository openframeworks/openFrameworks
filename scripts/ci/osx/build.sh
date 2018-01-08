#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# source $ROOT/scripts/ci/ccache.sh
CCACHE=$(which ccache)
CLANGXX=$(xcodebuild -find clang++)
CLANG=$(xcodebuild -find clang)
cat >$PWD/clang++ <<< EOF
     $CCACHE $CLANGXX
EOF
cat >$PWD/clang <<< EOF2
    $CCACHE $CLANG
EOF2

chmod 755 clang++
chmod 755 clang

export CXX="$PWD/clang++"
export CC="$PWD/clang"
echo "**** Building oF + emptyExample - OSX Template Project ****"
xcodebuild -configuration Release -target emptyExample -project "$ROOT/scripts/templates/osx/emptyExample.xcodeproj"
echo "**** Done building emptyExample ****"
