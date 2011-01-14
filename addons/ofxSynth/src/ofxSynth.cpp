#include "ofxSynth.h"

int channels=2;
int samplerate=44100; 
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
	output=sin (phase*(TWOPI));
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency)));
	return(output);
	
}

double maxiOsc::sinebuf4(double frequency) {
	double remainder;
	double a,b,c,d,a1,a2,a3;
	phase += 512./(samplerate/(frequency));
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
 	phase += 512./(samplerate/(frequency*chandiv));
	if ( phase >= 511 ) phase -=512;
	remainder = phase - floor(phase);
	output = (double) ((1-remainder) * sineBuffer[1+ (long) phase] + remainder * sineBuffer[2+(long) phase]);
	return(output);
}

double maxiOsc::coswave(double frequency) {
	output=cos (phase*(TWOPI));
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency)));
	return(output);
	
}

double maxiOsc::phasor(double frequency) {
	output=phase;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency)));
	return(output);
} 

double maxiOsc::square(double frequency) {
	if (phase<0.5) output=-1;
	if (phase>0.5) output=1;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency)));
	return(output);
}

double maxiOsc::pulse(double frequency, double duty) {
	if (duty<0.) duty=0;
	if (duty>1.) duty=1;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency)));
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
	phase += ((endphase-startphase)/(samplerate/(frequency)));
	return(output);
}


double maxiOsc::saw(double frequency) {
	
	output=phase;
	if ( phase >= 1.0 ) phase -= 2.0;
	phase += (1./(samplerate/(frequency)));
	return(output);
	
} 

double maxiOsc::triangle(double frequency, double phase) {
	output=tri*2;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency*chandiv)));
	if (phase <= 0.5 ) {
		tri = phase;
	} else {	
		tri =(1-phase);
	}
	return(output);
	
} 

ofxSynth::ofxSynth(){
	
	currentFrequency = 440;
	currentAmp = 0;
	usesEnv = false;
	ampMode = OFXSYNTHONESHOT;
	gain = 0.1;
	sustain = 0.5;
	modEnv.setADSR(44100*0.01, 44100*0.2, 0.0, 44100.0*0.1);
	filter.setup();
	setFilter(1.0, 0.0);
	setFilterLowPass();
	waveMode = 0;
	
}

void ofxSynth::audioRequested( float* buffer, int numFrames, int numChannels ){
	// fill the required inputs
	float envBuffer[numFrames];
	float modEnvBuffer[numFrames];
	//wave.setSampleRate(sampleRate);
	
	env.audioRequested(envBuffer, numFrames, 1);
	modEnv.audioRequested(modEnvBuffer, numFrames, 1); // we are only going to update once per buffer
	for (int i = 0; i<numFrames; i++) {
		currentFrequency = ofLerp(startFrequency, targetFrequency, fmin((float)noteTime, portamento+1)/(float)(portamento+20));
		noteTime++;
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
void ofxSynthADSR::trigger(){
	offset = 0;
	noteOn = true;
}
void ofxSynthADSR::release(){
	noteOn = false;
}
void ofxSynthADSR::audioRequested( float* buffer, int numFrames, int numChannels ){
	for (int i = 0; i < numFrames; i++){
		if(offset<a){ // attack
			buffer[i*numChannels] = ((float)offset)/a;
			offset++;
		}else if (offset>a&&offset<a+d) { // decay
			buffer[i*numChannels] = ofLerp(1.0, s, ((float)offset-a)/d);
			offset++;
		}else if(noteOn){ // sustain
			buffer[i*numChannels] = s;
		}else if(offset<a+d+r){ // release
			buffer[i*numChannels] = ofLerp(s, 0.0, (float)(offset-a-d)/(float)r);
			offset++;
		}else {
			buffer[i*numChannels] = 0;
		}

		// copy to the other channels that are being requested
		for (int j = 1; j<numChannels; j++) {
			buffer[i*numChannels+j] = buffer[i*numChannels];
		}
	}
}

/* ----------- */
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

/* ----------- */
bool ofxSynthWaveWriter::startWriting(string filename){
	sample_count_ = 0;
	rate = sampleRate;
	buf_pos = wav_header_size;
	writing = true;
	numChannels = 2;
	buf = (unsigned char*) malloc( buf_size * sizeof *buf );
	if ( !buf ){
		writing = false;
		//TODO: throw an out of memory error
		cout << "cannot write file, out of memory" << endl;
		return false;
	}
	
	file = fopen( ofToDataPath(filename).c_str(), "wb" );
	if ( !file ){
		writing = false;
		//TODO: throw a unable to open file error
		cout << "cannot open file" << endl;
		return false;
	}
	cout << buf_size;
	setvbuf( file, 0, _IOFBF, 32 * 1024L );
}
void ofxSynthWaveWriter::process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels ){
	if (writing) {
		for (int i=0; i<numFrames; i++) {
			for (int j = 0; j<numChannels; j++) {
				if ( buf_pos >= buf_size )
					flush();
				sample_count_ ++;
				long s = (long) (input[i*numInChannels+j] * 0x7FFF); // convert to a integer representation
				if ( (short) s != s )
					s = 0x7FFF - (s >> 24); // clamp to 16 bits
				buf[buf_pos++] = (unsigned char) s;
				buf[buf_pos++] = (unsigned char) (s>>8);
			}
		}
	}
	// this sample writer operates like a passthrough, even when it is writing samples to disk
	ofSoundEffectPassthrough::process(input, output, numFrames, numInChannels, numOutChannels);
}
void ofxSynthWaveWriter::flush(){
	if ( buf_pos && !fwrite( buf, buf_pos, 1, file ) ){
		writing = false;
		//TODO: throw a unable to open file error
		cout << "cannot write to file" << endl;
	}
	buf_pos = 0;
}
void ofxSynthWaveWriter::stopWriting(){
	if ( file )
	{
		flush();
		// generate header
		long ds = sample_count_ * sizeof (char*);
		long rs = wav_header_size - 8 + ds;
		int frame_size = numChannels * sizeof (char*);
		long bps = rate * frame_size;
		cout << rs << endl;
		cout << buf_pos << endl;
		writing = false;
		unsigned char header [wav_header_size] =
		{
			'R','I','F','F',
			rs,rs>>8,           // length of rest of file
			rs>>16,rs>>24,
			'W','A','V','E',
			'f','m','t',' ',
			0x10,0,0,0,         // size of fmt chunk
			1,0,                // uncompressed format
			numChannels,0,       // channel count
			rate,rate >> 8,     // sample rate
			rate>>16,rate>>24,
			bps,bps>>8,         // bytes per second
			bps>>16,bps>>24,
			frame_size,0,       // bytes per sample frame
			16,0,               // bits per sample
			'd','a','t','a',
			ds,ds>>8,ds>>16,ds>>24// size of sample data
			// ...              // sample data
		};
		
		// write header
		fseek( file, 0, SEEK_SET );
		fwrite( header, sizeof header, 1, file );
		
		fclose( file );
		file = 0;
		free( buf );
	}
}