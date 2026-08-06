#!/usr/bin/env bash
# Platform-clean removes only lib/msys2; shared include/bin need --full-clean.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

../dev/download_libs.sh -p msys2 -b "$@"
