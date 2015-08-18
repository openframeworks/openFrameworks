#!/bin/bash
set -ev
# Work around apt-get memory limitations on Travis by installing some packages
# separately.
# This can hopefully be removed when switching to 14.04 on Travis
sudo add-apt-repository ppa:ubuntu-toolchain-r/test --yes
sudo apt-get update -qq
sudo apt-get install -qq g++-4.9
