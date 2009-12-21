/*
     File:       GXMessages.h
 
     Contains:   This file contains all of the public data structures,
 
     Version:    Technology: Quickdraw GX 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __GXMESSAGES__
#define __GXMESSAGES__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
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

#if defined(__MWERKS__) && TARGET_CPU_68K
    #pragma push
    #pragma pointers_in_D0
#endif
 

/*

    >>>>>> CONSTANTS <<<<<<

*/
/* Message Manager Trap */
enum {
    messageManagerTrap          = 0xABFB
};


/* Message Manager Error Result Codes */
enum {
    messageStopLoopingErr       = -5775,
    cantDeleteRunningHandlerErr = -5776,
    noMessageTableErr           = -5777,
    dupSignatureErr             = -5778,
    messageNotReceivedErr       = -5799
};


/*
    DATA TYPES
*/
typedef CALLBACK_API_C( OSErr , MessageHandlerOverrideProcPtr )(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);
typedef STACK_UPP_TYPE(MessageHandlerOverrideProcPtr)           MessageHandlerOverrideUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(MessageHandlerOverrideUPP)
    NewMessageHandlerOverrideUPP    (MessageHandlerOverrideProcPtr userRoutine);

    EXTERN_API(void)
    DisposeMessageHandlerOverrideUPP    (MessageHandlerOverrideUPP userUPP);

    EXTERN_API(OSErr)
    InvokeMessageHandlerOverrideUPP    (long                arg1,
                                    long                    arg2,
                                    long                    arg3,
                                    long                    arg4,
                                    long                    arg5,
                                    long                    arg6,
                                    MessageHandlerOverrideUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppMessageHandlerOverrideProcInfo = 0x0003FFE1 };        /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewMessageHandlerOverrideUPP(userRoutine)               (MessageHandlerOverrideUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMessageHandlerOverrideProcInfo, GetCurrentArchitecture())
    #define DisposeMessageHandlerOverrideUPP(userUPP)               DisposeRoutineDescriptor(userUPP)
    #define InvokeMessageHandlerOverrideUPP(arg1, arg2, arg3, arg4, arg5, arg6, userUPP)  (OSErr)CALL_SIX_PARAMETER_UPP((userUPP), uppMessageHandlerOverrideProcInfo, (arg1), (arg2), (arg3), (arg4), (arg5), (arg6))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewMessageHandlerOverrideProc(userRoutine)              NewMessageHandlerOverrideUPP(userRoutine)
#define CallMessageHandlerOverrideProc(userRoutine, arg1, arg2, arg3, arg4, arg5, arg6) InvokeMessageHandlerOverrideUPP(arg1, arg2, arg3, arg4, arg5, arg6, userRoutine)
typedef CALLBACK_API_C( void , MessageGlobalsInitProcPtr )(void *messageGlobals);
typedef STACK_UPP_TYPE(MessageGlobalsInitProcPtr)               MessageGlobalsInitUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(MessageGlobalsInitUPP)
    NewMessageGlobalsInitUPP       (MessageGlobalsInitProcPtr userRoutine);

    EXTERN_API(void)
    DisposeMessageGlobalsInitUPP    (MessageGlobalsInitUPP  userUPP);

    EXTERN_API(void)
    InvokeMessageGlobalsInitUPP    (void *                  messageGlobals,
                                    MessageGlobalsInitUPP   userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppMessageGlobalsInitProcInfo = 0x000000C1 };            /* no_return_value Func(4_bytes) */
    #define NewMessageGlobalsInitUPP(userRoutine)                   (MessageGlobalsInitUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMessageGlobalsInitProcInfo, GetCurrentArchitecture())
    #define DisposeMessageGlobalsInitUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define InvokeMessageGlobalsInitUPP(messageGlobals, userUPP)    CALL_ONE_PARAMETER_UPP((userUPP), uppMessageGlobalsInitProcInfo, (messageGlobals))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewMessageGlobalsInitProc(userRoutine)                  NewMessageGlobalsInitUPP(userRoutine)
#define CallMessageGlobalsInitProc(userRoutine, messageGlobals) InvokeMessageGlobalsInitUPP(messageGlobals, userRoutine)
#if OLDROUTINENAMES
typedef MessageHandlerOverrideProcPtr   MessageHandlerOverrideProc;
typedef MessageGlobalsInitProcPtr       MessageGlobalsInitProc;
#endif  /* OLDROUTINENAMES */

typedef struct OpaqueMessageHandler*    MessageHandler;
typedef struct OpaqueMessageObject*     MessageObject;
/*

    PUBLIC INTERFACES

    Message Handler API Routines
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( long )
CountMessageHandlerInstances    (void)                                                      TWOWORDINLINE(0x7028, 0xABFB);

EXTERN_API_C( void *)
GetMessageHandlerClassContext   (void)                                                      TWOWORDINLINE(0x7029, 0xABFB);

EXTERN_API_C( void *)
SetMessageHandlerClassContext   (void *                 anyValue)                           TWOWORDINLINE(0x702A, 0xABFB);

EXTERN_API_C( void *)
GetMessageHandlerInstanceContext (void)                                                     TWOWORDINLINE(0x702B, 0xABFB);

EXTERN_API_C( void *)
SetMessageHandlerInstanceContext (void *                anyValue)                           TWOWORDINLINE(0x702C, 0xABFB);

EXTERN_API_C( OSErr )
NewMessageGlobals               (long                   messageGlobalsSize,
                                 MessageGlobalsInitUPP  initProc)                           TWOWORDINLINE(0x702D, 0xABFB);

EXTERN_API_C( void )
DisposeMessageGlobals           (void)                                                      TWOWORDINLINE(0x702E, 0xABFB);


/*
    Message Sending API Routines
*/
#ifndef BUILDING_GXMESSAGES
#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
SendObjectMessage               (MessageObject          msgObject,
                                 long                   messageSelector,
                                 ...)                                                       FIVEWORDINLINE(0x205F, 0x221F, 0x702F, 0xABFB, 0x518F);

EXTERN_API_C( OSErr )
SendObjectMessageTo             (MessageObject          msgObject,
                                 long                   messageSelector,
                                 MessageHandler         msgHandler,
                                 ...)                                                       SEVENWORDINLINE(0x205F, 0x221F, 0x241F, 0x7030, 0xABFB, 0x9EFC, 0x000C);

EXTERN_API_C( OSErr )
SendObjectMessageFor            (MessageObject          msgObject,
                                 long                   messageSelector,
                                 MessageHandler         msgHandler,
                                 ...)                                                       SEVENWORDINLINE(0x205F, 0x221F, 0x241F, 0x7037, 0xABFB, 0x9EFC, 0x000C);

EXTERN_API_C( OSErr )
StartObjectMessageAt            (MessageObject          msgObject,
                                 long                   messageSelector,
                                 MessageHandler         msgHandler,
                                 ...)                                                       SEVENWORDINLINE(0x205F, 0x221F, 0x241F, 0x7031, 0xABFB, 0x9EFC, 0x000C);

#if TARGET_OS_MAC
    #define MacSendMessage SendMessage
#endif
EXTERN_API_C( OSErr )
MacSendMessage                  (long                   messageSelector,
                                 ...)                                                       FOURWORDINLINE(0x221F, 0x7032, 0xABFB, 0x598F);

EXTERN_API_C( OSErr )
SendMessageTo                   (long                   messageSelector,
                                 MessageHandler         msgHandler,
                                 ...)                                                       FIVEWORDINLINE(0x221F, 0x241F, 0x7033, 0xABFB, 0x518F);

EXTERN_API_C( OSErr )
StartMessageAt                  (long                   messageSelector,
                                 MessageHandler         msgHandler,
                                 ...)                                                       FIVEWORDINLINE(0x221F, 0x241F, 0x7034, 0xABFB, 0x518F);

EXTERN_API_C( OSErr )
ForwardMessage                  (long                   messageSelector,
                                 ...)                                                       FOURWORDINLINE(0x221F, 0x7035, 0xABFB, 0x598F);

EXTERN_API_C( OSErr )
ForwardThisMessage              (void *                 parameter1,
                                 ...)                                                       TWOWORDINLINE(0x7036, 0xABFB);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* BUILDING_GXMESSAGES */

 
#if defined(__MWERKS__) && TARGET_CPU_68K
    #pragma pop
#endif


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

#endif /* __GXMESSAGES__ */

