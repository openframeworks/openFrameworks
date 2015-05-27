/*
     File:       Timer.h
 
     Contains:   Time Manager interfaces.
 
     Version:    Technology: Mac OS 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TIMER__
#define __TIMER__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __OSUTILS__
#include <OSUtils.h>
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
                                                                /* high bit of qType is set if task is active */
    kTMTaskActive               = (1L << 15)
};

typedef struct TMTask                   TMTask;
typedef TMTask *                        TMTaskPtr;
typedef CALLBACK_API( void , TimerProcPtr )(TMTaskPtr tmTaskPtr);
/*
    WARNING: TimerProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(TimerProcPtr)                         TimerUPP;

struct TMTask {
    QElemPtr                        qLink;
    short                           qType;
    TimerUPP                        tmAddr;
    long                            tmCount;
    long                            tmWakeUp;
    long                            tmReserved;
};

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter InsTime(__A0)
                                                                                            #endif
EXTERN_API( void )
InsTime                         (QElemPtr               tmTaskPtr)                          ONEWORDINLINE(0xA058);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter InsXTime(__A0)
                                                                                            #endif
EXTERN_API( void )
InsXTime                        (QElemPtr               tmTaskPtr)                          ONEWORDINLINE(0xA458);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PrimeTime(__A0, __D0)
                                                                                            #endif
EXTERN_API( void )
PrimeTime                       (QElemPtr               tmTaskPtr,
                                 long                   count)                              ONEWORDINLINE(0xA05A);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter RmvTime(__A0)
                                                                                            #endif
EXTERN_API( void )
RmvTime                         (QElemPtr               tmTaskPtr)                          ONEWORDINLINE(0xA059);


/* InstallTimeTask, InstallXTimeTask, PrimeTimeTask and RemoveTimeTask work */
/* just like InsTime, InsXTime, PrimeTime, and RmvTime except that they */
/* return an OSErr result. */
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 InstallTimeTask(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
InstallTimeTask                 (QElemPtr               tmTaskPtr)                          ONEWORDINLINE(0xA058);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 InstallXTimeTask(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
InstallXTimeTask                (QElemPtr               tmTaskPtr)                          ONEWORDINLINE(0xA458);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PrimeTimeTask(__A0, __D0)
                                                                                            #endif
EXTERN_API( OSErr )
PrimeTimeTask                   (QElemPtr               tmTaskPtr,
                                 long                   count)                              ONEWORDINLINE(0xA05A);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 RemoveTimeTask(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
RemoveTimeTask                  (QElemPtr               tmTaskPtr)                          ONEWORDINLINE(0xA059);


#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( void )
Microseconds                    (UnsignedWide *         microTickCount)                     FOURWORDINLINE(0xA193, 0x225F, 0x22C8, 0x2280);

#if OPAQUE_UPP_TYPES
    EXTERN_API(TimerUPP)
    NewTimerUPP                    (TimerProcPtr            userRoutine);

    EXTERN_API(void)
    DisposeTimerUPP                (TimerUPP                userUPP);

    EXTERN_API(void)
    InvokeTimerUPP                 (TMTaskPtr               tmTaskPtr,
                                    TimerUPP                userUPP);

#else
    enum { uppTimerProcInfo = 0x0000B802 };                         /* register no_return_value Func(4_bytes:A1) */
    #define NewTimerUPP(userRoutine)                                (TimerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTimerProcInfo, GetCurrentArchitecture())
    #define DisposeTimerUPP(userUPP)                                DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeTimerUPP(__A1, __A0)
    void InvokeTimerUPP(TMTaskPtr tmTaskPtr, TimerUPP userUPP) = 0x4E90;
    #else
        #define InvokeTimerUPP(tmTaskPtr, userUPP)                      CALL_ONE_PARAMETER_UPP((userUPP), uppTimerProcInfo, (tmTaskPtr))
    #endif
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewTimerProc(userRoutine)                               NewTimerUPP(userRoutine)
#define CallTimerProc(userRoutine, tmTaskPtr)                   InvokeTimerUPP(tmTaskPtr, userRoutine)



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

#endif /* __TIMER__ */

