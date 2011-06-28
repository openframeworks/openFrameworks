#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
    nImages = DIR.listDir("images/of_logos/");

 	images = new ofImage[nImages];
    //you can now iterate through the files as you like
    for(int i = 0; i < nImages; i++){
		images[i].loadImage(DIR.getPath(i));
    }
    currentImage = 0;

    ofBackground(255,255,255);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
	float textSpace = 40;
	
    if (nImages > 0){
        ofSetColor(0xffffff);
        images[currentImage].draw(210, textSpace,  images[currentImage].width/1.5,  images[currentImage].height/1.5);
        ofSetColor(0x999999);
        string pathInfo = "current path is: ";
		pathInfo += DIR.getPath(currentImage);
				
		ofDrawBitmapString(pathInfo, 20, 20);
		ofDrawBitmapString("touch screen to advance image \n\nmany thanks to hikaru furuhashi\nfor the OFs!" , 210, images[currentImage].height/1.5 + 20 + textSpace);
    }

    ofSetColor(0x000000);
    for(int i = 0; i < nImages; i++){
            if (currentImage == i) ofSetColor(0xff0000);
            else ofSetColor(0x000000);
            string fileInfo = "file " + ofToString(i+1) + " = " + DIR.getName(i); // +  "path is " + DIR.getPath(i);
            ofDrawBitmapString(fileInfo, 20 ,i*14 + textSpace);
    }

	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if (nImages > 0){
		currentImage++;
		currentImage %= nImages;
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

