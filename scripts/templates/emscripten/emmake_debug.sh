#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

emconfigure ./configure
emcc --version
emmake make -j Debug


emrun bin/em/emscripten_debug --browser chrome  --enable-unsafe-webgpu