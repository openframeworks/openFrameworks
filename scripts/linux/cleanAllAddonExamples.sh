#!/bin/bash

#careful...

cd ../../apps/addonsExamples

for example in $( ls . )
do
echo "-----------------------------------------------------------------"
echo "cleaning " + $example
cd $example
rm -rf obj
rm -rf *.layout
rm -rf *.backup
rm -rf *.depend
rm *~
cd bin
rm -rf libs
rm -rf *.sh
rm $example
rm $example"_debug"
cd ../
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
