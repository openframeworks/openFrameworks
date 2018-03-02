#!/bin/bash
set -ev #verbose; exit immediatly
ROOT=$(pwd -P)

$ROOT/scripts/msys2/install_dependencies.sh --noconfirm
$ROOT/scripts/msys2/download_libs.sh --silent
pacman -S --noconfirm ccache
