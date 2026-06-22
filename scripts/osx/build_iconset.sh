#!/bin/bash

# usage is: build_iconset.sh <path_to_1024x1024_PNG>
# scripts creates .icns beside the original PNG
# if orig PNG is not 1024x1024 it will get stretched/compressed
# if you have imagemagick install you get a free debug icon

IFS='.' read -ra ADDR <<< "$1"
ICONSET="${ADDR[0]}.iconset"
mkdir "$ICONSET"
sips -z 16 16     "$1" --out "$ICONSET/icon_16x16.png"
sips -z 32 32     "$1" --out "$ICONSET/icon_16x16@2x.png"
sips -z 32 32     "$1" --out "$ICONSET/icon_32x32.png"
sips -z 64 64     "$1" --out "$ICONSET/icon_32x32@2x.png"
sips -z 128 128   "$1" --out "$ICONSET/icon_128x128.png"
sips -z 256 256   "$1" --out "$ICONSET/icon_128x128@2x.png"
sips -z 256 256   "$1" --out "$ICONSET/icon_256x256.png"
sips -z 512 512   "$1" --out "$ICONSET/icon_256x256@2x.png"
sips -z 512 512   "$1" --out "$ICONSET/icon_512x512.png"
sips -z 1024 1024 "$1" --out "$ICONSET/icon_512x512@2x.png"
##cp "$1" "$ICONSET/icon_512x512@2x.png"
iconutil -c icns "$ICONSET"
rm -R "$ICONSET"
echo "plain icon done"

if ! command -v magick &> /dev/null
then
	echo "brew install imagemagick to generate a debug icon overlay"
	exit 1
else
	THE_SCRIPT_DIR=`dirname "$0"`
	echo "${THE_SCRIPT_DIR}"
	DEBUG_PNG="${ADDR[0]}_debug.png"
	magick composite -verbose  -gravity center "${THE_SCRIPT_DIR}/OF_debug_overlay.png" "$1" "${DEBUG_PNG}"
	ICONSET="${ADDR[0]}_debug.iconset"
	mkdir "$ICONSET"
	sips -z 16 16     "${DEBUG_PNG}" --out "$ICONSET/icon_16x16.png"
	sips -z 32 32     "${DEBUG_PNG}" --out "$ICONSET/icon_16x16@2x.png"
	sips -z 32 32     "${DEBUG_PNG}" --out "$ICONSET/icon_32x32.png"
	sips -z 64 64     "${DEBUG_PNG}" --out "$ICONSET/icon_32x32@2x.png"
	sips -z 128 128   "${DEBUG_PNG}" --out "$ICONSET/icon_128x128.png"
	sips -z 256 256   "${DEBUG_PNG}" --out "$ICONSET/icon_128x128@2x.png"
	sips -z 256 256   "${DEBUG_PNG}" --out "$ICONSET/icon_256x256.png"
	sips -z 512 512   "${DEBUG_PNG}" --out "$ICONSET/icon_256x256@2x.png"
	sips -z 512 512   "${DEBUG_PNG}" --out "$ICONSET/icon_512x512.png"
	sips -z 1024 1024 "${DEBUG_PNG}" --out "$ICONSET/icon_512x512@2x.png"
	###cp "$1" "$ICONSET/icon_512x512@2x.png"
	iconutil -c icns "$ICONSET"
	rm -R "$ICONSET"
	rm "${DEBUG_PNG}"
	echo "debug icon done"
fi
