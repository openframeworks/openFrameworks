#!/bin/bash

cd  ../../apps/addonExamples/
for example in $( ls )
do
echo "-----------------------------------------------------------------"
echo "building " $example
cd $example/
xcodebuild -configuration Debug -target "openFrameworks" -project "$example.xcodeproj"
echo "example " $example " built - running example"
./bin/openFrameworksDebug.app/Contents/MacOS/openFrameworksDebug
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
