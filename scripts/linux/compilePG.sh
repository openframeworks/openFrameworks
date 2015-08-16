#!/bin/bash

export LC_ALL=C

WHO=`who am i`;ID=`echo ${WHO%% *}`
GROUP_ID=`id --group -n ${ID}`

cd ../../apps/projectGenerator/projectGeneratorSimple
make Release
ret=$?
if [ $ret -ne 0 ]; then
  echo "there has been a problem compiling the projectGenerator"
  echo "please report this problem in the forums"
  exit $ret
fi
mkdir ../../../projectGenerator 2> /dev/null
cp -R bin/* ../../../projectGenerator
mv ../../../projectGenerator/projectGeneratorSimple ../../../projectGenerator/projectGenerator
mv ../../../projectGenerator/data/projectGeneratorSettings_production.xml ../../../projectGenerator/data/projectGeneratorSettings.xml
chown -R $ID:$GROUP_ID obj bin
chown -R $ID:$GROUP_ID ../../../projectGenerator
chown -R $ID:$GROUP_ID ../../../addons/obj


cd ../commandLine
make Release
ret=$?
if [ $ret -ne 0 ]; then
  echo "there has been a problem compiling the command line projectGenerator"
  echo "please report this problem in the forums"
  exit $ret
fi
sudo cp bin/projectGenerator /usr/local/bin/projectGenerator
cd ../../..
PG_OF_PATH=$PWD
grep PG_OF_PATH ~/.profile
if [ $? -eq 0 ]; then
    sed "s/PG_OF_ROOT=.*/PG_OF_PATH=${PG_OF_PATH}/"
else
    echo "export PG_OF_PATH=${PG_OF_PATH}" >> ~/.profile
fi

echo "GUI project generator was correctly installed in ${PG_OF_ROOT}/projectGenerator"
echo "Command line project generator was correctly installed and will work once the"
echo "terminal is restarted. To get more help run projectGenerator --help"
