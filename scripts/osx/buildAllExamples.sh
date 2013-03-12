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
		if [ ! -d Makefile ]; then
		    cp ../../../libs/openFrameworksCompiled/project/makefileCommon/Makefile.examples ./Makefile
		fi
		echo "-----------------------------------------------------------------"
		echo building $example Debug
		make Debug
		if [ "$?" != "0" ]; then
			echo failed building $example Debug
			exit
		fi
		echo "-----------------------------------------------------------------"
		echo building $example Release
		make Release
		if [ "$?" != "0" ]; then
			echo failed building $example Release
			exit
		fi
		cd ../
		echo "-----------------------------------------------------------------"
		echo ""
	done
	cd ..
done
