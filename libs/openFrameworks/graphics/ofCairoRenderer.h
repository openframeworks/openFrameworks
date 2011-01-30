#pragma once


#include <cairo-features.h>
#include <cairo-pdf.h>
#include <cairo-svg.h>
#include <cairo.h>
#include <deque>
#include <stack>
#include "ofMatrix4x4.h"
#include "ofBaseTypes.h"
#include "ofPath.h"



class ofCairoRenderer: public ofBaseRenderer{
public:
	ofCairoRenderer();
	~ofCairoRenderer();

	enum Type{
		PDF,
		SVG,
		PNG
	};
	void setup(string filename, Type type=ofCairoRenderer::PDF);
	void close();

	void draw(ofPath & path);
	void draw(ofPolyline & poly);
	void draw(ofShape & shape);
	void draw(ofVertexData & vertexData);

	bool rendersPathDirectly(){
		return true;
	}

	//--------------------------------------------
	// transformations
	void pushView();
	void popView();

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true);
	void setupScreenPerspective(float width = 0, float height = 0, bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0);
	void setupScreenOrtho(float width = 0, float height = 0, bool vFlip = true, float nearDist = -1, float farDist = 1);
	ofRectangle getCurrentViewport();
	int getViewportWidth();
	int getViewportHeight();

	void setCoordHandedness(ofHandednessType handedness);
	ofHandednessType getCoordHandedness();

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

	cairo_t * getCairoContext();
	cairo_surface_t * getCairoSurface();

private:

	void drawPath(const ofPath & path,bool is_subpath=false);

	deque<ofPoint> curvePoints;
	cairo_t * cr;
	cairo_surface_t * surface;

	stack<cairo_matrix_t> matrixStack;
	stack<ofMatrix4x4> projectionStack;
	stack<ofMatrix4x4> modelViewStack;
	cairo_matrix_t tmpMatrix;

	Type type;
	int page;

	ofMatrix4x4 projection;
	ofMatrix4x4 modelView;
};
