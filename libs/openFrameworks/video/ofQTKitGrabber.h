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

	    bool            setup(int w, int h);
	    //initGrabberNoPreview if you want to only use this grabber for recording
	    bool            initGrabberWithoutPreview();
		bool            isFrameNew() const;
		void            update();
		float           getWidth() const;
		float           getHeight() const;
        ofPixels&	    getPixels();
        const ofPixels& getPixels() const;
		void            setVerbose(bool bTalkToMe);

		vector <ofVideoDevice> listDevices() const;
		const vector <string> & listAudioDevices() const;
		const vector <string> & listVideoDevices() const;

		bool            initRecording();
		const vector <string> & listVideoCodecs() const;
		const vector <string> & listAudioCodecs() const;
		void            setVideoCodec(string videoCodecIDString);
		void            setAudioCodec(string audioCodecIDString);
		void            setUseAudio(bool bUseAudio);
		void            startRecording(string filePath);
		void            stopRecording();
		bool            isRecording() const;
	    bool            isRecordingReady() const;
        bool            isReady() const;
        bool            isInitialized() const;

		void            close();

		void            setDeviceID(int videoDeviceID);
		void            setDeviceID(string videoDeviceIDString);
		int             getDeviceID() const;

		void            setVideoDeviceID(int videoDeviceID);
		void            setVideoDeviceID(string videoDeviceIDString);
		int             getVideoDeviceID() const;

		void            setAudioDeviceID(int audioDeviceID);
		void            setAudioDeviceID(string audioDeviceIDString);
		int             getAudioDeviceID() const;

		void            setDesiredFrameRate(int framerate);
		bool            setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat   getPixelFormat() const;
	    bool			hasPreview() const;
		void            videoSettings();

		ofEvent<ofVideoSavedEventArgs> videoSavedEvent;
	protected:

		bool confirmInit() const;
		ofPixelFormat pixelFormat;
	    ofPixels pixels;
    
		mutable vector <string>  videoDeviceVec;
		mutable vector <string>  audioDeviceVec;
		mutable vector <string>  videoCodecsVec;
		mutable vector <string>  audioCodecsVec;

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


