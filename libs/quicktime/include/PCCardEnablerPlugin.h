/*
     File:       PCCardEnablerPlugin.h
 
     Contains:   Interfacer for PCCard Manager 3.0
 
     Version:    Technology: Mac OS 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1997-2001 by Apple Computer, Inc. and SystemSoft Corporation.  All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PCCARDENABLERPLUGIN__
#define __PCCARDENABLERPLUGIN__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __NAMEREGISTRY__
#include <NameRegistry.h>
#endif

#ifndef __INTERRUPTS__
#include <Interrupts.h>
#endif

#ifndef __PCCARD__
#include <PCCard.h>
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
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

#ifndef __CARDSERVICES__
/*
    NOTE: These prototypes conflict with CardServices.*
          You cannot use both PCCardEnablerPlugin.h and CardServices.h
          
*/

/*  Copyright:  (c) 1996 SystemSoft Corporation, all rights reserved. */
/*------------------------------------------------------------------------------------
    Card Services calls exported by the Family
------------------------------------------------------------------------------------*/
/*  Card Services for Card Enablers*/

#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
CSGetCardServicesInfo           (UInt32 *               socketCount,
                                 UInt32 *               complianceLevel,
                                 UInt32 *               version);


/* Function prototypes for tuples calls */

EXTERN_API_C( OSStatus )
CSGetFirstTuple                 (PCCardSocket           socket,
                                 UInt32                 device,
                                 PCCardTupleIterator    tupleIterator,
                                 Byte                   desiredTuple,
                                 void *                 tupleData,
                                 ByteCount *            tupleBufferSize,
                                 Byte *                 foundTuple);

EXTERN_API_C( OSStatus )
CSGetNextTuple                  (PCCardTupleIterator    tupleIterator,
                                 Byte                   desiredTuple,
                                 void *                 tupleData,
                                 ByteCount *            tupleBufferSize,
                                 Byte *                 foundTuple);


/* Function prototypes for window calls*/

EXTERN_API_C( OSStatus )
CSRequestWindow                 (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 PCCardWindowAttributes  windowAttributes,
                                 PCCardAccessSpeed      windowSpeed,
                                 LogicalAddress *       windowBase,
                                 PCCardWindowSize *     windowSize,
                                 PCCardWindowOffset *   windowOffset,
                                 PCCardWindowID *       requestedWindow);

EXTERN_API_C( OSStatus )
CSReleaseWindow                 (PCCardWindowID         windowToRelease);

EXTERN_API_C( OSStatus )
CSModifyWindow                  (PCCardWindowID         windowToModify,
                                 PCCardWindowType       windowAttributes,
                                 PCCardAccessSpeed      memorySpeed,
                                 PCCardWindowOffset *   windowOffset);

EXTERN_API_C( OSStatus )
CSGetWindowBaseAddress          (PCCardWindowID         window,
                                 LogicalAddress *       baseAddress);

EXTERN_API_C( OSStatus )
CSInquireWindow                 (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 PCCardWindowID         windowID,
                                 PCCardWindowAttributes * windowAttributes,
                                 PCCardAccessSpeed *    windowParam,
                                 UInt32 *               windowBase,
                                 PCCardWindowSize *     windowSize,
                                 PCCardWindowOffset *   windowOffset);

/* Function prototypes for CIS calls*/

EXTERN_API_C( OSStatus )
CSValidateCIS                   (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 UInt32 *               cisChainCount);

EXTERN_API_C( OSStatus )
CSGetDeviceCount                (PCCardSocket           vSocket,
                                 UInt32 *               deviceCount);


/* Function prototypes for Status calls*/

EXTERN_API_C( OSStatus )
CSGetStatus                     (PCCardSocket           vSocket,
                                 UInt32 *               currentState,
                                 UInt32 *               changedState,
                                 PCCardVoltage *        Vcc,
                                 PCCardVoltage *        Vpp);


/* Function prototypes for configuration calls*/

EXTERN_API_C( OSStatus )
CSRequestConfiguration          (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 PCCardConfigOptions    configOptions,
                                 PCCardInterfaceType    socketInterface,
                                 PCCardCustomInterfaceID  customInterface,
                                 PCCardVoltage          vcc,
                                 PCCardVoltage          vpp,
                                 PCCardIRQ              IRQ,
                                 PCCardDMA              DMA,
                                 UInt32                 configRegBaseAddress,
                                 PCCardConfigPresentMask  configRegPresentMask,
                                 PCCardFunctionConfigReg * configReg);

EXTERN_API_C( OSStatus )
CSModifyConfiguration           (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 PCCardConfigOptions    modifyAttributes,
                                 PCCardIRQ              IRQ,
                                 PCCardDMA              DMA,
                                 PCCardVoltage          Vpp);

EXTERN_API_C( OSStatus )
CSReleaseConfiguration          (PCCardSocket           vSocket,
                                 UInt32                 device);

EXTERN_API_C( OSStatus )
CSSetRingIndicate               (PCCardSocket           vSocket,
                                 Boolean                setRingIndicate);

EXTERN_API_C( OSStatus )
CSResetFunction                 (PCCardSocket           vSocket,
                                 UInt32                 device);

EXTERN_API_C( OSStatus )
CSReadConfigurationRegister     (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 PCCardConfigPresentMask  whichRegister,
                                 UInt32                 configRegBaseAddress,
                                 UInt8 *                registerValue);

EXTERN_API_C( OSStatus )
CSWriteConfigurationRegister    (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 PCCardConfigPresentMask  whichRegister,
                                 UInt32                 configRegBaseAddress,
                                 UInt8                  registerValue);

/* Function prototypes for Client Support calls*/
EXTERN_API_C( OSStatus )
CSRegisterClient                (PCCardSocket           vSocket,
                                 PCCardEventMask        eventMask,
                                 PCCardEventHandler     clientCallBack,
                                 void *                 clientParam,
                                 PCCardClientID *       newClientID);

EXTERN_API_C( OSStatus )
CSDeRegisterClient              (PCCardClientID         theClientID);

EXTERN_API_C( OSStatus )
CSSetEventMask                  (PCCardClientID         theClientID,
                                 PCCardEventMask        newEventMask);

EXTERN_API_C( OSStatus )
CSGetEventMask                  (PCCardClientID         theClientID,
                                 PCCardEventMask *      newEventMask);

EXTERN_API_C( OSStatus )
CSRegisterTimer                 (PCCardClientID         registeredClientID,
                                 PCCardTimerID *        lpNewTimerID,
                                 long                   delay);

EXTERN_API_C( OSStatus )
CSDeRegisterTimer               (PCCardTimerID          timerID);

/* Function prototypes for CardBus Config Space access*/
EXTERN_API_C( OSStatus )
CSReadCardBusConfigSpace        (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 UInt32                 configOffset,
                                 Byte *                 data,
                                 UInt32                 dataSize);

EXTERN_API_C( OSStatus )
CSWriteCardBusConfigSpace       (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 UInt32                 configOffset,
                                 Byte *                 data,
                                 UInt32                 dataSize);

/*------------------------------------------------------------------------------------
    Card Enabler Types
------------------------------------------------------------------------------------*/
#endif  /* CALL_NOT_IN_CARBON */

enum {
    kUnknownDeviceType          = FOUR_CHAR_CODE('unkn'),       /* class-code = 0x00 */
    kSCSIDeviceType             = FOUR_CHAR_CODE('scsi'),       /* class-code = 0x01, sub-class = 0x00 */
    kBlockDeviceType            = FOUR_CHAR_CODE('blok'),       /* class-code = 0x01, sub-class = 0xXX */
    kNetworkDeviceType          = FOUR_CHAR_CODE('netw'),       /* class-code = 0x02 */
    kDisplayDeviceType          = FOUR_CHAR_CODE('dspl'),       /* class-code = 0x03 */
    kMultimediaDeviceType       = FOUR_CHAR_CODE('mmdv'),       /* class-code = 0x04 */
    kMemoryDeviceType           = FOUR_CHAR_CODE('mem '),       /* class-code = 0x05 */
    kBridgeDeviceType           = FOUR_CHAR_CODE('brdg'),       /* class-code = 0x06 */
    kCommDeviceType             = FOUR_CHAR_CODE('comm'),       /* class-code = 0x07 */
    kPeripheralDeviceType       = FOUR_CHAR_CODE('sysp'),       /* class-code = 0x08 */
    kInputDeviceType            = FOUR_CHAR_CODE('inpt'),       /* class-code = 0x09 */
    kDockingDeviceType          = FOUR_CHAR_CODE('dock'),       /* class-code = 0x0A */
    kProcessorDeviceType        = FOUR_CHAR_CODE('proc'),       /* class-code = 0x0B */
    kFirewireBusDeviceType      = FOUR_CHAR_CODE('firw'),       /* class-code = 0x0C, sub-class = 0x00 */
    kACCESSBusDeviceType        = FOUR_CHAR_CODE('accs'),       /* class-code = 0x0C, sub-class = 0x01 */
    kSSABusDeviceType           = FOUR_CHAR_CODE('ssa '),       /* class-code = 0x0C, sub-class = 0x02 */
    kUSBBusDeviceType           = FOUR_CHAR_CODE('usb '),       /* class-code = 0x0C, sub-class = 0x03 */
    kFibreBusDeviceType         = FOUR_CHAR_CODE('fibr'),       /* class-code = 0x0C, sub-class = 0x04 */
    kByteDeviceType             = FOUR_CHAR_CODE('byte'),       /* class-code = 0x?? */
    kSerialDeviceType           = FOUR_CHAR_CODE('ser '),       /* class-code = 0x?? */
    kParallelDeviceType         = FOUR_CHAR_CODE('parl'),       /* class-code = 0x?? */
    kAIMSDeviceType             = FOUR_CHAR_CODE('aims')        /* class-code = 0x?? */
};

typedef OSType                          PCDeviceType;
enum {
    kAttributeType              = 0,
    kMemoryType                 = 1,
    kIOType                     = 2
};

typedef UInt32                          PCCardMemoryType;
enum {
    kUnknown                    = FOUR_CHAR_CODE('unkn'),
    kPCCard16                   = FOUR_CHAR_CODE('pc16'),
    kCardBus                    = FOUR_CHAR_CODE('cdbs')
};

typedef OSType                          PCCardArchitectureType;
#define kNodeNameName                           "name"
#define k16BitPCCardName                        "16bitcard"
#define kCardbusPropertyName                    "cardbus"
#define kNodeCompatiblePropertyName         "compatible"
#define kDevice_TypeName                        "device_type"
#define kDriverISTPropertyName                  "driver-ist"
#define kFCRAccessPropertyName                  "FCRAccess"
#define kConfigRegPropertyName                  "ConfigRegValues"
#define kNodeTypePropertyName                   "PCCardNodeType"
#define kDeviceNumberPropertName                "DeviceNumber"
#define kSocketNumberPropertName                "SocketNumber"

/*------------------------------------------------------------------------------------
  Plugin Dispatch Table
------------------------------------------------------------------------------------*/
enum {
    kServiceTypePCCardEnabler   = FOUR_CHAR_CODE('card'),
    kPCCardEnablerPluginVersion = 0x00000001,
    kPCCardEnablerPluginCurrentVersion = kPCCardEnablerPluginVersion
};

/*  Card Enabler Entrypoints*/
typedef CALLBACK_API_C( OSStatus , CEValidateHardwareProc )(const RegEntryID *cardRef);
typedef CALLBACK_API_C( OSStatus , CEInitializeProc )(const RegEntryID *cardRef, Boolean replacingOld);
typedef CALLBACK_API_C( OSStatus , CECleanupProc )(const RegEntryID *cardRef, Boolean beingReplaced);
typedef CALLBACK_API_C( OSStatus , CEPowerManagementProc )(const RegEntryID *lpCardEntry, PCCardPowerOptions powerLevel);
typedef CALLBACK_API_C( OSStatus , CEHandleEventProc )(const RegEntryID *cardRef, PCCardEvent theEvent);
typedef CALLBACK_API_C( OSStatus , CEGetCardInfoProc )(const RegEntryID *cardRef, PCCardDevType *cardType, PCCardSubType *cardSubType, StringPtr cardName, StringPtr vendorName);
typedef CALLBACK_API_C( OSStatus , CEAddCardPropertiesProc )(const RegEntryID *cardRef);
typedef CALLBACK_API_C( OSStatus , CEGetDeviceCountProc )(const RegEntryID *cardRef, ItemCount *numberOfDevices);
typedef CALLBACK_API_C( OSStatus , CEGetDeviceNameProc )(UInt32 socketNumber, UInt32 deviceNumber, char *deviceName);
typedef CALLBACK_API_C( OSStatus , CEGetDeviceCompatibleProc )(const RegEntryID *deviceRef, UInt32 socketNumber, UInt32 deviceNumber, char *name);
typedef CALLBACK_API_C( OSStatus , CEGetDeviceTypeProc )(const RegEntryID *deviceRef, UInt32 socketNumber, UInt32 deviceNumber, PCDeviceType *lpDeviceType);
typedef CALLBACK_API_C( OSStatus , CEGetDeviceTypeNameProc )(const RegEntryID *deviceRef, UInt32 socketNumber, UInt32 deviceNumber, char *name);
typedef CALLBACK_API_C( OSStatus , CEAddDevicePropertiesProc )(const RegEntryID *deviceRef, UInt32 device);
typedef CALLBACK_API_C( OSStatus , CEConfigureDeviceProc )(const RegEntryID *deviceRef, UInt32 deviceNumber);
typedef CALLBACK_API_C( OSStatus , CEFinalizeDeviceProc )(UInt32 socket, UInt32 device, const RegEntryID *deviceRef);
typedef CALLBACK_API_C( OSStatus , CEValidateCISProc )(UInt32 socket, UInt32 device, UInt32 *lpCISChainCount);
typedef CALLBACK_API_C( OSStatus , CEGetFirstTupleProc )(UInt32 socket, UInt32 device, PCCardTupleIterator lpTupleIterator, Byte desiredTuple, void *lptupleData, UInt32 *lpTupleBufferSize, Byte *lpFoundTuple);
typedef CALLBACK_API_C( OSStatus , CEGetNextTupleProc )(PCCardTupleIterator lpTupleIterator, Byte desiredTuple, void *lptupleData, UInt32 *lpTupleBufferSize, Byte *lpFoundTuple);

struct PCCardEnablerPluginHeader {
    UInt32                          pluginDispatchTableVersion;
    UInt32                          reserved1;
    UInt32                          reserved2;
    UInt32                          reserved3;
};
typedef struct PCCardEnablerPluginHeader PCCardEnablerPluginHeader;

struct PCCardEnablerPluginDispatchTable {
    PCCardEnablerPluginHeader       header;

                                                                /* General functions*/
    CEValidateHardwareProc          validateHardwareProc;
    CEInitializeProc                initializeProc;
    CECleanupProc                   cleanUpProc;
    CEPowerManagementProc           setPCCardPowerLevel;

                                                                /* Card functions*/
    CEHandleEventProc               handleEventProc;
    CEGetCardInfoProc               getCardInfoProc;
    CEAddCardPropertiesProc         addCardProperties;
    CEGetDeviceCountProc            getDeviceCount;

                                                                /* Device functions*/
    CEGetDeviceNameProc             getDeviceName;
    CEGetDeviceCompatibleProc       getDeviceCompatibleNames;
    CEGetDeviceTypeProc             getDeviceType;
    CEGetDeviceTypeNameProc         getDeviceTypeName;
    CEAddDevicePropertiesProc       addDeviceProperties;
    CEConfigureDeviceProc           configureDevice;
    CEFinalizeDeviceProc            finalizeDevice;

                                                                /* Card Services Overrides...*/
    CEValidateCISProc               validateCIS;
    CEGetFirstTupleProc             getFirstTuple;
    CEGetNextTupleProc              getNextTuple;

                                                                /* InterruptHandlers...*/
    InterruptHandler                cardInterruptHandlerFunction;
    InterruptEnabler                cardInterruptEnableFunction;
    InterruptDisabler               cardInterruptDisableFunction;
};
typedef struct PCCardEnablerPluginDispatchTable PCCardEnablerPluginDispatchTable;
typedef PCCardEnablerPluginDispatchTable * PCCardEnablerPluginDispatchTablePtr;
/*------------------------------------------------------------------------------------
    PC Card Enabler Utility calls exported by the Family
------------------------------------------------------------------------------------*/
/*  Generic Enabler Entrypoints*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
CEInitializeCard                (const RegEntryID *     cardRef,
                                 Boolean                replacingOld);

EXTERN_API_C( OSStatus )
CEFinalizeCard                  (const RegEntryID *     cardRef,
                                 Boolean                beingReplaced);

EXTERN_API_C( OSStatus )
CEPowerManagement               (const RegEntryID *     lpParentID,
                                 PCCardPowerOptions     powerLevel);

EXTERN_API_C( OSStatus )
CEHandleCardEvent               (const RegEntryID *     cardRef,
                                 PCCardEvent            lTheCardEvent);

EXTERN_API_C( OSStatus )
CEGetCardInfo                   (const RegEntryID *     cardRef,
                                 PCCardDevType *        cardType,
                                 PCCardSubType *        cardSubType,
                                 StringPtr              cardName,
                                 StringPtr              vendorName);

EXTERN_API_C( OSStatus )
CEAddCardProperties             (const RegEntryID *     cardRef);

EXTERN_API_C( OSStatus )
CEGetDeviceCount                (const RegEntryID *     cardRef,
                                 ItemCount *            numberOfDevices);

/* device functions*/
EXTERN_API_C( OSStatus )
CEGetDeviceName                 (UInt32                 socketNumber,
                                 UInt32                 deviceNumber,
                                 char *                 deviceName);

EXTERN_API_C( OSStatus )
CEGetDeviceCompatibleNames      (const RegEntryID *     deviceRef,
                                 UInt32                 socketNumber,
                                 UInt32                 deviceNumber,
                                 char *                 name);

EXTERN_API_C( OSStatus )
CEGetDeviceType                 (const RegEntryID *     deviceRef,
                                 UInt32                 socketNumber,
                                 UInt32                 deviceNumber,
                                 PCDeviceType *         lpDeviceType);

EXTERN_API_C( OSStatus )
CEGetDeviceTypeName             (const RegEntryID *     deviceRef,
                                 UInt32                 socketNumber,
                                 UInt32                 deviceNumber,
                                 char *                 name);

EXTERN_API_C( OSStatus )
CEAddDeviceProperties           (const RegEntryID *     deviceRef,
                                 UInt32                 deviceNumber);

EXTERN_API_C( OSStatus )
CEConfigureDevice               (const RegEntryID *     deviceRef,
                                 UInt32                 deviceNumber);

EXTERN_API_C( OSStatus )
CEFinalizeDevice                (UInt32                 socket,
                                 UInt32                 device,
                                 const RegEntryID *     deviceRef);


/*  RegEntryID <-> socket number mapping functions*/
EXTERN_API_C( OSStatus )
CEGetSocketAndDeviceFromRegEntry (const RegEntryID *    lpNode,
                                 UInt32 *               lpSocket,
                                 UInt32 *               lpDevice);

EXTERN_API_C( OSStatus )
CEGetPhysicalSocketNumber       (const RegEntryID *     socketRef,
                                 const RegEntryID *     lpDeviceEntry,
                                 UInt32 *               lpPhysicalSocketNumber);

/*  Hardware Validation Utilities*/
EXTERN_API_C( OSStatus )
CECompareCISTPL_VERS_1          (const RegEntryID *     cardRef,
                                 Byte                   majorVersion,
                                 Byte                   minorVersion,
                                 const char *           manufacturer,
                                 const char *           productName,
                                 const char *           info1,
                                 const char *           info2,
                                 Boolean *              identical);

EXTERN_API_C( OSStatus )
CECompareCISTPL_MANFID          (const RegEntryID *     cardRef,
                                 UInt16                 manufacturerCode,
                                 UInt16                 manufacturerInfo,
                                 Boolean *              identical);

EXTERN_API_C( OSStatus )
CECompareMemory                 (const RegEntryID *     cardRef,
                                 PCCardMemoryType       memType,
                                 ByteCount              offset,
                                 ByteCount              length,
                                 Byte *                 dataToCompare,
                                 Boolean *              identical);

EXTERN_API_C( OSStatus )
CEValidateCIS                   (UInt32                 socket,
                                 UInt32                 device,
                                 UInt32 *               lpCISChainCount);

EXTERN_API_C( InterruptMemberNumber )
CEDefaultInterruptHandler       (InterruptSetMember     ISTmember,
                                 void *                 refCon,
                                 UInt32                 theIntCount);


/*------------------------------------------------------------------------------------
    PC Card Customization Resources
------------------------------------------------------------------------------------*/
#endif  /* CALL_NOT_IN_CARBON */

enum {
    kPCCardCustomInfoResType    = FOUR_CHAR_CODE('pccd'),
    kPCCardCustomInfoVersion    = 0
};


struct PCCardCustomResource {
    long                            version;
    short                           customIconID;               /*    ICN#, etc. resource ID*/
    short                           customStringsID;            /*    STR# resource ID*/
    short                           customTypeStringIndex;
    short                           customHelpStringIndex;
    OSType                          customAction;
    long                            customActionParam1;
    long                            customActionParam2;
};
typedef struct PCCardCustomResource     PCCardCustomResource;
#endif  /* !defined(__CARDSERVICES__) */


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

#endif /* __PCCARDENABLERPLUGIN__ */

