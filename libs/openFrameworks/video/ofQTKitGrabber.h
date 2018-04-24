// Copyright (c) 2012 openFrameworks team
// openFrameworks is released under the MIT License. See libs/_Licence.txt
#pragma once

#include "ofConstants.h"
#include "ofEvents.h"
#include "ofVideoBaseTypes.h"
#include "ofPixels.h"

#ifdef OF_VIDEO_CAPTURE_QTKIT

#if !defined(MAC_OS_X_VERSION_10_12) || MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_12

//using this #ifdef lets this .h file be included in cpp files without throwing errors
//but when included in .m files it works correctly.  another useful trick for mixing oF/ObjC
#ifdef __OBJC__
	@class QTKitVideoGrabber;
#endif

class ofVideoSavedEventArgs : public ofEventArgs {
  public:
	std::string videoPath;
	ofBaseVideoGrabber* grabber;
	std::string error; //"" if there is no error
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

		std::vector <ofVideoDevice> listDevices() const;
		const std::vector <std::string> & listAudioDevices() const;
		const std::vector <std::string> & listVideoDevices() const;

		bool            initRecording();
		const std::vector <std::string> & listVideoCodecs() const;
		const std::vector <std::string> & listAudioCodecs() const;
		void            setVideoCodec(std::string videoCodecIDString);
		void            setAudioCodec(std::string audioCodecIDString);
		void            setUseAudio(bool bUseAudio);
		void            startRecording(std::string filePath);
		void            stopRecording();
		bool            isRecording() const;
	    bool            isRecordingReady() const;
        bool            isReady() const;
        bool            isInitialized() const;

		void            close();

		void            setDeviceID(int videoDeviceID);
		void            setDeviceID(std::string videoDeviceIDString);
		int             getDeviceID() const;

		void            setVideoDeviceID(int videoDeviceID);
		void            setVideoDeviceID(std::string videoDeviceIDString);
		int             getVideoDeviceID() const;

		void            setAudioDeviceID(int audioDeviceID);
		void            setAudioDeviceID(std::string audioDeviceIDString);
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

		mutable std::vector <std::string>  videoDeviceVec;
		mutable std::vector <std::string>  audioDeviceVec;
		mutable std::vector <std::string>  videoCodecsVec;
		mutable std::vector <std::string>  audioCodecsVec;

		int videoDeviceID;
		int audioDeviceID;
		std::string videoCodecIDString;
		std::string audioCodecIDString;

		bool isInited;
		bool bUseAudio;
	    bool bPreview;
		#ifdef __OBJC__
			QTKitVideoGrabber * grabber; //only obj-c needs to know the type of this protected var
		#else
			void * grabber;
		#endif
};

#endif
#endif
