#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	/* some standard setup stuff*/
	
	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0, 0, 0);
	
	/* This is stuff you always need.*/
	
	sampleRate 			= 44100; /* Sampling Rate */
	initialBufferSize	= 512;	/* Buffer Size. you have to fill this buffer with sound*/
	
	mixBuffer=(float *)malloc(initialBufferSize*sizeof(float));
	mixBuffer2=(float *)malloc(initialBufferSize*sizeof(float));
	mixBuffer3=(float *)malloc(initialBufferSize*sizeof(float));

	syn.setADRVol(0.1, 1, 1);
	syn.ampMode = OFXSYNTHADR;
	
	int bpm = 120;
	beatLength = (44100.0*60/bpm);
	framecounter = 0;
	
	sampler.loadFile(ofToDataPath("amen_brother.wav"));
	sampler.setNote(60);
	sampler.trigger();
	
	dl.setSize(1);
	dl.setFeedback(0.5);
	
	ofSoundStreamSetup(2,2,this, sampleRate, initialBufferSize, 2);/* Call this last ! */

}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	syn.audioRequested(mixBuffer, bufferSize);
	sampler.audioRequested(mixBuffer3, bufferSize);
	dl.audioRequested(mixBuffer, mixBuffer2, bufferSize);
	for (int i = 0; i < bufferSize; i++){
		framecounter++;
		int remainder = framecounter%beatLength;			
		if(remainder == 0){
			// do my triggering
			// syn.trigger();
		}
		mymix.stereo(mixBuffer[i]+mixBuffer2[i]+mixBuffer3[i], outputs, 0.33);
		output[i*nChannels    ] = ofClamp(outputs[1],-1,1); /* You may end up with lots of outputs. add them here */
		output[i*nChannels + 1] = ofClamp(outputs[1],-1,1);
	}
}

//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels){	
	
	
	/* You can just grab this input and stick it in a double, then use it above to create output*/
	
	for (int i = 0; i < bufferSize; i++){
		
		/* you can also grab the data out of the arrays*/
		
		
	}
	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	syn.setNote(key/2.0);
	syn.trigger();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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

