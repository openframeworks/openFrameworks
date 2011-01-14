

#import "ES1Renderer.h"

@implementation ES1Renderer

// Create an OpenGL ES 1.1 context
- (id)init
{
	return [self initWithDepth:false andAA:false  andFSAASamples:0 andRetina:false];
}

- (id)initWithDepth:(bool)depth andAA:(bool)fsaa andFSAASamples:(int)samples andRetina:(bool)retina
{
    if ((self = [super init]))
    {
		
		depthEnabled = depth;
		fsaaEnabled = false;
		fsaaSamples = samples;
		retinaEnabled = retina;
				
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		NSLog(@"Creating OpenGL ES1 Renderer");

        if (!context || ![EAGLContext setCurrentContext:context])
        {
			NSLog(@"OpenGL ES1 failed");
            [self release];
            return nil;
        }

        // Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
        glGenFramebuffersOES(1, &defaultFramebuffer);
        glGenRenderbuffersOES(1, &colorRenderbuffer);
		
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);

        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
		
		const GLubyte * extensions = glGetString(GL_EXTENSIONS);
		if(extensions != NULL && fsaa)
			if(strstr((const char*)extensions, "GL_APPLE_framebuffer_multisample"))
				fsaaEnabled=true;
		
		if(fsaaEnabled)
		{
			glGenFramebuffersOES(1, &fsaaFrameBuffer);
			glGenRenderbuffersOES(1, &fsaaColorRenderBuffer);
		}
		
	}

    return self;
}

-(EAGLContext*) context
{
	return context;
}

- (void)startRender
{
//    [EAGLContext setCurrentContext:context];
	
    // This application only creates a single default framebuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple framebuffers.
//    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
}
	

- (void)finishRender
{
    // This application only creates a single color renderbuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple renderbuffers.
//    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	
	if(fsaaEnabled)
	{
		glBindFramebufferOES(GL_READ_FRAMEBUFFER_APPLE, fsaaFrameBuffer);
		glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, defaultFramebuffer);
		glResolveMultisampleFramebufferAPPLE();
		
		if(depthEnabled)
		{
			GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES, GL_DEPTH_ATTACHMENT_OES};
			glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
		}
		else
		{
			GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES};
			glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
		}
	}
	
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
	
	if(fsaaEnabled)
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, fsaaFrameBuffer);
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer
{	
	layer.opaque = YES;
    // Allocate color buffer backing based on the current layer size
    //[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
	
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);

	if(fsaaEnabled)
	{
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, fsaaFrameBuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, fsaaColorRenderBuffer);
		glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, fsaaSamples, GL_RGB5_A1_OES, backingWidth, backingHeight);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, fsaaColorRenderBuffer);
	}
	
	if(depthEnabled)
	{
		if(!depthRenderbuffer)
			glGenRenderbuffersOES(1, &depthRenderbuffer);
		
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
		
		if(fsaaEnabled)
		{
			glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, fsaaSamples, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
			glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
		}
		else
		{
			glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight); // GL DEPTH COMPONENT ON THIS LINE ISNT CORRECT POTENTIALLY
			glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
		}
				
	}
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	
    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
    {
        NSLog(@"Failed to make complete framebuffer object %x %ix%i", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES), backingWidth, backingHeight);
        return NO;
    }

    return YES;
}

- (void)dealloc
{
    // Tear down GL
    if (defaultFramebuffer)
    {
        glDeleteFramebuffersOES(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }

    if (colorRenderbuffer)
    {
        glDeleteRenderbuffersOES(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }

	if(depthRenderbuffer) {
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
	
	if(fsaaFrameBuffer) {
		glDeleteRenderbuffersOES(1, &fsaaFrameBuffer);
		fsaaFrameBuffer = 0;
	}
	
	if(fsaaColorRenderBuffer) {
		glDeleteRenderbuffersOES(1, &fsaaColorRenderBuffer);
		fsaaColorRenderBuffer = 0;
	}
	
    // Tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];

    [context release];
    context = nil;

    [super dealloc];
}

@end
