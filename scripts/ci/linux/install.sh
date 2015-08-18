#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
sudo $ROOT/scripts/linux/ubuntu/install_dependencies.sh;
