#!/usr/bin/env bash
# Bleeding-edge Linux download. Only the requested ARCH under lib/linux/<arch>
# is replaced; other platforms and other Linux arches stay installed.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"
../dev/download_libs.sh -p linux -b "$@"
