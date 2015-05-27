/*
     File:       Terminals.h
 
     Contains:   Communications Toolbox Terminal tool Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1988-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TERMINALS__
#define __TERMINALS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __CTBUTILITIES__
#include <CTBUtilities.h>
#endif

#ifndef __CONNECTIONS__
#include <Connections.h>
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

#if CALL_NOT_IN_CARBON
enum {
    curTMVersion                = 2                             /* current Terminal Manager version */
};

enum {
    curTermEnvRecVers           = 0                             /* current Terminal Manager Environment Record version */
};

enum {
                                                                /* error codes    */
    tmGenericError              = -1,
    tmNoErr                     = 0,
    tmNotSent                   = 1,
    tmEnvironsChanged           = 2,
    tmNotSupported              = 7,
    tmNoTools                   = 8
};

typedef OSErr                           TMErr;
enum {
    tmInvisible                 = 1 << 0,
    tmSaveBeforeClear           = 1 << 1,
    tmNoMenus                   = 1 << 2,
    tmAutoScroll                = 1 << 3,
    tmConfigChanged             = 1 << 4
};

typedef unsigned long                   TMFlags;
enum {
    selTextNormal               = 1 << 0,
    selTextBoxed                = 1 << 1,
    selGraphicsMarquee          = 1 << 2,
    selGraphicsLasso            = 1 << 3,
    tmSearchNoDiacrit           = 1 << 8,
    tmSearchNoCase              = 1 << 9
};

typedef unsigned short                  TMSearchTypes;
typedef short                           TMSelTypes;
enum {
    cursorText                  = 1,
    cursorGraphics              = 2
};

typedef unsigned short                  TMCursorTypes;
enum {
    tmTextTerminal              = 1 << 0,
    tmGraphicsTerminal          = 1 << 1
};

typedef unsigned short                  TMTermTypes;

struct TermDataBlock {
    TMTermTypes                     flags;
    Handle                          theData;
    Handle                          auxData;
    long                            reserved;
};
typedef struct TermDataBlock            TermDataBlock;
typedef TermDataBlock *                 TermDataBlockPtr;
typedef TermDataBlockPtr *              TermDataBlockH;
typedef TermDataBlockPtr *              TermDataBlockHandle;

struct TermEnvironRec {
    short                           version;
    TMTermTypes                     termType;
    short                           textRows;
    short                           textCols;
    Point                           cellSize;
    Rect                            graphicSize;
    Point                           slop;
    Rect                            auxSpace;
};
typedef struct TermEnvironRec           TermEnvironRec;

typedef TermEnvironRec *                TermEnvironPtr;

union TMSelection {
    Rect                            selRect;
    RgnHandle                       selRgnHandle;
};
typedef union TMSelection               TMSelection;
typedef struct TermRecord               TermRecord;

typedef TermRecord *                    TermPtr;
typedef TermPtr *                       TermHandle;
typedef CALLBACK_API( long , TerminalSendProcPtr )(Ptr thePtr, long theSize, long refCon, CMFlags flags);
typedef CALLBACK_API( void , TerminalBreakProcPtr )(long duration, long refCon);
typedef CALLBACK_API( long , TerminalCacheProcPtr )(long refCon, TermDataBlockPtr theTermData);
typedef CALLBACK_API( void , TerminalSearchCallBackProcPtr )(TermHandle hTerm, short refNum, Rect *foundRect);
typedef CALLBACK_API( Boolean , TerminalClikLoopProcPtr )(long refCon);
typedef CALLBACK_API( CMErr , TerminalEnvironsProcPtr )(long refCon, ConnEnvironRec *theEnvirons);
typedef CALLBACK_API( void , TerminalChooseIdleProcPtr )(void );
typedef CALLBACK_API( long , TerminalToolDefProcPtr )(TermHandle hTerm, short msg, long p1, long p2, long p3);
typedef STACK_UPP_TYPE(TerminalSendProcPtr)                     TerminalSendUPP;
typedef STACK_UPP_TYPE(TerminalBreakProcPtr)                    TerminalBreakUPP;
typedef STACK_UPP_TYPE(TerminalCacheProcPtr)                    TerminalCacheUPP;
typedef STACK_UPP_TYPE(TerminalSearchCallBackProcPtr)           TerminalSearchCallBackUPP;
typedef STACK_UPP_TYPE(TerminalClikLoopProcPtr)                 TerminalClikLoopUPP;
typedef STACK_UPP_TYPE(TerminalEnvironsProcPtr)                 TerminalEnvironsUPP;
typedef STACK_UPP_TYPE(TerminalChooseIdleProcPtr)               TerminalChooseIdleUPP;
typedef STACK_UPP_TYPE(TerminalToolDefProcPtr)                  TerminalToolDefUPP;
/*    TMTermTypes     */

struct TermRecord {
    short                           procID;
    TMFlags                         flags;
    TMErr                           errCode;
    long                            refCon;
    long                            userData;
    TerminalToolDefUPP              defProc;
    Ptr                             config;
    Ptr                             oldConfig;
    TerminalEnvironsUPP             environsProc;
    long                            reserved1;
    long                            reserved2;
    Ptr                             tmPrivate;
    TerminalSendUPP                 sendProc;
    TerminalBreakUPP                breakProc;
    TerminalCacheUPP                cacheProc;
    TerminalClikLoopUPP             clikLoop;
    WindowPtr                       owner;
    Rect                            termRect;
    Rect                            viewRect;
    Rect                            visRect;
    long                            lastIdle;
    TMSelection                     selection;
    TMSelTypes                      selType;
    long                            mluField;
};

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(TerminalSendUPP)
    NewTerminalSendUPP             (TerminalSendProcPtr     userRoutine);

    EXTERN_API(TerminalBreakUPP)
    NewTerminalBreakUPP            (TerminalBreakProcPtr    userRoutine);

    EXTERN_API(TerminalCacheUPP)
    NewTerminalCacheUPP            (TerminalCacheProcPtr    userRoutine);

    EXTERN_API(TerminalSearchCallBackUPP)
    NewTerminalSearchCallBackUPP    (TerminalSearchCallBackProcPtr userRoutine);

    EXTERN_API(TerminalClikLoopUPP)
    NewTerminalClikLoopUPP         (TerminalClikLoopProcPtr userRoutine);

    EXTERN_API(TerminalEnvironsUPP)
    NewTerminalEnvironsUPP         (TerminalEnvironsProcPtr userRoutine);

    EXTERN_API(TerminalChooseIdleUPP)
    NewTerminalChooseIdleUPP       (TerminalChooseIdleProcPtr userRoutine);

    EXTERN_API(TerminalToolDefUPP)
    NewTerminalToolDefUPP          (TerminalToolDefProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeTerminalSendUPP         (TerminalSendUPP         userUPP);

    EXTERN_API(void)
    DisposeTerminalBreakUPP        (TerminalBreakUPP        userUPP);

    EXTERN_API(void)
    DisposeTerminalCacheUPP        (TerminalCacheUPP        userUPP);

    EXTERN_API(void)
    DisposeTerminalSearchCallBackUPP    (TerminalSearchCallBackUPP userUPP);

    EXTERN_API(void)
    DisposeTerminalClikLoopUPP     (TerminalClikLoopUPP     userUPP);

    EXTERN_API(void)
    DisposeTerminalEnvironsUPP     (TerminalEnvironsUPP     userUPP);

    EXTERN_API(void)
    DisposeTerminalChooseIdleUPP    (TerminalChooseIdleUPP  userUPP);

    EXTERN_API(void)
    DisposeTerminalToolDefUPP      (TerminalToolDefUPP      userUPP);

    EXTERN_API(long)
    InvokeTerminalSendUPP          (Ptr                     thePtr,
                                    long                    theSize,
                                    long                    refCon,
                                    CMFlags                 flags,
                                    TerminalSendUPP         userUPP);

    EXTERN_API(void)
    InvokeTerminalBreakUPP         (long                    duration,
                                    long                    refCon,
                                    TerminalBreakUPP        userUPP);

    EXTERN_API(long)
    InvokeTerminalCacheUPP         (long                    refCon,
                                    TermDataBlockPtr        theTermData,
                                    TerminalCacheUPP        userUPP);

    EXTERN_API(void)
    InvokeTerminalSearchCallBackUPP    (TermHandle          hTerm,
                                    short                   refNum,
                                    Rect *                  foundRect,
                                    TerminalSearchCallBackUPP userUPP);

    EXTERN_API(Boolean)
    InvokeTerminalClikLoopUPP      (long                    refCon,
                                    TerminalClikLoopUPP     userUPP);

    EXTERN_API(CMErr)
    InvokeTerminalEnvironsUPP      (long                    refCon,
                                    ConnEnvironRec *        theEnvirons,
                                    TerminalEnvironsUPP     userUPP);

    EXTERN_API(void)
    InvokeTerminalChooseIdleUPP    (TerminalChooseIdleUPP   userUPP);

    EXTERN_API(long)
    InvokeTerminalToolDefUPP       (TermHandle              hTerm,
                                    short                   msg,
                                    long                    p1,
                                    long                    p2,
                                    long                    p3,
                                    TerminalToolDefUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppTerminalSendProcInfo = 0x00002FF0 };                  /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes, 2_bytes) */
    enum { uppTerminalBreakProcInfo = 0x000003C0 };                 /* pascal no_return_value Func(4_bytes, 4_bytes) */
    enum { uppTerminalCacheProcInfo = 0x000003F0 };                 /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppTerminalSearchCallBackProcInfo = 0x00000EC0 };        /* pascal no_return_value Func(4_bytes, 2_bytes, 4_bytes) */
    enum { uppTerminalClikLoopProcInfo = 0x000000D0 };              /* pascal 1_byte Func(4_bytes) */
    enum { uppTerminalEnvironsProcInfo = 0x000003E0 };              /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    enum { uppTerminalChooseIdleProcInfo = 0x00000000 };            /* pascal no_return_value Func() */
    enum { uppTerminalToolDefProcInfo = 0x0000FEF0 };               /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewTerminalSendUPP(userRoutine)                         (TerminalSendUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTerminalSendProcInfo, GetCurrentArchitecture())
    #define NewTerminalBreakUPP(userRoutine)                        (TerminalBreakUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTerminalBreakProcInfo, GetCurrentArchitecture())
    #define NewTerminalCacheUPP(userRoutine)                        (TerminalCacheUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTerminalCacheProcInfo, GetCurrentArchitecture())
    #define NewTerminalSearchCallBackUPP(userRoutine)               (TerminalSearchCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTerminalSearchCallBackProcInfo, GetCurrentArchitecture())
    #define NewTerminalClikLoopUPP(userRoutine)                     (TerminalClikLoopUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTerminalClikLoopProcInfo, GetCurrentArchitecture())
    #define NewTerminalEnvironsUPP(userRoutine)                     (TerminalEnvironsUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTerminalEnvironsProcInfo, GetCurrentArchitecture())
    #define NewTerminalChooseIdleUPP(userRoutine)                   (TerminalChooseIdleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTerminalChooseIdleProcInfo, GetCurrentArchitecture())
    #define NewTerminalToolDefUPP(userRoutine)                      (TerminalToolDefUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTerminalToolDefProcInfo, GetCurrentArchitecture())
    #define DisposeTerminalSendUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeTerminalBreakUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeTerminalCacheUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeTerminalSearchCallBackUPP(userUPP)               DisposeRoutineDescriptor(userUPP)
    #define DisposeTerminalClikLoopUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeTerminalEnvironsUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeTerminalChooseIdleUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define DisposeTerminalToolDefUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeTerminalSendUPP(thePtr, theSize, refCon, flags, userUPP)  (long)CALL_FOUR_PARAMETER_UPP((userUPP), uppTerminalSendProcInfo, (thePtr), (theSize), (refCon), (flags))
    #define InvokeTerminalBreakUPP(duration, refCon, userUPP)       CALL_TWO_PARAMETER_UPP((userUPP), uppTerminalBreakProcInfo, (duration), (refCon))
    #define InvokeTerminalCacheUPP(refCon, theTermData, userUPP)    (long)CALL_TWO_PARAMETER_UPP((userUPP), uppTerminalCacheProcInfo, (refCon), (theTermData))
    #define InvokeTerminalSearchCallBackUPP(hTerm, refNum, foundRect, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppTerminalSearchCallBackProcInfo, (hTerm), (refNum), (foundRect))
    #define InvokeTerminalClikLoopUPP(refCon, userUPP)              (Boolean)CALL_ONE_PARAMETER_UPP((userUPP), uppTerminalClikLoopProcInfo, (refCon))
    #define InvokeTerminalEnvironsUPP(refCon, theEnvirons, userUPP)  (CMErr)CALL_TWO_PARAMETER_UPP((userUPP), uppTerminalEnvironsProcInfo, (refCon), (theEnvirons))
    #define InvokeTerminalChooseIdleUPP(userUPP)                    CALL_ZERO_PARAMETER_UPP((userUPP), uppTerminalChooseIdleProcInfo)
    #define InvokeTerminalToolDefUPP(hTerm, msg, p1, p2, p3, userUPP)  (long)CALL_FIVE_PARAMETER_UPP((userUPP), uppTerminalToolDefProcInfo, (hTerm), (msg), (p1), (p2), (p3))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewTerminalSendProc(userRoutine)                        NewTerminalSendUPP(userRoutine)
#define NewTerminalBreakProc(userRoutine)                       NewTerminalBreakUPP(userRoutine)
#define NewTerminalCacheProc(userRoutine)                       NewTerminalCacheUPP(userRoutine)
#define NewTerminalSearchCallBackProc(userRoutine)              NewTerminalSearchCallBackUPP(userRoutine)
#define NewTerminalClikLoopProc(userRoutine)                    NewTerminalClikLoopUPP(userRoutine)
#define NewTerminalEnvironsProc(userRoutine)                    NewTerminalEnvironsUPP(userRoutine)
#define NewTerminalChooseIdleProc(userRoutine)                  NewTerminalChooseIdleUPP(userRoutine)
#define NewTerminalToolDefProc(userRoutine)                     NewTerminalToolDefUPP(userRoutine)
#define CallTerminalSendProc(userRoutine, thePtr, theSize, refCon, flags) InvokeTerminalSendUPP(thePtr, theSize, refCon, flags, userRoutine)
#define CallTerminalBreakProc(userRoutine, duration, refCon)    InvokeTerminalBreakUPP(duration, refCon, userRoutine)
#define CallTerminalCacheProc(userRoutine, refCon, theTermData) InvokeTerminalCacheUPP(refCon, theTermData, userRoutine)
#define CallTerminalSearchCallBackProc(userRoutine, hTerm, refNum, foundRect) InvokeTerminalSearchCallBackUPP(hTerm, refNum, foundRect, userRoutine)
#define CallTerminalClikLoopProc(userRoutine, refCon)           InvokeTerminalClikLoopUPP(refCon, userRoutine)
#define CallTerminalEnvironsProc(userRoutine, refCon, theEnvirons) InvokeTerminalEnvironsUPP(refCon, theEnvirons, userRoutine)
#define CallTerminalChooseIdleProc(userRoutine)                 InvokeTerminalChooseIdleUPP(userRoutine)
#define CallTerminalToolDefProc(userRoutine, hTerm, msg, p1, p2, p3) InvokeTerminalToolDefUPP(hTerm, msg, p1, p2, p3, userRoutine)

#if CALL_NOT_IN_CARBON
EXTERN_API( TMErr )
InitTM                          (void);

EXTERN_API( Handle )
TMGetVersion                    (TermHandle             hTerm);

EXTERN_API( short )
TMGetTMVersion                  (void);

EXTERN_API( TermHandle )
TMNew                           (const Rect *           termRect,
                                 const Rect *           viewRect,
                                 TMFlags                flags,
                                 short                  procID,
                                 WindowPtr              owner,
                                 TerminalSendUPP        sendProc,
                                 TerminalCacheUPP       cacheProc,
                                 TerminalBreakUPP       breakProc,
                                 TerminalClikLoopUPP    clikLoop,
                                 TerminalEnvironsUPP    environsProc,
                                 long                   refCon,
                                 long                   userData);

EXTERN_API( void )
TMDispose                       (TermHandle             hTerm);

EXTERN_API( void )
TMKey                           (TermHandle             hTerm,
                                 const EventRecord *    theEvent);

EXTERN_API( void )
TMUpdate                        (TermHandle             hTerm,
                                 RgnHandle              visRgn);

EXTERN_API( void )
TMPaint                         (TermHandle             hTerm,
                                 const TermDataBlock *  theTermData,
                                 const Rect *           theRect);

EXTERN_API( void )
TMActivate                      (TermHandle             hTerm,
                                 Boolean                activate);

EXTERN_API( void )
TMResume                        (TermHandle             hTerm,
                                 Boolean                resume);

EXTERN_API( void )
TMClick                         (TermHandle             hTerm,
                                 const EventRecord *    theEvent);

EXTERN_API( void )
TMIdle                          (TermHandle             hTerm);

EXTERN_API( long )
TMStream                        (TermHandle             hTerm,
                                 void *                 theBuffer,
                                 long                   theLength,
                                 CMFlags                flags);

EXTERN_API( Boolean )
TMMenu                          (TermHandle             hTerm,
                                 short                  menuID,
                                 short                  item);

EXTERN_API( void )
TMReset                         (TermHandle             hTerm);

EXTERN_API( void )
TMClear                         (TermHandle             hTerm);

EXTERN_API( void )
TMResize                        (TermHandle             hTerm,
                                 const Rect *           newViewRect);

EXTERN_API( long )
TMGetSelect                     (TermHandle             hTerm,
                                 Handle                 theData,
                                 ResType *              theType);

EXTERN_API( void )
TMGetLine                       (TermHandle             hTerm,
                                 short                  lineNo,
                                 TermDataBlock *        theTermData);

EXTERN_API( void )
TMSetSelection                  (TermHandle             hTerm,
                                 const TMSelection *    theSelection,
                                 TMSelTypes             selType);

EXTERN_API( void )
TMScroll                        (TermHandle             hTerm,
                                 short                  dh,
                                 short                  dv);

EXTERN_API( Boolean )
TMValidate                      (TermHandle             hTerm);

EXTERN_API( void )
TMDefault                       (Ptr *                  theConfig,
                                 short                  procID,
                                 Boolean                allocate);

EXTERN_API( Handle )
TMSetupPreflight                (short                  procID,
                                 long *                 magicCookie);

EXTERN_API( void )
TMSetupSetup                    (short                  procID,
                                 const void *           theConfig,
                                 short                  count,
                                 DialogPtr              theDialog,
                                 long *                 magicCookie);

EXTERN_API( Boolean )
TMSetupFilter                   (short                  procID,
                                 const void *           theConfig,
                                 short                  count,
                                 DialogPtr              theDialog,
                                 EventRecord *          theEvent,
                                 short *                theItem,
                                 long *                 magicCookie);

EXTERN_API( void )
TMSetupItem                     (short                  procID,
                                 const void *           theConfig,
                                 short                  count,
                                 DialogPtr              theDialog,
                                 short *                theItem,
                                 long *                 magicCookie);

EXTERN_API( void )
TMSetupXCleanup                 (short                  procID,
                                 const void *           theConfig,
                                 short                  count,
                                 DialogPtr              theDialog,
                                 Boolean                OKed,
                                 long *                 magicCookie);

EXTERN_API( void )
TMSetupPostflight               (short                  procID);

EXTERN_API( Ptr )
TMGetConfig                     (TermHandle             hTerm);

EXTERN_API( short )
TMSetConfig                     (TermHandle             hTerm,
                                 const void *           thePtr);

EXTERN_API( OSErr )
TMIntlToEnglish                 (TermHandle             hTerm,
                                 const void *           inputPtr,
                                 Ptr *                  outputPtr,
                                 short                  language);

EXTERN_API( OSErr )
TMEnglishToIntl                 (TermHandle             hTerm,
                                 const void *           inputPtr,
                                 Ptr *                  outputPtr,
                                 short                  language);

EXTERN_API( void )
TMGetToolName                   (short                  id,
                                 Str255                 name);

EXTERN_API( short )
TMGetProcID                     (ConstStr255Param       name);

EXTERN_API( void )
TMSetRefCon                     (TermHandle             hTerm,
                                 long                   refCon);

EXTERN_API( long )
TMGetRefCon                     (TermHandle             hTerm);

EXTERN_API( void )
TMSetUserData                   (TermHandle             hTerm,
                                 long                   userData);

EXTERN_API( long )
TMGetUserData                   (TermHandle             hTerm);

EXTERN_API( short )
TMAddSearch                     (TermHandle             hTerm,
                                 ConstStr255Param       theString,
                                 const Rect *           where,
                                 TMSearchTypes          searchType,
                                 TerminalSearchCallBackUPP  callBack);

EXTERN_API( void )
TMRemoveSearch                  (TermHandle             hTerm,
                                 short                  refnum);

EXTERN_API( void )
TMClearSearch                   (TermHandle             hTerm);

EXTERN_API( Point )
TMGetCursor                     (TermHandle             hTerm,
                                 TMCursorTypes          cursType);

EXTERN_API( TMErr )
TMGetTermEnvirons               (TermHandle             hTerm,
                                 TermEnvironRec *       theEnvirons);

EXTERN_API( short )
TMChoose                        (TermHandle *           hTerm,
                                 Point                  where,
                                 TerminalChooseIdleUPP  idleProc);

EXTERN_API( void )
TMEvent                         (TermHandle             hTerm,
                                 const EventRecord *    theEvent);

EXTERN_API( Boolean )
TMDoTermKey                     (TermHandle             hTerm,
                                 ConstStr255Param       theKey);

EXTERN_API( short )
TMCountTermKeys                 (TermHandle             hTerm);

EXTERN_API( void )
TMGetIndTermKey                 (TermHandle             hTerm,
                                 short                  id,
                                 Str255                 theKey);

EXTERN_API( void )
TMGetErrorString                (TermHandle             hTerm,
                                 short                  id,
                                 Str255                 errMsg);

#endif  /* CALL_NOT_IN_CARBON */

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

#endif /* __TERMINALS__ */

