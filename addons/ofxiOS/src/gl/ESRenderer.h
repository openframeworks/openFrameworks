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

typedef enum {
    ESRendererVersion_11 = 1,
    ESRendererVersion_20 = 2,
    ESRendererVersion_30 = 3
} ESRendererVersion;

@protocol ESRenderer <NSObject>

- (void) startRender;
- (void) finishRender;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
- (EAGLContext*) context;

- (NSInteger)getWidth;
- (NSInteger)getHeight;

@end
