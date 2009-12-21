/*
     File:       Connections.h
 
     Contains:   Communications Toolbox Connection Manager Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1988-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CONNECTIONS__
#define __CONNECTIONS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
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
    curCMVersion                = 2                             /* current Connection Manager version*/
};

enum {
    curConnEnvRecVers           = 0                             /*    current Connection Manager Environment Record version*/
};

enum {
                                                                /* CMErr */
    cmGenericError              = -1,
    cmNoErr                     = 0,
    cmRejected                  = 1,
    cmFailed                    = 2,
    cmTimeOut                   = 3,
    cmNotOpen                   = 4,
    cmNotClosed                 = 5,
    cmNoRequestPending          = 6,
    cmNotSupported              = 7,
    cmNoTools                   = 8,
    cmUserCancel                = 9,
    cmUnknownError              = 11
};

typedef OSErr                           CMErr;

enum {
    cmData                      = 1L << 0,
    cmCntl                      = 1L << 1,
    cmAttn                      = 1L << 2,
    cmDataNoTimeout             = 1L << 4,
    cmCntlNoTimeout             = 1L << 5,
    cmAttnNoTimeout             = 1L << 6,
    cmDataClean                 = 1L << 8,
    cmCntlClean                 = 1L << 9,
    cmAttnClean                 = 1L << 10,                     /*       Only for CMRecFlags (not CMChannel) in the rest of this enum  */
    cmNoMenus                   = 1L << 16,
    cmQuiet                     = 1L << 17,
    cmConfigChanged             = 1L << 18
};

/* CMRecFlags and CMChannel     */
/*      Low word of CMRecFlags is same as CMChannel */
typedef long                            CMRecFlags;
typedef short                           CMChannel;


enum {
    cmStatusOpening             = 1L << 0,
    cmStatusOpen                = 1L << 1,
    cmStatusClosing             = 1L << 2,
    cmStatusDataAvail           = 1L << 3,
    cmStatusCntlAvail           = 1L << 4,
    cmStatusAttnAvail           = 1L << 5,
    cmStatusDRPend              = 1L << 6,                      /* data read pending  */
    cmStatusDWPend              = 1L << 7,                      /* data write pending */
    cmStatusCRPend              = 1L << 8,                      /* cntl read pending  */
    cmStatusCWPend              = 1L << 9,                      /* cntl write pending */
    cmStatusARPend              = 1L << 10,                     /* attn read pending  */
    cmStatusAWPend              = 1L << 11,                     /* attn write pending */
    cmStatusBreakPend           = 1L << 12,
    cmStatusListenPend          = 1L << 13,
    cmStatusIncomingCallPresent = 1L << 14,
    cmStatusReserved0           = 1L << 15
};

typedef unsigned long                   CMStatFlags;
enum {
    cmDataIn                    = 0,
    cmDataOut                   = 1,
    cmCntlIn                    = 2,
    cmCntlOut                   = 3,
    cmAttnIn                    = 4,
    cmAttnOut                   = 5,
    cmRsrvIn                    = 6,
    cmRsrvOut                   = 7
};

typedef unsigned short                  CMBufFields;
typedef Ptr                             CMBuffers[8];
typedef long                            CMBufferSizes[8];
typedef const long *                    ConstCMBufferSizesParam;
enum {
    cmSearchSevenBit            = 1L << 0
};

typedef unsigned short                  CMSearchFlags;
enum {
    cmFlagsEOM                  = 1L << 0
};

typedef unsigned short                  CMFlags;

struct ConnEnvironRec {
    short                           version;
    long                            baudRate;
    short                           dataBits;
    CMChannel                       channels;
    Boolean                         swFlowControl;
    Boolean                         hwFlowControl;
    CMFlags                         flags;
};
typedef struct ConnEnvironRec           ConnEnvironRec;
typedef ConnEnvironRec *                ConnEnvironRecPtr;
typedef struct ConnRecord               ConnRecord;

typedef ConnRecord *                    ConnPtr;
typedef ConnPtr *                       ConnHandle;
typedef CALLBACK_API( long , ConnectionToolDefProcPtr )(ConnHandle hConn, short msg, long p1, long p2, long p3);
typedef CALLBACK_API( void , ConnectionSearchCallBackProcPtr )(ConnHandle hConn, Ptr matchPtr, long refNum);
typedef CALLBACK_API( void , ConnectionCompletionProcPtr )(ConnHandle hConn);
typedef CALLBACK_API( void , ConnectionChooseIdleProcPtr )(void );
typedef STACK_UPP_TYPE(ConnectionToolDefProcPtr)                ConnectionToolDefUPP;
typedef STACK_UPP_TYPE(ConnectionSearchCallBackProcPtr)         ConnectionSearchCallBackUPP;
typedef STACK_UPP_TYPE(ConnectionCompletionProcPtr)             ConnectionCompletionUPP;
typedef STACK_UPP_TYPE(ConnectionChooseIdleProcPtr)             ConnectionChooseIdleUPP;

struct ConnRecord {
    short                           procID;
    CMRecFlags                      flags;
    CMErr                           errCode;
    long                            refCon;
    long                            userData;
    ConnectionToolDefUPP            defProc;
    Ptr                             config;
    Ptr                             oldConfig;
    long                            asyncEOM;
    long                            reserved1;
    long                            reserved2;
    Ptr                             cmPrivate;
    CMBuffers                       bufferArray;
    CMBufferSizes                   bufSizes;
    long                            mluField;
    CMBufferSizes                   asyncCount;
};

enum {
                                                                /* CMIOPB constants and structure */
    cmIOPBQType                 = 10,
    cmIOPBversion               = 0
};


struct CMIOPB {
    QElemPtr                        qLink;
    short                           qType;                      /* cmIOPBQType */
    ConnHandle                      hConn;
    Ptr                             theBuffer;
    long                            count;
    CMFlags                         flags;
    ConnectionCompletionUPP         userCompletion;
    long                            timeout;
    CMErr                           errCode;
    CMChannel                       channel;
    long                            asyncEOM;
    long                            reserved1;
    short                           reserved2;
    short                           version;                    /* cmIOPBversion */
    long                            refCon;                     /* for application */
    long                            toolData1;                  /* for tool */
    long                            toolData2;                  /* for tool */
};
typedef struct CMIOPB                   CMIOPB;
typedef CMIOPB *                        CMIOPBPtr;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ConnectionToolDefUPP)
    NewConnectionToolDefUPP        (ConnectionToolDefProcPtr userRoutine);

    EXTERN_API(ConnectionSearchCallBackUPP)
    NewConnectionSearchCallBackUPP    (ConnectionSearchCallBackProcPtr userRoutine);

    EXTERN_API(ConnectionCompletionUPP)
    NewConnectionCompletionUPP     (ConnectionCompletionProcPtr userRoutine);

    EXTERN_API(ConnectionChooseIdleUPP)
    NewConnectionChooseIdleUPP     (ConnectionChooseIdleProcPtr userRoutine);

    EXTERN_API(void)
    DisposeConnectionToolDefUPP    (ConnectionToolDefUPP    userUPP);

    EXTERN_API(void)
    DisposeConnectionSearchCallBackUPP    (ConnectionSearchCallBackUPP userUPP);

    EXTERN_API(void)
    DisposeConnectionCompletionUPP    (ConnectionCompletionUPP userUPP);

    EXTERN_API(void)
    DisposeConnectionChooseIdleUPP    (ConnectionChooseIdleUPP userUPP);

    EXTERN_API(long)
    InvokeConnectionToolDefUPP     (ConnHandle              hConn,
                                    short                   msg,
                                    long                    p1,
                                    long                    p2,
                                    long                    p3,
                                    ConnectionToolDefUPP    userUPP);

    EXTERN_API(void)
    InvokeConnectionSearchCallBackUPP    (ConnHandle        hConn,
                                    Ptr                     matchPtr,
                                    long                    refNum,
                                    ConnectionSearchCallBackUPP userUPP);

    EXTERN_API(void)
    InvokeConnectionCompletionUPP    (ConnHandle            hConn,
                                    ConnectionCompletionUPP userUPP);

    EXTERN_API(void)
    InvokeConnectionChooseIdleUPP    (ConnectionChooseIdleUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppConnectionToolDefProcInfo = 0x0000FEF0 };             /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppConnectionSearchCallBackProcInfo = 0x00000FC0 };      /* pascal no_return_value Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppConnectionCompletionProcInfo = 0x000000C0 };          /* pascal no_return_value Func(4_bytes) */
    enum { uppConnectionChooseIdleProcInfo = 0x00000000 };          /* pascal no_return_value Func() */
    #define NewConnectionToolDefUPP(userRoutine)                    (ConnectionToolDefUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppConnectionToolDefProcInfo, GetCurrentArchitecture())
    #define NewConnectionSearchCallBackUPP(userRoutine)             (ConnectionSearchCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppConnectionSearchCallBackProcInfo, GetCurrentArchitecture())
    #define NewConnectionCompletionUPP(userRoutine)                 (ConnectionCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppConnectionCompletionProcInfo, GetCurrentArchitecture())
    #define NewConnectionChooseIdleUPP(userRoutine)                 (ConnectionChooseIdleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppConnectionChooseIdleProcInfo, GetCurrentArchitecture())
    #define DisposeConnectionToolDefUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeConnectionSearchCallBackUPP(userUPP)             DisposeRoutineDescriptor(userUPP)
    #define DisposeConnectionCompletionUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define DisposeConnectionChooseIdleUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define InvokeConnectionToolDefUPP(hConn, msg, p1, p2, p3, userUPP)  (long)CALL_FIVE_PARAMETER_UPP((userUPP), uppConnectionToolDefProcInfo, (hConn), (msg), (p1), (p2), (p3))
    #define InvokeConnectionSearchCallBackUPP(hConn, matchPtr, refNum, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppConnectionSearchCallBackProcInfo, (hConn), (matchPtr), (refNum))
    #define InvokeConnectionCompletionUPP(hConn, userUPP)           CALL_ONE_PARAMETER_UPP((userUPP), uppConnectionCompletionProcInfo, (hConn))
    #define InvokeConnectionChooseIdleUPP(userUPP)                  CALL_ZERO_PARAMETER_UPP((userUPP), uppConnectionChooseIdleProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewConnectionToolDefProc(userRoutine)                   NewConnectionToolDefUPP(userRoutine)
#define NewConnectionSearchCallBackProc(userRoutine)            NewConnectionSearchCallBackUPP(userRoutine)
#define NewConnectionCompletionProc(userRoutine)                NewConnectionCompletionUPP(userRoutine)
#define NewConnectionChooseIdleProc(userRoutine)                NewConnectionChooseIdleUPP(userRoutine)
#define CallConnectionToolDefProc(userRoutine, hConn, msg, p1, p2, p3) InvokeConnectionToolDefUPP(hConn, msg, p1, p2, p3, userRoutine)
#define CallConnectionSearchCallBackProc(userRoutine, hConn, matchPtr, refNum) InvokeConnectionSearchCallBackUPP(hConn, matchPtr, refNum, userRoutine)
#define CallConnectionCompletionProc(userRoutine, hConn)        InvokeConnectionCompletionUPP(hConn, userRoutine)
#define CallConnectionChooseIdleProc(userRoutine)               InvokeConnectionChooseIdleUPP(userRoutine)


#if CALL_NOT_IN_CARBON
EXTERN_API( CMErr )
InitCM                          (void);

EXTERN_API( Handle )
CMGetVersion                    (ConnHandle             hConn);

EXTERN_API( short )
CMGetCMVersion                  (void);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API( ConnHandle )
CMNew                           (short                  procID,
                                 CMRecFlags             flags,
                                 ConstCMBufferSizesParam  desiredSizes,
                                 long                   refCon,
                                 long                   userData);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API( void )
CMDispose                       (ConnHandle             hConn);

EXTERN_API( CMErr )
CMListen                        (ConnHandle             hConn,
                                 Boolean                async,
                                 ConnectionCompletionUPP  completor,
                                 long                   timeout);

EXTERN_API( CMErr )
CMAccept                        (ConnHandle             hConn,
                                 Boolean                accept);

EXTERN_API( CMErr )
CMOpen                          (ConnHandle             hConn,
                                 Boolean                async,
                                 ConnectionCompletionUPP  completor,
                                 long                   timeout);

EXTERN_API( CMErr )
CMClose                         (ConnHandle             hConn,
                                 Boolean                async,
                                 ConnectionCompletionUPP  completor,
                                 long                   timeout,
                                 Boolean                now);

EXTERN_API( CMErr )
CMAbort                         (ConnHandle             hConn);

EXTERN_API( CMErr )
CMStatus                        (ConnHandle             hConn,
                                 CMBufferSizes          sizes,
                                 CMStatFlags *          flags);

EXTERN_API( void )
CMIdle                          (ConnHandle             hConn);

EXTERN_API( void )
CMReset                         (ConnHandle             hConn);

EXTERN_API( void )
CMBreak                         (ConnHandle             hConn,
                                 long                   duration,
                                 Boolean                async,
                                 ConnectionCompletionUPP  completor);

EXTERN_API( CMErr )
CMRead                          (ConnHandle             hConn,
                                 void *                 theBuffer,
                                 long *                 toRead,
                                 CMChannel              theChannel,
                                 Boolean                async,
                                 ConnectionCompletionUPP  completor,
                                 long                   timeout,
                                 CMFlags *              flags);

EXTERN_API( CMErr )
CMWrite                         (ConnHandle             hConn,
                                 const void *           theBuffer,
                                 long *                 toWrite,
                                 CMChannel              theChannel,
                                 Boolean                async,
                                 ConnectionCompletionUPP  completor,
                                 long                   timeout,
                                 CMFlags                flags);

EXTERN_API( CMErr )
CMIOKill                        (ConnHandle             hConn,
                                 short                  which);

EXTERN_API( void )
CMActivate                      (ConnHandle             hConn,
                                 Boolean                activate);

EXTERN_API( void )
CMResume                        (ConnHandle             hConn,
                                 Boolean                resume);

EXTERN_API( Boolean )
CMMenu                          (ConnHandle             hConn,
                                 short                  menuID,
                                 short                  item);

EXTERN_API( Boolean )
CMValidate                      (ConnHandle             hConn);

EXTERN_API( void )
CMDefault                       (Ptr *                  theConfig,
                                 short                  procID,
                                 Boolean                allocate);

EXTERN_API( Handle )
CMSetupPreflight                (short                  procID,
                                 long *                 magicCookie);

EXTERN_API( Boolean )
CMSetupFilter                   (short                  procID,
                                 const void *           theConfig,
                                 short                  count,
                                 DialogPtr              theDialog,
                                 EventRecord *          theEvent,
                                 short *                theItem,
                                 long *                 magicCookie);

EXTERN_API( void )
CMSetupSetup                    (short                  procID,
                                 const void *           theConfig,
                                 short                  count,
                                 DialogPtr              theDialog,
                                 long *                 magicCookie);

EXTERN_API( void )
CMSetupItem                     (short                  procID,
                                 const void *           theConfig,
                                 short                  count,
                                 DialogPtr              theDialog,
                                 short *                theItem,
                                 long *                 magicCookie);

EXTERN_API( void )
CMSetupXCleanup                 (short                  procID,
                                 const void *           theConfig,
                                 short                  count,
                                 DialogPtr              theDialog,
                                 Boolean                OKed,
                                 long *                 magicCookie);

EXTERN_API( void )
CMSetupPostflight               (short                  procID);

EXTERN_API( Ptr )
CMGetConfig                     (ConnHandle             hConn);

EXTERN_API( short )
CMSetConfig                     (ConnHandle             hConn,
                                 const void *           thePtr);

EXTERN_API( OSErr )
CMIntlToEnglish                 (ConnHandle             hConn,
                                 const void *           inputPtr,
                                 Ptr *                  outputPtr,
                                 short                  language);

EXTERN_API( OSErr )
CMEnglishToIntl                 (ConnHandle             hConn,
                                 const void *           inputPtr,
                                 Ptr *                  outputPtr,
                                 short                  language);

EXTERN_API( long )
CMAddSearch                     (ConnHandle             hConn,
                                 ConstStr255Param       theString,
                                 CMSearchFlags          flags,
                                 ConnectionSearchCallBackUPP  callBack);

EXTERN_API( void )
CMRemoveSearch                  (ConnHandle             hConn,
                                 long                   refnum);

EXTERN_API( void )
CMClearSearch                   (ConnHandle             hConn);

EXTERN_API( CMErr )
CMGetConnEnvirons               (ConnHandle             hConn,
                                 ConnEnvironRec *       theEnvirons);

EXTERN_API( short )
CMChoose                        (ConnHandle *           hConn,
                                 Point                  where,
                                 ConnectionChooseIdleUPP  idle);

EXTERN_API( void )
CMEvent                         (ConnHandle             hConn,
                                 const EventRecord *    theEvent);

EXTERN_API( void )
CMGetToolName                   (short                  procID,
                                 Str255                 name);

EXTERN_API( short )
CMGetProcID                     (ConstStr255Param       name);

EXTERN_API( void )
CMSetRefCon                     (ConnHandle             hConn,
                                 long                   refCon);

EXTERN_API( long )
CMGetRefCon                     (ConnHandle             hConn);

EXTERN_API( long )
CMGetUserData                   (ConnHandle             hConn);

EXTERN_API( void )
CMSetUserData                   (ConnHandle             hConn,
                                 long                   userData);

EXTERN_API( void )
CMGetErrorString                (ConnHandle             hConn,
                                 short                  id,
                                 Str255                 errMsg);

EXTERN_API( CMErr )
CMNewIOPB                       (ConnHandle             hConn,
                                 CMIOPBPtr *            theIOPB);

EXTERN_API( CMErr )
CMDisposeIOPB                   (ConnHandle             hConn,
                                 CMIOPBPtr              theIOPB);

EXTERN_API( CMErr )
CMPBRead                        (ConnHandle             hConn,
                                 CMIOPBPtr              theIOPB,
                                 Boolean                async);

EXTERN_API( CMErr )
CMPBWrite                       (ConnHandle             hConn,
                                 CMIOPBPtr              theIOPB,
                                 Boolean                async);

EXTERN_API( CMErr )
CMPBIOKill                      (ConnHandle             hConn,
                                 CMIOPBPtr              theIOPB);

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

#endif /* __CONNECTIONS__ */

