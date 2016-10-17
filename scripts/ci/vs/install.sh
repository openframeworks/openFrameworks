OF_ROOT=$PWD
cd ~/
rm -rf projectGenerator
mkdir -p ~/projectGenerator
cd ~/projectGenerator
echo "Downloading projectGenerator from ci server"
wget http://ci.openframeworks.cc/projectGenerator/projectGenerator-vs.zip 2> /dev/null
unzip projectGenerator-vs.zip 2> /dev/null
rm projectGenerator-vs.zip

cd $OF_ROOT/examples/templates/allAddonsExample/
sed -i s/ofxKinect// addons.make
sed -i "s/#include \"ofxKinect.h\"//" src/ofApp.h
sed -i "s/ofxKinect kinect;//" src/ofApp.h

cd $OF_ROOT
PG_OF_PATH=$OF_ROOT ~/projectGenerator/projectGenerator.exe examples/templates/emptyExample
PG_OF_PATH=$OF_ROOT ~/projectGenerator/projectGenerator.exe examples/templates/allAddonsExample
