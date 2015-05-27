/*
     File:       MacTCP.h
 
     Contains:   TCP Manager Interfaces.
 
     Version:    Technology: MacTCP 2.0.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1989-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MACTCP__
#define __MACTCP__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __APPLETALK__
#include <AppleTalk.h>
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
Developer Notes:
        0. This MacTCP header replaces what used to be defined in the following header files
            MacTCPCommonTypes.h
            GetMyIPAddr.h
            MiscIPPB.h
            TCPPB.h
            UDPPB.h 
            
            When the various control calls are made to the ip driver, you must set up a 
            NewRoutineDescriptor for every non-nil completion routine and/or notifyProc parameter.  
            Otherwise, the 68K driver code, will not correctly call your routine.
        1. For ipctlGetAddr Control calls, use NewGetIPIOCompletionProc
            to set up a GetIPIOCompletionUPP universal procptr to pass as
            the ioCompletion parameter.
        2. For the ipctlEchoICMP and ipctlLAPStats Control calls, use 
            NewIPIOCompletion to set up a IPIOCompletionUPP universal procptr
            to pass in the ioCompletion field of the parameter block.
        3. For TCPCreatePB Control calls, use NewTCPNotifyProc to set up a
            TCPNotifyUPP universal procptr to pass in the notifyProc field
            of the parameter block
        4. For all of the TCP Control calls using the TCPiopb parameter block,
            use NewTCPIOCompletionProc to set up a TCPIOCompletionUPP
            universal procptr to pass in the ioCompletion field of the paramter
            block.
        5. For UDBCreatePB Control calls, use NewUDPNotifyProc to set up a
            UDPNotifyUPP universal procptr to pass in the notifyProc field
            of the parameter block
        6. For all of the UDP Control calls using the UDPiopb parameter block,
            use NewUDPIOCompletionProc to set up a UDPIOCompletionUPP
            universal procptr to pass in the ioCompletion field of the paramter
            block.
        7. For all calls implementing a notifyProc or ioCompletion routine
            which was set up using a NewTCPRoutineProc call, do not call
            DisposeRoutineSDescriptor on the universal procptr until
            after the completion or notify proc has completed.
*/

/* MacTCP return Codes in the range -23000 through -23049 */
enum {
    inProgress                  = 1,                            /* I/O in progress */
    ipBadLapErr                 = -23000,                       /* bad network configuration */
    ipBadCnfgErr                = -23001,                       /* bad IP configuration error */
    ipNoCnfgErr                 = -23002,                       /* missing IP or LAP configuration error */
    ipLoadErr                   = -23003,                       /* error in MacTCP load */
    ipBadAddr                   = -23004,                       /* error in getting address */
    connectionClosing           = -23005,                       /* connection is closing */
    invalidLength               = -23006,
    connectionExists            = -23007,                       /* request conflicts with existing connection */
    connectionDoesntExist       = -23008,                       /* connection does not exist */
    insufficientResources       = -23009,                       /* insufficient resources to perform request */
    invalidStreamPtr            = -23010,
    streamAlreadyOpen           = -23011,
    connectionTerminated        = -23012,
    invalidBufPtr               = -23013,
    invalidRDS                  = -23014,
    invalidWDS                  = -23014,
    openFailed                  = -23015,
    commandTimeout              = -23016,
    duplicateSocket             = -23017
};

/* Error codes from internal IP functions */
enum {
    ipDontFragErr               = -23032,                       /* Packet too large to send w/o fragmenting */
    ipDestDeadErr               = -23033,                       /* destination not responding */
    icmpEchoTimeoutErr          = -23035,                       /* ICMP echo timed-out */
    ipNoFragMemErr              = -23036,                       /* no memory to send fragmented pkt */
    ipRouteErr                  = -23037,                       /* can't route packet off-net */
    nameSyntaxErr               = -23041,
    cacheFault                  = -23042,
    noResultProc                = -23043,
    noNameServer                = -23044,
    authNameErr                 = -23045,
    noAnsErr                    = -23046,
    dnrErr                      = -23047,
    outOfMemory                 = -23048
};


enum {
    BYTES_16WORD                = 2,                            /* bytes per = 16, bit ip word */
    BYTES_32WORD                = 4,                            /* bytes per = 32, bit ip word */
    BYTES_64WORD                = 8                             /* bytes per = 64, bit ip word */
};

/* 8-bit quantity */
typedef UInt8                           b_8;
/* 16-bit quantity */
typedef UInt16                          b_16;
/* 32-bit quantity */
typedef UInt32                          b_32;
/* IP address is 32-bits */
typedef b_32                            ip_addr;

struct ip_addrbytes {
    union {
        b_32                            addr;
        UInt8                           byte[4];
    }                                 a;
};
typedef struct ip_addrbytes             ip_addrbytes;

struct wdsEntry {
    unsigned short                  length;                     /* length of buffer */
    Ptr                             ptr;                        /* pointer to buffer */
};
typedef struct wdsEntry                 wdsEntry;

struct rdsEntry {
    unsigned short                  length;                     /* length of buffer */
    Ptr                             ptr;                        /* pointer to buffer */
};
typedef struct rdsEntry                 rdsEntry;

typedef unsigned long                   BufferPtr;
typedef unsigned long                   StreamPtr;
enum {
    netUnreach                  = 0,
    hostUnreach                 = 1,
    protocolUnreach             = 2,
    portUnreach                 = 3,
    fragReqd                    = 4,
    sourceRouteFailed           = 5,
    timeExceeded                = 6,
    parmProblem                 = 7,
    missingOption               = 8,
    lastICMPMsgType             = 32767
};

typedef unsigned short                  ICMPMsgType;
typedef b_16                            ip_port;

struct ICMPReport {
    StreamPtr                       streamPtr;
    ip_addr                         localHost;
    ip_port                         localPort;
    ip_addr                         remoteHost;
    ip_port                         remotePort;
    short                           reportType;
    unsigned short                  optionalAddlInfo;
    unsigned long                   optionalAddlInfoPtr;
};
typedef struct ICMPReport               ICMPReport;
/* csCode to get our IP address */
enum {
    ipctlGetAddr                = 15
};

typedef struct GetAddrParamBlock        GetAddrParamBlock;
typedef CALLBACK_API_C( void , GetIPIOCompletionProcPtr )(GetAddrParamBlock *iopb);
typedef STACK_UPP_TYPE(GetIPIOCompletionProcPtr)                GetIPIOCompletionUPP;

struct GetAddrParamBlock {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    GetIPIOCompletionUPP            ioCompletion;
    OSErr                           ioResult;
    StringPtr                       ioNamePtr;
    short                           ioVRefNum;
    short                           ioCRefNum;
    short                           csCode;
    ip_addr                         ourAddress;                 /* our IP address */
    long                            ourNetMask;                 /* our IP net mask */
};

/* control codes */
enum {
    ipctlEchoICMP               = 17,                           /* send icmp echo */
    ipctlLAPStats               = 19                            /* get lap stats */
};


typedef struct ICMPParamBlock           ICMPParamBlock;
typedef CALLBACK_API_C( void , IPIOCompletionProcPtr )(ICMPParamBlock *iopb);
typedef STACK_UPP_TYPE(IPIOCompletionProcPtr)                   IPIOCompletionUPP;

struct ICMPParamBlock {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    IPIOCompletionUPP               ioCompletion;
    OSErr                           ioResult;
    StringPtr                       ioNamePtr;
    short                           ioVRefNum;
    short                           ioCRefNum;
    short                           csCode;
    short                           params[11];
    struct {
        unsigned long                   echoRequestOut;         /* time in ticks of when the echo request went out */
        unsigned long                   echoReplyIn;            /* time in ticks of when the reply was received */
        rdsEntry                        echoedData;             /* data received in responce */
        Ptr                             options;
        unsigned long                   userDataPtr;
    }                                 icmpEchoInfo;
};

typedef CALLBACK_API_C( void , ICMPEchoNotifyProcPtr )(ICMPParamBlock *iopb);
typedef STACK_UPP_TYPE(ICMPEchoNotifyProcPtr)                   ICMPEchoNotifyUPP;
typedef struct LAPStats                 LAPStats;

struct IPParamBlock {
    QElem *                         qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    IPIOCompletionUPP               ioCompletion;
    OSErr                           ioResult;
    StringPtr                       ioNamePtr;
    short                           ioVRefNum;
    short                           ioCRefNum;
    short                           csCode;
    union {
        struct {
            ip_addr                         dest;               /* echo to IP address */
            wdsEntry                        data;
            short                           timeout;
            Ptr                             options;
            unsigned short                  optLength;
            ICMPEchoNotifyUPP               icmpCompletion;
            unsigned long                   userDataPtr;
        }                                 IPEchoPB;
        struct {
            LAPStats *                      lapStatsPtr;
        }                                 LAPStatsPB;
    }                                 csParam;
};
typedef struct IPParamBlock             IPParamBlock;


struct nbp_entry {
    ip_addr                         ip_address;                 /* IP address */
    AddrBlock                       at_address;                 /* matching AppleTalk address */
    Boolean                         gateway;                    /* TRUE if entry for a gateway */
    Boolean                         valid;                      /* TRUE if LAP address is valid */
    Boolean                         probing;                    /* TRUE if NBP lookup pending */
    SInt8                           afiller;                    /* Filler for proper byte alignment    */
    long                            age;                        /* ticks since cache entry verified */
    long                            access;                     /* ticks since last access */
    SInt8                           filler[116];                /* for internal use only !!! */
};
typedef struct nbp_entry                nbp_entry;

struct Enet_addr {
    b_16                            en_hi;
    b_32                            en_lo;
};
typedef struct Enet_addr                Enet_addr;

struct arp_entry {
    short                           age;                        /* cache aging field */
    b_16                            protocol;                   /* Protocol type */
    ip_addr                         ip_address;                 /* IP address */
    Enet_addr                       en_address;                 /* matching Ethernet address */
};
typedef struct arp_entry                arp_entry;

union LAPStatsAddrXlation {
    arp_entry *                     arp_table;
    nbp_entry *                     nbp_table;
};
typedef union LAPStatsAddrXlation       LAPStatsAddrXlation;

struct LAPStats {
    short                           ifType;
    char *                          ifString;
    short                           ifMaxMTU;
    long                            ifSpeed;
    short                           ifPhyAddrLength;
    char *                          ifPhysicalAddress;
    LAPStatsAddrXlation             AddrXlation;
    short                           slotNumber;
};

/* number of ARP table entries */
enum {
    ARP_TABLE_SIZE              = 20
};

enum {
    NBP_TABLE_SIZE              = 20,                           /* number of NBP table entries */
    NBP_MAX_NAME_SIZE           = 16 + 10 + 2
};



/* Command codes */
enum {
    TCPCreate                   = 30,
    TCPPassiveOpen              = 31,
    TCPActiveOpen               = 32,
    TCPSend                     = 34,
    TCPNoCopyRcv                = 35,
    TCPRcvBfrReturn             = 36,
    TCPRcv                      = 37,
    TCPClose                    = 38,
    TCPAbort                    = 39,
    TCPStatus                   = 40,
    TCPExtendedStat             = 41,
    TCPRelease                  = 42,
    TCPGlobalInfo               = 43,
    TCPCtlMax                   = 49
};

enum {
    TCPClosing                  = 1,
    TCPULPTimeout               = 2,
    TCPTerminate                = 3,
    TCPDataArrival              = 4,
    TCPUrgent                   = 5,
    TCPICMPReceived             = 6,
    lastEvent                   = 32767
};


typedef unsigned short                  TCPEventCode;
enum {
    TCPRemoteAbort              = 2,
    TCPNetworkFailure           = 3,
    TCPSecPrecMismatch          = 4,
    TCPULPTimeoutTerminate      = 5,
    TCPULPAbort                 = 6,
    TCPULPClose                 = 7,
    TCPServiceError             = 8,
    lastReason                  = 32767
};

typedef unsigned short                  TCPTerminationReason;
typedef CALLBACK_API( void , TCPNotifyProcPtr )(StreamPtr tcpStream, unsigned short eventCode, Ptr userDataPtr, unsigned short terminReason, ICMPReport *icmpMsg);
typedef STACK_UPP_TYPE(TCPNotifyProcPtr)                        TCPNotifyUPP;
typedef unsigned short                  tcp_port;
/* ValidityFlags */
enum {
    timeoutValue                = 0x80,
    timeoutAction               = 0x40,
    typeOfService               = 0x20,
    precedence                  = 0x10
};

/* TOSFlags */
enum {
    lowDelay                    = 0x01,
    throughPut                  = 0x02,
    reliability                 = 0x04
};


struct TCPCreatePB {
    Ptr                             rcvBuff;
    unsigned long                   rcvBuffLen;
    TCPNotifyUPP                    notifyProc;
    Ptr                             userDataPtr;
};
typedef struct TCPCreatePB              TCPCreatePB;

struct TCPOpenPB {
    SInt8                           ulpTimeoutValue;
    SInt8                           ulpTimeoutAction;
    SInt8                           validityFlags;
    SInt8                           commandTimeoutValue;
    ip_addr                         remoteHost;
    tcp_port                        remotePort;
    ip_addr                         localHost;
    tcp_port                        localPort;
    SInt8                           tosFlags;
    SInt8                           precedence;
    Boolean                         dontFrag;
    SInt8                           timeToLive;
    SInt8                           security;
    SInt8                           optionCnt;
    SInt8                           options[40];
    Ptr                             userDataPtr;
};
typedef struct TCPOpenPB                TCPOpenPB;


struct TCPSendPB {
    SInt8                           ulpTimeoutValue;
    SInt8                           ulpTimeoutAction;
    SInt8                           validityFlags;
    Boolean                         pushFlag;
    Boolean                         urgentFlag;
    SInt8                           filler;                     /* Filler for proper byte alignment    */
    Ptr                             wdsPtr;
    unsigned long                   sendFree;
    unsigned short                  sendLength;
    Ptr                             userDataPtr;
};
typedef struct TCPSendPB                TCPSendPB;

/* for receive and return rcv buff calls */
/*   Note: the filler in the following structure is in a different location than */
/*         that specified in the Programmer's Guide.  */

struct TCPReceivePB {
    SInt8                           commandTimeoutValue;
    Boolean                         markFlag;
    Boolean                         urgentFlag;
    SInt8                           filler;                     /* Filler for proper byte alignment  */
    Ptr                             rcvBuff;
    unsigned short                  rcvBuffLen;
    Ptr                             rdsPtr;
    unsigned short                  rdsLength;
    unsigned short                  secondTimeStamp;
    Ptr                             userDataPtr;
};
typedef struct TCPReceivePB             TCPReceivePB;


struct TCPClosePB {
    SInt8                           ulpTimeoutValue;
    SInt8                           ulpTimeoutAction;
    SInt8                           validityFlags;
    SInt8                           filler;                     /* Filler for proper byte alignment    */
    Ptr                             userDataPtr;
};
typedef struct TCPClosePB               TCPClosePB;

struct HistoBucket {
    unsigned short                  value;
    unsigned long                   counter;
};
typedef struct HistoBucket              HistoBucket;
enum {
    NumOfHistoBuckets           = 7
};


struct TCPConnectionStats {
    unsigned long                   dataPktsRcvd;
    unsigned long                   dataPktsSent;
    unsigned long                   dataPktsResent;
    unsigned long                   bytesRcvd;
    unsigned long                   bytesRcvdDup;
    unsigned long                   bytesRcvdPastWindow;
    unsigned long                   bytesSent;
    unsigned long                   bytesResent;
    unsigned short                  numHistoBuckets;
    HistoBucket                     sentSizeHisto[7];
    unsigned short                  lastRTT;
    unsigned short                  tmrSRTT;
    unsigned short                  rttVariance;
    unsigned short                  tmrRTO;
    SInt8                           sendTries;
    SInt8                           sourchQuenchRcvd;
};
typedef struct TCPConnectionStats       TCPConnectionStats;

struct TCPStatusPB {
    SInt8                           ulpTimeoutValue;
    SInt8                           ulpTimeoutAction;
    long                            unused;
    ip_addr                         remoteHost;
    tcp_port                        remotePort;
    ip_addr                         localHost;
    tcp_port                        localPort;
    SInt8                           tosFlags;
    SInt8                           precedence;
    SInt8                           connectionState;
    SInt8                           filler;                     /* Filler for proper byte alignment    */
    unsigned short                  sendWindow;
    unsigned short                  rcvWindow;
    unsigned short                  amtUnackedData;
    unsigned short                  amtUnreadData;
    Ptr                             securityLevelPtr;
    unsigned long                   sendUnacked;
    unsigned long                   sendNext;
    unsigned long                   congestionWindow;
    unsigned long                   rcvNext;
    unsigned long                   srtt;
    unsigned long                   lastRTT;
    unsigned long                   sendMaxSegSize;
    TCPConnectionStats *            connStatPtr;
    Ptr                             userDataPtr;
};
typedef struct TCPStatusPB              TCPStatusPB;

struct TCPAbortPB {
    Ptr                             userDataPtr;
};
typedef struct TCPAbortPB               TCPAbortPB;

struct TCPParam {
    unsigned long                   tcpRtoA;
    unsigned long                   tcpRtoMin;
    unsigned long                   tcpRtoMax;
    unsigned long                   tcpMaxSegSize;
    unsigned long                   tcpMaxConn;
    unsigned long                   tcpMaxWindow;
};
typedef struct TCPParam                 TCPParam;

struct TCPStats {
    unsigned long                   tcpConnAttempts;
    unsigned long                   tcpConnOpened;
    unsigned long                   tcpConnAccepted;
    unsigned long                   tcpConnClosed;
    unsigned long                   tcpConnAborted;
    unsigned long                   tcpOctetsIn;
    unsigned long                   tcpOctetsOut;
    unsigned long                   tcpOctetsInDup;
    unsigned long                   tcpOctetsRetrans;
    unsigned long                   tcpInputPkts;
    unsigned long                   tcpOutputPkts;
    unsigned long                   tcpDupPkts;
    unsigned long                   tcpRetransPkts;
};
typedef struct TCPStats                 TCPStats;

typedef StreamPtr *                     StreamPPtr;

struct TCPGlobalInfoPB {
    TCPParam *                      tcpParamPtr;
    TCPStats *                      tcpStatsPtr;
    StreamPPtr                      tcpCDBTable[1];
    Ptr                             userDataPtr;
    unsigned short                  maxTCPConnections;
};
typedef struct TCPGlobalInfoPB          TCPGlobalInfoPB;
typedef struct TCPiopb                  TCPiopb;
typedef CALLBACK_API_C( void , TCPIOCompletionProcPtr )(TCPiopb *iopb);
typedef STACK_UPP_TYPE(TCPIOCompletionProcPtr)                  TCPIOCompletionUPP;

struct TCPiopb {
    SInt8                           fill12[12];
    TCPIOCompletionUPP              ioCompletion;
    short                           ioResult;
    Ptr                             ioNamePtr;
    short                           ioVRefNum;
    short                           ioCRefNum;
    short                           csCode;
    StreamPtr                       tcpStream;
    union {
        TCPCreatePB                     create;
        TCPOpenPB                       open;
        TCPSendPB                       send;
        TCPReceivePB                    receive;
        TCPClosePB                      close;
        TCPAbortPB                      abort;
        TCPStatusPB                     status;
        TCPGlobalInfoPB                 globalInfo;
    }                                 csParam;
};

enum {
    UDPCreate                   = 20,
    UDPRead                     = 21,
    UDPBfrReturn                = 22,
    UDPWrite                    = 23,
    UDPRelease                  = 24,
    UDPMaxMTUSize               = 25,
    UDPStatus                   = 26,
    UDPMultiCreate              = 27,
    UDPMultiSend                = 28,
    UDPMultiRead                = 29,
    UDPCtlMax                   = 29
};

enum {
    UDPDataArrival              = 1,
    UDPICMPReceived             = 2,
    lastUDPEvent                = 32767
};


typedef unsigned short                  UDPEventCode;
typedef CALLBACK_API( void , UDPNotifyProcPtr )(StreamPtr udpStream, unsigned short eventCode, Ptr userDataPtr, ICMPReport *icmpMsg);
typedef STACK_UPP_TYPE(UDPNotifyProcPtr)                        UDPNotifyUPP;
typedef unsigned short                  udp_port;
/* for create and release calls */

struct UDPCreatePB {
    Ptr                             rcvBuff;
    unsigned long                   rcvBuffLen;
    UDPNotifyUPP                    notifyProc;
    unsigned short                  localPort;
    Ptr                             userDataPtr;
    udp_port                        endingPort;
};
typedef struct UDPCreatePB              UDPCreatePB;

struct UDPSendPB {
    unsigned short                  reserved;
    ip_addr                         remoteHost;
    udp_port                        remotePort;
    Ptr                             wdsPtr;
    Boolean                         checkSum;
    SInt8                           filler;                     /* Filler for proper byte alignment    */
    unsigned short                  sendLength;
    Ptr                             userDataPtr;
    udp_port                        localPort;
};
typedef struct UDPSendPB                UDPSendPB;
/* for receive and buffer return calls */

struct UDPReceivePB {
    unsigned short                  timeOut;
    ip_addr                         remoteHost;
    udp_port                        remotePort;
    Ptr                             rcvBuff;
    unsigned short                  rcvBuffLen;
    unsigned short                  secondTimeStamp;
    Ptr                             userDataPtr;
    ip_addr                         destHost;                   /* only for use with multi rcv */
    udp_port                        destPort;                   /* only for use with multi rcv */
};
typedef struct UDPReceivePB             UDPReceivePB;

struct UDPMTUPB {
    unsigned short                  mtuSize;
    ip_addr                         remoteHost;
    Ptr                             userDataPtr;
};
typedef struct UDPMTUPB                 UDPMTUPB;
typedef struct UDPiopb                  UDPiopb;
typedef CALLBACK_API_C( void , UDPIOCompletionProcPtr )(UDPiopb *iopb);
typedef STACK_UPP_TYPE(UDPIOCompletionProcPtr)                  UDPIOCompletionUPP;

struct UDPiopb {
    SInt8                           fill12[12];
    UDPIOCompletionUPP              ioCompletion;
    short                           ioResult;
    Ptr                             ioNamePtr;
    short                           ioVRefNum;
    short                           ioCRefNum;
    short                           csCode;
    StreamPtr                       udpStream;
    union {
        UDPCreatePB                     create;
        UDPSendPB                       send;
        UDPReceivePB                    receive;
        UDPMTUPB                        mtu;
    }                                 csParam;
};

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GetIPIOCompletionUPP)
    NewGetIPIOCompletionUPP        (GetIPIOCompletionProcPtr userRoutine);

    EXTERN_API(IPIOCompletionUPP)
    NewIPIOCompletionUPP           (IPIOCompletionProcPtr   userRoutine);

    EXTERN_API(ICMPEchoNotifyUPP)
    NewICMPEchoNotifyUPP           (ICMPEchoNotifyProcPtr   userRoutine);

    EXTERN_API(TCPNotifyUPP)
    NewTCPNotifyUPP                (TCPNotifyProcPtr        userRoutine);

    EXTERN_API(TCPIOCompletionUPP)
    NewTCPIOCompletionUPP          (TCPIOCompletionProcPtr  userRoutine);

    EXTERN_API(UDPNotifyUPP)
    NewUDPNotifyUPP                (UDPNotifyProcPtr        userRoutine);

    EXTERN_API(UDPIOCompletionUPP)
    NewUDPIOCompletionUPP          (UDPIOCompletionProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeGetIPIOCompletionUPP    (GetIPIOCompletionUPP    userUPP);

    EXTERN_API(void)
    DisposeIPIOCompletionUPP       (IPIOCompletionUPP       userUPP);

    EXTERN_API(void)
    DisposeICMPEchoNotifyUPP       (ICMPEchoNotifyUPP       userUPP);

    EXTERN_API(void)
    DisposeTCPNotifyUPP            (TCPNotifyUPP            userUPP);

    EXTERN_API(void)
    DisposeTCPIOCompletionUPP      (TCPIOCompletionUPP      userUPP);

    EXTERN_API(void)
    DisposeUDPNotifyUPP            (UDPNotifyUPP            userUPP);

    EXTERN_API(void)
    DisposeUDPIOCompletionUPP      (UDPIOCompletionUPP      userUPP);

    EXTERN_API(void)
    InvokeGetIPIOCompletionUPP     (GetAddrParamBlock *     iopb,
                                    GetIPIOCompletionUPP    userUPP);

    EXTERN_API(void)
    InvokeIPIOCompletionUPP        (ICMPParamBlock *        iopb,
                                    IPIOCompletionUPP       userUPP);

    EXTERN_API(void)
    InvokeICMPEchoNotifyUPP        (ICMPParamBlock *        iopb,
                                    ICMPEchoNotifyUPP       userUPP);

    EXTERN_API(void)
    InvokeTCPNotifyUPP             (StreamPtr               tcpStream,
                                    unsigned short          eventCode,
                                    Ptr                     userDataPtr,
                                    unsigned short          terminReason,
                                    ICMPReport *            icmpMsg,
                                    TCPNotifyUPP            userUPP);

    EXTERN_API(void)
    InvokeTCPIOCompletionUPP       (TCPiopb *               iopb,
                                    TCPIOCompletionUPP      userUPP);

    EXTERN_API(void)
    InvokeUDPNotifyUPP             (StreamPtr               udpStream,
                                    unsigned short          eventCode,
                                    Ptr                     userDataPtr,
                                    ICMPReport *            icmpMsg,
                                    UDPNotifyUPP            userUPP);

    EXTERN_API(void)
    InvokeUDPIOCompletionUPP       (UDPiopb *               iopb,
                                    UDPIOCompletionUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGetIPIOCompletionProcInfo = 0x000000C1 };             /* no_return_value Func(4_bytes) */
    enum { uppIPIOCompletionProcInfo = 0x000000C1 };                /* no_return_value Func(4_bytes) */
    enum { uppICMPEchoNotifyProcInfo = 0x000000C1 };                /* no_return_value Func(4_bytes) */
    enum { uppTCPNotifyProcInfo = 0x0000EEC0 };                     /* pascal no_return_value Func(4_bytes, 2_bytes, 4_bytes, 2_bytes, 4_bytes) */
    enum { uppTCPIOCompletionProcInfo = 0x000000C1 };               /* no_return_value Func(4_bytes) */
    enum { uppUDPNotifyProcInfo = 0x00003EC0 };                     /* pascal no_return_value Func(4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppUDPIOCompletionProcInfo = 0x000000C1 };               /* no_return_value Func(4_bytes) */
    #define NewGetIPIOCompletionUPP(userRoutine)                    (GetIPIOCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGetIPIOCompletionProcInfo, GetCurrentArchitecture())
    #define NewIPIOCompletionUPP(userRoutine)                       (IPIOCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIPIOCompletionProcInfo, GetCurrentArchitecture())
    #define NewICMPEchoNotifyUPP(userRoutine)                       (ICMPEchoNotifyUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppICMPEchoNotifyProcInfo, GetCurrentArchitecture())
    #define NewTCPNotifyUPP(userRoutine)                            (TCPNotifyUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTCPNotifyProcInfo, GetCurrentArchitecture())
    #define NewTCPIOCompletionUPP(userRoutine)                      (TCPIOCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTCPIOCompletionProcInfo, GetCurrentArchitecture())
    #define NewUDPNotifyUPP(userRoutine)                            (UDPNotifyUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppUDPNotifyProcInfo, GetCurrentArchitecture())
    #define NewUDPIOCompletionUPP(userRoutine)                      (UDPIOCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppUDPIOCompletionProcInfo, GetCurrentArchitecture())
    #define DisposeGetIPIOCompletionUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeIPIOCompletionUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposeICMPEchoNotifyUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposeTCPNotifyUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define DisposeTCPIOCompletionUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeUDPNotifyUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define DisposeUDPIOCompletionUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeGetIPIOCompletionUPP(iopb, userUPP)               CALL_ONE_PARAMETER_UPP((userUPP), uppGetIPIOCompletionProcInfo, (iopb))
    #define InvokeIPIOCompletionUPP(iopb, userUPP)                  CALL_ONE_PARAMETER_UPP((userUPP), uppIPIOCompletionProcInfo, (iopb))
    #define InvokeICMPEchoNotifyUPP(iopb, userUPP)                  CALL_ONE_PARAMETER_UPP((userUPP), uppICMPEchoNotifyProcInfo, (iopb))
    #define InvokeTCPNotifyUPP(tcpStream, eventCode, userDataPtr, terminReason, icmpMsg, userUPP)  CALL_FIVE_PARAMETER_UPP((userUPP), uppTCPNotifyProcInfo, (tcpStream), (eventCode), (userDataPtr), (terminReason), (icmpMsg))
    #define InvokeTCPIOCompletionUPP(iopb, userUPP)                 CALL_ONE_PARAMETER_UPP((userUPP), uppTCPIOCompletionProcInfo, (iopb))
    #define InvokeUDPNotifyUPP(udpStream, eventCode, userDataPtr, icmpMsg, userUPP)  CALL_FOUR_PARAMETER_UPP((userUPP), uppUDPNotifyProcInfo, (udpStream), (eventCode), (userDataPtr), (icmpMsg))
    #define InvokeUDPIOCompletionUPP(iopb, userUPP)                 CALL_ONE_PARAMETER_UPP((userUPP), uppUDPIOCompletionProcInfo, (iopb))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGetIPIOCompletionProc(userRoutine)                   NewGetIPIOCompletionUPP(userRoutine)
#define NewIPIOCompletionProc(userRoutine)                      NewIPIOCompletionUPP(userRoutine)
#define NewICMPEchoNotifyProc(userRoutine)                      NewICMPEchoNotifyUPP(userRoutine)
#define NewTCPNotifyProc(userRoutine)                           NewTCPNotifyUPP(userRoutine)
#define NewTCPIOCompletionProc(userRoutine)                     NewTCPIOCompletionUPP(userRoutine)
#define NewUDPNotifyProc(userRoutine)                           NewUDPNotifyUPP(userRoutine)
#define NewUDPIOCompletionProc(userRoutine)                     NewUDPIOCompletionUPP(userRoutine)
#define CallGetIPIOCompletionProc(userRoutine, iopb)            InvokeGetIPIOCompletionUPP(iopb, userRoutine)
#define CallIPIOCompletionProc(userRoutine, iopb)               InvokeIPIOCompletionUPP(iopb, userRoutine)
#define CallICMPEchoNotifyProc(userRoutine, iopb)               InvokeICMPEchoNotifyUPP(iopb, userRoutine)
#define CallTCPNotifyProc(userRoutine, tcpStream, eventCode, userDataPtr, terminReason, icmpMsg) InvokeTCPNotifyUPP(tcpStream, eventCode, userDataPtr, terminReason, icmpMsg, userRoutine)
#define CallTCPIOCompletionProc(userRoutine, iopb)              InvokeTCPIOCompletionUPP(iopb, userRoutine)
#define CallUDPNotifyProc(userRoutine, udpStream, eventCode, userDataPtr, icmpMsg) InvokeUDPNotifyUPP(udpStream, eventCode, userDataPtr, icmpMsg, userRoutine)
#define CallUDPIOCompletionProc(userRoutine, iopb)              InvokeUDPIOCompletionUPP(iopb, userRoutine)


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

#endif /* __MACTCP__ */

