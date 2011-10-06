#!/bin/bash

examples=$(ls -1d example*/*.xcodeproj)

for cur in $examples
do
	projectName=`echo "$cur" | sed -e 's/example-[^\/]*\/\([^.]*\)\(\.xcodeproj\)/\1/g'`
	if [ "$projectName" != "EmptyExample" ]
	then
		projectFile=$cur"/project.pbxproj"
		replace="s/EmptyExample/"$projectName"/g"
		cp example-empty/EmptyExample.xcodeproj/project.pbxproj $projectFile
		sed -i "" -e $replace $projectFile

		xcconfigFile=$cur"/../Project.xcconfig"
		cp example-empty/Project.xcconfig $xcconfigFile
	fi
done