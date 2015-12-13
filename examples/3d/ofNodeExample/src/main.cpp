#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2);
    settings.width = 900;
    settings.height = 500;
    ofCreateWindow(settings);

	ofRunApp(new ofApp());

}
