/*
     File:       Power.h
 
     Contains:   Power Manager Interfaces.
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc.  All rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __POWER__
#define __POWER__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __MULTIPROCESSING__
#include <Multiprocessing.h>
#endif

#ifndef __NAMEREGISTRY__
#include <NameRegistry.h>
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

enum {
                                                                /* Bit positions for ModemByte */
    modemOnBit                  = 0,
    ringWakeUpBit               = 2,
    modemInstalledBit           = 3,
    ringDetectBit               = 4,
    modemOnHookBit              = 5
};

enum {
                                                                /* masks for ModemByte */
    modemOnMask                 = 0x01,
    ringWakeUpMask              = 0x04,
    modemInstalledMask          = 0x08,
    ringDetectMask              = 0x10,
    modemOnHookMask             = 0x20
};

enum {
                                                                /* bit positions for BatteryByte */
    chargerConnBit              = 0,
    hiChargeBit                 = 1,
    chargeOverFlowBit           = 2,
    batteryDeadBit              = 3,
    batteryLowBit               = 4,
    connChangedBit              = 5
};

enum {
                                                                /* masks for BatteryByte */
    chargerConnMask             = 0x01,
    hiChargeMask                = 0x02,
    chargeOverFlowMask          = 0x04,
    batteryDeadMask             = 0x08,
    batteryLowMask              = 0x10,
    connChangedMask             = 0x20
};

enum {
                                                                /* bit positions for SoundMixerByte */
    MediaBaySndEnBit            = 0,
    PCISndEnBit                 = 1,
    ZVSndEnBit                  = 2,
    PCCardSndEnBit              = 3
};

enum {
                                                                /* masks for SoundMixerByte */
    MediaBaySndEnMask           = 0x01,
    PCISndEnMask                = 0x02,
    ZVSndEnMask                 = 0x04,
    PCCardSndEnMask             = 0x08
};

enum {
                                                                /* commands to SleepQRec sleepQProc */
    kSleepRequest               = 1,
    kSleepDemand                = 2,
    kSleepWakeUp                = 3,
    kSleepRevoke                = 4,
    kSleepUnlock                = 4,
    kSleepDeny                  = 5,
    kSleepNow                   = 6,
    kDozeDemand                 = 7,
    kDozeWakeUp                 = 8,
    kDozeRequest                = 9,                            /* additional messages for Power Mgr 2.0*/
    kEnterStandby               = 10,
    kEnterRun                   = 11,
    kSuspendRequest             = 12,
    kSuspendDemand              = 13,
    kSuspendRevoke              = 14,
    kSuspendWakeUp              = 15,
    kGetPowerLevel              = 16,
    kSetPowerLevel              = 17,
    kDeviceInitiatedWake        = 18,
    kWakeToDoze                 = 19,
    kDozeToFullWakeUp           = 20,
    kGetPowerInfo               = 21,
    kGetWakeOnNetInfo           = 22
};

enum {
                                                                /* depreciated commands to SleepQRec sleepQProc */
    sleepRequest                = kSleepRequest,
    sleepDemand                 = kSleepDemand,
    sleepWakeUp                 = kSleepWakeUp,
    sleepRevoke                 = kSleepRevoke,
    sleepUnlock                 = kSleepUnlock,
    sleepDeny                   = kSleepDeny,
    sleepNow                    = kSleepNow,
    dozeDemand                  = kDozeDemand,
    dozeWakeUp                  = kDozeWakeUp,
    dozeRequest                 = kDozeRequest,
    enterStandby                = kEnterStandby,
    enterRun                    = kEnterRun,
    suspendRequestMsg           = kSuspendRequest,
    suspendDemandMsg            = kSuspendDemand,
    suspendRevokeMsg            = kSuspendRevoke,
    suspendWakeUpMsg            = kSuspendWakeUp,
    getPowerLevel               = kGetPowerLevel,
    setPowerLevel               = kSetPowerLevel
};

/* Power Handler func messages */
typedef UInt32                          PowerLevel;
/* Power levels corresponding to PCI Bus Power Management Interface Spec (PMIS) */
enum {
    kPMDevicePowerLevel_On      = 0,                            /* fully-powered 'On' state (D0 state)    */
    kPMDevicePowerLevel_D1      = 1,                            /* not used by Apple system SW         */
    kPMDevicePowerLevel_D2      = 2,                            /* not used by Apple system SW         */
    kPMDevicePowerLevel_Off     = 3                             /* main PCI bus power 'Off', but PCI standby power available (D3cold state) */
};

/* PowerHandlerProc definition */
typedef CALLBACK_API( OSStatus , PowerHandlerProcPtr )(UInt32 message, void *param, UInt32 refCon, RegEntryID *regEntryID);
typedef STACK_UPP_TYPE(PowerHandlerProcPtr)                     PowerHandlerUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(PowerHandlerUPP)
    NewPowerHandlerUPP             (PowerHandlerProcPtr     userRoutine);

    EXTERN_API(void)
    DisposePowerHandlerUPP         (PowerHandlerUPP         userUPP);

    EXTERN_API(OSStatus)
    InvokePowerHandlerUPP          (UInt32                  message,
                                    void *                  param,
                                    UInt32                  refCon,
                                    RegEntryID *            regEntryID,
                                    PowerHandlerUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppPowerHandlerProcInfo = 0x00003FF0 };                  /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewPowerHandlerUPP(userRoutine)                         (PowerHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPowerHandlerProcInfo, GetCurrentArchitecture())
    #define DisposePowerHandlerUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokePowerHandlerUPP(message, param, refCon, regEntryID, userUPP)  (OSStatus)CALL_FOUR_PARAMETER_UPP((userUPP), uppPowerHandlerProcInfo, (message), (param), (refCon), (regEntryID))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewPowerHandlerProc(userRoutine)                        NewPowerHandlerUPP(userRoutine)
#define CallPowerHandlerProc(userRoutine, message, param, refCon, regEntryID) InvokePowerHandlerUPP(message, param, refCon, regEntryID, userRoutine)
/* DriverPowerCapabilities struct */
/*
   The DriverPowerCapabilities structure is used only by PPC native drivers (ndrv).
   A native driver will export this structure like it currently does now when
   exporting a DriverDescription structure (e.g. TheDriverDescription).
   The well-defined export that will be exported must be called: TheDriverPowerCapabilities
   IMPORTANT NOTE:  ALL native drivers that match to their respective PCI
                    slot devices MUST signify to the Power Manager that
                    they can recover their device functionality after
                    waking from Sleep with PCI bus power removed.
                    If ANY PCI slot driver has not been updated and fails
                    to indicate to the Power Manager that it can recover
                    after PCI bus power is restored, then PCI bus power
                    will NOT be removed during Sleep.  Without the ability
                    to remove PCI bus power, the ability to switch the main
                    power supply on some CPUs to a very-low power state
                    will be unavailable to the System.
   The PowerCapsFlags are used to determine:
    - if a PCI slot device can recover from PCI power removal during Sleep.
      Drivers should set the 'kDevicePowerCanBeRemovedForSleep' bit if true.
        ***  ALL PCI slot device drivers must set this bit in order    ***
        ***  that PCI bus power is allowed to be removed during Sleep. ***
    - if the driver overrides the device's PCI Config space for determining
      how much Standby (e.g. 3.3V Aux pin) power is used during Sleep.
      Drivers should set the 'kDriverSpecifiesStandbyPower' bit only if the
      device's PCI Config values are known to be corrupted or do not specify
      the Standby power consumed during D3cold.
        If 'kDriverSpecifiesStandbyPower' bit is set:
          - the 'kDeviceCanGeneratePMEDuringSleep' bit should be set if
            the PCI slot device is able to use standby (3.3V Aux) power
            to assert the PME signal during Sleep..
          - the 'powerCapsStandbyPowerMilliWatts' field is used to specify
            how much Standby power is needed.  This value is only used
            if both the 'kDriverSpecifiesStandbyPower' and
            'kDeviceCanGeneratePMEDuringSleep' bits are set.
    - the method of support chosen by the driver for implementing the Power
      handler mechanism:
        If 'kDriverPowerMgtAware' is set, then the driver supports the Power
        Handler mechanism which exists in the new Power Manager (v2.0 and later).
        If 'kDriverPowerMgtUnderExpertControl' is set, then the Power Manager
        will assume that the driver's Expert/Manager will communicate with the
        driver's Power handler and the 'kDriverHasPowerHandlerExport bit'
        will be ignored.
        If 'kDriverHasPowerHandlerExport' is set, then the driver must export
        a well-defined Power handler entry point called:
            DoDriverPowerManagement
        If this bit is not set and the 'kDriverPowerMgtAware' is set, then the
        device must support Power handling within its DoDriverIO entry point.
        Drivers that implement a DoDriverIO entry point will receive
        IO Control and Status selectors that are defined in Devices.h.
        Drivers that elect to export a "DoDriverPowerManagement" entry point should
        set up the entry point as follows:
            OSStatus
            DoDriverPowerManagement( UInt32             message,
                                     PowerLevel             *powerLevel,
                                     UInt32                 refCon,
                                     RegEntryIDPtr      regEntryID);
        "DoDriverPowerManagement" entry points have the same parameters and expected
        behavior as a Power handler that has been explicitly registered via the
        Driver Services API "AddDevicePowerHandler"; the difference is that,
        during system startup, the Power Manager will automatically register the
        "DoDriverPowerManagement" entry point found within each driver.
*/

/* Used in DriverPowerCapabilities*/
typedef UInt32                          PowerCapsVersion;
enum {
    kVersionOnePowerCapabilities = 1
};

typedef OptionBits                      PowerCapsFlags;
enum {
    kDevicePowerCanBeRemovedForSleep = 0x00000001,              /* only remove PCI power when every device has this bit set */
    kDriverSpecifiesStandbyPower = 0x00000002,                  /* set when driver overrides PCI device Config values */
    kDeviceCanGeneratePMEDuringSleep = 0x00000004,              /* 3.3V standby power can be used to assert the PME# */
    kDriverPowerMgtAware        = 0x00000008,                   /* new Power handler mechanism is supported */
    kDriverPowerMgtUnderExpertControl = 0x00000010,             /* Expert or Manager will call the driver's Power handler */
    kDriverHasPowerHandlerExport = 0x00000020                   /* Type of Power handler entry point: only set this bit if the */
};

/*  driver exports a "DevicePowerManagement" entry point */
/*
   A device reliability issue arises when a device is subjected to shortened power
   cycling intervals.  The new Power Manager allows a device to specify a minimum
   value (in seconds) that a device must be powered on before being powered off.
   The system has a default value of around 5 minutes (300 seconds).  If 5 minutes
   is sufficient for a device, then use the following constant within DriverPowerCapabilities.
*/
enum {
    kUseDefaultWakeTime         = 0                             /* used when the system's default is sufficient */
};

/*
   This structure describes a device's power capabilities as well as general Power Management support.
    Note: powerCapsMinimumWakeTimeSeconds can be specified as kUseDefaultWakeTime
    if the system's default time interval - 5 minutes - is sufficient.
*/

struct DriverPowerCapabilities {
    PowerCapsVersion                powerCapsVersion;           /* Version of this data structure */
    PowerCapsFlags                  powerCapsFlags;             /* Power Management support characteristics */
    UInt32                          powerCapsStandbyPowerMilliWatts; /* Power consumed during PCI bus sleep, 0 if none */
    UInt32                          powerCapsMinimumWakeTimeSeconds; /* Safe time interval between removing and */
                                                                /*   restoring power to this device */
};
typedef struct DriverPowerCapabilities  DriverPowerCapabilities;
typedef DriverPowerCapabilities *       DriverPowerCapabilitiesPtr;
/* PCI power management support*/
enum {
    kUseDefaultMinimumWakeTime  = 0,                            /* Defaults to 5 minutes*/
    kPowerSummaryVersion        = 1,                            /* Version of PowerSummary structure.*/
    kDevicePowerInfoVersion     = 1                             /* Version of DevicePowerInfo structure.*/
};

enum {
                                                                /* PowerSummary flags*/
    kPCIPowerOffAllowed         = (1L << 0)                     /* PCI power off is allowed.*/
};

enum {
                                                                /* DevicePowerInfo flags*/
    kDevicePCIPowerOffAllowed   = (1L << 0),                    /* PCI power off is allowed for device.*/
    kDeviceSupportsPMIS         = (1L << 1),                    /* Device supports Power Mgt Interface Spec.*/
    kDeviceCanAssertPMEDuringSleep = (1L << 2),                 /* Device can assert PME# during sleep.*/
    kDeviceUsesCommonLogicPower = (1L << 3),                    /* Device uses common-logic power*/
    kDeviceDriverPresent        = (1L << 4),                    /* Driver present for device.*/
    kDeviceDriverSupportsPowerMgt = (1L << 5)                   /* Driver installed a power handler.*/
};


struct DevicePowerInfo {
    UInt32                          version;                    /* Version of this structure.*/
    RegEntryID                      regID;                      /* RegEntryID for device.*/
    OptionBits                      flags;                      /* Flags*/
    UInt32                          minimumWakeTime;            /* Minimum seconds before sleeping again.*/
    UInt32                          sleepPowerNeeded;           /* Milliwatts needed in the sleep state.*/
};
typedef struct DevicePowerInfo          DevicePowerInfo;

struct PowerSummary {
    UInt32                          version;                    /* Version of this structure.*/
    OptionBits                      flags;                      /* Flags*/
    UInt32                          sleepPowerAvailable;        /* Milliwatts available during sleep.*/
    UInt32                          sleepPowerNeeded;           /* Milliwatts needed during sleep.*/
    UInt32                          minimumWakeTime;            /* Minimum seconds before sleeping again.*/
    ItemCount                       deviceCount;                /* Number of device power info records.*/
    DevicePowerInfo                 devices[1];                 /* Array of device power info records.*/
};
typedef struct PowerSummary             PowerSummary;
enum {
                                                                /* SleepQRec.sleepQFlags */
    noCalls                     = 1,
    noRequest                   = 2,
    slpQType                    = 16,
    sleepQType                  = 16
};

/* Power Mgt Apple Event types and errors */
enum {
                                                                /* power mgt class*/
    kAEMacPowerMgtEvt           = FOUR_CHAR_CODE('pmgt'),       /* event ids*/
    kAEMacToWake                = FOUR_CHAR_CODE('wake'),
    kAEMacLowPowerSaveData      = FOUR_CHAR_CODE('pmsd'),
    kAEMacEmergencySleep        = FOUR_CHAR_CODE('emsl'),
    kAEMacEmergencyShutdown     = FOUR_CHAR_CODE('emsd')
};


/*
   These are result values returned by a Power Handler when queries
   by the Power Mgr if the device which that Power Handler represents
   woke the machine.
*/
enum {
    kDeviceDidNotWakeMachine    = 0,                            /* device did NOT wake machine*/
    kDeviceRequestsFullWake     = 1,                            /* device did wake machine and requests full wakeup*/
    kDeviceRequestsWakeToDoze   = 2                             /* device did wake machine and requests partial wakeup*/
};

/* bits in bitfield returned by PMFeatures */
enum {
    hasWakeupTimer              = 0,                            /* 1=wakeup timer is supported                    */
    hasSharedModemPort          = 1,                            /* 1=modem port shared by SCC and internal modem       */
    hasProcessorCycling         = 2,                            /* 1=processor cycling is supported                */
    mustProcessorCycle          = 3,                            /* 1=processor cycling should not be turned off          */
    hasReducedSpeed             = 4,                            /* 1=processor can be started up at reduced speed        */
    dynamicSpeedChange          = 5,                            /* 1=processor speed can be switched dynamically       */
    hasSCSIDiskMode             = 6,                            /* 1=SCSI Disk Mode is supported                 */
    canGetBatteryTime           = 7,                            /* 1=battery time can be calculated                */
    canWakeupOnRing             = 8,                            /* 1=can wakeup when the modem detects a ring          */
    hasDimmingSupport           = 9,                            /* 1=has dimming support built in (DPMS standby by default)   */
    hasStartupTimer             = 10,                           /* 1=startup timer is supported                    */
    hasChargeNotification       = 11,                           /* 1=client can determine of charge connect status change notifications available */
    hasDimSuspendSupport        = 12                            /* 1=supports dimming LCD and CRT to DPMS suspend state     */
};

/* more bits in bitfield returned by PMFeatures to be merged with above when time */
enum {
    hasWakeOnNetActivity        = 13,                           /* 1=hardware supports wake on network activity          */
    hasWakeOnLid                = 14,                           /* 1=hardware can wake when opened                   */
    canPowerOffPCIBus           = 15,                           /* 1=hardware can power off PCI bus during sleep if cards allow */
    hasDeepSleep                = 16,                           /* 1=hardware supports deep sleep (hibernation) mode   */
    hasSleep                    = 17                            /* 1=hardware supports normal (PowerBook-like) sleep   */
};

/* bits in bitfield returned by GetIntModemInfo and set by SetIntModemState */
enum {
    hasInternalModem            = 0,                            /* 1=internal modem installed               */
    intModemRingDetect          = 1,                            /* 1=internal modem has detected a ring          */
    intModemOffHook             = 2,                            /* 1=internal modem is off hook               */
    intModemRingWakeEnb         = 3,                            /* 1=wakeup on ring is enabled                 */
    extModemSelected            = 4,                            /* 1=external modem selected             */
    modemSetBit                 = 15                            /* 1=set bit, 0=clear bit (SetIntModemState)   */
};

/* bits in BatteryInfo.flags                                    */
/* ("chargerConnected" doesn't mean the charger is plugged in)  */
enum {
    batteryInstalled            = 7,                            /* 1=battery is currently connected             */
    batteryCharging             = 6,                            /* 1=battery is being charged               */
    chargerConnected            = 5                             /* 1=charger is connected to the PowerBook         */
};

enum {
    HDPwrQType                  = 0x4844,                       /* 'HD' hard disk spindown queue element type     */
    PMgrStateQType              = 0x504D                        /* 'PM' Power Manager state queue element type       */
};

/* client notification bits in PMgrQueueElement.pmNotifyBits */
enum {
    pmSleepTimeoutChanged       = 0,
    pmSleepEnableChanged        = 1,
    pmHardDiskTimeoutChanged    = 2,
    pmHardDiskSpindownChanged   = 3,
    pmDimmingTimeoutChanged     = 4,
    pmDimmingEnableChanged      = 5,
    pmDiskModeAddressChanged    = 6,
    pmProcessorCyclingChanged   = 7,
    pmProcessorSpeedChanged     = 8,
    pmWakeupTimerChanged        = 9,
    pmStartupTimerChanged       = 10,
    pmHardDiskPowerRemovedbyUser = 11,
    pmChargeStatusChanged       = 12,
    pmPowerLevelChanged         = 13,
    pmWakeOnNetActivityChanged  = 14
};

enum {
    pmSleepTimeoutChangedMask   = (1 << pmSleepTimeoutChanged),
    pmSleepEnableChangedMask    = (1 << pmSleepEnableChanged),
    pmHardDiskTimeoutChangedMask = (1 << pmHardDiskTimeoutChanged),
    pmHardDiskSpindownChangedMask = (1 << pmHardDiskSpindownChanged),
    pmDimmingTimeoutChangedMask = (1 << pmDimmingTimeoutChanged),
    pmDimmingEnableChangedMask  = (1 << pmDimmingEnableChanged),
    pmDiskModeAddressChangedMask = (1 << pmDiskModeAddressChanged),
    pmProcessorCyclingChangedMask = (1 << pmProcessorCyclingChanged),
    pmProcessorSpeedChangedMask = (1 << pmProcessorSpeedChanged),
    pmWakeupTimerChangedMask    = (1 << pmWakeupTimerChanged),
    pmStartupTimerChangedMask   = (1 << pmStartupTimerChanged),
    pmHardDiskPowerRemovedbyUserMask = (1 << pmHardDiskPowerRemovedbyUser),
    pmChargeStatusChangedMask   = (1 << pmChargeStatusChanged),
    pmPowerLevelChangedMask     = (1 << pmPowerLevelChanged),
    pmWakeOnNetActivityChangedMask = (1 << pmWakeOnNetActivityChanged)
};

/* System Activity Selectors */
enum {
    OverallAct                  = 0,                            /* general type of activity                */
    UsrActivity                 = 1,                            /* user specific type of activity           */
    NetActivity                 = 2,                            /* network specific activity             */
    HDActivity                  = 3                             /* Hard Drive activity                    */
};

/* Storage Media sleep mode defines */
enum {
    kMediaModeOn                = 0,                            /* Media active (Drive spinning and at full power)    */
    kMediaModeStandBy           = 1,                            /* Media standby (not implemented)    */
    kMediaModeSuspend           = 2,                            /* Media Idle (not implemented)   */
    kMediaModeOff               = 3                             /* Media Sleep (Drive not spinning and at min power, max recovery time)   */
};

enum {
    kMediaPowerCSCode           = 70
};


/* definitions for HDQueueElement.hdFlags   */
enum {
    kHDQueuePostBit             = 0,                            /* 1 = call this routine on the second pass     */
    kHDQueuePostMask            = (1 << kHDQueuePostBit)
};


struct ActivityInfo {
    short                           ActivityType;               /* Type of activity to be fetched.  Same as UpdateSystemActivity Selectors */
    unsigned long                   ActivityTime;               /* Time of last activity (in ticks) of specified type. */
};
typedef struct ActivityInfo             ActivityInfo;
/* information returned by GetScaledBatteryInfo */

struct BatteryInfo {
    UInt8                           flags;                      /* misc flags (see below)                  */
    UInt8                           warningLevel;               /* scaled warning level (0-255)               */
    UInt8                           reserved;                   /* reserved for internal use             */
    UInt8                           batteryLevel;               /* scaled battery level (0-255)               */
};
typedef struct BatteryInfo              BatteryInfo;

typedef SInt8                           ModemByte;
typedef SInt8                           BatteryByte;
typedef SInt8                           SoundMixerByte;
typedef long                            PMResultCode;
typedef struct SleepQRec                SleepQRec;
typedef SleepQRec *                     SleepQRecPtr;
typedef struct HDQueueElement           HDQueueElement;
typedef struct PMgrQueueElement         PMgrQueueElement;
typedef CALLBACK_API( long , SleepQProcPtr )(long message, SleepQRecPtr qRecPtr);
/*
    WARNING: SleepQProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef CALLBACK_API( void , HDSpindownProcPtr )(HDQueueElement *theElement);
typedef CALLBACK_API( void , PMgrStateChangeProcPtr )(PMgrQueueElement *theElement, long stateBits);
typedef REGISTER_UPP_TYPE(SleepQProcPtr)                        SleepQUPP;
typedef STACK_UPP_TYPE(HDSpindownProcPtr)                       HDSpindownUPP;
typedef STACK_UPP_TYPE(PMgrStateChangeProcPtr)                  PMgrStateChangeUPP;

struct SleepQRec {
    SleepQRecPtr                    sleepQLink;                 /* pointer to next queue element          */
    short                           sleepQType;                 /* queue element type (must be SleepQType)       */
    SleepQUPP                       sleepQProc;                 /* pointer to sleep universal proc ptr         */
    short                           sleepQFlags;                /* flags                       */
};


struct HDQueueElement {
    struct HDQueueElement *         hdQLink;                    /* pointer to next queue element          */
    short                           hdQType;                    /* queue element type (must be HDPwrQType)       */
    short                           hdFlags;                    /* miscellaneous flags                   */
    HDSpindownUPP                   hdProc;                     /* pointer to routine to call           */
    long                            hdUser;                     /* user-defined (variable storage, etc.)   */
};


struct PMgrQueueElement {
    struct PMgrQueueElement *       pmQLink;                    /* pointer to next queue element          */
    short                           pmQType;                    /* queue element type (must be PMgrStateQType)    */
    short                           pmFlags;                    /* miscellaneous flags                   */
    long                            pmNotifyBits;               /* bitmap of which changes to be notified for */
    PMgrStateChangeUPP              pmProc;                     /* pointer to routine to call           */
    long                            pmUser;                     /* user-defined (variable storage, etc.)   */
};



struct BatteryTimeRec {
    unsigned long                   expectedBatteryTime;        /* estimated battery time remaining (seconds) */
    unsigned long                   minimumBatteryTime;         /* minimum battery time remaining (seconds)     */
    unsigned long                   maximumBatteryTime;         /* maximum battery time remaining (seconds)     */
    unsigned long                   timeUntilCharged;           /* time until battery is fully charged (seconds)*/
};
typedef struct BatteryTimeRec           BatteryTimeRec;


struct WakeupTime {
    unsigned long                   wakeTime;                   /* wakeup time (same format as current time)   */
    Boolean                         wakeEnabled;                /* 1=enable wakeup timer, 0=disable wakeup timer  */
    SInt8                           filler;
};
typedef struct WakeupTime               WakeupTime;


struct StartupTime {
    unsigned long                   startTime;                  /* startup time (same format as current time)     */
    Boolean                         startEnabled;               /* 1=enable startup timer, 0=disable startup timer    */
    SInt8                           filler;
};
typedef struct StartupTime              StartupTime;
/* PowerSource version*/
enum {
    kVersionOnePowerSource      = 1
};

/* PowerSourceAttrs bits*/

enum {
    bSourceIsBattery            = 0,                            /* power source is battery*/
    bSourceIsAC                 = 1,                            /* power source is AC*/
    bSourceCanBeCharged         = 2,                            /* power source can be charged*/
    bSourceIsUPS                = 3,                            /* power source is UPS. NOTE: software should set bSourceIsBattery and bSourceIsAC also, as appropriate*/
    kSourceIsBatteryMask        = (1 << bSourceIsBattery),
    kSourceIsACMask             = (1 << bSourceIsAC),
    kSourceCanBeChargedMask     = (1 << bSourceCanBeCharged),
    kSourceIsUPSMask            = (1 << bSourceIsUPS)
};

/* PowerSourceFlags bits*/

enum {
    bSourceIsAvailable          = 0,                            /* power source is installed*/
    bSourceIsCharging           = 1,                            /* power source being charged*/
    bChargerIsAttached          = 2,                            /* a charger is connected*/
    kSourceIsAvailableMask      = (1 << bSourceIsAvailable),
    kSourceIsChargingMask       = (1 << bSourceIsCharging),
    kChargerIsAttachedMask      = (1 << bChargerIsAttached)
};

/* Power Capacity Types*/

enum {
    kCapacityIsActual           = 0,                            /* current capacity is expessed as actual capacity in same units as max*/
    kCapacityIsPercentOfMax     = 1                             /* current capacity is expressed as a percentage of maximumCapacity*/
};

/* Net Activity Wake Options*/
enum {
    kConfigSupportsWakeOnNetBit = 0,
    kWakeOnNetAdminAccessesBit  = 1,
    kWakeOnAllNetAccessesBit    = 2,
    kUnmountServersBeforeSleepingBit = 3,
    kConfigSupportsWakeOnNetMask = (1 << kConfigSupportsWakeOnNetBit),
    kWakeOnNetAdminAccessesMask = (1 << kWakeOnNetAdminAccessesBit),
    kWakeOnAllNetAccessesMask   = (1 << kWakeOnAllNetAccessesBit),
    kUnmountServersBeforeSleepingMask = (1 << kUnmountServersBeforeSleepingBit)
};

/* Power Source capacity usage types*/
enum {
    kCurrentCapacityIsActualValue = 0,                          /* currentCapacity is a real value in same units as maxCapacity*/
    kCurrentCapacityIsPercentOfMax = 1                          /* currentCapacity is expressed as a percentage of maxCapacity.*/
};


typedef SInt16                          PowerSourceID;

struct PowerSourceParamBlock {
    PowerSourceID                   sourceID;                   /* unique id assigned by Power Mgr*/
    UInt16                          sourceCapacityUsage;        /* how currentCapacity is used*/
    UInt32                          sourceVersion;              /* version of this record*/
    OptionBits                      sourceAttr;                 /* attribute flags (see below)*/
    OptionBits                      sourceState;                /* state flags (see below)*/
    UInt32                          currentCapacity;            /* current capacity, in*/
                                                                /*   milliwatts*/
    UInt32                          maxCapacity;                /* full capacity, in milliwatts*/
    UInt32                          timeRemaining;              /* time left to deplete, */
                                                                /*   in milliwatt-hours*/
    UInt32                          timeToFullCharge;           /* time to charge, */
                                                                /*   in milliwatt-hours*/
    UInt32                          voltage;                    /* voltage in millivolts*/
    SInt32                          current;                    /* current in milliamperes */
                                                                /*  (negative if consuming, */
                                                                /*   positive if charging)*/
};
typedef struct PowerSourceParamBlock    PowerSourceParamBlock;
typedef PowerSourceParamBlock *         PowerSourceParamBlockPtr;
EXTERN_API( OSErr )
DisableWUTime                   (void);

EXTERN_API( OSErr )
SetWUTime                       (long                   wuTime);

EXTERN_API( OSErr )
GetWUTime                       (long *                 wuTime,
                                 Byte *                 wuFlag);

EXTERN_API( OSErr )
BatteryStatus                   (Byte *                 status,
                                 Byte *                 power);

EXTERN_API( OSErr )
ModemStatus                     (Byte *                 status);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 IdleUpdate
                                                                                            #endif
EXTERN_API( long )
IdleUpdate                      (void)                                                      ONEWORDINLINE(0xA285);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetCPUSpeed
                                                                                            #endif
EXTERN_API( long )
GetCPUSpeed                     (void)                                                      TWOWORDINLINE(0x70FF, 0xA485);

EXTERN_API( void )
EnableIdle                      (void)                                                      TWOWORDINLINE(0x7000, 0xA485);

EXTERN_API( void )
DisableIdle                     (void)                                                      TWOWORDINLINE(0x7001, 0xA485);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SleepQInstall(__A0)
                                                                                            #endif
EXTERN_API( void )
SleepQInstall                   (SleepQRecPtr           qRecPtr)                            ONEWORDINLINE(0xA28A);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SleepQRemove(__A0)
                                                                                            #endif
EXTERN_API( void )
SleepQRemove                    (SleepQRecPtr           qRecPtr)                            ONEWORDINLINE(0xA48A);

EXTERN_API( void )
AOn                             (void)                                                      TWOWORDINLINE(0x7004, 0xA685);

EXTERN_API( void )
AOnIgnoreModem                  (void)                                                      TWOWORDINLINE(0x7005, 0xA685);

EXTERN_API( void )
BOn                             (void)                                                      TWOWORDINLINE(0x7000, 0xA685);

EXTERN_API( void )
AOff                            (void)                                                      TWOWORDINLINE(0x7084, 0xA685);

EXTERN_API( void )
BOff                            (void)                                                      TWOWORDINLINE(0x7080, 0xA685);


/* Public Power Management API  */
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PMSelectorCount
                                                                                            #endif
EXTERN_API( short )
PMSelectorCount                 (void)                                                      TWOWORDINLINE(0x7000, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PMFeatures
                                                                                            #endif
EXTERN_API( UInt32 )
PMFeatures                      (void)                                                      TWOWORDINLINE(0x7001, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetSleepTimeout
                                                                                            #endif
EXTERN_API( UInt8 )
GetSleepTimeout                 (void)                                                      TWOWORDINLINE(0x7002, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetSleepTimeout(__D0)
                                                                                            #endif
EXTERN_API( void )
SetSleepTimeout                 (UInt8                  timeout)                            FOURWORDINLINE(0x4840, 0x303C, 0x0003, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetHardDiskTimeout
                                                                                            #endif
EXTERN_API( UInt8 )
GetHardDiskTimeout              (void)                                                      TWOWORDINLINE(0x7004, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetHardDiskTimeout(__D0)
                                                                                            #endif
EXTERN_API( void )
SetHardDiskTimeout              (UInt8                  timeout)                            FOURWORDINLINE(0x4840, 0x303C, 0x0005, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 HardDiskPowered
                                                                                            #endif
EXTERN_API( Boolean )
HardDiskPowered                 (void)                                                      TWOWORDINLINE(0x7006, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SpinDownHardDisk
                                                                                            #endif
EXTERN_API( void )
SpinDownHardDisk                (void)                                                      TWOWORDINLINE(0x7007, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 IsSpindownDisabled
                                                                                            #endif
EXTERN_API( Boolean )
IsSpindownDisabled              (void)                                                      TWOWORDINLINE(0x7008, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetSpindownDisable(__D0)
                                                                                            #endif
EXTERN_API( void )
SetSpindownDisable              (Boolean                setDisable)                         FOURWORDINLINE(0x4840, 0x303C, 0x0009, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 HardDiskQInstall(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
HardDiskQInstall                (HDQueueElement *       theElement)                         TWOWORDINLINE(0x700A, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 HardDiskQRemove(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
HardDiskQRemove                 (HDQueueElement *       theElement)                         TWOWORDINLINE(0x700B, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter GetScaledBatteryInfo(__D0, __A0)
                                                                                            #endif
EXTERN_API( void )
GetScaledBatteryInfo            (short                  whichBattery,
                                 BatteryInfo *          theInfo)                            FIVEWORDINLINE(0x4840, 0x303C, 0x000C, 0xA09E, 0x2080);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter AutoSleepControl(__D0)
                                                                                            #endif
EXTERN_API( void )
AutoSleepControl                (Boolean                enableSleep)                        FOURWORDINLINE(0x4840, 0x303C, 0x000D, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetIntModemInfo
                                                                                            #endif
EXTERN_API( UInt32 )
GetIntModemInfo                 (void)                                                      TWOWORDINLINE(0x700E, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetIntModemState(__D0)
                                                                                            #endif
EXTERN_API( void )
SetIntModemState                (short                  theState)                           FOURWORDINLINE(0x4840, 0x303C, 0x000F, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 MaximumProcessorSpeed
                                                                                            #endif
EXTERN_API( short )
MaximumProcessorSpeed           (void)                                                      TWOWORDINLINE(0x7010, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 CurrentProcessorSpeed
                                                                                            #endif
EXTERN_API( short )
CurrentProcessorSpeed           (void)                                                      TWOWORDINLINE(0x7011, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 FullProcessorSpeed
                                                                                            #endif
EXTERN_API( Boolean )
FullProcessorSpeed              (void)                                                      TWOWORDINLINE(0x7012, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SetProcessorSpeed(__D0)
                                                                                            #endif
EXTERN_API( Boolean )
SetProcessorSpeed               (Boolean                fullSpeed)                          FOURWORDINLINE(0x4840, 0x303C, 0x0013, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetSCSIDiskModeAddress
                                                                                            #endif
EXTERN_API( short )
GetSCSIDiskModeAddress          (void)                                                      TWOWORDINLINE(0x7014, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetSCSIDiskModeAddress(__D0)
                                                                                            #endif
EXTERN_API( void )
SetSCSIDiskModeAddress          (short                  scsiAddress)                        FOURWORDINLINE(0x4840, 0x303C, 0x0015, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter GetWakeupTimer(__A0)
                                                                                            #endif
EXTERN_API( void )
GetWakeupTimer                  (WakeupTime *           theTime)                            TWOWORDINLINE(0x7016, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetWakeupTimer(__A0)
                                                                                            #endif
EXTERN_API( void )
SetWakeupTimer                  (WakeupTime *           theTime)                            TWOWORDINLINE(0x7017, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 IsProcessorCyclingEnabled
                                                                                            #endif
EXTERN_API( Boolean )
IsProcessorCyclingEnabled       (void)                                                      TWOWORDINLINE(0x7018, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter EnableProcessorCycling(__D0)
                                                                                            #endif
EXTERN_API( void )
EnableProcessorCycling          (Boolean                enable)                             FOURWORDINLINE(0x4840, 0x303C, 0x0019, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 BatteryCount
                                                                                            #endif
EXTERN_API( short )
BatteryCount                    (void)                                                      TWOWORDINLINE(0x701A, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetBatteryVoltage(__D0)
                                                                                            #endif
EXTERN_API( Fixed )
GetBatteryVoltage               (short                  whichBattery)                       FOURWORDINLINE(0x4840, 0x303C, 0x001B, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter GetBatteryTimes(__D0, __A0)
                                                                                            #endif
EXTERN_API( void )
GetBatteryTimes                 (short                  whichBattery,
                                 BatteryTimeRec *       theTimes)                           FOURWORDINLINE(0x4840, 0x303C, 0x001C, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetDimmingTimeout
                                                                                            #endif
EXTERN_API( UInt8 )
GetDimmingTimeout               (void)                                                      TWOWORDINLINE(0x701D, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetDimmingTimeout(__D0)
                                                                                            #endif
EXTERN_API( void )
SetDimmingTimeout               (UInt8                  timeout)                            FOURWORDINLINE(0x4840, 0x303C, 0x001E, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter DimmingControl(__D0)
                                                                                            #endif
EXTERN_API( void )
DimmingControl                  (Boolean                enableSleep)                        FOURWORDINLINE(0x4840, 0x303C, 0x001F, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 IsDimmingControlDisabled
                                                                                            #endif
EXTERN_API( Boolean )
IsDimmingControlDisabled        (void)                                                      TWOWORDINLINE(0x7020, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 IsAutoSlpControlDisabled
                                                                                            #endif
EXTERN_API( Boolean )
IsAutoSlpControlDisabled        (void)                                                      TWOWORDINLINE(0x7021, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PMgrStateQInstall(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PMgrStateQInstall               (PMgrQueueElement *     theElement)                         TWOWORDINLINE(0x7022, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PMgrStateQRemove(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PMgrStateQRemove                (PMgrQueueElement *     theElement)                         TWOWORDINLINE(0x7023, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 UpdateSystemActivity(__D0)
                                                                                            #endif
EXTERN_API( OSErr )
UpdateSystemActivity            (UInt8                  activity)                           FOURWORDINLINE(0x4840, 0x303C, 0x0024, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 DelaySystemIdle
                                                                                            #endif
EXTERN_API( OSErr )
DelaySystemIdle                 (void)                                                      TWOWORDINLINE(0x7025, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetStartupTimer(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
GetStartupTimer                 (StartupTime *          theTime)                            TWOWORDINLINE(0x7026, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SetStartupTimer(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SetStartupTimer                 (StartupTime *          theTime)                            TWOWORDINLINE(0x7027, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetLastActivity(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
GetLastActivity                 (ActivityInfo *         theActivity)                        TWOWORDINLINE(0x7028, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetSoundMixerState(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
GetSoundMixerState              (SoundMixerByte *       theSoundMixerByte)                  TWOWORDINLINE(0x7029, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SetSoundMixerState(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
SetSoundMixerState              (SoundMixerByte *       theSoundMixerByte)                  TWOWORDINLINE(0x702A, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetDimSuspendState
                                                                                            #endif
EXTERN_API( Boolean )
GetDimSuspendState              (void)                                                      TWOWORDINLINE(0x702B, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetDimSuspendState(__D0)
                                                                                            #endif
EXTERN_API( void )
SetDimSuspendState              (Boolean                dimSuspendState)                    FOURWORDINLINE(0x4840, 0x303C, 0x002C, 0xA09E);

#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetCoreProcessorTemperature(__A0)
                                                                                            #endif
EXTERN_API( SInt32 )
GetCoreProcessorTemperature     (MPCpuID                inCpuID)                            TWOWORDINLINE(0x702D, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetWakeOnNetworkOptions
                                                                                            #endif
EXTERN_API( OptionBits )
GetWakeOnNetworkOptions         (void)                                                      TWOWORDINLINE(0x702E, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetWakeOnNetworkOptions(__A0)
                                                                                            #endif
EXTERN_API( void )
SetWakeOnNetworkOptions         (OptionBits             inOptions)                          TWOWORDINLINE(0x702F, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 AddPowerSource(__A0)
                                                                                            #endif
EXTERN_API( OSStatus )
AddPowerSource                  (PowerSourceParamBlock * ioPowerSource)                     TWOWORDINLINE(0x7030, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 RemovePowerSource(__D0)
                                                                                            #endif
EXTERN_API( OSStatus )
RemovePowerSource               (PowerSourceID          inSourceID)                         FOURWORDINLINE(0x4840, 0x303C, 0x0031, 0xA09E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 UpdatePowerSource(__A0)
                                                                                            #endif
EXTERN_API( OSStatus )
UpdatePowerSource               (PowerSourceParamBlock * ioSource)                          TWOWORDINLINE(0x7032, 0xA09E);

EXTERN_API( Boolean )
IsServerModeEnabled             (void)                                                      TWOWORDINLINE(0x7033, 0xA09E);

EXTERN_API( void )
EnableServerMode                (Boolean                inEnable)                           FOURWORDINLINE(0x4840, 0x303C, 0x0034, 0xA09E);

/* Power Handler Management */
EXTERN_API_C( Boolean )
IsPCIPowerOffDisabled           (void);

EXTERN_API_C( void )
EnablePCIPowerOff               (Boolean                inEnable);

EXTERN_API_C( OSStatus )
AddDevicePowerHandler           (RegEntryIDPtr          regEntryID,
                                 PowerHandlerProcPtr    handler,
                                 UInt32                 refCon,
                                 char *                 deviceType);

EXTERN_API_C( OSStatus )
RemoveDevicePowerHandler        (RegEntryIDPtr          regEntryID);

EXTERN_API_C( OSStatus )
RemoveDevicePowerHandlerForProc (PowerHandlerProcPtr    proc);

EXTERN_API_C( OSStatus )
GetDevicePowerLevel             (RegEntryIDPtr          regEntryID,
                                 PowerLevel *           devicePowerLevel);

EXTERN_API_C( OSStatus )
SetDevicePowerLevel             (RegEntryIDPtr          regEntryID,
                                 PowerLevel             devicePowerLevel);


#endif  /* CALL_NOT_IN_CARBON */

#if OPAQUE_UPP_TYPES
    EXTERN_API(SleepQUPP)
    NewSleepQUPP                   (SleepQProcPtr           userRoutine);

    EXTERN_API(HDSpindownUPP)
    NewHDSpindownUPP               (HDSpindownProcPtr       userRoutine);

    EXTERN_API(PMgrStateChangeUPP)
    NewPMgrStateChangeUPP          (PMgrStateChangeProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeSleepQUPP               (SleepQUPP               userUPP);

    EXTERN_API(void)
    DisposeHDSpindownUPP           (HDSpindownUPP           userUPP);

    EXTERN_API(void)
    DisposePMgrStateChangeUPP      (PMgrStateChangeUPP      userUPP);

    EXTERN_API(long)
    InvokeSleepQUPP                (long                    message,
                                    SleepQRecPtr            qRecPtr,
                                    SleepQUPP               userUPP);

    EXTERN_API(void)
    InvokeHDSpindownUPP            (HDQueueElement *        theElement,
                                    HDSpindownUPP           userUPP);

    EXTERN_API(void)
    InvokePMgrStateChangeUPP       (PMgrQueueElement *      theElement,
                                    long                    stateBits,
                                    PMgrStateChangeUPP      userUPP);

#else
    enum { uppSleepQProcInfo = 0x00131832 };                        /* register 4_bytes:D0 Func(4_bytes:D0, 4_bytes:A0) */
    enum { uppHDSpindownProcInfo = 0x000000C0 };                    /* pascal no_return_value Func(4_bytes) */
    enum { uppPMgrStateChangeProcInfo = 0x000003C0 };               /* pascal no_return_value Func(4_bytes, 4_bytes) */
    #define NewSleepQUPP(userRoutine)                               (SleepQUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSleepQProcInfo, GetCurrentArchitecture())
    #define NewHDSpindownUPP(userRoutine)                           (HDSpindownUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppHDSpindownProcInfo, GetCurrentArchitecture())
    #define NewPMgrStateChangeUPP(userRoutine)                      (PMgrStateChangeUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPMgrStateChangeProcInfo, GetCurrentArchitecture())
    #define DisposeSleepQUPP(userUPP)                               DisposeRoutineDescriptor(userUPP)
    #define DisposeHDSpindownUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposePMgrStateChangeUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter __D0 InvokeSleepQUPP(__D0, __A0, __A1)
    long InvokeSleepQUPP(long message, SleepQRecPtr qRecPtr, SleepQUPP userUPP) = 0x4E91;
    #else
        #define InvokeSleepQUPP(message, qRecPtr, userUPP)              (long)CALL_TWO_PARAMETER_UPP((userUPP), uppSleepQProcInfo, (message), (qRecPtr))
    #endif
    #define InvokeHDSpindownUPP(theElement, userUPP)                CALL_ONE_PARAMETER_UPP((userUPP), uppHDSpindownProcInfo, (theElement))
    #define InvokePMgrStateChangeUPP(theElement, stateBits, userUPP)  CALL_TWO_PARAMETER_UPP((userUPP), uppPMgrStateChangeProcInfo, (theElement), (stateBits))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewSleepQProc(userRoutine)                              NewSleepQUPP(userRoutine)
#define NewHDSpindownProc(userRoutine)                          NewHDSpindownUPP(userRoutine)
#define NewPMgrStateChangeProc(userRoutine)                     NewPMgrStateChangeUPP(userRoutine)
#define CallSleepQProc(userRoutine, message, qRecPtr)           InvokeSleepQUPP(message, qRecPtr, userRoutine)
#define CallHDSpindownProc(userRoutine, theElement)             InvokeHDSpindownUPP(theElement, userRoutine)
#define CallPMgrStateChangeProc(userRoutine, theElement, stateBits) InvokePMgrStateChangeUPP(theElement, stateBits, userRoutine)


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

#endif /* __POWER__ */

