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
		cd $example
		if [ ! -d Makefile ]; then
		    cp ../../../libs/openFrameworksCompiled/project/makefileCommon/Makefile.examples ./Makefile
		fi
		echo "-----------------------------------------------------------------"
		echo cleaning $example
		make clean
		if [ "$?" != "0" ]; then
			echo failed cleaning $example
			exit
		fi
		cd ..
		echo "-----------------------------------------------------------------"
		echo ""
	done
	cd ..
done
