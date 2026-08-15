#!/usr/bin/env bash

export LC_ALL=C

SCRIPTPATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OF_ROOT="$(cd "$SCRIPTPATH/../.." && pwd)"

ARCH="${ARCH:-$(uname -m)}"
case "$ARCH" in
	x86_64|64) ARCH=64; LIBSPATH=linux/64 ;;
	arm64) ARCH=arm64; LIBSPATH=linux/arm64 ;;
	aarch64) ARCH=aarch64; LIBSPATH=linux/aarch64 ;;
	armv7l) ARCH=armv7l; LIBSPATH=linux/armv7l ;;
	armv6l) ARCH=armv6l; LIBSPATH=linux/armv6l ;;
	jetson) ARCH=jetson; LIBSPATH=linux/jetson ;;
	*) LIBSPATH=linux ;;
esac
export ARCH

BUILD="install"
JOBS=1
while getopts tj: opt ; do
	case "$opt" in
		t) BUILD="test" ;;
		j) JOBS="$OPTARG" ;;
	esac
done

cd "${OF_ROOT}/libs/openFrameworksCompiled/project"
make -j$JOBS Debug
exit_code=$?
if [ $exit_code != 0 ]; then
	echo "there has been a problem compiling Debug OF library"
	echo "please report this problem in the forums"
	exit $exit_code
fi

if [ "$BUILD" == "install" ]; then
	make -j$JOBS Release
	exit_code=$?
	if [ $exit_code != 0 ]; then
		echo "there has been a problem compiling Release OF library"
		echo "please report this problem in the forums"
		exit $exit_code
	fi
fi
