/*
     File:       DrawSprocket.h
 
     Contains:   Games Sprockets: DrawSprocket interfaces
 
     Version:    Technology: Draw Sprocket 1.7
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1996-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DRAWSPROCKET__
#define __DRAWSPROCKET__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __QDOFFSCREEN__
#include <QDOffscreen.h>
#endif

#ifndef __DISPLAYS__
#include <Displays.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
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
        #define __DRAWSPROCKET__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __DRAWSPROCKET__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/*
********************************************************************************
** constants
********************************************************************************
*/

enum DSpDepthMask {
    kDSpDepthMask_1             = 1 << 0,
    kDSpDepthMask_2             = 1 << 1,
    kDSpDepthMask_4             = 1 << 2,
    kDSpDepthMask_8             = 1 << 3,
    kDSpDepthMask_16            = 1 << 4,
    kDSpDepthMask_32            = 1 << 5,
    kDSpDepthMask_All           = -1L
};
typedef enum DSpDepthMask DSpDepthMask;


enum DSpColorNeeds {
    kDSpColorNeeds_DontCare     = 0L,
    kDSpColorNeeds_Request      = 1L,
    kDSpColorNeeds_Require      = 2L
};
typedef enum DSpColorNeeds DSpColorNeeds;


enum DSpContextState {
    kDSpContextState_Active     = 0L,
    kDSpContextState_Paused     = 1L,
    kDSpContextState_Inactive   = 2L
};
typedef enum DSpContextState DSpContextState;

/* kDSpContextOption_QD3DAccel not yet implemented */

enum DSpContextOption {
                                                                /*    kDSpContextOption_QD3DAccel       = 1<<0,*/
    kDSpContextOption_PageFlip  = 1 << 1,
    kDSpContextOption_DontSyncVBL = 1 << 2,
    kDSpContextOption_Stereoscopic = 1 << 3
};
typedef enum DSpContextOption DSpContextOption;


enum DSpAltBufferOption {
    kDSpAltBufferOption_RowBytesEqualsWidth = 1 << 0
};
typedef enum DSpAltBufferOption DSpAltBufferOption;


enum DSpBufferKind {
    kDSpBufferKind_Normal       = 0
};
typedef enum DSpBufferKind DSpBufferKind;


enum DSpBlitMode {
    kDSpBlitMode_Plain          = 0,
    kDSpBlitMode_SrcKey         = 1 << 0,
    kDSpBlitMode_DstKey         = 1 << 1,
    kDSpBlitMode_Interpolation  = 1 << 2
};
typedef enum DSpBlitMode DSpBlitMode;

/*
********************************************************************************
** data types
********************************************************************************
*/
typedef struct OpaqueDSpAltBufferReference*  DSpAltBufferReference;
typedef struct OpaqueDSpContextReference*  DSpContextReference;
#define kDSpEveryContext ((DSpContextReference)NULL)
typedef CALLBACK_API_C( Boolean , DSpEventProcPtr )(EventRecord *inEvent);
typedef CALLBACK_API_C( Boolean , DSpCallbackProcPtr )(DSpContextReference inContext, void *inRefCon);

struct DSpContextAttributes {
    Fixed                           frequency;
    UInt32                          displayWidth;
    UInt32                          displayHeight;
    UInt32                          reserved1;
    UInt32                          reserved2;
    UInt32                          colorNeeds;
    CTabHandle                      colorTable;
    OptionBits                      contextOptions;
    OptionBits                      backBufferDepthMask;
    OptionBits                      displayDepthMask;
    UInt32                          backBufferBestDepth;
    UInt32                          displayBestDepth;
    UInt32                          pageCount;
    char                            filler[3];
    Boolean                         gameMustConfirmSwitch;
    UInt32                          reserved3[4];
};
typedef struct DSpContextAttributes     DSpContextAttributes;
typedef DSpContextAttributes *          DSpContextAttributesPtr;

struct DSpAltBufferAttributes {
    UInt32                          width;
    UInt32                          height;
    DSpAltBufferOption              options;
    UInt32                          reserved[4];
};
typedef struct DSpAltBufferAttributes   DSpAltBufferAttributes;
typedef struct DSpBlitInfo              DSpBlitInfo;
typedef CALLBACK_API_C( void , DSpBlitDoneProc )(DSpBlitInfo *info);

struct DSpBlitInfo {
    Boolean                         completionFlag;
    char                            filler[3];
    DSpBlitDoneProc                 completionProc;
    DSpContextReference             srcContext;
    CGrafPtr                        srcBuffer;
    Rect                            srcRect;
    UInt32                          srcKey;

    DSpContextReference             dstContext;
    CGrafPtr                        dstBuffer;
    Rect                            dstRect;
    UInt32                          dstKey;

    DSpBlitMode                     mode;
    UInt32                          reserved[4];
};


typedef DSpBlitInfo *                   DSpBlitInfoPtr;
/*
********************************************************************************
** function prototypes
********************************************************************************
*/
/*
** global operations
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
DSpStartup                      (void);

EXTERN_API_C( OSStatus )
DSpShutdown                     (void);

EXTERN_API_C( NumVersion )
DSpGetVersion                   (void);

EXTERN_API_C( OSStatus )
DSpGetFirstContext              (DisplayIDType          inDisplayID,
                                 DSpContextReference *  outContext);

EXTERN_API_C( OSStatus )
DSpGetNextContext               (DSpContextReference    inCurrentContext,
                                 DSpContextReference *  outContext);

EXTERN_API_C( OSStatus )
DSpGetCurrentContext            (DisplayIDType          inDisplayID,
                                 DSpContextReference *  outContext);

EXTERN_API_C( OSStatus )
DSpFindBestContext              (DSpContextAttributesPtr  inDesiredAttributes,
                                 DSpContextReference *  outContext);

EXTERN_API_C( OSStatus )
DSpFindBestContextOnDisplayID   (DSpContextAttributesPtr  inDesiredAttributes,
                                 DSpContextReference *  outContext,
                                 DisplayIDType          inDisplayID);

EXTERN_API_C( OSStatus )
DSpCanUserSelectContext         (DSpContextAttributesPtr  inDesiredAttributes,
                                 Boolean *              outUserCanSelectContext);

EXTERN_API_C( OSStatus )
DSpUserSelectContext            (DSpContextAttributesPtr  inDesiredAttributes,
                                 DisplayIDType          inDialogDisplayLocation,
                                 DSpEventProcPtr        inEventProc,
                                 DSpContextReference *  outContext);

EXTERN_API_C( OSStatus )
DSpProcessEvent                 (EventRecord *          inEvent,
                                 Boolean *              outEventWasProcessed);

EXTERN_API_C( OSStatus )
DSpSetBlankingColor             (const RGBColor *       inRGBColor);

EXTERN_API_C( OSStatus )
DSpSetDebugMode                 (Boolean                inDebugMode);

EXTERN_API_C( OSStatus )
DSpFindContextFromPoint         (Point                  inGlobalPoint,
                                 DSpContextReference *  outContext);

EXTERN_API_C( OSStatus )
DSpGetMouse                     (Point *                outGlobalPoint);

/*
** alternate buffer operations
*/
EXTERN_API_C( OSStatus )
DSpAltBuffer_New                (DSpContextReference    inContext,
                                 Boolean                inVRAMBuffer,
                                 DSpAltBufferAttributes * inAttributes,
                                 DSpAltBufferReference * outAltBuffer);

EXTERN_API_C( OSStatus )
DSpAltBuffer_Dispose            (DSpAltBufferReference  inAltBuffer);

EXTERN_API_C( OSStatus )
DSpAltBuffer_InvalRect          (DSpAltBufferReference  inAltBuffer,
                                 const Rect *           inInvalidRect);

EXTERN_API_C( OSStatus )
DSpAltBuffer_GetCGrafPtr        (DSpAltBufferReference  inAltBuffer,
                                 DSpBufferKind          inBufferKind,
                                 CGrafPtr *             outCGrafPtr,
                                 GDHandle *             outGDevice);

/*
** context operations
*/
/* general */
EXTERN_API_C( OSStatus )
DSpContext_GetAttributes        (DSpContextReference    inContext,
                                 DSpContextAttributesPtr  outAttributes);

EXTERN_API_C( OSStatus )
DSpContext_Reserve              (DSpContextReference    inContext,
                                 DSpContextAttributesPtr  inDesiredAttributes);

EXTERN_API_C( OSStatus )
DSpContext_Queue                (DSpContextReference    inParentContext,
                                 DSpContextReference    inChildContext,
                                 DSpContextAttributesPtr  inDesiredAttributes);

EXTERN_API_C( OSStatus )
DSpContext_Switch               (DSpContextReference    inOldContext,
                                 DSpContextReference    inNewContext);

EXTERN_API_C( OSStatus )
DSpContext_Release              (DSpContextReference    inContext);

EXTERN_API_C( OSStatus )
DSpContext_GetDisplayID         (DSpContextReference    inContext,
                                 DisplayIDType *        outDisplayID);

EXTERN_API_C( OSStatus )
DSpContext_GlobalToLocal        (DSpContextReference    inContext,
                                 Point *                ioPoint);

EXTERN_API_C( OSStatus )
DSpContext_LocalToGlobal        (DSpContextReference    inContext,
                                 Point *                ioPoint);

EXTERN_API_C( OSStatus )
DSpContext_SetVBLProc           (DSpContextReference    inContext,
                                 DSpCallbackProcPtr     inProcPtr,
                                 void *                 inRefCon);

EXTERN_API_C( OSStatus )
DSpContext_GetFlattenedSize     (DSpContextReference    inContext,
                                 UInt32 *               outFlatContextSize);

EXTERN_API_C( OSStatus )
DSpContext_Flatten              (DSpContextReference    inContext,
                                 void *                 outFlatContext);

EXTERN_API_C( OSStatus )
DSpContext_Restore              (void *                 inFlatContext,
                                 DSpContextReference *  outRestoredContext);

EXTERN_API_C( OSStatus )
DSpContext_GetMonitorFrequency  (DSpContextReference    inContext,
                                 Fixed *                outFrequency);

EXTERN_API_C( OSStatus )
DSpContext_SetMaxFrameRate      (DSpContextReference    inContext,
                                 UInt32                 inMaxFPS);

EXTERN_API_C( OSStatus )
DSpContext_GetMaxFrameRate      (DSpContextReference    inContext,
                                 UInt32 *               outMaxFPS);

EXTERN_API_C( OSStatus )
DSpContext_SetState             (DSpContextReference    inContext,
                                 DSpContextState        inState);

EXTERN_API_C( OSStatus )
DSpContext_GetState             (DSpContextReference    inContext,
                                 DSpContextState *      outState);

EXTERN_API_C( OSStatus )
DSpContext_IsBusy               (DSpContextReference    inContext,
                                 Boolean *              outBusyFlag);

/* dirty rectangles */
EXTERN_API_C( OSStatus )
DSpContext_SetDirtyRectGridSize (DSpContextReference    inContext,
                                 UInt32                 inCellPixelWidth,
                                 UInt32                 inCellPixelHeight);

EXTERN_API_C( OSStatus )
DSpContext_GetDirtyRectGridSize (DSpContextReference    inContext,
                                 UInt32 *               outCellPixelWidth,
                                 UInt32 *               outCellPixelHeight);

EXTERN_API_C( OSStatus )
DSpContext_GetDirtyRectGridUnits (DSpContextReference   inContext,
                                 UInt32 *               outCellPixelWidth,
                                 UInt32 *               outCellPixelHeight);

EXTERN_API_C( OSStatus )
DSpContext_InvalBackBufferRect  (DSpContextReference    inContext,
                                 const Rect *           inRect);

/* underlays */
EXTERN_API_C( OSStatus )
DSpContext_SetUnderlayAltBuffer (DSpContextReference    inContext,
                                 DSpAltBufferReference  inNewUnderlay);

EXTERN_API_C( OSStatus )
DSpContext_GetUnderlayAltBuffer (DSpContextReference    inContext,
                                 DSpAltBufferReference * outUnderlay);

/* gamma */
EXTERN_API_C( OSStatus )
DSpContext_FadeGammaOut         (DSpContextReference    inContext,
                                 RGBColor *             inZeroIntensityColor);

EXTERN_API_C( OSStatus )
DSpContext_FadeGammaIn          (DSpContextReference    inContext,
                                 RGBColor *             inZeroIntensityColor);

EXTERN_API_C( OSStatus )
DSpContext_FadeGamma            (DSpContextReference    inContext,
                                 SInt32                 inPercentOfOriginalIntensity,
                                 RGBColor *             inZeroIntensityColor);

/* buffering */
EXTERN_API_C( OSStatus )
DSpContext_SwapBuffers          (DSpContextReference    inContext,
                                 DSpCallbackProcPtr     inBusyProc,
                                 void *                 inUserRefCon);

EXTERN_API_C( OSStatus )
DSpContext_GetBackBuffer        (DSpContextReference    inContext,
                                 DSpBufferKind          inBufferKind,
                                 CGrafPtr *             outBackBuffer);

EXTERN_API_C( OSStatus )
DSpContext_GetFrontBuffer       (DSpContextReference    inContext,
                                 CGrafPtr *             outFrontBuffer);

/* clut operations */
EXTERN_API_C( OSStatus )
DSpContext_SetCLUTEntries       (DSpContextReference    inContext,
                                 const ColorSpec *      inEntries,
                                 UInt16                 inStartingEntry,
                                 UInt16                 inLastEntry);

EXTERN_API_C( OSStatus )
DSpContext_GetCLUTEntries       (DSpContextReference    inContext,
                                 ColorSpec *            outEntries,
                                 UInt16                 inStartingEntry,
                                 UInt16                 inLastEntry);

/* blit operations */
EXTERN_API_C( OSStatus )
DSpBlit_Faster                  (DSpBlitInfoPtr         inBlitInfo,
                                 Boolean                inAsyncFlag);

EXTERN_API_C( OSStatus )
DSpBlit_Fastest                 (DSpBlitInfoPtr         inBlitInfo,
                                 Boolean                inAsyncFlag);


#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __DRAWSPROCKET__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__DRAWSPROCKET__RESTORE_PACKED_ENUMS)
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

#endif /* __DRAWSPROCKET__ */

