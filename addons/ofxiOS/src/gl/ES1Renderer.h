
#pragma once

#import "ESRenderer.h"
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface ES1Renderer : NSObject <ESRenderer>
{
@private
    EAGLContext *context;

    // The pixel dimensions of the CAEAGLLayer
    GLint backingWidth;
    GLint backingHeight;

    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view
    GLuint defaultFramebuffer, colorRenderbuffer, depthRenderbuffer;
    GLuint msaaFrameBuffer, msaaColorRenderBuffer;
    
    //settings
    bool msaaEnabled;
    int msaaSamples;
    bool depthEnabled;
    bool retinaEnabled;
    bool bResize;
}

- (instancetype)initWithDepth:(bool)depth
                        andAA:(bool)msaa
               andMSAASamples:(int)samples
                    andRetina:(bool)retina
                   sharegroup:(EAGLSharegroup*)sharegroup;
- (void)startRender;
- (void)finishRender;
- (void)destroyFramebuffer;
- (BOOL)createFramebuffer:(CAEAGLLayer *)layer;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
- (EAGLContext*) context;

- (NSInteger)getWidth;
- (NSInteger)getHeight;

@end
