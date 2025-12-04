#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	//load the yolo5n model and the classes file which has the items it is trained to detect listed
	classify.setup("yolov5n.onnx", "classes.txt", false);
	
	//open the webcam
	grabber.setup(640, 480);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(30, 30, 30);

	grabber.update();
	if( grabber.isFrameNew() ){
			
		//get the ofPixels and convert to an ofxCvColorImage
		auto pixels = grabber.getPixels();
		colorImg.setFromPixels(pixels);

		//get the ofCvColorImage as a cv::Mat image to pass to the classifier
		auto cvMat = cv::cvarrToMat(colorImg.getCvImage());
		
		//get the restuls as a vector of detected items.
		//each result has an ofRectangle for the bounds, a label which identifies the object and the confidence of the classifier
		results = classify.classifyFrame(cvMat);
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255);
	grabber.draw(0, 0);
	
	//draw the detected objects on top of the webcam image 
	ofNoFill();
	ofSetColor(255, 0, 255);
	for(auto res: results){
		auto rect = res.rect;
		ofDrawRectangle(rect);
		ofDrawBitmapStringHighlight(res.label, rect.getTopLeft());
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
