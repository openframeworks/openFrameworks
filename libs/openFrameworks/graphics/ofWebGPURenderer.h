#pragma once

#include "ofConstants.h"
#if OF_USE_DAWN

#include "ofGraphicsBaseTypes.h"
#include "ofMatrixStack.h"
#include "of3dGraphics.h"
#include "ofPath.h"

#include <webgpu/webgpu_cpp.h>
#include <deque>
#include <vector>

class ofAppBaseWindow;

/// WebGPU renderer backed by Dawn (WGPU -> Metal on Apple).
/// Presents a CAMetalLayer, clears the background, and batches immediate-mode
/// lines, rectangles, triangles, circles, and ellipses. Bitmap strings,
/// meshes, textures, and shaders are not drawn yet.
class ofWebGPURenderer : public ofBaseRenderer {
public:
	static const std::string TYPE;

	ofWebGPURenderer(ofAppBaseWindow * window);
	~ofWebGPURenderer() override;

	const std::string & getType() override { return TYPE; }

	/// Attach a CAMetalLayer (void* to stay C++-safe) and create the device.
	bool setup(void * metalLayer, int width, int height, float contentsScale);
	void configureSurface(int width, int height);
	void shutdown();

	bool isSetup() const { return device_ != nullptr && surface_ != nullptr; }
	wgpu::Device getDevice() const { return device_; }
	wgpu::Queue getQueue() const { return queue_; }
	wgpu::Surface getSurface() const { return surface_; }
	void * getMetalLayer() const { return metalLayer_; }

	void startRender() override;
	void finishRender() override;

	using ofBaseRenderer::draw;
	void draw(const ofPolyline & poly) const override;
	void draw(const ofPath & shape) const override;
	void draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const override;
	void draw(const of3dPrimitive & model, ofPolyRenderMode renderType) const override;
	void draw(const ofNode & model) const override;
	void draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const override;
	void draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const override;
	void draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const override;
	void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const override;

	void pushView() override;
	void popView() override;
	void viewport(ofRectangle viewport) override;
	void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip = true) override;
	void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0) override;
	void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1) override;
	void setOrientation(ofOrientation orientation, bool vFlip) override;
	ofRectangle getCurrentViewport() const override;
	ofRectangle getNativeViewport() const override;
	int getViewportWidth() const override;
	int getViewportHeight() const override;
	bool isVFlipped() const override;
	void setCoordHandedness(ofHandednessType handedness) override;
	ofHandednessType getCoordHandedness() const override;

	void pushMatrix() override;
	void popMatrix() override;
	glm::mat4 getCurrentMatrix(ofMatrixMode matrixMode_) const override;
	glm::mat4 getCurrentOrientationMatrix() const override;
	void translate(float x, float y, float z = 0) override;
	void translate(const glm::vec3 & p) override;
	void scale(float xAmnt, float yAmnt, float zAmnt = 1) override;
	void rotateRad(float radians, float vecX, float vecY, float vecZ) override;
	void rotateXRad(float radians) override;
	void rotateYRad(float radians) override;
	void rotateZRad(float radians) override;
	void rotateRad(float radians) override;
	void matrixMode(ofMatrixMode mode) override;
	void loadIdentityMatrix() override;
	void loadMatrix(const glm::mat4 & m) override;
	void loadMatrix(const float * m) override;
	void multMatrix(const glm::mat4 & m) override;
	void multMatrix(const float * m) override;
	void loadViewMatrix(const glm::mat4 & m) override;
	void multViewMatrix(const glm::mat4 & m) override;
	glm::mat4 getCurrentViewMatrix() const override;
	glm::mat4 getCurrentNormalMatrix() const override;

	void bind(const ofCamera & camera, const ofRectangle & viewport) override;
	void unbind(const ofCamera & camera) override;
	void setupGraphicDefaults() override;
	void setupScreen() override;

	void setRectMode(ofRectMode mode) override;
	ofRectMode getRectMode() override;
	void setFillMode(ofFillFlag fill) override;
	ofFillFlag getFillMode() override;
	void setLineWidth(float lineWidth) override;
	void setPointSize(float pointSize) override;
	void setDepthTest(bool depthTest) override;
	void setBlendMode(ofBlendMode blendMode) override;
	void setLineSmoothing(bool smooth) override;
	void setCircleResolution(int res) override;
	void enableAntiAliasing() override;
	void disableAntiAliasing() override;

	void setColor(float r, float g, float b) override;
	void setColor(float r, float g, float b, float a) override;
	void setColor(const ofFloatColor & color) override;
	void setColor(const ofFloatColor & color, float _a) override;
	void setColor(float gray) override;
	void setHexColor(int hexColor) override;
	void setBitmapTextMode(ofDrawBitmapMode mode) override;

	ofFloatColor getBackgroundColor() override;
	void setBackgroundColor(const ofFloatColor & c) override;
	void background(const ofFloatColor & c) override;
	void background(float brightness) override;
	void background(int hexColor, int _a = 255) override;
	void background(float r, float g, float b, float a = 1.f) override;
	void setBackgroundAuto(bool bManual) override;
	bool getBackgroundAuto() override;
	void clear() override;
	void clear(float r, float g, float b, float a = 0) override;
	void clear(float brightness, float a = 0) override;
	void clearAlpha() override;

	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const override;
	void drawRectangle(float x, float y, float z, float w, float h) const override;
	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const override;
	void drawCircle(float x, float y, float z, float radius) const override;
	void drawEllipse(float x, float y, float z, float width, float height) const override;
	void drawString(std::string text, float x, float y, float z) const override;
	void drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const override;

	ofPath & getPath() override;
	ofStyle getStyle() const override;
	void setStyle(const ofStyle & style) override;
	void pushStyle() override;
	void popStyle() override;
	void setCurveResolution(int resolution) override;
	void setPolyMode(ofPolyWindingMode mode) override;
	const of3dGraphics & get3dGraphics() const override;
	of3dGraphics & get3dGraphics() override;

private:
	void applyClear(float r, float g, float b, float a);
	void logDrawOnce(const char * what) const;
	void ensurePipelines();
	void flushBatches();
	void beginBatch(bool lines) const;
	void pushVert(float x, float y, float z) const;
	void rebuildCircle(int res) const;
	glm::mat4 gpuMvp() const;

	struct GpuVertex {
		float x, y, z;
		float r, g, b, a;
	};
	struct DrawBatch {
		uint32_t first = 0;
		uint32_t count = 0;
		int pipeline = 0;
		glm::mat4 mvp { 1.f };
	};
	struct FrameSlot {
		wgpu::Buffer vertices;
		wgpu::Buffer uniforms;
		uint64_t vertexBytes = 0;
		uint64_t uniformBytes = 0;
	};

	ofAppBaseWindow * window_ = nullptr;
	void * metalLayer_ = nullptr;
	int width_ = 0;
	int height_ = 0;
	float contentsScale_ = 1.f;

	wgpu::Instance instance_;
	wgpu::Adapter adapter_;
	wgpu::Device device_;
	wgpu::Queue queue_;
	wgpu::Surface surface_;
	wgpu::TextureFormat surfaceFormat_ = wgpu::TextureFormat::BGRA8Unorm;

	wgpu::CommandEncoder encoder_;
	wgpu::RenderPassEncoder pass_;
	bool inFrame_ = false;
	bool surfaceConfigured_ = false;

	ofMatrixStack matrixStack;
	of3dGraphics graphics3d;
	ofPath path;
	ofStyle currentStyle;
	std::deque<ofStyle> styleHistory;
	bool bBackgroundAuto = true;
	bool bDepthTest_ = false;
	mutable bool loggedDraw_ = false;

	mutable std::vector<GpuVertex> gpuVerts_;
	mutable std::vector<DrawBatch> batches_;
	mutable std::vector<glm::vec3> circleUnit_;
	wgpu::BindGroupLayout bindGroupLayout_;
	wgpu::RenderPipeline pipelines_[4];
	bool pipelinesReady_ = false;
	FrameSlot slots_[3];
	int slotIndex_ = 0;
};

#endif // OF_USE_DAWN
