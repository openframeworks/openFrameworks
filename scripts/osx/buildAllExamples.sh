#!/bin/bash

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

       	if [ ! -e "$example"/Makefile ]; then
       	    cp ../../libs/openFrameworksCompiled/project/makefileCommon/Makefile.examples "$example"/Makefile
       	fi

        echo "-----------------------------------------------------------------"
        echo building $example Debug
        make Debug -j2 -C $example
        ret=$?
        if [ $ret -ne 0 ]; then
       	    echo failed building $example Debug
       	    exit
        fi

        echo "-----------------------------------------------------------------"
       	echo building $example Release
       	make Release -j2 -C $example
       	ret=$?
       	if [ $ret -ne 0 ]; then
       	    echo failed building $example Release
       	    exit
       	fi
       	echo "-----------------------------------------------------------------"
       	echo ""
    done
done
