#pragma once
#include "ofConstants.h"
#include "ofTypes.h"
#include "ofRectangle.h"
#include "ofMatrix4x4.h"
#include "ofURLFileLoader.h"
#include "ofMesh.h"

class ofAbstractParameter;

template<typename T>
class ofImage_;

typedef ofImage_<unsigned char> ofImage;
typedef ofImage_<float> ofFloatImage;
typedef ofImage_<unsigned short> ofShortImage;

template<typename T>
class ofPixels_;

typedef ofPixels_<unsigned char> ofPixels;
typedef ofPixels_<float> ofFloatPixels;
typedef ofPixels_<unsigned short> ofShortPixels;
typedef ofPixels& ofPixelsRef;

template<typename T>
class ofColor_;

typedef ofColor_<unsigned char> ofColor;

class ofVec3f;
typedef ofVec3f ofPoint;

class ofPath;
class ofPolyline;
class ofFbo;
class of3dPrimitive;
class ofLight;
class ofMaterial;
class ofBaseMaterial;

bool ofIsVFlipped();


//----------------------------------------------------------
// ofBaseDraws
//----------------------------------------------------------

class ofBaseDraws{
public:
	virtual ~ofBaseDraws(){}
	virtual void draw(float x, float y) const=0;
	virtual void draw(float x, float y, float w, float h) const=0;
	virtual void draw(const ofPoint & point) const {
		draw(point.x, point.y);
	}
	virtual void draw(const ofRectangle & rect) const {
		draw(rect.x, rect.y, rect.width, rect.height);
	}
	virtual void draw(const ofPoint & point, float w, float h) const {
		draw(point.x, point.y, w, h);
	}
	
	virtual float getHeight() const = 0;
	virtual float getWidth() const = 0;
	
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
	virtual const ofTexture & getTextureReference() const=0;
	virtual void setUseTexture(bool bUseTex)=0;
	virtual bool isUsingTexture() const=0;
};

class ofBaseHasTexturePlanes: public ofBaseHasTexture{
public:
	virtual ~ofBaseHasTexturePlanes(){}
	virtual vector<ofTexture> & getTexturePlanes()=0;
	virtual const vector<ofTexture> & getTexturePlanes() const=0;
};

//----------------------------------------------------------
// ofAbstractHasPixels
//----------------------------------------------------------
class ofAbstractHasPixels{
public:
	virtual ~ofAbstractHasPixels(){}
};

//----------------------------------------------------------
// ofBaseHasPixels
//----------------------------------------------------------
template<typename T>
class ofBaseHasPixels_: public ofAbstractHasPixels{
public:
	virtual ~ofBaseHasPixels_<T>(){}
	virtual ofPixels_<T> & getPixels()=0;
	virtual const ofPixels_<T> & getPixels() const=0;
};

typedef ofBaseHasPixels_<unsigned char> ofBaseHasPixels;
typedef ofBaseHasPixels_<float> ofBaseHasFloatPixels;
typedef ofBaseHasPixels_<unsigned short> ofBaseHasShortPixels;

//----------------------------------------------------------
// ofAbstractImage    ->   to be able to put different types of images in vectors...
//----------------------------------------------------------
class ofAbstractImage: public ofBaseDraws, public ofBaseHasTexture{
public:
	virtual ~ofAbstractImage(){}
};

//----------------------------------------------------------
// ofBaseImage
//----------------------------------------------------------
template<typename T>
class ofBaseImage_: public ofAbstractImage, virtual public ofBaseHasPixels_<T>{
public:
	virtual ~ofBaseImage_<T>(){};
};

typedef ofBaseImage_<unsigned char> ofBaseImage;
typedef ofBaseImage_<float> ofBaseFloatImage;
typedef ofBaseImage_<unsigned short> ofBaseShortImage;

//----------------------------------------------------------
// ofBaseHasSoundStream
//----------------------------------------------------------
class ofBaseSoundInput{

	public:
        virtual ~ofBaseSoundInput() {};
    
		virtual void audioIn( float * input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ){
			audioIn(input, bufferSize, nChannels);
		}

		virtual void audioIn( float * input, int bufferSize, int nChannels ){  
			audioReceived(input, bufferSize, nChannels);
		}

		virtual void audioReceived( float * input, int bufferSize, int nChannels ){}
};

//----------------------------------------------------------
// ofBaseHasSoundStream
//----------------------------------------------------------
class ofBaseSoundOutput{

	public:
        virtual ~ofBaseSoundOutput() {};
    
		virtual void audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount  ){
			audioOut(output, bufferSize, nChannels);
		}

		virtual void audioOut( float * output, int bufferSize, int nChannels ){
			audioRequested(output, bufferSize, nChannels);
		}

		//legacy
		virtual void audioRequested( float * output, int bufferSize, int nChannels ){
		}
};


//----------------------------------------------------------
// ofBaseVideo
//----------------------------------------------------------
class ofBaseVideo: virtual public ofBaseHasPixels, public ofBaseUpdates{
public:
	virtual ~ofBaseVideo(){}
	virtual bool isFrameNew() const =0;
	virtual void close()=0;
	virtual bool isInitialized() const=0;

	virtual bool setPixelFormat(ofPixelFormat pixelFormat) = 0;
	virtual ofPixelFormat getPixelFormat() const = 0;
};


//----------------------------------------------------------
// ofBaseVideoDraws
//----------------------------------------------------------
class ofBaseVideoDraws: virtual public ofBaseVideo, public ofBaseDraws, public ofBaseHasTexturePlanes,virtual public ofBaseHasPixels{
public:
	virtual ~ofBaseVideoDraws(){}
};

//----------------------------------------------------------
// ofBaseVideoGrabber
//----------------------------------------------------------
class ofBaseVideoGrabber: virtual public ofBaseVideo{
	
	public :
	virtual ~ofBaseVideoGrabber();

	//needs implementing
	virtual vector<ofVideoDevice>	listDevices() const = 0;
	virtual bool	initGrabber(int w, int h) = 0;
	
	virtual float	getHeight() const = 0;
	virtual float	getWidth() const = 0;

	// implement only if internal API can upload directly to texture
	virtual ofTexture * getTexture(){ return NULL; }

	//should implement!
	virtual void setVerbose(bool bTalkToMe);
	virtual void setDeviceID(int _deviceID);
	virtual void setDesiredFrameRate(int framerate);
	virtual void videoSettings();
	
};


//----------------------------------------------------------
// ofBaseVideoPlayer
//----------------------------------------------------------
class ofBaseVideoPlayer: virtual public ofBaseVideo{
	
public:
	virtual ~ofBaseVideoPlayer();
	
	//needs implementing
	virtual bool				loadMovie(string name) = 0;
	
	virtual void				play() = 0;
	virtual void				stop() = 0;		
	virtual ofTexture *			getTexture(){return NULL;}; // if your videoplayer needs to implement seperate texture and pixel returns for performance, implement this function to return a texture instead of a pixel array. see iPhoneVideoGrabber for reference
	
	virtual float 				getWidth() const = 0;
	virtual float 				getHeight() const = 0;
	
	virtual bool				isPaused() const = 0;
	virtual bool				isLoaded() const = 0;
	virtual bool				isPlaying() const = 0;
	virtual bool				isInitialized() const{ return isLoaded(); }
		
	//should implement!
	virtual float 				getPosition() const;
	virtual float 				getSpeed() const;
	virtual float 				getDuration() const;
	virtual bool				getIsMovieDone() const;
	
	virtual void 				setPaused(bool bPause);
	virtual void 				setPosition(float pct);
	virtual void 				setVolume(float volume); // 0..1
	virtual void 				setLoopState(ofLoopType state);
	virtual void   				setSpeed(float speed);
	virtual void				setFrame(int frame);  // frame 0 = first frame...
	
	virtual int					getCurrentFrame() const;
	virtual int					getTotalNumFrames() const;
	virtual ofLoopType			getLoopState() const;
	
	virtual void				firstFrame();
	virtual void				nextFrame();
	virtual void				previousFrame();
};

//----------------------------------------------------------
// base renderers
//----------------------------------------------------------
class of3dPrimitive;

class ofBaseRenderer{
public:
	virtual ~ofBaseRenderer(){}

	virtual const string & getType()=0;

	virtual void update()=0;

	virtual void draw(const ofPolyline & poly) const=0;
	virtual void draw(const ofPath & shape) const=0;
	virtual void draw(const ofMesh & mesh, ofPolyRenderMode renderType) const{
		draw(mesh,renderType,mesh.usingColors(),mesh.usingTextures(),mesh.usingNormals());
	}
	virtual void draw(const ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals) const=0;
	virtual void draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const=0;
    virtual void draw(const of3dPrimitive& model, ofPolyRenderMode renderType) const=0;
	virtual void draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const=0;
	virtual void draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const=0;
	virtual void draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const=0;
	virtual void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const=0;

	virtual void bind(const ofBaseVideoDraws & video) const=0;
	virtual void unbind(const ofBaseVideoDraws & video) const=0;

	//--------------------------------------------
	// transformations
	virtual void pushView()=0;
	virtual void popView()=0;

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	virtual void viewport(ofRectangle viewport)=0;
	virtual void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=ofIsVFlipped())=0;
	virtual void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0)=0;
	virtual void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1)=0;
	virtual void setOrientation(ofOrientation orientation, bool vFlip)=0;
	virtual ofRectangle getCurrentViewport() const=0;
	virtual ofRectangle getNativeViewport() const=0;
	virtual int getViewportWidth() const=0;
	virtual int getViewportHeight() const=0;
	virtual bool isVFlipped() const=0;

	virtual void setCoordHandedness(ofHandednessType handedness)=0;
	virtual ofHandednessType getCoordHandedness() const=0;

	//our openGL wrappers
	virtual void pushMatrix()=0;
	virtual void popMatrix()=0;
	virtual ofMatrix4x4 getCurrentMatrix(ofMatrixMode matrixMode_) const=0;
	virtual ofMatrix4x4 getCurrentOrientationMatrix() const=0;
	virtual void translate(float x, float y, float z = 0)=0;
	virtual void translate(const ofPoint & p)=0;
	virtual void scale(float xAmnt, float yAmnt, float zAmnt = 1)=0;
	virtual void rotate(float degrees, float vecX, float vecY, float vecZ)=0;
	virtual void rotateX(float degrees)=0;
	virtual void rotateY(float degrees)=0;
	virtual void rotateZ(float degrees)=0;
	virtual void rotate(float degrees)=0;
	virtual void matrixMode(ofMatrixMode mode)=0;
	virtual void loadIdentityMatrix (void)=0;
	virtual void loadMatrix (const ofMatrix4x4 & m)=0;
	virtual void loadMatrix (const float *m)=0;
	virtual void multMatrix (const ofMatrix4x4 & m)=0;
	virtual void multMatrix (const float *m)=0;
	virtual void loadViewMatrix(const ofMatrix4x4 & m)=0;
	virtual void multViewMatrix(const ofMatrix4x4 & m)=0;
	virtual ofMatrix4x4 getCurrentViewMatrix() const=0;
	virtual ofMatrix4x4 getCurrentNormalMatrix() const=0;
	
	// screen coordinate things / default gl values
	virtual void setupGraphicDefaults()=0;
	virtual void setupScreen()=0;

	// drawing modes
	virtual void setRectMode(ofRectMode mode)=0;
	virtual ofRectMode getRectMode()=0;
	virtual void setFillMode(ofFillFlag fill)=0;
	virtual ofFillFlag getFillMode()=0;
	virtual void setLineWidth(float lineWidth)=0;
	virtual void setDepthTest(bool depthTest)=0;
	virtual void setBlendMode(ofBlendMode blendMode)=0;
	virtual void setLineSmoothing(bool smooth)=0;
	virtual void setCircleResolution(int res)=0;
	virtual void enableAntiAliasing()=0;
	virtual void disableAntiAliasing()=0;

	// color options
	virtual void setColor(int r, int g, int b)=0; // 0-255
	virtual void setColor(int r, int g, int b, int a)=0; // 0-255
	virtual void setColor(const ofColor & color)=0;
	virtual void setColor(const ofColor & color, int _a)=0;
	virtual void setColor(int gray)=0; // new set a color as grayscale with one argument
	virtual void setHexColor( int hexColor )=0; // hex, like web 0xFF0033;

	// bg color
	virtual ofFloatColor & getBgColor()=0;
	virtual bool bClearBg()=0;
	virtual void background(const ofColor & c)=0;
	virtual void background(float brightness)=0;
	virtual void background(int hexColor, float _a=255.0f)=0;
	virtual void background(int r, int g, int b, int a=255)=0;

	virtual void setBackgroundAuto(bool bManual)=0;	// default is true

	virtual void clear(float r, float g, float b, float a=0)=0;
	virtual void clear(float brightness, float a=0)=0;
	virtual void clearAlpha()=0;

	// drawing
	virtual void drawLine(float x1, float y1, float z1, float x2, float y2, float z2)=0;
	virtual void drawRectangle(float x, float y, float z, float w, float h)=0;
	virtual void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)=0;
	virtual void drawCircle(float x, float y, float z, float radius)=0;
	virtual void drawEllipse(float x, float y, float z, float width, float height)=0;
	virtual void drawString(string text, float x, float y, float z, ofDrawBitmapMode mode)=0;


	// returns true if the renderer can render curves without decomposing them
	virtual bool rendersPathPrimitives()=0;
};

class ofBaseGLRenderer: public ofBaseRenderer{
public:
	virtual void setCurrentFBO(const ofFbo * fbo)=0;

	virtual void enableTextureTarget(int textureTarget, int textureID, int textureLocation)=0;
	virtual void disableTextureTarget(int textureTarget, int textureLocation)=0;
	virtual void setAlphaMaskTex(ofTexture & tex)=0;
	virtual void disableAlphaMask()=0;
	virtual void enablePointSprites()=0;
	virtual void disablePointSprites()=0;

	// lighting
	virtual void enableLighting()=0;
	virtual void disableLighting()=0;
	virtual void enableSeparateSpecularLight()=0;
	virtual void disableSeparateSpecularLight()=0;
	virtual bool getLightingEnabled()=0;
	virtual void setSmoothLighting(bool b)=0;
	virtual void setGlobalAmbientColor(const ofColor& c)=0;
	virtual void enableLight(int lightIndex)=0;
	virtual void disableLight(int lightIndex)=0;
	virtual void setLightSpotlightCutOff(int lightIndex, float spotCutOff)=0;
	virtual void setLightSpotConcentration(int lightIndex, float exponent)=0;
	virtual void setLightAttenuation(int lightIndex, float constant, float linear, float quadratic )=0;
	virtual void setLightAmbientColor(int lightIndex, const ofFloatColor& c)=0;
	virtual void setLightDiffuseColor(int lightIndex, const ofFloatColor& c)=0;
	virtual void setLightSpecularColor(int lightIndex, const ofFloatColor& c)=0;
	virtual void setLightPosition(int lightIndex, const ofVec4f & position)=0;
	virtual void setLightSpotDirection(int lightIndex, const ofVec4f & direction)=0;

	// materials
	virtual void setCurrentMaterial(ofBaseMaterial * material)=0;
};


class ofBaseSerializer{
public:
	virtual ~ofBaseSerializer(){}

	virtual void serialize(const ofAbstractParameter & parameter)=0;
	virtual void deserialize(ofAbstractParameter & parameter)=0;
};

class ofBaseFileSerializer: public ofBaseSerializer{
public:
	virtual ~ofBaseFileSerializer(){}

	virtual bool load(const string & path)=0;
	virtual bool save(const string & path)=0;
};

class ofBaseURLFileLoader{
public:
	virtual ~ofBaseURLFileLoader(){};
    virtual ofHttpResponse get(string url)=0;
    virtual int getAsync(string url, string name="")=0;
    virtual ofHttpResponse saveTo(string url, string path)=0;
    virtual int saveAsync(string url, string path)=0;
    virtual void remove(int id)=0;
    virtual void clear()=0;
    virtual void stop()=0;
};

class ofBaseMaterial{
public:
	virtual ~ofBaseMaterial(){};
	virtual void begin();
	virtual void end();
	virtual void beginShader(int textureTarget)=0;
};
