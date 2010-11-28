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

@end
