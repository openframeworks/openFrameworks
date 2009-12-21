/*
     File:       ATA.h
 
     Contains:   ATA (PC/AT Attachment) Interfaces
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ATA__
#define __ATA__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
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


/* This is the structure used for the AT Interface core routines below */
enum {
    kATATrap                    = 0xAAF1,                       /* Manager trap number <This should be defined in Traps.h>*/
    kATAPBVers1                 = 0x01,                         /* parameter block version number 1*/
    kATAPBVers2                 = 0x02,                         /* parameter block version number for structures*/
    kATAPBVers3                 = 0x03,                         /* parameter block version for ATA times*/
    kATADefaultBlockSize        = 512                           /* default block size*/
};

/* Used to determine the presence of traps*/
enum {
    kFSMTrap                    = 0xAC,
    mDQEChanged                 = 1                             /* DQE has changed */
};

/* Task file definition ooo Error Register ooo*/
enum {
    bATABadBlock                = 7,                            /* bit number of bad block error bit*/
    bATAUncorrectable           = 6,                            /* bit number of uncorrectable error bit*/
    bATAMediaChanged            = 5,                            /* bit number of media changed indicator*/
    bATAIDNotFound              = 4,                            /* bit number of ID not found error bit*/
    bATAMediaChangeReq          = 3,                            /* bit number of media changed request*/
    bATACommandAborted          = 2,                            /* bit number of command abort bit*/
    bATATrack0NotFound          = 1,                            /* bit number of track not found*/
    bATAAddressNotFound         = 0,                            /* bit number of address mark not found*/
    mATABadBlock                = 1 << bATABadBlock,            /* Bad Block Detected*/
    mATAUncorrectable           = 1 << bATAUncorrectable,       /* Uncorrectable Data Error*/
    mATAMediaChanged            = 1 << bATAMediaChanged,        /* Media Changed Indicator (for removable)*/
    mATAIDNotFound              = 1 << bATAIDNotFound,          /* ID Not Found*/
    mATAMediaChangeReq          = 1 << bATAMediaChangeReq,      /* Media Change Requested (NOT IMPLEMENTED)*/
    mATACommandAborted          = 1 << bATACommandAborted,      /* Aborted Command*/
    mATATrack0NotFound          = 1 << bATATrack0NotFound,      /* Track 0 Not Found*/
    mATAAddressNotFound         = 1 << bATAAddressNotFound      /* Address Mark Not Found*/
};

/* Task file definition ooo Features register ooo*/
enum {
    bATAPIuseDMA                = 0,                            /* bit number of useDMA bit (ATAPI)*/
    mATAPIuseDMA                = 1 << bATAPIuseDMA
};

/* Task file definition ooo ataTFSDH Register ooo*/
enum {
    mATAHeadNumber              = 0x0F,                         /* Head Number (bits 0-3) */
    mATASectorSize              = 0xA0,                         /* bit 7=1; bit 5 = 01 (512 sector size) <DP4>*/
    mATADriveSelect             = 0x10,                         /* Drive (0 = master, 1 = slave) */
    mATALBASelect               = 0x40                          /* LBA mode bit (0 = chs, 1 = LBA)*/
};

/* Task file definition ooo Status Register ooo*/
enum {
    bATABusy                    = 7,                            /* bit number of BSY bit*/
    bATADriveReady              = 6,                            /* bit number of drive ready bit*/
    bATAWriteFault              = 5,                            /* bit number of write fault bit*/
    bATASeekComplete            = 4,                            /* bit number of seek complete bit*/
    bATADataRequest             = 3,                            /* bit number of data request bit*/
    bATADataCorrected           = 2,                            /* bit number of data corrected bit*/
    bATAIndex                   = 1,                            /* bit number of index mark*/
    bATAError                   = 0,                            /* bit number of error bit*/
    mATABusy                    = 1 << bATABusy,                /* Unit is busy*/
    mATADriveReady              = 1 << bATADriveReady,          /* Unit is ready*/
    mATAWriteFault              = 1 << bATAWriteFault,          /* Unit has a write fault condition*/
    mATASeekComplete            = 1 << bATASeekComplete,        /* Unit seek complete*/
    mATADataRequest             = 1 << bATADataRequest,         /* Unit data request*/
    mATADataCorrected           = 1 << bATADataCorrected,       /* Data corrected*/
    mATAIndex                   = 1 << bATAIndex,               /* Index mark - NOT USED*/
    mATAError                   = 1 << bATAError                /* Error condition - see error register*/
};

/* Task file definition ooo Device Control Register ooo*/
enum {
    bATADCROne                  = 3,                            /* bit number of always one bit*/
    bATADCRReset                = 2,                            /* bit number of reset bit*/
    bATADCRnIntEnable           = 1,                            /* bit number of interrupt disable*/
    mATADCROne                  = 1 << bATADCROne,              /* always one bit*/
    mATADCRReset                = 1 << bATADCRReset,            /* Reset (1 = reset)*/
    mATADCRnIntEnable           = 1 << bATADCRnIntEnable        /* Interrupt Disable(0 = enabled)*/
};

/* ATA Command Opcode definition*/
enum {
    kATAcmdWORetry              = 0x01,                         /* Without I/O retry option*/
    kATAcmdNOP                  = 0x0000,                       /* NOP operation - media detect*/
    kATAcmdRecal                = 0x0010,                       /* Recalibrate command */
    kATAcmdRead                 = 0x0020,                       /* Read command */
    kATAcmdReadLong             = 0x0022,                       /* Read Long command*/
    kATAcmdWrite                = 0x0030,                       /* Write command */
    kATAcmdWriteLong            = 0x0032,                       /* Write Long*/
    kATAcmdWriteVerify          = 0x003C,                       /* Write verify*/
    kATAcmdReadVerify           = 0x0040,                       /* Read Verify command */
    kATAcmdFormatTrack          = 0x0050,                       /* Format Track command */
    kATAcmdSeek                 = 0x0070,                       /* Seek command */
    kATAcmdDiagnostic           = 0x0090,                       /* Drive Diagnostic command */
    kATAcmdInitDrive            = 0x0091,                       /* Init drive parameters command */
    kATAcmdReadMultiple         = 0x00C4,                       /* Read multiple*/
    kATAcmdWriteMultiple        = 0x00C5,                       /* Write multiple*/
    kATAcmdSetRWMultiple        = 0x00C6,                       /* Set Multiple for Read/Write Multiple*/
    kATAcmdReadDMA              = 0x00C8,                       /* Read DMA (with retries)*/
    kATAcmdWriteDMA             = 0x00CA,                       /* Write DMA (with retries)*/
    kATAcmdMCAcknowledge        = 0x00DB,                       /* Acknowledge media change - removable*/
    kATAcmdDoorLock             = 0x00DE,                       /* Door lock*/
    kATAcmdDoorUnlock           = 0x00DF,                       /* Door unlock*/
    kATAcmdStandbyImmed         = 0x00E0,                       /* Standby Immediate*/
    kATAcmdIdleImmed            = 0x00E1,                       /* Idle Immediate*/
    kATAcmdStandby              = 0x00E2,                       /* Standby*/
    kATAcmdIdle                 = 0x00E3,                       /* Idle*/
    kATAcmdReadBuffer           = 0x00E4,                       /* Read sector buffer command */
    kATAcmdCheckPowerMode       = 0x00E5,                       /* Check power mode command   <04/04/94>*/
    kATAcmdSleep                = 0x00E6,                       /* Sleep*/
    kATAcmdWriteBuffer          = 0x00E8,                       /* Write sector buffer command */
    kATAcmdWriteSame            = 0x00E9,                       /* Write same data to multiple sectors*/
    kATAcmdDriveIdentify        = 0x00EC,                       /* Identify Drive command */
    kATAcmdMediaEject           = 0x00ED,                       /* Media Eject*/
    kATAcmdSetFeatures          = 0x00EF                        /* Set Features*/
};

/* Set feature command opcodes*/
enum {
    kATAEnableWriteCache        = 0x02,                         /*       Enable write cache*/
    kATASetTransferMode         = 0x03,                         /*       Set transfer mode*/
    kATASetPIOMode              = 0x08,                         /*       PIO Flow Control Tx Mode bit*/
    kATAEnableECC               = 0x88,                         /*       ECC enable*/
    kATAEnableRetry             = 0x99,                         /*       Retry enable*/
    kATAEnableReadAhead         = 0xAA                          /*       Read look-ahead enable*/
};

/*
  --------------------------------------------------------------------------------
   enums for dealing with device IDs
*/

enum {
    kATABusIDMask               = 0x000000FF,
    kATADeviceIDMask            = 0x0000FF00,
    kATADeviceIDClippingMask    = 0x0000FFFF,
    kMinBusID                   = 0x00000000,
    kMaxBusID                   = 0x000000FE
};

enum {
    kATAStartIterateDeviceID    = 0xFFFF,
    kATAEndIterateDeviceID      = 0xFF
};

/*--------------------------------------------------------------------------------*/
/* Device Register Images  (8 bytes) */

struct ataTaskFile {
    UInt8                           ataTFFeatures;              /* <-> Error(R) or ataTFFeatures(W) register image */
    UInt8                           ataTFCount;                 /* <-> Sector count/remaining */
    UInt8                           ataTFSector;                /* <-> Sector start/finish */
    UInt8                           ataTFReserved;              /* reserved             */
    UInt16                          ataTFCylinder;              /* <-> ataTFCylinder (Big endian) */
    UInt8                           ataTFSDH;                   /* <-> ataTFSDH register image*/
    UInt8                           ataTFCommand;               /* <-> Status(R) or Command(W) register image */
};
typedef struct ataTaskFile              ataTaskFile;
/* ATA Manager Function Code Definition*/
enum {
    kATAMgrNOP                  = 0x00,                         /* No Operation*/
    kATAMgrExecIO               = 0x01,                         /* Execute ATA I/O*/
    kATAMgrBusInquiry           = 0x03,                         /* Bus Inquiry*/
    kATAMgrQRelease             = 0x04,                         /* I/O Queue Release*/
    kATAMgrAbort                = 0x10,                         /* Abort command*/
    kATAMgrBusReset             = 0x11,                         /* Reset ATA bus*/
    kATAMgrRegAccess            = 0x12,                         /* Register Access*/
    kATAMgrDriveIdentify        = 0x13,                         /* Drive Identify        <DP03/10/94>*/
    kATAMgrDriverLoad           = 0x82,                         /* Load driver from either Media, ROM, etc.*/
    kATAMgrDriveRegister        = 0x85,                         /* Register a driver   <4/18/94>*/
    kATAMgrFindDriverRefnum     = 0x86,                         /* lookup a driver refnum <4/18/94>*/
    kATAMgrRemoveDriverRefnum   = 0x87,                         /* De-register a driver   <4/18/94>*/
    kATAMgrModifyEventMask      = 0x88,                         /* Modify driver event mask*/
    kATAMgrDriveEject           = 0x89,                         /* Eject the drive       <8/1/94>*/
    kATAMgrGetDrvConfiguration  = 0x8A,                         /* Get device configuration   <8/6/94>*/
    kATAMgrSetDrvConfiguration  = 0x8B,                         /* Set device configuration <8/6/94>*/
    kATAMgrGetLocationIcon      = 0x8C,                         /* Get card location icon <SM4>*/
    kATAMgrManagerInquiry       = 0x90,                         /* Manager Inquiry*/
    kATAMgrManagerInit          = 0x91,                         /* Manager initialization*/
    kATAMgrManagerShutdown      = 0x92,                         /* Manager ShutDown*/
                                                                /* note: functions 0x93 to 0x97 are reserved*/
    kATAMgrFindSpecialDriverRefnum = 0x98,                      /* lookup a driver refnum; driverloader,notify-all or ROM driver.*/
    kATAMgrNextAvailable        = 0x99
};

/* used in the ataDrvrFlags field for kATAMgrDriveRegister,kATAMgrRemoveDriverRefnum & kATAMgrFindSpecialDriverRefnum*/
enum {
    kATANotifyAllDriver         = 0,                            /* Notify-All driver*/
    kATADriverLoader            = 1,                            /* Driver loader driver     */
    kATAROMDriver               = 2                             /* ROM driver*/
};

/* 'ATAFlags' field of the PB header definition*/
enum {
    bATAFlagUseConfigSpeed      = 15,                           /* bit number of use default speed flag*/
    bATAFlagByteSwap            = 14,                           /* bit number of byte swap flag*/
    bATAFlagIORead              = 13,                           /* bit number of I/O read flag*/
    bATAFlagIOWrite             = 12,                           /* bit number of I/O write flag*/
    bATAFlagImmediate           = 11,                           /* bit number of immediate flag*/
    bATAFlagQLock               = 10,                           /* bit number of que lock on error*/
    bATAFlagReserved1           = 9,                            /* reserved*/
    bATAFlagUseScatterGather    = 8,                            /* bit numbers of scatter gather*/
    bATAFlagUseDMA              = 7,                            /* bit number of use DMA flag*/
    bATAFlagProtocolATAPI       = 5,                            /* bit number of ATAPI protocol*/
    bATAFlagReserved2           = 4,                            /* reserved*/
    bATAFlagTFRead              = 3,                            /* bit number of register update*/
    bATAFlagLEDEnable           = 0,                            /* bit number of LED enable*/
    mATAFlagUseConfigSpeed      = 1 << bATAFlagUseConfigSpeed,
    mATAFlagByteSwap            = 1 << bATAFlagByteSwap,        /* Swap data bytes (read - after; write - before)*/
    mATAFlagIORead              = 1 << bATAFlagIORead,          /* Read (in) operation*/
    mATAFlagIOWrite             = 1 << bATAFlagIOWrite,         /* Write (out) operation*/
    mATAFlagImmediate           = 1 << bATAFlagImmediate,       /* Head of Que; Immediate operation*/
    mATAFlagQLock               = 1 << bATAFlagQLock,           /* Manager queue lock on error (freeze the queue)*/
    mATAFlagUseScatterGather    = 1 << bATAFlagUseScatterGather, /* Scatter gather enable*/
    mATAFlagUseDMA              = 1 << bATAFlagUseDMA,
    mATAFlagProtocolATAPI       = 1 << bATAFlagProtocolATAPI,   /* ATAPI protocol indicator*/
    mATAFlagTFRead              = 1 << bATAFlagTFRead,          /* update reg block request upon detection of an error*/
    mATAFlagLEDEnable           = 1 << bATAFlagLEDEnable        /* socket LED enable*/
};

/* These are legacy ATAFlags definitions, which will go away in the future*/
enum {
    bATAFlagScatterGather1      = bATAFlagReserved1,            /* 9*/
    bATAFlagScatterGather0      = bATAFlagUseScatterGather,     /* 8*/
    bATAFlagProtocol1           = bATAFlagProtocolATAPI,        /* 5*/
    bATAFlagProtocol0           = bATAFlagReserved2,            /* 4*/
    mATAFlagScatterGather1      = 1 << bATAFlagScatterGather1,
    mATAFlagScatterGather0      = mATAFlagUseScatterGather,
    mATAFlagScatterGathers      = mATAFlagScatterGather1 + mATAFlagScatterGather0,
    mATAFlagProtocol1           = mATAFlagProtocolATAPI,
    mATAFlagProtocol0           = 1 << bATAFlagProtocol0,
    mATAFlagProtocols           = mATAFlagProtocol1 + mATAFlagProtocol0
};


typedef CALLBACK_API( void , ATACallbackProcPtr )(void *ataPB);
typedef STACK_UPP_TYPE(ATACallbackProcPtr)                      ATACallbackUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ATACallbackUPP)
    NewATACallbackUPP              (ATACallbackProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeATACallbackUPP          (ATACallbackUPP          userUPP);

    EXTERN_API(void)
    InvokeATACallbackUPP           (void *                  ataPB,
                                    ATACallbackUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppATACallbackProcInfo = 0x000000C0 };                   /* pascal no_return_value Func(4_bytes) */
    #define NewATACallbackUPP(userRoutine)                          (ATACallbackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppATACallbackProcInfo, GetCurrentArchitecture())
    #define DisposeATACallbackUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeATACallbackUPP(ataPB, userUPP)                    CALL_ONE_PARAMETER_UPP((userUPP), uppATACallbackProcInfo, (ataPB))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewATACallbackProc(userRoutine)                         NewATACallbackUPP(userRoutine)
#define CallATACallbackProc(userRoutine, ataPB)                 InvokeATACallbackUPP(ataPB, userRoutine)
/*
   
   structure which defines the ATA bus/device ID, part of the Device 0/1 Software Guide
   see <http://developer.apple.com/techpubs/hardware/Developer_Notes/System_Software/ATA_Device_Zero_One.pdf>
   p 19 :   Although ataPBDeviceID remains defined as a 32-bit number, drivers and applications
            can typecast it to the ataDeviceID structure to determine the bus number and device number
            for a specific ATA or ATAPI device
*/

struct ataDeviceID {
    UInt16                          Reserved;                   /* The upperword is reserved (0)*/
    UInt8                           devNum;                     /* device number (0 or 1)*/
    UInt8                           busNum;                     /* bus number*/
};
typedef struct ataDeviceID              ataDeviceID;

struct ataPBHeader {
                                                                /* Start of cloned common header ataPBHdr */
    struct ataPBHeader *            ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
};
typedef struct ataPBHeader              ataPBHeader;
/* data request entry structure (16 bytes)*/

struct IOBlock {
    UInt8 *                         ataPBBuffer;                /* -->: Data buffer pointer*/
    UInt32                          ataPBByteCount;             /* -->: Data transfer length in bytes*/
};
typedef struct IOBlock                  IOBlock;
/*
   For ATAPI devices the ExtendedPB field is a pointer to the Command Packet
   record which exists of an array of words structured as follows...  <06/15/94>
*/

struct ATAPICmdPacket {
    SInt16                          atapiPacketSize;            /* Size of command packet in bytes    <06/15/94>*/
    SInt16                          atapiCommandByte[8];        /* The command packet itself  <06/15/94>*/
};
typedef struct ATAPICmdPacket           ATAPICmdPacket;
/* Manager parameter block structure (96 bytes)*/

struct ataIOPB {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    SInt8                           ataPBStatusRegister;        /* <--: Last ATA status image*/
    SInt8                           ataPBErrorRegister;         /* <--: Last ATA error image-valid if lsb of Status set*/
    SInt16                          ataPBReserved5;             /* Reserved*/
    UInt32                          ataPBLogicalBlockSize;      /* -->: Blind transfer size per interrupt (Logical block size)*/
    UInt8 *                         ataPBBuffer;                /* -->: Data buffer pointer*/
    UInt32                          ataPBByteCount;             /* -->: Data transfer length in bytes*/
    UInt32                          ataPBActualTxCount;         /* <--: Actual transfer count*/
    UInt32                          ataPBReserved6;             /* Reserved*/
    ataTaskFile                     ataPBTaskFile;              /* <->:   Device register images*/
    ATAPICmdPacket *                ataPBPacketPtr;             /* -->: ATAPI packet command block pointer (valid with ATAPI bit set)*/
    SInt16                          ataPBReserved7[6];          /* Reserved for future expansion*/
};
typedef struct ataIOPB                  ataIOPB;
/* Parameter block structure for bus and Manager inquiry command*/
/* Manager parameter block structure*/

struct ataBusInquiry {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    UInt16                          ataEngineCount;             /* <--: TBD; zero for now*/
    UInt16                          ataReserved1;               /* Reserved*/
    UInt32                          ataDataTypes;               /* <--: TBD; zero for now*/
    UInt16                          ataIOpbSize;                /* <--: Size of ATA IO PB*/
    UInt16                          ataMaxIOpbSize;             /* <--: TBD; zero for now*/
    UInt32                          ataFeatureFlags;            /* <--: TBD*/
    UInt8                           ataVersionNum;              /* <--: Version number for the HBA*/
    UInt8                           ataHBAInquiry;              /* <--: TBD; zero for now*/
    UInt16                          ataReserved2;               /* Reserved*/
    UInt32                          ataHBAPrivPtr;              /* <--: Ptr to HBA private data area*/
    UInt32                          ataHBAPrivSize;             /* <--: Size of HBA private data area*/
    UInt32                          ataAsyncFlags;              /* <--: Event capability for callback*/
    UInt8                           ataPIOModes;                /* <--: PIO modes supported (bit-significant)*/
    UInt8                           ataUltraDMAModes;           /* <--: Ultra DMA modes supported (b-sig)*/
    UInt8                           ataSingleDMAModes;          /* <--: Single Word DMA modes supported (b-sig)   */
    UInt8                           ataMultiDMAModes;           /* <--: Multiword DMA modes supported (b-sig)*/
    UInt32                          ataReserved4[4];            /* Reserved*/
    SInt8                           ataReserved5[16];           /* TBD*/
    SInt8                           ataHBAVendor[16];           /* <--: Vendor ID of the HBA*/
    SInt8                           ataContrlFamily[16];        /* <--: Family of ATA Controller*/
    SInt8                           ataContrlType[16];          /* <--: Model number of controller*/
    SInt8                           ataXPTversion[4];           /* <--: version number of XPT*/
    SInt8                           ataReserved6[4];            /* Reserved*/
    NumVersion                      ataHBAversion;              /* <--: version number of HBA*/
    UInt8                           ataHBAslotType;             /* <--: type of slot*/
    UInt8                           ataHBAslotNum;              /* <--: slot number of the HBA*/
    UInt16                          ataReserved7;               /* Reserved*/
    UInt32                          ataReserved8;               /* Reserved*/
};
typedef struct ataBusInquiry            ataBusInquiry;
/* Manager parameter block structure*/

struct ataMgrInquiry {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    NumVersion                      ataMgrVersion;              /* Manager Version information*/
    UInt8                           ataMgrPBVers;               /* <--: Manager PB version number supported*/
    UInt8                           Reserved1;                  /* Reserved*/
    UInt16                          ataBusCnt;                  /* <--: Number of ATA buses in the system*/
    UInt16                          ataDevCnt;                  /* <--: Total number of ATA devices detected*/
    UInt8                           ataPioModes;                /* <--: Maximum Programmed I/O speed mode supported*/
    UInt8                           Reserved2;                  /* Reserved*/
    UInt16                          ataIOClkResolution;         /* <--: IO Clock resolution in nsec (Not supported)*/
    UInt8                           ataSingleDMAModes;          /* <--: Single Word DMA modes supported   */
    UInt8                           ataMultiDMAModes;           /* <--: Multiword DMA modes supported*/
    SInt16                          Reserved[16];               /* Reserved for future expansion*/
};
typedef struct ataMgrInquiry            ataMgrInquiry;
/* Parameter block structure for Abort command*/
/* Manager parameter block structure*/

struct ataAbort {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    ataIOPB *                       ataAbortPB;                 /* -->: Parameter block to be aborted*/
    SInt16                          Reserved[22];               /* Reserved for future expansion*/
};
typedef struct ataAbort                 ataAbort;
/* Manager parameter block structure*/

struct ATAEventRec {
    UInt16                          ataEventCode;               /* --> ATA event code*/
    UInt16                          ataPhysicalID;              /* --> Physical drive reference*/
    SInt32                          ataDrvrContext;             /* Context pointer saved by driver*/
    UInt32                          ataMarker;                  /* Always 'LOAD'*/
    UInt32                          ataEventRecVersion;         /* Version number of this data structure*/
    UInt32                          ataDeviceType;              /* Device type on bus (valid for load driver only)*/
    UInt16                          ataRefNum;                  /* RefNum of driver (valid for remove driver only)*/
};
typedef struct ATAEventRec              ATAEventRec;
typedef ATAEventRec *                   ATAEventRecPtr;
typedef CALLBACK_API( SInt16 , ATAClientProcPtr )(ATAEventRecPtr ataERPtr);
typedef STACK_UPP_TYPE(ATAClientProcPtr)                        ATAClientUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ATAClientUPP)
    NewATAClientUPP                (ATAClientProcPtr        userRoutine);

    EXTERN_API(void)
    DisposeATAClientUPP            (ATAClientUPP            userUPP);

    EXTERN_API(SInt16)
    InvokeATAClientUPP             (ATAEventRecPtr          ataERPtr,
                                    ATAClientUPP            userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppATAClientProcInfo = 0x000000E0 };                     /* pascal 2_bytes Func(4_bytes) */
    #define NewATAClientUPP(userRoutine)                            (ATAClientUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppATAClientProcInfo, GetCurrentArchitecture())
    #define DisposeATAClientUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define InvokeATAClientUPP(ataERPtr, userUPP)                   (SInt16)CALL_ONE_PARAMETER_UPP((userUPP), uppATAClientProcInfo, (ataERPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewATAClientProc(userRoutine)                           NewATAClientUPP(userRoutine)
#define CallATAClientProc(userRoutine, ataERPtr)                InvokeATAClientUPP(ataERPtr, userRoutine)
/* Parameter block structure for Driver Register command*/
/* Manager parameter block structure*/

struct ataDrvrRegister {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    SInt16                          ataDrvrRefNum;              /* <->: Driver reference number*/
    UInt16                          ataDrvrFlags;               /* -->: 1 = loader driver if ataPBDeviceID = -1 {PB2}*/
    UInt16                          ataDeviceNextID;            /* <--: used to specified the next drive ID*/
    SInt16                          ataDrvrLoadPriv;            /* Driver loader private storage*/
    ATAClientUPP                    ataEventHandler;            /* <->: Pointer to ATA event callback routine {PB2}*/
    SInt32                          ataDrvrContext;             /* <->: Context data saved by driver {PB2}*/
    SInt32                          ataEventMask;               /* <->: Set to 1 for notification of event {PB2}*/
    SInt16                          Reserved[14];               /* Reserved for future expansion - from [21] {PB2}*/
};
typedef struct ataDrvrRegister          ataDrvrRegister;
/* Parameter block structure for Modify driver event mask command*/

struct ataModifyEventMask {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    SInt32                          ataModifiedEventMask;       /* -->: new event mask value*/
    SInt16                          Reserved[22];               /* Reserved for future expansion*/
};
typedef struct ataModifyEventMask       ataModifyEventMask;
/* 'ataRegMask' field of the ataRegAccess definition*/
enum {
    bATAAltSDevCValid           = 14,                           /* bit number of alternate status/device cntrl valid bit*/
    bATAStatusCmdValid          = 7,                            /* bit number of status/command valid bit*/
    bATASDHValid                = 6,                            /* bit number of ataTFSDH valid bit*/
    bATACylinderHiValid         = 5,                            /* bit number of cylinder high valid bit*/
    bATACylinderLoValid         = 4,                            /* bit number of cylinder low valid bit*/
    bATASectorNumValid          = 3,                            /* bit number of sector number valid bit*/
    bATASectorCntValid          = 2,                            /* bit number of sector count valid bit*/
    bATAErrFeaturesValid        = 1,                            /* bit number of error/features valid bit*/
    bATADataValid               = 0,                            /* bit number of data valid bit*/
    mATAAltSDevCValid           = 1 << bATAAltSDevCValid,       /* alternate status/device control valid*/
    mATAStatusCmdValid          = 1 << bATAStatusCmdValid,      /* status/command valid*/
    mATASDHValid                = 1 << bATASDHValid,            /* ataTFSDH valid*/
    mATACylinderHiValid         = 1 << bATACylinderHiValid,     /* cylinder high valid*/
    mATACylinderLoValid         = 1 << bATACylinderLoValid,     /* cylinder low valid*/
    mATASectorNumValid          = 1 << bATASectorNumValid,      /* sector number valid*/
    mATASectorCntValid          = 1 << bATASectorCntValid,      /* sector count valid*/
    mATAErrFeaturesValid        = 1 << bATAErrFeaturesValid,    /* error/features valid*/
    mATADataValid               = 1 << bATADataValid            /* data valid*/
};

/* Parameter block structure for device register access command*/

union ataRegValueUnion {
    UInt8                           ataByteRegValue;            /* <->: Byte register value read or to be written*/
    UInt16                          ataWordRegValue;            /* <->: Word register value read or to be written*/
};
typedef union ataRegValueUnion          ataRegValueUnion;
/* Manager parameter block structure*/

struct ataRegAccess {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    UInt16                          ataRegSelect;               /* -->: Device Register Selector*/
                                                                /*         DataReg       0  */
                                                                /*         ErrorReg(R) or FeaturesReg(W)    1*/
                                                                /*         SecCntReg       2*/
                                                                /*         SecNumReg       3*/
                                                                /*         CylLoReg     4*/
                                                                /*         CylHiReg     5*/
                                                                /*         SDHReg      6*/
                                                                /*         StatusReg(R) or CmdReg(W)       7*/
                                                                /*         AltStatus(R) or DevCntr(W)   0E*/
    ataRegValueUnion                ataRegValue;
                                                                /* Following fields are valid only if ataRegSelect = 0xFFFF*/
    UInt16                          ataRegMask;                 /* -->: mask for register(s) to update*/
                                                                /*       bit 0 : data register valid*/
                                                                /*       bit 1 : error/feaures register valid*/
                                                                /*       bit 2 : sector count register valid*/
                                                                /*       bit 3 : sector number register valid*/
                                                                /*       bit 4 : cylinder low register valid*/
                                                                /*       bit 5 : cylinder high register valid*/
                                                                /*       bit 6 : ataTFSDH register valid*/
                                                                /*       bit 7 : status/command register valid*/
                                                                /*       bits 8 - 13 : reserved (set to 0)*/
                                                                /*       bit 14: alternate status / device control reg valid*/
                                                                /*      bit 15: reserved (set to 0)*/
    ataTaskFile                     ataRegisterImage;           /* <->: register images*/
    UInt8                           ataAltSDevCReg;             /* <->: Alternate status(R) or Device Control(W) register image*/
    UInt8                           Reserved3;                  /* Reserved*/
    SInt16                          Reserved[16];               /* Reserved for future expansion*/
};
typedef struct ataRegAccess             ataRegAccess;
/* Manager parameter block structure    <DP03/10/94>*/

struct ataIdentify {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    UInt16                          Reserved1[4];               /* Reserved.  These are used internally by the Manager*/
    UInt8 *                         ataPBBuffer;                /* Buffer for the identify data (512 bytes)*/
    UInt16                          Reserved2[12];              /* Used internally by the ATA Manager*/
    SInt16                          Reserved3[6];               /* Reserved for future expansion*/
};
typedef struct ataIdentify              ataIdentify;
/* 'ataConfigSetting' field of the Get/Set Device Configuration definition <8/6/94>*/
enum {
    ATAPIpacketDRQ_bit          = 6,                            /* bit number of ATAPI command packet DRQ option*/
    ATAPIpacketDRQ              = 1 << ATAPIpacketDRQ_bit       /* ATAPI command packet DRQ option*/
};

/* atapcValid field definition*/
enum {
    bATApcAccessMode            = 0,
    bATApcVcc                   = 1,
    bATApcVpp1                  = 2,
    bATApcVpp2                  = 3,
    bATApcStatus                = 4,
    bATApcPin                   = 5,
    bATApcCopy                  = 6,
    bATApcConfigIndex           = 7,
    bATApcLockUnlock            = 15,
    mATApcAccessMode            = 1 << bATApcAccessMode,
    mATApcVcc                   = 1 << bATApcVcc,
    mATApcVpp1                  = 1 << bATApcVpp1,
    mATApcVpp2                  = 1 << bATApcVpp2,
    mATApcStatus                = 1 << bATApcStatus,
    mATApcPin                   = 1 << bATApcPin,
    mATApcCopy                  = 1 << bATApcCopy,
    mATApcConfigIndex           = 1 << bATApcConfigIndex,
    mATApcLockUnlock            = 1 << bATApcLockUnlock
};

/* Device physical type & socket type indicator definition*/
enum {
    kATADeviceUnknown           = 0x00,                         /* no device or type undetermined*/
    kATADeviceATA               = 0x01,                         /* traditional ATA protocol device <7/29/94>*/
    kATADeviceATAPI             = 0x02,                         /* ATAPI protocol device  <7/29/94>*/
    kATADeviceReserved          = 0x03                          /* reserved by Apple (was PCMCIA)*/
};

enum {
    kATASocketInternal          = 0x01,                         /* Internal ATA socket*/
    kATASocketMB                = 0x02,                         /* Media Bay socket*/
    kATASocketPCMCIA            = 0x03                          /* PCMCIA socket*/
};

/* reserved words at the end of the devConfig structure*/
enum {
    kATAConfigReserved          = 5                             /* number of reserved words at the end*/
};

/*
   Get/Set Device Configuration parameter block structure <8/6/94>
   Manager parameter block structure
*/

struct ataDevConfiguration {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    SInt32                          ataConfigSetting;           /* <->: Configuration setting*/
                                                                /*      Bits 3 - 0: Reserved*/
                                                                /*      Bit 4: Reserved (allowLBAAccess)*/
                                                                /*      Bit 5: Reserved (allowRWMultiple)*/
                                                                /*      Bit 6: ATAPIpacketDRQ*/
                                                                /*       1 = Check for Interrupt DRQ on ATAPI command packet DRQ*/
                                                                /*       0 = Default: Check only for the assertion of command packet DRQ*/
                                                                /*      Bits 31 - 7: Reserved*/
    UInt8                           ataPIOSpeedMode;            /* <->: Device access speed in PIO Mode*/
    UInt8                           Reserved3;                  /* Reserved to force word alignment*/
    UInt16                          atapcValid;                 /* <->: Set when pcXXX fields are valid (atapcAccessMode - atapcConfigIndex)*/
                                                                /*       bit 0 - atapcAccessMode field valid, when set*/
                                                                /*       bit 1 - atapcVcc field valid, when set*/
                                                                /*       bit 2 - atapcVpp1 field valid, when set*/
                                                                /*       bit 3 - atapcVpp2 field valid, when set*/
                                                                /*       bit 4 - atapcStatus field valid, when set*/
                                                                /*       bit 5 - atapcPin field valid, when set*/
                                                                /*       bit 6 - atapcCopy field valid, when set*/
                                                                /*       bit 7 - atapcConfigIndex field valid, when set*/
                                                                /*       bits 14-8 - Reserved*/
                                                                /*       bit 15 - device lock/unlock request (write only)*/
    UInt16                          ataRWMultipleCount;         /* Reserved for future (not supported yet)*/
    UInt16                          ataSectorsPerCylinder;      /* Reserved for future (not supported yet)*/
    UInt16                          ataHeads;                   /* Reserved for future (not supported yet)*/
    UInt16                          ataSectorsPerTrack;         /* Reserved for future (not supported yet)*/
    UInt16                          ataSocketNumber;            /* <--: Socket number used by the CardServices*/
                                                                /*       0xFF = socket number invalid (Not a CardServices device)*/
                                                                /*       other = socket number of the device*/
    UInt8                           ataSocketType;              /* <--: Specifies the socket type (get config only)*/
                                                                /*       00 = Unknown socket*/
                                                                /*      01 = Internal ATA bus*/
                                                                /*       02 = Media Bay*/
                                                                /*       03 = PCMCIA*/
    UInt8                           ataDeviceType;              /* <--: Specifies the device type (get config only)*/
                                                                /*       00 = Unknown device*/
                                                                /*       01 = standard ATA device (HD)*/
                                                                /*       02 = ATAPI device*/
                                                                /*       03 = PCMCIA ATA device*/
    UInt8                           atapcAccessMode;            /* <->: Access mode: Memory vs. I/O (PCMCIA only)*/
    UInt8                           atapcVcc;                   /* <->: Voltage in tenths of a volt (PCMCIA only)*/
    UInt8                           atapcVpp1;                  /* <->: Voltage in tenths of a volt (PCMCIA only)*/
    UInt8                           atapcVpp2;                  /* <->: Voltage in tenths of a volt (PCMCIA only)*/
    UInt8                           atapcStatus;                /* <->: Card Status register setting (PCMCIA only)*/
    UInt8                           atapcPin;                   /* <->: Card Pin register setting (PCMCIA only)*/
    UInt8                           atapcCopy;                  /* <->: Card Socket/Copy register setting (PCMCIA only)*/
    UInt8                           atapcConfigIndex;           /* <->: Card Option register setting (PCMCIA only)*/
    UInt8                           ataSingleDMASpeed;          /* <->: Single Word DMA Timing Class*/
    UInt8                           ataMultiDMASpeed;           /* <->: Multiple Word DMA Timing Class*/
    UInt16                          ataPIOCycleTime;            /* <->:Cycle time for PIO mode*/
    UInt16                          ataMultiCycleTime;          /* <->:Cycle time for Multiword DMA mode*/
    UInt8                           ataUltraDMASpeed;           /* <-> Ultra DMA timing class*/
    UInt8                           reserved2;                  /* reserved*/
    UInt16                          ataUltraCycleTime;          /* <-> Cycle time for Ultra DMA mode*/
    UInt16                          Reserved1[5];               /* Reserved for future*/
};
typedef struct ataDevConfiguration      ataDevConfiguration;
/* Get Card Location Icon/Text  <SM4>*/
enum {
    kATALargeIconHFS            = 0x0001,                       /* Large B&W icon with mask (HFS)*/
    kATALargeIconProDOS         = 0x0081                        /* Large B&W icon with mask (ProDOS)*/
};

/* Manager parameter block structure*/

struct ataLocationData {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    SInt16                          ataIconType;                /* -->: icon type specifier*/
                                                                /*      1 = Large B&W icon with mask (256 bytes)*/
                                                                /*       0x81 = Same as 1, but ProDOS icon*/
    SInt16                          ataIconReserved;            /* Reserved to be longword aligned*/
    SInt8 *                         ataLocationIconPtr;         /* -->: Icon Data buffer pointer*/
    SInt8 *                         ataLocationStringPtr;       /* -->: Icon String buffer pointer*/
    UInt16                          Reserved1[18];              /* Reserved for future*/
};
typedef struct ataLocationData          ataLocationData;
/* ataOSType available*/
enum {
    kATAddTypeMacOS             = 0x0001                        /* Blue Mac O/S ddType value*/
};

/* The parameter block definition for all other ATA Manager functions.*/


struct ataGeneric {
                                                                /* Start of cloned common header ataPBHdr*/
    ataPBHeader *                   ataPBLink;                  /* a pointer to the next entry in the queue   */
    UInt16                          ataPBQType;                 /* type byte for safety check*/
    UInt8                           ataPBVers;                  /* -->: parameter block version number; Must be 0x01*/
    UInt8                           ataPBReserved;              /* Reserved                         */
    Ptr                             ataPBReserved2;             /* Reserved                         */
    ATACallbackUPP                  ataPBCallbackPtr;           /* -->: Completion Routine Pointer*/
    OSErr                           ataPBResult;                /* <--: Returned result          */
    UInt8                           ataPBFunctionCode;          /* -->: Manager Function Code */
    UInt8                           ataPBIOSpeed;               /* -->: I/O Timing Class       */
    UInt16                          ataPBFlags;                 /* -->: Various control options   */
    SInt16                          ataPBReserved3;             /* Reserved                         */
    UInt32                          ataPBDeviceID;              /* -->: Device identifier (see ataDeviceID)      */
    UInt32                          ataPBTimeOut;               /* -->: Transaction timeout value in msec */
    Ptr                             ataPBClientPtr1;            /* Client's storage Ptr 1     */
    Ptr                             ataPBClientPtr2;            /* Client's storage Ptr 2     */
    UInt16                          ataPBState;                 /* Reserved for Manager; Initialize to 0 */
    UInt16                          ataPBSemaphores;            /* Used internally by the manager*/
    SInt32                          ataPBReserved4;             /* Reserved                         */
                                                                /* End of cloned common header ataPBHdr*/
    UInt16                          Reserved[24];               /* Reserved for future*/
};
typedef struct ataGeneric               ataGeneric;

union ataPB {
    ataIOPB                         ataIOParamBlock;            /* parameter block for I/O*/
    ataBusInquiry                   ataBIParamBlock;            /* parameter block for bus inquiry*/
    ataMgrInquiry                   ataMIParamBlock;            /* parameter block for Manager inquiry*/
    ataAbort                        ataAbortParamBlock;         /* parameter block for abort*/
    ataDrvrRegister                 ataDRParamBlock;            /* parameter block for driver register*/
    ataModifyEventMask              ataMEParamBlock;            /* parameter block for event mask modify*/
    ataRegAccess                    ataRAParamBlock;            /* parameter block for register access*/
    ataIdentify                     ataDIParamBlock;            /* parameter block for drive identify*/
    ataDevConfiguration             ataDCParamBlock;            /* parameter block for device configuration*/
    ataLocationData                 ataLDParamBlock;            /* parameter block for location icon data*/
                                                                /*ataManagerInit  ataInitParamBlock;     // parameter block for Manager initialization*/
                                                                /*ataManagerShutDn    ataSDParamBlock;     // parameter block for Manager shutdown*/
                                                                /*ataDrvrLoad     ataDLParamBlock;     // parameter block for Driver loading*/
    ataGeneric                      ataGenericParamBlock;       /* parameter block for all other functions*/
};
typedef union ataPB                     ataPB;
/* The ATA Event codes...*/
enum {
    kATANullEvent               = 0x00,                         /* Just kidding -- nothing happened*/
    kATAOnlineEvent             = 0x01,                         /* An ATA device has come online*/
    kATAOfflineEvent            = 0x02,                         /* An ATA device has gone offline*/
    kATARemovedEvent            = 0x03,                         /* An ATA device has been removed from the bus*/
    kATAResetEvent              = 0x04,                         /* Someone gave a hard reset to the drive*/
    kATAOfflineRequest          = 0x05,                         /* Someone requesting to offline the drive*/
    kATAEjectRequest            = 0x06,                         /* Someone requesting to eject the drive*/
    kATAUpdateEvent             = 0x07,                         /* Potential configuration change reported by CardServices <SM4>*/
    kATATaskTimeRequest         = 0x08,                         /* The manager is requesting to be called at Task Time*/
    kATALoadDriverNow           = 0x09,                         /* Load the driver for the given bus immediately*/
    kATAPIResetEvent            = 0x0A,                         /* Someone gave a ATAPI reset to the drive*/
                                                                /* The following describes bit definitions in the eventMask field of ataDrvrRegister*/
    bATANullEvent               = 1 << kATANullEvent,           /* null event bit*/
    bATAOnlineEvent             = 1 << kATAOnlineEvent,         /* online event bit*/
    bATAOfflineEvent            = 1 << kATAOfflineEvent,        /* offline event bit*/
    bATARemovedEvent            = 1 << kATARemovedEvent,        /* removed event bit*/
    bATAResetEvent              = 1 << kATAResetEvent,          /* ATA reset event bit*/
    bATAOfflineRequest          = 1 << kATAOfflineRequest,      /* offline request event bit*/
    bATAEjectRequest            = 1 << kATAEjectRequest,        /* eject request event bit*/
    bATAUpdateEvent             = 1 << kATAUpdateEvent,         /* configuration update event bit*/
    bATAPIResetEvent            = 1 << kATAPIResetEvent         /* ATAPI reset event bit*/
};

enum {
    kATAEventMarker             = FOUR_CHAR_CODE('LOAD'),       /* Marker for the event data structure*/
    kATAEventVersion1           = 0x00000001                    /* Version 1 of the event structure*/
};

typedef CALLBACK_API( OSErr , ATADispatchProcPtr )(ataPB *pb);
typedef STACK_UPP_TYPE(ATADispatchProcPtr)                      ATADispatchUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ATADispatchUPP)
    NewATADispatchUPP              (ATADispatchProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeATADispatchUPP          (ATADispatchUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeATADispatchUPP           (ataPB *                 pb,
                                    ATADispatchUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppATADispatchProcInfo = 0x000000E0 };                   /* pascal 2_bytes Func(4_bytes) */
    #define NewATADispatchUPP(userRoutine)                          (ATADispatchUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppATADispatchProcInfo, GetCurrentArchitecture())
    #define DisposeATADispatchUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeATADispatchUPP(pb, userUPP)                       (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppATADispatchProcInfo, (pb))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewATADispatchProc(userRoutine)                         NewATADispatchUPP(userRoutine)
#define CallATADispatchProc(userRoutine, pb)                    InvokeATADispatchUPP(pb, userRoutine)
#if CALL_NOT_IN_CARBON
EXTERN_API( SInt16 )
ataManager                      (ataPB *                pb)                                 ONEWORDINLINE(0xAAF1);

/* Device Error codes: 0xDB42 - 0xDB5F  */

#endif  /* CALL_NOT_IN_CARBON */

enum {
    ATABaseErrCode              = -9406,                        /* Base error code - 0xDB42   */
    ioPending                   = 1,                            /* Asynch I/O in progress status*/
    AT_NRdyErr                  = ATABaseErrCode + 0x01,        /* 0xDB43: Drive not Ready */
    AT_IDNFErr                  = ATABaseErrCode + 0x02,        /* 0xDB44: ID not found */
    AT_DMarkErr                 = ATABaseErrCode + 0x03,        /* 0xDB45: Data mark not found */
    AT_BadBlkErr                = ATABaseErrCode + 0x04,        /* 0xDB46: Bad Block */
    AT_CorDataErr               = ATABaseErrCode + 0x05,        /* 0xDB47: Data was corrected */
    AT_UncDataErr               = ATABaseErrCode + 0x06,        /* 0xDB48: Data was not corrected */
    AT_SeekErr                  = ATABaseErrCode + 0x07,        /* 0xDB49: Seek error */
    AT_WrFltErr                 = ATABaseErrCode + 0x08,        /* 0xDB4A: Write fault */
    AT_RecalErr                 = ATABaseErrCode + 0x09,        /* 0xDB4B: Recalibrate failed */
    AT_AbortErr                 = ATABaseErrCode + 0x0A,        /* 0xDB4C: Command aborted by drive */
    AT_MCErr                    = ATABaseErrCode + 0x0C,        /* 0xDB4E: Media Changed error*/
    ATAPICheckErr               = ATABaseErrCode + 0x0D,        /* 0xDB4F: ATAPI Check condition <06/15/94>*/
    AT_UltraDMAiCRCErr          = ATABaseErrCode + 0x0E,        /* 0xDB50: CRC error during Ultra DMA xfer */
                                                                /* System error codes...Custom Driver Error Codes 0xDB60 - 0xDB6F*/
    DRVRCantAllocate            = ATABaseErrCode + 0x1E,        /* 0xDB60: Allocation error during initialization*/
    NoATAMgr                    = ATABaseErrCode + 0x1F,        /* 0xDB61: MgrInquiry failed => No ATA Manager*/
    ATAInitFail                 = ATABaseErrCode + 0x20,        /* 0xDB62: Mgr Initialization failed*/
    ATABufFail                  = ATABaseErrCode + 0x21,        /* 0xDB63: Device buffer test failure*/
    ATADevUnsupported           = ATABaseErrCode + 0x22,        /* 0xDB64: Device type not supported*/
    ATAEjectDrvErr              = ATABaseErrCode + 0x23,        /* 0xDB65: Could not eject the drive*/
                                                                /* Manager Error Codes 0xDB70 - 0xDB8F*/
    ATAMgrNotInitialized        = ATABaseErrCode + 0x2E,        /* 0xDB70: Mgr has not been initialized*/
    ATAPBInvalid                = ATABaseErrCode + 0x2F,        /* 0xDB71: The bus base address couldn't be found*/
    ATAFuncNotSupported         = ATABaseErrCode + 0x30,        /* 0xDB72: An unknown function code specified*/
    ATABusy                     = ATABaseErrCode + 0x31,        /* 0xDB73: Selected device is busy*/
    ATATransTimeOut             = ATABaseErrCode + 0x32,        /* 0xDB74: Transaction timeout detected*/
    ATAReqInProg                = ATABaseErrCode + 0x33,        /* 0xDB75: Channel busy; channel is processing another cmd*/
    ATAUnknownState             = ATABaseErrCode + 0x34,        /* 0xDB76: Device status register reflects an unknown state*/
    ATAQLocked                  = ATABaseErrCode + 0x35,        /* 0xDB77: I/O Queue is locked due to previous I/O error.*/
    ATAReqAborted               = ATABaseErrCode + 0x36,        /* 0xDB78: The I/O queue entry was aborted due to an abort req.*/
                                                                /*         or due to Manager shutdown.*/
    ATAUnableToAbort            = ATABaseErrCode + 0x37,        /* 0xDB79: The I/O queue entry could not be aborted.*/
    ATAAbortedDueToRst          = ATABaseErrCode + 0x38,        /* 0xDB7A: Request aborted due to a device reset command.*/
    ATAPIPhaseErr               = ATABaseErrCode + 0x39,        /* 0xDB7B: Unexpected phase - oooIS THIS VALID ERROR??? <06/15/94>*/
    ATAPITxCntErr               = ATABaseErrCode + 0x3A,        /* 0xDB7C: Overrun/Underrun condition detected*/
    ATANoClientErr              = ATABaseErrCode + 0x3B,        /* 0xDB7D: No client present to handle the event*/
    ATAInternalErr              = ATABaseErrCode + 0x3C,        /* 0xDB7E: MagnumOpus returned an error*/
    ATABusErr                   = ATABaseErrCode + 0x3D,        /* 0xDB7F: Bus error detected on I/O  */
    AT_NoAddrErr                = ATABaseErrCode + 0x3E,        /* 0xDB80: Invalid AT base adress */
    DriverLocked                = ATABaseErrCode + 0x3F,        /* 0xDB81: Current driver must be removed before adding another*/
    CantHandleEvent             = ATABaseErrCode + 0x40,        /* 0xDB82: Particular event couldn't be handled (call others)*/
    ATAMgrMemoryErr             = ATABaseErrCode + 0x41,        /* 0xDB83: Manager memory allocation error    */
    ATASDFailErr                = ATABaseErrCode + 0x42,        /* 0xDB84: Shutdown failure          */
    ATAXferParamErr             = ATABaseErrCode + 0x43,        /* 0xDB85: I/O xfer parameters inconsistent */
    ATAXferModeErr              = ATABaseErrCode + 0x44,        /* 0xDB86: I/O xfer mode not supported */
    ATAMgrConsistencyErr        = ATABaseErrCode + 0x45,        /* 0XDB87: Manager detected internal inconsistency. */
    ATADmaXferErr               = ATABaseErrCode + 0x46,        /* 0XDB88: fatal error in DMA side of transfer */
                                                                /* Driver loader error Codes 0xDB90 - 0xDBA5*/
    ATAInvalidDrvNum            = ATABaseErrCode + 0x4E,        /* 0xDB90: Invalid drive number from event*/
    ATAMemoryErr                = ATABaseErrCode + 0x4F,        /* 0xDB91: Memory allocation error*/
    ATANoDDMErr                 = ATABaseErrCode + 0x50,        /* 0xDB92: No DDM found on media  */
    ATANoDriverErr              = ATABaseErrCode + 0x51         /* 0xDB93: No driver found on the media   */
};

/* ------------------------ Version 1 definition -------------------------------    */
enum {
    v1ATABaseErrCode            = 0x0700,                       /* This needs a home somewhere*/
    v1AT_NRdyErr                = 0x01 - v1ATABaseErrCode,      /* 0xF901: -0x1DBE */
    v1AT_IDNFErr                = 0x04 - v1ATABaseErrCode,      /* 0xF904: -0x1DC0 */
    v1AT_DMarkErr               = 0x05 - v1ATABaseErrCode,      /* 0xF905: -0x1DC0 */
    v1AT_BadBlkErr              = 0x06 - v1ATABaseErrCode,      /* 0xF906: -0x1DC0 */
    v1AT_CorDataErr             = 0x07 - v1ATABaseErrCode,      /* 0xF907: -0x1DC0 */
    v1AT_UncDataErr             = 0x08 - v1ATABaseErrCode,      /* 0xF908: -0x1DC0 */
    v1AT_SeekErr                = 0x09 - v1ATABaseErrCode,      /* 0xF909: -0x1DC0 */
    v1AT_WrFltErr               = 0x0A - v1ATABaseErrCode,      /* 0xF90A: -0x1DC0 */
    v1AT_RecalErr               = 0x0B - v1ATABaseErrCode,      /* 0xF90B: -0x1DC0 */
    v1AT_AbortErr               = 0x0C - v1ATABaseErrCode,      /* 0xF90C: -0x1DC0 */
    v1AT_NoAddrErr              = 0x0D - v1ATABaseErrCode,      /* 0xF90D: -0x1D8D */
    v1AT_MCErr                  = 0x0E - v1ATABaseErrCode,      /* 0xF90E: -0x1DC0*/
                                                                /* System error codes...Custom Driver Error Codes*/
    v1DRVRCantAllocate          = -(v1ATABaseErrCode + 1),      /* 0xF8FF: -0x1D9F*/
    v1NoATAMgr                  = -(v1ATABaseErrCode + 2),      /* 0xF8FE: -0x1D9D*/
    v1ATAInitFail               = -(v1ATABaseErrCode + 3),      /* 0xF8FD: -0x1D9B*/
    v1ATABufFail                = -(v1ATABaseErrCode + 4),      /* 0xF8FC: -0x1D99*/
    v1ATADevUnsupported         = -(v1ATABaseErrCode + 5),      /* 0xF8FB: -0x1c97*/
                                                                /* Manager Error Codes*/
    v1ATAMgrNotInitialized      = -(v1ATABaseErrCode + 10),     /* 0xF8F6: -0x1D86*/
    v1ATAPBInvalid              = -(v1ATABaseErrCode + 11),     /* 0xF8F5: -0x1D84*/
    v1ATAFuncNotSupported       = -(v1ATABaseErrCode + 12),     /* 0xF8F4: -0x1D82*/
    v1ATABusy                   = -(v1ATABaseErrCode + 13),     /* 0xF8F3: -0x1D80*/
    v1ATATransTimeOut           = -(v1ATABaseErrCode + 14),     /* 0xF8F2: -0x1D7E*/
    v1ATAReqInProg              = -(v1ATABaseErrCode + 15),     /* 0xF8F1: -0x1D7C*/
    v1ATAUnknownState           = -(v1ATABaseErrCode + 16),     /* 0xF8F0: -0x1D7A*/
    v1ATAQLocked                = -(v1ATABaseErrCode + 17),     /* 0xF8EF: -0x1D78*/
    v1ATAReqAborted             = -(v1ATABaseErrCode + 18),     /* 0xF8EE: -0x1D76*/
    v1ATAUnableToAbort          = -(v1ATABaseErrCode + 19),     /* 0xF8ED: -0x1D74*/
    v1ATAAbortedDueToRst        = -(v1ATABaseErrCode + 20)      /* 0xF8EC: -0x1D72*/
};



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

#endif /* __ATA__ */

