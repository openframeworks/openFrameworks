#!/bin/bash
set -e
if [ "$SCRIPT_PATH" == "" ]; then SCRIPT_PATH="."; fi
echo "Looking for OSX Template Project"
if [ ! -d $SCRIPT_PATH/template/bin/emptyExample.app ]; then
    echo "---------------------------------"
    if [ ! -e $SCRIPT_PATH/template/emptyExample.xcodeproj ]; then
        echo "-----------------------------"
        echo "No Xcode project found for emptyExample"
    fi
    echo "Building openFrameworks - OSX Template Project"
    xcodebuild -configuration Release -target emptyExample -project $SCRIPT_PATH/template/emptyExample.xcodeproj
    ret=$?
    if [ $ret -ne 0 ]; then
          echo "Failed building emptyExample"
          exit 1
    fi
    echo "---------------------------------"
fi
