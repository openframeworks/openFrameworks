

#import "ESRenderer.h"
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@interface ES2Renderer : NSObject <ESRenderer>
{
@private
    EAGLContext *context;

    // The pixel dimensions of the CAEAGLLayer
    GLint backingWidth;
    GLint backingHeight;

    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view
    GLuint defaultFramebuffer, colorRenderbuffer;

    GLuint program;
	
	//settings
	bool fsaaEnabled;
	int fsaaSamples;
	bool depthEnabled;
	bool retinaEnabled;
}

- (id)initWithDepth:(bool)depth andAA:(bool)fsaa andFSAASamples:(int)samples andRetina:(bool)retina;
- (void) startRender;
- (void) finishRender;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
- (EAGLContext*) context;

@end

