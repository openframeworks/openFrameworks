/*
     File:       AppleEvents.h
 
     Contains:   AppleEvent Package Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1989-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __APPLEEVENTS__
#define __APPLEEVENTS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __NOTIFICATION__
#include <Notification.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

/*
    Note:   The functions and types for the building and parsing AppleEvent  
            messages has moved to AEDataModel.h
*/
#ifndef __AEDATAMODEL__
#include <AEDataModel.h>
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
                                                                /* Keywords for Apple event parameters */
    keyDirectObject             = FOUR_CHAR_CODE('----'),
    keyErrorNumber              = FOUR_CHAR_CODE('errn'),
    keyErrorString              = FOUR_CHAR_CODE('errs'),
    keyProcessSerialNumber      = FOUR_CHAR_CODE('psn '),       /* Keywords for special handlers */
    keyPreDispatch              = FOUR_CHAR_CODE('phac'),       /* preHandler accessor call */
    keySelectProc               = FOUR_CHAR_CODE('selh'),       /* more selector call */
                                                                /* Keyword for recording */
    keyAERecorderCount          = FOUR_CHAR_CODE('recr'),       /* available only in vers 1.0.1 and greater */
                                                                /* Keyword for version information */
    keyAEVersion                = FOUR_CHAR_CODE('vers')        /* available only in vers 1.0.1 and greater */
};

/* Event Class */
enum {
    kCoreEventClass             = FOUR_CHAR_CODE('aevt')
};

/* Event ID's */
enum {
    kAEOpenApplication          = FOUR_CHAR_CODE('oapp'),
    kAEOpenDocuments            = FOUR_CHAR_CODE('odoc'),
    kAEPrintDocuments           = FOUR_CHAR_CODE('pdoc'),
    kAEQuitApplication          = FOUR_CHAR_CODE('quit'),
    kAEAnswer                   = FOUR_CHAR_CODE('ansr'),
    kAEApplicationDied          = FOUR_CHAR_CODE('obit')
};

/* Constants for recording */
enum {
    kAEStartRecording           = FOUR_CHAR_CODE('reca'),       /* available only in vers 1.0.1 and greater */
    kAEStopRecording            = FOUR_CHAR_CODE('recc'),       /* available only in vers 1.0.1 and greater */
    kAENotifyStartRecording     = FOUR_CHAR_CODE('rec1'),       /* available only in vers 1.0.1 and greater */
    kAENotifyStopRecording      = FOUR_CHAR_CODE('rec0'),       /* available only in vers 1.0.1 and greater */
    kAENotifyRecording          = FOUR_CHAR_CODE('recr')        /* available only in vers 1.0.1 and greater */
};


/* parameter to AESend */
typedef OptionBits                      AESendOptions;
enum {
    kAENeverInteract            = 0x00000010,                   /* server should not interact with user */
    kAECanInteract              = 0x00000020,                   /* server may try to interact with user */
    kAEAlwaysInteract           = 0x00000030,                   /* server should always interact with user where appropriate */
    kAECanSwitchLayer           = 0x00000040,                   /* interaction may switch layer */
    kAEDontRecord               = 0x00001000,                   /* don't record this event - available only in vers 1.0.1 and greater */
    kAEDontExecute              = 0x00002000,                   /* don't send the event for recording - available only in vers 1.0.1 and greater */
    kAEProcessNonReplyEvents    = 0x00008000                    /* allow processing of non-reply events while awaiting synchronous AppleEvent reply */
};

typedef SInt32                          AESendMode;
enum {
    kAENoReply                  = 0x00000001,                   /* sender doesn't want a reply to event */
    kAEQueueReply               = 0x00000002,                   /* sender wants a reply but won't wait */
    kAEWaitReply                = 0x00000003,                   /* sender wants a reply and will wait */
    kAEDontReconnect            = 0x00000080,                   /* don't reconnect if there is a sessClosedErr from PPCToolbox */
    kAEWantReceipt              = 0x00000200                    /* (nReturnReceipt) sender wants a receipt of message */
};


/* Constants for timeout durations */
enum {
    kAEDefaultTimeout           = -1,                           /* timeout value determined by AEM */
    kNoTimeOut                  = -2                            /* wait until reply comes back, however long it takes */
};


/* priority param of AESend */
typedef SInt16                          AESendPriority;
enum {
    kAENormalPriority           = 0x00000000,                   /* post message at the end of the event queue */
    kAEHighPriority             = 0x00000001                    /* post message at the front of the event queue (same as nAttnMsg) */
};


typedef SInt8                           AEEventSource;
enum {
    kAEUnknownSource            = 0,
    kAEDirectCall               = 1,
    kAESameProcess              = 2,
    kAELocalProcess             = 3,
    kAERemoteProcess            = 4
};



typedef CALLBACK_API( OSErr , AEEventHandlerProcPtr )(const AppleEvent *theAppleEvent, AppleEvent *reply, UInt32 handlerRefcon);
typedef CALLBACK_API( Boolean , AEIdleProcPtr )(EventRecord *theEvent, long *sleepTime, RgnHandle *mouseRgn);
typedef CALLBACK_API( Boolean , AEFilterProcPtr )(EventRecord *theEvent, long returnID, long transactionID, const AEAddressDesc *sender);
typedef STACK_UPP_TYPE(AEEventHandlerProcPtr)                   AEEventHandlerUPP;
typedef STACK_UPP_TYPE(AEIdleProcPtr)                           AEIdleUPP;
typedef STACK_UPP_TYPE(AEFilterProcPtr)                         AEFilterUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(AEEventHandlerUPP)
    NewAEEventHandlerUPP           (AEEventHandlerProcPtr   userRoutine);

    EXTERN_API(AEIdleUPP)
    NewAEIdleUPP                   (AEIdleProcPtr           userRoutine);

    EXTERN_API(AEFilterUPP)
    NewAEFilterUPP                 (AEFilterProcPtr         userRoutine);

    EXTERN_API(void)
    DisposeAEEventHandlerUPP       (AEEventHandlerUPP       userUPP);

    EXTERN_API(void)
    DisposeAEIdleUPP               (AEIdleUPP               userUPP);

    EXTERN_API(void)
    DisposeAEFilterUPP             (AEFilterUPP             userUPP);

    EXTERN_API(OSErr)
    InvokeAEEventHandlerUPP        (const AppleEvent *      theAppleEvent,
                                    AppleEvent *            reply,
                                    UInt32                  handlerRefcon,
                                    AEEventHandlerUPP       userUPP);

    EXTERN_API(Boolean)
    InvokeAEIdleUPP                (EventRecord *           theEvent,
                                    long *                  sleepTime,
                                    RgnHandle *             mouseRgn,
                                    AEIdleUPP               userUPP);

    EXTERN_API(Boolean)
    InvokeAEFilterUPP              (EventRecord *           theEvent,
                                    long                    returnID,
                                    long                    transactionID,
                                    const AEAddressDesc *   sender,
                                    AEFilterUPP             userUPP);

#else
    enum { uppAEEventHandlerProcInfo = 0x00000FE0 };                /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppAEIdleProcInfo = 0x00000FD0 };                        /* pascal 1_byte Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppAEFilterProcInfo = 0x00003FD0 };                      /* pascal 1_byte Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewAEEventHandlerUPP(userRoutine)                       (AEEventHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAEEventHandlerProcInfo, GetCurrentArchitecture())
    #define NewAEIdleUPP(userRoutine)                               (AEIdleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAEIdleProcInfo, GetCurrentArchitecture())
    #define NewAEFilterUPP(userRoutine)                             (AEFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAEFilterProcInfo, GetCurrentArchitecture())
    #define DisposeAEEventHandlerUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposeAEIdleUPP(userUPP)                               DisposeRoutineDescriptor(userUPP)
    #define DisposeAEFilterUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define InvokeAEEventHandlerUPP(theAppleEvent, reply, handlerRefcon, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppAEEventHandlerProcInfo, (theAppleEvent), (reply), (handlerRefcon))
    #define InvokeAEIdleUPP(theEvent, sleepTime, mouseRgn, userUPP)  (Boolean)CALL_THREE_PARAMETER_UPP((userUPP), uppAEIdleProcInfo, (theEvent), (sleepTime), (mouseRgn))
    #define InvokeAEFilterUPP(theEvent, returnID, transactionID, sender, userUPP)  (Boolean)CALL_FOUR_PARAMETER_UPP((userUPP), uppAEFilterProcInfo, (theEvent), (returnID), (transactionID), (sender))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewAEEventHandlerProc(userRoutine)                      NewAEEventHandlerUPP(userRoutine)
#define NewAEIdleProc(userRoutine)                              NewAEIdleUPP(userRoutine)
#define NewAEFilterProc(userRoutine)                            NewAEFilterUPP(userRoutine)
#define CallAEEventHandlerProc(userRoutine, theAppleEvent, reply, handlerRefcon) InvokeAEEventHandlerUPP(theAppleEvent, reply, handlerRefcon, userRoutine)
#define CallAEIdleProc(userRoutine, theEvent, sleepTime, mouseRgn) InvokeAEIdleUPP(theEvent, sleepTime, mouseRgn, userRoutine)
#define CallAEFilterProc(userRoutine, theEvent, returnID, transactionID, sender) InvokeAEFilterUPP(theEvent, returnID, transactionID, sender, userRoutine)

/**************************************************************************
  The next couple of calls are basic routines used to create, send,
  and process AppleEvents. 
**************************************************************************/
EXTERN_API( OSErr )
AESend                          (const AppleEvent *     theAppleEvent,
                                 AppleEvent *           reply,
                                 AESendMode             sendMode,
                                 AESendPriority         sendPriority,
                                 long                   timeOutInTicks,
                                 AEIdleUPP              idleProc, /* can be NULL */
                                 AEFilterUPP            filterProc) /* can be NULL */       THREEWORDINLINE(0x303C, 0x0D17, 0xA816);

EXTERN_API( OSErr )
AEProcessAppleEvent             (const EventRecord *    theEventRecord)                     THREEWORDINLINE(0x303C, 0x021B, 0xA816);


/* 
 Note: during event processing, an event handler may realize that it is likely
 to exceed the client's timeout limit. Passing the reply to this
 routine causes a wait event to be generated that asks the client
 for more time. 
*/
EXTERN_API( OSErr )
AEResetTimer                    (const AppleEvent *     reply)                              THREEWORDINLINE(0x303C, 0x0219, 0xA816);


/**************************************************************************
  The following three calls are used to allow applications to behave
  courteously when a user interaction such as a dialog box is needed. 
**************************************************************************/

typedef SInt8                           AEInteractAllowed;
enum {
    kAEInteractWithSelf         = 0,
    kAEInteractWithLocal        = 1,
    kAEInteractWithAll          = 2
};

EXTERN_API( OSErr )
AEGetInteractionAllowed         (AEInteractAllowed *    level)                              THREEWORDINLINE(0x303C, 0x021D, 0xA816);

EXTERN_API( OSErr )
AESetInteractionAllowed         (AEInteractAllowed      level)                              THREEWORDINLINE(0x303C, 0x011E, 0xA816);

EXTERN_API( OSErr )
AEInteractWithUser              (long                   timeOutInTicks,
                                 NMRecPtr               nmReqPtr,
                                 AEIdleUPP              idleProc)                           THREEWORDINLINE(0x303C, 0x061C, 0xA816);


/**************************************************************************
  These calls are used to set up and modify the event dispatch table.
**************************************************************************/
EXTERN_API( OSErr )
AEInstallEventHandler           (AEEventClass           theAEEventClass,
                                 AEEventID              theAEEventID,
                                 AEEventHandlerUPP      handler,
                                 long                   handlerRefcon,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x091F, 0xA816);

EXTERN_API( OSErr )
AERemoveEventHandler            (AEEventClass           theAEEventClass,
                                 AEEventID              theAEEventID,
                                 AEEventHandlerUPP      handler,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0720, 0xA816);

EXTERN_API( OSErr )
AEGetEventHandler               (AEEventClass           theAEEventClass,
                                 AEEventID              theAEEventID,
                                 AEEventHandlerUPP *    handler,
                                 long *                 handlerRefcon,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0921, 0xA816);


/**************************************************************************
 The following four calls are available for applications which need more
 sophisticated control over when and how events are processed. Applications
 which implement multi-session servers or which implement their own
 internal event queueing will probably be the major clients of these
 routines. They can be called from within a handler to prevent the AEM from
 disposing of the AppleEvent when the handler returns. They can be used to
 asynchronously process the event (as MacApp does).
**************************************************************************/
EXTERN_API( OSErr )
AESuspendTheCurrentEvent        (const AppleEvent *     theAppleEvent)                      THREEWORDINLINE(0x303C, 0x022B, 0xA816);

/* 
 Note: The following routine tells the AppleEvent manager that processing
 is either about to resume or has been completed on a previously suspended
 event. The procPtr passed in as the dispatcher parameter will be called to
 attempt to redispatch the event. Several constants for the dispatcher
 parameter allow special behavior. They are:
    - kAEUseStandardDispatch means redispatch as if the event was just
      received, using the standard AppleEvent dispatch mechanism.
    - kAENoDispatch means ignore the parameter.
      Use this in the case where the event has been handled and no
      redispatch is needed.
    - non nil means call the routine which the dispatcher points to.
*/
/* Constants for Refcon in AEResumeTheCurrentEvent with kAEUseStandardDispatch */
enum {
    kAEDoNotIgnoreHandler       = 0x00000000,
    kAEIgnoreAppPhacHandler     = 0x00000001,                   /* available only in vers 1.0.1 and greater */
    kAEIgnoreAppEventHandler    = 0x00000002,                   /* available only in vers 1.0.1 and greater */
    kAEIgnoreSysPhacHandler     = 0x00000004,                   /* available only in vers 1.0.1 and greater */
    kAEIgnoreSysEventHandler    = 0x00000008,                   /* available only in vers 1.0.1 and greater */
    kAEIngoreBuiltInEventHandler = 0x00000010,                  /* available only in vers 1.0.1 and greater */
    kAEDontDisposeOnResume      = (long)0x80000000              /* available only in vers 1.0.1 and greater */
};

/* Constants for AEResumeTheCurrentEvent */
enum {
    kAENoDispatch               = 0,                            /* dispatch parameter to AEResumeTheCurrentEvent takes a pointer to a dispatch */
    kAEUseStandardDispatch      = (long)0xFFFFFFFF              /* table, or one of these two constants */
};

EXTERN_API( OSErr )
AEResumeTheCurrentEvent         (const AppleEvent *     theAppleEvent,
                                 const AppleEvent *     reply,
                                 AEEventHandlerUPP      dispatcher,
                                 long                   handlerRefcon)                      THREEWORDINLINE(0x303C, 0x0818, 0xA816);

EXTERN_API( OSErr )
AEGetTheCurrentEvent            (AppleEvent *           theAppleEvent)                      THREEWORDINLINE(0x303C, 0x021A, 0xA816);

EXTERN_API( OSErr )
AESetTheCurrentEvent            (const AppleEvent *     theAppleEvent)                      THREEWORDINLINE(0x303C, 0x022C, 0xA816);


/**************************************************************************
  These calls are used to set up and modify special hooks into the
  AppleEvent manager.
**************************************************************************/
EXTERN_API( OSErr )
AEInstallSpecialHandler         (AEKeyword              functionClass,
                                 AEEventHandlerUPP      handler,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0500, 0xA816);

EXTERN_API( OSErr )
AERemoveSpecialHandler          (AEKeyword              functionClass,
                                 AEEventHandlerUPP      handler,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0501, 0xA816);

EXTERN_API( OSErr )
AEGetSpecialHandler             (AEKeyword              functionClass,
                                 AEEventHandlerUPP *    handler,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x052D, 0xA816);


/**************************************************************************
  This call was added in version 1.0.1. If called with the keyword
  keyAERecorderCount ('recr'), the number of recorders that are
  currently active is returned in 'result'
  (available only in vers 1.0.1 and greater).
**************************************************************************/
EXTERN_API( OSErr )
AEManagerInfo                   (AEKeyword              keyWord,
                                 long *                 result)                             THREEWORDINLINE(0x303C, 0x0441, 0xA816);






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

#endif /* __APPLEEVENTS__ */

