#!/bin/bash

export LC_ALL=C
cd ../../apps/examples

for example in $( ls . )
do
echo "-----------------------------------------------------------------"
echo "building " + $example
cd $example
#codeblocks --build --target="Debug" "${example}_$1.cbp"
#ret=$?
#if [ $ret -ne 0 ]; then
#exit
#fi
#codeblocks --build --target="Release" "$example.cbp"
make Debug
ret=$?
if [ $ret -ne 0 ]; then
  echo error comiling $example
  exit
fi
make Release
ret=$?
if [ $ret -ne 0 ]; then
  echo error comiling $example
  exit
fi
cd bin
./$example
cd ../../
echo "-----------------------------------------------------------------"
echo ""
done
