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

- (instancetype)initWithFrame:(CGRect)frame
          andPreferedRenderer:(ESRendererVersion)version
                        andAA:(bool)msaaEnabled
                    andRetina:(bool)retinaEnabled
               andRetinaScale:(CGFloat)retinaScale
                   sharegroup:(EAGLSharegroup*)sharegroup
                  colorFormat:(GLKViewDrawableColorFormat)colorFormat
                  depthFormat:(GLKViewDrawableDepthFormat)depthFormat
                stencilFormat:(GLKViewDrawableStencilFormat)stencilFormat {

    if((self = [super initWithFrame:frame])) {
        
        rendererVersion = version;
        bUseMSAA = msaaEnabled;
        bUseRetina = retinaEnabled;
        //------------------------------------------------------
        scaleFactorPref = retinaScale;
        
        if(bUseRetina == YES) {
            if(scaleFactorPref == 0.0) {
                scaleFactorPref = [[UIScreen mainScreen] nativeScale]; // no scale preference, default to max scale.
            } else {
                if(scaleFactorPref < 1.0) {
                    scaleFactorPref = 1.0; // invalid negative value, default scale to 1.
                } else if(scaleFactorPref > [[UIScreen mainScreen] nativeScale]) {
                    scaleFactorPref = [[UIScreen mainScreen] nativeScale];
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
            self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:sharegroup];
            NSLog(@"Creating OpenGL ES2 Renderer");
            if(!self.context) {
                NSLog(@"OpenGL ES2 failed");
                rendererVersion = ESRendererVersion_11;
            }
        }
        
        if(rendererVersion == ESRendererVersion_11) {
            NSLog(@"Creating OpenGL ES1.1 Renderer");
            self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1 sharegroup:sharegroup];
            
            if(!self.context){
                NSLog(@"Critical Error - ofiOS GLKView.m could not start any type of OpenGLES renderer");
                self = nil;
                return nil;
            }
        }
        
        self.drawableColorFormat = colorFormat;
        self.drawableDepthFormat = depthFormat;
        self.drawableStencilFormat = stencilFormat;
        
        if(msaaEnabled)
            self.drawableMultisample = GLKViewDrawableMultisample4X;
        else
            self.drawableMultisample = GLKViewDrawableMultisampleNone;
        
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
        self.multipleTouchEnabled = true;
#endif
        self.opaque = true;
        
        [self bindDrawable];
        
        bInit = YES;
    }
    
    return self;
}

- (void) destroy {
    if(!bInit) {
        return;
    }
    bInit = NO;
}

- (void) dealloc{
    [self destroy];
}

- (void) setup {
    
}
- (void) update{
    
}

- (void) draw{

}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { }
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { }
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { }
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { }
#ifdef __IPHONE_9_1
- (void)touchesEstimatedPropertiesUpdated:(NSSet<UITouch *> *)touches { }
#endif


//-------------------------------------------------------------------
- (void)updateScaleFactor {
    GLKView *view = (GLKView *)self;
    
    scaleFactor = MIN(scaleFactorPref, [view contentScaleFactor]);
    if(scaleFactor != self.contentScaleFactor) {
        self.contentScaleFactor = scaleFactor;
    }
}

- (void) setMSAA:(bool)on
{
    if(on)
        self.drawableMultisample = GLKViewDrawableMultisample4X;
    else
        self.drawableMultisample = GLKViewDrawableMultisampleNone;
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
