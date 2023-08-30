#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

for category in $( find ../../examples -maxdepth 1 -type d )
do
    if [ "$category" = "../../examples/android" -o "$category" = "../../examples/ios" -o "$category" = "../../examples" ]; then
       	continue
    fi
    echo "-----------------------------------------------------------------"
    echo cleaning ALL examples in $category

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
        echo cleaning $example Debug
        xcodebuild -configuration Debug -target $(basename $example) -project $example/$(basename $example).xcodeproj clean
        ret=$?
        if [ $ret -ne 0 ]; then
       	    echo failed cleaning $example Debug
       	    exit
        fi

        echo "-----------------------------------------------------------------"
       	echo cleaning $example Release
       	xcodebuild -configuration Release -target $(basename $example) -project $example/$(basename $example).xcodeproj clean
       	ret=$?
       	if [ $ret -ne 0 ]; then
       	    echo failed cleaning $example Release
       	    exit
       	fi
        echo rm -rf $example/build 2> /dev/null
        rm -rf $example/build 2> /dev/null

       	echo "-----------------------------------------------------------------"
       	echo ""
    done
done
