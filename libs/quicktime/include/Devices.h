/*
     File:       Devices.h
 
     Contains:   Device Manager Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DEVICES__
#define __DEVICES__

#ifndef __OSUTILS__
#include <OSUtils.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __NAMEREGISTRY__
#include <NameRegistry.h>
#endif

#ifndef __CODEFRAGMENTS__
#include <CodeFragments.h>
#endif

#ifndef __DRIVERFAMILYMATCHING__
#include <DriverFamilyMatching.h>
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

/* Values of the 'message' parameter to a Chooser device package */
enum {
    chooserInitMsg              = 11,                           /* the user selected this device package */
    newSelMsg                   = 12,                           /* the user made new device selections */
    fillListMsg                 = 13,                           /* fill the device list with choices */
    getSelMsg                   = 14,                           /* mark one or more choices as selected */
    selectMsg                   = 15,                           /* the user made a selection */
    deselectMsg                 = 16,                           /* the user canceled a selection */
    terminateMsg                = 17,                           /* allows device package to clean up */
    buttonMsg                   = 19                            /* the user selected a button */
};


/* Values of the 'caller' parameter to a Chooser device package */
enum {
    chooserID                   = 1
};


/* Values of the 'message' parameter to a Monitor 'mntr' */
enum {
    initMsg                     = 1,                            /*initialization*/
    okMsg                       = 2,                            /*user clicked OK button*/
    cancelMsg                   = 3,                            /*user clicked Cancel button*/
    hitMsg                      = 4,                            /*user clicked control in Options dialog*/
    nulMsg                      = 5,                            /*periodic event*/
    updateMsg                   = 6,                            /*update event*/
    activateMsg                 = 7,                            /*not used*/
    deactivateMsg               = 8,                            /*not used*/
    keyEvtMsg                   = 9,                            /*keyboard event*/
    superMsg                    = 10,                           /*show superuser controls*/
    normalMsg                   = 11,                           /*show only normal controls*/
    startupMsg                  = 12                            /*code has been loaded*/
};


/* control codes for DeskAccessories */
enum {
    goodbye                     = -1,                           /* heap being reinitialized */
    killCode                    = 1,                            /* KillIO requested */
    accEvent                    = 64,                           /* handle an event */
    accRun                      = 65,                           /* time for periodic action */
    accCursor                   = 66,                           /* change cursor shape */
    accMenu                     = 67,                           /* handle menu item */
    accUndo                     = 68,                           /* handle undo command */
    accCut                      = 70,                           /* handle cut command */
    accCopy                     = 71,                           /* handle copy command */
    accPaste                    = 72,                           /* handle paste command */
    accClear                    = 73                            /* handle clear command */
};

/* Control/Status Call Codes */
enum {
    drvStsCode                  = 8,                            /* status call code for drive status */
    ejectCode                   = 7,                            /* control call eject code */
    tgBuffCode                  = 8                             /* set tag buffer code */
};

/* miscellaneous Device Manager constants */
enum {
    ioInProgress                = 1,                            /* predefined value of ioResult while I/O is pending */
    aRdCmd                      = 2,                            /* low byte of ioTrap for Read calls */
    aWrCmd                      = 3,                            /* low byte of ioTrap for Write calls */
    asyncTrpBit                 = 10,                           /* trap word modifier */
    noQueueBit                  = 9                             /* trap word modifier */
};

/* flags used in the driver header and device control entry */
enum {
    dReadEnable                 = 0,                            /* set if driver responds to read requests */
    dWritEnable                 = 1,                            /* set if driver responds to write requests */
    dCtlEnable                  = 2,                            /* set if driver responds to control requests */
    dStatEnable                 = 3,                            /* set if driver responds to status requests */
    dNeedGoodBye                = 4,                            /* set if driver needs time for performing periodic tasks */
    dNeedTime                   = 5,                            /* set if driver needs time for performing periodic tasks */
    dNeedLock                   = 6                             /* set if driver must be locked in memory as soon as it is opened */
};

enum {
    dNeedLockMask               = 0x4000,                       /* set if driver must be locked in memory as soon as it is opened */
    dNeedTimeMask               = 0x2000,                       /* set if driver needs time for performing periodic tasks */
    dNeedGoodByeMask            = 0x1000,                       /* set if driver needs to be called before the application heap is initialized */
    dStatEnableMask             = 0x0800,                       /* set if driver responds to status requests */
    dCtlEnableMask              = 0x0400,                       /* set if driver responds to control requests */
    dWritEnableMask             = 0x0200,                       /* set if driver responds to write requests */
    dReadEnableMask             = 0x0100                        /* set if driver responds to read requests */
};


/* run-time flags used in the device control entry */
enum {
    dVMImmuneBit                = 0,                            /* driver does not need VM protection */
    dOpened                     = 5,                            /* driver is open */
    dRAMBased                   = 6,                            /* dCtlDriver is a handle (1) or pointer (0) */
    drvrActive                  = 7                             /* driver is currently processing a request */
};

enum {
    dVMImmuneMask               = 0x0001,                       /* driver does not need VM protection */
    dOpenedMask                 = 0x0020,                       /* driver is open */
    dRAMBasedMask               = 0x0040,                       /* dCtlDriver is a handle (1) or pointer (0) */
    drvrActiveMask              = 0x0080                        /* driver is currently processing a request */
};



struct DRVRHeader {
    short                           drvrFlags;
    short                           drvrDelay;
    short                           drvrEMask;
    short                           drvrMenu;
    short                           drvrOpen;
    short                           drvrPrime;
    short                           drvrCtl;
    short                           drvrStatus;
    short                           drvrClose;
    unsigned char                   drvrName[1];
};
typedef struct DRVRHeader               DRVRHeader;
typedef DRVRHeader *                    DRVRHeaderPtr;
typedef DRVRHeaderPtr *                 DRVRHeaderHandle;

struct DCtlEntry {
    Ptr                             dCtlDriver;
    volatile short                  dCtlFlags;
    QHdr                            dCtlQHdr;
    volatile long                   dCtlPosition;
    Handle                          dCtlStorage;
    short                           dCtlRefNum;
    long                            dCtlCurTicks;
    GrafPtr                         dCtlWindow;
    short                           dCtlDelay;
    short                           dCtlEMask;
    short                           dCtlMenu;
};
typedef struct DCtlEntry                DCtlEntry;

typedef DCtlEntry *                     DCtlPtr;
typedef DCtlPtr *                       DCtlHandle;

struct AuxDCE {
    Ptr                             dCtlDriver;
    volatile short                  dCtlFlags;
    QHdr                            dCtlQHdr;
    long                            dCtlPosition;
    Handle                          dCtlStorage;
    short                           dCtlRefNum;
    long                            dCtlCurTicks;
    GrafPtr                         dCtlWindow;
    short                           dCtlDelay;
    short                           dCtlEMask;
    short                           dCtlMenu;
    SInt8                           dCtlSlot;
    SInt8                           dCtlSlotId;
    long                            dCtlDevBase;
    Ptr                             dCtlOwner;
    SInt8                           dCtlExtDev;
    SInt8                           fillByte;
    UInt32                          dCtlNodeID;
};
typedef struct AuxDCE                   AuxDCE;
typedef AuxDCE *                        AuxDCEPtr;
typedef AuxDCEPtr *                     AuxDCEHandle;
/*  The NDRV Driver IO Entry Point and Commands */

typedef UInt16                          UnitNumber;
typedef UInt32                          DriverOpenCount;
typedef SInt16                          DriverRefNum;
typedef SInt16                          DriverFlags;
typedef UInt32                          IOCommandCode;
enum {
    kOpenCommand                = 0,
    kCloseCommand               = 1,
    kReadCommand                = 2,
    kWriteCommand               = 3,
    kControlCommand             = 4,
    kStatusCommand              = 5,
    kKillIOCommand              = 6,
    kInitializeCommand          = 7,                            /* init driver and device*/
    kFinalizeCommand            = 8,                            /* shutdown driver and device*/
    kReplaceCommand             = 9,                            /* replace an old driver*/
    kSupersededCommand          = 10,                           /* prepare to be replaced by a new driver*/
    kSuspendCommand             = 11,                           /* prepare driver to go to sleep*/
    kResumeCommand              = 12                            /* wake up sleeping driver*/
};

typedef struct OpaqueAddressSpaceID*    AddressSpaceID;
typedef struct OpaqueIOCommandID*       IOCommandID;
typedef UInt32                          IOCommandKind;
enum {
    kSynchronousIOCommandKind   = 0x00000001,
    kAsynchronousIOCommandKind  = 0x00000002,
    kImmediateIOCommandKind     = 0x00000004
};


struct DriverInitInfo {
    DriverRefNum                    refNum;
    RegEntryID                      deviceEntry;
};
typedef struct DriverInitInfo           DriverInitInfo;
typedef DriverInitInfo *                DriverInitInfoPtr;

typedef DriverInitInfo                  DriverReplaceInfo;
typedef DriverInitInfo *                DriverReplaceInfoPtr;

struct DriverFinalInfo {
    DriverRefNum                    refNum;
    RegEntryID                      deviceEntry;
};
typedef struct DriverFinalInfo          DriverFinalInfo;
typedef DriverFinalInfo *               DriverFinalInfoPtr;

typedef DriverFinalInfo                 DriverSupersededInfo;
typedef DriverFinalInfo *               DriverSupersededInfoPtr;

/* Contents are command specific*/


union IOCommandContents {
    ParmBlkPtr                      pb;
    DriverInitInfoPtr               initialInfo;
    DriverFinalInfoPtr              finalInfo;
    DriverReplaceInfoPtr            replaceInfo;
    DriverSupersededInfoPtr         supersededInfo;
};
typedef union IOCommandContents         IOCommandContents;
typedef CALLBACK_API_C( OSErr , DriverEntryPointPtr )(AddressSpaceID SpaceID, IOCommandID CommandID, IOCommandContents Contents, IOCommandCode Code, IOCommandKind Kind);
/* Record to describe a file-based driver candidate */

struct FileBasedDriverRecord {
    FSSpec                          theSpec;                    /* file specification*/
    MacDriverType                   theType;                    /* nameInfoStr + version number*/
    Boolean                         compatibleProp;             /* true if matched using a compatible name*/
    UInt8                           pad[3];                     /* alignment*/
};
typedef struct FileBasedDriverRecord    FileBasedDriverRecord;
typedef FileBasedDriverRecord *         FileBasedDriverRecordPtr;
/* Detailed Record to describe a file-based driver candidate. Includes fragment name */

struct FileBasedDriverDetailed {
    FileBasedDriverRecord           fileBasedDriver;
    Str63                           fragName;
};
typedef struct FileBasedDriverDetailed  FileBasedDriverDetailed;
typedef FileBasedDriverDetailed *       FileBasedDriverDetailedPtr;
/* Driver Loader API */
#define DECLARE_DRIVERDESCRIPTION(N_ADDITIONAL_SERVICES)\
        struct {\
            DriverDescription   fixed;\
            DriverServiceInfo   additional_service[N_ADDITIONAL_SERVICES-1];\
        };


#if CALL_NOT_IN_CARBON
EXTERN_API_C( SInt16 )
HigherDriverVersion             (const NumVersion *     driverVersion1,
                                 const NumVersion *     driverVersion2);

EXTERN_API_C( OSErr )
VerifyFragmentAsDriver          (CFragConnectionID      fragmentConnID,
                                 DriverEntryPointPtr *  fragmentMain,
                                 DriverDescriptionPtr * driverDesc);

EXTERN_API_C( OSErr )
GetDriverMemoryFragment         (Ptr                    memAddr,
                                 long                   length,
                                 ConstStr63Param        fragName,
                                 CFragConnectionID *    fragmentConnID,
                                 DriverEntryPointPtr *  fragmentMain,
                                 DriverDescriptionPtr * driverDesc);

EXTERN_API_C( OSErr )
GetDriverDiskFragment           (FSSpecPtr              fragmentSpec,
                                 CFragConnectionID *    fragmentConnID,
                                 DriverEntryPointPtr *  fragmentMain,
                                 DriverDescriptionPtr * driverDesc);

EXTERN_API_C( OSErr )
GetNamedDriverDiskFragment      (FSSpecPtr              fragmentSpec,
                                 ConstStr63Param        fragName,
                                 CFragConnectionID *    fragmentConnID,
                                 DriverEntryPointPtr *  fragmentMain,
                                 DriverDescriptionPtr * driverDesc);

EXTERN_API_C( OSErr )
InstallDriverFromFragment       (CFragConnectionID      fragmentConnID,
                                 RegEntryID *           device,
                                 UnitNumber             beginningUnit,
                                 UnitNumber             endingUnit,
                                 DriverRefNum *         refNum);

EXTERN_API_C( OSErr )
InstallDriverFromFile           (FSSpecPtr              fragmentSpec,
                                 RegEntryID *           device,
                                 UnitNumber             beginningUnit,
                                 UnitNumber             endingUnit,
                                 DriverRefNum *         refNum);

EXTERN_API_C( OSErr )
InstallDriverFromMemory         (Ptr                    memory,
                                 long                   length,
                                 ConstStr63Param        fragName,
                                 RegEntryID *           device,
                                 UnitNumber             beginningUnit,
                                 UnitNumber             endingUnit,
                                 DriverRefNum *         refNum);

EXTERN_API_C( OSErr )
InstallDriverFromResource       (SInt16                 theRsrcID,
                                 ConstStr255Param       theRsrcName,
                                 RegEntryIDPtr          theDevice,
                                 UnitNumber             theBeginningUnit,
                                 UnitNumber             theEndingUnit,
                                 DriverRefNum *         theRefNum);

EXTERN_API_C( OSErr )
InstallDriverFromDisk           (Ptr                    theDriverName,
                                 RegEntryID *           theDevice,
                                 UnitNumber             theBeginningUnit,
                                 UnitNumber             theEndingUnit,
                                 DriverRefNum *         theRefNum);

EXTERN_API_C( OSErr )
FindDriversForDevice            (RegEntryID *           device,
                                 FSSpec *               fragmentSpec,
                                 DriverDescription *    fileDriverDesc,
                                 Ptr *                  memAddr,
                                 long *                 length,
                                 StringPtr              fragName,
                                 DriverDescription *    memDriverDesc);

EXTERN_API_C( OSErr )
FindDriverForDeviceFromFile     (RegEntryID *           device,
                                 FSSpec *               fragmentSpec,
                                 DriverDescription *    driverDesc,
                                 StringPtr              fragName);

EXTERN_API_C( OSErr )
FindDriverCandidates            (RegEntryID *           deviceID,
                                 Ptr *                  propBasedDriver,
                                 RegPropertyValueSize * propBasedDriverSize,
                                 StringPtr              deviceName,
                                 MacDriverType *        propBasedDriverType,
                                 Boolean *              gotPropBasedDriver,
                                 FileBasedDriverRecordPtr  fileBasedDrivers,
                                 ItemCount *            nFileBasedDrivers);

EXTERN_API_C( OSErr )
FindDriverCandidatesDetailed    (RegEntryIDPtr          deviceID,
                                 Ptr *                  propBasedDriver,
                                 RegPropertyValueSize * propBasedDriverSize,
                                 StringPtr              deviceName,
                                 MacDriverType *        propBasedDriverType,
                                 Boolean *              gotPropBasedDriver,
                                 FileBasedDriverDetailedPtr  fileBasedDrivers,
                                 ItemCount *            nFileBasedDrivers);

EXTERN_API_C( OSErr )
ScanDriverCandidates            (RegEntryID *           deviceID,
                                 FileBasedDriverRecordPtr  fileBasedDrivers,
                                 ItemCount              nFileBasedDrivers,
                                 FileBasedDriverRecordPtr  matchingDrivers,
                                 ItemCount *            nMatchingDrivers);

EXTERN_API_C( OSErr )
ScanDriverCandidatesDetailed    (RegEntryID *           deviceID,
                                 FileBasedDriverDetailedPtr  fileBasedDrivers,
                                 ItemCount              nFileBasedDrivers,
                                 FileBasedDriverDetailedPtr  matchingDrivers,
                                 ItemCount *            nMatchingDrivers);

EXTERN_API_C( SInt16 )
CompareFileCandToPropCand       (RegEntryID *           device,
                                 StringPtr              deviceName,
                                 DriverTypePtr          propBasedCandidate,
                                 FileBasedDriverRecordPtr  fileBasedCandidate);

EXTERN_API_C( void )
GetCompatibleProperty           (RegEntryID *           device,
                                 StringPtr *            compatibleNames,
                                 ItemCount *            nCompatibleNames);

EXTERN_API_C( Boolean )
CompatibleDriverNames           (StringPtr              nameInfoStr,
                                 StringPtr              compatibleNames,
                                 ItemCount              nCompatibleNames,
                                 long *                 nameCount);

EXTERN_API_C( OSErr )
GetDriverForDevice              (RegEntryID *           device,
                                 CFragConnectionID *    fragmentConnID,
                                 DriverEntryPointPtr *  fragmentMain,
                                 DriverDescriptionPtr * driverDesc);

EXTERN_API_C( OSErr )
InstallDriverForDevice          (RegEntryID *           device,
                                 UnitNumber             beginningUnit,
                                 UnitNumber             endingUnit,
                                 DriverRefNum *         refNum);

EXTERN_API_C( OSErr )
GetDriverInformation            (DriverRefNum           refNum,
                                 UnitNumber *           unitNum,
                                 DriverFlags *          flags,
                                 DriverOpenCount *      count,
                                 StringPtr              name,
                                 RegEntryID *           device,
                                 CFragSystem7Locator *  driverLoadLocation,
                                 CFragConnectionID *    fragmentConnID,
                                 DriverEntryPointPtr *  fragmentMain,
                                 DriverDescription *    driverDesc);

EXTERN_API_C( OSErr )
GetDriverDescription            (LogicalAddress         fragmentPtr,
                                 DriverDescriptionPtr * theDriverDesc);

EXTERN_API_C( OSStatus )
GetNamedDriverDescFromFSSpec    (FSSpecPtr              fragmentSpec,
                                 StringPtr              fragName,
                                 DriverDescriptionPtr * driverDesc);

EXTERN_API_C( OSErr )
SetDriverClosureMemory          (CFragConnectionID      fragmentConnID,
                                 Boolean                holdDriverMemory);

EXTERN_API_C( OSErr )
ReplaceDriverWithFragment       (DriverRefNum           theRefNum,
                                 CFragConnectionID      fragmentConnID);

EXTERN_API_C( OSErr )
OpenInstalledDriver             (DriverRefNum           refNum,
                                 SInt8                  ioPermission);

EXTERN_API_C( OSErr )
RenameDriver                    (DriverRefNum           refNum,
                                 StringPtr              newDriverName);

EXTERN_API_C( OSErr )
RemoveDriver                    (DriverRefNum           refNum,
                                 Boolean                immediate);

EXTERN_API_C( OSErr )
LookupDrivers                   (UnitNumber             beginningUnit,
                                 UnitNumber             endingUnit,
                                 Boolean                emptyUnits,
                                 ItemCount *            returnedRefNums,
                                 DriverRefNum *         refNums);

EXTERN_API_C( UnitNumber )
HighestUnitNumber               (void);

EXTERN_API_C( OSErr )
DriverGestaltOn                 (DriverRefNum           refNum);

EXTERN_API_C( OSErr )
DriverGestaltOff                (DriverRefNum           refNum);

EXTERN_API_C( Boolean )
DriverGestaltIsOn               (DriverFlags            flags);


                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBOpenSync                      (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA000);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBOpenAsync                     (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA400);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenImmed(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBOpenImmed                     (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA200);

#endif  /* CALL_NOT_IN_CARBON */

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCloseSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCloseSync                     (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA001);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCloseAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCloseAsync                    (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA401);

#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCloseImmed(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCloseImmed                    (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA201);

#endif  /* CALL_NOT_IN_CARBON */

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBReadSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBReadSync                      (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA002);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBReadAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBReadAsync                     (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA402);

#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBReadImmed(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBReadImmed                     (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA202);

#endif  /* CALL_NOT_IN_CARBON */

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBWriteSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBWriteSync                     (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA003);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBWriteAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBWriteAsync                    (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA403);

#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBWriteImmed(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBWriteImmed                    (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA203);

/*
    PBWaitIOComplete is a friendly way for applications to monitor
    a pending asynchronous I/O operation in power-managed and
    preemptive multitasking systems.
 */
#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( OSErr )
PBWaitIOComplete                (ParmBlkPtr             paramBlock,
                                 Duration               timeout);


#if CALL_NOT_IN_CARBON
EXTERN_API( void )
AddDrive                        (short                  drvrRefNum,
                                 short                  drvNum,
                                 DrvQElPtr              qEl);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( QHdrPtr )
GetDrvQHdr                      (void)                                                      THREEWORDINLINE(0x2EBC, 0x0000, 0x0308);


EXTERN_API( DCtlHandle )
GetDCtlEntry                    (short                  refNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

/*
    SetChooserAlert used to simply set a bit in a low-mem global
    to tell the Chooser not to display its warning message when
    the printer is changed. However, under MultiFinder and System 7,
    this low-mem is swapped out when a layer change occurs, and the
    Chooser never sees the change. It is obsolete, and completely
    unsupported on the PowerPC. 68K apps can still call it if they
    wish.
    
    pascal Boolean SetChooserAlert(Boolean f);

*/
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 DriverInstall(__A0, __D0)
                                                                                            #endif
EXTERN_API( OSErr )
DriverInstall                   (DRVRHeaderPtr          drvrPtr,
                                 short                  refNum)                             ONEWORDINLINE(0xA03D);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 DriverInstallReserveMem(__A0, __D0)
                                                                                            #endif
EXTERN_API( OSErr )
DriverInstallReserveMem         (DRVRHeaderPtr          drvrPtr,
                                 short                  refNum)                             ONEWORDINLINE(0xA43D);

/*
  Note: DrvrInstall() is no longer supported, becuase it never really worked anyways.
        There will soon be a DriverInstall() which does the right thing.

        DrvrRemove has been renamed to DriverRemove.  But, InterfaceLib for PowerPC
        still exports DrvrRemove, so a macro is used to map the new name to old.

*/
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 DrvrRemove(__D0)
                                                                                            #endif
EXTERN_API( OSErr )
DrvrRemove                      (short                  refNum)                             ONEWORDINLINE(0xA03E);

#endif  /* CALL_NOT_IN_CARBON */

#define DriverRemove(refNum) DrvrRemove(refNum)

#if CALL_NOT_IN_CARBON
#if TARGET_OS_MAC
    #define MacOpenDriver OpenDriver
#endif
EXTERN_API( OSErr )
MacOpenDriver                   (ConstStr255Param       name,
                                 short *                drvrRefNum);

#if TARGET_OS_MAC
    #define MacCloseDriver CloseDriver
#endif
EXTERN_API( OSErr )
MacCloseDriver                  (short                  refNum);

EXTERN_API( OSErr )
Control                         (short                  refNum,
                                 short                  csCode,
                                 const void *           csParamPtr);

EXTERN_API( OSErr )
Status                          (short                  refNum,
                                 short                  csCode,
                                 void *                 csParamPtr);

EXTERN_API( OSErr )
KillIO                          (short                  refNum);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 Fetch(__A1)
                                                                                            #endif
EXTERN_API( long )
Fetch                           (DCtlPtr                dce)                                THREEWORDINLINE(0x2078, 0x08F4, 0x4E90);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 Stash(__A1, __D0)
                                                                                            #endif
EXTERN_API( long )
Stash                           (DCtlPtr                dce,
                                 char                   data)                               THREEWORDINLINE(0x2078, 0x08F8, 0x4E90);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter IODone(__A1, __D0)
                                                                                            #endif
EXTERN_API( void )
IODone                          (DCtlPtr                dce,
                                 OSErr                  ioResult)                           THREEWORDINLINE(0x2078, 0x08FC, 0x4E90);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBControlSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBControlSync                   (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA004);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBControlAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBControlAsync                  (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA404);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBControlImmed(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBControlImmed                  (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA204);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBStatusSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBStatusSync                    (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA005);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBStatusAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBStatusAsync                   (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA405);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBStatusImmed(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBStatusImmed                   (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA205);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBKillIOSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBKillIOSync                    (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA006);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBKillIOAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBKillIOAsync                   (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA406);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBKillIOImmed(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBKillIOImmed                   (ParmBlkPtr             paramBlock)                         ONEWORDINLINE(0xA206);

EXTERN_API( short )
OpenDeskAcc                     (ConstStr255Param       deskAccName)                        ONEWORDINLINE(0xA9B6);

EXTERN_API( void )
CloseDeskAcc                    (short                  refNum)                             ONEWORDINLINE(0xA9B7);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( short )
opendeskacc                     (const char *           deskAccName);

EXTERN_API_C( OSErr )
opendriver                      (const char *           driverName,
                                 short *                refNum);

#endif  /* CALL_NOT_IN_CARBON */

/*
    The PBxxx() routines are obsolete.  
    
    Use the PBxxxSync(), PBxxxAsync(), or PBxxxImmed version instead.
*/
#define PBControl(pb, async) ((async) ? PBControlAsync(pb) : PBControlSync(pb))
#define PBStatus(pb, async) ((async) ? PBStatusAsync(pb) : PBStatusSync(pb))
#define PBKillIO(pb, async) ((async) ? PBKillIOAsync(pb) : PBKillIOSync(pb))

#define PBOpen(pb, async) ((async) ? PBOpenAsync(pb) : PBOpenSync(pb))
#define PBClose(pb, async) ((async) ? PBCloseAsync(pb) : PBCloseSync(pb))
#define PBRead(pb, async) ((async) ? PBReadAsync(pb) : PBReadSync(pb))
#define PBWrite(pb, async) ((async) ? PBWriteAsync(pb) : PBWriteSync(pb))






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

#endif /* __DEVICES__ */

