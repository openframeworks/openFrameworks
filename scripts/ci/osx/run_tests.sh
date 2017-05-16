#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}

trap 'for f in ~/Library/Logs/DiagnosticReports/*; do cat $f; done' 11

echo "**** Running unit tests ****"
cd $ROOT/tests
for group in *; do
    if [ -d $group ]; then
        for test in $group/*; do
            if [ -d $test ]; then
                cd $test
                cp ../../../scripts/templates/osx/Makefile .
                cp ../../../scripts/templates/osx/config.make .
                make Debug
                binname=$(basename ${test})

                if [ "${test}" == "networkTcp" ] || [ "${test}" == "networkUdp" ]; then
                    counter=0
                    errorcode=1
                    while [ $counter -lt 5 ] && [ $errorcode -ne 0 ]
                    do
                        cd bin/${binname}_debug.app/Contents/MacOS/
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
