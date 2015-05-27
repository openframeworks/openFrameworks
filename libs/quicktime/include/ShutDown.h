/*
     File:       ShutDown.h
 
     Contains:   Shutdown Manager Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1987-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SHUTDOWN__
#define __SHUTDOWN__

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

enum {
    sdOnPowerOff                = 1,                            /*call procedure before power off.*/
    sdOnRestart                 = 2,                            /*call procedure before restart.*/
    sdOnUnmount                 = 4,                            /*call procedure before unmounting.*/
    sdOnDrivers                 = 8,                            /*call procedure before closing drivers.*/
    sdOnBootVolUnmount          = 16,                           /*call procedure before unmounting boot volume and VM volume but after unmounting all other volumes*/
    sdRestartOrPower            = 3                             /*call before either power off or restart.*/
};

#if CALL_NOT_IN_CARBON
typedef CALLBACK_API( void , ShutDwnProcPtr )(short shutDownStage);
/*
    WARNING: ShutDwnProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(ShutDwnProcPtr)                       ShutDwnUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ShutDwnUPP)
    NewShutDwnUPP                  (ShutDwnProcPtr          userRoutine);

    EXTERN_API(void)
    DisposeShutDwnUPP              (ShutDwnUPP              userUPP);

    EXTERN_API(void)
    InvokeShutDwnUPP               (short                   shutDownStage,
                                    ShutDwnUPP              userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppShutDwnProcInfo = 0x00001002 };                       /* register no_return_value Func(2_bytes:D0) */
    #define NewShutDwnUPP(userRoutine)                              (ShutDwnUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppShutDwnProcInfo, GetCurrentArchitecture())
    #define DisposeShutDwnUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeShutDwnUPP(__D0, __A0)
    void InvokeShutDwnUPP(short shutDownStage, ShutDwnUPP userUPP) = 0x4E90;
    #else
        #define InvokeShutDwnUPP(shutDownStage, userUPP)                CALL_ONE_PARAMETER_UPP((userUPP), uppShutDwnProcInfo, (shutDownStage))
    #endif
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewShutDwnProc(userRoutine)                             NewShutDwnUPP(userRoutine)
#define CallShutDwnProc(userRoutine, shutDownStage)             InvokeShutDwnUPP(shutDownStage, userRoutine)
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
ShutDwnPower                    (void)                                                      THREEWORDINLINE(0x3F3C, 0x0001, 0xA895);

EXTERN_API( void )
ShutDwnStart                    (void)                                                      THREEWORDINLINE(0x3F3C, 0x0002, 0xA895);

EXTERN_API( void )
ShutDwnInstall                  (ShutDwnUPP             shutDownProc,
                                 short                  flags)                              THREEWORDINLINE(0x3F3C, 0x0003, 0xA895);

EXTERN_API( void )
ShutDwnRemove                   (ShutDwnUPP             shutDownProc)                       THREEWORDINLINE(0x3F3C, 0x0004, 0xA895);

#endif  /* CALL_NOT_IN_CARBON */

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

#endif /* __SHUTDOWN__ */

