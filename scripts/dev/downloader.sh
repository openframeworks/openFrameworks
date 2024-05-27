#!/bin/bash

printDownloaderHelp(){
cat << EOF
    
    Usage: 
    ."$SCRIPT_DIR/downloader.sh"
    downloader [URL] [OPTIONS]

    Example: downloader http://ci.openframeworks.cc/libs/file.zip -s

    This script fixes downloads to use wget or curl depending if have package

    Options:
    -s, --silent                Silent download progress
    -h, --help                  Shows this message
EOF
}

downloader() { 
    echo "Downloading... $1"
	if [ -z "$1" ]; then printDownloaderHelp; fi
	SILENTARGS="";
    if [ $# -ge 2  ]; then SILENTARGS=$2; fi
    if [[ "${SILENTARGS}" == "-s" ]]; then
        if command -v wget2 2>/dev/null; then
            wget2 -q $@ 2> /dev/null;
		elif command -v curl 2>/dev/null; then
            curl -OL --parallel --retry 20 -s -N -L $@;
        else
            wget -q $@ 2> /dev/null;
        fi;
	else
        if command -v wget2 2>/dev/null; then
            wget2 --progress=bar $@ 2> /dev/null;
		elif command -v curl 2>/dev/null; then
            curl -OL --parallel --retry 20 --progress-bar -N -L $@ || echo $?;
        else
            wget --progress=bar $@ 2> /dev/null; fi;
	fi

}

