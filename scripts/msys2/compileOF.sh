#!/usr/bin/env bash

export LC_ALL=C

pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd`
popd > /dev/null

BUILD="install"
PARALLEL_MAKE=${NUMBER_OF_PROCESSORS}
while getopts tj: opt ; do
	case "$opt" in
		t)  # testing, only build Debug
		    BUILD="test" ;;
		j)  # make job count for parallel build
		    JOBS="$OPTARG"
	esac
done

if [ "$MSYSTEM" != "MINGW32" ]
then
	echo "This is not a MINGW32 shell!"
	echo "Please launch compileOF.sh from a MINGW32 shell."
	exit 1
fi

cd ${SCRIPTPATH}/../../libs/openFrameworksCompiled/project
make -j${PARALLEL_MAKE} Debug
exit_code=$?
if [ $exit_code != 0 ]; then
  echo "there has been a problem compiling Debug OF library"
  echo "please report this problem in the forums"
  exit $exit_code
fi

if [ "$BUILD" == "install" ]; then
    make -j${PARALLEL_MAKE} Release
    exit_code=$?
    if [ $exit_code != 0 ]; then
      echo "there has been a problem compiling Release OF library"
      echo "please report this problem in the forums"
      exit $exit_code
    fi
fi
