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
		echo cleaning $example Debug
		cd $example
		xcodebuild -configuration Debug -target "$example" -project "$example.xcodeproj" clean
		if [ "$?" != "0" ]; then
			echo failed cleaning $example Debug
			exit
		fi
		echo "-----------------------------------------------------------------"
		echo cleaning $example Release
		xcodebuild -configuration Release -target "$example" -project "$example.xcodeproj" clean
		if [ "$?" != "0" ]; then
			echo failed cleaning $example Release
			exit
		fi
		cd ..
		echo "-----------------------------------------------------------------"
		echo ""
	done
	cd ..
done
