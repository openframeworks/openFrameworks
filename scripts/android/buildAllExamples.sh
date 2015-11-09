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
        echo building ALL android examples in $category

        for example in $( find $category -maxdepth 1 -type d )
        do
            if [ "$example" = "$category" ]; then
                continue
            fi


            if [ ! -e "$example"/build.gradle ]; then
                echo "-----------------------------------------------------------------"
                echo no gradle project for $example
                continue
            fi

            echo "-----------------------------------------------------------------"  
            echo Updating android project
            android update project -p $example
            if [ $? -ne 0 ]; then
              echo failed updating android project $example
              exit
            fi
            
            echo building $example
            (cd $example; gradle assembleDebug )
            if [ $? -ne 0 ]; then
              echo failed building $example Debug
              exit
            fi
        done
    fi
    
done
