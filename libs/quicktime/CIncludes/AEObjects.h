/*
     File:       AEObjects.h
 
     Contains:   Object Support Library Interfaces.
 
     Version:    Technology: System 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1991-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AEOBJECTS__
#define __AEOBJECTS__

#ifndef __OSUTILS__
#include <OSUtils.h>
#endif

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
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
                                                                /**** LOGICAL OPERATOR CONSTANTS  ****/
    kAEAND                      = FOUR_CHAR_CODE('AND '),       /*  0x414e4420  */
    kAEOR                       = FOUR_CHAR_CODE('OR  '),       /*  0x4f522020  */
    kAENOT                      = FOUR_CHAR_CODE('NOT '),       /*  0x4e4f5420  */
                                                                /**** ABSOLUTE ORDINAL CONSTANTS  ****/
    kAEFirst                    = FOUR_CHAR_CODE('firs'),       /*  0x66697273  */
    kAELast                     = FOUR_CHAR_CODE('last'),       /*  0x6c617374  */
    kAEMiddle                   = FOUR_CHAR_CODE('midd'),       /*  0x6d696464  */
    kAEAny                      = FOUR_CHAR_CODE('any '),       /*  0x616e7920  */
    kAEAll                      = FOUR_CHAR_CODE('all '),       /*  0x616c6c20  */
                                                                /**** RELATIVE ORDINAL CONSTANTS  ****/
    kAENext                     = FOUR_CHAR_CODE('next'),       /*  0x6e657874  */
    kAEPrevious                 = FOUR_CHAR_CODE('prev'),       /*  0x70726576  */
                                                                /**** KEYWORD CONSTANT    ****/
    keyAECompOperator           = FOUR_CHAR_CODE('relo'),       /*  0x72656c6f  */
    keyAELogicalTerms           = FOUR_CHAR_CODE('term'),       /*  0x7465726d  */
    keyAELogicalOperator        = FOUR_CHAR_CODE('logc'),       /*  0x6c6f6763  */
    keyAEObject1                = FOUR_CHAR_CODE('obj1'),       /*  0x6f626a31  */
    keyAEObject2                = FOUR_CHAR_CODE('obj2'),       /*  0x6f626a32  */
                                                                /*    ... for Keywords for getting fields out of object specifier records. */
    keyAEDesiredClass           = FOUR_CHAR_CODE('want'),       /*  0x77616e74  */
    keyAEContainer              = FOUR_CHAR_CODE('from'),       /*  0x66726f6d  */
    keyAEKeyForm                = FOUR_CHAR_CODE('form'),       /*  0x666f726d  */
    keyAEKeyData                = FOUR_CHAR_CODE('seld')        /*  0x73656c64  */
};

enum {
                                                                /*    ... for Keywords for getting fields out of Range specifier records. */
    keyAERangeStart             = FOUR_CHAR_CODE('star'),       /*  0x73746172  */
    keyAERangeStop              = FOUR_CHAR_CODE('stop'),       /*  0x73746f70  */
                                                                /*    ... special handler selectors for OSL Callbacks. */
    keyDisposeTokenProc         = FOUR_CHAR_CODE('xtok'),       /*  0x78746f6b  */
    keyAECompareProc            = FOUR_CHAR_CODE('cmpr'),       /*  0x636d7072  */
    keyAECountProc              = FOUR_CHAR_CODE('cont'),       /*  0x636f6e74  */
    keyAEMarkTokenProc          = FOUR_CHAR_CODE('mkid'),       /*  0x6d6b6964  */
    keyAEMarkProc               = FOUR_CHAR_CODE('mark'),       /*  0x6d61726b  */
    keyAEAdjustMarksProc        = FOUR_CHAR_CODE('adjm'),       /*  0x61646a6d  */
    keyAEGetErrDescProc         = FOUR_CHAR_CODE('indc')        /*  0x696e6463  */
};

/****   VALUE and TYPE CONSTANTS    ****/
enum {
                                                                /*    ... possible values for the keyAEKeyForm field of an object specifier. */
    formAbsolutePosition        = FOUR_CHAR_CODE('indx'),       /*  0x696e6478  */
    formRelativePosition        = FOUR_CHAR_CODE('rele'),       /*  0x72656c65  */
    formTest                    = FOUR_CHAR_CODE('test'),       /*  0x74657374  */
    formRange                   = FOUR_CHAR_CODE('rang'),       /*  0x72616e67  */
    formPropertyID              = FOUR_CHAR_CODE('prop'),       /*  0x70726f70  */
    formName                    = FOUR_CHAR_CODE('name'),       /*  0x6e616d65  */
                                                                /*    ... relevant types (some of these are often pared with forms above). */
    typeObjectSpecifier         = FOUR_CHAR_CODE('obj '),       /*  0x6f626a20  */
    typeObjectBeingExamined     = FOUR_CHAR_CODE('exmn'),       /*  0x65786d6e  */
    typeCurrentContainer        = FOUR_CHAR_CODE('ccnt'),       /*  0x63636e74  */
    typeToken                   = FOUR_CHAR_CODE('toke'),       /*  0x746f6b65  */
    typeRelativeDescriptor      = FOUR_CHAR_CODE('rel '),       /*  0x72656c20  */
    typeAbsoluteOrdinal         = FOUR_CHAR_CODE('abso'),       /*  0x6162736f  */
    typeIndexDescriptor         = FOUR_CHAR_CODE('inde'),       /*  0x696e6465  */
    typeRangeDescriptor         = FOUR_CHAR_CODE('rang'),       /*  0x72616e67  */
    typeLogicalDescriptor       = FOUR_CHAR_CODE('logi'),       /*  0x6c6f6769  */
    typeCompDescriptor          = FOUR_CHAR_CODE('cmpd'),       /*  0x636d7064  */
    typeOSLTokenList            = FOUR_CHAR_CODE('ostl')        /*  0x6F73746C  */
};

/* Possible values for flags parameter to AEResolve.  They're additive */
enum {
    kAEIDoMinimum               = 0x0000,
    kAEIDoWhose                 = 0x0001,
    kAEIDoMarking               = 0x0004,
    kAEPassSubDescs             = 0x0008,
    kAEResolveNestedLists       = 0x0010,
    kAEHandleSimpleRanges       = 0x0020,
    kAEUseRelativeIterators     = 0x0040
};

/**** SPECIAL CONSTANTS FOR CUSTOM WHOSE-CLAUSE RESOLUTION */
enum {
    typeWhoseDescriptor         = FOUR_CHAR_CODE('whos'),       /*  0x77686f73  */
    formWhose                   = FOUR_CHAR_CODE('whos'),       /*  0x77686f73  */
    typeWhoseRange              = FOUR_CHAR_CODE('wrng'),       /*  0x77726e67  */
    keyAEWhoseRangeStart        = FOUR_CHAR_CODE('wstr'),       /*  0x77737472  */
    keyAEWhoseRangeStop         = FOUR_CHAR_CODE('wstp'),       /*  0x77737470  */
    keyAEIndex                  = FOUR_CHAR_CODE('kidx'),       /*  0x6b696478  */
    keyAETest                   = FOUR_CHAR_CODE('ktst')        /*  0x6b747374  */
};

/*
    used for rewriting tokens in place of 'ccnt' descriptors
    This record is only of interest to those who, when they...
    ...get ranges as key data in their accessor procs, choose
    ...to resolve them manually rather than call AEResolve again.
*/

struct ccntTokenRecord {
    DescType                        tokenClass;
    AEDesc                          token;
};
typedef struct ccntTokenRecord          ccntTokenRecord;

typedef ccntTokenRecord *               ccntTokenRecPtr;
typedef ccntTokenRecPtr *               ccntTokenRecHandle;
#if OLDROUTINENAMES
typedef AEDesc *                        DescPtr;
typedef DescPtr *                       DescHandle;
#endif  /* OLDROUTINENAMES */

/* typedefs providing type checking for procedure pointers */
typedef CALLBACK_API( OSErr , OSLAccessorProcPtr )(DescType desiredClass, const AEDesc *container, DescType containerClass, DescType form, const AEDesc *selectionData, AEDesc *value, long accessorRefcon);
typedef CALLBACK_API( OSErr , OSLCompareProcPtr )(DescType oper, const AEDesc *obj1, const AEDesc *obj2, Boolean *result);
typedef CALLBACK_API( OSErr , OSLCountProcPtr )(DescType desiredType, DescType containerClass, const AEDesc *container, long *result);
typedef CALLBACK_API( OSErr , OSLDisposeTokenProcPtr )(AEDesc *unneededToken);
typedef CALLBACK_API( OSErr , OSLGetMarkTokenProcPtr )(const AEDesc *dContainerToken, DescType containerClass, AEDesc *result);
typedef CALLBACK_API( OSErr , OSLGetErrDescProcPtr )(AEDesc **appDescPtr);
typedef CALLBACK_API( OSErr , OSLMarkProcPtr )(const AEDesc *dToken, const AEDesc *markToken, long index);
typedef CALLBACK_API( OSErr , OSLAdjustMarksProcPtr )(long newStart, long newStop, const AEDesc *markToken);
typedef STACK_UPP_TYPE(OSLAccessorProcPtr)                      OSLAccessorUPP;
typedef STACK_UPP_TYPE(OSLCompareProcPtr)                       OSLCompareUPP;
typedef STACK_UPP_TYPE(OSLCountProcPtr)                         OSLCountUPP;
typedef STACK_UPP_TYPE(OSLDisposeTokenProcPtr)                  OSLDisposeTokenUPP;
typedef STACK_UPP_TYPE(OSLGetMarkTokenProcPtr)                  OSLGetMarkTokenUPP;
typedef STACK_UPP_TYPE(OSLGetErrDescProcPtr)                    OSLGetErrDescUPP;
typedef STACK_UPP_TYPE(OSLMarkProcPtr)                          OSLMarkUPP;
typedef STACK_UPP_TYPE(OSLAdjustMarksProcPtr)                   OSLAdjustMarksUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(OSLAccessorUPP)
    NewOSLAccessorUPP              (OSLAccessorProcPtr      userRoutine);

    EXTERN_API(OSLCompareUPP)
    NewOSLCompareUPP               (OSLCompareProcPtr       userRoutine);

    EXTERN_API(OSLCountUPP)
    NewOSLCountUPP                 (OSLCountProcPtr         userRoutine);

    EXTERN_API(OSLDisposeTokenUPP)
    NewOSLDisposeTokenUPP          (OSLDisposeTokenProcPtr  userRoutine);

    EXTERN_API(OSLGetMarkTokenUPP)
    NewOSLGetMarkTokenUPP          (OSLGetMarkTokenProcPtr  userRoutine);

    EXTERN_API(OSLGetErrDescUPP)
    NewOSLGetErrDescUPP            (OSLGetErrDescProcPtr    userRoutine);

    EXTERN_API(OSLMarkUPP)
    NewOSLMarkUPP                  (OSLMarkProcPtr          userRoutine);

    EXTERN_API(OSLAdjustMarksUPP)
    NewOSLAdjustMarksUPP           (OSLAdjustMarksProcPtr   userRoutine);

    EXTERN_API(void)
    DisposeOSLAccessorUPP          (OSLAccessorUPP          userUPP);

    EXTERN_API(void)
    DisposeOSLCompareUPP           (OSLCompareUPP           userUPP);

    EXTERN_API(void)
    DisposeOSLCountUPP             (OSLCountUPP             userUPP);

    EXTERN_API(void)
    DisposeOSLDisposeTokenUPP      (OSLDisposeTokenUPP      userUPP);

    EXTERN_API(void)
    DisposeOSLGetMarkTokenUPP      (OSLGetMarkTokenUPP      userUPP);

    EXTERN_API(void)
    DisposeOSLGetErrDescUPP        (OSLGetErrDescUPP        userUPP);

    EXTERN_API(void)
    DisposeOSLMarkUPP              (OSLMarkUPP              userUPP);

    EXTERN_API(void)
    DisposeOSLAdjustMarksUPP       (OSLAdjustMarksUPP       userUPP);

    EXTERN_API(OSErr)
    InvokeOSLAccessorUPP           (DescType                desiredClass,
                                    const AEDesc *          container,
                                    DescType                containerClass,
                                    DescType                form,
                                    const AEDesc *          selectionData,
                                    AEDesc *                value,
                                    long                    accessorRefcon,
                                    OSLAccessorUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeOSLCompareUPP            (DescType                oper,
                                    const AEDesc *          obj1,
                                    const AEDesc *          obj2,
                                    Boolean *               result,
                                    OSLCompareUPP           userUPP);

    EXTERN_API(OSErr)
    InvokeOSLCountUPP              (DescType                desiredType,
                                    DescType                containerClass,
                                    const AEDesc *          container,
                                    long *                  result,
                                    OSLCountUPP             userUPP);

    EXTERN_API(OSErr)
    InvokeOSLDisposeTokenUPP       (AEDesc *                unneededToken,
                                    OSLDisposeTokenUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeOSLGetMarkTokenUPP       (const AEDesc *          dContainerToken,
                                    DescType                containerClass,
                                    AEDesc *                result,
                                    OSLGetMarkTokenUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeOSLGetErrDescUPP         (AEDesc **               appDescPtr,
                                    OSLGetErrDescUPP        userUPP);

    EXTERN_API(OSErr)
    InvokeOSLMarkUPP               (const AEDesc *          dToken,
                                    const AEDesc *          markToken,
                                    long                    index,
                                    OSLMarkUPP              userUPP);

    EXTERN_API(OSErr)
    InvokeOSLAdjustMarksUPP        (long                    newStart,
                                    long                    newStop,
                                    const AEDesc *          markToken,
                                    OSLAdjustMarksUPP       userUPP);

#else
    enum { uppOSLAccessorProcInfo = 0x000FFFE0 };                   /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppOSLCompareProcInfo = 0x00003FE0 };                    /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppOSLCountProcInfo = 0x00003FE0 };                      /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppOSLDisposeTokenProcInfo = 0x000000E0 };               /* pascal 2_bytes Func(4_bytes) */
    enum { uppOSLGetMarkTokenProcInfo = 0x00000FE0 };               /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppOSLGetErrDescProcInfo = 0x000000E0 };                 /* pascal 2_bytes Func(4_bytes) */
    enum { uppOSLMarkProcInfo = 0x00000FE0 };                       /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppOSLAdjustMarksProcInfo = 0x00000FE0 };                /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewOSLAccessorUPP(userRoutine)                          (OSLAccessorUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOSLAccessorProcInfo, GetCurrentArchitecture())
    #define NewOSLCompareUPP(userRoutine)                           (OSLCompareUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOSLCompareProcInfo, GetCurrentArchitecture())
    #define NewOSLCountUPP(userRoutine)                             (OSLCountUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOSLCountProcInfo, GetCurrentArchitecture())
    #define NewOSLDisposeTokenUPP(userRoutine)                      (OSLDisposeTokenUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOSLDisposeTokenProcInfo, GetCurrentArchitecture())
    #define NewOSLGetMarkTokenUPP(userRoutine)                      (OSLGetMarkTokenUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOSLGetMarkTokenProcInfo, GetCurrentArchitecture())
    #define NewOSLGetErrDescUPP(userRoutine)                        (OSLGetErrDescUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOSLGetErrDescProcInfo, GetCurrentArchitecture())
    #define NewOSLMarkUPP(userRoutine)                              (OSLMarkUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOSLMarkProcInfo, GetCurrentArchitecture())
    #define NewOSLAdjustMarksUPP(userRoutine)                       (OSLAdjustMarksUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOSLAdjustMarksProcInfo, GetCurrentArchitecture())
    #define DisposeOSLAccessorUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposeOSLCompareUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeOSLCountUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define DisposeOSLDisposeTokenUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeOSLGetMarkTokenUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeOSLGetErrDescUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeOSLMarkUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define DisposeOSLAdjustMarksUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define InvokeOSLAccessorUPP(desiredClass, container, containerClass, form, selectionData, value, accessorRefcon, userUPP)  (OSErr)CALL_SEVEN_PARAMETER_UPP((userUPP), uppOSLAccessorProcInfo, (desiredClass), (container), (containerClass), (form), (selectionData), (value), (accessorRefcon))
    #define InvokeOSLCompareUPP(oper, obj1, obj2, result, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppOSLCompareProcInfo, (oper), (obj1), (obj2), (result))
    #define InvokeOSLCountUPP(desiredType, containerClass, container, result, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppOSLCountProcInfo, (desiredType), (containerClass), (container), (result))
    #define InvokeOSLDisposeTokenUPP(unneededToken, userUPP)        (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppOSLDisposeTokenProcInfo, (unneededToken))
    #define InvokeOSLGetMarkTokenUPP(dContainerToken, containerClass, result, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppOSLGetMarkTokenProcInfo, (dContainerToken), (containerClass), (result))
    #define InvokeOSLGetErrDescUPP(appDescPtr, userUPP)             (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppOSLGetErrDescProcInfo, (appDescPtr))
    #define InvokeOSLMarkUPP(dToken, markToken, index, userUPP)     (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppOSLMarkProcInfo, (dToken), (markToken), (index))
    #define InvokeOSLAdjustMarksUPP(newStart, newStop, markToken, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppOSLAdjustMarksProcInfo, (newStart), (newStop), (markToken))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewOSLAccessorProc(userRoutine)                         NewOSLAccessorUPP(userRoutine)
#define NewOSLCompareProc(userRoutine)                          NewOSLCompareUPP(userRoutine)
#define NewOSLCountProc(userRoutine)                            NewOSLCountUPP(userRoutine)
#define NewOSLDisposeTokenProc(userRoutine)                     NewOSLDisposeTokenUPP(userRoutine)
#define NewOSLGetMarkTokenProc(userRoutine)                     NewOSLGetMarkTokenUPP(userRoutine)
#define NewOSLGetErrDescProc(userRoutine)                       NewOSLGetErrDescUPP(userRoutine)
#define NewOSLMarkProc(userRoutine)                             NewOSLMarkUPP(userRoutine)
#define NewOSLAdjustMarksProc(userRoutine)                      NewOSLAdjustMarksUPP(userRoutine)
#define CallOSLAccessorProc(userRoutine, desiredClass, container, containerClass, form, selectionData, value, accessorRefcon) InvokeOSLAccessorUPP(desiredClass, container, containerClass, form, selectionData, value, accessorRefcon, userRoutine)
#define CallOSLCompareProc(userRoutine, oper, obj1, obj2, result) InvokeOSLCompareUPP(oper, obj1, obj2, result, userRoutine)
#define CallOSLCountProc(userRoutine, desiredType, containerClass, container, result) InvokeOSLCountUPP(desiredType, containerClass, container, result, userRoutine)
#define CallOSLDisposeTokenProc(userRoutine, unneededToken)     InvokeOSLDisposeTokenUPP(unneededToken, userRoutine)
#define CallOSLGetMarkTokenProc(userRoutine, dContainerToken, containerClass, result) InvokeOSLGetMarkTokenUPP(dContainerToken, containerClass, result, userRoutine)
#define CallOSLGetErrDescProc(userRoutine, appDescPtr)          InvokeOSLGetErrDescUPP(appDescPtr, userRoutine)
#define CallOSLMarkProc(userRoutine, dToken, markToken, index)  InvokeOSLMarkUPP(dToken, markToken, index, userRoutine)
#define CallOSLAdjustMarksProc(userRoutine, newStart, newStop, markToken) InvokeOSLAdjustMarksUPP(newStart, newStop, markToken, userRoutine)



EXTERN_API( OSErr )
AEObjectInit                    (void);

/* Not done by inline, but by direct linking into code.  It sets up the pack
  such that further calls can be via inline */
EXTERN_API( OSErr )
AESetObjectCallbacks            (OSLCompareUPP          myCompareProc,
                                 OSLCountUPP            myCountProc,
                                 OSLDisposeTokenUPP     myDisposeTokenProc,
                                 OSLGetMarkTokenUPP     myGetMarkTokenProc,
                                 OSLMarkUPP             myMarkProc,
                                 OSLAdjustMarksUPP      myAdjustMarksProc,
                                 OSLGetErrDescUPP       myGetErrDescProcPtr)                THREEWORDINLINE(0x303C, 0x0E35, 0xA816);

EXTERN_API( OSErr )
AEResolve                       (const AEDesc *         objectSpecifier,
                                 short                  callbackFlags,
                                 AEDesc *               theToken)                           THREEWORDINLINE(0x303C, 0x0536, 0xA816);

EXTERN_API( OSErr )
AEInstallObjectAccessor         (DescType               desiredClass,
                                 DescType               containerType,
                                 OSLAccessorUPP         theAccessor,
                                 long                   accessorRefcon,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0937, 0xA816);

EXTERN_API( OSErr )
AERemoveObjectAccessor          (DescType               desiredClass,
                                 DescType               containerType,
                                 OSLAccessorUPP         theAccessor,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0738, 0xA816);

EXTERN_API( OSErr )
AEGetObjectAccessor             (DescType               desiredClass,
                                 DescType               containerType,
                                 OSLAccessorUPP *       accessor,
                                 long *                 accessorRefcon,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0939, 0xA816);

EXTERN_API( OSErr )
AEDisposeToken                  (AEDesc *               theToken)                           THREEWORDINLINE(0x303C, 0x023A, 0xA816);

EXTERN_API( OSErr )
AECallObjectAccessor            (DescType               desiredClass,
                                 const AEDesc *         containerToken,
                                 DescType               containerClass,
                                 DescType               keyForm,
                                 const AEDesc *         keyData,
                                 AEDesc *               token)                              THREEWORDINLINE(0x303C, 0x0C3B, 0xA816);



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

#endif /* __AEOBJECTS__ */

