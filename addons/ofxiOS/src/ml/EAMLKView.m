//
//  EAMLKView.h
//  iPhone+OF Static Library
//
//  Created by Dan Rosser on 11/5/22.
//

#include <TargetConditionals.h>

#import "EAMLKView.h"
#import <MetalANGLE/MGLKit.h>

#import "ES2Renderer.h"

@interface EAMLKView() {
    BOOL bInit;
    
    // The pixel dimensions of the backbuffer
    GLint backingWidth;
    GLint backingHeight;
    
    MGLContext *context;
    __weak MGLLayer *glLayer;
    
    // Shader objects
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    
    Boolean firstTouch;
    Boolean needsErase;

    // Buffer Objects
    GLuint vboId;

    BOOL initialized;
}
@end

@implementation EAMLKView

@synthesize delegate;
// You must implement this method
+ (Class) layerClass {
    return [MGLLayer class];
}

- (id)initWithCoder:(NSCoder *)coder
{
    if ((self = [super initWithCoder:coder]))
    {
        glLayer = (MGLLayer *)self.layer;

        glLayer.opaque = YES;
        // In this application, we want to retain the EAGLDrawable contents after a call to
        // presentRenderbuffer.
        glLayer.drawableColorFormat   = MGLDrawableColorFormatRGBA8888;
        glLayer.drawableDepthFormat   = MGLDrawableDepthFormatNone;
        glLayer.drawableStencilFormat = MGLDrawableStencilFormatNone;
        glLayer.retainedBacking       = YES;
       
        if(true)
            glLayer.drawableMultisample = MGLDrawableMultisample4X;
        else
            glLayer.drawableMultisample = MGLDrawableMultisampleNone;
        //
//        #if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
////            glLayer.multipleTouchEnabled = true;
//        #endif
        //        self.opaque = true;
        //
        //        [self bindDrawable];
        //
                bInit = YES;

        context = [[MGLContext alloc] initWithAPI:kMGLRenderingAPIOpenGLES2];

        if (!context || ![MGLContext setCurrentContext:context])
        {
            return nil;
        }

        // Set the view's scale factor as you wish
        self.contentScaleFactor = [[UIScreen mainScreen] scale];

        
    }

    return self;
}


// If our view is resized, we'll be asked to layout subviews.
// This is the perfect opportunity to also update the framebuffer so that it is
// the same size as our display area.
- (void)layoutSubviews
{
    [MGLContext setCurrentContext:context forLayer:glLayer];

    if (!initialized)
    {
        initialized = [self initGL];
    }
    else
    {
        [self resizeFromGLLayer];
    }

    // Clear the framebuffer the first time it is allocated
    if (needsErase)
    {
        [self erase];
        needsErase = NO;
    }
}

- (BOOL)resizeFromGLLayer
{
    backingWidth  = (GLint)glLayer.drawableSize.width;
    backingHeight = (GLint)glLayer.drawableSize.height;


    // Update viewport
    glViewport(0, 0, backingWidth, backingHeight);

    return YES;
}

- (BOOL)initGL
{
    backingWidth  = (GLint)glLayer.drawableSize.width;
    backingHeight = (GLint)glLayer.drawableSize.height;
    
    // Setup the view port in Pixels
    glViewport(0, 0, backingWidth, backingHeight);
    
}

// Releases resources when they are not longer needed.
- (void)dealloc
{
    [self destroy];
    // vbo
    if (vboId)
    {
        glDeleteBuffers(1, &vboId);
        vboId = 0;
    }

    // tear down context
    if ([MGLContext currentContext] == context)
        [MGLContext setCurrentContext:nil];
}

- (void)erase
{
    [MGLContext setCurrentContext:context forLayer:glLayer];

    // Clear the buffer
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Display the buffer
    [glLayer present];
}

//- (instancetype)initWithFrame:(CGRect)frame
//          andPreferedRenderer:(ESRendererVersion)version
//                        andAA:(bool)msaaEnabled
//                    andRetina:(bool)retinaEnabled
//               andRetinaScale:(CGFloat)retinaScale
//                  colorFormat:(MGLDrawableColorFormat)colorFormat
//                  depthFormat:(MGLDrawableDepthFormat)depthFormat
//                stencilFormat:(MGLDrawableStencilFormat)stencilFormat {
//
//    if((self = [super initWithFrame:frame])) {
//
//        rendererVersion = version;
//        bUseMSAA = msaaEnabled;
//        bUseRetina = retinaEnabled;
//        //------------------------------------------------------
//        scaleFactorPref = retinaScale;
//
//        if(bUseRetina == YES) {
//            if(scaleFactorPref == 0.0) {
//                scaleFactorPref = [[UIScreen mainScreen] nativeScale]; // no scale preference, default to max scale.
//            } else {
//                if(scaleFactorPref < 1.0) {
//                    scaleFactorPref = 1.0; // invalid negative value, default scale to 1.
//                } else if(scaleFactorPref > [[UIScreen mainScreen] nativeScale]) {
//                    scaleFactorPref = [[UIScreen mainScreen] nativeScale];
//                }
//            }
//        } else {
//            scaleFactorPref = 1.0;
//        }
//
//        [self updateScaleFactor];
//
//        //------------------------------------------------------
//        if(rendererVersion == ESRendererVersion_30) {
//            NSLog(@"OpenGLES 3.0 Renderer not implemented for oF. Defaulting to OpenGLES 2.0");
//            rendererVersion = ESRendererVersion_20;
//        }
//
//        if(rendererVersion == ESRendererVersion_20) {
//            self.context = [[MGLContext alloc] initWithAPI:kMGLRenderingAPIOpenGLES2];
//            NSLog(@"Creating OpenGL ES2 Renderer");
//            if(!self.context) {
//                NSLog(@"OpenGL ES2 failed");
//                rendererVersion = ESRendererVersion_11;
//            }
//        }
//
//
//
//        self.drawableColorFormat = colorFormat;
//        self.drawableDepthFormat = depthFormat;
//        self.drawableStencilFormat = stencilFormat;
//
//        if(msaaEnabled)
//            self.drawableMultisample = MGLDrawableMultisample4X;
//        else
//            self.drawableMultisample = MGLDrawableMultisampleNone;
//
//#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
//        self.multipleTouchEnabled = true;
//#endif
//        self.opaque = true;
//
//        [self bindDrawable];
//
//        bInit = YES;
//    }
//
//    return self;
//}

- (void) destroy {
    if(!bInit) {
        return;
    }
    bInit = NO;
}

//- (void) dealloc{
//    [self destroy];
//}

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
    MGLKView *view = (MGLKView *)self;
    
    scaleFactor = MIN(scaleFactorPref, [view contentScaleFactor]);
    if(scaleFactor != self.contentScaleFactor) {
        self.contentScaleFactor = scaleFactor;
    }
}

- (void) setMSAA:(bool)on
{
    if(on)
        glLayer.drawableMultisample = MGLDrawableMultisample4X;
    else
        glLayer.drawableMultisample = MGLDrawableMultisampleNone;
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

- (BOOL)canBecomeFirstResponder
{
    return YES;
}


@end
