#!/bin/bash

cd  ../../apps/examples/
for example in $( ls )
do
echo "-----------------------------------------------------------------"
echo "building " $example
cd $example/
xcodebuild -configuration Debug -target "$example" -project "$example.xcodeproj"
echo "example " $example " built - running example"
./bin/${example}Debug.app/Contents/MacOS/${example}Debug
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
