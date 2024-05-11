#!/bin/bash
# set -ev

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
		if command -v wget2 2>/dev/null; then 
			wget2 -q $@;
		elif command -v curl 2>/dev/null; then 
			for PKG in $@; do
				if [[ -z "$PKG" ]]; then
					curl -LO --retry 20 -s $PKG
				fi
			done
		else 
			for PKG in $@; do
				if [[ -z "$PKG" ]]; then
					wget -q $PKG@ 2> /dev/null
				fi
			done
		fi;
	else
		if command -v wget2 2>/dev/null; then 
			wget2 $@;
		elif 
		 command -v curl 2>/dev/null; then 
			for PKG in $@; do
				if [[ -z "$PKG" ]]; then
					curl -L --retry 20 --progress-bar -O $PKG
				fi
			done
		else 
			for PKG in $@; do
				if [[ -z "$PKG" ]]; then
					wget $PKG 2> /dev/null
				fi
			done
		fi;
	fi
}
