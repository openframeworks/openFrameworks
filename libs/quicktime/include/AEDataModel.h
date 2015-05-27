/*
     File:       AEDataModel.h
 
     Contains:   AppleEvent Data Model Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1996-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AEDATAMODEL__
#define __AEDATAMODEL__

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

/* Apple event descriptor types */
enum {
    typeBoolean                 = FOUR_CHAR_CODE('bool'),
    typeChar                    = FOUR_CHAR_CODE('TEXT')
};

/* Preferred numeric Apple event descriptor types */
enum {
    typeSInt16                  = FOUR_CHAR_CODE('shor'),
    typeSInt32                  = FOUR_CHAR_CODE('long'),
    typeUInt32                  = FOUR_CHAR_CODE('magn'),
    typeSInt64                  = FOUR_CHAR_CODE('comp'),
    typeIEEE32BitFloatingPoint  = FOUR_CHAR_CODE('sing'),
    typeIEEE64BitFloatingPoint  = FOUR_CHAR_CODE('doub'),
    type128BitFloatingPoint     = FOUR_CHAR_CODE('ldbl'),
    typeDecimalStruct           = FOUR_CHAR_CODE('decm')
};

/* Non-preferred Apple event descriptor types */
enum {
    typeSMInt                   = typeSInt16,
    typeShortInteger            = typeSInt16,
    typeInteger                 = typeSInt32,
    typeLongInteger             = typeSInt32,
    typeMagnitude               = typeUInt32,
    typeComp                    = typeSInt64,
    typeSMFloat                 = typeIEEE32BitFloatingPoint,
    typeShortFloat              = typeIEEE32BitFloatingPoint,
    typeFloat                   = typeIEEE64BitFloatingPoint,
    typeLongFloat               = typeIEEE64BitFloatingPoint,
    typeExtended                = FOUR_CHAR_CODE('exte')
};

/* More Apple event descriptor types */
enum {
    typeAEList                  = FOUR_CHAR_CODE('list'),
    typeAERecord                = FOUR_CHAR_CODE('reco'),
    typeAppleEvent              = FOUR_CHAR_CODE('aevt'),
    typeEventRecord             = FOUR_CHAR_CODE('evrc'),
    typeTrue                    = FOUR_CHAR_CODE('true'),
    typeFalse                   = FOUR_CHAR_CODE('fals'),
    typeAlias                   = FOUR_CHAR_CODE('alis'),
    typeEnumerated              = FOUR_CHAR_CODE('enum'),
    typeType                    = FOUR_CHAR_CODE('type'),
    typeAppParameters           = FOUR_CHAR_CODE('appa'),
    typeProperty                = FOUR_CHAR_CODE('prop'),
    typeFSS                     = FOUR_CHAR_CODE('fss '),
    typeKeyword                 = FOUR_CHAR_CODE('keyw'),
    typeSectionH                = FOUR_CHAR_CODE('sect'),
    typeWildCard                = FOUR_CHAR_CODE('****'),
    typeApplSignature           = FOUR_CHAR_CODE('sign'),
    typeQDRectangle             = FOUR_CHAR_CODE('qdrt'),
    typeFixed                   = FOUR_CHAR_CODE('fixd'),
    typeSessionID               = FOUR_CHAR_CODE('ssid'),
    typeTargetID                = FOUR_CHAR_CODE('targ'),
    typeProcessSerialNumber     = FOUR_CHAR_CODE('psn '),
    typeKernelProcessID         = FOUR_CHAR_CODE('kpid'),
    typeDispatcherID            = FOUR_CHAR_CODE('dspt'),
    typeNull                    = FOUR_CHAR_CODE('null')        /* null or nonexistent data */
};

/* Keywords for Apple event attributes */
enum {
    keyTransactionIDAttr        = FOUR_CHAR_CODE('tran'),
    keyReturnIDAttr             = FOUR_CHAR_CODE('rtid'),
    keyEventClassAttr           = FOUR_CHAR_CODE('evcl'),
    keyEventIDAttr              = FOUR_CHAR_CODE('evid'),
    keyAddressAttr              = FOUR_CHAR_CODE('addr'),
    keyOptionalKeywordAttr      = FOUR_CHAR_CODE('optk'),
    keyTimeoutAttr              = FOUR_CHAR_CODE('timo'),
    keyInteractLevelAttr        = FOUR_CHAR_CODE('inte'),       /* this attribute is read only - will be set in AESend */
    keyEventSourceAttr          = FOUR_CHAR_CODE('esrc'),       /* this attribute is read only */
    keyMissedKeywordAttr        = FOUR_CHAR_CODE('miss'),       /* this attribute is read only */
    keyOriginalAddressAttr      = FOUR_CHAR_CODE('from')        /* new in 1.0.1 */
};


/*  Constants used for specifying the factoring of AEDescLists. */
enum {
    kAEDescListFactorNone       = 0,
    kAEDescListFactorType       = 4,
    kAEDescListFactorTypeAndSize = 8
};

/* Constants used creating an AppleEvent */
enum {
                                                                /* Constant for the returnID param of AECreateAppleEvent */
    kAutoGenerateReturnID       = -1,                           /* AECreateAppleEvent will generate a session-unique ID */
                                                                /* Constant for transaction ID's */
    kAnyTransactionID           = 0                             /* no transaction is in use */
};

/* Apple event manager data types */
typedef ResType                         DescType;
typedef FourCharCode                    AEKeyword;
#if OPAQUE_TOOLBOX_STRUCTS
typedef struct OpaqueAEDataStorage*     AEDataStorage;
#else
typedef Handle                          AEDataStorage;
#endif  /* OPAQUE_TOOLBOX_STRUCTS */


struct AEDesc {
    DescType                        descriptorType;
    AEDataStorage                   dataHandle;
};
typedef struct AEDesc                   AEDesc;
typedef AEDesc *                        AEDescPtr;

struct AEKeyDesc {
    AEKeyword                       descKey;
    AEDesc                          descContent;
};
typedef struct AEKeyDesc                AEKeyDesc;
/* a list of AEDesc's is a special kind of AEDesc */

typedef AEDesc                          AEDescList;
/* AERecord is a list of keyworded AEDesc's */
typedef AEDescList                      AERecord;
/* an AEDesc which contains address data */
typedef AEDesc                          AEAddressDesc;
/* an AERecord that contains an AppleEvent, and related data types */
typedef AERecord                        AppleEvent;
typedef AppleEvent *                    AppleEventPtr;
typedef SInt16                          AEReturnID;
typedef SInt32                          AETransactionID;
typedef FourCharCode                    AEEventClass;
typedef FourCharCode                    AEEventID;
typedef SInt8                           AEArrayType;
enum {
    kAEDataArray                = 0,
    kAEPackedArray              = 1,
    kAEDescArray                = 3,
    kAEKeyDescArray             = 4
};


enum {
    kAEHandleArray              = 2
};


union AEArrayData {
    short                           kAEDataArray[1];
    char                            kAEPackedArray[1];
    Handle                          kAEHandleArray[1];
    AEDesc                          kAEDescArray[1];
    AEKeyDesc                       kAEKeyDescArray[1];
};
typedef union AEArrayData               AEArrayData;
typedef AEArrayData *                   AEArrayDataPointer;

/**************************************************************************
  These calls are used to set up and modify the coercion dispatch table.
**************************************************************************/
typedef CALLBACK_API( OSErr , AECoerceDescProcPtr )(const AEDesc *fromDesc, DescType toType, long handlerRefcon, AEDesc *toDesc);
typedef CALLBACK_API( OSErr , AECoercePtrProcPtr )(DescType typeCode, const void *dataPtr, Size dataSize, DescType toType, long handlerRefcon, AEDesc *result);
typedef STACK_UPP_TYPE(AECoerceDescProcPtr)                     AECoerceDescUPP;
typedef STACK_UPP_TYPE(AECoercePtrProcPtr)                      AECoercePtrUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(AECoerceDescUPP)
    NewAECoerceDescUPP             (AECoerceDescProcPtr     userRoutine);

    EXTERN_API(AECoercePtrUPP)
    NewAECoercePtrUPP              (AECoercePtrProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeAECoerceDescUPP         (AECoerceDescUPP         userUPP);

    EXTERN_API(void)
    DisposeAECoercePtrUPP          (AECoercePtrUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeAECoerceDescUPP          (const AEDesc *          fromDesc,
                                    DescType                toType,
                                    long                    handlerRefcon,
                                    AEDesc *                toDesc,
                                    AECoerceDescUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeAECoercePtrUPP           (DescType                typeCode,
                                    const void *            dataPtr,
                                    Size                    dataSize,
                                    DescType                toType,
                                    long                    handlerRefcon,
                                    AEDesc *                result,
                                    AECoercePtrUPP          userUPP);

#else
    enum { uppAECoerceDescProcInfo = 0x00003FE0 };                  /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppAECoercePtrProcInfo = 0x0003FFE0 };                   /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewAECoerceDescUPP(userRoutine)                         (AECoerceDescUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAECoerceDescProcInfo, GetCurrentArchitecture())
    #define NewAECoercePtrUPP(userRoutine)                          (AECoercePtrUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAECoercePtrProcInfo, GetCurrentArchitecture())
    #define DisposeAECoerceDescUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeAECoercePtrUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeAECoerceDescUPP(fromDesc, toType, handlerRefcon, toDesc, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppAECoerceDescProcInfo, (fromDesc), (toType), (handlerRefcon), (toDesc))
    #define InvokeAECoercePtrUPP(typeCode, dataPtr, dataSize, toType, handlerRefcon, result, userUPP)  (OSErr)CALL_SIX_PARAMETER_UPP((userUPP), uppAECoercePtrProcInfo, (typeCode), (dataPtr), (dataSize), (toType), (handlerRefcon), (result))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewAECoerceDescProc(userRoutine)                        NewAECoerceDescUPP(userRoutine)
#define NewAECoercePtrProc(userRoutine)                         NewAECoercePtrUPP(userRoutine)
#define CallAECoerceDescProc(userRoutine, fromDesc, toType, handlerRefcon, toDesc) InvokeAECoerceDescUPP(fromDesc, toType, handlerRefcon, toDesc, userRoutine)
#define CallAECoercePtrProc(userRoutine, typeCode, dataPtr, dataSize, toType, handlerRefcon, result) InvokeAECoercePtrUPP(typeCode, dataPtr, dataSize, toType, handlerRefcon, result, userRoutine)

typedef UniversalProcPtr                AECoercionHandlerUPP;
EXTERN_API( OSErr )
AEInstallCoercionHandler        (DescType               fromType,
                                 DescType               toType,
                                 AECoercionHandlerUPP   handler,
                                 long                   handlerRefcon,
                                 Boolean                fromTypeIsDesc,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0A22, 0xA816);

EXTERN_API( OSErr )
AERemoveCoercionHandler         (DescType               fromType,
                                 DescType               toType,
                                 AECoercionHandlerUPP   handler,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0723, 0xA816);

EXTERN_API( OSErr )
AEGetCoercionHandler            (DescType               fromType,
                                 DescType               toType,
                                 AECoercionHandlerUPP * handler,
                                 long *                 handlerRefcon,
                                 Boolean *              fromTypeIsDesc,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0B24, 0xA816);

/**************************************************************************
  The following calls provide for a coercion interface.
**************************************************************************/
EXTERN_API( OSErr )
AECoercePtr                     (DescType               typeCode,
                                 const void *           dataPtr,
                                 Size                   dataSize,
                                 DescType               toType,
                                 AEDesc *               result)                             THREEWORDINLINE(0x303C, 0x0A02, 0xA816);

EXTERN_API( OSErr )
AECoerceDesc                    (const AEDesc *         theAEDesc,
                                 DescType               toType,
                                 AEDesc *               result)                             THREEWORDINLINE(0x303C, 0x0603, 0xA816);


/**************************************************************************
 The following calls apply to any AEDesc. Every 'result' descriptor is
 created for you, so you will be responsible for memory management
 (including disposing) of the descriptors so created.  
**************************************************************************/
EXTERN_API( OSErr )
AECreateDesc                    (DescType               typeCode,
                                 const void *           dataPtr,
                                 Size                   dataSize,
                                 AEDesc *               result)                             THREEWORDINLINE(0x303C, 0x0825, 0xA816);

EXTERN_API( OSErr )
AEDisposeDesc                   (AEDesc *               theAEDesc)                          THREEWORDINLINE(0x303C, 0x0204, 0xA816);

EXTERN_API( OSErr )
AEDuplicateDesc                 (const AEDesc *         theAEDesc,
                                 AEDesc *               result)                             THREEWORDINLINE(0x303C, 0x0405, 0xA816);


/**************************************************************************
  The following calls apply to AEDescList. Since AEDescList is a subtype of
  AEDesc, the calls in the previous section can also be used for AEDescList.
  All list and array indices are 1-based. If the data was greater than
  maximumSize in the routines below, then actualSize will be greater than
  maximumSize, but only maximumSize bytes will actually be retrieved.
**************************************************************************/
EXTERN_API( OSErr )
AECreateList                    (const void *           factoringPtr,
                                 Size                   factoredSize,
                                 Boolean                isRecord,
                                 AEDescList *           resultList)                         THREEWORDINLINE(0x303C, 0x0706, 0xA816);

EXTERN_API( OSErr )
AECountItems                    (const AEDescList *     theAEDescList,
                                 long *                 theCount)                           THREEWORDINLINE(0x303C, 0x0407, 0xA816);

EXTERN_API( OSErr )
AEPutPtr                        (AEDescList *           theAEDescList,
                                 long                   index,
                                 DescType               typeCode,
                                 const void *           dataPtr,
                                 Size                   dataSize)                           THREEWORDINLINE(0x303C, 0x0A08, 0xA816);

EXTERN_API( OSErr )
AEPutDesc                       (AEDescList *           theAEDescList,
                                 long                   index,
                                 const AEDesc *         theAEDesc)                          THREEWORDINLINE(0x303C, 0x0609, 0xA816);

EXTERN_API( OSErr )
AEGetNthPtr                     (const AEDescList *     theAEDescList,
                                 long                   index,
                                 DescType               desiredType,
                                 AEKeyword *            theAEKeyword,
                                 DescType *             typeCode,
                                 void *                 dataPtr,
                                 Size                   maximumSize,
                                 Size *                 actualSize)                         THREEWORDINLINE(0x303C, 0x100A, 0xA816);

EXTERN_API( OSErr )
AEGetNthDesc                    (const AEDescList *     theAEDescList,
                                 long                   index,
                                 DescType               desiredType,
                                 AEKeyword *            theAEKeyword,
                                 AEDesc *               result)                             THREEWORDINLINE(0x303C, 0x0A0B, 0xA816);

EXTERN_API( OSErr )
AESizeOfNthItem                 (const AEDescList *     theAEDescList,
                                 long                   index,
                                 DescType *             typeCode,
                                 Size *                 dataSize)                           THREEWORDINLINE(0x303C, 0x082A, 0xA816);

EXTERN_API( OSErr )
AEGetArray                      (const AEDescList *     theAEDescList,
                                 AEArrayType            arrayType,
                                 AEArrayDataPointer     arrayPtr,
                                 Size                   maximumSize,
                                 DescType *             itemType,
                                 Size *                 itemSize,
                                 long *                 itemCount)                          THREEWORDINLINE(0x303C, 0x0D0C, 0xA816);

EXTERN_API( OSErr )
AEPutArray                      (AEDescList *           theAEDescList,
                                 AEArrayType            arrayType,
                                 const AEArrayData *    arrayPtr,
                                 DescType               itemType,
                                 Size                   itemSize,
                                 long                   itemCount)                          THREEWORDINLINE(0x303C, 0x0B0D, 0xA816);

EXTERN_API( OSErr )
AEDeleteItem                    (AEDescList *           theAEDescList,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x040E, 0xA816);


/**************************************************************************
 The following calls apply to AERecord. Since AERecord is a subtype of
 AEDescList, the calls in the previous sections can also be used for
 AERecord an AERecord can be created by using AECreateList with isRecord
 set to true. 
**************************************************************************/
/*
  Note: The following #defines map "key" calls on AERecords into "param" calls on 
  AppleEvents.  Although no errors are currently returned if AERecords are 
  passed to "param" calls and AppleEvents to "key" calls, the behavior of 
  this type of API-mixing is not explicitly documented in Inside Macintosh.  
  It just happens that the "key" calls have the same functionality as their 
  "param" counterparts.  Since none of the "key" calls are currently available 
  in the PowerPC IntefaceLib, the #defines exploit the fact that "key" and 
  "param" routines can be used interchangeably, and makes sure that every 
  invokation of a "key" API becomes an invokation of a "param" API.
*/
#define AEPutKeyPtr(theAERecord, theAEKeyword, typeCode, dataPtr, dataSize) \
    AEPutParamPtr((theAERecord), (theAEKeyword), (typeCode), (dataPtr), (dataSize))
#define AEPutKeyDesc(theAERecord, theAEKeyword, theAEDesc) \
    AEPutParamDesc((theAERecord), (theAEKeyword), (theAEDesc))
#define AEGetKeyPtr(theAERecord, theAEKeyword, desiredType, typeCode, dataPtr, maxSize, actualSize) \
    AEGetParamPtr((theAERecord), (theAEKeyword), (desiredType), (typeCode), (dataPtr), (maxSize), (actualSize))
#define AEGetKeyDesc(theAERecord, theAEKeyword, desiredType, result) \
    AEGetParamDesc((theAERecord), (theAEKeyword), (desiredType), (result))
#define AESizeOfKeyDesc(theAERecord, theAEKeyword, typeCode, dataSize) \
    AESizeOfParam((theAERecord), (theAEKeyword), (typeCode), (dataSize))
#define AEDeleteKeyDesc(theAERecord, theAEKeyword) \
    AEDeleteParam((theAERecord), (theAEKeyword))
/**************************************************************************
  The following calls create and manipulate the AppleEvent data type.
**************************************************************************/
EXTERN_API( OSErr )
AECreateAppleEvent              (AEEventClass           theAEEventClass,
                                 AEEventID              theAEEventID,
                                 const AEAddressDesc *  target,
                                 AEReturnID             returnID,
                                 AETransactionID        transactionID,
                                 AppleEvent *           result)                             THREEWORDINLINE(0x303C, 0x0B14, 0xA816);


/**************************************************************************
  The following calls are used to pack and unpack parameters from records
  of type AppleEvent. Since AppleEvent is a subtype of AERecord, the calls
  in the previous sections can also be used for variables of type
  AppleEvent. The next six calls are in fact identical to the six calls
  for AERecord.
**************************************************************************/
EXTERN_API( OSErr )
AEPutParamPtr                   (AppleEvent *           theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 DescType               typeCode,
                                 const void *           dataPtr,
                                 Size                   dataSize)                           THREEWORDINLINE(0x303C, 0x0A0F, 0xA816);

EXTERN_API( OSErr )
AEPutParamDesc                  (AppleEvent *           theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 const AEDesc *         theAEDesc)                          THREEWORDINLINE(0x303C, 0x0610, 0xA816);

EXTERN_API( OSErr )
AEGetParamPtr                   (const AppleEvent *     theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 DescType               desiredType,
                                 DescType *             typeCode,
                                 void *                 dataPtr,
                                 Size                   maximumSize,
                                 Size *                 actualSize)                         THREEWORDINLINE(0x303C, 0x0E11, 0xA816);

EXTERN_API( OSErr )
AEGetParamDesc                  (const AppleEvent *     theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 DescType               desiredType,
                                 AEDesc *               result)                             THREEWORDINLINE(0x303C, 0x0812, 0xA816);

EXTERN_API( OSErr )
AESizeOfParam                   (const AppleEvent *     theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 DescType *             typeCode,
                                 Size *                 dataSize)                           THREEWORDINLINE(0x303C, 0x0829, 0xA816);

EXTERN_API( OSErr )
AEDeleteParam                   (AppleEvent *           theAppleEvent,
                                 AEKeyword              theAEKeyword)                       THREEWORDINLINE(0x303C, 0x0413, 0xA816);



/**************************************************************************
 The following calls also apply to type AppleEvent. Message attributes are
 far more restricted, and can only be accessed through the following 5
 calls. The various list and record routines cannot be used to access the
 attributes of an event. 
**************************************************************************/
EXTERN_API( OSErr )
AEGetAttributePtr               (const AppleEvent *     theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 DescType               desiredType,
                                 DescType *             typeCode,
                                 void *                 dataPtr,
                                 Size                   maximumSize,
                                 Size *                 actualSize)                         THREEWORDINLINE(0x303C, 0x0E15, 0xA816);

EXTERN_API( OSErr )
AEGetAttributeDesc              (const AppleEvent *     theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 DescType               desiredType,
                                 AEDesc *               result)                             THREEWORDINLINE(0x303C, 0x0826, 0xA816);

EXTERN_API( OSErr )
AESizeOfAttribute               (const AppleEvent *     theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 DescType *             typeCode,
                                 Size *                 dataSize)                           THREEWORDINLINE(0x303C, 0x0828, 0xA816);

EXTERN_API( OSErr )
AEPutAttributePtr               (AppleEvent *           theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 DescType               typeCode,
                                 const void *           dataPtr,
                                 Size                   dataSize)                           THREEWORDINLINE(0x303C, 0x0A16, 0xA816);

EXTERN_API( OSErr )
AEPutAttributeDesc              (AppleEvent *           theAppleEvent,
                                 AEKeyword              theAEKeyword,
                                 const AEDesc *         theAEDesc)                          THREEWORDINLINE(0x303C, 0x0627, 0xA816);

/**************************************************************************
 The following calls are necessary to deal with opaque data in AEDescs, because the
 traditional way of dealing with a basic AEDesc has been to dereference the dataHandle
 directly.  This is not supported under Carbon.
**************************************************************************/
#if ACCESSOR_CALLS_ARE_FUNCTIONS
/*
        AEGetDescData no longer supports automatic coercion. If you'd like to
        coerce the descriptor use AECoerceDesc.
    */
EXTERN_API( OSErr )
AEGetDescData                   (const AEDesc *         theAEDesc,
                                 void *                 dataPtr,
                                 Size                   maximumSize);

EXTERN_API( Size )
AEGetDescDataSize               (const AEDesc *         theAEDesc);

EXTERN_API( OSErr )
AEReplaceDescData               (DescType               typeCode,
                                 const void *           dataPtr,
                                 Size                   dataSize,
                                 AEDesc *               theAEDesc);

#endif  /* ACCESSOR_CALLS_ARE_FUNCTIONS */


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

#endif /* __AEDATAMODEL__ */

