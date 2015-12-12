#pragma once

#include "ofTexture.h"
#include <stack>

class ofFbo : public ofBaseDraws, public ofBaseHasTexture {
public:
	struct Settings;

	ofFbo();
	ofFbo(const ofFbo & mom);
	ofFbo & operator=(const ofFbo & fbo);
    ofFbo(ofFbo && mom);
    ofFbo & operator=(ofFbo && fbo);
	virtual ~ofFbo();

	void allocate(int width, int height, int internalformat = GL_RGBA, int numSamples = 0);
	//void allocateForShadow( int width, int height );
	void allocate(Settings settings = Settings(nullptr));
	bool isAllocated() const;

	OF_DEPRECATED_MSG("Use clear instead",void destroy());
	void clear();

	using ofBaseDraws::draw;
	void draw(float x, float y) const;
	void draw(float x, float y, float width, float height) const;

	void setAnchorPercent(float xPct, float yPct);
    void setAnchorPoint(float x, float y);
	void resetAnchor();

	void setDefaultTextureIndex(int defaultTexture);
	int getDefaultTextureIndex() const;

	OF_DEPRECATED_MSG("Use getTexture",ofTexture & getTextureReference());
	OF_DEPRECATED_MSG("Use getTexture",ofTexture & getTextureReference(int attachmentPoint));
	ofTexture & getTexture();
	ofTexture & getTexture(int attachmentPoint);
	ofTexture & getDepthTexture();
	OF_DEPRECATED_MSG("Use getTexture",const ofTexture & getTextureReference() const);
	OF_DEPRECATED_MSG("Use getTexture",const ofTexture & getTextureReference(int attachmentPoint) const);
	const ofTexture & getTexture() const ;
	const ofTexture & getTexture(int attachmentPoint) const;
	const ofTexture & getDepthTexture() const;
	void setUseTexture(bool){ /*irrelevant*/ };
	bool isUsingTexture() const {return true;}

	/// \brief    Sets up the framebuffer and binds it for rendering.
	/// \warning  This is a convenience method, and is considered unsafe 
	///           in multi-window and/or multi-renderer scenarios.
	///           If you use more than one renderer, use each renderer's
	///           explicit void ofBaseGLRenderer::begin(const ofFbo & fbo, bool setupPerspective) 
	///           method instead.
	/// \sa       void ofBaseGLRenderer::begin(const ofFbo & fbo, bool setupPerspective) 
	void begin(bool setupScreen=true) const;

	/// \brief    Ends the current framebuffer render context.
	/// \sa       void begin(bool setupScreen=true) const;
	void end() const;

	void readToPixels(ofPixels & pixels, int attachmentPoint = 0) const;
	void readToPixels(ofShortPixels & pixels, int attachmentPoint = 0) const;
	void readToPixels(ofFloatPixels & pixels, int attachmentPoint = 0) const;

	float getWidth() const;
	float getHeight() const;

	// advanced functions

	/// \brief    Bind OpenGL GL_FRAMEBUFFER target to this ofFbo
	/// \warning  If you use this method, you need to manually keep track of the
	///           currently bound framebuffer, if you ever want to restore state.
	///           * use ofBaseGLRenderer::getCurrentFramebuffer() to query the current
	///           framebuffer binding state within the renderer.
	///           * Better, use the renderer's explicit method:
	///           ofBaseGLRenderer::bind(const ofFbo & fbo) to bind the fbo, to allow
	///           the renderer to keep track of any bound fbos.
	/// \sa       unbind()
	/// \sa       virtual void ofBaseGLRenderer::bind(const ofFbo & fbo)
	void bind() const;

	/// \brief    Unbinds OpenGL framebuffer target and restores the OpenGL framebuffer 
	///           render target to whatever this ofFbo stores in previousFramebufferBinding.
	/// \sa       bind()
	/// \sa       void setPreviousFramebufferBinding(const GLuint& previousFramebufferBinding_) const
	void unbind() const;

	void flagDirty() const; ///< check whether attached MSAA buffers need updating

	/// \brief    Explicityl resolve MSAA render buffers into textures 
	/// \note     if using MSAA, we will have rendered into a colorbuffer, not directly 
	///           into the texture call this to blit from the colorbuffer into the texture 
	///           so we can use the results for rendering, or input to a shader etc.
	/// \note     This will get called implicitly upon getTexture();
	void updateTexture(int attachmentPoint);


	bool checkStatus() const;
	void createAndAttachTexture(GLenum internalFormat, GLenum attachmentPoint);
    void attachTexture(ofTexture & texture, GLenum internalFormat, GLenum attachmentPoint);
	GLuint createAndAttachRenderbuffer(GLenum internalFormat, GLenum attachmentPoint);
	void createAndAttachDepthStencilTexture(GLenum target, GLint internalformat, GLenum attachment );
	void createAndAttachDepthStencilTexture(GLenum target, GLint internalformat, GLenum attachment, GLenum transferFormat, GLenum transferType );
	
	int	getNumTextures() const;

	void setActiveDrawBuffer(int i);
	void setActiveDrawBuffers(const vector<int>& i);
	void activateAllDrawBuffers();

	OF_DEPRECATED_MSG("Use getId()", GLuint getFbo() const);

	/// returns id of the underlying GL object for advanced actions
	GLuint getId() const;

	/// returns id of Fbo for texture attachments
	/// which is different when the fbo is using MSAA
	GLuint getIdDrawBuffer() const;

	static bool	checkGLSupport();
	static int maxColorAttachments();	// return max color attachments
	static int maxDrawBuffers();		// return max simultaneous draw buffers
	static int maxSamples();			// return max MSAA samples

	GLuint getDepthBuffer() const { return depthBuffer; }
	GLuint getStencilBuffer() const { return stencilBuffer; }

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
		Settings(std::shared_ptr<ofBaseGLRenderer> renderer=nullptr);
		bool operator!=(const Settings & other);
	private:
		std::weak_ptr<ofBaseGLRenderer> renderer;
		friend class ofFbo;
	};
private:
	Settings 			settings;

	GLuint				fbo;			// main fbo which we bind for drawing into, all renderbuffers are attached to this
	GLuint				fboTextures;	// textures are attached to this (if MSAA is disabled, this is equal to fbo, otherwise it's a new fbo)
	GLuint				depthBuffer;
	GLuint				stencilBuffer;

	vector<GLuint>		colorBuffers;
	vector<ofTexture>	textures;			

	ofTexture			depthBufferTex;

	static int			_maxColorAttachments;
	static int			_maxDrawBuffers;
	static int			_maxSamples;

	vector<GLenum>		activeDrawBuffers;  ///< table of currently active color draw buffers, allocate() defaults it to size(textures), with GL_COLOR_ATTACHMENT0..n as members, in order of allocation

	/// \brief  Flags used internally to keep track of MSAA renderbuffers / textures
	/// \note   The dirty flags are only used when dealing if the framebuffer has MSAA 
	///         enabled attachments, i.e. numSamples is > 0 and extra Textures have
	///         been bound so that the multisampled renderbuffers can be resolved to 
	///         textures.
	///         The flags are read whenever an attached texture is accessed. If the texture
	///         is dirty, i.e. it has not yet been resolved from its associated renderbuffer
	///         the texture will be resolved through blitting the renderbuffer into it.
	mutable vector<bool> dirty;

	int 				defaultTextureIndex; //used for getTextureReference
	bool				bIsAllocated;
	void reloadFbo();
#ifdef TARGET_OPENGLES
	static bool bglFunctionsInitialized;
#endif

};

