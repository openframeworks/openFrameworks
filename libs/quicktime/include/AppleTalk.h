/*
     File:       AppleTalk.h
 
     Contains:   AppleTalk Interfaces.
 
     Version:    Technology: System 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __APPLETALK__
#define __APPLETALK__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __OSUTILS__
#include <OSUtils.h>
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

enum {
                                                                /* Driver unit numbers (ADSP is dynamic) */
    mppUnitNum                  = 9,                            /* MPP unit number */
    atpUnitNum                  = 10,                           /* ATP unit number */
    xppUnitNum                  = 40                            /* XPP unit number */
};

enum {
                                                                /* Driver refNums (ADSP is dynamic) */
    mppRefNum                   = -10,                          /* MPP reference number */
    atpRefNum                   = -11,                          /* ATP reference number */
    xppRefNum                   = -41                           /* XPP reference number */
};

enum {
                                                                /* .MPP csCodes */
    lookupReply                 = 242,                          /* This command queued to ourself */
    writeLAP                    = 243,                          /* Write out LAP packet */
    detachPH                    = 244,                          /* Detach LAP protocol handler */
    attachPH                    = 245,                          /* Attach LAP protocol handler */
    writeDDP                    = 246,                          /* Write out DDP packet */
    closeSkt                    = 247,                          /* Close DDP socket */
    openSkt                     = 248,                          /* Open DDP socket */
    loadNBP                     = 249,                          /* Load NBP command-executing code */
    lastResident                = 249,                          /* Last resident command */
    confirmName                 = 250,                          /* Confirm name */
    lookupName                  = 251,                          /* Look up name on internet */
    removeName                  = 252,                          /* Remove name from Names Table */
    registerName                = 253,                          /* Register name in Names Table */
    killNBP                     = 254,                          /* Kill outstanding NBP request */
    unloadNBP                   = 255,                          /* Unload NBP command code */
    setSelfSend                 = 256,                          /* MPP: Set to allow writes to self */
    SetMyZone                   = 257,                          /* Set my zone name */
    GetATalkInfo                = 258,                          /* get AppleTalk information */
    ATalkClosePrep              = 259                           /* AppleTalk close query */
};

enum {
                                                                /* .ATP csCodes */
    nSendRequest                = 248,                          /* NSendRequest code */
    relRspCB                    = 249,                          /* Release RspCB */
    closeATPSkt                 = 250,                          /* Close ATP socket */
    addResponse                 = 251,                          /* Add response code | Require open skt */
    sendResponse                = 252,                          /* Send response code */
    getRequest                  = 253,                          /* Get request code */
    openATPSkt                  = 254,                          /* Open ATP socket */
    sendRequest                 = 255,                          /* Send request code */
    relTCB                      = 256,                          /* Release TCB */
    killGetReq                  = 257,                          /* Kill GetRequest */
    killSendReq                 = 258,                          /* Kill SendRequest */
    killAllGetReq               = 259                           /* Kill all getRequests for a skt */
};

enum {
                                                                /* .XPP csCodes */
    openSess                    = 255,                          /* Open session */
    closeSess                   = 254,                          /* Close session */
    userCommand                 = 253,                          /* User command */
    userWrite                   = 252,                          /* User write */
    getStatus                   = 251,                          /* Get status */
    afpCall                     = 250,                          /* AFP command (buffer has command code) */
    getParms                    = 249,                          /* Get parameters */
    abortOS                     = 248,                          /* Abort open session request */
    closeAll                    = 247,                          /* Close all open sessions */
    xCall                       = 246                           /* .XPP extended calls */
};

enum {
                                                                /* Transition Queue transition types */
    ATTransOpen                 = 0,                            /*AppleTalk has opened*/
    ATTransClose                = 2,                            /*AppleTalk is about to close*/
    ATTransClosePrep            = 3,                            /*Is it OK to close AppleTalk ?*/
    ATTransCancelClose          = 4                             /*Cancel the ClosePrep transition*/
};

enum {
    afpByteRangeLock            = 1,                            /*AFPCall command codes*/
    afpVolClose                 = 2,                            /*AFPCall command codes*/
    afpDirClose                 = 3,                            /*AFPCall command codes*/
    afpForkClose                = 4,                            /*AFPCall command codes*/
    afpCopyFile                 = 5,                            /*AFPCall command codes*/
    afpDirCreate                = 6,                            /*AFPCall command codes*/
    afpFileCreate               = 7,                            /*AFPCall command codes*/
    afpDelete                   = 8,                            /*AFPCall command codes*/
    afpEnumerate                = 9,                            /*AFPCall command codes*/
    afpFlush                    = 10,                           /*AFPCall command codes*/
    afpForkFlush                = 11,                           /*AFPCall command codes*/
    afpGetDirParms              = 12,                           /*AFPCall command codes*/
    afpGetFileParms             = 13,                           /*AFPCall command codes*/
    afpGetForkParms             = 14,                           /*AFPCall command codes*/
    afpGetSInfo                 = 15,                           /*AFPCall command codes*/
    afpGetSParms                = 16,                           /*AFPCall command codes*/
    afpGetVolParms              = 17,                           /*AFPCall command codes*/
    afpLogin                    = 18,                           /*AFPCall command codes*/
    afpContLogin                = 19,                           /*AFPCall command codes*/
    afpLogout                   = 20,                           /*AFPCall command codes*/
    afpMapID                    = 21,                           /*AFPCall command codes*/
    afpMapName                  = 22,                           /*AFPCall command codes*/
    afpMove                     = 23,                           /*AFPCall command codes*/
    afpOpenVol                  = 24,                           /*AFPCall command codes*/
    afpOpenDir                  = 25,                           /*AFPCall command codes*/
    afpOpenFork                 = 26,                           /*AFPCall command codes*/
    afpRead                     = 27,                           /*AFPCall command codes*/
    afpRename                   = 28,                           /*AFPCall command codes*/
    afpSetDirParms              = 29,                           /*AFPCall command codes*/
    afpSetFileParms             = 30,                           /*AFPCall command codes*/
    afpSetForkParms             = 31,                           /*AFPCall command codes*/
    afpSetVolParms              = 32,                           /*AFPCall command codes*/
    afpWrite                    = 33,                           /*AFPCall command codes*/
    afpGetFlDrParms             = 34,                           /*AFPCall command codes*/
    afpSetFlDrParms             = 35,                           /*AFPCall command codes*/
    afpDTOpen                   = 48,                           /*AFPCall command codes*/
    afpDTClose                  = 49,                           /*AFPCall command codes*/
    afpGetIcon                  = 51,                           /*AFPCall command codes*/
    afpGtIcnInfo                = 52,                           /*AFPCall command codes*/
    afpAddAPPL                  = 53,                           /*AFPCall command codes*/
    afpRmvAPPL                  = 54,                           /*AFPCall command codes*/
    afpGetAPPL                  = 55,                           /*AFPCall command codes*/
    afpAddCmt                   = 56,                           /*AFPCall command codes*/
    afpRmvCmt                   = 57,                           /*AFPCall command codes*/
    afpGetCmt                   = 58,                           /*AFPCall command codes*/
    afpAddIcon                  = 192                           /*Special code for ASP Write commands*/
};

enum {
    xppLoadedBit                = 5,                            /* XPP bit in PortBUse */
    scbMemSize                  = 192,                          /* Size of memory for SCB */
    xppFlagClr                  = 0                             /* Cs for AFPCommandBlock */
};

enum {
    xppFlagSet                  = 128                           /* StartEndFlag & NewLineFlag fields. */
};

enum {
    lapSize                     = 20,
    ddpSize                     = 26,
    nbpSize                     = 26,
    atpSize                     = 56
};

enum {
    atpXOvalue                  = 32,                           /*ATP exactly-once bit */
    atpEOMvalue                 = 16,                           /*ATP End-Of-Message bit */
    atpSTSvalue                 = 8,                            /*ATP Send-Transmission-Status bit */
    atpTIDValidvalue            = 2,                            /*ATP trans. ID valid bit */
    atpSendChkvalue             = 1                             /*ATP send checksum bit */
};

enum {
    zipGetLocalZones            = 5,
    zipGetZoneList              = 6,
    zipGetMyZone                = 7
};

enum {
    LAPMgrPtr                   = 0x0B18                        /*Entry point for LAP Manager*/
};

enum {
    LAPMgrCall                  = 2,                            /*Offset to LAP routines*/
    LAddAEQ                     = 23,                           /*LAPAddATQ routine selector*/
    LRmvAEQ                     = 24                            /*LAPRmvATQ routine selector*/
};

#define MPPioCompletion MPP.ioCompletion
#define MPPioResult MPP.ioResult
#define MPPioRefNum MPP.ioRefNum
#define MPPcsCode MPP.csCode
#define LAPprotType LAP.protType
#define LAPwdsPointer LAP.u.wdsPointer
#define LAPhandler LAP.u.handler
#define DDPsocket DDP.socket
#define DDPchecksumFlag DDP.checksumFlag
#define DDPwdsPointer DDP.u.wdsPointer
#define DDPlistener DDP.u.listener
#define NBPinterval NBP.interval
#define NBPcount NBP.count
#define NBPntQElPtr NBP.nbpPtrs.ntQElPtr
#define NBPentityPtr NBP.nbpPtrs.entityPtr
#define NBPverifyFlag NBP.parm.verifyFlag
#define NBPretBuffPtr NBP.parm.Lookup.retBuffPtr
#define NBPretBuffSize NBP.parm.Lookup.retBuffSize
#define NBPmaxToGet NBP.parm.Lookup.maxToGet
#define NBPnumGotten NBP.parm.Lookup.numGotten
#define NBPconfirmAddr NBP.parm.Confirm.confirmAddr
#define NBPnKillQEl NBPKILL.nKillQEl
#define NBPnewSocket NBP.parm.Confirm.newSocket
#define ATPioCompletion ATP.ioCompletion
#define ATPioResult ATP.ioResult
#define ATPuserData ATP.userData
#define ATPreqTID ATP.reqTID
#define ATPioRefNum ATP.ioRefNum
#define ATPcsCode ATP.csCode
#define ATPatpSocket ATP.atpSocket
#define ATPatpFlags ATP.atpFlags
#define ATPaddrBlock ATP.addrBlock
#define ATPreqLength ATP.reqLength
#define ATPreqPointer ATP.reqPointer
#define ATPbdsPointer ATP.bdsPointer
#define ATPtimeOutVal SREQ.timeOutVal
#define ATPnumOfResps SREQ.numOfResps
#define ATPretryCount SREQ.retryCount
#define ATPnumOfBuffs OTH1.u.numOfBuffs
#define ATPbitMap OTH1.u.bitMap
#define ATPrspNum OTH1.u.rspNum
#define ATPbdsSize OTH2.bdsSize
#define ATPtransID OTH2.transID
#define ATPaKillQEl KILL.aKillQEl
enum {
    tLAPRead                    = 0,
    tLAPWrite                   = 1,
    tDDPRead                    = 2,
    tDDPWrite                   = 3,
    tNBPLookup                  = 4,
    tNBPConfirm                 = 5,
    tNBPRegister                = 6,
    tATPSndRequest              = 7,
    tATPGetRequest              = 8,
    tATPSdRsp                   = 9,
    tATPAddRsp                  = 10,
    tATPRequest                 = 11,
    tATPResponse                = 12
};

typedef SInt8                           ABCallType;
enum {
    lapProto                    = 0,
    ddpProto                    = 1,
    nbpProto                    = 2,
    atpProto                    = 3
};

typedef UInt8                           ABProtoType;
typedef Byte                            ABByte;

struct LAPAdrBlock {
    UInt8                           dstNodeID;
    UInt8                           srcNodeID;
    ABByte                          lapProtType;
    UInt8                           filler;                     /*    Filler for proper byte alignment*/
};
typedef struct LAPAdrBlock              LAPAdrBlock;
typedef struct ATQEntry                 ATQEntry;

typedef ATQEntry *                      ATQEntryPtr;
typedef CALLBACK_API_C( long , ATalkTransitionEventProcPtr )(long eventCode, ATQEntryPtr qElem, void *eventParameter);
typedef STACK_UPP_TYPE(ATalkTransitionEventProcPtr)             ATalkTransitionEventUPP;
typedef ATalkTransitionEventUPP         ATalkTransitionEvent;

struct ATQEntry {
    struct ATQEntry *               qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    ATalkTransitionEventUPP         CallAddr;                   /*your routine descriptor*/
};

/* 
    Real definition of EntityName is 3 PACKED strings of any length (32 is just an example). No
    offests for Asm since each String address must be calculated by adding length byte to last string ptr.
    In Pascal, String(32) will be 34 bytes long since fields never start on an odd byte unless they are 
    only a byte long. So this will generate correct looking interfaces for Pascal and C, but they will not
    be the same, which is OK since they are not used. 
*/

struct EntityName {
    Str32Field                      objStr;
    Str32Field                      typeStr;
    Str32Field                      zoneStr;
};
typedef struct EntityName               EntityName;

typedef EntityName *                    EntityPtr;

struct AddrBlock {
    UInt16                          aNet;
    UInt8                           aNode;
    UInt8                           aSocket;
};
typedef struct AddrBlock                AddrBlock;

struct RetransType {
    UInt8                           retransInterval;
    UInt8                           retransCount;
};
typedef struct RetransType              RetransType;

struct BDSElement {
    short                           buffSize;
    void *                          buffPtr;
    short                           dataSize;
    long                            userBytes;
};
typedef struct BDSElement               BDSElement;

typedef BDSElement                      BDSType[8];
typedef BDSElement *                    BDSPtr;
typedef char                            BitMapType;

struct ATLAPRec {
    ABCallType                      abOpcode;
    SInt8                           filler;                     /*    Filler for proper byte alignment*/
    short                           abResult;
    long                            abUserReference;
    LAPAdrBlock                     lapAddress;
    short                           lapReqCount;
    short                           lapActCount;
    void *                          lapDataPtr;
};
typedef struct ATLAPRec                 ATLAPRec;
typedef ATLAPRec *                      ATLAPRecPtr;
typedef ATLAPRecPtr *                   ATLAPRecHandle;

struct ATDDPRec {
    ABCallType                      abOpcode;
    SInt8                           filler;                     /*    Filler for proper byte alignment*/
    short                           abResult;
    long                            abUserReference;
    short                           ddpType;
    short                           ddpSocket;
    AddrBlock                       ddpAddress;
    short                           ddpReqCount;
    short                           ddpActCount;
    void *                          ddpDataPtr;
    short                           ddpNodeID;
};
typedef struct ATDDPRec                 ATDDPRec;
typedef ATDDPRec *                      ATDDPRecPtr;
typedef ATDDPRecPtr *                   ATDDPRecHandle;

struct ATNBPRec {
    ABCallType                      abOpcode;
    SInt8                           filler;                     /*    Filler for proper byte alignment*/
    short                           abResult;
    long                            abUserReference;
    EntityPtr                       nbpEntityPtr;
    void *                          nbpBufPtr;
    short                           nbpBufSize;
    short                           nbpDataField;
    AddrBlock                       nbpAddress;
    RetransType                     nbpRetransmitInfo;
};
typedef struct ATNBPRec                 ATNBPRec;
typedef ATNBPRec *                      ATNBPRecPtr;
typedef ATNBPRecPtr *                   ATNBPRecHandle;

struct ATATPRec {
    ABCallType                      abOpcode;
    SInt8                           filler1;                    /*    Filler for proper byte alignment*/
    short                           abResult;
    long                            abUserReference;
    short                           atpSocket;
    AddrBlock                       atpAddress;
    short                           atpReqCount;
    void *                          atpDataPtr;
    BDSPtr                          atpRspBDSPtr;
    BitMapType                      atpBitMap;
    SInt8                           filler2;                    /*    Filler for proper byte alignment*/
    short                           atpTransID;
    short                           atpActCount;
    long                            atpUserData;
    Boolean                         atpXO;
    Boolean                         atpEOM;
    short                           atpTimeOut;
    short                           atpRetries;
    short                           atpNumBufs;
    short                           atpNumRsp;
    short                           atpBDSSize;
    long                            atpRspUData;
    void *                          atpRspBuf;
    short                           atpRspSize;
};
typedef struct ATATPRec                 ATATPRec;
typedef ATATPRec *                      ATATPRecPtr;
typedef ATATPRecPtr *                   ATATPRecHandle;

struct AFPCommandBlock {
    UInt8                           cmdByte;
    UInt8                           startEndFlag;
    short                           forkRefNum;
    long                            rwOffset;
    long                            reqCount;
    UInt8                           newLineFlag;
    char                            newLineChar;
};
typedef struct AFPCommandBlock          AFPCommandBlock;
typedef union MPPParamBlock             MPPParamBlock;
typedef union ATPParamBlock             ATPParamBlock;
typedef union XPPParamBlock             XPPParamBlock;

typedef MPPParamBlock *                 MPPPBPtr;
typedef ATPParamBlock *                 ATPPBPtr;
typedef XPPParamBlock *                 XPPParmBlkPtr;
typedef CALLBACK_API( void , MPPCompletionProcPtr )(MPPPBPtr thePBptr);
/*
    WARNING: MPPCompletionProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef CALLBACK_API( void , ATPCompletionProcPtr )(ATPPBPtr thePBptr);
/*
    WARNING: ATPCompletionProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef CALLBACK_API( void , XPPCompletionProcPtr )(XPPParmBlkPtr thePBptr);
/*
    WARNING: XPPCompletionProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef CALLBACK_API( void , AttnRoutineProcPtr )(short sessRefnum, short attnBytes);
/*
    WARNING: AttnRoutineProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(MPPCompletionProcPtr)                 MPPCompletionUPP;
typedef REGISTER_UPP_TYPE(ATPCompletionProcPtr)                 ATPCompletionUPP;
typedef REGISTER_UPP_TYPE(XPPCompletionProcPtr)                 XPPCompletionUPP;
typedef REGISTER_UPP_TYPE(AttnRoutineProcPtr)                   AttnRoutineUPP;


struct WDSElement {
    short                           entryLength;
    Ptr                             entryPtr;
};
typedef struct WDSElement               WDSElement;

struct NTElement {
    AddrBlock                       nteAddress;                 /*network address of entity*/
    SInt8                           filler;
    SInt8                           entityData[99];             /*Object, Type & Zone*/
};
typedef struct NTElement                NTElement;

struct NamesTableEntry {
    Ptr                             qNext;                      /*ptr to next NTE*/
    NTElement                       nt;
};
typedef struct NamesTableEntry          NamesTableEntry;
typedef CALLBACK_API( Boolean , MPPProtocolHandlerProcPtr )(Ptr SCCAddr1, Ptr SCCAddr2, Ptr MPPLocalVars, Ptr nextFreeByteInRHA, Ptr ReadPacketAndReadRestPtr, short numBytesLeftToReadInPacket);
typedef CALLBACK_API( Boolean , DDPSocketListenerProcPtr )(Ptr SCCAddr1, Ptr SCCAddr2, Ptr MPPLocalVars, Ptr nextFreeByteInRHA, Ptr ReadPacketAndReadRestPtr, UInt8 packetDestinationNumber, short numBytesLeftToReadInPacket);
typedef STACK_UPP_TYPE(MPPProtocolHandlerProcPtr)               MPPProtocolHandlerUPP;
typedef STACK_UPP_TYPE(DDPSocketListenerProcPtr)                DDPSocketListenerUPP;
/*
    MPPProtocolHandlerProcs and  DDPSocketListenerProcs cannot be written 
    in or called from a high-level language without the help of mixed mode 
    or assembly glue because they use the following parameter-passing conventions:

    typedef Boolean (*MPPProtocolHandlerProcPtr)(Ptr SCCAddr1, Ptr SCCAddr2, 
            Ptr MPPLocalVars, Ptr nextFreeByteInRHA, Ptr ReadPacketAndReadRestPtr, 
            short numBytesLeftToReadInPacket);

        In:
            =>  SCCAddr1                    A0.L
            =>  SCCAddr2                    A1.L
            =>  MPPLocalVars                A2.L
            =>  nextFreeByteInRHA           A3.L
            =>  ReadPacketAndReadRestPtr    A4.L
            =>  numBytesLeftToReadInPacket  D1.W
        Out:
            <=  Boolean                     Z bit of CCR

    typedef Boolean (*DDPSocketListenerProcPtr)(Ptr SCCAddr1, Ptr SCCAddr2, 
            Ptr MPPLocalVars, Ptr nextFreeByteInRHA, Ptr ReadPacketAndReadRestPtr, 
            UInt8 packetDestinationNumber, short numBytesLeftToReadInPacket);

        In:
            =>  SCCAddr1                    A0.L
            =>  SCCAddr2                    A1.L
            =>  MPPLocalVars                A2.L
            =>  nextFreeByteInRHA           A3.L
            =>  ReadPacketAndReadRestPtr    A4.L
            =>  packetDestinationNumber     D0.B
            =>  numBytesLeftToReadInPacket  D1.W
        Out:
            <=  Boolean                     Z bit of CCR

*/

struct MPPparms {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
};
typedef struct MPPparms                 MPPparms;

struct LAPparms {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    UInt8                           protType;                   /*ALAP protocol Type */
    UInt8                           filler;
    union {
        void *                          wdsPointer;             /*-> write data structure*/
        MPPProtocolHandlerUPP           handler;                /*-> protocol handler routine*/
    }                                 u;
};
typedef struct LAPparms                 LAPparms;

struct DDPparms {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    UInt8                           socket;                     /*socket number */
    UInt8                           checksumFlag;               /*check sum flag */
    union {
        void *                          wdsPointer;             /*-> write data structure*/
        DDPSocketListenerUPP            listener;               /*->write data structure or -> Listener*/
    }                                 u;
};
typedef struct DDPparms                 DDPparms;

union NBPPtrs {
    Ptr                             ntQElPtr;
    Ptr                             entityPtr;
};
typedef union NBPPtrs                   NBPPtrs;

union LookupConfirmParams {
    UInt8                           verifyFlag;
    struct {
        void *                          retBuffPtr;
        short                           retBuffSize;
        short                           maxToGet;
        short                           numGotten;
    }                                 Lookup;
    struct {
        AddrBlock                       confirmAddr;
        UInt8                           newSocket;
        SInt8                           filler;                 /*    Filler for proper byte alignment*/
    }                                 Confirm;
};
typedef union LookupConfirmParams       LookupConfirmParams;

struct NBPparms {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    UInt8                           interval;                   /*retry interval */
    UInt8                           count;                      /*retry count */
    NBPPtrs                         nbpPtrs;
    LookupConfirmParams             parm;
};
typedef struct NBPparms                 NBPparms;

struct SetSelfparms {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    UInt8                           newSelfFlag;                /*self-send toggle flag */
    UInt8                           oldSelfFlag;                /*previous self-send state */
};
typedef struct SetSelfparms             SetSelfparms;

struct NBPKillparms {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    void *                          nKillQEl;                   /*ptr to i/o queue element to cancel */
};
typedef struct NBPKillparms             NBPKillparms;


struct GetAppleTalkInfoParm {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
                                                                /*max. concurrent NBP requests*/
    short                           version;                    /*requested info version*/
    void *                          varsPtr;                    /*pointer to well known MPP vars*/
    void *                          DCEPtr;                     /*pointer to MPP DCE*/
    short                           portID;                     /*port number [0..7]*/
    long                            configuration;              /*32-bit configuration word*/
    short                           selfSend;                   /*non zero if SelfSend enabled*/
    short                           netLo;                      /*low value of network range*/
    short                           netHi;                      /*high value of network range*/
    long                            ourAdd;                     /*our 24-bit AppleTalk address*/
    long                            routerAddr;                 /*24-bit address of (last) router*/
    short                           numOfPHs;                   /*max. number of protocol handlers*/
    short                           numOfSkts;                  /*max. number of static sockets*/
    short                           numNBPEs;                   /*max. concurrent NBP requests*/
    void *                          nTQueue;                    /*pointer to registered name queue*/
    short                           LAlength;                   /*length in bytes of data link addr*/
    void *                          linkAddr;                   /*data link address returned*/
    Ptr                             zoneName;                   /*zone name returned*/
};
typedef struct GetAppleTalkInfoParm     GetAppleTalkInfoParm;

struct ATalkClosePrepParm {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    Ptr                             appName;                    /*pointer to application name in buffer*/
};
typedef struct ATalkClosePrepParm       ATalkClosePrepParm;

union MPPParamBlock {
    MPPparms                        MPP;                        /*General MPP parms*/
    LAPparms                        LAP;                        /*ALAP calls*/
    DDPparms                        DDP;                        /*DDP calls*/
    NBPparms                        NBP;                        /*NBP calls*/
    SetSelfparms                    SETSELF;
    NBPKillparms                    NBPKILL;
    GetAppleTalkInfoParm            GAIINFO;
    ATalkClosePrepParm              ATALKCLOSE;
};


struct XPPPrmBlk {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    XPPCompletionUPP                ioCompletion;
    OSErr                           ioResult;
    long                            cmdResult;
    short                           ioVRefNum;
    short                           ioRefNum;
    short                           csCode;
    short                           sessRefnum;                 /*Offset to session refnum */
    UInt8                           aspTimeout;                 /*Timeout for ATP */
    UInt8                           aspRetry;                   /*Retry count for ATP */
    short                           cbSize;                     /*Command block size */
    Ptr                             cbPtr;                      /*Command block pointer */
    short                           rbSize;                     /*Reply buffer size */
    Ptr                             rbPtr;                      /*Reply buffer pointer */
    short                           wdSize;                     /*Write Data size*/
    Ptr                             wdPtr;                      /*Write Data pointer*/
    UInt8                           ccbStart[296];              /*CCB memory allocated for driver afpWrite max size(CCB)=296 all other calls=150*/
};
typedef struct XPPPrmBlk                XPPPrmBlk;

struct ASPGetparmsBlk {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    XPPCompletionUPP                ioCompletion;
    OSErr                           ioResult;
    long                            cmdResult;
    short                           ioVRefNum;
    short                           ioRefNum;
    short                           csCode;
    short                           aspMaxCmdSize;              /*For SPGetParms*/
    short                           aspQuantumSize;
    short                           numSesss;
};
typedef struct ASPGetparmsBlk           ASPGetparmsBlk;

struct ASPAbortPrm {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    XPPCompletionUPP                ioCompletion;
    OSErr                           ioResult;
    long                            cmdResult;
    short                           ioVRefNum;
    short                           ioRefNum;
    short                           csCode;
    Ptr                             abortSCBPtr;                /*SCB pointer for AbortOS */
};
typedef struct ASPAbortPrm              ASPAbortPrm;

struct ASPOpenPrm {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    XPPCompletionUPP                ioCompletion;
    OSErr                           ioResult;
    long                            cmdResult;
    short                           ioVRefNum;
    short                           ioRefNum;
    short                           csCode;
    short                           sessRefnum;                 /*Offset to session refnum */
    UInt8                           aspTimeout;                 /*Timeout for ATP */
    UInt8                           aspRetry;                   /*Retry count for ATP */
    AddrBlock                       serverAddr;                 /*Server address block */
    Ptr                             scbPointer;                 /*SCB pointer */
    AttnRoutineUPP                  attnRoutine;                /*Attention routine pointer*/
};
typedef struct ASPOpenPrm               ASPOpenPrm;
typedef ASPOpenPrm *                    ASPOpenPrmPtr;

struct AFPLoginPrm {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    XPPCompletionUPP                ioCompletion;
    OSErr                           ioResult;
    long                            cmdResult;
    short                           ioVRefNum;
    short                           ioRefNum;
    short                           csCode;
    short                           sessRefnum;                 /*Offset to session refnum */
    UInt8                           aspTimeout;                 /*Timeout for ATP */
    UInt8                           aspRetry;                   /*Retry count for ATP */
    short                           cbSize;                     /*Command block size */
    void *                          cbPtr;                      /*Command block pointer */
    short                           rbSize;                     /*Reply buffer size */
    void *                          rbPtr;                      /*Reply buffer pointer */
    AddrBlock                       afpAddrBlock;               /*block in AFP login */
    void *                          afpSCBPtr;                  /*SCB pointer in AFP login */
    AttnRoutineUPP                  afpAttnRoutine;             /*routine pointer in AFP login */
    UInt8                           ccbFill[144];               /*CCB memory allocated for driver  Login needs only 150 bytes BUT CCB really starts in the middle of AFPSCBPtr and also clobbers AFPAttnRoutine. */
};
typedef struct AFPLoginPrm              AFPLoginPrm;

struct XCallParam {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    XPPCompletionUPP                ioCompletion;
    OSErr                           ioResult;
    long                            cmdResult;
    short                           ioVRefNum;
    short                           ioRefNum;
    short                           csCode;
    short                           xppSubCode;
    UInt8                           xppTimeout;                 /*retry interval (seconds)*/
    UInt8                           xppRetry;                   /*retry count*/
    short                           filler1;
    void *                          zipBuffPtr;                 /*pointer to buffer (must be 578 bytes)*/
    short                           zipNumZones;                /*no. of zone names in this response*/
    UInt8                           zipLastFlag;                /*non-zero if no more zones*/
    UInt8                           filler2;                    /*filler*/
    UInt8                           zipInfoField[70];           /*on initial call, set first word to zero*/
};
typedef struct XCallParam               XCallParam;

union XPPParamBlock {
    XPPPrmBlk                       XPP;
    ASPGetparmsBlk                  GETPARM;
    ASPAbortPrm                     ABORT;
    ASPOpenPrm                      OPEN;
    AFPLoginPrm                     LOGIN;
    XCallParam                      XCALL;
};



struct ATPparms {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    UInt8                           atpSocket;                  /*currbitmap for requests or ATP socket number*/
    UInt8                           atpFlags;                   /*control information*/
    AddrBlock                       addrBlock;                  /*source/dest. socket address*/
    short                           reqLength;                  /*request/response length*/
    void *                          reqPointer;                 /*->request/response Data*/
    void *                          bdsPointer;                 /*->response BDS */
};
typedef struct ATPparms                 ATPparms;

struct SendReqparms {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    UInt8                           atpSocket;                  /*currbitmap for requests or ATP socket number*/
    UInt8                           atpFlags;                   /*control information*/
    AddrBlock                       addrBlock;                  /*source/dest. socket address*/
    short                           reqLength;                  /*request/response length*/
    void *                          reqPointer;                 /*->request/response Data*/
    void *                          bdsPointer;                 /*->response BDS */
    UInt8                           numOfBuffs;                 /*numOfBuffs */
    UInt8                           timeOutVal;                 /*timeout interval */
    UInt8                           numOfResps;                 /*number of responses actually received */
    UInt8                           retryCount;                 /*number of retries */
    short                           intBuff;                    /*used internally for NSendRequest */
    UInt8                           TRelTime;                   /*TRelease time for extended send request */
    SInt8                           filler0;
};
typedef struct SendReqparms             SendReqparms;


struct ATPmisc1 {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    UInt8                           atpSocket;                  /*currbitmap for requests or ATP socket number*/
    UInt8                           atpFlags;                   /*control information*/
    AddrBlock                       addrBlock;                  /*source/dest. socket address*/
    short                           reqLength;                  /*request/response length*/
    void *                          reqPointer;                 /*->request/response Data*/
    void *                          bdsPointer;                 /*->response BDS */
    union {
        UInt8                           bitMap;                 /*bitmap received */
        UInt8                           numOfBuffs;             /*number of responses being sent*/
        UInt8                           rspNum;                 /*sequence number*/
    }                                 u;
};
typedef struct ATPmisc1                 ATPmisc1;

struct ATPmisc2 {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    UInt8                           atpSocket;                  /*currbitmap for requests or ATP socket number*/
    UInt8                           atpFlags;                   /*control information*/
    AddrBlock                       addrBlock;                  /*source/dest. socket address*/
    short                           reqLength;                  /*request/response length*/
    void *                          reqPointer;                 /*->request/response Data*/
    void *                          bdsPointer;                 /*->response BDS */
    UInt8                           filler;
    UInt8                           bdsSize;                    /*number of BDS elements */
    short                           transID;                    /*transaction ID recd. */
};
typedef struct ATPmisc2                 ATPmisc2;

struct Killparms {
    QElem *                         qLink;                      /*next queue entry*/
    short                           qType;                      /*queue type*/
    short                           ioTrap;                     /*routine trap*/
    Ptr                             ioCmdAddr;                  /*routine address*/
    ATPCompletionUPP                ioCompletion;               /*ATPCompletionUPP or MPPCompletionUPP*/
    OSErr                           ioResult;                   /*result code*/
    long                            userData;                   /*Command result (ATP user bytes)*/
    short                           reqTID;                     /*request transaction ID*/
    short                           ioRefNum;                   /*driver reference number*/
    short                           csCode;                     /*Call command code*/
    UInt8                           atpSocket;                  /*currbitmap for requests or ATP socket number*/
    UInt8                           atpFlags;                   /*control information*/
    AddrBlock                       addrBlock;                  /*source/dest. socket address*/
    short                           reqLength;                  /*request/response length*/
    void *                          reqPointer;                 /*->request/response Data*/
    void *                          bdsPointer;                 /*->response BDS */
    void *                          aKillQEl;                   /*ptr to i/o queue element to cancel*/
};
typedef struct Killparms                Killparms;

union ATPParamBlock {
    ATPparms                        ATP;                        /*General ATP parms*/
    SendReqparms                    SREQ;                       /*sendrequest parms*/
    ATPmisc1                        OTH1;                       /*and a few others*/
    ATPmisc2                        OTH2;                       /*and a few others*/
    Killparms                       KILL;                       /*and a few others*/
};

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ATalkTransitionEventUPP)
    NewATalkTransitionEventUPP     (ATalkTransitionEventProcPtr userRoutine);

    EXTERN_API(MPPCompletionUPP)
    NewMPPCompletionUPP            (MPPCompletionProcPtr    userRoutine);

    EXTERN_API(ATPCompletionUPP)
    NewATPCompletionUPP            (ATPCompletionProcPtr    userRoutine);

    EXTERN_API(XPPCompletionUPP)
    NewXPPCompletionUPP            (XPPCompletionProcPtr    userRoutine);

    EXTERN_API(AttnRoutineUPP)
    NewAttnRoutineUPP              (AttnRoutineProcPtr      userRoutine);

    EXTERN_API(MPPProtocolHandlerUPP)
    NewMPPProtocolHandlerUPP       (MPPProtocolHandlerProcPtr userRoutine);

    EXTERN_API(DDPSocketListenerUPP)
    NewDDPSocketListenerUPP        (DDPSocketListenerProcPtr userRoutine);

    EXTERN_API(void)
    DisposeATalkTransitionEventUPP    (ATalkTransitionEventUPP userUPP);

    EXTERN_API(void)
    DisposeMPPCompletionUPP        (MPPCompletionUPP        userUPP);

    EXTERN_API(void)
    DisposeATPCompletionUPP        (ATPCompletionUPP        userUPP);

    EXTERN_API(void)
    DisposeXPPCompletionUPP        (XPPCompletionUPP        userUPP);

    EXTERN_API(void)
    DisposeAttnRoutineUPP          (AttnRoutineUPP          userUPP);

    EXTERN_API(void)
    DisposeMPPProtocolHandlerUPP    (MPPProtocolHandlerUPP  userUPP);

    EXTERN_API(void)
    DisposeDDPSocketListenerUPP    (DDPSocketListenerUPP    userUPP);

    EXTERN_API(long)
    InvokeATalkTransitionEventUPP    (long                  eventCode,
                                    ATQEntryPtr             qElem,
                                    void *                  eventParameter,
                                    ATalkTransitionEventUPP userUPP);

    EXTERN_API(void)
    InvokeMPPCompletionUPP         (MPPPBPtr                thePBptr,
                                    MPPCompletionUPP        userUPP);

    EXTERN_API(void)
    InvokeATPCompletionUPP         (ATPPBPtr                thePBptr,
                                    ATPCompletionUPP        userUPP);

    EXTERN_API(void)
    InvokeXPPCompletionUPP         (XPPParmBlkPtr           thePBptr,
                                    XPPCompletionUPP        userUPP);

    EXTERN_API(void)
    InvokeAttnRoutineUPP           (short                   sessRefnum,
                                    short                   attnBytes,
                                    AttnRoutineUPP          userUPP);

    EXTERN_API(Boolean)
    InvokeMPPProtocolHandlerUPP    (Ptr                     SCCAddr1,
                                    Ptr                     SCCAddr2,
                                    Ptr                     MPPLocalVars,
                                    Ptr                     nextFreeByteInRHA,
                                    Ptr                     ReadPacketAndReadRestPtr,
                                    short                   numBytesLeftToReadInPacket,
                                    MPPProtocolHandlerUPP   userUPP);

    EXTERN_API(Boolean)
    InvokeDDPSocketListenerUPP     (Ptr                     SCCAddr1,
                                    Ptr                     SCCAddr2,
                                    Ptr                     MPPLocalVars,
                                    Ptr                     nextFreeByteInRHA,
                                    Ptr                     ReadPacketAndReadRestPtr,
                                    UInt8                   packetDestinationNumber,
                                    short                   numBytesLeftToReadInPacket,
                                    DDPSocketListenerUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppATalkTransitionEventProcInfo = 0x00000FF1 };          /* 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppMPPCompletionProcInfo = 0x00009802 };                 /* register no_return_value Func(4_bytes:A0) */
    enum { uppATPCompletionProcInfo = 0x00009802 };                 /* register no_return_value Func(4_bytes:A0) */
    enum { uppXPPCompletionProcInfo = 0x00009802 };                 /* register no_return_value Func(4_bytes:A0) */
    enum { uppAttnRoutineProcInfo = 0x00061002 };                   /* register no_return_value Func(2_bytes:D0, 2_bytes:D1) */
    enum { uppMPPProtocolHandlerProcInfo = 0x0000007F };            /* SPECIAL_CASE_PROCINFO(7) */
    enum { uppDDPSocketListenerProcInfo = 0x0000008F };             /* SPECIAL_CASE_PROCINFO(8) */
    #define NewATalkTransitionEventUPP(userRoutine)                 (ATalkTransitionEventUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppATalkTransitionEventProcInfo, GetCurrentArchitecture())
    #define NewMPPCompletionUPP(userRoutine)                        (MPPCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMPPCompletionProcInfo, GetCurrentArchitecture())
    #define NewATPCompletionUPP(userRoutine)                        (ATPCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppATPCompletionProcInfo, GetCurrentArchitecture())
    #define NewXPPCompletionUPP(userRoutine)                        (XPPCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppXPPCompletionProcInfo, GetCurrentArchitecture())
    #define NewAttnRoutineUPP(userRoutine)                          (AttnRoutineUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAttnRoutineProcInfo, GetCurrentArchitecture())
    #define NewMPPProtocolHandlerUPP(userRoutine)                   (MPPProtocolHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMPPProtocolHandlerProcInfo, GetCurrentArchitecture())
    #define NewDDPSocketListenerUPP(userRoutine)                    (DDPSocketListenerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDDPSocketListenerProcInfo, GetCurrentArchitecture())
    #define DisposeATalkTransitionEventUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define DisposeMPPCompletionUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeATPCompletionUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeXPPCompletionUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeAttnRoutineUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposeMPPProtocolHandlerUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define DisposeDDPSocketListenerUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeATalkTransitionEventUPP(eventCode, qElem, eventParameter, userUPP)  (long)CALL_THREE_PARAMETER_UPP((userUPP), uppATalkTransitionEventProcInfo, (eventCode), (qElem), (eventParameter))
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeMPPCompletionUPP(__A0, __A1)
    void InvokeMPPCompletionUPP(MPPPBPtr thePBptr, MPPCompletionUPP userUPP) = 0x4E91;
    #else
        #define InvokeMPPCompletionUPP(thePBptr, userUPP)               CALL_ONE_PARAMETER_UPP((userUPP), uppMPPCompletionProcInfo, (thePBptr))
    #endif
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeATPCompletionUPP(__A0, __A1)
    void InvokeATPCompletionUPP(ATPPBPtr thePBptr, ATPCompletionUPP userUPP) = 0x4E91;
    #else
        #define InvokeATPCompletionUPP(thePBptr, userUPP)               CALL_ONE_PARAMETER_UPP((userUPP), uppATPCompletionProcInfo, (thePBptr))
    #endif
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeXPPCompletionUPP(__A0, __A1)
    void InvokeXPPCompletionUPP(XPPParmBlkPtr thePBptr, XPPCompletionUPP userUPP) = 0x4E91;
    #else
        #define InvokeXPPCompletionUPP(thePBptr, userUPP)               CALL_ONE_PARAMETER_UPP((userUPP), uppXPPCompletionProcInfo, (thePBptr))
    #endif
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeAttnRoutineUPP(__D0, __D1, __A0)
    void InvokeAttnRoutineUPP(short sessRefnum, short attnBytes, AttnRoutineUPP userUPP) = 0x4E90;
    #else
        #define InvokeAttnRoutineUPP(sessRefnum, attnBytes, userUPP)    CALL_TWO_PARAMETER_UPP((userUPP), uppAttnRoutineProcInfo, (sessRefnum), (attnBytes))
    #endif
    #define InvokeMPPProtocolHandlerUPP(SCCAddr1, SCCAddr2, MPPLocalVars, nextFreeByteInRHA, ReadPacketAndReadRestPtr, numBytesLeftToReadInPacket, userUPP)  (Boolean)CALL_SIX_PARAMETER_UPP((userUPP), uppMPPProtocolHandlerProcInfo, (SCCAddr1), (SCCAddr2), (MPPLocalVars), (nextFreeByteInRHA), (ReadPacketAndReadRestPtr), (numBytesLeftToReadInPacket))
    #define InvokeDDPSocketListenerUPP(SCCAddr1, SCCAddr2, MPPLocalVars, nextFreeByteInRHA, ReadPacketAndReadRestPtr, packetDestinationNumber, numBytesLeftToReadInPacket, userUPP)  (Boolean)CALL_SEVEN_PARAMETER_UPP((userUPP), uppDDPSocketListenerProcInfo, (SCCAddr1), (SCCAddr2), (MPPLocalVars), (nextFreeByteInRHA), (ReadPacketAndReadRestPtr), (packetDestinationNumber), (numBytesLeftToReadInPacket))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewATalkTransitionEventProc(userRoutine)                NewATalkTransitionEventUPP(userRoutine)
#define NewMPPCompletionProc(userRoutine)                       NewMPPCompletionUPP(userRoutine)
#define NewATPCompletionProc(userRoutine)                       NewATPCompletionUPP(userRoutine)
#define NewXPPCompletionProc(userRoutine)                       NewXPPCompletionUPP(userRoutine)
#define NewAttnRoutineProc(userRoutine)                         NewAttnRoutineUPP(userRoutine)
#define NewMPPProtocolHandlerProc(userRoutine)                  NewMPPProtocolHandlerUPP(userRoutine)
#define NewDDPSocketListenerProc(userRoutine)                   NewDDPSocketListenerUPP(userRoutine)
#define CallATalkTransitionEventProc(userRoutine, eventCode, qElem, eventParameter) InvokeATalkTransitionEventUPP(eventCode, qElem, eventParameter, userRoutine)
#define CallMPPCompletionProc(userRoutine, thePBptr)            InvokeMPPCompletionUPP(thePBptr, userRoutine)
#define CallATPCompletionProc(userRoutine, thePBptr)            InvokeATPCompletionUPP(thePBptr, userRoutine)
#define CallXPPCompletionProc(userRoutine, thePBptr)            InvokeXPPCompletionUPP(thePBptr, userRoutine)
#define CallAttnRoutineProc(userRoutine, sessRefnum, attnBytes) InvokeAttnRoutineUPP(sessRefnum, attnBytes, userRoutine)
#define CallMPPProtocolHandlerProc(userRoutine, SCCAddr1, SCCAddr2, MPPLocalVars, nextFreeByteInRHA, ReadPacketAndReadRestPtr, numBytesLeftToReadInPacket) InvokeMPPProtocolHandlerUPP(SCCAddr1, SCCAddr2, MPPLocalVars, nextFreeByteInRHA, ReadPacketAndReadRestPtr, numBytesLeftToReadInPacket, userRoutine)
#define CallDDPSocketListenerProc(userRoutine, SCCAddr1, SCCAddr2, MPPLocalVars, nextFreeByteInRHA, ReadPacketAndReadRestPtr, packetDestinationNumber, numBytesLeftToReadInPacket) InvokeDDPSocketListenerUPP(SCCAddr1, SCCAddr2, MPPLocalVars, nextFreeByteInRHA, ReadPacketAndReadRestPtr, packetDestinationNumber, numBytesLeftToReadInPacket, userRoutine)
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OpenXPP                         (short *                xppRefnum);

EXTERN_API( OSErr )
ASPOpenSession                  (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
ASPCloseSession                 (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
ASPAbortOS                      (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
ASPGetParms                     (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
ASPCloseAll                     (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
ASPUserWrite                    (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
ASPUserCommand                  (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
ASPGetStatus                    (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

#define ASPGetStatusSync(paramBlock) ASPGetStatus((paramBlock), false)
EXTERN_API( OSErr )
AFPCommand                      (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
GetLocalZones                   (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
GetZoneList                     (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
GetMyZone                       (XPPParmBlkPtr          thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PAttachPH                       (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PDetachPH                       (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PWriteLAP                       (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
POpenSkt                        (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PCloseSkt                       (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PWriteDDP                       (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PRegisterName                   (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PLookupName                     (MPPPBPtr               thePBptr,
                                 Boolean                async);

#define PLookupNameSync(paramBlock) PLookupName((paramBlock), false)
EXTERN_API( OSErr )
PConfirmName                    (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PRemoveName                     (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PSetSelfSend                    (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PKillNBP                        (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PGetAppleTalkInfo               (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PATalkClosePrep                 (MPPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
POpenATPSkt                     (ATPPBPtr               thePBptr,
                                 Boolean                async);

EXTERN_API( OSErr )
PCloseATPSkt                    (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
PSendRequest                    (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
PGetRequest                     (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
PSendResponse                   (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
PAddResponse                    (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
PRelTCB                         (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
PRelRspCB                       (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
PNSendRequest                   (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
PKillSendReq                    (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
PKillGetReq                     (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( OSErr )
ATPKillAllGetReq                (ATPPBPtr               thePBPtr,
                                 Boolean                async);

EXTERN_API( void )
BuildLAPwds                     (Ptr                    wdsPtr,
                                 Ptr                    dataPtr,
                                 short                  destHost,
                                 short                  prototype,
                                 short                  frameLen);

EXTERN_API( void )
BuildDDPwds                     (Ptr                    wdsPtr,
                                 Ptr                    headerPtr,
                                 Ptr                    dataPtr,
                                 AddrBlock              netAddr,
                                 short                  ddpType,
                                 short                  dataLen);

EXTERN_API( void )
NBPSetEntity                    (Ptr                    buffer,
                                 ConstStr32Param        nbpObject,
                                 ConstStr32Param        nbpType,
                                 ConstStr32Param        nbpZone);

EXTERN_API( void )
NBPSetNTE                       (Ptr                    ntePtr,
                                 ConstStr32Param        nbpObject,
                                 ConstStr32Param        nbpType,
                                 ConstStr32Param        nbpZone,
                                 short                  socket);

EXTERN_API( short )
GetBridgeAddress                (void);

EXTERN_API( short )
BuildBDS                        (Ptr                    buffPtr,
                                 Ptr                    bdsPtr,
                                 short                  buffSize);

EXTERN_API( OSErr )
MPPOpen                         (void);

EXTERN_API( OSErr )
LAPAddATQ                       (ATQEntryPtr            theATQEntry);

EXTERN_API( OSErr )
LAPRmvATQ                       (ATQEntryPtr            theATQEntry);

EXTERN_API( OSErr )
ATPLoad                         (void);

EXTERN_API( OSErr )
ATPUnload                       (void);

EXTERN_API( OSErr )
NBPExtract                      (Ptr                    theBuffer,
                                 short                  numInBuf,
                                 short                  whichOne,
                                 EntityName *           abEntity,
                                 AddrBlock *            address);

EXTERN_API( OSErr )
GetNodeAddress                  (short *                myNode,
                                 short *                myNet);

EXTERN_API( Boolean )
IsMPPOpen                       (void);

EXTERN_API( Boolean )
IsATPOpen                       (void);

EXTERN_API( void )
ATEvent                         (long                   event,
                                 Ptr                    infoPtr);

EXTERN_API( OSErr )
ATPreFlightEvent                (long                   event,
                                 long                   cancel,
                                 Ptr                    infoPtr);

/*
    The following routines are obsolete and will not be supported on
    PowerPC. Equivalent functionality is provided by the routines
    above.
*/
#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_CPU_68K
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
MPPClose                        (void);

EXTERN_API( OSErr )
LAPOpenProtocol                 (ABByte                 theLAPType,
                                 Ptr                    protoPtr);

EXTERN_API( OSErr )
LAPCloseProtocol                (ABByte                 theLAPType);

EXTERN_API( OSErr )
LAPWrite                        (ATLAPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
LAPRead                         (ATLAPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
LAPRdCancel                     (ATLAPRecHandle         abRecord);

EXTERN_API( OSErr )
DDPOpenSocket                   (short *                theSocket,
                                 Ptr                    sktListener);

EXTERN_API( OSErr )
DDPCloseSocket                  (short                  theSocket);

EXTERN_API( OSErr )
DDPRead                         (ATDDPRecHandle         abRecord,
                                 Boolean                retCksumErrs,
                                 Boolean                async);

EXTERN_API( OSErr )
DDPWrite                        (ATDDPRecHandle         abRecord,
                                 Boolean                doChecksum,
                                 Boolean                async);

EXTERN_API( OSErr )
DDPRdCancel                     (ATDDPRecHandle         abRecord);

EXTERN_API( OSErr )
ATPOpenSocket                   (AddrBlock              addrRcvd,
                                 short *                atpSocket);

EXTERN_API( OSErr )
ATPCloseSocket                  (short                  atpSocket);

EXTERN_API( OSErr )
ATPSndRequest                   (ATATPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
ATPRequest                      (ATATPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
ATPReqCancel                    (ATATPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
ATPGetRequest                   (ATATPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
ATPSndRsp                       (ATATPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
ATPAddRsp                       (ATATPRecHandle         abRecord);

EXTERN_API( OSErr )
ATPResponse                     (ATATPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
ATPRspCancel                    (ATATPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
NBPRegister                     (ATNBPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
NBPLookup                       (ATNBPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
NBPConfirm                      (ATNBPRecHandle         abRecord,
                                 Boolean                async);

EXTERN_API( OSErr )
NBPRemove                       (EntityPtr              abEntity);

EXTERN_API( OSErr )
NBPLoad                         (void);

EXTERN_API( OSErr )
NBPUnload                       (void);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_CPU_68K */


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

#endif /* __APPLETALK__ */

