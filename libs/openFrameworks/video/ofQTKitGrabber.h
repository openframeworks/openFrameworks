// Copyright (c) 2012 openFrameworks team
// openFrameworks is released under the MIT License. See libs/_Licence.txt
#pragma once

#include "ofMain.h"

//using this #ifdef lets this .h file be included in cpp files without throwing errors
//but when included in .m files it works correctly.  another useful trick for mixing oF/ObjC
#ifdef __OBJC__
	@class QTKitVideoGrabber;
#endif

class ofQTKitGrabber : public ofBaseVideoGrabber {
	public:
		ofQTKitGrabber();
		~ofQTKitGrabber();

		bool            initGrabber(int w, int h);
		bool            isFrameNew();
		void            update();
		float           getWidth();
		float           getHeight();
		unsigned char * getPixels();
		ofPixelsRef     getPixelsRef();
		void            setVerbose(bool bTalkToMe);
		void            setUseTexture(bool bUse);

		// [added by gameover]
		void            initRecording();
		bool            initGrabberWithoutPreview(); // used to init with no preview/textures etc
		vector <string> & listVideoCodecs();
		vector <string> & listAudioCodecs();
		void            setVideoCodec(string videoCodecIDString);
		void            setAudioCodec(string audioCodecIDString);
		void            setUseAudio(bool bUseAudio);
		void            startRecording(string filePath);
		void            stopRecording();
		bool            isRecording();
		bool            isReady();

		void            listDevices(); // would be better if this returned a vector of devices too, but requires updating base class
		vector <string> & listAudioDevices();
		vector <string> & listVideoDevices();

		void            close();

		void            setDeviceID(int videoDeviceID);
		void            setDeviceID(string videoDeviceIDString);
		int             getDeviceID();

		void            setVideoDeviceID(int videoDeviceID);
		void            setVideoDeviceID(string videoDeviceIDString);
		int             getVideoDeviceID();

		void            setAudioDeviceID(int audioDeviceID);
		void            setAudioDeviceID(string audioDeviceIDString);
		int             getAudioDeviceID();

		void            setDesiredFrameRate(int framerate){ ofLog(OF_LOG_WARNING, "ofQTKitGrabber -- Cannot specify framerate."); }
		void            setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat   getPixelFormat();

		void            videoSettings();

	protected:

		bool confirmInit();
		ofPixelFormat pixelFormat;

		vector <string>  videoDeviceVec;
		vector <string>  audioDeviceVec;
		vector <string>  videoCodecsVec;
		vector <string>  audioCodecsVec;

		int videoDeviceID;
		int audioDeviceID;
		string videoCodecIDString;
		string audioCodecIDString;

		bool isInited;
		bool bUseAudio;

		#ifdef __OBJC__
			QTKitVideoGrabber * grabber; //only obj-c needs to know the type of this protected var
		#else
			void * grabber;
		#endif
};

