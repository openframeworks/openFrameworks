#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::hSlider_1onMousePressed(float & e){
	pd.sendFloat(patch.dollarZeroStr() + "-reverb", e); 
	label_2.symbol = ofToString(e); 
}

//--------------------------------------------------------------
void ofApp::hSlider_2onMousePressed(float & e){ 
	pd.sendFloat(patch.dollarZeroStr() + "-lowpass", e);  
	label_4.symbol = ofToString(e);
}

//--------------------------------------------------------------
void ofApp::hSlider_3onMousePressed(float & e){ 
	pd.sendFloat(patch.dollarZeroStr() + "-volume", e);
	label_6.symbol = ofToString(e); 
}
 
//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(100, 100, 100);
	ofAddListener(hSlider_1.onMousePressed, this, & ofApp::hSlider_1onMousePressed);
	ofAddListener(hSlider_2.onMousePressed, this, & ofApp::hSlider_2onMousePressed); 
	ofAddListener(hSlider_3.onMousePressed, this, & ofApp::hSlider_3onMousePressed);
	label_1.setup(155, 200, 100, 20, "Reverb");
	label_2.setup(155, 225, 100, 20, "0");
	label_3.setup(155, 250, 100, 20, "Lowpass");
	label_4.setup(155, 275, 100, 20, "100");
	label_5.setup(155, 300, 100, 20, "Volume");
	label_6.setup(155, 325, 100, 20, "50");
	hSlider_1.setup(50, 200, 100, 20, 0, 20);
	hSlider_1.slider = 0;
	hSlider_2.setup(50, 250, 100, 20, 0, 100);
	hSlider_2.slider = 0.8;
	hSlider_3.setup(50, 300, 100, 20, 0, 100);
	hSlider_3.slider = 0.5;
	
	//ofSetLogLevel("Pd", OF_LOG_VERBOSE); // see verbose info inside

	// double check where we are ...
	cout << ofFilePath::getCurrentWorkingDirectory() << endl;

	// the number of libpd ticks per buffer,
	// used to compute the audio buffer len: tpb * blocksize (always 64)
	#ifdef TARGET_LINUX_ARM
		// longer latency for Raspberry PI
		int ticksPerBuffer = 32; // 32 * 64 = buffer len of 2048
		int numInputs = 2; // no built in mic
	#else
		int ticksPerBuffer = 2; // 8 * 64 = buffer len of 512
		int numInputs = 2;
	#endif

	// setup OF sound stream
	ofSoundStreamSettings settings;
	settings.numInputChannels = 2;
	settings.numOutputChannels = 2;
	settings.sampleRate = ofSoundStream().getSampleRate();
	settings.bufferSize = ofxPd::blockSize() * ticksPerBuffer;
	settings.setInListener(this);
	settings.setOutListener(this);
	ofSoundStreamSetup(settings);

	if(!pd.init(2, numInputs, ofSoundStream().getSampleRate(), ticksPerBuffer, false)) {
		OF_EXIT_APP(1);
	}

	// subscribe to receive source names
	pd.subscribe("toOF");
	pd.addReceiver(*this); // automatically receives from all subscribed sources 
	pd.start();
	patch = pd.openPatch("pd/test.pd");
	 
	pd.sendFloat(patch.dollarZeroStr() + "-reverb", 0);
	pd.sendFloat(patch.dollarZeroStr() + "-lowpass", 100);
	pd.sendFloat(patch.dollarZeroStr() + "-volume", 50);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(240, 220, 100);
        ofDrawRectangle(20, 20, 270, 450);
	label_1.draw();
	label_2.draw();
	label_3.draw();
	label_4.draw();
	label_5.draw();
	label_6.draw();
	hSlider_1.draw();
	hSlider_2.draw();
	hSlider_3.draw();
	ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofSoundStreamStop();
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}
