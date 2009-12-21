/*
     File:       CMCalibrator.h
 
     Contains:   ColorSync Calibration API
 
     Version:    Technology: ColorSync 2.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1998-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CMCALIBRATOR__
#define __CMCALIBRATOR__

#ifndef __CMAPPLICATION__
#include <CMApplication.h>
#endif

#ifndef __DISPLAYS__
#include <Displays.h>
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

typedef CALLBACK_API( void , CalibrateEventProcPtr )(EventRecord *event);
typedef STACK_UPP_TYPE(CalibrateEventProcPtr)                   CalibrateEventUPP;

/* Interface for new ColorSync monitor calibrators (ColorSync 2.6 and greater) */

enum {
    kCalibratorNamePrefix       = FOUR_CHAR_CODE('cali')
};


struct CalibratorInfo {
    UInt32                          dataSize;                   /* Size of this structure - compatibility */
    AVIDType                        displayID;                  /* Contains an hDC on Win32 */
    UInt32                          profileLocationSize;        /* Max size for returned profile location */
    CMProfileLocation *             profileLocationPtr;         /* For returning the profile */
    CalibrateEventUPP               eventProc;                  /* Ignored on Win32 */
    Boolean                         isGood;                     /* true or false */
};
typedef struct CalibratorInfo           CalibratorInfo;
typedef CALLBACK_API( Boolean , CanCalibrateProcPtr )(AVIDType displayID, Str255 errMessage);
typedef CALLBACK_API( OSErr , CalibrateProcPtr )(CalibratorInfo *theInfo);
typedef STACK_UPP_TYPE(CanCalibrateProcPtr)                     CanCalibrateUPP;
typedef STACK_UPP_TYPE(CalibrateProcPtr)                        CalibrateUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(CalibrateEventUPP)
    NewCalibrateEventUPP           (CalibrateEventProcPtr   userRoutine);

    EXTERN_API(CanCalibrateUPP)
    NewCanCalibrateUPP             (CanCalibrateProcPtr     userRoutine);

    EXTERN_API(CalibrateUPP)
    NewCalibrateUPP                (CalibrateProcPtr        userRoutine);

    EXTERN_API(void)
    DisposeCalibrateEventUPP       (CalibrateEventUPP       userUPP);

    EXTERN_API(void)
    DisposeCanCalibrateUPP         (CanCalibrateUPP         userUPP);

    EXTERN_API(void)
    DisposeCalibrateUPP            (CalibrateUPP            userUPP);

    EXTERN_API(void)
    InvokeCalibrateEventUPP        (EventRecord *           event,
                                    CalibrateEventUPP       userUPP);

    EXTERN_API(Boolean)
    InvokeCanCalibrateUPP          (AVIDType                displayID,
                                    Str255                  errMessage,
                                    CanCalibrateUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeCalibrateUPP             (CalibratorInfo *        theInfo,
                                    CalibrateUPP            userUPP);

#else
    enum { uppCalibrateEventProcInfo = 0x000000C0 };                /* pascal no_return_value Func(4_bytes) */
    enum { uppCanCalibrateProcInfo = 0x000003D0 };                  /* pascal 1_byte Func(4_bytes, 4_bytes) */
    enum { uppCalibrateProcInfo = 0x000000E0 };                     /* pascal 2_bytes Func(4_bytes) */
    #define NewCalibrateEventUPP(userRoutine)                       (CalibrateEventUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCalibrateEventProcInfo, GetCurrentArchitecture())
    #define NewCanCalibrateUPP(userRoutine)                         (CanCalibrateUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCanCalibrateProcInfo, GetCurrentArchitecture())
    #define NewCalibrateUPP(userRoutine)                            (CalibrateUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCalibrateProcInfo, GetCurrentArchitecture())
    #define DisposeCalibrateEventUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposeCanCalibrateUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeCalibrateUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define InvokeCalibrateEventUPP(event, userUPP)                 CALL_ONE_PARAMETER_UPP((userUPP), uppCalibrateEventProcInfo, (event))
    #define InvokeCanCalibrateUPP(displayID, errMessage, userUPP)   (Boolean)CALL_TWO_PARAMETER_UPP((userUPP), uppCanCalibrateProcInfo, (displayID), (errMessage))
    #define InvokeCalibrateUPP(theInfo, userUPP)                    (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppCalibrateProcInfo, (theInfo))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewCalibrateEventProc(userRoutine)                      NewCalibrateEventUPP(userRoutine)
#define NewCanCalibrateProc(userRoutine)                        NewCanCalibrateUPP(userRoutine)
#define NewCalibrateProc(userRoutine)                           NewCalibrateUPP(userRoutine)
#define CallCalibrateEventProc(userRoutine, event)              InvokeCalibrateEventUPP(event, userRoutine)
#define CallCanCalibrateProc(userRoutine, displayID, errMessage) InvokeCanCalibrateUPP(displayID, errMessage, userRoutine)
#define CallCalibrateProc(userRoutine, theInfo)                 InvokeCalibrateUPP(theInfo, userRoutine)
#if OLDROUTINENAMES
/* Interface for original ColorSync monitor calibrators (ColorSync 2.5.x) */
enum {
    kOldCalibratorNamePrefix    = FOUR_CHAR_CODE('Cali')
};


struct OldCalibratorInfo {
    AVIDType                        displayID;                  /* Contains an hDC on Win32 */
    CMProfileLocation               profileLocation;
    CalibrateEventUPP               eventProc;                  /* Ignored on Win32 */
    UInt32                          reserved;                   /* Unused */
    UInt32                          flags;                      /* Unused */
    Boolean                         isGood;                     /* true or false */
    SInt8                           byteFiller;                 /* Unused */
};
typedef struct OldCalibratorInfo        OldCalibratorInfo;
typedef CALLBACK_API( Boolean , OldCanCalibrateProcPtr )(AVIDType displayID);
typedef CALLBACK_API( OSErr , OldCalibrateProcPtr )(OldCalibratorInfo *theInfo);
typedef STACK_UPP_TYPE(OldCanCalibrateProcPtr)                  OldCanCalibrateUPP;
typedef STACK_UPP_TYPE(OldCalibrateProcPtr)                     OldCalibrateUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(OldCanCalibrateUPP)
    NewOldCanCalibrateUPP          (OldCanCalibrateProcPtr  userRoutine);

    EXTERN_API(OldCalibrateUPP)
    NewOldCalibrateUPP             (OldCalibrateProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeOldCanCalibrateUPP      (OldCanCalibrateUPP      userUPP);

    EXTERN_API(void)
    DisposeOldCalibrateUPP         (OldCalibrateUPP         userUPP);

    EXTERN_API(Boolean)
    InvokeOldCanCalibrateUPP       (AVIDType                displayID,
                                    OldCanCalibrateUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeOldCalibrateUPP          (OldCalibratorInfo *     theInfo,
                                    OldCalibrateUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppOldCanCalibrateProcInfo = 0x000000D0 };               /* pascal 1_byte Func(4_bytes) */
    enum { uppOldCalibrateProcInfo = 0x000000E0 };                  /* pascal 2_bytes Func(4_bytes) */
    #define NewOldCanCalibrateUPP(userRoutine)                      (OldCanCalibrateUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOldCanCalibrateProcInfo, GetCurrentArchitecture())
    #define NewOldCalibrateUPP(userRoutine)                         (OldCalibrateUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOldCalibrateProcInfo, GetCurrentArchitecture())
    #define DisposeOldCanCalibrateUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeOldCalibrateUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeOldCanCalibrateUPP(displayID, userUPP)            (Boolean)CALL_ONE_PARAMETER_UPP((userUPP), uppOldCanCalibrateProcInfo, (displayID))
    #define InvokeOldCalibrateUPP(theInfo, userUPP)                 (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppOldCalibrateProcInfo, (theInfo))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewOldCanCalibrateProc(userRoutine)                     NewOldCanCalibrateUPP(userRoutine)
#define NewOldCalibrateProc(userRoutine)                        NewOldCalibrateUPP(userRoutine)
#define CallOldCanCalibrateProc(userRoutine, displayID)         InvokeOldCanCalibrateUPP(displayID, userRoutine)
#define CallOldCalibrateProc(userRoutine, theInfo)              InvokeOldCalibrateUPP(theInfo, userRoutine)
#endif  /* OLDROUTINENAMES */


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

#endif /* __CMCALIBRATOR__ */

