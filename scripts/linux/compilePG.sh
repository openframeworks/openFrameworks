#!/bin/bash

export LC_ALL=C

OF_ROOT=../..

make Release -C ${OF_ROOT}/apps/projectGenerator/projectGeneratorSimple
ret=$?
if [ $ret -ne 0 ]; then
  echo "there has been a problem compiling the projectGenerator"
  echo "please report this problem in the forums"
  exit $ret
fi
rm -rf ${OF_ROOT}/projectGenerator
mkdir ${OF_ROOT}/projectGenerator 2> /dev/null
cp -Rf ${OF_ROOT}/apps/projectGenerator/projectGeneratorSimple/bin/* ${OF_ROOT}/projectGenerator/
mv ${OF_ROOT}/projectGenerator/projectGeneratorSimple ${OF_ROOT}/projectGenerator/projectGenerator
mv ${OF_ROOT}/projectGenerator/data/projectGeneratorSettings_production.xml ${OF_ROOT}/projectGenerator/data/projectGeneratorSettings.xml

make Release -C ${OF_ROOT}/apps/projectGenerator/commandLine
ret=$?
if [ $ret -ne 0 ]; then
  echo "there has been a problem compiling the command line projectGenerator"
  echo "please report this problem in the forums"
  exit $ret
fi

echo
echo
echo "To copy the command line project generator we need root permissions."
echo "If you don't won't the command line project generator just press enter here"
sudo cp ${OF_ROOT}/apps/projectGenerator/commandLine/bin/projectGenerator /usr/local/bin/projectGenerator
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
echo "GUI project generator was correctly installed in ${PG_OF_PATH}/projectGenerator"
if [ $CORRECT_PATH -eq 0 ]; then
    echo "Command line project generator was correctly installed"
    echo "But there was a previous OF install in $PREV_OF_PATH" 
    echo "If you want to change the OF path change the value of PG_OF_PATH in ~/.profile"
else
    if [ "$PREV_OF_PATH" != "" ]; then
        echo "Command line project generator was correctly installed"
        echo "To get more help run projectGenerator --help"
    else
        echo "export PG_OF_PATH=${PG_OF_PATH}" >> ~/.profile
        echo "Command line project generator was correctly installed and will work once the"
        echo "terminal is restarted. To get more help run projectGenerator --help"
    fi
fi
