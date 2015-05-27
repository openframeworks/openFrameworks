#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right
    nImages = DIR.listDir("images/of_logos/");

 	images = new ofImage[nImages];
    //you can now iterate through the files as you like
    for(int i = 0; i < nImages; i++){
		images[i].load(DIR.getPath(i));
    }
    currentImage = 0;

    ofBackground(255,255,255);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	float textSpace = 40;
	
    if (nImages > 0){
        ofSetHexColor(0xffffff);
        images[currentImage].draw(210, textSpace,  images[currentImage].width/1.5,  images[currentImage].height/1.5);
        ofSetHexColor(0x999999);
        string pathInfo = "current path is: ";
		pathInfo += DIR.getPath(currentImage);
				
		ofDrawBitmapString(pathInfo, 20, 20);
		ofDrawBitmapString("touch screen to advance image \n\nmany thanks to hikaru furuhashi\nfor the OFs!" , 210, images[currentImage].height/1.5 + 20 + textSpace);
    }

    ofSetHexColor(0x000000);
    for(int i = 0; i < nImages; i++){
            if (currentImage == i) ofSetHexColor(0xff0000);
            else ofSetHexColor(0x000000);
            string fileInfo = "file " + ofToString(i+1) + " = " + DIR.getName(i); // +  "path is " + DIR.getPath(i);
            ofDrawBitmapString(fileInfo, 20 ,i*14 + textSpace);
    }

	
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	if (nImages > 0){
		currentImage++;
		currentImage %= nImages;
	}
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
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
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
