#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	//load the image object
	original.load("myImage.jpg");
	
	//get the pixels from the image
	ofPixelsRef pixels = original.getPixels();
	
	//let's manipulate the pixels by iterating through them
	//or a rough bluescreen
	for(int y = 0; y < original.getHeight(); y++){
		for(int x = 0; x < original.getWidth(); x++){
			
			//if the blue is greater than the combination of  red and green together, delete that pixel
			ofColor color = original.getColor(x, y);
			if(color.b > color.g + color.r){
				pixels.setColor(x, y, ofColor::black);
			}
		}
	}

	//after we're done we need to put the pixels into the keyed image
	keyed.setFromPixels(pixels);
}

//--------------------------------------------------------------
void ofApp::update(){
	//no need to use this
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	//draw the image every frame, the changes you made in setup() will be there
	original.draw(0,0);
	keyed.draw(original.getWidth(),0);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 's'){
		//we can save out the modified version here
		keyed.save("keyed_image.png");
	}
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
