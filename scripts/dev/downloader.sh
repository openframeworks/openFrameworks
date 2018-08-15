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
    if [ "$2" == "-s" ]; then
    	`sh $CATALINA_HOME/bin/startup.sh`
	elif [ "$2" == "-x" ]; then
		`sh $CATALINA_HOME/bin/shutdown.sh`
	else
		echo "Enter '-s' to start Tomcat, '-x' to shutdown."
	fi

    if command -v curl 2>/dev/null; then 
    	curl -LO --retry 20 -O --progress $@; 
    else 
        wget $@ 2> /dev/null; fi; 
}

