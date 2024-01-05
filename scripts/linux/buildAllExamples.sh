#!/bin/bash

export LC_ALL=C

for category in $( find ../../examples/ -maxdepth 1 -type d )
do
    if [ "$category" = "../../examples/android" -o "$category" = "../../examples/ios" -o "$category" = "../../examples/" -o "$category" = "../../examples/tvOS" ]; then
        continue
    fi

    echo category $category
    for example in $( find "$category" -maxdepth 1 -type d | grep -v osx )
    do
        if [ "$example" = "$category" ]; then
            continue
        fi

        echo -----------------------------------------------------------------
        echo building  $example

        #projectGenerator .
        make Debug -j -C "$example"
        ret=$?
        if [ $ret -ne 0 ]; then
            echo error compiling $example
            exit
        fi
        make Release -j -C "$example"
        ret=$?
        if [ $ret -ne 0 ]; then
            echo error compiling $example
            exit
        fi

        echo -----------------------------------------------------------------
        echo
    done
done
