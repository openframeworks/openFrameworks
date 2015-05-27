/*
     File:       LanguageAnalysis.h
 
     Contains:   Language Analysis Manager Interfaces
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1996-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __LANGUAGEANALYSIS__
#define __LANGUAGEANALYSIS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __AEREGISTRY__
#include <AERegistry.h>
#endif

#ifndef __DICTIONARY__
#include <Dictionary.h>
#endif

#ifndef __TEXTCOMMON__
#include <TextCommon.h>
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
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

typedef struct OpaqueLAEnvironmentRef*  LAEnvironmentRef;
typedef struct OpaqueLAContextRef*      LAContextRef;
typedef AEKeyword                       LAPropertyKey;
typedef DescType                        LAPropertyType;
/*
    Data structure for high level API
*/

struct LAMorphemeRec {
    ByteCount                       sourceTextLength;
    LogicalAddress                  sourceTextPtr;
    ByteCount                       morphemeTextLength;
    LogicalAddress                  morphemeTextPtr;
    UInt32                          partOfSpeech;
};
typedef struct LAMorphemeRec            LAMorphemeRec;

struct LAMorphemesArray {
    ItemCount                       morphemesCount;
    ByteCount                       processedTextLength;
    ByteCount                       morphemesTextLength;
    LAMorphemeRec                   morphemes[1];
};
typedef struct LAMorphemesArray         LAMorphemesArray;
typedef LAMorphemesArray *              LAMorphemesArrayPtr;
enum {
    kLAMorphemesArrayVersion    = 0
};

/*
    Definitions for result path/bundle structure
*/

typedef AERecord                        LAMorphemeBundle;
typedef AERecord                        LAMorphemePath;
typedef AERecord                        LAMorpheme;
typedef AERecord                        LAHomograph;
enum {
    keyAELAMorphemeBundle       = FOUR_CHAR_CODE('lmfb'),
    keyAELAMorphemePath         = FOUR_CHAR_CODE('lmfp'),
    keyAELAMorpheme             = FOUR_CHAR_CODE('lmfn'),
    keyAELAHomograph            = FOUR_CHAR_CODE('lmfh')
};

enum {
    typeLAMorphemeBundle        = typeAERecord,
    typeLAMorphemePath          = typeAERecord,
    typeLAMorpheme              = typeAEList,
    typeLAHomograph             = typeAEList
};

/*
    Definitions for morpheme/homograph information
*/
enum {
    keyAEMorphemePartOfSpeechCode = FOUR_CHAR_CODE('lamc'),
    keyAEMorphemeTextRange      = FOUR_CHAR_CODE('lamt')
};

enum {
    typeAEMorphemePartOfSpeechCode = FOUR_CHAR_CODE('lamc'),
    typeAEMorphemeTextRange     = FOUR_CHAR_CODE('lamt')
};

typedef UInt32                          MorphemePartOfSpeech;

struct MorphemeTextRange {
    UInt32                          sourceOffset;
    UInt32                          length;
};
typedef struct MorphemeTextRange        MorphemeTextRange;
/*
    Mask for High level API convert flags 
*/
enum {
    kLAEndOfSourceTextMask      = 0x00000001
};

/*
    Constants for leading/trailing path of analysis function
*/
enum {
    kLADefaultEdge              = 0,
    kLAFreeEdge                 = 1,
    kLAIncompleteEdge           = 2
};

/*
    Constants for confirm and shift function
*/
enum {
    kLAAllMorphemes             = 0
};


/*
    Library version
*/
EXTERN_API( UInt32 )
LALibraryVersion                (void);

/*
    High level API
*/
EXTERN_API( OSStatus )
LATextToMorphemes               (LAContextRef           context,
                                 TextEncoding           preferedEncoding,
                                 ByteCount              textLength,
                                 ConstLogicalAddress    sourceText,
                                 ByteCount              bufferSize,
                                 OptionBits             convertFlags,
                                 UInt32                 structureVersion,
                                 ByteCount *            acceptedLength,
                                 LAMorphemesArrayPtr    resultBuffer);

/*
    Handling Context
*/
EXTERN_API( OSStatus )
LAOpenAnalysisContext           (LAEnvironmentRef       environ,
                                 LAContextRef *         context);

EXTERN_API( OSStatus )
LACloseAnalysisContext          (LAContextRef           context);

/*
    Handling Environment
*/
EXTERN_API( OSStatus )
LAGetEnvironmentList            (UInt32                 maxCount,
                                 UInt32 *               actualCount,
                                 LAEnvironmentRef       environmentList[]);

EXTERN_API( OSStatus )
LAGetEnvironmentName            (LAEnvironmentRef       environment,
                                 Str63                  environmentName);

EXTERN_API( OSStatus )
LAGetEnvironmentRef             (ConstStr63Param        targetEnvironmentName,
                                 LAEnvironmentRef *     environment);

EXTERN_API( OSStatus )
LACreateCustomEnvironment       (LAEnvironmentRef       baseEnvironment,
                                 ConstStr63Param        newEnvironmentName,
                                 Boolean                persistent,
                                 LAEnvironmentRef *     newEnvironment);

EXTERN_API( OSStatus )
LADeleteCustomEnvironment       (LAEnvironmentRef       environment);

/*
    Handling dictionries
*/
EXTERN_API( OSStatus )
LAOpenDictionary                (LAEnvironmentRef       environ,
                                 const FSSpec *         dictionary);

EXTERN_API( OSStatus )
LACloseDictionary               (LAEnvironmentRef       environ,
                                 const FSSpec *         dictionary);

EXTERN_API( OSStatus )
LAListAvailableDictionaries     (LAEnvironmentRef       environ,
                                 ItemCount              maxCount,
                                 ItemCount *            actualCount,
                                 FSSpec                 dictionaryList[],
                                 Boolean                opened[]);

EXTERN_API( OSStatus )
LAAddNewWord                    (LAEnvironmentRef       environ,
                                 const FSSpec *         dictionary,
                                 const AEDesc *         dataList);

/*
    Analyzing text
*/
EXTERN_API( OSStatus )
LAMorphemeAnalysis              (LAContextRef           context,
                                 ConstUniCharArrayPtr   text,
                                 UniCharCount           textLength,
                                 LAMorphemePath *       leadingPath,
                                 LAMorphemePath *       trailingPath,
                                 ItemCount              pathCount,
                                 LAMorphemeBundle *     result);

EXTERN_API( OSStatus )
LAContinuousMorphemeAnalysis    (LAContextRef           context,
                                 ConstUniCharArrayPtr   text,
                                 UniCharCount           textLength,
                                 Boolean                incrementalText,
                                 LAMorphemePath *       leadingPath,
                                 LAMorphemePath *       trailingPath,
                                 Boolean *              modified);

EXTERN_API( OSStatus )
LAGetMorphemes                  (LAContextRef           context,
                                 LAMorphemePath *       result);

EXTERN_API( OSStatus )
LAShiftMorphemes                (LAContextRef           context,
                                 ItemCount              morphemeCount,
                                 LAMorphemePath *       path,
                                 UniCharCount *         shiftedLength);

EXTERN_API( OSStatus )
LAResetAnalysis                 (LAContextRef           context);

/*
    Check Language Analysis Manager availability
*/
#if TARGET_RT_MAC_CFM
#ifdef __cplusplus
    inline pascal Boolean LALanguageAnalysisAvailable() { return (LALibraryVersion != (void*)kUnresolvedCFragSymbolAddress); }
#else
    #define LALanguageAnalysisAvailable()   ((LALibraryVersion != (void*)kUnresolvedCFragSymbolAddress)
#endif
#elif TARGET_RT_MAC_MACHO
/* Language Analysis Manager is always available on OS X */
#ifdef __cplusplus
    inline pascal Boolean LALanguageAnalysisAvailable() { return true; }
#else
    #define LALanguageAnalysisAvailable()   (true)
#endif
#endif  /*  */

/*
=============================================================================================
    Definitions for Japanese Analysis Module
=============================================================================================
*/
/*
    Names for default environments for Japanese analysis
*/
#define kLAJapaneseKanaKanjiEnvironment             "\pKanaKanjiConversion"
#define kLAJapaneseMorphemeAnalysisEnvironment      "\pJapaneseMorphemeAnalysis"
#define kLAJapaneseTTSEnvironment                   "\pJapaneseTextToSpeech"
/*
    File cretor for dictionary of Apple Japanese access method
*/
enum {
    kAppleJapaneseDictionarySignature = FOUR_CHAR_CODE('jlan')
};

/*
    Engine limitations
*/
enum {
    kMaxInputLengthOfAppleJapaneseEngine = 200
};

/*
    Definitions of information in the path/bundle
*/

typedef MorphemePartOfSpeech            JapanesePartOfSpeech;
typedef UInt16                          HomographWeight;
typedef UInt8                           HomographAccent;
/*
    AE keywords and type definitions for morpheme/homograph information
*/
enum {
    keyAEHomographDicInfo       = FOUR_CHAR_CODE('lahd'),
    keyAEHomographWeight        = FOUR_CHAR_CODE('lahw'),
    keyAEHomographAccent        = FOUR_CHAR_CODE('laha')
};

enum {
    typeAEHomographDicInfo      = FOUR_CHAR_CODE('lahd'),
    typeAEHomographWeight       = typeShortInteger,
    typeAEHomographAccent       = FOUR_CHAR_CODE('laha')
};

/*
    Structure for dictionary information of homograph
*/

struct HomographDicInfoRec {
    DCMDictionaryID                 dictionaryID;
    DCMUniqueID                     uniqueID;
};
typedef struct HomographDicInfoRec      HomographDicInfoRec;
/*
=============================================================================================
    Definitions for Japanese part of speeches
=============================================================================================
*/
/*
    Masks for part of speeches
*/
enum {
    kLASpeechRoughClassMask     = 0x0000F000,
    kLASpeechMediumClassMask    = 0x0000FF00,
    kLASpeechStrictClassMask    = 0x0000FFF0,
    kLASpeechKatsuyouMask       = 0x0000000F
};


/*
    Part of speeches
*/
enum {
    kLASpeechMeishi             = 0x00000000,                   /* noun */
    kLASpeechFutsuuMeishi       = 0x00000000,                   /* general noun */
    kLASpeechJinmei             = 0x00000100,                   /* person name */
    kLASpeechJinmeiSei          = 0x00000110,                   /* family name */
    kLASpeechJinmeiMei          = 0x00000120,                   /* first name */
    kLASpeechChimei             = 0x00000200,                   /* place name */
    kLASpeechSetsubiChimei      = 0x00000210,                   /* place name with suffix */
    kLASpeechSoshikimei         = 0x00000300,                   /* organization name */
    kLASpeechKoyuuMeishi        = 0x00000400,                   /* proper noun */
    kLASpeechSahenMeishi        = 0x00000500,                   /* special noun */
    kLASpeechKeidouMeishi       = 0x00000600,                   /* special noun */
    kLASpeechRentaishi          = 0x00001000,
    kLASpeechFukushi            = 0x00002000,                   /* adverb */
    kLASpeechSetsuzokushi       = 0x00003000,                   /* conjunction */
    kLASpeechKandoushi          = 0x00004000,
    kLASpeechDoushi             = 0x00005000,                   /* verb */
    kLASpeechGodanDoushi        = 0x00005000,
    kLASpeechKagyouGodan        = 0x00005000,
    kLASpeechSagyouGodan        = 0x00005010,
    kLASpeechTagyouGodan        = 0x00005020,
    kLASpeechNagyouGodan        = 0x00005030,
    kLASpeechMagyouGodan        = 0x00005040,
    kLASpeechRagyouGodan        = 0x00005050,
    kLASpeechWagyouGodan        = 0x00005060,
    kLASpeechGagyouGodan        = 0x00005070,
    kLASpeechBagyouGodan        = 0x00005080,
    kLASpeechIchidanDoushi      = 0x00005100,
    kLASpeechKahenDoushi        = 0x00005200,
    kLASpeechSahenDoushi        = 0x00005300,
    kLASpeechZahenDoushi        = 0x00005400,
    kLASpeechKeiyoushi          = 0x00006000,                   /* adjective */
    kLASpeechKeiyoudoushi       = 0x00007000,
    kLASpeechSettougo           = 0x00008000,                   /* prefix*/
    kLASpeechSuujiSettougo      = 0x00008100,                   /* prefix for numbers */
    kLASpeechSetsubigo          = 0x00009000,                   /* suffix */
    kLASpeechJinmeiSetsubigo    = 0x00009100,                   /* suffix for person name */
    kLASpeechChimeiSetsubigo    = 0x00009200,                   /* suffix for place name */
    kLASpeechSoshikimeiSetsubigo = 0x00009300,                  /* suffix for organization name */
    kLASpeechSuujiSetsubigo     = 0x00009400,                   /* suffix for numbers */
    kLASpeechMuhinshi           = 0x0000A000,                   /* no category */
    kLASpeechTankanji           = 0x0000A000,                   /* character */
    kLASpeechKigou              = 0x0000A100,                   /* symbol */
    kLASpeechKuten              = 0x0000A110,
    kLASpeechTouten             = 0x0000A120,
    kLASpeechSuushi             = 0x0000A200,                   /* numbers */
    kLASpeechDokuritsugo        = 0x0000A300,
    kLASpeechSeiku              = 0x0000A400,
    kLASpeechJodoushi           = 0x0000B000,                   /* auxiliary verb */
    kLASpeechJoshi              = 0x0000C000                    /* postpositional particle */
};


/*
    Conjugations
 */
enum {
    kLASpeechKatsuyouGokan      = 0x00000001,                   /* stem */
    kLASpeechKatsuyouMizen      = 0x00000002,
    kLASpeechKatsuyouRenyou     = 0x00000003,
    kLASpeechKatsuyouSyuushi    = 0x00000004,
    kLASpeechKatsuyouRentai     = 0x00000005,
    kLASpeechKatsuyouKatei      = 0x00000006,
    kLASpeechKatsuyouMeirei     = 0x00000007
};



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

#endif /* __LANGUAGEANALYSIS__ */

