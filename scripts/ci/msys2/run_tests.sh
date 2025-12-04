#!/bin/bash
# set -ev
ROOT=$(pwd -P)
# source $ROOT/scripts/ci/ccache.sh

echo "##[group]**** Running unit tests ****"
cd $ROOT/tests
for group in *; do
	if [ -d $group ]; then
		echo "##[group] $group"
		for test in $group/*; do
			if [ -d $test ]; then
				cd $test
				cp ../../../scripts/templates/msys2/Makefile .
				cp ../../../scripts/templates/msys2/config.make .
				make -j2 Debug
				cd bin
				binname=$(basename ${test})
                #gdb -batch -ex "run" -ex "bt" -ex "q \$_exitcode" ./${binname}_debug
				./${binname}_debug.exe
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
