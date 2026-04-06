#!/bin/sh
cd "$(dirname "$0")"

plutil -convert json -r ./emptyExample.xcodeproj/project.pbxproj
echo "OF XCode template conversion to JSON complete"
