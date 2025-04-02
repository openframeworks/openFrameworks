#!/bin/bash

examples_dir=../../examples

for category in $( find $examples_dir/ -maxdepth 1 -type d )
do
    if [ "$category" = "$examples_dir/android" -o "$category" = "$examples_dir/ios" -o "$category" = "$examples_dir/" ]; then
            continue
    fi
    
    for example in $( find $category -maxdepth 1 -type d | grep -v osx )
    do
	    if [ "$example" = "$category" ]; then
		    continue
	    fi
        echo "-----------------------------------------------------------------"
        echo "cleaning " + $example
        
	    make clean -C $example
        rm -rf $example/obj 2> /dev/null
        rm -rf $example/*.layout 2> /dev/null
        rm -rf $example/*.backup 2> /dev/null
        rm -rf $example/*.depend 2> /dev/null
        rm $example/*~ 2> /dev/null
        
        echo "-----------------------------------------------------------------"
        echo ""
    done
done

