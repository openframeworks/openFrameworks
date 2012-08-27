#include "testApp.h"

//High performance OS X Video Playback example
//--------------------------------------------------------------
// This example shows how to use the OS X platform specific ofQTKitPlayer on its own
// without the cross platform ofVideoPlayer wrapper
// Apps you write in this way won't be cross platform,
// but can be faster for HD videos 
//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255, 255, 255);

	frameByframe = false;

	//# 1 Videos with alpha ---------------------------
	// support alpha channel, but the pixel format must be enabled prior to loading!
	// Turning off alpha will increase performance if it's not needed
	fingerMovie.setPixelFormat(OF_PIXELS_RGBA);


	//# 2 Decode Modes. -------------------------------
	// If you don't need access to pixels and aren't using the ofQTKitPlayer inside of an ofVideoPlayer
	// You can use QTKit's internal texture management system which is streamlined.

	//Texture only is fastest, but no pixel access allowed
	ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;

	//pixels and texture together is faster than PIXEL_ONLY and manually upload textures.
	//ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
	
    fingerMovie.loadMovie("movies/fingers.mov", decodeMode);

    // You can even load URLs!
    //fingerMovie.loadMovie("http://labs.silentlycrashing.net/fingers.mov", decodeMode);

	//# 3 Synchronous scrubbing ---------------------------
	// In many cases you want to be certain a call to movie.setFrame(x) and then a call to movie.update() will result in
	// immediate calls to movie.getPixels() or movie.draw() containing frame x. This is slow to guarentuee,
	// and QTKit naturally wants to update on its own time, a few moments later.
	// If you are working in a way where this small delay is acceptable, then disabling synchronous scrubbing will
	// speed things up
	// Note that this can be turned on and off freely without having to reload the video
	fingerMovie.setSynchronousScrubbing(false);

	fingerMovie.play();
}

//--------------------------------------------------------------
void testApp::update(){
	fingerMovie.update();
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetHexColor(0xFFFFFF);

    if (fingerMovie.isLoaded()) {
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
