#!/bin/bash


# this applies to all examples in this OF folder structure

cd ../
echo "-----------------------------------------------------------------"
echo "disabling open on build"
find . -name '*.cbp' | xargs perl -pi -e 's/name=\"nautilusOnBuild\" value=\"true/name=\"nautilusOnBuild\" value=\"false/g'
echo "all examples disabled!"
echo "-----------------------------------------------------------------"
cd scripts
