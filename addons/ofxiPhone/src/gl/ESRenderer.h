//
//  ESRenderer.h
//  iPhone OpenGL
//
//  Created by Mehmet Akten on 05/06/2010.
//  Copyright MSA Visuals Ltd. 2010. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

@protocol ESRenderer <NSObject>

- (void) startRender;
- (void) finishRender;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
- (EAGLContext*) context;

- (NSInteger)getWidth;
- (NSInteger)getHeight;

- (void) readPixels:(int)width:(int)height:(void *)buffer;
- (void) copyTexSubImage2D:(unsigned int)textureTarget:(int)level
                          :(int)xoffset:(int)yoffset:(int)x:(int)y:(size_t)width:(size_t)height;

@end
