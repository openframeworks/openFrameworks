/*
 * ofSoundMixer.cpp
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#include <float.h>
#include <assert.h>
#include "ofSoundMixer.h"
#include "ofSoundUtils.h"
#include "ofSoundStream.h"

ofSoundMixer* ofSoundMixerGetSystemMixer()
{
	static ofSoundMixer* systemMixer = new ofSoundMixer();
	return systemMixer;
}

ofSoundMixer::ofSoundMixer() {
	isSetup = false;
	tellBuffersChanged = false;
}


void ofSoundMixer::setup( ofSoundStream* outputStream ){
	outputStream->setOutput(this);
	isSetup = true;
}


void ofSoundMixer::addSoundOutput(ofBaseSoundOutput & out, float volume, float pan){

	Poco::ScopedLock<ofMutex> scopedMutex(mutex);

	if (!isSetup){
		if (isSystemMixer()){
			// this is the system mixer
			ofSoundStream* systemStream = ofSoundStreamGetSystemStream();
			// now setup the mixer using systemStream as output
			setup( systemStream );
		}
		else{
			// this is a user-defined mixer, and it has not been setup yet
			ofLogError("ofSoundMixer") << "attempting to use addSoundOutput on an ofSoundMixer that has not been setup. call setup first!";
			return;
		}
	}
		
	ofSoundMixerSource source;
	source.sourceOutput = &out;
	source.pan = pan;
	source.volume = volume;
	float left, right;
	ofStereoVolumes(volume,pan,left,right);
	source.volumeLeft = left;
	source.volumeRight = right;

	sources.push_back(source);
	
	if (!tellBuffersChanged && buffer.size()>0){
		//@todo source->buffersChanged()
		source.sourceOutput->audioOutBuffersChanged( buffer.getNumFrames(), buffer.getNumChannels(), buffer.getSampleRate());
	}
}

void ofSoundMixer::removeSoundOutput(ofBaseSoundOutput& out)
{
	Poco::ScopedLock<ofMutex> scopedMutex(mutex);
	vector<ofSoundMixerSource>::iterator it = std::find(sources.begin(), sources.end(), &out);
	if (it!=sources.end()){
		sources.erase(it);
		return;
	}
	ofLogWarning("ofSoundMixer") << "couldn't removeSoundOutput: requested soundOutput not found";
}

void ofSoundMixer::setVolume(ofBaseSoundOutput & out, float volume){
	
	for(int i=0;i<(int)sources.size();i++){
		if(sources[i].sourceOutput==&out){
			sources[i].volume = volume;
			setPan(out,sources[i].pan);
			return;
		}
	}
	ofLogWarning("ofSoundMixer") << "couldn't setVolume: requested soundOutput not found";
}

void ofSoundMixer::setPan(ofBaseSoundOutput & out, float p){
	
	for(int i=0;i<(int)sources.size();i++){
		if(sources[i].sourceOutput==&out){
			ofStereoVolumes(sources[i].volume,p,sources[i].volumeLeft,sources[i].volumeRight);
			return;
		}
	}
	ofLogWarning("ofSoundMixer") << "couldn't setPan: requested soundOutput not found";

}


void ofSoundMixer::audioOut(float * output, int nFrames, int nChannels, int deviceID, long unsigned long tickCount){
	//memset( output, 0, sizeof(float)*nFrames*nChannels );
	mutex.lock();
	for(int i=0;i<(int)sources.size();i++){
		if ( tellBuffersChanged ){
			sources[i].sourceOutput->audioOutBuffersChanged( buffer.getNumFrames(), buffer.getNumChannels(), buffer.getSampleRate() );
		}
		buffer.set(0);
		
		sources[i].sourceOutput->audioOut(buffer);
		
		if(buffer.getNumChannels()==2 && sources[i].volume-1<FLT_EPSILON && sources[i].pan<FLT_EPSILON){
			buffer.stereoPan(sources[i].volumeLeft,sources[i].volumeRight);
		}else if(sources[i].volume-1<FLT_EPSILON){
			buffer *= sources[i].volume;
		}
		
		for(int j=0;j<(int)buffer.size();j++){
			output[j] += buffer[j];
		}
	}
	tellBuffersChanged = false;
	mutex.unlock();
}


void ofSoundMixer::audioOutBuffersChanged(int nFrames, int nChannels, int sampleRate){
	buffer.setNumChannels(nChannels);
	buffer.setSampleRate(sampleRate);
	buffer.resize(nFrames*nChannels,0);
	tellBuffersChanged = true;
}
