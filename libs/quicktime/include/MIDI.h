/*
     File:       MIDI.h
 
     Contains:   MIDI Manager Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1988-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MIDI__
#define __MIDI__

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif

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

/*
                        * * *  N O T E  * * * 

    This file has been updated to include MIDI 2.0 interfaces.  
    
    The MIDI 2.0 interfaces were developed for the classic 68K runtime.
    Since then, Apple has created the PowerPC and CFM 68K runtimes.
    Currently, the extra functions in MIDI 2.0 are not in InterfaceLib
    and thus not callable from PowerPC and CFM 68K runtimes (you'll
    get a linker error).  
*/
enum {
    midiMaxNameLen              = 31                            /*maximum number of characters in port and client names*/
};

enum {
                                                                /* Time formats */
    midiFormatMSec              = 0,                            /*milliseconds*/
    midiFormatBeats             = 1,                            /*beats*/
    midiFormat24fpsBit          = 2,                            /*24 frames/sec.*/
    midiFormat25fpsBit          = 3,                            /*25 frames/sec.*/
    midiFormat30fpsDBit         = 4,                            /*30 frames/sec. drop-frame*/
    midiFormat30fpsBit          = 5,                            /*30 frames/sec.*/
    midiFormat24fpsQF           = 6,                            /*24 frames/sec. longInt format */
    midiFormat25fpsQF           = 7,                            /*25 frames/sec. longInt format */
    midiFormat30fpsDQF          = 8,                            /*30 frames/sec. drop-frame longInt format */
    midiFormat30fpsQF           = 9                             /*30 frames/sec. longInt format */
};

enum {
    midiInternalSync            = 0,                            /*internal sync*/
    midiExternalSync            = 1                             /*external sync*/
};

enum {
                                                                /* Port types*/
    midiPortTypeTime            = 0,                            /*time port*/
    midiPortTypeInput           = 1,                            /*input port*/
    midiPortTypeOutput          = 2,                            /*output port*/
    midiPortTypeTimeInv         = 3,                            /*invisible time port*/
    midiPortInvisible           = 0x8000,                       /*logical OR this to other types to make invisible ports*/
    midiPortTypeMask            = 0x0007                        /*logical AND with this to convert new port types to old, i.e. to strip the property bits*/
};

enum {
                                                                /* OffsetTimes  */
    midiGetEverything           = 0x7FFFFFFF,                   /*get all packets, regardless of time stamps*/
    midiGetNothing              = (long)0x80000000,             /*get no packets, regardless of time stamps*/
    midiGetCurrent              = 0x00000000                    /*get current packets only*/
};

/*    MIDI data and messages are passed in MIDIPacket records (see below).
    The first byte of every MIDIPacket contains a set of flags

    bits 0-1    00 = new MIDIPacket, not continued
                     01 = begining of continued MIDIPacket
                     10 = end of continued MIDIPacket
                     11 = continuation
    bits 2-3     reserved

    bits 4-6      000 = packet contains MIDI data

                  001 = packet contains MIDI Manager message

    bit 7         0 = MIDIPacket has valid stamp
                  1 = stamp with current clock
*/
enum {
    midiContMask                = 0x03,
    midiNoCont                  = 0x00,
    midiStartCont               = 0x01,
    midiMidCont                 = 0x03,
    midiEndCont                 = 0x02,
    midiTypeMask                = 0x70,
    midiMsgType                 = 0x00,
    midiMgrType                 = 0x10,
    midiTimeStampMask           = 0x80,
    midiTimeStampCurrent        = 0x80,
    midiTimeStampValid          = 0x00
};

enum {
                                                                /* MIDIPacket command words (the first word in the data field for midiMgrType messages) */
    midiOverflowErr             = 0x0001,
    midiSCCErr                  = 0x0002,
    midiPacketErr               = 0x0003,                       /*all command words less than this value are error indicators*/
    midiMaxErr                  = 0x00FF
};

enum {
                                                                /* Valid results to be returned by readHooks */
    midiKeepPacket              = 0,
    midiMorePacket              = 1,
    midiNoMorePacket            = 2,
    midiHoldPacket              = 3
};

enum {
                                                                /* Driver calls */
    midiOpenDriver              = 1,
    midiCloseDriver             = 2
};

enum {
    mdvrAbortNotesOff           = 0,                            /*abort previous mdvrNotesOff request*/
    mdvrChanNotesOff            = 1,                            /*generate channel note off messages*/
    mdvrAllNotesOff             = 2                             /*generate all note off messages*/
};

enum {
    mdvrStopOut                 = 0,                            /*stop calling MDVROut temporarily*/
    mdvrStartOut                = 1                             /*resume calling MDVROut*/
};


struct MIDIPacket {
    UInt8                           flags;
    UInt8                           len;
    long                            tStamp;
    UInt8                           data[249];
};
typedef struct MIDIPacket               MIDIPacket;
typedef MIDIPacket *                    MIDIPacketPtr;
typedef CALLBACK_API( short , MIDIReadHookProcPtr )(MIDIPacketPtr myPacket, long myRefCon);
typedef CALLBACK_API( void , MIDITimeProcPtr )(long curTime, long myRefCon);
typedef CALLBACK_API( void , MIDIConnectionProcPtr )(short refnum, long refcon, short portType, OSType clientID, OSType portID, Boolean connect, short direction);
typedef CALLBACK_API( long , MDVRCommProcPtr )(short refnum, short request, long refCon);
typedef CALLBACK_API( void , MDVRTimeCodeProcPtr )(short refnum, short newFormat, long refCon);
typedef CALLBACK_API( void , MDVRReadProcPtr )(char *midiChars, short length, long refCon);
typedef STACK_UPP_TYPE(MIDIReadHookProcPtr)                     MIDIReadHookUPP;
typedef STACK_UPP_TYPE(MIDITimeProcPtr)                         MIDITimeUPP;
typedef STACK_UPP_TYPE(MIDIConnectionProcPtr)                   MIDIConnectionUPP;
typedef STACK_UPP_TYPE(MDVRCommProcPtr)                         MDVRCommUPP;
typedef STACK_UPP_TYPE(MDVRTimeCodeProcPtr)                     MDVRTimeCodeUPP;
typedef STACK_UPP_TYPE(MDVRReadProcPtr)                         MDVRReadUPP;

struct MIDIClkInfo {
    short                           syncType;                   /*synchronization external/internal*/
    long                            curTime;                    /*current value of port's clock*/
    short                           format;                     /*time code format*/
};
typedef struct MIDIClkInfo              MIDIClkInfo;

struct MIDIIDRec {
    OSType                          clientID;
    OSType                          portID;
};
typedef struct MIDIIDRec                MIDIIDRec;

struct MIDIPortInfo {
    short                           portType;                   /*type of port*/
    MIDIIDRec                       timeBase;                   /*MIDIIDRec for time base*/
    short                           numConnects;                /*number of connections*/
    MIDIIDRec                       cList[1];                   /*ARRAY [1..numConnects] of MIDIIDRec*/
};
typedef struct MIDIPortInfo             MIDIPortInfo;
typedef MIDIPortInfo *                  MIDIPortInfoPtr;
typedef MIDIPortInfoPtr *               MIDIPortInfoHdl;
typedef MIDIPortInfoPtr *               MIDIPortInfoHandle;

struct MIDIPortParams {
    OSType                          portID;                     /*ID of port, unique within client*/
    short                           portType;                   /*Type of port - input, output, time, etc.*/
    short                           timeBase;                   /*refnum of time base, 0 if none*/
    long                            offsetTime;                 /*offset for current time stamps*/
    MIDIReadHookUPP                 readHook;                   /*routine to call when input data is valid*/
    long                            refCon;                     /*refcon for port (for client use)*/
    MIDIClkInfo                     initClock;                  /*initial settings for a time base*/
    Str255                          name;                       /*name of the port, This is a real live string, not a ptr.*/
};
typedef struct MIDIPortParams           MIDIPortParams;
typedef MIDIPortParams *                MIDIPortParamsPtr;

struct MIDIIDList {
    short                           numIDs;
    OSType                          list[1];
};
typedef struct MIDIIDList               MIDIIDList;
typedef MIDIIDList *                    MIDIIDListPtr;
typedef MIDIIDListPtr *                 MIDIIDListHdl;
typedef MIDIIDListPtr *                 MIDIIDListHandle;
/* MDVR Control structs*/

struct MDVRInCtlRec {
    short                           timeCodeClock;              /*refnum of time base for time code*/
    short                           timeCodeFormat;             /*format of time code output*/
    MDVRReadUPP                     readProc;                   /*proc to call with intput characters*/
    MDVRCommUPP                     commProc;                   /*proc to call for handshaking*/
    long                            refCon;                     /*refCon passed to readProc, commProc*/
};
typedef struct MDVRInCtlRec             MDVRInCtlRec;

typedef MDVRInCtlRec *                  MDVRInCtlPtr;

struct MDVROutCtlRec {
    short                           timeCodeClock;              /*time base driven by time code*/
    short                           timeCodeFormat;             /*format of time code to listen to*/
    MDVRTimeCodeUPP                 timeCodeProc;               /*proc called on time code fmt change*/
    MDVRCommUPP                     commProc;                   /*proc called for handshaking*/
    long                            refCon;                     /*refCon passed to timeCodeProc*/
    Boolean                         timeCodeFilter;             /*filter time code if true*/
    UInt8                           padding;                    /*unused pad byte*/
    long                            midiMsgTicks;               /*value of Ticks when MIDI msg rcvd*/
    long                            timeCodeTicks;              /*value of Ticks when time code rcvd*/
};
typedef struct MDVROutCtlRec            MDVROutCtlRec;

typedef MDVROutCtlRec *                 MDVROutCtlPtr;
typedef void *                          MDVRPtr;
#if CALL_NOT_IN_CARBON
EXTERN_API( NumVersion )
MIDIVersion                     (void)                                                      FOURWORDINLINE(0x203C, 0x0000, 0x0004, 0xA800);

EXTERN_API( OSErr )
MIDISignIn                      (OSType                 clientID,
                                 long                   refCon,
                                 Handle                 icon,
                                 ConstStr255Param       name)                               FOURWORDINLINE(0x203C, 0x0004, 0x0004, 0xA800);

EXTERN_API( void )
MIDISignOut                     (OSType                 clientID)                           FOURWORDINLINE(0x203C, 0x0008, 0x0004, 0xA800);

EXTERN_API( MIDIIDListHandle )
MIDIGetClients                  (void)                                                      FOURWORDINLINE(0x203C, 0x000C, 0x0004, 0xA800);

EXTERN_API( void )
MIDIGetClientName               (OSType                 clientID,
                                 Str255                 name)                               FOURWORDINLINE(0x203C, 0x0010, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetClientName               (OSType                 clientID,
                                 ConstStr255Param       name)                               FOURWORDINLINE(0x203C, 0x0014, 0x0004, 0xA800);

EXTERN_API( MIDIIDListHandle )
MIDIGetPorts                    (OSType                 clientID)                           FOURWORDINLINE(0x203C, 0x0018, 0x0004, 0xA800);

EXTERN_API( OSErr )
MIDIAddPort                     (OSType                 clientID,
                                 short                  BufSize,
                                 short *                refnum,
                                 MIDIPortParamsPtr      init)                               FOURWORDINLINE(0x203C, 0x001C, 0x0004, 0xA800);

EXTERN_API( MIDIPortInfoHandle )
MIDIGetPortInfo                 (OSType                 clientID,
                                 OSType                 portID)                             FOURWORDINLINE(0x203C, 0x0020, 0x0004, 0xA800);

EXTERN_API( OSErr )
MIDIConnectData                 (OSType                 srcClID,
                                 OSType                 srcPortID,
                                 OSType                 dstClID,
                                 OSType                 dstPortID)                          FOURWORDINLINE(0x203C, 0x0024, 0x0004, 0xA800);

EXTERN_API( OSErr )
MIDIUnConnectData               (OSType                 srcClID,
                                 OSType                 srcPortID,
                                 OSType                 dstClID,
                                 OSType                 dstPortID)                          FOURWORDINLINE(0x203C, 0x0028, 0x0004, 0xA800);

EXTERN_API( OSErr )
MIDIConnectTime                 (OSType                 srcClID,
                                 OSType                 srcPortID,
                                 OSType                 dstClID,
                                 OSType                 dstPortID)                          FOURWORDINLINE(0x203C, 0x002C, 0x0004, 0xA800);

EXTERN_API( OSErr )
MIDIUnConnectTime               (OSType                 srcClID,
                                 OSType                 srcPortID,
                                 OSType                 dstClID,
                                 OSType                 dstPortID)                          FOURWORDINLINE(0x203C, 0x0030, 0x0004, 0xA800);

EXTERN_API( void )
MIDIFlush                       (short                  refnum)                             FOURWORDINLINE(0x203C, 0x0034, 0x0004, 0xA800);

EXTERN_API( ProcPtr )
MIDIGetReadHook                 (short                  refnum)                             FOURWORDINLINE(0x203C, 0x0038, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetReadHook                 (short                  refnum,
                                 ProcPtr                hook)                               FOURWORDINLINE(0x203C, 0x003C, 0x0004, 0xA800);

EXTERN_API( void )
MIDIGetPortName                 (OSType                 clientID,
                                 OSType                 portID,
                                 Str255                 name)                               FOURWORDINLINE(0x203C, 0x0040, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetPortName                 (OSType                 clientID,
                                 OSType                 portID,
                                 ConstStr255Param       name)                               FOURWORDINLINE(0x203C, 0x0044, 0x0004, 0xA800);

EXTERN_API( void )
MIDIWakeUp                      (short                  refnum,
                                 long                   time,
                                 long                   period,
                                 MIDITimeUPP            timeProc)                           FOURWORDINLINE(0x203C, 0x0048, 0x0004, 0xA800);

EXTERN_API( void )
MIDIRemovePort                  (short                  refnum)                             FOURWORDINLINE(0x203C, 0x004C, 0x0004, 0xA800);

EXTERN_API( short )
MIDIGetSync                     (short                  refnum)                             FOURWORDINLINE(0x203C, 0x0050, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetSync                     (short                  refnum,
                                 short                  sync)                               FOURWORDINLINE(0x203C, 0x0054, 0x0004, 0xA800);

EXTERN_API( long )
MIDIGetCurTime                  (short                  refnum)                             FOURWORDINLINE(0x203C, 0x0058, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetCurTime                  (short                  refnum,
                                 long                   time)                               FOURWORDINLINE(0x203C, 0x005C, 0x0004, 0xA800);

EXTERN_API( void )
MIDIStartTime                   (short                  refnum)                             FOURWORDINLINE(0x203C, 0x0060, 0x0004, 0xA800);

EXTERN_API( void )
MIDIStopTime                    (short                  refnum)                             FOURWORDINLINE(0x203C, 0x0064, 0x0004, 0xA800);

EXTERN_API( void )
MIDIPoll                        (short                  refnum,
                                 long                   offsetTime)                         FOURWORDINLINE(0x203C, 0x0068, 0x0004, 0xA800);

EXTERN_API( OSErr )
MIDIWritePacket                 (short                  refnum,
                                 MIDIPacketPtr          packet)                             FOURWORDINLINE(0x203C, 0x006C, 0x0004, 0xA800);

EXTERN_API( Boolean )
MIDIWorldChanged                (OSType                 clientID)                           FOURWORDINLINE(0x203C, 0x0070, 0x0004, 0xA800);

EXTERN_API( long )
MIDIGetOffsetTime               (short                  refnum)                             FOURWORDINLINE(0x203C, 0x0074, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetOffsetTime               (short                  refnum,
                                 long                   offsetTime)                         FOURWORDINLINE(0x203C, 0x0078, 0x0004, 0xA800);

EXTERN_API( long )
MIDIConvertTime                 (short                  srcFormat,
                                 short                  dstFormat,
                                 long                   time)                               FOURWORDINLINE(0x203C, 0x007C, 0x0004, 0xA800);

EXTERN_API( long )
MIDIGetRefCon                   (short                  refnum)                             FOURWORDINLINE(0x203C, 0x0080, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetRefCon                   (short                  refnum,
                                 long                   refCon)                             FOURWORDINLINE(0x203C, 0x0084, 0x0004, 0xA800);

EXTERN_API( long )
MIDIGetClRefCon                 (OSType                 clientID)                           FOURWORDINLINE(0x203C, 0x0088, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetClRefCon                 (OSType                 clientID,
                                 long                   refCon)                             FOURWORDINLINE(0x203C, 0x008C, 0x0004, 0xA800);

EXTERN_API( short )
MIDIGetTCFormat                 (short                  refnum)                             FOURWORDINLINE(0x203C, 0x0090, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetTCFormat                 (short                  refnum,
                                 short                  format)                             FOURWORDINLINE(0x203C, 0x0094, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetRunRate                  (short                  refnum,
                                 short                  rate,
                                 long                   time)                               FOURWORDINLINE(0x203C, 0x0098, 0x0004, 0xA800);

EXTERN_API( Handle )
MIDIGetClientIcon               (OSType                 clientID)                           FOURWORDINLINE(0x203C, 0x009C, 0x0004, 0xA800);

EXTERN_API( ProcPtr )
MIDICallAddress                 (short                  callNum)                            FOURWORDINLINE(0x203C, 0x00A4, 0x0004, 0xA800);

EXTERN_API( void )
MIDISetConnectionProc           (short                  refNum,
                                 ProcPtr                connectionProc,
                                 long                   refCon)                             FOURWORDINLINE(0x203C, 0x00A8, 0x0004, 0xA800);

EXTERN_API( void )
MIDIGetConnectionProc           (short                  refnum,
                                 ProcPtr *              connectionProc,
                                 long *                 refCon)                             FOURWORDINLINE(0x203C, 0x00AC, 0x0004, 0xA800);

EXTERN_API( void )
MIDIDiscardPacket               (short                  refnum,
                                 MIDIPacketPtr          packet)                             FOURWORDINLINE(0x203C, 0x00B0, 0x0004, 0xA800);

EXTERN_API( OSErr )
MDVRSignIn                      (OSType                 clientID,
                                 long                   refCon,
                                 Handle                 icon,
                                 Str255                 name)                               FOURWORDINLINE(0x203C, 0x00B4, 0x0004, 0xA800);

EXTERN_API( void )
MDVRSignOut                     (OSType                 clientID)                           FOURWORDINLINE(0x203C, 0x00B8, 0x0004, 0xA800);

EXTERN_API( MDVRPtr )
MDVROpen                        (short                  portType,
                                 short                  refnum)                             FOURWORDINLINE(0x203C, 0x00BC, 0x0004, 0xA800);

EXTERN_API( void )
MDVRClose                       (MDVRPtr                driverPtr)                          FOURWORDINLINE(0x203C, 0x00C0, 0x0004, 0xA800);

EXTERN_API( void )
MDVRControlIn                   (MDVRPtr                portPtr,
                                 MDVRInCtlPtr           inputCtl)                           FOURWORDINLINE(0x203C, 0x00C4, 0x0004, 0xA800);

EXTERN_API( void )
MDVRControlOut                  (MDVRPtr                portPtr,
                                 MDVROutCtlPtr          outputCtl)                          FOURWORDINLINE(0x203C, 0x00C8, 0x0004, 0xA800);

EXTERN_API( void )
MDVRIn                          (MDVRPtr                portPtr)                            FOURWORDINLINE(0x203C, 0x00CC, 0x0004, 0xA800);

EXTERN_API( void )
MDVROut                         (MDVRPtr                portPtr,
                                 char *                 dataPtr,
                                 short                  length)                             FOURWORDINLINE(0x203C, 0x00D0, 0x0004, 0xA800);

EXTERN_API( void )
MDVRNotesOff                    (MDVRPtr                portPtr,
                                 short                  mode)                               FOURWORDINLINE(0x203C, 0x00D4, 0x0004, 0xA800);

#endif  /* CALL_NOT_IN_CARBON */

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(MIDIReadHookUPP)
    NewMIDIReadHookUPP             (MIDIReadHookProcPtr     userRoutine);

    EXTERN_API(MIDITimeUPP)
    NewMIDITimeUPP                 (MIDITimeProcPtr         userRoutine);

    EXTERN_API(MIDIConnectionUPP)
    NewMIDIConnectionUPP           (MIDIConnectionProcPtr   userRoutine);

    EXTERN_API(MDVRCommUPP)
    NewMDVRCommUPP                 (MDVRCommProcPtr         userRoutine);

    EXTERN_API(MDVRTimeCodeUPP)
    NewMDVRTimeCodeUPP             (MDVRTimeCodeProcPtr     userRoutine);

    EXTERN_API(MDVRReadUPP)
    NewMDVRReadUPP                 (MDVRReadProcPtr         userRoutine);

    EXTERN_API(void)
    DisposeMIDIReadHookUPP         (MIDIReadHookUPP         userUPP);

    EXTERN_API(void)
    DisposeMIDITimeUPP             (MIDITimeUPP             userUPP);

    EXTERN_API(void)
    DisposeMIDIConnectionUPP       (MIDIConnectionUPP       userUPP);

    EXTERN_API(void)
    DisposeMDVRCommUPP             (MDVRCommUPP             userUPP);

    EXTERN_API(void)
    DisposeMDVRTimeCodeUPP         (MDVRTimeCodeUPP         userUPP);

    EXTERN_API(void)
    DisposeMDVRReadUPP             (MDVRReadUPP             userUPP);

    EXTERN_API(short)
    InvokeMIDIReadHookUPP          (MIDIPacketPtr           myPacket,
                                    long                    myRefCon,
                                    MIDIReadHookUPP         userUPP);

    EXTERN_API(void)
    InvokeMIDITimeUPP              (long                    curTime,
                                    long                    myRefCon,
                                    MIDITimeUPP             userUPP);

    EXTERN_API(void)
    InvokeMIDIConnectionUPP        (short                   refnum,
                                    long                    refcon,
                                    short                   portType,
                                    OSType                  clientID,
                                    OSType                  portID,
                                    Boolean                 connect,
                                    short                   direction,
                                    MIDIConnectionUPP       userUPP);

    EXTERN_API(long)
    InvokeMDVRCommUPP              (short                   refnum,
                                    short                   request,
                                    long                    refCon,
                                    MDVRCommUPP             userUPP);

    EXTERN_API(void)
    InvokeMDVRTimeCodeUPP          (short                   refnum,
                                    short                   newFormat,
                                    long                    refCon,
                                    MDVRTimeCodeUPP         userUPP);

    EXTERN_API(void)
    InvokeMDVRReadUPP              (char *                  midiChars,
                                    short                   length,
                                    long                    refCon,
                                    MDVRReadUPP             userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppMIDIReadHookProcInfo = 0x000003E0 };                  /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    enum { uppMIDITimeProcInfo = 0x000003C0 };                      /* pascal no_return_value Func(4_bytes, 4_bytes) */
    enum { uppMIDIConnectionProcInfo = 0x0009FB80 };                /* pascal no_return_value Func(2_bytes, 4_bytes, 2_bytes, 4_bytes, 4_bytes, 1_byte, 2_bytes) */
    enum { uppMDVRCommProcInfo = 0x00000EB0 };                      /* pascal 4_bytes Func(2_bytes, 2_bytes, 4_bytes) */
    enum { uppMDVRTimeCodeProcInfo = 0x00000E80 };                  /* pascal no_return_value Func(2_bytes, 2_bytes, 4_bytes) */
    enum { uppMDVRReadProcInfo = 0x00000EC0 };                      /* pascal no_return_value Func(4_bytes, 2_bytes, 4_bytes) */
    #define NewMIDIReadHookUPP(userRoutine)                         (MIDIReadHookUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMIDIReadHookProcInfo, GetCurrentArchitecture())
    #define NewMIDITimeUPP(userRoutine)                             (MIDITimeUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMIDITimeProcInfo, GetCurrentArchitecture())
    #define NewMIDIConnectionUPP(userRoutine)                       (MIDIConnectionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMIDIConnectionProcInfo, GetCurrentArchitecture())
    #define NewMDVRCommUPP(userRoutine)                             (MDVRCommUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMDVRCommProcInfo, GetCurrentArchitecture())
    #define NewMDVRTimeCodeUPP(userRoutine)                         (MDVRTimeCodeUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMDVRTimeCodeProcInfo, GetCurrentArchitecture())
    #define NewMDVRReadUPP(userRoutine)                             (MDVRReadUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMDVRReadProcInfo, GetCurrentArchitecture())
    #define DisposeMIDIReadHookUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeMIDITimeUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define DisposeMIDIConnectionUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposeMDVRCommUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define DisposeMDVRTimeCodeUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeMDVRReadUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define InvokeMIDIReadHookUPP(myPacket, myRefCon, userUPP)      (short)CALL_TWO_PARAMETER_UPP((userUPP), uppMIDIReadHookProcInfo, (myPacket), (myRefCon))
    #define InvokeMIDITimeUPP(curTime, myRefCon, userUPP)           CALL_TWO_PARAMETER_UPP((userUPP), uppMIDITimeProcInfo, (curTime), (myRefCon))
    #define InvokeMIDIConnectionUPP(refnum, refcon, portType, clientID, portID, connect, direction, userUPP)  CALL_SEVEN_PARAMETER_UPP((userUPP), uppMIDIConnectionProcInfo, (refnum), (refcon), (portType), (clientID), (portID), (connect), (direction))
    #define InvokeMDVRCommUPP(refnum, request, refCon, userUPP)     (long)CALL_THREE_PARAMETER_UPP((userUPP), uppMDVRCommProcInfo, (refnum), (request), (refCon))
    #define InvokeMDVRTimeCodeUPP(refnum, newFormat, refCon, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppMDVRTimeCodeProcInfo, (refnum), (newFormat), (refCon))
    #define InvokeMDVRReadUPP(midiChars, length, refCon, userUPP)   CALL_THREE_PARAMETER_UPP((userUPP), uppMDVRReadProcInfo, (midiChars), (length), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewMIDIReadHookProc(userRoutine)                        NewMIDIReadHookUPP(userRoutine)
#define NewMIDITimeProc(userRoutine)                            NewMIDITimeUPP(userRoutine)
#define NewMIDIConnectionProc(userRoutine)                      NewMIDIConnectionUPP(userRoutine)
#define NewMDVRCommProc(userRoutine)                            NewMDVRCommUPP(userRoutine)
#define NewMDVRTimeCodeProc(userRoutine)                        NewMDVRTimeCodeUPP(userRoutine)
#define NewMDVRReadProc(userRoutine)                            NewMDVRReadUPP(userRoutine)
#define CallMIDIReadHookProc(userRoutine, myPacket, myRefCon)   InvokeMIDIReadHookUPP(myPacket, myRefCon, userRoutine)
#define CallMIDITimeProc(userRoutine, curTime, myRefCon)        InvokeMIDITimeUPP(curTime, myRefCon, userRoutine)
#define CallMIDIConnectionProc(userRoutine, refnum, refcon, portType, clientID, portID, connect, direction) InvokeMIDIConnectionUPP(refnum, refcon, portType, clientID, portID, connect, direction, userRoutine)
#define CallMDVRCommProc(userRoutine, refnum, request, refCon)  InvokeMDVRCommUPP(refnum, request, refCon, userRoutine)
#define CallMDVRTimeCodeProc(userRoutine, refnum, newFormat, refCon) InvokeMDVRTimeCodeUPP(refnum, newFormat, refCon, userRoutine)
#define CallMDVRReadProc(userRoutine, midiChars, length, refCon) InvokeMDVRReadUPP(midiChars, length, refCon, userRoutine)


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

#endif /* __MIDI__ */

