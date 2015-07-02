#!/bin/bash
OF_DIR=`pwd`;
echo $OF_DIR;
TEST_DIR="$OF_DIR/scripts/osx"
cd $TEST_DIR
  echo "Looking for OSX Template Project"
  if [ ! -d template/bin/emptyExample.app ]; then
      echo "-----------------------------------------------------------------"
 	    echo building emptyExample
      if [ ! -e template/emptyExample.xcodeproj ]; then
          echo "-----------------------------------------------------------------"
          echo no xcode project for emptyExample
      fi
      echo "Building openFrameworks - OSX Template Project"
      xcodebuild -configuration Release -target emptyExample -project template/emptyExample.xcodeproj
      ret=$?
      if [ $ret -ne 0 ]; then
 	        echo failed building emptyExample
 	        exit 1
      fi
      echo "-----------------------------------------------------------------"
  fi
cd $OF_DIR
