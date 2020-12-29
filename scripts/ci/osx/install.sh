#!/bin/bash
set -ev
brew update
brew install gdb
brew install ccache
brew upgrade coreutils

SDK_PATH=$(xcrun --sdk macosx --show-sdk-path)
echo "SDK PATH IS $SDK_PATH"
export DEVELOPER_DIR=$SDK_PATH
