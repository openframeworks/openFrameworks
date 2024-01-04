#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# source $ROOT/scripts/ci/ccache.sh

trap 'for f in ~/Library/Logs/DiagnosticReports/*; do cat $f; done' 11

echo "**** Building emptyExample ****"
cd $ROOT
cp scripts/templates/osx/Makefile examples/templates/emptyExample/
cp scripts/templates/osx/config.make examples/templates/emptyExample/
cd examples/templates/emptyExample/
make -j Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/osx/Makefile examples/templates/allAddonsExample/
cp scripts/templates/osx/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make -j Debug

echo "**** Running unit tests ****"
cd $ROOT/tests
for group in *; do
    if [ -d $group ]; then
        for test in $group/*; do
            if [ -d $test ]; then
                cd $test
                cp ../../../scripts/templates/osx/Makefile .
                cp ../../../scripts/templates/osx/config.make .
                make -j Debug
                make RunDebug
				errorcode=$?
				if [[ $errorcode -ne 0 ]]; then
					exit $errorcode
				fi
                cd $ROOT/tests
            fi
        done
    fi
done
