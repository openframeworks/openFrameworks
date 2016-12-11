#!/bin/bash
set -ev

if [ -z ${OF_ROOT} ]; then
    OF_ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
fi

# Trusty/14.04 builds don't have databases running - disable this
# echo "Memory usage debugging output"
# ps aux --sort -rss | head -n10
# echo "Stop services started by default to save memory on Travis"
# # use sudo /etc/init.d/foo stop instead of sudo service foo stop because that
# # fails on travis sometimes, see http://stackoverflow.com/a/27410479/599884
# # maybe works consistently on 14.04
# sudo service mysql stop
# sudo service postgresql stop

sudo $OF_ROOT/scripts/linux/ubuntu/install_dependencies.sh -y;
