#!/bin/bash

cd ../../examples

for category in $( ls . )
do
	if [ "$category" = "android" -o "$category" = "ios" ]; then
		continue
	fi
	cd $category
	for example in $( ls . )
	do
		cd $example
		echo "-----------------------------------------------------------------"
		echo running $example
		if [ ! -d bin/"$example".app ]; then
	            echo building $example
		    if [ ! -d Makefile ]; then
			cp ../../../libs/openFrameworksCompiled/project/makefileCommon/Makefile.examples ./Makefile
		    fi
		    make Release
		    if [ "$?" != "0" ]; then
			echo failed building $example
			exit
		    fi
		fi
		./bin/"$example".app/Contents/MacOS/"$example"
		cd ..
		echo "-----------------------------------------------------------------"
		echo ""
	done
	cd ..
done
