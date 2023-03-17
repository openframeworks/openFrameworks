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

using std::vector;

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
	[(ofxiOSSoundStreamDelegate *)[(__bridge id)soundInputStream delegate] setInput: settings.inCallback];
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::setOutput(ofBaseSoundOutput * soundOutput) {
	settings.setOutListener(soundOutput);
	[(ofxiOSSoundStreamDelegate *)[(__bridge id)soundOutputStream delegate] setOutput: settings.outCallback];
}

//------------------------------------------------------------------------------
bool ofxiOSSoundStream::setup(const ofSoundStreamSettings & settings) {
	close();
	
	this->settings = settings;
	
    if(settings.numInputChannels > 0) {
        __autoreleasing SoundInputStream *inputStream = [[SoundInputStream alloc] initWithNumOfChannels:settings.numInputChannels
                                                            withSampleRate:settings.sampleRate
                                                            withBufferSize:settings.bufferSize];
        ofxiOSSoundStreamDelegate * delegate = [[ofxiOSSoundStreamDelegate alloc] initWithSoundInputFn:settings.inCallback];
        inputStream.delegate = delegate;
        [inputStream start];
        soundInputStream = (__bridge_retained void *)inputStream;
    }
    
    if(settings.numOutputChannels > 0) {
        __autoreleasing SoundOutputStream *outputStream =
        [[SoundOutputStream alloc] initWithNumOfChannels:settings.numOutputChannels
                                                              withSampleRate:settings.sampleRate
                                                              withBufferSize:settings.bufferSize];
        ofxiOSSoundStreamDelegate * delegate = [[ofxiOSSoundStreamDelegate alloc] initWithSoundOutputFn:settings.outCallback];
        outputStream.delegate = delegate;
        [outputStream start];
        soundOutputStream = (__bridge_retained void *)outputStream;
    }
    
    bool bOk = (soundInputStream != NULL) || (soundOutputStream != NULL);
    return bOk;
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::start(){
    if(soundInputStream != NULL) {
        [(__bridge SoundInputStream *)soundInputStream start];
    }
    
    if(soundOutputStream != NULL) {
        [(__bridge SoundOutputStream *)soundOutputStream start];
    }
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::stop(){
    if(soundInputStream != NULL) {
        [(__bridge SoundInputStream *)soundInputStream stop];
    }
    
    if(soundOutputStream != NULL) {
        [(__bridge SoundOutputStream *)soundOutputStream stop];
    }
}

//------------------------------------------------------------------------------
void ofxiOSSoundStream::close(){
    if(soundInputStream != NULL) {
        __strong SoundInputStream *inputStream = (__bridge_transfer SoundInputStream *)soundInputStream;
        [inputStream setDelegate:nil];
        [inputStream stop];

        soundInputStream = NULL;
    }
    
    if(soundOutputStream != NULL) {
        __strong SoundOutputStream *outputStream = (__bridge_transfer SoundOutputStream *)soundOutputStream;
        [outputStream setDelegate:nil];
        [outputStream stop];
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
#if defined (TARGET_OF_TVOS) || defined (TARGET_OF_WATCHOS)
			if([audioSession setCategory:AVAudioSessionCategoryPlayAndRecord
                             withOptions:(AVAudioSessionCategoryOptionMixWithOthers)
                                   error:nil]) {
				success = true;
			}
#else
            if([audioSession setCategory:AVAudioSessionCategoryPlayAndRecord
                             withOptions:(AVAudioSessionCategoryOptionMixWithOthers |
                                          AVAudioSessionCategoryOptionAllowAirPlay |
                                          AVAudioSessionCategoryOptionAllowBluetooth |
                                          AVAudioSessionCategoryOptionAllowBluetoothA2DP)
                                   error:nil]) {
                success = true;
            }
#endif
		}
	} else {
    #endif
    
		// this is the default category + options setup
		// Note: using a sound input stream will set the category to PlayAndRecord
#if defined (TARGET_OF_TVOS) || defined (TARGET_OF_WATCHOS)
        if([audioSession setCategory:AVAudioSessionCategorySoloAmbient
                         error:nil]) {
            success = true;
        }
#else
        if([audioSession setCategory:AVAudioSessionCategorySoloAmbient
                         withOptions:(
                                      AVAudioSessionCategoryOptionAllowAirPlay |
                                      AVAudioSessionCategoryOptionAllowBluetooth |
                                      AVAudioSessionCategoryOptionAllowBluetoothA2DP)
                         error:nil]) {
            success = true;
        }
#endif

    #ifdef __IPHONE_6_0
	}
    #endif
    
	if(!success) {
		ofLogError("ofxiOSSoundStream") << "setMixWithOtherApps(): couldn't set app audio session category";
	}
	
	return success;
}
