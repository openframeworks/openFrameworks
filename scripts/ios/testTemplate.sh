#!/bin/bash
set -e
if [ "$SCRIPT_PATH" == "" ]; then SCRIPT_PATH="."; fi
echo "Looking for iOS Template Project"
if [ ! -d "$SCRIPT_PATH/template/bin/emptyExample.app" ]; then
    echo "---------------------------------"
    if [ ! -e "$SCRIPT_PATH/template/emptyExample.xcodeproj" ]; then
        echo "No Xcode project found for emptyExample"
        exit 1;
    fi
    echo "Building openFrameworks - iOS Template Project"
    xcodebuild -project "$SCRIPT_PATH/template/emptyExample.xcodeproj" -target emptyExample -sdk iphonesimulator ONLY_ACTIVE_ARCH=NO
    ret=$?
    if [ $ret -ne 0 ]; then
	        echo "Failed building emptyExample"
	        exit 1
    fi
    echo "---------------------------------"
fi
