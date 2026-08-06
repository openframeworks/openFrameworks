#!/usr/bin/env bash
# Downloads Apple multi-target package → lib/macos/*.xcframework
# Default clean is platform-scoped (lib/macos only); pass --full-clean for include wipe.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

if [ ! -z ${BITS+x} ]; then
    ../dev/download_libs.sh -p macos -b -a $BITS "$@"
else
    ../dev/download_libs.sh -p macos -b "$@"
fi
