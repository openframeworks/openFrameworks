#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# run linux script so DRY is followed
$ROOT/scripts/ci/linux/run_tests.sh
