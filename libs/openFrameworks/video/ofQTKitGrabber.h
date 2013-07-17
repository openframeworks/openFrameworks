// Copyright (c) 2012 openFrameworks team
// openFrameworks is released under the MIT License. See libs/_Licence.txt
#pragma once

#include "ofMain.h"

//using this #ifdef lets this .h file be included in cpp files without throwing errors
//but when included in .m files it works correctly.  another useful trick for mixing oF/ObjC
#ifdef __OBJC__
	@class QTKitVideoGrabber;
#endif

class ofVideoSavedEventArgs : public ofEventArgs {
  public:
	string videoPath;
	ofBaseVideoGrabber* grabber;
	string error; //"" if there is no error
};

class ofQTKitGrabber : public ofBaseVideoGrabber {
	public:
		ofQTKitGrabber();
		~ofQTKitGrabber();

	    bool            initGrabber(int w, int h);
	    //initGrabberNoPreview if you want to only use this grabber for recording
	    bool            initGrabberWithoutPreview();
		bool            isFrameNew();
		void            update();
		float           getWidth();
		float           getHeight();
		unsigned char * getPixels();
		ofPixelsRef     getPixelsRef();
		void            setVerbose(bool bTalkToMe);

		vector <ofVideoDevice> listDevices(); 
		vector <string> & listAudioDevices();
		vector <string> & listVideoDevices();

		bool            initRecording();
		vector <string> & listVideoCodecs();
		vector <string> & listAudioCodecs();
		void            setVideoCodec(string videoCodecIDString);
		void            setAudioCodec(string audioCodecIDString);
		void            setUseAudio(bool bUseAudio);
		void            startRecording(string filePath);
		void            stopRecording();
		bool            isRecording();
	    bool            isRecordingReady();
		bool            isReady();

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

		void            setDesiredFrameRate(int framerate);
		bool            setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat   getPixelFormat();
	    bool			hasPreview();
		void            videoSettings();

		ofEvent<ofVideoSavedEventArgs> videoSavedEvent;
	protected:

		bool confirmInit();
		ofPixelFormat pixelFormat;
	    ofPixels pixels;
    
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
	    bool bPreview;
		#ifdef __OBJC__
			QTKitVideoGrabber * grabber; //only obj-c needs to know the type of this protected var
		#else
			void * grabber;
		#endif
};


