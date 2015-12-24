#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2);
	settings.width = 1280;
	settings.height = 720;
    ofCreateWindow(settings);

    ofRunApp(new ofApp());

}
