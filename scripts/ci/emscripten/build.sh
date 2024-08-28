#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# Add compiler flag to reduce memory usage to enable builds to complete
# see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56746#c7
# the "proper" way does not work currently:
#export CXXFLAGS="$(CXXFLAGS) --param ftrack-macro-expansion=0"
#CUSTOMFLAGS="-ftrack-macro-expansion=0"
# source ~/emscripten-sdk/emsdk_env.sh

echo "**** Building OF core ****"
cd $ROOT
# this carries over to subsequent compilations of examples
#echo "PLATFORM_CFLAGS += $CUSTOMFLAGS" >> libs/openFrameworksCompiled/project/emscripten/config.emscripten.default.mk
sed -i "s/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = .*/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g0/" libs/openFrameworksCompiled/project/emscripten/config.emscripten.default.mk
cd libs/openFrameworksCompiled/project
emmake make -j Debug

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/linux64
emmake make -j Debug

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/linux64/Makefile examples/templates/allAddonsExample/
cp scripts/templates/linux64/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
sed -i s/ofxOsc// addons.make
sed -i s/ofxNetwork// addons.make
sed -i s/ofxKinect// addons.make
sed -i s/ofxThreadedImageLoader// addons.make
sed -i s/ofxPoco// addons.make

sed -i "s/#include \"ofxOsc.h\"//" src/ofApp.h
sed -i "s/#include \"ofxNetwork.h\"//" src/ofApp.h
sed -i "s/#include \"ofxKinect.h\"//" src/ofApp.h
sed -i "s/#include \"ofxThreadedImageLoader.h\"//" src/ofApp.h
sed -i "s/#include \"ofxXmlPoco.h\"//" src/ofApp.h

sed -i "s/ofxTCPClient client;//" src/ofApp.h
sed -i "s/ofxTCPServer server;//" src/ofApp.h
sed -i "s/ofxOscSender osc_sender;//" src/ofApp.h
sed -i "s/ofxKinect kinect;//" src/ofApp.h
sed -i "s/ofxThreadedImageLoader .*;//" src/ofApp.h
sed -i "s/ofxXmlPoco .*;//" src/ofApp.h

emmake make -j Debug
