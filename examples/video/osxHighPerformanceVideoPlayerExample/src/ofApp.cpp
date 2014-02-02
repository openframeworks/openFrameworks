#include "ofApp.h"

// High Performance OS X Video Playback Example
//--------------------------------------------------------------
// This example shows how to use the OS X platform specific
// ofQTKitPlayer on its own without the cross platform
// ofVideoPlayer wrapper.  Apps you write in this way won't be
// cross platform, but can be faster for HD videos .
//--------------------------------------------------------------

void ofApp::setup(){
	ofBackground(255, 255, 255);

	frameByframe = false;

	// #1 Play videos with an alpha channel. ---------------------------
	// ofQTKitPlayer videos encoded with Alpha channels (e.g. Animation Codec etc).
    // The pixel format MUST be enabled prior to loading!
	// If an alpha channels is not used, setting a non-alpha pixel format
    // (e.g. OF_PIXELS_RGB) will increase performance.
	fingerMovie.setPixelFormat(OF_PIXELS_RGBA);

	//# 2 Decode Modes and video loading. -------------------------------
	// If you don't need direct access to pixel data, you can use ofQTKitPlayer's
    // highly optimized internal texture management system.  These mode are not
    // available when using the cross-platform ofVideoPlayer.

	// Texture only is fastest, but no pixel access allowed.
	// ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;

	// Pixels and texture together is faster than PIXEL_ONLY and manually uploaded textures.
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
	
	fingerMovie.loadMovie("movies/fingers.mov", decodeMode);

    // You can also load movies from URLs.
    // fingerMovie.loadMovie("http://labs.silentlycrashing.net/fingers.mov", decodeMode);
	
    //# 3 Synchronous seeking. ---------------------------
    // Like the legacy player, by default frames are available immediately for display
    // via setFrame().  In many cases when random access via setFrame() is not required,
    // significant speed increases can be gained by disabling synchronous seeking
    // particularly when playing high bitrate HD video.
    
    // In "asynchronous" seek mode, individual frames can still be randomly accessed,
    // but the pixels may not be available immediately without calling update() first.
    
    // fingerMovie.setSynchronousSeeking(false);

	fingerMovie.play();
}

//--------------------------------------------------------------
void ofApp::update(){
	fingerMovie.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetHexColor(0xFFFFFF);

    if (fingerMovie.isLoaded()) {
        fingerMovie.draw(20, 20);
        if(fingerMovie.getDecodeMode() != OF_QTKIT_DECODE_TEXTURE_ONLY){ //pixel access will not work in this mode
            ofSetHexColor(0x000000);
            unsigned char * pixels = fingerMovie.getPixels();
            ofPixelsRef pixelsRef = fingerMovie.getPixelsRef();
            
            // let's move through the "RGB(A)" char array
            // using the red pixel to control the size of a circle.

            for(int i = 4; i < 320; i += 8){
                for(int j = 4; j < 240; j += 8){
                    int pixelArrayIndex = pixelsRef.getPixelIndex(i,j);
                    unsigned char r = pixels[(j * 320 + i) * pixelsRef.getNumChannels()];
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
void ofApp::keyPressed(int key){
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
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	if(!frameByframe){
		int width = ofGetWidth();
		float pct = (float)x / (float)width;
		float speed = (2 * pct - 1) * 5.0f;
		fingerMovie.setSpeed(speed);
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if(!frameByframe){
		int width = ofGetWidth();
		float pct = (float)x / (float)width;
		fingerMovie.setPosition(pct);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(!frameByframe){
		fingerMovie.setPaused(true);
	}
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if(!frameByframe){
		fingerMovie.setPaused(false);
	}
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
