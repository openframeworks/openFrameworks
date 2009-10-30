#!/bin/bash

# this applies to all examples in this OF folder structure

cd ../
echo "-----------------------------------------------------------------"
echo "setting all examples to run on build"
find . -name '*.cbp' | xargs perl -pi -e 's/name=\"runOnBuild\" value=\"false/name=\"runOnBuild\" value=\"true/g'
echo "all examples set!"
echo "-----------------------------------------------------------------"
cd scripts
