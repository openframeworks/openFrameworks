#!/bin/bash
#set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}

# fail the job when a build fails, with an error annotation naming what failed
build_or_fail(){
	if ! make "$@"; then
		echo "::error title=Build failed::make $* failed in ${PWD#$ROOT/}"
		exit 1
	fi
}

trap 'for f in ~/Library/Logs/DiagnosticReports/*; do cat $f; done' 11

echo "##[group]**** Building emptyExample ****"
cd $ROOT
cp scripts/templates/osx/Makefile examples/templates/emptyExample/
cp scripts/templates/osx/config.make examples/templates/emptyExample/
cd examples/templates/emptyExample/
build_or_fail -j2 Debug
echo "##[endgroup]"

echo "##[group]**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/osx/Makefile examples/templates/allAddonsExample/
cp scripts/templates/osx/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
build_or_fail -j2 Debug
echo "##[endgroup]"

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
                cp ../../../scripts/templates/osx/Makefile .
                cp ../../../scripts/templates/osx/config.make .
                build_or_fail -j2 Debug
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
