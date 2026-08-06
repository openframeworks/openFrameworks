#!/usr/bin/env bash
# Platform-clean removes only lib/emscripten; other platforms stay installed.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

if [ ! -z ${BITS+x} ]; then
    ../dev/download_libs.sh -p emscripten -b -a $BITS "$@"
else
    ../dev/download_libs.sh -p emscripten -b "$@"
fi
