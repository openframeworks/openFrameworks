/*
 * ofBasicSoundPlayer.cpp
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#include "ofBasicSoundPlayer.h"
#include "ofSoundUtils.h"
#include <float.h>

ofSoundStream ofBasicSoundPlayer::stream;
ofSoundMixer ofBasicSoundPlayer::mixer;
bool ofBasicSoundPlayer::initialized = false;
int ofBasicSoundPlayer::samplerate = 44100;
int ofBasicSoundPlayer::bufferSize = 256;
int ofBasicSoundPlayer::channels = 2;

ofBasicSoundPlayer::ofBasicSoundPlayer() {
	volume = 1;
	streaming = false;
	isPlaying = false;
	multiplay = false;
	bIsLoaded = false;
	positions.resize(1,0);
	loop = false;
	speed = 1;
	relativeSpeed.resize(1,1);
	volumesLeft.resize(1,1);
	volumesRight.resize(1,1);
	pan = 0;
}

ofBasicSoundPlayer::~ofBasicSoundPlayer() {
	// TODO Auto-generated destructor stub
}



void ofBasicSoundPlayer::loadSound(string fileName, bool _stream){
	ofLogNotice() << "loading " << fileName;
	if(!initialized){
		mixer.setup(bufferSize,channels,samplerate);
		stream.setup(channels,0,samplerate,bufferSize,4);
		stream.setOutput(&mixer);
		initialized = true;
	}
	ofLogNotice() << "adding output to mixer ";
	mixer.addSoundOutput(*this,volume);
	ofLogNotice() << "opening file ";
	bIsLoaded = soundFile.open(fileName);
	if(!_stream){
		ofLogNotice() << "reading whole file ";
		soundFile.readTo(buffer);
	}else{
		ofLogNotice() << "resizing buffer ";
		buffer.resize(bufferSize*channels,0);
	}

	ofLogNotice() << "finish loading " << fileName;
	streaming = _stream;
}

void ofBasicSoundPlayer::unloadSound(){
	soundFile.close();
	buffer.clear();
	isPlaying = false;
	bIsLoaded = false;
	positions.resize(1,0);
}

void ofBasicSoundPlayer::play(){
	int pos=0;
	float relSpeed = speed*(double(soundFile.getSampleRate())/double(playerSampleRate));
	float left,right;
	ofStereoVolumes(volume,pan,left,right);

	if (multiplay) {
		if(maxSounds>(int)positions.size()){
			positions.push_back(pos);
			relativeSpeed.push_back(relSpeed);
			volumesLeft.push_back(left);
			volumesRight.push_back(right);
		}
	}else{
		if (streaming) {
			soundFile.seekTo(pos);
		}
		positions.back() = pos;
		relativeSpeed.back() = relSpeed;
		volumesLeft.back() = left;
		volumesRight.back() = right;
	}
	isPlaying = true;
}

void ofBasicSoundPlayer::stop(){
	isPlaying = false;

	if (streaming){
		soundFile.seekTo(0);
	}

}


void ofBasicSoundPlayer::setVolume(float vol){
	volume = vol;
	ofStereoVolumes(volume,pan,volumesLeft.back(),volumesRight.back());
}

void ofBasicSoundPlayer::setPan(float _pan){
	pan = _pan;
	ofStereoVolumes(volume,pan,volumesLeft.back(),volumesRight.back());
}

void ofBasicSoundPlayer::setSpeed(float spd){
	speed = spd;
	relativeSpeed.back() = speed*(double(soundFile.getSampleRate())/double(samplerate));
}

void ofBasicSoundPlayer::setPaused(bool bP){
	isPlaying = false;
}

void ofBasicSoundPlayer::setLoop(bool bLp){
	loop = bLp;
}

void ofBasicSoundPlayer::setMultiPlay(bool bMp){
	multiplay = bMp;
	if(!multiplay){
		positions.resize(1);
		relativeSpeed.resize(1);
		volumesLeft.resize(1);
		volumesRight.resize(1);
	}
}

void ofBasicSoundPlayer::setPosition(float pct){
	positions.back() = pct*buffer.bufferSize();
}

void ofBasicSoundPlayer::setPositionMS(int ms){
	setPosition(float(ms)/float(buffer.getDuration()));
}


float ofBasicSoundPlayer::getPosition(){
	return float(positions.back())/float(buffer.bufferSize());
}

int ofBasicSoundPlayer::getPositionMS(){
	return buffer.getDuration() * getPosition();
}

bool ofBasicSoundPlayer::getIsPlaying(){
	return isPlaying;
}

float ofBasicSoundPlayer::getSpeed(){
	return speed;
}

float ofBasicSoundPlayer::getPan(){
	return pan;
}

bool ofBasicSoundPlayer::isLoaded(){
	return bIsLoaded;
}

float ofBasicSoundPlayer::getVolume(){
	return volume;
}

void ofBasicSoundPlayer::updatePositions(int bufferSize){
	for(int i=0;i<(int)positions.size();i++){
		// update positions
		positions[i] += nFrames*relativeSpeed[i];
		if(loop){
			positions[i] %= buffer.bufferSize();
		}else{
			positions[i] = ofClamp(positions[i],0,buffer.bufferSize());
			if(positions[i]==buffer.bufferSize()){
				if(positions.size()>1){
					positions.erase(positions.begin()+i);
					relativeSpeed.erase(relativeSpeed.begin()+i);
					volumesLeft.erase(volumesLeft.begin()+i);
					volumesRight.erase(volumesRight.begin()+i);
					i--;
				}
			}
		}
	}
	// finished?
	if(!loop && positions.size()==1 && positions[0]==buffer.getNumFrames()){
		isPlaying = false;
	}
}

void ofBasicSoundPlayer::audioOut(float * output, int bSize, int nChannels, int deviceID, long unsigned long tickCount){
	if(isPlaying){
		if(streaming){
			int samplesRead = soundFile.readTo(buffer,nFrames);
			if ( samplesRead==0 ){
				isPlaying=false;
				soundFile.seekTo(0);
			}
			else{
				buffer.stereoPan(volumesLeft.back(),volumesRight.back());
				newBufferE.notify(this,buffer);
				buffer.copyTo(outputBuffer);
			}
		}else{
			//assert( resampledBuffer.size() == bufferSize );
			for(int i=0;i<(int)positions.size();i++){
				if(abs(relativeSpeed[i] - 1)<FLT_EPSILON){
					buffer.copyTo(resampledBuffer,bufferSize,channels,positions[i],loop);
				}else{
					buffer.resampleTo(resampledBuffer,positions[i],bufferSize,relativeSpeed[i],loop);
				}
				resampledBuffer.stereoPan(volumesLeft[i],volumesRight[i]);
				newBufferE.notify(this,resampledBuffer);
				resampledBuffer.addTo(outputBuffer,0,loop);
			}
			updatePositions(bufferSize);
		}
	}else{
		for(int i=0;i<bufferSize*nChannels;i++){
			output[i] = 0;
		}
	}
}
