#ifndef _OF_VIDEO_GRABBER
#define _OF_VIDEO_GRABBER

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"


#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	#include "ofQtUtils.h"
#endif

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	#include "videoInput.h"
#endif

#ifdef OF_VIDEO_CAPTURE_UNICAP
	#include "ofUCUtils.h"
#endif



// todo:
// 		QT - callback, via SGSetDataProc - couldn't get this to work yet
// 		image decompress options ala mung...


class ofVideoGrabber{

	public :

		ofVideoGrabber();
		~ofVideoGrabber();

		void 			listDevices();
		bool 			isFrameNew();
		void			grabFrame();
		void			close();
		bool			initGrabber(int w, int h, bool bTexture = true);
		void			videoSettings();
		unsigned char 	* getPixels();
		void 			setVerbose(bool bTalkToMe);
		void			setDeviceID(int _deviceID);
		void 			setUseTexture(bool bUse);
		void 			draw(float x, float y, float w, float h);
		void 			draw(float x, float y);
		int 			width, height;

	protected:

		bool					bChooseDevice;
		int						deviceID;
		bool					bUseTexture;
		ofTexture 				tex;
		bool 					bVerbose;
		bool 					bGrabberInited;
	    unsigned char * 		pixels;
		bool 					bIsFrameNew;

		//--------------------------------- quicktime
		#ifdef OF_VIDEO_CAPTURE_QUICKTIME


			unsigned char * 	offscreenGWorldPixels;	// 32 bit: argb (qt k32ARGBPixelFormat)
			int 				w,h;
			bool 				bHavePixelsChanged;
			GWorldPtr 			videogworld;
			SeqGrabComponent	gSeqGrabber;
			SGChannel 			gVideoChannel;
			Rect				videoRect;
			bool 				bSgInited;
			string				deviceName;
			
			bool				qtInitSeqGrabber();
			bool				qtCloseSeqGrabber();
			bool				qtSelectDevice(int deviceNumber, bool didWeChooseADevice);

			//--------------------------------------------------------------------
			#ifdef TARGET_OSX
			//--------------------------------------------------------------------

			bool					saveSettings();
			bool					loadSettings();

			//--------------------------------------------------------------------
			#endif
			//--------------------------------------------------------------------


		#endif

		//--------------------------------- directshow
		#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
			int 					device;
			videoInput 				VI;

			bool 					bDoWeNeedToResize;

		#endif

		//--------------------------------- linux unicap
		#ifdef OF_VIDEO_CAPTURE_UNICAP
			ofUCUtils				ucGrabber;
		#endif


		//--------------------------------- linux V4L
		// if unicap doesn't work, we keep linux v4l in here....
		// so folks can switch, in ofConstants.h
		#ifdef OF_VIDEO_CAPTURE_V4L
			int 					device;
			char 					dev_name[80];
			bool					bV4LGrabberInited;
		#endif




};




#endif

