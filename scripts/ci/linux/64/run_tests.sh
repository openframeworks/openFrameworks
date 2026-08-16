#!/bin/bash
# Native Linux unit tests only. Other groups have their own CI jobs and
# often need a display, NDK, or emscripten toolchain.
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../../.." ; pwd -P )"}
TEMPLATE="$ROOT/scripts/templates/linux64"

# if [ "$OPT" == "qbs" ]; then
	# exit 0
# else

echo "##[group]**** Running unit tests ****"
cd $ROOT/tests
for group in *; do
	case "$group" in
		android|emscripten|ios|tvOS|tvos)
			echo "Skipping $group (not a native Linux unit-test group)"
			continue
			;;
	esac
	if [ -d $group ]; then
		echo "##[group] $group"
		for test in $group/*; do
			if [ -d $test ]; then
				cd $test
				cp "$TEMPLATE/Makefile" .
				cp "$TEMPLATE/config.make" .
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
