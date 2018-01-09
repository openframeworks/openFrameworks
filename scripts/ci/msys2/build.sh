#!/bin/bash
set -ev
ROOT="$PWD"
source $ROOT/scripts/ci/ccache.sh

echo "**** Building OF core ****"
cd $ROOT/libs/openFrameworksCompiled/project
make USE_CCACHE=1 -j4 Debug

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/linux64
make USE_CCACHE=1 -j4 Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/linux64/Makefile examples/templates/allAddonsExample/
cp scripts/templates/linux64/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make USE_CCACHE=1 -j4 Debug
