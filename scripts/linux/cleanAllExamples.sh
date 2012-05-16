#!/bin/bash

#careful...
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
        echo "cleaning " + $example
        cd $example
	make clean
        rm -rf obj 2> /dev/null
        rm -rf *.layout 2> /dev/null
        rm -rf *.backup 2> /dev/null
        rm -rf *.depend 2> /dev/null
        rm *~ 2> /dev/null
        echo "-----------------------------------------------------------------"
        echo ""
    done
    cd ..
done
