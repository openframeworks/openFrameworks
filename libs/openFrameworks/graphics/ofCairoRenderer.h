#pragma once


#include "cairo-features.h"
#include "cairo-pdf.h"
#include "cairo-svg.h"
#include "cairo.h"
#include <deque>
#include <stack>
#include "ofMatrix4x4.h"
#include "ofBaseTypes.h"
#include "ofPath.h"



class ofCairoRenderer: public ofBaseRenderer{
public:
	ofCairoRenderer();
	~ofCairoRenderer();

	string getType(){ return "cairo"; }

	enum Type{
		PDF,
		SVG,
		PNG
	};
	void setup(string filename, Type type=ofCairoRenderer::PDF, bool multiPage=true, bool b3D=false);
	void close();

	void draw(ofPath & shape);
	void draw(ofSubPath & path);
	void draw(ofPolyline & poly);
	void draw(ofMesh & vertexData);
	void draw(ofMesh & vertexData, ofPolyRenderMode mode);
	void draw(vector<ofPoint> & vertexData, ofPrimitiveMode drawMode);
	void draw(ofImage & img, float x, float y, float z, float w, float h);
	void draw(ofFloatImage & image, float x, float y, float z, float w, float h);
	void draw(ofShortImage & image, float x, float y, float z, float w, float h);

	bool rendersPathPrimitives(){
		return true;
	}

	//--------------------------------------------
	// transformations
	void pushView();
	void popView();

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport);
	void viewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true);
	void setupScreenPerspective(float width = 0, float height = 0, ofOrientation orientation = OF_ORIENTATION_UNKNOWN, bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0);
	void setupScreenOrtho(float width = 0, float height = 0, ofOrientation orientation = OF_ORIENTATION_UNKNOWN, bool vFlip = true, float nearDist = -1, float farDist = 1);
	ofRectangle getCurrentViewport();
	int getViewportWidth();
	int getViewportHeight();

	void setCoordHandedness(ofHandednessType handedness);
	ofHandednessType getCoordHandedness();

	// drawing modes
	void setRectMode(ofRectMode mode);
	ofRectMode getRectMode();
	void setFillMode(ofFillFlag fill);
	ofFillFlag getFillMode();
	void setLineWidth(float lineWidth);
	void setBlendMode(ofBlendMode blendMode);
	void setLineSmoothing(bool smooth);

	//our openGL wrappers
	void pushMatrix();
	void popMatrix();
	void translate(float x, float y, float z = 0);
	void translate(const ofPoint & p);
	void scale(float xAmnt, float yAmnt, float zAmnt = 1);
	void rotate(float degrees, float vecX, float vecY, float vecZ);
	void rotateX(float degrees);
	void rotateY(float degrees);
	void rotateZ(float degrees);
	void rotate(float degrees);

	// screen coordinate things / default gl values
	void setupGraphicDefaults();
	void setupScreen();

	// color options
	void setColor(int r, int g, int b); // 0-255
	void setColor(int r, int g, int b, int a); // 0-255
	void setColor(const ofColor & color);
	void setColor(const ofColor & color, int _a);
	void setColor(int gray); // new set a color as grayscale with one argument
	void setHexColor( int hexColor ); // hex, like web 0xFF0033;

	// bg color
	ofColor & getBgColor();
	bool bClearBg();
	void background(const ofColor & c);
	void background(float brightness);
	void background(int hexColor, float _a=255.0f);
	void background(int r, int g, int b, int a=255);

	void setBackgroundAuto(bool bManual);		// default is true

	void clear(float r, float g, float b, float a=0);
	void clear(float brightness, float a=0);
	void clearAlpha();

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);
	void drawRectangle(float x, float y, float z, float w, float h);
	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	void drawCircle(float x, float y, float z, float radius);
	void drawEllipse(float x, float y, float z, float width, float height);
	void drawString(string text, float x, float y, float z, ofDrawBitmapMode mode);

	// cairo specifics
	cairo_t * getCairoContext();
	cairo_surface_t * getCairoSurface();


private:
	cairo_matrix_t * getCairoMatrix();
	void setCairoMatrix();
	ofVec3f transform(ofVec3f vec);

	deque<ofPoint> curvePoints;
	cairo_t * cr;
	cairo_surface_t * surface;
	bool bBackgroundAuto;
	ofColor bgColor;

	stack<cairo_matrix_t> matrixStack;
	cairo_matrix_t tmpMatrix;

	Type type;
	int page;
	bool multiPage;

	// 3d transformation
	bool b3D;
	ofMatrix4x4 projection;
	ofMatrix4x4 modelView;
	ofRectangle viewportRect;

	stack<ofMatrix4x4> projectionStack;
	stack<ofMatrix4x4> modelViewStack;
	stack<ofRectangle> viewportStack;


	ofFillFlag bFilled;
	bool bSmoothHinted;
	ofRectMode rectMode;
};
