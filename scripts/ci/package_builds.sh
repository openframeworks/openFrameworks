#!/bin/bash
set -ev
ROOT=${GITHUB_WORKSPACE}
pwd
cd $ROOT
pwd
ls -la

OUTPUT_FOLDER=$ROOT/out

mkdir $OUTPUT_FOLDER

lastversion=$(date +%Y%m%d)

git submodule update --init --recursive
git submodule update --recursive --remote
cd apps/projectGenerator
git pull origin master

cd $OUTPUT_FOLDER

$ROOT/scripts/dev/create_package.sh linux64 $lastversion master gcc6
pwd
ls -la
cd $ROOT

#export a list of files for the next step - matching .zip or .tar anything and including the relative paths
echo "FILES_OUT=$( (ls -t out/*.zip 2> /dev/null || true) && (ls -t out/*.tar* 2> /dev/null || true) )" >> $GITHUB_OUTPUT
