#!/bin/bash
# pipe commands to core openFrameworks scripts
OF_SCRIPT_VERSION=0.1.1
# Dan Rosser 2025
OF_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OF_DIR="$(realpath "$OF_DIR/../")"
OF_CORE_SCRIPT_DIR="$(realpath "$OF_DIR/scripts")"
OF_CORE_CI_SCRIPT_DIR="$(realpath "$OF_DIR/scripts/ci")"
OF_PG_INSTALLED_DIR="$(realpath "$OF_DIR/projectGenerator")"
echo "$(date): [openFrameworks: $@]"
autoDetectOS() {
    if [[ -z "$PLATFORM" ]]; then
        export OF_OS=$(uname -s | tr '[:upper:]' '[:lower:]')
        case "$OF_OS" in
            darwin | Dawin)
                export OF_PLATFORM="macos"
                export OF_ARCH=$(uname -m)
                ;;
            linux | Linux)
                export OF_PLATFORM="linux"
                export OF_ARCH=$(uname -m)
                ;;
            mingw* | cygwin* | msys*)
                export OF_PLATFORM="vs"
                export OF_ARCH=${MSYSTEM,,}
                ;;
            *)
                echo "Unsupported platform: $OF_OS"
                exit 1
                ;;
        esac
    else
    	export OF_OS=$(${OF_PLATFORM} | tr '[:upper:]' '[:lower:]')
    	export OF_ARCH=""
    fi
}

coreScriptPath() {
    case "$OF_PLATFORM" in
        linux)
            case "$OF_ARCH" in
                arm64 | jetson | armv7l | armv8l | aarch64)
                    export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}/${OF_ARCH}/"
                    ;;
                *)
                    export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}"
                    ;;
            esac
            ;;
        macos | vs | emscripten | msys2 | android)
            export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}"
            ;;
        *)
            echo "  Error: Unsupported platform: $OF_PLATFORM"
            exit 1
            ;;
    esac
}

autoDetectOS
echo " platfrom:[$OF_PLATFORM] arch:[$OF_ARCH]"
coreScriptPath
echo " coreScriptPath: [$OF_SCRIPT_PATH]"

runCommand() {
    local CMD=$1
    local SUBCMD=$2
    local SCRIPT

    case "$CMD" in
        setup)
            echo "openFrameworks setup"
            SCRIPT="${OF_SCRIPT_PATH}/setup.sh"
            ;;
        update)
            echo "openFrameworks update"
            SCRIPT="${OF_SCRIPT_PATH}/download_libs.sh"
            ;;
        upgrade)
            echo "openFrameworks upgrade"
            case "$SUBCMD" in
                addons)
                    echo "Upgrading addons"
                    SCRIPT="${OF_SCRIPT_PATH}/dev/upgrade.sh"
                    ;;
                apps)
                    echo "Upgrading apps"
                    echo "Warning: This script will modify files in the Apps folder. Stop and back up the folder. Commit all to local repos before proceeding."
                    read -p "Do you want to continue? (Y/n): " CONFIRM
                    if [[ ! "$CONFIRM" =~ ^[Yy]$ ]]; then
                        echo "Upgrade cancelled. No changes were made."
                        exit 0
                    fi
                    SCRIPT="${OF_SCRIPT_PATH}/dev/upgrade.sh"
                    ;;
                *)
                    echo "Unknown upgrade action: $SUBCMD"
                    echo "Valid upgrade actions: addons, apps"
                    exit 1
                    ;;
            esac
            ;;
        *)
            echo "Unknown command: $CMD"
            echo "Valid commands: setup, update, upgrade"
            exit 1
            ;;
    esac

    if [[ -x "$SCRIPT" ]]; then
        echo "runCommand: [$SCRIPT]"
        "$SCRIPT" "${@:2}"
    else
        echo "Error: Script for ['$CMD'] not found or not executable at: [$SCRIPT]"
        exit 1
    fi
}
if [[ $# -eq 0 ]]; then
    echo "Usage: $0 <command> [args...]"
    echo "Valid commands: setup, build, clean, etc."
    exit 1
fi
runCommand $@
EXIT_CODE=$?
echo "$EXIT_CODE"
exit ${EXIT_CODE}
