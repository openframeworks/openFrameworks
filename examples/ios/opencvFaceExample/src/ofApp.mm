#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
    ofSetOrientation( OF_ORIENTATION_90_RIGHT );
	
	finder.setup("haarcascade_frontalface_default.xml");
	
	#ifdef USE_CAMERA 
		ofSetFrameRate(20);
		grabber.setDesiredFrameRate(20);
		grabber.initGrabber(480, 360);
		
		int w = grabber.getWidth();
		int h = grabber.getHeight();
		
		//we use different settings for the camera
		//so we can get a good frame rate 
		colorCv.allocate(w,h);
		colorCvSmall.allocate(w/4, h/4);
		grayCv.allocate(w/4, h/4);	
		
		finder.setNeighbors(1);
		finder.setScaleHaar(1.5);
	#else 
		img.loadImage("test.jpg");
	#endif 

}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(255,255,255);	
	
	#ifdef USE_CAMERA		
		grabber.update();
		colorCv = grabber.getPixels();
		colorCvSmall.scaleIntoMe(colorCv, CV_INTER_NN);
		grayCv = colorCvSmall;
		finder.findHaarObjects(grayCv);	
		faces = finder.blobs;
	#else
		//we don't really need to do this every frame
		//but it simulates closer what the camera demo would be doing
		finder.findHaarObjects(img);	
	#endif

	faces = finder.blobs;		
	cout << " found " << faces.size() << endl; 
	
}

//--------------------------------------------------------------
void ofApp::draw(){	
	
	ofSetColor(255);
	
	float scaleFactor = 1.0;
	#ifdef USE_CAMERA
		grabber.draw(0, 0);
		scaleFactor = 4.0;
	#else 
		img.draw(0,0);
	#endif
	
	ofPushStyle();
		ofNoFill();
		ofSetColor(255, 0, 255);
		for(int k = 0; k < faces.size(); k++){
			ofRectangle rect(faces[k].boundingRect.x * scaleFactor, faces[k].boundingRect.y * scaleFactor, faces[k].boundingRect.width * scaleFactor, faces[k].boundingRect.width * scaleFactor);
			ofRect(rect);
		}
	ofPopStyle();
	
	ofEnableAlphaBlending();
	ofSetColor(230, 0, 255, 200);
	ofRect(0, 0, ofGetWidth(), 16);
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("face detector: hold this way up", 5, 12);
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs& touch){

}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    
}

