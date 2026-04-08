#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

# emconfigure ./configure
emcc --clear-cache
emcc --version

make clean
rm -rf obj
rm -rf ../../../libs/openFrameworksCompiled/lib/emscripten
rm -rf ../../../addons/obj/emscripten

EMCC_DEBUG=0 emmake make clean

EMCC_DEBUG=0 emmake make

#echo "Generating CSP..."
# bash ../../../libs/openFrameworksCompiled/project/emscripten/generate_csp.sh

echo "openFrameworks Build Complete"

