/*
     File:       PCCard.h
 
     Contains:   PC Card Family Programming interface
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1996-2001 by Apple Computer, Inc.  All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PCCARD__
#define __PCCARD__

#ifndef __MACTYPES__
#include <MacTypes.h>
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
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

enum {
    kServiceCategoryPCCard      = FOUR_CHAR_CODE('pccd')
};


typedef UInt32                          PCCardEvent;
typedef UInt32                          PCCardEventMask;
typedef UInt32                          PCCardClientID;
typedef UInt32                          PCCardTimerID;
typedef UInt32                          PCCardSocket;
typedef UInt32                          PCCardWindowID;
typedef UInt32                          PCCardWindowType;
typedef UInt32                          PCCardWindowSize;
typedef UInt32                          PCCardWindowOffset;
typedef UInt32                          PCCardWindowAlign;
typedef OptionBits                      PCCardWindowState;
typedef UInt32                          PCCardAccessSpeed;
typedef UInt32                          PCCardWindowParam;
typedef UInt32                          PCCardPage;
typedef UInt32                          PCCardVoltage;
/*
    Several of the client notification bit flags have been REMOVED since the first
    release of this header.  These were unused codes that were either
    copied directly from PC Card 2.x, or from the PCMCIA standard.  In all cases,
    they were completely unimplemented and would never be sent under PCCard 3.0.
    
    The removed flags are:
        kPCCardClientInfoMessage, kPCCardSSUpdatedMessage,
        and kPCCardFunctionInterruptMessage.
    
    If your software used any of these flags, you should delete any references
    to them.  These event codes are being recycled for new features.
*/
/* Client notification bit flags */
enum {
    kPCCardNullMessage          = 0x00000000,                   /* no messages pending (not sent to clients)*/
    kPCCardInsertionMessage     = 0x00000001,                   /* card has been inserted into the socket*/
    kPCCardRemovalMessage       = 0x00000002,                   /* card has been removed from the socket- do not touch hardware!*/
                                                                /* Lock and Unlock may be used for a hardware locking card-cage. */
    kPCCardLockMessage          = 0x00000004,                   /* card is locked into the socket with a mechanical latch */
    kPCCardUnlockMessage        = 0x00000008,                   /* card is no longer locked into the socket */
                                                                /* Ready and Reset are holdovers from PC Card 2.x, but someone might be using them (!?) */
    kPCCardReadyMessage         = 0x00000010,                   /* card is ready to be accessed -- do not use! this event is never sent! (use kPCCardInsertion instead) */
    kPCCardResetMessage         = 0x00000020,                   /* physical reset has completed -- do not use! this event is never sent! (use kPCCardResetComplete instead) */
                                                                /* InsertionRequest and InsertionComplete may be used with certain cages (??) */
    kPCCardInsertionRequestMessage = 0x00000040,                /* request to insert a card using insertion motor */
    kPCCardInsertionCompleteMessage = 0x00000080,               /* insertion motor has finished inserting a card */
    kPCCardEjectionRequestMessage = 0x00000100,                 /* user or other client is requesting a card ejection*/
    kPCCardEjectionCompleteMessage = 0x00000200,                /* card ejection succeeded- do not touch hardware! */
    kPCCardEjectionFailedMessage = 0x00000400,                  /* eject failure due to electrical/mechanical problems*/
    kPCCardPMResumeMessage      = 0x00000800,                   /* power management resume */
    kPCCardPMSuspendMessage     = 0x00001000,                   /* power management suspend */
    kPCCardPMSuspendRequest     = 0x00002000,                   /* power management sleep request */
    kPCCardPMSuspendRevoke      = 0x00004000,                   /* power management sleep revoke */
    kPCCardResetPhysicalMessage = 0x00008000,                   /* physical reset is about to occur on this card -- this event is never sent! */
    kPCCardResetRequestMessage  = 0x00010000,                   /* physical reset has been requested by a client*/
    kPCCardResetCompleteMessage = 0x00020000,                   /* ResetCard() background reset has completed*/
    kPCCardBatteryDeadMessage   = 0x00040000,                   /* battery is no longer useable, data will be lost*/
    kPCCardBatteryLowMessage    = 0x00080000,                   /* battery is weak and should be replaced*/
    kPCCardWriteProtectMessage  = 0x00100000,                   /* card is now write protected*/
    kPCCardWriteEnabledMessage  = 0x00200000,                   /* card is now write enabled*/
    kPCCardDisplayEjectDSATMessage = 0x00400000,                /* about to display a DSAT for the user to re-insert a manually ejected card */
    kPCCardUnexpectedRemovalMessage = 0x02000000,               /* card has unexpectedly been manually ejected */
                                                                /* Unconfigured is a (currently unused) holdover from PC Card 2.x */
    kPCCardUnconfiguredMessage  = 0x04000000,                   /* a CARD_READY was delivered to all clients and no client */
                                                                /*    requested a configuration for the socket -- this event is never sent under PCCard 3.0! */
    kPCCardStatusChangedMessage = 0x08000000,                   /* status change for cards in I/O mode*/
    kPCCardTimerExpiredMessage  = 0x10000000,                   /* message sent when requested time has expired */
    kPCCardRequestAttentionMessage = 0x20000000,
    kPCCardEraseCompleteMessage = 0x40000000,
    kPCCardRegistrationCompleteMessage = (long)0x80000000,      /* notifications available only in PCCard 3.1 and later */
    kPCCardPMEnabledMessage     = 0x00800000                    /* power management has been enabled by the user; if appropriate, clients should call PCCardSetPowerLevel(off) */
};

typedef OptionBits                      PCCardWindowAttributes;
/*  window state (values of PCCardWindowAttributes) */
enum {
    kWSCommon                   = 0x0001,                       /* common memory window */
    kWSAttribute                = 0x0002,                       /* attribute memory window*/
    kWSIO                       = 0x0004,                       /* I/O window*/
    kWSCardBus                  = 0x0800,                       /* CardBus bridge window */
    kWSTypeMask                 = 0x0807,                       /* window type mask*/
    kWSEnabled                  = 0x0008,                       /* window enabled*/
    kWS8bit                     = 0x0010,                       /* 8-bit data width window*/
    kWS16bit                    = 0x0020,                       /* 16-bit data width window*/
    kWS32bit                    = 0x0040,                       /* 32-bit data width window*/
    kWSAutoSize                 = 0x0080,                       /* auto-size data width window*/
    kWSWidthMask                = 0x00F0,                       /* window data width mask*/
    kWSProtected                = 0x0100,                       /* window write protected*/
    kWSPrefetchable             = 0x0200,                       /* bridge window prefetchable*/
    kWSPageShared               = 0x0400,                       /* page register is shared*/
    kWSWindowSizeOffset         = 0x4000,
    kWSChangeAccessSpeed        = 0x8000                        /* Used by CSModifyWindow only */
};

/* window speed (sample values of PCCardAccessSpeed) for use in PCCardRequestWindow  */
enum {
    kAccessSpeed600ns           = 0x006A,
    kAccessSpeed500ns           = 0x005A,
    kAccessSpeed400ns           = 0x004A,
    kAccessSpeed300ns           = 0x003A,
    kAccessSpeed250ns           = 0x0001,
    kAccessSpeed200ns           = 0x0002,
    kAccessSpeed150ns           = 0x0003,
    kAccessSpeed100ns           = 0x0004
};

typedef UInt32                          PCCardInterfaceType;
/* InterfaceType bit-mask (values of PCCardInterfaceType) */
enum {
    kIFTypeMask                 = 0x03,                         /* IO & memory type mask*/
    kIFCardBus                  = 0x00,                         /* if bits 0 & 1 are zero then cardbus interface*/
    kIFMemory                   = 0x01,                         /* if bit 0 set memory IF*/
    kIFIO                       = 0x02,                         /* if bit 1 set IO IF*/
    kIFReserved                 = 0x03,                         /* bits 0 and 1 set is reserved */
    kIFDMA                      = 0x08,                         /* if bit 3 set DMA supported*/
    kIFVSKey                    = 0x10,                         /* if bit 4 set supports low Voltage key*/
    kIF33VCC                    = 0x20,                         /* if bit 5 set socket suports 3.3v*/
    kIFXXVCC                    = 0x40,                         /* if bit 6 set socket supports X.X voltage*/
    kIFYYVCC                    = 0x80                          /* if bit 7 set socket supports Y.Y voltage*/
};

typedef UInt32                          PCCardCustomInterfaceID;
/* Custom Interface Identifiers (values of PCCardCustomInterfaceID) */
enum {
    kIFCustom_None              = 0x00,                         /* no custom interface ID */
    kIFCustom_ZOOM              = 0x41                          /* ZOOM Video Mode custom interface identifier */
};

typedef OptionBits                      PCCardConfigOptions;
/* Bit mask values for PCCardConfigOptions in the configuration calls */
enum {
    kEnableIRQSteering          = 0x0002,
    kIRQChangeValid             = 0x0004,
    kVppChangeValid             = 0x0010,
    kEnableDMAChannel           = 0x0040,
    kDMAChangeValid             = 0x0080,
    kVSOverride                 = 0x0200                        /* Bits 10..31 reserved */
};

/*
   Configuration Registers Presence Mask for the FCR
   Used by PCCardConfigPresentMask
*/
enum {
    kConfigOptionPresent        = 0x00000001,
    kConfigStatusPresent        = 0x00000002,
    kPinReplacePresent          = 0x00000004,
    kSocketCopyPresent          = 0x00000008,
    kExtendedStatusPresent      = 0x00000010,
    kIOBase0Present             = 0x00000020,
    kIOBase1Present             = 0x00000040,
    kIOBase2Present             = 0x00000080,
    kIOBase3Present             = 0x00000100,
    kIOLimitPresent             = 0x00000200
};

typedef UInt32                          PCCardConfigPresentMask;
typedef UInt32                          PCCardConfigRegisterIndex;
typedef UInt32                          PCCardConfigRegisterOffset;

struct PCCardFunctionConfigReg {
    Byte                            configOptionReg;
    Byte                            configStatusReg;
    Byte                            pinReplaceReg;
    Byte                            socketCopyReg;
    Byte                            extendedStatusReg;
    Byte                            ioBase0;
    Byte                            ioBase1;
    Byte                            ioBase2;
    Byte                            ioBase3;
    Byte                            ioLimit;
};
typedef struct PCCardFunctionConfigReg  PCCardFunctionConfigReg;

typedef OptionBits                      PCCardSocketStatus;
/*  general socket status bits (values of PCCardSocketStatus) */
enum {
    kSTBatteryDead              = 0x0001,                       /* battery dead*/
    kSTBatteryLow               = 0x0002,                       /* battery low*/
    kSTBatteryGood              = 0x0004,                       /* battery good*/
    kSTPower                    = 0x0008,                       /* power is applied*/
    kST16bit                    = 0x0010,                       /* 16-bit PC Card present*/
    kSTCardBus                  = 0x0020,                       /* CardBus PC Card present*/
    kSTMemoryCard               = 0x0040,                       /* memory card present*/
    kSTIOCard                   = 0x0080,                       /* I/O card present*/
    kSTNotACard                 = 0x0100,                       /* unrecognizable PC Card detected*/
    kSTReady                    = 0x0200,                       /* ready*/
    kSTWriteProtect             = 0x0400,                       /* card is write-protected*/
    kSTDataLost                 = 0x0800,                       /* data may have been lost due to card removal*/
    kSTRingIndicate             = 0x1000,                       /* ring indicator is active*/
    kSTReserved                 = 0xE000
};

/* Bit mask for PCCardPowerOptions in the power management calls */
typedef OptionBits                      PCCardPowerOptions;
enum {
    kPCCardPowerOn              = 0x00000001,
    kPCCardPowerOff             = 0x00000002,
    kPCCardLowPower             = 0x00000004
};

typedef OptionBits                      PCCardAdapterCapabilities;
typedef UInt32                          PCCardAdapterPowerState;
typedef OptionBits                      PCCardSCEvents;
typedef UInt32                          PCCardWindow;
typedef UInt32                          PCCardIRQ;
typedef UInt32                          PCCardDMA;
/* Selectors for PCCardGetGlobalOptions */
/*  The type of the "value" parameter is provided for each selector. */
typedef UInt32                          PCCardOptionSelector;
enum {
    kPCCardPowerManagementAttrib = 1                            /* value = (Boolean*) enabled  */
};



/* Types and structures for accessing the PCCard Assigned-Address property.*/

#define kPCCardAssignedAddressProperty "assigned-addresses"
enum {
    kPCCardNonRelocatableSpace  = 0x80,
    kPCCardPrefetchableSpace    = 0x40,
    kPCCard16BitSpace           = 0x20,
    kPCCardAddressTypeCodeMask  = 0x07,
    kPCCardConfigSpace          = 0,
    kPCCardIOSpace              = 1,
    kPCCardMemorySpace          = 2,
    kPCCardAttributeMemorySpace = 4
};

typedef UInt8                           PCCardAddressSpaceFlags;
enum {
    kPCCardSocketNumberMask     = 0xF8,
    kPCCardFunctionNumberMask   = 0x07
};

typedef UInt8                           PCCardSocketFunction;
typedef UInt8                           PCCardBusNumber;
typedef UInt8                           PCCardRegisterNumber;
/*
   note: this structure is similar, but not the same as the PCIAssignedAddress structure
         16-bit cards use this structure, CardBus cards use PCIAssignedAddress
*/

struct PCCardAssignedAddress {
    PCCardAddressSpaceFlags         addressSpaceFlags;
    UInt8                           reserved;
    PCCardSocketFunction            socketFunctionNumber;
    PCCardRegisterNumber            registerNumber;
    UInt32                          address;
    UInt32                          size;
};
typedef struct PCCardAssignedAddress    PCCardAssignedAddress;
typedef PCCardAssignedAddress *         PCCardAssignedAddressPtr;
#define GetPCCardIsNonRelocatable( AssignedAddressPtr )     ((AssignedAddressPtr)->addressSpaceFlags & kPCCardNonRelocatableSpace)
#define GetPCCardIsPrefetchable( AssignedAddressPtr )       ((AssignedAddressPtr)->addressSpaceFlags & kPCCardPrefetchableSpace)
#define GetPCCardIs16BitSpace( AssignedAddressPtr )         ((AssignedAddressPtr)->addressSpaceFlags & kPCCard16BitSpace)
#define GetPCCardAddressSpaceType( AssignedAddressPtr )     ((AssignedAddressPtr)->addressSpaceFlags & kPCCardAddressTypeCodeMask)
#define GetPCCardSocketNumber( AssignedAddressPtr )         (((AssignedAddressPtr)->socketFunctionNumber & kPCCardSocketNumberMask) >> 3)
#define GetPCCardFunctionNumber( AssignedAddressPtr )       ((AssignedAddressPtr)->socketFunctionNumber & kPCCardFunctionNumberMask)
#define GetPCCardRegisterNumber( AssignedAddressPtr )       ((AssignedAddressPtr)->registerNumber)
/*----------------------------------------------------------------------
    Client Support
----------------------------------------------------------------------*/
/* Prototype for client callback */
typedef CALLBACK_API_C( OSStatus , PCCardEventHandler )(PCCardEvent theEvent, PCCardSocket vSocket, UInt32 device, UInt32 info, UInt32 MTDRequest, UInt32 *Buffer, UInt32 misc, UInt32 status, void *clientParam);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
PCCardRegisterClient            (const RegEntryID *     deviceRef,
                                 PCCardEventMask        eventMask,
                                 PCCardEventHandler     clientCallBack,
                                 void *                 clientParam,
                                 PCCardClientID *       newClientID);

EXTERN_API_C( OSStatus )
PCCardDeRegisterClient          (PCCardClientID         theClientID);

EXTERN_API_C( OSStatus )
PCCardRegisterTimer             (PCCardClientID         registeredClientID,
                                 PCCardTimerID *        lpNewTimerID,
                                 long                   delay);

EXTERN_API_C( void )
PCCardDeRegisterTimer           (PCCardTimerID          timerID);

EXTERN_API_C( OSStatus )
PCCardSetEventMask              (PCCardClientID         theClientID,
                                 PCCardEventMask        newEventMask);

EXTERN_API_C( OSStatus )
PCCardGetEventMask              (PCCardClientID         theClientID,
                                 PCCardEventMask *      newEventMask);

EXTERN_API_C( OSStatus )
PCCardGetCardServicesInfo       (ItemCount *            socketCount,
                                 UInt32 *               complianceLevel,
                                 UInt32 *               version);

EXTERN_API_C( OSStatus )
PCCardGetSocketRef              (PCCardSocket           vSocket,
                                 RegEntryID *           socketRef);

EXTERN_API_C( OSStatus )
PCCardGetCardRef                (PCCardSocket           vSocket,
                                 RegEntryID *           cardRef);

EXTERN_API_C( OSStatus )
PCCardGetDeviceRef              (PCCardSocket           vSocket,
                                 UInt32                 device,
                                 RegEntryID *           deviceRef);

EXTERN_API_C( OSStatus )
PCCardGetSocketAndDeviceFromDeviceRef (const RegEntryID * deviceRef,
                                 PCCardSocket *         vSocket,
                                 UInt32 *               device);

EXTERN_API_C( OSStatus )
PCCardGetCardRefFromDeviceRef   (const RegEntryID *     deviceRef,
                                 RegEntryID *           cardRef);


/*----------------------------------------------------------------------
    Resource Management
----------------------------------------------------------------------*/
EXTERN_API_C( OSStatus )
PCCardRequestWindow             (const RegEntryID *     deviceRef,
                                 PCCardWindowAttributes  windowAttributes,
                                 LogicalAddress *       windowBase,
                                 ByteCount *            windowSize,
                                 PCCardAccessSpeed *    windowSpeed,
                                 PCCardWindowOffset *   windowOffset,
                                 PCCardWindowID *       windowID);

EXTERN_API_C( OSStatus )
PCCardModifyWindow              (PCCardWindowID         windowID,
                                 PCCardWindowAttributes  windowAttributes,
                                 PCCardAccessSpeed      windowSpeed,
                                 PCCardWindowOffset     windowOffset);

EXTERN_API_C( OSStatus )
PCCardReleaseWindow             (PCCardWindowID         windowID);

EXTERN_API_C( OSStatus )
PCCardInquireWindow             (const RegEntryID *     deviceRef,
                                 PCCardWindowID         windowID,
                                 PCCardWindowAttributes * windowAttributes,
                                 LogicalAddress *       windowBase,
                                 ByteCount *            windowSize,
                                 PCCardAccessSpeed *    windowSpeed,
                                 PCCardWindowOffset *   windowOffset);

EXTERN_API_C( OSStatus )
PCCardGetStatus                 (const RegEntryID *     deviceRef,
                                 UInt32 *               currentState,
                                 UInt32 *               changedState,
                                 PCCardVoltage *        Vcc,
                                 PCCardVoltage *        Vpp);

EXTERN_API_C( OSStatus )
PCCardRequestConfiguration      (const RegEntryID *     deviceRef,
                                 PCCardConfigOptions    configOptions,
                                 PCCardInterfaceType    ifType,
                                 PCCardCustomInterfaceID  ifCustomType,
                                 PCCardVoltage          vcc,
                                 PCCardVoltage          vpp,
                                 LogicalAddress         configRegistersBase,
                                 PCCardConfigPresentMask  configRegistersPresent,
                                 PCCardFunctionConfigReg * configRegisterValues);

EXTERN_API_C( OSStatus )
PCCardReleaseConfiguration      (const RegEntryID *     deviceRef);

EXTERN_API_C( OSStatus )
PCCardModifyConfiguration       (const RegEntryID *     deviceRef,
                                 PCCardConfigOptions    configOptions,
                                 PCCardVoltage          vpp);

EXTERN_API_C( OSStatus )
PCCardReadConfigurationRegister (const RegEntryID *     deviceRef,
                                 PCCardConfigRegisterIndex  whichRegister,
                                 PCCardConfigRegisterOffset  offset,
                                 UInt8 *                value);

EXTERN_API_C( OSStatus )
PCCardWriteConfigurationRegister (const RegEntryID *    deviceRef,
                                 PCCardConfigRegisterIndex  whichRegister,
                                 PCCardConfigRegisterOffset  offset,
                                 UInt8                  value);

EXTERN_API_C( OSStatus )
PCCardResetFunction             (const RegEntryID *     deviceRef);

/*----------------------------------------------------------------------
    Client Utilities
----------------------------------------------------------------------*/
#endif  /* CALL_NOT_IN_CARBON */


typedef UInt8                           PCCardTupleKind;
typedef struct OpaquePCCardTupleIterator*  PCCardTupleIterator;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( PCCardTupleIterator )
PCCardNewTupleIterator          (void);

EXTERN_API_C( OSStatus )
PCCardDisposeTupleIterator      (PCCardTupleIterator    tupleIterator);

EXTERN_API_C( OSStatus )
PCCardGetFirstTuple             (const RegEntryID *     deviceID,
                                 PCCardTupleKind        desiredTuple,
                                 PCCardTupleIterator    tupleIterator,
                                 void *                 dataBuffer,
                                 UInt32 *               dataBufferSize,
                                 PCCardTupleKind *      foundTuple,
                                 UInt32 *               foundTupleDataSize);

EXTERN_API_C( OSStatus )
PCCardGetNextTuple              (const RegEntryID *     deviceRef,
                                 PCCardTupleKind        desiredTuple,
                                 PCCardTupleIterator    tupleIterator,
                                 void *                 dataBuffer,
                                 UInt32 *               dataBufferSize,
                                 PCCardTupleKind *      foundTuple,
                                 UInt32 *               foundTupleDataSize);

/*----------------------------------------------------------------------
    Miscellaneous
----------------------------------------------------------------------*/
EXTERN_API_C( OSStatus )
PCCardEject                     (const RegEntryID *     cardRef);

EXTERN_API_C( OSStatus )
PCCardEnableModemSound          (const RegEntryID *     cardRef,
                                 Boolean                enableSound);

EXTERN_API_C( OSStatus )
PCCardEnableZoomedVideo         (const RegEntryID *     cardRef,
                                 Boolean                enableZoomedVideo);

EXTERN_API_C( OSStatus )
PCCardEnableZoomedVideoSound    (const RegEntryID *     cardRef,
                                 Boolean                enableSound);

EXTERN_API_C( OSStatus )
PCCardSetPowerLevel             (const RegEntryID *     deviceRef,
                                 PCCardPowerOptions     powerLevel);

EXTERN_API_C( OSStatus )
PCCardSetRingIndicate           (const RegEntryID *     deviceRef,
                                 Boolean                setRingIndicate);

EXTERN_API_C( OSStatus )
PCCardGetGlobalOptions          (PCCardOptionSelector   selector,
                                 void *                 value);

#endif  /* CALL_NOT_IN_CARBON */

typedef UInt32                          PCCardDevType;
typedef UInt32                          PCCardSubType;
/* values for PCCardType and PCCardSubType*/
enum {
    kPCCardUnknownType          = 0,
    kPCCardMultiFunctionType    = 1,
    kPCCardMemoryType           = 2,
    kPCCardNullSubType          = 0,                            /* Memory sub types */
    kPCCardRomSubType           = 1,
    kPCCardOTPromSubType        = 2,
    kPCCardEpromSubType         = 3,
    kPCCardEEpromSubType        = 4,
    kPCCardFlashSubType         = 5,
    kPCCardSramSubType          = 6,
    kPCCardDramSubType          = 7,
    kPCCardSerialPortType       = 3,
    kPCCardSerialOnlySubType    = 0,
    kPCCardDataModemSubType     = 1,
    kPCCardFaxModemSubType      = 2,
    kPCCardFaxAndDataModemMask  = (kPCCardDataModemSubType | kPCCardFaxModemSubType),
    kPCCardVoiceEncodingSubType = 4,
    kPCCardParallelPortType     = 4,
    kPCCardFixedDiskType        = 5,
    kPCCardUnknownFixedDiskType = 0,
    kPCCardATAInterfaceDiskSubType = 1,
    kPCCardRotatingDeviceSubType = (0 << 7),
    kPCCardSiliconDevice        = (1 << 7),
    kPCCardVideoAdaptorType     = 6,
    kPCCardNetworkAdaptorType   = 7,
    kPCCardArcNetSubType        = 1,                            /* network sub types */
    kPCCardEthernetSubType      = 2,
    kPCCardTokenRingSubType     = 3,
    kPCCardLocalTalkSubType     = 4,
    kPCCardFDDI_CDDISubType     = 5,
    kPCCardATMSubType           = 6,
    kPCCardWirelessSubType      = 7,
    kPCCardAIMSType             = 8,
    kPCCardSCSIType             = 9,
    kPCCardSerialBusType        = 10,
    kPCCardUSBBusSubType        = 1,
    kPCCardFirewireBusSubType   = 2
};

#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
PCCardGetCardInfo               (const RegEntryID *     cardRef,
                                 PCCardDevType *        cardType,
                                 PCCardSubType *        cardSubType,
                                 StringPtr              cardName,
                                 StringPtr              vendorName);

#endif  /* CALL_NOT_IN_CARBON */

enum {
    kPCCard16HardwareType       = FOUR_CHAR_CODE('pc16'),
    kCardBusHardwareType        = FOUR_CHAR_CODE('cdbs')
};

typedef UInt32                          PCCardHardwareType;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
PCCardGetCardType               (const RegEntryID *     socketRef,
                                 PCCardHardwareType *   cardType);


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

#endif /* __PCCARD__ */

