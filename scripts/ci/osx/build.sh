#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# source $ROOT/scripts/ci/ccache.sh
cat >$PWD/clang++ <<< EOF
    $(which ccache) $(xcodebuild -find clang++)
EOF
cat >$PWD/clang <<< EOF
    $(which ccache) $(xcodebuild -find clang)
EOF

chmod 755 clang++
chmod 755 clang

export CXX="$PWD/clang++"
export CC="$PWD/clang"
echo "**** Building oF + emptyExample - OSX Template Project ****"
xcodebuild -configuration Release -target emptyExample -project "$ROOT/scripts/templates/osx/emptyExample.xcodeproj"
echo "**** Done building emptyExample ****"
