/*
     File:       CFStringEncodingExt.h
 
     Contains:   CoreFoundation string encodings
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFSTRINGENCODINGEXT__
#define __CFSTRINGENCODINGEXT__

#ifndef __CFBASE__
#include <CFBase.h>
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
                                                                /*  kCFStringEncodingMacRoman = 0L,     */
    kCFStringEncodingMacJapanese = 1,
    kCFStringEncodingMacChineseTrad = 2,
    kCFStringEncodingMacKorean  = 3,
    kCFStringEncodingMacArabic  = 4,
    kCFStringEncodingMacHebrew  = 5,
    kCFStringEncodingMacGreek   = 6,
    kCFStringEncodingMacCyrillic = 7,
    kCFStringEncodingMacDevanagari = 9,
    kCFStringEncodingMacGurmukhi = 10,
    kCFStringEncodingMacGujarati = 11,
    kCFStringEncodingMacOriya   = 12,
    kCFStringEncodingMacBengali = 13,
    kCFStringEncodingMacTamil   = 14,
    kCFStringEncodingMacTelugu  = 15,
    kCFStringEncodingMacKannada = 16,
    kCFStringEncodingMacMalayalam = 17,
    kCFStringEncodingMacSinhalese = 18,
    kCFStringEncodingMacBurmese = 19,
    kCFStringEncodingMacKhmer   = 20,
    kCFStringEncodingMacThai    = 21,
    kCFStringEncodingMacLaotian = 22,
    kCFStringEncodingMacGeorgian = 23,
    kCFStringEncodingMacArmenian = 24,
    kCFStringEncodingMacChineseSimp = 25,
    kCFStringEncodingMacTibetan = 26,
    kCFStringEncodingMacMongolian = 27,
    kCFStringEncodingMacEthiopic = 28,
    kCFStringEncodingMacCentralEurRoman = 29,
    kCFStringEncodingMacVietnamese = 30,
    kCFStringEncodingMacExtArabic = 31,                         /* The following use script code 0, smRoman*/
    kCFStringEncodingMacSymbol  = 33,
    kCFStringEncodingMacDingbats = 34,
    kCFStringEncodingMacTurkish = 35,
    kCFStringEncodingMacCroatian = 36,
    kCFStringEncodingMacIcelandic = 37,
    kCFStringEncodingMacRomanian = 38,                          /* The following use script code 4, smArabic*/
    kCFStringEncodingMacFarsi   = 0x8C,                         /* Like MacArabic but uses Farsi digits*/
                                                                /* The following use script code 7, smCyrillic*/
    kCFStringEncodingMacUkrainian = 0x98,                       /* The following use script code 32, smUnimplemented*/
    kCFStringEncodingMacVT100   = 0xFC,                         /* VT100/102 font from Comm Toolbox: Latin-1 repertoire + box drawing etc*/
                                                                /* Special Mac OS encodings*/
    kCFStringEncodingMacHFS     = 0xFF,                         /* Meta-value, should never appear in a table.*/
                                                                /* Unicode & ISO UCS encodings begin at 0x100*/
                                                                /* We don't use Unicode variations defined in TextEncoding.  Use ones in CFString.h, instead */
                                                                /* ISO 8-bit and 7-bit encodings begin at 0x200*/
                                                                /*  kCFStringEncodingISOLatin1 = 0x0201,   */
    kCFStringEncodingISOLatin2  = 0x0202,                       /* ISO 8859-2*/
    kCFStringEncodingISOLatinCyrillic = 0x0205,                 /* ISO 8859-5*/
    kCFStringEncodingISOLatinArabic = 0x0206,                   /* ISO 8859-6, = ASMO 708, =DOS CP 708*/
    kCFStringEncodingISOLatinGreek = 0x0207,                    /* ISO 8859-7*/
    kCFStringEncodingISOLatinHebrew = 0x0208,                   /* ISO 8859-8*/
    kCFStringEncodingISOLatin5  = 0x0209,                       /* ISO 8859-9*/
    kCFStringEncodingISOLatin6  = 0x020A,                       /* ISO 8859-10 */
    kCFStringEncodingISOLatinThai = 0x020B,                     /* ISO 8859-11 */
    kCFStringEncodingISOLatin7  = 0x020D,                       /* ISO 8859-13 */
    kCFStringEncodingISOLatin8  = 0x020E,                       /* ISO 8859-14 */
    kCFStringEncodingISOLatin9  = 0x020F,                       /* ISO 8859-15 */
                                                                /* MS-DOS & Windows encodings begin at 0x400*/
    kCFStringEncodingDOSLatinUS = 0x0400,                       /* code page 437*/
    kCFStringEncodingDOSGreek   = 0x0405,                       /* code page 737 (formerly code page 437G)*/
    kCFStringEncodingDOSBalticRim = 0x0406,                     /* code page 775*/
    kCFStringEncodingDOSLatin1  = 0x0410,                       /* code page 850, "Multilingual"*/
    kCFStringEncodingDOSGreek1  = 0x0411,                       /* code page 851*/
    kCFStringEncodingDOSLatin2  = 0x0412,                       /* code page 852, Slavic*/
    kCFStringEncodingDOSCyrillic = 0x0413,                      /* code page 855, IBM Cyrillic*/
    kCFStringEncodingDOSTurkish = 0x0414,                       /* code page 857, IBM Turkish*/
    kCFStringEncodingDOSPortuguese = 0x0415,                    /* code page 860*/
    kCFStringEncodingDOSIcelandic = 0x0416,                     /* code page 861*/
    kCFStringEncodingDOSHebrew  = 0x0417,                       /* code page 862*/
    kCFStringEncodingDOSCanadianFrench = 0x0418,                /* code page 863*/
    kCFStringEncodingDOSArabic  = 0x0419,                       /* code page 864*/
    kCFStringEncodingDOSNordic  = 0x041A,                       /* code page 865*/
    kCFStringEncodingDOSRussian = 0x041B,                       /* code page 866*/
    kCFStringEncodingDOSGreek2  = 0x041C,                       /* code page 869, IBM Modern Greek*/
    kCFStringEncodingDOSThai    = 0x041D,                       /* code page 874, also for Windows*/
    kCFStringEncodingDOSJapanese = 0x0420,                      /* code page 932, also for Windows*/
    kCFStringEncodingDOSChineseSimplif = 0x0421,                /* code page 936, also for Windows*/
    kCFStringEncodingDOSKorean  = 0x0422,                       /* code page 949, also for Windows; Unified Hangul Code*/
    kCFStringEncodingDOSChineseTrad = 0x0423,                   /* code page 950, also for Windows*/
                                                                /*  kCFStringEncodingWindowsLatin1 = 0x0500,  */
    kCFStringEncodingWindowsLatin2 = 0x0501,                    /* code page 1250, Central Europe*/
    kCFStringEncodingWindowsCyrillic = 0x0502,                  /* code page 1251, Slavic Cyrillic*/
    kCFStringEncodingWindowsGreek = 0x0503,                     /* code page 1253*/
    kCFStringEncodingWindowsLatin5 = 0x0504,                    /* code page 1254, Turkish*/
    kCFStringEncodingWindowsHebrew = 0x0505,                    /* code page 1255*/
    kCFStringEncodingWindowsArabic = 0x0506,                    /* code page 1256*/
    kCFStringEncodingWindowsBalticRim = 0x0507,                 /* code page 1257*/
    kCFStringEncodingWindowsKoreanJohab = 0x0510,               /* code page 1361, for Windows NT*/
    kCFStringEncodingWindowsVietnamese = 0x0508,                /* code page 1258*/
                                                                /* Various national standards begin at 0x600*/
                                                                /*  kCFStringEncodingASCII = 0x0600,  */
    kCFStringEncodingJIS_X0201_76 = 0x0620,
    kCFStringEncodingJIS_X0208_83 = 0x0621,
    kCFStringEncodingJIS_X0208_90 = 0x0622,
    kCFStringEncodingJIS_X0212_90 = 0x0623,
    kCFStringEncodingJIS_C6226_78 = 0x0624,
    kCFStringEncodingGB_2312_80 = 0x0630,
    kCFStringEncodingGBK_95     = 0x0631,                       /* annex to GB 13000-93; for Windows 95*/
    kCFStringEncodingKSC_5601_87 = 0x0640,                      /* same as KSC 5601-92 without Johab annex*/
    kCFStringEncodingKSC_5601_92_Johab = 0x0641,                /* KSC 5601-92 Johab annex*/
    kCFStringEncodingCNS_11643_92_P1 = 0x0651,                  /* CNS 11643-1992 plane 1*/
    kCFStringEncodingCNS_11643_92_P2 = 0x0652,                  /* CNS 11643-1992 plane 2*/
    kCFStringEncodingCNS_11643_92_P3 = 0x0653,                  /* CNS 11643-1992 plane 3 (was plane 14 in 1986 version)*/
                                                                /* ISO 2022 collections begin at 0x800*/
    kCFStringEncodingISO_2022_JP = 0x0820,
    kCFStringEncodingISO_2022_JP_2 = 0x0821,
    kCFStringEncodingISO_2022_CN = 0x0830,
    kCFStringEncodingISO_2022_CN_EXT = 0x0831,
    kCFStringEncodingISO_2022_KR = 0x0840,                      /* EUC collections begin at 0x900*/
    kCFStringEncodingEUC_JP     = 0x0920,                       /* ISO 646, 1-byte katakana, JIS 208, JIS 212*/
    kCFStringEncodingEUC_CN     = 0x0930,                       /* ISO 646, GB 2312-80*/
    kCFStringEncodingEUC_TW     = 0x0931,                       /* ISO 646, CNS 11643-1992 Planes 1-16*/
    kCFStringEncodingEUC_KR     = 0x0940,                       /* ISO 646, KS C 5601-1987*/
                                                                /* Misc standards begin at 0xA00*/
    kCFStringEncodingShiftJIS   = 0x0A01,                       /* plain Shift-JIS*/
    kCFStringEncodingKOI8_R     = 0x0A02,                       /* Russian internet standard*/
    kCFStringEncodingBig5       = 0x0A03,                       /* Big-5 (has variants)*/
    kCFStringEncodingMacRomanLatin1 = 0x0A04,                   /* Mac OS Roman permuted to align with ISO Latin-1*/
    kCFStringEncodingHZ_GB_2312 = 0x0A05,                       /* HZ (RFC 1842, for Chinese mail & news)*/
    kCFStringEncodingAutoDetect_JP = 0x0AFE,                    /* Auto Detect (Japanese) */
                                                                /* Other platform encodings*/
                                                                /*  kCFStringEncodingNextStepLatin = 0x0B01,  */
                                                                /* EBCDIC & IBM host encodings begin at 0xC00*/
    kCFStringEncodingEBCDIC_US  = 0x0C01,                       /* basic EBCDIC-US*/
    kCFStringEncodingEBCDIC_CP037 = 0x0C02                      /* code page 037, extended EBCDIC (Latin-1 set) for US,Canada...*/
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

#endif /* __CFSTRINGENCODINGEXT__ */

