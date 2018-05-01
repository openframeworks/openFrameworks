#!/bin/bash

export LC_ALL=C

OF_ROOT=$(cd $(dirname $0)/../..; pwd -P)

make Release -C ${OF_ROOT}/apps/projectGenerator/commandLine
ret=$?
if [ $ret -ne 0 ]; then
  echo "There has been a problem compiling the command line projectGenerator."
  echo "Please report this problem in the forums."
  exit $ret
fi

echo
echo
read -p "Do you want to install the command line project generator? [Y/n] " -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "To copy the command line project generator we need root permission."
    sudo cp ${OF_ROOT}/apps/projectGenerator/commandLine/bin/projectGenerator /usr/local/bin/projectGenerator
    if [ ! $? -eq 0 ]; then
        echo "Failed to copy the projectGenerator file."
        exit
    fi
    cd ${OF_ROOT}
    PG_OF_PATH=$PWD

    CORRECT_PATH=0
    grep PG_OF_PATH ~/.profile
    if [ $? -eq 0 ]; then
        PREV_OF_PATH=$(grep PG_OF_PATH ~/.profile | sed "s/export PG_OF_PATH=\(.*\)/\1/")
        if [ "$PG_OF_PATH" == "$PREV_OF_PATH" ]; then
            CORRECT_PATH=1
        fi
    else
        CORRECT_PATH=1
    fi

    echo
    echo
    if [ $CORRECT_PATH -eq 0 ]; then
        echo "Command line project generator was correctly installed but there was a previous OF install in $PREV_OF_PATH"
        echo
        echo "You can change the OF path by adding the following line to ~/.profile:"
        echo "    export PG_OF_PATH=\"MY_FULL_PATH_TO_OF\""
    else
        if [ "$PREV_OF_PATH" != "" ]; then
            echo "Command line project generator was correctly installed"
            echo "To get started run: projectGenerator --help"
        else
            echo "The command line project generator was correctly installed, but the PG_OF_PATH environment variable has not been set yet."
            echo "You can create this variable by adding the following line to ~/.profile."
            echo "    export PG_OF_PATH=${PG_OF_PATH}"
            read -p "Do you want to add this line to ~/.profile now?? [Y/n] " -r
            echo
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                echo "export PG_OF_PATH=${PG_OF_PATH}" >> ~/.profile
                echo "PG_OF_PATH was added. Restart the console for the changes to take effect"
            fi
            echo "To get started run: projectGenerator --help"
        fi
    fi
fi
