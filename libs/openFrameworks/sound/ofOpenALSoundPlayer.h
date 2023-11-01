#pragma once

#include "ofConstants.h"

#ifdef OF_SOUND_PLAYER_OPENAL
#include "ofSoundBaseTypes.h"
#include "ofThread.h"

typedef unsigned int ALuint;

#include "kiss_fft.h"
#include "kiss_fftr.h"
#include <sndfile.h>

#ifdef OF_USING_MPG123
	typedef struct mpg123_handle_struct mpg123_handle;
#endif

class ofEventArgs;

//		TO DO :
//		---------------------------
// 		-fft via fmod, as in the last time...
// 		-close fmod if it's up
//		-loadSoundForStreaming(char * fileName);
//		---------------------------

// 		interesting:
//		http://www.compuphase.com/mp3/mp3loops.htm


// ---------------------------------------------------------------------------- SOUND SYSTEM FMOD

// --------------------- global functions:
void ofFmodSoundStopAll();
void ofFmodSoundSetVolume(float vol);
void ofOpenALSoundUpdate();						// calls FMOD update.
float * ofFmodSoundGetSpectrum(int nBands);		// max 512...


// --------------------- player functions:
class ofOpenALSoundPlayer : public ofBaseSoundPlayer, public ofThread {

	public:

		ofOpenALSoundPlayer();
		virtual ~ofOpenALSoundPlayer();

        bool load(const of::filesystem::path& fileName, bool stream = false);
		void unload();
		void play();
		void stop();

		void setVolume(float vol);
		void setPan(float vol); // -1 to 1
		void setSpeed(float spd);
		void setPaused(bool bP);
		void setLoop(bool bLp);
		void setMultiPlay(bool bMp);
		void setPosition(float pct); // 0 = start, 1 = end;
		void setPositionMS(int ms);


		float getPosition() const;
		int getPositionMS() const;
		bool isPlaying() const;
		float getSpeed() const;
		float getPan() const;
        float getVolume() const;
		bool isPaused() const;
		bool isLoaded() const;

		static void initialize();
		static void close();

		float * getSpectrum(int bands);

		static float * getSystemSpectrum(int bands);

	protected:
		void threadedFunction();

	private:
		friend void ofOpenALSoundUpdate();
		void update(ofEventArgs & args);
		void initFFT(int bands);
		float * getCurrentBufferSum(int size);

		static void createWindow(int size);
		static void runWindow(std::vector<float> & signal);
		static void initSystemFFT(int bands);

        bool sfReadFile(const of::filesystem::path& path,std::vector<short> & buffer,std::vector<float> & fftAuxBuffer);
        bool sfStream(const of::filesystem::path& path,std::vector<short> & buffer,std::vector<float> & fftAuxBuffer);
#ifdef OF_USING_MPG123
        bool mpg123ReadFile(const of::filesystem::path& path,std::vector<short> & buffer,std::vector<float> & fftAuxBuffer);
        bool mpg123Stream(const of::filesystem::path& path,std::vector<short> & buffer,std::vector<float> & fftAuxBuffer);
#endif

        bool readFile(const of::filesystem::path& fileName,std::vector<short> & buffer);
        bool stream(const of::filesystem::path& fileName, std::vector<short> & buffer);

		bool isStreaming;
		bool bMultiPlay;
		bool bLoop;
		bool bLoadedOk;
		bool bPaused;
		float pan; // 0 - 1
		float volume; // 0 - 1
		float internalFreq; // 44100 ?
		float speed; // -n to n, 1 = normal, -1 backwards
		unsigned int length; // in samples;

		static std::vector<float> window;
		static float windowSum;

		int channels;
		float duration; //in secs
		int samplerate;
		std::vector<ALuint> buffers;
		std::vector<ALuint> sources;

		// fft structures
		std::vector<std::vector<float> > fftBuffers;
		kiss_fftr_cfg fftCfg;
		std::vector<float> windowedSignal;
		std::vector<float> bins;
		std::vector<kiss_fft_cpx> cx_out;


		static kiss_fftr_cfg systemFftCfg;
		static std::vector<float> systemWindowedSignal;
		static std::vector<float> systemBins;
		static std::vector<kiss_fft_cpx> systemCx_out;

		SNDFILE* streamf;
		size_t stream_samples_read;
#ifdef OF_USING_MPG123
		mpg123_handle * mp3streamf;
		int stream_encoding;
#endif
		int mp3_buffer_size;
		int stream_subformat;
		double stream_scale;
		std::vector<short> buffer;
		std::vector<float> fftAuxBuffer;

		bool stream_end;
};

#endif
