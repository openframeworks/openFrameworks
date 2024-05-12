#!/bin/bash

printDownloaderHelp(){
cat << EOF
    Usage: download.sh [URL] [OPTIONS]

    Example: download.sh http://ci.openframeworks.cc/libs/file.zip -s

    Options:
    -s, --silent                Silent download progress
    -h, --help                  Shows this message
EOF
}

downloader() { 

	if [ -z "$1" ]
	then printDownloaderHelp; fi

    	SILENTARGS="";
        if [ $# -ge 2  ]; then
            SILENTARGS=$2
        fi
        if [[ "${SILENTARGS}" == "-s" ]]; then
		if command -v curl 2>/dev/null; then 
    			curl -LO --retry 20 -O -s $@; 
    		else 
        		wget -q $@ 2> /dev/null; fi;
	else
		if command -v curl 2>/dev/null; then 
    			curl -LO --retry 20 -O --progress-bar $@; 
    		else 
        		wget $@ 2> /dev/null; fi;
	fi

 
}

