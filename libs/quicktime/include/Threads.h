/*
     File:       Threads.h
 
     Contains:   Thread Manager Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1991-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __THREADS__
#define __THREADS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
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

/* Thread states*/

typedef UInt16 ThreadState;
enum {
    kReadyThreadState           = 0,
    kStoppedThreadState         = 1,
    kRunningThreadState         = 2
};

/* Error codes have been meoved to Errors.(pah)*/

/* Thread environment characteristics*/
typedef void *                          ThreadTaskRef;
/* Thread characteristics*/

typedef UInt32 ThreadStyle;
enum {
    kCooperativeThread          = 1L << 0,
    kPreemptiveThread           = 1L << 1
};

/* Thread identifiers*/

typedef UInt32 ThreadID;
enum {
    kNoThreadID                 = 0,
    kCurrentThreadID            = 1,
    kApplicationThreadID        = 2
};

/* Options when creating a thread*/

typedef UInt32 ThreadOptions;
enum {
    kNewSuspend                 = (1 << 0),
    kUsePremadeThread           = (1 << 1),
    kCreateIfNeeded             = (1 << 2),
    kFPUNotNeeded               = (1 << 3),
    kExactMatchThread           = (1 << 4)
};

/* Information supplied to the custom scheduler*/

struct SchedulerInfoRec {
    UInt32                          InfoRecSize;
    ThreadID                        CurrentThreadID;
    ThreadID                        SuggestedThreadID;
    ThreadID                        InterruptedCoopThreadID;
};
typedef struct SchedulerInfoRec         SchedulerInfoRec;
typedef SchedulerInfoRec *              SchedulerInfoRecPtr;

/*
    The following ProcPtrs cannot be interchanged with UniversalProcPtrs because
    of differences between 680x0 and PowerPC runtime architectures with regard to
    the implementation of the Thread Manager.
 */

typedef void *                          voidPtr;
/* Prototype for thread's entry (main) routine*/
typedef CALLBACK_API( voidPtr , ThreadEntryProcPtr )(void *threadParam);
/* Prototype for custom thread scheduler routine*/
typedef CALLBACK_API( ThreadID , ThreadSchedulerProcPtr )(SchedulerInfoRecPtr schedulerInfo);
/* Prototype for custom thread switcher routine*/
typedef CALLBACK_API( void , ThreadSwitchProcPtr )(ThreadID threadBeingSwitched, void *switchProcParam);
/* Prototype for thread termination notification routine*/
typedef CALLBACK_API( void , ThreadTerminationProcPtr )(ThreadID threadTerminated, void *terminationProcParam);
/* Prototype for debugger NewThread notification*/
typedef CALLBACK_API( void , DebuggerNewThreadProcPtr )(ThreadID threadCreated);
/* Prototype for debugger DisposeThread notification*/
typedef CALLBACK_API( void , DebuggerDisposeThreadProcPtr )(ThreadID threadDeleted);
/* Prototype for debugger schedule notification*/
typedef CALLBACK_API( ThreadID , DebuggerThreadSchedulerProcPtr )(SchedulerInfoRecPtr schedulerInfo);
typedef TVECTOR_UPP_TYPE(ThreadEntryProcPtr)                    ThreadEntryUPP;
typedef TVECTOR_UPP_TYPE(ThreadSchedulerProcPtr)                ThreadSchedulerUPP;
typedef TVECTOR_UPP_TYPE(ThreadSwitchProcPtr)                   ThreadSwitchUPP;
typedef TVECTOR_UPP_TYPE(ThreadTerminationProcPtr)              ThreadTerminationUPP;
typedef TVECTOR_UPP_TYPE(DebuggerNewThreadProcPtr)              DebuggerNewThreadUPP;
typedef TVECTOR_UPP_TYPE(DebuggerDisposeThreadProcPtr)          DebuggerDisposeThreadUPP;
typedef TVECTOR_UPP_TYPE(DebuggerThreadSchedulerProcPtr)        DebuggerThreadSchedulerUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(ThreadEntryUPP)
    NewThreadEntryUPP              (ThreadEntryProcPtr      userRoutine);

    EXTERN_API(ThreadSchedulerUPP)
    NewThreadSchedulerUPP          (ThreadSchedulerProcPtr  userRoutine);

    EXTERN_API(ThreadSwitchUPP)
    NewThreadSwitchUPP             (ThreadSwitchProcPtr     userRoutine);

    EXTERN_API(ThreadTerminationUPP)
    NewThreadTerminationUPP        (ThreadTerminationProcPtr userRoutine);

    EXTERN_API(DebuggerNewThreadUPP)
    NewDebuggerNewThreadUPP        (DebuggerNewThreadProcPtr userRoutine);

    EXTERN_API(DebuggerDisposeThreadUPP)
    NewDebuggerDisposeThreadUPP    (DebuggerDisposeThreadProcPtr userRoutine);

    EXTERN_API(DebuggerThreadSchedulerUPP)
    NewDebuggerThreadSchedulerUPP    (DebuggerThreadSchedulerProcPtr userRoutine);

    EXTERN_API(void)
    DisposeThreadEntryUPP          (ThreadEntryUPP          userUPP);

    EXTERN_API(void)
    DisposeThreadSchedulerUPP      (ThreadSchedulerUPP      userUPP);

    EXTERN_API(void)
    DisposeThreadSwitchUPP         (ThreadSwitchUPP         userUPP);

    EXTERN_API(void)
    DisposeThreadTerminationUPP    (ThreadTerminationUPP    userUPP);

    EXTERN_API(void)
    DisposeDebuggerNewThreadUPP    (DebuggerNewThreadUPP    userUPP);

    EXTERN_API(void)
    DisposeDebuggerDisposeThreadUPP    (DebuggerDisposeThreadUPP userUPP);

    EXTERN_API(void)
    DisposeDebuggerThreadSchedulerUPP    (DebuggerThreadSchedulerUPP userUPP);

    EXTERN_API(voidPtr)
    InvokeThreadEntryUPP           (void *                  threadParam,
                                    ThreadEntryUPP          userUPP);

    EXTERN_API(ThreadID)
    InvokeThreadSchedulerUPP       (SchedulerInfoRecPtr     schedulerInfo,
                                    ThreadSchedulerUPP      userUPP);

    EXTERN_API(void)
    InvokeThreadSwitchUPP          (ThreadID                threadBeingSwitched,
                                    void *                  switchProcParam,
                                    ThreadSwitchUPP         userUPP);

    EXTERN_API(void)
    InvokeThreadTerminationUPP     (ThreadID                threadTerminated,
                                    void *                  terminationProcParam,
                                    ThreadTerminationUPP    userUPP);

    EXTERN_API(void)
    InvokeDebuggerNewThreadUPP     (ThreadID                threadCreated,
                                    DebuggerNewThreadUPP    userUPP);

    EXTERN_API(void)
    InvokeDebuggerDisposeThreadUPP    (ThreadID             threadDeleted,
                                    DebuggerDisposeThreadUPP userUPP);

    EXTERN_API(ThreadID)
    InvokeDebuggerThreadSchedulerUPP    (SchedulerInfoRecPtr schedulerInfo,
                                    DebuggerThreadSchedulerUPP userUPP);

#else
    #define NewThreadEntryUPP(userRoutine)                          (userRoutine)
    #define NewThreadSchedulerUPP(userRoutine)                      (userRoutine)
    #define NewThreadSwitchUPP(userRoutine)                         (userRoutine)
    #define NewThreadTerminationUPP(userRoutine)                    (userRoutine)
    #define NewDebuggerNewThreadUPP(userRoutine)                    (userRoutine)
    #define NewDebuggerDisposeThreadUPP(userRoutine)                (userRoutine)
    #define NewDebuggerThreadSchedulerUPP(userRoutine)              (userRoutine)
    #define DisposeThreadEntryUPP(userUPP)                          
    #define DisposeThreadSchedulerUPP(userUPP)                      
    #define DisposeThreadSwitchUPP(userUPP)                         
    #define DisposeThreadTerminationUPP(userUPP)                    
    #define DisposeDebuggerNewThreadUPP(userUPP)                    
    #define DisposeDebuggerDisposeThreadUPP(userUPP)                
    #define DisposeDebuggerThreadSchedulerUPP(userUPP)              
    #define InvokeThreadEntryUPP(threadParam, userUPP)              (*userUPP)(threadParam)
    #define InvokeThreadSchedulerUPP(schedulerInfo, userUPP)        (*userUPP)(schedulerInfo)
    #define InvokeThreadSwitchUPP(threadBeingSwitched, switchProcParam, userUPP)  (*userUPP)(threadBeingSwitched, switchProcParam)
    #define InvokeThreadTerminationUPP(threadTerminated, terminationProcParam, userUPP)  (*userUPP)(threadTerminated, terminationProcParam)
    #define InvokeDebuggerNewThreadUPP(threadCreated, userUPP)      (*userUPP)(threadCreated)
    #define InvokeDebuggerDisposeThreadUPP(threadDeleted, userUPP)  (*userUPP)(threadDeleted)
    #define InvokeDebuggerThreadSchedulerUPP(schedulerInfo, userUPP)  (*userUPP)(schedulerInfo)
#endif
/*
   Thread Manager function pointers (TPP):
   on classic 68k use raw function pointers (same as UPP's)
   on classic PowerPC, use raw function pointers
   on CFM-68K, use UPP's
   on Carbon, use UPP's
*/

#if TARGET_OS_MAC && TARGET_CPU_PPC && !TARGET_API_MAC_CARBON
/* use raw function pointers*/
typedef ThreadEntryProcPtr              ThreadEntryTPP;
typedef ThreadSchedulerProcPtr          ThreadSchedulerTPP;
typedef ThreadSwitchProcPtr             ThreadSwitchTPP;
typedef ThreadTerminationProcPtr        ThreadTerminationTPP;
typedef DebuggerNewThreadProcPtr        DebuggerNewThreadTPP;
typedef DebuggerDisposeThreadProcPtr    DebuggerDisposeThreadTPP;
typedef DebuggerThreadSchedulerProcPtr  DebuggerThreadSchedulerTPP;
#else
/* use UPP's*/
typedef ThreadEntryUPP                  ThreadEntryTPP;
typedef ThreadSchedulerUPP              ThreadSchedulerTPP;
typedef ThreadSwitchUPP                 ThreadSwitchTPP;
typedef ThreadTerminationUPP            ThreadTerminationTPP;
typedef DebuggerNewThreadUPP            DebuggerNewThreadTPP;
typedef DebuggerDisposeThreadUPP        DebuggerDisposeThreadTPP;
typedef DebuggerThreadSchedulerUPP      DebuggerThreadSchedulerTPP;
#endif  /* TARGET_OS_MAC && TARGET_CPU_PPC && !TARGET_API_MAC_CARBON */

EXTERN_API( OSErr )
NewThread                       (ThreadStyle            threadStyle,
                                 ThreadEntryTPP         threadEntry,
                                 void *                 threadParam,
                                 Size                   stackSize,
                                 ThreadOptions          options,
                                 void **                threadResult,
                                 ThreadID *             threadMade)                         THREEWORDINLINE(0x303C, 0x0E03, 0xABF2);

EXTERN_API( OSErr )
SetThreadScheduler              (ThreadSchedulerTPP     threadScheduler)                    THREEWORDINLINE(0x303C, 0x0209, 0xABF2);

EXTERN_API( OSErr )
SetThreadSwitcher               (ThreadID               thread,
                                 ThreadSwitchTPP        threadSwitcher,
                                 void *                 switchProcParam,
                                 Boolean                inOrOut)                            THREEWORDINLINE(0x303C, 0x070A, 0xABF2);

EXTERN_API( OSErr )
SetThreadTerminator             (ThreadID               thread,
                                 ThreadTerminationTPP   threadTerminator,
                                 void *                 terminationProcParam)               THREEWORDINLINE(0x303C, 0x0611, 0xABF2);

EXTERN_API( OSErr )
SetDebuggerNotificationProcs    (DebuggerNewThreadTPP   notifyNewThread,
                                 DebuggerDisposeThreadTPP  notifyDisposeThread,
                                 DebuggerThreadSchedulerTPP  notifyThreadScheduler)         THREEWORDINLINE(0x303C, 0x060D, 0xABF2);

EXTERN_API( OSErr )
CreateThreadPool                (ThreadStyle            threadStyle,
                                 SInt16                 numToCreate,
                                 Size                   stackSize)                          THREEWORDINLINE(0x303C, 0x0501, 0xABF2);

EXTERN_API( OSErr )
GetFreeThreadCount              (ThreadStyle            threadStyle,
                                 SInt16 *               freeCount)                          THREEWORDINLINE(0x303C, 0x0402, 0xABF2);

EXTERN_API( OSErr )
GetSpecificFreeThreadCount      (ThreadStyle            threadStyle,
                                 Size                   stackSize,
                                 SInt16 *               freeCount)                          THREEWORDINLINE(0x303C, 0x0615, 0xABF2);

EXTERN_API( OSErr )
GetDefaultThreadStackSize       (ThreadStyle            threadStyle,
                                 Size *                 stackSize)                          THREEWORDINLINE(0x303C, 0x0413, 0xABF2);

EXTERN_API( OSErr )
ThreadCurrentStackSpace         (ThreadID               thread,
                                 UInt32 *               freeStack)                          THREEWORDINLINE(0x303C, 0x0414, 0xABF2);

EXTERN_API( OSErr )
DisposeThread                   (ThreadID               threadToDump,
                                 void *                 threadResult,
                                 Boolean                recycleThread)                      THREEWORDINLINE(0x303C, 0x0504, 0xABF2);

EXTERN_API( OSErr )
YieldToThread                   (ThreadID               suggestedThread)                    THREEWORDINLINE(0x303C, 0x0205, 0xABF2);

EXTERN_API( OSErr )
YieldToAnyThread                (void)                                                      FOURWORDINLINE(0x42A7, 0x303C, 0x0205, 0xABF2);

#if TARGET_OS_MAC
    #define MacGetCurrentThread GetCurrentThread
#endif
EXTERN_API( OSErr )
MacGetCurrentThread             (ThreadID *             currentThreadID)                    THREEWORDINLINE(0x303C, 0x0206, 0xABF2);

EXTERN_API( OSErr )
GetThreadState                  (ThreadID               threadToGet,
                                 ThreadState *          threadState)                        THREEWORDINLINE(0x303C, 0x0407, 0xABF2);

EXTERN_API( OSErr )
SetThreadState                  (ThreadID               threadToSet,
                                 ThreadState            newState,
                                 ThreadID               suggestedThread)                    THREEWORDINLINE(0x303C, 0x0508, 0xABF2);

EXTERN_API( OSErr )
SetThreadStateEndCritical       (ThreadID               threadToSet,
                                 ThreadState            newState,
                                 ThreadID               suggestedThread)                    THREEWORDINLINE(0x303C, 0x0512, 0xABF2);

EXTERN_API( OSErr )
ThreadBeginCritical             (void)                                                      THREEWORDINLINE(0x303C, 0x000B, 0xABF2);

EXTERN_API( OSErr )
ThreadEndCritical               (void)                                                      THREEWORDINLINE(0x303C, 0x000C, 0xABF2);

EXTERN_API( OSErr )
GetThreadCurrentTaskRef         (ThreadTaskRef *        threadTRef)                         THREEWORDINLINE(0x303C, 0x020E, 0xABF2);

EXTERN_API( OSErr )
GetThreadStateGivenTaskRef      (ThreadTaskRef          threadTRef,
                                 ThreadID               threadToGet,
                                 ThreadState *          threadState)                        THREEWORDINLINE(0x303C, 0x060F, 0xABF2);

EXTERN_API( OSErr )
SetThreadReadyGivenTaskRef      (ThreadTaskRef          threadTRef,
                                 ThreadID               threadToSet)                        THREEWORDINLINE(0x303C, 0x0410, 0xABF2);


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

#endif /* __THREADS__ */

