#!/bin/bash

SECONDS=0

set -ev #verbose; exit immediatly
ROOT=$(pwd -P)

$ROOT/scripts/msys2/install_dependencies.sh --noconfirm

echo ">>> install.sh end install_dependencies $SECONDS";

$ROOT/scripts/msys2/download_libs.sh --silent

echo ">>> install.sh end download_libs $SECONDS";
if [[ $MINGW_PACKAGE_PREFIX ]]; then 
    pacman -S --noconfirm $MINGW_PACKAGE_PREFIX-ccache
fi
