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
    -k, --no-ssl                allow no SSL
EOF
}

validate_url(){
    if [[ $1 =~ ^https?://.+ ]]; then
        return 0
    else
        return 1
    fi
}

downloader() { 
    echo " [downloader] ... "
    if [ -z "$1" ]; then 
        printDownloaderHelp
        exit 1
    fi
    SILENT=0
    NO_SSL=0
    URLS=()

    while [[ $# -gt 0 ]]; do
        key="$1"
        case $key in
            -s|--silent)
            SILENT=1
            shift # past argument
            ;;
            -k|--no-ssl)
            NO_SSL=1
            shift # past argument
            ;;
            -h|--help)
            printDownloaderHelp
            exit 0
            ;;
            *)
            if validate_url "$1"; then
                URLS+=("$1")
            else
                if [ $1 != "0" ]; then 
                    echo "Invalid URL: [$1]"
                fi
            fi
            shift # past argument
            ;;
        esac
    done

    SSL_ARGS=""
    if [[ "$NO_SSL" == "1" ]]; then 
        echo "no SSL == 1"
        if command -v wget2 > /dev/null 2>&1; then
            SSL_ARGS="--no-check-certificate"
        elif command -v curl > /dev/null 2>&1; then
            SSL_ARGS="--insecure"
        else
            SSL_ARGS="--no-check-certificate"
        fi
        echo " [Warning SSL Validation is Disabled with -k or --no-ssl]"
    fi

    for URL in "${URLS[@]}"; do
        FILENAME=$(basename "$URL")
        echo "Downloading [$FILENAME] @ [$URL]"
        if [[ "${SILENT}" == "1" ]]; then
            if command -v wget2 2>/dev/null; then
                wget2 -q $URL 2> /dev/null;
            elif command -v curl 2>/dev/null; then
                curl -L --parallel --retry 20 -s $SSL_ARGS -N -O $URL;
            else
                wget -q $URL 2> /dev/null;
            fi;
        else
             if command -v wget2 2>/dev/null; then
                wget2 --progress=bar $URL 2> /dev/null;
            elif command -v curl 2>/dev/null; then
                curl -L --parallel --retry 20 --progress-bar -N $SSL_ARGS -O $URL || echo $?;
            else
                wget --progress=bar $URL 2> /dev/null;
            fi
        fi
    done
}
