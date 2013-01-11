#!/bin/bash

cd ../../examples

for category in $( ls . )
do
    if [ "$category" = "android" -o "$category" = "ios" ]; then
            continue
    fi
    cd $category
    for example in $( ls . )
    do
        echo "-----------------------------------------------------------------"
        echo building $example Debug
        cd $example
	xcodebuild -configuration Debug -target "$example" -project "$example.xcodeproj"
	if [ "$?" != "0" ]; then
	   echo failed building $example Debug
	   exit
	fi
        echo "-----------------------------------------------------------------"
        echo building $example Release
	xcodebuild -configuration Release -target "$example" -project "$example.xcodeproj"
	if [ "$?" != "0" ]; then
	   echo failed building $example Release
	   exit
	fi
        cd ../
        echo "-----------------------------------------------------------------"
        echo ""
    done
    cd ..
done
