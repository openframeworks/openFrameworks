/*
     File:       QD3DViewer.h
 
     Contains:   MacOS Viewer Controller Interface File.
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DVIEWER__
#define __QD3DVIEWER__

#ifndef __QD3D__
#include <QD3D.h>
#endif

#ifndef __QD3DGROUP__
#include <QD3DGroup.h>
#endif


#if TARGET_OS_MAC
#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
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
        #define __QD3DVIEWER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DVIEWER__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

typedef void *                          TQ3ViewerObject;

typedef CALLBACK_API_C( TQ3Status , TQ3ViewerWindowResizeCallbackMethod )(TQ3ViewerObject theViewer, const void *data);
typedef CALLBACK_API_C( TQ3Status , TQ3ViewerPaneResizeNotifyCallbackMethod )(TQ3ViewerObject theViewer, const void *data);

#if TARGET_OS_MAC
typedef CALLBACK_API_C( OSErr , TQ3ViewerDrawingCallbackMethod )(TQ3ViewerObject theViewer, const void *data);

enum {
    kQ3ViewerShowBadge          = 1 << 0,
    kQ3ViewerActive             = 1 << 1,
    kQ3ViewerControllerVisible  = 1 << 2,
    kQ3ViewerDrawFrame          = 1 << 3,
    kQ3ViewerDraggingOff        = 1 << 4,
    kQ3ViewerButtonCamera       = 1 << 5,
    kQ3ViewerButtonTruck        = 1 << 6,
    kQ3ViewerButtonOrbit        = 1 << 7,
    kQ3ViewerButtonZoom         = 1 << 8,
    kQ3ViewerButtonDolly        = 1 << 9,
    kQ3ViewerButtonReset        = 1 << 10,
    kQ3ViewerOutputTextMode     = 1 << 11,
    kQ3ViewerDragMode           = 1 << 12,
    kQ3ViewerDrawGrowBox        = 1 << 13,
    kQ3ViewerDrawDragBorder     = 1 << 14,
    kQ3ViewerDraggingInOff      = 1 << 15,
    kQ3ViewerDraggingOutOff     = 1 << 16,
    kQ3ViewerButtonOptions      = 1 << 17,
    kQ3ViewerPaneGrowBox        = 1 << 18,
    kQ3ViewerDefault            = 1 << 31
};

enum {
    kQ3ViewerEmpty              = 0,
    kQ3ViewerHasModel           = 1 << 0,
    kQ3ViewerHasUndo            = 1 << 1
};


enum TQ3ViewerCameraView {
    kQ3ViewerCameraRestore      = 0,
    kQ3ViewerCameraFit          = 1,
    kQ3ViewerCameraFront        = 2,
    kQ3ViewerCameraBack         = 3,
    kQ3ViewerCameraLeft         = 4,
    kQ3ViewerCameraRight        = 5,
    kQ3ViewerCameraTop          = 6,
    kQ3ViewerCameraBottom       = 7
};
typedef enum TQ3ViewerCameraView TQ3ViewerCameraView;




/******************************************************************************
 **                                                                          **
 **     Return viewer version number                                         **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
Q3ViewerGetVersion              (unsigned long *        majorRevision,
                                 unsigned long *        minorRevision);


/******************************************************************************
 **                                                                          **
 **     Return viewer release version number                                 **
 **     (in 'vers' format - e.g. 0x01518000 ==> 1.5.1 release)               **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerGetReleaseVersion       (unsigned long *        releaseRevision);


/******************************************************************************
 **                                                                          **
 **                     Creation and destruction                             **
 **             Note that this is not a QuickDraw 3D object                  **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ViewerObject )
Q3ViewerNew                     (CGrafPtr               port,
                                 Rect *                 rect,
                                 unsigned long          flags);

EXTERN_API_C( OSErr )
Q3ViewerDispose                 (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **                 Functions to attach data to a viewer                     **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerUseFile                 (TQ3ViewerObject        theViewer,
                                 long                   refNum);

EXTERN_API_C( OSErr )
Q3ViewerUseData                 (TQ3ViewerObject        theViewer,
                                 void *                 data,
                                 long                   size);


/******************************************************************************
 **                                                                          **
 **     Functions to write data out from the Viewer                          **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerWriteFile               (TQ3ViewerObject        theViewer,
                                 long                   refNum);

EXTERN_API_C( unsigned long )
Q3ViewerWriteData               (TQ3ViewerObject        theViewer,
                                 Handle                 data);


/******************************************************************************
 **                                                                          **
 **     Use this function to force the Viewer to re-draw                     **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerDraw                    (TQ3ViewerObject        theViewer);

EXTERN_API_C( OSErr )
Q3ViewerDrawContent             (TQ3ViewerObject        theViewer);

EXTERN_API_C( OSErr )
Q3ViewerDrawControlStrip        (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **     Function used by the Viewer to filter and handle events              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( Boolean )
Q3ViewerEvent                   (TQ3ViewerObject        theViewer,
                                 EventRecord *          evt);


/******************************************************************************
 **                                                                          **
 **     This function returns a PICT of the contents of the                  **
 **     Viewer's window.  The application should dispose the PICT.           **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( PicHandle )
Q3ViewerGetPict                 (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **                     Calls for dealing with Buttons                       **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerGetButtonRect           (TQ3ViewerObject        theViewer,
                                 unsigned long          button,
                                 Rect *                 rect);

EXTERN_API_C( unsigned long )
Q3ViewerGetCurrentButton        (TQ3ViewerObject        theViewer);

EXTERN_API_C( OSErr )
Q3ViewerSetCurrentButton        (TQ3ViewerObject        theViewer,
                                 unsigned long          button);


/******************************************************************************
 **                                                                          **
 **     Functions to set/get the group to be displayed by the Viewer.        **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerUseGroup                (TQ3ViewerObject        theViewer,
                                 TQ3GroupObject         group);

EXTERN_API_C( TQ3GroupObject )
Q3ViewerGetGroup                (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **     Functions to set/get the color used to clear the window              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerSetBackgroundColor      (TQ3ViewerObject        theViewer,
                                 TQ3ColorARGB *         color);

EXTERN_API_C( OSErr )
Q3ViewerGetBackgroundColor      (TQ3ViewerObject        theViewer,
                                 TQ3ColorARGB *         color);


/******************************************************************************
 **                                                                          **
 **     Getting/Setting a Viewer's View object.                              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ViewObject )
Q3ViewerGetView                 (TQ3ViewerObject        theViewer);

EXTERN_API_C( OSErr )
Q3ViewerRestoreView             (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **     Calls for setting/getting viewer flags                               **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerSetFlags                (TQ3ViewerObject        theViewer,
                                 unsigned long          flags);

EXTERN_API_C( unsigned long )
Q3ViewerGetFlags                (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **     Calls related to bounds/dimensions.  Bounds is the size of           **
 **     the window.  Dimensions can either be the Rect from the ViewHints    **
 **     or the current dimensions of the window (if you do a Set).           **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerSetBounds               (TQ3ViewerObject        theViewer,
                                 Rect *                 bounds);

EXTERN_API_C( OSErr )
Q3ViewerGetBounds               (TQ3ViewerObject        theViewer,
                                 Rect *                 bounds);

EXTERN_API_C( OSErr )
Q3ViewerSetDimension            (TQ3ViewerObject        theViewer,
                                 unsigned long          width,
                                 unsigned long          height);

EXTERN_API_C( OSErr )
Q3ViewerGetDimension            (TQ3ViewerObject        theViewer,
                                 unsigned long *        width,
                                 unsigned long *        height);

EXTERN_API_C( OSErr )
Q3ViewerGetMinimumDimension     (TQ3ViewerObject        theViewer,
                                 unsigned long *        width,
                                 unsigned long *        height);


/******************************************************************************
 **                                                                          **
 **                         Port related calls                               **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerSetPort                 (TQ3ViewerObject        theViewer,
                                 CGrafPtr               port);

EXTERN_API_C( CGrafPtr )
Q3ViewerGetPort                 (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **     Adjust Cursor should be called from idle loop to allow the Viewer    **
 **     to change the cursor according to the cursor position/object under   **
 **     the cursor.                                                          **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( Boolean )
Q3ViewerAdjustCursor            (TQ3ViewerObject        theViewer,
                                 Point *                pt);

EXTERN_API_C( OSErr )
Q3ViewerCursorChanged           (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **     Returns the state of the viewer.  See the constant defined at the    **
 **     top of this file.                                                    **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( unsigned long )
Q3ViewerGetState                (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **                         Clipboard utilities                              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerClear                   (TQ3ViewerObject        theViewer);

EXTERN_API_C( OSErr )
Q3ViewerCut                     (TQ3ViewerObject        theViewer);

EXTERN_API_C( OSErr )
Q3ViewerCopy                    (TQ3ViewerObject        theViewer);

EXTERN_API_C( OSErr )
Q3ViewerPaste                   (TQ3ViewerObject        theViewer);


/******************************************************************************
 **                                                                          **
 **                         New Event Model                                  **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( Boolean )
Q3ViewerMouseDown               (TQ3ViewerObject        theViewer,
                                 long                   x,
                                 long                   y);

EXTERN_API_C( Boolean )
Q3ViewerContinueTracking        (TQ3ViewerObject        theViewer,
                                 long                   x,
                                 long                   y);

EXTERN_API_C( Boolean )
Q3ViewerMouseUp                 (TQ3ViewerObject        theViewer,
                                 long                   x,
                                 long                   y);

EXTERN_API_C( Boolean )
Q3ViewerHandleKeyEvent          (TQ3ViewerObject        theViewer,
                                 EventRecord *          evt);


/******************************************************************************
 **                                                                          **
 **                                 CallBacks                                **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerSetDrawingCallbackMethod (TQ3ViewerObject       theViewer,
                                 TQ3ViewerDrawingCallbackMethod  callbackMethod,
                                 const void *           data);

EXTERN_API_C( OSErr )
Q3ViewerSetWindowResizeCallback (TQ3ViewerObject        theViewer,
                                 TQ3ViewerWindowResizeCallbackMethod  windowResizeCallbackMethod,
                                 const void *           data);

EXTERN_API_C( OSErr )
Q3ViewerSetPaneResizeNotifyCallback (TQ3ViewerObject    theViewer,
                                 TQ3ViewerPaneResizeNotifyCallbackMethod  paneResizeNotifyCallbackMethod,
                                 const void *           data);


/******************************************************************************
 **                                                                          **
 **                                 Undo                                     **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerUndo                    (TQ3ViewerObject        theViewer);

EXTERN_API_C( Boolean )
Q3ViewerGetUndoString           (TQ3ViewerObject        theViewer,
                                 char *                 str,
                                 unsigned long *        cnt);


/******************************************************************************
 **                                                                          **
 **                             Camera Support                               **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerGetCameraCount          (TQ3ViewerObject        theViewer,
                                 unsigned long *        cnt);

EXTERN_API_C( OSErr )
Q3ViewerSetCameraByNumber       (TQ3ViewerObject        theViewer,
                                 unsigned long          cameraNo);

EXTERN_API_C( OSErr )
Q3ViewerSetCameraByView         (TQ3ViewerObject        theViewer,
                                 TQ3ViewerCameraView    viewType);


/******************************************************************************
 **                                                                          **
 **                         Pop-up Button Options                            **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( OSErr )
Q3ViewerSetRendererType         (TQ3ViewerObject        theViewer,
                                 TQ3ObjectType          rendererType);

EXTERN_API_C( OSErr )
Q3ViewerGetRendererType         (TQ3ViewerObject        theViewer,
                                 TQ3ObjectType *        rendererType);

EXTERN_API_C( OSErr )
Q3ViewerChangeBrightness        (TQ3ViewerObject        theViewer,
                                 float                  brightness);

EXTERN_API_C( OSErr )
Q3ViewerSetRemoveBackfaces      (TQ3ViewerObject        theViewer,
                                 TQ3Boolean             remove);

EXTERN_API_C( OSErr )
Q3ViewerGetRemoveBackfaces      (TQ3ViewerObject        theViewer,
                                 TQ3Boolean *           remove);

EXTERN_API_C( OSErr )
Q3ViewerSetPhongShading         (TQ3ViewerObject        theViewer,
                                 TQ3Boolean             phong);

EXTERN_API_C( OSErr )
Q3ViewerGetPhongShading         (TQ3ViewerObject        theViewer,
                                 TQ3Boolean *           phong);


#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_MAC */





#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DVIEWER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DVIEWER__RESTORE_PACKED_ENUMS)
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

#endif /* __QD3DVIEWER__ */

