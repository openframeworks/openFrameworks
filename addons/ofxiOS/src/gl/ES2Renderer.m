

#import "ES2Renderer.h"

@implementation ES2Renderer

// Create an OpenGL ES 2.0 context
- (instancetype)init {
    return [self initWithDepth:false andAA:false andMSAASamples:0 andRetina:false sharegroup:nil];
}

- (instancetype)initWithDepth:(bool)depth andAA:(bool)msaa andMSAASamples:(int)samples andRetina:(bool)retina sharegroup:(EAGLSharegroup*)sharegroup{
    if((self = [super init])) {
        depthEnabled = depth;
        msaaEnabled = msaa;
        msaaSamples = samples;
        retinaEnabled = retina;
        bResize = false;
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:sharegroup];
        NSLog(@"Creating OpenGL ES2 Renderer");

        if(!context || ![EAGLContext setCurrentContext:context]) {
            NSLog(@"OpenGL ES2 failed");
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
            GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
            glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
        } else {
            GLenum attachments[] = {GL_COLOR_ATTACHMENT0};
            glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
        }
        
        glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, msaaFrameBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, defaultFramebuffer);
        glResolveMultisampleFramebufferAPPLE();
    }
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    if(bResize) {
        /**
         *  when resizing glView there is a visual glitch
         *  which shows the screen distorted for a split second.
         *  the below fixes it.
         */
        glClear(GL_COLOR_BUFFER_BIT);
        bResize = false;
    }
    [context presentRenderbuffer:GL_RENDERBUFFER];
    
    if(msaaEnabled) {
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
    }
}


- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer {
    [self destroyFramebuffer];
    BOOL bOk = [self createFramebuffer:layer];
    bResize = true;
    return bOk;
}

- (BOOL)createFramebuffer:(CAEAGLLayer *)layer {
    glGenFramebuffers(1, &defaultFramebuffer);
    glGenRenderbuffers(1, &colorRenderbuffer);
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    if(msaaEnabled) {
        glGenFramebuffers(1, &msaaFrameBuffer);
        glGenRenderbuffers(1, &msaaColorRenderBuffer);
    }
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    if(msaaEnabled) {
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, msaaColorRenderBuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, msaaSamples, GL_RGB5_A1, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaColorRenderBuffer);
    }
    
    if(depthEnabled) {
        if(!depthRenderbuffer) {
            glGenRenderbuffers(1, &depthRenderbuffer);
        }
        
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        
        if(msaaEnabled) {
            glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, msaaSamples, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        } else {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight); // GL DEPTH COMPONENT ON THIS LINE ISNT CORRECT POTENTIALLY
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        }
    }
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"Failed to make complete framebuffer object %x %ix%i", glCheckFramebufferStatus(GL_FRAMEBUFFER), backingWidth, backingHeight);
        return NO;
    }
    
    return YES;
}

- (void)destroyFramebuffer {
    if(defaultFramebuffer) {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
    
    if(colorRenderbuffer) {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    
    if(msaaFrameBuffer) {
        glDeleteRenderbuffers(1, &msaaFrameBuffer);
        msaaFrameBuffer = 0;
    }
    
    if(msaaColorRenderBuffer) {
        glDeleteRenderbuffers(1, &msaaColorRenderBuffer);
        msaaColorRenderBuffer = 0;
    }
}

- (void)dealloc {
    [self destroyFramebuffer];

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
