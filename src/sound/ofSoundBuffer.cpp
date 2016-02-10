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

#if !defined(TARGET_ANDROID) && !defined(TARGET_IPHONE) && !defined(TARGET_LINUX_ARM)
ofSoundBuffer::InterpolationAlgorithm ofSoundBuffer::defaultAlgorithm = ofSoundBuffer::Hermite;
#else
ofSoundBuffer::InterpolationAlgorithm ofSoundBuffer::defaultAlgorithm = ofSoundBuffer::Linear;
#endif

ofSoundBuffer::ofSoundBuffer()
:channels(1)
,samplerate(44100)
,tickCount(0)
,soundStreamDeviceID(0){

}

ofSoundBuffer::ofSoundBuffer(short * shortBuffer, std::size_t numFrames, std::size_t numChannels, unsigned int sampleRate)
:tickCount(0)
,soundStreamDeviceID(0) {
	copyFrom(shortBuffer, numFrames, numChannels, sampleRate);
	checkSizeAndChannelsConsistency("constructor");
}

void ofSoundBuffer::copyFrom(const short * shortBuffer, std::size_t numFrames, std::size_t numChannels, unsigned int sampleRate) {
	this->channels = numChannels;
	this->samplerate = sampleRate;
	buffer.resize(numFrames * numChannels);
	for(std::size_t i = 0; i < size(); i++){
		buffer[i] = shortBuffer[i]/float(numeric_limits<short>::max());
	}
	checkSizeAndChannelsConsistency("copyFrom");
}

void ofSoundBuffer::copyFrom(const float * floatBuffer, std::size_t numFrames, std::size_t numChannels, unsigned int sampleRate) {
	this->channels = numChannels;
	this->samplerate = sampleRate;
	buffer.assign(floatBuffer, floatBuffer + (numFrames * numChannels));
	checkSizeAndChannelsConsistency("copyFrom");
}

void ofSoundBuffer::copyFrom(const vector<short> & shortBuffer, std::size_t numChannels, unsigned int sampleRate){
	copyFrom(&shortBuffer[0],shortBuffer.size()/numChannels,numChannels,sampleRate);
}

void ofSoundBuffer::copyFrom(const vector<float> & floatBuffer, std::size_t numChannels, unsigned int sampleRate){
	copyFrom(&floatBuffer[0],floatBuffer.size()/numChannels,numChannels,sampleRate);
}

void ofSoundBuffer::toShortPCM(vector<short> & dst) const{
	dst.resize(size());
	for(std::size_t i = 0; i < size(); i++){
		dst[i] = buffer[i]*float(numeric_limits<short>::max());
	}
}

void ofSoundBuffer::toShortPCM(short * dst) const{
	for(std::size_t i = 0; i < size(); i++){
		dst[i] = buffer[i]*float(numeric_limits<short>::max());
	}
}

vector<float> & ofSoundBuffer::getBuffer(){
	return buffer;
}

const vector<float> & ofSoundBuffer::getBuffer() const{
	return buffer;
}

uint64_t ofSoundBuffer::getDurationMS() const{
	return uint64_t(getNumFrames()) * uint64_t(1000) / uint64_t(samplerate);
}

uint64_t ofSoundBuffer::getDurationMicros() const{
	return uint64_t(getNumFrames()) * uint64_t(1000000) / uint64_t(samplerate);
}

uint64_t ofSoundBuffer::getDurationNanos() const{
	return uint64_t(getNumFrames()) * uint64_t(1000000000) / uint64_t(samplerate);
}

void ofSoundBuffer::setNumChannels(int channels){
	this->channels = channels;
	checkSizeAndChannelsConsistency("setNumChannels");
}

void ofSoundBuffer::setSampleRate(unsigned int rate){
	samplerate = rate;
}

void ofSoundBuffer::allocate(size_t numSamples, size_t numChannels){
	resize(numSamples*numChannels);
	channels = numChannels;
}

void ofSoundBuffer::resize(std::size_t samples, float val){
	buffer.resize(samples, val);
	checkSizeAndChannelsConsistency("resize(samples,val)");
}

void ofSoundBuffer::clear(){
	buffer.clear();
}

void ofSoundBuffer::set(float value){
	buffer.assign(buffer.size(), value);
	checkSizeAndChannelsConsistency("set");
}

bool ofSoundBuffer::checkSizeAndChannelsConsistency(const std::string& _function ) {
	std::string function = _function;

	if ( function.size()!= 0 ){
		function += ": ";
	}
	if ( (size()%channels) != 0 ){
		ofLogWarning("ofSoundBuffer") << function << "channel count " << channels << " is not consistent with sample count " << size() << " (non-zero remainder)";
		return false;
	}
	return true;
}

float & ofSoundBuffer::operator[](std::size_t pos){
	return buffer[pos];
}

const float & ofSoundBuffer::operator[](std::size_t pos) const{
	return buffer[pos];
}

float & ofSoundBuffer::getSample(std::size_t frameIndex, std::size_t channel){
	return buffer[(frameIndex * channels) + channel];
}

const float & ofSoundBuffer::getSample(std::size_t frameIndex, std::size_t channel) const {
	return buffer[(frameIndex * channels) + channel];
}

void ofSoundBuffer::swap(ofSoundBuffer & buffer){
	std::swap(this->channels, buffer.channels);
	std::swap(this->samplerate, buffer.samplerate);
	std::swap(this->tickCount, buffer.tickCount);
	std::swap(this->soundStreamDeviceID, buffer.soundStreamDeviceID);
	std::swap(this->buffer, buffer.buffer);
}

ofSoundBuffer ofSoundBuffer::operator*(float value){
	ofSoundBuffer ret = *this;
	ret *= value;
	return ret;
}

ofSoundBuffer & ofSoundBuffer::operator*=(float value){
	for(std::size_t i=0;i<buffer.size();i++){
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
	for(std::size_t i=0;i<getNumFrames();i++){
		*bufferPtr++ *= left;
		*bufferPtr++ *= right;
	}
}

void ofSoundBuffer::copyTo(ofSoundBuffer & soundBuffer, std::size_t nFrames, std::size_t outChannels,std::size_t fromFrame,bool loop) const{
	soundBuffer.resize(nFrames*outChannels);
	soundBuffer.setNumChannels(outChannels);
	soundBuffer.setSampleRate(samplerate);
	copyTo(&soundBuffer[0], nFrames, outChannels, fromFrame, loop);
}

void ofSoundBuffer::copyTo(ofSoundBuffer & outBuffer, std::size_t fromFrame, bool loop) const{
	copyTo(&outBuffer[0], outBuffer.getNumFrames(), outBuffer.getNumChannels(), fromFrame, loop);
}

void ofSoundBuffer::addTo(ofSoundBuffer & soundBuffer, std::size_t nFrames, std::size_t outChannels,std::size_t fromFrame, bool loop) const {
	soundBuffer.resize(nFrames*outChannels);
	soundBuffer.setNumChannels(outChannels);
	soundBuffer.setSampleRate(samplerate);
	addTo(&soundBuffer.getBuffer()[0], nFrames, outChannels, fromFrame, loop);
}

void ofSoundBuffer::addTo(ofSoundBuffer & outBuffer, std::size_t fromFrame, bool loop) const{
	addTo(&outBuffer[0], outBuffer.getNumFrames(), outBuffer.getNumChannels(), fromFrame, loop);
}

void ofSoundBuffer::copyTo(float * outBuffer, std::size_t nFrames, std::size_t outChannels, std::size_t fromFrame, bool loop) const{
	// figure out how many frames we can copy before we need to stop or loop
	std::size_t nFramesToCopy = nFrames;
	if ((fromFrame + nFrames) >= this->getNumFrames()){
		nFramesToCopy = this->getNumFrames() - fromFrame;
	}
		
	const float * buffPtr = &buffer[fromFrame * channels];
	// if channels count matches we can just memcpy
	if(channels == outChannels){
		memcpy(outBuffer, buffPtr, nFramesToCopy * channels * sizeof(float));
		outBuffer += nFramesToCopy * outChannels;
	} else if(channels > outChannels){
		// otherwise, if we have more channels than the output is requesting,
		// we copy the first outChannels channels
		for(std::size_t i = 0; i < nFramesToCopy; i++){
			for(std::size_t j = 0; j < outChannels; j++){
				*outBuffer++ = *buffPtr++;
			}
			// and skip the rest
			buffPtr += channels - outChannels;
		}
	} else {
		// we have fewer channels than output is requesting. so replicate as many channels as possible then loop.
		// if we have 2 channels and output wants 5, data is copied from our channels in the following in order:
		// 1 2 1 2 1
		for(std::size_t i = 0; i < nFramesToCopy; i++){
			for(std::size_t j = 0; j < outChannels; j++){
				*outBuffer++ = buffPtr[(j%channels)];
			}
			buffPtr += channels;
		}
	}

	// do we have anything left?
	int framesRemaining = nFrames - (int)nFramesToCopy;
	if (framesRemaining > 0){
		if(!loop || size() == 0){
			// fill with 0s
			for(std::size_t i = 0; i < framesRemaining * outChannels; i++){
				outBuffer[i] = 0;
			}
		}else{
			// loop
			copyTo(outBuffer, framesRemaining, outChannels, 0, loop);
		}
	}
}

void ofSoundBuffer::addTo(float * outBuffer, std::size_t nFrames, std::size_t outChannels, std::size_t fromFrame, bool loop) const{
	// figure out how many frames we can copy before we need to stop or loop
	std::size_t nFramesToCopy = nFrames;
	if ((fromFrame + nFrames) >= this->getNumFrames()){
		nFramesToCopy = this->getNumFrames() - fromFrame;
	}

	const float * buffPtr = &buffer[fromFrame * channels];
	// if channels count matches it is easy
	if(channels == outChannels){
		for(std::size_t i = 0; i < (nFramesToCopy * outChannels); i++){
			outBuffer[i] += buffPtr[i];
		}
		outBuffer += nFramesToCopy * outChannels;
	} else if(channels > outChannels){
		// otherwise, if we have more channels than the output is requesting,
		// we copy the first outChannels channels
		for(std::size_t i = 0; i < nFramesToCopy; i++){
			for(std::size_t j = 0; j < outChannels; j++){
				*outBuffer++ += *buffPtr++;
			}
			// and skip the rest
			buffPtr += channels - outChannels;
		}
	} else {
		// we have fewer channels than output is requesting. so replicate as many channels as possible then loop.
		// if we have 2 channels and output wants 5, data is copied from our channels in the following in order:
		// 1 2 1 2 1
		for(std::size_t i = 0; i < nFramesToCopy; i++){
			for(std::size_t j = 0; j < outChannels; j++){
				*outBuffer++ += buffPtr[(j%channels)];
			}
			buffPtr += channels;
		}
	}

	// do we have anything left?
	int framesRemaining = nFrames - (int)nFramesToCopy;
	if (framesRemaining > 0 && loop){
		// loop
		addTo(outBuffer, framesRemaining, outChannels, 0, loop);
	}
}


void ofSoundBuffer::append(ofSoundBuffer & other){
	if(other.getNumChannels() != getNumChannels()){
		ofLogError() << "can't append sound buffers with different num channels";
		return;
	}
	buffer.insert(buffer.end(),other.buffer.begin(),other.buffer.end());
}

static bool prepareBufferForResampling(const ofSoundBuffer &in, ofSoundBuffer &out, std::size_t numFrames) {
	std::size_t totalOutBufferSize = numFrames * in.getNumChannels();
	
	if(totalOutBufferSize < out.getBuffer().max_size()) {
		out.resize(totalOutBufferSize,0);
	} else {
		ofLogError("ofSoundUtils") << "resampling would create a buffer size of " << totalOutBufferSize << " (too large for std::vector)";
		return false;
	}
	
	out.setNumChannels(in.getNumChannels());
	out.setSampleRate(in.getSampleRate());
	return true;
}

// based on maximilian optimized for performance.
// might lose 1 or 2 samples when it reaches the end of the buffer
void ofSoundBuffer::linearResampleTo(ofSoundBuffer &outBuffer, std::size_t fromFrame, std::size_t numFrames, float speed, bool loop) const {
	
	std::size_t inChannels = getNumChannels();
	std::size_t inFrames = getNumFrames();
	bool bufferReady = prepareBufferForResampling(*this, outBuffer, numFrames);
	
	if(!bufferReady) {
		outBuffer = *this;
		return;
	}
	
	std::size_t start = fromFrame;
	std::size_t end = start*inChannels + double(numFrames*inChannels)*speed;
	double position = start;
	std::size_t intPosition = position;
	float increment = speed;
	std::size_t copySize = inChannels*sizeof(float);
	std::size_t to;
	
	if(end<size()-2*inChannels){
		to = numFrames;
	}else if(fromFrame+2>inFrames){
		to = 0;
	}else{
		to = ceil(float(inFrames-2-fromFrame)/speed);
	}
	
	float remainder = position - intPosition;
	float * resBufferPtr = &outBuffer[0];
	float a, b;
	
	for(std::size_t i=0;i<to;i++){
		intPosition *= inChannels;
		for(std::size_t j=0;j<inChannels;j++){
			a = buffer[intPosition];
			b = buffer[intPosition+inChannels];
			*resBufferPtr++ = ofLerp(a,b,remainder);
		}
		position += increment;
		intPosition = position;
		remainder = position - intPosition;
	}
	if(end>=size()-2*inChannels){
		to = numFrames-to;
		if(loop){
			intPosition %= inFrames;
			for(std::size_t i=0;i<to;i++){
				intPosition *= inChannels;
				for(std::size_t j=0;j<inChannels;j++){
					a = buffer[intPosition];
					b = buffer[intPosition+inChannels];
					*resBufferPtr++ = (b-a)*remainder+a;
				}
				resBufferPtr+=inChannels;
				position += increment;
				intPosition = position;
			}
		}else{
			memset(resBufferPtr,0,to*copySize);
		}
	}
}

// based on maximilian optimized for performance.
// might lose 1 to 3 samples when it reaches the end of the buffer
void ofSoundBuffer::hermiteResampleTo(ofSoundBuffer &outBuffer, std::size_t fromFrame, std::size_t numFrames, float speed, bool loop) const {
	
	std::size_t inChannels = getNumChannels();
	std::size_t inFrames = getNumFrames();
	bool bufferReady = prepareBufferForResampling(*this, outBuffer, numFrames);
	
	if(!bufferReady) {
		outBuffer = *this;
		return;
	}
	
	std::size_t start = fromFrame;
	std::size_t end = start*inChannels + double(numFrames*inChannels)*speed;
	double position = start;
	std::size_t intPosition = position;
	float remainder = position - intPosition;
	float increment = speed;
	std::size_t copySize = inChannels*sizeof(float);
	std::size_t to;
	
	if(end<size()-3*inChannels){
		to = numFrames;
	}else if(fromFrame+3>inFrames){
		to = 0;
	}else{
		to = double(inFrames-3-fromFrame)/speed;
	}
	
	float * resBufferPtr = &outBuffer[0];
	float a,b,c,d;
	std::size_t from = 0;
	
	while(intPosition==0){
		intPosition *= inChannels;
		for(std::size_t j=0;j<inChannels;++j){
			a=loop?buffer[j]:0;
			b=buffer[intPosition+j];
			c=buffer[intPosition+j+inChannels];
			d=buffer[intPosition+j+inChannels*2];
			*resBufferPtr++ = ofInterpolateHermite(a, b, c, d, remainder);
		}
		position += increment;
		intPosition = position;
		remainder = position - intPosition;
		from++;
	}
	
	for(std::size_t i=from;i<to;++i){
		intPosition *= inChannels;
		for(std::size_t j=0;j<inChannels;++j){
			a=buffer[intPosition+j-inChannels];
			b=buffer[intPosition+j];
			c=buffer[intPosition+j+inChannels];
			d=buffer[intPosition+j+inChannels*2];
			*resBufferPtr++ = ofInterpolateHermite(a, b, c, d, remainder);
		}
		position += increment;
		intPosition = position;
		remainder = position - intPosition;
	}
	
	if(end>=size()-3*inChannels){
		to = numFrames-to;
		if(loop){
			intPosition %= size();
			for(std::size_t i=0;i<to;++i){
				for(std::size_t j=0;j<inChannels;++j){
					a=buffer[intPosition+j-inChannels];
					b=buffer[intPosition+j];
					c=buffer[intPosition+j+inChannels];
					d=buffer[intPosition+j+inChannels*2];
					*resBufferPtr++ = ofInterpolateHermite(a, b, c, d, remainder);
				}
				position += increment;
				intPosition = position;
				remainder = position - intPosition;
				intPosition *= inChannels;
			}
		}else{
			memset(resBufferPtr,0,to*copySize);
		}
	}
}

void ofSoundBuffer::resampleTo(ofSoundBuffer & buffer, std::size_t fromFrame, std::size_t numFrames, float speed, bool loop, InterpolationAlgorithm algorithm) const {
	switch(algorithm){
		case Linear:
			linearResampleTo(buffer, fromFrame, numFrames, speed, loop);
			break;
		case Hermite:
			hermiteResampleTo(buffer, fromFrame, numFrames, speed, loop);
			break;
	}
}

void ofSoundBuffer::resample(float speed, InterpolationAlgorithm algorithm){
	ofSoundBuffer resampled;
	resampleTo(resampled, 0, ceilf(getNumFrames() / speed), speed, false, algorithm);
	*this = resampled;
}

void ofSoundBuffer::getChannel(ofSoundBuffer & targetBuffer, std::size_t sourceChannel) const {
	if(channels == 0) {
		ofLogWarning("ofSoundBuffer") << "getChannel requested on empty buffer";
		return;
	}
	if (sourceChannel >= channels){
		ofLogWarning("ofSoundBuffer") << "getChannel requested channel " << sourceChannel << " but we only have " << channels << " channels. clamping channel to " << channels-1;
		sourceChannel = channels-1;
	}
	targetBuffer.setNumChannels(1);
	targetBuffer.setSampleRate(samplerate);
	if(channels == 1){
		copyTo(targetBuffer, getNumFrames(), 1, 0);
	}else{
		// fetch samples from only one channel
		targetBuffer.resize(getNumFrames());
		const float * bufferPtr = &this->buffer[sourceChannel];
		for(std::size_t i = 0; i < targetBuffer.getNumFrames(); i++){
			targetBuffer[i] = *bufferPtr;
			bufferPtr += channels;
		}
	}
}

void ofSoundBuffer::setChannel(const ofSoundBuffer & inBuffer, std::size_t targetChannel){
	// resize ourself to match inBuffer
	resize(inBuffer.getNumFrames() * channels);
	// copy from inBuffer to targetChannel
	float * bufferPtr = &this->buffer[targetChannel];
	const float * inBufferPtr = &(inBuffer[targetChannel]);
	for(std::size_t i = 0; i < getNumFrames(); i++){
		*bufferPtr = *inBufferPtr;
		bufferPtr += channels;
		// inBuffer.getNumChannels() is probably 1 but let's be safe
		inBufferPtr += inBuffer.getNumChannels(); 
	}
}

float ofSoundBuffer::getRMSAmplitude() const {
	double acc = 0;
	for(size_t i = 0; i < buffer.size(); i++){
		acc += buffer[i] * buffer[i];
	}
	return sqrt(acc / (double)buffer.size());
}

float ofSoundBuffer::getRMSAmplitudeChannel(std::size_t channel) const {
	if(channel > channels - 1) {
		return 0;
	}

	double acc = 0;
	for(size_t i = 0; i < getNumFrames(); i++) {
		float sample = getSample(i, channel);
		acc += sample * sample;
	}
	return sqrt(acc / (double)getNumFrames());
}

void ofSoundBuffer::normalize(float level){
	float maxAmplitude = 0;
	for(std::size_t i = 0; i < size(); i++) {
		maxAmplitude = max(maxAmplitude, abs(buffer[i]));
	}
	float normalizationFactor = level/maxAmplitude;
	for(std::size_t i = 0; i < size(); i++) {
		buffer[i] *= normalizationFactor;
	}
}

bool ofSoundBuffer::trimSilence(float threshold, bool trimStart, bool trimEnd) {
	if(buffer.empty()) {
		ofLogVerbose("ofSoundBuffer") << "attempted to trim empty buffer";
		return true;
	}
	std::size_t firstNonSilence = 0;
	std::size_t lastNonSilence = buffer.size() - 1;
	if(trimStart) {
		for(std::size_t i = 0; i < buffer.size(); ++i) {
			if(abs(buffer[i]) > threshold) {
				firstNonSilence = i;
				break;
			}
		}
	}
	if(trimEnd) {
		for(std::size_t i = lastNonSilence; i > firstNonSilence; --i) {
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
	for (std::size_t i=0; i<size(); i++ ) {
		buffer[i] = ofRandom(-amplitude, amplitude);
	}
}

float ofSoundBuffer::fillWithTone( float pitchHz, float phase ){
	float step = TWO_PI*(pitchHz/samplerate);
	for (std::size_t i=0; i<size()/channels; i++ ) {
		std::size_t base = i*channels;
		for (std::size_t j=0; j<channels; j++)
			buffer[base+j] = sinf(phase);
		phase += step;
	}
	return phase;
}

namespace std{
	void swap(ofSoundBuffer & src, ofSoundBuffer & dst){
		src.swap(dst);
	}
}

