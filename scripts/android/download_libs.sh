#!/usr/bin/env bash
# Platform-clean removes only lib/android; other platforms stay installed.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"
../dev/download_libs.sh -p android -b "$@"
