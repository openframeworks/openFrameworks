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
    echo " [openFrameworks downloader] ... "
    if [ -z "$1" ]; then 
        printDownloaderHelp
        exit 1
    fi
    ERROR_MSG=" Downloading failed: No Installed: wget2, curl or wget. Please install (via homebrew, winget, apt-get) and try again. "
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
                    echo " Invalid URL: [$1]"
                fi
            fi
            shift # past argument
            ;;
        esac
    done
    WGET2_INSTALLED=$(command -v wget2 > /dev/null 2>&1; echo $?)
    CURL_INSTALLED=$(command -v curl > /dev/null 2>&1; echo $?)
    WGET_INSTALLED=$(command -v wget > /dev/null 2>&1; echo $?)
    WGET2=0
    CURL=1
    WGET=1
    SSL_ARGS=""
    if [[ "$NO_SSL" == "1" ]]; then 
        if  [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 0 ]]; then
            SSL_ARGS="--no-check-certificate"
        elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 0 ]]; then
            SSL_ARGS="--insecure"
        else
            SSL_ARGS="--no-check-certificate"
        fi
        echo " [WARNING SSL Validation is Disabled with -k or --no-ssl]"
    fi
    URLS_TO_DOWNLOAD=""
    for ((i = 0; i < ${#URLS[@]}; i++)); do
        URL="${URLS[$i]}"
        FILENAME=$(basename "$URL")
        if  [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 0 ]]; then
            URLS_TO_DOWNLOAD+="${URL} "
        elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 0 ]]; then
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
        if  [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 0 ]]; then
            wget2 --progress=none -nc -t20 $URLS_TO_DOWNLOAD
        elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 0 ]]; then
            curl -L --parallel --retry 20 -s ${SSL_ARGS} -N ${URLS_TO_DOWNLOAD}
        elif [[ $WGET == 1 ]] && [[ $WGET_INSTALLED == 0 ]]; then
            wget -qnc -t20 ${URLS_TO_DOWNLOAD} 
        else 
            echo $ERROR_MSG;
        fi;
    else
        if [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 0 ]]; then # 0 means true in this context
            echo " Downloading [wget2] urls:[$URLS_TO_DOWNLOAD]"
            echo
            wget2 --progress=bar -vnc -t20 ${URLS_TO_DOWNLOAD}
        elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 0 ]]; then
            echo " Downloading [cURL] urls:[$URLS_TO_DOWNLOAD]"
            echo
            curl -L --parallel --retry 20 ${SSL_ARGS} -N ${URLS_TO_DOWNLOAD}
        elif [[ $WGET == 1 ]] && [[ $WGET_INSTALLED == 0 ]]; then
            echo " Downloading [wget] [$FILENAME] urls:[$URLS_TO_DOWNLOAD]"
            echo
            wget --progress=bar -nc -t20 $URLS_TO_DOWNLOAD
        else 
            echo $ERROR_MSG;
            exit 1;
        fi
    fi
    echo
}
