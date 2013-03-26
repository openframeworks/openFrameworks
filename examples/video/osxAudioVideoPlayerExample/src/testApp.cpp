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
        // Draw the movie.
        ofSetColor(ofColor::white);
        catMovie.draw(20, 20);

        // Draw the audio frequency bands.
        ofSetColor(ofColor::black);
        ofNoFill();
        ofRect(520, 20, catMovie.getWidth(), catMovie.getHeight());
        ofFill();
        int totalNumBands = catMovie.getAudioFrequencyLevels()->numChannels * catMovie.getAudioFrequencyLevels()->numFrequencyBands;
        float bandWidth = catMovie.getWidth() / (float)totalNumBands;
        for (int i = 0; i < catMovie.getAudioFrequencyLevels()->numChannels; i++) {
            for (int j = 0; j < catMovie.getAudioFrequencyLevels()->numFrequencyBands; j++) {
                int index = (i * catMovie.getAudioFrequencyLevels()->numFrequencyBands) + j;
                float bandHeight = catMovie.getAudioFrequencyLevels()->level[index] * catMovie.getHeight();

                ofSetColor(255 / totalNumBands * (index + 1), 0, 0);
                ofRect(520 + index * bandWidth, 20 + catMovie.getHeight() - bandHeight, bandWidth, bandHeight);
            }
        }

        // Draw the volume level.
        // The levels returned are measured in decibels, where 0.0 means full volume, -6.0 means half volume,
        // -12.0 means quarter volume, and -inf means silence.
        ofSetColor(ofColor::black);
        ofNoFill();
        ofRect(20, 420, 980, 32);
        ofDrawBitmapString("-inf", 10, 472);
        ofDrawBitmapString("-12.0", 250, 472);
        ofDrawBitmapString("-6.0", 490, 472);
        ofDrawBitmapString("0.0", 990, 472);
        ofFill();
        ofSetColor(ofColor::gray);
        float volumeLevel = catMovie.getAudioVolumeLevels()->level[0];
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
