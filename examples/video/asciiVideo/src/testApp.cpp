#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
    ofBackground(0,0,0);
	
	camWidth 		= 640;	// try to grab at this size. 
	camHeight 		= 480;
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);

    font.loadFont("Courier New Bold.ttf", 15);
    
    // this set of characters comes from processing: 
    //http://processing.org/learning/library/asciivideo.html
    
    asciiCharacters =  string(" .`-_':,;^=+/\"|)\\<>)iv%xclrs{*}I?!][1taeo7zjLunT#JCwfy325Fp6mqSghVd4EgXPGZbYkOA&8U$@KHDBWNMR0Q");
	
    ofEnableAlphaBlending();
}


//--------------------------------------------------------------
void testApp::update(){
	
	
	vidGrabber.grabFrame();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
 
    ofSetColor(255,255,255,ofMap(mouseX, 0,ofGetWidth(),0,255));
    vidGrabber.draw(0,0);
    
    
    unsigned char * pixels = vidGrabber.getPixels();
    
    ofSetHexColor(0xffffff);
	
    int brightness;
    int character;
    char temp;
    string tempStr;

    for (int i = 0; i < camWidth; i+= 12){
        for (int j = 0; j < camHeight; j+= 15){
             brightness = (pixels[(j*camWidth + i) * 3] +     
                              pixels[(j*camWidth + i) * 3 + 1] + 
                              pixels[(j*camWidth + i) * 3 + 2]) / 3;
             character = ofMap(brightness, 0,255, 0, asciiCharacters.size());
             temp = asciiCharacters[character];
             tempStr = "";
             tempStr += temp;
             font.drawString(tempStr, i, j);
            //ofDrawBitmapString(tempStr, ofPoint(i*10, j*10));
        }
    }
    
    
    //unsigned char * pixels = vidGrabber.getPixels();
		

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	// in fullscreen mode, on a pc at least, the 
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...
	
	if (key == 's' || key == 'S'){
		vidGrabber.videoSettings();
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
