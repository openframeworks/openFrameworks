#!/bin/bash
#set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}

trap 'for f in ~/Library/Logs/DiagnosticReports/*; do cat $f; done' 11

echo "##[group]**** Building emptyExample ****"
cd $ROOT
cd examples/templates/emptyExample/
cp ../../../scripts/templates/macos/{Makefile,config.make} .
make -j2 Debug
echo "##[endgroup]"

echo "##[group]**** Building allAddonsExample ****"
cd $ROOT
cd examples/templates/allAddonsExample/
cp ../../../scripts/templates/macos/{Makefile,config.make} .
make -j2 Debug
echo "##[endgroup]"

echo "##[group]**** Running unit tests ****"
cd $ROOT/tests
for group in *; do
    if [ -d $group ]; then
        echo "##[group] $group"
        for test in $group/*; do
            if [ -d $test ]; then
                cd $test
                cp ../../../scripts/templates/macos/{Makefile,config.make} .
                # cp ../../../scripts/templates/osx/Makefile .
                # cp ../../../scripts/templates/osx/config.make .
                make -j2 Debug
                make RunDebug
				errorcode=$?
				if [[ $errorcode -ne 0 ]]; then
					exit $errorcode
				fi
                cd $ROOT/tests
            fi
        done
        echo "##[endgroup]"
    fi
done
echo "##[endgroup]"