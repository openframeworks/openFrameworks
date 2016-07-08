//
//  ES3Renderer.h
//  iOS+OFLib
//
//  Created by Daniel Rosser on 5/03/2016.
//
#import "ESRenderer.h"
#import "ES2Renderer.h"

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>


@interface ES3Renderer : NSObject <ESRenderer>
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
	bool bResize;
}

- (id)initWithDepth:(bool)depth andAA:(bool)fsaa andFSAASamples:(int)samples andRetina:(bool)retina;
- (void)startRender;
- (void)finishRender;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
- (BOOL)createFramebuffer:(CAEAGLLayer *)layer;
- (void)destroyFramebuffer;
- (EAGLContext*) context;

- (NSInteger)getWidth;
- (NSInteger)getHeight;

@end

