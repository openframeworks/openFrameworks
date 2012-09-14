

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
	GLuint fsaaFrameBuffer, fsaaColorRenderBuffer;
	
	//settings
	bool fsaaEnabled;
	int fsaaSamples;
	bool depthEnabled;
	bool retinaEnabled;
}

- (void) startRender;
- (void) finishRender;
- (void)destroyFramebuffer;
- (BOOL)createFramebuffer:(CAEAGLLayer *)layer;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
- (id)initWithDepth:(bool)depth andAA:(bool)fsaa andFSAASamples:(int)samples andRetina:(bool)retina;
- (EAGLContext*) context;

- (NSInteger)getWidth;
- (NSInteger)getHeight;

@end
