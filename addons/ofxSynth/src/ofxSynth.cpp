#include "ofxSynth.h"

int channels=2;
int buffersize=1024;
float chandiv= 1;

double sineBuffer[514]={0,0.012268,0.024536,0.036804,0.049042,0.06131,0.073547,0.085785,0.097992,0.1102,0.12241,0.13455,0.1467,0.15884,0.17093,0.18301,0.19507,0.20709,0.21909,0.23105,0.24295,0.25485,0.26669,0.2785,0.29025,0.30197,0.31366,0.32529,0.33685,0.34839,0.35986,0.37128,0.38266,0.39395,0.40521,0.41641,0.42752,0.4386,0.44958,0.46051,0.47137,0.48215,0.49286,0.50351,0.51407,0.52457,0.53497,0.54529,0.55554,0.5657,0.57578,0.58575,0.59567,0.60547,0.6152,0.62482,0.63437,0.6438,0.65314,0.66238,0.67151,0.68057,0.68951,0.69833,0.70706,0.7157,0.72421,0.7326,0.74091,0.74908,0.75717,0.76514,0.77298,0.7807,0.7883,0.79581,0.80316,0.81042,0.81754,0.82455,0.83142,0.8382,0.84482,0.85132,0.8577,0.86392,0.87006,0.87604,0.88187,0.8876,0.89319,0.89862,0.90396,0.90912,0.91415,0.91907,0.92383,0.92847,0.93295,0.93729,0.9415,0.94556,0.94949,0.95325,0.95691,0.96039,0.96375,0.96692,0.97,0.9729,0.97565,0.97827,0.98074,0.98306,0.98523,0.98724,0.98914,0.99084,0.99243,0.99387,0.99515,0.99628,0.99725,0.99808,0.99875,0.99927,0.99966,0.99988,0.99997,0.99988,0.99966,0.99927,0.99875,0.99808,0.99725,0.99628,0.99515,0.99387,0.99243,0.99084,0.98914,0.98724,0.98523,0.98306,0.98074,0.97827,0.97565,0.9729,0.97,0.96692,0.96375,0.96039,0.95691,0.95325,0.94949,0.94556,0.9415,0.93729,0.93295,0.92847,0.92383,0.91907,0.91415,0.90912,0.90396,0.89862,0.89319,0.8876,0.88187,0.87604,0.87006,0.86392,0.8577,0.85132,0.84482,0.8382,0.83142,0.82455,0.81754,0.81042,0.80316,0.79581,0.7883,0.7807,0.77298,0.76514,0.75717,0.74908,0.74091,0.7326,0.72421,0.7157,0.70706,0.69833,0.68951,0.68057,0.67151,0.66238,0.65314,0.6438,0.63437,0.62482,0.6152,0.60547,0.59567,0.58575,0.57578,0.5657,0.55554,0.54529,0.53497,0.52457,0.51407,0.50351,0.49286,0.48215,0.47137,0.46051,0.44958,0.4386,0.42752,0.41641,0.40521,0.39395,0.38266,0.37128,0.35986,0.34839,0.33685,0.32529,0.31366,0.30197,0.29025,0.2785,0.26669,0.25485,0.24295,0.23105,0.21909,0.20709,0.19507,0.18301,0.17093,0.15884,0.1467,0.13455,0.12241,0.1102,0.097992,0.085785,0.073547,0.06131,0.049042,0.036804,0.024536,0.012268,0,-0.012268,-0.024536,-0.036804,-0.049042,-0.06131,-0.073547,-0.085785,-0.097992,-0.1102,-0.12241,-0.13455,-0.1467,-0.15884,-0.17093,-0.18301,-0.19507,-0.20709,-0.21909,-0.23105,-0.24295,-0.25485,-0.26669,-0.2785,-0.29025,-0.30197,-0.31366,-0.32529,-0.33685,-0.34839,-0.35986,-0.37128,-0.38266,-0.39395,-0.40521,-0.41641,-0.42752,-0.4386,-0.44958,-0.46051,-0.47137,-0.48215,-0.49286,-0.50351,-0.51407,-0.52457,-0.53497,-0.54529,-0.55554,-0.5657,-0.57578,-0.58575,-0.59567,-0.60547,-0.6152,-0.62482,-0.63437,-0.6438,-0.65314,-0.66238,-0.67151,-0.68057,-0.68951,-0.69833,-0.70706,-0.7157,-0.72421,-0.7326,-0.74091,-0.74908,-0.75717,-0.76514,-0.77298,-0.7807,-0.7883,-0.79581,-0.80316,-0.81042,-0.81754,-0.82455,-0.83142,-0.8382,-0.84482,-0.85132,-0.8577,-0.86392,-0.87006,-0.87604,-0.88187,-0.8876,-0.89319,-0.89862,-0.90396,-0.90912,-0.91415,-0.91907,-0.92383,-0.92847,-0.93295,-0.93729,-0.9415,-0.94556,-0.94949,-0.95325,-0.95691,-0.96039,-0.96375,-0.96692,-0.97,-0.9729,-0.97565,-0.97827,-0.98074,-0.98306,-0.98523,-0.98724,-0.98914,-0.99084,-0.99243,-0.99387,-0.99515,-0.99628,-0.99725,-0.99808,-0.99875,-0.99927,-0.99966,-0.99988,-0.99997,-0.99988,-0.99966,-0.99927,-0.99875,-0.99808,-0.99725,-0.99628,-0.99515,-0.99387,-0.99243,-0.99084,-0.98914,-0.98724,-0.98523,-0.98306,-0.98074,-0.97827,-0.97565,-0.9729,-0.97,-0.96692,-0.96375,-0.96039,-0.95691,-0.95325,-0.94949,-0.94556,-0.9415,-0.93729,-0.93295,-0.92847,-0.92383,-0.91907,-0.91415,-0.90912,-0.90396,-0.89862,-0.89319,-0.8876,-0.88187,-0.87604,-0.87006,-0.86392,-0.8577,-0.85132,-0.84482,-0.8382,-0.83142,-0.82455,-0.81754,-0.81042,-0.80316,-0.79581,-0.7883,-0.7807,-0.77298,-0.76514,-0.75717,-0.74908,-0.74091,-0.7326,-0.72421,-0.7157,-0.70706,-0.69833,-0.68951,-0.68057,-0.67151,-0.66238,-0.65314,-0.6438,-0.63437,-0.62482,-0.6152,-0.60547,-0.59567,-0.58575,-0.57578,-0.5657,-0.55554,-0.54529,-0.53497,-0.52457,-0.51407,-0.50351,-0.49286,-0.48215,-0.47137,-0.46051,-0.44958,-0.4386,-0.42752,-0.41641,-0.40521,-0.39395,-0.38266,-0.37128,-0.35986,-0.34839,-0.33685,-0.32529,-0.31366,-0.30197,-0.29025,-0.2785,-0.26669,-0.25485,-0.24295,-0.23105,-0.21909,-0.20709,-0.19507,-0.18301,-0.17093,-0.15884,-0.1467,-0.13455,-0.12241,-0.1102,-0.097992,-0.085785,-0.073547,-0.06131,-0.049042,-0.036804,-0.024536,-0.012268,0,0.012268
};

void setup();//use this to do any initialisation if you want.

void play(double *channels);//run dac! 

maxiOsc::maxiOsc(){
	phase = 0.0;
//	memset(phases,0,500);
//	memset(freqs,0,500);
}

double maxiOsc::noise() {
	//always the same unless you seed it.
	float r = rand()/(float)RAND_MAX;
	output=r*2-1;
	return(output);
}

double maxiOsc::sinewave(double frequency) {
	output=sin (phase*(TWO_PI));
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(sampleRate/(frequency)));
	return(output);
	
}

double maxiOsc::sinebuf4(double frequency) {
	double remainder;
	double a,b,c,d,a1,a2,a3;
	phase += 512./(sampleRate/(frequency));
	if ( phase >= 511 ) phase -=512;
	remainder = phase - floor(phase);
	
	if (phase==0) {
		a=sineBuffer[(long) 512];
		b=sineBuffer[(long) phase];
		c=sineBuffer[(long) phase+1];
		d=sineBuffer[(long) phase+2];
		
	} else {
		a=sineBuffer[(long) phase-1];
		b=sineBuffer[(long) phase];
		c=sineBuffer[(long) phase+1];
		d=sineBuffer[(long) phase+2];
		
	}
	
	a1 = 0.5f * (c - a);
	a2 = a - 2.5 * b + 2.f * c - 0.5f * d;
	a3 = 0.5f * (d - a) + 1.5f * (b - c);
	output = double (((a3 * remainder + a2) * remainder + a1) * remainder + b);
	return(output);
}

double maxiOsc::sinebuf(double frequency) {
	double remainder;
 	phase += 512./(sampleRate/(frequency*chandiv));
	if ( phase >= 511 ) phase -=512;
	remainder = phase - floor(phase);
	output = (double) ((1-remainder) * sineBuffer[1+ (long) phase] + remainder * sineBuffer[2+(long) phase]);
	return(output);
}

double maxiOsc::coswave(double frequency) {
	output=cos (phase*(TWO_PI));
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(sampleRate/(frequency)));
	return(output);
	
}

double maxiOsc::phasor(double frequency) {
	output=phase;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(sampleRate/(frequency)));
	return(output);
} 

double maxiOsc::square(double frequency) {
	if (phase<0.5) output=-1;
	if (phase>0.5) output=1;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(sampleRate/(frequency)));
	return(output);
}

double maxiOsc::pulse(double frequency, double duty) {
	if (duty<0.) duty=0;
	if (duty>1.) duty=1;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(sampleRate/(frequency)));
	if (phase<duty) output=-1.;
	if (phase>duty) output=1.;
	return(output);
}

double maxiOsc::phasor(double frequency, double startphase, double endphase) {
	output=phase;
	if (phase<startphase) {
		phase=startphase;
	}
	if ( phase >= endphase ) phase = startphase;
	phase += ((endphase-startphase)/(sampleRate/(frequency)));
	return(output);
}


double maxiOsc::saw(double frequency) {
	
	output=phase;
	if ( phase >= 1.0 ) phase -= 2.0;
	phase += (1./(sampleRate/(frequency)));
	return(output);
	
} 

double maxiOsc::triangle(double frequency, double phase) {
	output=tri*2;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(sampleRate/(frequency*chandiv)));
	if (phase <= 0.5 ) {
		tri = phase;
	} else {	
		tri =(1-phase);
	}
	return(output);
	
} 
void maxiOsc::setSampleRate(int rate){
	sampleRate = rate;
}

ofxSynth::ofxSynth(){

		float currentFrequency,startFrequency, targetFrequency, currentAmp, noteTime;
		float sustain, gain, cutoff, res, filterMod, portamento;
		int filterMode, waveMode, sampleRate;


	currentFrequency = startFrequency = targetFrequency = 440;
	
	setFrequency(440);
	currentAmp = 0;
	noteTime = 0;
	usesEnv = false;
	ampMode = OFXSYNTHONESHOT;
	gain = 0.1;
	sustain = 0.5;
	modEnv.setADSR(44100*0.01, 44100*0.2, 0.0, 44100.0*0.1);
	filter.setup();
	setFilter(1.0, 0.01);
	setFilterLowPass();
	waveMode = 0;
	
}

void ofxSynth::audioRequested( float* buffer, int numFrames, int numChannels ){
	// fill the required inputs
	float envBuffer[numFrames];
	float modEnvBuffer[numFrames];
	wave.setSampleRate(sampleRate);
	
	env.audioRequested(envBuffer, numFrames, 1);
	modEnv.audioRequested(modEnvBuffer, numFrames, 1); // we are only going to update once per buffer
	for (int i = 0; i<numFrames; i++) {
		noteTime++;
		currentFrequency = ofLerp(startFrequency, targetFrequency, fmin((float)noteTime, portamento+1)/(float)(portamento+20));
		if (ampMode == OFXSYNTHONESHOT) {
			currentAmp = 1;
		}else if(ampMode == OFXSYNTHADR){
			currentAmp = envBuffer[i];
		}
		if (currentAmp > 1) {
			currentAmp = 1;
		}
		
		// load the proper waveform
		switch (waveMode) {
			case 0:
				buffer[i*numChannels] = wave.square(currentFrequency);
				break;
			case 1:
				buffer[i*numChannels] = wave.triangle(currentFrequency, 0);
				break;
			case 2:
				buffer[i*numChannels] = wave.saw(currentFrequency);
				break;
			default:
				buffer[i*numChannels] = 0;
				break;
		}
		buffer[i*numChannels] *= currentAmp;
		for (int j=1; j<numChannels; j++) {
			buffer[i*numChannels+j]=buffer[i*numChannels];
		}
	}
	if (filterMode != 0) {
		//filter.setCutoff(modEnvBuffer[0]*cutoff);
		filter.process(buffer, buffer, numFrames, numChannels, numChannels);
	}
}

void ofxSynth::trigger(){
	if (ampMode == OFXSYNTHONESHOT) {
		currentAmp = 1;
	}else if(ampMode == OFXSYNTHADR){
		env.trigger();
		env.release();
		modEnv.trigger();
		modEnv.release();
	}
}
void ofxSynth::setFilter(float _cutoff, float _res){
	cutoff = _cutoff;
	res = _res;
	filter.setCutoff(_cutoff);
	filter.setRes(_res);
}
void ofxSynth::setFrequency(float freq){
	startFrequency = currentFrequency;
	targetFrequency = freq;
	noteTime = 0;
}
void ofxSynth::setFrequencyMidiNote(float note){
	setFrequency(440.0*pow(2.0, (note-60.0)/12.0));
}
void ofxSynth::setSampleRate( int rate )
{
	sampleRate = rate;
}

/* ----------- */