/*
 * ofSoundMixer.cpp
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#include <float.h>
#include "ofSoundMixer.h"
#include "ofSoundUtils.h"

ofSoundMixer::ofSoundMixer() {

}


void ofSoundMixer::setup(int bufferSize, int nChannels, int sampleRate){
	buffer.setNumChannels(nChannels);
	buffer.setSampleRate(sampleRate);
	buffer.resize(bufferSize*nChannels,0);
}

void ofSoundMixer::addSoundOutput(ofBaseSoundOutput & out, float volume, float pan){
	sources.push_back(&out);
	if(buffer.getNumChannels()==2){
		float left, right;
		ofStereoVolumes(volume,pan,left,right);
		volumesLeft.push_back(left);
		volumesRight.push_back(right);
	}
	pans.push_back(pan);
	volumes.push_back(volume);
}

void ofSoundMixer::setVolume(ofBaseSoundOutput & out, float volume){
	for(int i=0;i<(int)sources.size();i++){
		if(sources[i]==&out){
			volumes[i] = volume;
			return;
		}
	}
}

void ofSoundMixer::setPan(ofBaseSoundOutput & out, float p){
	for(int i=0;i<(int)volumesLeft.size();i++){
		if(sources[i]==&out){
			ofStereoVolumes(volumes[i],p,volumesLeft[i],volumesRight[i]);
			return;
		}
	}
}

void ofSoundMixer::audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount){
	for(int i=0;i<(int)sources.size();i++){
		buffer.set(0);
		sources[i]->audioOut(&buffer[0],bufferSize,nChannels,deviceID,tickCount);
		if(buffer.getNumChannels()==2 && volumes[i]-1<FLT_EPSILON && pans[i]<FLT_EPSILON){
			buffer.stereoPan(volumesLeft[i],volumesRight[i]);
		}else if(volumes[i]-1<FLT_EPSILON){
			buffer *= volumes[i];
		}
		for(int j=0;j<(int)buffer.size();j++){
			output[j] += buffer[j];
		}
	}
}
