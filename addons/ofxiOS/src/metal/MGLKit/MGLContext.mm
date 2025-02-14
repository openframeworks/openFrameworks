//
//  MGLContext.m
//  OpenGLES
//
//  Created by Le Quyen on 16/10/19.
//  Copyright © 2019 Google. All rights reserved.
//

#import "MGLContext.h"
#import "MGLContext+Private.h"

#if defined(OF_METAL)

#include <pthread.h>
#include <vector>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglext_angle.h>
#include <EGL/eglplatform.h>
#include <src/common/debug.h>
#import "MGLLayer+Private.h"

namespace
{
struct ThreadLocalInfo
{
    __weak MGLContext *currentContext = nil;
    __weak MGLLayer *currentLayer     = nil;
};

#if TARGET_OS_SIMULATOR
pthread_key_t gThreadSpecificKey;
void ThreadTLSDestructor(void *data)
{
    auto tlsData = reinterpret_cast<ThreadLocalInfo *>(data);
    delete tlsData;
}

#endif

ThreadLocalInfo &CurrentTLS()
{
#if TARGET_OS_SIMULATOR
    // There are some issuess with C++11 TLS could not be compiled on iOS
    // simulator, so we have to fallback to use pthread TLS.
    static pthread_once_t sKeyOnce = PTHREAD_ONCE_INIT;
    pthread_once(&sKeyOnce, [] { pthread_key_create(&gThreadSpecificKey, ThreadTLSDestructor); });

    auto tlsData = reinterpret_cast<ThreadLocalInfo *>(pthread_getspecific(gThreadSpecificKey));
    if (!tlsData)
    {
        tlsData = new ThreadLocalInfo();
        pthread_setspecific(gThreadSpecificKey, tlsData);
    }
    return *tlsData;
#else  // TARGET_OS_SIMULATOR
    static thread_local ThreadLocalInfo tls;
    return tls;
#endif
}

void Throw(NSString *msg)
{
    [NSException raise:@"MGLSurfaceException" format:@"%@", msg];
}

EGLContext CreateEGLContext(EGLDisplay display, MGLRenderingAPI api, EGLContext sharedContext)
{
    // Init config
    std::vector<EGLint> surfaceAttribs = {
        EGL_RED_SIZE,       EGL_DONT_CARE, EGL_GREEN_SIZE,   EGL_DONT_CARE,
        EGL_BLUE_SIZE,      EGL_DONT_CARE, EGL_ALPHA_SIZE,   EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     EGL_DONT_CARE, EGL_STENCIL_SIZE, EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, EGL_DONT_CARE, EGL_SAMPLES,      EGL_DONT_CARE,
    };
    surfaceAttribs.push_back(EGL_NONE);
    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(display, surfaceAttribs.data(), &config, 1, &numConfigs) || numConfigs < 1)
    {
        Throw(@"Failed to call eglChooseConfig()");
    }

    // Init context
    int ctxMajorVersion = 2;
    int ctxMinorVersion = 0;
    switch (api)
    {
        case kMGLRenderingAPIOpenGLES1:
            ctxMajorVersion = 1;
            ctxMinorVersion = 0;
            break;
        case kMGLRenderingAPIOpenGLES2:
            ctxMajorVersion = 2;
            ctxMinorVersion = 0;
            break;
        case kMGLRenderingAPIOpenGLES3:
            ctxMajorVersion = 3;
            ctxMinorVersion = 0;
            break;
        default:
            UNREACHABLE();
    }
    EGLint ctxAttribs[] = {EGL_CONTEXT_MAJOR_VERSION, ctxMajorVersion, EGL_CONTEXT_MINOR_VERSION,
                           ctxMinorVersion, EGL_NONE};

    EGLContext eglContext = eglCreateContext(display, config, sharedContext, ctxAttribs);
    if (eglContext == EGL_NO_CONTEXT)
    {
        Throw(@"Failed to call eglCreateContext()");
    }

    return eglContext;
}
}

// MGLSharegroup implementation
@interface MGLSharegroup () {
    __weak MGLContext *_firstContext;
    EGLDisplay _sharedEGLDisplay;
    EGLContext _sharedEGLContext;
}

// This shared context is only created when the shared group has more
// than one context.
- (EGLContext)sharedEGLContext;
- (void)addContext:(MGLContext *)context;

@end

@implementation MGLSharegroup

- (id)init
{
    if (self = [super init])
    {
        _sharedEGLContext = EGL_NO_CONTEXT;
    }
    return self;
}

- (void)dealloc
{
    if (_sharedEGLContext != EGL_NO_CONTEXT)
    {
        eglDestroyContext(_sharedEGLDisplay, _sharedEGLContext);
        _sharedEGLContext = EGL_NO_CONTEXT;
    }
}

- (EGLContext)sharedEGLContext
{
    @synchronized(self)
    {
        return _sharedEGLContext;
    }
}

- (void)addContext:(MGLContext *)context
{
    @synchronized(self)
    {
        if (!_firstContext)
        {
            _firstContext = context;
        }
        else if (_sharedEGLContext == EGL_NO_CONTEXT)
        {
            // If we have more than one context in the shared group, create
            // a "master" context to be shared by all contexts in the group.
            _sharedEGLDisplay = _firstContext.display.eglDisplay;
            _sharedEGLContext =
                CreateEGLContext(_sharedEGLDisplay, _firstContext.API, _firstContext.eglContext);
        }
    }
}

@end

// MGLContext implementation

@implementation MGLContext

- (id)initWithAPI:(MGLRenderingAPI)api
{
    return [self initWithAPI:api sharegroup:nil];
}

- (id)initWithAPI:(MGLRenderingAPI)api sharegroup:(MGLSharegroup *)sharegroup
{
    if (self = [super init])
    {
        _renderingApi = api;
        _display      = [MGLDisplay defaultDisplay];
        if (sharegroup)
        {
            _sharegroup = sharegroup;
        }
        else
        {
            _sharegroup = [MGLSharegroup new];
        }

        [_sharegroup addContext:self];

        [self initContext];
    }
    return self;
}

- (MGLRenderingAPI)API
{
    return _renderingApi;
}

- (EGLDisplay)eglDisplay
{
    return _display.eglDisplay;
}

- (void)dealloc
{
    [self releaseContext];

    _display = nil;
}

- (void)releaseContext
{
    if (eglGetCurrentContext() == _eglContext)
    {
        eglMakeCurrent(_display.eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }

    if (_eglContext != EGL_NO_CONTEXT)
    {
        eglDestroyContext(_display.eglDisplay, _eglContext);
        _eglContext = EGL_NO_CONTEXT;
    }
}

- (void)initContext
{
    _eglContext =
        CreateEGLContext(_display.eglDisplay, _renderingApi, _sharegroup.sharedEGLContext);
}

- (BOOL)present:(MGLLayer *)layer
{
    return [layer present];
}

+ (MGLContext *)currentContext
{
    return CurrentTLS().currentContext;
}

+ (MGLLayer *)currentLayer
{
    return CurrentTLS().currentLayer;
}

+ (BOOL)setCurrentContext:(MGLContext *)context
{
    ThreadLocalInfo &tlsData = CurrentTLS();
    if (context)
    {
        return [context setCurrentContextForLayer:tlsData.currentLayer];
    }

    // No context
    tlsData.currentContext = nil;
    tlsData.currentLayer   = nil;

    return eglMakeCurrent([MGLDisplay defaultDisplay].eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
                          EGL_NO_CONTEXT);
}

+ (BOOL)setCurrentContext:(MGLContext *)context forLayer:(MGLLayer *)layer
{
    if (context)
    {
        return [context setCurrentContextForLayer:layer];
    }
    return [self setCurrentContext:nil];
}

- (BOOL)setCurrentContextForLayer:(MGLLayer *_Nullable)layer
{
    if (!layer)
    {
        if (eglGetCurrentContext() != _eglContext ||
            eglGetCurrentSurface(EGL_READ) != EGL_NO_SURFACE ||
            eglGetCurrentSurface(EGL_DRAW) != EGL_NO_SURFACE)
        {
            if (!eglMakeCurrent(_display.eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, _eglContext))
            {
                return NO;
            }
        }
    }
    else
    {
        if (![layer setCurrentContext:self])
        {
            return NO;
        }
    }

    ThreadLocalInfo &tlsData = CurrentTLS();
    tlsData.currentContext   = self;
    tlsData.currentLayer     = layer;

    return YES;
}

@end

#endif
