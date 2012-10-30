

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
		
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		NSLog(@"Creating OpenGL ES2 Renderer");

        if(!context || ![EAGLContext setCurrentContext:context]) {
			NSLog(@"OpenGL ES2 failed");
            [self release];
            return nil;
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
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer {
    [self destroyFramebuffer];
    [self createFramebuffer:layer];
}

- (BOOL)createFramebuffer:(CAEAGLLayer *)layer {
    glGenFramebuffers(1, &defaultFramebuffer);
    glGenRenderbuffers(1, &colorRenderbuffer);
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
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
