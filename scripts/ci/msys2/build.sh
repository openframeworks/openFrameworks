#!/bin/bash
set -ev
ROOT="$PWD"

echo "##[group]**** Building OF core ****"
cd $ROOT/libs/openFrameworksCompiled/project
make -j2 -s Debug
echo "##[endgroup]"

echo "##[group]**** Building emptyExample ****"
cd $ROOT/scripts/templates/msys2
make -j2 -s Debug
echo "##[endgroup]"

echo "##[group]**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/msys2/Makefile examples/templates/allAddonsExample/
cp scripts/templates/msys2/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make -j2 -s Debug
echo "##[endgroup]"