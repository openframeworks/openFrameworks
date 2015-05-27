/*
     File:       HTMLRendering.h
 
     Contains:   HTML Rendering Library Interfaces.
 
     Version:    Technology: 1.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HTMLRENDERING__
#define __HTMLRENDERING__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __CODEFRAGMENTS__
#include <CodeFragments.h>
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
    #pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

typedef struct OpaqueHRReference*       HRReference;
#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
HRGetHTMLRenderingLibVersion    (NumVersion *           returnVers);

#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_RT_MAC_CFM
#if CALL_NOT_IN_CARBON
#ifdef __cplusplus
    inline pascal Boolean HRHTMLRenderingLibAvailable() { return ((HRGetHTMLRenderingLibVersion != (void*)kUnresolvedCFragSymbolAddress) ); }
#else
    #define HRHTMLRenderingLibAvailable()   ((HRGetHTMLRenderingLibVersion != (void*)kUnresolvedCFragSymbolAddress) )
#endif
#endif
#elif TARGET_RT_MAC_MACHO
/* HTML Rendering is currently not available on OS X */
#if CALL_NOT_IN_CARBON
#ifdef __cplusplus
    inline pascal Boolean HRHTMLRenderingLibAvailable() { return false; }
#else
    #define HRHTMLRenderingLibAvailable()   (false)
#endif
#endif
#endif  /*  */

enum {
    kHRRendererHTML32Type       = FOUR_CHAR_CODE('ht32')        /* HTML 3.2 */
};


#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
HRNewReference                  (HRReference *          hrRef,
                                 OSType                 rendererType,
                                 GrafPtr                grafPtr);

EXTERN_API( OSStatus )
HRDisposeReference              (HRReference            hrRef);


EXTERN_API( SInt32 )
HRFreeMemory                    (Size                   inBytesNeeded);


/* System level notifications */
EXTERN_API( void )
HRScreenConfigurationChanged    (void);

EXTERN_API( Boolean )
HRIsHREvent                     (const EventRecord *    eventRecord);


/* Drawing */
EXTERN_API( OSStatus )
HRSetGrafPtr                    (HRReference            hrRef,
                                 GrafPtr                grafPtr);

EXTERN_API( OSStatus )
HRActivate                      (HRReference            hrRef);

EXTERN_API( OSStatus )
HRDeactivate                    (HRReference            hrRef);

EXTERN_API( OSStatus )
HRDraw                          (HRReference            hrRef,
                                 RgnHandle              updateRgnH);

EXTERN_API( OSStatus )
HRSetRenderingRect              (HRReference            hrRef,
                                 const Rect *           renderingRect);

EXTERN_API( OSStatus )
HRGetRenderedImageSize          (HRReference            hrRef,
                                 Point *                renderingSize);

EXTERN_API( OSStatus )
HRScrollToLocation              (HRReference            hrRef,
                                 Point *                location);

EXTERN_API( OSStatus )
HRForceQuickdraw                (HRReference            hrRef,
                                 Boolean                forceQuickdraw);

#endif  /* CALL_NOT_IN_CARBON */


typedef SInt16 HRScrollbarState;
enum {
    eHRScrollbarOn              = 0,
    eHRScrollbarOff             = 1,
    eHRScrollbarAuto            = 2
};

#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
HRSetScrollbarState             (HRReference            hrRef,
                                 HRScrollbarState       hScrollbarState,
                                 HRScrollbarState       vScrollbarState);

EXTERN_API( OSStatus )
HRSetDrawBorder                 (HRReference            hrRef,
                                 Boolean                drawBorder);

EXTERN_API( OSStatus )
HRSetGrowboxCutout              (HRReference            hrRef,
                                 Boolean                allowCutout);

/* Navigation */
EXTERN_API( OSStatus )
HRGoToFile                      (HRReference            hrRef,
                                 const FSSpec *         fsspec,
                                 Boolean                addToHistory,
                                 Boolean                forceRefresh);

EXTERN_API( OSStatus )
HRGoToURL                       (HRReference            hrRef,
                                 const char *           url,
                                 Boolean                addToHistory,
                                 Boolean                forceRefresh);

EXTERN_API( OSStatus )
HRGoToAnchor                    (HRReference            hrRef,
                                 const char *           anchorName);

EXTERN_API( OSStatus )
HRGoToPtr                       (HRReference            hrRef,
                                 char *                 buffer,
                                 UInt32                 bufferSize,
                                 Boolean                addToHistory,
                                 Boolean                forceRefresh);

/* Accessors */
/* either file url or url of <base> tag */
EXTERN_API( OSStatus )
HRGetRootURL                    (HRReference            hrRef,
                                 Handle                 rootURLH);

/* url of <base> tag */
EXTERN_API( OSStatus )
HRGetBaseURL                    (HRReference            hrRef,
                                 Handle                 baseURLH);

/* file url */
EXTERN_API( OSStatus )
HRGetHTMLURL                    (HRReference            hrRef,
                                 Handle                 HTMLURLH);

EXTERN_API( OSStatus )
HRGetTitle                      (HRReference            hrRef,
                                 StringPtr              title);

EXTERN_API( OSStatus )
HRGetHTMLFile                   (HRReference            hrRef,
                                 FSSpec *               fsspec);


/* Utilities */
EXTERN_API( OSStatus )
HRUtilCreateFullURL             (const char *           rootURL,
                                 const char *           linkURL,
                                 Handle                 fullURLH);

EXTERN_API( OSStatus )
HRUtilGetFSSpecFromURL          (const char *           rootURL,
                                 const char *           linkURL,
                                 FSSpec *               destSpec);

/* urlHandle should be valid on input */
EXTERN_API( OSStatus )
HRUtilGetURLFromFSSpec          (const FSSpec *         fsspec,
                                 Handle                 urlHandle);

/*
    Visited links

    If you register a function here, it will be called to determine
    whether or not the given URL has been visited. It should return
    true if the URL has been visited.
    
    In addition to the URLs that the application may add to the list
    of visited links, it should also add URLs that the user clicks
    on. These URLs can be caught by the "add URL to history" callback
    below.
 */
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( Boolean , HRWasURLVisitedProcPtr )(const char *url, void *refCon);
typedef STACK_UPP_TYPE(HRWasURLVisitedProcPtr)                  HRWasURLVisitedUPP;
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
HRRegisterWasURLVisitedUPP      (HRWasURLVisitedUPP     inWasURLVisitedUPP,
                                 HRReference            hrRef,
                                 void *                 inRefCon);

EXTERN_API( void )
HRUnregisterWasURLVisitedUPP    (HRReference            hrRef);



/*
    New URL

    If you register a function here, it will be called every time
    the renderer is going to display a new URL. A few examples of how
    you might use this include...
    
        (a) maintaining a history of URLs
        (b) maintainging a list of visited links
        (c) setting a window title based on the new URL
*/
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( OSStatus , HRNewURLProcPtr )(const char *url, const char *targetFrame, Boolean addToHistory, void *refCon);
typedef STACK_UPP_TYPE(HRNewURLProcPtr)                         HRNewURLUPP;
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
HRRegisterNewURLUPP             (HRNewURLUPP            inNewURLUPP,
                                 HRReference            hrRef,
                                 void *                 inRefCon);

EXTERN_API( void )
HRUnregisterNewURLUPP           (HRReference            hrRef);




/*
    URL to FSSpec function

    If you register a function here, it will be called every time
    the renderer is going to locate a file. The function will be
    passed an enum indicating the type of file being asked for.
 */
#endif  /* CALL_NOT_IN_CARBON */


typedef UInt16 URLSourceType;
enum {
    kHRLookingForHTMLSource     = 1,
    kHRLookingForImage          = 2,
    kHRLookingForEmbedded       = 3,
    kHRLookingForImageMap       = 4,
    kHRLookingForFrame          = 5
};

typedef CALLBACK_API( OSStatus , HRURLToFSSpecProcPtr )(const char *rootURL, const char *linkURL, FSSpec *fsspec, URLSourceType urlSourceType, void *refCon);
typedef STACK_UPP_TYPE(HRURLToFSSpecProcPtr)                    HRURLToFSSpecUPP;
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
HRRegisterURLToFSSpecUPP        (HRURLToFSSpecUPP       inURLToFSSpecUPP,
                                 HRReference            hrRef,
                                 void *                 inRefCon);

EXTERN_API( void )
HRUnregisterURLToFSSpecUPP      (HRReference            hrRef);



#endif  /* CALL_NOT_IN_CARBON */

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(HRWasURLVisitedUPP)
    NewHRWasURLVisitedUPP          (HRWasURLVisitedProcPtr  userRoutine);

    EXTERN_API(HRNewURLUPP)
    NewHRNewURLUPP                 (HRNewURLProcPtr         userRoutine);

    EXTERN_API(HRURLToFSSpecUPP)
    NewHRURLToFSSpecUPP            (HRURLToFSSpecProcPtr    userRoutine);

    EXTERN_API(void)
    DisposeHRWasURLVisitedUPP      (HRWasURLVisitedUPP      userUPP);

    EXTERN_API(void)
    DisposeHRNewURLUPP             (HRNewURLUPP             userUPP);

    EXTERN_API(void)
    DisposeHRURLToFSSpecUPP        (HRURLToFSSpecUPP        userUPP);

    EXTERN_API(Boolean)
    InvokeHRWasURLVisitedUPP       (const char *            url,
                                    void *                  refCon,
                                    HRWasURLVisitedUPP      userUPP);

    EXTERN_API(OSStatus)
    InvokeHRNewURLUPP              (const char *            url,
                                    const char *            targetFrame,
                                    Boolean                 addToHistory,
                                    void *                  refCon,
                                    HRNewURLUPP             userUPP);

    EXTERN_API(OSStatus)
    InvokeHRURLToFSSpecUPP         (const char *            rootURL,
                                    const char *            linkURL,
                                    FSSpec *                fsspec,
                                    URLSourceType           urlSourceType,
                                    void *                  refCon,
                                    HRURLToFSSpecUPP        userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppHRWasURLVisitedProcInfo = 0x000003D0 };               /* pascal 1_byte Func(4_bytes, 4_bytes) */
    enum { uppHRNewURLProcInfo = 0x000037F0 };                      /* pascal 4_bytes Func(4_bytes, 4_bytes, 1_byte, 4_bytes) */
    enum { uppHRURLToFSSpecProcInfo = 0x0000EFF0 };                 /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes, 2_bytes, 4_bytes) */
    #define NewHRWasURLVisitedUPP(userRoutine)                      (HRWasURLVisitedUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppHRWasURLVisitedProcInfo, GetCurrentArchitecture())
    #define NewHRNewURLUPP(userRoutine)                             (HRNewURLUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppHRNewURLProcInfo, GetCurrentArchitecture())
    #define NewHRURLToFSSpecUPP(userRoutine)                        (HRURLToFSSpecUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppHRURLToFSSpecProcInfo, GetCurrentArchitecture())
    #define DisposeHRWasURLVisitedUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeHRNewURLUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define DisposeHRURLToFSSpecUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define InvokeHRWasURLVisitedUPP(url, refCon, userUPP)          (Boolean)CALL_TWO_PARAMETER_UPP((userUPP), uppHRWasURLVisitedProcInfo, (url), (refCon))
    #define InvokeHRNewURLUPP(url, targetFrame, addToHistory, refCon, userUPP)  (OSStatus)CALL_FOUR_PARAMETER_UPP((userUPP), uppHRNewURLProcInfo, (url), (targetFrame), (addToHistory), (refCon))
    #define InvokeHRURLToFSSpecUPP(rootURL, linkURL, fsspec, urlSourceType, refCon, userUPP)  (OSStatus)CALL_FIVE_PARAMETER_UPP((userUPP), uppHRURLToFSSpecProcInfo, (rootURL), (linkURL), (fsspec), (urlSourceType), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewHRWasURLVisitedProc(userRoutine)                     NewHRWasURLVisitedUPP(userRoutine)
#define NewHRNewURLProc(userRoutine)                            NewHRNewURLUPP(userRoutine)
#define NewHRURLToFSSpecProc(userRoutine)                       NewHRURLToFSSpecUPP(userRoutine)
#define CallHRWasURLVisitedProc(userRoutine, url, refCon)       InvokeHRWasURLVisitedUPP(url, refCon, userRoutine)
#define CallHRNewURLProc(userRoutine, url, targetFrame, addToHistory, refCon) InvokeHRNewURLUPP(url, targetFrame, addToHistory, refCon, userRoutine)
#define CallHRURLToFSSpecProc(userRoutine, rootURL, linkURL, fsspec, urlSourceType, refCon) InvokeHRURLToFSSpecUPP(rootURL, linkURL, fsspec, urlSourceType, refCon, userRoutine)

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

#endif /* __HTMLRENDERING__ */

