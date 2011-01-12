#include "ofxSynth.h"

#define SAMPLE_RATE 44100.0

ofxSynth::ofxSynth(){
	
	currentFrequency = 440;
	currentAmp = 0;
	usesEnv = false;
	ampMode = OFXSYNTHONESHOT;
	gain = 0.1;
	sustain = 0.5;
	
	env.gateOpen();
	
	filter.setup();
	filter.setRes(0.0);
	filter.setCutoff(1.0);
	setFilterMode(1);
	waveMode = 0;
}
void ofxSynth::audioRequested(float * output, int bufferSize){
	for (int i = 0; i < bufferSize; i++){
		if (ampMode == OFXSYNTHONESHOT) {
			currentAmp = 1;
		}else if(ampMode == OFXSYNTHADR){
			currentAmp = env.getLevel();
		}
		if (currentAmp > 1) {
			currentAmp = 1;
		}
		switch (waveMode) {
			case 0:
				output[i] = wave.square(currentFrequency);
				break;
			case 1:
				output[i] = wave.triangle(currentFrequency, 0);
				break;
			case 2:
				output[i] = wave.saw(currentFrequency);
				break;
			default:
				output[i] = 0;
				break;
		}
		if (filterMode != 0) {
			filter.processSample(&output[i]);
		}
		output[i] *= currentAmp;
	}
}
void ofxSynth::trigger(){
	if (ampMode == OFXSYNTHONESHOT) {
		currentAmp = 1;
	}else if(ampMode == OFXSYNTHADR){
		env.gateOpen();
	}
}
void ofxSynth::setADRVol(float atk, float dec, float rel){
	// scale to sample rate
	env.inTime = atk*SAMPLE_RATE;
	env.holdTime = dec*SAMPLE_RATE;
	env.outTime = rel*SAMPLE_RATE;
}
void ofxSynth::setADRMod(float atk, float dec, float rel){
	// scale to sample rate
	modEnv.inTime = atk*SAMPLE_RATE;
	modEnv.holdTime = dec*SAMPLE_RATE;
	modEnv.outTime = rel*SAMPLE_RATE;
}
void ofxSynth::setFilter(float _cutoff, float _res){
	filter.setCutoff(_cutoff);
	filter.setRes(_res);
}
void ofxSynth::setFrequency(float freq){
	currentFrequency = freq;
}
void ofxSynth::setNote(float note){
	setFrequency(440.0*pow(2.0, (note-60.0)/12.0));
}
void ofxSynth::setFilterMode(int mode){
	filterMode = mode;
	if (filterMode < 0) {
		filter.lowPass = filterMode == 1 ? true : false;
	}
}

/* ----------- */
void ofxSynthSampler::audioRequested(float * input, int bufferSize){
	for (int i = 0; i < bufferSize; i++){
		input[i] = sample.play(currentFrequency);
	}
}
void ofxSynthSampler::setNote(float note){
	currentFrequency = pow(2.0, (note-60.0)/12.0f);
}
void ofxSynthSampler::loadFile(string file){
	cout << sample.load(file) << endl;
	printf("Summary:\n%s", sample.getSummary());
}


/* ----------- */
void ofxSynthEnvAHD::gateOpen(){
	offset = 0;
}
float ofxSynthEnvAHD::getLevel(){
	offset++;
	if(offset<inTime){
		level = ((float)offset)/inTime+0.01;
	}
	if (offset>inTime&&offset<inTime+holdTime) {
		level = 1.0;
	}
	if(offset>inTime+holdTime){
		level = 1.0-((float)offset-inTime-holdTime)/outTime;
	}
	if(offset>inTime+holdTime+outTime){
		level = 0;
	}
	return level;
}

/* ----------- */
void ofxSynthFilter::setup()
{
	b0 = b1 = b2 = b3 = b4 = 0.0;  //filter buffers (beware denormals!)
	t1 = t2 = 0.0;              //temporary buffers
	lowPass = true;
}	
void ofxSynthFilter::setRes(float _res)
{
	resonance = _res;
	calc();
}
void ofxSynthFilter::setCutoff(float _cutoff)
{
	if(lowPass){
		cutoff = _cutoff;
	}else{
		cutoff = _cutoff-1.0;
	}
	cutoff = fmin(fmax(cutoff, 0.0), 1.0);
	calc();
}
void ofxSynthFilter::calc()
{
	q = 1.0 - cutoff;
	p = cutoff + 0.8 * cutoff * q;
	f = p + p - 1.0;
	q = resonance * (1.0 + 0.5 * q * (1.0f - q + 5.6 * q * q));
}

void ofxSynthFilter::processSample(float *inputSample)
{
	*inputSample -= q * b4;                          //feedback
	t1 = b1;  b1 = (*inputSample + b0) * p - b1 * f;
	t2 = b2;  b2 = (b1 + t1) * p - b2 * f;
	t1 = b3;  b3 = (b2 + t2) * p - b3 * f;
	b4 = (b3 + t1) * p - b4 * f;
	b4 -= b4 * b4 * b4 * 0.166667;    //clipping
	b0 = *inputSample;
	if(lowPass){
		*inputSample = b4;
	}else{
		*inputSample = *inputSample-b4;
	}
}

/* ----------- */
void ofxSynthDelayline::process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels )
{
	for (int i = 0; i < numFrames; i++){
		if (phase>=size) {
			phase=0;
		}
		memory[phase]=(memory[phase]*feedback)+(input[i*numInChannels]*feedback)*0.5;
		phase+=1;
		for (int j=0; j<numOutChannels; j++) {
			// puts the sound in memory on to all the output channels
			output[i*numOutChannels+j]=memory[phase+1];
		}
	}
}
void ofxSynthDelayline::setSize(float _size){
	size = (int)_size*(int)sampleRate;
}
void ofxSynthDelayline::setFeedback(float _feedback){
	feedback = _feedback;
}
