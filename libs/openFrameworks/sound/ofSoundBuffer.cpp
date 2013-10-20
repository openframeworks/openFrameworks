/*
 * ofSoundBuffer.cpp
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#include "ofSoundBuffer.h"
#include "ofSoundUtils.h"
#include "ofLog.h"
#include <limits>

#if !defined(TARGET_ANDROID) && !defined(TARGET_IPHONE)
ofSoundBuffer::InterpolationAlgorithm ofSoundBuffer::defaultAlgorithm = ofSoundBuffer::Hermite;
#else
ofSoundBuffer::InterpolationAlgorithm ofSoundBuffer::defaultAlgorithm = ofSoundBuffer::Linear;
#endif

ofSoundBuffer::ofSoundBuffer() {
	channels=1;
	samplerate=44100;
	tickCount=0;
}

ofSoundBuffer::ofSoundBuffer(short * shortBuffer, unsigned int numFrames, int _channels, int _samplerate){
	tickCount=0;
	copyFrom(shortBuffer,numFrames,_channels,_samplerate);
	checkSizeAndChannelsConsistency("constructor");
}

void ofSoundBuffer::copyFrom(short * shortBuffer, unsigned int numFrames, int _channels, int _samplerate){
	channels = _channels;
	samplerate = _samplerate;
	buffer.resize(numFrames*channels);
	for(unsigned int i=0;i<size();i++){
		buffer[i] = shortBuffer[i]/float(numeric_limits<short>::max());
	}
	checkSizeAndChannelsConsistency("copyFrom");
}

vector<float> & ofSoundBuffer::getBuffer(){
	return buffer;
}

unsigned long ofSoundBuffer::getDurationMS() const{
	return getNumFrames()/samplerate;
}

void ofSoundBuffer::setNumChannels(int _channels){
	channels = _channels;
	checkSizeAndChannelsConsistency("setNumChannels");
}

void ofSoundBuffer::setSampleRate(int rate){
	samplerate = rate;
}

void ofSoundBuffer::resize(unsigned int samples, float val){
	buffer.resize(samples,val);
	checkSizeAndChannelsConsistency("resize(samples,val)");
}

void ofSoundBuffer::clear(){
	buffer.clear();
}

void ofSoundBuffer::set(float value){
	buffer.assign(buffer.size(),value);
	checkSizeAndChannelsConsistency("set");
}

bool ofSoundBuffer::checkSizeAndChannelsConsistency( string function ) {
	if ( function.size()!= 0 ){
		function += ": ";
	}
	if ( (size()%channels) != 0 ){
		ofLogWarning("ofSoundBuffer") << function << "channel count " << channels << " is not consistent with sample count " << size() << " (non-zero remainder)";
		return false;
	}
	return true;
}


void ofSoundBuffer::set(float * _buffer, unsigned int nFrames, unsigned int nChannels){
	buffer.assign(_buffer,_buffer+nFrames*nChannels);
	setNumChannels(nChannels);
	checkSizeAndChannelsConsistency("set(buffer,size,nChannels)");
}

float & ofSoundBuffer::operator[](unsigned int pos){
	return buffer[pos];
}

const float & ofSoundBuffer::operator[](unsigned int pos) const{
	return buffer[pos];
}

void ofSoundBuffer::swap(ofSoundBuffer & buffer){
	std::swap(channels,buffer.channels);
	std::swap(samplerate,buffer.samplerate);
	std::swap(this->buffer,buffer.buffer);
}

ofSoundBuffer ofSoundBuffer::operator*(float value){
	ofSoundBuffer ret = *this;
	ret *= value;
	return ret;
}

ofSoundBuffer & ofSoundBuffer::operator*=(float value){
	for(unsigned int i=0;i<buffer.size();i++){
		buffer[i] *= value;
	}
	return *this;
}

void ofSoundBuffer::stereoPan(float left, float right){
	if(channels!=2){
		ofLogWarning("ofSoundBuffer") << "stereoPan called on a buffer with " << channels << " channels, only works with 2 channels";
		return;
	}
	float * bufferPtr = &buffer[0];
	for(unsigned int i=0;i<getNumFrames();i++){
		*bufferPtr++ *= left;
		*bufferPtr++ *= right;
	}
}

void ofSoundBuffer::copyTo(ofSoundBuffer & soundBuffer, unsigned int nFrames, unsigned int outChannels,unsigned int fromFrame,bool loop) const{
	soundBuffer.resize(nFrames*outChannels);
	soundBuffer.setNumChannels(outChannels);
	soundBuffer.setSampleRate(samplerate);
	copyTo(&soundBuffer[0],nFrames,outChannels,fromFrame,loop);
}

void ofSoundBuffer::copyTo(ofSoundBuffer & outBuffer, unsigned int fromFrame, bool loop) const{
	copyTo(&outBuffer[0],outBuffer.getNumFrames(),outBuffer.getNumChannels(),fromFrame,loop);
}

void ofSoundBuffer::addTo(ofSoundBuffer & soundBuffer, unsigned int nFrames, unsigned int outChannels,unsigned int fromFrame,bool loop) const{
	soundBuffer.resize(nFrames*outChannels);
	soundBuffer.setNumChannels(outChannels);
	soundBuffer.setSampleRate(samplerate);
	addTo(&soundBuffer.getBuffer()[0],nFrames,outChannels,fromFrame,loop);
}

void ofSoundBuffer::addTo(ofSoundBuffer & outBuffer, unsigned int fromFrame, bool loop) const{
	addTo(&outBuffer[0],outBuffer.getNumFrames(),outBuffer.getNumChannels(),fromFrame,loop);
}

void ofSoundBuffer::copyTo(float * out, unsigned int nFrames, unsigned int outChannels,unsigned int fromFrame, bool loop) const{
	// figure out how many frames we can copy before we need to stop or loop
	int nFramesToCopy = nFrames;
	if (int(this->getNumFrames()-fromFrame)<nFrames){
		nFramesToCopy = this->getNumFrames()-fromFrame;
	}
		
	const float * buffPtr = &buffer[fromFrame*channels];
	// if channels count matches we can just memcpy
	if(channels==(int)outChannels){
		memcpy(out,buffPtr,nFramesToCopy*channels*sizeof(float));
	}else if(channels>(int)outChannels){
		// otherwise, if we have more channels than the output is requesting,
		// we copy the first outChannels channels
		for(unsigned int i=0;i<nFramesToCopy;i++){
			for(unsigned int j=0;j<outChannels;j++){
				*out++ =  *buffPtr++;
			}
			// and skip the rest
			buffPtr += channels - outChannels;
		}
	}else{ 
		// we have fewer channels than output is requesting. so replicate as many channels as possible then loop.
		// if we have 2 channels and output wants 5, data is copied from our channels in the following in order:
		// 1 2 1 2 1
		for(unsigned int i=0;i<nFramesToCopy;i++){
			for(unsigned int j=0;j<outChannels;j++){
				*out++ = buffPtr[(j%channels)];
			}
			buffPtr += channels;
		}
	}

	// do we have anything left?
	int framesRemaining = nFrames - nFramesToCopy;
	if ( framesRemaining > 0 ){
		if(!loop || size()==0){
			// fill with 0s
			for(unsigned int i=0;i<framesRemaining*outChannels;i++){
				out[i] = 0;
			}
		}else{
			// loop
			copyTo(out,framesRemaining,outChannels,0,loop);
		}
	}
}

void ofSoundBuffer::addTo(float * out, unsigned int nFrames, unsigned int outChannels,unsigned int fromFrame,bool loop) const{
	// figure out how many frames we can copy before we need to stop or loop
	int nFramesToCopy = nFrames;
	if (int(this->getNumFrames()-fromFrame)<nFrames){
		nFramesToCopy = this->getNumFrames()-fromFrame;
	}

	const float * buffPtr = &buffer[fromFrame*channels];
	// if channels count matches it is easy
	if(channels==(int)outChannels){
		for(unsigned int i=0;i<nFramesToCopy*outChannels;i++){
			out[i] += buffPtr[i];
		}
	}else if(channels>(int)outChannels){
		// otherwise, if we have more channels than the output is requesting,
		// we copy the first outChannels channels
		for(unsigned int i=0;i<nFramesToCopy;i++){
			for(unsigned int j=0;j<outChannels;j++){
				*out++ += *buffPtr++;
			}
			// and skip the rest
			buffPtr += channels - outChannels;
		}
	}else{ 
		// we have fewer channels than output is requesting. so replicate as many channels as possible then loop.
		// if we have 2 channels and output wants 5, data is copied from our channels in the following in order:
		// 1 2 1 2 1
		for(unsigned int i=0;i<nFramesToCopy;i++){
			for(unsigned int j=0;j<outChannels;j++){
				*out++ += buffPtr[(j%channels)];
			}
			buffPtr += channels;
		}
	}

	// do we have anything left?
	int framesRemaining = nFrames - nFramesToCopy;
	if ( framesRemaining>0 && loop ){
		// loop
		addTo(out,framesRemaining,outChannels,0,loop);
	}
}

void ofSoundBuffer::resampleTo(ofSoundBuffer & buffer, unsigned int fromFrame, unsigned int numFrames, float speed, bool loop, InterpolationAlgorithm algorithm){
	switch(algorithm){
		case Linear:
			ofResampleLinear(*this, buffer, fromFrame, numFrames, speed, loop);
			break;
		case Hermite:
			ofResampleHermite(*this, buffer, fromFrame, numFrames, speed, loop);
			break;
	}
}

void ofSoundBuffer::resample(float speed, InterpolationAlgorithm algorithm){
	ofSoundBuffer resampled;
	resampleTo(resampled, 0, ceilf(getNumFrames() / speed), speed, false, algorithm);
	*this = resampled;
}

void ofSoundBuffer::getChannel(ofSoundBuffer & targetBuffer, int sourceChannel) {
	if ( sourceChannel>=channels ){
		ofLogWarning("ofSoundBuffer") << "getChannel requested channel " << sourceChannel << " but we only have " << channels << " channels. clamping channel to " << channels-1;
		sourceChannel = channels-1;
	}
	if ( sourceChannel<0 ){
		ofLogError("ofSoundBuffer") << "getChannel requested invalid channel " << sourceChannel << ", bailing out";
		return;
	}
	targetBuffer.setNumChannels(1);
	targetBuffer.setSampleRate(samplerate);
	if(channels==1){
		copyTo(targetBuffer,getNumFrames(),1,0);
	}else{
		// fetch samples from only one channel
		targetBuffer.resize(getNumFrames()/getNumChannels());
		const float * bufferPtr = &this->buffer[0];
		for(unsigned int i=0;i<targetBuffer.getNumFrames();i++){
			targetBuffer[i] = *bufferPtr;
			bufferPtr+=channels;
		}
	}
}

void ofSoundBuffer::setChannel(const ofSoundBuffer & inBuffer, int targetChannel){
	// resize ourself to match inBuffer
	resize(inBuffer.getNumFrames()*channels);
	// copy from inBuffer to targetChannel
	float * bufferPtr = &this->buffer[targetChannel];
	const float * inBufferPtr = &(inBuffer[0]);
	for(unsigned int i=0;i<getNumFrames();i++){
		*bufferPtr = *inBufferPtr;
		bufferPtr += channels;
		// inBuffer.getNumChannels() is probably 1 but let's be safe
		inBufferPtr += inBuffer.getNumChannels(); 
	}
}

float ofSoundBuffer::getRMSAmplitude(){
	float rmsAmplitude  = 0;
	for(unsigned int i=0;i<size();i++){
		rmsAmplitude += abs(buffer[i]);
	}
	rmsAmplitude /= float(size());
	return rmsAmplitude;
}

float ofSoundBuffer::getRMSAmplitudeChannel(unsigned int channel){
	if(channel>getNumChannels()-1) return 0;
	float rmsAmplitude  = 0;
	for(unsigned int i=0;i<getNumFrames();i++){
		rmsAmplitude += abs(buffer[i*getNumChannels()+channel]);
	}
	rmsAmplitude /= float(getNumFrames());
	return rmsAmplitude;
}

void ofSoundBuffer::normalize(float level){
	float maxAmplitude = 0;
	for(unsigned i = 0; i < size(); i++) {
		maxAmplitude = max(maxAmplitude, abs(buffer[i]));
	}
	float normalizationFactor = level/maxAmplitude;
	for(unsigned i = 0; i < size(); i++) {
		buffer[i] *= normalizationFactor;
	}
}

bool ofSoundBuffer::trimSilence(float threshold, bool trimStart, bool trimEnd) {
	if(buffer.empty()) {
		ofLogVerbose("ofSoundBuffer") << "attempted to trim empty buffer";
		return true;
	}
	size_t firstNonSilence = 0;
	size_t lastNonSilence = buffer.size() - 1;
	if(trimStart) {
		for(size_t i = 0; i < buffer.size(); ++i) {
			if(abs(buffer[i]) > threshold) {
				firstNonSilence = i;
				break;
			}
		}
	}
	if(trimEnd) {
		for(size_t i = lastNonSilence; i > firstNonSilence; --i) {
			if(abs(buffer[i]) > threshold) {
				lastNonSilence = i;
				break;
			}
		}
	}
	firstNonSilence -= firstNonSilence % getNumChannels();
	lastNonSilence  -= lastNonSilence  % getNumChannels();
	if(trimEnd) {
		buffer.erase(buffer.begin() + lastNonSilence, buffer.end());
	}
	if(trimStart) {
		buffer.erase(buffer.begin(), buffer.begin() + firstNonSilence);
	}
	return checkSizeAndChannelsConsistency("trimSilence");
}

void ofSoundBuffer::fillWithNoise(float amplitude){
	for ( unsigned i=0; i<size(); i++ ) {
		buffer[i] = ofRandom(-amplitude, amplitude);
	}
}

float ofSoundBuffer::fillWithTone( float pitchHz, float phase ){
	float step = TWO_PI*(pitchHz/samplerate);
	for ( unsigned i=0; i<size()/channels; i++ ) {
		unsigned int base = i*channels;
		for ( unsigned j=0; j<channels; j++)
			buffer[base+j] = sinf(phase);
		phase += step;
	}
	return phase;
}
