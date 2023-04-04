#!/bin/bash
set -ev
ROOT=${GITHUB_WORKSPACE}

cd ROOT
pwd
ls -la

OUTPUT_FOLDER=$ROOT/out

mkdir $OUTPUT_FOLDER

lastversion=$(date +%Y%m%d)

pwd
ls -la

cd $OUTPUT_FOLDER

pwd
ls -la

./$ROOT/scripts/dev/create_package.sh linux64 $lastversion master gcc6


