#include "ofxiOSMGLView.h"
#if defined(TARGET_OF_IOS) && OF_USE_ANGLE

#include "ofAppiOSWindow.h"
#include "ofGLProgrammableRenderer.h"
#include "ofGLRenderer.h"
#include "ofxiOSApp.h"
#include "ofLog.h"

#import <MGLKit/MGLKView.h>

static ofxiOSMGLViewController * _instanceRef = nil;

@class ofxiOSMGLViewController;

/// Touch and layout forwarding only. Drawing stays on the MGLKViewDelegate.
@interface ofxiOSMGLKitView : MGLKView
@property (nonatomic, weak) ofxiOSMGLViewController * owner;
@end

@interface ofxiOSMGLViewController () <MGLKViewDelegate> {
	ofAppiOSWindow * window;
	ofxiOSApp * app;
	glm::vec2 windowPos;
	glm::vec2 windowSize;
	glm::vec2 screenSize;
	MGLContext * context;
	ofxiOSMGLKitView * glView;
	CADisplayLink * displayLink;
	NSMutableDictionary<NSValue *, NSNumber *> * activeTouches;
	BOOL didNotifySetup;
	CGFloat pixelScale;
}
- (void)updateDimensions;
- (int)indexForTouch:(UITouch *)touch create:(BOOL)create;
- (CGPoint)pixelPointForTouch:(UITouch *)touch inView:(UIView *)view;
@end

@implementation ofxiOSMGLKitView

- (void)layoutSubviews {
	[super layoutSubviews];
	[self.owner updateDimensions];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	[self.owner touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	[self.owner touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	[self.owner touchesEnded:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	[self.owner touchesCancelled:touches withEvent:event];
}

@end

@implementation ofxiOSMGLViewController

+ (ofxiOSMGLViewController *)getInstance {
	return _instanceRef;
}

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)appPtr {
	self = [super init];
	if(!self) {
		return nil;
	}
	_instanceRef = self;
	app = appPtr;
	window = ofAppiOSWindow::getInstance();
	activeTouches = [NSMutableDictionary dictionary];
	didNotifySetup = NO;
	pixelScale = 1.f;
	if(window && window->isRetinaEnabled()) {
		pixelScale = [UIScreen mainScreen].scale;
		if(pixelScale < 1.f) {
			pixelScale = 1.f;
		}
	}

	MGLRenderingAPI api = kMGLRenderingAPIOpenGLES2;
	if(window) {
		const int gles = window->getSettings().glesVersion;
		if(gles <= 1) {
			api = kMGLRenderingAPIOpenGLES1;
		} else if(gles >= 3) {
			api = kMGLRenderingAPIOpenGLES3;
		}
	}
	context = [[MGLContext alloc] initWithAPI:api];
	if(!context) {
		ofLogError("ofxiOSMGLView") << "MGLContext initWithAPI failed";
		return self;
	}

	glView = [[ofxiOSMGLKitView alloc] initWithFrame:frame context:context];
	glView.owner = self;
	glView.delegate = self;
	glView.enableSetNeedsDisplay = NO;
	glView.contentScaleFactor = pixelScale;
	glView.multipleTouchEnabled = window ? window->isMultiTouch() : NO;
	glView.drawableColorFormat = MGLDrawableColorFormatRGBA8888;
	if(window) {
		if(window->getSettings().enableDepth) {
			if(window->getRendererDepthType() == DEPTH_24) {
				glView.drawableDepthFormat = MGLDrawableDepthFormat24;
			} else if(window->getRendererDepthType() == DEPTH_16) {
				glView.drawableDepthFormat = MGLDrawableDepthFormat16;
			}
		}
		if(window->getRendererStencilType() == STENCIL_8) {
			glView.drawableStencilFormat = MGLDrawableStencilFormat8;
		}
		if(window->isAntiAliasingEnabled()) {
			glView.drawableMultisample = MGLDrawableMultisample4X;
		}
	}
	[self updateDimensions];
	return self;
}

- (void)dealloc {
	[displayLink invalidate];
	displayLink = nil;
	if(_instanceRef == self) {
		_instanceRef = nil;
	}
}

- (void)loadView {
	self.view = glView ? glView : [[UIView alloc] initWithFrame:CGRectZero];
}

- (void)viewDidAppear:(BOOL)animated {
	[super viewDidAppear:animated];
	if(!displayLink && glView && context) {
		displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(tick:)];
		[displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
	}
}

- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
	[displayLink invalidate];
	displayLink = nil;
}

- (BOOL)prefersStatusBarHidden {
	return YES;
}

- (glm::vec2 *)getWindowPosition {
	return &windowPos;
}

- (glm::vec2 *)getWindowSize {
	return &windowSize;
}

- (glm::vec2 *)getScreenSize {
	return &screenSize;
}

- (void)updateDimensions {
	if(!glView) {
		return;
	}
	CGSize drawable = glView.drawableSize;
	if(drawable.width < 1.f || drawable.height < 1.f) {
		drawable = CGSizeMake(glView.bounds.size.width * pixelScale, glView.bounds.size.height * pixelScale);
	}
	glm::vec2 nextSize = { (float)drawable.width, (float)drawable.height };
	bool changed = nextSize.x != windowSize.x || nextSize.y != windowSize.y;
	windowPos = { (float)(glView.frame.origin.x * pixelScale), (float)(glView.frame.origin.y * pixelScale) };
	windowSize = nextSize;
	UIScreen * screen = glView.window.screen ?: [UIScreen mainScreen];
	screenSize = { (float)(screen.bounds.size.width * pixelScale), (float)(screen.bounds.size.height * pixelScale) };
	if(window && didNotifySetup && changed) {
		window->events().notifyWindowResized(windowSize.x, windowSize.y);
	}
}

- (void)tick:(CADisplayLink *)link {
	(void)link;
	[glView display];
}

- (void)mglkView:(MGLKView *)view drawInRect:(CGRect)rect {
	(void)rect;
	if(!window || !window->renderer()) {
		return;
	}
	[self updateDimensions];
	if(!didNotifySetup) {
		[view bindDrawable];
		const int gles = window->getSettings().glesVersion;
		if(window->isProgrammableRenderer()) {
			int major = gles >= 3 ? 3 : 2;
			int minor = 0;
#if defined(OF_TEST_GLES_MINOR)
			minor = OF_TEST_GLES_MINOR;
#endif
			static_cast<ofGLProgrammableRenderer *>(window->renderer().get())->setup(major, minor);
		} else {
			static_cast<ofGLRenderer *>(window->renderer().get())->setup();
		}
		const char * version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
		const char * rendererName = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
		ofLogNotice("ofxiOSMGLView") << "ANGLE context ready " << (version ? version : "?")
			<< " renderer " << (rendererName ? rendererName : "?")
			<< " " << (int)windowSize.x << "x" << (int)windowSize.y;
		window->events().notifySetup();
		didNotifySetup = YES;
	}
	window->events().notifyUpdate();
	window->renderer()->startRender();
	if(window->isSetupScreenEnabled()) {
		window->renderer()->setupScreen();
	}
	window->events().notifyDraw();
	window->renderer()->finishRender();
}

- (int)indexForTouch:(UITouch *)touch create:(BOOL)create {
	NSValue * key = [NSValue valueWithPointer:(__bridge void *)touch];
	NSNumber * existing = activeTouches[key];
	if(existing) {
		return existing.intValue;
	}
	if(!create) {
		return 0;
	}
	int touchIndex = 0;
	while([activeTouches.allValues containsObject:@(touchIndex)]) {
		touchIndex++;
	}
	activeTouches[key] = @(touchIndex);
	return touchIndex;
}

- (CGPoint)pixelPointForTouch:(UITouch *)touch inView:(UIView *)view {
	CGPoint p = [touch locationInView:view];
	p.x *= pixelScale;
	p.y *= pixelScale;
	return p;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	(void)event;
	if(!window || !didNotifySetup) {
		return;
	}
	for(UITouch * touch in touches) {
		int touchIndex = [self indexForTouch:touch create:YES];
		CGPoint p = [self pixelPointForTouch:touch inView:glView];
		if(touchIndex == 0) {
			window->events().notifyMousePressed(p.x, p.y, 0);
		}
		window->events().notifyTouchDown(p.x, p.y, touchIndex);
	}
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	(void)event;
	if(!window || !didNotifySetup) {
		return;
	}
	for(UITouch * touch in touches) {
		int touchIndex = [self indexForTouch:touch create:NO];
		CGPoint p = [self pixelPointForTouch:touch inView:glView];
		if(touchIndex == 0) {
			window->events().notifyMouseDragged(p.x, p.y, 0);
		}
		window->events().notifyTouchMoved(p.x, p.y, touchIndex);
	}
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	(void)event;
	if(!window || !didNotifySetup) {
		return;
	}
	for(UITouch * touch in touches) {
		int touchIndex = [self indexForTouch:touch create:NO];
		[activeTouches removeObjectForKey:[NSValue valueWithPointer:(__bridge void *)touch]];
		CGPoint p = [self pixelPointForTouch:touch inView:glView];
		if(touchIndex == 0) {
			window->events().notifyMouseReleased(p.x, p.y, 0);
		}
		window->events().notifyTouchUp(p.x, p.y, touchIndex);
	}
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	(void)event;
	if(!window || !didNotifySetup) {
		return;
	}
	for(UITouch * touch in touches) {
		int touchIndex = [self indexForTouch:touch create:NO];
		[activeTouches removeObjectForKey:[NSValue valueWithPointer:(__bridge void *)touch]];
		CGPoint p = [self pixelPointForTouch:touch inView:glView];
		window->events().notifyTouchCancelled(p.x, p.y, touchIndex);
	}
}

@end

// The static lib still compiles the EAGL / GLKit views. ANGLE never creates
// them, but the linker still needs these Apple-only symbols, and OpenGLES
// cannot be linked beside MetalANGLE.
@implementation EAGLContext
@end

@interface GLKView : UIView
@end
@implementation GLKView
@end

@interface GLKViewController : UIViewController
@end
@implementation GLKViewController
@end

extern "C" void glRenderbufferStorageMultisampleAPPLE(unsigned int, int, unsigned int, int, int) {}
extern "C" void glResolveMultisampleFramebufferAPPLE(void) {}

NSString * const kEAGLDrawablePropertyColorFormat = @"EAGLDrawablePropertyColorFormat";
NSString * const kEAGLColorFormatRGBA8 = @"EAGLColorFormatRGBA8";
NSString * const kEAGLDrawablePropertyRetainedBacking = @"EAGLDrawablePropertyRetainedBacking";

#endif
