#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	counter = 0;
	ofSetCircleResolution(100);
	texScreen.allocate(300,300,GL_RGB);
	ofBackground(230,230,240);
	
}
 

//--------------------------------------------------------------
void ofApp::update(){
	counter += 1;
}


//--------------------------------------------------------------
void ofApp::draw(){

	// 1st, draw on screen:
	ofSetHexColor(0x66CC33);	
	ofDrawRectangle(100,100,300,300);
	
	ofSetHexColor(0xffffff);
	ofPushMatrix();
		ofTranslate(200,200,0);
		ofRotate(counter,0,0,1);
		ofDrawCircle(0,0,80);
		ofDrawCircle(100,0,10);	// a small one
	ofPopMatrix();
	ofSetHexColor(0x333333);
	ofDrawBitmapString("(a) on screen", 150,200);

	ofSetHexColor(0xFFCC33);	
	ofDrawCircle(mouseX, mouseY,20);
	

	// 2nd, grab a portion of the screen into a texture
	// this is quicker then grabbing into an ofImage
	// because the transfer is done in the graphics card
	// as opposed to bringing pixels back to memory
	// note: you need to allocate the texture to the right size
	texScreen.loadScreenData(100,100,300,300);
	
	

	// finally, draw that texture on screen, how ever you want
	// (note: you can even draw the texture before you call loadScreenData, 
	// in order to make some trails or feedback type effects)
	ofPushMatrix();
		ofSetHexColor(0xffffff);
		ofTranslate(550,300,0);
		//glRotatef(counter, 0.1f, 0.03f, 0);
		float width = 200 + 100 * sin(counter/200.0f);
		float height = 200 + 100 * sin(counter/200.0f);;
		texScreen.draw(-width/2,-height/2,width,height);
	ofPopMatrix();

	ofPushMatrix();
		ofSetHexColor(0xffffff);
		ofTranslate(700,210,0);
		ofRotate(counter, 0.1f, 0.03f, 0);
		texScreen.draw(-50,-50,100,100);
	ofPopMatrix();

	ofSetHexColor(0x333333);
	ofDrawBitmapString("(b) in a texture, very meta!", 500,200);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
