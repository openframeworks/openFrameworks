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
for GROUP in *; do
	case "$GROUP" in
		android|emscripten|ios|tvOS|tvos)
			echo "Skipping $GROUP (not a native Linux unit-test group)"
			continue
			;;
	esac
	if [ -d $GROUP ]; then
		echo "##[group] $GROUP"
		for TEST in $GROUP/*; do
			if [ -d $TEST ]; then
				cd $TEST
				cp "$TEMPLATE/Makefile" .
				cp "$TEMPLATE/config.make" .
				sleep 0.3
				make -j2 Debug
				sleep 0.3
				cd bin
				BINNAME=$(basename ${TEST})

				if [[ -f ./${BINNAME}_debug ]]; then
					gdb -batch -ex "run" -ex "bt" -ex "q \$_exitcode" ./${BINNAME}_debug
					#./${BINNAME}_debug
				else
					echo "Binary not found: ${BINNAME}_debug"
					exit 1
				fi
				ERRORCODE=$?
				if [[ $ERRORCODE -ne 0 ]]; then
					echo "Test failed: ${BINNAME}_debug with error code: $ERRORCODE"
					exit $ERRORCODE
				fi
				cd $ROOT/tests
			fi
		done
		echo "##[endgroup]"
	fi
done
# fi
echo "##[endgroup]"
