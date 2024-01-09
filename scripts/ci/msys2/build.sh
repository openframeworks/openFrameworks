#!/bin/bash
set -ev
ROOT="$PWD"

echo "**** Building OF core ****"
cd $ROOT/libs/openFrameworksCompiled/project
make -j -s Debug

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/msys2
make -j -s Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/msys2/Makefile examples/templates/allAddonsExample/
cp scripts/templates/msys2/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make -j -s Debug
