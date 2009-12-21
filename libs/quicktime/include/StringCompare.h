/*
     File:       StringCompare.h
 
     Contains:   Public interfaces for String Comparison and related operations
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __STRINGCOMPARE__
#define __STRINGCOMPARE__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __TEXTCOMMON__
#include <TextCommon.h>
#endif

#ifndef __SCRIPT__
#include <Script.h>
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

/*

    Here are the current System 7 routine names and the translations to the older forms.
    Please use the newer forms in all new code and migrate the older names out of existing
    code as maintenance permits.
    
    NEW NAME                    OLD NAME                    OBSOLETE FORM (no handle)
    
    CompareString (Str255)      IUCompPString (hp only)     IUCompString (hp only)
    CompareText (ptr/len)       IUMagPString                IUMagString
    IdenticalString (Str255)    IUEqualPString (hp only)    IUEqualString  (hp only)
    IdenticalText (ptr/len)     IUMagIDPString              IUMagIDString
    LanguageOrder               IULangOrder
    ScriptOrder                 IUScriptOrder
    StringOrder (Str255)        IUStringOrder (hp only)
    TextOrder (ptr/len)         IUTextOrder

    RelString
    CmpString (a only)                  
    EqualString (hp only)
    
    ReplaceText

    Carbon only supports the new names.  The old names are undefined for Carbon targets.

    InterfaceLib always has exported the old names.  For C macros have been defined to allow
    the use of the new names.  For Pascal and Assembly using the new names will result
    in link errors. 
    
*/

enum {
                                                                /* Special language code values for Language Order*/
    systemCurLang               = -2,                           /* current (itlbLang) lang for system script*/
    systemDefLang               = -3,                           /* default (table) lang for system script*/
    currentCurLang              = -4,                           /* current (itlbLang) lang for current script*/
    currentDefLang              = -5,                           /* default lang for current script*/
    scriptCurLang               = -6,                           /* current (itlbLang) lang for specified script*/
    scriptDefLang               = -7                            /* default language for a specified script*/
};

/* obsolete names*/
enum {
    iuSystemCurLang             = systemCurLang,
    iuSystemDefLang             = systemDefLang,
    iuCurrentCurLang            = currentCurLang,
    iuCurrentDefLang            = currentDefLang,
    iuScriptCurLang             = scriptCurLang,
    iuScriptDefLang             = scriptDefLang
};


/*
 *  Type Select Utils - now public in Carbon
 */

typedef SInt16 TSCode;
enum {
    tsPreviousSelectMode        = -1,
    tsNormalSelectMode          = 0,
    tsNextSelectMode            = 1
};


struct TypeSelectRecord {
    unsigned long                   tsrLastKeyTime;
    ScriptCode                      tsrScript;
    Str63                           tsrKeyStrokes;
};
typedef struct TypeSelectRecord         TypeSelectRecord;
typedef CALLBACK_API( Boolean , IndexToStringProcPtr )(short item, ScriptCode *itemsScript, StringPtr *itemsStringPtr, void *yourDataPtr);
typedef STACK_UPP_TYPE(IndexToStringProcPtr)                    IndexToStringUPP;
EXTERN_API( void )
TypeSelectClear                 (TypeSelectRecord *     tsr)                                THREEWORDINLINE(0x3F3C, 0x0028, 0xA9ED);

/*
        Long ago the implementation of TypeSelectNewKey had a bug that
        required the high word of D0 to be zero or the function did not work.
        Although fixed now, we are continuing to clear the high word
        just in case someone tries to run on an older system.
    */
EXTERN_API( Boolean )
TypeSelectNewKey                (const EventRecord *    theEvent,
                                 TypeSelectRecord *     tsr)                                FOURWORDINLINE(0x7000, 0x3F3C, 0x002A, 0xA9ED);

EXTERN_API( short )
TypeSelectFindItem              (const TypeSelectRecord * tsr,
                                 short                  listSize,
                                 TSCode                 selectMode,
                                 IndexToStringUPP       getStringProc,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x3F3C, 0x002C, 0xA9ED);


EXTERN_API( short )
TypeSelectCompare               (const TypeSelectRecord * tsr,
                                 ScriptCode             testStringScript,
                                 StringPtr              testStringPtr)                      THREEWORDINLINE(0x3F3C, 0x002E, 0xA9ED);

#if OPAQUE_UPP_TYPES
    EXTERN_API(IndexToStringUPP)
    NewIndexToStringUPP            (IndexToStringProcPtr    userRoutine);

    EXTERN_API(void)
    DisposeIndexToStringUPP        (IndexToStringUPP        userUPP);

    EXTERN_API(Boolean)
    InvokeIndexToStringUPP         (short                   item,
                                    ScriptCode *            itemsScript,
                                    StringPtr *             itemsStringPtr,
                                    void *                  yourDataPtr,
                                    IndexToStringUPP        userUPP);

#else
    enum { uppIndexToStringProcInfo = 0x00003F90 };                 /* pascal 1_byte Func(2_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewIndexToStringUPP(userRoutine)                        (IndexToStringUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIndexToStringProcInfo, GetCurrentArchitecture())
    #define DisposeIndexToStringUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define InvokeIndexToStringUPP(item, itemsScript, itemsStringPtr, yourDataPtr, userUPP)  (Boolean)CALL_FOUR_PARAMETER_UPP((userUPP), uppIndexToStringProcInfo, (item), (itemsScript), (itemsStringPtr), (yourDataPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewIndexToStringProc(userRoutine)                       NewIndexToStringUPP(userRoutine)
#define CallIndexToStringProc(userRoutine, item, itemsScript, itemsStringPtr, yourDataPtr) InvokeIndexToStringUPP(item, itemsScript, itemsStringPtr, yourDataPtr, userRoutine)
/*
 *  These routines are available in Carbon with the new names.
 */
#if TARGET_OS_MAC
    #define MacReplaceText ReplaceText
#endif
EXTERN_API( short )
MacReplaceText                  (Handle                 baseText,
                                 Handle                 substitutionText,
                                 Str15                  key)                                FOURWORDINLINE(0x2F3C, 0x820C, 0xFFDC, 0xA8B5);

EXTERN_API( short )
ScriptOrder                     (ScriptCode             script1,
                                 ScriptCode             script2)                            THREEWORDINLINE(0x3F3C, 0x001E, 0xA9ED);

#if TARGET_OS_MAC
    #define MacCompareString CompareString
#endif
EXTERN_API( short )
MacCompareString                (ConstStr255Param       aStr,
                                 ConstStr255Param       bStr,
                                 Handle                 itl2Handle);

EXTERN_API( short )
IdenticalString                 (ConstStr255Param       aStr,
                                 ConstStr255Param       bStr,
                                 Handle                 itl2Handle);

EXTERN_API( short )
StringOrder                     (ConstStr255Param       aStr,
                                 ConstStr255Param       bStr,
                                 ScriptCode             aScript,
                                 ScriptCode             bScript,
                                 LangCode               aLang,
                                 LangCode               bLang);

EXTERN_API( short )
CompareText                     (const void *           aPtr,
                                 const void *           bPtr,
                                 short                  aLen,
                                 short                  bLen,
                                 Handle                 itl2Handle)                         THREEWORDINLINE(0x3F3C, 0x001A, 0xA9ED);

EXTERN_API( short )
IdenticalText                   (const void *           aPtr,
                                 const void *           bPtr,
                                 short                  aLen,
                                 short                  bLen,
                                 Handle                 itl2Handle)                         THREEWORDINLINE(0x3F3C, 0x001C, 0xA9ED);

EXTERN_API( short )
TextOrder                       (const void *           aPtr,
                                 const void *           bPtr,
                                 short                  aLen,
                                 short                  bLen,
                                 ScriptCode             aScript,
                                 ScriptCode             bScript,
                                 LangCode               aLang,
                                 LangCode               bLang)                              THREEWORDINLINE(0x3F3C, 0x0022, 0xA9ED);

EXTERN_API( short )
LanguageOrder                   (LangCode               language1,
                                 LangCode               language2)                          THREEWORDINLINE(0x3F3C, 0x0020, 0xA9ED);


/*
 *  These routines are available in InterfaceLib with old names.
 *  Macros are provided for C to allow source code use to the new names.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API( short )
IUMagPString                    (const void *           aPtr,
                                 const void *           bPtr,
                                 short                  aLen,
                                 short                  bLen,
                                 Handle                 itl2Handle)                         THREEWORDINLINE(0x3F3C, 0x001A, 0xA9ED);

EXTERN_API( short )
IUMagIDPString                  (const void *           aPtr,
                                 const void *           bPtr,
                                 short                  aLen,
                                 short                  bLen,
                                 Handle                 itl2Handle)                         THREEWORDINLINE(0x3F3C, 0x001C, 0xA9ED);

EXTERN_API( short )
IUTextOrder                     (const void *           aPtr,
                                 const void *           bPtr,
                                 short                  aLen,
                                 short                  bLen,
                                 ScriptCode             aScript,
                                 ScriptCode             bScript,
                                 LangCode               aLang,
                                 LangCode               bLang)                              THREEWORDINLINE(0x3F3C, 0x0022, 0xA9ED);

EXTERN_API( short )
IULangOrder                     (LangCode               language1,
                                 LangCode               language2)                          THREEWORDINLINE(0x3F3C, 0x0020, 0xA9ED);

EXTERN_API( short )
IUScriptOrder                   (ScriptCode             script1,
                                 ScriptCode             script2)                            THREEWORDINLINE(0x3F3C, 0x001E, 0xA9ED);

EXTERN_API( short )
IUMagString                     (const void *           aPtr,
                                 const void *           bPtr,
                                 short                  aLen,
                                 short                  bLen)                               THREEWORDINLINE(0x3F3C, 0x000A, 0xA9ED);

EXTERN_API( short )
IUMagIDString                   (const void *           aPtr,
                                 const void *           bPtr,
                                 short                  aLen,
                                 short                  bLen)                               THREEWORDINLINE(0x3F3C, 0x000C, 0xA9ED);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API( short )
IUCompPString                   (ConstStr255Param       aStr,
                                 ConstStr255Param       bStr,
                                 Handle                 itl2Handle);

EXTERN_API( short )
IUEqualPString                  (ConstStr255Param       aStr,
                                 ConstStr255Param       bStr,
                                 Handle                 itl2Handle);

EXTERN_API( short )
IUStringOrder                   (ConstStr255Param       aStr,
                                 ConstStr255Param       bStr,
                                 ScriptCode             aScript,
                                 ScriptCode             bScript,
                                 LangCode               aLang,
                                 LangCode               bLang);

EXTERN_API( short )
IUCompString                    (ConstStr255Param       aStr,
                                 ConstStr255Param       bStr);

EXTERN_API( short )
IUEqualString                   (ConstStr255Param       aStr,
                                 ConstStr255Param       bStr);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if TARGET_OS_MAC
    #define CompareString(aStr, bStr, itl2Handle) \
             IUCompPString(aStr, bStr, itl2Handle)
#endif
#ifdef MacCompareString
#undef MacCompareString
#endif
#define MacCompareString(aStr, bStr, itl2Handle) \
         IUCompPString(aStr, bStr, itl2Handle)
#define CompareText(aPtr, bPtr, aLen, bLen, itl2Handle) \
         IUMagPString(aPtr, bPtr, aLen, bLen, itl2Handle)
#define IdenticalString(aStr, bStr, itl2Handle) \
         IUEqualPString(aStr, bStr, itl2Handle)
#define IdenticalText(aPtr, bPtr, aLen, bLen, itl2Handle) \
         IUMagIDPString(aPtr, bPtr, aLen, bLen, itl2Handle)
#define StringOrder(aStr, bStr, aScript, bScript, aLang, bLang) \
         IUStringOrder(aStr, bStr, aScript, bScript, aLang, bLang)
#define TextOrder(aPtr, bPtr, aLen, bLen, aScript, bScript, aLang, bLang) \
         IUTextOrder(aPtr, bPtr, aLen, bLen, aScript, bScript, aLang, bLang)
#define LanguageOrder(language1, language2) \
         IULangOrder(language1, language2)
#endif /* CALL_NOT_IN_CARBON */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( short )
iucomppstring                   (const char *           aStr,
                                 const char *           bStr,
                                 Handle                 intlHandle);

EXTERN_API_C( short )
iuequalpstring                  (const char *           aStr,
                                 const char *           bStr,
                                 Handle                 intlHandle);

EXTERN_API_C( short )
iustringorder                   (const char *           aStr,
                                 const char *           bStr,
                                 ScriptCode             aScript,
                                 ScriptCode             bScript,
                                 LangCode               aLang,
                                 LangCode               bLang);

EXTERN_API_C( short )
iucompstring                    (const char *           aStr,
                                 const char *           bStr);

EXTERN_API_C( short )
iuequalstring                   (const char *           aStr,
                                 const char *           bStr);

#endif  /* CALL_NOT_IN_CARBON */


EXTERN_API( short )
RelString                       (ConstStr255Param       str1,
                                 ConstStr255Param       str2,
                                 Boolean                caseSensitive,
                                 Boolean                diacSensitive);

EXTERN_API( Boolean )
EqualString                     (ConstStr255Param       str1,
                                 ConstStr255Param       str2,
                                 Boolean                caseSensitive,
                                 Boolean                diacSensitive);

EXTERN_API_C( short )
relstring                       (const char *           str1,
                                 const char *           str2,
                                 Boolean                caseSensitive,
                                 Boolean                diacSensitive);

#if CALL_NOT_IN_CARBON
EXTERN_API_C( Boolean )
equalstring                     (const char *           str1,
                                 const char *           str2,
                                 Boolean                caseSensitive,
                                 Boolean                diacSensitive);

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

#endif /* __STRINGCOMPARE__ */

