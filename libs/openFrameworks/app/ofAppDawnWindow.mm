#include "ofAppDawnWindow.h"
#if OF_USE_DAWN && !defined(TARGET_OF_IOS)

#include "ofWebGPURenderer.h"
#include "ofBaseApp.h"
#include "ofLog.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include <cmath>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#if defined(TARGET_OSX) || defined(TARGET_OF_MAC)
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#endif

#if defined(TARGET_OF_IOS)
#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>
#endif

namespace {
	ofAppDawnWindow * currentDawnWindow = nullptr;
}

void ofAppDawnWindow::error_cb(int errorCode, const char * errorDescription) {
	ofLogError("ofAppDawnWindow") << "GLFW " << errorCode << ": " << errorDescription;
}

ofAppDawnWindow::ofAppDawnWindow()
	: coreEvents(new ofCoreEvents) {
	glfwSetErrorCallback(error_cb);
}

ofAppDawnWindow::~ofAppDawnWindow() {
	close();
}

void ofAppDawnWindow::pollEvents() {
	glfwPollEvents();
}

void ofAppDawnWindow::setup(const ofWindowSettings & settings) {
	const ofDawnWindowSettings * dawn = dynamic_cast<const ofDawnWindowSettings *>(&settings);
	if(dawn) {
		setup(*dawn);
	} else {
		setup(ofDawnWindowSettings(settings));
	}
}

void ofAppDawnWindow::setup(const ofDawnWindowSettings & _settings) {
	if(windowP) {
		ofLogError("ofAppDawnWindow") << "window already setup";
		return;
	}
	settings = _settings;

	if(!glfwInit()) {
		ofLogError("ofAppDawnWindow") << "glfwInit failed";
		return;
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, settings.decorated ? GLFW_TRUE : GLFW_FALSE);

	windowW = settings.isSizeSet() ? settings.getWidth() : 1024;
	windowH = settings.isSizeSet() ? settings.getHeight() : 768;
	const char * title = settings.title.empty() ? "openFrameworks (Dawn)" : settings.title.c_str();
	windowP = glfwCreateWindow(windowW, windowH, title, nullptr, nullptr);
	if(!windowP) {
		ofLogError("ofAppDawnWindow") << "glfwCreateWindow failed";
		return;
	}

	if(settings.isPositionSet()) {
		glfwSetWindowPos(windowP, settings.getPosition().x, settings.getPosition().y);
	}

	glfwSetWindowUserPointer(windowP, this);
	glfwSetMouseButtonCallback(windowP, mouse_cb);
	glfwSetCursorPosCallback(windowP, motion_cb);
	glfwSetCursorEnterCallback(windowP, entry_cb);
	glfwSetKeyCallback(windowP, keyboard_cb);
	glfwSetWindowSizeCallback(windowP, resize_cb);
	glfwSetFramebufferSizeCallback(windowP, framebuffer_cb);
	glfwSetWindowCloseCallback(windowP, exit_cb);
	glfwSetScrollCallback(windowP, scroll_cb);
	glfwSetDropCallback(windowP, drop_cb);

	attachMetalLayer();

	auto renderer = std::make_shared<ofWebGPURenderer>(this);
	int fbw = 0;
	int fbh = 0;
	glfwGetFramebufferSize(windowP, &fbw, &fbh);
	if(!renderer->setup(metalLayer, fbw, fbh, pixelScale)) {
		ofLogError("ofAppDawnWindow") << "Dawn renderer setup failed";
		return;
	}
	currentRenderer = renderer;
	ofSetCurrentRenderer(currentRenderer, true);

	bWindowNeedsShowing = true;
	currentDawnWindow = this;
}

void ofAppDawnWindow::attachMetalLayer() {
#if defined(TARGET_OSX) || defined(TARGET_OF_MAC)
	NSWindow * nsWindow = glfwGetCocoaWindow(windowP);
	NSView * view = nsWindow.contentView;
	view.wantsLayer = YES;
	CAMetalLayer * layer = [CAMetalLayer layer];
	layer.contentsScale = nsWindow.backingScaleFactor;
	layer.opaque = YES;
	layer.framebufferOnly = YES;
	view.layer = layer;
	int fbw = 0;
	int fbh = 0;
	glfwGetFramebufferSize(windowP, &fbw, &fbh);
	if(fbw > 0 && fbh > 0) {
		layer.drawableSize = CGSizeMake(fbw, fbh);
	}
	metalLayer = (__bridge void *)layer;
	pixelScale = nsWindow.backingScaleFactor;
	ofLogNotice("ofAppDawnWindow") << "CAMetalLayer attached scale=" << pixelScale;
#else
	ofLogError("ofAppDawnWindow") << "desktop Dawn window is macOS-only; use ofxiOSDawnView on iOS";
#endif
}

void ofAppDawnWindow::resizeSurface(int w, int h) {
	windowW = w;
	windowH = h;
	int fbw = 0;
	int fbh = 0;
	if(windowP) {
		glfwGetFramebufferSize(windowP, &fbw, &fbh);
	}
	if(metalLayer && fbw > 0 && fbh > 0) {
		((__bridge CAMetalLayer *)metalLayer).drawableSize = CGSizeMake(fbw, fbh);
	}
	auto * dawn = dynamic_cast<ofWebGPURenderer *>(currentRenderer.get());
	if(dawn) {
		dawn->configureSurface(fbw > 0 ? fbw : w, fbh > 0 ? fbh : h);
	}
}

void ofAppDawnWindow::update() {
	events().notifyUpdate();
}

void ofAppDawnWindow::draw() {
	if(bWindowNeedsShowing && windowP) {
		glfwShowWindow(windowP);
		bWindowNeedsShowing = false;
	}
	currentRenderer->startRender();
	if(currentRenderer->getBackgroundAuto()) {
		currentRenderer->background(currentRenderer->getBackgroundColor());
	}
	currentRenderer->setupScreen();
	events().notifyDraw();
	currentRenderer->finishRender();
}

void ofAppDawnWindow::close() {
	if(windowP) {
		glfwSetWindowUserPointer(windowP, nullptr);
		glfwDestroyWindow(windowP);
		windowP = nullptr;
	}
	currentRenderer.reset();
	metalLayer = nullptr;
	if(currentDawnWindow == this) {
		currentDawnWindow = nullptr;
	}
}

bool ofAppDawnWindow::getWindowShouldClose() {
	return windowP ? glfwWindowShouldClose(windowP) : true;
}

void ofAppDawnWindow::setWindowShouldClose() {
	if(windowP) {
		glfwSetWindowShouldClose(windowP, GL_TRUE);
	}
}

ofCoreEvents & ofAppDawnWindow::events() {
	return *coreEvents;
}

std::shared_ptr<ofBaseRenderer> & ofAppDawnWindow::renderer() {
	return currentRenderer;
}

glm::vec2 ofAppDawnWindow::getWindowSize() {
	float scale = pixelScale > 0.f ? pixelScale : 1.f;
	return { (float)windowW * scale, (float)windowH * scale };
}

glm::vec2 ofAppDawnWindow::getScreenSize() {
	if(!windowP) {
		return getWindowSize();
	}
	GLFWmonitor * monitor = glfwGetWindowMonitor(windowP);
	if(!monitor) {
		monitor = glfwGetPrimaryMonitor();
	}
	const GLFWvidmode * mode = monitor ? glfwGetVideoMode(monitor) : nullptr;
	if(!mode) {
		return getWindowSize();
	}
	return { (float)mode->width, (float)mode->height };
}

glm::vec2 ofAppDawnWindow::getWindowPosition() {
	int x = 0;
	int y = 0;
	if(windowP) {
		glfwGetWindowPos(windowP, &x, &y);
	}
	return { (float)x, (float)y };
}

int ofAppDawnWindow::getWidth() {
	float scale = pixelScale > 0.f ? pixelScale : 1.f;
	return std::max(1, (int)std::lround((float)windowW * scale));
}

int ofAppDawnWindow::getHeight() {
	float scale = pixelScale > 0.f ? pixelScale : 1.f;
	return std::max(1, (int)std::lround((float)windowH * scale));
}

void ofAppDawnWindow::setWindowTitle(std::string title) {
	settings.title = title;
	if(windowP) {
		glfwSetWindowTitle(windowP, title.c_str());
	}
}

void ofAppDawnWindow::setWindowPosition(int x, int y) {
	if(windowP) {
		glfwSetWindowPos(windowP, x, y);
	}
}

void ofAppDawnWindow::setWindowShape(int w, int h) {
	if(windowP) {
		glfwSetWindowSize(windowP, w, h);
	}
	resizeSurface(w, h);
}

void ofAppDawnWindow::setVerticalSync(bool) {}

void * ofAppDawnWindow::getWindowContext() {
	return windowP;
}

void * ofAppDawnWindow::getCocoaWindow() {
#if defined(TARGET_OSX) || defined(TARGET_OF_MAC)
	return windowP ? (__bridge void *)glfwGetCocoaWindow(windowP) : nullptr;
#else
	return nullptr;
#endif
}

void ofAppDawnWindow::makeCurrent() {
	currentDawnWindow = this;
	if(currentRenderer) {
		ofSetCurrentRenderer(currentRenderer, false);
	}
}

ofAppDawnWindow * ofAppDawnWindow::setCurrent(GLFWwindow * windowP_) {
	ofAppDawnWindow * window = static_cast<ofAppDawnWindow *>(glfwGetWindowUserPointer(windowP_));
	if(window) {
		window->makeCurrent();
	}
	return window;
}

void ofAppDawnWindow::mouse_cb(GLFWwindow * windowP_, int button, int state, int mods) {
	auto * window = setCurrent(windowP_);
	if(!window) {
		return;
	}
	float x = window->events().getMouseX();
	float y = window->events().getMouseY();
	if(state == GLFW_PRESS) {
		window->events().notifyMousePressed(x, y, button);
	} else {
		window->events().notifyMouseReleased(x, y, button);
	}
	(void)mods;
}

void ofAppDawnWindow::motion_cb(GLFWwindow * windowP_, double x, double y) {
	auto * window = setCurrent(windowP_);
	if(!window) {
		return;
	}
	if(glfwGetMouseButton(windowP_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		window->events().notifyMouseDragged(x, y, 0);
	} else {
		window->events().notifyMouseMoved(x, y);
	}
}

void ofAppDawnWindow::entry_cb(GLFWwindow * windowP_, int entered) {
	auto * window = setCurrent(windowP_);
	if(!window) {
		return;
	}
	if(entered) {
		window->events().notifyMouseEntered(window->events().getMouseX(), window->events().getMouseY());
	} else {
		window->events().notifyMouseExited(window->events().getMouseX(), window->events().getMouseY());
	}
}

void ofAppDawnWindow::keyboard_cb(GLFWwindow * windowP_, int key, int scancode, int action, int mods) {
	auto * window = setCurrent(windowP_);
	if(!window) {
		return;
	}
	(void)scancode;
	(void)mods;
	if(action == GLFW_PRESS || action == GLFW_REPEAT) {
		window->events().notifyKeyPressed(key);
	} else if(action == GLFW_RELEASE) {
		window->events().notifyKeyReleased(key);
	}
}

void ofAppDawnWindow::resize_cb(GLFWwindow * windowP_, int w, int h) {
	auto * window = setCurrent(windowP_);
	if(!window) {
		return;
	}
	window->resizeSurface(w, h);
	window->events().notifyWindowResized(w, h);
}

void ofAppDawnWindow::framebuffer_cb(GLFWwindow * windowP_, int w, int h) {
	auto * window = setCurrent(windowP_);
	if(!window) {
		return;
	}
	auto * dawn = dynamic_cast<ofWebGPURenderer *>(window->currentRenderer.get());
	if(dawn) {
		dawn->configureSurface(w, h);
	}
}

void ofAppDawnWindow::exit_cb(GLFWwindow * windowP_) {
	auto * window = setCurrent(windowP_);
	if(window) {
		window->events().notifyExit();
	}
}

void ofAppDawnWindow::scroll_cb(GLFWwindow * windowP_, double x, double y) {
	auto * window = setCurrent(windowP_);
	if(window) {
		window->events().notifyMouseScrolled(window->events().getMouseX(), window->events().getMouseY(), x, y);
	}
}

void ofAppDawnWindow::drop_cb(GLFWwindow * windowP_, int numFiles, const char ** dropString) {
	auto * window = setCurrent(windowP_);
	if(!window) {
		return;
	}
	ofDragInfo info;
	for(int i = 0; i < numFiles; ++i) {
		info.files.emplace_back(dropString[i]);
	}
	info.position = { window->events().getMouseX(), window->events().getMouseY() };
	window->events().notifyDragEvent(info);
}

#endif
