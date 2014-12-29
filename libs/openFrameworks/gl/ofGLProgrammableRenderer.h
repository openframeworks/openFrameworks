#pragma once
#include "ofBaseTypes.h"
#include "ofPolyline.h"
#include "ofMatrix4x4.h"
#include "ofShader.h"
#include "ofMatrixStack.h"
#include "ofVboMesh.h"


class ofShapeTessellation;
class ofMesh;
class ofFbo;
class ofVbo;
class ofMaterial;
static const int OF_NO_TEXTURE=-1;

class ofGLProgrammableRenderer: public ofBaseGLRenderer{
public:
	ofGLProgrammableRenderer(const ofAppBaseWindow * window);
	~ofGLProgrammableRenderer();

	void setup(const string & glslVersion);

    static const string TYPE;
	const string & getType(){ return TYPE; }
    
    void startRender();
    void finishRender();

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
	void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0);
	void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1);
	void setOrientation(ofOrientation orientation, bool vFlip);
	ofRectangle getCurrentViewport() const;
	ofRectangle getNativeViewport() const;
	int getViewportWidth() const;
	int getViewportHeight() const;
	bool isVFlipped() const;
    
	void setCoordHandedness(ofHandednessType handedness);
	ofHandednessType getCoordHandedness() const;
    
	//our openGL wrappers
	void pushMatrix();
	void popMatrix();
	void translate(float x, float y, float z = 0);
	void translate(const ofVec3f & p);
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

    /// \brief Queries the current OpenGL matrix state
    ///
    /// Returns the specified matrix as held by the renderer's current matrix
    /// stack.
    ///
    /// You can query one of the following:
    ///
    /// [OF_MATRIX_MODELVIEW | OF_MATRIX_PROJECTION | OF_MATRIX_TEXTURE]
    ///
    /// Each query will return the state of the matrix as it was uploaded to
    /// the shader currently bound.
    ///
    /// \param	matrixMode_ Which matrix mode to query
    /// \note   If an invalid matrixMode is queried, this method will return the
    ///         identity matrix, and print an error message.
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
	void setSphereResolution(int res);
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
	ofColor getBackgroundColor();
	void setBackgroundColor(const ofColor & c);
	void background(const ofColor & c);
	void background(float brightness);
	void background(int hexColor, float _a=255.0f);
	void background(int r, int g, int b, int a=255);

	bool getBackgroundAuto();
	void setBackgroundAuto(bool bManual);		// default is true
    
	void clear();
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

	const ofShader & getCurrentShader() const;

	void enableTextureTarget(int textureTarget, int textureID, int textureLocation);
	void disableTextureTarget(int textureTarget, int textureLocation);
	void setAlphaMaskTex(ofTexture & tex);
	void disableAlphaMask();
	GLenum getCurrentTextureTarget();

	void beginCustomShader(const ofShader & shader);
	void endCustomShader();

	void setCurrentMaterial(ofBaseMaterial * material);

	void setAttributes(bool vertices, bool color, bool tex, bool normals);
	void setAlphaBitmapText(bool bitmapText);

	ofShader & defaultTexRectColor() const;
	ofShader & defaultTexRectNoColor() const;
	ofShader & defaultTex2DColor() const;
	ofShader & defaultTex2DNoColor() const;
	ofShader & defaultNoTexColor() const;
	ofShader & defaultNoTexNoColor() const;
	ofShader & alphaMaskRectShader() const;
	ofShader & alphaMask2DShader() const;
	ofShader & bitmapStringShader() const;
	ofShader & defaultUniqueShader() const;
	ofShader & getShaderPlanarYUY2() const;
	ofShader & getShaderNV12() const;
	ofShader & getShaderNV21() const;
	ofShader & getShaderPlanarYUV() const;
	ofShader & getShaderPlanarYUY2Rect() const;
	ofShader & getShaderNV12Rect() const;
	ofShader & getShaderNV21Rect() const;
	ofShader & getShaderPlanarYUVRect() const;
	const ofShader * getVideoShader(const ofBaseVideoDraws & video) const;
	void setVideoShaderUniforms(const ofBaseVideoDraws & video, const ofShader & shader) const;

	void enableLighting(){};
	void disableLighting(){};
	void enableSeparateSpecularLight(){};
	void disableSeparateSpecularLight(){}
	bool getLightingEnabled(){return true;}
	void setSmoothLighting(bool b){}
	void setGlobalAmbientColor(const ofColor& c){}
	void enableLight(int lightIndex){}
	void disableLight(int lightIndex){}
	void setLightSpotlightCutOff(int lightIndex, float spotCutOff){}
	void setLightSpotConcentration(int lightIndex, float exponent){}
	void setLightAttenuation(int lightIndex, float constant, float linear, float quadratic ){}
	void setLightAmbientColor(int lightIndex, const ofFloatColor& c){}
	void setLightDiffuseColor(int lightIndex, const ofFloatColor& c){}
	void setLightSpecularColor(int lightIndex, const ofFloatColor& c){}
	void setLightPosition(int lightIndex, const ofVec4f & position){}
	void setLightSpotDirection(int lightIndex, const ofVec4f & direction){}

private:


	mutable ofPolyline circlePolyline;
#if defined(TARGET_OPENGLES) && !defined(TARGET_EMSCRIPTEN)
	mutable ofMesh circleMesh;
	mutable ofMesh triangleMesh;
	mutable ofMesh rectMesh;
	mutable ofMesh lineMesh;
	mutable ofVbo meshVbo;
#else
	mutable ofVboMesh circleMesh;
	mutable ofVboMesh triangleMesh;
	mutable ofVboMesh rectMesh;
	mutable ofVboMesh lineMesh;
	mutable ofVbo meshVbo;
	mutable ofVbo vertexDataVbo;
#endif

	void uploadCurrentMatrix();


	void startSmoothing();
	void endSmoothing();

	void beginDefaultShader();
	void uploadMatrices();
	void setDefaultUniforms();

    
	ofMatrixStack matrixStack;

	bool bBackgroundAuto;
	ofFloatColor bgColor;
    ofColor currentColor;
    
	ofFillFlag bFilled;
	bool bSmoothHinted;
	ofRectMode rectMode;
	
	const ofShader * currentShader;

	bool verticesEnabled, colorsEnabled, texCoordsEnabled, normalsEnabled, bitmapStringEnabled;
	bool usingCustomShader, settingDefaultShader;
	int currentTextureTarget;

	bool wrongUseLoggedOnce;
	bool uniqueShader;

	ofBaseMaterial * currentMaterial;
	int alphaMaskTextureTarget;
};
