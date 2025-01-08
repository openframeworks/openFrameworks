//
// Copyright 2019 Le Hoang Quyen. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#ifndef MGLContext_Private_h
#define MGLContext_Private_h

#import "MGLContext.h"
#import "MGLDisplay.h"

#include <EGL/egl.h>

@interface MGLContext () {
    MGLRenderingAPI _renderingApi;
}

@property(nonatomic, readonly) MGLDisplay *display;
@property(nonatomic, readonly) EGLContext eglContext;

@end

#endif /* MGLContext_Private_h */
