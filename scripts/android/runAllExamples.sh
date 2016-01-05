#!/bin/bash
#
# This script requires following:
# - ANDROID_HOME should be set 
#     export ANDROID_HOME=~/Library/Android/sdk/platforms/
# - tools and platform-tools should be added to PATH
#     export PATH=$PATH:~/Library/Android/sdk/platform-tools/
#     export PATH=$PATH:~/Library/Android/sdk/tools/

for category in $( find ../../examples -maxdepth 1 -type d )
do
    if [ "$category" = "../../examples/android" ]; then
       	echo "-----------------------------------------------------------------"
        echo running ALL android examples in $category

        for example in $( find $category -maxdepth 1 -type d )
        do
            if [ "$example" = "$category" ]; then
                continue
            fi

            exampleName=$(basename "$example")

            if [ ! -e "$example"/build/outputs/apk ]; then
                echo "-----------------------------------------------------------------"
                echo no apk output folder for $example
                continue
            fi

            echo Installing $exampleName
            adb install "$example"/build/outputs/apk/"$exampleName"-debug.apk
            echo Starting $exampleName
            adb shell am start -a android.intent.action.MAIN -n cc.openframeworks."$exampleName"/.OFActivity

            echo 
            echo 
            read -p "$exampleName started! Press [Enter] key to start next example..."

            adb shell am force-stop cc.openframeworks."$exampleName"

        done
    fi
    
done
