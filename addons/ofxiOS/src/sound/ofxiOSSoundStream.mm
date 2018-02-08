//
//  SoundInputStream.h
//  Created by Lukasz Karluk on 13/06/13.
//  http://julapy.com/blog
//

#include "ofxiOSSoundStream.h"
#include "ofxiOSSoundStreamDelegate.h"
#include "ofSoundStream.h"
#include "ofBaseApp.h"
#include "ofLog.h"
#include <functional>

#import "SoundInputStream.h"
#import "SoundOutputStream.h"
#import <AVFoundation/AVFoundation.h>

using namespace std;

//------------------------------------------------------------------------------
ofxiOSSoundStream::ofxiOSSoundStream() {
    soundInputStream = NULL;
    soundOutputStream = NULL;
}

//------------------------------------------------------------------------------
ofxiOSSoundStream::~ofxiOSSoundStream() {
    close();
}

//------------------------------------------------------------------------------
vector<ofSoundDevice> ofxiOSSoundStream::getDeviceList(ofSoundDevice::Api api)  const{
	ofLogWarning("ofxiOSSoundStream") << "getDeviceList() isn't implemented on iOS";
	return vector<ofSoundDevice>();
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::setDeviceID(int _deviceID) {
    //
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getDeviceID()  const{
	return 0;
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::setInput(ofBaseSoundInput * soundInput) {
	settings.setInListener(soundInput);
	[(ofxiOSSoundStreamDelegate *)[(id)soundInputStream delegate] setInput: settings.inCallback];
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::setOutput(ofBaseSoundOutput * soundOutput) {
	settings.setOutListener(soundOutput);
	[(ofxiOSSoundStreamDelegate *)[(id)soundOutputStream delegate] setOutput: settings.outCallback];
}

//------------------------------------------------------------------------------
bool ofxiOSSoundStream::setup(const ofSoundStreamSettings & settings) {
	close();
	
	this->settings = settings;
	
    if(settings.numInputChannels > 0) {
        soundInputStream = [[SoundInputStream alloc] initWithNumOfChannels:settings.numInputChannels
                                                            withSampleRate:settings.sampleRate
                                                            withBufferSize:settings.bufferSize];
        ofxiOSSoundStreamDelegate * delegate = [[ofxiOSSoundStreamDelegate alloc] initWithSoundInputFn:settings.inCallback];
        ((SoundInputStream *)soundInputStream).delegate = delegate;
        [(SoundInputStream *)soundInputStream start];
    }
    
    if(settings.numOutputChannels > 0) {
        soundOutputStream = [[SoundOutputStream alloc] initWithNumOfChannels:settings.numOutputChannels
                                                              withSampleRate:settings.sampleRate
                                                              withBufferSize:settings.bufferSize];
        ofxiOSSoundStreamDelegate * delegate = [[ofxiOSSoundStreamDelegate alloc] initWithSoundOutputFn:settings.outCallback];
        ((SoundInputStream *)soundOutputStream).delegate = delegate;
        [(SoundInputStream *)soundOutputStream start];
    }
    
    bool bOk = (soundInputStream != NULL) || (soundOutputStream != NULL);
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
	
	settings = ofSoundStreamSettings();
}

//------------------------------------------------------------------------------
long unsigned long ofxiOSSoundStream::getTickCount() const{
	return 0;
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getNumOutputChannels() const{
	return settings.numOutputChannels;
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getNumInputChannels() const{
	return settings.numInputChannels;
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getSampleRate() const{
    return settings.sampleRate;
}

//------------------------------------------------------------------------------
int ofxiOSSoundStream::getBufferSize() const{
    return settings.bufferSize;
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
