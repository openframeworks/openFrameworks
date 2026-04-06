#!/bin/bash
# pipe commands to core openFrameworks scripts
OF_SCRIPT_VERSION=0.2.2
# Dan Rosser 2025
OF_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OF_DIR="$(realpath "$OF_DIR/../")"
OF_CORE_SCRIPT_DIR="$(realpath "$OF_DIR/scripts")"
OF_CORE_CI_SCRIPT_DIR="$(realpath "$OF_DIR/scripts/ci")"
OF_PG_INSTALLED_DIR="$(realpath "$OF_DIR/projectGenerator")"
VERBOSE=${VERBOSE:-0}
echoVerbose() {
    if [[ "$VERBOSE" = 1 ]]; then
        echo "$@"
    fi
}
echoVerbose "$(date): [openFrameworks: $@]"
autoDetectOS() {
    if [[ -z "$PLATFORM" ]]; then
        export OF_OS=$(uname -s | tr '[:upper:]' '[:lower:]')
        case "$OF_OS" in
            darwin | Dawin)
                export OF_PLATFORM="osx"
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
        macos | osx | vs | emscripten | msys2 | android)
            export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}"
            ;;
        *)
            echo "  Error: Unsupported platform: $OF_PLATFORM"
            exit 1
            ;;
    esac
}

autoDetectOS
echoVerbose " platfrom:[$OF_PLATFORM] arch:[$OF_ARCH]"
coreScriptPath
echoVerbose " coreScriptPath: [$OF_SCRIPT_PATH]"

printHelp() {
    cat << EOF
Usage: $0 <command> [subcommand] [options]

Commands:
  update [subcommand]    Update openFrameworks components
    libs                 Download openFrameworks libraries (default)
    libs [platform]      i.e update libs ios
    pg | projectgenerator  Download Project Generator

  version [subcommand]   Show version information
    of                   Show openFrameworks version (default)
    pg | projectgenerator Show Project Generator version

  upgrade [subcommand]   Upgrade openFrameworks components
    addons               Upgrade addons (backup recommended)
    apps                 Upgrade apps (backup recommended)

Options:
  -h, --help             Show this help message

Examples:
  $0 update              # Updates libraries
  $0 update pg           # Updates Project Generator
  $0 version             # Shows openFrameworks version
  $0 version pg          # Shows Project Generator version
  $0 upgrade addons      # Upgrades addons
  $0 upgrade apps        # Upgrades apps
EOF
}

runCommand() {
    local CMD=$1
    local SUBCMD=$2
    local SUBCMD2=$3
    local SCRIPT
    local PLATFORM_DIR="${SUBCMD2:-$OF_PLATFORM}"  # Use SUBCMD2 if provided, otherwise default to OF_PLATFORM
    local EXTRA_ARGS=$2

    case "$CMD" in
        help|--help)
            printHelp
            exit 0
            ;;
        # setup)
        #     echo "openFrameworks setup - TODO"
        #     SCRIPT="${OF_SCRIPT_PATH}/setup.sh"
        #     ;;
        update)
            echo "openFrameworks update"
            EXTRA_ARGS=""
            case "$SUBCMD" in
                ""|libs)
                    SCRIPT="${OF_CORE_SCRIPT_DIR}/${PLATFORM_DIR}/download_libs.sh"
                    ;;
                pg|projectgenerator)  # Calls download_projectGenerator.sh
                    SCRIPT="${OF_CORE_SCRIPT_DIR}/${PLATFORM_DIR}/download_projectGenerator.sh"
                    ;;
                *)
                    SCRIPT="${OF_CORE_SCRIPT_DIR}/${PLATFORM_DIR}/download_libs.sh"
                    exit 1
                    ;;
            esac
            ;;
        version)
            EXTRA_ARGS=""
            case "$SUBCMD" in
                ""|of)  # Default: Show openFrameworks version
                    OF_CONSTANTS_H="${OF_DIR}/libs/openFrameworks/utils/ofConstants.h"
                    SCRIPT="NO"
                    if [[ -f "$OF_CONSTANTS_H" ]]; then
                        OF_VERSION_MAJOR=$(grep "#define OF_VERSION_MAJOR" "$OF_CONSTANTS_H" | awk '{print $3}')
                        OF_VERSION_MINOR=$(grep "#define OF_VERSION_MINOR" "$OF_CONSTANTS_H" | awk '{print $3}')
                        OF_VERSION_PATCH=$(grep "#define OF_VERSION_PATCH" "$OF_CONSTANTS_H" | awk '{print $3}')
                        OF_VERSION_PRE_RELEASE=$(grep "#define OF_VERSION_PRE_RELEASE" "$OF_CONSTANTS_H" | awk '{print $3}' | tr -d '"')

                        OF_VERSION="${OF_VERSION_MAJOR}.${OF_VERSION_MINOR}.${OF_VERSION_PATCH}"
                        if [[ -n "$OF_VERSION_PRE_RELEASE" && "$OF_VERSION_PRE_RELEASE" != "0" ]]; then
                            OF_VERSION+="-${OF_VERSION_PRE_RELEASE}"
                        fi

                        echo "openFrameworks version: [$OF_VERSION]"
                    else
                        echo "Error: $OF_CONSTANTS_H not found."
                        exit 1
                    fi
                    ;;
                pg|projectgenerator)  # Show Project Generator version
                    SCRIPT="${OF_DIR}/projectGenerator/projectGenerator"
                    EXTRA_ARGS="--version"
                    ;;
                *)
                    echo "Unknown version action: $SUBCMD"
                    echo "Valid version actions: [of (default), pg, projectgenerator]"
                    exit 1
                    ;;
            esac
            ;;
        upgrade)
            echo "openFrameworks upgrade"
            case "$SUBCMD" in
                addons)
                    echo "Upgrading addons"
                    echo "Warning: This script will modify files in the addons folder. Stop and back up the folder. Commit all to local repos before proceeding."
                    read -p "Do you want to continue? (Y/n): " CONFIRM
                    if [[ ! "$CONFIRM" =~ ^[Yy]$ ]]; then
                        echo "Upgrade cancelled. No changes were made."
                        exit 0
                    fi
                    SCRIPT="${OF_CORE_SCRIPT_DIR}/dev/upgrade.sh"
                    ;;
                apps)
                    echo "Upgrading apps"
                    echo "Warning: This script will modify files in the Apps folder. Stop and back up the folder. Commit all to local repos before proceeding."
                    read -p "Do you want to continue? (Y/n): " CONFIRM
                    if [[ ! "$CONFIRM" =~ ^[Yy]$ ]]; then
                        echo "Upgrade cancelled. No changes were made."
                        exit 0
                    fi
                    SCRIPT="${OF_CORE_SCRIPT_DIR}/dev/upgrade.sh"
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

    if [[ "$SCRIPT" == "NO" ]]; then
        exit 0
    fi
    if [[ -x "$SCRIPT" ]]; then
        echo "runCommand: [$SCRIPT]"
        "$SCRIPT" "$EXTRA_ARGS"
    elif [[ -f "$SCRIPT" ]]; then
        echo "Error: Script for ['$CMD'] exists but is not executable at: [$SCRIPT]"
        exit 1
    else
        echo "Error: Command ['$CMD'] not implemented yet. [$SCRIPT]"
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
