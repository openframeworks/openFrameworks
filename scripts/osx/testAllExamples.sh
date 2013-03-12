#!/bin/bash

for category in $( find ../../examples -maxdepth 1 -type d )
do
    if [ "$category" = "../../examples/android" -o "$category" = "../../examples/ios" -o "$category" = "../../examples" ]; then
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
	    if [ ! -e "$example"/Makefile ]; then
       	        cp ../../libs/openFrameworksCompiled/project/makefileCommon/Makefile.examples "$example"/Makefile
       	    fi
            make Release -j2 -C $example
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
