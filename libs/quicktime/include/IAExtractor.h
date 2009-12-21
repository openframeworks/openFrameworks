/*
     File:       IAExtractor.h
 
     Contains:   Interfaces to Find by Content Plugins that scan files
 
     Version:    Technology: Mac OS 8.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __IAEXTRACTOR__
#define __IAEXTRACTOR__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
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
    #pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

/* modes for IASetDocAccessorReadPositionProc */
enum {
    kIAFromStartMode            = 0,
    kIAFromCurrMode             = 1,
    kIAFromEndMode              = 2
};

/* versions for plug-ins */
enum {
    kIAExtractorVersion1        = 0x00010001,
    kIAExtractorCurrentVersion  = kIAExtractorVersion1
};

/* types */
typedef OSStatus                        IAResult;
typedef struct OpaqueIAPluginRef*       IAPluginRef;
typedef struct OpaqueIADocAccessorRef*  IADocAccessorRef;
typedef struct OpaqueIADocRef*          IADocRef;
/* IAPluginInitBlock functions */
typedef CALLBACK_API_C( void *, IAAllocProcPtr )(UInt32 inSize);
typedef CALLBACK_API_C( void , IAFreeProcPtr )(void *inObject);
typedef CALLBACK_API_C( UInt8 , IAIdleProcPtr )(void);
typedef STACK_UPP_TYPE(IAAllocProcPtr)                          IAAllocUPP;
typedef STACK_UPP_TYPE(IAFreeProcPtr)                           IAFreeUPP;
typedef STACK_UPP_TYPE(IAIdleProcPtr)                           IAIdleUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(IAAllocUPP)
    NewIAAllocUPP                  (IAAllocProcPtr          userRoutine);

    EXTERN_API(IAFreeUPP)
    NewIAFreeUPP                   (IAFreeProcPtr           userRoutine);

    EXTERN_API(IAIdleUPP)
    NewIAIdleUPP                   (IAIdleProcPtr           userRoutine);

    EXTERN_API(void)
    DisposeIAAllocUPP              (IAAllocUPP              userUPP);

    EXTERN_API(void)
    DisposeIAFreeUPP               (IAFreeUPP               userUPP);

    EXTERN_API(void)
    DisposeIAIdleUPP               (IAIdleUPP               userUPP);

    EXTERN_API(void *)
    InvokeIAAllocUPP               (UInt32                  inSize,
                                    IAAllocUPP              userUPP);

    EXTERN_API(void)
    InvokeIAFreeUPP                (void *                  inObject,
                                    IAFreeUPP               userUPP);

    EXTERN_API(UInt8)
    InvokeIAIdleUPP                (IAIdleUPP               userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppIAAllocProcInfo = 0x000000F1 };                       /* 4_bytes Func(4_bytes) */
    enum { uppIAFreeProcInfo = 0x000000C1 };                        /* no_return_value Func(4_bytes) */
    enum { uppIAIdleProcInfo = 0x00000011 };                        /* 1_byte Func() */
    #define NewIAAllocUPP(userRoutine)                              (IAAllocUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIAAllocProcInfo, GetCurrentArchitecture())
    #define NewIAFreeUPP(userRoutine)                               (IAFreeUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIAFreeProcInfo, GetCurrentArchitecture())
    #define NewIAIdleUPP(userRoutine)                               (IAIdleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIAIdleProcInfo, GetCurrentArchitecture())
    #define DisposeIAAllocUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define DisposeIAFreeUPP(userUPP)                               DisposeRoutineDescriptor(userUPP)
    #define DisposeIAIdleUPP(userUPP)                               DisposeRoutineDescriptor(userUPP)
    #define InvokeIAAllocUPP(inSize, userUPP)                       (void *)CALL_ONE_PARAMETER_UPP((userUPP), uppIAAllocProcInfo, (inSize))
    #define InvokeIAFreeUPP(inObject, userUPP)                      CALL_ONE_PARAMETER_UPP((userUPP), uppIAFreeProcInfo, (inObject))
    #define InvokeIAIdleUPP(userUPP)                                (UInt8)CALL_ZERO_PARAMETER_UPP((userUPP), uppIAIdleProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewIAAllocProc(userRoutine)                             NewIAAllocUPP(userRoutine)
#define NewIAFreeProc(userRoutine)                              NewIAFreeUPP(userRoutine)
#define NewIAIdleProc(userRoutine)                              NewIAIdleUPP(userRoutine)
#define CallIAAllocProc(userRoutine, inSize)                    InvokeIAAllocUPP(inSize, userRoutine)
#define CallIAFreeProc(userRoutine, inObject)                   InvokeIAFreeUPP(inObject, userRoutine)
#define CallIAIdleProc(userRoutine)                             InvokeIAIdleUPP(userRoutine)

struct IAPluginInitBlock {
    IAAllocUPP                      Alloc;
    IAFreeUPP                       Free;
    IAIdleUPP                       Idle;
};
typedef struct IAPluginInitBlock        IAPluginInitBlock;
typedef IAPluginInitBlock *             IAPluginInitBlockPtr;
/* IADocAccessorRecord  functions */
typedef CALLBACK_API_C( OSStatus , IADocAccessorOpenProcPtr )(IADocAccessorRef inAccessor);
typedef CALLBACK_API_C( OSStatus , IADocAccessorCloseProcPtr )(IADocAccessorRef inAccessor);
typedef CALLBACK_API_C( OSStatus , IADocAccessorReadProcPtr )(IADocAccessorRef inAccessor, void *buffer, UInt32 *ioSize);
typedef CALLBACK_API_C( OSStatus , IASetDocAccessorReadPositionProcPtr )(IADocAccessorRef inAccessor, SInt32 inMode, SInt32 inOffset);
typedef CALLBACK_API_C( OSStatus , IAGetDocAccessorReadPositionProcPtr )(IADocAccessorRef inAccessor, SInt32 *outPostion);
typedef CALLBACK_API_C( OSStatus , IAGetDocAccessorEOFProcPtr )(IADocAccessorRef inAccessor, SInt32 *outEOF);
typedef STACK_UPP_TYPE(IADocAccessorOpenProcPtr)                IADocAccessorOpenUPP;
typedef STACK_UPP_TYPE(IADocAccessorCloseProcPtr)               IADocAccessorCloseUPP;
typedef STACK_UPP_TYPE(IADocAccessorReadProcPtr)                IADocAccessorReadUPP;
typedef STACK_UPP_TYPE(IASetDocAccessorReadPositionProcPtr)     IASetDocAccessorReadPositionUPP;
typedef STACK_UPP_TYPE(IAGetDocAccessorReadPositionProcPtr)     IAGetDocAccessorReadPositionUPP;
typedef STACK_UPP_TYPE(IAGetDocAccessorEOFProcPtr)              IAGetDocAccessorEOFUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(IADocAccessorOpenUPP)
    NewIADocAccessorOpenUPP        (IADocAccessorOpenProcPtr userRoutine);

    EXTERN_API(IADocAccessorCloseUPP)
    NewIADocAccessorCloseUPP       (IADocAccessorCloseProcPtr userRoutine);

    EXTERN_API(IADocAccessorReadUPP)
    NewIADocAccessorReadUPP        (IADocAccessorReadProcPtr userRoutine);

    EXTERN_API(IASetDocAccessorReadPositionUPP)
    NewIASetDocAccessorReadPositionUPP    (IASetDocAccessorReadPositionProcPtr userRoutine);

    EXTERN_API(IAGetDocAccessorReadPositionUPP)
    NewIAGetDocAccessorReadPositionUPP    (IAGetDocAccessorReadPositionProcPtr userRoutine);

    EXTERN_API(IAGetDocAccessorEOFUPP)
    NewIAGetDocAccessorEOFUPP      (IAGetDocAccessorEOFProcPtr userRoutine);

    EXTERN_API(void)
    DisposeIADocAccessorOpenUPP    (IADocAccessorOpenUPP    userUPP);

    EXTERN_API(void)
    DisposeIADocAccessorCloseUPP    (IADocAccessorCloseUPP  userUPP);

    EXTERN_API(void)
    DisposeIADocAccessorReadUPP    (IADocAccessorReadUPP    userUPP);

    EXTERN_API(void)
    DisposeIASetDocAccessorReadPositionUPP    (IASetDocAccessorReadPositionUPP userUPP);

    EXTERN_API(void)
    DisposeIAGetDocAccessorReadPositionUPP    (IAGetDocAccessorReadPositionUPP userUPP);

    EXTERN_API(void)
    DisposeIAGetDocAccessorEOFUPP    (IAGetDocAccessorEOFUPP userUPP);

    EXTERN_API(OSStatus)
    InvokeIADocAccessorOpenUPP     (IADocAccessorRef        inAccessor,
                                    IADocAccessorOpenUPP    userUPP);

    EXTERN_API(OSStatus)
    InvokeIADocAccessorCloseUPP    (IADocAccessorRef        inAccessor,
                                    IADocAccessorCloseUPP   userUPP);

    EXTERN_API(OSStatus)
    InvokeIADocAccessorReadUPP     (IADocAccessorRef        inAccessor,
                                    void *                  buffer,
                                    UInt32 *                ioSize,
                                    IADocAccessorReadUPP    userUPP);

    EXTERN_API(OSStatus)
    InvokeIASetDocAccessorReadPositionUPP    (IADocAccessorRef inAccessor,
                                    SInt32                  inMode,
                                    SInt32                  inOffset,
                                    IASetDocAccessorReadPositionUPP userUPP);

    EXTERN_API(OSStatus)
    InvokeIAGetDocAccessorReadPositionUPP    (IADocAccessorRef inAccessor,
                                    SInt32 *                outPostion,
                                    IAGetDocAccessorReadPositionUPP userUPP);

    EXTERN_API(OSStatus)
    InvokeIAGetDocAccessorEOFUPP    (IADocAccessorRef       inAccessor,
                                    SInt32 *                outEOF,
                                    IAGetDocAccessorEOFUPP  userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppIADocAccessorOpenProcInfo = 0x000000F1 };             /* 4_bytes Func(4_bytes) */
    enum { uppIADocAccessorCloseProcInfo = 0x000000F1 };            /* 4_bytes Func(4_bytes) */
    enum { uppIADocAccessorReadProcInfo = 0x00000FF1 };             /* 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppIASetDocAccessorReadPositionProcInfo = 0x00000FF1 };  /* 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppIAGetDocAccessorReadPositionProcInfo = 0x000003F1 };  /* 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppIAGetDocAccessorEOFProcInfo = 0x000003F1 };           /* 4_bytes Func(4_bytes, 4_bytes) */
    #define NewIADocAccessorOpenUPP(userRoutine)                    (IADocAccessorOpenUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIADocAccessorOpenProcInfo, GetCurrentArchitecture())
    #define NewIADocAccessorCloseUPP(userRoutine)                   (IADocAccessorCloseUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIADocAccessorCloseProcInfo, GetCurrentArchitecture())
    #define NewIADocAccessorReadUPP(userRoutine)                    (IADocAccessorReadUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIADocAccessorReadProcInfo, GetCurrentArchitecture())
    #define NewIASetDocAccessorReadPositionUPP(userRoutine)         (IASetDocAccessorReadPositionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIASetDocAccessorReadPositionProcInfo, GetCurrentArchitecture())
    #define NewIAGetDocAccessorReadPositionUPP(userRoutine)         (IAGetDocAccessorReadPositionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIAGetDocAccessorReadPositionProcInfo, GetCurrentArchitecture())
    #define NewIAGetDocAccessorEOFUPP(userRoutine)                  (IAGetDocAccessorEOFUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIAGetDocAccessorEOFProcInfo, GetCurrentArchitecture())
    #define DisposeIADocAccessorOpenUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeIADocAccessorCloseUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define DisposeIADocAccessorReadUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeIASetDocAccessorReadPositionUPP(userUPP)         DisposeRoutineDescriptor(userUPP)
    #define DisposeIAGetDocAccessorReadPositionUPP(userUPP)         DisposeRoutineDescriptor(userUPP)
    #define DisposeIAGetDocAccessorEOFUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define InvokeIADocAccessorOpenUPP(inAccessor, userUPP)         (OSStatus)CALL_ONE_PARAMETER_UPP((userUPP), uppIADocAccessorOpenProcInfo, (inAccessor))
    #define InvokeIADocAccessorCloseUPP(inAccessor, userUPP)        (OSStatus)CALL_ONE_PARAMETER_UPP((userUPP), uppIADocAccessorCloseProcInfo, (inAccessor))
    #define InvokeIADocAccessorReadUPP(inAccessor, buffer, ioSize, userUPP)  (OSStatus)CALL_THREE_PARAMETER_UPP((userUPP), uppIADocAccessorReadProcInfo, (inAccessor), (buffer), (ioSize))
    #define InvokeIASetDocAccessorReadPositionUPP(inAccessor, inMode, inOffset, userUPP)  (OSStatus)CALL_THREE_PARAMETER_UPP((userUPP), uppIASetDocAccessorReadPositionProcInfo, (inAccessor), (inMode), (inOffset))
    #define InvokeIAGetDocAccessorReadPositionUPP(inAccessor, outPostion, userUPP)  (OSStatus)CALL_TWO_PARAMETER_UPP((userUPP), uppIAGetDocAccessorReadPositionProcInfo, (inAccessor), (outPostion))
    #define InvokeIAGetDocAccessorEOFUPP(inAccessor, outEOF, userUPP)  (OSStatus)CALL_TWO_PARAMETER_UPP((userUPP), uppIAGetDocAccessorEOFProcInfo, (inAccessor), (outEOF))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewIADocAccessorOpenProc(userRoutine)                   NewIADocAccessorOpenUPP(userRoutine)
#define NewIADocAccessorCloseProc(userRoutine)                  NewIADocAccessorCloseUPP(userRoutine)
#define NewIADocAccessorReadProc(userRoutine)                   NewIADocAccessorReadUPP(userRoutine)
#define NewIASetDocAccessorReadPositionProc(userRoutine)        NewIASetDocAccessorReadPositionUPP(userRoutine)
#define NewIAGetDocAccessorReadPositionProc(userRoutine)        NewIAGetDocAccessorReadPositionUPP(userRoutine)
#define NewIAGetDocAccessorEOFProc(userRoutine)                 NewIAGetDocAccessorEOFUPP(userRoutine)
#define CallIADocAccessorOpenProc(userRoutine, inAccessor)      InvokeIADocAccessorOpenUPP(inAccessor, userRoutine)
#define CallIADocAccessorCloseProc(userRoutine, inAccessor)     InvokeIADocAccessorCloseUPP(inAccessor, userRoutine)
#define CallIADocAccessorReadProc(userRoutine, inAccessor, buffer, ioSize) InvokeIADocAccessorReadUPP(inAccessor, buffer, ioSize, userRoutine)
#define CallIASetDocAccessorReadPositionProc(userRoutine, inAccessor, inMode, inOffset) InvokeIASetDocAccessorReadPositionUPP(inAccessor, inMode, inOffset, userRoutine)
#define CallIAGetDocAccessorReadPositionProc(userRoutine, inAccessor, outPostion) InvokeIAGetDocAccessorReadPositionUPP(inAccessor, outPostion, userRoutine)
#define CallIAGetDocAccessorEOFProc(userRoutine, inAccessor, outEOF) InvokeIAGetDocAccessorEOFUPP(inAccessor, outEOF, userRoutine)
/* IADocAccessorRecord */

struct IADocAccessorRecord {
    IADocAccessorRef                docAccessor;
    IADocAccessorOpenUPP            OpenDoc;
    IADocAccessorCloseUPP           CloseDoc;
    IADocAccessorReadUPP            ReadDoc;
    IASetDocAccessorReadPositionUPP  SetReadPosition;
    IAGetDocAccessorReadPositionUPP  GetReadPosition;
    IAGetDocAccessorEOFUPP          GetEOF;
};
typedef struct IADocAccessorRecord      IADocAccessorRecord;

typedef IADocAccessorRecord *           IADocAccessorPtr;
/*
   If building a text extractor, define BUILDING_IAEXTRACTOR to 1 to export
   the following functions.  If building a client of text extractor plug-ins,
   define BUILDING_IAEXTRACTOR to false so the following methods are not
   exported.
*/
#ifndef BUILDING_IAEXTRACTOR
#define BUILDING_IAEXTRACTOR 1
#endif
#if BUILDING_IAEXTRACTOR
#if PRAGMA_IMPORT
#pragma export on
#endif
#endif
/*
    A Sherlock Plugin is a CFM shared library that implements the following functions:
*/

/*
   IAPluginInit - plug-in's method that is called when opened by a client.  The
   plug-in retuns an IAPluginRef which is an opaque type defined by the plug-in
   and used for the current session.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
IAPluginInit                    (IAPluginInitBlockPtr   initBlock,
                                 IAPluginRef *          outPluginRef);

/*
   IAPluginTerm - plug-in's method that is called when closed by a client.  The
   client passes back the IAPluginRef that was returned from IAPluginInit.  At
   this time the plug-in can perform any needed cleanup.
*/
EXTERN_API_C( OSStatus )
IAPluginTerm                    (IAPluginRef            inPluginRef);

/*
   IAGetExtractorVersion - returns the version of the Text Extractor interface that the
   plug-in was built with.
*/
EXTERN_API_C( OSStatus )
IAGetExtractorVersion           (IAPluginRef            inPluginRef,
                                 UInt32 *               outPluginVersion);

/*
   IACountSupportedDocTypes - returns number of document types the plug-in supports
*/
EXTERN_API_C( OSStatus )
IACountSupportedDocTypes        (IAPluginRef            inPluginRef,
                                 UInt32 *               outCount);

/*
   IAGetIndSupportedDocType - returns the nth document type the plug-in supports.
   First item is one not zero.  Returns the MIME type of supported document
*/
EXTERN_API_C( OSStatus )
IAGetIndSupportedDocType        (IAPluginRef            inPluginRef,
                                 UInt32                 inIndex,
                                 char **                outMIMEType);

/*
   IAOpenDocument - returns a reference to the text of a document.  Client passes in an
   IADocAccessorRecord* that the plug-in can use to to access a document.  Plug-in
   returns IADocRef which an opaque type defined by plug-in to reference the text of
   a document.
*/
EXTERN_API_C( OSStatus )
IAOpenDocument                  (IAPluginRef            inPluginRef,
                                 IADocAccessorRecord *  inDoc,
                                 IADocRef *             outDoc);

/*
   IACloseDocument - perform any cleanup for IADocRef that was returned from IAOpenDocument.
*/
EXTERN_API_C( OSStatus )
IACloseDocument                 (IADocRef               inDoc);

/*
   IAGetNextTextRun - get next run of text.  On input, ioSize is the length of buffer argument.
   On output, ioSize contains the number of bytes written to buffer argument
*/
EXTERN_API_C( OSStatus )
IAGetNextTextRun                (IADocRef               inDoc,
                                 void *                 buffer,
                                 UInt32 *               ioSize);

/*
   IAGetTextRunInfo - get information about the text returned from the last call to IAGetNextTextRun.
   Arguments outEncoding ane outLanguage are options an NULL can be passed in.  If arguments are
   non-null, plug-in will return pointer to internet encoding and language of last run of text.
   If encoding or language are unknown, plug-in will set *outEncoding and/or *outLanguage to NULL. 
*/
EXTERN_API_C( OSStatus )
IAGetTextRunInfo                (IADocRef               inDoc,
                                 char **                outEncoding,
                                 char **                outLanguage);



#if BUILDING_IAEXTRACTOR
#if PRAGMA_IMPORT
#pragma export off
#endif
#endif




#endif  /* CALL_NOT_IN_CARBON */


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

#endif /* __IAEXTRACTOR__ */

