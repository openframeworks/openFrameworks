#!/bin/bash
set -ev
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
# Add compiler flag to reduce memory usage to enable builds to complete
# see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56746#c7
# the "proper" way does not work currently:
#export CXXFLAGS="$(CXXFLAGS) --param ftrack-macro-expansion=0"
#CUSTOMFLAGS="-ftrack-macro-expansion=0"
source ~/emscripten-sdk/emsdk_env.sh
source $ROOT/scripts/ci/ccache.sh

echo "**** Building OF core ****"
cd $ROOT
# this carries over to subsequent compilations of examples
#echo "PLATFORM_CFLAGS += $CUSTOMFLAGS" >> libs/openFrameworksCompiled/project/emscripten/config.emscripten.default.mk
sed -i "s/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = .*/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g0/" libs/openFrameworksCompiled/project/emscripten/config.emscripten.default.mk
cd libs/openFrameworksCompiled/project
emmake make Debug # USE_CCACHE=1

echo "**** Building emptyExample ****"
cd $ROOT/scripts/templates/linux64
emmake make Debug # USE_CCACHE=1

echo "**** Building allAddonsExample ****"
cd $ROOT
cp scripts/templates/linux64/Makefile examples/templates/allAddonsExample/
cp scripts/templates/linux64/config.make examples/templates/allAddonsExample/
cd examples/templates/allAddonsExample/
sed -i s/ofxOsc// addons.make
sed -i s/ofxNetwork// addons.make
#sed -i s/ofxSvg// addons.make
sed -i s/ofxKinect// addons.make
sed -i s/ofxThreadedImageLoader// addons.make
#sed -i s/ofxAssimpModelLoader// addons.make
#sed -i s/ofxGui// addons.make
#sed -i s/ofxXmlSettings// addons.make
#sed -i s/ofxOpenCv// addons.make

sed -i "s/#include \"ofxOsc.h\"//" src/ofApp.h
sed -i "s/#include \"ofxNetwork.h\"//" src/ofApp.h
#sed -i "s/#include \"ofxSvg.h\"//" src/ofApp.h
sed -i "s/#include \"ofxKinect.h\"//" src/ofApp.h
sed -i "s/#include \"ofxThreadedImageLoader.h\"//" src/ofApp.h
#sed -i "s/#include \"ofxAssimpModelLoader.h\"//" src/ofApp.h
#sed -i "s/#include \"ofxGui.h\"//" src/ofApp.h
#sed -i "s/#include \"ofxXmlSettings.h\"//" src/ofApp.h
#sed -i "s/#include \"ofxOpenCv.h\"//" src/ofApp.h

sed -i "s/ofxTCPClient client;//" src/ofApp.h
sed -i "s/ofxTCPServer server;//" src/ofApp.h
sed -i "s/ofxOscSender osc_sender;//" src/ofApp.h
#sed -i "s/ofxSVG svg;//" src/ofApp.h
sed -i "s/ofxKinect kinect;//" src/ofApp.h
sed -i "s/ofxThreadedImageLoader .*;//" src/ofApp.h
#sed -i "s/ofxAssimpModelLoader .*;//" src/ofApp.h
#sed -i "s/ofxPanel .*;//" src/ofApp.h
#sed -i "s/ofxXmlSettings .*;//" src/ofApp.h
#sed -i "s/ofxCv.* .*;//" src/ofApp.h

emmake make Debug # USE_CCACHE=1
