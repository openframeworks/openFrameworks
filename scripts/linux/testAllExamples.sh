#!/bin/bash

export LC_ALL=C
OFDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OFDIR="$(realpath "$OF_DIR/../..")"
OFCORE_EXAMPLES_DIR="$(realpath "$OF_DIR/examples")"

cd ${OFCORE_EXAMPLES_DIR}

for category in $( ls . )
do
    if [ ! -d "$category" -o "$category" = "android" -o "$category" = "ios" -o "$category" = "gles" ]; then
            continue
    fi
    cd $category
    for example in $( ls . )
    do
        if [[ "$example" == osx* ]] || [[ "$example" != *Example ]]
        then
            continue
        fi    
        echo "-----------------------------------------------------------------"
        echo "building [" + $example "]"
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
        echo "build success [" + $example "]"
        cd bin
        ./$example
        cd ../../
        echo "-----------------------------------------------------------------"
        echo ""
    done
    cd ..
done
