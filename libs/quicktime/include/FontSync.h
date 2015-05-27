/*
     File:       FontSync.h
 
     Contains:   Public interface for FontSync
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FONTSYNC__
#define __FONTSYNC__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __FONTS__
#include <Fonts.h>
#endif

#ifndef __SFNTTYPES__
#include <SFNTTypes.h>
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


/* Matching Options */

typedef UInt32 FNSMatchOptions;
enum {
    kFNSMatchNames              = 0x00000001,                   /* font names must match */
    kFNSMatchTechnology         = 0x00000002,                   /* scaler technology must match */
    kFNSMatchGlyphs             = 0x00000004,                   /* glyph data must match */
    kFNSMatchEncodings          = 0x00000008,                   /* cmaps must match */
    kFNSMatchQDMetrics          = 0x00000010,                   /* QuickDraw Text metrics must match */
    kFNSMatchATSUMetrics        = 0x00000020,                   /* ATSUI metrics (incl. vertical) must match */
    kFNSMatchKerning            = 0x00000040,                   /* kerning data must match */
    kFNSMatchWSLayout           = 0x00000080,                   /* WorldScript layout tables must match */
    kFNSMatchAATLayout          = 0x00000100,                   /* AAT (incl. OpenType) layout tables must match */
    kFNSMatchPrintEncoding      = 0x00000200,                   /* PostScript font and glyph names and re-encoding vector must match */
    kFNSMissingDataNoMatch      = (unsigned long)0x80000000,    /* treat missing data as mismatch */
    kFNSMatchAll                = (unsigned long)0xFFFFFFFF,    /* everything must match */
    kFNSMatchDefaults           = 0                             /* use global default match options */
};

EXTERN_API_C( FNSMatchOptions )
FNSMatchDefaultsGet             (void);


/* Version control */

typedef UInt32 FNSObjectVersion;
enum {
    kFNSVersionDontCare         = 0,
    kFNSCurSysInfoVersion       = 1
};

/* No features defined yet.*/
typedef UInt32                          FNSFeatureFlags;
/*
   The FontSync library version number is binary-coded decimal:
   8 bits of major version, 4 minor version and 4 bits revision.
*/

struct FNSSysInfo {
    FNSObjectVersion                iSysInfoVersion;            /* fill this in before calling FNSSysInfoGet*/
    FNSFeatureFlags                 oFeatures;
    FNSObjectVersion                oCurRefVersion;
    FNSObjectVersion                oMinRefVersion;
    FNSObjectVersion                oCurProfileVersion;
    FNSObjectVersion                oMinProfileVersion;
    UInt16                          oFontSyncVersion;
};
typedef struct FNSSysInfo               FNSSysInfo;
EXTERN_API_C( void )
FNSSysInfoGet                   (FNSSysInfo *           ioInfo);


/* FontSync References */
typedef struct OpaqueFNSFontReference*  FNSFontReference;
EXTERN_API_C( OSStatus )
FNSReferenceGetVersion          (FNSFontReference       iReference,
                                 FNSObjectVersion *     oVersion);

EXTERN_API_C( OSStatus )
FNSReferenceDispose             (FNSFontReference       iReference);

EXTERN_API_C( OSStatus )
FNSReferenceMatch               (FNSFontReference       iReference1,
                                 FNSFontReference       iReference2,
                                 FNSMatchOptions        iOptions,
                                 FNSMatchOptions *      oFailedMatchOptions) /* can be NULL */;

EXTERN_API_C( OSStatus )
FNSReferenceFlattenedSize       (FNSFontReference       iReference,
                                 ByteCount *            oFlattenedSize);

EXTERN_API_C( OSStatus )
FNSReferenceFlatten             (FNSFontReference       iReference,
                                 void *                 oFlatReference, /* can be NULL */
                                 ByteCount *            oFlattenedSize) /* can be NULL */;

EXTERN_API_C( OSStatus )
FNSReferenceUnflatten           (const void *           iFlatReference,
                                 ByteCount              iFlattenedSize,
                                 FNSFontReference *     oReference);


/* FontSync Profiles */
enum {
    kFNSCreatorDefault          = 0,
    kFNSProfileFileType         = FOUR_CHAR_CODE('fnsp')
};

typedef struct OpaqueFNSFontProfile*    FNSFontProfile;
EXTERN_API_C( OSStatus )
FNSProfileCreate                (const FSSpec *         iFile,
                                 FourCharCode           iCreator,
                                 ItemCount              iEstNumRefs,
                                 FNSObjectVersion       iDesiredVersion,
                                 FNSFontProfile *       oProfile);

EXTERN_API_C( OSStatus )
FNSProfileOpen                  (const FSSpec *         iFile,
                                 Boolean                iOpenForWrite,
                                 FNSFontProfile *       oProfile);

EXTERN_API_C( OSStatus )
FNSProfileGetVersion            (FNSFontProfile         iProfile,
                                 FNSObjectVersion *     oVersion);

EXTERN_API_C( OSStatus )
FNSProfileCompact               (FNSFontProfile         iProfile);

EXTERN_API_C( OSStatus )
FNSProfileClose                 (FNSFontProfile         iProfile);

EXTERN_API_C( OSStatus )
FNSProfileAddReference          (FNSFontProfile         iProfile,
                                 FNSFontReference       iReference);

EXTERN_API_C( OSStatus )
FNSProfileRemoveReference       (FNSFontProfile         iProfile,
                                 FNSFontReference       iReference);

EXTERN_API_C( OSStatus )
FNSProfileRemoveIndReference    (FNSFontProfile         iProfile,
                                 UInt32                 iIndex);

EXTERN_API_C( OSStatus )
FNSProfileClear                 (FNSFontProfile         iProfile);

EXTERN_API_C( OSStatus )
FNSProfileCountReferences       (FNSFontProfile         iProfile,
                                 ItemCount *            oCount);

EXTERN_API_C( OSStatus )
FNSProfileGetIndReference       (FNSFontProfile         iProfile,
                                 UInt32                 iWhichReference,
                                 FNSFontReference *     oReference);

EXTERN_API_C( OSStatus )
FNSProfileMatchReference        (FNSFontProfile         iProfile,
                                 FNSFontReference       iReference,
                                 FNSMatchOptions        iMatchOptions,
                                 ItemCount              iOutputSize,
                                 UInt32                 oIndices[], /* can be NULL */
                                 ItemCount *            oNumMatches) /* can be NULL */;


/* Mapping to and from Font Objects */
EXTERN_API_C( OSStatus )
FNSReferenceCreate              (FMFont                 iFont,
                                 FNSObjectVersion       iDesiredVersion,
                                 FNSFontReference *     oReference);

EXTERN_API_C( OSStatus )
FNSReferenceMatchFonts          (FNSFontReference       iReference,
                                 FNSMatchOptions        iMatchOptions,
                                 ItemCount              iOutputSize,
                                 FMFont                 oFonts[], /* can be NULL */
                                 ItemCount *            oNumMatches) /* can be NULL */;


/* Mapping to and from Font Families */
EXTERN_API_C( OSStatus )
FNSReferenceCreateFromFamily    (FMFontFamily           iFamily,
                                 FMFontStyle            iStyle,
                                 FNSObjectVersion       iDesiredVersion,
                                 FNSFontReference *     oReference, /* can be NULL */
                                 FMFontStyle *          oActualStyle) /* can be NULL */;

EXTERN_API_C( OSStatus )
FNSReferenceMatchFamilies       (FNSFontReference       iReference,
                                 FNSMatchOptions        iMatchOptions,
                                 ItemCount              iOutputSize,
                                 FMFontFamilyInstance   oFonts[], /* can be NULL */
                                 ItemCount *            oNumMatches) /* can be NULL */;


/* UI Support */
EXTERN_API_C( OSStatus )
FNSReferenceGetFamilyInfo       (FNSFontReference       iReference,
                                 Str255                 oFamilyName, /* can be NULL */
                                 ScriptCode *           oFamilyNameScript, /* can be NULL */
                                 FMFontStyle *          oActualStyle) /* can be NULL */;

EXTERN_API_C( OSStatus )
FNSReferenceCountNames          (FNSFontReference       iReference,
                                 ItemCount *            oNameCount);

EXTERN_API_C( OSStatus )
FNSReferenceGetIndName          (FNSFontReference       iReference,
                                 ItemCount              iFontNameIndex,
                                 ByteCount              iMaximumNameLength,
                                 Ptr                    oName, /* can be NULL */
                                 ByteCount *            oActualNameLength, /* can be NULL */
                                 FontNameCode *         oFontNameCode, /* can be NULL */
                                 FontPlatformCode *     oFontNamePlatform, /* can be NULL */
                                 FontScriptCode *       oFontNameScript, /* can be NULL */
                                 FontLanguageCode *     oFontNameLanguage) /* can be NULL */;

EXTERN_API_C( OSStatus )
FNSReferenceFindName            (FNSFontReference       iReference,
                                 FontNameCode           iFontNameCode,
                                 FontPlatformCode       iFontNamePlatform,
                                 FontScriptCode         iFontNameScript,
                                 FontLanguageCode       iFontNameLanguage,
                                 ByteCount              iMaximumNameLength,
                                 Ptr                    oName, /* can be NULL */
                                 ByteCount *            oActualNameLength, /* can be NULL */
                                 ItemCount *            oFontNameIndex) /* can be NULL */;

/* Miscellany */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( Boolean )
FNSEnabled                      (void);

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

#endif /* __FONTSYNC__ */

