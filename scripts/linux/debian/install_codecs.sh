#!/bin/bash

if [ $EUID != 0 ]; then
	echo "this script must be run using sudo"
	echo ""
	echo "usage:"
	echo "sudo ./install_dependencies.sh"
	exit $exit_code
   exit 1
fi

GSTREAMER_VERSION=0.10

RET=$(apt-cache show -n libgstreamer1.0-dev &> /dev/null; echo $?)

if [ "$RET" -eq "0" ]; then
    GSTREAMER_VERSION=1.0
fi

apt-get install libmpg123-dev gstreamer${GSTREAMER_VERSION}-plugins-ugly
