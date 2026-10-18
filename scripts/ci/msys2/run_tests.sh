#!/bin/bash
# set -ev
ROOT=$(pwd -P)

# fail the job when a build fails, with an error annotation naming what failed
build_or_fail(){
	if ! make "$@"; then
		echo "::error title=Build failed::make $* failed in ${PWD#$ROOT/}"
		exit 1
	fi
}
# source $ROOT/scripts/ci/ccache.sh

echo "##[group]**** Running unit tests ****"
cd $ROOT/tests
for group in *; do
	case "$group" in
		android|ios|tvOS|emscripten) continue ;;
	esac
	if [ -d $group ]; then
		echo "##[group] $group"
		for test in $group/*; do
			if [ -d $test ]; then
				cd $test
				cp ../../../scripts/templates/msys2/Makefile .
				cp ../../../scripts/templates/msys2/config.make .
				build_or_fail -j2 Debug
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
