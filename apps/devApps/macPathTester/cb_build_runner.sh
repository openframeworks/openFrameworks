#!/bin/sh

#should we run the app based on build
if [ "$2" = "true" ]
then
cd bin
export LD_LIBRARY_PATH=$(pwd)/libs/
./$1
fi

#should we launch nautilus to this directory on build?
if [ "$3" = "true" ]
then
nautilus $(pwd)/bin/
fi

