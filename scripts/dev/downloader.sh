#!/bin/bash
VERSION=3.2.3
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

#!/bin/bash

CHECK_RESULT=0
check_remote_vs_local() {
  LOCAL_FILE="$1"
  REMOTE_URL="$2"

  if [ ! -f "$LOCAL_FILE" ]; then
    echo "  cURL [check_remote_vs_local] - LOCAL_FILE:[$LOCAL_FILE] not found. Download"
    CHECK_RESULT=0
    return
  else
    echo "  cURL [check_remote_vs_local] - LOCAL_FILE:[$LOCAL_FILE] exists"
  fi

  # Get local file size
  LocalSize=$(wc -c < "$LOCAL_FILE")

  # Get remote file size
  RemoteSize=$(curl -sI -L --retry 20 "$REMOTE_URL" | awk '/Content-Length/ {print $2}' | tr -d '\r' | tail -n 1)

  # Get remote file modification time
  modified=$(curl -L --retry 20 --silent --head "$REMOTE_URL" | awk '/^Last-Modified/{print $0}' | sed 's/^Last-Modified: //')
  remote_ctime=$(date --date="$modified" +%s)

  # Get local file modification time
  local_ctime=$(stat -c %z "$LOCAL_FILE")
  local_ctime=$(date --date="$local_ctime" +%s)

  # Check size
  if [ "$LocalSize" != "$RemoteSize" ]; then
    echo "  File sizes differ. Run the cURL. LocalSize:[$LocalSize] RemoteSize:[$RemoteSize]"
    CHECK_RESULT=0
    return
  fi

  # Check modification time
  if [ "$local_ctime" -lt "$remote_ctime" ]; then
    echo "  Remote file is newer. Run the cURL. local_ctime[$local_ctime] remote_ctime:[$remote_ctime]"
    CHECK_RESULT=0
    return
  fi

  echo "  Files are the same. Do not run the cURL."
  CHECK_RESULT=1
  return
}

downloader() { 
    echo " [openFrameworks downloader v${VERSION}] ... "
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
            shift
            ;;
            -k|--no-ssl)
            NO_SSL=1
            shift
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
            shift
            ;;
        esac
    done
    if command -v wget2 > /dev/null 2>&1; then
        WGET2_INSTALLED=1
    else
        WGET2_INSTALLED=0
    fi
    if command -v curl > /dev/null 2>&1; then
        CURL_INSTALLED=1
    else
        CURL_INSTALLED=0
    fi
    if command -v wget > /dev/null 2>&1; then
        WGET_INSTALLED=1
    else
        WGET_INSTALLED=0
    fi
    WGET2=1
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
        if  [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 1 ]]; then
            URLS_TO_DOWNLOAD+="${URL} "
        elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 1 ]]; then
            LOCAL_FILE=$FILENAME
            REMOTE_URL=$URL
            check_remote_vs_local "$LOCAL_FILE" "$REMOTE_URL"
            if [ $CHECK_RESULT -eq 0 ]; then
                URLS_TO_DOWNLOAD+="${URL} -o ${FILENAME} "
                if [ $((i + 1)) -lt ${#URLS[@]} ]; then
                    URLS_TO_DOWNLOAD+="-k ";
                fi
            else
                echo "  LOCAL_FILE:[${LOCAL_FILE}] is same as remote - skipping download"
            fi
        else
           URLS_TO_DOWNLOAD+="${URL} "
        fi
    done
    echo
    if [ -z "$URLS_TO_DOWNLOAD" ]; then
        echo "  No URLS to download, continue"
    else
        if [[ "${SILENT}" == 1 ]]; then
            if  [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 1 ]]; then
                echo
                wget2 -nv --progress=bar -N -t20 $SSL_ARGS $URLS_TO_DOWNLOAD
            elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 1 ]]; then
                echo
                curl -L --retry 20 --progress-bar $SSL_ARGS ${URLS_TO_DOWNLOAD}
            elif [[ $WGET == 1 ]] && [[ $WGET_INSTALLED == 1 ]]; then
                echo
                wget -nv -N -t20 ${URLS_TO_DOWNLOAD} 
            else 
                echo $ERROR_MSG;
                exit 1;
            fi;
        else
            if [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 1 ]]; then 
                echo " Downloading [wget2] urls:[$URLS_TO_DOWNLOAD]"
                echo
                wget2 -N -nv --progress=bar -t20 $SSL_ARGS ${URLS_TO_DOWNLOAD}
            elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 1 ]]; then
                echo " Downloading [cURL] urls:[$URLS_TO_DOWNLOAD]"
                echo
                curl -L --retry 20 --progress-bar $SSL_ARGS ${URLS_TO_DOWNLOAD}
            elif [[ $WGET == 1 ]] && [[ $WGET_INSTALLED == 1 ]]; then
                echo " Downloading [wget] [$FILENAME] urls:[$URLS_TO_DOWNLOAD]"
                echo
                wget -nv --progress=bar -N -t20 $SSL_ARGS $URLS_TO_DOWNLOAD
            else 
                echo $ERROR_MSG;
                exit 1;
            fi
        fi
    fi
    echo
}
