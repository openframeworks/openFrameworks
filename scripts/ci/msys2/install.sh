#!/bin/bash
set -ev #verbose; exit immediatly
ROOT=$(pwd -P)

function usage {
    echo usage:
    echo ./install.sh [--help] [--noconfirm] [--msystem=]
    echo --help:
    echo display this message
    echo
    echo --noconfirm:
    echo install packages without user confirmation
    echo --msystem:
    echo specify the msystem value
}

# Analyse script arguments
confirm="--noconfirm"
msystem=""
while [[ $# > 0 ]] ; do
    arg=$1
    shift
    if [ "$arg" == "--noconfirm" ]; then
        confirm="--noconfirm"
        continue
    fi
    if [ "$arg" == "--help" ]; then
        usage
        exit 1
    fi
    if [[ "$arg" == "--msystem="* ]]; then
        msystem="${arg#*=}"
        continue
    fi
    echo Invalid argument: $arg
    usage
    exit 1
done


$ROOT/scripts/msys2/install_dependencies.sh $confirm --msystem=$msystem
$ROOT/scripts/msys2/download_libs.sh --silent -a $msystem
if [[ $MINGW_PACKAGE_PREFIX ]]; then
    pacman -S $confirm $MINGW_PACKAGE_PREFIX-ccache
fi
