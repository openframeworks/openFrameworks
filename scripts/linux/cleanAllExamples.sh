#!/bin/bash

#careful...
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
        echo "cleaning " + $example
        cd $example
	ret=$?
	if [ $ret -ne 0 ]; then
		echo error trying to cd $example
		exit 1
	fi 
	make clean
        rm -rf obj 2> /dev/null
        rm -rf *.layout 2> /dev/null
        rm -rf *.backup 2> /dev/null
        rm -rf *.depend 2> /dev/null
        rm *~ 2> /dev/null
        cd ..
        echo "-----------------------------------------------------------------"
        echo ""
    done
    cd ..
done
