#!/bin/bash
set -ev
echo "Building openFrameworks - OSX Template Project"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
xcodebuild -configuration Release -target emptyExample -project "$ROOT/scripts/osx/template/emptyExample.xcodeproj"

echo "Unit tests"
cd $ROOT/tests
for group in *; do
	if [ -d $group ]; then
		for test in $group/*; do
			if [ -d $test ]; then
				cd $test
				cp ../../../osx/template/Makefile .
				cp ../../../osx/template/config.make . 
				make Debug
				make RunDebug 
			fi
		done
	fi
done