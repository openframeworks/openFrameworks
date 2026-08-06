#!/usr/bin/env bash
# Apple multi-target xcframeworks ship as the "macos" package (ios · tvos · … slices).
# Platform-clean only touches lib/macos; other platforms (android, emscripten, …) are kept.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"
if [ ! -z ${BITS+x} ]; then
    ../dev/download_libs.sh -p macos -b -a $BITS "$@"
else
    ../dev/download_libs.sh -p macos -b "$@"
fi
