//
//  EAGLKView.h
//  iPhone+OF Static Library
//
//  Created by Dan Rosser on 7/3/18.
//

#include <TargetConditionals.h>

#import "EAGLKView.h"

#import "ES1Renderer.h"
#import "ES2Renderer.h"

@interface EAGLKView() {
    BOOL bInit;
}
@end

@implementation EAGLKView

@synthesize delegate;
// You must implement this method
+ (Class) layerClass {
	return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
andPreferedRenderer:(ESRendererVersion)version
           andDepth:(bool)depth
              andAA:(bool)msaaEnabled
      andNumSamples:(int)samples
          andRetina:(bool)retinaEnabled
     andRetinaScale:(CGFloat)retinaScale {

	if((self = [super initWithFrame:frame])) {
        
        rendererVersion = version;
        bUseDepth = depth;
        bUseMSAA = msaaEnabled;
        bUseRetina = retinaEnabled;
        msaaSamples = samples;

        //------------------------------------------------------
//        CAEAGLLayer * eaglLayer = (CAEAGLLayer *)super.layer;
//        eaglLayer.opaque = true;
//		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
//										[NSNumber numberWithBool:YES], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
//
        //------------------------------------------------------
        scaleFactorPref = retinaScale;
        
        if(bUseRetina == YES) {
            if(scaleFactorPref == 0.0) {
                scaleFactorPref = [[UIScreen mainScreen] scale]; // no scale preference, default to max scale.
            } else {
                if(scaleFactorPref < 1.0) {
                    scaleFactorPref = 1.0; // invalid negative value, default scale to 1.
                } else if(scaleFactorPref > [[UIScreen mainScreen] scale]) {
                    scaleFactorPref = [[UIScreen mainScreen] scale];
                }
            }
        } else {
            scaleFactorPref = 1.0;
        }
        
        [self updateScaleFactor];
        
        //------------------------------------------------------
        if(rendererVersion == ESRendererVersion_30) {
            NSLog(@"OpenGLES 3.0 Renderer not implemented for oF. Defaulting to OpenGLES 2.0");
            rendererVersion = ESRendererVersion_20;
        }
        
        if(rendererVersion == ESRendererVersion_20) {
            renderer = [[ES2Renderer alloc] initWithDepth:bUseDepth
                                                    andAA:bUseMSAA
                                           andMSAASamples:msaaSamples
                                                andRetina:bUseRetina
												 andGLKit:true
											   sharegroup:nil];
        }
		
        if(!renderer){ // if OpenGLES 2.0 fails to load try OpenGLES 1.1
            rendererVersion = ESRendererVersion_11;
            renderer = [[ES1Renderer alloc] initWithDepth:bUseDepth 
                                                    andAA:bUseMSAA 
                                           andMSAASamples:msaaSamples 
                                                andRetina:bUseRetina
												 andGLKit:true
											   sharegroup:nil];
			
            if(!renderer){
                NSLog(@"Critical Error - ofiOS EAGLView.m could not start any type of OpenGLES renderer");
				[self release];
				return nil;
			}
            
            
        }
		
		self.context = renderer;

#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
		self.multipleTouchEnabled = true;
#endif
		self.opaque = true;

        glLock = [[NSLock alloc] init];

        bInit = YES;
	}
	
	return self;
}

- (void) destroy {
    if(!bInit) {
        return;
    }
    [renderer release];
    [glLock release];
    
    bInit = NO;
}

- (void) dealloc{
    [self destroy];
	[super dealloc];
}

- (void) setup {
	
}
- (void) update{
	
}

- (void) draw{

}

- (void)startRender {
    [renderer startRender];
}
	
- (void)finishRender {
    [renderer finishRender];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { }
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { }
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { }
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { }
#ifdef __IPHONE_9_1
- (void)touchesEstimatedPropertiesUpdated:(NSSet<UITouch *> *)touches { }
#endif
	
- (void)layoutSubviews{
    [self updateScaleFactor];

    [renderer startRender];
    [renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
    [renderer finishRender];
    
    [self notifyResized];
}

//-------------------------------------------------------------------
- (void)updateScaleFactor {
	
    
    GLKView *view = (GLKView *)self;
	
    scaleFactor = MIN(scaleFactorPref, [view contentScaleFactor]);
    if(scaleFactor != self.contentScaleFactor) {
        self.contentScaleFactor = scaleFactor;
    }
}

//------------------------------------------------------------------- lock/unlock GL context.
- (void)lockGL {
	[glLock lock];
}

- (void)unlockGL {
	[glLock unlock];
}

//------------------------------------------------------------------- getters.
-(EAGLContext*) context{
	return [renderer context];
}

- (GLint)getWidth {
    return [renderer getWidth];
} 

- (GLint)getHeight {
    return [renderer getHeight];
}

//------------------------------------------------------------------- notify.
- (void) notifyAnimationStarted {
    if([self.delegate respondsToSelector:@selector(glViewAnimationStarted)]) {
        [self.delegate glViewAnimationStarted];
    }
}

- (void) notifyAnimationStopped {
    if([self.delegate respondsToSelector:@selector(glViewAnimationStopped)]) {
        [self.delegate glViewAnimationStopped];
    }
}

- (void) notifyDraw {
    if([self.delegate respondsToSelector:@selector(glViewDraw)]) {
        [self.delegate glViewDraw];
    }
}

- (void) notifyResized {
    if([self.delegate respondsToSelector:@selector(glViewResized)]) {
        [self.delegate glViewResized];
    }
}


@end
