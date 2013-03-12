#!/bin/bash

for category in $( find ../../examples -maxdepth 1 -type d )
do
    if [ "$category" = "../../examples/android" -o "$category" = "../../examples/ios" -o "$category" = "../../examples" ]; then
       	continue
    fi
    echo "-----------------------------------------------------------------"
    echo cleaning ALL examples in $category

    for example in $( find $category -maxdepth 1 -type d )
    do
        if [ "$example" = "$category" ]; then
       	    continue
       	fi

       	if [ ! -e "$example"/Makefile ]; then
       	    cp ../../libs/openFrameworksCompiled/project/makefileCommon/Makefile.examples "$example"/Makefile
       	fi

	echo "-----------------------------------------------------------------"
	echo cleaning $example
	make clean -C $example
	ret=$?
	if [ $ret -ne 0 ]; then
       	    echo failed cleaning $example
       	    exit
       	fi
	echo rm -rf $example/obj 2> /dev/null
	rm -rf $example/obj 2> /dev/null

       	echo "-----------------------------------------------------------------"
       	echo ""
    done
done
