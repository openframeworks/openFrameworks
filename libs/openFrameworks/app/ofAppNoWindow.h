#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofBaseTypes.h"


//#ifdef TARGET_WIN32
//	#define GLUT_BUILDING_LIB
//	#include "glut.h"
//#endif
//#ifdef TARGET_OSX
//	#include <GLUT/glut.h>
//#endif
//#ifdef TARGET_LINUX
//	#include <GL/glut.h>
//#endif

class ofBaseApp;

class ofAppNoWindow : public ofAppBaseWindow {

public:
	ofAppNoWindow();
	~ofAppNoWindow(){}

	void runAppViaInfiniteLoop(ofBaseApp * appPtr);

	static void exitApp();
	void setupOpenGL(int w, int h, int screenMode);

	ofPoint		getWindowPosition();
	ofPoint		getWindowSize();
	ofPoint		getScreenSize();

	int			getWidth();
	int			getHeight();


private:
	int width, height;

    ofBaseApp *		ofAppPtr;
};

class ofNoopRenderer: public ofBaseRenderer{
	static const string TYPE;
	virtual const string & getType(){return TYPE;}

	virtual void update(){}

	virtual void draw(ofPolyline & poly){}
	virtual void draw(ofPath & shape){}
	virtual void draw(of3dPrimitive&, ofPolyRenderMode){}
	virtual void draw(ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals){}
	virtual void draw(ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals){}
	virtual void draw(vector<ofPoint> & vertexData, ofPrimitiveMode drawMode){}
	virtual void draw(ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){}
	virtual void draw(ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){}
	virtual void draw(ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){}

	//--------------------------------------------
	// transformations
	virtual void pushView(){};
	virtual void popView(){};

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	virtual void viewport(ofRectangle viewport){};
	virtual void viewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true){};
	virtual void setupScreenPerspective(float width = 0, float height = 0, float fov = 60, float nearDist = 0, float farDist = 0){}
	virtual void setupScreenOrtho(float width = 0, float height = 0, float nearDist = -1, float farDist = 1){};
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

	// drawing modes
	virtual void setRectMode(ofRectMode mode){}
	virtual ofRectMode getRectMode(){return OF_RECTMODE_CORNER;}
	virtual void setFillMode(ofFillFlag fill){}
	virtual ofFillFlag getFillMode(){return OF_FILLED;}
	virtual void setLineWidth(float lineWidth){}
	virtual void setBlendMode(ofBlendMode blendMode){}
	virtual void setLineSmoothing(bool smooth){}
	virtual void setCircleResolution(int res){};
	virtual void setSphereResolution(int res){};
	virtual void enablePointSprites(){};
	virtual void disablePointSprites(){};
	virtual void setDepthTest(bool){};

	// color options
	virtual void setColor(int r, int g, int b){}; // 0-255
	virtual void setColor(int r, int g, int b, int a){}; // 0-255
	virtual void setColor(const ofColor & color){};
	virtual void setColor(const ofColor & color, int _a){};
	virtual void setColor(int gray){}; // new set a color as grayscale with one argument
	virtual void setHexColor( int hexColor ){}; // hex, like web 0xFF0033;

	// bg color
	virtual ofFloatColor & getBgColor(){return bgColor;}
	virtual bool bClearBg(){
		return true;
	}
	virtual void background(const ofColor & c){};
	virtual void background(float brightness){};
	virtual void background(int hexColor, float _a=255.0f){};
	virtual void background(int r, int g, int b, int a=255){};

	virtual void setBackgroundAuto(bool bManual){};		// default is true

	virtual void clear(float r, float g, float b, float a=0){};
	virtual void clear(float brightness, float a=0){};
	virtual void clearAlpha(){};

	// drawing
	virtual void drawLine(float x1, float y1, float z1, float x2, float y2, float z2){}
	virtual void drawRectangle(float x, float y, float z, float w, float h){}
	virtual void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){}
	virtual void drawCircle(float x, float y, float z, float radius){}
	virtual void drawSphere(float x, float y, float z, float radius){}
	virtual void drawEllipse(float x, float y, float z, float width, float height){}
	virtual void drawString(string text, float x, float y, float z, ofDrawBitmapMode mode){}


	// returns true if the renderer can render curves without decomposing them
	virtual bool rendersPathPrimitives(){return true;}
private:
	ofFloatColor bgColor;
};

