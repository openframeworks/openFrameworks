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
		echo "-----------------------------------------------------------------"
		echo running $example
		cd $example
		if [ ! -d bin/"$example".app ]; then
	        echo building $example
			xcodebuild -configuration Release -target "$example" -project "$example.xcodeproj"
			if [ "$?" != "0" ]; then
				echo failed building $example
				exit
			fi
		fi
		cd bin
		./"$example".app/Contents/MacOS/"$example"
		cd ../../
		echo "-----------------------------------------------------------------"
		echo ""
	done
	cd ..
done
