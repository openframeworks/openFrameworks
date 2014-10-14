#pragma once
#include "ofBaseTypes.h"
#include "ofPolyline.h"
#include "ofMatrix4x4.h"
#include <stack>
#include "ofGraphics.h"
#include "ofMatrixStack.h"
#include <set>

class ofShapeTessellation;
class ofMesh;
class ofFbo;
class of3dPrimitive;

class ofGLRenderer: public ofBaseGLRenderer{
public:
	ofGLRenderer(bool useShapeColor=true);
	~ofGLRenderer(){}

    static const string TYPE;
    const string & getType(){ return TYPE; }

	void setCurrentFBO(const ofFbo * fbo);

	void update();
	using ofBaseRenderer::draw;
	void draw(const ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals) const;
	void draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const;
    void draw(const of3dPrimitive& model, ofPolyRenderMode renderType) const;
	void draw(const ofPolyline & poly) const;
	void draw(const ofPath & path) const;
	void draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const;

	void bind(const ofBaseVideoDraws & video) const;
	void unbind(const ofBaseVideoDraws & video) const;
	bool rendersPathPrimitives(){
		return false;
	}



	//--------------------------------------------
	// transformations
	void pushView();
	void popView();

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport);
	void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=ofIsVFlipped());
	void setOrientation(ofOrientation orientation, bool vFlip);
	void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0);
	void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1);
	ofRectangle getCurrentViewport() const;
	ofRectangle getNativeViewport() const;
	int getViewportWidth() const;
	int getViewportHeight() const;
	bool isVFlipped() const;
	bool texturesNeedVFlip() const;

	void setCoordHandedness(ofHandednessType handedness);
	ofHandednessType getCoordHandedness() const;

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
	void matrixMode(ofMatrixMode mode);
	void loadIdentityMatrix (void);
	void loadMatrix (const ofMatrix4x4 & m);
	void loadMatrix (const float * m);
	void multMatrix (const ofMatrix4x4 & m);
	void multMatrix (const float * m);
	void loadViewMatrix(const ofMatrix4x4 & m);
	void multViewMatrix(const ofMatrix4x4 & m);

	ofMatrix4x4 getCurrentMatrix(ofMatrixMode matrixMode_) const;
	ofMatrix4x4 getCurrentOrientationMatrix() const;
	ofMatrix4x4 getCurrentViewMatrix() const;
	ofMatrix4x4 getCurrentNormalMatrix() const;
	
	// screen coordinate things / default gl values
	void setupGraphicDefaults();
	void setupScreen();

	// drawing modes
	void setFillMode(ofFillFlag fill);
	ofFillFlag getFillMode();
	void setCircleResolution(int res);
	void setRectMode(ofRectMode mode);
	ofRectMode getRectMode();
	void setLineWidth(float lineWidth);
	void setDepthTest(bool depthTest);
	void setLineSmoothing(bool smooth);
	void setBlendMode(ofBlendMode blendMode);
	void enablePointSprites();
	void disablePointSprites();
	void enableAntiAliasing();
	void disableAntiAliasing();

	// color options
	void setColor(int r, int g, int b); // 0-255
	void setColor(int r, int g, int b, int a); // 0-255
	void setColor(const ofColor & color);
	void setColor(const ofColor & color, int _a);
	void setColor(int gray); // new set a color as grayscale with one argument
	void setHexColor( int hexColor ); // hex, like web 0xFF0033;

	// bg color
	ofFloatColor & getBgColor();
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
	//	void drawSphere(float x, float y, float z, float radius);
	void drawEllipse(float x, float y, float z, float width, float height);
	void drawString(string text, float x, float y, float z, ofDrawBitmapMode mode);


	// gl specifics
	void enableTextureTarget(int textureTarget, int textureID, int textureLocation);
	void disableTextureTarget(int textureTarget, int textureLocation);
	void setAlphaMaskTex(ofTexture & tex);
	void disableAlphaMask();

	// lighting globals
	void enableLighting();
	void disableLighting();
	void enableSeparateSpecularLight();
	void disableSeparateSpecularLight();
	bool getLightingEnabled();
	void setSmoothLighting(bool b);
	void setGlobalAmbientColor(const ofColor& c);

	// lighting per light
	void enableLight(int lightIndex);
	void disableLight(int lightIndex);
	void setLightSpotlightCutOff(int lightIndex, float spotCutOff);
	void setLightSpotConcentration(int lightIndex, float exponent);
	void setLightAttenuation(int lightIndex, float constant, float linear, float quadratic );
	void setLightAmbientColor(int lightIndex, const ofFloatColor& c);
	void setLightDiffuseColor(int lightIndex, const ofFloatColor& c);
	void setLightSpecularColor(int lightIndex, const ofFloatColor& c);
	void setLightPosition(int lightIndex, const ofVec4f & position);
	void setLightSpotDirection(int lightIndex, const ofVec4f & direction);


	void setCurrentMaterial(ofBaseMaterial * material){}

private:
	void startSmoothing();
	void endSmoothing();


	bool bBackgroundAuto;
	ofFloatColor bgColor;

	vector<ofPoint> linePoints;
	vector<ofPoint> rectPoints;
	vector<ofPoint> triPoints;
	vector<ofPoint> circlePoints;
	ofPolyline circlePolyline;

	ofFillFlag fillFlag;
	bool bSmoothHinted;
	ofRectMode rectMode;

	ofMatrixStack matrixStack;
	bool normalsEnabled;
	bool lightingEnabled;
	set<int> textureLocationsEnabled;

	int alphaMaskTextureTarget;
};
