#include "ofxSynthWaveWriter.h"

bool ofxSynthWaveWriter::startWriting(string filename){
	sample_count_ = 0;
	rate = sampleRate;
	buf_pos = wav_header_size;
	writing = true;
	numChannels = 2;
	buf = (unsigned char*) malloc( buf_size * sizeof *buf );
	if ( !buf ){
		writing = false;
		ofLogError("ofxSynthWaveWriter") << "startWriting(): cannot write file, out of memory";
		return false;
	}
	
	file = fopen( ofToDataPath(filename).c_str(), "wb" );
	if ( !file ){
		writing = false;
		ofLogError("ofxSynthWaveWriter") << "startWriting(): couldn't open file";
		return false;
	}
	setvbuf( file, 0, _IOFBF, 32 * 1024L );
	return true;
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
	ofxSoundEffectPassthrough::process(input, output, numFrames, numInChannels, numOutChannels);
}

void ofxSynthWaveWriter::flush(){
	if ( buf_pos && !fwrite( buf, buf_pos, 1, file ) ){
		writing = false;
		ofLogError("ofxSynthWaveWriter") << "flush(): couldn't write to file";
	}
	buf_pos = 0;
}

// completes writing the wave file to disk
void ofxSynthWaveWriter::stopWriting(){
	if ( file )
	{
		// finish writing any remaining files
		flush();
		// generate header
		long ds = sample_count_ * sizeof (char*);
		long rs = wav_header_size - 8 + ds;
		int frame_size = numChannels * sizeof (char*);
		long bps = rate * frame_size;
		writing = false;
		unsigned char header [wav_header_size] =
		{
			'R','I','F','F',
			(unsigned char)(rs),    (unsigned char)(rs>>8),      // length of rest of file
			(unsigned char)(rs>>16),(unsigned char)(rs>>24),
			'W','A','V','E',
			'f','m','t',' ',
			0x10,0,0,0,                                          // size of fmt chunk
			1,0,                                                 // uncompressed format
			(unsigned char)(numChannels),0,                      // channel count
			(unsigned char)(rate),    (unsigned char)(rate>>8),  // sample rate
			(unsigned char)(rate>>16),(unsigned char)(rate>>24),
			(unsigned char)(bps),     (unsigned char)(bps>>8),   // bytes per second
			(unsigned char)(bps>>16), (unsigned char)(bps>>24),
			(unsigned char)(frame_size),0,                       // bytes per sample frame
			16,0,                                                // bits per sample
			'd','a','t','a',
			(unsigned char)(ds),(unsigned char)(ds>>8),(unsigned char)(ds>>16),(unsigned char)(ds>>24) // size of sample data
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
