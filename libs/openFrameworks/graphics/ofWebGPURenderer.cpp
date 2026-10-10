#include "ofWebGPURenderer.h"
#if OF_USE_DAWN

#include "ofAppBaseWindow.h"
#include "ofCamera.h"
#include "ofLog.h"
#include "ofNode.h"
#include "ofPath.h"
#include "ofTrueTypeFont.h"

#include <webgpu/webgpu.h>
#include "ofPolyline.h"
#include <cmath>
#include <cstring>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	config.presentMode = wgpu::PresentMode::Fifo;
	surface_.Configure(&config);
	surfaceConfigured_ = true;
	matrixStack.viewport(0, 0, width_, height_, true);
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
	wgpu::RenderPassColorAttachment color{};
	color.view = surfaceTexture.texture.CreateView();
	color.loadOp = wgpu::LoadOp::Clear;
	color.storeOp = wgpu::StoreOp::Store;
	const ofFloatColor bg = currentStyle.bgColor;
	color.clearValue = { bg.r, bg.g, bg.b, bg.a };

	wgpu::RenderPassDescriptor passDesc{};
	passDesc.colorAttachmentCount = 1;
	passDesc.colorAttachments = &color;
	pass_ = encoder_.BeginRenderPass(&passDesc);
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
	wgpu::RenderPassColorAttachment color{};
	color.view = surfaceTexture.texture.CreateView();
	color.loadOp = wgpu::LoadOp::Clear;
	color.storeOp = wgpu::StoreOp::Store;
	color.clearValue = { r, g, b, a };
	wgpu::RenderPassDescriptor passDesc{};
	passDesc.colorAttachmentCount = 1;
	passDesc.colorAttachments = &color;
	pass_ = encoder_.BeginRenderPass(&passDesc);
}

void ofWebGPURenderer::logDrawOnce(const char * what) const {
	if(!loggedDraw_) {
		ofLogVerbose("ofWebGPURenderer") << "draw(" << what << ") not implemented yet — clear/present only";
		loggedDraw_ = true;
	}
}

void ofWebGPURenderer::draw(const ofPolyline &) const { logDrawOnce("polyline"); }
void ofWebGPURenderer::draw(const ofPath &) const { logDrawOnce("path"); }
void ofWebGPURenderer::draw(const ofMesh &, ofPolyRenderMode, bool, bool, bool) const { logDrawOnce("mesh"); }
void ofWebGPURenderer::draw(const of3dPrimitive &, ofPolyRenderMode) const { logDrawOnce("3dPrimitive"); }
void ofWebGPURenderer::draw(const ofNode & node) const { const_cast<ofNode &>(node).customDraw(this); }
void ofWebGPURenderer::draw(const ofImage &, float, float, float, float, float, float, float, float, float) const { logDrawOnce("image"); }
void ofWebGPURenderer::draw(const ofFloatImage &, float, float, float, float, float, float, float, float, float) const { logDrawOnce("floatImage"); }
void ofWebGPURenderer::draw(const ofShortImage &, float, float, float, float, float, float, float, float, float) const { logDrawOnce("shortImage"); }
void ofWebGPURenderer::draw(const ofBaseVideoDraws &, float, float, float, float) const { logDrawOnce("video"); }

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
	const ofFloatColor & c = currentStyle.color;
	gpuVerts_.push_back({ x, y, z, c.r, c.g, c.b, c.a });
	if(!batches_.empty()) {
		batches_.back().count++;
	}
}

void ofWebGPURenderer::beginBatch(bool lines) const {
	const bool alpha = currentStyle.blendingMode != OF_BLENDMODE_DISABLED;
	const int pipeline = (lines ? 2 : 0) + (alpha ? 1 : 0);
	const glm::mat4 mvp = gpuMvp();
	if(!batches_.empty()) {
		DrawBatch & last = batches_.back();
		if(last.pipeline == pipeline && last.mvp == mvp) {
			return;
		}
	}
	DrawBatch batch;
	batch.first = (uint32_t)gpuVerts_.size();
	batch.pipeline = pipeline;
	batch.mvp = mvp;
	batches_.push_back(batch);
}

void ofWebGPURenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const {
	beginBatch(true);
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
		beginBatch(false);
		pushVert(x0, y0, z);
		pushVert(x1, y0, z);
		pushVert(x1, y1, z);
		pushVert(x0, y0, z);
		pushVert(x1, y1, z);
		pushVert(x0, y1, z);
	} else {
		beginBatch(true);
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
		beginBatch(false);
		pushVert(x1, y1, z1);
		pushVert(x2, y2, z2);
		pushVert(x3, y3, z3);
	} else {
		beginBatch(true);
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
		beginBatch(false);
		const glm::vec3 & c0 = circleUnit_[0];
		for(size_t i = 1; i + 1 < circleUnit_.size(); i++) {
			pushVert(x + radius * c0.x, y + radius * c0.y, z);
			pushVert(x + radius * circleUnit_[i].x, y + radius * circleUnit_[i].y, z);
			pushVert(x + radius * circleUnit_[i + 1].x, y + radius * circleUnit_[i + 1].y, z);
		}
	} else {
		beginBatch(true);
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
		beginBatch(false);
		const glm::vec3 & c0 = circleUnit_[0];
		for(size_t i = 1; i + 1 < circleUnit_.size(); i++) {
			pushVert(x + rx * c0.x, y + ry * c0.y, z);
			pushVert(x + rx * circleUnit_[i].x, y + ry * circleUnit_[i].y, z);
			pushVert(x + rx * circleUnit_[i + 1].x, y + ry * circleUnit_[i + 1].y, z);
		}
	} else {
		beginBatch(true);
		for(size_t i = 0; i + 1 < circleUnit_.size(); i++) {
			pushVert(x + rx * circleUnit_[i].x, y + ry * circleUnit_[i].y, z);
			pushVert(x + rx * circleUnit_[i + 1].x, y + ry * circleUnit_[i + 1].y, z);
		}
	}
}

void ofWebGPURenderer::drawString(std::string, float, float, float) const { logDrawOnce("string"); }
void ofWebGPURenderer::drawString(const ofTrueTypeFont &, std::string, float, float) const { logDrawOnce("ttf"); }

void ofWebGPURenderer::ensurePipelines() {
	if(pipelinesReady_ || !device_) {
		return;
	}
	static const char * kWGSL = R"(
struct Uniforms { mvp: mat4x4f }
@group(0) @binding(0) var<uniform> u: Uniforms;
struct VSIn {
  @location(0) pos: vec3f,
  @location(1) color: vec4f,
}
struct VSOut {
  @builtin(position) clip: vec4f,
  @location(0) color: vec4f,
}
@vertex fn vs(in: VSIn) -> VSOut {
  var o: VSOut;
  o.clip = u.mvp * vec4f(in.pos, 1.0);
  o.color = in.color;
  return o;
}
@fragment fn fs(in: VSOut) -> @location(0) vec4f {
  return in.color;
}
)";
	wgpu::ShaderSourceWGSL wgsl;
	wgsl.code = kWGSL;
	wgpu::ShaderModuleDescriptor shaderDesc;
	shaderDesc.nextInChain = &wgsl;
	wgpu::ShaderModule module = device_.CreateShaderModule(&shaderDesc);

	wgpu::BindGroupLayoutEntry layoutEntry = {};
	layoutEntry.binding = 0;
	layoutEntry.visibility = wgpu::ShaderStage::Vertex;
	layoutEntry.buffer.type = wgpu::BufferBindingType::Uniform;
	layoutEntry.buffer.hasDynamicOffset = true;
	layoutEntry.buffer.minBindingSize = 64;
	wgpu::BindGroupLayoutDescriptor layoutDesc;
	layoutDesc.entryCount = 1;
	layoutDesc.entries = &layoutEntry;
	bindGroupLayout_ = device_.CreateBindGroupLayout(&layoutDesc);

	wgpu::PipelineLayoutDescriptor pipelineLayoutDesc;
	pipelineLayoutDesc.bindGroupLayoutCount = 1;
	pipelineLayoutDesc.bindGroupLayouts = &bindGroupLayout_;
	wgpu::PipelineLayout pipelineLayout = device_.CreatePipelineLayout(&pipelineLayoutDesc);

	wgpu::VertexAttribute attributes[2] = {};
	attributes[0].format = wgpu::VertexFormat::Float32x3;
	attributes[0].offset = 0;
	attributes[0].shaderLocation = 0;
	attributes[1].format = wgpu::VertexFormat::Float32x4;
	attributes[1].offset = 12;
	attributes[1].shaderLocation = 1;
	wgpu::VertexBufferLayout bufferLayout = {};
	bufferLayout.arrayStride = sizeof(GpuVertex);
	bufferLayout.attributeCount = 2;
	bufferLayout.attributes = attributes;

	for(int i = 0; i < 4; i++) {
		const bool lines = i >= 2;
		const bool alpha = (i % 2) == 1;
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

		wgpu::RenderPipelineDescriptor desc = {};
		desc.layout = pipelineLayout;
		desc.vertex.module = module;
		desc.vertex.entryPoint = "vs";
		desc.vertex.bufferCount = 1;
		desc.vertex.buffers = &bufferLayout;
		desc.fragment = &fragment;
		desc.primitive.topology = lines ? wgpu::PrimitiveTopology::LineList : wgpu::PrimitiveTopology::TriangleList;
		desc.primitive.cullMode = wgpu::CullMode::None;
		desc.multisample.count = 1;
		pipelines_[i] = device_.CreateRenderPipeline(&desc);
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
	if(!pipelinesReady_) {
		gpuVerts_.clear();
		batches_.clear();
		return;
	}

	const uint64_t vertexBytes = (uint64_t)gpuVerts_.size() * sizeof(GpuVertex);
	const uint64_t uniformStride = 256;
	const uint64_t uniformBytes = std::max<uint64_t>(uniformStride, (uint64_t)batches_.size() * uniformStride);
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

	std::vector<unsigned char> uniformData((size_t)uniformBytes, 0);
	for(size_t i = 0; i < batches_.size(); i++) {
		std::memcpy(uniformData.data() + i * uniformStride, &batches_[i].mvp, sizeof(glm::mat4));
	}
	queue_.WriteBuffer(slot.uniforms, 0, uniformData.data(), uniformBytes);

	wgpu::BindGroupEntry entry = {};
	entry.binding = 0;
	entry.buffer = slot.uniforms;
	entry.offset = 0;
	entry.size = uniformStride;
	wgpu::BindGroupDescriptor bgDesc = {};
	bgDesc.layout = bindGroupLayout_;
	bgDesc.entryCount = 1;
	bgDesc.entries = &entry;
	wgpu::BindGroup bindGroup = device_.CreateBindGroup(&bgDesc);

	pass_.SetViewport(0, 0, (float)width_, (float)height_, 0.f, 1.f);
	pass_.SetVertexBuffer(0, slot.vertices, 0, vertexBytes);
	for(size_t i = 0; i < batches_.size(); i++) {
		const DrawBatch & batch = batches_[i];
		if(batch.count == 0 || !pipelines_[batch.pipeline]) {
			continue;
		}
		uint32_t offset = (uint32_t)(i * uniformStride);
		pass_.SetPipeline(pipelines_[batch.pipeline]);
		pass_.SetBindGroup(0, bindGroup, 1, &offset);
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
