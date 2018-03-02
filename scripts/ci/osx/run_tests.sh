#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# source $ROOT/scripts/ci/ccache.sh
export CXX="$(which ccache) $(xcodebuild -find clang++)"
export CC="$(which ccache) $(xcodebuild -find clang)"

trap 'for f in ~/Library/Logs/DiagnosticReports/*; do cat $f; done' 11

echo "**** Building emptyExample ****"
cd $ROOT
cp scripts/templates/osx/Makefile examples/templates/emptyExample/
cp scripts/templates/osx/config.make examples/templates/emptyExample/
cd examples/templates/emptyExample/
make -j2 Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/osx/Makefile examples/templates/allAddonsExample/
cp scripts/templates/osx/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
make -j2 Debug

echo "**** Running unit tests ****"
cd $ROOT/tests
for group in *; do
    if [ -d $group ]; then
        for test in $group/*; do
            if [ -d $test ]; then
                cd $test
                cp ../../../scripts/templates/osx/Makefile .
                cp ../../../scripts/templates/osx/config.make .
                make -j2 Debug
                binname=$(basename ${test})

                if [ "${binname}" == "networkTcp" ] || [ "${binname}" == "networkUdp" ]; then
                    counter=0
                    errorcode=1
                    cd bin/${binname}_debug.app/Contents/MacOS/
                    while [ $counter -lt 5 ] && [ $errorcode -ne 0 ]
                    do
                        echo "Running ${text} $counter"
                        #sudo gdb -batch -ex "run" -ex "bt" -ex "q \$_exitcode" ./${binname}_debug
                        ./${binname}_debug
                        errorcode=$?
                        counter=$[$counter +1]
                    done
                    if [[ $errorcode -ne 0 ]]; then
                        exit $errorcode
                    fi
                else
                    cd bin/${binname}_debug.app/Contents/MacOS/
                    #sudo gdb -batch -ex "run" -ex "bt" -ex "q \$_exitcode" ./${binname}_debug
                    ./${binname}_debug
                    errorcode=$?
                    if [[ $errorcode -ne 0 ]]; then
                        exit $errorcode
                    fi
                fi
                cd $ROOT/tests
            fi
        done
    fi
done
