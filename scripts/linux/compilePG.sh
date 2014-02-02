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
sed -i "s/..\/..\/..\/..\//..\//g" ../../../projectGenerator/data/settings/projectGeneratorSettings.xml
chown -R $ID:$GROUP_ID obj bin
chown -R $ID:$GROUP_ID ../../../projectGenerator
chown -R $ID:$GROUP_ID ../../../addons/obj
