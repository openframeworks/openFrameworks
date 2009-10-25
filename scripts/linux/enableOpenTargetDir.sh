#!/bin/bash


# this applies to all examples in this OF folder structure

cd ../
echo "-----------------------------------------------------------------"
echo "setting all examples to open on build"
find . -name '*.cbp' | xargs perl -pi -e 's/name=\"nautilusOnBuild\" value=\"false/name=\"nautilusOnBuild\" value=\"true/g'
echo "all examples set!"
echo "-----------------------------------------------------------------"
cd scripts

