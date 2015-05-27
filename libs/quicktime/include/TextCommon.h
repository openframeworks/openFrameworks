/*
     File:       TextCommon.h
 
     Contains:   TextEncoding-related types and constants, and prototypes for related functions
 
     Version:    Technology: Mac OS 9.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TEXTCOMMON__
#define __TEXTCOMMON__

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

/* TextEncodingBase type & values */
/* (values 0-32 correspond to the Script Codes defined in Inside Macintosh: Text pages 6-52 and 6-53 */
typedef UInt32                          TextEncodingBase;
enum {
                                                                /* Mac OS encodings*/
    kTextEncodingMacRoman       = 0L,
    kTextEncodingMacJapanese    = 1,
    kTextEncodingMacChineseTrad = 2,
    kTextEncodingMacKorean      = 3,
    kTextEncodingMacArabic      = 4,
    kTextEncodingMacHebrew      = 5,
    kTextEncodingMacGreek       = 6,
    kTextEncodingMacCyrillic    = 7,
    kTextEncodingMacDevanagari  = 9,
    kTextEncodingMacGurmukhi    = 10,
    kTextEncodingMacGujarati    = 11,
    kTextEncodingMacOriya       = 12,
    kTextEncodingMacBengali     = 13,
    kTextEncodingMacTamil       = 14,
    kTextEncodingMacTelugu      = 15,
    kTextEncodingMacKannada     = 16,
    kTextEncodingMacMalayalam   = 17,
    kTextEncodingMacSinhalese   = 18,
    kTextEncodingMacBurmese     = 19,
    kTextEncodingMacKhmer       = 20,
    kTextEncodingMacThai        = 21,
    kTextEncodingMacLaotian     = 22,
    kTextEncodingMacGeorgian    = 23,
    kTextEncodingMacArmenian    = 24,
    kTextEncodingMacChineseSimp = 25,
    kTextEncodingMacTibetan     = 26,
    kTextEncodingMacMongolian   = 27,
    kTextEncodingMacEthiopic    = 28,
    kTextEncodingMacCentralEurRoman = 29,
    kTextEncodingMacVietnamese  = 30,
    kTextEncodingMacExtArabic   = 31,                           /* The following use script code 0, smRoman*/
    kTextEncodingMacSymbol      = 33,
    kTextEncodingMacDingbats    = 34,
    kTextEncodingMacTurkish     = 35,
    kTextEncodingMacCroatian    = 36,
    kTextEncodingMacIcelandic   = 37,
    kTextEncodingMacRomanian    = 38,
    kTextEncodingMacCeltic      = 39,
    kTextEncodingMacGaelic      = 40,                           /* Beginning in Mac OS 8.5, the following meta-value is used to indicate Unicode in some parts*/
                                                                /* of the Mac OS which previously only expected a Mac OS script code. In some of these places,*/
                                                                /* only 7 bits are available to indicate encoding (script code), so kTextEncodingUnicodeDefault*/
                                                                /* cannot be used. For example, kTextEncodingMacUnicode can be used to indicate Unicode in the*/
                                                                /* 7-bit script code field of a Unicode input method's ComponentDescription.componentFlags field;*/
                                                                /* it can also be used to indicate Unicode in the 16-bit script code field of an AppleEvent's*/
                                                                /* typeIntlWritingCode text tag.*/
    kTextEncodingMacUnicode     = 0x7E,                         /* Meta-value, Unicode as a Mac encoding*/
                                                                /* The following use script code 4, smArabic*/
    kTextEncodingMacFarsi       = 0x8C,                         /* Like MacArabic but uses Farsi digits*/
                                                                /* The following use script code 7, smCyrillic*/
    kTextEncodingMacUkrainian   = 0x98,                         /* Meta-value in TEC 1.5 & later; maps to kTextEncodingMacCyrillic variant    */
                                                                /* The following use script code 28, smEthiopic*/
    kTextEncodingMacInuit       = 0xEC,                         /* The following use script code 32, smUnimplemented*/
    kTextEncodingMacVT100       = 0xFC,                         /* VT100/102 font from Comm Toolbox: Latin-1 repertoire + box drawing etc*/
                                                                /* Special Mac OS encodings*/
    kTextEncodingMacHFS         = 0xFF,                         /* Meta-value, should never appear in a table.*/
                                                                /* Unicode & ISO UCS encodings begin at 0x100*/
    kTextEncodingUnicodeDefault = 0x0100,                       /* Meta-value, should never appear in a table.*/
    kTextEncodingUnicodeV1_1    = 0x0101,
    kTextEncodingISO10646_1993  = 0x0101,                       /* Code points identical to Unicode 1.1*/
    kTextEncodingUnicodeV2_0    = 0x0103,                       /* New location for Korean Hangul*/
    kTextEncodingUnicodeV2_1    = 0x0103,                       /* We treat both Unicode 2.0 and Unicode 2.1 as 2.1*/
                                                                /* ISO 8-bit and 7-bit encodings begin at 0x200*/
    kTextEncodingISOLatin1      = 0x0201,                       /* ISO 8859-1*/
    kTextEncodingISOLatin2      = 0x0202,                       /* ISO 8859-2*/
    kTextEncodingISOLatin3      = 0x0203,                       /* ISO 8859-3*/
    kTextEncodingISOLatin4      = 0x0204,                       /* ISO 8859-4*/
    kTextEncodingISOLatinCyrillic = 0x0205,                     /* ISO 8859-5*/
    kTextEncodingISOLatinArabic = 0x0206,                       /* ISO 8859-6, = ASMO 708, =DOS CP 708*/
    kTextEncodingISOLatinGreek  = 0x0207,                       /* ISO 8859-7*/
    kTextEncodingISOLatinHebrew = 0x0208,                       /* ISO 8859-8*/
    kTextEncodingISOLatin5      = 0x0209,                       /* ISO 8859-9*/
    kTextEncodingISOLatin6      = 0x020A,                       /* ISO 8859-10                           */
    kTextEncodingISOLatin7      = 0x020D,                       /* ISO 8859-13, Baltic Rim                   */
    kTextEncodingISOLatin8      = 0x020E,                       /* ISO 8859-14, Celtic                    */
    kTextEncodingISOLatin9      = 0x020F,                       /* ISO 8859-15, 8859-1 changed for EURO & CP1252 letters  */
                                                                /* MS-DOS & Windows encodings begin at 0x400*/
    kTextEncodingDOSLatinUS     = 0x0400,                       /* code page 437*/
    kTextEncodingDOSGreek       = 0x0405,                       /* code page 737 (formerly code page 437G)*/
    kTextEncodingDOSBalticRim   = 0x0406,                       /* code page 775*/
    kTextEncodingDOSLatin1      = 0x0410,                       /* code page 850, "Multilingual"*/
    kTextEncodingDOSGreek1      = 0x0411,                       /* code page 851*/
    kTextEncodingDOSLatin2      = 0x0412,                       /* code page 852, Slavic*/
    kTextEncodingDOSCyrillic    = 0x0413,                       /* code page 855, IBM Cyrillic*/
    kTextEncodingDOSTurkish     = 0x0414,                       /* code page 857, IBM Turkish*/
    kTextEncodingDOSPortuguese  = 0x0415,                       /* code page 860*/
    kTextEncodingDOSIcelandic   = 0x0416,                       /* code page 861*/
    kTextEncodingDOSHebrew      = 0x0417,                       /* code page 862*/
    kTextEncodingDOSCanadianFrench = 0x0418,                    /* code page 863*/
    kTextEncodingDOSArabic      = 0x0419,                       /* code page 864*/
    kTextEncodingDOSNordic      = 0x041A,                       /* code page 865*/
    kTextEncodingDOSRussian     = 0x041B,                       /* code page 866*/
    kTextEncodingDOSGreek2      = 0x041C,                       /* code page 869, IBM Modern Greek*/
    kTextEncodingDOSThai        = 0x041D,                       /* code page 874, also for Windows*/
    kTextEncodingDOSJapanese    = 0x0420,                       /* code page 932, also for Windows; Shift-JIS with additions*/
    kTextEncodingDOSChineseSimplif = 0x0421,                    /* code page 936, also for Windows; was EUC-CN, now GBK (EUC-CN extended)*/
    kTextEncodingDOSKorean      = 0x0422,                       /* code page 949, also for Windows; Unified Hangul Code (EUC-KR extended)*/
    kTextEncodingDOSChineseTrad = 0x0423,                       /* code page 950, also for Windows; Big-5*/
    kTextEncodingWindowsLatin1  = 0x0500,                       /* code page 1252*/
    kTextEncodingWindowsANSI    = 0x0500,                       /* code page 1252 (alternate name)*/
    kTextEncodingWindowsLatin2  = 0x0501,                       /* code page 1250, Central Europe*/
    kTextEncodingWindowsCyrillic = 0x0502,                      /* code page 1251, Slavic Cyrillic*/
    kTextEncodingWindowsGreek   = 0x0503,                       /* code page 1253*/
    kTextEncodingWindowsLatin5  = 0x0504,                       /* code page 1254, Turkish*/
    kTextEncodingWindowsHebrew  = 0x0505,                       /* code page 1255*/
    kTextEncodingWindowsArabic  = 0x0506,                       /* code page 1256*/
    kTextEncodingWindowsBalticRim = 0x0507,                     /* code page 1257*/
    kTextEncodingWindowsVietnamese = 0x0508,                    /* code page 1258*/
    kTextEncodingWindowsKoreanJohab = 0x0510,                   /* code page 1361, for Windows NT*/
                                                                /* Various national standards begin at 0x600*/
    kTextEncodingUS_ASCII       = 0x0600,
    kTextEncodingJIS_X0201_76   = 0x0620,                       /* JIS Roman and 1-byte katakana (halfwidth)*/
    kTextEncodingJIS_X0208_83   = 0x0621,
    kTextEncodingJIS_X0208_90   = 0x0622,
    kTextEncodingJIS_X0212_90   = 0x0623,
    kTextEncodingJIS_C6226_78   = 0x0624,
    kTextEncodingGB_2312_80     = 0x0630,
    kTextEncodingGBK_95         = 0x0631,                       /* annex to GB 13000-93; for Windows 95; EUC-CN extended*/
    kTextEncodingKSC_5601_87    = 0x0640,                       /* same as KSC 5601-92 without Johab annex*/
    kTextEncodingKSC_5601_92_Johab = 0x0641,                    /* KSC 5601-92 Johab annex*/
    kTextEncodingCNS_11643_92_P1 = 0x0651,                      /* CNS 11643-1992 plane 1*/
    kTextEncodingCNS_11643_92_P2 = 0x0652,                      /* CNS 11643-1992 plane 2*/
    kTextEncodingCNS_11643_92_P3 = 0x0653,                      /* CNS 11643-1992 plane 3 (was plane 14 in 1986 version)*/
                                                                /* ISO 2022 collections begin at 0x800*/
    kTextEncodingISO_2022_JP    = 0x0820,
    kTextEncodingISO_2022_JP_2  = 0x0821,
    kTextEncodingISO_2022_CN    = 0x0830,
    kTextEncodingISO_2022_CN_EXT = 0x0831,
    kTextEncodingISO_2022_KR    = 0x0840,                       /* EUC collections begin at 0x900*/
    kTextEncodingEUC_JP         = 0x0920,                       /* ISO 646, 1-byte katakana, JIS 208, JIS 212*/
    kTextEncodingEUC_CN         = 0x0930,                       /* ISO 646, GB 2312-80*/
    kTextEncodingEUC_TW         = 0x0931,                       /* ISO 646, CNS 11643-1992 Planes 1-16*/
    kTextEncodingEUC_KR         = 0x0940,                       /* ISO 646, KS C 5601-1987*/
                                                                /* Misc standards begin at 0xA00*/
    kTextEncodingShiftJIS       = 0x0A01,                       /* plain Shift-JIS*/
    kTextEncodingKOI8_R         = 0x0A02,                       /* Russian internet standard*/
    kTextEncodingBig5           = 0x0A03,                       /* Big-5 (has variants)*/
    kTextEncodingMacRomanLatin1 = 0x0A04,                       /* Mac OS Roman permuted to align with ISO Latin-1*/
    kTextEncodingHZ_GB_2312     = 0x0A05,                       /* HZ (RFC 1842, for Chinese mail & news)*/
                                                                /* Other platform encodings*/
    kTextEncodingNextStepLatin  = 0x0B01,                       /* NextStep encoding*/
                                                                /* EBCDIC & IBM host encodings begin at 0xC00*/
    kTextEncodingEBCDIC_US      = 0x0C01,                       /* basic EBCDIC-US*/
    kTextEncodingEBCDIC_CP037   = 0x0C02,                       /* code page 037, extended EBCDIC (Latin-1 set) for US,Canada...*/
                                                                /* Special values*/
    kTextEncodingMultiRun       = 0x0FFF,                       /* Multi-encoding text with external run info*/
    kTextEncodingUnknown        = 0xFFFF,                       /* Unknown or unspecified                  */
                                                                /* The following are older names for backward compatibility*/
    kTextEncodingMacTradChinese = 2,
    kTextEncodingMacRSymbol     = 8,
    kTextEncodingMacSimpChinese = 25,
    kTextEncodingMacGeez        = 28,
    kTextEncodingMacEastEurRoman = 29,
    kTextEncodingMacUninterp    = 32
};

/* TextEncodingVariant type & values */
typedef UInt32                          TextEncodingVariant;
enum {
                                                                /* Default TextEncodingVariant, for any TextEncodingBase*/
    kTextEncodingDefaultVariant = 0,                            /* Variants of kTextEncodingMacRoman                                 */
    kMacRomanDefaultVariant     = 0,                            /* meta value, maps to 1 or 2 depending on System */
    kMacRomanCurrencySignVariant = 1,                           /* Mac OS version < 8.5, 0xDB is CURRENCY SIGN*/
    kMacRomanEuroSignVariant    = 2,                            /* Mac OS version >= 8.5, 0xDB is EURO SIGN      */
                                                                /* Variants of kTextEncodingMacCyrillic (for TEC 1.5 and later)                    */
    kMacCyrillicDefaultVariant  = 0,                            /* meta value, maps to 1, 2, or 3 depending on System*/
    kMacCyrillicCurrSignStdVariant = 1,                         /* Mac OS < 9.0 (RU,BG), 0xFF = CURRENCY SIGN, 0xA2/0xB6 = CENT / PARTIAL DIFF.*/
    kMacCyrillicCurrSignUkrVariant = 2,                         /* Mac OS < 9.0 (UA,LangKit), 0xFF = CURRENCY SIGN, 0xA2/0xB6 = GHE WITH UPTURN*/
    kMacCyrillicEuroSignVariant = 3,                            /* Mac OS >= 9.0, 0xFF is EURO SIGN, 0xA2/0xB6 = GHE WITH UPTURN*/
                                                                /* Variants of kTextEncodingMacIcelandic                                */
    kMacIcelandicStdDefaultVariant = 0,                         /* meta value, maps to 2 or 4 depending on System */
    kMacIcelandicTTDefaultVariant = 1,                          /* meta value, maps to 3 or 5 depending on System */
                                                                /* The following are for Mac OS version < 8.5, 0xDB is CURRENCY SIGN             */
    kMacIcelandicStdCurrSignVariant = 2,                        /* 0xBB/0xBC are fem./masc. ordinal indicators*/
    kMacIcelandicTTCurrSignVariant = 3,                         /* 0xBB/0xBC are fi/fl ligatures*/
                                                                /* The following are for Mac OS version >= 8.5, 0xDB is EURO SIGN                  */
    kMacIcelandicStdEuroSignVariant = 4,                        /* 0xBB/0xBC are fem./masc. ordinal indicators*/
    kMacIcelandicTTEuroSignVariant = 5,                         /* 0xBB/0xBC are fi/fl ligatures*/
                                                                /* Variants of kTextEncodingMacCroatian                                   */
    kMacCroatianDefaultVariant  = 0,                            /* meta value, maps to 1 or 2 depending on System */
    kMacCroatianCurrencySignVariant = 1,                        /* Mac OS version < 8.5, 0xDB is CURRENCY SIGN       */
    kMacCroatianEuroSignVariant = 2,                            /* Mac OS version >= 8.5, 0xDB is EURO SIGN      */
                                                                /* Variants of kTextEncodingMacRomanian                                   */
    kMacRomanianDefaultVariant  = 0,                            /* meta value, maps to 1 or 2 depending on System */
    kMacRomanianCurrencySignVariant = 1,                        /* Mac OS version < 8.5, 0xDB is CURRENCY SIGN       */
    kMacRomanianEuroSignVariant = 2,                            /* Mac OS version >= 8.5, 0xDB is EURO SIGN      */
                                                                /* Variants of kTextEncodingMacJapanese*/
    kMacJapaneseStandardVariant = 0,
    kMacJapaneseStdNoVerticalsVariant = 1,
    kMacJapaneseBasicVariant    = 2,
    kMacJapanesePostScriptScrnVariant = 3,
    kMacJapanesePostScriptPrintVariant = 4,
    kMacJapaneseVertAtKuPlusTenVariant = 5,                     /* Variants of kTextEncodingMacArabic*/
    kMacArabicStandardVariant   = 0,                            /* 0xC0 is 8-spoke asterisk, 0x2A & 0xAA are asterisk (e.g. Cairo)*/
    kMacArabicTrueTypeVariant   = 1,                            /* 0xC0 is asterisk, 0x2A & 0xAA are multiply signs (e.g. Baghdad)*/
    kMacArabicThuluthVariant    = 2,                            /* 0xC0 is Arabic five-point star, 0x2A & 0xAA are multiply signs*/
    kMacArabicAlBayanVariant    = 3,                            /* 8-spoke asterisk, multiply sign, Koranic ligatures & parens*/
                                                                /* Variants of kTextEncodingMacFarsi*/
    kMacFarsiStandardVariant    = 0,                            /* 0xC0 is 8-spoke asterisk, 0x2A & 0xAA are asterisk (e.g. Tehran)*/
    kMacFarsiTrueTypeVariant    = 1,                            /* asterisk, multiply signs, Koranic ligatures, geometric shapes*/
                                                                /* Variants of kTextEncodingMacHebrew*/
    kMacHebrewStandardVariant   = 0,
    kMacHebrewFigureSpaceVariant = 1,                           /* Variants of kTextEncodingMacVT100                                 */
    kMacVT100DefaultVariant     = 0,                            /* meta value, maps to 1 or 2 depending on System */
    kMacVT100CurrencySignVariant = 1,                           /* Mac OS version < 8.5, 0xDB is CURRENCY SIGN       */
    kMacVT100EuroSignVariant    = 2,                            /* Mac OS version >= 8.5, 0xDB is EURO SIGN      */
                                                                /* Variants of Unicode & ISO 10646 encodings*/
    kUnicodeNoSubset            = 0,
    kUnicodeCanonicalDecompVariant = 2,                         /* canonical decomposition; excludes composed characters*/
                                                                /* Variants of Big-5 encoding*/
    kBig5_BasicVariant          = 0,
    kBig5_StandardVariant       = 1,                            /* 0xC6A1-0xC7FC: kana, Cyrillic, enclosed numerics*/
    kBig5_ETenVariant           = 2,                            /* adds kana, Cyrillic, radicals, etc with hi bytes C6-C8,F9*/
                                                                /* Unicode variants not yet supported (and not fully defined)*/
    kUnicodeNoCompatibilityVariant = 1,
    kUnicodeNoComposedVariant   = 3,
    kUnicodeNoCorporateVariant  = 4,                            /* The following are older names for backward compatibility*/
    kMacRomanStandardVariant    = 0,
    kMacIcelandicStandardVariant = 0,
    kMacIcelandicTrueTypeVariant = 1,
    kJapaneseStandardVariant    = 0,
    kJapaneseStdNoVerticalsVariant = 1,
    kJapaneseBasicVariant       = 2,
    kJapanesePostScriptScrnVariant = 3,
    kJapanesePostScriptPrintVariant = 4,
    kJapaneseVertAtKuPlusTenVariant = 5,                        /* kJapaneseStdNoOneByteKanaVariant = 6,  // replaced by kJapaneseNoOneByteKanaOption*/
                                                                /* kJapaneseBasicNoOneByteKanaVariant = 7,    // replaced by kJapaneseNoOneByteKanaOption    */
    kHebrewStandardVariant      = 0,
    kHebrewFigureSpaceVariant   = 1,
    kUnicodeMaxDecomposedVariant = 2,                           /* replaced by kUnicodeCanonicalDecompVariant*/
                                                                /* The following Japanese variant options were never supported and are now deprecated.*/
                                                                /* In TEC 1.4 and later their functionality is replaced by the Unicode Converter options listed.*/
    kJapaneseNoOneByteKanaOption = 0x20,                        /* replaced by UnicodeConverter option kUnicodeNoHalfwidthCharsBit*/
    kJapaneseUseAsciiBackslashOption = 0x40                     /* replaced by UnicodeConverter option kUnicodeForceASCIIRangeBit*/
};

/* TextEncodingFormat type & values */
typedef UInt32                          TextEncodingFormat;
enum {
                                                                /* Default TextEncodingFormat for any TextEncodingBase*/
    kTextEncodingDefaultFormat  = 0,                            /* Formats for Unicode & ISO 10646*/
    kUnicode16BitFormat         = 0,
    kUnicodeUTF7Format          = 1,
    kUnicodeUTF8Format          = 2,
    kUnicode32BitFormat         = 3
};

/* TextEncoding type */
typedef UInt32                          TextEncoding;
/* name part selector for GetTextEncodingName*/
typedef UInt32                          TextEncodingNameSelector;
enum {
    kTextEncodingFullName       = 0,
    kTextEncodingBaseName       = 1,
    kTextEncodingVariantName    = 2,
    kTextEncodingFormatName     = 3
};

/* Types used in conversion */

struct TextEncodingRun {
    ByteOffset                      offset;
    TextEncoding                    textEncoding;
};
typedef struct TextEncodingRun          TextEncodingRun;
typedef TextEncodingRun *               TextEncodingRunPtr;

typedef const TextEncodingRun *         ConstTextEncodingRunPtr;

struct ScriptCodeRun {
    ByteOffset                      offset;
    ScriptCode                      script;
};
typedef struct ScriptCodeRun            ScriptCodeRun;
typedef ScriptCodeRun *                 ScriptCodeRunPtr;

typedef const ScriptCodeRun *           ConstScriptCodeRunPtr;
typedef UInt8 *                         TextPtr;
typedef const UInt8 *                   ConstTextPtr;
/* Basic types for Unicode characters and strings:*/
typedef UniChar *                       UniCharArrayPtr;
typedef const UniChar *                 ConstUniCharArrayPtr;
/*
   UniCharArrayHandle is a handle type to correspond to UniCharArrayPtr,
   i.e. a handle to an array of UniChars (UInt16s).
*/
typedef UniCharArrayPtr *               UniCharArrayHandle;
/*
   UniCharArrayOffset is used to indicate an edge offset in an array
   of UniChars (UInt16s).  
*/
typedef UInt32                          UniCharArrayOffset;
/* enums for TextEncoding Conversion routines*/
enum {
    kTextScriptDontCare         = -128,
    kTextLanguageDontCare       = -128,
    kTextRegionDontCare         = -128
};

/* struct for TECGetInfo*/


struct TECInfo {
    UInt16                          format;                     /* format code for this struct*/
    UInt16                          tecVersion;                 /* TEC version in BCD, e.g. 0x0121 for 1.2.1*/
    UInt32                          tecTextConverterFeatures;   /* bitmask indicating TEC features/fixes*/
    UInt32                          tecUnicodeConverterFeatures; /* bitmask indicating UnicodeConverter features/fixes*/
    UInt32                          tecTextCommonFeatures;      /* bitmask indicating TextCommon features/fixes*/
    Str31                           tecTextEncodingsFolderName; /* localized name of Text Encodings folder (pascal string)*/
    Str31                           tecExtensionFileName;       /* localized name of TEC extension (pascal string)*/
    UInt16                          tecLowestTEFileVersion;     /* Lowest version (BCD) of all files in Text Encodings folder*/
    UInt16                          tecHighestTEFileVersion;    /* Highest version (BCD) of all files in Text Encodings folder*/
};
typedef struct TECInfo                  TECInfo;
typedef TECInfo *                       TECInfoPtr;
typedef TECInfoPtr *                    TECInfoHandle;
/* Value for TECInfo format code*/
enum {
    kTECInfoCurrentFormat       = 2                             /* any future formats will just add fields at the end*/
};

/*
   Defined feature/fix bits for tecUnicodeConverterFeatures field
   Bit:                             Meaning if set:
   ----                             ---------------
   kTECKeepInfoFixBit               Unicode Converter no longer ignores other control flags if
                                    kUnicodeKeepInfoBit is set. Bug fix in TEC Manager 1.2.1.
   kTECFallbackTextLengthFixBit     Unicode Converter honors the *srcConvLen and *destConvLen
                                    returned by caller-supplied fallback handler for any status it
                                    returns except for kTECUnmappableElementErr (previously it only
                                    honored these values if noErr was returned). Bug fix in TEC
                                    Manager 1.2.1.
   kTECTextRunBitClearFixBit        ConvertFromUnicodeToTextRun & ConvertFromUnicodeToScriptCodeRun
                                    function correctly if the kUnicodeTextRunBit is set (previously
                                    their determination of best target encoding was incorrect). Bug
                                    fix in TEC Manager 1.3.
   kTECTextToUnicodeScanFixBit      ConvertFromTextToUnicode uses an improved scanner and maintains
                                    some resulting state information, which it uses for mapping.
                                    This has several effects:
                                    - Improved mapping of 0x30-0x39 digits in Mac OS Arabic, fewer
                                      direction overrides when mapping Mac OS Arabic & Hebrew, and
                                      improved mapping of certain characters in Indic encodings.
                                    - Malformed input produces kTextMalformedInputErr.
                                    - ConvertFromTextToUnicode accepts and uses the control flags
                                      kUnicodeKeepInfoMask and kUnicodeStringUnterminatedMask.
                                    Bug fix and enhancement in TEC Manager 1.3.
   kTECAddForceASCIIChangesBit      Define new control flag bits kUnicodeForceASCIIRangeBit and
                                    kUnicodeNoHalfwidthCharsBit for use with
                                    ConvertFromTextToUnicode, ConvertFromUnicodeToText, etc.
                                    Enhancement in TEC Manager 1.4.
   kTECPreferredEncodingFixBit      CreateUnicodeToTextRunInfo and related functions fix a problem
                                    that occurred when a preferred encoding was specified that did
                                    not match the System script; the preferred script was not
                                    actually placed first in the ordered list of encodings to use.
                                    Bug fix in TEC Manager 1.4.
   kTECAddTextRunHeuristicsBit      Define new control flag bit kUnicodeTextRunHeuristicsBit for
                                    use with ConvertFromUnicodeToTextRun.
   kTECAddFallbackInterruptBit      Define new option kUnicodeFallbackInterruptSafeMask for use
                                    with SetFallbackUnicodeToText. If a client fallback handler is
                                    installed without specifying this bit, ConvertFromUnicodeToText
                                    will HLock the tables it uses (in case the fallback handler
                                    moves memory); otherwise, it won't.
*/

enum {
    kTECKeepInfoFixBit          = 0,
    kTECFallbackTextLengthFixBit = 1,
    kTECTextRunBitClearFixBit   = 2,
    kTECTextToUnicodeScanFixBit = 3,
    kTECAddForceASCIIChangesBit = 4,
    kTECPreferredEncodingFixBit = 5,
    kTECAddTextRunHeuristicsBit = 6,
    kTECAddFallbackInterruptBit = 7
};

enum {
    kTECKeepInfoFixMask         = 1L << kTECKeepInfoFixBit,
    kTECFallbackTextLengthFixMask = 1L << kTECFallbackTextLengthFixBit,
    kTECTextRunBitClearFixMask  = 1L << kTECTextRunBitClearFixBit,
    kTECTextToUnicodeScanFixMask = 1L << kTECTextToUnicodeScanFixBit,
    kTECAddForceASCIIChangesMask = 1L << kTECAddForceASCIIChangesBit,
    kTECPreferredEncodingFixMask = 1L << kTECPreferredEncodingFixBit,
    kTECAddTextRunHeuristicsMask = 1L << kTECAddTextRunHeuristicsBit,
    kTECAddFallbackInterruptMask = 1L << kTECAddFallbackInterruptBit
};

/*
   -------------------------------------------------------------------------------------------------
   CONSTANTS & DATA STRUCTURES for Unicode Properties
   -------------------------------------------------------------------------------------------------
*/

typedef SInt32                          UCCharPropertyType;
enum {
    kUCCharPropTypeGenlCategory = 1,                            /* requests enumeration value*/
    kUCCharPropTypeCombiningClass = 2,                          /* requests numeric value 0..255*/
    kUCCharPropTypeBidiCategory = 3                             /* requests enumeration value*/
};

typedef UInt32                          UCCharPropertyValue;
/* General Category enumeration values (requested by kUCCharPropTypeGenlCategory)*/
enum {
                                                                /* Normative categories:*/
    kUCGenlCatOtherNotAssigned  = 0,                            /* Cn Other, Not Assigned*/
    kUCGenlCatOtherControl      = 1,                            /* Cc Other, Control*/
    kUCGenlCatOtherFormat       = 2,                            /* Cf Other, Format*/
    kUCGenlCatOtherSurrogate    = 3,                            /* Cs Other, Surrogate*/
    kUCGenlCatOtherPrivateUse   = 4,                            /* Co Other, Private Use*/
    kUCGenlCatMarkNonSpacing    = 5,                            /* Mn Mark, Non-Spacing*/
    kUCGenlCatMarkSpacingCombining = 6,                         /* Mc Mark, Spacing Combining*/
    kUCGenlCatMarkEnclosing     = 7,                            /* Me Mark, Enclosing*/
    kUCGenlCatNumberDecimalDigit = 8,                           /* Nd Number, Decimal Digit*/
    kUCGenlCatNumberLetter      = 9,                            /* Nl Number, Letter*/
    kUCGenlCatNumberOther       = 10,                           /* No Number, Other*/
    kUCGenlCatSeparatorSpace    = 11,                           /* Zs Separator, Space*/
    kUCGenlCatSeparatorLine     = 12,                           /* Zl Separator, Line*/
    kUCGenlCatSeparatorParagraph = 13,                          /* Zp Separator, Paragraph*/
                                                                /* Informative categories:*/
    kUCGenlCatLetterUppercase   = 14,                           /* Lu Letter, Uppercase*/
    kUCGenlCatLetterLowercase   = 15,                           /* Ll Letter, Lowercase*/
    kUCGenlCatLetterTitlecase   = 16,                           /* Lt Letter, Titlecase*/
    kUCGenlCatLetterModifier    = 17,                           /* Lm Letter, Modifier*/
    kUCGenlCatLetterOther       = 18,                           /* Lo Letter, Other*/
    kUCGenlCatPunctConnector    = 20,                           /* Pc Punctuation, Connector*/
    kUCGenlCatPunctDash         = 21,                           /* Pd Punctuation, Dash*/
    kUCGenlCatPunctOpen         = 22,                           /* Ps Punctuation, Open*/
    kUCGenlCatPunctClose        = 23,                           /* Pe Punctuation, Close*/
    kUCGenlCatPunctInitialQuote = 24,                           /* Pi Punctuation, Initial quote*/
    kUCGenlCatPunctFinalQuote   = 25,                           /* Pf Punctuation, Final quote*/
    kUCGenlCatPunctOther        = 26,                           /* Po Punctuation, Other*/
    kUCGenlCatSymbolMath        = 28,                           /* Sm Symbol, Math*/
    kUCGenlCatSymbolCurrency    = 29,                           /* Sc Symbol, Currency*/
    kUCGenlCatSymbolModifier    = 30,                           /* Sk Symbol, Modifier*/
    kUCGenlCatSymbolOther       = 31                            /* So Symbol, Other*/
};

/* Bidirectional Category enumeration values (requested by kUCCharPropTypeBidiCategory)*/
enum {
    kUCBidiCatNotApplicable     = 0,                            /* for now use this for unassigned*/
                                                                /* Strong types:*/
    kUCBidiCatLeftRight         = 1,                            /* L  Left-Right*/
    kUCBidiCatRightLeft         = 2,                            /* R  Right-Left*/
                                                                /* Weak types:*/
    kUCBidiCatEuroNumber        = 3,                            /* EN European Number*/
    kUCBidiCatEuroNumberSeparator = 4,                          /* ES European Number Separator*/
    kUCBidiCatEuroNumberTerminator = 5,                         /* ET European Number Terminator*/
    kUCBidiCatArabicNumber      = 6,                            /* AN Arabic Number*/
    kUCBidiCatCommonNumberSeparator = 7,                        /* CS Common Number Separator*/
                                                                /* Separators:*/
    kUCBidiCatBlockSeparator    = 8,                            /* B  Block Separator*/
    kUCBidiCatSegmentSeparator  = 9,                            /* S  Segment Separator*/
                                                                /* Neutrals:*/
    kUCBidiCatWhitespace        = 10,                           /* WS Whitespace*/
    kUCBidiCatOtherNeutral      = 11                            /* ON Other Neutrals (unassigned codes could use this)*/
};

/* Prototypes for TextEncoding functions*/

EXTERN_API( TextEncoding )
CreateTextEncoding              (TextEncodingBase       encodingBase,
                                 TextEncodingVariant    encodingVariant,
                                 TextEncodingFormat     encodingFormat);

EXTERN_API( TextEncodingBase )
GetTextEncodingBase             (TextEncoding           encoding);

EXTERN_API( TextEncodingVariant )
GetTextEncodingVariant          (TextEncoding           encoding);

EXTERN_API( TextEncodingFormat )
GetTextEncodingFormat           (TextEncoding           encoding);

EXTERN_API( TextEncoding )
ResolveDefaultTextEncoding      (TextEncoding           encoding);

EXTERN_API_C( OSStatus )
GetTextEncodingName             (TextEncoding           iEncoding,
                                 TextEncodingNameSelector  iNamePartSelector,
                                 RegionCode             iPreferredRegion,
                                 TextEncoding           iPreferredEncoding,
                                 ByteCount              iOutputBufLen,
                                 ByteCount *            oNameLength,
                                 RegionCode *           oActualRegion, /* can be NULL */
                                 TextEncoding *         oActualEncoding, /* can be NULL */
                                 TextPtr                oEncodingName);

EXTERN_API( OSStatus )
TECGetInfo                      (TECInfoHandle *        tecInfo);


EXTERN_API( OSStatus )
UpgradeScriptInfoToTextEncoding (ScriptCode             iTextScriptID,
                                 LangCode               iTextLanguageID,
                                 RegionCode             iRegionID,
                                 ConstStr255Param       iTextFontname,
                                 TextEncoding *         oEncoding);

EXTERN_API( OSStatus )
RevertTextEncodingToScriptInfo  (TextEncoding           iEncoding,
                                 ScriptCode *           oTextScriptID,
                                 LangCode *             oTextLanguageID, /* can be NULL */
                                 Str255                 oTextFontname) /* can be NULL */;

EXTERN_API( OSStatus )
NearestMacTextEncodings         (TextEncoding           generalEncoding,
                                 TextEncoding *         bestMacEncoding,
                                 TextEncoding *         alternateMacEncoding);

EXTERN_API_C( OSStatus )
UCGetCharProperty               (const UniChar *        charPtr,
                                 UniCharCount           textLength,
                                 UCCharPropertyType     propType,
                                 UCCharPropertyValue *  propValue);


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

#endif /* __TEXTCOMMON__ */

