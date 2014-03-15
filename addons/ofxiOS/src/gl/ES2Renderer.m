

#import "ES2Renderer.h"

@implementation ES2Renderer

// Create an OpenGL ES 2.0 context
- (id)init {
	return [self initWithDepth:false andAA:false andFSAASamples:0 andRetina:false];
}

- (id)initWithDepth:(bool)depth andAA:(bool)fsaa andFSAASamples:(int)samples andRetina:(bool)retina {
    if((self = [super init])) {
		depthEnabled = depth;
		fsaaEnabled = fsaa;
		fsaaSamples = samples;
		retinaEnabled = retina;
        bResize = false;
		
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		NSLog(@"Creating OpenGL ES2 Renderer");

        if(!context || ![EAGLContext setCurrentContext:context]) {
			NSLog(@"OpenGL ES2 failed");
            [self release];
            return nil;
        }
        
        const GLubyte * extensions = glGetString(GL_EXTENSIONS);
        if(extensions != NULL && fsaaEnabled) {
            if(strstr((const char*)extensions, "GL_APPLE_framebuffer_multisample")) {
                fsaaEnabled = true;
            } else {
                fsaaEnabled = false;
            }
        } else {
            fsaaEnabled = false;
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
	if(fsaaEnabled) {
		if(depthEnabled) {
			GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
			glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
		} else {
			GLenum attachments[] = {GL_COLOR_ATTACHMENT0};
			glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
		}
        
		glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, fsaaFrameBuffer);
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
	
	if(fsaaEnabled) {
		glBindFramebuffer(GL_FRAMEBUFFER, fsaaFrameBuffer);
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
    
    if(fsaaEnabled) {
        glGenFramebuffers(1, &fsaaFrameBuffer);
        glGenRenderbuffers(1, &fsaaColorRenderBuffer);
    }
	
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
	
	if(fsaaEnabled) {
		glBindFramebuffer(GL_FRAMEBUFFER, fsaaFrameBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, fsaaColorRenderBuffer);
		glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, fsaaSamples, GL_RGB5_A1, backingWidth, backingHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, fsaaColorRenderBuffer);
	}
	
	if(depthEnabled) {
		if(!depthRenderbuffer) {
			glGenRenderbuffers(1, &depthRenderbuffer);
        }
		
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
		
		if(fsaaEnabled) {
			glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, fsaaSamples, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
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
	
	if(fsaaFrameBuffer) {
		glDeleteRenderbuffers(1, &fsaaFrameBuffer);
		fsaaFrameBuffer = 0;
	}
	
	if(fsaaColorRenderBuffer) {
		glDeleteRenderbuffers(1, &fsaaColorRenderBuffer);
		fsaaColorRenderBuffer = 0;
	}
}

- (void)dealloc {
    [self destroyFramebuffer];

    if([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }

    [context release];
    context = nil;

    [super dealloc];
}

- (NSInteger)getWidth {
    return backingWidth;
} 

- (NSInteger)getHeight {
    return backingHeight;
}

@end
