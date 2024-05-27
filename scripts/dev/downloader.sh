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
    WGET2=1
    CURL=1

    SSL_ARGS=""
    if [[ "$NO_SSL" == "1" ]]; then 
        if  [[ $WGET2 == 1 ]] && command -v wget2 2>/dev/null 2>&1; then
            SSL_ARGS="--no-check-certificate"
        elif [[ $CURL == 1 ]] && command -v curl > /dev/null 2>&1; then
            SSL_ARGS="--insecure"
        else
            SSL_ARGS="--no-check-certificate"
        fi
        echo " [Warning SSL Validation is Disabled with -k or --no-ssl]"
    fi
    URLS_TO_DOWNLOAD=""
    for ((i = 0; i < ${#URLS[@]}; i++)); do
        URL="${URLS[$i]}"
        FILENAME=$(basename "$URL")
        #echo "Downloading [$FILENAME] @ [$URL]"
        if  [[ $WGET2 == 1 ]] && command -v wget2 /dev/null 2>&1; then
            URLS_TO_DOWNLOAD+="${URL} "
        elif [[ $CURL == 1 ]] && command -v curl /dev/null 2>&1; then
           URLS_TO_DOWNLOAD+="${URL} -o ${FILENAME} "
            if [ -n "${URLS[$i+1]}" ]; then
                URLS_TO_DOWNLOAD+="-k ";
            fi
        else
           URLS_TO_DOWNLOAD+="${URL} "
        fi
        
    done
    echo
    if [[ "${SILENT}" == "1" ]]; then
        if  [[ $WGET2 == 1 ]] && command -v wget2 /dev/null 2>&1; then
            #echo " Downloading [silent] using wget urls [$URLS_TO_DOWNLOAD]"
            wget2 -q $URLS_TO_DOWNLOAD
        elif [[ $CURL == 1 ]] && command -v curl /dev/null 2>&1; then
            #echo " Downloading [silent] using cURL urls [$URLS_TO_DOWNLOAD]"
            curl -L --parallel --retry 20 -s ${SSL_ARGS} -N ${URLS_TO_DOWNLOAD}
        else
            #echo " Downloading [silent] using wget urls [$URLS_TO_DOWNLOAD]"
            wget -q ${URLS_TO_DOWNLOAD} 
        fi;
    else
        if  [[ $WGET2 == 1 ]] && command -v wget2 /dev/null 2>&1; then
            wget2 --progress=bar ${URLS_TO_DOWNLOAD}
        elif [[ $CURL == 1 ]] && command -v curl /dev/null 2>&1; then
            #echo " Downloading using cURL urls [$URLS_TO_DOWNLOAD]"
            curl -L --parallel --retry 20 ${SSL_ARGS} -N ${URLS_TO_DOWNLOAD}
        else
            #echo " Downloading using wget [$FILENAME] urls [$URLS_TO_DOWNLOAD]"
            wget --progress=bar $URLS_TO_DOWNLOAD
        fi
    fi
    echo
    
}
