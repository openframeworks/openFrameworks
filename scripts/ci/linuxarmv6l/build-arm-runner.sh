#!/bin/bash
set -ev
pwd
OF_ROOT=$( cd "$(dirname "$0")/../../.." ; pwd -P )
PROJECTS=$OF_ROOT/libs/openFrameworksCompiled/project

# Add compiler flag to reduce memory usage to enable builds to complete
# see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56746#c7
# the "proper" way does not work currently:
export CXXFLAGS="${CXXFLAGS} -ftrack-macro-expansion=0"

echo "**** Building emptyExample ****"
cd $OF_ROOT/scripts/templates/linuxarmv6l
make Debug -j4

echo "**** Building allAddonsExample ****"
cd $OF_ROOT
cp scripts/templates/linuxarmv6l/Makefile examples/templates/allAddonsExample/
cp scripts/templates/linuxarmv6l/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make Debug -j4
