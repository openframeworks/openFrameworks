#!/bin/bash
# set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}

# if [ "$OPT" == "qbs" ]; then
	# exit 0
# else

echo "##[group]**** Running unit tests ****"
cd $ROOT/tests
for group in *; do
	if [ -d $group ]; then
		echo "##[group] $group"
		for test in $group/*; do
			if [ -d $test ]; then
				cd $test
				cp ../../../scripts/templates/linux/Makefile .
				cp ../../../scripts/templates/linux/config.make .
				sleep 0.3 
				make -j2 Debug
				sleep 0.3 
				cd bin
				binname=$(basename ${test})
				
				if [[ -f ./${binname}_debug ]]; then
					gdb -batch -ex "run" -ex "bt" -ex "q \$_exitcode" ./${binname}_debug
					#./${binname}_debug
				else
					echo "Binary not found: ${binname}_debug"
					exit 1
				fi
				errorcode=$?
				if [[ $errorcode -ne 0 ]]; then
					echo "Test failed: ${binname}_debug with error code: $errorcode"
					exit $errorcode
				fi
				cd $ROOT/tests
			fi
		done
		echo "##[endgroup]"
	fi
done
# fi
echo "##[endgroup]"
