#include "testApp.h"

static const float PENTATONIC[7] = { 0, 2, 5, 7, 10, 12, 17 };

//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(255,255,255);
	ofSetLogLevel(OF_LOG_NOTICE);

	sampleRate 			= 44100;
	volume				= 0.1f;
	lAudio = new float[1024];
	rAudio = new float[1024];
	
	
	soundStream.setup(this,2,0, sampleRate,1024, 4);

	int bpm = 120;
	beatLength = ((sampleRate*60)/bpm);
	
	// setup synthesizer
	synth.setFrequencyMidiNote(20+ofRandom(-0.01, 0.01));
	synth.env.setADSR(0.01*sampleRate, 0.1*sampleRate, 0.5, 0.03*sampleRate);
	synth.setFilterLowPass();
	synth.setWaveSquare();
	synth.setFilter(0.7, 0.5);
	
	mixer.addInputFrom(&synth);
	mixer.setVolume(&synth, 0.2);
	
	sampler.loadFile("amen_brother.wav");
	sampler.setFrequencySyncToLength(beatLength*4.0*4.0); // the length of a beat is in quarter notes * 4 measures
	
	filter.setup();
	filter.addInputFrom( &sampler );
	
	// allows us to send the filter to multiple places
	multiplex.addInputFrom( &filter );
	delay.addInputFrom( &multiplex);
	
	mixer.addInputFrom( &delay );
	mixer.setVolume(&delay, 1.0);

	mixer.addInputFrom( &multiplex );
	mixer.setVolume(&multiplex, 1.0);
	
	passthrough.addInputFrom( &mixer );

	mixer.setAudioInput(this);
//	writer.addInputFrom(&passthrough);
	soundStream.setOutput( &passthrough );
	
	delay.setSize(0.1);
	delay.setFeedback(0.7);
	
	beatPos = 0;
}

void testApp::audioRequested(float * output, int bufferSize, int nChannels){
	for (int i = 0; i < bufferSize; i++){
		frameCounter++;
		int remainder = frameCounter%(beatLength); // trigger drum on 8ths
		if(remainder == 0){
			if(effectIndex<=0||effectIndex>=3){
				cutStart = beatPos/2%8/8.0;
				cutEnd = cutStart + 1/8.0;
			}
			sampler.setLoopPoints(cutStart, cutEnd);
			sampler.trigger();
		}
		remainder = frameCounter%(beatLength/4); // trigger synth on 16ths
		if (remainder+ofRandom(-2, 1) <= 0) {
			int noteOffset = 40;
			synth.setPortamento(0.0);
			synth.setFrequencyMidiNote(noteOffset+PENTATONIC[beatPos%3]);
			if (remainder-ofRandom(-1, 1)<0) {
				noteOffset -= 12;
			}
			if (remainder-ofRandom(0, 10)<0) {
				synth.setPortamento(0.1*sampleRate);
				synth.env.setADSR(0.01*sampleRate, 0.1*sampleRate, 0.5, 0.93*sampleRate);
				synth.setFrequencyMidiNote(noteOffset+PENTATONIC[(int)(beatPos+ofRandom(5))%5]);
			}
			synth.trigger();
		}
		if (frameCounter%(beatLength/2) == 0) {
			beatPos++;
		}
	}
}
//--------------------------------------------------------------
void testApp::update(){
	const ofxSoundBuffer& output = passthrough.getBuffer();
	output.copyChannel( 0, lAudio );
	output.copyChannel( 1, rAudio );
	
}

//--------------------------------------------------------------
void testApp::draw(){

	// background
	ofSetHexColor(0xCCCCCC);
	ofRect(0, effectIndex*ofGetHeight()/8.0, ofGetWidth(), ofGetHeight()/8);
	ofSetHexColor(0xFF3333);
	ofRect(0, effectIndex*ofGetHeight()/8.0, mouseX, ofGetHeight()/8);
	
	
	// draw the left:
	ofSetHexColor(0xFFFFFF);
	for (int i = 0; i < 256; i++){
		ofLine(cutStart*ofGetWidth()+i,(effectIndex+1)*ofGetHeight()/8.0,cutStart*ofGetWidth()+i,effectIndex*(ofGetHeight()/8.0)+lAudio[i]*200.0f);
	}

	ofSetHexColor(0x333333);
	ofDrawBitmapString("NORMAL",80,ofGetHeight()/8.0-ofGetHeight()/16.0);
	ofDrawBitmapString("CHANGE LOOP",80,(ofGetHeight()/8.0)*2-ofGetHeight()/16.0);
	ofDrawBitmapString("SHORT LOOP",80,(ofGetHeight()/8.0)*3-ofGetHeight()/16.0);
	ofDrawBitmapString("FILTER LOW",80,(ofGetHeight()/8.0)*4-ofGetHeight()/16.0);
	ofDrawBitmapString("FILTER HIGH",80,(ofGetHeight()/8.0)*5-ofGetHeight()/16.0);
	ofDrawBitmapString("DELAY",80,(ofGetHeight()/8.0)*6-ofGetHeight()/16.0);
	ofDrawBitmapString("SYNTH CUTOFF",80,(ofGetHeight()/8.0)*7-ofGetHeight()/16.0);
	ofDrawBitmapString("SYNTH RES",80,(ofGetHeight()/8.0)*8-ofGetHeight()/16.0);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	synth.setFrequencyMidiNote(key/2);
	synth.trigger();
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	x = fmax(x, 0);
	effectIndex = floor(((float)y/(float)ofGetHeight())*8.0);
	filter.setCutoff(1.0);
	filter.setRes(0.0);
	filter.setLowPass();
	
	mixer.setVolume(&delay, 0.0);
	
	switch (effectIndex) {
		case 1:
			cutStart = floor(((float)x/(float)ofGetWidth())*8.0)/8.0;
			cutEnd = cutStart+1/8.0f;
			break;
		case 2:
			cutStart = floor(((float)x/(float)ofGetWidth())*8.0)/8.0;
			cutEnd = (float)x/(float)ofGetWidth();
			break;
		case 3:
			filter.setCutoff((float)x/(float)ofGetWidth());
			filter.setRes(0.3);
			filter.setLowPass();
			break;
		case 4:
			filter.setCutoff((float)x/(float)ofGetWidth());
			filter.setRes(0.3);
			filter.setHighPass();
			break;
		case 5:
			delay.setSize((float)x/(float)ofGetWidth());
			mixer.setVolume(&delay, 1.0);
			break;
		case 6:
			synth.setFilter(powf((float)x/(float)ofGetWidth(), 3), synth.getRes());
			break;
		case 7:
			synth.setFilter(synth.getCutoff(), (float)x/(float)ofGetWidth());
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	/*cout << "pressed" << endl;
	writer.startWriting("out.wav");*/
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	/*cout << "released" << endl;
	writer.stopWriting();*/
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

