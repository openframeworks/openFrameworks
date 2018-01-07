#!/bin/bash
export PATH=~/.local/share/junest/bin:$PATH
ROOT=$(cd $(dirname $0); pwd -P)
source $ROOT/scripts/ci/ccache.sh
junest -u $ROOT/build_junest.sh
