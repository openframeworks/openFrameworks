#!/bin/bash

export LC_ALL=C
cd ../../apps/addonsExamples

for example in $( ls . )
do
echo "-----------------------------------------------------------------"
echo "building " + $example
cd $example
#codeblocks --build --target="Debug" "$example.cbp"
#codeblocks --build --target="Release" "$example.cbp"
make Debug
ret=$?
if [ $ret -ne 0 ]; then
  echo error compiling $example
  exit
fi
make Release
ret=$?
if [ $ret -ne 0 ]; then
  echo error compiling $example
  exit
fi

cd ../
echo "-----------------------------------------------------------------"
echo ""
done
