#!/bin/bash
set -ev #verbose; exit immediatly
ROOT=$(pwd -P)

$ROOT/scripts/msys2/install_dependencies.sh --noconfirm
$ROOT/scripts/msys2/download_libs.sh --silent
if [[ $MINGW_PACKAGE_PREFIX ]]; then 
    pacman -S --noconfirm $MINGW_PACKAGE_PREFIX-ccache
fi
