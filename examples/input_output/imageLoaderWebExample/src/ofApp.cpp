#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	mouseX = 100;
	loading=false;
	ofRegisterURLNotification(this);
	
	//to load synchronously
	img.load("https://openframeworks.cc/about/0.jpg");
}

//--------------------------------------------------------------
void ofApp::urlResponse(ofHttpResponse & response){
	if(response.status==200 && response.request.name == "about"){
		img.load(response.data);
		loading=false;
	}else{
		cout << response.status << " " << response.error << " for request " << response.request.name << endl;
		if(response.status!=-1) loading=false;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofSetColor(220);
	ofDrawBitmapString("hit spacebar to load image from web", 10, ofGetHeight()-20);
	if(loading)
		ofDrawBitmapString("loading...", 10, ofGetHeight()+20);
	float divider = ofMap( mouseX, 0, ofGetWidth(), 1, 48, true );
	
	if(img.isAllocated()){
		for(int y = 0; y < img.getHeight(); y+= divider){
			for(int x = 0; x < img.getWidth(); x+=divider){
				ofColor c = img.getColor(x, y);
				
				ofSetColor( c.r, c.g, c.b );
				ofDrawCircle( x, y, divider/2 );
			}
		}
		
		ofSetColor(255);
		img.draw(img.getWidth(), 0);	
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofUnregisterURLNotification(this);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	img.clear();
	ofLoadURLAsync("https://openframeworks.cc/about/0.jpg","about");
	loading =true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
