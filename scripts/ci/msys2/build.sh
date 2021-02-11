#!/bin/bash
set -ev
ROOT="$PWD"
source $ROOT/scripts/ci/ccache.sh

echo "**** Building OF core ****"
cd $ROOT/libs/openFrameworksCompiled/project
make ${USE_CCACHE} -j4 Debug

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/msys2
make ${USE_CCACHE} -j4 Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/msys2/Makefile examples/templates/allAddonsExample/
cp scripts/templates/msys2/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/

# this is maybe a fix for some weird issues where the linker runs out of address space
echo "PROJECT_CFLAGS = -fpic -mmode=medium" >> config.make
echo "PROJECT_OPTIMIZATION_CFLAGS_DEBUG = -Os" >> config.make

make ${USE_CCACHE} -j4 Debug
