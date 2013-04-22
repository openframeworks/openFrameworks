#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	mouseX = 100;
	loading=false;
	ofRegisterURLNotification(this);
	
	//to load synchronously
	//image.loadImage("http://blah.com/img.jpg");
}

//--------------------------------------------------------------
void testApp::urlResponse(ofHttpResponse & response){
	if(response.status==200 && response.request.name == "tsingy_forest"){
		img.loadImage(response.data);
		loading=false;
	}else{
		cout << response.status << " " << response.error << endl;
		if(response.status!=-1) loading=false;
	}
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(0, 0, 0);
	ofDrawBitmapString("hit spacebar to load image from web", 10, ofGetHeight()/2);
	if(loading)
		ofDrawBitmapString("loading...", 10, ofGetHeight()+20);
	float divider = ofMap( mouseX, 0, ofGetWidth(), 1, 48, true );
	
	if(img.bAllocated()){
		for(int y = 0; y < img.getHeight(); y+= divider){
			for(int x = 0; x < img.getWidth(); x+=divider){
				ofColor c = img.getColor(x, y);
				
				ofSetColor( c.r, c.g, c.b );
				ofCircle( x, y, divider/2 );
			}
		}
		
		ofSetColor(255);
		img.draw(img.getWidth(), 0);	
	}
}

//--------------------------------------------------------------
void testApp::exit() {
	ofUnregisterURLNotification(this);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	img.clear();
	ofLoadURLAsync("http://images.wildmadagascar.org/pictures/bemaraha/tsingy_forest.JPG","tsingy_forest");
	loading =true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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
