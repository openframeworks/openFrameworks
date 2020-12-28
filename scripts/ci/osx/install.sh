#!/bin/bash
set -ev
brew update
brew install python@3.9
brew link python@3.9 --force
brew install gdb
#brew install ccache
brew upgrade coreutils
