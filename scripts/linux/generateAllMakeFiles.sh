#!/bin/bash

export LC_ALL=C
cd ../../apps/examples

for example in $( ls . )
do
echo "-----------------------------------------------------------------"
echo "building " + $example
export example_arch=${example}_${1}
cd $example
echo "creating makefile for " + $example_arch.cbp
cbp2makefile.py $example_arch.cbp Makefile_$1
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
