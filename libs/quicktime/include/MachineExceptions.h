/*
     File:       MachineExceptions.h
 
     Contains:   Processor Exception Handling Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MACHINEEXCEPTIONS__
#define __MACHINEEXCEPTIONS__

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
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

/* Some basic declarations used throughout the kernel */
typedef struct OpaqueAreaID*            AreaID;
#if TARGET_OS_MAC
/* Machine Dependent types for PowerPC: */

struct MachineInformationPowerPC {
    UnsignedWide                    CTR;
    UnsignedWide                    LR;
    UnsignedWide                    PC;
    unsigned long                   CR;
    unsigned long                   XER;
    unsigned long                   MSR;
    unsigned long                   MQ;
    unsigned long                   ExceptKind;
    unsigned long                   DSISR;
    UnsignedWide                    DAR;
    UnsignedWide                    Reserved;
};
typedef struct MachineInformationPowerPC MachineInformationPowerPC;

struct RegisterInformationPowerPC {
    UnsignedWide                    R0;
    UnsignedWide                    R1;
    UnsignedWide                    R2;
    UnsignedWide                    R3;
    UnsignedWide                    R4;
    UnsignedWide                    R5;
    UnsignedWide                    R6;
    UnsignedWide                    R7;
    UnsignedWide                    R8;
    UnsignedWide                    R9;
    UnsignedWide                    R10;
    UnsignedWide                    R11;
    UnsignedWide                    R12;
    UnsignedWide                    R13;
    UnsignedWide                    R14;
    UnsignedWide                    R15;
    UnsignedWide                    R16;
    UnsignedWide                    R17;
    UnsignedWide                    R18;
    UnsignedWide                    R19;
    UnsignedWide                    R20;
    UnsignedWide                    R21;
    UnsignedWide                    R22;
    UnsignedWide                    R23;
    UnsignedWide                    R24;
    UnsignedWide                    R25;
    UnsignedWide                    R26;
    UnsignedWide                    R27;
    UnsignedWide                    R28;
    UnsignedWide                    R29;
    UnsignedWide                    R30;
    UnsignedWide                    R31;
};
typedef struct RegisterInformationPowerPC RegisterInformationPowerPC;

struct FPUInformationPowerPC {
    UnsignedWide                    Registers[32];
    unsigned long                   FPSCR;
    unsigned long                   Reserved;
};
typedef struct FPUInformationPowerPC    FPUInformationPowerPC;

union Vector128 {
#ifdef __VEC__
 vector unsigned long            v;
#endif
    unsigned long                   l[4];
    unsigned short                  s[8];
    unsigned char                   c[16];
};
typedef union Vector128                 Vector128;

struct VectorInformationPowerPC {
    Vector128                       Registers[32];
    Vector128                       VSCR;
    UInt32                          VRsave;
};
typedef struct VectorInformationPowerPC VectorInformationPowerPC;
/* Exception related declarations */
enum {
    kWriteReference             = 0,
    kReadReference              = 1,
    kFetchReference             = 2,
    writeReference              = kWriteReference,              /* Obsolete name*/
    readReference               = kReadReference,               /* Obsolete name*/
    fetchReference              = kFetchReference               /* Obsolete name*/
};


typedef unsigned long                   MemoryReferenceKind;

struct MemoryExceptionInformation {
    AreaID                          theArea;
    LogicalAddress                  theAddress;
    OSStatus                        theError;
    MemoryReferenceKind             theReference;
};
typedef struct MemoryExceptionInformation MemoryExceptionInformation;
enum {
    kUnknownException           = 0,
    kIllegalInstructionException = 1,
    kTrapException              = 2,
    kAccessException            = 3,
    kUnmappedMemoryException    = 4,
    kExcludedMemoryException    = 5,
    kReadOnlyMemoryException    = 6,
    kUnresolvablePageFaultException = 7,
    kPrivilegeViolationException = 8,
    kTraceException             = 9,
    kInstructionBreakpointException = 10,
    kDataBreakpointException    = 11,
    kIntegerException           = 12,
    kFloatingPointException     = 13,
    kStackOverflowException     = 14,
    kTaskTerminationException   = 15,
    kTaskCreationException      = 16,
    kDataAlignmentException     = 17
};

#if OLDROUTINENAMES
enum {
    unknownException            = kUnknownException,            /* Obsolete name*/
    illegalInstructionException = kIllegalInstructionException, /* Obsolete name*/
    trapException               = kTrapException,               /* Obsolete name*/
    accessException             = kAccessException,             /* Obsolete name*/
    unmappedMemoryException     = kUnmappedMemoryException,     /* Obsolete name*/
    excludedMemoryException     = kExcludedMemoryException,     /* Obsolete name*/
    readOnlyMemoryException     = kReadOnlyMemoryException,     /* Obsolete name*/
    unresolvablePageFaultException = kUnresolvablePageFaultException, /* Obsolete name*/
    privilegeViolationException = kPrivilegeViolationException, /* Obsolete name*/
    traceException              = kTraceException,              /* Obsolete name*/
    instructionBreakpointException = kInstructionBreakpointException, /* Obsolete name*/
    dataBreakpointException     = kDataBreakpointException,     /* Obsolete name*/
    integerException            = kIntegerException,            /* Obsolete name*/
    floatingPointException      = kFloatingPointException,      /* Obsolete name*/
    stackOverflowException      = kStackOverflowException,      /* Obsolete name*/
    terminationException        = kTaskTerminationException,    /* Obsolete name*/
    kTerminationException       = kTaskTerminationException     /* Obsolete name*/
};

#endif  /* OLDROUTINENAMES */


typedef unsigned long                   ExceptionKind;

union ExceptionInfo {
    MemoryExceptionInformation *    memoryInfo;
};
typedef union ExceptionInfo             ExceptionInfo;

struct ExceptionInformationPowerPC {
    ExceptionKind                   theKind;
    MachineInformationPowerPC *     machineState;
    RegisterInformationPowerPC *    registerImage;
    FPUInformationPowerPC *         FPUImage;
    ExceptionInfo                   info;
    VectorInformationPowerPC *      vectorImage;
};
typedef struct ExceptionInformationPowerPC ExceptionInformationPowerPC;
#if TARGET_CPU_PPC || TARGET_CPU_68K

typedef ExceptionInformationPowerPC     ExceptionInformation;
typedef MachineInformationPowerPC       MachineInformation;
typedef RegisterInformationPowerPC      RegisterInformation;
typedef FPUInformationPowerPC           FPUInformation;
typedef VectorInformationPowerPC        VectorInformation;
#endif  /* TARGET_CPU_PPC || TARGET_CPU_68K */

/* 
    Note:   An ExceptionHandler is NOT a UniversalProcPtr, except in Carbon.
            It must be a PowerPC function pointer with NO routine descriptor, 
            except on Carbon, where it must be a UniversalProcPtr (TPP actually)
            to allow the interface to work from both CFM and Mach-O.
*/
typedef CALLBACK_API_C( OSStatus , ExceptionHandlerProcPtr )(ExceptionInformation *theException);
typedef TVECTOR_UPP_TYPE(ExceptionHandlerProcPtr)               ExceptionHandlerUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(ExceptionHandlerUPP)
    NewExceptionHandlerUPP         (ExceptionHandlerProcPtr userRoutine);

    EXTERN_API(void)
    DisposeExceptionHandlerUPP     (ExceptionHandlerUPP     userUPP);

    EXTERN_API(OSStatus)
    InvokeExceptionHandlerUPP      (ExceptionInformation *  theException,
                                    ExceptionHandlerUPP     userUPP);

#else
    #define NewExceptionHandlerUPP(userRoutine)                     (userRoutine)
    #define DisposeExceptionHandlerUPP(userUPP)                     
    #define InvokeExceptionHandlerUPP(theException, userUPP)        (*userUPP)(theException)
#endif
/*
   ExceptionHandler function pointers (TPP):
   on classic PowerPC, use raw function pointers
   on classic PowerPC with OPAQUE_UPP_TYPES=1, use UPP's
   on Carbon, use UPP's
*/
#if TARGET_OS_MAC && !OPAQUE_UPP_TYPES
/* use raw function pointers*/
typedef ExceptionHandlerProcPtr         ExceptionHandlerTPP;
#else
/* use UPP's*/
typedef ExceptionHandlerUPP             ExceptionHandlerTPP;
#endif  /* TARGET_OS_MAC && !OPAQUE_UPP_TYPES */

typedef ExceptionHandlerTPP             ExceptionHandler;
/* Routine for installing per-process exception handlers */
EXTERN_API( ExceptionHandlerTPP ) InstallExceptionHandler(ExceptionHandlerTPP theHandler);

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

#endif /* __MACHINEEXCEPTIONS__ */

