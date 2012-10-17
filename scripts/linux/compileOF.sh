#!/bin/bash

# This script compiles the release and debug libraries for the current linux architecture

# force all commands to use a traditional C locale
export LC_ALL=C

# determine the current architecture
ARCH=$(uname -m)

# set the LIBSPATH variable based on the ARCH
if [ "$ARCH" = "x86_64" ]; then
    LIBSPATH=linux64
elif [ "$ARCH" = "armv6l" ]; then
    LIBSPATH=linuxarmv6l
else
    LIBSPATH=linux
fi

# record our id to set up ownership later
WHO=`who am i`;ID=`echo ${WHO%% *}`
GROUP_ID=`id --group -n ${ID}`

# change into our $LIBSPATH directory
cd ../../libs/openFrameworksCompiled/project/$LIBSPATH

# make the DEBUG target based on the makefile in the $LIBSPATH directory
make Debug
exit_code=$?
if [ $exit_code != 0 ]; then
    echo "there has been a problem compiling Debug OF library"
    echo "please report this problem in the forums"
    chown -R $ID:$GROUP_ID obj ../../lib/${LIBSPATH}/*
    exit $exit_code
fi

# make the Release target based on the makefile in the $LIBSPATH directory
make Release
exit_code=$?
if [ $exit_code != 0 ]; then
    echo "there has been a problem compiling Release OF library"
    echo "please report this problem in the forums"
    chown -R $ID:$GROUP_ID obj ../../lib/${LIBSPATH}/*
    exit $exit_code
fi

# set up ownership of the libs that were just compiled based on the current user/groupid
chown -R $ID:$GROUP_ID obj ../../lib/${LIBSPATH}/*

