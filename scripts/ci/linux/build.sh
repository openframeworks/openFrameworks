#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# Add compiler flag to reduce memory usage to enable builds to complete
# see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56746#c7
# the "proper" way does not work currently:
#export CXXFLAGS="$(CXXFLAGS) --param ftrack-macro-expansion=0"
CUSTOMFLAGS="-ftrack-macro-expansion=0"

echo "Memory usage debugging output"
ps aux --sort -rss | head -n20

echo "Building OF core"
cd $ROOT
# this carries over to subsequent compilations of examples
echo "PLATFORM_CFLAGS += $CUSTOMFLAGS" >> libs/openFrameworksCompiled/project/linux64/config.linux64.default.mk
cd libs/openFrameworksCompiled/project
make Debug

echo "Building emptyExample"
cd $ROOT
cp scripts/linux/template/linux64/Makefile examples/empty/emptyExample/
cp scripts/linux/template/linux64/config.make examples/empty/emptyExample/
cd examples/empty/emptyExample
make Debug

# skip this for now as too many parts don't work (due to 12.04?)
#echo "Building allAddonsExample"
#cd $ROOT
#cp scripts/linux/template/linux64/Makefile examples/addons/allAddonsExample/
#cp scripts/linux/template/linux64/config.make examples/addons/allAddonsExample/
#cd examples/addons/allAddonsExample/
#make Debug

echo "Unit tests"
cd $ROOT/tests
for group in *; do
	if [ -d $group ]; then
		for test in $group/*; do
			if [ -d $test ]; then
				cd $test
				cp ../../../scripts/linux/template/linux/Makefile .
				cp ../../../scripts/linux/template/linux/config.make . 
				make Debug
				make RunDebug 
			fi
		done
	fi
done