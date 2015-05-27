/*
     File:       FindByContent.h
 
     Contains:   Public search interface for the Find by Content shared library
 
     Version:    Technology: 2.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1997-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FINDBYCONTENT__
#define __FINDBYCONTENT__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
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

/*
   ***************************************************************************
   Phase values
   These values are passed to the client's callback function to indicate what
   the FBC code is doing.
   ***************************************************************************
*/
enum {
                                                                /* indexing phases*/
    kFBCphIndexing              = 0,
    kFBCphFlushing              = 1,
    kFBCphMerging               = 2,
    kFBCphMakingIndexAccessor   = 3,
    kFBCphCompacting            = 4,
    kFBCphIndexWaiting          = 5,                            /* access phases*/
    kFBCphSearching             = 6,
    kFBCphMakingAccessAccessor  = 7,
    kFBCphAccessWaiting         = 8,                            /* summarization*/
    kFBCphSummarizing           = 9,                            /* indexing or access*/
    kFBCphIdle                  = 10,
    kFBCphCanceling             = 11
};



/*
   ***************************************************************************
   Pointer types
   These point to memory allocated by the FBC shared library, and must be deallocated
   by calls that are defined below.
   ***************************************************************************
*/

/* A collection of state information for searching*/
typedef struct OpaqueFBCSearchSession*  FBCSearchSession;
/* An ordinary C string (used for hit/doc terms)*/
typedef char *                          FBCWordItem;
/* An array of WordItems*/
typedef FBCWordItem *                   FBCWordList;
/*
   ***************************************************************************
   Callback function type for progress reporting and cancelation during
   searching and indexing.  The client's callback function should call
   WaitNextEvent; a "sleep" value of 1 is suggested.  If the callback function
   wants to cancel the current operation (indexing, search, or doc-terms
   retrieval) it should return true.
   ***************************************************************************
*/

typedef CALLBACK_API_C( Boolean , FBCCallbackProcPtr )(UInt16 phase, float percentDone, void *data);
/*
   ***************************************************************************
   Set the callback function for progress reporting and cancelation during
   searching and indexing, and set the amount of heap space to reserve for
   the client's use when FBC allocates memory.
   ***************************************************************************
*/
EXTERN_API_C( void )
FBCSetCallback                  (FBCCallbackProcPtr     fn,
                                 void *                 data);

EXTERN_API_C( void )
FBCSetHeapReservation           (UInt32                 bytes);

/*
   ***************************************************************************
   Find out whether a volume is indexed, the date & time of its last
   completed  update, and its physical size.
   ***************************************************************************
*/

EXTERN_API_C( Boolean )
FBCVolumeIsIndexed              (SInt16                 theVRefNum);

EXTERN_API_C( Boolean )
FBCVolumeIsRemote               (SInt16                 theVRefNum);

EXTERN_API_C( OSErr )
FBCVolumeIndexTimeStamp         (SInt16                 theVRefNum,
                                 UInt32 *               timeStamp);

EXTERN_API_C( OSErr )
FBCVolumeIndexPhysicalSize      (SInt16                 theVRefNum,
                                 UInt32 *               size);

/*
   ***************************************************************************
   Create & configure a search session
   ***************************************************************************
*/

EXTERN_API_C( OSErr )
FBCCreateSearchSession          (FBCSearchSession *     searchSession);

EXTERN_API_C( OSErr )
FBCAddAllVolumesToSession       (FBCSearchSession       theSession,
                                 Boolean                includeRemote);

EXTERN_API_C( OSErr )
FBCSetSessionVolumes            (FBCSearchSession       theSession,
                                 const SInt16           vRefNums[],
                                 UInt16                 numVolumes);

EXTERN_API_C( OSErr )
FBCAddVolumeToSession           (FBCSearchSession       theSession,
                                 SInt16                 vRefNum);

EXTERN_API_C( OSErr )
FBCRemoveVolumeFromSession      (FBCSearchSession       theSession,
                                 SInt16                 vRefNum);

EXTERN_API_C( OSErr )
FBCGetSessionVolumeCount        (FBCSearchSession       theSession,
                                 UInt16 *               count);

EXTERN_API_C( OSErr )
FBCGetSessionVolumes            (FBCSearchSession       theSession,
                                 SInt16                 vRefNums[],
                                 UInt16 *               numVolumes);

EXTERN_API_C( OSErr )
FBCCloneSearchSession           (FBCSearchSession       original,
                                 FBCSearchSession *     clone);

/*
   ***************************************************************************
   Execute a search
   ***************************************************************************
*/

EXTERN_API_C( OSErr )
FBCDoQuerySearch                (FBCSearchSession       theSession,
                                 char *                 queryText,
                                 const FSSpec           targetDirs[],
                                 UInt32                 numTargets,
                                 UInt32                 maxHits,
                                 UInt32                 maxHitWords);

EXTERN_API_C( OSErr )
FBCDoExampleSearch              (FBCSearchSession       theSession,
                                 const UInt32 *         exampleHitNums,
                                 UInt32                 numExamples,
                                 const FSSpec           targetDirs[],
                                 UInt32                 numTargets,
                                 UInt32                 maxHits,
                                 UInt32                 maxHitWords);

EXTERN_API_C( OSErr )
FBCBlindExampleSearch           (FSSpec                 examples[],
                                 UInt32                 numExamples,
                                 const FSSpec           targetDirs[],
                                 UInt32                 numTargets,
                                 UInt32                 maxHits,
                                 UInt32                 maxHitWords,
                                 Boolean                allIndexes,
                                 Boolean                includeRemote,
                                 FBCSearchSession *     theSession);


/*
   ***************************************************************************
   Get information about hits [wrapper for THitItem C++ API]
   ***************************************************************************
*/

EXTERN_API_C( OSErr )
FBCGetHitCount                  (FBCSearchSession       theSession,
                                 UInt32 *               count);

EXTERN_API_C( OSErr )
FBCGetHitDocument               (FBCSearchSession       theSession,
                                 UInt32                 hitNumber,
                                 FSSpec *               theDocument);

EXTERN_API_C( OSErr )
FBCGetHitScore                  (FBCSearchSession       theSession,
                                 UInt32                 hitNumber,
                                 float *                score);

EXTERN_API_C( OSErr )
FBCGetMatchedWords              (FBCSearchSession       theSession,
                                 UInt32                 hitNumber,
                                 UInt32 *               wordCount,
                                 FBCWordList *          list);

EXTERN_API_C( OSErr )
FBCGetTopicWords                (FBCSearchSession       theSession,
                                 UInt32                 hitNumber,
                                 UInt32 *               wordCount,
                                 FBCWordList *          list);


/*
   ***************************************************************************
   Summarize a buffer of text
   ***************************************************************************
*/

EXTERN_API_C( OSErr )
FBCSummarize                    (void *                 inBuf,
                                 UInt32                 inLength,
                                 void *                 outBuf,
                                 UInt32 *               outLength,
                                 UInt32 *               numSentences);

/*
   ***************************************************************************
   Deallocate hit lists, word arrays, and search sessions
   ***************************************************************************
*/

EXTERN_API_C( OSErr )
FBCReleaseSessionHits           (FBCSearchSession       theSession);

EXTERN_API_C( OSErr )
FBCDestroyWordList              (FBCWordList            theList,
                                 UInt32                 wordCount);

EXTERN_API_C( OSErr )
FBCDestroySearchSession         (FBCSearchSession       theSession);

/*
   ***************************************************************************
   Index one or more files and/or folders
   ***************************************************************************
*/

#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
FBCIndexItems                   (FSSpecArrayPtr         theItems,
                                 UInt32                 itemCount);

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

#endif /* __FINDBYCONTENT__ */

