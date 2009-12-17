/*
     File:       DriverFamilyMatching.h
 
     Contains:   Interfaces for create native drivers NDRV
 
     Version:    Technology: 
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DRIVERFAMILYMATCHING__
#define __DRIVERFAMILYMATCHING__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __NAMEREGISTRY__
#include <NameRegistry.h>
#endif

#ifndef __CODEFRAGMENTS__
#include <CodeFragments.h>
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
  ##############################################
   Well known properties in the Name Registry
  ##############################################
*/

#define kPropertyName                  "name"
#define kPropertyCompatible              "compatible"
#define    kPropertyDriverPtr              "driver-ptr"
#define kPropertyDriverDesc                "driver-description"
#define kPropertyReg                   "reg"
#define kPropertyAAPLAddress          "AAPL,address"
#define kPropertyMatching                "matching"
#define kPropertyAAPLDriverName          "AAPL,driver-name"

/* CPassThru */
/*
  #########################################################
   Descriptor for Drivers and NDRVs
  #########################################################
*/
/* 
    QuickTime 3.0: "DriverType" has a name collision with cross-platform code.
    Use Mac prefix to avoid collision 
*/
/* Driver Typing Information Used to Match Drivers With Devices */

struct MacDriverType {
    Str31                           nameInfoStr;                /* Driver Name/Info String*/
    NumVersion                      version;                    /* Driver Version Number*/
};
typedef struct MacDriverType            MacDriverType;
#if TARGET_OS_MAC

typedef MacDriverType                   DriverType;
#endif  /* TARGET_OS_MAC */

typedef MacDriverType *                 DriverTypePtr;
/* OS Runtime Information Used to Setup and Maintain a Driver's Runtime Environment */
typedef OptionBits                      RuntimeOptions;
enum {
    kDriverIsLoadedUponDiscovery = 0x00000001,                  /* auto-load driver when discovered*/
    kDriverIsOpenedUponLoad     = 0x00000002,                   /* auto-open driver when loaded*/
    kDriverIsUnderExpertControl = 0x00000004,                   /* I/O expert handles loads/opens*/
    kDriverIsConcurrent         = 0x00000008,                   /* supports concurrent requests*/
    kDriverQueuesIOPB           = 0x00000010,                   /* device manager doesn't queue IOPB*/
    kDriverIsLoadedAtBoot       = 0x00000020,                   /* Driver is loaded at the boot time */
    kDriverIsForVirtualDevice   = 0x00000040,                   /* Driver is for a virtual Device */
    kDriverSupportDMSuspendAndResume = 0x00000080               /* Driver supports Device Manager Suspend and Resume command */
};


struct DriverOSRuntime {
    RuntimeOptions                  driverRuntime;              /* Options for OS Runtime*/
    Str31                           driverName;                 /* Driver's name to the OS*/
    UInt32                          driverDescReserved[8];      /* Reserved area*/
};
typedef struct DriverOSRuntime          DriverOSRuntime;
typedef DriverOSRuntime *               DriverOSRuntimePtr;
/* OS Service Information Used To Declare What APIs a Driver Supports */

typedef UInt32                          ServiceCount;

struct DriverServiceInfo {
    OSType                          serviceCategory;            /* Service Category Name*/
    OSType                          serviceType;                /* Type within Category*/
    NumVersion                      serviceVersion;             /* Version of service*/
};
typedef struct DriverServiceInfo        DriverServiceInfo;
typedef DriverServiceInfo *             DriverServiceInfoPtr;

struct DriverOSService {
    ServiceCount                    nServices;                  /* Number of Services Supported*/
    DriverServiceInfo               service[1];                 /* The List of Services (at least one)*/
};
typedef struct DriverOSService          DriverOSService;
typedef DriverOSService *               DriverOSServicePtr;
/* Categories */
enum {
    kServiceCategoryDisplay     = FOUR_CHAR_CODE('disp'),       /* Display Manager*/
    kServiceCategoryOpenTransport = FOUR_CHAR_CODE('otan'),     /* Open Transport*/
    kServiceCategoryBlockStorage = FOUR_CHAR_CODE('blok'),      /* Block Storage*/
    kServiceCategoryNdrvDriver  = FOUR_CHAR_CODE('ndrv'),       /* Generic Native Driver*/
    kServiceCategoryScsiSIM     = FOUR_CHAR_CODE('scsi'),       /* SCSI */
    kServiceCategoryFileManager = FOUR_CHAR_CODE('file'),       /* File Manager */
    kServiceCategoryIDE         = FOUR_CHAR_CODE('ide-'),       /* ide */
    kServiceCategoryADB         = FOUR_CHAR_CODE('adb-'),       /* adb */
    kServiceCategoryPCI         = FOUR_CHAR_CODE('pci-'),       /* pci bus */
                                                                /* Nu Bus */
    kServiceCategoryDFM         = FOUR_CHAR_CODE('dfm-'),       /* DFM */
    kServiceCategoryMotherBoard = FOUR_CHAR_CODE('mrbd'),       /* mother Board */
    kServiceCategoryKeyboard    = FOUR_CHAR_CODE('kybd'),       /* Keyboard */
    kServiceCategoryPointing    = FOUR_CHAR_CODE('poit'),       /* Pointing */
    kServiceCategoryRTC         = FOUR_CHAR_CODE('rtc-'),       /* RTC */
    kServiceCategoryNVRAM       = FOUR_CHAR_CODE('nram'),       /* NVRAM */
    kServiceCategorySound       = FOUR_CHAR_CODE('sond'),       /* Sound (1/3/96 MCS) */
    kServiceCategoryPowerMgt    = FOUR_CHAR_CODE('pgmt'),       /* Power Management */
    kServiceCategoryGeneric     = FOUR_CHAR_CODE('genr')        /* Generic Service Category to receive general Events */
};

/* Ndrv ServiceCategory Types */
enum {
    kNdrvTypeIsGeneric          = FOUR_CHAR_CODE('genr'),       /* generic*/
    kNdrvTypeIsVideo            = FOUR_CHAR_CODE('vido'),       /* video*/
    kNdrvTypeIsBlockStorage     = FOUR_CHAR_CODE('blok'),       /* block storage*/
    kNdrvTypeIsNetworking       = FOUR_CHAR_CODE('netw'),       /* networking*/
    kNdrvTypeIsSerial           = FOUR_CHAR_CODE('serl'),       /* serial*/
    kNdrvTypeIsParallel         = FOUR_CHAR_CODE('parl'),       /* parallel */
    kNdrvTypeIsSound            = FOUR_CHAR_CODE('sond'),       /* sound*/
    kNdrvTypeIsBusBridge        = FOUR_CHAR_CODE('brdg'),
    kNdrvTypeIsFWConference     = FOUR_CHAR_CODE('crsh'),       /* FireWire conference camera */
    kNdrvTypeIsAVC              = FOUR_CHAR_CODE('avc ')        /* FireWire AVC devices (DV cameras) */
};


typedef UInt32                          DriverDescVersion;
/*  The Driver Description */
enum {
    kTheDescriptionSignature    = FOUR_CHAR_CODE('mtej'),
    kDriverDescriptionSignature = FOUR_CHAR_CODE('pdes')
};

enum {
    kInitialDriverDescriptor    = 0,
    kVersionOneDriverDescriptor = 1
};


struct DriverDescription {
    OSType                          driverDescSignature;        /* Signature field of this structure*/
    DriverDescVersion               driverDescVersion;          /* Version of this data structure*/
    MacDriverType                   driverType;                 /* Type of Driver*/
    DriverOSRuntime                 driverOSRuntimeInfo;        /* OS Runtime Requirements of Driver*/
    DriverOSService                 driverServices;             /* Apple Service API Membership*/
};
typedef struct DriverDescription        DriverDescription;
typedef DriverDescription *             DriverDescriptionPtr;



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

#endif /* __DRIVERFAMILYMATCHING__ */

