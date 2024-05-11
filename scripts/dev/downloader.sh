#!/bin/bash
set -ev

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
	# if [[ "${SILENTARGS}" == "-s" ]]; then
	# 	if command -v wget2 2>/dev/null; then 
	# 		wget2 -q $@;
	# 	elif command -v curl 2>/dev/null; then 
	# 		for PKG in $@; do
	# 			curl -LO --retry 20 -O -s $PKG; 
	# 		done
	# 	else 
	# 		for PKG in $@; do
	# 			wget -q $PKG@ 2> /dev/null; 
	# 		done
	# 	fi;
	# else
		if command -v wget2 2>/dev/null; then 
			wget2 $@;
		elif command -v curl 2>/dev/null; then 
			for PKG in $@; do
				curl -LO --retry 20 -O --progress-bar $PKG;
			done
		else 
			for PKG in $@; do
				wget $PKG 2> /dev/null; 
			done
		fi;
	# fi

 
}

