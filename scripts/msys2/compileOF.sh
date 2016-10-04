#!/usr/bin/env bash

export LC_ALL=C

pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd`
popd > /dev/null

BUILD="install"
JOBS=1
while getopts tj: opt ; do
	case "$opt" in
		t)  # testing, only build Debug
		    BUILD="test" ;;
		j)  # make job count for parallel build
		    JOBS="$OPTARG"
	esac
done

if [ "${MSYSTEM:0:5}" != "MINGW" ]
then
	echo "This is not a MINGW(32|64) shell!"
	echo "Please launch compileOF.sh from a MINGW(32|64) shell."
	exit 1
fi

cd ${SCRIPTPATH}/../../libs/openFrameworksCompiled/project

if [ -v CI ]; then
	echo "Building in CI mode"
	make  Debug
	exit_code=$?
	exit $exit_code
fi
 
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
