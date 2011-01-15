#pragma once

class ofxSynthSampler : public ofxSynth {
	public:
		ofxSynthSampler()	{direction=1;inPoint=0.0;outPoint=1.0;};
		void				loadFile(string file);
		
		void				trigger();
		void				setFrequencyMidiNote(float note);
		void				setFrequencySyncToLength(int length);
		void				setDirectionForward(){direction = 1;};
		void				setDirectionBackward(){direction = -1;};
		void				setLoopPoints(float i, float o);

		void				setSampleRate(int rate);
		void				audioRequested( float* buffer, int numFrames, int numChannels );

	private:
		int					sampleRate, direction;
		float				inPoint, outPoint;
		ofxSynthSample		sample;
};

class ofxSynthSample  {
	
private:
	string 	myPath;
	int 	myChunkSize;
	int	mySubChunk1Size;
	short 	myFormat;
	int   	myByteRate;
	short 	myBlockAlign;
	short 	myBitsPerSample;
	int	myDataSize;
	double speed;
	double output;
	
public:
	double position;
	short 	myChannels;
	int   	mySampleRate;
	long length;
	void getLength();

	char* 	myData;
	
	// get/set for the Path property

	~ofxSynthSample()
	{
		delete myData;
		myChunkSize = NULL;
		mySubChunk1Size = NULL;
		myFormat = NULL;
		myChannels = NULL;
		mySampleRate = NULL;
		myByteRate = NULL;
		myBlockAlign = NULL;
		myBitsPerSample = NULL;
		myDataSize = NULL;
	}
	
	bool load(string fileName);

	// read a wav file into this class
	bool read()
	{
		bool result;
		ifstream inFile( myPath.c_str(), ios::in | ios::binary);
		result = inFile;
		
		if (inFile) {
			//printf("Reading wav file...\n"); // for debugging only
			
			inFile.seekg(4, ios::beg);
			inFile.read( (char*) &myChunkSize, 4 ); // read the ChunkSize
			
			inFile.seekg(16, ios::beg);
			inFile.read( (char*) &mySubChunk1Size, 4 ); // read the SubChunk1Size
			
			//inFile.seekg(20, ios::beg);
			inFile.read( (char*) &myFormat, sizeof(short) ); // read the file format.  This should be 1 for PCM
			
			//inFile.seekg(22, ios::beg);
			inFile.read( (char*) &myChannels, sizeof(short) ); // read the # of channels (1 or 2)
			
			//inFile.seekg(24, ios::beg);
			inFile.read( (char*) &mySampleRate, sizeof(int) ); // read the samplerate
			
			//inFile.seekg(28, ios::beg);
			inFile.read( (char*) &myByteRate, sizeof(int) ); // read the byterate
			
			//inFile.seekg(32, ios::beg);
			inFile.read( (char*) &myBlockAlign, sizeof(short) ); // read the blockalign
			
			//inFile.seekg(34, ios::beg);
			inFile.read( (char*) &myBitsPerSample, sizeof(short) ); // read the bitspersample
			
			//ignore any extra chunks
			char chunkID[4]="";
			int filePos = 36;
			bool found = false;
			while(!found && !inFile.eof()) {
				inFile.seekg(filePos, ios::beg);
				inFile.read((char*) &chunkID, sizeof(char) * 4);
				inFile.seekg(filePos + 4, ios::beg);
				inFile.read( (char*) &myDataSize, sizeof(int) ); // read the size of the data
				filePos += 8;
				if (strcmp(chunkID,"data") == 0) {
					found = true;
				}else{
					filePos += myDataSize;
				}
			}
			
			
			
			// read the data chunk
			myData = (char*) malloc(myDataSize * sizeof(char));
			inFile.seekg(filePos, ios::beg);
			inFile.read(myData, myDataSize);
			length=myDataSize*(0.5/myChannels);
			
			inFile.close(); // close the input file
		}
		
		
		return result; // this should probably be something more descriptive
	}
	
	double play4(double frequency, double start, double end, int sampleRate);
	
	// return a printable summary of the wav file
	char *getSummary()
	{
		char *summary = new char[250];
		sprintf(summary, " Format: %d\n Channels: %d\n SampleRate: %d\n ByteRate: %d\n BlockAlign: %d\n BitsPerSample: %d\n DataSize: %d\n", myFormat, myChannels, mySampleRate, myByteRate, myBlockAlign, myBitsPerSample, myDataSize);
		std::cout << myDataSize;
		return summary;
	}
};