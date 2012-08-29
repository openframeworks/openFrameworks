#include "testApp.h"

//Async OS X Video Playback example
//--------------------------------------------------------------
// This example shows how to use the OS X platform specific ofQTKitPlayer on its own
// without the cross platform ofVideoPlayer wrapper
// Apps you write in this way won't be cross platform,
// but can be faster for HD videos 
//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255, 255, 255);

	frameByframe = false;

	// Texture only is fastest, but no pixel access allowed
	//ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;

	// Pixels and texture together is faster than PIXEL_ONLY and manually upload textures.
	ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;

    // Load a local file or a URL
    // Set the 3rd parameter to TRUE for asynchronous loading
    fingerMovie.loadMovie("movies/fingers.mov", decodeMode, false);
    //fingerMovie.loadMovie("http://labs.silentlycrashing.net/fingers.mov", decodeMode, true);

    // You can use this event to be notified when the video is ready for playback
    ofAddListener(fingerMovie.videoReadyEvent, this, &testApp::videoReady);
}

//--------------------------------------------------------------
void testApp::update(){
	fingerMovie.update();
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetHexColor(0xFFFFFF);

    if (fingerMovie.isReady()) {
        fingerMovie.draw(20, 20);
        if(fingerMovie.getDecodeMode() != OF_QTKIT_DECODE_TEXTURE_ONLY){ //pixel access will not work in this mode
            ofSetHexColor(0x000000);
            unsigned char * pixels = fingerMovie.getPixels();
            // let's move through the "RGB" char array
            // using the red pixel to control the size of a circle.
            for(int i = 4; i < 320; i += 8){
                for(int j = 4; j < 240; j += 8){
                    unsigned char r = pixels[(j * 320 + i) * 3];
                    float val = 1 - ((float)r / 255.0f);
                    ofCircle(400 + i, 20 + j, 10 * val);
                }
            }
        }
    }

    ofSetHexColor(0x000000);
    ofDrawBitmapString("press f to change", 20, 320);
    if(frameByframe){
        ofSetHexColor(0xCCCCCC);
    }
    ofDrawBitmapString("mouse speed position", 20, 340);
    if(!frameByframe){
        ofSetHexColor(0xCCCCCC);
    }
    else{ofSetHexColor(0x000000);
    }
    ofDrawBitmapString("keys <- -> frame by frame ", 190, 340);
    ofSetHexColor(0x000000);

    ofDrawBitmapString("frame: " + ofToString(fingerMovie.getCurrentFrame()) + "/" + ofToString(fingerMovie.getTotalNumFrames()), 20, 380);
    ofDrawBitmapString("duration: " + ofToString(fingerMovie.getPosition() * fingerMovie.getDuration(), 2) + "/" + ofToString(fingerMovie.getDuration(), 2), 20, 400);
    ofDrawBitmapString("speed: " + ofToString(fingerMovie.getSpeed(), 2), 20, 420);

    if(fingerMovie.getIsMovieDone()){
        ofSetHexColor(0xFF0000);
        ofDrawBitmapString("end of movie", 20, 440);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key){
	 case 'f':
		 frameByframe = !frameByframe;
		 fingerMovie.setPaused(frameByframe);
		 break;

	 case OF_KEY_LEFT:
		 fingerMovie.previousFrame();
		 break;

	 case OF_KEY_RIGHT:
		 fingerMovie.nextFrame();
		 break;

	 case '0':
		 fingerMovie.firstFrame();
		 break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	if(!frameByframe){
		int width = ofGetWidth();
		float pct = (float)x / (float)width;
		float speed = (2 * pct - 1) * 5.0f;
		fingerMovie.setSpeed(speed);
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(!frameByframe){
		int width = ofGetWidth();
		float pct = (float)x / (float)width;
		fingerMovie.setPosition(pct);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(!frameByframe){
		fingerMovie.setPaused(true);
	}
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(!frameByframe){
		fingerMovie.setPaused(false);
	}
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

//--------------------------------------------------------------
void testApp::videoReady(ofVideoReadyEventArgs &event){
    fingerMovie.play();
}
