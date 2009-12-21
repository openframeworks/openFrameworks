/*
     File:       Processes.h
 
     Contains:   Process Manager Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1989-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PROCESSES__
#define __PROCESSES__

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

/* type for unique process identifier */

struct ProcessSerialNumber {
    unsigned long                   highLongOfPSN;
    unsigned long                   lowLongOfPSN;
};
typedef struct ProcessSerialNumber      ProcessSerialNumber;
typedef ProcessSerialNumber *           ProcessSerialNumberPtr;
enum {
                                                                /* Process identifier - Various reserved process serial numbers */
    kNoProcess                  = 0,
    kSystemProcess              = 1,
    kCurrentProcess             = 2
};

/* Definition of the parameter block passed to _Launch */
/* Typedef and flags for launchControlFlags field*/

typedef unsigned short                  LaunchFlags;
enum {
    launchContinue              = 0x4000,
    launchNoFileFlags           = 0x0800,
    launchUseMinimum            = 0x0400,
    launchDontSwitch            = 0x0200,
    launchAllow24Bit            = 0x0100,
    launchInhibitDaemon         = 0x0080
};

/* Format for first AppleEvent to pass to new process.  The size of the overall
  buffer variable: the message body immediately follows the messageLength */

struct AppParameters {
    EventRecord                     theMsgEvent;
    unsigned long                   eventRefCon;
    unsigned long                   messageLength;
};
typedef struct AppParameters            AppParameters;
typedef AppParameters *                 AppParametersPtr;
/* Parameter block to _Launch */

struct LaunchParamBlockRec {
    unsigned long                   reserved1;
    unsigned short                  reserved2;
    unsigned short                  launchBlockID;
    unsigned long                   launchEPBLength;
    unsigned short                  launchFileFlags;
    LaunchFlags                     launchControlFlags;
    FSSpecPtr                       launchAppSpec;
    ProcessSerialNumber             launchProcessSN;
    unsigned long                   launchPreferredSize;
    unsigned long                   launchMinimumSize;
    unsigned long                   launchAvailableSize;
    AppParametersPtr                launchAppParameters;
};
typedef struct LaunchParamBlockRec      LaunchParamBlockRec;

typedef LaunchParamBlockRec *           LaunchPBPtr;
/* Set launchBlockID to extendedBlock to specify that extensions exist.
 Set launchEPBLength to extendedBlockLen for compatibility.*/
enum {
    extendedBlock               = 0x4C43,                       /* 'LC' */
    extendedBlockLen            = sizeof(LaunchParamBlockRec) - 12
};

enum {
                                                                /* Definition of the information block returned by GetProcessInformation */
    modeReserved                = 0x01000000,
    modeControlPanel            = 0x00080000,
    modeLaunchDontSwitch        = 0x00040000,
    modeDeskAccessory           = 0x00020000,
    modeMultiLaunch             = 0x00010000,
    modeNeedSuspendResume       = 0x00004000,
    modeCanBackground           = 0x00001000,
    modeDoesActivateOnFGSwitch  = 0x00000800,
    modeOnlyBackground          = 0x00000400,
    modeGetFrontClicks          = 0x00000200,
    modeGetAppDiedMsg           = 0x00000100,
    mode32BitCompatible         = 0x00000080,
    modeHighLevelEventAware     = 0x00000040,
    modeLocalAndRemoteHLEvents  = 0x00000020,
    modeStationeryAware         = 0x00000010,
    modeUseTextEditServices     = 0x00000008,
    modeDisplayManagerAware     = 0x00000004
};

/* Record returned by GetProcessInformation */

struct ProcessInfoRec {
    unsigned long                   processInfoLength;
    StringPtr                       processName;
    ProcessSerialNumber             processNumber;
    unsigned long                   processType;
    OSType                          processSignature;
    unsigned long                   processMode;
    Ptr                             processLocation;
    unsigned long                   processSize;
    unsigned long                   processFreeMem;
    ProcessSerialNumber             processLauncher;
    unsigned long                   processLaunchDate;
    unsigned long                   processActiveTime;
    FSSpecPtr                       processAppSpec;
};
typedef struct ProcessInfoRec           ProcessInfoRec;
typedef ProcessInfoRec *                ProcessInfoRecPtr;
/* Record corresponding to the SIZE resource definition */

struct SizeResourceRec {
    unsigned short                  flags;
    unsigned long                   preferredHeapSize;
    unsigned long                   minimumHeapSize;
};
typedef struct SizeResourceRec          SizeResourceRec;
typedef SizeResourceRec *               SizeResourceRecPtr;
typedef SizeResourceRecPtr *            SizeResourceRecHandle;
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 LaunchApplication(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
LaunchApplication               (LaunchPBPtr            LaunchParams)                       ONEWORDINLINE(0xA9F2);

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
LaunchDeskAccessory             (const FSSpec *         pFileSpec,
                                 ConstStr255Param       pDAName)                            THREEWORDINLINE(0x3F3C, 0x0036, 0xA88F);

#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_OS_MAC
    #define MacGetCurrentProcess GetCurrentProcess
#endif
EXTERN_API( OSErr )
MacGetCurrentProcess            (ProcessSerialNumber *  PSN)                                THREEWORDINLINE(0x3F3C, 0x0037, 0xA88F);

EXTERN_API( OSErr )
GetFrontProcess                 (ProcessSerialNumber *  PSN)                                FIVEWORDINLINE(0x70FF, 0x2F00, 0x3F3C, 0x0039, 0xA88F);

EXTERN_API( OSErr )
GetNextProcess                  (ProcessSerialNumber *  PSN)                                THREEWORDINLINE(0x3F3C, 0x0038, 0xA88F);

EXTERN_API( OSErr )
GetProcessInformation           (const ProcessSerialNumber * PSN,
                                 ProcessInfoRec *       info)                               THREEWORDINLINE(0x3F3C, 0x003A, 0xA88F);

EXTERN_API( OSErr )
SetFrontProcess                 (const ProcessSerialNumber * PSN)                           THREEWORDINLINE(0x3F3C, 0x003B, 0xA88F);

EXTERN_API( OSErr )
WakeUpProcess                   (const ProcessSerialNumber * PSN)                           THREEWORDINLINE(0x3F3C, 0x003C, 0xA88F);

EXTERN_API( OSErr )
SameProcess                     (const ProcessSerialNumber * PSN1,
                                 const ProcessSerialNumber * PSN2,
                                 Boolean *              result)                             THREEWORDINLINE(0x3F3C, 0x003D, 0xA88F);

/*  ExitToShell was previously in SegLoad.h*/
EXTERN_API( void )
ExitToShell                     (void)                                                      ONEWORDINLINE(0xA9F4);

/*
   LaunchControlPanel is similar to LaunchDeskAccessory, but for Control Panel files instead.
   It launches a control panel in an application shell maintained by the Process Manager.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
LaunchControlPanel              (const FSSpec *         pFileSpec)                          THREEWORDINLINE(0x3F3C, 0x007B, 0xA88F);

#endif  /* CALL_NOT_IN_CARBON */

/* Values of the 'message' parameter to a Control Panel 'cdev' */
enum {
    initDev                     = 0,                            /*Time for cdev to initialize itself*/
    hitDev                      = 1,                            /*Hit on one of my items*/
    closeDev                    = 2,                            /*Close yourself*/
    nulDev                      = 3,                            /*Null event*/
    updateDev                   = 4,                            /*Update event*/
    activDev                    = 5,                            /*Activate event*/
    deactivDev                  = 6,                            /*Deactivate event*/
    keyEvtDev                   = 7,                            /*Key down/auto key*/
    macDev                      = 8,                            /*Decide whether or not to show up*/
    undoDev                     = 9,
    cutDev                      = 10,
    copyDev                     = 11,
    pasteDev                    = 12,
    clearDev                    = 13,
    cursorDev                   = 14
};

/* Special values a Control Panel 'cdev' can return */
enum {
    cdevGenErr                  = -1,                           /*General error; gray cdev w/o alert*/
    cdevMemErr                  = 0,                            /*Memory shortfall; alert user please*/
    cdevResErr                  = 1,                            /*Couldn't get a needed resource; alert*/
    cdevUnset                   = 3                             /* cdevValue is initialized to this*/
};

/* Control Panel Default Proc */
typedef CALLBACK_API( long , ControlPanelDefProcPtr )(short message, short item, short numItems, short cPanelID, EventRecord *theEvent, long cdevValue, DialogPtr cpDialog);
typedef STACK_UPP_TYPE(ControlPanelDefProcPtr)                  ControlPanelDefUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ControlPanelDefUPP)
    NewControlPanelDefUPP          (ControlPanelDefProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeControlPanelDefUPP      (ControlPanelDefUPP      userUPP);

    EXTERN_API(long)
    InvokeControlPanelDefUPP       (short                   message,
                                    short                   item,
                                    short                   numItems,
                                    short                   cPanelID,
                                    EventRecord *           theEvent,
                                    long                    cdevValue,
                                    DialogPtr               cpDialog,
                                    ControlPanelDefUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppControlPanelDefProcInfo = 0x000FEAB0 };               /* pascal 4_bytes Func(2_bytes, 2_bytes, 2_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewControlPanelDefUPP(userRoutine)                      (ControlPanelDefUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlPanelDefProcInfo, GetCurrentArchitecture())
    #define DisposeControlPanelDefUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeControlPanelDefUPP(message, item, numItems, cPanelID, theEvent, cdevValue, cpDialog, userUPP)  (long)CALL_SEVEN_PARAMETER_UPP((userUPP), uppControlPanelDefProcInfo, (message), (item), (numItems), (cPanelID), (theEvent), (cdevValue), (cpDialog))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewControlPanelDefProc(userRoutine)                     NewControlPanelDefUPP(userRoutine)
#define CallControlPanelDefProc(userRoutine, message, item, numItems, cPanelID, theEvent, cdevValue, cpDialog) InvokeControlPanelDefUPP(message, item, numItems, cPanelID, theEvent, cdevValue, cpDialog, userRoutine)


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

#endif /* __PROCESSES__ */

