#!/bin/bash

cd  ../../apps/examples/
for example in $( ls )
do
echo "-----------------------------------------------------------------"
echo "cleaning " $example
cd $example/
xcodebuild -configuration Debug -target "$example" -project "$example.xcodeproj" clean
if [ "$?" != "0" ]; then
   echo failed cleaning $example
   exit
fi
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
