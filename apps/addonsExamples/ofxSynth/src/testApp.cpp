#include "testApp.h"

static const float PENTATONIC[7] = { 0, 2, 5, 7, 10, 12, 17 };

//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(255,255,255);

	// 2 output channels,
	// 0 input channels
	// 22050 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)

	sampleRate 			= 44100;
	volume				= 0.1f;
	lAudio = new float[1024];
	rAudio = new float[1024];
	
	
	ofSoundStreamSetup(2,0, sampleRate,1024, 4);

	ofSetFrameRate(60);
	for (int i=0; i<15; i++) {
		synth[i].setFrequencyMidiNote(60+ofRandom(-0.01, 0.01));	
		synth[i].env.setADSR(0.51*sampleRate, 0.2*sampleRate, 0.9, 2.3*sampleRate);
		synth[i].ampMode = OFXSYNTHADR;
		synth[i].setFilterMode(1);
		synth[i].setFilter(0.5, 0.1);
		mixer.addInputFrom(&synth[i]);
		mixer.setVolume(&synth[i], 1.0f/15.0f);
	}
	
	passthrough.addInputFrom( &mixer );
	writer.addInputFrom(&passthrough);
	ofSoundStreamAddSoundSource( &writer );
	delay.setSize(1.1);
	keyChange = 0;
}


//--------------------------------------------------------------
void testApp::update(){
	const ofSoundBuffer& output = passthrough.getBuffer();
	output.copyChannel( 0, lAudio );
	output.copyChannel( 1, rAudio );
	
}

//--------------------------------------------------------------
void testApp::draw(){


	// draw the left:
	ofSetHexColor(0x333333);
	ofRect(100,100,256,200);
	ofSetHexColor(0xFFFFFF);
	for (int i = 0; i < 256; i++){
		ofLine(100+i,200,100+i,200+lAudio[i]*100.0f);
	}

	// draw the right:
	ofSetHexColor(0x333333);
	ofRect(600,100,256,200);
	ofSetHexColor(0xFFFFFF);
	for (int i = 0; i < 256; i++){
		ofLine(600+i,200,600+i,200+rAudio[i]*100.0f);
	}

	ofSetHexColor(0x333333);
	char reportString[255];
	//sprintf(reportString, "volume: (%6.3f) modify with -/+ keys\nvolume: (%6.3f) modify with up/down keys", volume, testVolume.getVolume() );
	//if (!bNoise) sprintf(reportString, "%s (%fhz)", reportString, targetFrequency);

	ofDrawBitmapString(reportString,80,380);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	int synthToChange = (int)ofRandom(15);
	synth[synthToChange].setFrequencyMidiNote(key/2);
	synth[synthToChange].trigger();
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	cout << "pressed" << endl;
	writer.startWriting("out.wav");
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	cout << "released" << endl;
	writer.stopWriting();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

