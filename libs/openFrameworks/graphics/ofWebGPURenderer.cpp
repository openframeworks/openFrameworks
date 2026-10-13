#include "ofWebGPURenderer.h"
#if OF_USE_DAWN

#include "ofAppBaseWindow.h"
#include "ofCamera.h"
#include "ofLog.h"
#include "ofNode.h"
#include "ofPath.h"
#include "ofTrueTypeFont.h"
#include "ofImage.h"
#include "of3dPrimitives.h"
#include "ofMesh.h"
#include "ofVideoBaseTypes.h"

#include <webgpu/webgpu.h>
#include "ofPolyline.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

const std::string ofWebGPURenderer::TYPE = "WebGPU";

ofWebGPURenderer::ofWebGPURenderer(ofAppBaseWindow * window)
	: window_(window)
	, matrixStack(window)
	, graphics3d(this) {
	currentStyle = ofStyle();
}

ofWebGPURenderer::~ofWebGPURenderer() {
	shutdown();
}

void ofWebGPURenderer::shutdown() {
	if(inFrame_ && pass_) {
		pass_.End();
		pass_ = nullptr;
		inFrame_ = false;
	}
	encoder_ = nullptr;
	queue_ = nullptr;
	device_ = nullptr;
	adapter_ = nullptr;
	surface_ = nullptr;
	instance_ = nullptr;
	surfaceConfigured_ = false;
	pipelinesReady_ = false;
	for(auto & pipeline : pipelines_) {
		pipeline = nullptr;
	}
	bindGroupLayout_ = nullptr;
	whiteTex_ = nullptr;
	whiteView_ = nullptr;
	activeView_ = nullptr;
	sampler_ = nullptr;
	depthTex_ = nullptr;
	depthView_ = nullptr;
	texCache_.clear();
	gpuVerts_.clear();
	batches_.clear();
	for(auto & slot : slots_) {
		slot.vertices = nullptr;
		slot.uniforms = nullptr;
		slot.vertexBytes = 0;
		slot.uniformBytes = 0;
	}
}

bool ofWebGPURenderer::setup(void * metalLayer, int width, int height, float contentsScale) {
	shutdown();
	metalLayer_ = metalLayer;
	width_ = std::max(1, width);
	height_ = std::max(1, height);
	contentsScale_ = contentsScale > 0.f ? contentsScale : 1.f;

	if(!metalLayer_) {
		ofLogError("ofWebGPURenderer") << "setup: CAMetalLayer is null";
		return false;
	}

	wgpu::InstanceDescriptor instanceDesc = {};
	wgpu::InstanceFeatureName features[] = { wgpu::InstanceFeatureName::TimedWaitAny };
	instanceDesc.requiredFeatureCount = 1;
	instanceDesc.requiredFeatures = features;
	instance_ = wgpu::CreateInstance(&instanceDesc);
	if(!instance_) {
		ofLogError("ofWebGPURenderer") << "wgpuCreateInstance failed";
		return false;
	}

	wgpu::SurfaceSourceMetalLayer metalSrc;
	metalSrc.layer = metalLayer_;
	wgpu::SurfaceDescriptor surfDesc;
	surfDesc.nextInChain = &metalSrc;
	surface_ = instance_.CreateSurface(&surfDesc);
	if(!surface_) {
		ofLogError("ofWebGPURenderer") << "CreateSurface failed";
		return false;
	}

	wgpu::RequestAdapterOptions adapterOpts = {};
	adapterOpts.compatibleSurface = surface_;
	adapterOpts.powerPreference = wgpu::PowerPreference::HighPerformance;

	struct AdapterResult {
		wgpu::Adapter adapter;
		bool done = false;
		std::string message;
	} adapterResult;

	auto adapterFuture = instance_.RequestAdapter(
		&adapterOpts,
		wgpu::CallbackMode::WaitAnyOnly,
		[&adapterResult](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message) {
			if(status == wgpu::RequestAdapterStatus::Success) {
				adapterResult.adapter = adapter;
			} else if(message.data) {
				adapterResult.message = (message.length == WGPU_STRLEN) ? std::string(message.data) : std::string(message.data, message.length);
			}
			adapterResult.done = true;
		});
	instance_.WaitAny(adapterFuture, UINT64_MAX);
	if(!adapterResult.adapter) {
		ofLogError("ofWebGPURenderer") << "RequestAdapter failed: " << adapterResult.message;
		return false;
	}
	adapter_ = adapterResult.adapter;

	struct DeviceResult {
		wgpu::Device device;
		bool done = false;
		std::string message;
	} deviceResult;

	wgpu::DeviceDescriptor deviceDesc = {};
	deviceDesc.SetUncapturedErrorCallback([](const wgpu::Device &, wgpu::ErrorType type, wgpu::StringView message) {
		std::string msg = message.data ? ((message.length == WGPU_STRLEN) ? std::string(message.data) : std::string(message.data, message.length)) : "";
		ofLogError("Dawn") << "error " << (uint32_t)type << ": " << msg;
	});
	auto deviceFuture = adapter_.RequestDevice(
		&deviceDesc,
		wgpu::CallbackMode::WaitAnyOnly,
		[&deviceResult](wgpu::RequestDeviceStatus status, wgpu::Device device, wgpu::StringView message) {
			if(status == wgpu::RequestDeviceStatus::Success) {
				deviceResult.device = device;
			} else if(message.data) {
				deviceResult.message = (message.length == WGPU_STRLEN) ? std::string(message.data) : std::string(message.data, message.length);
			}
			deviceResult.done = true;
		});
	instance_.WaitAny(deviceFuture, UINT64_MAX);
	if(!deviceResult.device) {
		ofLogError("ofWebGPURenderer") << "RequestDevice failed: " << deviceResult.message;
		return false;
	}
	device_ = deviceResult.device;
	queue_ = device_.GetQueue();

	configureSurface(width_, height_);
	setupGraphicDefaults();
	ofLogNotice("ofWebGPURenderer") << "Dawn device ready " << width_ << "x" << height_ << " @" << contentsScale_;
	return true;
}

void ofWebGPURenderer::configureSurface(int width, int height) {
	if(!device_ || !surface_) {
		return;
	}
	width_ = std::max(1, width);
	height_ = std::max(1, height);

	wgpu::SurfaceCapabilities caps = {};
	surface_.GetCapabilities(adapter_, &caps);
	if(caps.formatCount > 0) {
		surfaceFormat_ = caps.formats[0];
	}

	wgpu::SurfaceConfiguration config = {};
	config.device = device_;
	config.format = surfaceFormat_;
	config.usage = wgpu::TextureUsage::RenderAttachment;
	config.alphaMode = wgpu::CompositeAlphaMode::Opaque;
	config.width = static_cast<uint32_t>(width_);
	config.height = static_cast<uint32_t>(height_);
	wgpu::PresentMode mode = wgpu::PresentMode::Fifo;
	if(!vsync_) {
		bool immediate = false;
		bool mailbox = false;
		for(size_t i = 0; i < caps.presentModeCount; i++) {
			if(caps.presentModes[i] == wgpu::PresentMode::Immediate) {
				immediate = true;
			} else if(caps.presentModes[i] == wgpu::PresentMode::Mailbox) {
				mailbox = true;
			}
		}
		if(immediate) {
			mode = wgpu::PresentMode::Immediate;
		} else if(mailbox) {
			mode = wgpu::PresentMode::Mailbox;
		}
	}
	config.presentMode = mode;
	surface_.Configure(&config);
	surfaceConfigured_ = true;
	matrixStack.viewport(0, 0, width_, height_, true);
	depthTex_ = nullptr;
	depthView_ = nullptr;
}

void ofWebGPURenderer::setPresentVSync(bool enabled) {
	if(vsync_ == enabled) {
		return;
	}
	vsync_ = enabled;
	if(surfaceConfigured_) {
		configureSurface(width_, height_);
	}
}

void ofWebGPURenderer::startRender() {
	if(!isSetup() || inFrame_) {
		return;
	}
	if(!surfaceConfigured_) {
		configureSurface(width_, height_);
	}

	wgpu::SurfaceTexture surfaceTexture;
	surface_.GetCurrentTexture(&surfaceTexture);
	if(surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal &&
	   surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessSuboptimal) {
		ofLogVerbose("ofWebGPURenderer") << "GetCurrentTexture status " << (uint32_t)surfaceTexture.status;
		configureSurface(width_, height_);
		surface_.GetCurrentTexture(&surfaceTexture);
		if(surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal &&
		   surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessSuboptimal) {
			return;
		}
	}

	encoder_ = device_.CreateCommandEncoder();
	const ofFloatColor bg = currentStyle.bgColor;
	beginColorPass(surfaceTexture.texture.CreateView(), true, bg.r, bg.g, bg.b, bg.a);
	inFrame_ = true;
}

void ofWebGPURenderer::finishRender() {
	if(!inFrame_) {
		return;
	}
	flushBatches();
	if(pass_) {
		pass_.End();
		pass_ = nullptr;
	}
	if(encoder_ && queue_) {
		wgpu::CommandBuffer cmd = encoder_.Finish();
		queue_.Submit(1, &cmd);
	}
	encoder_ = nullptr;
	if(surface_) {
		surface_.Present();
	}
	inFrame_ = false;
}

void ofWebGPURenderer::applyClear(float r, float g, float b, float a) {
	currentStyle.bgColor.set(r, g, b, a);
	if(!inFrame_) {
		return;
	}
	flushBatches();
	// Mid-frame clear: end the current pass and start a new one with this color.
	if(pass_) {
		pass_.End();
		pass_ = nullptr;
	}
	wgpu::SurfaceTexture surfaceTexture;
	surface_.GetCurrentTexture(&surfaceTexture);
	if(!surfaceTexture.texture) {
		return;
	}
	beginColorPass(surfaceTexture.texture.CreateView(), true, r, g, b, a);
}


namespace {

uint64_t hashBytes(const void * data, size_t bytes) {
	const auto * p = static_cast<const unsigned char *>(data);
	uint64_t h = 14695981039346656037ull;
	for(size_t i = 0; i < bytes; i++) {
		h ^= p[i];
		h *= 1099511628211ull;
	}
	return h;
}

template <typename T>
unsigned char channelTo8(T v) {
	if constexpr(std::is_same<T, unsigned char>::value) {
		return v;
	} else if constexpr(std::is_floating_point<T>::value) {
		float c = static_cast<float>(v);
		if(c < 0.f) {
			c = 0.f;
		} else if(c > 1.f) {
			c = 1.f;
		}
		return static_cast<unsigned char>(c * 255.f + 0.5f);
	} else {
		float c = static_cast<float>(v) / 65535.f;
		if(c < 0.f) {
			c = 0.f;
		} else if(c > 1.f) {
			c = 1.f;
		}
		return static_cast<unsigned char>(c * 255.f + 0.5f);
	}
}

template <typename T>
bool pixelsToRGBA(const ofPixels_<T> & pix, std::vector<unsigned char> & rgba, int & width, int & height) {
	if(!pix.isAllocated() || pix.getWidth() == 0 || pix.getHeight() == 0) {
		return false;
	}
	width = static_cast<int>(pix.getWidth());
	height = static_cast<int>(pix.getHeight());
	const int channels = static_cast<int>(pix.getNumChannels());
	if(channels < 1 || channels > 4) {
		return false;
	}
	const bool bgr = pix.getPixelFormat() == OF_PIXELS_BGR || pix.getPixelFormat() == OF_PIXELS_BGRA;
	rgba.resize(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
	const T * src = pix.getData();
	for(int i = 0; i < width * height; i++) {
		const T * p = src + static_cast<size_t>(i) * channels;
		unsigned char r = 255, g = 255, b = 255, a = 255;
		if(channels == 1) {
			r = g = b = channelTo8(p[0]);
		} else if(channels == 2) {
			r = g = b = channelTo8(p[0]);
			a = channelTo8(p[1]);
		} else if(channels == 3) {
			r = channelTo8(p[0]);
			g = channelTo8(p[1]);
			b = channelTo8(p[2]);
		} else {
			r = channelTo8(p[0]);
			g = channelTo8(p[1]);
			b = channelTo8(p[2]);
			a = channelTo8(p[3]);
		}
		if(bgr) {
			std::swap(r, b);
		}
		rgba[static_cast<size_t>(i) * 4 + 0] = r;
		rgba[static_cast<size_t>(i) * 4 + 1] = g;
		rgba[static_cast<size_t>(i) * 4 + 2] = b;
		rgba[static_cast<size_t>(i) * 4 + 3] = a;
	}
	return true;
}

}

void ofWebGPURenderer::ensureDepth() {
	if(!device_ || width_ <= 0 || height_ <= 0 || depthView_) {
		return;
	}
	wgpu::TextureDescriptor desc = {};
	desc.usage = wgpu::TextureUsage::RenderAttachment;
	desc.dimension = wgpu::TextureDimension::e2D;
	desc.size = { static_cast<uint32_t>(width_), static_cast<uint32_t>(height_), 1 };
	desc.format = wgpu::TextureFormat::Depth24Plus;
	desc.mipLevelCount = 1;
	desc.sampleCount = 1;
	depthTex_ = device_.CreateTexture(&desc);
	if(depthTex_) {
		depthView_ = depthTex_.CreateView();
	}
}

void ofWebGPURenderer::beginColorPass(wgpu::TextureView colorView, bool clear, float r, float g, float b, float a) {
	ensureDepth();
	wgpu::RenderPassColorAttachment color = {};
	color.view = colorView;
	color.loadOp = clear ? wgpu::LoadOp::Clear : wgpu::LoadOp::Load;
	color.storeOp = wgpu::StoreOp::Store;
	color.clearValue = { r, g, b, a };

	wgpu::RenderPassDepthStencilAttachment depth = {};
	depth.view = depthView_;
	depth.depthLoadOp = clear ? wgpu::LoadOp::Clear : wgpu::LoadOp::Load;
	depth.depthStoreOp = wgpu::StoreOp::Store;
	depth.depthClearValue = 1.f;

	wgpu::RenderPassDescriptor passDesc = {};
	passDesc.colorAttachmentCount = 1;
	passDesc.colorAttachments = &color;
	if(depthView_) {
		passDesc.depthStencilAttachment = &depth;
	}
	pass_ = encoder_.BeginRenderPass(&passDesc);
}

void ofWebGPURenderer::logDrawOnce(const char * what) const {
	if(!loggedDraw_) {
		ofLogVerbose("ofWebGPURenderer") << "draw(" << what << ") is not implemented";
		loggedDraw_ = true;
	}
}

void ofWebGPURenderer::pushView() { matrixStack.pushView(); }
void ofWebGPURenderer::popView() { matrixStack.popView(); }
void ofWebGPURenderer::viewport(ofRectangle vp) { matrixStack.viewport(vp.x, vp.y, vp.width, vp.height, isVFlipped()); }
void ofWebGPURenderer::viewport(float x, float y, float width, float height, bool vflip) {
	if(width < 0) {
		width = width_;
	}
	if(height < 0) {
		height = height_;
	}
	matrixStack.viewport(x, y, width, height, vflip);
}
void ofWebGPURenderer::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist) {
	float viewW = width < 0 ? getViewportWidth() : width;
	float viewH = height < 0 ? getViewportHeight() : height;
	if(viewH < 1.f) {
		viewH = 1.f;
	}
	float eyeX = viewW / 2.f;
	float eyeY = viewH / 2.f;
	float halfFov = glm::pi<float>() * fov / 360.0f;
	float dist = eyeY / tanf(halfFov);
	float aspect = viewW / viewH;
	if(nearDist == 0) {
		nearDist = dist / 10.0f;
	}
	if(farDist == 0) {
		farDist = dist * 10.0f;
	}
	matrixMode(OF_MATRIX_PROJECTION);
	loadMatrix(glm::perspective(glm::radians(fov), aspect, nearDist, farDist));
	matrixMode(OF_MATRIX_MODELVIEW);
	loadViewMatrix(glm::lookAt(glm::vec3(eyeX, eyeY, dist), glm::vec3(eyeX, eyeY, 0), glm::vec3(0, 1, 0)));
}
void ofWebGPURenderer::setupScreenOrtho(float width, float height, float nearDist, float farDist) {
	float viewW = width < 0 ? getViewportWidth() : width;
	float viewH = height < 0 ? getViewportHeight() : height;
	matrixMode(OF_MATRIX_PROJECTION);
	loadMatrix(glm::ortho(0.f, viewW, 0.f, viewH, nearDist, farDist));
	matrixMode(OF_MATRIX_MODELVIEW);
	loadViewMatrix(glm::mat4(1.0));
}
void ofWebGPURenderer::setOrientation(ofOrientation orientation, bool vFlip) { matrixStack.setOrientation(orientation, vFlip); }
ofRectangle ofWebGPURenderer::getCurrentViewport() const { return matrixStack.getCurrentViewport(); }
ofRectangle ofWebGPURenderer::getNativeViewport() const { return matrixStack.getNativeViewport(); }
int ofWebGPURenderer::getViewportWidth() const { return getCurrentViewport().width; }
int ofWebGPURenderer::getViewportHeight() const { return getCurrentViewport().height; }
bool ofWebGPURenderer::isVFlipped() const { return matrixStack.isVFlipped(); }
void ofWebGPURenderer::setCoordHandedness(ofHandednessType) {}
ofHandednessType ofWebGPURenderer::getCoordHandedness() const { return matrixStack.getHandedness(); }

void ofWebGPURenderer::pushMatrix() { matrixStack.pushMatrix(); }
void ofWebGPURenderer::popMatrix() { matrixStack.popMatrix(); }
glm::mat4 ofWebGPURenderer::getCurrentMatrix(ofMatrixMode) const { return matrixStack.getCurrentMatrix(); }
glm::mat4 ofWebGPURenderer::getCurrentOrientationMatrix() const { return matrixStack.getOrientationMatrix(); }
void ofWebGPURenderer::translate(float x, float y, float z) { matrixStack.translate(x, y, z); }
void ofWebGPURenderer::translate(const glm::vec3 & p) { matrixStack.translate(p.x, p.y, p.z); }
void ofWebGPURenderer::scale(float xAmnt, float yAmnt, float zAmnt) { matrixStack.scale(xAmnt, yAmnt, zAmnt); }
void ofWebGPURenderer::rotateRad(float radians, float vecX, float vecY, float vecZ) { matrixStack.rotateRad(radians, vecX, vecY, vecZ); }
void ofWebGPURenderer::rotateXRad(float radians) { matrixStack.rotateRad(radians, 1, 0, 0); }
void ofWebGPURenderer::rotateYRad(float radians) { matrixStack.rotateRad(radians, 0, 1, 0); }
void ofWebGPURenderer::rotateZRad(float radians) { matrixStack.rotateRad(radians, 0, 0, 1); }
void ofWebGPURenderer::rotateRad(float radians) { matrixStack.rotateRad(radians, 0, 0, 1); }
void ofWebGPURenderer::matrixMode(ofMatrixMode mode) { matrixStack.matrixMode(mode); }
void ofWebGPURenderer::loadIdentityMatrix() { matrixStack.loadIdentityMatrix(); }
void ofWebGPURenderer::loadMatrix(const glm::mat4 & m) { matrixStack.loadMatrix(m); }
void ofWebGPURenderer::loadMatrix(const float * m) { matrixStack.loadMatrix(glm::make_mat4(m)); }
void ofWebGPURenderer::multMatrix(const glm::mat4 & m) { matrixStack.multMatrix(m); }
void ofWebGPURenderer::multMatrix(const float * m) { matrixStack.multMatrix(glm::make_mat4(m)); }
void ofWebGPURenderer::loadViewMatrix(const glm::mat4 & m) { matrixStack.loadViewMatrix(m); }
void ofWebGPURenderer::multViewMatrix(const glm::mat4 & m) { matrixStack.multViewMatrix(m); }
glm::mat4 ofWebGPURenderer::getCurrentViewMatrix() const { return matrixStack.getViewMatrix(); }
glm::mat4 ofWebGPURenderer::getCurrentNormalMatrix() const { return glm::inverseTranspose(glm::mat3(matrixStack.getModelViewMatrix())); }

void ofWebGPURenderer::bind(const ofCamera &, const ofRectangle &) {}
void ofWebGPURenderer::unbind(const ofCamera &) {}

void ofWebGPURenderer::setupGraphicDefaults() {
	matrixStack.setRenderSurface(*window_);
	matrixStack.loadIdentityMatrix();
	currentStyle = ofStyle();
}

void ofWebGPURenderer::setupScreen() {
	setupScreenPerspective();
}

void ofWebGPURenderer::setRectMode(ofRectMode mode) { currentStyle.rectMode = mode; }
ofRectMode ofWebGPURenderer::getRectMode() { return currentStyle.rectMode; }
void ofWebGPURenderer::setFillMode(ofFillFlag fill) { currentStyle.bFill = (fill == OF_FILLED); }
ofFillFlag ofWebGPURenderer::getFillMode() { return currentStyle.bFill ? OF_FILLED : OF_OUTLINE; }
void ofWebGPURenderer::setLineWidth(float lineWidth) { currentStyle.lineWidth = lineWidth; }
void ofWebGPURenderer::setPointSize(float pointSize) { currentStyle.pointSize = pointSize; }
void ofWebGPURenderer::setDepthTest(bool depthTest) { bDepthTest_ = depthTest; }
void ofWebGPURenderer::setBlendMode(ofBlendMode blendMode) { currentStyle.blendingMode = blendMode; }
void ofWebGPURenderer::setLineSmoothing(bool smooth) { currentStyle.smoothing = smooth; }
void ofWebGPURenderer::setCircleResolution(int res) {
	currentStyle.circleResolution = res;
	rebuildCircle(res);
}
void ofWebGPURenderer::enableAntiAliasing() {}
void ofWebGPURenderer::disableAntiAliasing() {}

void ofWebGPURenderer::setColor(float r, float g, float b) { setColor(r, g, b, 1.f); }
void ofWebGPURenderer::setColor(float r, float g, float b, float a) { currentStyle.color.set(r, g, b, a); }
void ofWebGPURenderer::setColor(const ofFloatColor & color) { currentStyle.color = color; }
void ofWebGPURenderer::setColor(const ofFloatColor & color, float _a) {
	currentStyle.color = color;
	currentStyle.color.a = _a;
}
void ofWebGPURenderer::setColor(float gray) { setColor(gray, gray, gray); }
void ofWebGPURenderer::setHexColor(int hexColor) { currentStyle.color.setHex(hexColor); }
void ofWebGPURenderer::setBitmapTextMode(ofDrawBitmapMode mode) { currentStyle.drawBitmapMode = mode; }

ofFloatColor ofWebGPURenderer::getBackgroundColor() { return currentStyle.bgColor; }
void ofWebGPURenderer::setBackgroundColor(const ofFloatColor & c) { currentStyle.bgColor = c; }
void ofWebGPURenderer::background(const ofFloatColor & c) {
	setBackgroundColor(c);
	applyClear(c.r, c.g, c.b, c.a);
}
void ofWebGPURenderer::background(float brightness) { background(ofFloatColor(brightness, brightness, brightness)); }
void ofWebGPURenderer::background(int hexColor, int _a) {
	ofFloatColor c;
	c.setHex(hexColor, _a / 255.f);
	background(c);
}
void ofWebGPURenderer::background(float r, float g, float b, float a) { background(ofFloatColor(r, g, b, a)); }
void ofWebGPURenderer::setBackgroundAuto(bool bManual) { bBackgroundAuto = bManual; }
bool ofWebGPURenderer::getBackgroundAuto() { return bBackgroundAuto; }
void ofWebGPURenderer::clear() { applyClear(0, 0, 0, 0); }
void ofWebGPURenderer::clear(float r, float g, float b, float a) { applyClear(r, g, b, a); }
void ofWebGPURenderer::clear(float brightness, float a) { applyClear(brightness, brightness, brightness, a); }
void ofWebGPURenderer::clearAlpha() {}

void ofWebGPURenderer::draw(const ofPolyline & poly) const {
	const auto & verts = poly.getVertices();
	if(verts.size() < 2) {
		return;
	}
	beginBatch(2);
	for(size_t i = 0; i + 1 < verts.size(); i++) {
		pushVert(verts[i].x, verts[i].y, verts[i].z);
		pushVert(verts[i + 1].x, verts[i + 1].y, verts[i + 1].z);
	}
	if(poly.isClosed() && verts.front() != verts.back()) {
		pushVert(verts.back().x, verts.back().y, verts.back().z);
		pushVert(verts.front().x, verts.front().y, verts.front().z);
	}
}

void ofWebGPURenderer::draw(const ofPath & shape) const {
	ofFloatColor prev = currentStyle.color;
	auto * mut = const_cast<ofWebGPURenderer *>(this);
	if(shape.isFilled()) {
		if(shape.getUseShapeColor()) {
			mut->setColor(shape.getFillColor());
		}
		draw(shape.getTessellation(), OF_MESH_FILL, false, false, false);
	}
	if(shape.hasOutline()) {
		const float lineWidth = currentStyle.lineWidth;
		if(shape.getUseShapeColor()) {
			mut->setColor(shape.getStrokeColor());
		}
		mut->setLineWidth(shape.getStrokeWidth());
		const std::vector<ofPolyline> & outlines = shape.getOutline();
		for(const ofPolyline & line : outlines) {
			draw(line);
		}
		mut->setLineWidth(lineWidth);
	}
	if(shape.getUseShapeColor()) {
		mut->setColor(prev);
	}
}

void ofWebGPURenderer::draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool) const {
	drawMesh(vertexData, renderType, useColors, useTextures);
}

void ofWebGPURenderer::draw(const of3dPrimitive & model, ofPolyRenderMode renderType) const {
	auto * mut = const_cast<ofWebGPURenderer *>(this);
	mut->pushMatrix();
	mut->multMatrix(model.getGlobalTransformMatrix());
	const ofMesh & mesh = model.getMesh();
	drawMesh(mesh, renderType, mesh.usingColors(), mesh.usingTextures());
	mut->popMatrix();
}

void ofWebGPURenderer::draw(const ofNode & node) const {
	auto * mut = const_cast<ofWebGPURenderer *>(this);
	mut->pushMatrix();
	mut->multMatrix(node.getGlobalTransformMatrix());
	const_cast<ofNode &>(node).customDraw(this);
	mut->popMatrix();
}

void ofWebGPURenderer::drawTexturedQuad(float x, float y, float z, float w, float h, float u0, float v0, float u1, float v1, const ofTexture & tex) const {
	if(w == 0.f || h == 0.f) {
		return;
	}
	float x0 = x;
	float y0 = y;
	float x1 = x + w;
	float y1 = y + h;
	if(currentStyle.rectMode == OF_RECTMODE_CENTER) {
		x0 -= w * 0.5f;
		y0 -= h * 0.5f;
		x1 -= w * 0.5f;
		y1 -= h * 0.5f;
	}
	const glm::vec2 anchor = tex.getAnchor();
	const float ax = tex.isAnchorPercent() ? anchor.x * w : anchor.x;
	const float ay = tex.isAnchorPercent() ? anchor.y * h : anchor.y;
	x0 -= ax;
	y0 -= ay;
	x1 -= ax;
	y1 -= ay;
	if(tex.texData.bFlipTexture == isVFlipped()) {
		std::swap(y0, y1);
	}
	const ofFloatColor color = currentStyle.color;
	beginBatch(0);
	pushVert(x0, y0, z, color, u0, v0);
	pushVert(x1, y0, z, color, u1, v0);
	pushVert(x1, y1, z, color, u1, v1);
	pushVert(x0, y0, z, color, u0, v0);
	pushVert(x1, y1, z, color, u1, v1);
	pushVert(x0, y1, z, color, u0, v1);
}

template <typename PixelType>
void ofWebGPURenderer::drawImagePixels(const ofImage_<PixelType> & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const {
	if(!image.isAllocated()) {
		return;
	}
	const auto & pix = image.getPixels();
	std::vector<unsigned char> rgba;
	int pw = 0;
	int ph = 0;
	if(!pixelsToRGBA(pix, rgba, pw, ph) || pw <= 0 || ph <= 0) {
		return;
	}
	if(sw == 0.f) {
		sw = static_cast<float>(pw);
	}
	if(sh == 0.f) {
		sh = static_cast<float>(ph);
	}
	const float u0 = sx / static_cast<float>(pw);
	const float v0 = sy / static_cast<float>(ph);
	const float u1 = (sx + sw) / static_cast<float>(pw);
	const float v1 = (sy + sh) / static_cast<float>(ph);
	const uint64_t stamp = hashBytes(pix.getData(), pix.size() * sizeof(PixelType)) ^ (static_cast<uint64_t>(pw) << 32) ^ static_cast<uint64_t>(ph);
	activeView_ = uploadRGBA(&image, rgba.data(), pw, ph, stamp);
	drawTexturedQuad(x, y, z, w, h, u0, v0, u1, v1, image.getTexture());
	activeView_ = whiteView_;
}

void ofWebGPURenderer::draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const {
	drawImagePixels(image, x, y, z, w, h, sx, sy, sw, sh);
}

void ofWebGPURenderer::draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const {
	drawImagePixels(image, x, y, z, w, h, sx, sy, sw, sh);
}

void ofWebGPURenderer::draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const {
	drawImagePixels(image, x, y, z, w, h, sx, sy, sw, sh);
}

void ofWebGPURenderer::draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const {
	if(!video.isInitialized()) {
		return;
	}
	const ofPixels & pix = video.getPixels();
	std::vector<unsigned char> rgba;
	int pw = 0;
	int ph = 0;
	if(!pixelsToRGBA(pix, rgba, pw, ph)) {
		return;
	}
	const uint64_t stamp = hashBytes(pix.getData(), pix.size()) ^ (static_cast<uint64_t>(pw) << 32) ^ static_cast<uint64_t>(ph);
	activeView_ = uploadRGBA(&video, rgba.data(), pw, ph, stamp);
	ofTexture placeholder;
	drawTexturedQuad(x, y, 0.f, w, h, 0.f, 0.f, 1.f, 1.f, placeholder);
	activeView_ = whiteView_;
}

void ofWebGPURenderer::drawMesh(const ofMesh & mesh, ofPolyRenderMode renderType, bool useColors, bool useTextures) const {
	const auto & verts = mesh.getVertices();
	const size_t nVerts = verts.size();
	if(nVerts == 0) {
		return;
	}
	const bool indexed = mesh.getNumIndices() > 0 && renderType != OF_MESH_POINTS;
	const ofIndexType * indices = indexed ? mesh.getIndexPointer() : nullptr;
	const size_t n = indexed ? mesh.getNumIndices() : nVerts;
	auto nth = [&](size_t i) -> size_t {
		return indexed ? static_cast<size_t>(indices[i]) : i;
	};

	const ofPrimitiveMode mode = mesh.getMode();
	const bool points = renderType == OF_MESH_POINTS || mode == OF_PRIMITIVE_POINTS;
	const bool triangleLike = mode == OF_PRIMITIVE_TRIANGLES || mode == OF_PRIMITIVE_TRIANGLE_STRIP || mode == OF_PRIMITIVE_TRIANGLE_FAN;
	const bool lineLike = mode == OF_PRIMITIVE_LINES || mode == OF_PRIMITIVE_LINE_STRIP || mode == OF_PRIMITIVE_LINE_LOOP;

	const bool colored = useColors && mesh.usingColors() && mesh.getNumColors() > 0;
	const ofFloatColor * cols = colored ? mesh.getColorsPointer() : nullptr;
	const size_t nCols = colored ? mesh.getNumColors() : 0;
	const bool textured = useTextures && mesh.usingTextures() && mesh.getNumTexCoords() > 0 && activeView_ && whiteView_ && activeView_.Get() != whiteView_.Get();
	const glm::vec2 * tcs = textured ? mesh.getTexCoordsPointer() : nullptr;
	const size_t nT = textured ? mesh.getNumTexCoords() : 0;

	auto emit = [&](size_t vi) {
		const auto & p = verts[vi];
		ofFloatColor color = currentStyle.color;
		if(cols && vi < nCols) {
			color = cols[vi];
		}
		float u = 0.f;
		float v = 0.f;
		if(tcs && vi < nT) {
			u = tcs[vi].x;
			v = tcs[vi].y;
		}
		pushVert(p.x, p.y, p.z, color, u, v);
	};
	auto emit2 = [&](size_t a, size_t b) {
		const size_t ia = nth(a);
		const size_t ib = nth(b);
		if(ia >= nVerts || ib >= nVerts) {
			return;
		}
		emit(ia);
		emit(ib);
	};
	auto emit3 = [&](size_t a, size_t b, size_t c) {
		const size_t ia = nth(a);
		const size_t ib = nth(b);
		const size_t ic = nth(c);
		if(ia >= nVerts || ib >= nVerts || ic >= nVerts) {
			return;
		}
		emit(ia);
		emit(ib);
		emit(ic);
	};

	if(points) {
		const float ps = std::max(currentStyle.pointSize, 1.f);
		const bool expand = pointSprites_ || ps > 1.01f;
		if(!expand) {
			beginBatch(4);
			for(size_t i = 0; i < nVerts; i++) {
				emit(i);
			}
			return;
		}
		// WebGPU points are 1px. Expand to a screen-space quad so point size and
		// point sprites match the GL path. Identity MVP consumes NDC positions.
		const glm::mat4 mvp = gpuMvp();
		const ofRectangle vp = getCurrentViewport();
		const float vw = vp.width > 1.f ? vp.width : static_cast<float>(std::max(width_, 1));
		const float vh = vp.height > 1.f ? vp.height : static_cast<float>(std::max(height_, 1));
		const float dxNdc = ps / vw;
		const float dyNdc = ps / vh;
		const bool sprite = pointSprites_ && textured;
		beginBatch(0, glm::mat4(1.f));
		for(size_t i = 0; i < nVerts; i++) {
			const glm::vec3 & p = verts[i];
			const glm::vec4 clip = mvp * glm::vec4(p, 1.f);
			if(clip.w <= 0.f) {
				continue;
			}
			const float invW = 1.f / clip.w;
			const float nx = clip.x * invW;
			const float ny = clip.y * invW;
			const float nz = clip.z * invW;
			ofFloatColor color = currentStyle.color;
			if(cols && i < nCols) {
				color = cols[i];
			}
			float u0 = 0.f;
			float v0 = 0.f;
			float u1 = 0.f;
			float v1 = 0.f;
			if(sprite) {
				u1 = 1.f;
				v1 = 1.f;
			} else if(tcs && i < nT) {
				u0 = u1 = tcs[i].x;
				v0 = v1 = tcs[i].y;
			}
			const float x0 = nx - dxNdc;
			const float x1 = nx + dxNdc;
			const float y0 = ny - dyNdc;
			const float y1 = ny + dyNdc;
			pushVert(x0, y0, nz, color, u0, v0);
			pushVert(x1, y0, nz, color, u1, v0);
			pushVert(x1, y1, nz, color, u1, v1);
			pushVert(x0, y0, nz, color, u0, v0);
			pushVert(x1, y1, nz, color, u1, v1);
			pushVert(x0, y1, nz, color, u0, v1);
		}
		return;
	}

	if(renderType == OF_MESH_WIREFRAME && triangleLike) {
		beginBatch(2);
		if(mode == OF_PRIMITIVE_TRIANGLES) {
			for(size_t i = 0; i + 2 < n; i += 3) {
				emit2(i, i + 1);
				emit2(i + 1, i + 2);
				emit2(i + 2, i);
			}
		} else if(mode == OF_PRIMITIVE_TRIANGLE_STRIP) {
			for(size_t i = 0; i + 2 < n; i++) {
				emit2(i, i + 1);
				emit2(i + 1, i + 2);
				emit2(i + 2, i);
			}
		} else {
			for(size_t i = 1; i + 1 < n; i++) {
				emit2(0, i);
				emit2(i, i + 1);
				emit2(i + 1, 0);
			}
		}
		return;
	}

	if(lineLike) {
		beginBatch(2);
		if(mode == OF_PRIMITIVE_LINES) {
			for(size_t i = 0; i + 1 < n; i += 2) {
				emit2(i, i + 1);
			}
		} else if(mode == OF_PRIMITIVE_LINE_LOOP) {
			for(size_t i = 0; i + 1 < n; i++) {
				emit2(i, i + 1);
			}
			if(n >= 2) {
				emit2(n - 1, 0);
			}
		} else {
			for(size_t i = 0; i + 1 < n; i++) {
				emit2(i, i + 1);
			}
		}
		return;
	}

	if(triangleLike) {
		beginBatch(0);
		if(mode == OF_PRIMITIVE_TRIANGLE_STRIP) {
			for(size_t i = 0; i + 2 < n; i++) {
				emit3(i, i + 1, i + 2);
			}
		} else if(mode == OF_PRIMITIVE_TRIANGLE_FAN) {
			for(size_t i = 1; i + 1 < n; i++) {
				emit3(0, i, i + 1);
			}
		} else {
			for(size_t i = 0; i + 2 < n; i += 3) {
				emit3(i, i + 1, i + 2);
			}
		}
		return;
	}

	const bool wire = renderType == OF_MESH_WIREFRAME;
	auto emitTri = [&](size_t a, size_t b, size_t c) {
		if(wire) {
			emit2(a, b);
			emit2(b, c);
			emit2(c, a);
		} else {
			emit3(a, b, c);
		}
	};
#ifndef TARGET_OPENGLES
	if(mode == OF_PRIMITIVE_LINES_ADJACENCY) {
		beginBatch(2);
		for(size_t i = 0; i + 3 < n; i += 4) {
			emit2(i + 1, i + 2);
		}
		return;
	}
	if(mode == OF_PRIMITIVE_LINE_STRIP_ADJACENCY) {
		beginBatch(2);
		if(n >= 4) {
			for(size_t i = 1; i + 2 < n; i++) {
				emit2(i, i + 1);
			}
		}
		return;
	}
	if(mode == OF_PRIMITIVE_TRIANGLES_ADJACENCY) {
		beginBatch(wire ? 2 : 0);
		for(size_t i = 0; i + 5 < n; i += 6) {
			emitTri(i, i + 2, i + 4);
		}
		return;
	}
	if(mode == OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY) {
		// OpenGL triangle-strip adjacency: even i uses (2i, 2i+2, 2i+4),
		// odd i uses (2i+2, 2i, 2i+4). Adjacency vertices stay unused.
		beginBatch(wire ? 2 : 0);
		for(size_t tri = 0;; tri++) {
			size_t i0 = 0;
			size_t i1 = 0;
			size_t i2 = 0;
			if((tri & 1u) == 0u) {
				i0 = 2 * tri;
				i1 = i0 + 2;
				i2 = i0 + 4;
			} else {
				i0 = 2 * tri + 2;
				i1 = 2 * tri;
				i2 = 2 * tri + 4;
			}
			if(i2 >= n) {
				break;
			}
			emitTri(i0, i1, i2);
		}
		return;
	}
	if(mode == OF_PRIMITIVE_PATCHES) {
		// No tessellation stage. Draw the control polygon so the mesh is visible.
		if(n >= 3 && !wire) {
			beginBatch(0);
			for(size_t i = 1; i + 1 < n; i++) {
				emit3(0, i, i + 1);
			}
		} else if(n >= 2) {
			beginBatch(2);
			for(size_t i = 0; i + 1 < n; i++) {
				emit2(i, i + 1);
			}
			if(n >= 3) {
				emit2(n - 1, 0);
			}
		}
		return;
	}
#endif

	logDrawOnce("mesh primitive");
}

wgpu::TextureView ofWebGPURenderer::uploadRGBA(const void * owner, const unsigned char * rgba, int width, int height, uint64_t stamp) const {
	if(!device_ || !queue_ || !rgba || width <= 0 || height <= 0) {
		return whiteView_;
	}
	for(size_t i = 0; i < texCache_.size(); i++) {
		TexCache & cached = texCache_[i];
		if(cached.owner == owner && cached.stamp == stamp && cached.width == width && cached.height == height && cached.view) {
			if(i + 1 != texCache_.size()) {
				std::rotate(texCache_.begin() + static_cast<std::ptrdiff_t>(i), texCache_.begin() + static_cast<std::ptrdiff_t>(i + 1), texCache_.end());
			}
			return texCache_.back().view;
		}
	}

	wgpu::TextureDescriptor desc = {};
	desc.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
	desc.dimension = wgpu::TextureDimension::e2D;
	desc.size = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };
	desc.format = wgpu::TextureFormat::RGBA8Unorm;
	desc.mipLevelCount = 1;
	desc.sampleCount = 1;
	wgpu::Texture texture = device_.CreateTexture(&desc);
	if(!texture) {
		return whiteView_;
	}
	const uint32_t rowBytes = (static_cast<uint32_t>(width) * 4u + 255u) & ~255u;
	std::vector<unsigned char> padded(static_cast<size_t>(rowBytes) * static_cast<size_t>(height));
	for(int y = 0; y < height; y++) {
		std::memcpy(padded.data() + static_cast<size_t>(y) * rowBytes, rgba + static_cast<size_t>(y) * static_cast<size_t>(width) * 4u, static_cast<size_t>(width) * 4u);
	}
	wgpu::TexelCopyTextureInfo dst = {};
	dst.texture = texture;
	dst.aspect = wgpu::TextureAspect::All;
	wgpu::TexelCopyBufferLayout layout = {};
	layout.bytesPerRow = rowBytes;
	layout.rowsPerImage = static_cast<uint32_t>(height);
	wgpu::Extent3D size = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };
	queue_.WriteTexture(&dst, padded.data(), padded.size(), &layout, &size);
	wgpu::TextureView view = texture.CreateView();

	if(texCache_.size() >= 24) {
		texCache_.erase(texCache_.begin());
	}
	TexCache entry;
	entry.owner = owner;
	entry.stamp = stamp;
	entry.width = width;
	entry.height = height;
	entry.texture = texture;
	entry.view = view;
	texCache_.push_back(entry);
	return view;
}

void ofWebGPURenderer::drawString(std::string text, float x, float y, float z) const {
	auto * mut = const_cast<ofWebGPURenderer *>(this);
	float sx = 0.f;
	float sy = 0.f;
	bool hasModelView = false;
	bool hasProjection = false;
	bool hasViewport = false;
	ofRectangle viewportRect;

	switch(currentStyle.drawBitmapMode) {
	case OF_BITMAPMODE_SIMPLE:
		sx = x;
		sy = y;
		break;
	case OF_BITMAPMODE_SCREEN: {
		hasViewport = true;
		mut->pushView();
		viewportRect = matrixStack.getFullSurfaceViewport();
		mut->viewport(viewportRect);
		mut->matrixMode(OF_MATRIX_PROJECTION);
		mut->loadIdentityMatrix();
		mut->matrixMode(OF_MATRIX_MODELVIEW);
		glm::mat4 modelView(1.f);
		modelView = glm::translate(modelView, glm::vec3(-1.f, -1.f, 0.f));
		modelView = glm::scale(modelView, glm::vec3(2.f / viewportRect.width, 2.f / viewportRect.height, 1.f));
		modelView = glm::translate(modelView, glm::vec3(x, y, 0.f));
		mut->loadMatrix(modelView);
		break;
	}
	case OF_BITMAPMODE_VIEWPORT:
		viewportRect = getCurrentViewport();
		hasProjection = true;
		mut->matrixMode(OF_MATRIX_PROJECTION);
		mut->pushMatrix();
		mut->loadIdentityMatrix();
		hasModelView = true;
		mut->matrixMode(OF_MATRIX_MODELVIEW);
		mut->pushMatrix();
		{
			glm::mat4 modelView(1.f);
			modelView = glm::translate(modelView, glm::vec3(-1.f, -1.f, 0.f));
			modelView = glm::scale(modelView, glm::vec3(2.f / viewportRect.width, 2.f / viewportRect.height, 1.f));
			modelView = glm::translate(modelView, glm::vec3(x, y, 0.f));
			mut->loadMatrix(modelView);
		}
		break;
	case OF_BITMAPMODE_MODEL:
		hasModelView = true;
		mut->matrixMode(OF_MATRIX_MODELVIEW);
		mut->pushMatrix();
		mut->translate(x, y, z);
		break;
	case OF_BITMAPMODE_MODEL_BILLBOARD: {
		viewportRect = getCurrentViewport();
		glm::mat4 mat = matrixStack.getProjectionMatrixNoOrientation() * matrixStack.getModelViewMatrix();
		glm::vec4 projected = mat * glm::vec4(x, y, z, 1.f);
		if(projected.w == 0.f) {
			return;
		}
		glm::vec3 screen = glm::vec3(projected) / projected.w;
		screen += glm::vec3(1.f);
		screen *= 0.5f;
		screen.x += viewportRect.x;
		screen.x *= viewportRect.width;
		screen.y += viewportRect.y;
		screen.y *= viewportRect.height;
		if(screen.z >= 1.f) {
			return;
		}
		hasProjection = true;
		mut->matrixMode(OF_MATRIX_PROJECTION);
		mut->pushMatrix();
		mut->loadIdentityMatrix();
		hasModelView = true;
		mut->matrixMode(OF_MATRIX_MODELVIEW);
		mut->pushMatrix();
		glm::mat4 modelView(1.f);
		modelView = glm::translate(modelView, glm::vec3(-1.f, -1.f, 0.f));
		modelView = glm::scale(modelView, glm::vec3(2.f / viewportRect.width, 2.f / viewportRect.height, 1.f));
		modelView = glm::translate(modelView, glm::vec3(screen.x, screen.y, 0.f));
		mut->loadMatrix(modelView);
		break;
	}
	default:
		break;
	}

	const ofPixels & atlas = bitmapFont_.getAtlasPixels();
	std::vector<unsigned char> rgba;
	int pw = 0;
	int ph = 0;
	if(pixelsToRGBA(atlas, rgba, pw, ph)) {
		const uint64_t stamp = hashBytes(atlas.getData(), atlas.size());
		const ofBlendMode blend = currentStyle.blendingMode;
		mut->setBlendMode(OF_BLENDMODE_ALPHA);
		activeView_ = uploadRGBA(&bitmapFont_, rgba.data(), pw, ph, stamp);
		ofMesh glyphs = bitmapFont_.getMesh(text, static_cast<int>(sx), static_cast<int>(sy), currentStyle.drawBitmapMode, isVFlipped());
		drawMesh(glyphs, OF_MESH_FILL, false, true);
		activeView_ = whiteView_;
		mut->setBlendMode(blend);
	}

	if(hasViewport) {
		mut->popView();
	} else {
		if(hasModelView) {
			mut->popMatrix();
		}
		if(hasProjection) {
			mut->matrixMode(OF_MATRIX_PROJECTION);
			mut->popMatrix();
			mut->matrixMode(OF_MATRIX_MODELVIEW);
		}
	}
}

void ofWebGPURenderer::drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const {
	if(!font.isLoaded()) {
		return;
	}
	const ofPixels & atlas = font.getFontAtlasPixels();
	std::vector<unsigned char> rgba;
	int pw = 0;
	int ph = 0;
	if(!pixelsToRGBA(atlas, rgba, pw, ph)) {
		logDrawOnce("ttf");
		return;
	}
	auto * mut = const_cast<ofWebGPURenderer *>(this);
	const ofBlendMode blend = currentStyle.blendingMode;
	mut->setBlendMode(OF_BLENDMODE_ALPHA);
	const uint64_t stamp = hashBytes(atlas.getData(), atlas.size());
	activeView_ = uploadRGBA(&font, rgba.data(), pw, ph, stamp);
	drawMesh(font.getStringMesh(text, x, y, isVFlipped()), OF_MESH_FILL, false, true);
	activeView_ = whiteView_;
	mut->setBlendMode(blend);
}

glm::mat4 ofWebGPURenderer::gpuMvp() const {
	glm::mat4 zFix(1.f);
	zFix[2][2] = 0.5f;
	zFix[3][2] = 0.5f;
	return zFix * matrixStack.getModelViewProjectionMatrix();
}

void ofWebGPURenderer::rebuildCircle(int res) const {
	if(res < 3) {
		res = 3;
	}
	if((int)circleUnit_.size() == res + 1) {
		return;
	}
	ofPolyline poly;
	poly.arc(0, 0, 0, 1, 1, 0, 360, res);
	circleUnit_.assign(poly.getVertices().begin(), poly.getVertices().end());
}

void ofWebGPURenderer::pushVert(float x, float y, float z) const {
	pushVert(x, y, z, currentStyle.color, 0.f, 0.f);
}

void ofWebGPURenderer::pushVert(float x, float y, float z, const ofFloatColor & color, float u, float v) const {
	gpuVerts_.push_back({ x, y, z, color.r, color.g, color.b, color.a, u, v });
	if(!batches_.empty()) {
		batches_.back().count++;
	}
}

void ofWebGPURenderer::beginBatch(int primBase) const {
	beginBatch(primBase, gpuMvp());
}

void ofWebGPURenderer::beginBatch(int primBase, const glm::mat4 & mvp) const {
	const bool alpha = currentStyle.blendingMode != OF_BLENDMODE_DISABLED;
	const int pipeline = primBase + (alpha ? 1 : 0) + (bDepthTest_ ? 6 : 0);
	const WGPUTextureView view = activeView_ ? activeView_.Get() : nullptr;
	if(!batches_.empty()) {
		DrawBatch & last = batches_.back();
		const WGPUTextureView lastView = last.view ? last.view.Get() : nullptr;
		if(last.pipeline == pipeline && last.mvp == mvp && lastView == view) {
			return;
		}
	}
	DrawBatch batch;
	batch.first = static_cast<uint32_t>(gpuVerts_.size());
	batch.pipeline = pipeline;
	batch.mvp = mvp;
	batch.view = activeView_;
	batches_.push_back(batch);
}

void ofWebGPURenderer::enablePointSprites() {
	pointSprites_ = true;
}

void ofWebGPURenderer::disablePointSprites() {
	pointSprites_ = false;
}

void ofWebGPURenderer::drawBackgroundGradient(const ofFloatColor & start, const ofFloatColor & end, ofGradientMode mode) const {
	const float w = static_cast<float>(getViewportWidth());
	const float h = static_cast<float>(getViewportHeight());
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	if(mode == OF_GRADIENT_CIRCULAR) {
		const glm::vec2 center(w / 2.f, h / 2.f);
		mesh.addVertex(glm::vec3(center, 0.f));
		mesh.addColor(start);
		const float n = 32.f;
		const float angleBisector = glm::two_pi<float>() / (n * 2.f);
		const float smallRadius = glm::length(glm::vec2(w / 2.f, h / 2.f));
		const float bigRadius = smallRadius / std::cos(angleBisector);
		for(int i = 0; i <= 32; i++) {
			const float theta = static_cast<float>(i) * glm::two_pi<float>() / n;
			mesh.addVertex(glm::vec3(center + glm::vec2(std::sin(theta), std::cos(theta)) * bigRadius, 0.f));
			mesh.addColor(end);
		}
	} else if(mode == OF_GRADIENT_LINEAR) {
		mesh.addVertex(glm::vec3(0.f, 0.f, 0.f));
		mesh.addVertex(glm::vec3(w, 0.f, 0.f));
		mesh.addVertex(glm::vec3(w, h, 0.f));
		mesh.addVertex(glm::vec3(0.f, h, 0.f));
		mesh.addColor(start);
		mesh.addColor(start);
		mesh.addColor(end);
		mesh.addColor(end);
	} else if(mode == OF_GRADIENT_BAR) {
		mesh.addVertex(glm::vec3(w / 2.f, h / 2.f, 0.f));
		mesh.addVertex(glm::vec3(0.f, h / 2.f, 0.f));
		mesh.addVertex(glm::vec3(0.f, 0.f, 0.f));
		mesh.addVertex(glm::vec3(w, 0.f, 0.f));
		mesh.addVertex(glm::vec3(w, h / 2.f, 0.f));
		mesh.addVertex(glm::vec3(w, h, 0.f));
		mesh.addVertex(glm::vec3(0.f, h, 0.f));
		mesh.addVertex(glm::vec3(0.f, h / 2.f, 0.f));
		mesh.addColor(start);
		mesh.addColor(start);
		mesh.addColor(end);
		mesh.addColor(end);
		mesh.addColor(start);
		mesh.addColor(end);
		mesh.addColor(end);
		mesh.addColor(start);
	}
	auto * mut = const_cast<ofWebGPURenderer *>(this);
	const bool depth = bDepthTest_;
	mut->bDepthTest_ = false;
	drawMesh(mesh, OF_MESH_FILL, true, false);
	mut->bDepthTest_ = depth;
}

void ofWebGPURenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const {
	beginBatch(2);
	pushVert(x1, y1, z1);
	pushVert(x2, y2, z2);
}

void ofWebGPURenderer::drawRectangle(float x, float y, float z, float w, float h) const {
	float x0 = x;
	float y0 = y;
	float x1 = x + w;
	float y1 = y + h;
	if(currentStyle.rectMode != OF_RECTMODE_CORNER) {
		x0 = x - w * 0.5f;
		y0 = y - h * 0.5f;
		x1 = x + w * 0.5f;
		y1 = y + h * 0.5f;
	}
	if(currentStyle.bFill) {
		beginBatch(0);
		pushVert(x0, y0, z);
		pushVert(x1, y0, z);
		pushVert(x1, y1, z);
		pushVert(x0, y0, z);
		pushVert(x1, y1, z);
		pushVert(x0, y1, z);
	} else {
		beginBatch(2);
		pushVert(x0, y0, z);
		pushVert(x1, y0, z);
		pushVert(x1, y0, z);
		pushVert(x1, y1, z);
		pushVert(x1, y1, z);
		pushVert(x0, y1, z);
		pushVert(x0, y1, z);
		pushVert(x0, y0, z);
	}
}

void ofWebGPURenderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const {
	if(currentStyle.bFill) {
		beginBatch(0);
		pushVert(x1, y1, z1);
		pushVert(x2, y2, z2);
		pushVert(x3, y3, z3);
	} else {
		beginBatch(2);
		pushVert(x1, y1, z1);
		pushVert(x2, y2, z2);
		pushVert(x2, y2, z2);
		pushVert(x3, y3, z3);
		pushVert(x3, y3, z3);
		pushVert(x1, y1, z1);
	}
}

void ofWebGPURenderer::drawCircle(float x, float y, float z, float radius) const {
	rebuildCircle(currentStyle.circleResolution);
	if(circleUnit_.size() < 3) {
		return;
	}
	if(currentStyle.bFill) {
		beginBatch(0);
		const glm::vec3 & c0 = circleUnit_[0];
		for(size_t i = 1; i + 1 < circleUnit_.size(); i++) {
			pushVert(x + radius * c0.x, y + radius * c0.y, z);
			pushVert(x + radius * circleUnit_[i].x, y + radius * circleUnit_[i].y, z);
			pushVert(x + radius * circleUnit_[i + 1].x, y + radius * circleUnit_[i + 1].y, z);
		}
	} else {
		beginBatch(2);
		for(size_t i = 0; i + 1 < circleUnit_.size(); i++) {
			pushVert(x + radius * circleUnit_[i].x, y + radius * circleUnit_[i].y, z);
			pushVert(x + radius * circleUnit_[i + 1].x, y + radius * circleUnit_[i + 1].y, z);
		}
	}
}

void ofWebGPURenderer::drawEllipse(float x, float y, float z, float width, float height) const {
	rebuildCircle(currentStyle.circleResolution);
	if(circleUnit_.size() < 3) {
		return;
	}
	const float rx = width * 0.5f;
	const float ry = height * 0.5f;
	if(currentStyle.bFill) {
		beginBatch(0);
		const glm::vec3 & c0 = circleUnit_[0];
		for(size_t i = 1; i + 1 < circleUnit_.size(); i++) {
			pushVert(x + rx * c0.x, y + ry * c0.y, z);
			pushVert(x + rx * circleUnit_[i].x, y + ry * circleUnit_[i].y, z);
			pushVert(x + rx * circleUnit_[i + 1].x, y + ry * circleUnit_[i + 1].y, z);
		}
	} else {
		beginBatch(2);
		for(size_t i = 0; i + 1 < circleUnit_.size(); i++) {
			pushVert(x + rx * circleUnit_[i].x, y + ry * circleUnit_[i].y, z);
			pushVert(x + rx * circleUnit_[i + 1].x, y + ry * circleUnit_[i + 1].y, z);
		}
	}
}

void ofWebGPURenderer::ensurePipelines() {
	if(pipelinesReady_ || !device_) {
		return;
	}
	static const char * kWGSL = R"(
struct Uniforms { mvp: mat4x4f }
@group(0) @binding(0) var<uniform> u: Uniforms;
@group(0) @binding(1) var tex: texture_2d<f32>;
@group(0) @binding(2) var samp: sampler;
struct VSIn {
  @location(0) pos: vec3f,
  @location(1) color: vec4f,
  @location(2) uv: vec2f,
}
struct VSOut {
  @builtin(position) clip: vec4f,
  @location(0) color: vec4f,
  @location(1) uv: vec2f,
}
@vertex fn vs(in: VSIn) -> VSOut {
  var o: VSOut;
  o.clip = u.mvp * vec4f(in.pos, 1.0);
  o.color = in.color;
  o.uv = in.uv;
  return o;
}
@fragment fn fs(in: VSOut) -> @location(0) vec4f {
  return in.color * textureSample(tex, samp, in.uv);
}
)";
	wgpu::ShaderSourceWGSL wgsl;
	wgsl.code = kWGSL;
	wgpu::ShaderModuleDescriptor shaderDesc;
	shaderDesc.nextInChain = &wgsl;
	wgpu::ShaderModule module = device_.CreateShaderModule(&shaderDesc);

	const unsigned char whitePx[4] = { 255, 255, 255, 255 };
	wgpu::TextureDescriptor whiteDesc = {};
	whiteDesc.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
	whiteDesc.dimension = wgpu::TextureDimension::e2D;
	whiteDesc.size = { 1, 1, 1 };
	whiteDesc.format = wgpu::TextureFormat::RGBA8Unorm;
	whiteDesc.mipLevelCount = 1;
	whiteDesc.sampleCount = 1;
	whiteTex_ = device_.CreateTexture(&whiteDesc);
	if(whiteTex_) {
		unsigned char padded[256] = {};
		padded[0] = whitePx[0];
		padded[1] = whitePx[1];
		padded[2] = whitePx[2];
		padded[3] = whitePx[3];
		wgpu::TexelCopyTextureInfo dst = {};
		dst.texture = whiteTex_;
		dst.aspect = wgpu::TextureAspect::All;
		wgpu::TexelCopyBufferLayout layout = {};
		layout.bytesPerRow = 256;
		layout.rowsPerImage = 1;
		wgpu::Extent3D size = { 1, 1, 1 };
		queue_.WriteTexture(&dst, padded, sizeof(padded), &layout, &size);
		whiteView_ = whiteTex_.CreateView();
	}
	if(!activeView_) {
		activeView_ = whiteView_;
	}

	wgpu::SamplerDescriptor samplerDesc = {};
	samplerDesc.addressModeU = wgpu::AddressMode::ClampToEdge;
	samplerDesc.addressModeV = wgpu::AddressMode::ClampToEdge;
	samplerDesc.addressModeW = wgpu::AddressMode::ClampToEdge;
	samplerDesc.magFilter = wgpu::FilterMode::Linear;
	samplerDesc.minFilter = wgpu::FilterMode::Linear;
	samplerDesc.mipmapFilter = wgpu::MipmapFilterMode::Nearest;
	sampler_ = device_.CreateSampler(&samplerDesc);

	wgpu::BindGroupLayoutEntry layoutEntries[3] = {};
	layoutEntries[0].binding = 0;
	layoutEntries[0].visibility = wgpu::ShaderStage::Vertex;
	layoutEntries[0].buffer.type = wgpu::BufferBindingType::Uniform;
	layoutEntries[0].buffer.hasDynamicOffset = true;
	layoutEntries[0].buffer.minBindingSize = 64;
	layoutEntries[1].binding = 1;
	layoutEntries[1].visibility = wgpu::ShaderStage::Fragment;
	layoutEntries[1].texture.sampleType = wgpu::TextureSampleType::Float;
	layoutEntries[1].texture.viewDimension = wgpu::TextureViewDimension::e2D;
	layoutEntries[2].binding = 2;
	layoutEntries[2].visibility = wgpu::ShaderStage::Fragment;
	layoutEntries[2].sampler.type = wgpu::SamplerBindingType::Filtering;
	wgpu::BindGroupLayoutDescriptor layoutDesc;
	layoutDesc.entryCount = 3;
	layoutDesc.entries = layoutEntries;
	bindGroupLayout_ = device_.CreateBindGroupLayout(&layoutDesc);

	wgpu::PipelineLayoutDescriptor pipelineLayoutDesc;
	pipelineLayoutDesc.bindGroupLayoutCount = 1;
	pipelineLayoutDesc.bindGroupLayouts = &bindGroupLayout_;
	wgpu::PipelineLayout pipelineLayout = device_.CreatePipelineLayout(&pipelineLayoutDesc);

	wgpu::VertexAttribute attributes[3] = {};
	attributes[0].format = wgpu::VertexFormat::Float32x3;
	attributes[0].offset = 0;
	attributes[0].shaderLocation = 0;
	attributes[1].format = wgpu::VertexFormat::Float32x4;
	attributes[1].offset = 12;
	attributes[1].shaderLocation = 1;
	attributes[2].format = wgpu::VertexFormat::Float32x2;
	attributes[2].offset = 28;
	attributes[2].shaderLocation = 2;
	wgpu::VertexBufferLayout bufferLayout = {};
	bufferLayout.arrayStride = sizeof(GpuVertex);
	bufferLayout.stepMode = wgpu::VertexStepMode::Vertex;
	bufferLayout.attributeCount = 3;
	bufferLayout.attributes = attributes;

	for(int i = 0; i < 12; i++) {
		const int prim = i % 6;
		const bool depth = i >= 6;
		const bool lines = prim >= 2 && prim < 4;
		const bool points = prim >= 4;
		const bool alpha = (prim % 2) == 1;
		wgpu::BlendState blend = {};
		if(alpha) {
			blend.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
			blend.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
			blend.alpha.srcFactor = wgpu::BlendFactor::SrcAlpha;
			blend.alpha.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
		} else {
			blend.color.srcFactor = wgpu::BlendFactor::One;
			blend.color.dstFactor = wgpu::BlendFactor::Zero;
			blend.alpha.srcFactor = wgpu::BlendFactor::One;
			blend.alpha.dstFactor = wgpu::BlendFactor::Zero;
		}
		blend.color.operation = wgpu::BlendOperation::Add;
		blend.alpha.operation = wgpu::BlendOperation::Add;

		wgpu::ColorTargetState target = {};
		target.format = surfaceFormat_;
		target.blend = &blend;
		target.writeMask = wgpu::ColorWriteMask::All;

		wgpu::FragmentState fragment = {};
		fragment.module = module;
		fragment.entryPoint = "fs";
		fragment.targetCount = 1;
		fragment.targets = &target;

		wgpu::DepthStencilState depthState = {};
		depthState.format = wgpu::TextureFormat::Depth24Plus;
		depthState.depthWriteEnabled = depth ? wgpu::OptionalBool::True : wgpu::OptionalBool::False;
		depthState.depthCompare = depth ? wgpu::CompareFunction::Less : wgpu::CompareFunction::Always;

		wgpu::PrimitiveTopology topology = wgpu::PrimitiveTopology::TriangleList;
		if(lines) {
			topology = wgpu::PrimitiveTopology::LineList;
		} else if(points) {
			topology = wgpu::PrimitiveTopology::PointList;
		}

		wgpu::RenderPipelineDescriptor pipe = {};
		pipe.layout = pipelineLayout;
		pipe.vertex.module = module;
		pipe.vertex.entryPoint = "vs";
		pipe.vertex.bufferCount = 1;
		pipe.vertex.buffers = &bufferLayout;
		pipe.fragment = &fragment;
		pipe.primitive.topology = topology;
		pipe.primitive.cullMode = wgpu::CullMode::None;
		pipe.depthStencil = &depthState;
		pipe.multisample.count = 1;
		pipelines_[i] = device_.CreateRenderPipeline(&pipe);
	}
	pipelinesReady_ = true;
}

void ofWebGPURenderer::flushBatches() {
	if(!inFrame_ || !pass_ || gpuVerts_.empty() || !device_ || !queue_) {
		gpuVerts_.clear();
		batches_.clear();
		return;
	}
	ensurePipelines();
	if(!pipelinesReady_ || !whiteView_ || !sampler_) {
		gpuVerts_.clear();
		batches_.clear();
		return;
	}

	const uint64_t vertexBytes = static_cast<uint64_t>(gpuVerts_.size()) * sizeof(GpuVertex);
	const uint64_t uniformStride = 256;
	const uint64_t uniformBytes = std::max<uint64_t>(uniformStride, static_cast<uint64_t>(batches_.size()) * uniformStride);
	FrameSlot & slot = slots_[slotIndex_];
	slotIndex_ = (slotIndex_ + 1) % 3;
	if(!slot.vertices || slot.vertexBytes < vertexBytes) {
		wgpu::BufferDescriptor bd = {};
		bd.size = std::max<uint64_t>(vertexBytes, 256);
		bd.usage = wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst;
		slot.vertices = device_.CreateBuffer(&bd);
		slot.vertexBytes = bd.size;
	}
	if(!slot.uniforms || slot.uniformBytes < uniformBytes) {
		wgpu::BufferDescriptor bd = {};
		bd.size = std::max<uint64_t>(uniformBytes, 256);
		bd.usage = wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst;
		slot.uniforms = device_.CreateBuffer(&bd);
		slot.uniformBytes = bd.size;
	}
	queue_.WriteBuffer(slot.vertices, 0, gpuVerts_.data(), vertexBytes);

	std::vector<unsigned char> uniformData(static_cast<size_t>(uniformBytes), 0);
	for(size_t i = 0; i < batches_.size(); i++) {
		std::memcpy(uniformData.data() + i * uniformStride, &batches_[i].mvp, sizeof(glm::mat4));
	}
	queue_.WriteBuffer(slot.uniforms, 0, uniformData.data(), uniformBytes);

	struct CachedGroup {
		WGPUTextureView key = nullptr;
		wgpu::BindGroup group;
	};
	std::vector<CachedGroup> groups;
	auto bindFor = [&](wgpu::TextureView view) -> wgpu::BindGroup {
		if(!view) {
			view = whiteView_;
		}
		const WGPUTextureView key = view.Get();
		for(const CachedGroup & group : groups) {
			if(group.key == key) {
				return group.group;
			}
		}
		wgpu::BindGroupEntry entries[3] = {};
		entries[0].binding = 0;
		entries[0].buffer = slot.uniforms;
		entries[0].offset = 0;
		entries[0].size = uniformStride;
		entries[1].binding = 1;
		entries[1].textureView = view;
		entries[2].binding = 2;
		entries[2].sampler = sampler_;
		wgpu::BindGroupDescriptor bgDesc = {};
		bgDesc.layout = bindGroupLayout_;
		bgDesc.entryCount = 3;
		bgDesc.entries = entries;
		CachedGroup created;
		created.key = key;
		created.group = device_.CreateBindGroup(&bgDesc);
		groups.push_back(created);
		return created.group;
	};

	pass_.SetViewport(0, 0, static_cast<float>(width_), static_cast<float>(height_), 0.f, 1.f);
	pass_.SetVertexBuffer(0, slot.vertices, 0, vertexBytes);
	for(size_t i = 0; i < batches_.size(); i++) {
		const DrawBatch & batch = batches_[i];
		if(batch.count == 0 || batch.pipeline < 0 || batch.pipeline >= 12 || !pipelines_[batch.pipeline]) {
			continue;
		}
		uint32_t offset = static_cast<uint32_t>(i * uniformStride);
		wgpu::BindGroup group = bindFor(batch.view);
		pass_.SetPipeline(pipelines_[batch.pipeline]);
		pass_.SetBindGroup(0, group, 1, &offset);
		pass_.Draw(batch.count, 1, batch.first, 0);
	}
	gpuVerts_.clear();
	batches_.clear();
}
ofPath & ofWebGPURenderer::getPath() { return path; }
ofStyle ofWebGPURenderer::getStyle() const { return currentStyle; }
void ofWebGPURenderer::setStyle(const ofStyle & style) { currentStyle = style; }
void ofWebGPURenderer::pushStyle() { styleHistory.push_back(currentStyle); }
void ofWebGPURenderer::popStyle() {
	if(!styleHistory.empty()) {
		currentStyle = styleHistory.back();
		styleHistory.pop_back();
	}
}
void ofWebGPURenderer::setCurveResolution(int resolution) { currentStyle.curveResolution = resolution; }
void ofWebGPURenderer::setPolyMode(ofPolyWindingMode mode) { currentStyle.polyMode = mode; }
const of3dGraphics & ofWebGPURenderer::get3dGraphics() const { return graphics3d; }
of3dGraphics & ofWebGPURenderer::get3dGraphics() { return graphics3d; }

#endif
