/*
     File:       IsochronousDataHandler.h
 
     Contains:   The defines the client API to an Isochronous Data Handler, which is
 
     Version:    Technology: xxx put version here xxx
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1997-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
//
// Check for Prior Inclusion of IsochronousDataHandler.r
//  If this header is trying to be included via a Rez path, make it act
//  as a NOP.  This will allow both Rez & C files to get to use the
//  contants for the component type, subtype, and interface version.
#ifndef __ISOCHRONOUSDATAHANDLER_R__


#ifndef __ISOCHRONOUSDATAHANDLER__
#define __ISOCHRONOUSDATAHANDLER__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MOVIESFORMAT__
#include <MoviesFormat.h>
#endif

#ifndef __QUICKTIMECOMPONENTS__
#include <QuickTimeComponents.h>
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
    kIDHComponentType           = FOUR_CHAR_CODE('ihlr'),       /* Component type*/
    kIDHSubtypeDV               = FOUR_CHAR_CODE('dv  '),       /* Subtype for DV (over FireWire)*/
    kIDHSubtypeFireWireConference = FOUR_CHAR_CODE('fwc ')      /* Subtype for FW Conference*/
};


/* Version of Isochronous Data Handler API*/
enum {
    kIDHInterfaceVersion1       = 0x0001                        /* Initial relase (Summer '99)*/
};


/* atom types*/

enum {
    kIDHDeviceListAtomType      = FOUR_CHAR_CODE('dlst'),
    kIDHDeviceAtomType          = FOUR_CHAR_CODE('devc'),       /* to be defined elsewhere*/
    kIDHIsochServiceAtomType    = FOUR_CHAR_CODE('isoc'),
    kIDHIsochModeAtomType       = FOUR_CHAR_CODE('mode'),
    kIDHDeviceIDType            = FOUR_CHAR_CODE('dvid'),
    kIDHDefaultIOType           = FOUR_CHAR_CODE('dfio'),
    kIDHIsochVersionAtomType    = FOUR_CHAR_CODE('iver'),
    kIDHUniqueIDType            = FOUR_CHAR_CODE('unid'),
    kIDHNameAtomType            = FOUR_CHAR_CODE('name'),
    kIDHUseCMPAtomType          = FOUR_CHAR_CODE('ucmp'),
    kIDHIsochMediaType          = FOUR_CHAR_CODE('av  '),
    kIDHDataTypeAtomType        = FOUR_CHAR_CODE('dtyp'),
    kIDHDataSizeAtomType        = FOUR_CHAR_CODE('dsiz'),       /* ??? packet size vs. buffer size*/
    kIDHDataBufferSizeAtomType  = FOUR_CHAR_CODE('dbuf'),       /* ??? packet size vs. buffer size*/
    kIDHDataIntervalAtomType    = FOUR_CHAR_CODE('intv'),
    kIDHDataIODirectionAtomType = FOUR_CHAR_CODE('ddir'),
    kIDHSoundMediaAtomType      = FOUR_CHAR_CODE('soun'),
    kIDHSoundTypeAtomType       = FOUR_CHAR_CODE('type'),
    kIDHSoundChannelCountAtomType = FOUR_CHAR_CODE('ccnt'),
    kIDHSoundSampleSizeAtomType = FOUR_CHAR_CODE('ssiz'),
    kIDHSoundSampleRateAtomType = FOUR_CHAR_CODE('srat'),       /* same as video out... (what does this comment mean?)*/
    kIDHVideoMediaAtomType      = FOUR_CHAR_CODE('vide'),
    kIDHVideoDimensionsAtomType = FOUR_CHAR_CODE('dimn'),
    kIDHVideoResolutionAtomType = FOUR_CHAR_CODE('resl'),
    kIDHVideoRefreshRateAtomType = FOUR_CHAR_CODE('refr'),
    kIDHVideoPixelTypeAtomType  = FOUR_CHAR_CODE('pixl'),
    kIDHVideoDecompressorAtomType = FOUR_CHAR_CODE('deco'),
    kIDHVideoDecompressorTypeAtomType = FOUR_CHAR_CODE('dety'),
    kIDHVideoDecompressorContinuousAtomType = FOUR_CHAR_CODE('cont'),
    kIDHVideoDecompressorComponentAtomType = FOUR_CHAR_CODE('cmpt')
};

/* I/O Flags */
enum {
    kIDHDataTypeIsInput         = 1L << 0,
    kIDHDataTypeIsOutput        = 1L << 1,
    kIDHDataTypeIsInputAndOutput = 1L << 2
};


/* Permission Flags */
enum {
    kIDHOpenForReadTransactions = 1L << 0,
    kIDHOpenForWriteTransactions = 1L << 1,
    kIDHOpenWithExclusiveAccess = 1L << 2,
    kIDHOpenWithHeldBuffers     = 1L << 3,                      /* IDH will hold buffer until ReleaseBuffer()*/
    kIDHCloseForReadTransactions = 1L << 4,
    kIDHCloseForWriteTransactions = 1L << 5
};


/*
   Errors 
    These REALLY need to be moved into Errors.h
   ooo.needs officially assigned numbers
*/
enum {
    kIDHErrDeviceDisconnected   = -14101,
    kIDHErrInvalidDeviceID      = -14102,
    kIDHErrDeviceInUse          = -14104,
    kIDHErrDeviceNotOpened      = -14105,
    kIDHErrDeviceBusy           = -14106,
    kIDHErrDeviceReadError      = -14107,
    kIDHErrDeviceWriteError     = -14108,
    kIDHErrDeviceNotConfigured  = -14109,
    kIDHErrDeviceList           = -14110,
    kIDHErrCompletionPending    = -14111,
    kIDHErrDeviceTimeout        = -14112,
    kIDHErrInvalidIndex         = -14113,
    kIDHErrDeviceCantRead       = -14114,
    kIDHErrDeviceCantWrite      = -14115,
    kIDHErrCallNotSupported     = -14116
};




/* Holds Device Identification...*/
typedef UInt32                          IDHDeviceID;
enum {
    kIDHInvalidDeviceID         = 0,
    kIDHDeviceIDEveryDevice     = (long)0xFFFFFFFF
};

/* Values for 5 bit STYPE part of CIP header*/
enum {
    kIDHDV_SD                   = 0,
    kIDHDV_SDL                  = 1,
    kIDHDV_HD                   = 2,
    kIDHDVCPro_25               = 0x1E
};

/*  Isoch Interval Atom Data*/

struct IDHIsochInterval {
    SInt32                          duration;
    TimeScale                       scale;
};
typedef struct IDHIsochInterval         IDHIsochInterval;
/* Need to fix this.  For now, cast this as a FWReferenceID*/
typedef struct OpaquePsuedoID*          PsuedoID;
/*
   Isoch Device Status
    This is atom-like, but isn't an atom
*/

struct IDHDeviceStatus {
    UInt32                          version;
    Boolean                         physicallyConnected;
    Boolean                         readEnabled;
    Boolean                         writeEnabled;
    Boolean                         exclusiveAccess;
    UInt32                          currentBandwidth;
    UInt32                          currentChannel;
    PsuedoID                        localNodeID;                /*ooo.may go in atoms */
    SInt16                          inputStandard;              /* One of the QT input standards*/
    Boolean                         deviceActive;
    UInt8                           inputFormat;                /* Expected STYPE of data from device, if version >= 0x200*/
    UInt32                          outputFormats;              /* Bitmask for supported STYPE values, if version >= 0x200*/
};
typedef struct IDHDeviceStatus          IDHDeviceStatus;
/*
   Isochronous Data Handler Events
    
*/

typedef UInt32 IDHEvent;
enum {
    kIDHEventInvalid            = 0,
    kIDHEventDeviceAdded        = 1L << 0,                      /* A new device has been added to the bus*/
    kIDHEventDeviceRemoved      = 1L << 1,                      /* A device has been removed from the bus*/
    kIDHEventDeviceChanged      = 1L << 2,                      /* Some device has changed state on the bus*/
    kIDHEventReadEnabled        = 1L << 3,                      /* A client has enabled a device for read*/
    kIDHEventReserved1          = 1L << 4,                      /* Reserved for future use*/
    kIDHEventReadDisabled       = 1L << 5,                      /* A client has disabled a device from read*/
    kIDHEventWriteEnabled       = 1L << 6,                      /* A client has enabled a device for write*/
    kIDHEventReserved2          = 1L << 7,                      /* Reserved for future use*/
    kIDHEventWriteDisabled      = 1L << 8,                      /* A client has disabled a device for write*/
    kIDHEventEveryEvent         = (unsigned long)0xFFFFFFFF
};


typedef UInt32                          IDHNotificationID;

struct IDHEventHeader {
    IDHDeviceID                     deviceID;                   /* Device which generated event*/
    IDHNotificationID               notificationID;
    IDHEvent                        event;                      /* What the event is*/
};
typedef struct IDHEventHeader           IDHEventHeader;



/*
   IDHGenericEvent
    An IDH will often have to post events from at interrupt time.  Since memory
    allocation cannot occur from the interrupt handler, the IDH can preallocate
    storage needed for handling the event by creating some IDHGenericEvent items.
    Subsequently, when an event is generated, the type of event (specified in the
    IDHEventHeader) will dictate how the IDHGenericEvent should be interpretted.
    
    IMPORTANT NOTE : This means that a specific event structure can NEVER be greater
    than the size of the generic one.
    
*/

struct IDHGenericEvent {
    IDHEventHeader                  eventHeader;
    UInt32                          pad[4];
};
typedef struct IDHGenericEvent          IDHGenericEvent;
/*
   IDHDeviceConnectionEvent
    For kIDHEventDeviceAdded or kIDHEventDeviceRemoved events.
*/

struct IDHDeviceConnectionEvent {
    IDHEventHeader                  eventHeader;
};
typedef struct IDHDeviceConnectionEvent IDHDeviceConnectionEvent;
/*
   IDHDeviceIOEnableEvent
    For kIDHEventReadEnabled, kIDHEventReadDisabled, kIDHEventWriteEnabled, or
    kIDHEventWriteDisabled.
*/

struct IDHDeviceIOEnableEvent {
    IDHEventHeader                  eventHeader;
};
typedef struct IDHDeviceIOEnableEvent   IDHDeviceIOEnableEvent;
typedef CALLBACK_API_C( OSStatus , IDHNotificationProcPtr )(IDHGenericEvent *event, void *userData);

typedef IDHNotificationProcPtr          IDHNotificationProc;
typedef TVECTOR_UPP_TYPE(IDHNotificationProcPtr)                IDHNotificationUPP;

struct IDHParameterBlock {
    UInt32                          reserved1;
    UInt16                          reserved2;
    void *                          buffer;
    ByteCount                       requestedCount;
    ByteCount                       actualCount;
    IDHNotificationUPP              completionProc;
    void *                          refCon;
    OSErr                           result;
};
typedef struct IDHParameterBlock        IDHParameterBlock;

struct IDHResolution {
    UInt32                          x;
    UInt32                          y;
};
typedef struct IDHResolution            IDHResolution;

struct IDHDimension {
    Fixed                           x;
    Fixed                           y;
};
typedef struct IDHDimension             IDHDimension;

EXTERN_API( ComponentResult )
IDHGetDeviceList                (ComponentInstance      idh,
                                 QTAtomContainer *      deviceList)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHGetDeviceConfiguration       (ComponentInstance      idh,
                                 QTAtomSpec *           configurationID)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHSetDeviceConfiguration       (ComponentInstance      idh,
                                 const QTAtomSpec *     configurationID)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHGetDeviceStatus              (ComponentInstance      idh,
                                 const QTAtomSpec *     configurationID,
                                 IDHDeviceStatus *      status)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHGetDeviceClock               (ComponentInstance      idh,
                                 Component *            clock)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHOpenDevice                   (ComponentInstance      idh,
                                 UInt32                 permissions)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHCloseDevice                  (ComponentInstance      idh)                                FIVEWORDINLINE(0x2F3C, 0x0000, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHRead                         (ComponentInstance      idh,
                                 IDHParameterBlock *    pb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0008, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHWrite                        (ComponentInstance      idh,
                                 IDHParameterBlock *    pb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0009, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHNewNotification              (ComponentInstance      idh,
                                 IDHDeviceID            deviceID,
                                 IDHNotificationUPP     notificationProc,
                                 void *                 userData,
                                 IDHNotificationID *    notificationID)                     FIVEWORDINLINE(0x2F3C, 0x0010, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHNotifyMeWhen                 (ComponentInstance      idh,
                                 IDHNotificationID      notificationID,
                                 IDHEvent               events)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHCancelNotification           (ComponentInstance      idh,
                                 IDHNotificationID      notificationID)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x000C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHDisposeNotification          (ComponentInstance      idh,
                                 IDHNotificationID      notificationID)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHReleaseBuffer                (ComponentInstance      idh,
                                 IDHParameterBlock *    pb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x000E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHCancelPendingIO              (ComponentInstance      idh,
                                 IDHParameterBlock *    pb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHGetDeviceControl             (ComponentInstance      idh,
                                 ComponentInstance *    deviceControl)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHUpdateDeviceList             (ComponentInstance      idh,
                                 QTAtomContainer *      deviceList)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHGetDeviceTime                (ComponentInstance      idh,
                                 TimeRecord *           deviceTime)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHSetFormat                    (ComponentInstance      idh,
                                 UInt32                 format)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0013, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
IDHGetFormat                    (ComponentInstance      idh,
                                 UInt32 *               format)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0014, 0x7000, 0xA82A);


#if OPAQUE_UPP_TYPES
    EXTERN_API(IDHNotificationUPP)
    NewIDHNotificationUPP          (IDHNotificationProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeIDHNotificationUPP      (IDHNotificationUPP      userUPP);

    EXTERN_API(OSStatus)
    InvokeIDHNotificationUPP       (IDHGenericEvent *       event,
                                    void *                  userData,
                                    IDHNotificationUPP      userUPP);

#else
    #define NewIDHNotificationUPP(userRoutine)                      (userRoutine)
    #define DisposeIDHNotificationUPP(userUPP)                      
    #define InvokeIDHNotificationUPP(event, userData, userUPP)      (*userUPP)(event, userData)
#endif

/* selectors for component calls */
enum {
    kIDHGetDeviceListSelect                    = 0x0001,
    kIDHGetDeviceConfigurationSelect           = 0x0002,
    kIDHSetDeviceConfigurationSelect           = 0x0003,
    kIDHGetDeviceStatusSelect                  = 0x0004,
    kIDHGetDeviceClockSelect                   = 0x0005,
    kIDHOpenDeviceSelect                       = 0x0006,
    kIDHCloseDeviceSelect                      = 0x0007,
    kIDHReadSelect                             = 0x0008,
    kIDHWriteSelect                            = 0x0009,
    kIDHNewNotificationSelect                  = 0x000A,
    kIDHNotifyMeWhenSelect                     = 0x000B,
    kIDHCancelNotificationSelect               = 0x000C,
    kIDHDisposeNotificationSelect              = 0x000D,
    kIDHReleaseBufferSelect                    = 0x000E,
    kIDHCancelPendingIOSelect                  = 0x000F,
    kIDHGetDeviceControlSelect                 = 0x0010,
    kIDHUpdateDeviceListSelect                 = 0x0011,
    kIDHGetDeviceTimeSelect                    = 0x0012,
    kIDHSetFormatSelect                        = 0x0013,
    kIDHGetFormatSelect                        = 0x0014
};


#endif /* ifndef __ISOCHRONOUSDATAHANDLER_R__ */

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

#endif /* __ISOCHRONOUSDATAHANDLER__ */

