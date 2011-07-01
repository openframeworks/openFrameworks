#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(255,255,255);

	sampleRate 			= 44100;
	
	lAudio = new float[1024];
	rAudio = new float[1024];
	
	ofSoundStreamSetup(2,0,sampleRate,1024, 4);
	
	// setup synthesizer
	synth.setPortamento(0.4*sampleRate);

	// setup delay and attach synth
	delay.setSize(0.2);
	delay.setFeedback(0.7);
	delay.addInputFrom( &synth );
	
	// attach delay to the filter
	filter.addInputFrom( &delay );
	filter.setLowPass();
	filter.setRes(0.4);
	
	// attach the filter passthough, will allow us to look at our buffers
	passthrough.addInputFrom( &filter );
	
	//passthrough.addInputFrom( &mixer );
	ofSoundStreamAddSoundSource( &passthrough );	
}

//--------------------------------------------------------------
void testApp::update(){
	// load the current 
	const ofSoundBuffer& output = passthrough.getBuffer();
	output.copyChannel( 0, lAudio );
	output.copyChannel( 1, rAudio );

}

//--------------------------------------------------------------
void testApp::draw(){
	for (int i=0; i<20; i++) {
		ofSetColor(0);
		ofCircle(ofGetWidth()/20.0*i+20, ofGetHeight()/2.0, lAudio[i*20]*40);
	}
	ofDrawBitmapString("keys to play",80,380);
	ofDrawBitmapString("mouse to set filter / delay",80,480);

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	synth.setFrequencyMidiNote(key/2);
	synth.trigger();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	filter.setCutoff((float)x/(float)ofGetWidth());
	delay.setSize((float)y/(float)ofGetHeight());
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
