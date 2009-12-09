/*
     File:       UnicodeUtilities.h
 
     Contains:   Types, constants, prototypes for Unicode Utilities (Unicode input and text utils)
 
     Version:    Technology: Mac OS 9.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1997-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __UNICODEUTILITIES__
#define __UNICODEUTILITIES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif


#ifndef __MACLOCALES__
#include <MacLocales.h>
#endif

#ifndef __TEXTCOMMON__
#include <TextCommon.h>
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
   -------------------------------------------------------------------------------------------------
   CONSTANTS & DATA STRUCTURES for UCKeyTranslate & UCKeyboardLayout ('uchr' resource)
   -------------------------------------------------------------------------------------------------
*/

/*
   -------------------------------------------------------------------------------------------------
   UCKeyOutput & related stuff
   The interpretation of UCKeyOutput depends on bits 15-14.
   If they are 01, then bits 0-13 are an index in UCKeyStateRecordsIndex (resource-wide list).
   If they are 10, then bits 0-13 are an index in UCKeySequenceDataIndex (resource-wide list),
     or if UCKeySequenceDataIndex is not present or the index is beyond the end of the list,
     then bits 0-15 are a single Unicode character.
   Otherwise, bits 0-15 are a single Unicode character; a value of 0xFFFE-0xFFFF means no character
     output.
   UCKeyCharSeq is similar, but does not support indices in UCKeyStateRecordsIndex. For bits 15-14:
   If they are 10, then bits 0-13 are an index in UCKeySequenceDataIndex (resource-wide list),
     or if UCKeySequenceDataIndex is not present or the index is beyond the end of the list,
     then bits 0-15 are a single Unicode character.
   Otherwise, bits 0-15 are a single Unicode character; a value of 0xFFFE-0xFFFF means no character
     output.
   -------------------------------------------------------------------------------------------------
*/

typedef UInt16                          UCKeyOutput;
typedef UInt16                          UCKeyCharSeq;
enum {
    kUCKeyOutputStateIndexMask  = 0x4000,
    kUCKeyOutputSequenceIndexMask = 0x8000,
    kUCKeyOutputTestForIndexMask = 0xC000,                      /* test bits 14-15*/
    kUCKeyOutputGetIndexMask    = 0x3FFF                        /* get bits 0-13*/
};

/*
   -------------------------------------------------------------------------------------------------
   UCKeyStateRecord & related stuff
   The UCKeyStateRecord information is used as follows. If the current state is zero,
   output stateZeroCharData and set the state to stateZeroNextState. If the current state
   is non-zero and there is an entry for it in stateEntryData, then output the corresponding
   charData and set the state to nextState. Otherwise, output the state terminator from
   UCKeyStateTerminators for the current state (or nothing if there is no UCKeyStateTerminators
   table or it has no entry for the current state), then output stateZeroCharData and set the
   state to stateZeroNextState.
   -------------------------------------------------------------------------------------------------
*/


struct UCKeyStateRecord {
    UCKeyCharSeq                    stateZeroCharData;
    UInt16                          stateZeroNextState;
    UInt16                          stateEntryCount;
    UInt16                          stateEntryFormat;
                                                                /* This is followed by an array of stateEntryCount elements*/
                                                                /* in the specified format. Here we just show a dummy array.*/
    UInt32                          stateEntryData[1];
};
typedef struct UCKeyStateRecord         UCKeyStateRecord;
/*
   Here are the codes for entry formats currently defined.
   Each entry maps from curState to charData and nextState.
*/
enum {
    kUCKeyStateEntryTerminalFormat = 0x0001,
    kUCKeyStateEntryRangeFormat = 0x0002
};

/*
   For UCKeyStateEntryTerminal -
   nextState is always 0, so we don't have a field for it
*/


struct UCKeyStateEntryTerminal {
    UInt16                          curState;
    UCKeyCharSeq                    charData;
};
typedef struct UCKeyStateEntryTerminal  UCKeyStateEntryTerminal;
/*
   For UCKeyStateEntryRange -
   If curState >= curStateStart and curState <= curStateStart+curStateRange,
   then it matches the entry, and we transform charData and nextState as follows:
   If charData < 0xFFFE, then charData += (curState-curStateStart)*deltaMultiplier
   If nextState != 0, then nextState += (curState-curStateStart)*deltaMultiplier
*/

struct UCKeyStateEntryRange {
    UInt16                          curStateStart;
    UInt8                           curStateRange;
    UInt8                           deltaMultiplier;
    UCKeyCharSeq                    charData;
    UInt16                          nextState;
};
typedef struct UCKeyStateEntryRange     UCKeyStateEntryRange;
/*
   -------------------------------------------------------------------------------------------------
   UCKeyboardLayout & related stuff
   The UCKeyboardLayout struct given here is only for the resource header. It specifies
   offsets to the various subtables which each have their own structs, given below.
   The keyboardTypeHeadList array selects table offsets that depend on keyboardType. The
   first entry in keyboardTypeHeadList is the default entry, which will be used if the
   keyboardType passed to UCKeyTranslate does not match any other entry - i.e. does not fall
   within the range keyboardTypeFirst..keyboardTypeLast for some entry. The first entry
   should have keyboardTypeFirst = keyboardTypeLast = 0.
   -------------------------------------------------------------------------------------------------
*/

struct UCKeyboardTypeHeader {
    UInt32                          keyboardTypeFirst;          /* first keyboardType in this entry*/
    UInt32                          keyboardTypeLast;           /* last keyboardType in this entry*/
    ByteOffset                      keyModifiersToTableNumOffset; /* required*/
    ByteOffset                      keyToCharTableIndexOffset;  /* required*/
    ByteOffset                      keyStateRecordsIndexOffset; /* 0 => no table*/
    ByteOffset                      keyStateTerminatorsOffset;  /* 0 => no table*/
    ByteOffset                      keySequenceDataIndexOffset; /* 0 => no table*/
};
typedef struct UCKeyboardTypeHeader     UCKeyboardTypeHeader;

struct UCKeyboardLayout {
                                                                /* header only; other tables accessed via offsets*/
    UInt16                          keyLayoutHeaderFormat;      /* =kUCKeyLayoutHeaderFormat*/
    UInt16                          keyLayoutDataVersion;       /* 0x0100 = 1.0, 0x0110 = 1.1, etc.*/
    ByteOffset                      keyLayoutFeatureInfoOffset; /* may be 0                       */
    ItemCount                       keyboardTypeCount;          /* Dimension for keyboardTypeHeadList[]     */
    UCKeyboardTypeHeader            keyboardTypeList[1];
};
typedef struct UCKeyboardLayout         UCKeyboardLayout;
/* -------------------------------------------------------------------------------------------------*/

struct UCKeyLayoutFeatureInfo {
    UInt16                          keyLayoutFeatureInfoFormat; /* =kUCKeyLayoutFeatureInfoFormat*/
    UInt16                          reserved;
    UniCharCount                    maxOutputStringLength;      /* longest possible output string*/
};
typedef struct UCKeyLayoutFeatureInfo   UCKeyLayoutFeatureInfo;
/* -------------------------------------------------------------------------------------------------*/

struct UCKeyModifiersToTableNum {
    UInt16                          keyModifiersToTableNumFormat; /* =kUCKeyModifiersToTableNumFormat*/
    UInt16                          defaultTableNum;            /* For modifier combos not in tableNum[]*/
    ItemCount                       modifiersCount;             /* Dimension for tableNum[]*/
    UInt8                           tableNum[1];

                                                                /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeyModifiersToTableNum UCKeyModifiersToTableNum;
/* -------------------------------------------------------------------------------------------------*/

struct UCKeyToCharTableIndex {
    UInt16                          keyToCharTableIndexFormat;  /* =kUCKeyToCharTableIndexFormat*/
    UInt16                          keyToCharTableSize;         /* Max keyCode (128 for ADB keyboards)*/
    ItemCount                       keyToCharTableCount;        /* Dimension for keyToCharTableOffsets[] (usually 6 to 12 tables)*/
    ByteOffset                      keyToCharTableOffsets[1];

                                                                /* Each offset in keyToCharTableOffsets is from the beginning of the resource to a*/
                                                                /* table as follows:*/
                                                                /*    UCKeyOutput       keyToCharData[keyToCharTableSize];*/
                                                                /* These tables follow the UCKeyToCharTableIndex.*/
                                                                /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeyToCharTableIndex    UCKeyToCharTableIndex;
/* -------------------------------------------------------------------------------------------------*/

struct UCKeyStateRecordsIndex {
    UInt16                          keyStateRecordsIndexFormat; /* =kUCKeyStateRecordsIndexFormat*/
    UInt16                          keyStateRecordCount;        /* Dimension for keyStateRecordOffsets[]*/
    ByteOffset                      keyStateRecordOffsets[1];

                                                                /* Each offset in keyStateRecordOffsets is from the beginning of the resource to a*/
                                                                /* UCKeyStateRecord. These UCKeyStateRecords follow the keyStateRecordOffsets[] array.*/
                                                                /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeyStateRecordsIndex   UCKeyStateRecordsIndex;
/* -------------------------------------------------------------------------------------------------*/

struct UCKeyStateTerminators {
    UInt16                          keyStateTerminatorsFormat;  /* =kUCKeyStateTerminatorsFormat*/
    UInt16                          keyStateTerminatorCount;    /* Dimension for keyStateTerminators[] (# of nonzero states)*/
    UCKeyCharSeq                    keyStateTerminators[1];

                                                                /* Note: keyStateTerminators[0] is terminator for state 1, etc.*/
                                                                /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeyStateTerminators    UCKeyStateTerminators;
/* -------------------------------------------------------------------------------------------------*/

struct UCKeySequenceDataIndex {
    UInt16                          keySequenceDataIndexFormat; /* =kUCKeySequenceDataIndexFormat*/
    UInt16                          charSequenceCount;          /* Dimension of charSequenceOffsets[] is charSequenceCount+1*/
    UInt16                          charSequenceOffsets[1];

                                                                /* Each offset in charSequenceOffsets is in bytes, from the beginning of*/
                                                                /* UCKeySequenceDataIndex to a sequence of UniChars; the next offset indicates the*/
                                                                /* end of the sequence. The UniChar sequences follow the UCKeySequenceDataIndex.*/
                                                                /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeySequenceDataIndex   UCKeySequenceDataIndex;
/* -------------------------------------------------------------------------------------------------*/
/* Current format codes for the various tables (bits 12-15 indicate which table)*/

enum {
    kUCKeyLayoutHeaderFormat    = 0x1002,
    kUCKeyLayoutFeatureInfoFormat = 0x2001,
    kUCKeyModifiersToTableNumFormat = 0x3001,
    kUCKeyToCharTableIndexFormat = 0x4001,
    kUCKeyStateRecordsIndexFormat = 0x5001,
    kUCKeyStateTerminatorsFormat = 0x6001,
    kUCKeySequenceDataIndexFormat = 0x7001
};


/*
   -------------------------------------------------------------------------------------------------
   Constants for keyAction parameter in UCKeyTranslate() 
   -------------------------------------------------------------------------------------------------
*/

enum {
    kUCKeyActionDown            = 0,                            /* key is going down*/
    kUCKeyActionUp              = 1,                            /* key is going up*/
    kUCKeyActionAutoKey         = 2,                            /* auto-key down*/
    kUCKeyActionDisplay         = 3                             /* get information for key display (as in Key Caps)      */
};

/*
   -------------------------------------------------------------------------------------------------
   Bit assignments & masks for keyTranslateOptions parameter in UCKeyTranslate() 
   -------------------------------------------------------------------------------------------------
*/

enum {
    kUCKeyTranslateNoDeadKeysBit = 0                            /* Prevents setting any new dead-key states*/
};

enum {
    kUCKeyTranslateNoDeadKeysMask = 1L << kUCKeyTranslateNoDeadKeysBit
};

/*
   -------------------------------------------------------------------------------------------------
   CONSTANTS & DATA STRUCTURES for Unicode Collation
   -------------------------------------------------------------------------------------------------
*/
/* constant for LocaleOperationClass*/
enum {
    kUnicodeCollationClass      = FOUR_CHAR_CODE('ucol')
};

typedef struct OpaqueCollatorRef*       CollatorRef;

typedef UInt32 UCCollateOptions;
enum {
                                                                /* Sensitivity options*/
    kUCCollateComposeInsensitiveMask = 1L << 1,
    kUCCollateWidthInsensitiveMask = 1L << 2,
    kUCCollateCaseInsensitiveMask = 1L << 3,
    kUCCollateDiacritInsensitiveMask = 1L << 4,                 /* Other general options */
    kUCCollatePunctuationSignificantMask = 1L << 15,            /* Number-handling options */
    kUCCollateDigitsOverrideMask = 1L << 16,
    kUCCollateDigitsAsNumberMask = 1L << 17
};

enum {
    kUCCollateStandardOptions   = kUCCollateComposeInsensitiveMask | kUCCollateWidthInsensitiveMask
};

/*
   Special values to specify various invariant orders for UCCompareTextNoLocale.
   These values use the high 8 bits of UCCollateOptions.
*/
enum {
    kUCCollateTypeHFSExtended   = 1
};

/* These constants are used for masking and shifting the invariant order type.*/
enum {
    kUCCollateTypeSourceMask    = 0x000000FF,
    kUCCollateTypeShiftBits     = 24
};

enum {
    kUCCollateTypeMask          = kUCCollateTypeSourceMask << kUCCollateTypeShiftBits
};


typedef UInt32                          UCCollationValue;
/*
   -------------------------------------------------------------------------------------------------
   CONSTANTS & DATA STRUCTURES for Unicode TextBreak
   -------------------------------------------------------------------------------------------------
*/
/* constant for LocaleOperationClass*/
enum {
    kUnicodeTextBreakClass      = FOUR_CHAR_CODE('ubrk')
};

typedef struct OpaqueTextBreakLocatorRef*  TextBreakLocatorRef;

typedef UInt32 UCTextBreakType;
enum {
    kUCTextBreakCharMask        = 1L << 0,
    kUCTextBreakClusterMask     = 1L << 2,
    kUCTextBreakWordMask        = 1L << 4,
    kUCTextBreakLineMask        = 1L << 6
};


typedef UInt32 UCTextBreakOptions;
enum {
    kUCTextBreakLeadingEdgeMask = 1L << 0,
    kUCTextBreakGoBackwardsMask = 1L << 1,
    kUCTextBreakIterateMask     = 1L << 2
};

/*
   -------------------------------------------------------------------------------------------------
   FUNCTION PROTOTYPES
   -------------------------------------------------------------------------------------------------
*/

EXTERN_API( OSStatus )
UCKeyTranslate                  (const UCKeyboardLayout * keyLayoutPtr,
                                 UInt16                 virtualKeyCode,
                                 UInt16                 keyAction,
                                 UInt32                 modifierKeyState,
                                 UInt32                 keyboardType,
                                 OptionBits             keyTranslateOptions,
                                 UInt32 *               deadKeyState,
                                 UniCharCount           maxStringLength,
                                 UniCharCount *         actualStringLength,
                                 UniChar                unicodeString[]);

/* Standard collation functions*/
EXTERN_API_C( OSStatus )
UCCreateCollator                (LocaleRef              locale,
                                 LocaleOperationVariant  opVariant,
                                 UCCollateOptions       options,
                                 CollatorRef *          collatorRef);

EXTERN_API_C( OSStatus )
UCGetCollationKey               (CollatorRef            collatorRef,
                                 const UniChar *        textPtr,
                                 UniCharCount           textLength,
                                 ItemCount              maxKeySize,
                                 ItemCount *            actualKeySize,
                                 UCCollationValue       collationKey[]);

EXTERN_API_C( OSStatus )
UCCompareCollationKeys          (const UCCollationValue * key1Ptr,
                                 ItemCount              key1Length,
                                 const UCCollationValue * key2Ptr,
                                 ItemCount              key2Length,
                                 Boolean *              equivalent,
                                 SInt32 *               order);

EXTERN_API_C( OSStatus )
UCCompareText                   (CollatorRef            collatorRef,
                                 const UniChar *        text1Ptr,
                                 UniCharCount           text1Length,
                                 const UniChar *        text2Ptr,
                                 UniCharCount           text2Length,
                                 Boolean *              equivalent,
                                 SInt32 *               order);

EXTERN_API_C( OSStatus )
UCDisposeCollator               (CollatorRef *          collatorRef);

/* Simple collation using default locale*/

EXTERN_API_C( OSStatus )
UCCompareTextDefault            (UCCollateOptions       options,
                                 const UniChar *        text1Ptr,
                                 UniCharCount           text1Length,
                                 const UniChar *        text2Ptr,
                                 UniCharCount           text2Length,
                                 Boolean *              equivalent,
                                 SInt32 *               order);


/* Simple locale-independent collation*/

EXTERN_API_C( OSStatus )
UCCompareTextNoLocale           (UCCollateOptions       options,
                                 const UniChar *        text1Ptr,
                                 UniCharCount           text1Length,
                                 const UniChar *        text2Ptr,
                                 UniCharCount           text2Length,
                                 Boolean *              equivalent,
                                 SInt32 *               order);

/* Standard text break (text boundary) functions*/
EXTERN_API_C( OSStatus )
UCCreateTextBreakLocator        (LocaleRef              locale,
                                 LocaleOperationVariant  opVariant,
                                 UCTextBreakType        breakTypes,
                                 TextBreakLocatorRef *  breakRef);

EXTERN_API_C( OSStatus )
UCFindTextBreak                 (TextBreakLocatorRef    breakRef,
                                 UCTextBreakType        breakType,
                                 UCTextBreakOptions     options,
                                 const UniChar *        textPtr,
                                 UniCharCount           textLength,
                                 UniCharArrayOffset     startOffset,
                                 UniCharArrayOffset *   breakOffset);

EXTERN_API_C( OSStatus )
UCDisposeTextBreakLocator       (TextBreakLocatorRef *  breakRef);


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

#endif /* __UNICODEUTILITIES__ */

