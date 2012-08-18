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
int ofBasicSoundPlayer::maxSoundsTotal=128;
int ofBasicSoundPlayer::maxSoundsPerPlayer=16;


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
	maxSounds = maxSoundsPerPlayer;
}

ofBasicSoundPlayer::~ofBasicSoundPlayer() {
	// TODO Auto-generated destructor stub
}



bool ofBasicSoundPlayer::loadSound(string fileName, bool _stream){
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
	if(!bIsLoaded) return false;
	if(!_stream){
		ofLogNotice() << "reading whole file ";
		soundFile.readTo(buffer);
	}else{
		ofLogNotice() << "resizing buffer ";
		buffer.resize(bufferSize*channels,0);
	}

	streaming = _stream;
	return true;
}

void ofBasicSoundPlayer::unloadSound(){
	soundFile.close();
	buffer.clear();
	isPlaying = false;
	bIsLoaded = false;
	positions.resize(1,0);
}

void ofBasicSoundPlayer::play(){
	if(!multiplay || !isPlaying){
		positions.back() = 0;
	}else if(maxSounds>(int)positions.size()){
		positions.push_back(0);
		relativeSpeed.push_back(speed*(double(soundFile.getSampleRate())/double(samplerate)));
		float left,right;
		ofStereoVolumes(volume,pan,left,right);
		volumesLeft.push_back(left);
		volumesRight.push_back(right);
	}
	isPlaying = true;
}

void ofBasicSoundPlayer::stop(){
	isPlaying = false;
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
		positions[i] += bufferSize*relativeSpeed[i];
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
	if(!loop && positions.size()==1 && positions[0]==buffer.bufferSize()) isPlaying = false;
}

void ofBasicSoundPlayer::audioOut(float * output, int bSize, int nChannels, int deviceID, long unsigned long tickCount){
	if(isPlaying){
		if(streaming){
			soundFile.readTo(buffer,bufferSize);
			buffer.stereoPan(volumesLeft.back(),volumesRight.back());
			buffer.copyTo(output,bufferSize,channels,0);
		}else{
			for(int i=0;i<(int)positions.size();i++){
				if(abs(relativeSpeed[i] - 1)<FLT_EPSILON){
					buffer.copyTo(resampledBuffer,bufferSize,channels,positions[i],loop);
				}else{
					buffer.resampleTo(resampledBuffer,positions[i],bufferSize,relativeSpeed[i],loop);
				}
				resampledBuffer.stereoPan(volumesLeft[i],volumesRight[i]);
				//resampledBufferE.notify(this,resampledBuffer);
				resampledBuffer.addTo(output,bufferSize,channels,0,loop);
			}
			updatePositions(bufferSize);
		}
	}else{
		for(int i=0;i<bufferSize*nChannels;i++){
			output[i] = 0;
		}
	}
}

ofSoundBuffer & ofBasicSoundPlayer::getCurrentBuffer(){
	if(streaming){
		return buffer;
	}else{
		return resampledBuffer;
	}
}

static void setMaxSoundsTotal(int max);
static void setMaxSoundsPerPlayer(int max);
void setMaxSounds(int max);
