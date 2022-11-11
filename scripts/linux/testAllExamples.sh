#!/bin/bash

export LC_ALL=C
cd ../../examples

THREADS=""
if [[ "$1" == -j* ]]; then
  THREADS="$1"
fi

for category in $( ls . )
do
    if [ "$category" = "android" -o "$category" = "ios" -o "$category" = "gles" ]; then
            continue
    fi
    cd $category
    for example in $( ls . )
    do
        if [[ "$example" == osx* ]]
        then
            continue
        fi    
        echo "-----------------------------------------------------------------"
        echo "building " + $example
        cd $example
        make Debug ${THREADS}
        ret=$?
        if [ $ret -ne 0 ]; then
          echo error compiling $example
          exit
        fi
        make Release ${THREADS}
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
