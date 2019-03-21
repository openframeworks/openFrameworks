#!/bin/bash
set -ev
ROOT=$(docker exec -i emscripten pwd)

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
sed -i s/ofxKinect// addons.make
sed -i s/ofxThreadedImageLoader// addons.make

sed -i "s/#include \"ofxOsc.h\"//" src/ofApp.h
sed -i "s/#include \"ofxNetwork.h\"//" src/ofApp.h
sed -i "s/#include \"ofxKinect.h\"//" src/ofApp.h
sed -i "s/#include \"ofxThreadedImageLoader.h\"//" src/ofApp.h

sed -i "s/ofxTCPClient client;//" src/ofApp.h
sed -i "s/ofxTCPServer server;//" src/ofApp.h
sed -i "s/ofxOscSender osc_sender;//" src/ofApp.h
sed -i "s/ofxKinect kinect;//" src/ofApp.h
sed -i "s/ofxThreadedImageLoader .*;//" src/ofApp.h

emmake make Debug # USE_CCACHE=1
