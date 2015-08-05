#!/bin/bash
set -ev
echo "Installing documentation dependencies"
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
sudo apt-get -qq update;
# don't install recommends to avoid pulling in the huge doxygen-latex
# not a problem after 12.04, as doxygen-latex was demoted to suggests
sudo apt-get install --no-install-recommends python-software-properties doxygen ncftp;
gem install sass --version "=3.2.12";
gem install compass --version "=0.12.2";
cd $ROOT/scripts;
git clone https://github.com/halfdanJ/ofDocGenerator;
cd ofDocGenerator;
npm install;
