#pragma once

class ofSoundBuffer;


/// \brief A base class representing a sound input stream.
class ofBaseSoundInput{
	public:
		/// \brief Destroy the ofBaseSoundInput.
		virtual ~ofBaseSoundInput() {};

		/// \brief Receive an audio buffer.
		/// \param buffer An audio buffer.
		virtual void audioIn( ofSoundBuffer& buffer );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioIn(ofSoundBuffer& buffer) instead.
		virtual void audioIn( float * input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioIn(ofSoundBuffer& buffer) instead.
		virtual void audioIn( float * input, int bufferSize, int nChannels );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioIn(ofSoundBuffer& buffer) instead.
		virtual void audioReceived( float * input, int bufferSize, int nChannels ){}
};


/// \brief A base class representing a sound output stream.
class ofBaseSoundOutput{
	public:
		/// \brief Destroy the ofBaseSoundOutput.
		virtual ~ofBaseSoundOutput() {};

		/// \brief Output an audio buffer.
		/// \param buffer An audio buffer.
		virtual void audioOut( ofSoundBuffer& buffer );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioOut(ofSoundBuffer& buffer) instead.
		virtual void audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount  );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioOut(ofSoundBuffer& buffer) instead.
		virtual void audioOut( float * output, int bufferSize, int nChannels );

		/// \deprecated This legacy method is deprecated and will be removed.
		/// Use void audioOut(ofSoundBuffer& buffer) instead.
		virtual void audioRequested( float * output, int bufferSize, int nChannels ){}
};
