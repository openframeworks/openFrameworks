#!/bin/bash

cd  ../../examples/
for folder in $( ls )
do
cd "$folder"
for example in $( ls )
do
echo "-----------------------------------------------------------------"
echo "building " $example
cd $example/
xcodebuild -configuration Release -target "$example" -project "$example.xcodeproj"
if [ "$?" != "0" ]; then
   echo failed building $example
   exit
fi
echo "example " $example " built - running example"
./bin/${example}.app/Contents/MacOS/${example}
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
cd ../
done