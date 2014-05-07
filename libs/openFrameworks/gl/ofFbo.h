#pragma once

#include "ofTexture.h"

class ofFbo : public ofBaseDraws, public ofBaseHasTexture {
public:
	struct Settings;

	ofFbo();
	ofFbo(const ofFbo & mom);
	ofFbo & operator=(const ofFbo & fbo);
	virtual ~ofFbo();

	void allocate(int width, int height, int internalformat = GL_RGBA, int numSamples = 0);
	//void allocateForShadow( int width, int height );
	void allocate(Settings settings = Settings());
	bool isAllocated();

	using ofBaseDraws::draw;
	void draw(float x, float y);
	void draw(float x, float y, float width, float height);

	void setAnchorPercent(float xPct, float yPct);
    void setAnchorPoint(float x, float y);
	void resetAnchor();

	void setDefaultTextureIndex(int defaultTexture);
	int getDefaultTextureIndex();

	ofTexture & getTextureReference();
	ofTexture & getTextureReference(int attachmentPoint);
	ofTexture & getDepthTexture();
	void setUseTexture(bool bUseTex){ /*irrelevant*/ };

	void begin(bool setupScreen=true);
	void end();

	void readToPixels(ofPixels & pixels, int attachmentPoint = 0);
	void readToPixels(ofShortPixels & pixels, int attachmentPoint = 0);
	void readToPixels(ofFloatPixels & pixels, int attachmentPoint = 0);

	float getWidth();
	float getHeight();

	// advanced functions
	void bind();
	void unbind();

	bool checkStatus();
	void createAndAttachTexture(GLenum internalFormat, GLenum attachmentPoint);
    void attachTexture(ofTexture & texture, GLenum internalFormat, GLenum attachmentPoint);
	GLuint createAndAttachRenderbuffer(GLenum internalFormat, GLenum attachmentPoint);
	void createAndAttachDepthStencilTexture(GLenum target, GLint internalformat, GLenum attachment );
	void createAndAttachDepthStencilTexture(GLenum target, GLint internalformat, GLenum attachment, GLenum transferFormat, GLenum transferType );
	
	int	getNumTextures();

	void setActiveDrawBuffer(int i);
	void setActiveDrawBuffers(const vector<int>& i);
	void activateAllDrawBuffers();

	GLuint getFbo();	// returns GLuint of Fbo for advanced actions


	static bool	checkGLSupport();
	static int maxColorAttachments();	// return max color attachments
	static int maxDrawBuffers();		// return max simultaneous draw buffers
	static int maxSamples();			// return max MSAA samples

	GLuint getDepthBuffer(){ return depthBuffer; }
	GLuint getStencilBuffer(){ return stencilBuffer; }

	struct Settings {
		int		width;					// width of images attached to fbo
		int		height;					// height of images attached to fbo
		int		numColorbuffers;		// how many color buffers to create
		vector<GLint> colorFormats;		// format of the color attachments for MRT.
		bool	useDepth;				// whether to use depth buffer or not
		bool	useStencil;				// whether to use stencil buffer or not
		bool	depthStencilAsTexture;			// use a texture instead of a renderbuffer for depth (useful to draw it or use it in a shader later)
		GLenum	textureTarget;			// GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB
		GLint	internalformat;			// GL_RGBA, GL_RGBA16F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB etc.
		GLint	depthStencilInternalFormat; 	// GL_DEPTH_COMPONENT(16/24/32)
		int		wrapModeHorizontal;		// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
		int		wrapModeVertical;		// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
		int		minFilter;				// GL_NEAREST, GL_LINEAR etc.
		int		maxFilter;				// GL_NEAREST, GL_LINEAR etc.
		int		numSamples;				// number of samples for multisampling (set 0 to disable)

		Settings();
		bool operator!=(const Settings & other);
	};
private:
	Settings 			settings;
	int					isBound;

	GLuint				fbo;			// main fbo which we bind for drawing into, all renderbuffers are attached to this
	GLuint				fboTextures;	// textures are attached to this (if MSAA is disabled, this is equal to fbo, otherwise it's a new fbo)
	GLuint				depthBuffer;
	GLuint				stencilBuffer;

	GLint				savedFramebuffer;	// save bound framebuffer before switching

	vector<GLuint>		colorBuffers;	// only used if using MSAA...maybe...what about MRT?
	vector<ofTexture>	textures;

	ofTexture			depthBufferTex;

	static int			_maxColorAttachments;
	static int			_maxDrawBuffers;
	static int			_maxSamples;

	bool				dirty;

	int 				defaultTextureIndex; //used for getTextureReference
	bool				bIsAllocated;

#ifdef TARGET_OPENGLES
	static bool bglFunctionsInitialized;
#endif

	void destroy();

	// if using MSAA, we will have rendered into a colorbuffer, not directly into the texture
	// call this to blit from the colorbuffer into the texture so we can use the results for rendering, or input to a shader etc.
	void updateTexture(int attachmentPoint);
};

