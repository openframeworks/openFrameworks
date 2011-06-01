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
void ofxSynthSampler::audioOut( float* buffer, int numFrames, int numChannels,long unsigned long tickCount  ){
	sample.setSampleRate(sampleRate);
	for (int i = 0; i < numFrames; i++){
		buffer[i*numChannels] = sample.play4(currentFrequency, inPoint*sample.length, outPoint*sample.length);
	}
}
void ofxSynthSampler::setFrequencyMidiNote(float note){
	currentFrequency = pow(2.0, (note-60.0)/12.0f);
	
}
void ofxSynthSampler::setFrequencySyncToLength(int length){
	currentFrequency = sample.length/(float)length;
}
void ofxSynthSampler::loadFile(string file){
	printf("sampleload test: %i\n",sample.load(ofToDataPath(file)));
	printf("Summary:\n%s", sample.getSummary());
}

/* ============================ */
ofxSynthSample::ofxSynthSample() {
	position = 0;
	myChannels = 2;
	mySampleRate = 44100;
	length = 0;

}

bool ofxSynthSample::load(string fileName) {
	myPath = fileName;
	return read();
}
bool ofxSynthSample::read(){
	bool result;
	ifstream inFile( myPath.c_str(), ios::in | ios::binary);
	result = inFile;

	if (inFile) {
		printf("Reading wav file...\n"); // for debugging only
		
		inFile.seekg(4, ios::beg);
		inFile.read( (char*) &myChunkSize, 4 ); // read the ChunkSize
		
		inFile.seekg(16, ios::beg);
		inFile.read( (char*) &mySubChunk1Size, 4 ); // read the SubChunk1Size
		
		inFile.read( (char*) &myFormat, sizeof(short) ); // read the file format.  This should be 1 for PCM
		
		inFile.read( (char*) &myChannels, sizeof(short) ); // read the # of channels (1 or 2)
		
		inFile.read( (char*) &mySampleRate, sizeof(int) ); // read the samplerate
		
		inFile.read( (char*) &myByteRate, sizeof(int) ); // read the byterate
		
		inFile.read( (char*) &myBlockAlign, sizeof(short) ); // read the blockalign
		
		inFile.read( (char*) &myBitsPerSample, sizeof(short) ); // read the bitspersample
		
		//ignore any extra chunks
		char chunkID[4]="";
		int filePos = 36;
		bool found = false;
		while(!found && !inFile.eof()) {
			cout << "count" << endl;
			inFile.seekg(filePos, ios::beg);
			inFile.read((char*) &chunkID, sizeof(char) * 4);
			inFile.seekg(filePos + 4, ios::beg);
			inFile.read( (char*) &myDataSize, sizeof(int) ); // read the size of the data
			filePos += 8;
			if (chunkID[0] == 'd' && chunkID[1] == 'a' && chunkID[2] == 't' && chunkID[3] == 'a') {
				printf("found at: %i\n", filePos);
				found = true;
			}else{
				printf("chunkid: %s\n", chunkID);
				filePos += myDataSize;
			}
		}
		// clear the error bits that were set,
		// so tha we can continue to perform operations on the file
		inFile.clear();

		
		
		// read the data chunk
		myData = (char*) malloc(myDataSize * sizeof(char));
		printf("myDataSize %i\n", myDataSize);
		
		inFile.seekg(filePos, ios::beg);
		printf("filePos:%i\n", filePos);
		if ( (inFile.rdstate() & ifstream::eofbit ) != 0 )
			cerr << "error 1'\n";

		inFile.read(myData, myDataSize);
		
		if ( (inFile.rdstate() & ifstream::eofbit ) != 0 )
			cerr << "error 2'\n";

		length=myDataSize*(0.5/myChannels);
		
		inFile.close(); // close the input file
	}


	return result; // this should probably be something more descriptive
}
char * ofxSynthSample::getSummary()
{
	char *summary = new char[250];
	sprintf(summary, " Format: %d\n Channels: %d\n SampleRate: %d\n ByteRate: %d\n BlockAlign: %d\n BitsPerSample: %d\n DataSize: %d\n", myFormat, myChannels, mySampleRate, myByteRate, myBlockAlign, myBitsPerSample, myDataSize);
	cout << myDataSize;
	return summary;
}
void ofxSynthSample::getLength() {
	length=myDataSize*0.5;	
}
//better cubic inerpolation. Cobbled together from various (pd externals, yehar, other places).
//better cubic inerpolation. Cobbled together from various (pd externals, yehar, other places).
double ofxSynthSample::play4(double frequency, double start, double end) {
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
double ofxSynthSample::play(double frequency, double start, double end) {
	double remainder;
//	long length=myDataSize;
//	printf("position: %f\n", position);
	if (end>=length) end=length-1;
	long a,b;
	short* buffer = (short *)myData;
	if (frequency >0.) {
		if (position<start) {
			position=start;
		}
		
		if ( position >= end ) position = start;
		position += ((end-start)/(sampleRate/(frequency)));
		remainder = position - floor(position);
		long pos = floor(position);
		if (pos+1<length) {
			a=pos+1;

		}
		else {
			a=pos-1;
		}
		if (pos+2<length) {
			b=pos+2;
		}
		else {
			b=length-1;
		}
	
		output = (double) ((1-remainder) * buffer[a] +
							   remainder * buffer[b])/32767.0;//linear interpolation
		// output = ofRandom(-1.0, 1.0);
	} else {
		frequency=frequency-(frequency+frequency);
		if ( position <= start ) position = end;
		position -= ((end-start)/(sampleRate/(frequency)));
		remainder = position - floor(position);
		long pos = floor(position);
			if (pos-1>=0) {
				a=pos-1;
			}
			else {
				a=0;
			}
		if (pos-2>=0) {
		b=pos-2;
		}
		else {
			b=0;
		}		
		output = (double) ((-1-remainder) * buffer[a] +
						   remainder * buffer[b])/32767.0;//linear interpolation
		
	}
	
	return(output);
}
void ofxSynthSample::setSampleRate(int rate){
	sampleRate = rate;
}
