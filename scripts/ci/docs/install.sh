#!/bin/bash
set -ev
echo "Installing documentation dependencies"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
gem install sass --version "=3.2.12"
gem install compass --version "=0.12.2"
cd $ROOT/scripts
git clone https://github.com/halfdanJ/ofDocGenerator
cd ofDocGenerator
npm install
