/*
     File:       AppleGuide.h
 
     Contains:   Apple Guide Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __APPLEGUIDE__
#define __APPLEGUIDE__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
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

/* Types*/
typedef UInt32                          AGRefNum;
typedef UInt32                          AGCoachRefNum;
typedef UInt32                          AGContextRefNum;

struct AGAppInfo {
    AEEventID                       eventId;
    long                            refCon;
    void *                          contextObj;                 /* private system field*/
};
typedef struct AGAppInfo                AGAppInfo;
typedef AGAppInfo *                     AGAppInfoPtr;
typedef AGAppInfoPtr *                  AGAppInfoHdl;
typedef CALLBACK_API( OSErr , CoachReplyProcPtr )(Rect *pRect, Ptr name, long refCon);
typedef CALLBACK_API( OSErr , ContextReplyProcPtr )(Ptr pInputData, Size inputDataSize, Ptr *ppOutputData, Size *pOutputDataSize, AGAppInfoHdl hAppInfo);
typedef STACK_UPP_TYPE(CoachReplyProcPtr)                       CoachReplyUPP;
typedef STACK_UPP_TYPE(ContextReplyProcPtr)                     ContextReplyUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(CoachReplyUPP)
    NewCoachReplyUPP               (CoachReplyProcPtr       userRoutine);

    EXTERN_API(ContextReplyUPP)
    NewContextReplyUPP             (ContextReplyProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeCoachReplyUPP           (CoachReplyUPP           userUPP);

    EXTERN_API(void)
    DisposeContextReplyUPP         (ContextReplyUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeCoachReplyUPP            (Rect *                  pRect,
                                    Ptr                     name,
                                    long                    refCon,
                                    CoachReplyUPP           userUPP);

    EXTERN_API(OSErr)
    InvokeContextReplyUPP          (Ptr                     pInputData,
                                    Size                    inputDataSize,
                                    Ptr *                   ppOutputData,
                                    Size *                  pOutputDataSize,
                                    AGAppInfoHdl            hAppInfo,
                                    ContextReplyUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppCoachReplyProcInfo = 0x00000FE0 };                    /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppContextReplyProcInfo = 0x0000FFE0 };                  /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewCoachReplyUPP(userRoutine)                           (CoachReplyUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCoachReplyProcInfo, GetCurrentArchitecture())
    #define NewContextReplyUPP(userRoutine)                         (ContextReplyUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppContextReplyProcInfo, GetCurrentArchitecture())
    #define DisposeCoachReplyUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeContextReplyUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeCoachReplyUPP(pRect, name, refCon, userUPP)       (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppCoachReplyProcInfo, (pRect), (name), (refCon))
    #define InvokeContextReplyUPP(pInputData, inputDataSize, ppOutputData, pOutputDataSize, hAppInfo, userUPP)  (OSErr)CALL_FIVE_PARAMETER_UPP((userUPP), uppContextReplyProcInfo, (pInputData), (inputDataSize), (ppOutputData), (pOutputDataSize), (hAppInfo))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewCoachReplyProc(userRoutine)                          NewCoachReplyUPP(userRoutine)
#define NewContextReplyProc(userRoutine)                        NewContextReplyUPP(userRoutine)
#define CallCoachReplyProc(userRoutine, pRect, name, refCon)    InvokeCoachReplyUPP(pRect, name, refCon, userRoutine)
#define CallContextReplyProc(userRoutine, pInputData, inputDataSize, ppOutputData, pOutputDataSize, hAppInfo) InvokeContextReplyUPP(pInputData, inputDataSize, ppOutputData, pOutputDataSize, hAppInfo, userRoutine)
/* Constants*/


enum {
    kAGDefault                  = 0,
    kAGFrontDatabase            = 1,
    kAGNoMixin                  = (-1)
};


enum {
    kAGViewFullHowdy            = 1,                            /* Full-size Howdy*/
    kAGViewTopicAreas           = 2,                            /* Full-size Topic Areas*/
    kAGViewIndex                = 3,                            /* Full-size Index Terms*/
    kAGViewLookFor              = 4,                            /* Full-size Look-For (Search)*/
    kAGViewSingleHowdy          = 5,                            /* Single-list-size Howdy*/
    kAGViewSingleTopics         = 6                             /* Single-list-size Topics*/
};


enum {
    kAGFileMain                 = FOUR_CHAR_CODE('poco'),
    kAGFileMixin                = FOUR_CHAR_CODE('mixn')
};

/* To test against AGGetAvailableDBTypes*/
enum {
    kAGDBTypeBitAny             = 0x00000001,
    kAGDBTypeBitHelp            = 0x00000002,
    kAGDBTypeBitTutorial        = 0x00000004,
    kAGDBTypeBitShortcuts       = 0x00000008,
    kAGDBTypeBitAbout           = 0x00000010,
    kAGDBTypeBitOther           = 0x00000080
};



typedef UInt16                          AGStatus;
/* Returned by AGGetStatus*/
enum {
    kAGIsNotRunning             = 0,
    kAGIsSleeping               = 1,
    kAGIsActive                 = 2
};

/* Flags used in AGOpen, AGOpenWithSearch, AGOpenWithView*/
enum {
    kHelpViewerSearchAll        = (1 << 0)
};

typedef UInt16                          AGWindowKind;
/* Returned by AGGetFrontWindowKind*/
enum {
    kAGNoWindow                 = 0,
    kAGAccessWindow             = 1,
    kAGPresentationWindow       = 2
};

/* Error Codes*/

/* Not an enum, because other OSErrs are valid.*/
typedef SInt16                          AGErr;
/* Apple Guide error codes*/
enum {
                                                                /* -------------------- Apple event reply codes*/
    kAGErrUnknownEvent          = -2900,
    kAGErrCantStartup           = -2901,
    kAGErrNoAccWin              = -2902,
    kAGErrNoPreWin              = -2903,
    kAGErrNoSequence            = -2904,
    kAGErrNotOopsSequence       = -2905,
    kAGErrReserved06            = -2906,
    kAGErrNoPanel               = -2907,
    kAGErrContentNotFound       = -2908,
    kAGErrMissingString         = -2909,
    kAGErrInfoNotAvail          = -2910,
    kAGErrEventNotAvailable     = -2911,
    kAGErrCannotMakeCoach       = -2912,
    kAGErrSessionIDsNotMatch    = -2913,
    kAGErrMissingDatabaseSpec   = -2914,
    kAGMissingSequenceMap       = -2915,                        /* -------------------- Coach's Chalkboard reply codes*/
    kAGErrItemNotFound          = -2925,
    kAGErrBalloonResourceNotFound = -2926,
    kAGErrChalkResourceNotFound = -2927,
    kAGErrChdvResourceNotFound  = -2928,
    kAGErrAlreadyShowing        = -2929,
    kAGErrBalloonResourceSkip   = -2930,
    kAGErrItemNotVisible        = -2931,
    kAGErrReserved32            = -2932,
    kAGErrNotFrontProcess       = -2933,
    kAGErrMacroResourceNotFound = -2934,                        /* -------------------- API reply codes*/
    kAGErrAppleGuideNotAvailable = -2951,
    kAGErrCannotInitCoach       = -2952,
    kAGErrCannotInitContext     = -2953,
    kAGErrCannotOpenAliasFile   = -2954,
    kAGErrNoAliasResource       = -2955,
    kAGErrDatabaseNotAvailable  = -2956,
    kAGErrDatabaseNotOpen       = -2957,
    kAGErrMissingAppInfoHdl     = -2958,
    kAGErrMissingContextObject  = -2959,
    kAGErrInvalidRefNum         = -2960,
    kAGErrDatabaseOpen          = -2961,
    kAGErrInsufficientMemory    = -2962
};

/* Events*/

/* Not an enum because we want to make assignments.*/
typedef UInt32                          AGEvent;
/* Handy events for AGGeneral.*/
enum {
                                                                /* Panel actions (Require a presentation window).*/
    kAGEventDoCoach             = FOUR_CHAR_CODE('doco'),
    kAGEventDoHuh               = FOUR_CHAR_CODE('dhuh'),
    kAGEventGoNext              = FOUR_CHAR_CODE('gonp'),
    kAGEventGoPrev              = FOUR_CHAR_CODE('gopp'),
    kAGEventHidePanel           = FOUR_CHAR_CODE('pahi'),
    kAGEventReturnBack          = FOUR_CHAR_CODE('gobk'),
    kAGEventShowPanel           = FOUR_CHAR_CODE('pash'),
    kAGEventTogglePanel         = FOUR_CHAR_CODE('patg')
};

/* Functions*/

/*
   AGClose
   Close the database associated with the AGRefNum.
*/

#if CALL_NOT_IN_CARBON
EXTERN_API( AGErr )
AGClose                         (AGRefNum *             refNum)                             TWOWORDINLINE(0x7011, 0xAA6E);

/*
   AGGeneral
   Cause various events to happen.
*/

EXTERN_API( AGErr )
AGGeneral                       (AGRefNum               refNum,
                                 AGEvent                theEvent)                           TWOWORDINLINE(0x700D, 0xAA6E);

/*
   AGGetAvailableDBTypes
   Return the database types available for this application.
*/

EXTERN_API( UInt32 )
AGGetAvailableDBTypes           (void)                                                      TWOWORDINLINE(0x7008, 0xAA6E);

/*
   AGGetFrontWindowKind
   Return the kind of the front window.
*/

EXTERN_API( AGWindowKind )
AGGetFrontWindowKind            (AGRefNum               refNum)                             TWOWORDINLINE(0x700C, 0xAA6E);

/*
   AGGetFSSpec
   Return the FSSpec for the AGRefNum.
*/

EXTERN_API( AGErr )
AGGetFSSpec                     (AGRefNum               refNum,
                                 FSSpec *               fileSpec)                           TWOWORDINLINE(0x700F, 0xAA6E);

/*
   AGGetStatus
   Return the status of Apple Guide.
*/

EXTERN_API( AGStatus )
AGGetStatus                     (void)                                                      TWOWORDINLINE(0x7009, 0xAA6E);

/*
   AGInstallCoachHandler
   Install a Coach object location query handler.
*/

EXTERN_API( AGErr )
AGInstallCoachHandler           (CoachReplyUPP          coachReplyProc,
                                 long                   refCon,
                                 AGCoachRefNum *        resultRefNum)                       TWOWORDINLINE(0x7012, 0xAA6E);

/*
   AGInstallContextHandler
   Install a context check query handler.
*/

EXTERN_API( AGErr )
AGInstallContextHandler         (ContextReplyUPP        contextReplyProc,
                                 AEEventID              eventID,
                                 long                   refCon,
                                 AGContextRefNum *      resultRefNum)                       TWOWORDINLINE(0x7013, 0xAA6E);

/*
   AGIsDatabaseOpen
   Return true if the database associated with the AGRefNum is open.
*/

EXTERN_API( Boolean )
AGIsDatabaseOpen                (AGRefNum               refNum)                             TWOWORDINLINE(0x7006, 0xAA6E);

/*
   AGOpen
   Open a guide database.
*/

EXTERN_API( AGErr )
AGOpen                          (ConstFSSpecPtr         fileSpec,
                                 UInt32                 flags,
                                 Handle                 mixinControl,
                                 AGRefNum *             resultRefNum)                       TWOWORDINLINE(0x7001, 0xAA6E);

/*
   AGOpenWithSearch
   Open a guide database and preset a search string.
*/

EXTERN_API( AGErr )
AGOpenWithSearch                (ConstFSSpecPtr         fileSpec,
                                 UInt32                 flags,
                                 Handle                 mixinControl,
                                 ConstStr255Param       searchString,
                                 AGRefNum *             resultRefNum)                       TWOWORDINLINE(0x7002, 0xAA6E);

/*
   AGOpenWithSequence
   Open a guide database and display a presentation window sequence.
*/

EXTERN_API( AGErr )
AGOpenWithSequence              (ConstFSSpecPtr         fileSpec,
                                 UInt32                 flags,
                                 Handle                 mixinControl,
                                 short                  sequenceID,
                                 AGRefNum *             resultRefNum)                       TWOWORDINLINE(0x7004, 0xAA6E);

/*
   AGOpenWithView
   Open a guide database and override the default view.
*/

EXTERN_API( AGErr )
AGOpenWithView                  (ConstFSSpecPtr         fileSpec,
                                 UInt32                 flags,
                                 Handle                 mixinControl,
                                 short                  viewNum,
                                 AGRefNum *             resultRefNum)                       TWOWORDINLINE(0x7005, 0xAA6E);

/*
   AGQuit
   Make Apple Guide quit.
*/

EXTERN_API( AGErr )
AGQuit                          (void)                                                      TWOWORDINLINE(0x7010, 0xAA6E);

/*
   AGRemoveCoachHandler
   Remove the Coach object location query handler.
*/

EXTERN_API( AGErr )
AGRemoveCoachHandler            (AGCoachRefNum *        resultRefNum)                       TWOWORDINLINE(0x7014, 0xAA6E);

/*
   AGRemoveContextHandler
   Remove the context check query handler.
*/

EXTERN_API( AGErr )
AGRemoveContextHandler          (AGContextRefNum *      resultRefNum)                       TWOWORDINLINE(0x7015, 0xAA6E);

/*
   AGStart
   Start up Apple Guide in the background.
*/

EXTERN_API( AGErr )
AGStart                         (void)                                                      TWOWORDINLINE(0x700A, 0xAA6E);



/* typedef's*/

#endif  /* CALL_NOT_IN_CARBON */

typedef FSSpec                          AGFileFSSpecType;
typedef short                           AGFileSelectorCountType;
typedef short                           AGFileSelectorIndexType;
typedef OSType                          AGFileSelectorType;
typedef long                            AGFileSelectorValueType;
typedef short                           AGFileDBType;
typedef Str63                           AGFileDBMenuNamePtr;
typedef short                           AGFileDBScriptType;
typedef short                           AGFileDBRegionType;
typedef short                           AGFileMajorRevType;
typedef short                           AGFileMinorRevType;
typedef short                           AGFileCountType;
/* Database types (for AGFileDBType parameter).*/
enum {
    kAGFileDBTypeAny            = 0,
    kAGFileDBTypeHelp           = 1,
    kAGFileDBTypeTutorial       = 2,
    kAGFileDBTypeShortcuts      = 3,
    kAGFileDBTypeAbout          = 4,
    kAGFileDBTypeOther          = 8
};



#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
AGFileGetDBMenuName             (const FSSpec *         fileSpec,
                                 Str63                  menuItemNameString);

/* Get the database type.*/
EXTERN_API( OSErr )
AGFileGetDBType                 (const FSSpec *         fileSpec,
                                 AGFileDBType *         databaseType);

/*
   Get the version of the software
   that created this database.
*/
EXTERN_API( OSErr )
AGFileGetDBVersion              (const FSSpec *         fileSpec,
                                 AGFileMajorRevType *   majorRev,
                                 AGFileMinorRevType *   minorRev);

/* Get the database script and region information.*/
EXTERN_API( OSErr )
AGFileGetDBCountry              (const FSSpec *         fileSpec,
                                 AGFileDBScriptType *   script,
                                 AGFileDBRegionType *   region);

/* Return the number of selectors in database.*/
EXTERN_API( AGFileSelectorCountType )
AGFileGetSelectorCount          (const FSSpec *         fileSpec);

/*
   Get the i-th database selector (1 to AGFileSelectorCountType)
   and its value.
*/
EXTERN_API( OSErr )
AGFileGetSelector               (const FSSpec *         fileSpec,
                                 AGFileSelectorIndexType  selectorNumber,
                                 AGFileSelectorType *   selector,
                                 AGFileSelectorValueType * value);

/* Return true if database is mixin.*/
EXTERN_API( Boolean )
AGFileIsMixin                   (const FSSpec *         fileSpec);

/*
   Return the number of database files
   of the specified databaseType and main/mixin.
   Any file creator is acceptible,
   but type must be kAGFileMain or kAGFileMixin.
*/
EXTERN_API( AGFileCountType )
AGFileGetDBCount                (short                  vRefNum,
                                 long                   dirID,
                                 AGFileDBType           databaseType,
                                 Boolean                wantMixin);

/*
   Get the FSSpec for the dbIndex-th database
   of the specified databaseType and main/mixin.
   Any file creator is acceptible,
   but type must be kAGFileMain or kAGFileMixin.
*/
EXTERN_API( OSErr )
AGFileGetIndDB                  (short                  vRefNum,
                                 long                   dirID,
                                 AGFileDBType           databaseType,
                                 Boolean                wantMixin,
                                 short                  dbIndex,
                                 FSSpec *               fileSpec);

/*
   This selector must match with the application
   creator in order for this file to appear in the 
   application's Help menu. Ignored for mixin files
   because they never appear in the Help menu anyway.
   If empty (zeros), will appear in the Help menu
   of any host application.
*/
EXTERN_API( OSErr )
AGFileGetHelpMenuAppCreator     (const FSSpec *         fileSpec,
                                 OSType *               helpMenuAppCreator);

/*
   This selector must match in the main and mixin
   files in order for the mixin to mix-in with the main.
   Empty (zeros) selectors are valid matches.
   A '****' selector will mix-in with any main.
*/
EXTERN_API( OSErr )
AGFileGetMixinMatchSelector     (const FSSpec *         fileSpec,
                                 OSType *               mixinMatchSelector);

/*
   This is the text of the balloon for the
   Help menu item for this database.
*/
EXTERN_API( OSErr )
AGFileGetHelpMenuBalloonText    (const FSSpec *         fileSpec,
                                 Str255                 helpMenuBalloonString);


EXTERN_API( OSErr )
AGGetSystemDB                   (AGFileDBType           databaseType,
                                 Boolean                wantMixin,
                                 FSSpec *               pFileSpec);

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

#endif /* __APPLEGUIDE__ */

