#!/bin/bash
set -ev
# capture failing exits in commands obscured behind a pipe
set -o pipefail

ROOT=$(docker exec -i emscripten pwd)

run(){
    echo "TARGET=\"emscripten\" $@"
    docker exec -it emscripten sh -c "TARGET=\"emscripten\" $@"
}

echo "**** Building OF core ****"

# this carries over to subsequent compilations of examples
run "sed -i \"s/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = .*/PLATFORM_OPTIMIZATION_CFLAGS_DEBUG = -g0/\" libs/openFrameworksCompiled/project/emscripten/config.emscripten.default.mk"
run "cd libs/openFrameworksCompiled/project; emmake make Debug"

echo "**** Building emptyExample ****"
run "cd $ROOT/scripts/templates/linux64; emmake make Debug"

echo "**** Building allAddonsExample ****"
run "cp $ROOT/scripts/templates/linux64/Makefile $ROOT/examples/templates/allAddonsExample/"
run "cp $ROOT/scripts/templates/linux64/config.make $ROOT/examples/templates/allAddonsExample/"
run "sed -i s/ofxOsc// $ROOT/examples/templates/allAddonsExample/addons.make"
run "sed -i s/ofxNetwork// $ROOT/examples/templates/allAddonsExample/addons.make"
run "sed -i s/ofxKinect// $ROOT/examples/templates/allAddonsExample/addons.make"
run "sed -i s/ofxThreadedImageLoader// $ROOT/examples/templates/allAddonsExample/addons.make"

run "sed -i \"s/#include \"ofxOsc.h\"//\" $ROOT/examples/templates/allAddonsExample/src/ofApp.h"
run "sed -i \"s/#include \"ofxNetwork.h\"//\" $ROOT/examples/templates/allAddonsExample/src/ofApp.h"
run "sed -i \"s/#include \"ofxKinect.h\"//\" $ROOT/examples/templates/allAddonsExample/src/ofApp.h"
run "sed -i \"s/#include \"ofxThreadedImageLoader.h\"//\" $ROOT/examples/templates/allAddonsExample/src/ofApp.h"

run "sed -i \"s/ofxTCPClient client;//\" $ROOT/examples/templates/allAddonsExample/src/ofApp.h"
run "sed -i \"s/ofxTCPServer server;//\" $ROOT/examples/templates/allAddonsExample/src/ofApp.h"
run "sed -i \"s/ofxOscSender osc_sender;//\" $ROOT/examples/templates/allAddonsExample/src/ofApp.h"
run "sed -i \"s/ofxKinect kinect;//\" $ROOT/examples/templates/allAddonsExample/src/ofApp.h"
run "sed -i \"s/ofxThreadedImageLoader .*;//\" $ROOT/examples/templates/allAddonsExample/src/ofApp.h"

run "cd $ROOT/examples/templates/allAddonsExample; emmake make Debug"
