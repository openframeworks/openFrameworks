#!/bin/bash

export LC_ALL=C
cd ../../examples

for category in $( find . -maxdepth 1 -type d )
do
    if [ "$category" = "./android" -o "$category" = "./ios" -o "$category" = "." ]; then
            continue
    fi
    cd $category
    for example in $( find . -maxdepth 1 -type d | grep -v osx )
    do        
	if [ "$example" = "." ]; then
                continue
        fi

        echo "-----------------------------------------------------------------"
        echo "building " + $category/$example
        cd $example
	#projectGenerator .
        make Debug -j2
        ret=$?
        if [ $ret -ne 0 ]; then
          echo error compiling $example
          exit
        fi
        make Release -j2
        ret=$?
        if [ $ret -ne 0 ]; then
          echo error compiling $example
          exit
        fi
        cd ../
        echo "-----------------------------------------------------------------"
        echo ""
    done
    cd ..
done
