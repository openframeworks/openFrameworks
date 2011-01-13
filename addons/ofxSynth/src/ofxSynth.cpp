#include "ofxSynth.h"

#define SAMPLE_RATE 44100.0

ofxSynth::ofxSynth(){
	
	currentFrequency = 440;
	currentAmp = 0;
	usesEnv = false;
	ampMode = OFXSYNTHONESHOT;
	gain = 0.1;
	sustain = 0.5;
		
	filter.setup();
	filter.setRes(0.0);
	filter.setCutoff(1.0);
	setFilterMode(1);
	waveMode = 0;
}

void ofxSynth::audioRequested( float* buffer, int numFrames, int numChannels ){
	// fill the required inputs
	float envBuffer[numFrames];
	env.audioRequested(envBuffer, numFrames, 1);

	for (int i = 0; i<numFrames; i++) {
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
		filter.process(buffer, buffer, numFrames, numChannels, numChannels);
	}
}

void ofxSynth::trigger(){
	if (ampMode == OFXSYNTHONESHOT) {
		currentAmp = 1;
	}else if(ampMode == OFXSYNTHADR){
		env.trigger();
		env.release();
	}
}
void ofxSynth::setFilter(float _cutoff, float _res){
	filter.setCutoff(_cutoff);
	filter.setRes(_res);
}
void ofxSynth::setFrequency(float freq){
	currentFrequency = freq;
}
void ofxSynth::setFrequencyMidiNote(float note){
	setFrequency(440.0*pow(2.0, (note-60.0)/12.0));
}
void ofxSynth::setFilterMode(int mode){
	filterMode = mode;
	if (filterMode < 0) {
		filter.lowPass = filterMode == 1 ? true : false;
	}
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