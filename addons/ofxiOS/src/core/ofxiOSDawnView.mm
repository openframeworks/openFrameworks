#include "ofxiOSDawnView.h"
#if defined(TARGET_OF_IOS) && OF_USE_DAWN

#include "ofAppiOSWindow.h"
#include "ofWebGPURenderer.h"
#include "ofxiOSApp.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"

#import <QuartzCore/CAMetalLayer.h>

static ofxiOSDawnView * _instanceRef = nil;

@interface ofxiOSDawnView () {
	ofAppiOSWindow * window;
	ofxiOSApp * app;
	glm::vec2 windowPos;
	glm::vec2 windowSize;
	glm::vec2 screenSize;
	CADisplayLink * displayLink;
	NSMutableDictionary<NSValue *, NSNumber *> * activeTouches;
	BOOL didNotifySetup;
}
- (int)indexForTouch:(UITouch *)touch create:(BOOL)create;
- (CGPoint)pixelPointForTouch:(UITouch *)touch;
@end

@implementation ofxiOSDawnView

+ (Class)layerClass {
	return [CAMetalLayer class];
}

+ (ofxiOSDawnView *)getInstance {
	return _instanceRef;
}

- (instancetype)initWithFrame:(CGRect)frame andApp:(ofxiOSApp *)appPtr {
	self = [super initWithFrame:frame];
	if(!self) {
		return nil;
	}
	_instanceRef = self;
	app = appPtr;
	window = ofAppiOSWindow::getInstance();
	self.multipleTouchEnabled = window ? window->isMultiTouch() : NO;
	self.contentScaleFactor = [UIScreen mainScreen].scale;

	CAMetalLayer * layer = (CAMetalLayer *)self.layer;
	layer.opaque = YES;
	layer.contentsScale = self.contentScaleFactor;
	layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
	layer.framebufferOnly = YES;

	CGFloat scale = self.contentScaleFactor;
	screenSize = { (float)([UIScreen mainScreen].bounds.size.width * scale), (float)([UIScreen mainScreen].bounds.size.height * scale) };
	windowPos = { (float)(frame.origin.x * scale), (float)(frame.origin.y * scale) };
	windowSize = { (float)(frame.size.width * scale), (float)(frame.size.height * scale) };
	activeTouches = [NSMutableDictionary dictionary];
	didNotifySetup = NO;

	if(window) {
		auto renderer = std::dynamic_pointer_cast<ofWebGPURenderer>(window->renderer());
		if(!renderer) {
			renderer = std::make_shared<ofWebGPURenderer>(window);
			window->renderer() = renderer;
			ofSetCurrentRenderer(renderer, true);
		}
		CGSize native = layer.drawableSize;
		if(native.width < 1 || native.height < 1) {
			native = CGSizeMake(frame.size.width * layer.contentsScale, frame.size.height * layer.contentsScale);
			layer.drawableSize = native;
		}
		if(renderer->setup((__bridge void *)layer, (int)native.width, (int)native.height, layer.contentsScale)) {
			window->events().notifySetup();
			didNotifySetup = YES;
		}
	}

	displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawFrame)];
	[displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
	return self;
}

- (void)dealloc {
	[displayLink invalidate];
	displayLink = nil;
	if(_instanceRef == self) {
		_instanceRef = nil;
	}
}

- (void *)metalLayer {
	return (__bridge void *)self.layer;
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

- (void)layoutSubviews {
	[super layoutSubviews];
	CAMetalLayer * layer = (CAMetalLayer *)self.layer;
	CGSize size = self.bounds.size;
	layer.drawableSize = CGSizeMake(size.width * layer.contentsScale, size.height * layer.contentsScale);
	windowSize = { (float)layer.drawableSize.width, (float)layer.drawableSize.height };
	if(window) {
		if(auto renderer = std::dynamic_pointer_cast<ofWebGPURenderer>(window->renderer())) {
			renderer->configureSurface((int)layer.drawableSize.width, (int)layer.drawableSize.height);
		}
		window->events().notifyWindowResized(windowSize.x, windowSize.y);
	}
}

- (void)startRender {
	if(window && window->renderer()) {
		window->renderer()->startRender();
	}
}

- (void)finishRender {
	if(window && window->renderer()) {
		window->renderer()->finishRender();
	}
}

- (void)drawFrame {
	if(!window) {
		return;
	}
	if(!didNotifySetup) {
		return;
	}
	window->events().notifyUpdate();
	[self startRender];
	if(window->isSetupScreenEnabled()) {
		window->renderer()->setupScreen();
	}
	window->events().notifyDraw();
	[self finishRender];
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

- (CGPoint)pixelPointForTouch:(UITouch *)touch {
	CGPoint p = [touch locationInView:self];
	p.x *= self.contentScaleFactor;
	p.y *= self.contentScaleFactor;
	return p;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	(void)event;
	if(!window) {
		return;
	}
	for(UITouch * touch in touches) {
		int touchIndex = [self indexForTouch:touch create:YES];
		CGPoint p = [self pixelPointForTouch:touch];
		if(touchIndex == 0) {
			window->events().notifyMousePressed(p.x, p.y, 0);
		}
		window->events().notifyTouchDown(p.x, p.y, touchIndex);
	}
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	(void)event;
	if(!window) {
		return;
	}
	for(UITouch * touch in touches) {
		int touchIndex = [self indexForTouch:touch create:NO];
		CGPoint p = [self pixelPointForTouch:touch];
		if(touchIndex == 0) {
			window->events().notifyMouseDragged(p.x, p.y, 0);
		}
		window->events().notifyTouchMoved(p.x, p.y, touchIndex);
	}
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	(void)event;
	if(!window) {
		return;
	}
	for(UITouch * touch in touches) {
		int touchIndex = [self indexForTouch:touch create:NO];
		[activeTouches removeObjectForKey:[NSValue valueWithPointer:(__bridge void *)touch]];
		CGPoint p = [self pixelPointForTouch:touch];
		if(touchIndex == 0) {
			window->events().notifyMouseReleased(p.x, p.y, 0);
		}
		window->events().notifyTouchUp(p.x, p.y, touchIndex);
	}
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
	(void)event;
	if(!window) {
		return;
	}
	for(UITouch * touch in touches) {
		int touchIndex = [self indexForTouch:touch create:NO];
		[activeTouches removeObjectForKey:[NSValue valueWithPointer:(__bridge void *)touch]];
		CGPoint p = [self pixelPointForTouch:touch];
		window->events().notifyTouchCancelled(p.x, p.y, touchIndex);
	}
}

@end

@implementation ofxiOSDawnViewController

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app {
	self = [super init];
	if(!self) {
		return nil;
	}
	self.dawnView = [[ofxiOSDawnView alloc] initWithFrame:frame andApp:app];
	return self;
}

- (void)loadView {
	self.view = self.dawnView;
}

- (BOOL)prefersStatusBarHidden {
	return YES;
}

@end

#endif
