/*
     File:       PCCardAdapterPlugin.h
 
     Contains:   PC Card Socket Service Plug-In Programming Interface
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1996-2001 by Apple Computer, Inc. and SystemSoft Corporation. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PCCARDADAPTERPLUGIN__
#define __PCCARDADAPTERPLUGIN__

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


/*------------------------------------------------------------------------------------
  Types
------------------------------------------------------------------------------------*/
/*  Interrupt Status Change bits*/

enum {
    kSCBatteryDead              = 0x01,                         /* Battery Dead Status Change*/
    kSCBatteryLow               = 0x02,                         /* Battery Warning Status Change*/
    kSCReady                    = 0x04,                         /* Ready Status Change*/
    kSCCardDetect               = 0x08,                         /* Card Detect Status Change*/
    kSCCardEjected              = 0x10,                         /* Card Ejected*/
    kSCStatusChange             = 0x20,                         /* PC Card Status Change Signal Asserted*/
    kSCRingIndicate             = 0x40                          /* PC Card Ring Indicate Signal Asserted*/
};


/*  IRQ bits */
enum {
    kIRQLevelMask               = 0x1F,
    kIRQInvalid                 = 0x20,
    kIRQHigh                    = 0x40,
    kIRQEnable                  = 0x80
};


/*  bits for adapter characteristics flags*/

enum {
    SS_ADPT_FLG_IND             = 0x01,                         /* indicators for write-protect, card lock,*/
                                                                /* battery status, busy status, and XIP*/
                                                                /* are shared for all sockets*/
    SS_ADPT_FLG_PWR             = 0x02,                         /* if set indicates that the sockets*/
                                                                /* share the same power control*/
    SS_ADPT_FLG_DBW             = 0x04,                         /* all windows on the adapter must use*/
                                                                /* the same Data Bus Width*/
    SS_ADPT_FLG_CARDBUS         = 0x08,                         /* all sockets are CardBus PC Card capable*/
    SS_ADPT_FLG_DMA             = 0x10,                         /* the adapter has DMA capability*/
                                                                /*    bits for adapter power characteristics*/
    SS_ADPT_FLG_V33             = 0x20,                         /* adapter supports 3.3 volt power to socket*/
    SS_ADPT_FLG_V50             = 0x40,                         /* adapter supports 5.0 volt power to socket*/
    SS_ADPT_FLG_V12             = 0x80                          /* adapter supports 12.0 volt power to socket*/
};


/*------------------------------------------------------------------------------------
  Calls exported by the Family
------------------------------------------------------------------------------------*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
CSReportStatusChange            (const RegEntryID *     adapterRef,
                                 PCCardSocket           whichSocket,
                                 PCCardSCEvents         statusChange,
                                 PCCardSocketStatus     socketStatus);

/*------------------------------------------------------------------------------------
  Plugin Dispatch Table
------------------------------------------------------------------------------------*/
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( OSStatus , SSValidateHardwareProc )(const RegEntryID *deviceID);
typedef CALLBACK_API_C( OSStatus , SSInitializeProc )(const RegEntryID *deviceID, Boolean replacingOldDriver);
typedef CALLBACK_API_C( OSStatus , SSSuspendProc )(const RegEntryID *deviceID);
typedef CALLBACK_API_C( OSStatus , SSResumeProc )(const RegEntryID *deviceID);
typedef CALLBACK_API_C( OSStatus , SSFinalizeProc )(const RegEntryID *deviceID, Boolean beingReplaced);
typedef CALLBACK_API_C( OSStatus , SSInquireAdapterProc )(ItemCount *numberOfSockets, ItemCount *numberOfWindows, ItemCount *numberOfBridgeWindow, PCCardAdapterCapabilities *capabilities);
typedef CALLBACK_API_C( OSStatus , SSInquireSocketProc )(PCCardSocket socket, ItemCount *numberOfWindows, PCCardSocketStatus *supportedSocketStatus, PCCardSCEvents *supportedStatusChange);
typedef CALLBACK_API_C( OSStatus , SSGetSocketProc )(PCCardSocket socket, PCCardVoltage *Vcc, PCCardVoltage *Vpp, PCCardVoltage *Vs, PCCardInterfaceType *socketIF, PCCardCustomInterfaceID *customIFID, PCCardSocketStatus *socketStatus, PCCardSCEvents *SCEventsMask, PCCardIRQ *IRQ, PCCardDMA *DMA);
typedef CALLBACK_API_C( OSStatus , SSSetSocketProc )(PCCardSocket socket, PCCardVoltage Vcc, PCCardVoltage Vpp, PCCardInterfaceType socketIF, PCCardCustomInterfaceID customIFID, PCCardSocketStatus statusReset, PCCardSCEvents SCEventsMask, PCCardIRQ IRQ, PCCardDMA DMA);
typedef CALLBACK_API_C( OSStatus , SSResetSocketProc )(PCCardSocket socket);
typedef CALLBACK_API_C( OSStatus , SSGetStatusProc )(PCCardSocket socket, PCCardSocketStatus *currentStatus, PCCardSocketStatus *changedStatus);
typedef CALLBACK_API_C( OSStatus , SSInquireWindowProc )(PCCardSocket *socket, PCCardWindow window, PCCardWindowState *windowState, PCCardWindowSize *windowSize, PCCardWindowAlign *windowAlign);
typedef CALLBACK_API_C( OSStatus , SSGetWindowProc )(PCCardSocket *socket, PCCardWindow window, PCCardWindowState *windowState, PhysicalAddress *startAddress, PCCardWindowSize *windowSize, PCCardWindowOffset *windowOffset, PCCardAccessSpeed *memSpeed);
typedef CALLBACK_API_C( OSStatus , SSSetWindowProc )(PCCardSocket socket, PCCardWindow window, PCCardWindowState windowState, PhysicalAddress startAddress, PCCardWindowSize windowSize, PCCardWindowOffset windowOffset, PCCardAccessSpeed memSpeed);
typedef CALLBACK_API_C( OSStatus , SSGetWindowOffsetProc )(PCCardSocket *socket, PCCardWindow window, PCCardWindowState *windowState, PCCardWindowOffset *windowOffset);
typedef CALLBACK_API_C( OSStatus , SSSetWindowOffsetProc )(PCCardSocket socket, PCCardWindow window, PCCardWindowState windowState, PCCardWindowOffset windowOffset);
typedef CALLBACK_API_C( OSStatus , SSInquireBridgeWindowProc )(PCCardSocket *socket, PCCardWindow window, PCCardWindowState *windowState, PCCardWindowSize *windowSize, PCCardWindowAlign *windowAlign);
typedef CALLBACK_API_C( OSStatus , SSGetBridgeWindowProc )(PCCardSocket *socket, PCCardWindow window, PCCardWindowState *windowState, PhysicalAddress *startAddress, PCCardWindowSize *windowSize);
typedef CALLBACK_API_C( OSStatus , SSSetBridgeWindowProc )(PCCardSocket socket, PCCardWindow window, PCCardWindowState windowState, PhysicalAddress startAddress, PCCardWindowSize windowSize);
typedef CALLBACK_API_C( OSStatus , SSGetInterruptSetMemberProc )(PCCardSocket socket, InterruptSetMember *interruptSetMember);
typedef CALLBACK_API_C( OSStatus , SSEjectCardProc )(PCCardSocket socket);
enum {
    kServiceTypePCCardAdapter   = FOUR_CHAR_CODE('sock'),
    kPCCardAdapterPluginVersion = 0x00000001,
    kPCCardAdapterPluginCurrentVersion = kPCCardAdapterPluginVersion
};


struct PCCardAdapterPluginHeader {
    UInt32                          version;
    UInt32                          reserved1;
    UInt32                          reserved2;
    UInt32                          reserved3;
};
typedef struct PCCardAdapterPluginHeader PCCardAdapterPluginHeader;

struct PCCardAdapterPluginDispatchTable {
    PCCardAdapterPluginHeader       header;

    SSValidateHardwareProc          validateHardware;

    SSInitializeProc                initialize;
    SSSuspendProc                   suspend;
    SSResumeProc                    resume;
    SSFinalizeProc                  finalize;

    SSInquireAdapterProc            inquireAdapter;
    SSInquireSocketProc             inquireSocket;
    SSGetSocketProc                 getSocket;
    SSSetSocketProc                 setSocket;
    SSResetSocketProc               resetSocket;
    SSGetStatusProc                 getStatus;
    SSInquireWindowProc             inquireWindow;
    SSGetWindowProc                 getWindow;
    SSSetWindowProc                 setWindow;
    SSGetWindowOffsetProc           getWindowOffset;
    SSSetWindowOffsetProc           setWindowOffset;
    SSInquireBridgeWindowProc       inquireBridgeWindow;
    SSGetBridgeWindowProc           getBridgeWindow;
    SSSetBridgeWindowProc           setBridgeWindow;
    SSGetInterruptSetMemberProc     getInterruptSetMember;
    SSEjectCardProc                 ejectCard;
};
typedef struct PCCardAdapterPluginDispatchTable PCCardAdapterPluginDispatchTable;
typedef PCCardAdapterPluginDispatchTable * PCCardAdapterPluginDispatchTablePtr;

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

#endif /* __PCCARDADAPTERPLUGIN__ */

