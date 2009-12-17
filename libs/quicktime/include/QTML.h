/*
     File:       QTML.h
 
     Contains:   QuickTime Cross-platform specific interfaces
 
     Version:    Technology: QuickTime 4.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1997-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QTML__
#define __QTML__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MACMEMORY__
#include <MacMemory.h>
#endif

#ifndef __MACWINDOWS__
#include <MacWindows.h>
#endif

#ifndef __OSUTILS__
#include <OSUtils.h>
#endif

#ifndef __FILES__
#include <Files.h>
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

EXTERN_API( void )
QTMLYieldCPU                    (void);

/* QTMLYieldCPUTime flags*/
enum {
    kQTMLHandlePortEvents       = (1L << 0)                     /* ask for event handling during the yield*/
};

EXTERN_API( void )
QTMLYieldCPUTime                (long                   milliSeconds,
                                 unsigned long          flags);

typedef struct OpaqueQTMLMutex*         QTMLMutex;
#if !(TARGET_OS_MAC && TARGET_API_MAC_OS8)
typedef struct OpaqueQTMLSyncVar*       QTMLSyncVar;
typedef QTMLSyncVar *                   QTMLSyncVarPtr;
/* InitializeQTML flags*/
enum {
    kInitializeQTMLNoSoundFlag  = (1L << 0),                    /* flag for requesting no sound when calling InitializeQTML*/
    kInitializeQTMLUseGDIFlag   = (1L << 1),                    /* flag for requesting GDI when calling InitializeQTML*/
    kInitializeQTMLDisableDirectSound = (1L << 2),              /* disables QTML's use of DirectSound*/
    kInitializeQTMLUseExclusiveFullScreenModeFlag = (1L << 3),  /* later than QTML 3.0: qtml starts up in exclusive full screen mode*/
    kInitializeQTMLDisableDDClippers = (1L << 4)                /* flag for requesting QTML not to use DirectDraw clipper objects; QTML 5.0 and later*/
};

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
InitializeQTML                  (long                   flag);

EXTERN_API( void )
TerminateQTML                   (void);


/* CreatePortAssociation flags*/
#endif  /* CALL_NOT_IN_CARBON */

enum {
    kQTMLNoIdleEvents           = (1L << 1)                     /* ask for a non-auto-idled port to be created*/
};

#if CALL_NOT_IN_CARBON
EXTERN_API( GrafPtr )
CreatePortAssociation           (void *                 theWnd,
                                 Ptr                    storage,
                                 long                   flags);

EXTERN_API( void )
DestroyPortAssociation          (CGrafPtr               cgp);


#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( void )
QTMLGrabMutex                   (QTMLMutex              mu);

EXTERN_API( Boolean )
QTMLTryGrabMutex                (QTMLMutex              mu);

EXTERN_API( void )
QTMLReturnMutex                 (QTMLMutex              mu);

EXTERN_API( QTMLMutex )
QTMLCreateMutex                 (void);

EXTERN_API( void )
QTMLDestroyMutex                (QTMLMutex              mu);


#if CALL_NOT_IN_CARBON
EXTERN_API( QTMLSyncVarPtr )
QTMLCreateSyncVar               (void);

EXTERN_API( void )
QTMLDestroySyncVar              (QTMLSyncVarPtr         p);

EXTERN_API( long )
QTMLTestAndSetSyncVar           (QTMLSyncVarPtr         sync);

EXTERN_API( void )
QTMLWaitAndSetSyncVar           (QTMLSyncVarPtr         sync);

EXTERN_API( void )
QTMLResetSyncVar                (QTMLSyncVarPtr         sync);


EXTERN_API( void )
InitializeQHdr                  (QHdr *                 qhdr);

EXTERN_API( void )
TerminateQHdr                   (QHdr *                 qhdr);


EXTERN_API( void )
QTMLAcquireWindowList           (void);

EXTERN_API( void )
QTMLReleaseWindowList           (void);

/*
   These routines are here to support "interrupt level" code
      These are dangerous routines, only use if you know what you are doing.
*/

EXTERN_API( long )
QTMLRegisterInterruptSafeThread (unsigned long          threadID,
                                 void *                 threadInfo);

EXTERN_API( long )
QTMLUnregisterInterruptSafeThread (unsigned long        threadID);


EXTERN_API( long )
NativeEventToMacEvent           (void *                 nativeEvent,
                                 EventRecord *          macEvent);

#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_OS_WIN32
#if CALL_NOT_IN_CARBON
EXTERN_API( long )
WinEventToMacEvent              (void *                 winMsg,
                                 EventRecord *          macEvent);

#define WinEventToMacEvent  NativeEventToMacEvent
EXTERN_API( Boolean )
IsTaskBarVisible                (void);

EXTERN_API( void )
ShowHideTaskBar                 (Boolean                showIt);

#endif  /* CALL_NOT_IN_CARBON */

enum {
    kDDSurfaceLocked            = (1L << 0),
    kDDSurfaceStatic            = (1L << 1)
};

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
QTGetDDObject                   (void **                lpDDObject);

EXTERN_API( OSErr )
QTSetDDObject                   (void *                 lpNewDDObject);

EXTERN_API( OSErr )
QTSetDDPrimarySurface           (void *                 lpNewDDSurface,
                                 unsigned long          flags);


EXTERN_API( OSErr )
QTMLGetVolumeRootPath           (char *                 fullPath,
                                 char *                 volumeRootPath,
                                 unsigned long          volumeRootLen);


EXTERN_API( void )
QTMLSetWindowWndProc            (WindowRef              theWindow,
                                 void *                 windowProc);

EXTERN_API( void *)
QTMLGetWindowWndProc            (WindowRef              theWindow);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_WIN32 */

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
QTMLGetCanonicalPathName        (char *                 inName,
                                 char *                 outName,
                                 unsigned long          outLen);

#endif  /* CALL_NOT_IN_CARBON */

enum {
    kFullNativePath             = 0,
    kFileNameOnly               = (1 << 0),
    kDirectoryPathOnly          = (1 << 1),
    kUFSFullPathName            = (1 << 2),
    kTryVDIMask                 = (1 << 3),                     /*    Used in NativePathNameToFSSpec to specify to search VDI mountpoints*/
    kFullPathSpecifiedMask      = (1 << 4)                      /*    the passed in name is a fully qualified full path*/
};

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
FSSpecToNativePathName          (const FSSpec *         inFile,
                                 char *                 outName,
                                 unsigned long          outLen,
                                 long                   flags);

#endif  /* CALL_NOT_IN_CARBON */

enum {
    kErrorIfFileNotFound        = 1L << 31
};

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
NativePathNameToFSSpec          (char *                 inName,
                                 FSSpec *               outFile,
                                 long                   flags);

EXTERN_API( OSErr )
QTGetAliasInfo                  (AliasHandle            alias,
                                 AliasInfoType          index,
                                 char *                 outBuf,
                                 long                   bufLen,
                                 long *                 outLen,
                                 unsigned long          flags);

/*%if QUICKTIME_ONLY*/

#endif  /* CALL_NOT_IN_CARBON */

#else

#define QTMLGrabMutex(MU)
#define QTMLTryGrabMutex(MU) true
#define QTMLReturnMutex(MU)
#define QTMLCreateMutex()      ((struct OpaqueQTMLMutex*) 1)   /* nonzero, since zero indicates creation failure */
#define QTMLDestroyMutex(MU)

/*%endif*/

#endif  /* !(TARGET_OS_MAC && TARGET_API_MAC_OS8) */


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

#endif /* __QTML__ */

