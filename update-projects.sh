#!/bin/bash

directories=(
"example-background"
"example-bayer"
"example-contours-advanced"
"example-contours-basic"
"example-contours-color"
"example-contours-tracking"
"example-difference"
"example-face"
"example-flow"
"example-homography"
"example-threshold"
"example-undistortion")

projects=(
"BackgroundExample"
"BayerExample"
"ContoursAdvancedExample"
"ContoursBasicExample"
"ContoursColorExample"
"ContoursTrackingExample"
"DifferenceExample"
"FaceExample"
"FlowExample"
"HomographyExample"
"ThresholdExample"
"UndistortionExample")

n=${#directories[@]}

for ((i=0; i<$n; i++));
do
	curDirectory=${directories[i]}
	curProject=${projects[i]}
	target=$curDirectory/$curProject.xcodeproj/project.pbxproj
	replace="s/EmptyExample/"$curProject"/g"
	cp example-empty/EmptyExample.xcodeproj/project.pbxproj $target
	sed -i "" -e $replace $target
done
