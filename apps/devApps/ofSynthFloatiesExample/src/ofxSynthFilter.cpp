#include "ofxSynthFilter.h"
void ofxSynthFilter::setup(){
	cutoff = 1.0;
	resonance = 0.0;
	
	filterStates.resize(8);
	for ( int i=0; i<filterStates.size(); i++ ) { 
		filterState& fs = filterStates[i];
		fs.b0 = fs.b1 = fs.b2 = fs.b3 = fs.b4 = 0.0;  //filter buffers (beware denormals!)
		fs.t1 = fs.t2 = 0.0;					//temporary buffers
	}
	lowPass = true;
	calc();
	

}
void ofxSynthFilter::setRes(float _res) {
	if ( !isnan(_res) && isfinite(_res ) ) {
		resonance = _res;
		calc();
	}
}
void ofxSynthFilter::setCutoff(float _cutoff)
{
	if ( !isnan(_cutoff) && isfinite(_cutoff ) ) {
		if(lowPass){
			cutoff = _cutoff;
		}else{
			cutoff = _cutoff-1.0;
		}
		cutoff = fmin(fmax(cutoff, 0.0), 1.0);
		calc();
	}
}
void ofxSynthFilter::calc()
{
	q = 1.0 - cutoff;
	p = cutoff + 0.8 * cutoff * q;
	f = p + p - 1.0;
	q = resonance * (1.0 + 0.5 * q * (1.0f - q + 5.6 * q * q));
}

void ofxSynthFilter::process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels ){
	for (int i = 0; i<numFrames; i++) {
		for ( int j=0; j<numOutChannels; j++ ) {
			filterState& fs = filterStates[j];
			
			// clamp input
			output[i*numOutChannels+j] = min(1.0f,max(-1.0f,input[i*numInChannels+(j%numInChannels)]));
			output[i*numOutChannels+j] -= q * fs.b4;				//feedback
			fs.t1 = fs.b1;  fs.b1 = (output[i*numOutChannels+j] + fs.b0) * p - fs.b1 * f;
			fs.t2 = fs.b2;  fs.b2 = (fs.b1 + fs.t1) * p - fs.b2 * f;
			fs.t1 = fs.b3;  fs.b3 = (fs.b2 + fs.t2) * p - fs.b3 * f;
			fs.b4 = (fs.b3 + fs.t1) * p - fs.b4 * f;
			fs.b4 -= fs.b4 * fs.b4 * fs.b4 * 0.166667;		//clipping
			fs.b0 = output[i*numOutChannels+j];
			if(lowPass){
				output[i*numOutChannels+j] = fs.b4;
			}else{
				output[i*numOutChannels+j] = output[i*numOutChannels+j]-fs.b4;
			}
/*			if (numOutChannels > 1) {
				for (int j=1; j<numOutChannels; j++) {
					output[i*numOutChannels+j] = output[i*numOutChannels];
				}
			}*/
		}
	}
}
void ofxSynthFilter::processSample(float *inputSample)
{
}
