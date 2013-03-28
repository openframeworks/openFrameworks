#include "testApp.h"

// OS X Audio Video Playback Example
//--------------------------------------------------------------
// This example shows how to use the OS X platform specific
// ofQTKitPlayer on its own without the cross platform
// ofVideoPlayer wrapper.  Apps you write in this way won't be
// cross platform, but will allow to access audio data.
//--------------------------------------------------------------

void testApp::setup(){
	ofBackground(27);

    // Enable audio frequency and volume metering.
    catMovie.enableAudioFrequencyMetering(2);
    catMovie.enableAudioVolumeMetering(2);

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
        int margin = 20;
        int currTop = margin;
        
        // Draw the movie.
        ofSetColor(ofColor::white);
        catMovie.draw((ofGetWidth() - catMovie.getWidth()) / 2, currTop);

        currTop += catMovie.getHeight() + margin;

        // Draw the volume levels.
        // The levels returned are measured in deciBels, where 0.0 means full volume, -6.0 means half volume,
        // -12.0 means quarter volume, and -inf means silence.
        int volHeight = 20;
        for (int i = 0; i < catMovie.getNumAudioVolumeChannels(); i++) {
            ofFill();
            ofSetColor(42);
            ofRect(margin, currTop, 980, volHeight);

            ofSetColor(255);
            float volumeLevel = catMovie.getAudioVolumeLevel(i);
            float volumeWidth;
            if (volumeLevel >= 0.0)
                volumeWidth = 980;
            else if (volumeLevel >= -6.0)
                volumeWidth = ofMap(volumeLevel, -6.0, 0.0, 490, 980);
            else if (volumeLevel >= -12.0)
                volumeWidth = ofMap(volumeLevel, -12.0, -6.0, 245, 490);
            else
                volumeWidth = ofMap(volumeLevel, -INFINITY, -12.0, 0, 245);
            ofRect(margin, currTop, volumeWidth, volHeight);

            ofSetColor(0);
            ofNoFill();
            ofRect(margin, currTop, 980, volHeight);

            currTop += volHeight + 10;
        }

        currTop += 10;

        ofSetColor(255);
        ofDrawBitmapString("-inf", 10, currTop);
        ofDrawBitmapString("-12 dB", 245, currTop);
        ofDrawBitmapString("-6 dB", 485, currTop);
        ofDrawBitmapString("0 dB", 985, currTop);

        currTop += margin;

        // Draw the audio frequency bands.
        // The levels returned are normalized (0.0 to 1.0), but measured in Hertz.
        int freqLeft = margin;
        int freqWidth = 420;
        int freqHeight = 380;
        for (int i = 0; i < catMovie.getNumAudioFrequencyChannels(); i++) {
            float bandTop = currTop;
            float bandHeight = freqHeight / (float)catMovie.getNumAudioFrequencyBands();

            ofFill();
            ofSetColor(42);
            ofRect(freqLeft, currTop, freqWidth, freqHeight);

            for (int j = 0; j < catMovie.getNumAudioFrequencyBands(); j++) {
                float bandWidth = MIN(1, catMovie.getAudioFrequencyLevel(i, j)) * freqWidth;
                
                ofSetColor(255 - (12 * j));
                ofRect(freqLeft, bandTop, bandWidth, bandHeight);

                if (i == 0) {
                    ofSetColor(255);
                    ofDrawBitmapString(ofToString(catMovie.getAudioFrequencyMeteringBand(j), 1) + " Hz", 470, bandTop + 18);
                }

                bandTop += bandHeight;
            }

            ofNoFill();
            ofSetColor(0);
            ofRect(freqLeft, currTop, freqWidth, freqHeight);

            freqLeft = 580;
        }
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
