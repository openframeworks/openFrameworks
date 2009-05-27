#!/bin/bash

# this applies to all examples in this OF folder structure

cd ../
echo "-----------------------------------------------------------------"
echo "disabling run on build"
find . -name '*.cbp' | xargs perl -pi -e 's/name=\"runOnBuild\" value=\"true/name=\"runOnBuild\" value=\"false/g'
echo "runOnBuild disabled!"
echo "-----------------------------------------------------------------"
cd scripts
