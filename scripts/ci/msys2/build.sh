#!/bin/bash
set -ev
ROOT="$PWD"
source $ROOT/scripts/ci/ccache.sh

echo "**** Building OF core ****"
cd $ROOT/libs/openFrameworksCompiled/project
make USE_CCACHE=1 -j4 Debug

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/msys2
make USE_CCACHE=1 -j4 Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/msys2/Makefile examples/templates/allAddonsExample/
cp scripts/templates/msys2/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make USE_CCACHE=1 -j4 Debug
