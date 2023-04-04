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
cd $OUTPUT_FOLDER
ls -la
pwd
