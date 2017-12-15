#pragma once

#include "ofGraphicsBaseTypes.h"
#include "of3dGraphics.h"
#include "ofPath.h"

class ofRendererCollection: public ofBaseRenderer{
public:
	 ofRendererCollection():graphics3d(this){}
	 ~ofRendererCollection(){}

	 static const std::string TYPE;
	 const std::string & getType(){ return TYPE; }

	 std::shared_ptr<ofBaseGLRenderer> getGLRenderer();

	 bool rendersPathPrimitives();

	 void startRender();

	 void finishRender();


	 using ofBaseRenderer::draw;

	 void draw(const ofPolyline & poly) const;
	 void draw(const ofPath & shape) const;

	 void draw(const ofMesh & vertexData, ofPolyRenderMode mode, bool useColors, bool useTextures, bool useNormals) const;

	void draw(const  of3dPrimitive& model, ofPolyRenderMode renderType ) const ;

	void draw(const  ofNode& node) const ;

	void draw(const ofImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;

	void draw(const ofFloatImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;

	void draw(const ofShortImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;

	void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const;

	/*void bind(const ofBaseVideoDraws & video) const{
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->bind(video);
		}
	}

	void unbind(const ofBaseVideoDraws & video) const{
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->unbind(video);
		}
	}*/


	glm::mat4 getCurrentMatrix(ofMatrixMode matrixMode_) const;


	glm::mat4 getCurrentOrientationMatrix() const;

	glm::mat4 getCurrentNormalMatrix() const;

	//--------------------------------------------
	// transformations
	 void pushView();

	 void popView();
	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport);

	 void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=true);

	 void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0);

	 void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1);

	 ofRectangle getCurrentViewport() const;

	 ofRectangle getNativeViewport() const;

	 int getViewportWidth() const;
	 int getViewportHeight() const;

	 void setCoordHandedness(ofHandednessType handedness);
	 ofHandednessType getCoordHandedness() const;

	//our openGL wrappers
	 void pushMatrix();
	 void popMatrix();
	 void translate(float x, float y, float z = 0);
	 void translate(const glm::vec3 & p);
	 void scale(float xAmnt, float yAmnt, float zAmnt = 1);

	 void rotateDeg(float degrees, float vecX, float vecY, float vecZ);
	 void rotateXDeg(float degrees);
	 void rotateYDeg(float degrees);
	 void rotateZDeg(float degrees);
	 void rotateDeg(float degrees);

	 void rotateRad(float radians, float vecX, float vecY, float vecZ);
	 void rotateXRad(float radians);
	 void rotateYRad(float radians);
	 void rotateZRad(float radians);
	 void rotateRad(float radians);
	void loadIdentityMatrix (void);

	void loadMatrix (const glm::mat4 & m);

	void loadMatrix (const float * m);

	void multMatrix (const glm::mat4 & m);

	void multMatrix (const float * m);

	void setOrientation(ofOrientation orientation, bool vflip);

	bool isVFlipped() const;

	void matrixMode(ofMatrixMode mode);

	void loadViewMatrix(const glm::mat4& m);

	void multViewMatrix(const glm::mat4& m);

	glm::mat4 getCurrentViewMatrix() const;


	// screen coordinate things / default gl values
	 void setupGraphicDefaults();

	 void setupScreen();

	// color options
	void setColor(int r, int g, int b);

	void setColor(int r, int g, int b, int a);

	void setColor(const ofColor & color);

	void setColor(const ofColor & color, int _a);

	void setColor(int gray);

	void setHexColor( int hexColor );

	// bg color
	ofColor getBackgroundColor();

	void setBackgroundColor(const ofColor & color);

	bool getBackgroundAuto();

	void background(const ofColor & c);

	void background(float brightness);

	void background(int hexColor, float _a=255.0f);

	void background(int r, int g, int b, int a=255);

	void setBackgroundAuto(bool bManual);

	void clear();

	void clear(float r, float g, float b, float a=0);

	void clear(float brightness, float a=0);

	void clearAlpha();

	// drawing modes
	void setRectMode(ofRectMode mode);

	ofRectMode getRectMode();

	void setFillMode(ofFillFlag fill);

	ofFillFlag getFillMode();

	void setLineWidth(float lineWidth);

	void setDepthTest(bool depthTest);

	void setBlendMode(ofBlendMode blendMode);
	void setLineSmoothing(bool smooth);
	void setCircleResolution(int res);
	void enablePointSprites();
	void disablePointSprites();

	void enableAntiAliasing();

	void disableAntiAliasing();

	void setBitmapTextMode(ofDrawBitmapMode mode);

	ofStyle getStyle() const;

	void pushStyle();

	void popStyle();

	void setStyle(const ofStyle & style);

	void setCurveResolution(int res);

	void setPolyMode(ofPolyWindingMode mode);

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const;

	void drawRectangle(float x, float y, float z, float w, float h) const;

	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const;

	void drawCircle(float x, float y, float z, float radius) const;

	void drawEllipse(float x, float y, float z, float width, float height) const;

	void drawString(std::string text, float x, float y, float z) const;

	void drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const;

	virtual void bind(const ofCamera & camera, const ofRectangle & viewport);
	virtual void unbind(const ofCamera & camera);

	const of3dGraphics & get3dGraphics() const;

	of3dGraphics & get3dGraphics();

	ofPath & getPath();

	std::vector<std::shared_ptr<ofBaseRenderer> > renderers;
	of3dGraphics graphics3d;
	ofPath path;
};
