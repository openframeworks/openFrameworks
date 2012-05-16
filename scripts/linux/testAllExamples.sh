#!/bin/bash

export LC_ALL=C
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
        echo "building " + $example
        cd $example
        make Debug
        ret=$?
        if [ $ret -ne 0 ]; then
          echo error compiling $example
          exit
        fi
        make Release
        ret=$?
        if [ $ret -ne 0 ]; then
          echo error compiling $example
          exit
        fi
        cd bin
        ./$example
        cd ../../
        echo "-----------------------------------------------------------------"
        echo ""
    done
    cd ..
done
