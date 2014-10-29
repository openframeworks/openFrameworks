#pragma once
#include "ofBaseTypes.h"
#include "ofPolyline.h"
#include "ofMatrix4x4.h"
#include <stack>
#include "of3dGraphics.h"
#include "ofBitmapFont.h"
#include "ofMatrixStack.h"
#include "ofPath.h"
#include <set>

class ofShapeTessellation;
class ofMesh;
class ofFbo;
class of3dPrimitive;

class ofGLRenderer: public ofBaseGLRenderer{
public:
	ofGLRenderer(const ofAppBaseWindow * window);
	~ofGLRenderer(){}

    static const string TYPE;
    const string & getType(){ return TYPE; }

    void setup();

	void startRender();
	void finishRender();
	void update();
	using ofBaseRenderer::draw;
	using ofBaseGLRenderer::draw;
	void draw(const ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals) const;
	void draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const;
    void draw(const of3dPrimitive& model, ofPolyRenderMode renderType) const;
    void draw(const ofNode& model) const;
	void draw(const ofPolyline & poly) const;
	void draw(const ofPath & path) const;
	void draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofTexture & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const;
	void draw(const ofVbo & vbo, GLuint drawMode, int first, int total) const;
	void drawElements(const ofVbo & vbo, GLuint drawMode, int amt) const;
	void drawInstanced(const ofVbo & vbo, GLuint drawMode, int first, int total, int primCount) const;
	void drawElementsInstanced(const ofVbo & vbo, GLuint drawMode, int amt, int primCount) const;
	void draw(const ofVboMesh & mesh, ofPolyRenderMode renderType) const;
	void drawInstanced(const ofVboMesh & mesh, ofPolyRenderMode renderType, int primCount) const;
	ofPath & getPath();



	//--------------------------------------------
	// transformations
	void pushView();
	void popView();

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport);
	void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=true);
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

	void setBitmapTextMode(ofDrawBitmapMode mode);

	// bg color
	ofColor getBackgroundColor();
	void setBackgroundColor(const ofColor & c);
	void background(const ofColor & c);
	void background(float brightness);
	void background(int hexColor, float _a=255.0f);
	void background(int r, int g, int b, int a=255);

	void setBackgroundAuto(bool bManual);		// default is true
	bool getBackgroundAuto();

	void clear();
	void clear(float r, float g, float b, float a=0);
	void clear(float brightness, float a=0);
	void clearAlpha();

	ofStyle getStyle() const;
	void pushStyle();
	void popStyle();
	void setStyle(const ofStyle & style);
	void setCurveResolution(int resolution);
	void setPolyMode(ofPolyWindingMode mode);


	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const;
	void drawRectangle(float x, float y, float z, float w, float h) const;
	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const;
	void drawCircle(float x, float y, float z, float radius) const;
	void drawEllipse(float x, float y, float z, float width, float height) const;
	void drawString(string text, float x, float y, float z) const;
	void drawString(const ofTrueTypeFont & font, string text, float x, float y) const;


	// gl specifics
	void enableTextureTarget(const ofTexture & tex, int textureLocation);
	void disableTextureTarget(int textureTarget, int textureLocation);
	void setAlphaMaskTex(const ofTexture & tex);
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


	void bind(const ofBaseVideoDraws & video);
	void bind(const ofBaseMaterial & material);
	void bind(const ofFbo & fbo, bool setupPerspective);
	void bind(const ofShader & shader);
	void bind(const ofTexture & texture, int location);
	void bind(const ofCamera & camera, const ofRectangle & viewport);
	void unbind(const ofBaseVideoDraws & video);
	void unbind(const ofBaseMaterial & material);
	void unbind(const ofFbo & fbo);
	void unbind(const ofShader & shader);
	void unbind(const ofTexture & texture, int location);
	void unbind(const ofCamera & camera);

	int getGLVersionMajor();
	int getGLVersionMinor();

	void saveScreen(int x, int y, int w, int h, ofPixels & pixels);
	void saveFullViewport(ofPixels & pixels);

	const of3dGraphics & get3dGraphics() const;
	of3dGraphics & get3dGraphics();
private:
	void startSmoothing();
	void endSmoothing();


	bool bBackgroundAuto;

	mutable vector<ofPoint> linePoints;
	mutable vector<ofPoint> rectPoints;
	mutable vector<ofPoint> triPoints;
	mutable vector<ofPoint> circlePoints;
	ofPolyline circlePolyline;

	ofMatrixStack matrixStack;
	bool normalsEnabled;
	bool lightingEnabled;
	set<int> textureLocationsEnabled;

	int alphaMaskTextureTarget;

	ofStyle currentStyle;
	deque <ofStyle> styleHistory;
	of3dGraphics graphics3d;
	ofBitmapFont bitmapFont;
	ofPath path;
	const ofAppBaseWindow * window;
};
