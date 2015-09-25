#!/usr/bin/env bash
set -e
export LC_ALL=C

function compilation_cores() {
	# check if cores are set via variable
	if [ -z ${OF_COMPILATION_CORES+0} ]; then
		# (backwards-compatible) default behavior
		echo 1
	else
		if [[ $OF_COMPILATION_CORES == "max" ]]; then
			# leave one core for the host system to
			# continue its work
			local sys_cores=`getconf _NPROCESSORS_ONLN`
			if [ $sys_cores -gt 1 ]; then
				echo $(($sys_cores-1))
			else
				echo 1
			fi
		else
			echo ${OF_COMPILATION_CORES}
		fi
	fi
}

echo "You can speed up the compilation by setting the OF_COMPILATION_CORES variable."
echo ""
echo "You can either set a specific number of cores by executing"
echo "        OF_COMPILATION_CORES=23 ./compileOF.sh"
echo "or use the maximum number of cores available (= total number of cores - 1)"
echo "        OF_COMPILATION_CORES=max ./compileOF.sh"
echo ""
echo "Hit CTR+C to abort the script and restart it with the variable"
echo "set if you want to."
sleep 10

ARCH=$(uname -m)
if [ "$ARCH" = "x86_64" ]; then
        LIBSPATH=linux64
else
        LIBSPATH=linux
fi

WHO=`who am i`;ID=`echo ${WHO%% *}`
GROUP_ID=`id --group -n ${ID}`

pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd`
popd > /dev/null

BUILD="install"
while getopts t opt ; do
	case "$opt" in
		t) # testing, only build Debug
		   BUILD="test" ;;
	esac
done

cd ${SCRIPTPATH}/../../libs/openFrameworksCompiled/project
make -j`compilation_cores` Debug
exit_code=$?
if [ $exit_code != 0 ]; then
  echo "there has been a problem compiling Debug OF library"
  echo "please report this problem in the forums"
  chown -R $ID:$GROUP_ID ../lib/*
  exit $exit_code
fi

if [ "$BUILD" == "install" ]; then
    make -j`compilation_cores` Release
    exit_code=$?
    if [ $exit_code != 0 ]; then
      echo "there has been a problem compiling Release OF library"
      echo "please report this problem in the forums"
      chown -R $ID:$GROUP_ID ../lib/*
      exit $exit_code
    fi
fi

chown -R $ID:$GROUP_ID ../lib/*
