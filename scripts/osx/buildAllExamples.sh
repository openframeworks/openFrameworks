#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

for category in $( find ../../examples -maxdepth 1 -type d )
do
    if [ "$category" = "../../examples/android" -o "$category" = "../../examples/ios" -o "$category" = "../../examples" ]; then
       	continue
    fi
    echo "-----------------------------------------------------------------"
    echo building ALL examples in $category

    for example in $( find $category -maxdepth 1 -type d )
    do
        if [ "$example" = "$category" ]; then
       	    continue
       	fi

       	if [ ! -e "$example"/$(basename $example).xcodeproj ]; then
       	    echo "-----------------------------------------------------------------"
       	    echo no xcode project for $example
       	    continue
       	fi

        echo "-----------------------------------------------------------------"
        echo building $example Debug
        xcodebuild -configuration Debug -target $(basename $example) -project $example/$(basename $example).xcodeproj
        ret=$?
        if [ $ret -ne 0 ]; then
       	    echo failed building $example Debug
       	    exit
        fi

        echo "-----------------------------------------------------------------"
       	echo building $example Release
       	xcodebuild -configuration Release -target $(basename $example) -project $example/$(basename $example).xcodeproj
       	ret=$?
       	if [ $ret -ne 0 ]; then
       	    echo failed building $example Release
       	    exit
       	fi
       	echo "-----------------------------------------------------------------"
       	echo ""
    done
done
