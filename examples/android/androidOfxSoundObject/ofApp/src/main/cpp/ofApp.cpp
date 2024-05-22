#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(255,255,255);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();


    //----- Loading sound player begin -------.
    // ofSystemLoadDialog() does not work on Android

    player.load( ofToDataPath("Sounds/popisdead.mp3", true),
            //set the following to true if you want to stream the audio data from the disk on demand instead of
            //reading the whole file into memory. Default is false
                 false);
    //----- Loading sound player end -------.

    //----- Sound stream setup begin -------.
    // the sound stream is in charge of dealing with your computers audio device.
    // lets print to the console the sound devices that can output sound.
    ofxSoundUtils::printOutputSoundDevices();

    auto outDevices = ofxSoundUtils::getOutputSoundDevices();

    // IMPORTANT!!!
    // The following line of code is where you set which audio interface to use.
    // the index is the number printed in the console inside [ ] before the interface name
    // You can use a different input and output device.

    int outDeviceIndex = 0;

    //cout << ofxSoundUtils::getSoundDeviceString(outDevices[outDeviceIndex], false, true) << endl;

    ofSoundStreamSettings soundSettings;
    soundSettings.numInputChannels = 0;
    soundSettings.numOutputChannels = 2;
    soundSettings.sampleRate = player.getSoundFile().getSampleRate();
    soundSettings.bufferSize = 256;
    soundSettings.numBuffers = 1;

    stream.setup(soundSettings);

    // it is important to set up which object is going to deliver the audio data to the sound stream.
    // thus, we need to set the stream's output. The output object is going to be the last one of the audio signal chain, which is set up further down
    stream.setOutput(output);

    //-------Sound stream setup end -------.


    // ------ waveforms ---------
    // the waveformDraw class setup receives the rectangle where it is going to be drawn
    // you can skip this and pass this while drawing if you are changing where this is going to be drawn.
    // As well, the waveformDraw class inherits from ofRectangle so you can access the functions of the latter.

    fullFileWaveform.setup( 0, 0, ofGetWidth(), ofGetHeight()/3);
    wave.setup(0, fullFileWaveform.getMaxY(), ofGetWidth(), ofGetHeight() - fullFileWaveform.getMaxY());
    // the fullFileWaveform object will have a static waveform for the whole audio file data. This is only created once as it will read the whole sound file data.
    // For such, we need to get the sound buffer from the sound file in order to get the whole file's data.
    // calling player.getBuffer(), which actually is a function, will return the players current buffer, the one that is being sent to the sound device, so it will not work for what we are trying to achieve.
    // the waveformDraw's makeMeshFromBuffer(ofBuffer&) function will create a waveform from the buffer passed

    fullFileWaveform.makeMeshFromBuffer( player.getSoundFile().getBuffer());


    // wave object will be part of the signal chain and will update on real time as the audio passes to the output

    // --------- Audio signal chain setup.-------
    // Each of our objects need to connect to each other in order to create a signal chain, which ends with the output; the object that we set as the sound stream output.

    player.connectTo(wave).connectTo(output);

    player.play();

    // set if you want to either have the player looping (playing over and over again) or not (stop once it reaches the its end).
    player.setLoop(true);

    // the endEvent gets triggered when it reaches the end of the file, regardless of it being
    // in looping or not
    playerEndListener = player.endEvent.newListener(this, &ofApp::playerEnded);

}

// -------------------------------------------------------------
void ofApp::exit(){
    stream.close();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    int r = 128 + 127 * cosf(ofGetElapsedTimef());
    int g = 0;
    int b = 128 + 127 * sinf(ofGetElapsedTimef());

    ofBackground(r,g,b);

    ofSetColor(ofColor::white);

    fullFileWaveform.draw();

    ofSetColor(ofColor::red);
    float playhead = ofMap(player.getPosition(), 0,1, fullFileWaveform.getMinX(),fullFileWaveform.getMaxX());
    ofDrawLine(playhead, 0, playhead, fullFileWaveform.getMaxY());

    if(fullFileWaveform.inside(ofGetMouseX(), ofGetMouseY())){
        ofSetColor(ofColor::cyan);
        ofDrawLine(ofGetMouseX(), 0, ofGetMouseX(), fullFileWaveform.getMaxY());
    }
    ofSetColor(ofColor::white);
    wave.draw();

    ofPushMatrix();
    ofSetColor(ofColor::yellow);
    ofScale(3, 3);
    player.drawDebug(20, 250.);
    ofPopMatrix();

}
//--------------------------------------------------------------
void ofApp::playerEnded(size_t & id){
	// This function gets called when the player ends. You can do whatever you need to here.
	// This event happens in the main thread, not in the audio thread.
	//cout << "the player's instance " << id << "finished playing" << endl;
	
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
   if(fullFileWaveform.inside(x, y)){
    player.setPositionMS(ofMap(x, fullFileWaveform.getMinX(), fullFileWaveform.getMaxX(), 0, player.getDurationMS()));
    }
    /**/
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}

void ofApp::deviceRefreshRateChanged(int refreshRate) {

}

void ofApp::deviceHighestRefreshRateChanged(int refreshRate) {

}

void ofApp::deviceRefreshRateChangedEvent(int &refreshRate) {

}

void ofApp::deviceHighestRefreshRateChangedEvent(int &refreshRate) {

}
