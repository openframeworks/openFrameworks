#!/bin/bash

export LC_ALL=C

ARCH=$(uname -m)
if [ "$ARCH" = "x86_64" ]; then
        LIBSPATH=linux64
else
        LIBSPATH=linux
fi

WHO=`who am i`;ID=`echo ${WHO%% *}`
GROUP_ID=`id --group -n ${ID}`

cd ../../libs/openFrameworksCompiled/project
make Debug
exit_code=$?
if [ $exit_code != 0 ]; then
  echo "there has been a problem compiling Debug OF library"
  echo "please report this problem in the forums"
  chown -R $ID:$GROUP_ID ../lib/*
  exit $exit_code
fi

make Release
exit_code=$?
if [ $exit_code != 0 ]; then
  echo "there has been a problem compiling Release OF library"
  echo "please report this problem in the forums"
  chown -R $ID:$GROUP_ID ../lib/*
  exit $exit_code
fi

chown -R $ID:$GROUP_ID ../lib/*


