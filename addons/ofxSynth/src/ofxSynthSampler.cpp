#include "ofxSynthSampler.h"

void ofxSynthSampler::setSampleRate( int rate )
{
	sampleRate = rate;
}
void ofxSynthSampler::setLoopPoints(float i, float o){
	inPoint=fmax(0, i);
	outPoint=fmin(1, o);
}
void ofxSynthSampler::trigger(){
	sample.position = inPoint*sample.length;
}
void ofxSynthSampler::audioRequested( float* buffer, int numFrames, int numChannels ){
	for (int i = 0; i < numFrames; i++){
		buffer[i*numChannels] = sample.play4(currentFrequency, inPoint*sample.length, outPoint*sample.length, sampleRate);
		for (int j = 1; j<numChannels; j++) {
			buffer[i*numChannels+j] = buffer[i*numChannels];
		}
	}
}
void ofxSynthSampler::setFrequencyMidiNote(float note){
	currentFrequency = pow(2.0, (note-60.0)/12.0f);
}
void ofxSynthSampler::setFrequencySyncToLength(int length){
	currentFrequency = sample.length/(float)length;
}
void ofxSynthSampler::loadFile(string file){
	sample.load(ofToDataPath(file));
	printf("Summary:\n%s", sample.getSummary());
}
bool ofxSynthSample::load(string fileName) {
	myPath = fileName;
	return read();
}
void ofxSynthSample::getLength() {
	length=myDataSize*0.5;	
}
//better cubic inerpolation. Cobbled together from various (pd externals, yehar, other places).
double ofxSynthSample::play4(double frequency, double start, double end, int sampleRate) {
	double remainder;
	double a,b,c,d,a1,a2,a3;
	short* buffer = (short*)myData;
	if (frequency >0.) {
		if (position<start) {
			position=start;
		}
		if ( position >= end ) position = start;
		position += frequency*2;
		remainder = position - floor(position);
		if (position>0) {
			a=buffer[(int)(floor(position))-1];

		} else {
			a=buffer[0];
			
		}
		
		b=buffer[(long) position];
		if (position<end-2) {
			c=buffer[(long) position+1];

		} else {
			c=buffer[0];

		}
		if (position<end-3) {
			d=buffer[(long) position+2];

		} else {
			d=buffer[0];
		}
		a1 = 0.5f * (c - a);
		a2 = a - 2.5 * b + 2.f * c - 0.5f * d;
		a3 = 0.5f * (d - a) + 1.5f * (b - c);
		output = (double) (((a3 * remainder + a2) * remainder + a1) * remainder + b) / 32767;
		
	} else {
		frequency=frequency-(frequency+frequency);
		if ( position <= start ) position = end;
		position -= frequency*2;
		remainder = position - floor(position);
		if (position>start && position < end-1) {
			a=buffer[(long) position+1];
			
		} else {
			a=buffer[0];
			
		}
		
		b=buffer[(long) position];
		if (position>start) {
			c=buffer[(long) position-1];
			
		} else {
			c=buffer[0];
			
		}
		if (position>start+1) {
			d=buffer[(long) position-2];
			
		} else {
			d=buffer[0];
		}
		a1 = 0.5f * (c - a);
		a2 = a - 2.5 * b + 2.f * c - 0.5f * d;
		a3 = 0.5f * (d - a) + 1.5f * (b - c);
		output = (double) (((a3 * remainder + a2) * -remainder + a1) * -remainder + b) / 32767;
		
	}
	
	return(output);
}