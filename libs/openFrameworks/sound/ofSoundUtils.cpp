/*
 * ofSoundUtils.cpp
 *
 *  Created on: 30/07/2012
 *      Author: arturo
 */

#include "ofSoundUtils.h"
#include "ofLog.h"

bool prepareBufferForResampling(const ofSoundBuffer &in, ofSoundBuffer &out, unsigned int numFrames) {
	unsigned int totalOutBufferSize = numFrames * in.getNumChannels();
	
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
void ofResampleLinear(const ofSoundBuffer &inBuffer, ofSoundBuffer &outBuffer, unsigned int fromFrame, unsigned int numFrames, float speed, bool loop) {
	
	int inChannels = inBuffer.getNumChannels();
	unsigned long inFrames = inBuffer.getNumFrames();
	bool bufferReady = prepareBufferForResampling(inBuffer, outBuffer, numFrames);
	
	if(!bufferReady) {
		outBuffer = inBuffer;
		return;
	}
	
	unsigned int start = fromFrame;
	unsigned int end = start*inChannels + double(numFrames*inChannels)*speed;
	double position = start;
	unsigned int intPosition = position;
	float increment = speed;
	unsigned int copySize = inChannels*sizeof(float);
	unsigned int to;
	
	if(end<inBuffer.size()-2*inChannels){
		to = numFrames;
	}else if(fromFrame+2>inFrames){
		to = 0;
	}else{
		to = ceil(float(inFrames-2-fromFrame)/speed);
	}
	
	float remainder = position - intPosition;
	float * resBufferPtr = &outBuffer[0];
	float a, b;
	
	for(unsigned int i=0;i<to;i++){
		intPosition *= inChannels;
		for(int j=0;j<inChannels;j++){
			a = inBuffer[intPosition];
			b = inBuffer[intPosition+inChannels];
			*resBufferPtr++ = ofLerp(a,b,remainder);
		}
		position += increment;
		intPosition = position;
		remainder = position - intPosition;
	}
	if(end>=inBuffer.size()-2*inChannels){
		to = numFrames-to;
		if(loop){
			intPosition %= inFrames;
			for(unsigned int i=0;i<to;i++){
				intPosition *= inChannels;
				for(int j=0;j<inChannels;j++){
					a = inBuffer[intPosition];
					b = inBuffer[intPosition+inChannels];
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
void ofResampleHermite(const ofSoundBuffer &inBuffer, ofSoundBuffer &outBuffer, unsigned int fromFrame, unsigned int numFrames, float speed, bool loop) {
	
	int inChannels = inBuffer.getNumChannels();
	unsigned long inFrames = inBuffer.getNumFrames();
	bool bufferReady = prepareBufferForResampling(inBuffer, outBuffer, numFrames);
	
	if(!bufferReady) {
		outBuffer = inBuffer;
		return;
	}
	
	unsigned int start = fromFrame;
	unsigned int end = start*inChannels + double(numFrames*inChannels)*speed;
	double position = start;
	unsigned int intPosition = position;
	float remainder = position - intPosition;
	float increment = speed;
	unsigned int copySize = inChannels*sizeof(float);
	unsigned int to;
	
	if(end<inBuffer.size()-3*inChannels){
		to = numFrames;
	}else if(fromFrame+3>inFrames){
		to = 0;
	}else{
		to = double(inFrames-3-fromFrame)/speed;
	}
	
	float * resBufferPtr = &outBuffer[0];
	float a,b,c,d;
	unsigned int from = 0;
	
	while(intPosition==0){
		intPosition *= inChannels;
		for(int j=0;j<inChannels;++j){
			a=loop?inBuffer[j]:0;
			b=inBuffer[intPosition+j];
			c=inBuffer[intPosition+j+inChannels];
			d=inBuffer[intPosition+j+inChannels*2];
			*resBufferPtr++ = ofInterpolateHermite(a, b, c, d, remainder);
		}
		position += increment;
		intPosition = position;
		remainder = position - intPosition;
		from++;
	}
	
	for(unsigned int i=from;i<to;++i){
		intPosition *= inChannels;
		for(int j=0;j<inChannels;++j){
			a=inBuffer[intPosition+j-inChannels];
			b=inBuffer[intPosition+j];
			c=inBuffer[intPosition+j+inChannels];
			d=inBuffer[intPosition+j+inChannels*2];
			*resBufferPtr++ = ofInterpolateHermite(a, b, c, d, remainder);
		}
		position += increment;
		intPosition = position;
		remainder = position - intPosition;
	}
	
	if(end>=inBuffer.size()-3*inChannels){
		to = numFrames-to;
		if(loop){
			intPosition %= inBuffer.size();
			for(unsigned int i=0;i<to;++i){
				for(int j=0;j<inChannels;++j){
					a=inBuffer[intPosition+j-inChannels];
					b=inBuffer[intPosition+j];
					c=inBuffer[intPosition+j+inChannels];
					d=inBuffer[intPosition+j+inChannels*2];
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
