/*
     File:       QD3DRenderer.h
 
     Contains:   Q3Renderer types and routines
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DRENDERER__
#define __QD3DRENDERER__

#ifndef __QD3D__
#include <QD3D.h>
#endif

#ifndef __QD3DSET__
#include <QD3DSet.h>
#endif

#ifndef __QD3DVIEW__
#include <QD3DView.h>
#endif

#ifndef __RAVE__
#include <RAVE.h>
#endif


#if TARGET_OS_MAC
#ifndef __EVENTS__
#include <Events.h>
#endif

#endif  /* TARGET_OS_MAC */



#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_IMPORT
#pragma import on
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

#if PRAGMA_ENUM_ALWAYSINT
    #if defined(__fourbyteints__) && !__fourbyteints__ 
        #define __QD3DRENDERER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DRENDERER__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                         User Interface Things                            **
 **                                                                          **
 *****************************************************************************/
#if TARGET_OS_MAC
/*
 *  A callback to an application's event handling code. This is needed to    
 *  support movable modal dialogs. The dialog's event filter calls this      
 *  callback with events it does not handle.                                 
 *  If an application handles the event it should return kQ3True.            
 *  If the application does not handle the event it must return kQ3False and 
 *  the dialog's event filter will pass the event to the system unhandled.   
 */
typedef CALLBACK_API_C( TQ3Boolean , TQ3MacOSDialogEventHandler )(const EventRecord *event);

struct TQ3DialogAnchor {
    TQ3MacOSDialogEventHandler      clientEventHandler;
};
typedef struct TQ3DialogAnchor          TQ3DialogAnchor;
#endif  /* TARGET_OS_MAC */

#if TARGET_OS_WIN32

struct TQ3DialogAnchor {
    HWND                            ownerWindow;
};
typedef struct TQ3DialogAnchor          TQ3DialogAnchor;
#endif  /* TARGET_OS_WIN32 */

#if TARGET_OS_UNIX

struct TQ3DialogAnchor {
    void *                          notUsed;                    /* place holder */
};
typedef struct TQ3DialogAnchor          TQ3DialogAnchor;
#endif  /* TARGET_OS_UNIX */

/******************************************************************************
 **                                                                          **
 **                         Renderer Functions                               **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3RendererObject )
Q3Renderer_NewFromType          (TQ3ObjectType          rendererObjectType);

EXTERN_API_C( TQ3ObjectType )
Q3Renderer_GetType              (TQ3RendererObject      renderer);


/* Q3Renderer_Flush has been replaced by Q3View_Flush */
/* Q3Renderer_Sync has been replaced by Q3View_Sync */
#endif  /* CALL_NOT_IN_CARBON */


/*
 *  Q3Renderer_IsInteractive
 *      Determine if this renderer is intended to be used interactively.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Boolean )
Q3Renderer_IsInteractive        (TQ3RendererObject      renderer);


/*
 *  Q3Renderer_HasModalConfigure
 *      Determine if this renderer has a modal settings dialog.
 *
 *  Q3Renderer_ModalConfigure
 *      Have the renderer pop up a modal dialog box to configure its settings.
 *  dialogAnchor - is platform specific data passed by the client to support
 *      movable modal dialogs. 
 *    MacOS: this is a callback to the calling application's event handler.
 *      The renderer calls this function with events not handled by the 
 *      settings dialog. This is necessary in order to support movable modal 
 *      dialogs. An application's event handler must return kQ3True if it 
 *      handles the event passed to the callback or kQ3False if not. 
 *      An application which doesn't want to support a movable modal configure
 *      dialog should pass NULL for the clientEventHandler of TQ3DialogAnchor.
 *    Win32: this is the HWND of the owning window (typically an application's
 *      main window).
 *  canceled - returns a boolean inditacating that the user canceled the 
 *  dialog.
 *      
 */
EXTERN_API_C( TQ3Boolean )
Q3Renderer_HasModalConfigure    (TQ3RendererObject      renderer);

EXTERN_API_C( TQ3Status )
Q3Renderer_ModalConfigure       (TQ3RendererObject      renderer,
                                 TQ3DialogAnchor        dialogAnchor,
                                 TQ3Boolean *           canceled);

/*
 *  Q3RendererClass_GetNickNameString
 *      Allows an application to get a renderers name string, the 
 *      renderer is responsible for storing this in a localizable format
 *      for example as a resource.  This string can then be used to provide
 *      a selection mechanism for an application (for example in a menu).
 *
 *      If this call returns nil in the supplied string, then the App may 
 *      choose to use the class name for the renderer.  You should always 
 *      try to get the name string before using the class name, since the
 *      class name is not localizable.
 */
EXTERN_API_C( TQ3Status )
Q3RendererClass_GetNickNameString (TQ3ObjectType        rendererClassType,
                                 TQ3ObjectClassNameString  rendererClassString);


/*
 *  Q3Renderer_GetConfigurationData
 *      Allows an application to collect private renderer configuration data
 *      which it will then save. For example in a preference file or in a 
 *      style template. An application should tag this data with the 
 *      Renderer's object  name.
 *  
 *      if dataBuffer is NULL actualDataSize returns the required size in 
 *      bytes of a data buffer large enough to store private data. 
 *
 *      bufferSize is the actual size of the memory block pointed to by 
 *      dataBuffer
 *
 *      actualDataSize - on return the actual number of bytes written to the 
 *      buffer or if dataBuffer is NULL the required size of dataBuffer
 * 
 */
EXTERN_API_C( TQ3Status )
Q3Renderer_GetConfigurationData (TQ3RendererObject      renderer,
                                 unsigned char *        dataBuffer,
                                 unsigned long          bufferSize,
                                 unsigned long *        actualDataSize);

EXTERN_API_C( TQ3Status )
Q3Renderer_SetConfigurationData (TQ3RendererObject      renderer,
                                 unsigned char *        dataBuffer,
                                 unsigned long          bufferSize);



/******************************************************************************
 **                                                                          **
 **                     Interactive Renderer Specific Functions              **
 **                                                                          **
 *****************************************************************************/
/* CSG IDs attribute */
#define kQ3AttributeTypeConstructiveSolidGeometryID     Q3_OBJECT_TYPE('c','s','g','i')
/* Object IDs, to be applied as attributes on geometries */
#endif  /* CALL_NOT_IN_CARBON */

#define kQ3SolidGeometryObjNone (-1)
#define kQ3SolidGeometryObjA    0
#define kQ3SolidGeometryObjB    1
#define kQ3SolidGeometryObjC    2
#define kQ3SolidGeometryObjD    3
#define kQ3SolidGeometryObjE    4
/* Possible CSG equations */

enum TQ3CSGEquation {
    kQ3CSGEquationAandB         = (long)0x88888888,
    kQ3CSGEquationAandnotB      = 0x22222222,
    kQ3CSGEquationAanBonCad     = 0x2F222F22,
    kQ3CSGEquationnotAandB      = 0x44444444,
    kQ3CSGEquationnAaBorCanB    = 0x74747474
};
typedef enum TQ3CSGEquation TQ3CSGEquation;

#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_SetCSGEquation (TQ3RendererObject  renderer,
                                 TQ3CSGEquation         equation);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_GetCSGEquation (TQ3RendererObject  renderer,
                                 TQ3CSGEquation *       equation);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_SetPreferences (TQ3RendererObject  renderer,
                                 long                   vendorID,
                                 long                   engineID);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_GetPreferences (TQ3RendererObject  renderer,
                                 long *                 vendorID,
                                 long *                 engineID);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_SetDoubleBufferBypass (TQ3RendererObject  renderer,
                                 TQ3Boolean             bypass);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_GetDoubleBufferBypass (TQ3RendererObject  renderer,
                                 TQ3Boolean *           bypass);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_SetRAVEContextHints (TQ3RendererObject  renderer,
                                 unsigned long          RAVEContextHints);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_GetRAVEContextHints (TQ3RendererObject  renderer,
                                 unsigned long *        RAVEContextHints);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_SetRAVETextureFilter (TQ3RendererObject  renderer,
                                 unsigned long          RAVEtextureFilterValue);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_GetRAVETextureFilter (TQ3RendererObject  renderer,
                                 unsigned long *        RAVEtextureFilterValue);

EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_CountRAVEDrawContexts (TQ3RendererObject  renderer,
                                 unsigned long *        numRAVEContexts);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( void , TQ3RaveDestroyCallback )(TQ3RendererObject renderer);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3InteractiveRenderer_GetRAVEDrawContexts (TQ3RendererObject  renderer,
                                 TQADrawContext **      raveDrawContextList,
                                 TQAEngine **           raveDrawingEnginesList,
                                 unsigned long *        numRAVEContexts,
                                 TQ3RaveDestroyCallback  raveDestroyCallback);




/******************************************************************************
 **                                                                          **
 **                         Renderer View Tools                              **
 **                                                                          **
 **                 You may only call these methods from a plug-in           **
 **                                                                          **
 *****************************************************************************/
/*
 *  Call by a renderer to call the user "idle" method, with progress 
 *  information.
 *  
 *  Pass in (view, 0, n) on first call
 *  Pass in (view, 1..n-1, n) during rendering
 *  Pass in (view, n, n) upon completion
 *  
 *  Note: The user must have supplied an idleProgress method with 
 *  Q3XView_SetIdleProgressMethod. Otherwise, the generic idle method is
 *  called with no progress data. e.g. the Q3View_SetIdleMethod method
 *  is called instead. (current and final are ignored, essentially.)
 *
 *  Returns kQ3Failure if rendering is cancelled.
 */
EXTERN_API_C( TQ3Status )
Q3XView_IdleProgress            (TQ3ViewObject          view,
                                 unsigned long          current,
                                 unsigned long          completed);

/*
 *  Called by an asynchronous renderer when it completes a frame.
 */
EXTERN_API_C( TQ3Status )
Q3XView_EndFrame                (TQ3ViewObject          view);


/******************************************************************************
 **                                                                          **
 **                         Renderer AttributeSet Tools                      **
 **                                                                          **
 **                 You may only call these methods from a plug-in           **
 **                                                                          **
 *****************************************************************************/
/*
 *  Faster access to geometry attribute sets.
 *  
 *  Returns pointer to INTERNAL data structure for elements and attributes
 *  in an attributeSet, or NULL if no attribute exists.
 *  
 *  For attributes of type kQ3AttributeType..., the internal data structure
 *  is identical to the data structure used in Q3AttributeSet_Add.
 */
EXTERN_API_C( void *)
Q3XAttributeSet_GetPointer      (TQ3AttributeSet        attributeSet,
                                 TQ3AttributeType       attributeType);


#endif  /* CALL_NOT_IN_CARBON */

enum {
    kQ3XAttributeMaskNone       = 0L,
    kQ3XAttributeMaskSurfaceUV  = 1 << (kQ3AttributeTypeSurfaceUV - 1),
    kQ3XAttributeMaskShadingUV  = 1 << (kQ3AttributeTypeShadingUV - 1),
    kQ3XAttributeMaskNormal     = 1 << (kQ3AttributeTypeNormal - 1),
    kQ3XAttributeMaskAmbientCoefficient = 1 << (kQ3AttributeTypeAmbientCoefficient - 1),
    kQ3XAttributeMaskDiffuseColor = 1 << (kQ3AttributeTypeDiffuseColor - 1),
    kQ3XAttributeMaskSpecularColor = 1 << (kQ3AttributeTypeSpecularColor - 1),
    kQ3XAttributeMaskSpecularControl = 1 << (kQ3AttributeTypeSpecularControl - 1),
    kQ3XAttributeMaskTransparencyColor = 1 << (kQ3AttributeTypeTransparencyColor - 1),
    kQ3XAttributeMaskSurfaceTangent = 1 << (kQ3AttributeTypeSurfaceTangent - 1),
    kQ3XAttributeMaskHighlightState = 1 << (kQ3AttributeTypeHighlightState - 1),
    kQ3XAttributeMaskSurfaceShader = 1 << (kQ3AttributeTypeSurfaceShader - 1),
    kQ3XAttributeMaskCustomAttribute = (long)0x80000000,
    kQ3XAttributeMaskAll        = 0x800007FF,
    kQ3XAttributeMaskInherited  = 0x03FF,
    kQ3XAttributeMaskInterpolated = kQ3XAttributeMaskSurfaceUV | kQ3XAttributeMaskShadingUV | kQ3XAttributeMaskNormal | kQ3XAttributeMaskAmbientCoefficient | kQ3XAttributeMaskDiffuseColor | kQ3XAttributeMaskSpecularColor | kQ3XAttributeMaskSpecularControl | kQ3XAttributeMaskTransparencyColor | kQ3XAttributeMaskSurfaceTangent
};


typedef unsigned long                   TQ3XAttributeMask;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3XAttributeMask )
Q3XAttributeSet_GetMask         (TQ3AttributeSet        attributeSet);


/******************************************************************************
 **                                                                          **
 **                         Renderer Draw Context Tools                      **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */

typedef struct OpaqueTQ3XDrawRegion*    TQ3XDrawRegion;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3XDrawContext_GetDrawRegion    (TQ3DrawContextObject   drawContext,
                                 TQ3XDrawRegion *       drawRegion);

#endif  /* CALL_NOT_IN_CARBON */


enum TQ3XDrawContextValidationMasks {
    kQ3XDrawContextValidationClearFlags = 0x00000000,
    kQ3XDrawContextValidationDoubleBuffer = 1 << 0,
    kQ3XDrawContextValidationShader = 1 << 1,
    kQ3XDrawContextValidationClearFunction = 1 << 2,
    kQ3XDrawContextValidationActiveBuffer = 1 << 3,
    kQ3XDrawContextValidationInternalOffScreen = 1 << 4,
    kQ3XDrawContextValidationPane = 1 << 5,
    kQ3XDrawContextValidationMask = 1 << 6,
    kQ3XDrawContextValidationDevice = 1 << 7,
    kQ3XDrawContextValidationWindow = 1 << 8,
    kQ3XDrawContextValidationWindowSize = 1 << 9,
    kQ3XDrawContextValidationWindowClip = 1 << 10,
    kQ3XDrawContextValidationWindowPosition = 1 << 11,
    kQ3XDrawContextValidationPlatformAttributes = 1 << 12,
    kQ3XDrawContextValidationForegroundShader = 1 << 13,
    kQ3XDrawContextValidationBackgroundShader = 1 << 14,
    kQ3XDrawContextValidationColorPalette = 1 << 15,
    kQ3XDrawContextValidationAll = (long)0xFFFFFFFF
};
typedef enum TQ3XDrawContextValidationMasks TQ3XDrawContextValidationMasks;

typedef unsigned long                   TQ3XDrawContextValidation;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3XDrawContext_ClearValidationFlags (TQ3DrawContextObject  drawContext);

EXTERN_API_C( TQ3Status )
Q3XDrawContext_GetValidationFlags (TQ3DrawContextObject  drawContext,
                                 TQ3XDrawContextValidation * validationFlags);


/******************************************************************************
 **                                                                          **
 **                         Renderer Draw Region Tools                       **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3XDevicePixelType {
                                                                /* These do not indicate byte ordering   */
    kQ3XDevicePixelTypeInvalid  = 0,                            /* Unknown, un-initialized type    */
    kQ3XDevicePixelTypeRGB32    = 1,                            /* Alpha:8 (ignored), R:8, G:8, B:8 */
    kQ3XDevicePixelTypeARGB32   = 2,                            /* Alpha:8, R:8, G:8, B:8           */
    kQ3XDevicePixelTypeRGB24    = 3,                            /* 24 bits/pixel, R:8, G:8, B:8    */
    kQ3XDevicePixelTypeRGB16    = 4,                            /* Alpha:1 (ignored), R:5, G:5, B:5 */
    kQ3XDevicePixelTypeARGB16   = 5,                            /* Alpha:1, R:5, G:5, B:5           */
    kQ3XDevicePixelTypeRGB16_565 = 6,                           /* 16 bits/pixel, R:5, G:6, B:5    */
    kQ3XDevicePixelTypeIndexed8 = 7,                            /* 8-bit color table index          */
    kQ3XDevicePixelTypeIndexed4 = 8,                            /* 4-bit color table index          */
    kQ3XDevicePixelTypeIndexed2 = 9,                            /* 2-bit color table index          */
    kQ3XDevicePixelTypeIndexed1 = 10                            /* 1-bit color table index          */
};
typedef enum TQ3XDevicePixelType TQ3XDevicePixelType;


enum TQ3XClipMaskState {
    kQ3XClipMaskFullyExposed    = 0,
    kQ3XClipMaskPartiallyExposed = 1,
    kQ3XClipMaskNotExposed      = 2
};
typedef enum TQ3XClipMaskState TQ3XClipMaskState;


struct TQ3XColorDescriptor {
    unsigned long                   redShift;
    unsigned long                   redMask;
    unsigned long                   greenShift;
    unsigned long                   greenMask;
    unsigned long                   blueShift;
    unsigned long                   blueMask;
    unsigned long                   alphaShift;
    unsigned long                   alphaMask;
};
typedef struct TQ3XColorDescriptor      TQ3XColorDescriptor;

struct TQ3XDrawRegionDescriptor {
    unsigned long                   width;
    unsigned long                   height;
    unsigned long                   rowBytes;
    unsigned long                   pixelSize;
    TQ3XDevicePixelType             pixelType;
    TQ3XColorDescriptor             colorDescriptor;
    TQ3Endian                       bitOrder;
    TQ3Endian                       byteOrder;
    TQ3Bitmap *                     clipMask;
};
typedef struct TQ3XDrawRegionDescriptor TQ3XDrawRegionDescriptor;

enum TQ3XDrawRegionServicesMasks {
    kQ3XDrawRegionServicesNoneFlag = 0L,
    kQ3XDrawRegionServicesClearFlag = 1 << 0,
    kQ3XDrawRegionServicesDontLockDDSurfaceFlag = 1 << 1
};
typedef enum TQ3XDrawRegionServicesMasks TQ3XDrawRegionServicesMasks;


typedef unsigned long                   TQ3XDrawRegionServices;
typedef CALLBACK_API_C( void , TQ3XDrawRegionRendererPrivateDeleteMethod )(void *rendererPrivate);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetDeviceScaleX   (TQ3XDrawRegion         drawRegion,
                                 float *                deviceScaleX);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetDeviceScaleY   (TQ3XDrawRegion         drawRegion,
                                 float *                deviceScaleY);


EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetDeviceOffsetX  (TQ3XDrawRegion         drawRegion,
                                 float *                deviceOffsetX);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetDeviceOffsetY  (TQ3XDrawRegion         drawRegion,
                                 float *                deviceOffsetX);


EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetWindowScaleX   (TQ3XDrawRegion         drawRegion,
                                 float *                windowScaleX);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetWindowScaleY   (TQ3XDrawRegion         drawRegion,
                                 float *                windowScaleY);


EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetWindowOffsetX  (TQ3XDrawRegion         drawRegion,
                                 float *                windowOffsetX);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetWindowOffsetY  (TQ3XDrawRegion         drawRegion,
                                 float *                windowOffsetY);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_IsActive          (TQ3XDrawRegion         drawRegion,
                                 TQ3Boolean *           isActive);


EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetNextRegion     (TQ3XDrawRegion         drawRegion,
                                 TQ3XDrawRegion *       nextDrawRegion);

/* 
 *  One of the next two functions must be called before using a draw region 
 */
/*
 *  Use this Start function if double buffering/image access services from the
 *  Draw Context are not needed, you may still request clear for example
 */
EXTERN_API_C( TQ3Status )
Q3XDrawRegion_Start             (TQ3XDrawRegion         drawRegion,
                                 TQ3XDrawRegionServices  services,
                                 TQ3XDrawRegionDescriptor ** descriptor);

/*
 *  Use this Start function if double buffering or image access services from 
 *  the Draw Context are needed.
 */
EXTERN_API_C( TQ3Status )
Q3XDrawRegion_StartAccessToImageBuffer (TQ3XDrawRegion  drawRegion,
                                 TQ3XDrawRegionServices  services,
                                 TQ3XDrawRegionDescriptor ** descriptor,
                                 void **                image);

/*
 *  This function is used to indicate that access to a DrawRegion is ended.
 */
EXTERN_API_C( TQ3Status )
Q3XDrawRegion_End               (TQ3XDrawRegion         drawRegion);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetDeviceTransform (TQ3XDrawRegion        drawRegion,
                                 TQ3Matrix4x4 **        deviceTransform);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetClipFlags      (TQ3XDrawRegion         drawRegion,
                                 TQ3XClipMaskState *    clipMaskState);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetClipMask       (TQ3XDrawRegion         drawRegion,
                                 TQ3Bitmap **           clipMask);

#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_OS_MAC
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetClipRegion     (TQ3XDrawRegion         drawRegion,
                                 RgnHandle *            rgnHandle);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetGDHandle       (TQ3XDrawRegion         drawRegion,
                                 GDHandle *             gdHandle);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_MAC */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetRendererPrivate (TQ3XDrawRegion        drawRegion,
                                 void **                rendererPrivate);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_SetRendererPrivate (TQ3XDrawRegion        drawRegion,
                                 const void *           rendererPrivate,
                                 TQ3XDrawRegionRendererPrivateDeleteMethod  deleteMethod);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_SetUseDefaultRendererFlag (TQ3XDrawRegion  drawRegion,
                                 TQ3Boolean             flag);

EXTERN_API_C( TQ3Status )
Q3XDrawRegion_GetUseDefaultRendererFlag (TQ3XDrawRegion  drawRegion,
                                 TQ3Boolean *           useDefaultRenderingFlag);



/******************************************************************************
 **                                                                          **
 **                         Renderer Class Methods                           **
 **                                                                          **
 *****************************************************************************/
/*
 *  Methods from Object
 *      kQ3XMethodTypeObjectClassRegister
 *      kQ3XMethodTypeObjectClassUnregister
 *      kQ3XMethodTypeObjectNew
 *      kQ3XMethodTypeObjectDelete
 *      kQ3XMethodTypeObjectRead
 *      kQ3XMethodTypeObjectTraverse
 *      kQ3XMethodTypeObjectWrite
 *      
 *  Methods from Shared
 *      kQ3MethodTypeSharedEdited
 *
 *  Renderer Methods
 *  
 *  The renderer methods should be implemented according to the type
 *  of renderer being written.
 *
 *  For the purposes of documentation, there are two basic types of
 *  renderers: 
 *
 *      Interactive
 *          Interactive Renderer
 *          WireFrame Renderer
 *      
 *      Deferred
 *          a ray-tracer
 *          painter's algorithm renderer (cached in a BSP triangle tree)
 *          an artistic renderer (simulates a pencil drawing, etc.)
 *
 *  The main difference is how each renderer handles incoming state and 
 *  geometry.
 *
 *  An interactive renderer immediately transforms, culls, and shades
 *  incoming geometry and performs rasterization. For example, in a 
 *  single-buffered WireFrame renderer, you will see a new triangle
 *  immediately after Q3Triangle_Draw (if it's visible, of course).
 *
 *  A deferred renderer caches the view state and each geometry, 
 *  converting into any internal queue of drawing commands. Rasterization
 *  is not actually performed until all data has been submitted.
 *  
 *  For example, a ray-tracer may not rasterize anything until the
 *  end of the rendering loop, or until an EndFrame call is made.
 */

/******************************************************************************
 **                                                                          **
 **                     Renderer User Interface Methods                      **
 **                                                                          **
 *****************************************************************************/
/*
 *  kQ3XMethodTypeRendererIsInteractive
 *  
 *  There is no actual method with this - the metahandler simply returns
 *  "(TQ3XFunctionPointer)kQ3True" for this "method" if the renderer is 
 *  intended to be used in interactive settings, and   
 *  "(TQ3XFunctionPointer)kQ3False" otherwise. 
 *  
 *  If neither value is specified in the metahandler, the renderer 
 *  is *assumed to be non-interactive*!!!
 *  
 *  OPTIONAL
 */
#endif  /* CALL_NOT_IN_CARBON */

enum {
    kQ3XMethodTypeRendererIsInteractive = FOUR_CHAR_CODE('isin')
};


/*
 *  TQ3XRendererModalConfigureMethod
 *  
 *  This method should pop up a modal dialog to edit the renderer settings 
 *  found in the renderer private. 
 *  
 *  dialogAnchor - is platform specific data passed by the client to support
 *      movable modal dialogs. 
 *    MacOS: this is a callback to the calling application's event handler.
 *      The renderer calls this function with events not handled by the 
 *      settings dialog. This is necessary in order to support movable modal 
 *      dialogs. An application's event handler must return kQ3True if it 
 *      handles the event passed to the callback or kQ3False if not. 
 *      An application which doesn't want to support a movable modal configure
 *      dialog should pass NULL for the clientEventHandler of TQ3DialogAnchor.
 *      A renderer should implement a non-movable style dialog in that case.
 *    Win32: this is the HWND of the owning window (typically an application's
 *      main window).  (Win32 application modal dialogs are always movable.)
 *  canceled - returns a boolean inditacating that the user canceled the 
 *  dialog.
 *  
 *  OPTIONAL
 */
enum {
    kQ3XMethodTypeRendererModalConfigure = FOUR_CHAR_CODE('rdmc')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererModalConfigureMethod )(TQ3RendererObject renderer, TQ3DialogAnchor dialogAnchor, TQ3Boolean *canceled, void *rendererPrivate);
/*
 *  kQ3XMethodTypeRendererGetNickNameString
 *  
 *      Allows an application to collect the name of the renderer for
 *      display in a user interface item such as a menu.
 *  
 *      If dataBuffer is NULL actualDataSize returns the required size in 
 *      bytes of a data buffer large enough to store the renderer name. 
 *
 *      bufferSize is the actual size of the memory block pointed to by 
 *      dataBuffer
 *
 *      actualDataSize - on return the actual number of bytes written to the
 *      buffer or if dataBuffer is NULL the required size of dataBuffer
 *
 *  OPTIONAL
 */
enum {
    kQ3XMethodTypeRendererGetNickNameString = FOUR_CHAR_CODE('rdns')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererGetNickNameStringMethod )(unsigned char *dataBuffer, unsigned long bufferSize, unsigned long *actualDataSize);
/*
 *  kQ3XMethodTypeRendererGetConfigurationData
 *  
 *      Allows an application to collect private configuration data from the
 *      renderer which it will then save. For example in a preference file, 
 *      a registry key (on Windows) or in a style template. An application 
 *      should tag this data with the renderer's object name.
 *  
 *      If dataBuffer is NULL actualDataSize returns the required size in 
 *      bytes of a data buffer large enough to store private data. 
 *
 *      bufferSize is the actual size of the memory block pointed to by 
 *      dataBuffer
 *
 *      actualDataSize - on return the actual number of bytes written to the
 *      buffer or if dataBuffer is NULL the required size of dataBuffer
 *
 *  OPTIONAL
 */
enum {
    kQ3XMethodTypeRendererGetConfigurationData = FOUR_CHAR_CODE('rdgp')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererGetConfigurationDataMethod )(TQ3RendererObject renderer, unsigned char *dataBuffer, unsigned long bufferSize, unsigned long *actualDataSize, void *rendererPrivate);
/*
 *  TQ3XRendererSetConfigurationDataMethod
 *  
 *      Allows an application to pass private configuration data which has
 *      previously  been obtained from a renderer via 
 *      Q3Renderer_GetConfigurationData. For example in a preference file or 
 *      in a style template. An application should tag this data with the 
 *      renderer's object name.
 *  
 *      bufferSize is the actual size of the memory block pointed to by 
 *      dataBuffer
 *
 *  OPTIONAL
 */
enum {
    kQ3XMethodTypeRendererSetConfigurationData = FOUR_CHAR_CODE('rdsp')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererSetConfigurationDataMethod )(TQ3RendererObject renderer, unsigned char *dataBuffer, unsigned long bufferSize, void *rendererPrivate);
/******************************************************************************
 **                                                                          **
 **                     Renderer Drawing State Methods                       **
 **                                                                          **
 *****************************************************************************/
/*
 *  TQ3RendererStartFrame
 *  
 *  The StartFrame method is called first at Q3View_StartRendering
 *  and should:
 *      - initialize any renderer state to defaults
 *      - extract any and all useful data from the drawContext
 *
 *  If your renderer passed in kQ3RendererFlagClearBuffer at 
 *  registration, then it should also:
 *      - clear the drawContext 
 *  
 *      When clearing, your renderer may opt to:
 *      - NOT clear anything (if you touch every pixel, for example)
 *      - to clear with your own routine, or
 *      - to use the draw context default clear method by calling 
 *      Q3DrawContext_Clear. Q3DrawContext_Clear takes advantage of
 *      any available hardware in the system for clearing.
 *  
 *  This call also signals the start of all default submit commands from
 *  the view. The renderer will receive updates for the default view
 *  state via its Update methods before StartPass is called.
 *  
 *  REQUIRED
 */
enum {
    kQ3XMethodTypeRendererStartFrame = FOUR_CHAR_CODE('rdcl')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererStartFrameMethod )(TQ3ViewObject view, void *rendererPrivate, TQ3DrawContextObject drawContext);
/*
 *  kQ3XMethodTypeRendererStartPass
 *  TQ3XRendererStartPassMethod
 *  
 *  The StartPass method is called during Q3View_StartRendering but after
 *  the StartFrame command. It should:
 *      - collect camera and light information
 *  
 *  If your renderer supports deferred camera transformation, camera is the
 *  main camera which will be submitted in the hierarchy somewhere. It
 *  is never NULL.
 *
 *  If your renderer does not support deferred camera transformation, camera
 *  is the transformed camera.
 *
 *  If your renderer supports deferred light transformation, lights will be
 *  NULL, and will be submitted to your light draw methods instead.
 *
 *  This call signals the end of the default update state, and the start of 
 *  submit commands from the user to the view.
 *
 *  REQUIRED
 */
enum {
    kQ3XMethodTypeRendererStartPass = FOUR_CHAR_CODE('rdst')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererStartPassMethod )(TQ3ViewObject view, void *rendererPrivate, TQ3CameraObject camera, TQ3GroupObject lightGroup);
/*
 *  kQ3XMethodTypeRendererFlushFrame
 *  TQ3XRendererFlushFrameMethod
 *  
 *  This call is only implemented by asynchronous renderers.
 *  
 *  The FlushFrame method is called between the StartPass and EndPass
 *  methods and is called when the user wishes to flush any asynchronous
 *  drawing tasks (which draw to the drawcontext), but does not want 
 *  to block.
 *  
 *  The result of this call is that an image should "eventually" appear
 *  asynchronously.
 *  
 *  For asynchronous rendering, this call is non-blocking.
 *  
 *  An interactive renderer should ensure that all received
 *  geometries are drawn in the image.
 *  
 *  An interactive renderer that talks to hardware should force
 *  the hardware to generate an image.
 *  
 *  A deferred renderer should exhibit a similar behaviour,
 *  though it is not required.  A deferred renderer should spawn
 *  a process that generates a partial image from the currently
 *  accumulated drawing state. 
 *  
 *  However, for renderers such as ray-tracers which generally are
 *  quite compute-intensive, FlushFrame is not required and is a no-op.
 *
 *  OPTIONAL
 */
enum {
    kQ3XMethodTypeRendererFlushFrame = FOUR_CHAR_CODE('rdfl')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererFlushFrameMethod )(TQ3ViewObject view, void *rendererPrivate, TQ3DrawContextObject drawContext);
/*
 *  kQ3XMethodTypeRendererEndPass
 *  TQ3XRendererEndPassMethod
 *  
 *  The EndPass method is called at Q3View_EndRendering and signals
 *  the end of submit commands to the view.
 *
 *  If an error occurs, the renderer should call Q3XError_Post and
 *  return kQ3ViewStatusError.
 *  
 *  If a renderer requires another pass on the renderering data,
 *  it should return kQ3ViewStatusRetraverse.
 *  
 *  If rendering was cancelled, this function will not be called
 *  and the view will handle returning kQ3ViewStatusCancelled;
 *  
 *  Otherwise, your renderer should begin completing the process of 
 *  generating the image in the drawcontext. If you have buffered
 *  any drawing data, flush it. RendererEnd should have a similar
 *  effect as RendererFlushFrame.
 *  
 *  If the renderer is synchronous:
 *      - complete rendering of the entire frame
 *      if the renderer supports kQ3RendererClassSupportDoubleBuffer
 *          - Update the front buffer
 *      else
 *          - DrawContext will update the front buffer after returning
 *
 *  If the renderer is asynchronous
 *      - spawn rendering thread for entire frame
 *      if the renderer supports kQ3RendererClassSupportDoubleBuffer,
 *          - you must eventually update the front buffer asynchronously
 *      else
 *          - you must eventually update the back buffer asynchronously
 *          
 *  REQUIRED
 */
enum {
    kQ3XMethodTypeRendererEndPass = FOUR_CHAR_CODE('rded')
};

typedef CALLBACK_API_C( TQ3ViewStatus , TQ3XRendererEndPassMethod )(TQ3ViewObject view, void *rendererPrivate);
/*
 *  kQ3XMethodTypeRendererEndFrame
 *  TQ3XRendererEndFrame
 *  
 *  This call is only implemented by asynchronous renderers.
 *
 *  The EndFrame method is called from Q3View_Sync, which is
 *  called after Q3View_EndRendering and signals that the user
 *  wishes to see the completed image and is willing to block.
 *  
 *  If your renderer supports kQ3RendererFlagDoubleBuffer
 *      - update the front buffer completely 
 *  else
 *      - update the back buffer completely
 *
 *  This call is equivalent in functionality to RendererFlushFrame
 *  but blocks until the image is completed.
 *  
 *  If no method is supplied, the default is a no-op.
 *  
 *  NOTE: Registering a method of this type indicates that your renderer will
 *  be rendering after Q3View_EndRendering has been called.
 *  
 *  OPTIONAL
 */
enum {
    kQ3XMethodTypeRendererEndFrame = FOUR_CHAR_CODE('rdsy')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererEndFrameMethod )(TQ3ViewObject view, void *rendererPrivate, TQ3DrawContextObject drawContext);
/*
 *  The RendererCancel method is called after Q3View_StartRendering
 *  and signals the termination of all rendering operations.
 *
 *  A renderer should clean up any cached data, and cancel all 
 *  rendering operations.
 *  
 *  If called before Q3View_EndRendering, the RendererEnd method
 *  is NOT called.
 *  
 *  If called after Q3View_EndRendering, the renderer should kill
 *  any threads and terminate any further rendering.
 *  
 *  REQUIRED
 */
enum {
    kQ3XMethodTypeRendererCancel = FOUR_CHAR_CODE('rdab')
};

typedef CALLBACK_API_C( void , TQ3XRendererCancelMethod )(TQ3ViewObject view, void *rendererPrivate);
/******************************************************************************
 **                                                                          **
 **                     Renderer DrawContext Methods                         **
 **                                                                          **
 *****************************************************************************/
/*
 *  kQ3XMethodTypeRendererPush
 *  TQ3XRendererPushMethod
 *  
 *  kQ3XMethodTypeRendererPop
 *  TQ3XRendererPopMethod
 *  
 *  These methods are called whenever the graphics state in the view
 *  is pushed or popped. The user may isolate state by calling:
 *  
 *  Q3Attribute_Submit(kQ3AttributeTypeDiffuseColor, &red, view);
 *  Q3Attribute_Submit(kQ3AttributeTypeTransparencyColor, &blue, view);
 *  Q3Attribute_Submit(kQ3AttributeTypeSpecularColor, &white, view);
 *  Q3Box_Submit(&unitBox, view);
 *  Q3TranslateTransform_Submit(&unitVector, view);
 *  Q3Push_Submit(view);
 *      Q3Attribute_Submit(kQ3AttributeTypeDiffuseColor, &blue, view);
 *      Q3Attribute_Submit(kQ3AttributeTypeTransparencyColor, &green, view);
 *      Q3Box_Submit(&unitBox, view);
 *  Q3Pop_Submit(view); 
 *  Q3TranslateTransform_Submit(&unitVector, view);
 *  Q3Box_Submit(&unitBox, view);
 *  
 *  or by submitting a display group which pushes and pops.
 *  
 *  If you support RendererPush and RendererPop in your renderer:
 *      - you must maintain your drawing state as a stack, as well.
 *      - you will not be updated with the popped state after
 *          RendererPop is called.
 *
 *  If you do not support Push and Pop in your renderer:
 *      - you may maintain a single copy of the drawing state.
 *      - you will be updated with changed fields after the view stack is
 *          popped.
 *
 *  A renderer that supports Push and Pop gets called in the following
 *  sequence (from example above):
 *  
 *  RendererUpdateAttributeDiffuseColor(&red,...)
 *  RendererUpdateAttributeTransparencyColor(&blue,...)
 *  RendererUpdateAttributeSpecularColor(&white,...)
 *  RendererUpdateMatrixLocalToWorld(...)
 *  RendererSubmitGeometryBox(...)
 *  RendererPush(...)
 *      RendererUpdateAttributeDiffuseColor(&blue,...)
 *      RendererUpdateAttributeTransparencyColor(&green,...)
 *      RendererSubmitGeometryBox(...)
 *  RendererPop(...)
 *  RendererUpdateMatrixLocalToWorld(...)
 *  RendererSubmitGeometryBox(...)
 *
 *  A renderer that does not supports Push and Pop gets called in the
 *  following sequence:
 *  
 *  RendererUpdateAttributeDiffuseColor(&red,...)
 *  RendererUpdateAttributeTransparencyColor(&blue,...)
 *  RendererUpdateAttributeSpecularColor(&white,...)
 *  RendererUpdateMatrixLocalToWorld(...)
 *  RendererSubmitGeometryBox(...)
 *      RendererUpdateAttributeDiffuseColor(&blue,...)
 *      RendererUpdateAttributeTransparencyColor(&green,...)
 *      RendererSubmitGeometryBox(...)
 *  RendererUpdateAttributeDiffuseColor(&red,...)
 *  RendererUpdateAttributeTransparencyColor(&blue,...)
 *  RendererUpdateMatrixLocalToWorld(...)
 *  RendererSubmitGeometryBox(...)
 *  
 */
enum {
    kQ3XMethodTypeRendererPush  = FOUR_CHAR_CODE('rdps')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererPushMethod )(TQ3ViewObject view, void *rendererPrivate);
enum {
    kQ3XMethodTypeRendererPop   = FOUR_CHAR_CODE('rdpo')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XRendererPopMethod )(TQ3ViewObject view, void *rendererPrivate);
/******************************************************************************
 **                                                                          **
 **                         Renderer Cull Methods                            **
 **                                                                          **
 *****************************************************************************/
/*
 *  kQ3XMethodTypeRendererIsBoundingBoxVisible
 *  TQ3XRendererIsBoundingBoxVisibleMethod
 *  
 *  This method is called to cull complex groups and geometries 
 *  given their bounding box in local space.
 *  
 *  It should transform the local-space bounding box coordinates to
 *  frustum space and return a TQ3Boolean return value indicating
 *  whether the box appears within the viewing frustum.
 *  
 *  If no method is supplied, the default behavior is to return
 *  kQ3True.
 *  
 */
enum {
    kQ3XMethodTypeRendererIsBoundingBoxVisible = FOUR_CHAR_CODE('rdbx')
};

typedef CALLBACK_API_C( TQ3Boolean , TQ3XRendererIsBoundingBoxVisibleMethod )(TQ3ViewObject view, void *rendererPrivate, const TQ3BoundingBox *bBox);

/******************************************************************************
 **                                                                          **
 **                     Renderer Object Support Methods                      **
 **                                                                          **
 *****************************************************************************/
/*
 *  Drawing methods (Geometry, Camera, Lights)
 *
 */
/*
 *  Geometry MetaHandler
 *  
 *  This metaHandler is required to support 
 *  
 *  kQ3GeometryTypeTriangle
 *  kQ3GeometryTypeLine
 *  kQ3GeometryTypePoint
 *  kQ3GeometryTypeMarker
 *  kQ3GeometryTypePixmapMarker
 *  
 *  REQUIRED
 */
enum {
    kQ3XMethodTypeRendererSubmitGeometryMetaHandler = FOUR_CHAR_CODE('rdgm')
};

typedef CALLBACK_API_C( TQ3XFunctionPointer , TQ3XRendererSubmitGeometryMetaHandlerMethod )(TQ3ObjectType geometryType);
/*
 *  The TQ3XRendererSubmitGeometryMetaHandlerMethod switches on geometryType
 *  of kQ3GeometryTypeFoo and returns methods of type:
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XRendererSubmitGeometryMethod )(TQ3ViewObject view, void *rendererPrivate, TQ3GeometryObject geometry, const void *publicData);
/*
 *  Camera MetaHandler
 *  
 *  This metaHandler, if supplied, indicates that your renderer
 *  handles deferred transformation of the main camera within a scene.
 *  
 *  If not supplied, or an unsupported camera is used, the view will do
 *  the transformation for the renderer and pass in a camera in the 
 *  StartPass method.
 *  
 *  OPTIONAL
 */
enum {
    kQ3XMethodTypeRendererSubmitCameraMetaHandler = FOUR_CHAR_CODE('rdcm')
};

typedef CALLBACK_API_C( TQ3XFunctionPointer , TQ3XRendererSubmitCameraMetaHandlerMethod )(TQ3ObjectType cameraType);
/*
 *  The TQ3XRendererSubmitCameraMetaHandlerMethod switches on cameraType
 *  of kQ3CameraTypeFoo and returns methods of type:
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XRendererSubmitCameraMethod )(TQ3ViewObject view, void *rendererPrivate, TQ3CameraObject camera, const void *publicData);
/*
 *  Light MetaHandler
 *  
 *  This metaHandler, if supplied, indicates that your renderer
 *  handles deferred transformation of lights within a scene.
 *  
 *  If an unsupported light is encountered, it is ignored.
 *
 *  OPTIONAL
 */
enum {
    kQ3XMethodTypeRendererSubmitLightMetaHandler = FOUR_CHAR_CODE('rdlg')
};

typedef CALLBACK_API_C( TQ3XFunctionPointer , TQ3XRendererSubmitLightMetaHandlerMethod )(TQ3ObjectType lightType);
/*
 *  The TQ3XRendererSubmitLightMetaHandlerMethod switches on lightType
 *  of kQ3LightTypeFoo and returns methods of type:
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XRendererSubmitLightMethod )(TQ3ViewObject view, void *rendererPrivate, TQ3LightObject light, const void *publicData);
/*
 *
 *  Update methods
 *
 *  They are called whenever the state has changed. If the renderer supports
 *  the RendererPush and RendererPop methods, it must maintain its own state
 *  stack. Updates are not called for changed data when the view stack is
 *  popped.
 *
 *  See the comments for the RendererPush and RendererPop methods above
 *  for an example of how data is updated.
 *
 */
/*
 *  Style
 */
enum {
    kQ3XMethodTypeRendererUpdateStyleMetaHandler = FOUR_CHAR_CODE('rdyu')
};

typedef CALLBACK_API_C( TQ3XFunctionPointer , TQ3XRendererUpdateStyleMetaHandlerMethod )(TQ3ObjectType styleType);
/*
 *  The TQ3XRendererUpdateStyleMetaHandlerMethod switches on styleType
 *  of kQ3StyleTypeFoo and returns methods of type:
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XRendererUpdateStyleMethod )(TQ3ViewObject view, void *rendererPrivate, const void *publicData);
/*
 *  Attributes
 */
enum {
    kQ3XMethodTypeRendererUpdateAttributeMetaHandler = FOUR_CHAR_CODE('rdau')
};

typedef CALLBACK_API_C( TQ3XFunctionPointer , TQ3XRendererUpdateAttributeMetaHandlerMethod )(TQ3AttributeType attributeType);
/*
 *  The TQ3XRendererUpdateStyleMetaHandlerMethod switches on attributeType
 *  of kQ3AttributeTypeFoo and returns methods of type:
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XRendererUpdateAttributeMethod )(TQ3ViewObject view, void *rendererPrivate, const void *publicData);
/*
 *  Shaders
 */
enum {
    kQ3XMethodTypeRendererUpdateShaderMetaHandler = FOUR_CHAR_CODE('rdsu')
};

typedef CALLBACK_API_C( TQ3XFunctionPointer , TQ3XRendererUpdateShaderMetaHandlerMethod )(TQ3ObjectType shaderType);
/*
 *  The TQ3XRendererUpdateShaderMetaHandlerMethod switches on shaderType
 *  of kQ3ShaderTypeFoo and returns methods of type:
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XRendererUpdateShaderMethod )(TQ3ViewObject view, void *rendererPrivate, TQ3Object *shaderObject);
/*
 *  Matrices
 */
enum {
    kQ3XMethodTypeRendererUpdateMatrixMetaHandler = FOUR_CHAR_CODE('rdxu')
};

typedef TQ3XMetaHandler                 TQ3XRendererUpdateMatrixMetaHandlerMethod;
/*
 *  The TQ3XRendererUpdateShaderMetaHandlerMethod switches on methods
 *  of the form kQ3MethodTypeRendererUpdateMatrixFoo:
 */
enum {
    kQ3XMethodTypeRendererUpdateMatrixLocalToWorld = FOUR_CHAR_CODE('ulwx')
};

enum {
    kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverse = FOUR_CHAR_CODE('ulwi')
};

enum {
    kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverseTranspose = FOUR_CHAR_CODE('ulwt')
};

enum {
    kQ3XMethodTypeRendererUpdateMatrixLocalToCamera = FOUR_CHAR_CODE('ulcx')
};

enum {
    kQ3XMethodTypeRendererUpdateMatrixLocalToFrustum = FOUR_CHAR_CODE('ulfx')
};

enum {
    kQ3XMethodTypeRendererUpdateMatrixWorldToFrustum = FOUR_CHAR_CODE('uwfx')
};

/*
 *  and returns methods of type:
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XRendererUpdateMatrixMethod )(TQ3ViewObject view, void *rendererPrivate, const TQ3Matrix4x4 *matrix);


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DRENDERER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DRENDERER__RESTORE_PACKED_ENUMS)
    #pragma options(pack_enums)
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef PRAGMA_IMPORT_OFF
#pragma import off
#elif PRAGMA_IMPORT
#pragma import reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __QD3DRENDERER__ */

