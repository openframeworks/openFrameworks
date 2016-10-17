OF_ROOT=$PWD
mkdir -p ~/projectGenerator
cd ~/projectGenerator
wget http://ci.openframeworks.cc/projectGenerator/projectGenerator-vs.zip 2> /dev/null
unzip projectGenerator-vs.zip 2> /dev/null
rm projectGenerator-vs.zip
cd $OF_ROOT
PG_OF_PATH=$OF_ROOT ~/projectGenerator/projectGenerator.exe examples/templates/emptyExample
PG_OF_PATH=$OF_ROOT ~/projectGenerator/projectGenerator.exe examples/templates/allAddonsExample
