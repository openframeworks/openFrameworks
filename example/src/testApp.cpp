#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(127,127,127);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	ofDrawBitmapString("this app doesn't do anything :) \nIt's just a test to see if many of the common addons \ncan work in the same place w/ no include \nor linking issues...\n\nif you're seeing this, it's good!", 100,100);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
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
void testApp::mouseReleased(){

}
