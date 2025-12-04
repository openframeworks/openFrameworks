#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

# Define the directory to the examples/graphics
EXAMPLES_DIR="../../examples/graphics"

echo "-----------------------------------------------------------------"
echo "Running ALL examples in $EXAMPLES_DIR"
echo "-----------------------------------------------------------------"

mkdir -p regressionTest

# Loop through each example in the graphics directory
for example in $( find $EXAMPLES_DIR -maxdepth 1 -type d )
do
    if [ "$example" = "$EXAMPLES_DIR" ]; then
        continue
    fi

    if [ ! -d "$example"/bin/$(basename $example).app ]; then
        echo "-----------------------------------------------------------------"
        echo "Building $example"
        if [ ! -e "$example"/$(basename $example).xcodeproj ]; then
            if [ ! -e "../../projectGenerator.app/Contents/MacOS/projectGenerator" ]; then
                ./../projectGenerator.app/Contents/MacOS/projectGenerator --recursive -posx -o../../ "../../examples/graphics/$example"
            elif [[ ! -e "../../projectGenerator/projectGenerator.app/Contents/MacOS/projectGenerator" ]]; then
                ./../projectGenerator/projectGenerator.app/Contents/MacOS/projectGenerator --recursive -posx -o../../ "../../examples/graphics/$example"
            fi
            errorcode=$?
            if [[ $errorcode -ne 0 ]]; then
                    exit $errorcode
            fi
            if [ ! -e "$example"/$(basename $example).xcodeproj ]; then
                echo "-----------------------------------------------------------------"
                echo "No Xcode project for $example"
                continue
            fi
        fi
        xcodebuild -configuration Release -target $(basename $example) -project $example/$(basename $example).xcodeproj
        ret=$?
        if [ $ret -ne 0 ]; then
            echo "Failed building $example"
            exit 1
        fi
        echo "-----------------------------------------------------------------"
    fi
    echo "Running $example"
    echo "-----------------------------------------------------------------"
    "$example"/bin/$(basename $example).app/Contents/MacOS/$(basename $example)
    echo "-----------------------------------------------------------------"
    # Copy the output PNG to the regressionTest folder
        PNG_PATH="$example/bin/data/$(basename $example).png"
        if [ -e "$PNG_PATH" ]; then
            cp "$PNG_PATH" regressionTest/
            echo "Copied $PNG_PATH to regressionTest/"
        else
            echo "No PNG found at $PNG_PATH"
        fi
done
