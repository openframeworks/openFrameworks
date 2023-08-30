#pragma once

#include "ofGLBaseTypes.h"
#include "ofPolyline.h"
#include "ofShader.h"
#include "ofMatrixStack.h"
#include "of3dGraphics.h"
#include "ofBitmapFont.h"
#include "ofPath.h"
//#include "ofMaterialBaseTypes.h"

class ofShapeTessellation;
class ofFbo;
class ofVbo;
static const int OF_NO_TEXTURE=-1;

class ofGLProgrammableRenderer: public ofBaseGLRenderer{
public:
    ofGLProgrammableRenderer(const ofAppBaseWindow * window);

	void setup(int glVersionMajor, int glVersionMinor);

    static const std::string TYPE;
	const std::string & getType(){ return TYPE; }
    
    void startRender();
    void finishRender();

	using ofBaseRenderer::draw;
	using ofBaseGLRenderer::draw;
	void draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const;
    void draw(const of3dPrimitive& model, ofPolyRenderMode renderType) const;
    void draw(const ofNode& node) const;
	void draw(const ofPolyline & poly) const;
	void draw(const ofPath & path) const;
	void draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofTexture & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
    void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const;
	void draw(const ofVbo & vbo, GLuint drawMode, int first, int total) const;
	void drawElements(const ofVbo & vbo, GLuint drawMode, int amt, int offsetelements = 0) const;
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
	void translate(const glm::vec3 & p);
	void scale(float xAmnt, float yAmnt, float zAmnt = 1);
	void rotateRad(float radians, float vecX, float vecY, float vecZ);
	void rotateXRad(float radians);
	void rotateYRad(float radians);
	void rotateZRad(float radians);
	void rotateRad(float radians);
	void matrixMode(ofMatrixMode mode);
	void loadIdentityMatrix (void);
	void loadMatrix (const glm::mat4 & m);
	void loadMatrix (const float * m);
	void multMatrix (const glm::mat4 & m);
	void multMatrix (const float * m);
	void loadViewMatrix(const glm::mat4 & m);
	void multViewMatrix(const glm::mat4 & m);

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
	glm::mat4 getCurrentMatrix(ofMatrixMode matrixMode_) const;
	glm::mat4 getCurrentOrientationMatrix() const;
	glm::mat4 getCurrentViewMatrix() const;
	glm::mat4 getCurrentNormalMatrix() const;
	glm::mat4 getCurrentModelMatrix() const;
	
	glm::vec3 getCurrentEyePosition() const;
	
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

	bool getBackgroundAuto();
	void setBackgroundAuto(bool bManual);		// default is true
    
	void clear();
	void clear(float r, float g, float b, float a=0);
	void clear(float brightness, float a=0);
	void clearAlpha();
    
    
	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const;
	void drawRectangle(float x, float y, float z, float w, float h) const;
	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const;
	void drawCircle(float x, float y, float z, float radius) const;
	void drawEllipse(float x, float y, float z, float width, float height) const;
	void drawString(std::string text, float x, float y, float z) const;
	void drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const;


	void enableTextureTarget(const ofTexture & tex, int textureLocation);
	void disableTextureTarget(int textureTarget, int textureLocation);
	void setAlphaMaskTex(const ofTexture & tex);
	void disableAlphaMask();
	GLenum getCurrentTextureTarget();

	const ofShader & getCurrentShader() const;

	void bind(const ofBaseMaterial & material);
	void bind(const ofShadow & shadow);
	void bind(const ofShadow & shadow, GLenum aCubeFace);
	void bind(const ofShader & shader);
	void bind(const ofTexture & texture, int location);
	void bind(const ofBaseVideoDraws & video);
	void bind(const ofCamera & camera, const ofRectangle & viewport);
	void unbind(const ofBaseMaterial & material);
	void unbind(const ofShadow & shadow);
	void unbind(const ofShadow & shadow, GLenum aCubeFace);
	void unbind(const ofShader & shader);
	void unbind(const ofTexture & texture, int location);
	void unbind(const ofBaseVideoDraws & video);
	void unbind(const ofCamera & camera);

	void bind(const ofFbo & fbo);
#ifndef TARGET_OPENGLES
	void bindForBlitting(const ofFbo & fboSrc, ofFbo & fboDst, int attachmentPoint);
#endif
	void unbind(const ofFbo & fbo);

    void begin(const ofFbo & fbo, ofFboMode mode);
	void end(const ofFbo & fbo);

	ofStyle getStyle() const;
	void pushStyle();
	void popStyle();
	void setStyle(const ofStyle & style);
	void setCurveResolution(int resolution);
	void setPolyMode(ofPolyWindingMode mode);

	const ofShader * getVideoShader(const ofBaseVideoDraws & video) const;
	void setVideoShaderUniforms(const ofBaseVideoDraws & video, const ofShader & shader) const;

    void enableLighting();
    void disableLighting();
    bool getLightingEnabled();
    void enableSeparateSpecularLight(){}
    void disableSeparateSpecularLight(){}
	void setSmoothLighting(bool b){}
	void setGlobalAmbientColor(const ofColor& c){}
    void enableLight(int lightIndex);
    void disableLight(int lightIndex);
	void setLightSpotlightCutOff(int lightIndex, float spotCutOff){}
	void setLightSpotConcentration(int lightIndex, float exponent){}
	void setLightAttenuation(int lightIndex, float constant, float linear, float quadratic ){}
	void setLightAmbientColor(int lightIndex, const ofFloatColor& c){}
	void setLightDiffuseColor(int lightIndex, const ofFloatColor& c){}
	void setLightSpecularColor(int lightIndex, const ofFloatColor& c){}
	void setLightPosition(int lightIndex, const glm::vec4 & position){}
	void setLightSpotDirection(int lightIndex, const glm::vec4 & direction){}

	std::string defaultVertexShaderHeader(GLenum textureTarget);
	std::string defaultFragmentShaderHeader(GLenum textureTarget);

	int getGLVersionMajor();
	int getGLVersionMinor();

	void saveScreen(int x, int y, int w, int h, ofPixels & pixels);
	void saveFullViewport(ofPixels & pixels);

	const of3dGraphics & get3dGraphics() const;
	of3dGraphics & get3dGraphics();

private:


	ofPolyline circlePolyline;
	mutable ofMesh circleMesh;
	mutable ofMesh triangleMesh;
	mutable ofMesh rectMesh;
	mutable ofMesh lineMesh;
	mutable ofVbo meshVbo;

	void uploadCurrentMatrix();


	void startSmoothing();
	void endSmoothing();

	void beginDefaultShader();
	void uploadMatrices();
	void setDefaultUniforms();

	void setAttributes(bool vertices, bool color, bool tex, bool normals);
	void setAlphaBitmapText(bool bitmapText);

    
	ofMatrixStack matrixStack;

	bool bBackgroundAuto;
	int major, minor;
	
	const ofShader * currentShader;

	bool verticesEnabled, colorsEnabled, texCoordsEnabled, normalsEnabled, bitmapStringEnabled;
	bool usingCustomShader, settingDefaultShader, usingVideoShader;
	int currentTextureTarget;

	bool wrongUseLoggedOnce;
	bool uniqueShader;

	const ofBaseMaterial * currentMaterial;
	int alphaMaskTextureTarget;
	
	const ofShadow* currentShadow;
	bool bIsShadowDepthPass;
	GLenum shadowCubeFace;
	bool bCustomShadowShader = false;

	ofStyle currentStyle;
	std::deque <ofStyle> styleHistory;
	of3dGraphics graphics3d;
	ofBitmapFont bitmapFont;
	ofPath path;
	const ofAppBaseWindow * window;

	ofShader defaultTexRectColor;
	ofShader defaultTexRectNoColor;
	ofShader defaultTex2DColor;
	ofShader defaultTex2DNoColor;
	ofShader defaultNoTexColor;
	ofShader defaultNoTexNoColor;
	ofShader defaultUniqueShader;
#ifdef TARGET_ANDROID
	ofShader defaultOESTexColor;
	ofShader defaultOESTexNoColor;
#endif
	
	ofShader alphaMaskRectShader;
	ofShader alphaMask2DShader;
	
	ofShader bitmapStringShader;
	
	ofShader shaderPlanarYUY2;
	ofShader shaderNV12;
	ofShader shaderNV21;
	ofShader shaderPlanarYUV;
	ofShader shaderPlanarYUY2Rect;
	ofShader shaderNV12Rect;
	ofShader shaderNV21Rect;
	ofShader shaderPlanarYUVRect;
	
	glm::vec3 currentEyePos;

	//void setDefaultFramebufferId(const GLuint& fboId_); ///< windowing systems might use this to set the default framebuffer for this renderer.

	//void pushFramebufferId(); // pushes currentFramebuffer onto framebufferStack
	//const GLuint& popFramebufferId(); /// returns topmost element in framebufferIdStack or 0, removes topmost element from stack.
	//const GLuint& getFramebufferId(); ///< returns current target bound to GL_FRAMEBUFFER_BINDING, initially set to defaultFramebufferId
	//void setFramebufferId(const GLuint& fboId_); // sets the current framebuffer id

	// framebuffer binding state
	std::deque<GLuint> framebufferIdStack;	///< keeps track of currently bound framebuffers
	GLuint defaultFramebufferId;		///< default GL_FRAMEBUFFER_BINDING, windowing frameworks might want to set this to their MSAA framebuffer, defaults to 0
    GLuint currentFramebufferId;		///< the framebuffer id currently bound to the GL_FRAMEBUFFER target
};
