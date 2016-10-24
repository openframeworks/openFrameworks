#!/usr/bin/env bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OF_ROOT=$SCRIPT_DIR/../..
cd $OF_ROOT/libs
mkdir -p openFrameworksCompiled
if [ ! -e openFrameworksCompiled/project ]; then
    ln -s ../../build/projects openFrameworksCompiled/project
fi
if [ ! -e openFrameworksCompiled/lib ]; then
    ln -s ../../build/lib openFrameworksCompiled/lib
fi

if [ ! -e openFrameworks ]; then
    ln -s ../src openFrameworks
fi


