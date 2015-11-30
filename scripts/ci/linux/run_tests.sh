#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}

echo "**** Running unit tests ****"
cd $ROOT/tests
for group in *; do
	if [ -d $group ]; then
		for test in $group/*; do
			if [ -d $test ]; then
				cd $test
				cp ../../../scripts/templates/linux/Makefile .
				cp ../../../scripts/templates/linux/config.make .
				make Debug
				cd bin
				./${test}_debug
				cd $ROOT/tests
			fi
		done
	fi
done
