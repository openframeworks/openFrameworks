#include "testApp.h"

// OS X Audio Video Playback Example
//--------------------------------------------------------------
// This example shows how to use the OS X platform specific
// ofQTKitPlayer on its own without the cross platform
// ofVideoPlayer wrapper.  Apps you write in this way won't be
// cross platform, but will allow to access audio data.
//--------------------------------------------------------------

void testApp::setup(){
	ofBackground(255, 255, 255);

    // Enable audio frequency and volume metering.
    catMovie.enableAudioFrequencyMetering();
    catMovie.enableAudioVolumeMetering();

    // Load and start the movie.
	catMovie.loadMovie("movies/cat.mp4", OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
	catMovie.play();
}

//--------------------------------------------------------------
void testApp::update(){
	catMovie.update();
}

//--------------------------------------------------------------
void testApp::draw(){

    if (catMovie.isLoaded()) {
        int currLeft = 20;
        int currTop = 20;
        
        // Draw the movie.
        ofSetColor(ofColor::white);
        catMovie.draw(currLeft, currTop);

        // Draw the audio frequency bands.
        // The levels returned are normalized (0.0 to 1.0), but measured in Hertz.
        currLeft += catMovie.getWidth() + 20;
        int freqWidth = ofGetWidth() - currLeft - 20;
        int freqHeight = catMovie.getHeight();
        ofSetColor(ofColor::black);
        ofLine(currLeft, currTop, currLeft, currTop + freqHeight);
        int totalNumBands = catMovie.getNumAudioFrequencyChannels() * catMovie.getNumAudioFrequencyBands();
        float bandTop = currTop;
        float bandWidth;
        float bandHeight = freqHeight / (float)totalNumBands;
        for (int i = 0; i < catMovie.getNumAudioFrequencyChannels(); i++) {
            for (int j = 0; j < catMovie.getNumAudioFrequencyBands(); j++) {
                bandWidth = catMovie.getAudioFrequencyLevel(i, j) * freqWidth;

                ofSetColor(255, 0, 0);
                ofRect(currLeft, bandTop, bandWidth, bandHeight);

                ofSetColor(ofColor::black);
                ofDrawBitmapString(ofToString(catMovie.getAudioFrequencyMeteringBand(i, j), 1) + " Hz", currLeft + bandWidth + 15, bandTop + 15);

                bandTop += bandHeight;
            }
        }

        // Draw the volume level.
        // The levels returned are measured in deciBels, where 0.0 means full volume, -6.0 means half volume,
        // -12.0 means quarter volume, and -inf means silence.
        ofSetColor(ofColor::black);
        ofNoFill();
        ofRect(20, 420, 980, 32);
        ofDrawBitmapString("-inf", 10, 472);
        ofDrawBitmapString("-12 dB", 245, 472);
        ofDrawBitmapString("-6 dB", 485, 472);
        ofDrawBitmapString("0 dB", 985, 472);
        ofFill();
        ofSetColor(ofColor::gray);
        float volumeLevel = catMovie.getAudioVolumeLevel(0);
        float volumeWidth;
        if (volumeLevel >= 0.0) {
            volumeWidth = 980;
        }
        else if (volumeLevel >= -6.0) {
            volumeWidth = ofMap(volumeLevel, -6.0, 0.0, 490, 980);
        }
        else if (volumeLevel >= -12.0) {
            volumeWidth = ofMap(volumeLevel, -12.0, -6.0, 245, 490);
        }
        else {
            volumeWidth = ofMap(volumeLevel, -INFINITY, -12.0, 0, 245);
        }
        ofRect(20, 420, volumeWidth, 32);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
