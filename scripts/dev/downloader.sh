#!/bin/bash
VERSION=4.5.0
printDownloaderHelp() {
    cat <<EOF
    
    Usage: 
    ."$SCRIPT_DIR/downloader.sh"
    downloader [URL] [OPTIONS]

    Example: downloader http://ci.openframeworks.cc/libs/file.zip -s

    This script fixes downloads to use wget or curl depending if have package

    Options:
    -s, --silent                Silent download progress
    -h, --help                  Shows this message
    -k, --no-ssl                Disable Forced SSL/HTPS - Allow HTTP
    -z, --no-compression        disallow the server response to be compressed brotli/gzip
    -v, --verbose               verbose logs
    -2, --wget2                 using wget2 (default if system installed)
    -c, --curl                  using cURL (if system installed), disables wget2
    -w, --wget                  using wget (fallback - if system installed), disables wget2/cURL
    -x, --close-connection      close connections ports TCP/HTTP 1.0/1.1/2.0 (default)
    -p, --keep-port-alive       (disabled)

EOF
}

validate_url() {
    if [[ $1 =~ ^https?://.+ ]]; then
        return 0
    else
        return 1
    fi
}

convert_bytes_to_mb() {
    bytes=$1
    mb=$((bytes / 1024 / 1024))
    echo "${mb}MB"
}

CHECK_RESULT=0
MAX_REDIRECTS=20
RETRY_MAX=20
RETRY_DELAY_S=1
EXTRA_ARGS=""

# ansi console escape codes
CON_DEFAULT="\033[0m"
CON_WHITE="\033[37m"
CON_LIGHT_GRAY="\033[37;1m"
CON_BOLD="\033[1m"
CON_RED="\033[31m"
CON_YELLOW="\033[33m"
CON_YELLOW_GREEN="\033[33m"
CON_GREEN="\033[32m"

echoError() {
    echo -e "$CON_BOLD$CON_RED$1$CON_DEFAULT"
}
echoWarning() {
    echo -e "$CON_BOLD$CON_YELLOW$1$CON_DEFAULT"
}
echoInfo() {
    echo -e "$CON_BOLD$CON_WHITE$1$CON_DEFAULT"
}

check_remote_vs_local() {
    LOCAL_FILE="$1"
    REMOTE_URL="$2"
    USE_WGET2="$3"
    echo "  [downloader] check if local == remote: [$REMOTE_URL]"
    if [ ! -f "$LOCAL_FILE" ]; then
        echo "  [downloader] No download cache"
        echo "  [downloader] Proceeding with download. "
        CHECK_RESULT=0
        return
    else
        echo "  [downloader] Found download cache."
        echo "  [cache] [$LOCAL_FILE]"
    fi
    REMOTE_URL=$(echo "$REMOTE_URL" | sed 's/[[:space:]]*$//')
    LocalSize=$(wc -c <"$LOCAL_FILE" | tr -d '[:space:]')
    #REMOTE_CALL="wget2 --spider --max-redirect=${MAX_REDIRECTS} ${EXTRA_ARGS}"
    REMOTE_CALL=""
    headers=$(curl -L -I --retry ${RETRY_MAX} --max-redirs ${MAX_REDIRECTS} ${EXTRA_ARGS} --retry-connrefused --silent --head $REMOTE_URL)
    RemoteSize=$(echo "$headers" | awk '/[cC]ontent-[lL]ength/ {print $2}' | tr -d '\r' | tail -n 1)
    modified=$(echo "$headers" | awk '/[lL]ast-[mM]odified/ {print $0}' | sed 's/^[lL]ast-[mM]odified: //')
    LocalSizeMB=$(convert_bytes_to_mb $LocalSize)
    RemoteSizeMB=$(convert_bytes_to_mb $RemoteSize)

    echo "  [downloader] Remote size:[${RemoteSizeMB}] | Local size:[${LocalSizeMB}]"
    if [ -z "$modified" ]; then
        echo "  [downloader] failed to retrieve last-modified header from remote ["$REMOTE_URL"] ... Proceeding with download"
        CHECK_RESULT=0
        rm -f $LOCAL_FILE
        return
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
        rm -f $LOCAL_FILE
        return
    fi

    if [[ "$OSTYPE" == "darwin"* ]]; then # Get local file modification time
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
        rm -f $LOCAL_FILE
        return
    fi
    if [ "$LocalSize" != "$RemoteSize" ]; then
        echo "  [downloader] Remote size bits:[${RemoteSize}] | Local size bits:[${LocalSize}]"
        echo "  [downloader] File sizes differ between remote and local file."
        echo "  [downloader] ... Proceeding with download"
        CHECK_RESULT=0
        rm -f $LOCAL_FILE
        return
    fi
    if [ "$local_ctime" -lt "$remote_ctime" ]; then
        echo "  [downloader] Remote modification Time:[${remote_ctime}] | Local modification Time:[${local_ctime}]"
        echo "  [downloader] Remote file is newer."
        echo "  [downloader] ... Proceeding with download"
        CHECK_RESULT=0
        rm -f $LOCAL_FILE
        return
    fi
    echo "  [downloader] No need to download again. Every bit matters."
    CHECK_RESULT=1
    return
}

finalurl() {
    local input_url="$1"
    local trimmed_url=$(echo "$input_url" | sed 's/[[:space:]]*$//')
    local resolved_url=$(curl -L -I --retry "${RETRY_MAX:-5}" --max-redirs "${MAX_REDIRECTS:-5}" \
        --retry-connrefused --write-out '%{url_effective}' --silent --output /dev/null "$trimmed_url")
    if [[ -z "$resolved_url" ]]; then
        echoError "Failed to resolve URL: $trimmed_url"
        return 1
    fi
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
    COMPRESSION=1
    VERBOSE=0
    WGET2=1
    CURL=1
    WGET=1
    CLOSE_CONNECTION=1
    URLS=()
    FORWARDED_URLS=()
    FINAL_EXTRA_ARGS=""
    CLOSE_EXTRA_ARGS=""
    EXTRA_ARGS=""
    while [[ $# -gt 0 ]]; do
        key="$1"
        case $key in
            -s | --silent)
                SILENT=1
                shift
                ;;
            -k | --no-ssl)
                NO_SSL=1
                shift
                ;;
            -z | --no-compression)
                COMPRESSION=0
                shift
                ;;
            -v | --verbose)
                VERBOSE=1
                shift
                ;;
            -x | --close-connection)
                CLOSE_CONNECTION=1
                shift
                ;;
            -p | --keep-port-alive)
                CLOSE_CONNECTION=0
                shift
                ;;
            -c | --curl)
                WGET2=0
                CURL=1
                shift
                ;;
            -2 | --wget2)
                WGET2=1
                CURL=1
                shift
                ;;
            -w | --wget)
                WGET2=0
                CURL=0
                WGET=1
                shift
                ;;
            -g | --man)
                shift
                ;;
            -h | --help)
                printDownloaderHelp
                exit 0
                ;;
            *)
                if validate_url "$1"; then
                    URLS+=("$1")
                else
                    if [ $1 != "0" ] && [ $1 != "1" ]; then
                        echoError "  [downloader] Invalid URL: [$1]"
                    fi
                fi
                shift
                ;;
        esac
    done
    # if [[ "$OSTYPE" == "msys"* || "$OSTYPE" == "cygwin"* || "$OSTYPE" == "win32"* ]]; then
    #     echo "Detected Windows OS. Skipping wget2..."
    #     WGET2=0
    #     WGET2_INSTALLED=0
    # fi
    # [wget2]
    if command -v wget2 >/dev/null 2>&1; then
        WGET2_INSTALLED=1
        if [[ "$COMPRESSION" == "1" ]] && [[ $WGET2 == 1 ]]; then
            COMPRESS=0
            if wget2 -V | grep -q "+brotlidec"; then
                FINAL_EXTRA_ARGS+="--compression=br "
                COMPRESS=1
            fi
            if wget2 -V | grep -q "+zlib"; then
                FINAL_EXTRA_ARGS+="--compression=gzip "
                COMPRESS=1
            fi
            COMPRESSION=$COMPRESS
        fi
    else
        WGET2_INSTALLED=0
        WGET2=0
    fi

    # [cURL]
    if command -v curl >/dev/null 2>&1; then
        CURL_INSTALLED=1
        CURL_VERSION=$(curl -V | head -n 1 | awk '{print $2}')
        CURL_MIN=7.71.0
        if [ "$(printf '%s\n' "$CURL_MIN" "$CURL_VERSION" | sort -V | head -n1)" = "$CURL_MIN" ] && [ "$CURL_VERSION" != "$CURL_MIN" ]; then
            if [[ $CURL == 1 && $CURL_INSTALLED == 1 ]] && [[ $WGET2 == 0 ]]; then
                EXTRA_ARGS+="--retry-all-errors "
            fi
        fi
        CURL_MIN=7.83.0
        if [ "$(printf '%s\n' "$CURL_MIN" "$CURL_VERSION" | sort -V | head -n1)" = "$CURL_MIN" ] && [ "$CURL_VERSION" != "$CURL_MIN" ]; then
            if [[ $CURL == 1 && $CURL_INSTALLED == 1 ]] && [[ $WGET2 == 0 ]]; then
                EXTRA_ARGS+="--remove-on-error "
            fi
        fi
        if curl -V | grep -q 'HTTP2'; then
            CURL_SUPPORTS_HTTP2=1
            CLOSE_CONNECTION=0
        else
            CURL_SUPPORTS_HTTP2=0
        fi
        if [[ "$COMPRESSION" == "1" ]] && [[ $CURL == 1 ]] && [[ $WGET2 == 0 || $WGET2_INSTALLED == 0 ]]; then
            if curl -V | grep -q "brotli"; then
                FINAL_EXTRA_ARGS+="--compressed "
            else
                COMPRESSION=0
            fi
        fi
    else
        CURL_INSTALLED=0
        CURL=0
    fi
    # [wget]
    if command -v wget >/dev/null 2>&1; then
        WGET_INSTALLED=1
        if [[ "$COMPRESSION" == "1" ]] && [[ $CURL == 0 || $CURL_INSTALLED == 0 ]] && [[ $WGET2 == 0 || $WGET2_INSTALLED == 0 ]]; then
            if wget -V | grep -q "zlib"; then
                EXTRA_ARGS+="--compression "
            else
                COMPRESSION=0
            fi
        fi
    else
        WGET_INSTALLED=0
    fi
    # [options]
    if [[ "$COMPRESSION" == "1" ]]; then
        echo "  [downloader] enabled brotli/zlib losslesss compression response"
    elif [[ "$COMPRESSION" == "0" ]] && [[ $CURL == 1 && $CURL_INSTALLED == 1 ]] && [[ $WGET2 == 0 || $WGET2_INSTALLED == 0 ]]; then
        EXTRA_ARGS+="-Z "
    fi
    if [[ "$NO_SSL" == "1" ]]; then
        if [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 1 ]] || [[ $WGET == 1 && $WGET_INSTALLED == 1 && $CURL == 0 ]]; then
            FINAL_EXTRA_ARGS+="--no-check-certificate"
        elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 1 ]]; then
            FINAL_EXTRA_ARGS+="--insecure "
        else
            FINAL_EXTRA_ARGS+="--no-check-certificate"
        fi
        echoError "  [downloader] [WARNING SSL Validation is Disabled with -k or --no-ssl]"
    fi
    # [download]
    URLS_TO_DOWNLOAD=""
    FINAL_URLS=""
    EXTRA_ARGS=$(echo "$EXTRA_ARGS" | sed 's/[[:space:]]*$//')
    FINAL_EXTRA_ARGS=$(echo "$FINAL_EXTRA_ARGS" | sed 's/[[:space:]]*$//')
    URLS_TO_DOWNLOAD=$(echo "$URLS_TO_DOWNLOAD" | sed 's/[[:space:]]*$//')
    for ((i = 0; i < ${#URLS[@]}; i++)); do
        URL="${URLS[$i]}"
        FILENAME=$(basename "$URL")
        if [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 1 ]]; then
            if [[ "$CURL_INSTALLED" == "1" ]]; then
                LOCAL_FILE=$FILENAME
                REMOTE_URL=$URL
                FORWARDED=$(finalurl "$URL")
                FORWARDED_URLS+=($FORWARDED)
                FINAL_URLS+="$FORWARDED"
                check_remote_vs_local "$LOCAL_FILE" "$REMOTE_URL" 1
                if [ $CHECK_RESULT -eq 0 ]; then
                    URLS_TO_DOWNLOAD+="${URL}"
                    if [ $((i + 1)) -lt ${#URLS[@]} ]; then
                        URLS_TO_DOWNLOAD+=" "
                    fi
                fi
            else
                URLS_TO_DOWNLOAD+="${URL} "
            fi
        elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 1 ]]; then
            LOCAL_FILE=$FILENAME
            REMOTE_URL=$URL
            if validate_url "$URL"; then
                FORWARDED=$(finalurl "$URL")
                FORWARDED_URLS+=($FORWARDED)
                FINAL_URLS+="$FORWARDED"
                check_remote_vs_local "$LOCAL_FILE" "$REMOTE_URL" 0
                if [ $CHECK_RESULT -eq 0 ]; then
                    URLS_TO_DOWNLOAD+="${URL} -o ${FILENAME}"
                    if [ $((i + 1)) -lt ${#URLS[@]} ]; then
                        URLS_TO_DOWNLOAD+=" "
                        FINAL_URLS+=" "
                    fi
                fi
            else
                if [ $1 != "0" ]; then
                    echoError "  [downloader] Invalid URL: [$1]"
                fi
            fi
        else
            URLS_TO_DOWNLOAD+="${URL} "
        fi
    done

    CONNECTION_EXTRA_ARGS=("Connection: close")
    if [[ "$CLOSE_CONNECTION" == "0" ]]; then
        CONNECTION_EXTRA_ARGS=""
    fi

    if [[ $VERBOSE == 1 ]]; then
        EXTRA_ARGS+=" --verbose "
    fi
    EXTRA_ARGS=$(echo "$EXTRA_ARGS" | sed 's/[[:space:]]*$//')
    FINAL_EXTRA_ARGS=$(echo "$FINAL_EXTRA_ARGS" | sed 's/[[:space:]]*$//')
    URLS_TO_DOWNLOAD=$(echo "$URLS_TO_DOWNLOAD" | sed 's/[[:space:]]*$//')
    echo
    if [ -z "$URLS_TO_DOWNLOAD" ]; then
        echo "  [downloader] No URLS to download, continue..."
    else
        if [[ "${SILENT}" == 1 ]]; then
            if [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 1 ]]; then
                echo ""
                wget2 -N -nv --no-tcp-fastopen --progress=bar --tries=${RETRY_MAX} --max-redirect=${MAX_REDIRECTS} --retry-connrefused --timeout=1500 --waitretry=${RETRY_DELAY_S} ${EXTRA_ARGS} ${FINAL_EXTRA_ARGS} ${URLS_TO_DOWNLOAD}
            elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 1 ]]; then
                echo
                curl -Z -L --silent --retry ${RETRY_MAX} --retry-delay ${RETRY_DELAY_S} --max-redirs ${MAX_REDIRECTS} --header "Connection: close" --progress-bar ${EXTRA_ARGS} ${FINAL_EXTRA_ARGS} ${URLS_TO_DOWNLOAD}
            elif [[ $WGET == 1 ]] && [[ $WGET_INSTALLED == 1 ]]; then
                echo
                wget -nv -N --tries=${RETRY_MAX} --retry-connrefused --waitretry=${RETRY_DELAY_S} ${EXTRA_ARGS} ${FINAL_EXTRA_ARGS} ${URLS_TO_DOWNLOAD}
            else
                echo $ERROR_MSG
                exit 1
            fi
        else
            if [[ $WGET2 == 1 ]] && [[ $WGET2_INSTALLED == 1 ]]; then
                echo "  [downloader] [wget2] urls:[$URLS_TO_DOWNLOAD] args:[$EXTRA_ARGS $FINAL_EXTRA_ARGS"
                echo ""
                wget2 -N -c --no-tcp-fastopen --progress=bar --force-progress --tries=${RETRY_MAX} --max-redirect=${MAX_REDIRECTS} --retry-connrefused --waitretry=${RETRY_DELAY_S} --timeout=1500 ${EXTRA_ARGS} ${FINAL_EXTRA_ARGS} ${URLS_TO_DOWNLOAD}
            elif [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 1 ]]; then
                echo "  [downloader] [cURL] urls:[$URLS_TO_DOWNLOAD] args:[$EXTRA_ARGS $FINAL_EXTRA_ARGS ${CONNECTION_EXTRA_ARGS[@]}]"
                echo
                curl -Z -L --retry ${RETRY_MAX} --retry-delay ${RETRY_DELAY_S} --max-redirs ${MAX_REDIRECTS} --progress-bar --header "Connection: close" ${EXTRA_ARGS} ${FINAL_EXTRA_ARGS} ${URLS_TO_DOWNLOAD}

            elif [[ $WGET == 1 ]] && [[ $WGET_INSTALLED == 1 ]]; then
                echo "  [downloader] [wget] [$FILENAME] urls:[$URLS_TO_DOWNLOAD] args:[$EXTRA_ARGS $FINAL_EXTRA_ARGS]"
                echo
                wget -nv --progress=bar -N --tries=${RETRY_MAX} --retry-connrefused --waitretry=${RETRY_DELAY_S} ${EXTRA_ARGS} ${FINAL_EXTRA_ARGS} ${URLS_TO_DOWNLOAD}
            else
                echo $ERROR_MSG
                exit 1
            fi
        fi
        if [[ "$CLOSE_CONNECTION" == "1" ]]; then
            if [[ $CURL == 1 ]] && [[ $CURL_INSTALLED == 1 ]] || [[ $CURL == 1 && $WGET2 == 1 ]]; then
                if [[ ${#FORWARDED_URLS[@]} -eq 0 ]]; then
                    echo "  [downloader] No active connections to close"
                else
                    FIRST_URL="${FORWARDED_URLS[0]}"
                    FIRST_URL=$(echo "$FIRST_URL" | sed 's/[[:space:]]*$//')
                    echo "  [downloader] Closing the ports yarr url:[$FIRST_URL]"
                    curl -I -L --retry-connrefused --insecure --silent --head --max-time 1 --retry ${RETRY_MAX} ${CLOSE_EXTRA_ARGS} --no-keepalive --header "Connection: close" --retry-delay ${RETRY_DELAY_S} --max-redirs ${MAX_REDIRECTS} ${FIRST_URL}
                fi
            fi
        fi
    fi

}
