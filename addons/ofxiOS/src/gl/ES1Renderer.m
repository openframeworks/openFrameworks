

#import "ES1Renderer.h"

@implementation ES1Renderer

// Create an OpenGL ES 1.1 context
- (instancetype)init {
    return [self initWithDepth:false andAA:false  andMSAASamples:0 andRetina:false sharegroup:nil];
}

- (instancetype)initWithDepth:(bool)depth andAA:(bool)msaa andMSAASamples:(int)samples andRetina:(bool)retina sharegroup:(EAGLSharegroup*)sharegroup{

    if((self = [super init])) {
        
        depthEnabled = depth;
        msaaEnabled = msaa;
        msaaSamples = samples;
        retinaEnabled = retina;
        bResize = false;
                
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        NSLog(@"Creating OpenGL ES1 Renderer");
        
        if(!context || ![EAGLContext setCurrentContext:context]) {
            NSLog(@"OpenGL ES1 failed");
            self = nil;
            return nil;
        }
        
        const GLubyte * extensions = glGetString(GL_EXTENSIONS);
        if(extensions != NULL && msaaEnabled) {
            if(strstr((const char*)extensions, "GL_APPLE_framebuffer_multisample")) {
                msaaEnabled = true;
            } else {
                msaaEnabled = false;
            }
        } else {
            msaaEnabled = false;
        }
    }

    return self;
}

-(EAGLContext*) context {
    return context;
}

- (void)startRender {
    [EAGLContext setCurrentContext:context];
}

- (void)finishRender {
    
    if(msaaEnabled) {
        if(depthEnabled) {
            GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES, GL_DEPTH_ATTACHMENT_OES};
            glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
        } else {
            GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES};
            glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
        }
        
        glBindFramebufferOES(GL_READ_FRAMEBUFFER_APPLE, msaaFrameBuffer);
        glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, defaultFramebuffer);
        glResolveMultisampleFramebufferAPPLE();
    }
    
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    if(bResize) {
        /**
         *  when resizing glView there is a visual glitch
         *  which shows the screen distorted for a split second.
         *  the below fixes it.
         */
        glClear(GL_COLOR_BUFFER_BIT);
        bResize = false;
    }
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
    
    if(msaaEnabled) {
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, msaaFrameBuffer);
    }
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer {  
    [self destroyFramebuffer];
    BOOL bOk = [self createFramebuffer:layer];
    bResize = true;
    return bOk;
}

- (BOOL)createFramebuffer:(CAEAGLLayer *)layer {
    glGenFramebuffersOES(1, &defaultFramebuffer);
    glGenRenderbuffersOES(1, &colorRenderbuffer);
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
    
    if(msaaEnabled) {
        glGenFramebuffersOES(1, &msaaFrameBuffer);
        glGenRenderbuffersOES(1, &msaaColorRenderBuffer);
    }
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    if(msaaEnabled) {
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, msaaFrameBuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, msaaColorRenderBuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, msaaSamples, GL_RGB5_A1_OES, backingWidth, backingHeight);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, msaaColorRenderBuffer);
    }
    
    if(depthEnabled) {
        if(!depthRenderbuffer) {
            glGenRenderbuffersOES(1, &depthRenderbuffer);
        }
        
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
        
        if(msaaEnabled) {
            glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, msaaSamples, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
            glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
        } else {
            glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight); // GL DEPTH COMPONENT ON THIS LINE ISNT CORRECT POTENTIALLY
            glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
        }
    }
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }
    
    return YES;
}


- (void)destroyFramebuffer {
    if(defaultFramebuffer) {
        glDeleteFramebuffersOES(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
    
    if(colorRenderbuffer) {
        glDeleteRenderbuffersOES(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    
    if(msaaFrameBuffer) {
        glDeleteRenderbuffersOES(1, &msaaFrameBuffer);
        msaaFrameBuffer = 0;
    }
    
    if(msaaColorRenderBuffer) {
        glDeleteRenderbuffersOES(1, &msaaColorRenderBuffer);
        msaaColorRenderBuffer = 0;
    }
}

- (void)dealloc {
    [self destroyFramebuffer];
    
    // Tear down context
    if([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }

    context = nil;
}

- (NSInteger)getWidth {
    return backingWidth;
} 

- (NSInteger)getHeight {
    return backingHeight;
}

@end
