#!/usr/bin/env bash
# osx host package; Apple multi-target xcframeworks prefer scripts/macos/download_libs.sh
# Platform-clean maps osx → lib/macos so sibling platforms are preserved.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"
if [ ! -z ${BITS+x} ]; then
    ../dev/download_libs.sh -p osx -b -a $BITS "$@"
else
    ../dev/download_libs.sh -p osx -b "$@"
fi
