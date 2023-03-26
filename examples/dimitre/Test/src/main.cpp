//#include "ofMain.h"
#include "ofApp.h"
#include "ofAppRunner.h"
#include "ofWindowSettings.h"

int main( ){
	ofSetupOpenGL(500,260,OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new ofApp());
}
