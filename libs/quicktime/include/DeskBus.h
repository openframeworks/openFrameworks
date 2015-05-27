/*
     File:       DeskBus.h
 
     Contains:   Apple Desktop Bus (ADB) Interfaces.
 
     Version:    Technology: System 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1987-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DESKBUS__
#define __DESKBUS__

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

#if TARGET_OS_MAC
typedef SInt8                           ADBAddress;
#if CALL_NOT_IN_CARBON
typedef CALLBACK_API( void , ADBCompletionProcPtr )(Ptr buffer, Ptr refCon, long command);
/*
    WARNING: ADBCompletionProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef CALLBACK_API( void , ADBDeviceDriverProcPtr )(SInt8 devAddress, SInt8 devType);
/*
    WARNING: ADBDeviceDriverProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
/*
    TempADBServiceRoutineUPP is needed because of circular definition of
    ADBServiceRoutineProcPtr and ADBServiceRoutineUPP depending on each other.
*/
typedef REGISTER_UPP_TYPE(ADBServiceRoutineProcPtr)             TempADBServiceRoutineUPP;
typedef CALLBACK_API( void , ADBServiceRoutineProcPtr )(Ptr buffer, TempADBServiceRoutineUPP completionProc, Ptr refCon, long command);
/*
    WARNING: ADBServiceRoutineProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef CALLBACK_API( void , ADBInitProcPtr )(SInt8 callOrder);
/*
    WARNING: ADBInitProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(ADBCompletionProcPtr)                 ADBCompletionUPP;
typedef REGISTER_UPP_TYPE(ADBDeviceDriverProcPtr)               ADBDeviceDriverUPP;
typedef REGISTER_UPP_TYPE(ADBServiceRoutineProcPtr)             ADBServiceRoutineUPP;
typedef REGISTER_UPP_TYPE(ADBInitProcPtr)                       ADBInitUPP;

struct ADBDataBlock {
    SInt8                           devType;                    /* original handler ID */
    SInt8                           origADBAddr;                /* original ADB Address */
    ADBServiceRoutineUPP            dbServiceRtPtr;             /* service routine pointer */
    Ptr                             dbDataAreaAddr;             /* this field is passed as the refCon parameter to the service routine */
};
typedef struct ADBDataBlock             ADBDataBlock;

typedef ADBDataBlock *                  ADBDBlkPtr;

struct ADBSetInfoBlock {
    ADBServiceRoutineUPP            siService;                  /* service routine pointer */
    Ptr                             siDataAreaAddr;             /* this field is passed as the refCon parameter to the service routine */
};
typedef struct ADBSetInfoBlock          ADBSetInfoBlock;

typedef ADBSetInfoBlock *               ADBSInfoPtr;
/* ADBOpBlock is only used when calling ADBOp from 68k assembly code */

struct ADBOpBlock {
    Ptr                             dataBuffPtr;                /* buffer: pointer to variable length data buffer */
    ADBServiceRoutineUPP            opServiceRtPtr;             /* completionProc: completion routine pointer */
    Ptr                             opDataAreaPtr;              /* refCon: this field is passed as the refCon parameter to the completion routine */
};
typedef struct ADBOpBlock               ADBOpBlock;

typedef ADBOpBlock *                    ADBOpBPtr;
#endif  /* CALL_NOT_IN_CARBON */
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
ADBReInit                       (void)                                                      ONEWORDINLINE(0xA07B);

/*
    ADBOp has a different interface for 68k assembly than for everything else
    for 68k assembly the interface is 
    #pragma parameter __D0 ADBOp(__A0,__D0)
    OSErr ADBOp( ADBOpBlock * pb, short commandNum );
*/
/*
    IMPORTANT NOTE:
    "Inside Macintosh: Devices" documents the completion routine for ADBOp will be called with
    four parameters using 68k register based calling conventions, specifically the completion routine
    passed in should be of type ADBServiceRoutineProcPtr. However, when upp types were first added
    to this interface file, the type ADBCompletionUPP was mistakenly used for the second parameter
    to ADBOp. Since applications have shipped using completion routines of type ADBCompletionUPP,
    the mistake cannot be corrected.
    The only difference between ADBServiceRoutineUPP and ADBCompletionUPP is the former takes an extra
    argument which is a pointer to itself, fortunately not needed for PowerPC code.
    For compatibility with existing 68k code, when an ADBOp completion routine is called,
    68k register A1 will point to the completion routine, as documented in Inside Mac.
*/
#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
ADBOp                           (Ptr                    refCon,
                                 ADBCompletionUPP       compRout,
                                 Ptr                    buffer,
                                 short                  commandNum);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 CountADBs
                                                                                            #endif
EXTERN_API( short )
CountADBs                       (void)                                                      ONEWORDINLINE(0xA077);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetIndADB(__A0, __D0)
                                                                                            #endif
EXTERN_API( ADBAddress )
GetIndADB                       (ADBDataBlock *         info,
                                 short                  devTableIndex)                      ONEWORDINLINE(0xA078);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 GetADBInfo(__A0, __D0)
                                                                                            #endif
EXTERN_API( OSErr )
GetADBInfo                      (ADBDataBlock *         info,
                                 ADBAddress             adbAddr)                            ONEWORDINLINE(0xA079);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 SetADBInfo(__A0, __D0)
                                                                                            #endif
EXTERN_API( OSErr )
SetADBInfo                      (const ADBSetInfoBlock * info,
                                 ADBAddress             adbAddr)                            ONEWORDINLINE(0xA07A);

#endif  /* CALL_NOT_IN_CARBON */

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ADBCompletionUPP)
    NewADBCompletionUPP            (ADBCompletionProcPtr    userRoutine);

    EXTERN_API(ADBDeviceDriverUPP)
    NewADBDeviceDriverUPP          (ADBDeviceDriverProcPtr  userRoutine);

    EXTERN_API(ADBServiceRoutineUPP)
    NewADBServiceRoutineUPP        (ADBServiceRoutineProcPtr userRoutine);

    EXTERN_API(ADBInitUPP)
    NewADBInitUPP                  (ADBInitProcPtr          userRoutine);

    EXTERN_API(void)
    DisposeADBCompletionUPP        (ADBCompletionUPP        userUPP);

    EXTERN_API(void)
    DisposeADBDeviceDriverUPP      (ADBDeviceDriverUPP      userUPP);

    EXTERN_API(void)
    DisposeADBServiceRoutineUPP    (ADBServiceRoutineUPP    userUPP);

    EXTERN_API(void)
    DisposeADBInitUPP              (ADBInitUPP              userUPP);

    EXTERN_API(void)
    InvokeADBCompletionUPP         (Ptr                     buffer,
                                    Ptr                     refCon,
                                    long                    command,
                                    ADBCompletionUPP        userUPP);

    EXTERN_API(void)
    InvokeADBDeviceDriverUPP       (SInt8                   devAddress,
                                    SInt8                   devType,
                                    ADBDeviceDriverUPP      userUPP);

    EXTERN_API(void)
    InvokeADBServiceRoutineUPP     (Ptr                     buffer,
                                    TempADBServiceRoutineUPP completionProc,
                                    Ptr                     refCon,
                                    long                    command,
                                    ADBServiceRoutineUPP    userUPP);

    EXTERN_API(void)
    InvokeADBInitUPP               (SInt8                   callOrder,
                                    ADBInitUPP              userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppADBCompletionProcInfo = 0x007B9802 };                 /* register no_return_value Func(4_bytes:A0, 4_bytes:A2, 4_bytes:D0) */
    enum { uppADBDeviceDriverProcInfo = 0x00050802 };               /* register no_return_value Func(1_byte:D0, 1_byte:D1) */
    enum { uppADBServiceRoutineProcInfo = 0x0F779802 };             /* register no_return_value Func(4_bytes:A0, 4_bytes:A1, 4_bytes:A2, 4_bytes:D0) */
    enum { uppADBInitProcInfo = 0x00000802 };                       /* register no_return_value Func(1_byte:D0) */
    #define NewADBCompletionUPP(userRoutine)                        (ADBCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppADBCompletionProcInfo, GetCurrentArchitecture())
    #define NewADBDeviceDriverUPP(userRoutine)                      (ADBDeviceDriverUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppADBDeviceDriverProcInfo, GetCurrentArchitecture())
    #define NewADBServiceRoutineUPP(userRoutine)                    (ADBServiceRoutineUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppADBServiceRoutineProcInfo, GetCurrentArchitecture())
    #define NewADBInitUPP(userRoutine)                              (ADBInitUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppADBInitProcInfo, GetCurrentArchitecture())
    #define DisposeADBCompletionUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeADBDeviceDriverUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeADBServiceRoutineUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeADBInitUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    /* InvokeADBCompletionUPP can't be called from classic 68k without glue code */
    #else
        #define InvokeADBCompletionUPP(buffer, refCon, command, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppADBCompletionProcInfo, (buffer), (refCon), (command))
    #endif
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeADBDeviceDriverUPP(__D0, __D1, __A0)
    void InvokeADBDeviceDriverUPP(SInt8 devAddress, SInt8 devType, ADBDeviceDriverUPP userUPP) = 0x4E90;
    #else
        #define InvokeADBDeviceDriverUPP(devAddress, devType, userUPP)  CALL_TWO_PARAMETER_UPP((userUPP), uppADBDeviceDriverProcInfo, (devAddress), (devType))
    #endif
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    /* InvokeADBServiceRoutineUPP can't be called from classic 68k without glue code */
    #else
        #define InvokeADBServiceRoutineUPP(buffer, completionProc, refCon, command, userUPP)  CALL_FOUR_PARAMETER_UPP((userUPP), uppADBServiceRoutineProcInfo, (buffer), (completionProc), (refCon), (command))
    #endif
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeADBInitUPP(__D0, __A0)
    void InvokeADBInitUPP(SInt8 callOrder, ADBInitUPP userUPP) = 0x4E90;
    #else
        #define InvokeADBInitUPP(callOrder, userUPP)                    CALL_ONE_PARAMETER_UPP((userUPP), uppADBInitProcInfo, (callOrder))
    #endif
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewADBCompletionProc(userRoutine)                       NewADBCompletionUPP(userRoutine)
#define NewADBDeviceDriverProc(userRoutine)                     NewADBDeviceDriverUPP(userRoutine)
#define NewADBServiceRoutineProc(userRoutine)                   NewADBServiceRoutineUPP(userRoutine)
#define NewADBInitProc(userRoutine)                             NewADBInitUPP(userRoutine)
#define CallADBCompletionProc(userRoutine, buffer, refCon, command) InvokeADBCompletionUPP(buffer, refCon, command, userRoutine)
#define CallADBDeviceDriverProc(userRoutine, devAddress, devType) InvokeADBDeviceDriverUPP(devAddress, devType, userRoutine)
#define CallADBServiceRoutineProc(userRoutine, buffer, completionProc, refCon, command) InvokeADBServiceRoutineUPP(buffer, completionProc, refCon, command, userRoutine)
#define CallADBInitProc(userRoutine, callOrder)                 InvokeADBInitUPP(callOrder, userRoutine)
#endif  /* TARGET_OS_MAC */


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

#endif /* __DESKBUS__ */

