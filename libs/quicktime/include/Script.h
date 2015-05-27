/*
     File:       Script.h
 
     Contains:   Script Manager interfaces
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1986-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SCRIPT__
#define __SCRIPT__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __INTLRESOURCES__
#include <IntlResources.h>
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

/* Meta script codes:*/
enum {
    smSystemScript              = -1,                           /* designates system script.*/
    smCurrentScript             = -2,                           /* designates current font script.*/
    smAllScripts                = -3                            /* designates any script*/
};

/*
   Script codes:
   These specify a Mac OS encoding that is related to a FOND ID range.
   Some of the encodings have several variants (e.g. for different localized systems)
    which all share the same script code.
   Not all of these script codes are currently supported by Apple software.
   Notes:
   - Script code 0 (smRoman) is also used (instead of smGreek) for the Greek encoding
     in the Greek localized system.
   - Script code 28 (smEthiopic) is also used for the Inuit encoding in the Inuktitut
     system.
*/
enum {
    smRoman                     = 0,
    smJapanese                  = 1,
    smTradChinese               = 2,                            /* Traditional Chinese*/
    smKorean                    = 3,
    smArabic                    = 4,
    smHebrew                    = 5,
    smGreek                     = 6,
    smCyrillic                  = 7,
    smRSymbol                   = 8,                            /* Right-left symbol*/
    smDevanagari                = 9,
    smGurmukhi                  = 10,
    smGujarati                  = 11,
    smOriya                     = 12,
    smBengali                   = 13,
    smTamil                     = 14,
    smTelugu                    = 15,
    smKannada                   = 16,                           /* Kannada/Kanarese*/
    smMalayalam                 = 17,
    smSinhalese                 = 18,
    smBurmese                   = 19,
    smKhmer                     = 20,                           /* Khmer/Cambodian*/
    smThai                      = 21,
    smLao                       = 22,
    smGeorgian                  = 23,
    smArmenian                  = 24,
    smSimpChinese               = 25,                           /* Simplified Chinese*/
    smTibetan                   = 26,
    smMongolian                 = 27,
    smEthiopic                  = 28,
    smGeez                      = 28,                           /* Synonym for smEthiopic*/
    smCentralEuroRoman          = 29,                           /* For Czech, Slovak, Polish, Hungarian, Baltic langs*/
    smVietnamese                = 30,
    smExtArabic                 = 31,                           /* extended Arabic*/
    smUninterp                  = 32                            /* uninterpreted symbols, e.g. palette symbols*/
};

/* Extended script code for full Unicode input*/
enum {
    smUnicodeScript             = 0x7E
};

/* Obsolete script code names (kept for backward compatibility):*/
enum {
    smChinese                   = 2,                            /* (Use smTradChinese or smSimpChinese)*/
    smRussian                   = 7,                            /* Use smCyrillic*/
                                                                /* smMaldivian = 25: deleted, no code for Maldivian*/
    smLaotian                   = 22,                           /* Use smLao                                     */
    smAmharic                   = 28,                           /* Use smEthiopic or smGeez*/
    smSlavic                    = 29,                           /* Use smCentralEuroRoman*/
    smEastEurRoman              = 29,                           /* Use smCentralEuroRoman*/
    smSindhi                    = 31,                           /* Use smExtArabic*/
    smKlingon                   = 32
};

/*
   Language codes:
   These specify a language implemented using a particular Mac OS encoding.
   Not all of these language codes are currently supported by Apple software.
*/
enum {
    langEnglish                 = 0,                            /* smRoman script*/
    langFrench                  = 1,                            /* smRoman script*/
    langGerman                  = 2,                            /* smRoman script*/
    langItalian                 = 3,                            /* smRoman script*/
    langDutch                   = 4,                            /* smRoman script*/
    langSwedish                 = 5,                            /* smRoman script*/
    langSpanish                 = 6,                            /* smRoman script*/
    langDanish                  = 7,                            /* smRoman script*/
    langPortuguese              = 8,                            /* smRoman script*/
    langNorwegian               = 9,                            /* smRoman script*/
    langHebrew                  = 10,                           /* smHebrew script*/
    langJapanese                = 11,                           /* smJapanese script*/
    langArabic                  = 12,                           /* smArabic script*/
    langFinnish                 = 13,                           /* smRoman script*/
    langGreek                   = 14,                           /* Greek script using smRoman script code*/
    langIcelandic               = 15,                           /* modified smRoman/Icelandic script*/
    langMaltese                 = 16,                           /* Roman script*/
    langTurkish                 = 17,                           /* modified smRoman/Turkish script*/
    langCroatian                = 18,                           /* modified smRoman/Croatian script*/
    langTradChinese             = 19,                           /* Chinese (Mandarin) in traditional characters*/
    langUrdu                    = 20,                           /* smArabic script*/
    langHindi                   = 21,                           /* smDevanagari script*/
    langThai                    = 22,                           /* smThai script*/
    langKorean                  = 23                            /* smKorean script*/
};

enum {
    langLithuanian              = 24,                           /* smCentralEuroRoman script*/
    langPolish                  = 25,                           /* smCentralEuroRoman script*/
    langHungarian               = 26,                           /* smCentralEuroRoman script*/
    langEstonian                = 27,                           /* smCentralEuroRoman script*/
    langLatvian                 = 28,                           /* smCentralEuroRoman script*/
    langSami                    = 29,                           /* language of the Sami people of N. Scandinavia             */
    langFaroese                 = 30,                           /* modified smRoman/Icelandic script                      */
    langFarsi                   = 31,                           /* modified smArabic/Farsi script*/
    langPersian                 = 31,                           /* Synonym for langFarsi*/
    langRussian                 = 32,                           /* smCyrillic script*/
    langSimpChinese             = 33,                           /* Chinese (Mandarin) in simplified characters*/
    langFlemish                 = 34,                           /* smRoman script*/
    langIrishGaelic             = 35,                           /* smRoman or modified smRoman/Celtic script (without dot above)   */
    langAlbanian                = 36,                           /* smRoman script*/
    langRomanian                = 37,                           /* modified smRoman/Romanian script*/
    langCzech                   = 38,                           /* smCentralEuroRoman script*/
    langSlovak                  = 39,                           /* smCentralEuroRoman script*/
    langSlovenian               = 40,                           /* modified smRoman/Croatian script*/
    langYiddish                 = 41,                           /* smHebrew script*/
    langSerbian                 = 42,                           /* smCyrillic script*/
    langMacedonian              = 43,                           /* smCyrillic script*/
    langBulgarian               = 44,                           /* smCyrillic script*/
    langUkrainian               = 45,                           /* modified smCyrillic/Ukrainian script*/
    langByelorussian            = 46,                           /* smCyrillic script*/
    langBelorussian             = 46                            /* Synonym for langByelorussian                          */
};

enum {
    langUzbek                   = 47,                           /* Cyrillic script*/
    langKazakh                  = 48,                           /* Cyrillic script*/
    langAzerbaijani             = 49,                           /* Azerbaijani in Cyrillic script*/
    langAzerbaijanAr            = 50,                           /* Azerbaijani in Arabic script*/
    langArmenian                = 51,                           /* smArmenian script*/
    langGeorgian                = 52,                           /* smGeorgian script*/
    langMoldavian               = 53,                           /* smCyrillic script*/
    langKirghiz                 = 54,                           /* Cyrillic script*/
    langTajiki                  = 55,                           /* Cyrillic script*/
    langTurkmen                 = 56,                           /* Cyrillic script*/
    langMongolian               = 57,                           /* Mongolian in smMongolian script*/
    langMongolianCyr            = 58,                           /* Mongolian in Cyrillic script*/
    langPashto                  = 59,                           /* Arabic script*/
    langKurdish                 = 60,                           /* smArabic script*/
    langKashmiri                = 61,                           /* Arabic script*/
    langSindhi                  = 62,                           /* Arabic script*/
    langTibetan                 = 63,                           /* smTibetan script*/
    langNepali                  = 64,                           /* smDevanagari script*/
    langSanskrit                = 65,                           /* smDevanagari script*/
    langMarathi                 = 66,                           /* smDevanagari script*/
    langBengali                 = 67,                           /* smBengali script*/
    langAssamese                = 68,                           /* smBengali script*/
    langGujarati                = 69,                           /* smGujarati script*/
    langPunjabi                 = 70                            /* smGurmukhi script*/
};

enum {
    langOriya                   = 71,                           /* smOriya script*/
    langMalayalam               = 72,                           /* smMalayalam script*/
    langKannada                 = 73,                           /* smKannada script*/
    langTamil                   = 74,                           /* smTamil script*/
    langTelugu                  = 75,                           /* smTelugu script*/
    langSinhalese               = 76,                           /* smSinhalese script*/
    langBurmese                 = 77,                           /* smBurmese script*/
    langKhmer                   = 78,                           /* smKhmer script*/
    langLao                     = 79,                           /* smLao script*/
    langVietnamese              = 80,                           /* smVietnamese script*/
    langIndonesian              = 81,                           /* smRoman script*/
    langTagalog                 = 82,                           /* Roman script*/
    langMalayRoman              = 83,                           /* Malay in smRoman script*/
    langMalayArabic             = 84,                           /* Malay in Arabic script*/
    langAmharic                 = 85,                           /* smEthiopic script*/
    langTigrinya                = 86,                           /* smEthiopic script*/
    langOromo                   = 87,                           /* smEthiopic script*/
    langSomali                  = 88,                           /* smRoman script*/
    langSwahili                 = 89,                           /* smRoman script*/
    langKinyarwanda             = 90,                           /* smRoman script*/
    langRuanda                  = 90,                           /* synonym for langKinyarwanda*/
    langRundi                   = 91,                           /* smRoman script*/
    langNyanja                  = 92,                           /* smRoman script*/
    langChewa                   = 92,                           /* synonym for langNyanja*/
    langMalagasy                = 93,                           /* smRoman script*/
    langEsperanto               = 94                            /* Roman script*/
};

enum {
    langWelsh                   = 128,                          /* modified smRoman/Celtic script*/
    langBasque                  = 129,                          /* smRoman script*/
    langCatalan                 = 130,                          /* smRoman script*/
    langLatin                   = 131,                          /* smRoman script*/
    langQuechua                 = 132,                          /* smRoman script*/
    langGuarani                 = 133,                          /* smRoman script*/
    langAymara                  = 134,                          /* smRoman script*/
    langTatar                   = 135,                          /* Cyrillic script*/
    langUighur                  = 136,                          /* Arabic script*/
    langDzongkha                = 137,                          /* (lang of Bhutan) smTibetan script*/
    langJavaneseRom             = 138,                          /* Javanese in smRoman script*/
    langSundaneseRom            = 139,                          /* Sundanese in smRoman script*/
    langGalician                = 140,                          /* smRoman script*/
    langAfrikaans               = 141                           /* smRoman script                                   */
};

enum {
    langBreton                  = 142,                          /* smRoman or modified smRoman/Celtic script                 */
    langInuktitut               = 143,                          /* Inuit script using smEthiopic script code                 */
    langScottishGaelic          = 144,                          /* smRoman or modified smRoman/Celtic script                 */
    langManxGaelic              = 145,                          /* smRoman or modified smRoman/Celtic script                 */
    langIrishGaelicScript       = 146,                          /* modified smRoman/Gaelic script (using dot above)               */
    langTongan                  = 147,                          /* smRoman script                                   */
    langGreekPoly               = 148,                          /* smGreek script                                   */
    langGreenlandic             = 149                           /* smRoman script                                   */
};

enum {
    langUnspecified             = 32767                         /* Special code for use in resources (such as 'itlm')           */
};

/*
   Obsolete language code names (kept for backward compatibility):
   Misspelled, ambiguous, misleading, considered pejorative, archaic, etc.
*/
enum {
    langPortugese               = 8,                            /* Use langPortuguese*/
    langMalta                   = 16,                           /* Use langMaltese*/
    langYugoslavian             = 18,                           /* (use langCroatian, langSerbian, etc.)*/
    langChinese                 = 19,                           /* (use langTradChinese or langSimpChinese)*/
    langLettish                 = 28,                           /* Use langLatvian                                     */
    langLapponian               = 29,                           /* Use langSami*/
    langLappish                 = 29,                           /* Use langSami*/
    langSaamisk                 = 29,                           /* Use langSami                                    */
    langFaeroese                = 30,                           /* Use langFaroese                                     */
    langIrish                   = 35,                           /* Use langIrishGaelic                                  */
    langGalla                   = 87,                           /* Use langOromo                                 */
    langAfricaans               = 141                           /* Use langAfrikaans                                */
};

/*
   Region codes:
   These typically specify a combination of a language code and a particular region.
   Some of these numeric values are reserved just for extra resource IDs associated
   with certain regions; these are not actual region codes, and are noted in the comments.
   Not all of the region codes are currently supported by Apple software.
   When relevant, the following list also provides:
   - The Apple part number (P/N) code for software localized for the specified region.
   - The two-letter ISO language and country codes (from ISO 639 and ISO 3166). The
     language code (lowercase) is first, then '_', then the country code (uppercase).
*/

enum {
                                                                /* P/N    ISO    codes  comments*/
    verUS                       = 0,                            /*       en_US*/
    verFrance                   = 1,                            /* F  fr_FR*/
    verBritain                  = 2,                            /* B  en_GB*/
    verGermany                  = 3,                            /* D  de_DE*/
    verItaly                    = 4,                            /* T  it_IT*/
    verNetherlands              = 5,                            /* N  nl_NL*/
    verFlemish                  = 6,                            /* FN nl_BE     Flemish (Dutch) for Belgium                 */
    verSweden                   = 7,                            /* S  sv_SE*/
    verSpain                    = 8,                            /* E  es_ES       Spanish for Spain*/
    verDenmark                  = 9,                            /* DK da_DK*/
    verPortugal                 = 10,                           /* PO pt_PT     Portuguese for Portugal*/
    verFrCanada                 = 11,                           /* C  fr_CA       French for Canada*/
    verNorway                   = 12,                           /* H  no_NO       Bokm.l*/
    verIsrael                   = 13,                           /* HB iw_IL     Hebrew*/
    verJapan                    = 14,                           /* J  ja_JP*/
    verAustralia                = 15,                           /* X  en_AU       English for Australia*/
    verArabic                   = 16,                           /* AB ar       Arabic for N Africa, Arabian peninsula, Levant*/
    verFinland                  = 17,                           /* K  fi_FI*/
    verFrSwiss                  = 18,                           /* SF fr_CH     French Swiss*/
    verGrSwiss                  = 19,                           /* SD de_CH     German Swiss*/
    verGreece                   = 20,                           /* GR el_GR     Monotonic Greek (modern)*/
    verIceland                  = 21,                           /* IS is_IS*/
    verMalta                    = 22,                           /* MA mt_MT*/
    verCyprus                   = 23,                           /* CY   _CY     Greek or Turkish language? Checking...*/
    verTurkey                   = 24,                           /* TU tr_TR*/
    verYugoCroatian             = 25                            /* YU          Croatian for Yugoslavia; now use verCroatia (68)*/
};

enum {
    verNetherlandsComma         = 26,                           /*              ID for KCHR resource - Dutch*/
    verBelgiumLuxPoint          = 27,                           /*              ID for KCHR resource - Belgium*/
    verCanadaComma              = 28,                           /*              ID for KCHR resource - Canadian ISO*/
    verCanadaPoint              = 29,                           /*              ID for KCHR resource - Canadian; now unused*/
    vervariantPortugal          = 30,                           /*              ID for resource; now unused*/
    vervariantNorway            = 31,                           /*              ID for resource; now unused*/
    vervariantDenmark           = 32                            /*              ID for KCHR resource - Danish Mac Plus*/
};

enum {
    verIndiaHindi               = 33,                           /*       hi_IN     Hindi for India*/
    verPakistanUrdu             = 34,                           /* UR ur_PK     Urdu for Pakistan                        */
    verTurkishModified          = 35,
    verItalianSwiss             = 36,                           /* ST it_CH     Italian Swiss*/
    verInternational            = 37,                           /* Z  en      English for international use               */
                                                                /*              38 is unassigned*/
    verRomania                  = 39,                           /* RO ro_RO*/
    verGreecePoly               = 40,                           /*              Polytonic Greek (classical)                   */
    verLithuania                = 41,                           /* LT lt_LT*/
    verPoland                   = 42,                           /* PL pl_PL*/
    verHungary                  = 43,                           /* MG hu_HU*/
    verEstonia                  = 44,                           /* EE et_EE*/
    verLatvia                   = 45,                           /* LV lv_LV*/
    verSami                     = 46,                           /*       se                                        */
    verFaroeIsl                 = 47,                           /* FA fo_FO                                    */
    verIran                     = 48,                           /* PS fa_IR     Persian/Farsi*/
    verRussia                   = 49,                           /* RS ru_RU     Russian*/
    verIreland                  = 50,                           /* GA ga_IE     Irish Gaelic for Ireland (without dot above)     */
    verKorea                    = 51,                           /* KH ko_KR*/
    verChina                    = 52,                           /* CH zh_CN     Simplified Chinese*/
    verTaiwan                   = 53,                           /* TA zh_TW     Traditional Chinese*/
    verThailand                 = 54,                           /* TH th_TH*/
    verScriptGeneric            = 55,                           /* SS          Generic script system (no language or script)       */
    verCzech                    = 56,                           /* CZ cs_CZ*/
    verSlovak                   = 57,                           /* SL sk_SK*/
    verFarEastGeneric           = 58,                           /* FE          Generic Far East system (no language or script)   */
    verMagyar                   = 59,                           /*              Unused; see verHungary*/
    verBengali                  = 60,                           /*       bn         Bangladesh or India*/
    verByeloRussian             = 61                            /* BY be_BY*/
};

enum {
    verUkraine                  = 62,                           /* UA uk_UA*/
                                                                /*              63 is unassigned*/
    verGreeceAlt                = 64,                           /*              unused                              */
    verSerbian                  = 65,                           /* SR sr_YU, sh_YU                                  */
    verSlovenian                = 66,                           /* SV sl_SI                                    */
    verMacedonian               = 67,                           /* MD mk_MK                                    */
    verCroatia                  = 68,                           /* CR hr_HR, sh_HR*/
                                                                /*              69 is unassigned*/
                                                                /*              70 is unassigned*/
    verBrazil                   = 71,                           /* BR pt_BR     Portuguese for Brazil*/
    verBulgaria                 = 72,                           /* BG bg_BG*/
    verCatalonia                = 73,                           /* CA ca_ES     Catalan for Spain*/
    verMultilingual             = 74,                           /* ZM          (no language or script)*/
    verScottishGaelic           = 75,                           /* GD gd*/
    verManxGaelic               = 76,                           /* GV gv       Isle of Man*/
    verBreton                   = 77,                           /* BZ br*/
    verNunavut                  = 78,                           /* IU iu_CA     Inuktitut for Canada*/
    verWelsh                    = 79,                           /* CU cy*/
                                                                /*              80 is ID for KCHR resource - Canadian CSA*/
    verIrishGaelicScript        = 81,                           /* GS ga_IE     Irish Gaelic for Ireland (using dot above)*/
    verEngCanada                = 82,                           /* V  en_CA       English for Canada*/
    verBhutan                   = 83,                           /* BH dz_BT     Dzongkha for Bhutan*/
    verArmenian                 = 84,                           /* HY hy_AM*/
    verGeorgian                 = 85,                           /* KR ka_GE*/
    verSpLatinAmerica           = 86,                           /* LA es       Spanish for Latin America*/
                                                                /*              87 is ID for KCHR resource - Spanish ISO*/
    verTonga                    = 88,                           /* TS to_TO*/
                                                                /*              89 is ID for KCHR resource - Polish Modified*/
                                                                /*              90 is ID for KCHR resource - Catalan ISO*/
    verFrenchUniversal          = 91,                           /*       fr         French generic*/
    verAustria                  = 92,                           /* AU de_AT     German for Austria*/
                                                                /* Y          93 is unused alternate for verSpLatinAmerica*/
    verGujarati                 = 94,                           /*       gu_IN*/
    verPunjabi                  = 95,                           /*       pa         Pakistan or India*/
    verIndiaUrdu                = 96,                           /*       ur_IN     Urdu for India*/
    verVietnam                  = 97                            /*       vi_VN*/
};

enum {
    verFrBelgium                = 98,                           /* BF fr_BE     French for Belgium                       */
    verUzbek                    = 99,                           /* BD uz_UZ                                    */
    verSingapore                = 100,                          /* SG                                         */
    verNynorsk                  = 101,                          /* NY   _NO     Norwegian Nynorsk                        */
    verAfrikaans                = 102,                          /* AK af_ZA                                    */
    verEsperanto                = 103,                          /*       eo                                        */
    verMarathi                  = 104,                          /*       mr_IN                                      */
    verTibetan                  = 105,                          /*       bo                                        */
    verNepal                    = 106,                          /*       ne_NP                                      */
    verGreenland                = 107                           /*       kl                                        */
};

/*
   Other extra resource IDs assigned in the same number space:
    179 is ID for KCHR & itl_ resources - Cornish
    581 is ID for KCHR resource - Irish Gaelic script alternate
    582 is ID for KCHR resource - Ogham
    779 is ID for KCHR resource - Welsh alternate
   1111 is ID for KCHR resource - French numeric
*/

/*
   Obsolete region code names (kept for backward compatibility):
   Misspelled or alternate form, ambiguous, misleading, considered pejorative, archaic, etc.
*/
enum {
    verFrBelgiumLux             = 6,                            /* Incorrect; 6 is Flemish, not French, for Belgium; use verFlemish     */
    verBelgiumLux               = 6,                            /* Use verFlemish*/
    verArabia                   = 16,                           /* Use verArabic*/
    verYugoslavia               = 25,                           /* Use verYugoCroatian (same number, now unused), or newer verCroatia*/
    verIndia                    = 33,                           /* Use verIndiaHindi*/
    verPakistan                 = 34,                           /* Use verPakistanUrdu                                  */
    verRumania                  = 39,                           /* Alternate for verRomania                              */
    verGreekAncient             = 40,                           /* Use verGreecePoly                                */
    verLapland                  = 46,                           /* Use verSami                                       */
    verFaeroeIsl                = 47,                           /* Use verFaroeIsl                                     */
    verGenericFE                = 58,                           /* Use verFarEastGeneric                              */
    verBelarus                  = 61,                           /* Alternate for verByelorussian                       */
    verUkrania                  = 62,                           /* Use verUkraine*/
    verAlternateGr              = 64,                           /* Use verGreeceAlt                                   */
    verSerbia                   = 65,                           /* Alternate for verSerbian                              */
    verSlovenia                 = 66,                           /* Alternate for verSlovenian                            */
    verMacedonia                = 67,                           /* Alternate for verMacedonian                             */
    verBrittany                 = 77,                           /* Alternate for verBreton                              */
    verWales                    = 79,                           /* Alternate for verWelsh                              */
    verArmenia                  = 84,                           /* Alternate for verArmenian                           */
    verGeorgia                  = 85,                           /* Alternate for verGeorgian                           */
    verAustriaGerman            = 92,                           /* Use verAustria                                   */
    verTibet                    = 105                           /* Use verTibetan                                   */
};

enum {
    minCountry                  = verUS,
    maxCountry                  = verGreenland
};

enum {
                                                                /* Calendar Codes */
    calGregorian                = 0,
    calArabicCivil              = 1,
    calArabicLunar              = 2,
    calJapanese                 = 3,
    calJewish                   = 4,
    calCoptic                   = 5,
    calPersian                  = 6
};

enum {
                                                                /* Integer Format Codes */
    intWestern                  = 0,
    intArabic                   = 1,
    intRoman                    = 2,
    intJapanese                 = 3,
    intEuropean                 = 4,
    intOutputMask               = 0x8000
};

enum {
                                                                /* CharByte byte types */
    smSingleByte                = 0,
    smFirstByte                 = -1,
    smLastByte                  = 1,
    smMiddleByte                = 2
};

enum {
                                                                /* CharType field masks */
    smcTypeMask                 = 0x000F,
    smcReserved                 = 0x00F0,
    smcClassMask                = 0x0F00,
    smcOrientationMask          = 0x1000,                       /*two-byte script glyph orientation*/
    smcRightMask                = 0x2000,
    smcUpperMask                = 0x4000,
    smcDoubleMask               = 0x8000
};

enum {
                                                                /* Basic CharType character types */
    smCharPunct                 = 0x0000,
    smCharAscii                 = 0x0001,
    smCharEuro                  = 0x0007,
    smCharExtAscii              = 0x0007,                       /* More correct synonym for smCharEuro */
                                                                /* Additional CharType character types for script systems */
    smCharKatakana              = 0x0002,                       /*Japanese Katakana*/
    smCharHiragana              = 0x0003,                       /*Japanese Hiragana*/
    smCharIdeographic           = 0x0004,                       /*Hanzi, Kanji, Hanja*/
    smCharTwoByteGreek          = 0x0005,                       /*2-byte Greek in Far East systems*/
    smCharTwoByteRussian        = 0x0006,                       /*2-byte Cyrillic in Far East systems*/
    smCharBidirect              = 0x0008,                       /*Arabic/Hebrew*/
    smCharContextualLR          = 0x0009,                       /*Contextual left-right: Thai, Indic scripts*/
    smCharNonContextualLR       = 0x000A,                       /*Non-contextual left-right: Cyrillic, Greek*/
    smCharHangul                = 0x000C,                       /*Korean Hangul*/
    smCharJamo                  = 0x000D,                       /*Korean Jamo*/
    smCharBopomofo              = 0x000E,                       /*Chinese Bopomofo*/
    smCharGanaKana              = 0x000F,                       /*Shared for Japanese Hiragana & Katakana*/
                                                                /* old names for some of above, for backward compatibility */
    smCharFISKana               = 0x0002,                       /*Katakana*/
    smCharFISGana               = 0x0003,                       /*Hiragana*/
    smCharFISIdeo               = 0x0004                        /*Hanzi, Kanji, Hanja*/
};

enum {
    smCharFISGreek              = 0x0005,                       /*2-byte Greek in Far East systems*/
    smCharFISRussian            = 0x0006,                       /*2-byte Cyrillic in Far East systems*/
                                                                /* CharType classes for punctuation (smCharPunct) */
    smPunctNormal               = 0x0000,
    smPunctNumber               = 0x0100,
    smPunctSymbol               = 0x0200,
    smPunctBlank                = 0x0300,                       /* Additional CharType classes for punctuation in two-byte systems */
    smPunctRepeat               = 0x0400,                       /* repeat marker */
    smPunctGraphic              = 0x0500,                       /* line graphics */
                                                                /* CharType Katakana and Hiragana classes for two-byte systems */
    smKanaSmall                 = 0x0100,                       /*small kana character*/
    smKanaHardOK                = 0x0200,                       /*can have dakuten*/
    smKanaSoftOK                = 0x0300,                       /*can have dakuten or han-dakuten*/
                                                                /* CharType Ideographic classes for two-byte systems */
    smIdeographicLevel1         = 0x0000,                       /*level 1 char*/
    smIdeographicLevel2         = 0x0100,                       /*level 2 char*/
    smIdeographicUser           = 0x0200,                       /*user char*/
                                                                /* old names for above, for backward compatibility */
    smFISClassLvl1              = 0x0000,                       /*level 1 char*/
    smFISClassLvl2              = 0x0100,                       /*level 2 char*/
    smFISClassUser              = 0x0200,                       /*user char*/
                                                                /* CharType Jamo classes for Korean systems */
    smJamoJaeum                 = 0x0000,                       /*simple consonant char*/
    smJamoBogJaeum              = 0x0100,                       /*complex consonant char*/
    smJamoMoeum                 = 0x0200,                       /*simple vowel char*/
    smJamoBogMoeum              = 0x0300                        /*complex vowel char*/
};

enum {
                                                                /* CharType glyph orientation for two-byte systems */
    smCharHorizontal            = 0x0000,                       /* horizontal character form, or for both */
    smCharVertical              = 0x1000,                       /* vertical character form */
                                                                /* CharType directions */
    smCharLeft                  = 0x0000,
    smCharRight                 = 0x2000,                       /* CharType case modifers */
    smCharLower                 = 0x0000,
    smCharUpper                 = 0x4000,                       /* CharType character size modifiers (1 or multiple bytes). */
    smChar1byte                 = 0x0000,
    smChar2byte                 = 0x8000
};

enum {
                                                                /* TransliterateText target types for Roman */
    smTransAscii                = 0,                            /*convert to ASCII*/
    smTransNative               = 1,                            /*convert to font script*/
    smTransCase                 = 0xFE,                         /*convert case for all text*/
    smTransSystem               = 0xFF,                         /*convert to system script*/
                                                                /* TransliterateText target types for two-byte scripts */
    smTransAscii1               = 2,                            /*1-byte Roman*/
    smTransAscii2               = 3,                            /*2-byte Roman*/
    smTransKana1                = 4,                            /*1-byte Japanese Katakana*/
    smTransKana2                = 5                             /*2-byte Japanese Katakana*/
};

enum {
    smTransGana2                = 7,                            /*2-byte Japanese Hiragana (no 1-byte Hiragana)*/
    smTransHangul2              = 8,                            /*2-byte Korean Hangul*/
    smTransJamo2                = 9,                            /*2-byte Korean Jamo*/
    smTransBopomofo2            = 10,                           /*2-byte Chinese Bopomofo*/
                                                                /* TransliterateText target modifiers */
    smTransLower                = 0x4000,                       /*target becomes lowercase*/
    smTransUpper                = 0x8000,                       /*target becomes uppercase*/
                                                                /* TransliterateText resource format numbers */
    smTransRuleBaseFormat       = 1,                            /*Rule based trsl resource format */
    smTransHangulFormat         = 2,                            /*Table based Hangul trsl resource format*/
                                                                /* TransliterateText property flags */
    smTransPreDoubleByting      = 1,                            /*Convert all text to double byte before transliteration*/
    smTransPreLowerCasing       = 2                             /*Convert all text to lower case before transliteration*/
};

enum {
                                                                /* TransliterateText source mask - general */
    smMaskAll                   = (long)0xFFFFFFFF,             /*Convert all text*/
                                                                /* TransliterateText source masks */
    smMaskAscii                 = 0x00000001,                   /*2^smTransAscii*/
    smMaskNative                = 0x00000002,                   /*2^smTransNative*/
                                                                /* TransliterateText source masks for two-byte scripts */
    smMaskAscii1                = 0x00000004,                   /*2^smTransAscii1*/
    smMaskAscii2                = 0x00000008,                   /*2^smTransAscii2*/
    smMaskKana1                 = 0x00000010,                   /*2^smTransKana1*/
    smMaskKana2                 = 0x00000020,                   /*2^smTransKana2*/
    smMaskGana2                 = 0x00000080,                   /*2^smTransGana2*/
    smMaskHangul2               = 0x00000100,                   /*2^smTransHangul2*/
    smMaskJamo2                 = 0x00000200,                   /*2^smTransJamo2*/
    smMaskBopomofo2             = 0x00000400                    /*2^smTransBopomofo2*/
};

enum {
                                                                /* Result values from GetScriptManagerVariable and SetScriptManagerVariable calls. */
    smNotInstalled              = 0,                            /*routine not available in script*/
    smBadVerb                   = -1,                           /*Bad verb passed to a routine*/
    smBadScript                 = -2                            /*Bad script code passed to a routine*/
};

enum {
                                                                /* Values for script redraw flag. */
    smRedrawChar                = 0,                            /*Redraw character only*/
    smRedrawWord                = 1,                            /*Redraw entire word (2-byte systems)*/
    smRedrawLine                = -1                            /*Redraw entire line (bidirectional systems)*/
};

enum {
                                                                /* GetScriptManagerVariable and SetScriptManagerVariable verbs */
    smVersion                   = 0,                            /*Script Manager version number*/
    smMunged                    = 2,                            /*Globals change count*/
    smEnabled                   = 4,                            /*Count of enabled scripts, incl Roman*/
    smBidirect                  = 6,                            /*At least one bidirectional script*/
    smFontForce                 = 8,                            /*Force font flag*/
    smIntlForce                 = 10,                           /*Force intl flag*/
    smForced                    = 12,                           /*Script was forced to system script*/
    smDefault                   = 14,                           /*Script was defaulted to Roman script*/
    smPrint                     = 16,                           /*Printer action routine*/
    smSysScript                 = 18,                           /*System script*/
    smLastScript                = 20,                           /*Last keyboard script*/
    smKeyScript                 = 22,                           /*Keyboard script*/
    smSysRef                    = 24,                           /*System folder refNum*/
    smKeyCache                  = 26,                           /*obsolete*/
    smKeySwap                   = 28,                           /*Swapping table handle*/
    smGenFlags                  = 30,                           /*General flags long*/
    smOverride                  = 32,                           /*Script override flags*/
    smCharPortion               = 34,                           /*Ch vs SpExtra proportion*/
                                                                /* New for System 7.0: */
    smDoubleByte                = 36,                           /*Flag for double-byte script installed*/
    smKCHRCache                 = 38,                           /*Returns pointer to KCHR cache*/
    smRegionCode                = 40,                           /*Returns current region code (verXxx)*/
    smKeyDisableState           = 42                            /*Returns current keyboard disable state*/
};

enum {
                                                                /* GetScriptVariable and SetScriptVariable verbs. */
                                                                /* Note: Verbs private to script systems are negative, while */
                                                                /* those general across script systems are non-negative. */
    smScriptVersion             = 0,                            /*Script software version*/
    smScriptMunged              = 2,                            /*Script entry changed count*/
    smScriptEnabled             = 4,                            /*Script enabled flag*/
    smScriptRight               = 6,                            /*Right to left flag*/
    smScriptJust                = 8,                            /*Justification flag*/
    smScriptRedraw              = 10,                           /*Word redraw flag*/
    smScriptSysFond             = 12,                           /*Preferred system font*/
    smScriptAppFond             = 14,                           /*Preferred Application font*/
    smScriptBundle              = 16,                           /*Beginning of itlb verbs*/
    smScriptNumber              = 16,                           /*Script itl0 id*/
    smScriptDate                = 18,                           /*Script itl1 id*/
    smScriptSort                = 20,                           /*Script itl2 id*/
    smScriptFlags               = 22,                           /*flags word*/
    smScriptToken               = 24,                           /*Script itl4 id*/
    smScriptEncoding            = 26,                           /*id of optional itl5, if present*/
    smScriptLang                = 28                            /*Current language for script*/
};

enum {
    smScriptNumDate             = 30,                           /*Script Number/Date formats.*/
    smScriptKeys                = 32,                           /*Script KCHR id*/
    smScriptIcon                = 34,                           /*ID # of SICN or kcs#/kcs4/kcs8 suite*/
    smScriptPrint               = 36,                           /*Script printer action routine*/
    smScriptTrap                = 38,                           /*Trap entry pointer*/
    smScriptCreator             = 40,                           /*Script file creator*/
    smScriptFile                = 42,                           /*Script file name*/
    smScriptName                = 44,                           /*Script name*/
                                                                /* There is a hole here for old Kanji private verbs 46-76 */
                                                                /* New for System 7.0: */
    smScriptMonoFondSize        = 78,                           /*default monospace FOND (hi) & size (lo)*/
    smScriptPrefFondSize        = 80,                           /*preferred FOND (hi) & size (lo)*/
    smScriptSmallFondSize       = 82,                           /*default small FOND (hi) & size (lo)*/
    smScriptSysFondSize         = 84,                           /*default system FOND (hi) & size (lo)*/
    smScriptAppFondSize         = 86,                           /*default app FOND (hi) & size (lo)*/
    smScriptHelpFondSize        = 88,                           /*default Help Mgr FOND (hi) & size (lo)*/
    smScriptValidStyles         = 90,                           /*mask of valid styles for script*/
    smScriptAliasStyle          = 92                            /*style (set) to use for aliases*/
};

/* special negative verbs for Get/SetScriptVariable that were associated with WorldScriptI */
/* move them here to be public */
enum {
                                                                /* WorldScript private verbs */
    smLayoutCache               = -309,                         /* HiWrd(param) is # entries, LoWrd is max input length*/
    smOldVerbSupport            = -311,                         /* param is added to old verbs to map to WSI verb*/
    smSetKashidas               = -291,                         /* param is ON or OFF, old verb = -36*/
    smSetKashProp               = -287,                         /* param is kashida proportion, old verb = -32*/
    smScriptSysBase             = -281,                         /* param is associated font to use w/ system font (old verb = -26)*/
    smScriptAppBase             = -283,                         /* param is associated font to use w/ app font (old verb = -28)*/
    smScriptFntBase             = -285,                         /* param is associated font to use w/ all other fonts (old verb = -30)*/
    smScriptLigatures           = -263,                         /* old verb = -8*/
    smScriptNumbers             = -267                          /* old verb = -12*/
};

enum {
                                                                /* Special script code values for International Utilities */
    iuSystemScript              = -1,                           /* <obsolete>  system script */
    iuCurrentScript             = -2                            /* <obsolete>  current script (for font of grafPort) */
};

enum {
                                                                /* Negative verbs for KeyScript */
    smKeyNextScript             = -1,                           /* Switch to next available script */
    smKeySysScript              = -2,                           /* Switch to the system script */
    smKeySwapScript             = -3,                           /* Switch to previously-used script */
                                                                /* New for System 7.0: */
    smKeyNextKybd               = -4,                           /* Switch to next keyboard in current keyscript */
    smKeySwapKybd               = -5,                           /* Switch to previously-used keyboard in current keyscript */
    smKeyDisableKybds           = -6,                           /* Disable keyboards not in system or Roman script */
    smKeyEnableKybds            = -7,                           /* Re-enable keyboards for all enabled scripts */
    smKeyToggleInline           = -8,                           /* Toggle inline input for current keyscript */
    smKeyToggleDirection        = -9,                           /* Toggle default line direction (TESysJust) */
    smKeyNextInputMethod        = -10,                          /* Switch to next input method in current keyscript */
    smKeySwapInputMethod        = -11,                          /* Switch to last-used input method in current keyscript */
    smKeyDisableKybdSwitch      = -12,                          /* Disable switching from the current keyboard */
    smKeySetDirLeftRight        = -15,                          /* Set default line dir to left-right, align left */
    smKeySetDirRightLeft        = -16,                          /* Set default line dir to right-left, align right */
    smKeyRoman                  = -17                           /* Set keyscript to Roman. Does nothing if Roman-only system, unlike KeyScript(smRoman) which forces an update to current default Roman keyboard */
};

/* Optional font and keyboard script synchronization */
enum {
                                                                /* One more flag in the smGenFlags long. */
    smfDisableKeyScriptSync     = 27                            /*Disable font and keyboard script synchronization*/
};

enum {
                                                                /* We should define masks, too. */
    smfDisableKeyScriptSyncMask = 1L << smfDisableKeyScriptSync /*Disable font and keyboard script synchronization mask*/
};

enum {
                                                                /* Force keyboard script switching flag and mask for zero and positive KeyScript verbs */
    smKeyForceKeyScriptBit      = 7,                            /* Force keyboard script switching flag */
    smKeyForceKeyScriptMask     = 1 << smKeyForceKeyScriptBit   /* its mask */
};

enum {
                                                                /* Bits in the smScriptFlags word */
                                                                /*  (bits above 8 are non-static) */
    smsfIntellCP                = 0,                            /*Script has intelligent cut & paste*/
    smsfSingByte                = 1,                            /*Script has only single bytes*/
    smsfNatCase                 = 2,                            /*Native chars have upper & lower case*/
    smsfContext                 = 3,                            /*Script is contextual*/
    smsfNoForceFont             = 4,                            /*Script will not force characters*/
    smsfB0Digits                = 5,                            /*Script has alternate digits at B0-B9*/
    smsfAutoInit                = 6,                            /*Auto initialize the script*/
    smsfUnivExt                 = 7,                            /*Script is handled by universal extension*/
    smsfSynchUnstyledTE         = 8,                            /*Script synchronizes for unstyled TE*/
    smsfForms                   = 13,                           /*Uses contextual forms for letters*/
    smsfLigatures               = 14,                           /*Uses contextual ligatures*/
    smsfReverse                 = 15,                           /*Reverses native text, right-left*/
                                                                /* Bits in the smGenFlags long. */
                                                                /* First (high-order) byte is set from itlc flags byte. */
    smfShowIcon                 = 31,                           /*Show icon even if only one script*/
    smfDualCaret                = 30,                           /*Use dual caret for mixed direction text*/
    smfNameTagEnab              = 29,                           /*Reserved for internal use*/
    smfUseAssocFontInfo         = 28                            /*Use the associated font info for FontMetrics calls <48>*/
};

enum {
                                                                /* Roman script constants */
                                                                /* The following are here for backward compatibility, but should not be used. */
                                                                /* This information should be obtained using GetScript. */
    romanSysFond                = 0x3FFF,                       /*system font id number*/
    romanAppFond                = 3,                            /*application font id number*/
    romanFlags                  = 0x0007,                       /*roman settings*/
                                                                /* Script Manager font equates. */
    smFondStart                 = 0x4000,                       /*start from 16K*/
    smFondEnd                   = 0xC000,                       /*past end of range at 48K*/
                                                                /* Miscellaneous font equates. */
    smUprHalfCharSet            = 0x80                          /*first char code in top half of std char set*/
};

enum {
                                                                /* Character Set Extensions */
    diaeresisUprY               = 0xD9,
    fraction                    = 0xDA,
    intlCurrency                = 0xDB,
    leftSingGuillemet           = 0xDC,
    rightSingGuillemet          = 0xDD,
    fiLigature                  = 0xDE,
    flLigature                  = 0xDF,
    dblDagger                   = 0xE0,
    centeredDot                 = 0xE1,
    baseSingQuote               = 0xE2,
    baseDblQuote                = 0xE3,
    perThousand                 = 0xE4,
    circumflexUprA              = 0xE5,
    circumflexUprE              = 0xE6,
    acuteUprA                   = 0xE7,
    diaeresisUprE               = 0xE8,
    graveUprE                   = 0xE9,
    acuteUprI                   = 0xEA,
    circumflexUprI              = 0xEB,
    diaeresisUprI               = 0xEC,
    graveUprI                   = 0xED,
    acuteUprO                   = 0xEE,
    circumflexUprO              = 0xEF,
    appleLogo                   = 0xF0,
    graveUprO                   = 0xF1,
    acuteUprU                   = 0xF2,
    circumflexUprU              = 0xF3,
    graveUprU                   = 0xF4,
    dotlessLwrI                 = 0xF5,
    circumflex                  = 0xF6,
    tilde                       = 0xF7,
    macron                      = 0xF8,
    breveMark                   = 0xF9,
    overDot                     = 0xFA,
    ringMark                    = 0xFB,
    cedilla                     = 0xFC,
    doubleAcute                 = 0xFD,
    ogonek                      = 0xFE,
    hachek                      = 0xFF
};

enum {
                                                                /* ScriptTokenType values */
    tokenIntl                   = 4,                            /*the itl resource number of the tokenizer*/
    tokenEmpty                  = -1                            /*used internally as an empty flag*/
};

enum {
    tokenUnknown                = 0,                            /*chars that do not match a defined token type*/
    tokenWhite                  = 1,                            /*white space*/
    tokenLeftLit                = 2,                            /*literal begin*/
    tokenRightLit               = 3,                            /*literal end*/
    tokenAlpha                  = 4,                            /*alphabetic*/
    tokenNumeric                = 5,                            /*numeric*/
    tokenNewLine                = 6,                            /*new line*/
    tokenLeftComment            = 7,                            /*open comment*/
    tokenRightComment           = 8,                            /*close comment*/
    tokenLiteral                = 9,                            /*literal*/
    tokenEscape                 = 10,                           /*character escape (e.g. '\' in "\n", "\t")*/
    tokenAltNum                 = 11,                           /*alternate number (e.g. $B0-B9 in Arabic,Hebrew)*/
    tokenRealNum                = 12,                           /*real number*/
    tokenAltReal                = 13,                           /*alternate real number*/
    tokenReserve1               = 14,                           /*reserved*/
    tokenReserve2               = 15,                           /*reserved*/
    tokenLeftParen              = 16,                           /*open parenthesis*/
    tokenRightParen             = 17,                           /*close parenthesis*/
    tokenLeftBracket            = 18,                           /*open square bracket*/
    tokenRightBracket           = 19                            /*close square bracket*/
};

enum {
    tokenLeftCurly              = 20,                           /*open curly bracket*/
    tokenRightCurly             = 21,                           /*close curly bracket*/
    tokenLeftEnclose            = 22,                           /*open guillemet*/
    tokenRightEnclose           = 23,                           /*close guillemet*/
    tokenPlus                   = 24,
    tokenMinus                  = 25,
    tokenAsterisk               = 26,                           /*times/multiply*/
    tokenDivide                 = 27,
    tokenPlusMinus              = 28,                           /*plus or minus symbol*/
    tokenSlash                  = 29,
    tokenBackSlash              = 30,
    tokenLess                   = 31,                           /*less than symbol*/
    tokenGreat                  = 32,                           /*greater than symbol*/
    tokenEqual                  = 33,
    tokenLessEqual2             = 34,                           /*less than or equal, 2 characters (e.g. <=)*/
    tokenLessEqual1             = 35,                           /*less than or equal, 1 character*/
    tokenGreatEqual2            = 36,                           /*greater than or equal, 2 characters (e.g. >=)*/
    tokenGreatEqual1            = 37,                           /*greater than or equal, 1 character*/
    token2Equal                 = 38,                           /*double equal (e.g. ==)*/
    tokenColonEqual             = 39                            /*colon equal*/
};

enum {
    tokenNotEqual               = 40,                           /*not equal, 1 character*/
    tokenLessGreat              = 41,                           /*less/greater, Pascal not equal (e.g. <>)*/
    tokenExclamEqual            = 42,                           /*exclamation equal, C not equal (e.g. !=)*/
    tokenExclam                 = 43,                           /*exclamation point*/
    tokenTilde                  = 44,                           /*centered tilde*/
    tokenComma                  = 45,
    tokenPeriod                 = 46,
    tokenLeft2Quote             = 47,                           /*open double quote*/
    tokenRight2Quote            = 48,                           /*close double quote*/
    tokenLeft1Quote             = 49,                           /*open single quote*/
    tokenRight1Quote            = 50,                           /*close single quote*/
    token2Quote                 = 51,                           /*double quote*/
    token1Quote                 = 52,                           /*single quote*/
    tokenSemicolon              = 53,
    tokenPercent                = 54,
    tokenCaret                  = 55,
    tokenUnderline              = 56,
    tokenAmpersand              = 57,
    tokenAtSign                 = 58,
    tokenBar                    = 59                            /*vertical bar*/
};

enum {
    tokenQuestion               = 60,
    tokenPi                     = 61,                           /*lower-case pi*/
    tokenRoot                   = 62,                           /*square root symbol*/
    tokenSigma                  = 63,                           /*capital sigma*/
    tokenIntegral               = 64,                           /*integral sign*/
    tokenMicro                  = 65,
    tokenCapPi                  = 66,                           /*capital pi*/
    tokenInfinity               = 67,
    tokenColon                  = 68,
    tokenHash                   = 69,                           /*e.g. #*/
    tokenDollar                 = 70,
    tokenNoBreakSpace           = 71,                           /*non-breaking space*/
    tokenFraction               = 72,
    tokenIntlCurrency           = 73,
    tokenLeftSingGuillemet      = 74,
    tokenRightSingGuillemet     = 75,
    tokenPerThousand            = 76,
    tokenEllipsis               = 77,
    tokenCenterDot              = 78,
    tokenNil                    = 127
};

enum {
    delimPad                    = -2,                           /* obsolete, misspelled token names kept for backward compatibility */
    tokenTilda                  = 44,
    tokenCarat                  = 55
};

enum {
                                                                /* Table selectors for GetItlTable */
    smWordSelectTable           = 0,                            /* get word select break table from 'itl2' */
    smWordWrapTable             = 1,                            /* get word wrap break table from 'itl2' */
    smNumberPartsTable          = 2,                            /* get default number parts table from 'itl4' */
    smUnTokenTable              = 3,                            /* get unToken table from 'itl4' */
    smWhiteSpaceList            = 4,                            /* get white space list from 'itl4' */
    iuWordSelectTable           = 0,                            /* <obsolete>  get word select break table from 'itl2' */
    iuWordWrapTable             = 1,                            /* <obsolete>  get word wrap break table from 'itl2' */
    iuNumberPartsTable          = 2,                            /* <obsolete>  get default number parts table from 'itl4' */
    iuUnTokenTable              = 3,                            /* <obsolete>  get unToken table from 'itl4' */
    iuWhiteSpaceList            = 4                             /* <obsolete>  get white space list from 'itl4' */
};

/* end of stuff moved from Packages.h */
enum {
    tokenOK                     = 0,                            /* TokenResults */
    tokenOverflow               = 1,                            /* TokenResults */
    stringOverflow              = 2,                            /* TokenResults */
    badDelim                    = 3,                            /* TokenResults */
    badEnding                   = 4,                            /* TokenResults */
    crash                       = 5                             /* TokenResults */
};

typedef SInt8                           TokenResults;
typedef char                            CharByteTable[256];
/* "TokenType" was renamed to "ScriptTokenType" because of a name collision in QuickTime 3.0 on other OS's*/
typedef short                           ScriptTokenType;
#if TARGET_OS_MAC
typedef ScriptTokenType                 TokenType;
#endif  /* TARGET_OS_MAC */

typedef ScriptTokenType                 DelimType[2];
typedef ScriptTokenType                 CommentType[4];

struct TokenRec {
    ScriptTokenType                 theToken;
    Ptr                             position;                   /*pointer into original source*/
    long                            length;                     /*length of text in original source*/
    StringPtr                       stringPosition;             /*Pascal/C string copy of identifier*/
};
typedef struct TokenRec                 TokenRec;
typedef TokenRec *                      TokenRecPtr;

struct TokenBlock {
    Ptr                             source;                     /*pointer to stream of characters*/
    long                            sourceLength;               /*length of source stream*/
    Ptr                             tokenList;                  /*pointer to array of tokens*/
    long                            tokenLength;                /*maximum length of TokenList*/
    long                            tokenCount;                 /*number tokens generated by tokenizer*/
    Ptr                             stringList;                 /*pointer to stream of identifiers*/
    long                            stringLength;               /*length of string list*/
    long                            stringCount;                /*number of bytes currently used*/
    Boolean                         doString;                   /*make strings & put into StringList*/
    Boolean                         doAppend;                   /*append to TokenList rather than replace*/
    Boolean                         doAlphanumeric;             /*identifiers may include numeric*/
    Boolean                         doNest;                     /*do comments nest?*/
    ScriptTokenType                 leftDelims[2];
    ScriptTokenType                 rightDelims[2];
    ScriptTokenType                 leftComment[4];
    ScriptTokenType                 rightComment[4];
    ScriptTokenType                 escapeCode;                 /*escape symbol code*/
    ScriptTokenType                 decimalCode;
    Handle                          itlResource;                /*handle to itl4 resource of current script*/
    long                            reserved[8];                /*must be zero!*/
};
typedef struct TokenBlock               TokenBlock;
typedef TokenBlock *                    TokenBlockPtr;
EXTERN_API( short )
GetSysDirection                 (void)                                                      TWOWORDINLINE(0x3EB8, 0x0BAC);

EXTERN_API( void )
SetSysDirection                 (short                  value)                              TWOWORDINLINE(0x31DF, 0x0BAC);

EXTERN_API( short )
FontScript                      (void)                                                      FOURWORDINLINE(0x2F3C, 0x8200, 0x0000, 0xA8B5);

EXTERN_API( short )
IntlScript                      (void)                                                      FOURWORDINLINE(0x2F3C, 0x8200, 0x0002, 0xA8B5);

EXTERN_API( void )
KeyScript                       (short                  code)                               FOURWORDINLINE(0x2F3C, 0x8002, 0x0004, 0xA8B5);

EXTERN_API( Boolean )
IsCmdChar                       (const EventRecord *    event,
                                 short                  test)                               FOURWORDINLINE(0x2F3C, 0x8206, 0xFFD0, 0xA8B5);

EXTERN_API( short )
FontToScript                    (short                  fontNumber)                         FOURWORDINLINE(0x2F3C, 0x8202, 0x0006, 0xA8B5);

EXTERN_API( long )
GetScriptManagerVariable        (short                  selector)                           FOURWORDINLINE(0x2F3C, 0x8402, 0x0008, 0xA8B5);

EXTERN_API( OSErr )
SetScriptManagerVariable        (short                  selector,
                                 long                   param)                              FOURWORDINLINE(0x2F3C, 0x8206, 0x000A, 0xA8B5);

EXTERN_API( long )
GetScriptVariable               (short                  script,
                                 short                  selector)                           FOURWORDINLINE(0x2F3C, 0x8404, 0x000C, 0xA8B5);

EXTERN_API( OSErr )
SetScriptVariable               (short                  script,
                                 short                  selector,
                                 long                   param)                              FOURWORDINLINE(0x2F3C, 0x8208, 0x000E, 0xA8B5);

EXTERN_API( short )
CharacterByteType               (Ptr                    textBuf,
                                 short                  textOffset,
                                 ScriptCode             script)                             FOURWORDINLINE(0x2F3C, 0xC206, 0x0010, 0xA8B5);

EXTERN_API( short )
CharacterType                   (Ptr                    textBuf,
                                 short                  textOffset,
                                 ScriptCode             script)                             FOURWORDINLINE(0x2F3C, 0xC206, 0x0012, 0xA8B5);

EXTERN_API( OSErr )
TransliterateText               (Handle                 srcHandle,
                                 Handle                 dstHandle,
                                 short                  target,
                                 long                   srcMask,
                                 ScriptCode             script)                             FOURWORDINLINE(0x2F3C, 0xC20E, 0x0018, 0xA8B5);

EXTERN_API( Boolean )
FillParseTable                  (CharByteTable          table,
                                 ScriptCode             script)                             FOURWORDINLINE(0x2F3C, 0xC204, 0x0022, 0xA8B5);

EXTERN_API( Handle )
GetIntlResource                 (short                  theID)                              THREEWORDINLINE(0x3F3C, 0x0006, 0xA9ED);

EXTERN_API( void )
ClearIntlResourceCache          (void)                                                      THREEWORDINLINE(0x3F3C, 0x0018, 0xA9ED);

EXTERN_API( void )
GetIntlResourceTable            (ScriptCode             script,
                                 short                  tableCode,
                                 Handle *               itlHandle,
                                 long *                 offset,
                                 long *                 length)                             THREEWORDINLINE(0x3F3C, 0x0024, 0xA9ED);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( UniversalProcPtr )
GetScriptUtilityAddress         (short                  selector,
                                 Boolean                Before,
                                 ScriptCode             script)                             FOURWORDINLINE(0x2F3C, 0xC404, 0x0038, 0xA8B5);

EXTERN_API( void )
SetScriptUtilityAddress         (short                  selector,
                                 Boolean                Before,
                                 UniversalProcPtr       routineAddr,
                                 ScriptCode             script)                             FOURWORDINLINE(0x2F3C, 0xC008, 0x003A, 0xA8B5);

EXTERN_API( UniversalProcPtr )
GetScriptQDPatchAddress         (short                  trapNum,
                                 Boolean                Before,
                                 Boolean                forPrinting,
                                 ScriptCode             script)                             FOURWORDINLINE(0x2F3C, 0xC406, 0x003C, 0xA8B5);

EXTERN_API( void )
SetScriptQDPatchAddress         (short                  trapNum,
                                 Boolean                Before,
                                 Boolean                forPrinting,
                                 UniversalProcPtr       routineAddr,
                                 ScriptCode             script)                             FOURWORDINLINE(0x2F3C, 0xC00A, 0x003E, 0xA8B5);

EXTERN_API( void )
SetIntlResource                 (short                  refNum,
                                 short                  theID,
                                 Handle                 intlHandle)                         THREEWORDINLINE(0x3F3C, 0x0008, 0xA9ED);

EXTERN_API( short )
CharByte                        (Ptr                    textBuf,
                                 short                  textOffset)                         FOURWORDINLINE(0x2F3C, 0x8206, 0x0010, 0xA8B5);

EXTERN_API( short )
CharType                        (Ptr                    textBuf,
                                 short                  textOffset)                         FOURWORDINLINE(0x2F3C, 0x8206, 0x0012, 0xA8B5);

EXTERN_API( OSErr )
Transliterate                   (Handle                 srcHandle,
                                 Handle                 dstHandle,
                                 short                  target,
                                 long                   srcMask)                            FOURWORDINLINE(0x2F3C, 0x820E, 0x0018, 0xA8B5);

EXTERN_API( Boolean )
ParseTable                      (CharByteTable          table)                              FOURWORDINLINE(0x2F3C, 0x8204, 0x0022, 0xA8B5);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( TokenResults )
IntlTokenize                    (TokenBlockPtr          tokenParam)                         FOURWORDINLINE(0x2F3C, 0x8204, 0xFFFA, 0xA8B5);

#if OLDROUTINENAMES
#define SetSysJust(newJust) SetSysDirection(newJust)
#define GetSysJust() GetSysDirection()
#define Font2Script(fontNumber) FontToScript(fontNumber)
#define GetEnvirons(verb) GetScriptManagerVariable(verb)
#define SetEnvirons(verb, param) SetScriptManagerVariable(verb, param)
#define GetScript(script, verb) GetScriptVariable(script, verb)
#define SetScript(script, verb, param) SetScriptVariable(script, verb, param)
#define IUGetIntl(theID) GetIntlResource(theID) 
#define IUSetIntl(refNum, theID, intlHandle) SetIntlResource(refNum, theID, intlHandle)
#define IUClearCache() ClearIntlResourceCache()
#define IUGetItlTable(script, tableCode, itlHandle, offset, length) \
         GetIntlResourceTable(script, tableCode, itlHandle, offset, length)
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

#endif /* __SCRIPT__ */

