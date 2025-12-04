#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	// IMPORTANT!!! if your sound doesn't work in the simulator - read this post - which requires you set the output stream to 24bit 
	//	http://www.cocos2d-iphone.org/forum/topic/4159

	ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right

	ofBackground(255, 255, 255);
    sampleRate = 44100;
	phase = 0;
	phaseAdder = 0.0f;
	phaseAdderTarget = 0.0;
	volume = 0.15f;
	pan = 0.5;
	bNoise = false;
    int bufferSize = 512;
    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);
	
	//we do this because we don't have a mouse move function to work with:
	targetFrequency = 444.0;
	phaseAdderTarget = (targetFrequency / (float)sampleRate) * glm::two_pi<float>();
	
	ofSoundStreamSettings settings;
	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);
	
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	float boxW = 200.0;
	float boxH = boxW * 0.75;
	
	float topY = 30;
	float leftX = 30;
	float rightX = leftX + boxW + 20;

	// draw the left:
	ofSetHexColor(0x333333);
	ofDrawRectangle(leftX, topY, boxW, boxH);
	ofSetHexColor(0xFFFFFF);
	for(int i = 0; i < lAudio.size(); i++){
		float x = ofMap(i, 0, lAudio.size(), 0, boxW, true);
		ofDrawLine(leftX + x,topY + boxH / 2,leftX + x, topY + boxH / 2 + lAudio[i] * boxH * 0.5);
	}

	// draw the right:
	ofSetHexColor(0x333333);
	ofDrawRectangle(rightX, topY, boxW, boxH);
	ofSetHexColor(0xFFFFFF);
	for(int i = 0; i < rAudio.size(); i++){
		float x = ofMap(i, 0,  rAudio.size(), 0, boxW, true);
		ofDrawLine(rightX + x, topY + boxH / 2, rightX + x, topY + boxH / 2 + rAudio[i] * boxH * 0.5);
	}

	ofSetHexColor(0x333333);
	stringstream reportString;
	reportString << "volume: (" << volume << ") \npan: (" << pan << ")\nsynthesis: " << ( bNoise ? "noise" : "sine wave" );
	reportString << " (" << targetFrequency << "hz)\n";
	ofDrawBitmapString(reportString.str(), leftX, topY + boxH + 20);

}

//--------------------------------------------------------------
void ofApp::exit(){
	
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){

	float leftScale = 1 - pan;
	float rightScale = pan;

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-glm::two_pi<float>() like this:
	while (phase > glm::two_pi<float>()){
		phase -= glm::two_pi<float>();
	}

	if (bNoise == true){
		// ---------------------- noise --------------
		for (size_t i = 0; i < buffer.getNumFrames(); i++){
			lAudio[i] = buffer[i*buffer.getNumChannels()    ] = ofRandom(0, 1) * volume * leftScale;
			rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = ofRandom(0, 1) * volume * rightScale;
		}
	} else {
		phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		for (size_t i = 0; i < buffer.getNumFrames(); i++){
			phase += phaseAdder;
			float sample = std::sin(phase);
			lAudio[i] = buffer[i*buffer.getNumChannels()    ] = sample * volume * leftScale;
			rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = sample * volume * rightScale;
		}
	}
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	bNoise = true;
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
	if(touch.id ==  0){
		int width = ofGetWidth();
		pan = (float)touch.x / (float)width;
		
		int height = ofGetHeight();
		targetFrequency = ((float)touch.y / (float)height) * 1000;
		phaseAdderTarget = (targetFrequency / (float)sampleRate) * glm::two_pi<float>();
	}
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
	bNoise = false;
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
	bNoise = false;
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
