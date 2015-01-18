//
//  SoundInputStream.h
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//

#include "ofxiOSSoundStream.h"
#include "ofxiOSSoundStreamDelegate.h"
#include "ofBaseApp.h"

#import "SoundInputStream.h"
#import "SoundOutputStream.h"
#import <AVFoundation/AVFoundation.h>

//------------------------------------------------------------------------------
ofxiOSSoundStream::ofxiOSSoundStream() {
    soundInputStream = NULL;
    soundOutputStream = NULL;

	soundInputPtr = NULL;
	soundOutputPtr = NULL;
    
    numOfInChannels = 0;
    numOfOutChannels = 0;
    sampleRate = 0;
    bufferSize = 0;
    numOfBuffers = 0;
}

//------------------------------------------------------------------------------
ofxiOSSoundStream::~ofxiOSSoundStream() {
    close();
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::listDevices() {
    //
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::setDeviceID(int _deviceID) {
    //
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getDeviceID() {
	return 0;
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::setInput(ofBaseSoundInput * soundInput) {
	soundInputPtr = soundInput;
	[(ofxiOSSoundStreamDelegate *)[(id)soundInputStream delegate] setInput:soundInputPtr];
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::setOutput(ofBaseSoundOutput * soundOutput) {
	soundOutputPtr = soundOutput;
	[(ofxiOSSoundStreamDelegate *)[(id)soundOutputStream delegate] setOutput:soundOutputPtr];
}

//------------------------------------------------------------------------------
ofBaseSoundInput * ofxiOSSoundStream::getInput(){
	return soundInputPtr;
}

//------------------------------------------------------------------------------
ofBaseSoundOutput * ofxiOSSoundStream::getOutput(){
	return soundOutputPtr;
}

//------------------------------------------------------------------------------
bool ofxiOSSoundStream::setup(int numOfOutChannels, int numOfInChannels, int sampleRate, int bufferSize, int numOfBuffers) {
    close();
    
    this->numOfOutChannels = numOfOutChannels;
    this->numOfInChannels = numOfInChannels;
    this->sampleRate = sampleRate;
    this->bufferSize = bufferSize;
    this->numOfBuffers = numOfBuffers;
    
    if(numOfInChannels > 0) {
        soundInputStream = [[SoundInputStream alloc] initWithNumOfChannels:numOfInChannels
                                                            withSampleRate:sampleRate
                                                            withBufferSize:bufferSize];
        ofxiOSSoundStreamDelegate * delegate = [[ofxiOSSoundStreamDelegate alloc] initWithSoundInputApp:soundInputPtr];
        ((SoundInputStream *)soundInputStream).delegate = delegate;
        [(SoundInputStream *)soundInputStream start];
    }
    
    if(numOfOutChannels > 0) {
        soundOutputStream = [[SoundOutputStream alloc] initWithNumOfChannels:numOfOutChannels
                                                              withSampleRate:sampleRate
                                                              withBufferSize:bufferSize];
        ofxiOSSoundStreamDelegate * delegate = [[ofxiOSSoundStreamDelegate alloc] initWithSoundOutputApp:soundOutputPtr];
        ((SoundInputStream *)soundOutputStream).delegate = delegate;
        [(SoundInputStream *)soundOutputStream start];
    }
    
    bool bOk = (soundInputStream != NULL) || (soundOutputStream != NULL);
    return bOk;
}

//------------------------------------------------------------------------------
bool ofxiOSSoundStream::setup(ofBaseApp * app, int numOfOutChannels, int numOfInChannels, int sampleRate, int bufferSize, int numOfBuffers){
    setInput(app);
	setOutput(app);
	bool bOk = setup(numOfOutChannels, numOfInChannels, sampleRate, bufferSize, numOfBuffers);
    return bOk;
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::start(){
    if(soundInputStream != NULL) {
        [(SoundInputStream *)soundInputStream start];
    }
    
    if(soundOutputStream != NULL) {
        [(SoundOutputStream *)soundOutputStream start];
    }
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::stop(){
    if(soundInputStream != NULL) {
        [(SoundInputStream *)soundInputStream stop];
    }
    
    if(soundOutputStream != NULL) {
        [(SoundOutputStream *)soundOutputStream stop];
    }
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::close(){
    if(soundInputStream != NULL) {
        [((SoundInputStream *)soundInputStream).delegate release];
        [(SoundInputStream *)soundInputStream setDelegate:nil];
        [(SoundInputStream *)soundInputStream stop];
        [(SoundInputStream *)soundInputStream release];
        soundInputStream = NULL;
    }
    
    if(soundOutputStream != NULL) {
        [((SoundOutputStream *)soundInputStream).delegate release];
        [(SoundOutputStream *)soundInputStream setDelegate:nil];
        [(SoundOutputStream *)soundOutputStream stop];
        [(SoundOutputStream *)soundOutputStream release];
        soundOutputStream = NULL;
    }
        
    numOfInChannels = 0;
    numOfOutChannels = 0;
    sampleRate = 0;
    bufferSize = 0;
    numOfBuffers = 0;
}

//------------------------------------------------------------------------------
long unsigned long ofxiOSSoundStream::getTickCount(){
	return 0;
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getNumOutputChannels(){
	return numOfOutChannels;
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getNumInputChannels(){
	return numOfInChannels;
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getSampleRate(){
    return sampleRate;
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getBufferSize(){
    return bufferSize;
}

//------------------------------------------------------------------------------
bool ofxiOSSoundStream::setMixWithOtherApps(bool bMix){
	AVAudioSession * audioSession = [AVAudioSession sharedInstance];
	bool success = false;
	
    #ifdef __IPHONE_6_0
	if(bMix) {
		if([audioSession respondsToSelector:@selector(setCategory:withOptions:error:)]) {
			if([audioSession setCategory:AVAudioSessionCategoryPlayAndRecord
							 withOptions:AVAudioSessionCategoryOptionMixWithOthers
								   error:nil]) {
				success = true;
			}
		}
	} else {
    #endif
    
		// this is the default category + options setup
		// Note: using a sound input stream will set the category to PlayAndRecord
		if([audioSession setCategory:AVAudioSessionCategorySoloAmbient error:nil]) {
			success = true;
		}
        
    #ifdef __IPHONE_6_0
	}
    #endif
    
	if(!success) {
		ofLogError("ofxiOSSoundStream") << "setMixWithOtherApps(): couldn't set app audio session category";
	}
	
	return success;
}
