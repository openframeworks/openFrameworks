/*
 *  ofBaseTypes.h
 *  openFrameworksLib
 *
 *  Created by zachary lieberman on 1/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */



#pragma once
#include "ofConstants.h"
#include "ofPoint.h"
#include "ofRectangle.h"


//----------------------------------------------------------
// ofBaseDraws
//----------------------------------------------------------

class ofBaseDraws{
public:
	virtual ~ofBaseDraws(){}
	virtual void draw(float x,float y)=0;
	virtual void draw(float x,float y,float w, float h)=0;
	
	virtual void draw(const ofPoint & point){
		draw( point.x, point.y);
	}
	
	virtual void draw(const ofRectangle & rect){
		draw(rect.x, rect.y, rect.width, rect.height); 
	}
	
	virtual float getHeight()=0;
	virtual float getWidth()=0;
	
	virtual void setAnchorPercent(float xPct, float yPct){};
    virtual void setAnchorPoint(float x, float y){};
	virtual void resetAnchor(){};
	
};

//----------------------------------------------------------
// ofBaseUpdates
//----------------------------------------------------------

class ofBaseUpdates{
public:
	virtual ~ofBaseUpdates(){}
	virtual void update()=0;
};


//----------------------------------------------------------
// ofBaseHasTexture
//----------------------------------------------------------
class ofTexture;

class ofBaseHasTexture{
public:
	virtual ~ofBaseHasTexture(){}
	virtual ofTexture & getTextureReference()=0;
	virtual void setUseTexture(bool bUseTex)=0;
};

//----------------------------------------------------------
// ofBaseHasPixels
//----------------------------------------------------------
class ofBaseHasPixels{
public:
	virtual ~ofBaseHasPixels(){}
	virtual unsigned char * getPixels()=0;
};

//----------------------------------------------------------
// ofBaseImage
//----------------------------------------------------------
class ofBaseImage: public ofBaseDraws, public ofBaseHasTexture, public ofBaseHasPixels{
public:
	
};

//----------------------------------------------------------
// ofBaseVideo
//----------------------------------------------------------
class ofBaseVideo: public ofBaseImage, public ofBaseUpdates{
public:
	virtual ~ofBaseVideo(){}
	virtual bool isFrameNew()=0;
	virtual void close()=0;
};


//----------------------------------------------------------
// ofBaseVideoGrabber
//----------------------------------------------------------
class ofBaseVideoGrabber{
	
	public :
	
	ofBaseVideoGrabber();
	virtual ~ofBaseVideoGrabber();
	
	//needs implementing
	virtual void	listDevices() = 0;		
	virtual bool	initGrabber(int w, int h) = 0;		
	virtual void	grabFrame() = 0;
	virtual bool	isFrameNew() = 0;
	
	virtual unsigned char 	* getPixels() = 0;
	
	virtual void	close() = 0;	
	virtual void	clearMemory() = 0;
	
	virtual float	getHeight() = 0;
	virtual float	getWidth() = 0;
	
	//should implement!
	virtual void setVerbose(bool bTalkToMe);
	virtual void setDeviceID(int _deviceID);
	virtual void setDesiredFrameRate(int framerate);
	virtual void videoSettings();
	
};


//----------------------------------------------------------
// ofBaseVideoPlayer
//----------------------------------------------------------
class ofBaseVideoPlayer{
	
public:
	
	ofBaseVideoPlayer();
	virtual ~ofBaseVideoPlayer();
	
	//needs implementing
	virtual bool				loadMovie(string name) = 0;
	virtual void				closeMovie() = 0;
	virtual void				idleMovie() = 0;
	
	virtual void				play() = 0;
	virtual void				stop() = 0;		
	
	virtual void				clearMemory() = 0;
	
	virtual bool 				isFrameNew() = 0;
	virtual unsigned char * 	getPixels() = 0;
	
	virtual float 				getWidth() = 0;
	virtual float 				getHeight() = 0;
	
	virtual bool				isPaused() = 0;
	virtual bool				isLoaded() = 0;
	virtual bool				isPlaying() = 0;
	
	//should implement!
	virtual float 				getPosition();
	virtual float 				getSpeed();
	virtual float 				getDuration();
	virtual bool				getIsMovieDone();
	
	virtual void 				setPaused(bool bPause);
	virtual void 				setPosition(float pct);
	virtual void 				setVolume(int volume);
	virtual void 				setLoopState(int state);
	virtual void   				setSpeed(float speed);
	virtual void				setFrame(int frame);  // frame 0 = first frame...
	
	virtual int					getCurrentFrame();
	virtual int					getTotalNumFrames();
	virtual int					getLoopState();
	
	virtual void				firstFrame();
	virtual void				nextFrame();
	virtual void				previousFrame();
	
	
};


//----------------------------------------------------------
// ofBaseHasDevices
//----------------------------------------------------------
class ofBaseHasDevices {
	
public: 
	
	virtual void buildDeviceList() = 0; 
	
	virtual vector < string > getDeviceList(){
		return deviceNames;
	}
	
	virtual void listDevices(){
		if (!bHaveEnumeratedDevices){
			buildDeviceList();
		}
		if (bHaveEnumeratedDevices){
			printf("-------------------------------------------- \n");
			printf("listing devices for %s \n", deviceType.c_str());
			printf("\n");
			for (int i = 0; i < deviceNames.size(); i++){
				printf("device [%i] - %s\n", i, deviceNames[i].c_str());
			}
			printf("-------------------------------------------- \n");
		}
	}
	
	string				deviceType;
	vector < string >	deviceNames;
	bool bHaveEnumeratedDevices;
	
};




