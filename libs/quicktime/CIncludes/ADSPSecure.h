/*
     File:       ADSPSecure.h
 
     Contains:   Secure AppleTalk Data Stream Protocol Interfaces.
 
     Version:    Technology: AOCE Toolbox 1.02
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ADSPSECURE__
#define __ADSPSECURE__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __ADSP__
#include <ADSP.h>
#endif

#ifndef __OCEAUTHDIR__
#include <OCEAuthDir.h>
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

/* New ADSP control codes

 * open a secure connection */
enum {
    sdspOpen                    = 229
};

/*
For secure connections, the eom field of ioParams contains two single-bit flags
(instead of a zero/non-zero byte). They are an encrypt flag (see below), and an
eom flag.  All other bits in that field should be zero.

To write an encrypted message, you must set an encrypt bit in the eom field of
the ioParams of your write call. Note: this flag is only checked on the first
write of a message (the first write on a connection, or the first write following
a write with eom set.
*/
enum {
    dspEOMBit                   = 0,                            /* set if EOM at end of write */
    dspEncryptBit               = 1                             /* set to encrypt message */
};

enum {
    dspEOMMask                  = 1 << dspEOMBit,
    dspEncryptMask              = 1 << dspEncryptBit
};


/*
Define an ADSPSecure parameter block, as used for the secure Open call.

 * size of ADSPSecure workspace */
enum {
    sdspWorkSize                = 2048
};


struct TRSecureParams {
    unsigned short                  localCID;                   /* local connection id */
    unsigned short                  remoteCID;                  /* remote connection id */
    AddrBlock                       remoteAddress;              /* address of remote end */
    AddrBlock                       filterAddress;              /* address filter */
    unsigned long                   sendSeq;                    /* local send sequence number */
    unsigned short                  sendWindow;                 /* send window size */
    unsigned long                   recvSeq;                    /* receive sequence number */
    unsigned long                   attnSendSeq;                /* attention send sequence number */
    unsigned long                   attnRecvSeq;                /* attention receive sequence number */
    unsigned char                   ocMode;                     /* open connection mode */
    unsigned char                   ocInterval;                 /* open connection request retry interval */
    unsigned char                   ocMaximum;                  /* open connection request retry maximum */

    Boolean                         secure;                     /*  --> TRUE if session was authenticated */
    AuthKeyPtr                      sessionKey;                 /* <--> encryption key for session */
    unsigned long                   credentialsSize;            /*  --> length of credentials */
    void *                          credentials;                /*  --> pointer to credentials */
    void *                          workspace;                  /*  --> pointer to workspace for connection align on even boundary and length = sdspWorkSize */
    AuthIdentity                    recipient;                  /*  --> identity of recipient (or initiator if active mode */
    UTCTime                         issueTime;                  /*  --> when credentials were issued */
    UTCTime                         expiry;                     /*  --> when credentials expiry */
    RecordIDPtr                     initiator;                  /* <--  RecordID of initiator returned here. Must give appropriate Buffer to hold RecordID (Only for passive or accept mode) */
    Boolean                         hasIntermediary;            /* <--  will be set if credentials has an intermediary */
    Boolean                         filler1;
    RecordIDPtr                     intermediary;               /* <--  RecordID of intermediary returned here. (If intermediary is found in credentials Must give appropriate Buffer to hold RecordID (Only for passive or accept mode) */
};
typedef struct TRSecureParams           TRSecureParams;
typedef struct SDSPParamBlock           SDSPParamBlock;

typedef SDSPParamBlock *                SDSPPBPtr;
typedef CALLBACK_API( void , SDSPIOCompletionProcPtr )(SDSPPBPtr paramBlock);
/*
    WARNING: SDSPIOCompletionProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(SDSPIOCompletionProcPtr)              SDSPIOCompletionUPP;

struct SDSPParamBlock {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    SDSPIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    StringPtr                       ioNamePtr;
    short                           ioVRefNum;
    short                           ioCRefNum;                  /* adsp driver refNum */
    short                           csCode;                     /* adsp driver control code */
    long                            qStatus;                    /* adsp internal use */
    short                           ccbRefNum;
    union {
        TRinitParams                    initParams;             /* dspInit, dspCLInit */
        TRopenParams                    openParams;             /* dspOpen, dspCLListen, dspCLDeny */
        TRcloseParams                   closeParams;            /*dspClose, dspRemove*/
        TRioParams                      ioParams;               /*dspRead, dspWrite*/
        TRattnParams                    attnParams;             /*dspAttention*/
        TRstatusParams                  statusParams;           /*dspStatus*/
        TRoptionParams                  optionParams;           /*dspOptions*/
        TRnewcidParams                  newCIDParams;           /*dspNewCID*/
        TRSecureParams                  secureParams;           /* dspOpenSecure */
    }                                 u;
};

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(SDSPIOCompletionUPP)
    NewSDSPIOCompletionUPP         (SDSPIOCompletionProcPtr userRoutine);

    EXTERN_API(void)
    DisposeSDSPIOCompletionUPP     (SDSPIOCompletionUPP     userUPP);

    EXTERN_API(void)
    InvokeSDSPIOCompletionUPP      (SDSPPBPtr               paramBlock,
                                    SDSPIOCompletionUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppSDSPIOCompletionProcInfo = 0x00009802 };              /* register no_return_value Func(4_bytes:A0) */
    #define NewSDSPIOCompletionUPP(userRoutine)                     (SDSPIOCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSDSPIOCompletionProcInfo, GetCurrentArchitecture())
    #define DisposeSDSPIOCompletionUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeSDSPIOCompletionUPP(__A0, __A1)
    void InvokeSDSPIOCompletionUPP(SDSPPBPtr paramBlock, SDSPIOCompletionUPP userUPP) = 0x4E91;
    #else
        #define InvokeSDSPIOCompletionUPP(paramBlock, userUPP)          CALL_ONE_PARAMETER_UPP((userUPP), uppSDSPIOCompletionProcInfo, (paramBlock))
    #endif
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewSDSPIOCompletionProc(userRoutine)                    NewSDSPIOCompletionUPP(userRoutine)
#define CallSDSPIOCompletionProc(userRoutine, paramBlock)       InvokeSDSPIOCompletionUPP(paramBlock, userRoutine)


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

#endif /* __ADSPSECURE__ */

