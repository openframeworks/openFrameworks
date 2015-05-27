/*
     File:       SFNTTypes.h
 
     Contains:   Font file structures.
 
     Version:    Technology: Quickdraw GX 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SFNTTYPES__
#define __SFNTTYPES__

#ifndef __MACTYPES__
#include <MacTypes.h>
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


struct sfntDirectoryEntry {
    FourCharCode                    tableTag;
    UInt32                          checkSum;
    UInt32                          offset;
    UInt32                          length;
};
typedef struct sfntDirectoryEntry       sfntDirectoryEntry;
/* The search fields limits numOffsets to 4096. */

struct sfntDirectory {
    FourCharCode                    format;
    UInt16                          numOffsets;                 /* number of tables */
    UInt16                          searchRange;                /* (max2 <= numOffsets)*16 */
    UInt16                          entrySelector;              /* log2(max2 <= numOffsets) */
    UInt16                          rangeShift;                 /* numOffsets*16-searchRange*/
    sfntDirectoryEntry              table[1];                   /* table[numOffsets] */
};
typedef struct sfntDirectory            sfntDirectory;
enum {
    sizeof_sfntDirectory        = 12
};

/* Cmap - character id to glyph id mapping */
enum {
    cmapFontTableTag            = FOUR_CHAR_CODE('cmap')
};

enum {
    kFontUnicodePlatform        = 0,
    kFontMacintoshPlatform      = 1,
    kFontReservedPlatform       = 2,
    kFontMicrosoftPlatform      = 3,
    kFontCustomPlatform         = 4
};

enum {
    kFontUnicodeDefaultSemantics = 0,
    kFontUnicodeV1_1Semantics   = 1,
    kFontISO10646_1993Semantics = 2
};

enum {
    kFontRomanScript            = 0,
    kFontJapaneseScript         = 1,
    kFontTraditionalChineseScript = 2,
    kFontChineseScript          = kFontTraditionalChineseScript,
    kFontKoreanScript           = 3,
    kFontArabicScript           = 4,
    kFontHebrewScript           = 5,
    kFontGreekScript            = 6,
    kFontCyrillicScript         = 7,
    kFontRussian                = kFontCyrillicScript,
    kFontRSymbolScript          = 8,
    kFontDevanagariScript       = 9,
    kFontGurmukhiScript         = 10,
    kFontGujaratiScript         = 11,
    kFontOriyaScript            = 12,
    kFontBengaliScript          = 13,
    kFontTamilScript            = 14,
    kFontTeluguScript           = 15,
    kFontKannadaScript          = 16,
    kFontMalayalamScript        = 17,
    kFontSinhaleseScript        = 18,
    kFontBurmeseScript          = 19,
    kFontKhmerScript            = 20,
    kFontThaiScript             = 21,
    kFontLaotianScript          = 22,
    kFontGeorgianScript         = 23,
    kFontArmenianScript         = 24,
    kFontSimpleChineseScript    = 25,
    kFontTibetanScript          = 26,
    kFontMongolianScript        = 27,
    kFontGeezScript             = 28,
    kFontEthiopicScript         = kFontGeezScript,
    kFontAmharicScript          = kFontGeezScript,
    kFontSlavicScript           = 29,
    kFontEastEuropeanRomanScript = kFontSlavicScript,
    kFontVietnameseScript       = 30,
    kFontExtendedArabicScript   = 31,
    kFontSindhiScript           = kFontExtendedArabicScript,
    kFontUninterpretedScript    = 32
};

enum {
    kFontMicrosoftSymbolScript  = 0,
    kFontMicrosoftStandardScript = 1
};


enum {
    kFontCustom8BitScript       = 0,
    kFontCustom816BitScript     = 1,
    kFontCustom16BitScript      = 2
};

/* Language codes are zero based everywhere but within a 'cmap' table */
enum {
    kFontEnglishLanguage        = 0,
    kFontFrenchLanguage         = 1,
    kFontGermanLanguage         = 2,
    kFontItalianLanguage        = 3,
    kFontDutchLanguage          = 4,
    kFontSwedishLanguage        = 5,
    kFontSpanishLanguage        = 6,
    kFontDanishLanguage         = 7,
    kFontPortugueseLanguage     = 8,
    kFontNorwegianLanguage      = 9,
    kFontHebrewLanguage         = 10,
    kFontJapaneseLanguage       = 11,
    kFontArabicLanguage         = 12,
    kFontFinnishLanguage        = 13,
    kFontGreekLanguage          = 14,
    kFontIcelandicLanguage      = 15,
    kFontMalteseLanguage        = 16,
    kFontTurkishLanguage        = 17,
    kFontCroatianLanguage       = 18,
    kFontTradChineseLanguage    = 19,
    kFontUrduLanguage           = 20,
    kFontHindiLanguage          = 21,
    kFontThaiLanguage           = 22,
    kFontKoreanLanguage         = 23,
    kFontLithuanianLanguage     = 24,
    kFontPolishLanguage         = 25,
    kFontHungarianLanguage      = 26,
    kFontEstonianLanguage       = 27,
    kFontLettishLanguage        = 28,
    kFontLatvianLanguage        = kFontLettishLanguage,
    kFontSaamiskLanguage        = 29,
    kFontLappishLanguage        = kFontSaamiskLanguage,
    kFontFaeroeseLanguage       = 30,
    kFontFarsiLanguage          = 31,
    kFontPersianLanguage        = kFontFarsiLanguage,
    kFontRussianLanguage        = 32,
    kFontSimpChineseLanguage    = 33,
    kFontFlemishLanguage        = 34,
    kFontIrishLanguage          = 35,
    kFontAlbanianLanguage       = 36,
    kFontRomanianLanguage       = 37,
    kFontCzechLanguage          = 38,
    kFontSlovakLanguage         = 39,
    kFontSlovenianLanguage      = 40,
    kFontYiddishLanguage        = 41,
    kFontSerbianLanguage        = 42,
    kFontMacedonianLanguage     = 43,
    kFontBulgarianLanguage      = 44,
    kFontUkrainianLanguage      = 45,
    kFontByelorussianLanguage   = 46,
    kFontUzbekLanguage          = 47,
    kFontKazakhLanguage         = 48,
    kFontAzerbaijaniLanguage    = 49,
    kFontAzerbaijanArLanguage   = 50,
    kFontArmenianLanguage       = 51,
    kFontGeorgianLanguage       = 52,
    kFontMoldavianLanguage      = 53,
    kFontKirghizLanguage        = 54,
    kFontTajikiLanguage         = 55,
    kFontTurkmenLanguage        = 56,
    kFontMongolianLanguage      = 57,
    kFontMongolianCyrLanguage   = 58,
    kFontPashtoLanguage         = 59,
    kFontKurdishLanguage        = 60,
    kFontKashmiriLanguage       = 61,
    kFontSindhiLanguage         = 62,
    kFontTibetanLanguage        = 63,
    kFontNepaliLanguage         = 64,
    kFontSanskritLanguage       = 65,
    kFontMarathiLanguage        = 66,
    kFontBengaliLanguage        = 67,
    kFontAssameseLanguage       = 68,
    kFontGujaratiLanguage       = 69,
    kFontPunjabiLanguage        = 70,
    kFontOriyaLanguage          = 71,
    kFontMalayalamLanguage      = 72,
    kFontKannadaLanguage        = 73,
    kFontTamilLanguage          = 74,
    kFontTeluguLanguage         = 75,
    kFontSinhaleseLanguage      = 76,
    kFontBurmeseLanguage        = 77,
    kFontKhmerLanguage          = 78,
    kFontLaoLanguage            = 79,
    kFontVietnameseLanguage     = 80,
    kFontIndonesianLanguage     = 81,
    kFontTagalogLanguage        = 82,
    kFontMalayRomanLanguage     = 83,
    kFontMalayArabicLanguage    = 84,
    kFontAmharicLanguage        = 85,
    kFontTigrinyaLanguage       = 86,
    kFontGallaLanguage          = 87,
    kFontOromoLanguage          = kFontGallaLanguage,
    kFontSomaliLanguage         = 88,
    kFontSwahiliLanguage        = 89,
    kFontRuandaLanguage         = 90,
    kFontRundiLanguage          = 91,
    kFontChewaLanguage          = 92,
    kFontMalagasyLanguage       = 93,
    kFontEsperantoLanguage      = 94,
    kFontWelshLanguage          = 128,
    kFontBasqueLanguage         = 129,
    kFontCatalanLanguage        = 130,
    kFontLatinLanguage          = 131,
    kFontQuechuaLanguage        = 132,
    kFontGuaraniLanguage        = 133,
    kFontAymaraLanguage         = 134,
    kFontTatarLanguage          = 135,
    kFontUighurLanguage         = 136,
    kFontDzongkhaLanguage       = 137,
    kFontJavaneseRomLanguage    = 138,
    kFontSundaneseRomLanguage   = 139
};

/* The following are special "don't care" values to be used in interfaces */
enum {
    kFontNoPlatform             = -1,
    kFontNoScript               = -1,
    kFontNoLanguage             = -1
};


struct sfntCMapSubHeader {
    UInt16                          format;
    UInt16                          length;
    UInt16                          languageID;                 /* base-1 */
};
typedef struct sfntCMapSubHeader        sfntCMapSubHeader;
enum {
    sizeof_sfntCMapSubHeader    = 6
};


struct sfntCMapEncoding {
    UInt16                          platformID;                 /* base-0 */
    UInt16                          scriptID;                   /* base-0 */
    UInt32                          offset;
};
typedef struct sfntCMapEncoding         sfntCMapEncoding;
enum {
    sizeof_sfntCMapEncoding     = 8
};


struct sfntCMapHeader {
    UInt16                          version;
    UInt16                          numTables;
    sfntCMapEncoding                encoding[1];
};
typedef struct sfntCMapHeader           sfntCMapHeader;
enum {
    sizeof_sfntCMapHeader       = 4
};

/* Name table */
enum {
    nameFontTableTag            = FOUR_CHAR_CODE('name')
};

enum {
    kFontCopyrightName          = 0,
    kFontFamilyName             = 1,
    kFontStyleName              = 2,
    kFontUniqueName             = 3,
    kFontFullName               = 4,
    kFontVersionName            = 5,
    kFontPostscriptName         = 6,
    kFontTrademarkName          = 7,
    kFontManufacturerName       = 8,
    kFontDesignerName           = 9,
    kFontDescriptionName        = 10,
    kFontVendorURLName          = 11,
    kFontDesignerURLName        = 12,
    kFontLicenseDescriptionName = 13,
    kFontLicenseInfoURLName     = 14,
    kFontLastReservedName       = 255
};

/* The following is a special "don't care" value to be used in interfaces */
enum {
    kFontNoName                 = -1
};


struct sfntNameRecord {
    UInt16                          platformID;                 /* base-0 */
    UInt16                          scriptID;                   /* base-0 */
    UInt16                          languageID;                 /* base-0 */
    UInt16                          nameID;                     /* base-0 */
    UInt16                          length;
    UInt16                          offset;
};
typedef struct sfntNameRecord           sfntNameRecord;
enum {
    sizeof_sfntNameRecord       = 12
};


struct sfntNameHeader {
    UInt16                          format;
    UInt16                          count;
    UInt16                          stringOffset;
    sfntNameRecord                  rec[1];
};
typedef struct sfntNameHeader           sfntNameHeader;
enum {
    sizeof_sfntNameHeader       = 6
};

/* Fvar table - font variations */
enum {
    variationFontTableTag       = FOUR_CHAR_CODE('fvar')
};

/* These define each font variation */

struct sfntVariationAxis {
    FourCharCode                    axisTag;
    Fixed                           minValue;
    Fixed                           defaultValue;
    Fixed                           maxValue;
    SInt16                          flags;
    SInt16                          nameID;
};
typedef struct sfntVariationAxis        sfntVariationAxis;
enum {
    sizeof_sfntVariationAxis    = 20
};

/* These are named locations in style-space for the user */

struct sfntInstance {
    SInt16                          nameID;
    SInt16                          flags;
    Fixed                           coord[1];                   /* [axisCount] */
                                                                /* room to grow since the header carries a tupleSize field */
};
typedef struct sfntInstance             sfntInstance;
enum {
    sizeof_sfntInstance         = 4
};


struct sfntVariationHeader {
    Fixed                           version;                    /* 1.0 Fixed */
    UInt16                          offsetToData;               /* to first axis = 16*/
    UInt16                          countSizePairs;             /* axis+inst = 2 */
    UInt16                          axisCount;
    UInt16                          axisSize;
    UInt16                          instanceCount;
    UInt16                          instanceSize;
                                                                /* ...other <count,size> pairs */
    sfntVariationAxis               axis[1];                    /* [axisCount] */
    sfntInstance                    instance[1];                /* [instanceCount]  ...other arrays of data */
};
typedef struct sfntVariationHeader      sfntVariationHeader;
enum {
    sizeof_sfntVariationHeader  = 16
};

/* Fdsc table - font descriptor */
enum {
    descriptorFontTableTag      = FOUR_CHAR_CODE('fdsc')
};


struct sfntFontDescriptor {
    FourCharCode                    name;
    Fixed                           value;
};
typedef struct sfntFontDescriptor       sfntFontDescriptor;

struct sfntDescriptorHeader {
    Fixed                           version;                    /* 1.0 in Fixed */
    SInt32                          descriptorCount;
    sfntFontDescriptor              descriptor[1];
};
typedef struct sfntDescriptorHeader     sfntDescriptorHeader;
enum {
    sizeof_sfntDescriptorHeader = 8
};

/* Feat Table - layout feature table */
enum {
    featureFontTableTag         = FOUR_CHAR_CODE('feat')
};


struct sfntFeatureName {
    UInt16                          featureType;
    UInt16                          settingCount;
    SInt32                          offsetToSettings;
    UInt16                          featureFlags;
    UInt16                          nameID;
};
typedef struct sfntFeatureName          sfntFeatureName;

struct sfntFontFeatureSetting {
    UInt16                          setting;
    UInt16                          nameID;
};
typedef struct sfntFontFeatureSetting   sfntFontFeatureSetting;

struct sfntFontRunFeature {
    UInt16                          featureType;
    UInt16                          setting;
};
typedef struct sfntFontRunFeature       sfntFontRunFeature;

struct sfntFeatureHeader {
    SInt32                          version;                    /* 1.0 */
    UInt16                          featureNameCount;
    UInt16                          featureSetCount;
    SInt32                          reserved;                   /* set to 0 */
    sfntFeatureName                 names[1];
    sfntFontFeatureSetting          settings[1];
    sfntFontRunFeature              runs[1];
};
typedef struct sfntFeatureHeader        sfntFeatureHeader;
/* OS/2 Table */
enum {
    os2FontTableTag             = FOUR_CHAR_CODE('OS/2')
};

/*  Special invalid glyph ID value, useful as a sentinel value, for example */
enum {
    nonGlyphID                  = 65535L
};

/*  Data type used to access names from font name table */

typedef UInt32                          FontNameCode;
/* Data types for encoding components as used in interfaces */
typedef UInt32                          FontPlatformCode;
typedef UInt32                          FontScriptCode;
typedef UInt32                          FontLanguageCode;
/*
**  FontVariation is used to specify a coordinate along a variation axis. The name
**  identifies the axes to be applied, and value is the setting to be used.
*/

struct FontVariation {
    FourCharCode                    name;
    Fixed                           value;
};
typedef struct FontVariation            FontVariation;

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

#endif /* __SFNTTYPES__ */

