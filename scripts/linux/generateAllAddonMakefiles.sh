#!/bin/bash

export LC_ALL=C
cd ../../apps/addonsExamples

for example in $( ls . )
do
echo "-----------------------------------------------------------------"
echo "building " + $example
example_arch = $example_$1
cd $example
#codeblocks --build --target="Debug" "$example.cbp"
#codeblocks --build --target="Release" "$example.cbp"
#make Debug
#make Release
cbp2makefile.py $example_arch.cbp Makefile
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
