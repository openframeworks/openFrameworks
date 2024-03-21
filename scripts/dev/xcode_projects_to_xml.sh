#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

pwd

echo "Converting Xcode pbxproj to XML"
plutil -convert xml1 ../templates/osx/emptyExample.xcodeproj/project.pbxproj
echo "-"
plutil -convert xml1 ../templates/tvOS/emptyExample.xcodeproj/project.pbxproj
echo "--"
plutil -convert xml1 ../templates/ios/emptyExample.xcodeproj/project.pbxproj
echo "---"
plutil -convert xml1 ../../libs/openFrameworksCompiled/project/osx/openFrameworksLib.xcodeproj/project.pbxproj
echo "----"
plutil -convert xml1 ../../libs/openFrameworksCompiled/project/tvOS/tvOS+OFLib.xcodeproj/project.pbxproj
echo "-----"
plutil -convert xml1 ../../libs/openFrameworksCompiled/project/ios/ios+OFLib.xcodeproj/project.pbxproj
echo "------"

echo " Completed !"