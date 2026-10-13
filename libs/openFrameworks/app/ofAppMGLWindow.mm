#include "ofAppMGLWindow.h"
#if OF_USE_ANGLE && defined(TARGET_OSX) && !defined(TARGET_OF_IOS)

#include "ofGLProgrammableRenderer.h"
#include "ofGLRenderer.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofLog.h"
#include <cmath>

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <MGLKit/MGLKit.h>

@class ofMGLWindowOwner;

@interface ofMGLKitView : MGLKView
@property (nonatomic, weak) ofMGLWindowOwner * owner;
@end

@interface ofMGLWindowOwner : NSObject <NSWindowDelegate, MGLKViewDelegate>
@property (nonatomic, strong) NSWindow * window;
@property (nonatomic, strong) ofMGLKitView * glView;
@property (nonatomic, strong) MGLContext * context;
@property (nonatomic, assign) ofAppMGLWindow * cppWindow;
- (void)noteResize;
- (glm::vec2)pointForEvent:(NSEvent *)event;
- (void)mouseDown:(NSEvent *)event;
- (void)mouseUp:(NSEvent *)event;
- (void)mouseDragged:(NSEvent *)event;
- (void)mouseMoved:(NSEvent *)event;
- (void)rightMouseDown:(NSEvent *)event;
- (void)rightMouseUp:(NSEvent *)event;
- (void)rightMouseDragged:(NSEvent *)event;
- (void)otherMouseDown:(NSEvent *)event;
- (void)otherMouseUp:(NSEvent *)event;
- (void)otherMouseDragged:(NSEvent *)event;
- (void)scrollWheel:(NSEvent *)event;
- (void)keyEvent:(NSEvent *)event pressed:(BOOL)pressed;
@end

namespace {

int ofKeyFromEvent(NSEvent * event) {
	switch (event.keyCode) {
		case 53: return OF_KEY_ESC;
		case 36: return OF_KEY_RETURN;
		case 48: return OF_KEY_TAB;
		case 51: return OF_KEY_BACKSPACE;
		case 117: return OF_KEY_DEL;
		case 123: return OF_KEY_LEFT;
		case 124: return OF_KEY_RIGHT;
		case 125: return OF_KEY_DOWN;
		case 126: return OF_KEY_UP;
		case 115: return OF_KEY_HOME;
		case 119: return OF_KEY_END;
		case 116: return OF_KEY_PAGE_UP;
		case 121: return OF_KEY_PAGE_DOWN;
		default: break;
	}
	NSString * chars = event.charactersIgnoringModifiers;
	if (chars.length > 0) {
		unichar c = [chars characterAtIndex:0];
		if (c == 27) {
			return OF_KEY_ESC;
		}
		if (c == 127 || c == 8) {
			return OF_KEY_BACKSPACE;
		}
		if (c >= 32 && c < 127) {
			return (int)c;
		}
	}
	static const int fKeys[] = { 122, 120, 99, 118, 96, 97, 98, 100, 101, 109, 103, 111 };
	static const int fOf[] = {
		OF_KEY_F1, OF_KEY_F2, OF_KEY_F3, OF_KEY_F4, OF_KEY_F5, OF_KEY_F6,
		OF_KEY_F7, OF_KEY_F8, OF_KEY_F9, OF_KEY_F10, OF_KEY_F11, OF_KEY_F12
	};
	for (int i = 0; i < 12; ++i) {
		if (event.keyCode == fKeys[i]) {
			return fOf[i];
		}
	}
	return 0;
}

int buttonForType(NSEventType type) {
	switch (type) {
		case NSEventTypeRightMouseDown:
		case NSEventTypeRightMouseUp:
		case NSEventTypeRightMouseDragged:
			return OF_MOUSE_BUTTON_RIGHT;
		case NSEventTypeOtherMouseDown:
		case NSEventTypeOtherMouseUp:
		case NSEventTypeOtherMouseDragged:
			return OF_MOUSE_BUTTON_MIDDLE;
		default:
			return OF_MOUSE_BUTTON_LEFT;
	}
}

void glesVersionFromSettings(const ofWindowSettings & settings, int & major, int & minor) {
	major = 2;
	minor = 0;
	if (const auto * gles = dynamic_cast<const ofGLESWindowSettings *>(&settings)) {
		major = gles->glesVersion;
		minor = gles->glesVersionMinor;
	} else if (const auto * gl = dynamic_cast<const ofGLWindowSettings *>(&settings)) {
		major = gl->glVersionMajor;
		if (major >= 3) {
			major = 3;
		} else if (major <= 1) {
			major = 1;
		} else {
			major = 2;
		}
		minor = 0;
	}
#if defined(OF_TEST_GLES_VERSION)
	major = OF_TEST_GLES_VERSION;
#endif
#if defined(OF_TEST_GLES_MINOR)
	minor = OF_TEST_GLES_MINOR;
#endif
}

} // namespace

@implementation ofMGLKitView

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (void)layout {
	[super layout];
	[self.owner noteResize];
}

- (void)mouseDown:(NSEvent *)event { [self.owner mouseDown:event]; }
- (void)mouseUp:(NSEvent *)event { [self.owner mouseUp:event]; }
- (void)mouseDragged:(NSEvent *)event { [self.owner mouseDragged:event]; }
- (void)mouseMoved:(NSEvent *)event { [self.owner mouseMoved:event]; }
- (void)rightMouseDown:(NSEvent *)event { [self.owner rightMouseDown:event]; }
- (void)rightMouseUp:(NSEvent *)event { [self.owner rightMouseUp:event]; }
- (void)rightMouseDragged:(NSEvent *)event { [self.owner rightMouseDragged:event]; }
- (void)otherMouseDown:(NSEvent *)event { [self.owner otherMouseDown:event]; }
- (void)otherMouseUp:(NSEvent *)event { [self.owner otherMouseUp:event]; }
- (void)otherMouseDragged:(NSEvent *)event { [self.owner otherMouseDragged:event]; }
- (void)scrollWheel:(NSEvent *)event { [self.owner scrollWheel:event]; }
- (void)keyDown:(NSEvent *)event { [self.owner keyEvent:event pressed:YES]; }
- (void)keyUp:(NSEvent *)event { [self.owner keyEvent:event pressed:NO]; }

@end

@implementation ofMGLWindowOwner

- (glm::vec2)pointForEvent:(NSEvent *)event {
	if (!self.glView || !self.cppWindow) {
		return { 0.f, 0.f };
	}
	NSPoint p = [self.glView convertPoint:event.locationInWindow fromView:nil];
	const float scale = self.glView.layer.contentsScale > 0.f ? self.glView.layer.contentsScale : 1.f;
	const float height = self.glView.bounds.size.height;
	return { (float)p.x * scale, (height - (float)p.y) * scale };
}

- (void)noteResize {
	if (self.cppWindow) {
		const int previousW = self.cppWindow->getWidth();
		const int previousH = self.cppWindow->getHeight();
		self.cppWindow->updateDrawableSize();
		if (previousW != self.cppWindow->getWidth() || previousH != self.cppWindow->getHeight()) {
			self.cppWindow->events().notifyWindowResized(self.cppWindow->getWidth(), self.cppWindow->getHeight());
		}
	}
}

- (void)mouseDown:(NSEvent *)event {
	if (!self.cppWindow) return;
	glm::vec2 p = [self pointForEvent:event];
	self.cppWindow->events().notifyMousePressed(p.x, p.y, buttonForType(event.type));
}
- (void)mouseUp:(NSEvent *)event {
	if (!self.cppWindow) return;
	glm::vec2 p = [self pointForEvent:event];
	self.cppWindow->events().notifyMouseReleased(p.x, p.y, buttonForType(event.type));
}
- (void)mouseDragged:(NSEvent *)event {
	if (!self.cppWindow) return;
	glm::vec2 p = [self pointForEvent:event];
	self.cppWindow->events().notifyMouseDragged(p.x, p.y, buttonForType(event.type));
}
- (void)mouseMoved:(NSEvent *)event {
	if (!self.cppWindow) return;
	glm::vec2 p = [self pointForEvent:event];
	self.cppWindow->events().notifyMouseMoved(p.x, p.y);
}
- (void)rightMouseDown:(NSEvent *)event { [self mouseDown:event]; }
- (void)rightMouseUp:(NSEvent *)event { [self mouseUp:event]; }
- (void)rightMouseDragged:(NSEvent *)event { [self mouseDragged:event]; }
- (void)otherMouseDown:(NSEvent *)event { [self mouseDown:event]; }
- (void)otherMouseUp:(NSEvent *)event { [self mouseUp:event]; }
- (void)otherMouseDragged:(NSEvent *)event { [self mouseDragged:event]; }

- (void)scrollWheel:(NSEvent *)event {
	if (!self.cppWindow) return;
	glm::vec2 p = [self pointForEvent:event];
	self.cppWindow->events().notifyMouseScrolled(p.x, p.y, event.scrollingDeltaX, event.scrollingDeltaY);
}

- (void)keyEvent:(NSEvent *)event pressed:(BOOL)pressed {
	if (!self.cppWindow) return;
	const int key = ofKeyFromEvent(event);
	if (key == 0) return;
	if (pressed) {
		self.cppWindow->events().notifyKeyPressed(key);
	} else {
		self.cppWindow->events().notifyKeyReleased(key);
	}
}

- (void)mglkView:(MGLKView *)view drawInRect:(CGRect)rect {
	(void)view;
	(void)rect;
	if (self.cppWindow) {
		self.cppWindow->renderFrame();
	}
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
	(void)sender;
	if (self.cppWindow) {
		self.cppWindow->setWindowShouldClose();
	}
	return NO;
}

@end

ofAppMGLWindow::ofAppMGLWindow()
	: coreEvents(new ofCoreEvents) {
}

ofAppMGLWindow::~ofAppMGLWindow() {
	close();
}

void ofAppMGLWindow::pollEvents() {
	@autoreleasepool {
		while (NSEvent * event = [NSApp nextEventMatchingMask:NSEventMaskAny
													 untilDate:[NSDate distantPast]
														inMode:NSDefaultRunLoopMode
													   dequeue:YES]) {
			[NSApp sendEvent:event];
		}
	}
}

void ofAppMGLWindow::setup(const ofWindowSettings & _settings) {
	if (bSetup) {
		ofLogError("ofAppMGLWindow") << "window already setup";
		return;
	}
	settings = _settings;
	// settings is ofWindowSettings, so the copy drops ofGLWindowSettings.
	// Read the version from the caller's object.
	glesVersionFromSettings(_settings, glesMajor, glesMinor);

	[NSApplication sharedApplication];
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	if (![[NSRunningApplication currentApplication] isFinishedLaunching]) {
		[NSApp finishLaunching];
	}

	const int width = settings.isSizeSet() ? settings.getWidth() : 1024;
	const int height = settings.isSizeSet() ? settings.getHeight() : 768;
	NSRect frame = NSMakeRect(0, 0, width, height);
	NSWindowStyleMask style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

	ofMGLWindowOwner * windowOwner = [[ofMGLWindowOwner alloc] init];
	windowOwner.cppWindow = this;
	windowOwner.window = [[NSWindow alloc] initWithContentRect:frame
													  styleMask:style
														backing:NSBackingStoreBuffered
														  defer:NO];
	NSString * title = settings.title.empty() ? @"openFrameworks (ANGLE)" : [NSString stringWithUTF8String:settings.title.c_str()];
	windowOwner.window.title = title;
	windowOwner.window.delegate = windowOwner;
	if (settings.isPositionSet()) {
		[windowOwner.window setFrameOrigin:NSMakePoint(settings.getPosition().x, settings.getPosition().y)];
	} else {
		[windowOwner.window center];
	}

	MGLRenderingAPI api = kMGLRenderingAPIOpenGLES2;
	if (glesMajor <= 1) {
		api = kMGLRenderingAPIOpenGLES1;
	} else if (glesMajor >= 3) {
		api = kMGLRenderingAPIOpenGLES3;
	}
	windowOwner.context = [[MGLContext alloc] initWithAPI:api];
	if (!windowOwner.context) {
		ofLogError("ofAppMGLWindow") << "MGLContext initWithAPI failed";
		return;
	}

	windowOwner.glView = [[ofMGLKitView alloc] initWithFrame:frame context:windowOwner.context];
	windowOwner.glView.owner = windowOwner;
	windowOwner.glView.delegate = windowOwner;
	windowOwner.glView.drawableColorFormat = MGLDrawableColorFormatRGBA8888;
	windowOwner.glView.drawableDepthFormat = MGLDrawableDepthFormat24;
	pixelScale = settings.isHiDPI() ? windowOwner.window.backingScaleFactor : 1.f;
	if (pixelScale < 1.f) {
		pixelScale = 1.f;
	}
	windowOwner.glView.wantsLayer = YES;
	windowOwner.glView.layer.contentsScale = pixelScale;
	windowOwner.window.contentView = windowOwner.glView;

	NSTrackingArea * tracking = [[NSTrackingArea alloc] initWithRect:windowOwner.glView.bounds
															  options:NSTrackingMouseMoved | NSTrackingActiveInKeyWindow | NSTrackingInVisibleRect
																owner:windowOwner.glView
															 userInfo:nil];
	[windowOwner.glView addTrackingArea:tracking];

	owner = (__bridge_retained void *)windowOwner;
	updateDrawableSize();

	if (glesMajor >= 2) {
		currentRenderer = std::make_shared<ofGLProgrammableRenderer>(this);
	} else {
		currentRenderer = std::make_shared<ofGLRenderer>(this);
	}
	makeCurrent();
	[windowOwner.glView bindDrawable];
	if (glesMajor >= 2) {
		static_cast<ofGLProgrammableRenderer *>(currentRenderer.get())->setup(glesMajor, glesMinor);
	} else {
		static_cast<ofGLRenderer *>(currentRenderer.get())->setup();
	}
	ofSetCurrentRenderer(currentRenderer, true);

	const char * version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	const char * rendererName = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
	ofLogNotice("ofAppMGLWindow") << "ANGLE context ready " << (version ? version : "?")
		<< " renderer " << (rendererName ? rendererName : "?")
		<< " " << windowW << "x" << windowH << " scale=" << pixelScale;

	bSetup = true;
	bWindowNeedsShowing = true;
}

void ofAppMGLWindow::updateDrawableSize() {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (!windowOwner.glView) {
		return;
	}
	CGSize drawable = windowOwner.glView.drawableSize;
	if (drawable.width < 1.0 || drawable.height < 1.0) {
		drawable = CGSizeMake(windowOwner.glView.bounds.size.width * pixelScale,
							  windowOwner.glView.bounds.size.height * pixelScale);
	}
	windowW = std::max(1, (int)std::lround(drawable.width));
	windowH = std::max(1, (int)std::lround(drawable.height));
}

void ofAppMGLWindow::showWindow() {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (!windowOwner.window) {
		return;
	}
	[windowOwner.window makeKeyAndOrderFront:nil];
	[windowOwner.window makeFirstResponder:windowOwner.glView];
	[NSApp activate];
	if (settings.windowMode == OF_FULLSCREEN) {
		[windowOwner.window toggleFullScreen:nil];
	}
	bWindowNeedsShowing = false;
}

void ofAppMGLWindow::update() {
	events().notifyUpdate();
}

void ofAppMGLWindow::draw() {
	if (bWindowNeedsShowing) {
		showWindow();
	}
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (windowOwner.glView) {
		[windowOwner.glView display];
	}
}

void ofAppMGLWindow::renderFrame() {
	if (!currentRenderer) {
		return;
	}
	updateDrawableSize();
	makeCurrent();
	currentRenderer->startRender();
	if (currentRenderer->getBackgroundAuto()) {
		currentRenderer->background(currentRenderer->getBackgroundColor());
	}
	if (bEnableSetupScreen) {
		currentRenderer->setupScreen();
	}
	events().notifyDraw();
	currentRenderer->finishRender();
}

void ofAppMGLWindow::close() {
	if (!owner) {
		currentRenderer.reset();
		return;
	}
	ofMGLWindowOwner * windowOwner = (__bridge_transfer ofMGLWindowOwner *)owner;
	owner = nullptr;
	windowOwner.cppWindow = nil;
	[windowOwner.window orderOut:nil];
	windowOwner.window.delegate = nil;
	windowOwner.window = nil;
	windowOwner.glView = nil;
	windowOwner.context = nil;
	currentRenderer.reset();
}

bool ofAppMGLWindow::getWindowShouldClose() {
	return bShouldClose || owner == nullptr;
}

void ofAppMGLWindow::setWindowShouldClose() {
	bShouldClose = true;
}

ofCoreEvents & ofAppMGLWindow::events() {
	return *coreEvents;
}

std::shared_ptr<ofBaseRenderer> & ofAppMGLWindow::renderer() {
	return currentRenderer;
}

glm::vec2 ofAppMGLWindow::getWindowSize() {
	return { (float)windowW, (float)windowH };
}

glm::vec2 ofAppMGLWindow::getScreenSize() {
	NSScreen * screen = [NSScreen mainScreen];
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (windowOwner.window.screen) {
		screen = windowOwner.window.screen;
	}
	if (!screen) {
		return getWindowSize();
	}
	return { (float)screen.frame.size.width * pixelScale, (float)screen.frame.size.height * pixelScale };
}

glm::vec2 ofAppMGLWindow::getWindowPosition() {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (!windowOwner.window) {
		return { 0.f, 0.f };
	}
	NSRect frame = windowOwner.window.frame;
	return { (float)frame.origin.x, (float)frame.origin.y };
}

int ofAppMGLWindow::getWidth() {
	return std::max(1, windowW);
}

int ofAppMGLWindow::getHeight() {
	return std::max(1, windowH);
}

void ofAppMGLWindow::setWindowTitle(std::string title) {
	settings.title = title;
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (windowOwner.window) {
		windowOwner.window.title = [NSString stringWithUTF8String:title.c_str()];
	}
}

void ofAppMGLWindow::setWindowPosition(int x, int y) {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (windowOwner.window) {
		[windowOwner.window setFrameOrigin:NSMakePoint(x, y)];
	}
}

void ofAppMGLWindow::setWindowShape(int w, int h) {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (!windowOwner.window) {
		return;
	}
	NSRect frame = windowOwner.window.frame;
	frame.size = NSMakeSize(w, h);
	[windowOwner.window setFrame:frame display:YES];
	updateDrawableSize();
}

void ofAppMGLWindow::setFullscreen(bool fullscreen) {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (!windowOwner.window) {
		settings.windowMode = fullscreen ? OF_FULLSCREEN : OF_WINDOW;
		return;
	}
	const bool isFullscreen = (windowOwner.window.styleMask & NSWindowStyleMaskFullScreen) != 0;
	if (isFullscreen != fullscreen) {
		[windowOwner.window toggleFullScreen:nil];
	}
	settings.windowMode = fullscreen ? OF_FULLSCREEN : OF_WINDOW;
}

void ofAppMGLWindow::toggleFullscreen() {
	setFullscreen(getWindowMode() != OF_FULLSCREEN);
}

ofWindowMode ofAppMGLWindow::getWindowMode() {
	return settings.windowMode;
}

void ofAppMGLWindow::enableSetupScreen() {
	bEnableSetupScreen = true;
}

void ofAppMGLWindow::disableSetupScreen() {
	bEnableSetupScreen = false;
}

void ofAppMGLWindow::setVerticalSync(bool enabled) {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	CALayer * layer = windowOwner.glView.glLayer;
	for(CALayer * sublayer in layer.sublayers) {
		if([sublayer isKindOfClass:[CAMetalLayer class]]) {
			((CAMetalLayer *)sublayer).displaySyncEnabled = enabled ? YES : NO;
		}
	}
}

void * ofAppMGLWindow::getWindowContext() {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	return (__bridge void *)windowOwner.context;
}

void * ofAppMGLWindow::getCocoaWindow() {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	return (__bridge void *)windowOwner.window;
}

void ofAppMGLWindow::makeCurrent() {
	ofMGLWindowOwner * windowOwner = (__bridge ofMGLWindowOwner *)owner;
	if (windowOwner.context) {
		[MGLContext setCurrentContext:windowOwner.context forLayer:windowOwner.glView.glLayer];
	}
	if (currentRenderer) {
		ofSetCurrentRenderer(currentRenderer, false);
	}
}

#endif
