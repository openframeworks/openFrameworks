//
//  ES3Renderer.h
//  iOS+OFLib
//
//  Created by Daniel Rosser on 5/03/2016.
//
#pragma once
#include "ofxiOSConstants.h"
#if defined(OF_UI_KIT) && defined(OF_GL_KIT)
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
	bool msaaEnabled;
	int msaaSamples;
	bool depthEnabled;
	bool retinaEnabled;
	bool bResize;
}

- (id)initWithDepth:(bool)depth andAA:(bool)fsaa andMSAASamples:(int)samples andRetina:(bool)retina sharegroup:(EAGLSharegroup*)sharegroup;
- (void)startRender;
- (void)finishRender;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
- (BOOL)createFramebuffer:(CAEAGLLayer *)layer;
- (void)destroyFramebuffer;
- (EAGLContext*) context;

- (NSInteger)getWidth;
- (NSInteger)getHeight;

@end
#endif
