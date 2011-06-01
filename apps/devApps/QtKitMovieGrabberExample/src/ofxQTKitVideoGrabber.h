/*
 * QTKit / CoreVideo based Video Grabber for openFrameworks 
 *
 * Copyright 2010 (c) James George, http://www.jamesgeorge.org
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 *
 * ofxQTKitVideoGrabber works exactly the same as the standard ofMovieGrabber
 * but uses the QTKit Objective-C Libraries to drive the video display.
 * These libraries are naturally GPU enabled, multi-threaded, as well
 * as supporting more Quicktime capture codecs such as HDV.
 *
 * You will need to add the QTKit.framework and CoreVide.framework
 * to the openFrameworks Xcode project
 *
 * Requires Mac OS 10.5 or greater
 * 
 */

#ifndef _OFXPH_QTKITVIDEOGRABBER
#define _OFXPH_QTKITVIDEOGRABBER

#include "ofMain.h"


//class ofBaseVideoGrabber{
//
//	public :
//
//		ofBaseVideoGrabber();
//		virtual ~ofBaseVideoGrabber();
//		
//		//needs implementing
//		virtual void	listDevices() = 0;		
//		virtual bool	initGrabber(int w, int h) = 0;		
//		virtual void	grabFrame() = 0;
//		virtual bool	isFrameNew() = 0;
//
//		virtual unsigned char 	* getPixels() = 0;
//
//		virtual void	close() = 0;	
//		virtual void	clearMemory() = 0;
//		
//		virtual float	getHeight() = 0;
//		virtual float	getWidth() = 0;
//		
//		//should implement!
//		virtual void setVerbose(bool bTalkToMe);
//		virtual void setDeviceID(int _deviceID);
//		virtual void setDesiredFrameRate(int framerate);
//		virtual void videoSettings();
//			
//};


class ofxQTKitVideoGrabber: public ofBaseVideoGrabber{
  public:
	ofxQTKitVideoGrabber();
	~ofxQTKitVideoGrabber();
   
	bool			initGrabber(int w, int h);
	void			grabFrame();
	bool			isFrameNew();
	void			update();
	void 			setUseTexture(bool bUse);
	
	void 			listDevices();
	void			close();
	void			clearMemory();
	unsigned char 	* getPixels();
	ofTexture &		getTextureReference();
	void 			setVerbose(bool bTalkToMe);
	void			setDeviceID(int deviceID);
	void			setDesiredFrameRate(int framerate){ ofLog(OF_LOG_WARNING, "ofxQTKitVideoGrabber -- Cannot specify framerate.");  };
	void			videoSettings() { ofLog(OF_LOG_WARNING, "ofxQTKitVideoGrabber -- No video settings available.");  };
	void 			draw(float x, float y, float w, float h);
	void 			draw(float x, float y);
	
	float 			getHeight();
	float 			getWidth();
	
  protected:

	bool confirmInit();
	bool isInited;
	int deviceID;
	void* grabber;
};

#endif