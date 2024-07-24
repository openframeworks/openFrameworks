#!/bin/bash
VERSION=3.3.0
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

convert_bytes_to_mb() {
  bytes=$1
  mb=$(echo "scale=2; $bytes / (1024 * 1024)" | bc)
  echo "${mb}MB"
}


CHECK_RESULT=0
check_remote_vs_local() {
  LOCAL_FILE="$1"
  REMOTE_URL="$2"
  echo "  [downloader] cURL: [$REMOTE_URL]"
  if [ ! -f "$LOCAL_FILE" ]; then
    echo "  [downloader] No download cache"
    echo "  [downloader] Proceeding with download. "
    CHECK_RESULT=0
    return
  else
    echo "  [downloader] Found download cache."
    echo "  [cache] [$LOCAL_FILE]"
  fi

  LocalSize=$(wc -c < "$LOCAL_FILE" | tr -d '[:space:]')
  RemoteSize=$(curl -sI -L --retry 20 "$REMOTE_URL" | awk '/content-length/ {print $2}' | tr -d '\r' | tail -n 1)
  if [ -z "$RemoteSize" ]; then
    RemoteSize=$(curl -sI -L --retry 20 "$REMOTE_URL" | awk '/Content-Length/ {print $2}' | tr -d '\r' | tail -n 1)
  fi

  LocalSizeMB=$(convert_bytes_to_mb $LocalSize)
  RemoteSizeMB=$(convert_bytes_to_mb $RemoteSize)

  echo "  [downloader] Remote size:[${RemoteSizeMB}] | Local size:[${LocalSizeMB}]"
	modified=$(curl -L --retry 20 --silent --head "$REMOTE_URL" | awk '/^last-modified/{print $0}' | sed 's/^last-modified: //')
	if [ -z "$modified" ]; then
		modified=$(curl -L --retry 20 --silent --head "$REMOTE_URL" | awk '/^Last-Modified/{print $0}' | sed 's/^Last-Modified: //')
		if [ -z "$modified" ]; then
			echo "  [cURL] failed to retrieve last-modified header from remote ["$REMOTE_URL"] ... Proceeding with download"
			CHECK_RESULT=0
			return
		fi
	fi

  if [[ "$OSTYPE" == "darwin"* ]]; then
    remote_ctime=$(date -j -f "%a, %d %b %Y %H:%M:%S %Z" "$modified" "+%s" 2>/dev/null)
  else
    remote_ctime=$(date --date="$modified" +%s 2>/dev/null)
  fi
  if [ $? -ne 0 ]; then
    echo "  [downloader] Error in converting Remote modification time [report this openFrameworks devs]."
    echo "  [downloader] ... Proceeding with download"
    CHECK_RESULT=0
    return
  fi
  # Get local file modification time
	if [[ "$OSTYPE" == "darwin"* ]]; then
		local_ctime=$(stat -f "%Sm" -t "%a, %d %b %Y %H:%M:%S %Z" "$LOCAL_FILE" 2>/dev/null)
		local_ctime=$(date -j -f "%a, %d %b %Y %H:%M:%S %Z" "$local_ctime" "+%s" 2>/dev/null)
	else
		local_ctime=$(stat -c %y "$LOCAL_FILE" 2>/dev/null)
		local_ctime=$(date --date="$local_ctime" +%s 2>/dev/null)
	fi
  if [ $? -ne 0 ]; then
    echo "  [downloader] Error in converting Local modification time [report this openFrameworks devs]."
    echo "  [downloader] ... Proceeding with download"
    CHECK_RESULT=0
    return
  fi
  if [ "$LocalSize" != "$RemoteSize" ]; then 
  	echo "  [downloader] Remote size bits:[${RemoteSize}] | Local size bits:[${LocalSize}]"
    echo "  [downloader] File sizes differ between remote and local file."
    echo "  [downloader] ... Proceeding with download"
    CHECK_RESULT=0
    return
  fi
  # Check modification time
  if [ "$local_ctime" -lt "$remote_ctime" ]; then
  	echo "  [downloader] Remote modification Time:[${remote_ctime}] | Local modification Time:[${local_ctime}]"
    echo "  [downloader] Remote file is newer."
    echo "  [downloader] ... Proceeding with download"
    CHECK_RESULT=0
    return
  fi
  echo "  [downloader] No need to download again. Every bit matters."
  CHECK_RESULT=1
  return
}

downloader() { 
    echo " [openFrameworks downloader v${VERSION}] ... "
    if [ -z "$1" ]; then 
        printDownloaderHelp
        exit 1
    fi
    ERROR_MSG="  [downloader] Failed: No Installed: [wget2], [curl] or [wget]. Please install (via homebrew, winget or apt-get) and try again."
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
                    echo "  [downloader] Invalid URL: [$1]"
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
        echo "  [downloader] [WARNING SSL Validation is Disabled with -k or --no-ssl]"
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
            fi
        else
           URLS_TO_DOWNLOAD+="${URL} "
        fi
    done
    echo
    if [ -z "$URLS_TO_DOWNLOAD" ]; then
        echo "  [downloader] No URLS to download, continue..."
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
                echo "  [downloader] [wget2] urls:[$URLS_TO_DOWNLOAD]"
                echo
                wget2 -N -nv --progress=bar -t20 $SSL_ARGS ${URLS_TO_DOWNLOAD}
            elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 1 ]]; then
                echo "  [downloader] [cURL] urls:[$URLS_TO_DOWNLOAD]"
                echo
                curl -L --retry 20 --progress-bar $SSL_ARGS ${URLS_TO_DOWNLOAD}
            elif [[ $WGET == 1 ]] && [[ $WGET_INSTALLED == 1 ]]; then
                echo "  [downloader] [wget] [$FILENAME] urls:[$URLS_TO_DOWNLOAD]"
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
