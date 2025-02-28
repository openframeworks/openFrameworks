#!/bin/bash

OF_ROOT=$PWD
SCRIPT_DIR="${BASH_SOURCE%/*}"
cd "${OF_ROOT}"

# Convert paths to Windows paths
OFW_ROOT=$(cygpath -w "$PWD")
PG_OFX_PATH="${OF_ROOT}\\projectGenerator\\projectGenerator.exe"
PG_OF_PATH="${OF_ROOT}/projectGenerator/projectGenerator.exe"

PGW_OFX_PATH=$(cygpath -w "$PG_OF_PATH")

PROJECTS=(
    "examples\\templates\\emptyExample"
    "examples\\templates\\allAddonsExample"
)

ADDONS=(
    ""
    "ofxAssimpModelLoader,ofxGui,ofxKinect,ofxNetwork,ofxOpenCv,ofxOsc,ofxSvg,ofxThreadedImageLoader,ofxXmlSettings"
)

# Debugging step
echo "Checking projectGenerator directory contents:"
ls -la "${OF_ROOT}/projectGenerator"

echo "Current working directory: $PWD"
echo "Updating projects with PG at:${PG_OF_PATH}"
for i in "${!PROJECTS[@]}"; do
    PROJECT=${PROJECTS[i]}
    ADDON=${ADDONS[i]}
    OPTIONS="-o\"${OF_ROOT}\" -v -a\"$ADDON\" -p\"vs\"  -t\"\" \"${OF_ROOT}\\${PROJECT}\""
    echo "Updating: ${PROJECT} with:${PG_OF_PATH}"
    echo "Command: ${PG_OF_PATH} ${OPTIONS}"
    eval "${PG_OF_PATH} ${OPTIONS}"
done
