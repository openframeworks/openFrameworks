#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

for category in $( find ../../examples -maxdepth 1 -type d )
do
    if [ "$category" = "../../examples/android" -o "$category" = "../../examples/ios" -o "$category" = "../../examples/gles" -o "$category" = "../../examples" ]; then
       	continue
    fi
    echo "-----------------------------------------------------------------"
    echo running ALL examples in $category
    echo "-----------------------------------------------------------------"

    for example in $( find $category -maxdepth 1 -type d )
    do
        if [ "$example" = "$category" ]; then
       	    continue
       	fi

        if [ ! -d "$example"/bin/$(basename $example).app ]; then
            echo "-----------------------------------------------------------------"
       	    echo building $example
            if [ ! -e "$example"/$(basename $example).xcodeproj ]; then
                echo "-----------------------------------------------------------------"
                echo no xcode project for $example
                continue
            fi
            xcodebuild -configuration Release -target $(basename $example) -project $example/$(basename $example).xcodeproj
            ret=$?
            if [ $ret -ne 0 ]; then
       	        echo failed building $example
       	        exit
            fi
            echo "-----------------------------------------------------------------"
        fi
        echo running $example
       	echo "-----------------------------------------------------------------"
        "$example"/bin/$(basename $example).app/Contents/MacOS/$(basename $example)
       	echo "-----------------------------------------------------------------"
    done
done
