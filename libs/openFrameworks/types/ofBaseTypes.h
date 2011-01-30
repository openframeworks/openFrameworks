/*
 *  ofBaseTypes.h
 *  openFrameworksLib
 *
 *  Created by zachary lieberman on 1/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */



#pragma once
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofConstants.h"
class ofShape;
class ofPixels;
class ofPath;
class ofPolyline;
class ofVertexData;


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
//	virtual ofPixels getOFPixels()=0;
//	virtual ofPixels getOFPixels() const=0;
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
class ofBaseVideo: public ofBaseHasPixels, public ofBaseUpdates{
public:
	virtual ~ofBaseVideo(){}
	virtual bool isFrameNew()=0;
	virtual void close()=0;
};


//----------------------------------------------------------
// ofBaseVideoGrabber
//----------------------------------------------------------
class ofBaseVideoGrabber: public ofBaseVideo{
	
	public :
	
	ofBaseVideoGrabber();
	virtual ~ofBaseVideoGrabber();
	
	//needs implementing
	virtual void	listDevices() = 0;		
	virtual bool	initGrabber(int w, int h) = 0;
	//virtual void	grabFrame() = 0;
	virtual bool	isFrameNew() = 0;
	
	virtual unsigned char 	* getPixels() = 0;
	
	virtual void	close() = 0;	
	
	virtual float	getHeight() = 0;
	virtual float	getWidth() = 0;
	
	//should implement!
	virtual void setVerbose(bool bTalkToMe);
	virtual void setDeviceID(int _deviceID);
	virtual void setDesiredFrameRate(int framerate);
	virtual void videoSettings();
	virtual void setPixelFormat(ofPixelFormat pixelFormat);
	
};


//----------------------------------------------------------
// ofBaseVideoPlayer
//----------------------------------------------------------
class ofBaseVideoPlayer: public ofBaseVideo{
	
public:
	
	ofBaseVideoPlayer();
	virtual ~ofBaseVideoPlayer();
	
	//needs implementing
	virtual bool				loadMovie(string name) = 0;
	virtual void				close() = 0;
	//virtual void				idleMovie() = 0;
	
	virtual void				play() = 0;
	virtual void				stop() = 0;		
	
	virtual bool 				isFrameNew() = 0;
	virtual unsigned char * 	getPixels() = 0;
	virtual ofTexture *			getTexture(){return NULL;}; // if your videoplayer needs to implement seperate texture and pixel returns for performance, implement this function to return a texture instead of a pixel array. see iPhoneVideoGrabber for reference
	
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
	virtual void 				setLoopState(ofLoopType state);
	virtual void   				setSpeed(float speed);
	virtual void				setFrame(int frame);  // frame 0 = first frame...
	
	virtual int					getCurrentFrame();
	virtual int					getTotalNumFrames();
	virtual int					getLoopState();
	
	virtual void				firstFrame();
	virtual void				nextFrame();
	virtual void				previousFrame();
	virtual void				setPixelFormat(ofPixelFormat pixelFormat);
	
};

//----------------------------------------------------------
// base renderers
//----------------------------------------------------------

class ofBaseRenderer{
public:
	virtual ~ofBaseRenderer(){}
	virtual bool rendersPathDirectly()=0;
	//virtual bool is3DRenderer()=0;
	//virtual bool isFrameByFrameRenderer()=0;
	virtual void draw(ofPolyline & poly)=0;
	virtual void draw(ofShape & shape)=0;
	virtual void draw(ofPath & shape)=0;
	virtual void draw(ofVertexData & vertexData)=0;

	//--------------------------------------------
	// transformations
	virtual void pushView(){};
	virtual void popView(){};

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	virtual void viewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true){};
	virtual void setupScreenPerspective(float width = 0, float height = 0, int orientation=0, bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0){};
	virtual void setupScreenOrtho(float width = 0, float height = 0, bool vFlip = true, float nearDist = -1, float farDist = 1){};
	virtual ofRectangle getCurrentViewport(){return ofRectangle();};
	virtual int getViewportWidth(){return 0;};
	virtual int getViewportHeight(){return 0;};

	virtual void setCoordHandedness(ofHandednessType handedness){};
	virtual ofHandednessType getCoordHandedness(){return OF_LEFT_HANDED;};

	//our openGL wrappers
	virtual void pushMatrix(){};
	virtual void popMatrix(){};
	virtual void translate(float x, float y, float z = 0){};
	virtual void translate(const ofPoint & p){};
	virtual void scale(float xAmnt, float yAmnt, float zAmnt = 1){};
	virtual void rotate(float degrees, float vecX, float vecY, float vecZ){};
	virtual void rotateX(float degrees){};
	virtual void rotateY(float degrees){};
	virtual void rotateZ(float degrees){};
	virtual void rotate(float degrees){};

	// screen coordinate things / default gl values
	virtual void setupGraphicDefaults(){};
	virtual void setupScreen(){};
};

/*class ofBaseRenders{

	virtual ~ofBaseRenders(){}
	virtual void lineTo(const ofPoint & p)=0;
	virtual void lineTo(float x, float y, float z=0){lineTo(ofPoint(x,y,z));}

	// starts a new subpath with defaults for winding, stroke, fill...
	// the new subpath starts in p
	virtual void moveTo(const ofPoint & p)=0;
	virtual void moveTo(float x, float y, float z=0)
		{moveTo(ofPoint(x,y,z));}

	virtual void curveTo(const ofPoint & p)=0;
	virtual void curveTo(float x, float y, float z=0)
		{curveTo(ofPoint(x,y,z));}

	virtual void bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p)=0;
	virtual void bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y)
		{bezierTo(ofPoint(cx1,cy1),ofPoint(cx2,cy2),ofPoint(x,y));}
	virtual void bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z)
		{bezierTo(ofPoint(cx1,cy1),ofPoint(cx2,cy2),ofPoint(x,y));}

	virtual void arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd)=0;
	virtual void arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd)
		{arc(ofPoint(x,y),radiusX,radiusY,angleBegin,angleEnd);}
	virtual void arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd)
		{arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd);}
	// closes current path, next command starts a new one with defaults for winding, stroke, fill...
	virtual void close()=0;

	virtual void setPolyWindingMode(ofPolyWindingMode mode)=0;
	virtual void setColor( const ofColor& color ) { setFillColor( color ); setStrokeColor( color ); }
	virtual void setHexColor( int hex ) { setColor( ofColor().fromHex( hex ) ); };
	virtual void setFilled(bool hasFill)=0; // default false
	virtual void setFillColor(const ofColor & color)=0;
	virtual void setFillHexColor( int hex ) { setFillColor( ofColor().fromHex( hex ) ); };
	virtual void setStrokeColor(const ofColor & color)=0;
	virtual void setStrokeHexColor( int hex ) { setStrokeColor( ofColor().fromHex( hex ) ); };
	virtual void setStrokeWidth(float width)=0; // default 1
};
 */

