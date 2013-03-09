#include "testApp.h"

bool locked = false;

//--------------------------------------------------------------
void testApp::setup(){
	mainAppsCount = 0;
	TO.start();
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(0,0,0);   // black because threads are EVIL ;)
	mainAppsCount++;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
	TO.draw();

    string str = "I am a the main opengl thread.\nmy current count is: ";
	str += ofToString(mainAppsCount);
    ofDrawBitmapString(str, 350, 56);


    ofSetHexColor(0xff0033);

    ofDrawBitmapString("press 's' to stop the thread\npress 'a' to start the thread\npress '!' to test the exception handler (will log an error and stop the thread).", 50, 160);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    if (key == 'a'){
        TO.start();
    } else if (key == 's'){
        TO.stop();
    } else if (key == '!'){
        TO.throwErrorTest();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}