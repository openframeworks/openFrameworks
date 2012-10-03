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
	int bufferSize = outputStream->getBufferSize();
	int nChannels = outputStream->getNumOutputChannels();
	buffer.setNumChannels(nChannels);
	buffer.setSampleRate(outputStream->getSampleRate());
	buffer.resize(bufferSize*nChannels,0);
	isSetup = true;
}


void ofSoundMixer::addSoundOutput(ofBaseSoundOutput & out, float volume, float pan){

	if (!isSetup){
		if (isSystemMixer()){
			// this is the system mixer
			ofSoundStream* systemStream = ofSoundStreamGetSystemStream();
			// setup the system stream if necessary
			if (!systemStream->isSetup()){
				bool success = systemStream->setupOutput();
				if (!success){
					ofLogError("ofSoundMixer") << "setupOutput() failed on the system sound stream, bailing out";
					return;
				}
			}
			// now setup the mixer using systemStream as output
			setup( systemStream );
			systemStream->setOutput(this);
		}
		else{
			// this is a user-defined mixer, and it has not been setup yet
			ofLogError("ofSoundMixer") << "attempting to use addSoundOutput on an ofSoundMixer that has not been setup. call setup first!";
			return;
		}
	}
		
	ofSoundMixerSource source;
	source.sourceOutput = &out;
	if(buffer.getNumChannels()==2){
		float left, right;
		ofStereoVolumes(volume,pan,left,right);
		source.volumeLeft = left;
		source.volumeRight = right;
	}
	source.pan = pan;
	source.volume = volume;
	sources.push_back(source);
	
	if (!tellBuffersChanged && buffer.size()>0)
		//@todo source->buffersChanged()
}

void ofSoundMixer::removeSoundOutput(ofBaseSoundOutput& out)
{
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


void ofSoundMixer::audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount){
	//memset( output, 0, sizeof(float)*bufferSize*nChannels );
	assert( bufferSize == buffer.bufferSize() );
	assert( nChannels == buffer.getNumChannels() );
	for(int i=0;i<(int)sources.size();i++){
		if ( tellBuffersChanged )
			sources[i].sourceOutput->buffersChanged( buffer.getBufferSize(), buffer.getNumChannels(), buffer.getSampleRate() );
		buffer.set(0);
		
		sources[i].sourceOutput->audioOut(&buffer[0],bufferSize,nChannels,deviceID,tickCount);
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
}


void ofSoundMixer::buffersChanged(int bufferSize, int nChannels, int sampleRate){
	buffer.setNumChannels(nChannels);
	buffer.setSampleRate(sampleRate);
	buffer.resize(bufferSize*nChannels,0);
	tellBuffersChanged = true;
}
