#!/bin/bash

cd  ../../examples/basicExamples/
for example in $( ls )
do
echo "-----------------------------------------------------------------"
echo "building " $example
cd $example/
xcodebuild -configuration Debug -target "$example" -project "$example.xcodeproj"
if [ "$?" != "0" ]; then
   echo failed building $example
   exit
fi
echo "example " $example " built - running example"
./bin/${example}Debug.app/Contents/MacOS/${example}Debug
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
