/*
     File:       QD3DView.h
 
     Contains:   View types and routines
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DVIEW__
#define __QD3DVIEW__

#ifndef __QD3D__
#include <QD3D.h>
#endif


#ifndef __QD3DSTYLE__
#include <QD3DStyle.h>
#endif

#ifndef __QD3DSET__
#include <QD3DSet.h>
#endif




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
        #define __QD3DVIEW__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DVIEW__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                     View Type Definitions                                **
 **                                                                          **
 *****************************************************************************/

enum TQ3ViewStatus {
    kQ3ViewStatusDone           = 0,
    kQ3ViewStatusRetraverse     = 1,
    kQ3ViewStatusError          = 2,
    kQ3ViewStatusCancelled      = 3
};
typedef enum TQ3ViewStatus TQ3ViewStatus;


/******************************************************************************
 **                                                                          **
 **                     Default Attribute Set                                **
 **                                                                          **
 *****************************************************************************/
#define kQ3ViewDefaultAmbientCoefficient    1.0
#define kQ3ViewDefaultDiffuseColor          1.0, 1.0, 1.0
#define kQ3ViewDefaultSpecularColor         0.5, 0.5, 0.5
#define kQ3ViewDefaultSpecularControl       4.0
#define kQ3ViewDefaultTransparency          1.0, 1.0, 1.0
#define kQ3ViewDefaultHighlightState        kQ3Off
#define kQ3ViewDefaultHighlightColor        1.0, 0.0, 0.0
#define kQ3ViewDefaultSubdivisionMethod     kQ3SubdivisionMethodScreenSpace
#define kQ3ViewDefaultSubdivisionC1         20.0
#define kQ3ViewDefaultSubdivisionC2         20.0

/******************************************************************************
 **                                                                          **
 **                         View Routines                                    **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3ViewObject )
Q3View_New                      (void);

EXTERN_API_C( TQ3Status )
Q3View_Cancel                   (TQ3ViewObject          view);

/******************************************************************************
 **                                                                          **
 **                     View Rendering routines                              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3View_SetRendererByType        (TQ3ViewObject          view,
                                 TQ3ObjectType          theType);

EXTERN_API_C( TQ3Status )
Q3View_SetRenderer              (TQ3ViewObject          view,
                                 TQ3RendererObject      renderer);

EXTERN_API_C( TQ3Status )
Q3View_GetRenderer              (TQ3ViewObject          view,
                                 TQ3RendererObject *    renderer);

EXTERN_API_C( TQ3Status )
Q3View_StartRendering           (TQ3ViewObject          view);

EXTERN_API_C( TQ3ViewStatus )
Q3View_EndRendering             (TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3View_Flush                    (TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3View_Sync                     (TQ3ViewObject          view);


/******************************************************************************
 **                                                                          **
 **                     View/Bounds/Pick routines                            **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3View_StartBoundingBox         (TQ3ViewObject          view,
                                 TQ3ComputeBounds       computeBounds);

EXTERN_API_C( TQ3ViewStatus )
Q3View_EndBoundingBox           (TQ3ViewObject          view,
                                 TQ3BoundingBox *       result);

EXTERN_API_C( TQ3Status )
Q3View_StartBoundingSphere      (TQ3ViewObject          view,
                                 TQ3ComputeBounds       computeBounds);

EXTERN_API_C( TQ3ViewStatus )
Q3View_EndBoundingSphere        (TQ3ViewObject          view,
                                 TQ3BoundingSphere *    result);

EXTERN_API_C( TQ3Status )
Q3View_StartPicking             (TQ3ViewObject          view,
                                 TQ3PickObject          pick);

EXTERN_API_C( TQ3ViewStatus )
Q3View_EndPicking               (TQ3ViewObject          view);


/******************************************************************************
 **                                                                          **
 **                         View/Camera routines                             **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3View_GetCamera                (TQ3ViewObject          view,
                                 TQ3CameraObject *      camera);

EXTERN_API_C( TQ3Status )
Q3View_SetCamera                (TQ3ViewObject          view,
                                 TQ3CameraObject        camera);


/******************************************************************************
 **                                                                          **
 **                         View/Lights routines                             **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3View_SetLightGroup            (TQ3ViewObject          view,
                                 TQ3GroupObject         lightGroup);

EXTERN_API_C( TQ3Status )
Q3View_GetLightGroup            (TQ3ViewObject          view,
                                 TQ3GroupObject *       lightGroup);


/******************************************************************************
 **                                                                          **
 **                             Idle Method                                  **
 **                                                                          **
 *****************************************************************************/
/*
 *  The idle methods allow the application to register callback routines 
 *  which will be called by the view during especially long operations.
 *
 *  The idle methods may also be used to interrupt long renderings or
 *  traversals.  Inside the idler callback the application can check for
 *  Command-Period, Control-C or clicking a "Cancel" button or whatever else
 *  may be used to let the user interrupt rendering.    
 *
 *  It is NOT LEGAL to call QD3D routines inside an idler callback.
 *
 *  Return kQ3Failure to cancel rendering, kQ3Success to continue. Don't
 *  bother posting an error.
 *
 *  Q3View_SetIdleMethod registers a callback that can be called
 *  by the system during rendering.  Unfortunately there is no way yet
 *  to set timer intervals when you want to be called.  Basically, it is
 *  up to the application's idler callback to check clocks to see if you
 *  were called back only a millisecond ago or an hour ago!
 *
 *  Q3View_SetIdleProgressMethod registers a callback that also gives
 *  progress information. This information is supplied by the renderer, and
 *  may or may not be based on real time.
 *
 *  If a renderer doesn't support the progress method, your method will be
 *  called with current == 0 and completed == 0.
 *  
 *  Otherwise, you are GUARANTEED to get called at least 2 or more times:
 *  
 *  ONCE            idleMethod(view, 0, n)      -> Initialize, Show Dialog
 *  zero or more    idleMethod(view, 1..n-1, n) -> Update progress
 *  ONCE            idleMethod(view, n, n)      -> Exit, Hide Dialog
 *  
 *  "current" is guaranteed to be less than or equal to "completed"
 *  "completed" may change values, but current/complete always indicates
 *  the degree of completion.
 *
 *  The calling conventions aid in managing any data associated with a 
 *  progress user interface indicator.
 */
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( TQ3Status , TQ3ViewIdleMethod )(TQ3ViewObject view, const void *idlerData);
typedef CALLBACK_API_C( TQ3Status , TQ3ViewIdleProgressMethod )(TQ3ViewObject view, const void *idlerData, unsigned long current, unsigned long completed);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3View_SetIdleMethod            (TQ3ViewObject          view,
                                 TQ3ViewIdleMethod      idleMethod,
                                 const void *           idleData);

EXTERN_API_C( TQ3Status )
Q3View_SetIdleProgressMethod    (TQ3ViewObject          view,
                                 TQ3ViewIdleProgressMethod  idleMethod,
                                 const void *           idleData);


/******************************************************************************
 **                                                                          **
 **                             EndFrame Method                              **
 **                                                                          **
 *****************************************************************************/
/*
 *  The end frame method is an alternate way of determining when an
 *  asynchronous renderer has completed rendering a frame. It differs from
 *  Q3View_Sync in that notification of the frame completion is the opposite
 *  direction. 
 *  
 *  With Q3View_Sync the application asks a renderer to finish rendering
 *  a frame, and blocks until the frame is complete.
 *  
 *  With the EndFrame method, the renderer tells the application that is has
 *  completed a frame.
 *
 *  If "Q3View_Sync" is called BEFORE this method has been called, this
 *  method will NOT be called ever.
 *  
 *  If "Q3View_Sync" is called AFTER this method has been called, the
 *  call will return immediately (as the frame has already been completed).
 */
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( void , TQ3ViewEndFrameMethod )(TQ3ViewObject view, void *endFrameData);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3View_SetEndFrameMethod        (TQ3ViewObject          view,
                                 TQ3ViewEndFrameMethod  endFrame,
                                 void *                 endFrameData);


/******************************************************************************
 **                                                                          **
 **                         Push/Pop routines                                **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3Push_Submit                   (TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3Pop_Submit                    (TQ3ViewObject          view);

EXTERN_API_C( TQ3StateOperatorObject )
Q3Push_New                      (void);

EXTERN_API_C( TQ3StateOperatorObject )
Q3Pop_New                       (void);

EXTERN_API_C( TQ3Status )
Q3StateOperator_Submit          (TQ3StateOperatorObject  stateOperator,
                                 TQ3ViewObject          view);


/******************************************************************************
 **                                                                          **
 **     Check if bounding box is visible in the viewing frustum.  Transforms **
 **     the bbox by the current local_to_world transformation matrix and     **
 **     does a clip test to see if it lies in the viewing frustum.           **
 **     This can be used by applications to cull out large chunks of scenes  **
 **     that are not going to be visible.                                    **
 **                                                                          **
 **     The default implementation is to always return kQ3True.  Renderers   **
 **     may override this routine however to do the checking.                **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Boolean )
Q3View_IsBoundingBoxVisible     (TQ3ViewObject          view,
                                 const TQ3BoundingBox * bbox);


/******************************************************************************
 **                                                                          **
 **     Allows display groups to be culled if they are assigned bounding     **
 **     boxes.                                                               **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3View_AllowAllGroupCulling     (TQ3ViewObject          view,
                                 TQ3Boolean             allowCulling);



/******************************************************************************
 **                                                                          **
 **                         DrawContext routines                             **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3View_SetDrawContext           (TQ3ViewObject          view,
                                 TQ3DrawContextObject   drawContext);

EXTERN_API_C( TQ3Status )
Q3View_GetDrawContext           (TQ3ViewObject          view,
                                 TQ3DrawContextObject * drawContext);


/******************************************************************************
 **                                                                          **
 **                         Graphics State routines                          **
 **                                                                          **
 ** The graphics state routines can only be called while rendering (ie. in   **
 ** between calls to start and end rendering calls).  If they are called     **
 ** outside of a rendering loop, they will return with error.                **
 **                                                                          **
 *****************************************************************************/
/******************************************************************************
 **                                                                          **
 **                         Transform routines                               **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3View_GetLocalToWorldMatrixState (TQ3ViewObject        view,
                                 TQ3Matrix4x4 *         matrix);

EXTERN_API_C( TQ3Status )
Q3View_GetWorldToFrustumMatrixState (TQ3ViewObject      view,
                                 TQ3Matrix4x4 *         matrix);

EXTERN_API_C( TQ3Status )
Q3View_GetFrustumToWindowMatrixState (TQ3ViewObject     view,
                                 TQ3Matrix4x4 *         matrix);


/******************************************************************************
 **                                                                          **
 **                         Style state routines                             **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3View_GetBackfacingStyleState  (TQ3ViewObject          view,
                                 TQ3BackfacingStyle *   backfacingStyle);

EXTERN_API_C( TQ3Status )
Q3View_GetInterpolationStyleState (TQ3ViewObject        view,
                                 TQ3InterpolationStyle * interpolationType);

EXTERN_API_C( TQ3Status )
Q3View_GetFillStyleState        (TQ3ViewObject          view,
                                 TQ3FillStyle *         fillStyle);

EXTERN_API_C( TQ3Status )
Q3View_GetHighlightStyleState   (TQ3ViewObject          view,
                                 TQ3AttributeSet *      highlightStyle);

EXTERN_API_C( TQ3Status )
Q3View_GetSubdivisionStyleState (TQ3ViewObject          view,
                                 TQ3SubdivisionStyleData * subdivisionStyle);

EXTERN_API_C( TQ3Status )
Q3View_GetOrientationStyleState (TQ3ViewObject          view,
                                 TQ3OrientationStyle *  fontFacingDirectionStyle);

EXTERN_API_C( TQ3Status )
Q3View_GetReceiveShadowsStyleState (TQ3ViewObject       view,
                                 TQ3Boolean *           receives);

EXTERN_API_C( TQ3Status )
Q3View_GetPickIDStyleState      (TQ3ViewObject          view,
                                 unsigned long *        pickIDStyle);

EXTERN_API_C( TQ3Status )
Q3View_GetPickPartsStyleState   (TQ3ViewObject          view,
                                 TQ3PickParts *         pickPartsStyle);

EXTERN_API_C( TQ3Status )
Q3View_GetAntiAliasStyleState   (TQ3ViewObject          view,
                                 TQ3AntiAliasStyleData * antiAliasData);


/******************************************************************************
 **                                                                          **
 **                     Attribute state routines                             **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3View_GetDefaultAttributeSet   (TQ3ViewObject          view,
                                 TQ3AttributeSet *      attributeSet);

EXTERN_API_C( TQ3Status )
Q3View_SetDefaultAttributeSet   (TQ3ViewObject          view,
                                 TQ3AttributeSet        attributeSet);


EXTERN_API_C( TQ3Status )
Q3View_GetAttributeSetState     (TQ3ViewObject          view,
                                 TQ3AttributeSet *      attributeSet);

EXTERN_API_C( TQ3Status )
Q3View_GetAttributeState        (TQ3ViewObject          view,
                                 TQ3AttributeType       attributeType,
                                 void *                 data);



#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DVIEW__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DVIEW__RESTORE_PACKED_ENUMS)
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

#endif /* __QD3DVIEW__ */

