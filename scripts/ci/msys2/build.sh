#!/bin/bash
set -ev
$ROOT=$PWD

echo "**** Building OF core ****"
cd $ROOT/libs/openFrameworksCompiled/project
make Debug

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/linux64
make Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/linux64/Makefile examples/templates/allAddonsExample/
cp scripts/templates/linux64/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make Debug
