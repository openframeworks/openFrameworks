OF_ROOT=$PWD
cd ~/
rm -rf projectGenerator
mkdir -p ~/projectGenerator
cd ~/projectGenerator
downloader() { if command -v wget 2>/dev/null; then wget $1 $2 $3; else curl -LO --retry 20 -O --progress $1 $2 $3; fi; }
echo "Downloading projectGenerator from ci server"
downloader http://ci.openframeworks.cc/projectGenerator/projectGenerator-vs.zip 2> /dev/null
unzip projectGenerator-vs.zip 2> /dev/null
rm projectGenerator-vs.zip

cd $OF_ROOT
PG_OF_PATH=$OF_ROOT ~/projectGenerator/projectGenerator.exe examples/templates/emptyExample
PG_OF_PATH=$OF_ROOT ~/projectGenerator/projectGenerator.exe examples/templates/allAddonsExample
