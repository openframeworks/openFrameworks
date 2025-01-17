#!/bin/bash

export LC_ALL=C

OFDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OFDIR="$(realpath "$OF_DIR/../..")"
OFCORE_EXAMPLES_DIR="$(realpath "$OF_DIR/examples")"

for category in $( find "${OFCORE_EXAMPLES_DIR}/" -maxdepth 1 -type d )
do
    if [ "$category" = "${OFCORE_EXAMPLES_DIR}/android" -o "$category" = "${OFCORE_EXAMPLES_DIR}/ios" -o "$category" = "${OFCORE_EXAMPLES_DIR}/" -o "$category" = "${OFCORE_EXAMPLES_DIR}/tvOS" ]; then
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
