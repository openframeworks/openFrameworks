/*
     File:       Slots.h
 
     Contains:   Slot Manager Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1986-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SLOTS__
#define __SLOTS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
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

enum {
    fCardIsChanged              = 1,                            /*Card is Changed field in StatusFlags field of sInfoArray*/
    fCkForSame                  = 0,                            /*For SearchSRT. Flag to check for SAME sResource in the table. */
    fCkForNext                  = 1,                            /*For SearchSRT. Flag to check for NEXT sResource in the table. */
    fWarmStart                  = 2                             /*If this bit is set then warm start else cold start.*/
};

enum {
    stateNil                    = 0,                            /*State*/
    stateSDMInit                = 1,                            /*:Slot declaration manager Init*/
    statePRAMInit               = 2,                            /*:sPRAM record init*/
    statePInit                  = 3,                            /*:Primary init*/
    stateSInit                  = 4                             /*:Secondary init*/
};

enum {
                                                                /* flags for spParamData */
    fall                        = 0,                            /* bit 0: set=search enabled/disabled sRsrc's */
    foneslot                    = 1,                            /*    1: set=search sRsrc's in given slot only */
    fnext                       = 2                             /*    2: set=search for next sRsrc */
};

enum {
                                                                /* Misc masks */
    catMask                     = 0x08,                         /* sets spCategory field of spTBMask (bit 3) */
    cTypeMask                   = 0x04,                         /* sets spCType    field of spTBMask (bit 2) */
    drvrSWMask                  = 0x02,                         /* sets spDrvrSW   field of spTBMask (bit 1) */
    drvrHWMask                  = 0x01                          /* sets spDrvrHW    field of spTBMask (bit 0) */
};

typedef CALLBACK_API( short , SlotIntServiceProcPtr )(long sqParameter);
/*
    WARNING: SlotIntServiceProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(SlotIntServiceProcPtr)                SlotIntServiceUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(SlotIntServiceUPP)
    NewSlotIntServiceUPP           (SlotIntServiceProcPtr   userRoutine);

    EXTERN_API(void)
    DisposeSlotIntServiceUPP       (SlotIntServiceUPP       userUPP);

    EXTERN_API(short)
    InvokeSlotIntServiceUPP        (long                    sqParameter,
                                    SlotIntServiceUPP       userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppSlotIntServiceProcInfo = 0x0000B822 };                /* register 2_bytes:D0 Func(4_bytes:A1) */
    #define NewSlotIntServiceUPP(userRoutine)                       (SlotIntServiceUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSlotIntServiceProcInfo, GetCurrentArchitecture())
    #define DisposeSlotIntServiceUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter __D0 InvokeSlotIntServiceUPP(__A1, __A0)
    short InvokeSlotIntServiceUPP(long sqParameter, SlotIntServiceUPP userUPP) = 0x4E90;
    #else
        #define InvokeSlotIntServiceUPP(sqParameter, userUPP)           (short)CALL_ONE_PARAMETER_UPP((userUPP), uppSlotIntServiceProcInfo, (sqParameter))
    #endif
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewSlotIntServiceProc(userRoutine)                      NewSlotIntServiceUPP(userRoutine)
#define CallSlotIntServiceProc(userRoutine, sqParameter)        InvokeSlotIntServiceUPP(sqParameter, userRoutine)

struct SlotIntQElement {
    Ptr                             sqLink;                     /*ptr to next element*/
    short                           sqType;                     /*queue type ID for validity*/
    short                           sqPrio;                     /*priority*/
    SlotIntServiceUPP               sqAddr;                     /*interrupt service routine*/
    long                            sqParm;                     /*optional A1 parameter*/
};
typedef struct SlotIntQElement          SlotIntQElement;

typedef SlotIntQElement *               SQElemPtr;

struct SpBlock {
    long                            spResult;                   /*FUNCTION Result*/
    Ptr                             spsPointer;                 /*structure pointer*/
    long                            spSize;                     /*size of structure*/
    long                            spOffsetData;               /*offset/data field used by sOffsetData*/
    Ptr                             spIOFileName;               /*ptr to IOFile name for sDisDrvrName*/
    Ptr                             spsExecPBlk;                /*pointer to sExec parameter block.*/
    long                            spParamData;                /*misc parameter data (formerly spStackPtr).*/
    long                            spMisc;                     /*misc field for SDM.*/
    long                            spReserved;                 /*reserved for future expansion*/
    short                           spIOReserved;               /*Reserved field of Slot Resource Table*/
    short                           spRefNum;                   /*RefNum*/
    short                           spCategory;                 /*sType: Category*/
    short                           spCType;                    /*Type*/
    short                           spDrvrSW;                   /*DrvrSW*/
    short                           spDrvrHW;                   /*DrvrHW*/
    SInt8                           spTBMask;                   /*type bit mask bits 0..3 mask words 0..3*/
    SInt8                           spSlot;                     /*slot number*/
    SInt8                           spID;                       /*structure ID*/
    SInt8                           spExtDev;                   /*ID of the external device*/
    SInt8                           spHwDev;                    /*Id of the hardware device.*/
    SInt8                           spByteLanes;                /*bytelanes from card ROM format block*/
    SInt8                           spFlags;                    /*standard flags*/
    SInt8                           spKey;                      /*Internal use only*/
};
typedef struct SpBlock                  SpBlock;
typedef SpBlock *                       SpBlockPtr;

struct SInfoRecord {
    Ptr                             siDirPtr;                   /*Pointer to directory*/
    short                           siInitStatusA;              /*initialization E*/
    short                           siInitStatusV;              /*status returned by vendor init code*/
    SInt8                           siState;                    /*initialization state*/
    SInt8                           siCPUByteLanes;             /*0=[d0..d7] 1=[d8..d15]*/
    SInt8                           siTopOfROM;                 /*Top of ROM= $FssFFFFx: x is TopOfROM*/
    SInt8                           siStatusFlags;              /*bit 0 - card is changed*/
    short                           siTOConst;                  /*Time Out C for BusErr*/
    SInt8                           siReserved[2];              /*reserved*/
    Ptr                             siROMAddr;                  /* addr of top of ROM */
    SInt8                           siSlot;                     /* slot number */
    SInt8                           siPadding[3];               /* reserved */
};
typedef struct SInfoRecord              SInfoRecord;

typedef SInfoRecord *                   SInfoRecPtr;

struct SDMRecord {
    ProcPtr                         sdBEVSave;                  /*Save old BusErr vector*/
    ProcPtr                         sdBusErrProc;               /*Go here to determine if it is a BusErr*/
    ProcPtr                         sdErrorEntry;               /*Go here if BusErrProc finds real BusErr*/
    long                            sdReserved;                 /*Reserved*/
};
typedef struct SDMRecord                SDMRecord;

struct FHeaderRec {
    long                            fhDirOffset;                /*offset to directory*/
    long                            fhLength;                   /*length of ROM*/
    long                            fhCRC;                      /*CRC*/
    SInt8                           fhROMRev;                   /*revision of ROM*/
    SInt8                           fhFormat;                   /*format - 2*/
    long                            fhTstPat;                   /*test pattern*/
    SInt8                           fhReserved;                 /*reserved*/
    SInt8                           fhByteLanes;                /*ByteLanes*/
};
typedef struct FHeaderRec               FHeaderRec;
typedef FHeaderRec *                    FHeaderRecPtr;
/*
   
    Extended Format header block  -  extended declaration ROM format header for super sRsrc directories.    <H2><SM0>
   
*/


struct XFHeaderRec {
    long                            fhXSuperInit;               /*Offset to SuperInit SExecBlock  <fhFormat,offset>*/
    long                            fhXSDirOffset;              /*Offset to SuperDirectory         <$FE,offset>*/
    long                            fhXEOL;                     /*Psuedo end-of-list          <$FF,nil>*/
    long                            fhXSTstPat;                 /*TestPattern*/
    long                            fhXDirOffset;               /*Offset to (minimal) directory*/
    long                            fhXLength;                  /*Length of ROM*/
    long                            fhXCRC;                     /*CRC*/
    SInt8                           fhXROMRev;                  /*Revision of ROM*/
    SInt8                           fhXFormat;                  /*Format-2*/
    long                            fhXTstPat;                  /*TestPattern*/
    SInt8                           fhXReserved;                /*Reserved*/
    SInt8                           fhXByteLanes;               /*ByteLanes*/
};
typedef struct XFHeaderRec              XFHeaderRec;
typedef XFHeaderRec *                   XFHeaderRecPtr;

struct SEBlock {
    UInt8                           seSlot;                     /*Slot number.*/
    UInt8                           sesRsrcId;                  /*sResource Id.*/
    short                           seStatus;                   /*Status of code executed by sExec.*/
    UInt8                           seFlags;                    /*Flags*/
    UInt8                           seFiller0;                  /*Filler, must be SignedByte to align on odd boundry*/
    UInt8                           seFiller1;                  /*Filler*/
    UInt8                           seFiller2;                  /*Filler*/
    long                            seResult;                   /*Result of sLoad.*/
    long                            seIOFileName;               /*Pointer to IOFile name.*/
    UInt8                           seDevice;                   /*Which device to read from.*/
    UInt8                           sePartition;                /*The partition.*/
    UInt8                           seOSType;                   /*Type of OS.*/
    UInt8                           seReserved;                 /*Reserved field.*/
    UInt8                           seRefNum;                   /*RefNum of the driver.*/
    UInt8                           seNumDevices;               /* Number of devices to load.*/
    UInt8                           seBootState;                /*State of StartBoot code.*/
    SInt8                           filler;
};
typedef struct SEBlock                  SEBlock;
/*  Principle  */
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SReadByte(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SReadByte                       (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7000, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SReadWord(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SReadWord                       (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7001, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SReadLong(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SReadLong                       (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7002, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SGetCString(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SGetCString                     (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7003, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SGetBlock(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SGetBlock                       (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7005, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SFindStruct(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SFindStruct                     (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7006, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SReadStruct(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SReadStruct                     (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7007, 0xA06E);

/*  Special  */
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SReadInfo(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SReadInfo                       (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7010, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SReadPRAMRec(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SReadPRAMRec                    (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7011, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SPutPRAMRec(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SPutPRAMRec                     (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7012, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SReadFHeader(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SReadFHeader                    (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7013, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SNextSRsrc(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SNextSRsrc                      (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7014, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SNextTypeSRsrc(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SNextTypeSRsrc                  (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7015, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SRsrcInfo(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SRsrcInfo                       (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7016, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SDisposePtr(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SDisposePtr                     (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7017, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SCkCardStat(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SCkCardStat                     (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7018, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SReadDrvrName(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SReadDrvrName                   (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7019, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SFindSRTRec(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SFindSRTRec                     (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x701A, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SFindDevBase(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SFindDevBase                    (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x701B, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SFindBigDevBase(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SFindBigDevBase                 (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x701C, 0xA06E);

/*  Advanced  */
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 InitSDeclMgr(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
InitSDeclMgr                    (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7020, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SPrimaryInit(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SPrimaryInit                    (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7021, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SCardChanged(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SCardChanged                    (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7022, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SExec(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SExec                           (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7023, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SOffsetData(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SOffsetData                     (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7024, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SInitPRAMRecs(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SInitPRAMRecs                   (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7025, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SReadPBSize(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SReadPBSize                     (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7026, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SCalcStep(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SCalcStep                       (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7028, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SInitSRsrcTable(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SInitSRsrcTable                 (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7029, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SSearchSRT(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SSearchSRT                      (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x702A, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SUpdateSRT(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SUpdateSRT                      (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x702B, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SCalcSPointer(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SCalcSPointer                   (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x702C, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SGetDriver(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SGetDriver                      (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x702D, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SPtrToSlot(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SPtrToSlot                      (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x702E, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SFindSInfoRecPtr(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SFindSInfoRecPtr                (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x702F, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SFindSRsrcPtr(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SFindSRsrcPtr                   (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7030, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SDeleteSRTRec(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SDeleteSRTRec                   (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7031, 0xA06E);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OpenSlot                        (ParmBlkPtr             paramBlock,
                                 Boolean                async);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 OpenSlotSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
OpenSlotSync                    (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA200);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 OpenSlotAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
OpenSlotAsync                   (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA600);

/*  Device Manager Slot Support  */
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SIntInstall(__A0, __D0)
                                                                                            #endif
EXTERN_API( OSErr )
SIntInstall                     (SQElemPtr              sIntQElemPtr,
                                 short                  theSlot)                            ONEWORDINLINE(0xA075);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SIntRemove(__A0, __D0)
                                                                                            #endif
EXTERN_API( OSErr )
SIntRemove                      (SQElemPtr              sIntQElemPtr,
                                 short                  theSlot)                            ONEWORDINLINE(0xA076);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SVersion(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SVersion                        (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7008, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SetSRsrcState(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SetSRsrcState                   (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x7009, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 InsertSRTRec(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
InsertSRTRec                    (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x700A, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SGetSRsrc(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SGetSRsrc                       (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x700B, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SGetTypeSRsrc(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SGetTypeSRsrc                   (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x700C, 0xA06E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SGetSRsrcPtr(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SGetSRsrcPtr                    (SpBlockPtr             spBlkPtr)                           TWOWORDINLINE(0x701D, 0xA06E);



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

#endif /* __SLOTS__ */

