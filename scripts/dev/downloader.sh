#!/bin/bash

downloader() { 
    if command -v curl 2>/dev/null; 
    then 
    	curl -LO --retry 20 -O --progress $1 $2 $3; 
    else 
        wget $1 $2 $3 2> /dev/null; fi; 
}

