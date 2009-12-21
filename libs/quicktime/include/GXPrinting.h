/*
     File:       GXPrinting.h
 
     Contains:   This file contains all printing APIs except for driver/extension specific ones.
 
     Version:    Technology: Quickdraw GX 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __GXPRINTING__
#define __GXPRINTING__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __COLLECTIONS__
#include <Collections.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __GXFONTS__
#include <GXFonts.h>
#endif

#ifndef __GXMATH__
#include <GXMath.h>
#endif

#ifndef __GXTYPES__
#include <GXTypes.h>
#endif

#ifndef __LISTS__
#include <Lists.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __GXMESSAGES__
#include <GXMessages.h>
#endif

#ifndef __PRINTING__
#include <Printing.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
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

/********************************************************************
                    Start of old "GXPrintingManager.h/a/p" interface file.
            *********************************************************************/
/* ------------------------------------------------------------------------------

                            Printing Manager API Contants and Types

-------------------------------------------------------------------------------- */

typedef unsigned long                   gxOwnerSignature;
#if OLDROUTINENAMES
typedef unsigned long                   Signature;
#endif  /* OLDROUTINENAMES */

/*

    ABSTRACT DATA TYPES

*/
/*
   typedef struct gxPrivatePrinterRecord *gxPrinter;
   typedef struct gxPrivateJobRecord *gxJob;
   typedef struct gxPrivateFormatRecord *gxFormat;
   typedef struct gxPrivatePaperTypeRecord *gxPaperType;
   typedef struct gxPrivatePrintFileRecord *gxPrintFile;
*/

typedef struct OpaquegxPrinter*         gxPrinter;
typedef struct OpaquegxJob*             gxJob;
typedef struct OpaquegxFormat*          gxFormat;
typedef struct OpaquegxPaperType*       gxPaperType;
typedef struct OpaquegxPrintFile*       gxPrintFile;
/* Possible values for LoopStatus */
typedef Boolean                         gxLoopStatus;
enum {
    gxStopLooping               = false,
    gxKeepLooping               = true
};

typedef CALLBACK_API( gxLoopStatus , GXViewDeviceProcPtr )(gxViewDevice aViewDevice, void *refCon);
typedef STACK_UPP_TYPE(GXViewDeviceProcPtr)                     GXViewDeviceUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXViewDeviceUPP)
    NewGXViewDeviceUPP             (GXViewDeviceProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeGXViewDeviceUPP         (GXViewDeviceUPP         userUPP);

    EXTERN_API(gxLoopStatus)
    InvokeGXViewDeviceUPP          (gxViewDevice            aViewDevice,
                                    void *                  refCon,
                                    GXViewDeviceUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXViewDeviceProcInfo = 0x000003D0 };                  /* pascal 1_byte Func(4_bytes, 4_bytes) */
    #define NewGXViewDeviceUPP(userRoutine)                         (GXViewDeviceUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXViewDeviceProcInfo, GetCurrentArchitecture())
    #define DisposeGXViewDeviceUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeGXViewDeviceUPP(aViewDevice, refCon, userUPP)     (gxLoopStatus)CALL_TWO_PARAMETER_UPP((userUPP), uppGXViewDeviceProcInfo, (aViewDevice), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXViewDeviceProc(userRoutine)                        NewGXViewDeviceUPP(userRoutine)
#define CallGXViewDeviceProc(userRoutine, aViewDevice, refCon)  InvokeGXViewDeviceUPP(aViewDevice, refCon, userRoutine)
typedef CALLBACK_API( gxLoopStatus , GXFormatProcPtr )(gxFormat aFormat, void *refCon);
typedef STACK_UPP_TYPE(GXFormatProcPtr)                         GXFormatUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFormatUPP)
    NewGXFormatUPP                 (GXFormatProcPtr         userRoutine);

    EXTERN_API(void)
    DisposeGXFormatUPP             (GXFormatUPP             userUPP);

    EXTERN_API(gxLoopStatus)
    InvokeGXFormatUPP              (gxFormat                aFormat,
                                    void *                  refCon,
                                    GXFormatUPP             userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFormatProcInfo = 0x000003D0 };                      /* pascal 1_byte Func(4_bytes, 4_bytes) */
    #define NewGXFormatUPP(userRoutine)                             (GXFormatUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFormatProcInfo, GetCurrentArchitecture())
    #define DisposeGXFormatUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFormatUPP(aFormat, refCon, userUPP)             (gxLoopStatus)CALL_TWO_PARAMETER_UPP((userUPP), uppGXFormatProcInfo, (aFormat), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFormatProc(userRoutine)                            NewGXFormatUPP(userRoutine)
#define CallGXFormatProc(userRoutine, aFormat, refCon)          InvokeGXFormatUPP(aFormat, refCon, userRoutine)
typedef CALLBACK_API( gxLoopStatus , GXPaperTypeProcPtr )(gxPaperType aPapertype, void *refCon);
typedef STACK_UPP_TYPE(GXPaperTypeProcPtr)                      GXPaperTypeUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXPaperTypeUPP)
    NewGXPaperTypeUPP              (GXPaperTypeProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeGXPaperTypeUPP          (GXPaperTypeUPP          userUPP);

    EXTERN_API(gxLoopStatus)
    InvokeGXPaperTypeUPP           (gxPaperType             aPapertype,
                                    void *                  refCon,
                                    GXPaperTypeUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXPaperTypeProcInfo = 0x000003D0 };                   /* pascal 1_byte Func(4_bytes, 4_bytes) */
    #define NewGXPaperTypeUPP(userRoutine)                          (GXPaperTypeUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXPaperTypeProcInfo, GetCurrentArchitecture())
    #define DisposeGXPaperTypeUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeGXPaperTypeUPP(aPapertype, refCon, userUPP)       (gxLoopStatus)CALL_TWO_PARAMETER_UPP((userUPP), uppGXPaperTypeProcInfo, (aPapertype), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXPaperTypeProc(userRoutine)                         NewGXPaperTypeUPP(userRoutine)
#define CallGXPaperTypeProc(userRoutine, aPapertype, refCon)    InvokeGXPaperTypeUPP(aPapertype, refCon, userRoutine)
typedef CALLBACK_API( OSErr , GXPrintingFlattenProcPtr )(long size, void *data, void *refCon);
typedef STACK_UPP_TYPE(GXPrintingFlattenProcPtr)                GXPrintingFlattenUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXPrintingFlattenUPP)
    NewGXPrintingFlattenUPP        (GXPrintingFlattenProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXPrintingFlattenUPP    (GXPrintingFlattenUPP    userUPP);

    EXTERN_API(OSErr)
    InvokeGXPrintingFlattenUPP     (long                    size,
                                    void *                  data,
                                    void *                  refCon,
                                    GXPrintingFlattenUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXPrintingFlattenProcInfo = 0x00000FE0 };             /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXPrintingFlattenUPP(userRoutine)                    (GXPrintingFlattenUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXPrintingFlattenProcInfo, GetCurrentArchitecture())
    #define DisposeGXPrintingFlattenUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeGXPrintingFlattenUPP(size, data, refCon, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXPrintingFlattenProcInfo, (size), (data), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXPrintingFlattenProc(userRoutine)                   NewGXPrintingFlattenUPP(userRoutine)
#define CallGXPrintingFlattenProc(userRoutine, size, data, refCon) InvokeGXPrintingFlattenUPP(size, data, refCon, userRoutine)
#if OLDROUTINENAMES
typedef GXViewDeviceProcPtr             gxViewDeviceProc;
typedef GXFormatProcPtr                 gxFormatProc;
typedef GXPaperTypeProcPtr              gxPaperTypeProc;
typedef GXPrintingFlattenProcPtr        gxPrintingFlattenProc;
#endif  /* OLDROUTINENAMES */

/*
    The following constants are used to set collection item flags in printing
    collections. The Printing Manager purges certain items whenever a driver
    switch occurs. If the formatting driver changes, all items marked as
    gxVolatileFormattingDriverCategory will be purged.  If the output driver
    changes, all items marked as gxVolatileOutputDriverCategory will be purged.
    Note that to prevent items from being flattened when GXFlattenJob is called,
    you should unset the collectionPersistenceBit (defined in Collections.h),
    which is on by default.
*/
/* Structure stored in collection items' user attribute bits */
typedef short                           gxCollectionCategory;
enum {
    gxNoCollectionCategory      = 0x0000,
    gxOutputDriverCategory      = 0x0001,
    gxFormattingDriverCategory  = 0x0002,
    gxDriverVolatileCategory    = 0x0004,
    gxVolatileOutputDriverCategory = gxOutputDriverCategory + gxDriverVolatileCategory,
    gxVolatileFormattingDriverCategory = gxFormattingDriverCategory + gxDriverVolatileCategory
};


/*

    >>>>>> JOB COLLECTION ITEMS <<<<<<

*/

/* gxJobInfo COLLECTION ITEM */
enum {
    gxJobTag                    = FOUR_CHAR_CODE('job ')
};


struct gxJobInfo {
    long                            numPages;                   /* Number of pages in the document */
    long                            priority;                   /* Priority of this job plus "is it on hold?" */
    unsigned long                   timeToPrint;                /* When to print job, if scheduled */
    long                            jobTimeout;                 /* Timeout value, in ticks */
    long                            firstPageToPrint;           /* Start printing from this page */
    short                           jobAlert;                   /* How to alert user when printing */
    Str31                           appName;                    /* Which application printed the document */
    Str31                           documentName;               /* The name of the document being printed */
    Str31                           userName;                   /* The owner name of the machine that printed the document */
};
typedef struct gxJobInfo                gxJobInfo;
/* gxPDDDefaultSettingTag COLLECTION ITEM */
enum {
    gxPDDDefaultSettingTag      = FOUR_CHAR_CODE('pdds')
};


struct gxPDDDefaultSettingInfo {
    Boolean                         useDefaultSetting;          /* true if PDD default setting should be used */
    SInt8                           pad;
};
typedef struct gxPDDDefaultSettingInfo  gxPDDDefaultSettingInfo;
/* priority field constants */
enum {
    gxPrintJobHoldingBit        = 0x00001000                    /* This bit is set if the job is on hold. */
};

enum {
    gxPrintJobUrgent            = 0x00000001,
    gxPrintJobAtTime            = 0x00000002,
    gxPrintJobASAP              = 0x00000003,
    gxPrintJobHolding           = (gxPrintJobHoldingBit + gxPrintJobASAP),
    gxPrintJobHoldingAtTime     = (gxPrintJobHoldingBit + gxPrintJobAtTime),
    gxPrintJobHoldingUrgent     = (gxPrintJobHoldingBit + gxPrintJobUrgent)
};

/* jobAlert field constants */
enum {
    gxNoPrintTimeAlert          = 0,                            /* Don't alert user when we print */
    gxAlertBefore               = 1,                            /* Alert user before we print */
    gxAlertAfter                = 2,                            /* Alert user after we print */
    gxAlertBothTimes            = 3                             /* Alert before and after we print */
};

/* jobTimeout field constants */
enum {
    gxThirtySeconds             = 1800,                         /* 30 seconds in ticks */
    gxTwoMinutes                = 7200                          /* 2 minutes in ticks */
};


/* gxCollationTag COLLECTION ITEM */
enum {
    gxCollationTag              = FOUR_CHAR_CODE('sort')
};


struct gxCollationInfo {
    Boolean                         collation;                  /* True if copies are to be collated */
    char                            padByte;
};
typedef struct gxCollationInfo          gxCollationInfo;

/* gxCopiesTag COLLECTION ITEM */
enum {
    gxCopiesTag                 = FOUR_CHAR_CODE('copy')
};


struct gxCopiesInfo {
    long                            copies;                     /* Number of copies of the document to print */
};
typedef struct gxCopiesInfo             gxCopiesInfo;

/* gxPageRangeTag COLLECTION ITEM */
enum {
    gxPageRangeTag              = FOUR_CHAR_CODE('rang')
};


struct gxSimplePageRangeInfo {
    char                            optionChosen;               /* From options listed below */
    Boolean                         printAll;                   /* True if user wants to print all pages */
    long                            fromPage;                   /* For gxDefaultPageRange, current value */
    long                            toPage;                     /* For gxDefaultPageRange, current value */
};
typedef struct gxSimplePageRangeInfo    gxSimplePageRangeInfo;

struct gxPageRangeInfo {
    gxSimplePageRangeInfo           simpleRange;                /* Info which will be returned for GetJobPageRange */
    Str31                           fromString;                 /* For gxCustomizePageRange, current value */
    Str31                           toString;                   /* For gxCustomizePageRange, current value */
    long                            minFromPage;                /* For gxDefaultPageRange, we parse with this, ignored if nil */
    long                            maxToPage;                  /* For gxDefaultPageRange, we parse with this, ignored if nil */
    char                            replaceString[1];           /* For gxReplacePageRange, string to display */
};
typedef struct gxPageRangeInfo          gxPageRangeInfo;

/* optionChosen field constants for SimplePageRangeInfo */
enum {
    gxDefaultPageRange          = 0,
    gxReplacePageRange          = 1,
    gxCustomizePageRange        = 2
};


/* gxQualityTag COLLECTION ITEM */
enum {
    gxQualityTag                = FOUR_CHAR_CODE('qual')
};


struct gxQualityInfo {
    Boolean                         disableQuality;             /* True to disable standard quality controls */
    char                            padByte;
    short                           defaultQuality;             /* The default quality value */
    short                           currentQuality;             /* The current quality value */
    short                           qualityCount;               /* The number of quality menu items in popup menu */
    char                            qualityNames[1];            /* An array of packed pascal strings for popup menu titles */
};
typedef struct gxQualityInfo            gxQualityInfo;

/* gxFileDestinationTag COLLECTION ITEM */
enum {
    gxFileDestinationTag        = FOUR_CHAR_CODE('dest')
};


struct gxFileDestinationInfo {
    Boolean                         toFile;                     /* True if destination is a file */
    char                            padByte;
};
typedef struct gxFileDestinationInfo    gxFileDestinationInfo;

/* gxFileLocationTag COLLECTION ITEM */
enum {
    gxFileLocationTag           = FOUR_CHAR_CODE('floc')
};


struct gxFileLocationInfo {
    FSSpec                          fileSpec;                   /* Location to put file, if destination is file */
};
typedef struct gxFileLocationInfo       gxFileLocationInfo;

/* gxFileFormatTag COLLECTION ITEM */
enum {
    gxFileFormatTag             = FOUR_CHAR_CODE('ffmt')
};


struct gxFileFormatInfo {
    Str31                           fileFormatName;             /* Name of file format (e.g. "PostScript") if destination is file */
};
typedef struct gxFileFormatInfo         gxFileFormatInfo;

/* gxFileFontsTag COLLECTION ITEM */
enum {
    gxFileFontsTag              = FOUR_CHAR_CODE('incf')
};


struct gxFileFontsInfo {
    char                            includeFonts;               /* Which fonts to include, if destination is file */
    char                            padByte;
};
typedef struct gxFileFontsInfo          gxFileFontsInfo;
/* includeFonts field constants */
enum {
    gxIncludeNoFonts            = 1,                            /* Include no fonts */
    gxIncludeAllFonts           = 2,                            /* Include all fonts */
    gxIncludeNonStandardFonts   = 3                             /* Include only fonts that aren't in the standard LW set */
};


/* gxPaperFeedTag COLLECTION ITEM */
enum {
    gxPaperFeedTag              = FOUR_CHAR_CODE('feed')
};


struct gxPaperFeedInfo {
    Boolean                         autoFeed;                   /* True if automatic feed, false if manual */
    char                            padByte;
};
typedef struct gxPaperFeedInfo          gxPaperFeedInfo;

/* gxTrayFeedTag COLLECTION ITEM */
enum {
    gxTrayFeedTag               = FOUR_CHAR_CODE('tray')
};


typedef long                            gxTrayIndex;

struct gxTrayFeedInfo {
    gxTrayIndex                     feedTrayIndex;              /* Tray to feed paper from */
    Boolean                         manualFeedThisPage;         /* Signals manual feeding for the page */
    char                            padByte;
};
typedef struct gxTrayFeedInfo           gxTrayFeedInfo;

/* gxManualFeedTag COLLECTION ITEM */
enum {
    gxManualFeedTag             = FOUR_CHAR_CODE('manf')
};


struct gxManualFeedInfo {
    long                            numPaperTypeNames;          /* Number of paperTypes to manually feed */
    Str31                           paperTypeNames[1];          /* Array of names of paperTypes to manually feed */
};
typedef struct gxManualFeedInfo         gxManualFeedInfo;

/* gxNormalMappingTag COLLECTION ITEM */
enum {
    gxNormalMappingTag          = FOUR_CHAR_CODE('nmap')
};


struct gxNormalMappingInfo {
    Boolean                         normalPaperMapping;         /* True if not overriding normal paper mapping */
    char                            padByte;
};
typedef struct gxNormalMappingInfo      gxNormalMappingInfo;

/* gxSpecialMappingTag COLLECTION ITEM */
enum {
    gxSpecialMappingTag         = FOUR_CHAR_CODE('smap')
};


struct gxSpecialMappingInfo {
    char                            specialMapping;             /* Enumerated redirect, scale or tile setting */
    char                            padByte;
};
typedef struct gxSpecialMappingInfo     gxSpecialMappingInfo;
/* specialMapping field constants */
enum {
    gxRedirectPages             = 1,                            /* Redirect pages to a papertype and clip if necessary */
    gxScalePages                = 2,                            /* Scale pages if necessary */
    gxTilePages                 = 3                             /* Tile pages if necessary */
};


/* gxTrayMappingTag COLLECTION ITEM */
enum {
    gxTrayMappingTag            = FOUR_CHAR_CODE('tmap')
};


struct gxTrayMappingInfo {
    gxTrayIndex                     mapPaperToTray;             /* Tray to map all paper to */
};
typedef struct gxTrayMappingInfo        gxTrayMappingInfo;

/* gxPaperMappingTag COLLECTION ITEM */
/* This collection item contains a flattened paper type resource */
enum {
    gxPaperMappingTag           = FOUR_CHAR_CODE('pmap')
};


/* gxPrintPanelTag COLLECTION ITEM */
enum {
    gxPrintPanelTag             = FOUR_CHAR_CODE('ppan')
};


struct gxPrintPanelInfo {
    Str31                           startPanelName;             /* Name of starting panel in Print dialog */
};
typedef struct gxPrintPanelInfo         gxPrintPanelInfo;

/* gxFormatPanelTag COLLECTION ITEM */
enum {
    gxFormatPanelTag            = FOUR_CHAR_CODE('fpan')
};


struct gxFormatPanelInfo {
    Str31                           startPanelName;             /* Name of starting panel in Format dialog */
};
typedef struct gxFormatPanelInfo        gxFormatPanelInfo;

/* gxTranslatedDocumentTag COLLECTION ITEM */
enum {
    gxTranslatedDocumentTag     = FOUR_CHAR_CODE('trns')
};


struct gxTranslatedDocumentInfo {
    long                            translatorInfo;             /* Information from the translation process */
};
typedef struct gxTranslatedDocumentInfo gxTranslatedDocumentInfo;

/* gxCoverPageTag COLLECTION ITEM */
enum {
    gxCoverPageTag              = FOUR_CHAR_CODE('cvpg')
};



struct gxCoverPageInfo {
    long                            coverPage;                  /* Use same enum values as for PrintRecord field in GXPrinterDrivers.h */
};
typedef struct gxCoverPageInfo          gxCoverPageInfo;
/*

    >>>>>> FORMAT COLLECTION ITEMS <<<<<<

*/
/* gxPaperTypeLockTag COLLECTION ITEM */
enum {
    gxPaperTypeLockTag          = FOUR_CHAR_CODE('ptlk')
};


struct gxPaperTypeLockInfo {
    Boolean                         paperTypeLocked;            /* True if format's paperType is locked */
    char                            padByte;
};
typedef struct gxPaperTypeLockInfo      gxPaperTypeLockInfo;

/* gxOrientationTag COLLECTION ITEM */
enum {
    gxOrientationTag            = FOUR_CHAR_CODE('layo')
};


struct gxOrientationInfo {
    char                            orientation;                /* An enumerated orientation value */
    char                            padByte;
};
typedef struct gxOrientationInfo        gxOrientationInfo;
/* orientation field constants */
enum {
    gxPortraitLayout            = 0,                            /* Portrait */
    gxLandscapeLayout           = 1,                            /* Landscape */
    gxRotatedPortraitLayout     = 2,                            /* Portrait, rotated 180. */
    gxRotatedLandscapeLayout    = 3                             /* Landscape, rotated 180.  */
};


/* gxScalingTag COLLECTION ITEM */
enum {
    gxScalingTag                = FOUR_CHAR_CODE('scal')
};


struct gxScalingInfo {
    Fixed                           horizontalScaleFactor;      /* Current horizontal scaling factor */
    Fixed                           verticalScaleFactor;        /* Current vertical scaling factor */
    short                           minScaling;                 /* Minimum scaling allowed */
    short                           maxScaling;                 /* Maximum scaling allowed */
};
typedef struct gxScalingInfo            gxScalingInfo;

/* gxDirectModeTag COLLECTION ITEM */
enum {
    gxDirectModeTag             = FOUR_CHAR_CODE('dirm')
};


struct gxDirectModeInfo {
    Boolean                         directModeOn;               /* True if a direct mode is enabled */
    char                            padByte;
};
typedef struct gxDirectModeInfo         gxDirectModeInfo;

/* gxFormatHalftoneTag COLLECTION ITEM */
enum {
    gxFormatHalftoneTag         = FOUR_CHAR_CODE('half')
};


struct gxFormatHalftoneInfo {
    long                            numHalftones;               /* Number of halftone records */
    gxHalftone                      halftones[1];               /* The halftone records */
};
typedef struct gxFormatHalftoneInfo     gxFormatHalftoneInfo;

/* gxInvertPageTag COLLECTION ITEM */
enum {
    gxInvertPageTag             = FOUR_CHAR_CODE('invp')
};


struct gxInvertPageInfo {
    char                            padByte;
    Boolean                         invert;                     /* If true, invert page */
};
typedef struct gxInvertPageInfo         gxInvertPageInfo;

/* gxFlipPageHorizontalTag COLLECTION ITEM */
enum {
    gxFlipPageHorizontalTag     = FOUR_CHAR_CODE('flph')
};


struct gxFlipPageHorizontalInfo {
    char                            padByte;
    Boolean                         flipHorizontal;             /* If true, flip x coordinates on page */
};
typedef struct gxFlipPageHorizontalInfo gxFlipPageHorizontalInfo;

/* gxFlipPageVerticalTag COLLECTION ITEM */
enum {
    gxFlipPageVerticalTag       = FOUR_CHAR_CODE('flpv')
};


struct gxFlipPageVerticalInfo {
    char                            padByte;
    Boolean                         flipVertical;               /* If true, flip y coordinates on page */
};
typedef struct gxFlipPageVerticalInfo   gxFlipPageVerticalInfo;

/* gxPreciseBitmapsTag COLLECTION ITEM */
enum {
    gxPreciseBitmapsTag         = FOUR_CHAR_CODE('pbmp')
};


struct gxPreciseBitmapInfo {
    Boolean                         preciseBitmaps;             /* If true, scale page by 96% */
    char                            padByte;
};
typedef struct gxPreciseBitmapInfo      gxPreciseBitmapInfo;

/*

    >>>>>> PAPERTYPE COLLECTION ITEMS <<<<<<

*/
/* gxBaseTag COLLECTION ITEM */
enum {
    gxBaseTag                   = FOUR_CHAR_CODE('base')
};


struct gxBaseInfo {
    long                            baseType;                   /* PaperType's base type */
};
typedef struct gxBaseInfo               gxBaseInfo;
/* baseType field constants */
enum {
    gxUnknownBase               = 0,                            /* Base paper type from which this paper type is */
    gxUSLetterBase              = 1,                            /* derived.  This is not a complete set. */
    gxUSLegalBase               = 2,
    gxA4LetterBase              = 3,
    gxB5LetterBase              = 4,
    gxTabloidBase               = 5
};


/* gxCreatorTag COLLECTION ITEM */
enum {
    gxCreatorTag                = FOUR_CHAR_CODE('crea')
};


struct gxCreatorInfo {
    OSType                          creator;                    /* PaperType's creator */
};
typedef struct gxCreatorInfo            gxCreatorInfo;
/* gxUnitsTag COLLECTION ITEM */
enum {
    gxUnitsTag                  = FOUR_CHAR_CODE('unit')
};


struct gxUnitsInfo {
    char                            units;                      /* PaperType's units (used by PaperType Editor). */
    char                            padByte;
};
typedef struct gxUnitsInfo              gxUnitsInfo;
/* units field constants */
enum {
    gxPicas                     = 0,                            /* Pica measurement */
    gxMMs                       = 1,                            /* Millimeter measurement */
    gxInches                    = 2                             /* Inches measurement */
};


/* gxFlagsTag COLLECTION ITEM */
enum {
    gxFlagsTag                  = FOUR_CHAR_CODE('flag')
};


struct gxFlagsInfo {
    long                            flags;                      /* PaperType's flags */
};
typedef struct gxFlagsInfo              gxFlagsInfo;
/* flags field constants */
enum {
    gxOldPaperTypeFlag          = 0x00800000,                   /* Indicates a paper type for compatibility printing */
    gxNewPaperTypeFlag          = 0x00400000,                   /* Indicates a paper type for QuickDraw GX-aware printing */
    gxOldAndNewFlag             = 0x00C00000,                   /* Indicates a paper type that's both old and new */
    gxDefaultPaperTypeFlag      = 0x00100000                    /* Indicates the default paper type in the group */
};


/* gxCommentTag COLLECTION ITEM */
enum {
    gxCommentTag                = FOUR_CHAR_CODE('cmnt')
};


struct gxCommentInfo {
    Str255                          comment;                    /* PaperType's comment */
};
typedef struct gxCommentInfo            gxCommentInfo;

/*

    >>>>>> PRINTER VIEWDEVICE TAGS <<<<<<

*/
/* gxPenTableTag COLLECTION ITEM */
enum {
    gxPenTableTag               = FOUR_CHAR_CODE('pent')
};


struct gxPenTableEntry {
    Str31                           penName;                    /* Name of the pen */
    gxColor                         penColor;                   /* Color to use from the color set */
    Fixed                           penThickness;               /* Size of the pen */
    short                           penUnits;                   /* Specifies units in which pen thickness is defined */
    short                           penPosition;                /* Pen position in the carousel, -1 (kPenNotLoaded) if not loaded */
};
typedef struct gxPenTableEntry          gxPenTableEntry;

struct gxPenTable {
    long                            numPens;                    /* Number of pen entries in the following array */
    gxPenTableEntry                 pens[1];                    /* Array of pen entries */
};
typedef struct gxPenTable               gxPenTable;
typedef gxPenTable *                    gxPenTablePtr;
typedef gxPenTablePtr *                 gxPenTableHdl;
/* penUnits field constants */
enum {
    gxDeviceUnits               = 0,
    gxMMUnits                   = 1,
    gxInchesUnits               = 2
};

/* penPosition field constants */
enum {
    gxPenNotLoaded              = -1
};


/*

    >>>>>> DIALOG-RELATED CONSTANTS AND TYPES <<<<<<

*/

typedef long                            gxDialogResult;
enum {
    gxCancelSelected            = 0L,
    gxOKSelected                = 1L,
    gxRevertSelected            = 2L
};



struct gxEditMenuRecord {
    short                           editMenuID;
    short                           cutItem;
    short                           copyItem;
    short                           pasteItem;
    short                           clearItem;
    short                           undoItem;
};
typedef struct gxEditMenuRecord         gxEditMenuRecord;

/*

    >>>>>> JOB FORMAT MODE CONSTANTS AND TYPES <<<<<<

*/

typedef OSType                          gxJobFormatMode;

struct gxJobFormatModeTable {
    long                            numModes;                   /* Number of job format modes to choose from */
    gxJobFormatMode                 modes[1];                   /* The job format modes */
};
typedef struct gxJobFormatModeTable     gxJobFormatModeTable;
typedef gxJobFormatModeTable *          gxJobFormatModeTablePtr;
typedef gxJobFormatModeTablePtr *       gxJobFormatModeTableHdl;
enum {
    gxGraphicsJobFormatMode     = FOUR_CHAR_CODE('grph'),
    gxTextJobFormatMode         = FOUR_CHAR_CODE('text'),
    gxPostScriptJobFormatMode   = FOUR_CHAR_CODE('post')
};


typedef long                            gxQueryType;
enum {
    gxGetJobFormatLineConstraintQuery = 0L,
    gxGetJobFormatFontsQuery    = 1L,
    gxGetJobFormatFontCommonStylesQuery = 2L,
    gxGetJobFormatFontConstraintQuery = 3L,
    gxSetStyleJobFormatCommonStyleQuery = 4L
};


/* Structures used for Text mode field constants */

struct gxPositionConstraintTable {
    gxPoint                         phase;                      /* Position phase */
    gxPoint                         offset;                     /* Position offset */
    long                            numSizes;                   /* Number of available font sizes */
    Fixed                           sizes[1];                   /* The available font sizes */
};
typedef struct gxPositionConstraintTable gxPositionConstraintTable;
typedef gxPositionConstraintTable *     gxPositionConstraintTablePtr;
typedef gxPositionConstraintTablePtr *  gxPositionConstraintTableHdl;
/* numSizes field constants */
enum {
    gxConstraintRange           = -1
};


struct gxStyleNameTable {
    long                            numStyleNames;              /* Number of style names */
    Str255                          styleNames[1];              /* The style names */
};
typedef struct gxStyleNameTable         gxStyleNameTable;
typedef gxStyleNameTable *              gxStyleNameTablePtr;
typedef gxStyleNameTablePtr *           gxStyleNameTableHdl;

struct gxFontTable {
    long                            numFonts;                   /* Number of font references */
    gxFont                          fonts[1];                   /* The font references */
};
typedef struct gxFontTable              gxFontTable;
typedef gxFontTable *                   gxFontTablePtr;
typedef gxFontTablePtr *                gxFontTableHdl;
/* ------------------------------------------------------------------------------

                                Printing Manager API Functions

-------------------------------------------------------------------------------- */
/*
    Global Routines
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
GXInitPrinting                  (void)                                                      FOURWORDINLINE(0x203C, 0x0000, 0x0000, 0xABFE);

EXTERN_API( OSErr )
GXExitPrinting                  (void)                                                      FOURWORDINLINE(0x203C, 0x0000, 0x0001, 0xABFE);


/*
    Error-Handling Routines
*/
EXTERN_API( OSErr )
GXGetJobError                   (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x000E, 0xABFE);

EXTERN_API( void )
GXSetJobError                   (gxJob                  aJob,
                                 OSErr                  anErr)                              FOURWORDINLINE(0x203C, 0x0000, 0x000F, 0xABFE);


/*
    Job Routines
*/
EXTERN_API( OSErr )
GXNewJob                        (gxJob *                aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x0002, 0xABFE);

EXTERN_API( OSErr )
GXDisposeJob                    (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x0003, 0xABFE);

EXTERN_API( void )
GXFlattenJob                    (gxJob                  aJob,
                                 GXPrintingFlattenUPP   flattenProc,
                                 void *                 aVoid)                              FOURWORDINLINE(0x203C, 0x0000, 0x0004, 0xABFE);

EXTERN_API( gxJob )
GXUnflattenJob                  (gxJob                  aJob,
                                 GXPrintingFlattenUPP   flattenProc,
                                 void *                 aVoid)                              FOURWORDINLINE(0x203C, 0x0000, 0x0005, 0xABFE);

EXTERN_API( Handle )
GXFlattenJobToHdl               (gxJob                  aJob,
                                 Handle                 aHdl)                               FOURWORDINLINE(0x203C, 0x0000, 0x0006, 0xABFE);

EXTERN_API( gxJob )
GXUnflattenJobFromHdl           (gxJob                  aJob,
                                 Handle                 aHdl)                               FOURWORDINLINE(0x203C, 0x0000, 0x0007, 0xABFE);

EXTERN_API( void )
GXInstallApplicationOverride    (gxJob                  aJob,
                                 short                  messageID,
                                 void *                 override)                           FOURWORDINLINE(0x203C, 0x0000, 0x0008, 0xABFE);

EXTERN_API( Collection )
GXGetJobCollection              (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x001D, 0xABFE);

EXTERN_API( void *)
GXGetJobRefCon                  (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x001E, 0xABFE);

EXTERN_API( void )
GXSetJobRefCon                  (gxJob                  aJob,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x0000, 0x001F, 0xABFE);

EXTERN_API( gxJob )
GXCopyJob                       (gxJob                  srcJob,
                                 gxJob                  dstJob)                             FOURWORDINLINE(0x203C, 0x0000, 0x0020, 0xABFE);

EXTERN_API( void )
GXSelectJobFormattingPrinter    (gxJob                  aJob,
                                 Str31                  printerName)                        FOURWORDINLINE(0x203C, 0x0000, 0x0021, 0xABFE);

EXTERN_API( void )
GXSelectJobOutputPrinter        (gxJob                  aJob,
                                 Str31                  printerName)                        FOURWORDINLINE(0x203C, 0x0000, 0x0022, 0xABFE);

EXTERN_API( void )
GXForEachJobFormatDo            (gxJob                  aJob,
                                 GXFormatUPP            formatProc,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x0000, 0x0023, 0xABFE);

EXTERN_API( long )
GXCountJobFormats               (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x0024, 0xABFE);

EXTERN_API( Boolean )
GXUpdateJob                     (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x0025, 0xABFE);

EXTERN_API( void )
GXConvertPrintRecord            (gxJob                  aJob,
                                 THPrint                hPrint)                             FOURWORDINLINE(0x203C, 0x0000, 0x0026, 0xABFE);

EXTERN_API( void )
GXIdleJob                       (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x0057, 0xABFE);


/*
    Job Format Modes Routines
*/
EXTERN_API( void )
GXSetAvailableJobFormatModes    (gxJob                  aJob,
                                 gxJobFormatModeTableHdl  formatModeTable)                  FOURWORDINLINE(0x203C, 0x0000, 0x003B, 0xABFE);

EXTERN_API( gxJobFormatMode )
GXGetPreferredJobFormatMode     (gxJob                  aJob,
                                 Boolean *              directOnly)                         FOURWORDINLINE(0x203C, 0x0000, 0x003C, 0xABFE);

EXTERN_API( gxJobFormatMode )
GXGetJobFormatMode              (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x003D, 0xABFE);

EXTERN_API( void )
GXSetJobFormatMode              (gxJob                  aJob,
                                 gxJobFormatMode        formatMode)                         FOURWORDINLINE(0x203C, 0x0000, 0x003E, 0xABFE);

EXTERN_API( void )
GXJobFormatModeQuery            (gxJob                  aJob,
                                 gxQueryType            aQueryType,
                                 void *                 srcData,
                                 void *                 dstData)                            FOURWORDINLINE(0x203C, 0x0000, 0x003F, 0xABFE);


/*
    Format Routines
*/
EXTERN_API( gxFormat )
GXNewFormat                     (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x0009, 0xABFE);

EXTERN_API( void )
GXDisposeFormat                 (gxFormat               aFormat)                            FOURWORDINLINE(0x203C, 0x0000, 0x000A, 0xABFE);

EXTERN_API( gxFormat )
GXGetJobFormat                  (gxJob                  aJob,
                                 long                   whichFormat)                        FOURWORDINLINE(0x203C, 0x0000, 0x0013, 0xABFE);

EXTERN_API( gxJob )
GXGetFormatJob                  (gxFormat               aFormat)                            FOURWORDINLINE(0x203C, 0x0000, 0x0014, 0xABFE);

EXTERN_API( gxPaperType )
GXGetFormatPaperType            (gxFormat               aFormat)                            FOURWORDINLINE(0x203C, 0x0000, 0x0015, 0xABFE);

EXTERN_API( void )
GXGetFormatDimensions           (gxFormat               aFormat,
                                 gxRectangle *          pageSize,
                                 gxRectangle *          paperSize)                          FOURWORDINLINE(0x203C, 0x0000, 0x0016, 0xABFE);

EXTERN_API( Collection )
GXGetFormatCollection           (gxFormat               aFormat)                            FOURWORDINLINE(0x203C, 0x0000, 0x0033, 0xABFE);

EXTERN_API( void )
GXChangedFormat                 (gxFormat               aFormat)                            FOURWORDINLINE(0x203C, 0x0000, 0x0034, 0xABFE);

EXTERN_API( gxFormat )
GXCopyFormat                    (gxFormat               srcFormat,
                                 gxFormat               dstFormat)                          FOURWORDINLINE(0x203C, 0x0000, 0x0035, 0xABFE);

EXTERN_API( gxFormat )
GXCloneFormat                   (gxFormat               aFormat)                            FOURWORDINLINE(0x203C, 0x0000, 0x0036, 0xABFE);

EXTERN_API( long )
GXCountFormatOwners             (gxFormat               aFormat)                            FOURWORDINLINE(0x203C, 0x0000, 0x0037, 0xABFE);

EXTERN_API( void )
GXGetFormatMapping              (gxFormat               aFormat,
                                 gxMapping *            fmtMapping)                         FOURWORDINLINE(0x203C, 0x0000, 0x0038, 0xABFE);

EXTERN_API( gxShape )
GXGetFormatForm                 (gxFormat               aFormat,
                                 gxShape *              mask)                               FOURWORDINLINE(0x203C, 0x0000, 0x0039, 0xABFE);

EXTERN_API( void )
GXSetFormatForm                 (gxFormat               aFormat,
                                 gxShape                form,
                                 gxShape                mask)                               FOURWORDINLINE(0x203C, 0x0000, 0x003A, 0xABFE);


/*
    PaperType Routines
*/
EXTERN_API( gxPaperType )
GXNewPaperType                  (gxJob                  aJob,
                                 Str31                  name,
                                 gxRectangle *          pageSize,
                                 gxRectangle *          paperSize)                          FOURWORDINLINE(0x203C, 0x0000, 0x000B, 0xABFE);

EXTERN_API( void )
GXDisposePaperType              (gxPaperType            aPaperType)                         FOURWORDINLINE(0x203C, 0x0000, 0x000C, 0xABFE);

EXTERN_API( gxPaperType )
GXGetNewPaperType               (gxJob                  aJob,
                                 short                  resID)                              FOURWORDINLINE(0x203C, 0x0000, 0x000D, 0xABFE);

EXTERN_API( long )
GXCountJobPaperTypes            (gxJob                  aJob,
                                 Boolean                forFormatDevice)                    FOURWORDINLINE(0x203C, 0x0000, 0x0042, 0xABFE);

EXTERN_API( gxPaperType )
GXGetJobPaperType               (gxJob                  aJob,
                                 long                   whichPaperType,
                                 Boolean                forFormatDevice,
                                 gxPaperType            aPaperType)                         FOURWORDINLINE(0x203C, 0x0000, 0x0043, 0xABFE);

EXTERN_API( void )
GXForEachJobPaperTypeDo         (gxJob                  aJob,
                                 GXPaperTypeUPP         aProc,
                                 void *                 refCon,
                                 Boolean                forFormattingPrinter)               FOURWORDINLINE(0x203C, 0x0000, 0x0044, 0xABFE);

EXTERN_API( gxPaperType )
GXCopyPaperType                 (gxPaperType            srcPaperType,
                                 gxPaperType            dstPaperType)                       FOURWORDINLINE(0x203C, 0x0000, 0x0045, 0xABFE);

EXTERN_API( void )
GXGetPaperTypeName              (gxPaperType            aPaperType,
                                 Str31                  papertypeName)                      FOURWORDINLINE(0x203C, 0x0000, 0x0046, 0xABFE);

EXTERN_API( void )
GXGetPaperTypeDimensions        (gxPaperType            aPaperType,
                                 gxRectangle *          pageSize,
                                 gxRectangle *          paperSize)                          FOURWORDINLINE(0x203C, 0x0000, 0x0047, 0xABFE);

EXTERN_API( gxJob )
GXGetPaperTypeJob               (gxPaperType            aPaperType)                         FOURWORDINLINE(0x203C, 0x0000, 0x0048, 0xABFE);

EXTERN_API( Collection )
GXGetPaperTypeCollection        (gxPaperType            aPaperType)                         FOURWORDINLINE(0x203C, 0x0000, 0x0049, 0xABFE);


/*
    Printer Routines
*/
EXTERN_API( gxPrinter )
GXGetJobFormattingPrinter       (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x0027, 0xABFE);

EXTERN_API( gxPrinter )
GXGetJobOutputPrinter           (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x0028, 0xABFE);

EXTERN_API( gxPrinter )
GXGetJobPrinter                 (gxJob                  aJob)                               FOURWORDINLINE(0x203C, 0x0000, 0x0029, 0xABFE);

EXTERN_API( gxJob )
GXGetPrinterJob                 (gxPrinter              aPrinter)                           FOURWORDINLINE(0x203C, 0x0000, 0x002A, 0xABFE);

EXTERN_API( void )
GXForEachPrinterViewDeviceDo    (gxPrinter              aPrinter,
                                 GXViewDeviceUPP        aProc,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x0000, 0x002B, 0xABFE);

EXTERN_API( long )
GXCountPrinterViewDevices       (gxPrinter              aPrinter)                           FOURWORDINLINE(0x203C, 0x0000, 0x002C, 0xABFE);

EXTERN_API( gxViewDevice )
GXGetPrinterViewDevice          (gxPrinter              aPrinter,
                                 long                   whichViewDevice)                    FOURWORDINLINE(0x203C, 0x0000, 0x002D, 0xABFE);

EXTERN_API( void )
GXSelectPrinterViewDevice       (gxPrinter              aPrinter,
                                 long                   whichViewDevice)                    FOURWORDINLINE(0x203C, 0x0000, 0x002E, 0xABFE);

EXTERN_API( void )
GXGetPrinterName                (gxPrinter              aPrinter,
                                 Str31                  printerName)                        FOURWORDINLINE(0x203C, 0x0000, 0x002F, 0xABFE);

EXTERN_API( OSType )
GXGetPrinterType                (gxPrinter              aPrinter)                           FOURWORDINLINE(0x203C, 0x0000, 0x0030, 0xABFE);

EXTERN_API( void )
GXGetPrinterDriverName          (gxPrinter              aPrinter,
                                 Str31                  driverName)                         FOURWORDINLINE(0x203C, 0x0000, 0x0031, 0xABFE);

EXTERN_API( OSType )
GXGetPrinterDriverType          (gxPrinter              aPrinter)                           FOURWORDINLINE(0x203C, 0x0000, 0x0032, 0xABFE);


/*
    Dialog Routines
*/
EXTERN_API( gxDialogResult )
GXJobDefaultFormatDialog        (gxJob                  aJob,
                                 gxEditMenuRecord *     anEditMenuRec)                      FOURWORDINLINE(0x203C, 0x0000, 0x0010, 0xABFE);

EXTERN_API( gxDialogResult )
GXJobPrintDialog                (gxJob                  aJob,
                                 gxEditMenuRecord *     anEditMenuRec)                      FOURWORDINLINE(0x203C, 0x0000, 0x0011, 0xABFE);

EXTERN_API( gxDialogResult )
GXFormatDialog                  (gxFormat               aFormat,
                                 gxEditMenuRecord *     anEditMenuRec,
                                 StringPtr              title)                              FOURWORDINLINE(0x203C, 0x0000, 0x0012, 0xABFE);

EXTERN_API( void )
GXEnableJobScalingPanel         (gxJob                  aJob,
                                 Boolean                enabled)                            FOURWORDINLINE(0x203C, 0x0000, 0x0040, 0xABFE);

EXTERN_API( void )
GXGetJobPanelDimensions         (gxJob                  aJob,
                                 Rect *                 panelArea)                          FOURWORDINLINE(0x203C, 0x0000, 0x0041, 0xABFE);


/*
    Spooling Routines
*/
EXTERN_API( void )
GXGetJobPageRange               (gxJob                  theJob,
                                 long *                 firstPage,
                                 long *                 lastPage)                           FOURWORDINLINE(0x203C, 0x0000, 0x0017, 0xABFE);

EXTERN_API( void )
GXStartJob                      (gxJob                  theJob,
                                 StringPtr              docName,
                                 long                   pageCount)                          FOURWORDINLINE(0x203C, 0x0000, 0x0018, 0xABFE);

EXTERN_API( void )
GXPrintPage                     (gxJob                  theJob,
                                 long                   pageNumber,
                                 gxFormat               theFormat,
                                 gxShape                thePage)                            FOURWORDINLINE(0x203C, 0x0000, 0x0019, 0xABFE);

EXTERN_API( Boolean )
GXStartPage                     (gxJob                  theJob,
                                 long                   pageNumber,
                                 gxFormat               theFormat,
                                 long                   numViewPorts,
                                 gxViewPort *           viewPortList)                       FOURWORDINLINE(0x203C, 0x0000, 0x001A, 0xABFE);

EXTERN_API( void )
GXFinishPage                    (gxJob                  theJob)                             FOURWORDINLINE(0x203C, 0x0000, 0x001B, 0xABFE);

EXTERN_API( void )
GXFinishJob                     (gxJob                  theJob)                             FOURWORDINLINE(0x203C, 0x0000, 0x001C, 0xABFE);


/*
    PrintFile Routines
*/
EXTERN_API( gxPrintFile )
GXOpenPrintFile                 (gxJob                  theJob,
                                 FSSpecPtr              anFSSpec,
                                 char                   permission)                         FOURWORDINLINE(0x203C, 0x0000, 0x004A, 0xABFE);

EXTERN_API( void )
GXClosePrintFile                (gxPrintFile            aPrintFile)                         FOURWORDINLINE(0x203C, 0x0000, 0x004B, 0xABFE);

EXTERN_API( gxJob )
GXGetPrintFileJob               (gxPrintFile            aPrintFile)                         FOURWORDINLINE(0x203C, 0x0000, 0x004C, 0xABFE);

EXTERN_API( long )
GXCountPrintFilePages           (gxPrintFile            aPrintFile)                         FOURWORDINLINE(0x203C, 0x0000, 0x004D, 0xABFE);

EXTERN_API( void )
GXReadPrintFilePage             (gxPrintFile            aPrintFile,
                                 long                   pageNumber,
                                 long                   numViewPorts,
                                 gxViewPort *           viewPortList,
                                 gxFormat *             pgFormat,
                                 gxShape *              pgShape)                            FOURWORDINLINE(0x203C, 0x0000, 0x004E, 0xABFE);

EXTERN_API( void )
GXReplacePrintFilePage          (gxPrintFile            aPrintFile,
                                 long                   pageNumber,
                                 gxFormat               aFormat,
                                 gxShape                aShape)                             FOURWORDINLINE(0x203C, 0x0000, 0x004F, 0xABFE);

EXTERN_API( void )
GXInsertPrintFilePage           (gxPrintFile            aPrintFile,
                                 long                   atPageNumber,
                                 gxFormat               pgFormat,
                                 gxShape                pgShape)                            FOURWORDINLINE(0x203C, 0x0000, 0x0050, 0xABFE);

EXTERN_API( void )
GXDeletePrintFilePageRange      (gxPrintFile            aPrintFile,
                                 long                   fromPageNumber,
                                 long                   toPageNumber)                       FOURWORDINLINE(0x203C, 0x0000, 0x0051, 0xABFE);

EXTERN_API( void )
GXSavePrintFile                 (gxPrintFile            aPrintFile,
                                 FSSpec *               anFSSpec)                           FOURWORDINLINE(0x203C, 0x0000, 0x0052, 0xABFE);


/*
    ColorSync Routines
*/
EXTERN_API( long )
GXFindPrinterProfile            (gxPrinter              aPrinter,
                                 void *                 searchData,
                                 long                   index,
                                 gxColorProfile *       returnedProfile)                    FOURWORDINLINE(0x203C, 0x0000, 0x0053, 0xABFE);

EXTERN_API( long )
GXFindFormatProfile             (gxFormat               aFormat,
                                 void *                 searchData,
                                 long                   index,
                                 gxColorProfile *       returnedProfile)                    FOURWORDINLINE(0x203C, 0x0000, 0x0054, 0xABFE);

EXTERN_API( void )
GXSetPrinterProfile             (gxPrinter              aPrinter,
                                 gxColorProfile         oldProfile,
                                 gxColorProfile         newProfile)                         FOURWORDINLINE(0x203C, 0x0000, 0x0055, 0xABFE);

EXTERN_API( void )
GXSetFormatProfile              (gxFormat               aFormat,
                                 gxColorProfile         oldProfile,
                                 gxColorProfile         newProfile)                         FOURWORDINLINE(0x203C, 0x0000, 0x0056, 0xABFE);


/************************************************************************
                        Start of old "GXPrintingResEquates.h/a/p" interface file.
                *************************************************************************/
/*    ------------------------------------
                Basic client types
    ------------------------------------ */
#endif  /* CALL_NOT_IN_CARBON */

enum {
    gxPrintingManagerType       = FOUR_CHAR_CODE('pmgr'),
    gxImagingSystemType         = FOUR_CHAR_CODE('gxis'),
    gxPrinterDriverType         = FOUR_CHAR_CODE('pdvr'),
    gxPrintingExtensionType     = FOUR_CHAR_CODE('pext'),
    gxUnknownPrinterType        = FOUR_CHAR_CODE('none'),
    gxAnyPrinterType            = FOUR_CHAR_CODE('univ'),
    gxQuickdrawPrinterType      = FOUR_CHAR_CODE('qdrw'),
    gxPortableDocPrinterType    = FOUR_CHAR_CODE('gxpd'),
    gxRasterPrinterType         = FOUR_CHAR_CODE('rast'),
    gxPostscriptPrinterType     = FOUR_CHAR_CODE('post'),
    gxVectorPrinterType         = FOUR_CHAR_CODE('vect')
};

/* All pre-defined printing collection items have this ID */
enum {
    gxPrintingTagID             = -28672
};


/*    ----------------------------------------------------------------------

        Resource types and IDs used by both extension and driver writers

    ---------------------------------------------------------------------- */
/* Resources in a printer driver or extension must be based off of these IDs */
enum {
    gxPrintingDriverBaseID      = -27648,
    gxPrintingExtensionBaseID   = -27136
};

/*    Override resources tell the system what messages a driver or extension
        is overriding.  A driver may have a series of these resources. */
/* Override resource type for 68k resource-based code:*/

enum {
    gxOverrideType              = FOUR_CHAR_CODE('over')
};


/* Override resource type for PowerPC datafork-based code:*/

enum {
    gxNativeOverrideType        = FOUR_CHAR_CODE('povr')
};


/*    --------------------------------------------------------------

        Message ID definitions by both extension and driver writers

    --------------------------------------------------------------- */
/* Identifiers for universal message overrides. */
enum {
    gxInitializeMsg             = 0,
    gxShutDownMsg               = 1,
    gxJobIdleMsg                = 2,
    gxJobStatusMsg              = 3,
    gxPrintingEventMsg          = 4,
    gxJobDefaultFormatDialogMsg = 5,
    gxFormatDialogMsg           = 6,
    gxJobPrintDialogMsg         = 7,
    gxFilterPanelEventMsg       = 8,
    gxHandlePanelEventMsg       = 9,
    gxParsePageRangeMsg         = 10,
    gxDefaultJobMsg             = 11,
    gxDefaultFormatMsg          = 12,
    gxDefaultPaperTypeMsg       = 13,
    gxDefaultPrinterMsg         = 14,
    gxCreateSpoolFileMsg        = 15,
    gxSpoolPageMsg              = 16,
    gxSpoolDataMsg              = 17,
    gxSpoolResourceMsg          = 18,
    gxCompleteSpoolFileMsg      = 19,
    gxCountPagesMsg             = 20,
    gxDespoolPageMsg            = 21,
    gxDespoolDataMsg            = 22,
    gxDespoolResourceMsg        = 23,
    gxCloseSpoolFileMsg         = 24,
    gxStartJobMsg               = 25,
    gxFinishJobMsg              = 26,
    gxStartPageMsg              = 27,
    gxFinishPageMsg             = 28,
    gxPrintPageMsg              = 29,
    gxSetupImageDataMsg         = 30,
    gxImageJobMsg               = 31,
    gxImageDocumentMsg          = 32,
    gxImagePageMsg              = 33,
    gxRenderPageMsg             = 34,
    gxCreateImageFileMsg        = 35,
    gxOpenConnectionMsg         = 36,
    gxCloseConnectionMsg        = 37,
    gxStartSendPageMsg          = 38,
    gxFinishSendPageMsg         = 39,
    gxWriteDataMsg              = 40,
    gxBufferDataMsg             = 41,
    gxDumpBufferMsg             = 42,
    gxFreeBufferMsg             = 43,
    gxCheckStatusMsg            = 44,
    gxGetDeviceStatusMsg        = 45,
    gxFetchTaggedDataMsg        = 46,
    gxGetDTPMenuListMsg         = 47,
    gxDTPMenuSelectMsg          = 48,
    gxHandleAlertFilterMsg      = 49,
    gxJobFormatModeQueryMsg     = 50,
    gxWriteStatusToDTPWindowMsg = 51,
    gxInitializeStatusAlertMsg  = 52,
    gxHandleAlertStatusMsg      = 53,
    gxHandleAlertEventMsg       = 54,
    gxCleanupStartJobMsg        = 55,
    gxCleanupStartPageMsg       = 56,
    gxCleanupOpenConnectionMsg  = 57,
    gxCleanupStartSendPageMsg   = 58,
    gxDefaultDesktopPrinterMsg  = 59,
    gxCaptureOutputDeviceMsg    = 60,
    gxOpenConnectionRetryMsg    = 61,
    gxExamineSpoolFileMsg       = 62,
    gxFinishSendPlaneMsg        = 63,
    gxDoesPaperFitMsg           = 64,
    gxChooserMessageMsg         = 65,
    gxFindPrinterProfileMsg     = 66,
    gxFindFormatProfileMsg      = 67,
    gxSetPrinterProfileMsg      = 68,
    gxSetFormatProfileMsg       = 69,
    gxHandleAltDestinationMsg   = 70,
    gxSetupPageImageDataMsg     = 71
};


/* Identifiers for Quickdraw message overrides. */
enum {
    gxPrOpenDocMsg              = 0,
    gxPrCloseDocMsg             = 1,
    gxPrOpenPageMsg             = 2,
    gxPrClosePageMsg            = 3,
    gxPrintDefaultMsg           = 4,
    gxPrStlDialogMsg            = 5,
    gxPrJobDialogMsg            = 6,
    gxPrStlInitMsg              = 7,
    gxPrJobInitMsg              = 8,
    gxPrDlgMainMsg              = 9,
    gxPrValidateMsg             = 10,
    gxPrJobMergeMsg             = 11,
    gxPrGeneralMsg              = 12,
    gxConvertPrintRecordToMsg   = 13,
    gxConvertPrintRecordFromMsg = 14,
    gxPrintRecordToJobMsg       = 15
};


/* Identifiers for raster imaging message overrides. */
enum {
    gxRasterDataInMsg           = 0,
    gxRasterLineFeedMsg         = 1,
    gxRasterPackageBitmapMsg    = 2
};


/* Identifiers for PostScript imaging message overrides. */
enum {
    gxPostscriptQueryPrinterMsg = 0,
    gxPostscriptInitializePrinterMsg = 1,
    gxPostscriptResetPrinterMsg = 2,
    gxPostscriptExitServerMsg   = 3,
    gxPostscriptGetStatusTextMsg = 4,
    gxPostscriptGetPrinterTextMsg = 5,
    gxPostscriptScanStatusTextMsg = 6,
    gxPostscriptScanPrinterTextMsg = 7,
    gxPostscriptGetDocumentProcSetListMsg = 8,
    gxPostscriptDownloadProcSetListMsg = 9,
    gxPostscriptGetPrinterGlyphsInformationMsg = 10,
    gxPostscriptStreamFontMsg   = 11,
    gxPostscriptDoDocumentHeaderMsg = 12,
    gxPostscriptDoDocumentSetUpMsg = 13,
    gxPostscriptDoDocumentTrailerMsg = 14,
    gxPostscriptDoPageSetUpMsg  = 15,
    gxPostscriptSelectPaperTypeMsg = 16,
    gxPostscriptDoPageTrailerMsg = 17,
    gxPostscriptEjectPageMsg    = 18,
    gxPostscriptProcessShapeMsg = 19,
    gxPostScriptEjectPendingPageMsg = 20
};


/* Identifiers for Vector imaging message overrides. */
enum {
    gxVectorPackageDataMsg      = 0,
    gxVectorLoadPensMsg         = 1,
    gxVectorVectorizeShapeMsg   = 2
};


/* Dialog related resource types */
enum {
    gxPrintingAlertType         = FOUR_CHAR_CODE('plrt'),
    gxStatusType                = FOUR_CHAR_CODE('stat'),
    gxExtendedDITLType          = FOUR_CHAR_CODE('xdtl'),
    gxPrintPanelType            = FOUR_CHAR_CODE('ppnl'),
    gxCollectionType            = FOUR_CHAR_CODE('cltn')
};


/* Communication resource types */
/*
    The looker resource is used by the Chooser PACK to determine what kind
    of communications this driver supports. (In order to generate/handle the 
    pop-up menu for "Connect via:".
    
    The looker resource is also used by PrinterShare to determine the AppleTalk NBP Type
    for servers created for this driver.
*/
enum {
    gxLookerType                = FOUR_CHAR_CODE('look'),
    gxLookerID                  = -4096
};


/* The communications method and private data used to connect to the printer */
enum {
    gxDeviceCommunicationsType  = FOUR_CHAR_CODE('comm')
};

/*    -------------------------------------------------

    Resource types and IDs used by extension writers

    ------------------------------------------------- */
enum {
    gxExtensionUniversalOverrideID = gxPrintingExtensionBaseID
};

enum {
    gxExtensionImagingOverrideSelectorID = gxPrintingExtensionBaseID
};

enum {
    gxExtensionScopeType        = FOUR_CHAR_CODE('scop'),
    gxDriverScopeID             = gxPrintingExtensionBaseID,
    gxPrinterScopeID            = gxPrintingExtensionBaseID + 1,
    gxPrinterExceptionScopeID   = gxPrintingExtensionBaseID + 2
};

enum {
    gxExtensionLoadType         = FOUR_CHAR_CODE('load'),
    gxExtensionLoadID           = gxPrintingExtensionBaseID
};

enum {
    gxExtensionLoadFirst        = 0x00000100,
    gxExtensionLoadAnywhere     = 0x7FFFFFFF,
    gxExtensionLoadLast         = (long)0xFFFFFF00
};

enum {
    gxExtensionOptimizationType = FOUR_CHAR_CODE('eopt'),
    gxExtensionOptimizationID   = gxPrintingExtensionBaseID
};


/*    -----------------------------------------------

    Resource types and IDs used by driver writers

    ----------------------------------------------- */
enum {
    gxDriverUniversalOverrideID = gxPrintingDriverBaseID,
    gxDriverImagingOverrideID   = gxPrintingDriverBaseID + 1,
    gxDriverCompatibilityOverrideID = gxPrintingDriverBaseID + 2
};

enum {
    gxDriverFileFormatType      = FOUR_CHAR_CODE('pfil'),
    gxDriverFileFormatID        = gxPrintingDriverBaseID
};


enum {
    gxDestinationAdditionType   = FOUR_CHAR_CODE('dsta'),
    gxDestinationAdditionID     = gxPrintingDriverBaseID
};


/* IMAGING RESOURCES */
/*    The imaging system resource specifies which imaging system a printer
        driver wishes to use. */

enum {
    gxImagingSystemSelectorType = FOUR_CHAR_CODE('isys'),
    gxImagingSystemSelectorID   = gxPrintingDriverBaseID
};


/* 'exft' resource ID -- exclude font list */
enum {
    kExcludeFontListType        = FOUR_CHAR_CODE('exft'),
    kExcludeFontListID          = gxPrintingDriverBaseID
};

/* Resource for type for color matching */
enum {
    gxColorMatchingDataType     = FOUR_CHAR_CODE('prof'),
    gxColorMatchingDataID       = gxPrintingDriverBaseID
};


/* Resource type and id for the tray count */
enum {
    gxTrayCountDataType         = FOUR_CHAR_CODE('tray'),
    gxTrayCountDataID           = gxPrintingDriverBaseID
};


/* Resource type for the tray names */
enum {
    gxTrayNameDataType          = FOUR_CHAR_CODE('tryn')
};


/* Resource type for manual feed preferences, stored in DTP. */
enum {
    gxManualFeedAlertPrefsType  = FOUR_CHAR_CODE('mfpr'),
    gxManualFeedAlertPrefsID    = gxPrintingDriverBaseID
};

/* Resource type for desktop printer output characteristics, stored in DTP. */
enum {
    gxDriverOutputType          = FOUR_CHAR_CODE('outp'),
    gxDriverOutputTypeID        = 1
};


/* IO Resources */
/* Resource type and ID for default IO and buffering resources */
enum {
    gxUniversalIOPrefsType      = FOUR_CHAR_CODE('iobm'),
    gxUniversalIOPrefsID        = gxPrintingDriverBaseID
};


/*    Resource types and IDs for default implementation of CaptureOutputDevice.
        The default implementation of CaptureOutputDevice only handles PAP devices */
enum {
    gxCaptureType               = FOUR_CHAR_CODE('cpts'),
    gxCaptureStringID           = gxPrintingDriverBaseID,
    gxReleaseStringID           = gxPrintingDriverBaseID + 1,
    gxUncapturedAppleTalkType   = gxPrintingDriverBaseID + 2,
    gxCapturedAppleTalkType     = gxPrintingDriverBaseID + 3
};


/* Resource type and ID for custom halftone matrix */
enum {
    gxCustomMatrixType          = FOUR_CHAR_CODE('dmat'),
    gxCustomMatrixID            = gxPrintingDriverBaseID
};

/* Resource type and ID for raster driver rendering preferences */
enum {
    gxRasterPrefsType           = FOUR_CHAR_CODE('rdip'),
    gxRasterPrefsID             = gxPrintingDriverBaseID
};


/* Resource type for specifiying a colorset */
enum {
    gxColorSetResType           = FOUR_CHAR_CODE('crst')
};


/* Resource type and ID for raster driver packaging preferences */
enum {
    gxRasterPackType            = FOUR_CHAR_CODE('rpck'),
    gxRasterPackID              = gxPrintingDriverBaseID
};


/* Resource type and ID for raster driver packaging options */

enum {
    gxRasterNumNone             = 0,                            /* Number isn't output at all */
    gxRasterNumDirect           = 1,                            /* Lowest minWidth bytes as data */
    gxRasterNumToASCII          = 2                             /* minWidth ASCII characters */
};


enum {
    gxRasterPackOptionsType     = FOUR_CHAR_CODE('ropt'),
    gxRasterPackOptionsID       = gxPrintingDriverBaseID
};


/* Resource type for the PostScript imaging system procedure set control resource */
enum {
    gxPostscriptProcSetControlType = FOUR_CHAR_CODE('prec')
};


/* Resource type for the PostScript imaging system printer font resource */
enum {
    gxPostscriptPrinterFontType = FOUR_CHAR_CODE('pfnt')
};


/* Resource type and ID for the PostScript imaging system imaging preferences */
enum {
    gxPostscriptPrefsType       = FOUR_CHAR_CODE('pdip'),
    gxPostscriptPrefsID         = gxPrintingDriverBaseID
};

/* Resource type and ID for the PostScript imaging system default scanning code */
enum {
    gxPostscriptScanningType    = FOUR_CHAR_CODE('scan'),
    gxPostscriptScanningID      = gxPrintingDriverBaseID
};


/* Old Application Support Resources */
enum {
    gxCustType                  = FOUR_CHAR_CODE('cust'),
    gxCustID                    = -8192
};


enum {
    gxReslType                  = FOUR_CHAR_CODE('resl'),
    gxReslID                    = -8192
};


enum {
    gxDiscreteResolution        = 0
};


enum {
    gxStlDialogResID            = -8192
};


enum {
    gxJobDialogResID            = -8191
};


enum {
    gxScaleTableType            = FOUR_CHAR_CODE('stab'),
    gxDITLControlType           = FOUR_CHAR_CODE('dctl')
};

/*    The default implementation of gxPrintDefault loads and
    PrValidates a print record stored in the following driver resource. */
enum {
    gxPrintRecordType           = FOUR_CHAR_CODE('PREC'),
    gxDefaultPrintRecordID      = 0
};


/*
    -----------------------------------------------

    Resource types and IDs used in papertype files

    -----------------------------------------------
*/
/* Resource type and ID for driver papertypes placed in individual files */
enum {
    gxSignatureType             = FOUR_CHAR_CODE('sig '),
    gxPapertypeSignatureID      = 0
};


/* Papertype creator types */
enum {
    gxDrvrPaperType             = FOUR_CHAR_CODE('drpt'),
    gxSysPaperType              = FOUR_CHAR_CODE('sypt'),       /* System paper type creator */
    gxUserPaperType             = FOUR_CHAR_CODE('uspt'),       /* User paper type creator */
                                                                /* Driver creator types == driver file's creator value */
    gxPaperTypeType             = FOUR_CHAR_CODE('ptyp')
};


/*********************************************************************
                    Start of old "GXPrintingMessages.h/a/p" interface file.
            **********************************************************************/
/* ------------------------------------------------------------------------------

                                    Constants and Types

-------------------------------------------------------------------------------- */
/*

    ABSTRACT DATA TYPES

*/
typedef struct OpaquegxSpoolFile*       gxSpoolFile;
/*

    DIALOG PANEL CONSTANTS AND TYPES

*/

typedef long                            gxPanelEvent;
/* Dialog panel event equates */
enum {
    gxPanelNoEvt                = 0L,
    gxPanelOpenEvt              = 1L,                           /* Initialize and draw */
    gxPanelCloseEvt             = 2L,                           /* Your panel is going away (panel switchL, confirm or cancel) */
    gxPanelHitEvt               = 3L,                           /* There's a hit in your panel */
    gxPanelActivateEvt          = 4L,                           /* The dialog window has just been activated */
    gxPanelDeactivateEvt        = 5L,                           /* The dialog window is about to be deactivated */
    gxPanelIconFocusEvt         = 6L,                           /* The focus changes from the panel to the icon list */
    gxPanelPanelFocusEvt        = 7L,                           /* The focus changes from the icon list to the panel */
    gxPanelFilterEvt            = 8L,                           /* Every event is filtered */
    gxPanelCancelEvt            = 9L,                           /* The user has cancelled the dialog */
    gxPanelConfirmEvt           = 10L,                          /* The user has confirmed the dialog */
    gxPanelDialogEvt            = 11L,                          /* Event to be handle by dialoghandler */
    gxPanelOtherEvt             = 12L,                          /* osEvts, etc. */
    gxPanelUserWillConfirmEvt   = 13L                           /* User has selected confirm, time to parse panel interdependencies */
};


/* Constants for panel responses to dialog handler calls */
typedef long                            gxPanelResult;
enum {
    gxPanelNoResult             = 0,
    gxPanelCancelConfirmation   = 1                             /* Only valid from panelUserWillConfirmEvt - used to keep the dialog from going away */
};


/* Panel event info record for FilterPanelEvent and HandlePanelEvent messages */

struct gxPanelInfoRecord {
    gxPanelEvent                    panelEvt;                   /* Why we were called */
    short                           panelResId;                 /* 'ppnl' resource ID of current panel */
    DialogPtr                       pDlg;                       /* Pointer to dialog */
    EventRecord *                   theEvent;                   /* Pointer to event */
    short                           itemHit;                    /* Actual item number as Dialog Mgr thinks */
    short                           itemCount;                  /* Number of items before your items */
    short                           evtAction;                  /* Once this event is processed, the action that will result */
                                                                /* (evtAction is only meaningful during filtering) */
    short                           errorStringId;              /* STR ID of string to put in error alert (0 means no string) */
    gxFormat                        theFormat;                  /* The current format (only meaningful in a format dialog) */
    void *                          refCon;                     /* refCon passed in PanelSetupRecord */
};
typedef struct gxPanelInfoRecord        gxPanelInfoRecord;
/* Constants for the evtAction field in PanelInfoRecord */
enum {
    gxOtherAction               = 0,                            /* Current item will not change */
    gxClosePanelAction          = 1,                            /* Panel will be closed */
    gxCancelDialogAction        = 2,                            /* Dialog will be cancelled */
    gxConfirmDialogAction       = 3                             /* Dialog will be confirmed */
};


/* Constants for the panelKind field in gxPanelSetupRecord */

typedef long                            gxPrintingPanelKind;
/* The gxPanelSetupInfo structure is passed to GXSetupDialogPanel */

struct gxPanelSetupRecord {
    gxPrintingPanelKind             panelKind;
    short                           panelResId;
    short                           resourceRefNum;
    void *                          refCon;
};
typedef struct gxPanelSetupRecord       gxPanelSetupRecord;
enum {
    gxApplicationPanel          = 0L,
    gxExtensionPanel            = 1L,
    gxDriverPanel               = 2L
};


/* Constants returned by gxParsePageRange message */

typedef long                            gxParsePageRangeResult;
enum {
    gxRangeNotParsed            = 0L,                           /* Default initial value */
    gxRangeParsed               = 1L,                           /* Range has been parsed */
    gxRangeBadFromValue         = 2L,                           /* From value is bad */
    gxRangeBadToValue           = 3L                            /* To value is bad */
};

/*

    STATUS-RELATED CONSTANTS AND TYPES

*/

/* Structure for status messages */

struct gxStatusRecord {
    unsigned short                  statusType;                 /* One of the ids listed above (nonFatalError, etc. ) */
    unsigned short                  statusId;                   /* Specific status (out of paper, etc.) */
    unsigned short                  statusAlertId;              /*    Printing alert ID (if any) for status */
    gxOwnerSignature                statusOwner;                /* Creator type of status owner */
    short                           statResId;                  /* ID for 'stat' resource */
    short                           statResIndex;               /* Index into 'stat' resource for this status */
    short                           dialogResult;               /* ID of button string selected on dismissal of printing alert */
    unsigned short                  bufferLen;                  /* Number of bytes in status buffer - total record size must be <= 512 */
    char                            statusBuffer[1];            /* User response from alert */
};
typedef struct gxStatusRecord           gxStatusRecord;

/* Constants for statusType field of gxStatusRecord */
enum {
    gxNonFatalError             = 1,                            /* An error occurred, but the job can continue */
    gxFatalError                = 2,                            /* A fatal error occurred-- halt job */
    gxPrinterReady              = 3,                            /* Tells QDGX to leave alert mode */
    gxUserAttention             = 4,                            /* Signals initiation of a modal alert */
    gxUserAlert                 = 5,                            /* Signals initiation of a moveable modal alert */
    gxPageTransmission          = 6,                            /* Signals page sent to printer, increments page count in strings to user */
    gxOpenConnectionStatus      = 7,                            /* Signals QDGX to begin animation on printer icon */
    gxInformationalStatus       = 8,                            /* Default status type, no side effects */
    gxSpoolingPageStatus        = 9,                            /* Signals page spooled, increments page count in spooling dialog */
    gxEndStatus                 = 10,                           /* Signals end of spooling */
    gxPercentageStatus          = 11                            /* Signals QDGX as to the amount of the job which is currently complete */
};


/* Structure for gxWriteStatusToDTPWindow message */

struct gxDisplayRecord {
    Boolean                         useText;                    /* Use text as opposed to a picture */
    char                            padByte;
    Handle                          hPicture;                   /* if !useText, the picture handle */
    Str255                          theText;                    /* if useText, the text */
};
typedef struct gxDisplayRecord          gxDisplayRecord;

/*-----------------------------------------------*/
/* paper mapping-related constants and types...  */
/*-----------------------------------------------*/


typedef long                            gxTrayMapping;
enum {
    gxDefaultTrayMapping        = 0L,
    gxConfiguredTrayMapping     = 1L
};



/* ------------------------------------------------------------------------------

                API Functions callable only from within message overrides

-------------------------------------------------------------------------------- */
#define GXPRINTINGDISPATCH(segID, selector) {0x203C, 0x0001, 0, 0x223C, (segID & 0x0FFF), selector << 2, 0xABFE}

/*
    Message Sending API Routines
*/
#if TARGET_CPU_68K
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
GXPrintingDispatch              (long                   selector,
                                 ...)                                                       SIXWORDINLINE(0x221F, 0x203C, 0x0001, 0x0000, 0xABFE, 0x598F);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_CPU_68K */

/*

    How to use the GXPRINTINGDISPATCH macro...
    
    If your driver or extension is large, you may want to segment it
    across smaller boundaries than is permitted by the messaging system.
    Without using the Printing Manager's segmentation manager directly,
    the smallest segment you can create consists of the code to override
    a single message.  If you are overriding workhorse messages such as
    RenderPage, you may want to divide up the work among many functions
    distributed across several segments.  Here's how...
    
    The Printing Manager segment scheme involves the construction of a
    single 32-bit dispatch selector, which contains all the information
    necessary for the dispatcher to find a single routine.  It contains the
    segment's resource ID, and the offset within the segment which contains
    the start of the routine.  The GXPRINTINGDISPATCH macro will construct the
    dispatch selector for you, as well as the code to do the dispatch.
    
    Usually, it is convenient to start your segment with a long aligned jump table,
    beginning after the 4 byte header required by the Printing Manager.  The
    macro assumes this is the case and takes a 1-based routine selector from
    which it conmstructs the offset.
    
    For example, if your code is in resource 'pdvr' (print driver), ID=2
    at offset=12 (third routine in segment), you would declare your
    routine as follows:
    
    OSErr MyRenderingRoutine (long param1, Ptr param2)
        = GXPRINTINGDISPATCH(2, 3);
        
    Remember, ALL segment dispatches must return OSErr.  If your routine
    does not generate errors, you must still declare it to return OSErr
    and have the routine itself return noErr.
    
    An alternative way to call across segments is to call the GXPrintingDispatch
    function directly.  You must construct the 32-bit selector yourself and pass
    it as the first parameter.  This is usually not preferable since you don't get
    type-checking unless you declare a prototype as shown above, and your code
    isn't as easy to read.
    
    So given the above prototype, there are two ways to call the function:
    
        anErr = MyRenderingRoutine(p1, p2);         // Free type checking!
        
    or:
    
        #define kMyRenderRoutineSelector 0x0002000C
        anErr = GXPrintingDispatch(kMyRenderRoutineSelector, p1, p2);       // No type-checking!
    
    
    Both have the same effect.

*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( gxJob )
GXGetJob                        (void)                                                      FOURWORDINLINE(0x203C, 0x0001, 0x0001, 0xABFE);

EXTERN_API_C( short )
GXGetMessageHandlerResFile      (void)                                                      FOURWORDINLINE(0x203C, 0x0001, 0x0002, 0xABFE);

EXTERN_API_C( Boolean )
GXSpoolingAborted               (void)                                                      FOURWORDINLINE(0x203C, 0x0001, 0x0003, 0xABFE);

EXTERN_API_C( OSErr )
GXJobIdle                       (void)                                                      FOURWORDINLINE(0x203C, 0x0001, 0x0004, 0xABFE);

EXTERN_API_C( OSErr )
GXReportStatus                  (long                   statusID,
                                 unsigned long          statusIndex)                        FOURWORDINLINE(0x203C, 0x0001, 0x0005, 0xABFE);

EXTERN_API_C( OSErr )
GXAlertTheUser                  (gxStatusRecord *       statusRec)                          FOURWORDINLINE(0x203C, 0x0001, 0x0006, 0xABFE);

EXTERN_API_C( OSErr )
GXSetupDialogPanel              (gxPanelSetupRecord *   panelRec)                           FOURWORDINLINE(0x203C, 0x0001, 0x0007, 0xABFE);

EXTERN_API_C( OSErr )
GXCountTrays                    (gxTrayIndex *          numTrays)                           FOURWORDINLINE(0x203C, 0x0001, 0x0008, 0xABFE);

EXTERN_API_C( OSErr )
GXGetTrayName                   (gxTrayIndex            trayNumber,
                                 Str31                  trayName)                           FOURWORDINLINE(0x203C, 0x0001, 0x0009, 0xABFE);

EXTERN_API_C( OSErr )
GXSetTrayPaperType              (gxTrayIndex            whichTray,
                                 gxPaperType            aPapertype)                         FOURWORDINLINE(0x203C, 0x0001, 0x000A, 0xABFE);

EXTERN_API_C( OSErr )
GXGetTrayPaperType              (gxTrayIndex            whichTray,
                                 gxPaperType            aPapertype)                         FOURWORDINLINE(0x203C, 0x0001, 0x000B, 0xABFE);

EXTERN_API_C( OSErr )
GXGetTrayMapping                (gxTrayMapping *        trayMapping)                        FOURWORDINLINE(0x203C, 0x0001, 0x000C, 0xABFE);

EXTERN_API_C( void )
GXCleanupStartJob               (void)                                                      FOURWORDINLINE(0x203C, 0x0001, 0x000D, 0xABFE);

EXTERN_API_C( void )
GXCleanupStartPage              (void)                                                      FOURWORDINLINE(0x203C, 0x0001, 0x000E, 0xABFE);

EXTERN_API_C( void )
GXCleanupOpenConnection         (void)                                                      FOURWORDINLINE(0x203C, 0x0001, 0x000F, 0xABFE);

EXTERN_API_C( void )
GXCleanupStartSendPage          (void)                                                      FOURWORDINLINE(0x203C, 0x0001, 0x0010, 0xABFE);


/* ------------------------------------------------------------------------------

                    Constants and types for Universal Printing Messages

-------------------------------------------------------------------------------- */

/* Options for gxCreateSpoolFile message */
#endif  /* CALL_NOT_IN_CARBON */

enum {
    gxNoCreateOptions           = 0x00000000,                   /* Just create the file */
    gxInhibitAlias              = 0x00000001,                   /* Do not create an alias in the PMD folder */
    gxInhibitUniqueName         = 0x00000002,                   /* Do not append to the filename to make it unique */
    gxResolveBitmapAlias        = 0x00000004                    /* Resolve bitmap aliases and duplicate data in file */
};


/* Options for gxCloseSpoolFile message */
enum {
    gxNoCloseOptions            = 0x00000000,                   /* Just close the file */
    gxDeleteOnClose             = 0x00000001,                   /* Delete the file rather than closing it */
    gxUpdateJobData             = 0x00000002,                   /* Write current job information into file prior to closing */
    gxMakeRemoteFile            = 0x00000004                    /* Mark job as a remote file */
};


/* Options for gxCreateImageFile message */
enum {
    gxNoImageFile               = 0x00000000,                   /* Don't create image file */
    gxMakeImageFile             = 0x00000001,                   /* Create an image file */
    gxEachPlane                 = 0x00000002,                   /* Only save up planes before rewinding */
    gxEachPage                  = 0x00000004,                   /* Save up entire pages before rewinding */
    gxEntireFile                = gxEachPlane + gxEachPage      /* Save up the entire file before rewinding */
};


/* Options for gxBufferData message */
enum {
    gxNoBufferOptions           = 0x00000000,
    gxMakeBufferHex             = 0x00000001,
    gxDontSplitBuffer           = 0x00000002
};


/* Structure for gxDumpBuffer and gxFreeBuffer messages */

struct gxPrintingBuffer {
    long                            size;                       /* Size of buffer in bytes */
    long                            userData;                   /* Client assigned id for the buffer */
    char                            data[1];                    /* Array of size bytes */
};
typedef struct gxPrintingBuffer         gxPrintingBuffer;

/* Structure for gxRenderPage message */

struct gxPageInfoRecord {
    long                            docPageNum;                 /* Number of page being printed */
    long                            copyNum;                    /* Copy number being printed */
    Boolean                         formatChanged;              /* True if format changed from last page */
    Boolean                         pageChanged;                /* True if page contents changed from last page */
    long                            internalUse;                /* Private */
};
typedef struct gxPageInfoRecord         gxPageInfoRecord;
/* ------------------------------------------------------------------------------
    
                                    Universal Printing Messages
    
    -------------------------------------------------------------------------------- */
typedef CALLBACK_API_C( OSErr , GXJobIdleProcPtr )(void );
typedef STACK_UPP_TYPE(GXJobIdleProcPtr)                        GXJobIdleUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXJobIdleUPP)
    NewGXJobIdleUPP                (GXJobIdleProcPtr        userRoutine);

    EXTERN_API(void)
    DisposeGXJobIdleUPP            (GXJobIdleUPP            userUPP);

    EXTERN_API(OSErr)
    InvokeGXJobIdleUPP             (GXJobIdleUPP            userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXJobIdleProcInfo = 0x00000021 };                     /* 2_bytes Func() */
    #define NewGXJobIdleUPP(userRoutine)                            (GXJobIdleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXJobIdleProcInfo, GetCurrentArchitecture())
    #define DisposeGXJobIdleUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define InvokeGXJobIdleUPP(userUPP)                             (OSErr)CALL_ZERO_PARAMETER_UPP((userUPP), uppGXJobIdleProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXJobIdleProc(userRoutine)                           NewGXJobIdleUPP(userRoutine)
#define CallGXJobIdleProc(userRoutine)                          InvokeGXJobIdleUPP(userRoutine)
#define Send_GXJobIdle() MacSendMessage(0x00000002)
#define Forward_GXJobIdle() ForwardThisMessage((void *) (0))

typedef CALLBACK_API_C( OSErr , GXJobStatusProcPtr )(gxStatusRecord *pStatus);
typedef STACK_UPP_TYPE(GXJobStatusProcPtr)                      GXJobStatusUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXJobStatusUPP)
    NewGXJobStatusUPP              (GXJobStatusProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeGXJobStatusUPP          (GXJobStatusUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeGXJobStatusUPP           (gxStatusRecord *        pStatus,
                                    GXJobStatusUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXJobStatusProcInfo = 0x000000E1 };                   /* 2_bytes Func(4_bytes) */
    #define NewGXJobStatusUPP(userRoutine)                          (GXJobStatusUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXJobStatusProcInfo, GetCurrentArchitecture())
    #define DisposeGXJobStatusUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeGXJobStatusUPP(pStatus, userUPP)                  (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXJobStatusProcInfo, (pStatus))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXJobStatusProc(userRoutine)                         NewGXJobStatusUPP(userRoutine)
#define CallGXJobStatusProc(userRoutine, pStatus)               InvokeGXJobStatusUPP(pStatus, userRoutine)
#define Send_GXJobStatus(pStatus) \
          MacSendMessage(0x00000003, pStatus)
 
#define Forward_GXJobStatus(pStatus) \
          ForwardThisMessage((void *) (pStatus))

typedef CALLBACK_API_C( OSErr , GXPrintingEventProcPtr )(EventRecord *evtRecord, Boolean filterEvent);
typedef STACK_UPP_TYPE(GXPrintingEventProcPtr)                  GXPrintingEventUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXPrintingEventUPP)
    NewGXPrintingEventUPP          (GXPrintingEventProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeGXPrintingEventUPP      (GXPrintingEventUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeGXPrintingEventUPP       (EventRecord *           evtRecord,
                                    Boolean                 filterEvent,
                                    GXPrintingEventUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXPrintingEventProcInfo = 0x000001E1 };               /* 2_bytes Func(4_bytes, 1_byte) */
    #define NewGXPrintingEventUPP(userRoutine)                      (GXPrintingEventUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXPrintingEventProcInfo, GetCurrentArchitecture())
    #define DisposeGXPrintingEventUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeGXPrintingEventUPP(evtRecord, filterEvent, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXPrintingEventProcInfo, (evtRecord), (filterEvent))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXPrintingEventProc(userRoutine)                     NewGXPrintingEventUPP(userRoutine)
#define CallGXPrintingEventProc(userRoutine, evtRecord, filterEvent) InvokeGXPrintingEventUPP(evtRecord, filterEvent, userRoutine)
#define Send_GXPrintingEvent(evtRecord, filterEvent) \
          MacSendMessage(0x00000004, evtRecord, filterEvent)
 
#define Forward_GXPrintingEvent(evtRecord, filterEvent) \
          ForwardThisMessage((void *) (evtRecord), (void *) (filterEvent))

typedef CALLBACK_API_C( OSErr , GXJobDefaultFormatDialogProcPtr )(gxDialogResult *dlgResult);
typedef STACK_UPP_TYPE(GXJobDefaultFormatDialogProcPtr)         GXJobDefaultFormatDialogUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXJobDefaultFormatDialogUPP)
    NewGXJobDefaultFormatDialogUPP    (GXJobDefaultFormatDialogProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXJobDefaultFormatDialogUPP    (GXJobDefaultFormatDialogUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXJobDefaultFormatDialogUPP    (gxDialogResult *  dlgResult,
                                    GXJobDefaultFormatDialogUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXJobDefaultFormatDialogProcInfo = 0x000000E1 };      /* 2_bytes Func(4_bytes) */
    #define NewGXJobDefaultFormatDialogUPP(userRoutine)             (GXJobDefaultFormatDialogUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXJobDefaultFormatDialogProcInfo, GetCurrentArchitecture())
    #define DisposeGXJobDefaultFormatDialogUPP(userUPP)             DisposeRoutineDescriptor(userUPP)
    #define InvokeGXJobDefaultFormatDialogUPP(dlgResult, userUPP)   (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXJobDefaultFormatDialogProcInfo, (dlgResult))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXJobDefaultFormatDialogProc(userRoutine)            NewGXJobDefaultFormatDialogUPP(userRoutine)
#define CallGXJobDefaultFormatDialogProc(userRoutine, dlgResult) InvokeGXJobDefaultFormatDialogUPP(dlgResult, userRoutine)
#define Send_GXJobDefaultFormatDialog(dlgResult) \
          MacSendMessage(0x00000005, dlgResult)
 
#define Forward_GXJobDefaultFormatDialog(dlgResult) \
          ForwardThisMessage((void *) (dlgResult))

typedef CALLBACK_API_C( OSErr , GXFormatDialogProcPtr )(gxFormat theFormat, StringPtr title, gxDialogResult *dlgResult);
typedef STACK_UPP_TYPE(GXFormatDialogProcPtr)                   GXFormatDialogUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFormatDialogUPP)
    NewGXFormatDialogUPP           (GXFormatDialogProcPtr   userRoutine);

    EXTERN_API(void)
    DisposeGXFormatDialogUPP       (GXFormatDialogUPP       userUPP);

    EXTERN_API(OSErr)
    InvokeGXFormatDialogUPP        (gxFormat                theFormat,
                                    StringPtr               title,
                                    gxDialogResult *        dlgResult,
                                    GXFormatDialogUPP       userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFormatDialogProcInfo = 0x00000FE1 };                /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXFormatDialogUPP(userRoutine)                       (GXFormatDialogUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFormatDialogProcInfo, GetCurrentArchitecture())
    #define DisposeGXFormatDialogUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFormatDialogUPP(theFormat, title, dlgResult, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXFormatDialogProcInfo, (theFormat), (title), (dlgResult))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFormatDialogProc(userRoutine)                      NewGXFormatDialogUPP(userRoutine)
#define CallGXFormatDialogProc(userRoutine, theFormat, title, dlgResult) InvokeGXFormatDialogUPP(theFormat, title, dlgResult, userRoutine)
#define Send_GXFormatDialog(theFormat, title, dlgResult) \
          MacSendMessage(0x00000006, theFormat, title, dlgResult)
 
#define Forward_GXFormatDialog(theFormat, title, dlgResult) \
          ForwardThisMessage((void *) (theFormat),(void *) (title),(void *) (dlgResult))

typedef CALLBACK_API_C( OSErr , GXJobPrintDialogProcPtr )(gxDialogResult *dlgResult);
typedef STACK_UPP_TYPE(GXJobPrintDialogProcPtr)                 GXJobPrintDialogUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXJobPrintDialogUPP)
    NewGXJobPrintDialogUPP         (GXJobPrintDialogProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXJobPrintDialogUPP     (GXJobPrintDialogUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeGXJobPrintDialogUPP      (gxDialogResult *        dlgResult,
                                    GXJobPrintDialogUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXJobPrintDialogProcInfo = 0x000000E1 };              /* 2_bytes Func(4_bytes) */
    #define NewGXJobPrintDialogUPP(userRoutine)                     (GXJobPrintDialogUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXJobPrintDialogProcInfo, GetCurrentArchitecture())
    #define DisposeGXJobPrintDialogUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeGXJobPrintDialogUPP(dlgResult, userUPP)           (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXJobPrintDialogProcInfo, (dlgResult))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXJobPrintDialogProc(userRoutine)                    NewGXJobPrintDialogUPP(userRoutine)
#define CallGXJobPrintDialogProc(userRoutine, dlgResult)        InvokeGXJobPrintDialogUPP(dlgResult, userRoutine)
#define Send_GXJobPrintDialog(dlgResult) \
          MacSendMessage(0x00000007, dlgResult)
 
#define Forward_GXJobPrintDialog(dlgResult) \
          ForwardThisMessage((void *) (dlgResult))

typedef CALLBACK_API_C( OSErr , GXFilterPanelEventProcPtr )(gxPanelInfoRecord *pHitInfo, Boolean *returnImmed);
typedef STACK_UPP_TYPE(GXFilterPanelEventProcPtr)               GXFilterPanelEventUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFilterPanelEventUPP)
    NewGXFilterPanelEventUPP       (GXFilterPanelEventProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXFilterPanelEventUPP    (GXFilterPanelEventUPP  userUPP);

    EXTERN_API(OSErr)
    InvokeGXFilterPanelEventUPP    (gxPanelInfoRecord *     pHitInfo,
                                    Boolean *               returnImmed,
                                    GXFilterPanelEventUPP   userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFilterPanelEventProcInfo = 0x000003E1 };            /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXFilterPanelEventUPP(userRoutine)                   (GXFilterPanelEventUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFilterPanelEventProcInfo, GetCurrentArchitecture())
    #define DisposeGXFilterPanelEventUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFilterPanelEventUPP(pHitInfo, returnImmed, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXFilterPanelEventProcInfo, (pHitInfo), (returnImmed))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFilterPanelEventProc(userRoutine)                  NewGXFilterPanelEventUPP(userRoutine)
#define CallGXFilterPanelEventProc(userRoutine, pHitInfo, returnImmed) InvokeGXFilterPanelEventUPP(pHitInfo, returnImmed, userRoutine)
#define Send_GXFilterPanelEvent(pHitInfo, returnImmed) \
          MacSendMessage(0x00000008, pHitInfo, returnImmed)

typedef CALLBACK_API_C( OSErr , GXHandlePanelEventProcPtr )(gxPanelInfoRecord *pHitInfo, gxPanelResult *panelResponse);
typedef STACK_UPP_TYPE(GXHandlePanelEventProcPtr)               GXHandlePanelEventUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXHandlePanelEventUPP)
    NewGXHandlePanelEventUPP       (GXHandlePanelEventProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXHandlePanelEventUPP    (GXHandlePanelEventUPP  userUPP);

    EXTERN_API(OSErr)
    InvokeGXHandlePanelEventUPP    (gxPanelInfoRecord *     pHitInfo,
                                    gxPanelResult *         panelResponse,
                                    GXHandlePanelEventUPP   userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXHandlePanelEventProcInfo = 0x000003E1 };            /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXHandlePanelEventUPP(userRoutine)                   (GXHandlePanelEventUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXHandlePanelEventProcInfo, GetCurrentArchitecture())
    #define DisposeGXHandlePanelEventUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define InvokeGXHandlePanelEventUPP(pHitInfo, panelResponse, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXHandlePanelEventProcInfo, (pHitInfo), (panelResponse))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXHandlePanelEventProc(userRoutine)                  NewGXHandlePanelEventUPP(userRoutine)
#define CallGXHandlePanelEventProc(userRoutine, pHitInfo, panelResponse) InvokeGXHandlePanelEventUPP(pHitInfo, panelResponse, userRoutine)
#define Send_GXHandlePanelEvent(pHitInfo, panelResponse) \
          MacSendMessage(0x00000009, pHitInfo, panelResponse)

typedef CALLBACK_API_C( OSErr , GXParsePageRangeProcPtr )(StringPtr fromString, StringPtr toString, gxParsePageRangeResult *result);
typedef STACK_UPP_TYPE(GXParsePageRangeProcPtr)                 GXParsePageRangeUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXParsePageRangeUPP)
    NewGXParsePageRangeUPP         (GXParsePageRangeProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXParsePageRangeUPP     (GXParsePageRangeUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeGXParsePageRangeUPP      (StringPtr               fromString,
                                    StringPtr               toString,
                                    gxParsePageRangeResult * result,
                                    GXParsePageRangeUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXParsePageRangeProcInfo = 0x00000FE1 };              /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXParsePageRangeUPP(userRoutine)                     (GXParsePageRangeUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXParsePageRangeProcInfo, GetCurrentArchitecture())
    #define DisposeGXParsePageRangeUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeGXParsePageRangeUPP(fromString, toString, result, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXParsePageRangeProcInfo, (fromString), (toString), (result))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXParsePageRangeProc(userRoutine)                    NewGXParsePageRangeUPP(userRoutine)
#define CallGXParsePageRangeProc(userRoutine, fromString, toString, result) InvokeGXParsePageRangeUPP(fromString, toString, result, userRoutine)
#define Send_GXParsePageRange(fromString, toString, result) \
          MacSendMessage(0x0000000A, fromString, toString, result)
 
#define Forward_GXParsePageRange(fromString, toString, result) \
          ForwardThisMessage((void *) (fromString), (void *) (toString), (void *) (result))

typedef CALLBACK_API_C( OSErr , GXDefaultJobProcPtr )(void );
typedef STACK_UPP_TYPE(GXDefaultJobProcPtr)                     GXDefaultJobUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDefaultJobUPP)
    NewGXDefaultJobUPP             (GXDefaultJobProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeGXDefaultJobUPP         (GXDefaultJobUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeGXDefaultJobUPP          (GXDefaultJobUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDefaultJobProcInfo = 0x00000021 };                  /* 2_bytes Func() */
    #define NewGXDefaultJobUPP(userRoutine)                         (GXDefaultJobUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDefaultJobProcInfo, GetCurrentArchitecture())
    #define DisposeGXDefaultJobUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDefaultJobUPP(userUPP)                          (OSErr)CALL_ZERO_PARAMETER_UPP((userUPP), uppGXDefaultJobProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDefaultJobProc(userRoutine)                        NewGXDefaultJobUPP(userRoutine)
#define CallGXDefaultJobProc(userRoutine)                       InvokeGXDefaultJobUPP(userRoutine)
#define Send_GXDefaultJob() MacSendMessage(0x0000000B)
#define Forward_GXDefaultJob() ForwardThisMessage((void *) (0))

typedef CALLBACK_API_C( OSErr , GXDefaultFormatProcPtr )(gxFormat theFormat);
typedef STACK_UPP_TYPE(GXDefaultFormatProcPtr)                  GXDefaultFormatUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDefaultFormatUPP)
    NewGXDefaultFormatUPP          (GXDefaultFormatProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeGXDefaultFormatUPP      (GXDefaultFormatUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeGXDefaultFormatUPP       (gxFormat                theFormat,
                                    GXDefaultFormatUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDefaultFormatProcInfo = 0x000000E1 };               /* 2_bytes Func(4_bytes) */
    #define NewGXDefaultFormatUPP(userRoutine)                      (GXDefaultFormatUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDefaultFormatProcInfo, GetCurrentArchitecture())
    #define DisposeGXDefaultFormatUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDefaultFormatUPP(theFormat, userUPP)            (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXDefaultFormatProcInfo, (theFormat))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDefaultFormatProc(userRoutine)                     NewGXDefaultFormatUPP(userRoutine)
#define CallGXDefaultFormatProc(userRoutine, theFormat)         InvokeGXDefaultFormatUPP(theFormat, userRoutine)
#define Send_GXDefaultFormat(theFormat) \
          MacSendMessage(0x0000000C, theFormat)
 
#define Forward_GXDefaultFormat(theFormat) \
          ForwardThisMessage((void *) (theFormat))

typedef CALLBACK_API_C( OSErr , GXDefaultPaperTypeProcPtr )(gxPaperType thePaperType);
typedef STACK_UPP_TYPE(GXDefaultPaperTypeProcPtr)               GXDefaultPaperTypeUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDefaultPaperTypeUPP)
    NewGXDefaultPaperTypeUPP       (GXDefaultPaperTypeProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXDefaultPaperTypeUPP    (GXDefaultPaperTypeUPP  userUPP);

    EXTERN_API(OSErr)
    InvokeGXDefaultPaperTypeUPP    (gxPaperType             thePaperType,
                                    GXDefaultPaperTypeUPP   userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDefaultPaperTypeProcInfo = 0x000000E1 };            /* 2_bytes Func(4_bytes) */
    #define NewGXDefaultPaperTypeUPP(userRoutine)                   (GXDefaultPaperTypeUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDefaultPaperTypeProcInfo, GetCurrentArchitecture())
    #define DisposeGXDefaultPaperTypeUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDefaultPaperTypeUPP(thePaperType, userUPP)      (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXDefaultPaperTypeProcInfo, (thePaperType))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDefaultPaperTypeProc(userRoutine)                  NewGXDefaultPaperTypeUPP(userRoutine)
#define CallGXDefaultPaperTypeProc(userRoutine, thePaperType)   InvokeGXDefaultPaperTypeUPP(thePaperType, userRoutine)
#define Send_GXDefaultPaperType(thePaperType) \
          MacSendMessage(0x0000000D, thePaperType)
 
#define Forward_GXDefaultPaperType(thePaperType) \
          ForwardThisMessage((void *) thePaperType)

typedef CALLBACK_API_C( OSErr , GXDefaultPrinterProcPtr )(gxPrinter thePrinter);
typedef STACK_UPP_TYPE(GXDefaultPrinterProcPtr)                 GXDefaultPrinterUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDefaultPrinterUPP)
    NewGXDefaultPrinterUPP         (GXDefaultPrinterProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXDefaultPrinterUPP     (GXDefaultPrinterUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeGXDefaultPrinterUPP      (gxPrinter               thePrinter,
                                    GXDefaultPrinterUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDefaultPrinterProcInfo = 0x000000E1 };              /* 2_bytes Func(4_bytes) */
    #define NewGXDefaultPrinterUPP(userRoutine)                     (GXDefaultPrinterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDefaultPrinterProcInfo, GetCurrentArchitecture())
    #define DisposeGXDefaultPrinterUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDefaultPrinterUPP(thePrinter, userUPP)          (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXDefaultPrinterProcInfo, (thePrinter))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDefaultPrinterProc(userRoutine)                    NewGXDefaultPrinterUPP(userRoutine)
#define CallGXDefaultPrinterProc(userRoutine, thePrinter)       InvokeGXDefaultPrinterUPP(thePrinter, userRoutine)
#define Send_GXDefaultPrinter(thePrinter) \
          MacSendMessage(0x0000000E, thePrinter)
 
#define Forward_GXDefaultPrinter(thePrinter) \
          ForwardThisMessage((void *) thePrinter)

typedef CALLBACK_API_C( OSErr , GXCreateSpoolFileProcPtr )(FSSpecPtr pFileSpec, long createOptions, gxSpoolFile *theSpoolFile);
typedef STACK_UPP_TYPE(GXCreateSpoolFileProcPtr)                GXCreateSpoolFileUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCreateSpoolFileUPP)
    NewGXCreateSpoolFileUPP        (GXCreateSpoolFileProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCreateSpoolFileUPP    (GXCreateSpoolFileUPP    userUPP);

    EXTERN_API(OSErr)
    InvokeGXCreateSpoolFileUPP     (FSSpecPtr               pFileSpec,
                                    long                    createOptions,
                                    gxSpoolFile *           theSpoolFile,
                                    GXCreateSpoolFileUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCreateSpoolFileProcInfo = 0x00000FE1 };             /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXCreateSpoolFileUPP(userRoutine)                    (GXCreateSpoolFileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCreateSpoolFileProcInfo, GetCurrentArchitecture())
    #define DisposeGXCreateSpoolFileUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCreateSpoolFileUPP(pFileSpec, createOptions, theSpoolFile, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXCreateSpoolFileProcInfo, (pFileSpec), (createOptions), (theSpoolFile))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCreateSpoolFileProc(userRoutine)                   NewGXCreateSpoolFileUPP(userRoutine)
#define CallGXCreateSpoolFileProc(userRoutine, pFileSpec, createOptions, theSpoolFile) InvokeGXCreateSpoolFileUPP(pFileSpec, createOptions, theSpoolFile, userRoutine)
#define Send_GXCreateSpoolFile(pFileSpec, createOptions, theSpoolFile) \
          MacSendMessage(0x0000000F, pFileSpec, createOptions, \
                          theSpoolFile)
 
#define Forward_GXCreateSpoolFile(pFileSpec, createOptions, theSpoolFile) \
          ForwardThisMessage((void *) pFileSpec, (void *) (createOptions), (void *) theSpoolFile)

typedef CALLBACK_API_C( OSErr , GXSpoolPageProcPtr )(gxSpoolFile theSpoolFile, gxFormat theFormat, gxShape thePage);
typedef STACK_UPP_TYPE(GXSpoolPageProcPtr)                      GXSpoolPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXSpoolPageUPP)
    NewGXSpoolPageUPP              (GXSpoolPageProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeGXSpoolPageUPP          (GXSpoolPageUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeGXSpoolPageUPP           (gxSpoolFile             theSpoolFile,
                                    gxFormat                theFormat,
                                    gxShape                 thePage,
                                    GXSpoolPageUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXSpoolPageProcInfo = 0x00000FE1 };                   /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXSpoolPageUPP(userRoutine)                          (GXSpoolPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXSpoolPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXSpoolPageUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeGXSpoolPageUPP(theSpoolFile, theFormat, thePage, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXSpoolPageProcInfo, (theSpoolFile), (theFormat), (thePage))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXSpoolPageProc(userRoutine)                         NewGXSpoolPageUPP(userRoutine)
#define CallGXSpoolPageProc(userRoutine, theSpoolFile, theFormat, thePage) InvokeGXSpoolPageUPP(theSpoolFile, theFormat, thePage, userRoutine)
#define Send_GXSpoolPage(theSpoolFile, theFormat, thePage) \
          MacSendMessage(0x00000010, theSpoolFile, theFormat, thePage)
 
#define Forward_GXSpoolPage(theSpoolFile, theFormat, thePage) \
          ForwardThisMessage((void *) theSpoolFile, (void *) theFormat, (void *) thePage)

typedef CALLBACK_API_C( OSErr , GXSpoolDataProcPtr )(gxSpoolFile theSpoolFile, Ptr data, long *length);
typedef STACK_UPP_TYPE(GXSpoolDataProcPtr)                      GXSpoolDataUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXSpoolDataUPP)
    NewGXSpoolDataUPP              (GXSpoolDataProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeGXSpoolDataUPP          (GXSpoolDataUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeGXSpoolDataUPP           (gxSpoolFile             theSpoolFile,
                                    Ptr                     data,
                                    long *                  length,
                                    GXSpoolDataUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXSpoolDataProcInfo = 0x00000FE1 };                   /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXSpoolDataUPP(userRoutine)                          (GXSpoolDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXSpoolDataProcInfo, GetCurrentArchitecture())
    #define DisposeGXSpoolDataUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeGXSpoolDataUPP(theSpoolFile, data, length, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXSpoolDataProcInfo, (theSpoolFile), (data), (length))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXSpoolDataProc(userRoutine)                         NewGXSpoolDataUPP(userRoutine)
#define CallGXSpoolDataProc(userRoutine, theSpoolFile, data, length) InvokeGXSpoolDataUPP(theSpoolFile, data, length, userRoutine)
#define Send_GXSpoolData(theSpoolFile, data, length) \
          MacSendMessage(0x00000011, theSpoolFile, data, length)
 
#define Forward_GXSpoolData(theSpoolFile, data, length) \
          ForwardThisMessage((void *) theSpoolFile, (void *) data, (void *) length)

typedef CALLBACK_API_C( OSErr , GXSpoolResourceProcPtr )(gxSpoolFile theSpoolFile, Handle theResource, ResType theType, long id);
typedef STACK_UPP_TYPE(GXSpoolResourceProcPtr)                  GXSpoolResourceUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXSpoolResourceUPP)
    NewGXSpoolResourceUPP          (GXSpoolResourceProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeGXSpoolResourceUPP      (GXSpoolResourceUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeGXSpoolResourceUPP       (gxSpoolFile             theSpoolFile,
                                    Handle                  theResource,
                                    ResType                 theType,
                                    long                    id,
                                    GXSpoolResourceUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXSpoolResourceProcInfo = 0x00003FE1 };               /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXSpoolResourceUPP(userRoutine)                      (GXSpoolResourceUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXSpoolResourceProcInfo, GetCurrentArchitecture())
    #define DisposeGXSpoolResourceUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeGXSpoolResourceUPP(theSpoolFile, theResource, theType, id, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppGXSpoolResourceProcInfo, (theSpoolFile), (theResource), (theType), (id))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXSpoolResourceProc(userRoutine)                     NewGXSpoolResourceUPP(userRoutine)
#define CallGXSpoolResourceProc(userRoutine, theSpoolFile, theResource, theType, id) InvokeGXSpoolResourceUPP(theSpoolFile, theResource, theType, id, userRoutine)
#define Send_GXSpoolResource(theSpoolFile, theResource, theType, id) \
          MacSendMessage(0x00000012, theSpoolFile, theResource, \
                          theType, id)
 
#define Forward_GXSpoolResource(theSpoolFile, theResource, theType, id) \
          ForwardThisMessage((void *) theSpoolFile, (void *) theResource, \
                                    (void *) theType, (void *) (id))

typedef CALLBACK_API_C( OSErr , GXCompleteSpoolFileProcPtr )(gxSpoolFile theSpoolFile);
typedef STACK_UPP_TYPE(GXCompleteSpoolFileProcPtr)              GXCompleteSpoolFileUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCompleteSpoolFileUPP)
    NewGXCompleteSpoolFileUPP      (GXCompleteSpoolFileProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCompleteSpoolFileUPP    (GXCompleteSpoolFileUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXCompleteSpoolFileUPP    (gxSpoolFile            theSpoolFile,
                                    GXCompleteSpoolFileUPP  userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCompleteSpoolFileProcInfo = 0x000000E1 };           /* 2_bytes Func(4_bytes) */
    #define NewGXCompleteSpoolFileUPP(userRoutine)                  (GXCompleteSpoolFileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCompleteSpoolFileProcInfo, GetCurrentArchitecture())
    #define DisposeGXCompleteSpoolFileUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCompleteSpoolFileUPP(theSpoolFile, userUPP)     (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXCompleteSpoolFileProcInfo, (theSpoolFile))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCompleteSpoolFileProc(userRoutine)                 NewGXCompleteSpoolFileUPP(userRoutine)
#define CallGXCompleteSpoolFileProc(userRoutine, theSpoolFile)  InvokeGXCompleteSpoolFileUPP(theSpoolFile, userRoutine)
#define Send_GXCompleteSpoolFile(theSpoolFile) \
          MacSendMessage(0x00000013, theSpoolFile)
 
#define Forward_GXCompleteSpoolFile(theSpoolFile) \
          ForwardThisMessage((void *) (theSpoolFile))

typedef CALLBACK_API_C( OSErr , GXCountPagesProcPtr )(gxSpoolFile theSpoolFile, long *numPages);
typedef STACK_UPP_TYPE(GXCountPagesProcPtr)                     GXCountPagesUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCountPagesUPP)
    NewGXCountPagesUPP             (GXCountPagesProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeGXCountPagesUPP         (GXCountPagesUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeGXCountPagesUPP          (gxSpoolFile             theSpoolFile,
                                    long *                  numPages,
                                    GXCountPagesUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCountPagesProcInfo = 0x000003E1 };                  /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXCountPagesUPP(userRoutine)                         (GXCountPagesUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCountPagesProcInfo, GetCurrentArchitecture())
    #define DisposeGXCountPagesUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCountPagesUPP(theSpoolFile, numPages, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXCountPagesProcInfo, (theSpoolFile), (numPages))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCountPagesProc(userRoutine)                        NewGXCountPagesUPP(userRoutine)
#define CallGXCountPagesProc(userRoutine, theSpoolFile, numPages) InvokeGXCountPagesUPP(theSpoolFile, numPages, userRoutine)
#define Send_GXCountPages(theSpoolFile, numPages) \
          MacSendMessage(0x00000014, theSpoolFile, numPages)
 
#define Forward_GXCountPages(theSpoolFile, numPages) \
          ForwardThisMessage((void *) (theSpoolFile), (void *) (numPages))

typedef CALLBACK_API_C( OSErr , GXDespoolPageProcPtr )(gxSpoolFile theSpoolFile, long numPages, gxFormat theFormat, gxShape *thePage, Boolean *formatChanged);
typedef STACK_UPP_TYPE(GXDespoolPageProcPtr)                    GXDespoolPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDespoolPageUPP)
    NewGXDespoolPageUPP            (GXDespoolPageProcPtr    userRoutine);

    EXTERN_API(void)
    DisposeGXDespoolPageUPP        (GXDespoolPageUPP        userUPP);

    EXTERN_API(OSErr)
    InvokeGXDespoolPageUPP         (gxSpoolFile             theSpoolFile,
                                    long                    numPages,
                                    gxFormat                theFormat,
                                    gxShape *               thePage,
                                    Boolean *               formatChanged,
                                    GXDespoolPageUPP        userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDespoolPageProcInfo = 0x0000FFE1 };                 /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXDespoolPageUPP(userRoutine)                        (GXDespoolPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDespoolPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXDespoolPageUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDespoolPageUPP(theSpoolFile, numPages, theFormat, thePage, formatChanged, userUPP)  (OSErr)CALL_FIVE_PARAMETER_UPP((userUPP), uppGXDespoolPageProcInfo, (theSpoolFile), (numPages), (theFormat), (thePage), (formatChanged))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDespoolPageProc(userRoutine)                       NewGXDespoolPageUPP(userRoutine)
#define CallGXDespoolPageProc(userRoutine, theSpoolFile, numPages, theFormat, thePage, formatChanged) InvokeGXDespoolPageUPP(theSpoolFile, numPages, theFormat, thePage, formatChanged, userRoutine)
#define Send_GXDespoolPage(theSpoolFile, numPages, theFormat, thePage, formatChanged) \
          MacSendMessage(0x00000015, theSpoolFile, numPages, \
                          theFormat, thePage, formatChanged)
 
#define Forward_GXDespoolPage(theSpoolFile, numPages, theFormat, thePage, formatChanged) \
          ForwardThisMessage((void *) (theSpoolFile), (void *) (numPages), (void *) (theFormat), \
                                    (void *) (thePage), (void *) (formatChanged))

typedef CALLBACK_API_C( OSErr , GXDespoolDataProcPtr )(gxSpoolFile theSpoolFile, Ptr data, long *length);
typedef STACK_UPP_TYPE(GXDespoolDataProcPtr)                    GXDespoolDataUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDespoolDataUPP)
    NewGXDespoolDataUPP            (GXDespoolDataProcPtr    userRoutine);

    EXTERN_API(void)
    DisposeGXDespoolDataUPP        (GXDespoolDataUPP        userUPP);

    EXTERN_API(OSErr)
    InvokeGXDespoolDataUPP         (gxSpoolFile             theSpoolFile,
                                    Ptr                     data,
                                    long *                  length,
                                    GXDespoolDataUPP        userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDespoolDataProcInfo = 0x00000FE1 };                 /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXDespoolDataUPP(userRoutine)                        (GXDespoolDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDespoolDataProcInfo, GetCurrentArchitecture())
    #define DisposeGXDespoolDataUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDespoolDataUPP(theSpoolFile, data, length, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXDespoolDataProcInfo, (theSpoolFile), (data), (length))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDespoolDataProc(userRoutine)                       NewGXDespoolDataUPP(userRoutine)
#define CallGXDespoolDataProc(userRoutine, theSpoolFile, data, length) InvokeGXDespoolDataUPP(theSpoolFile, data, length, userRoutine)
#define Send_GXDespoolData(theSpoolFile, data, length) \
          MacSendMessage(0x00000016, theSpoolFile, data, length)
 
#define Forward_GXDespoolData(theSpoolFile, data, length) \
          ForwardThisMessage((void *) (theSpoolFile), (void *) (data), (void *) (length))

typedef CALLBACK_API_C( OSErr , GXDespoolResourceProcPtr )(gxSpoolFile theSpoolFile, ResType theType, long id, Handle *theResource);
typedef STACK_UPP_TYPE(GXDespoolResourceProcPtr)                GXDespoolResourceUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDespoolResourceUPP)
    NewGXDespoolResourceUPP        (GXDespoolResourceProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXDespoolResourceUPP    (GXDespoolResourceUPP    userUPP);

    EXTERN_API(OSErr)
    InvokeGXDespoolResourceUPP     (gxSpoolFile             theSpoolFile,
                                    ResType                 theType,
                                    long                    id,
                                    Handle *                theResource,
                                    GXDespoolResourceUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDespoolResourceProcInfo = 0x00003FE1 };             /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXDespoolResourceUPP(userRoutine)                    (GXDespoolResourceUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDespoolResourceProcInfo, GetCurrentArchitecture())
    #define DisposeGXDespoolResourceUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDespoolResourceUPP(theSpoolFile, theType, id, theResource, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppGXDespoolResourceProcInfo, (theSpoolFile), (theType), (id), (theResource))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDespoolResourceProc(userRoutine)                   NewGXDespoolResourceUPP(userRoutine)
#define CallGXDespoolResourceProc(userRoutine, theSpoolFile, theType, id, theResource) InvokeGXDespoolResourceUPP(theSpoolFile, theType, id, theResource, userRoutine)
#define Send_GXDespoolResource(theSpoolFile, theType, id, theResource) \
          MacSendMessage(0x00000017, theSpoolFile, theType, \
                          id, theResource)
 
#define Forward_GXDespoolResource(theSpoolFile, theType, id, theResource) \
          ForwardThisMessage((void *) (theSpoolFile), (void *) (theType), (void *) (id), \
                                    (void *) (theResource))

typedef CALLBACK_API_C( OSErr , GXCloseSpoolFileProcPtr )(gxSpoolFile theSpoolFile, long closeOptions);
typedef STACK_UPP_TYPE(GXCloseSpoolFileProcPtr)                 GXCloseSpoolFileUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCloseSpoolFileUPP)
    NewGXCloseSpoolFileUPP         (GXCloseSpoolFileProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCloseSpoolFileUPP     (GXCloseSpoolFileUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeGXCloseSpoolFileUPP      (gxSpoolFile             theSpoolFile,
                                    long                    closeOptions,
                                    GXCloseSpoolFileUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCloseSpoolFileProcInfo = 0x000003E1 };              /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXCloseSpoolFileUPP(userRoutine)                     (GXCloseSpoolFileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCloseSpoolFileProcInfo, GetCurrentArchitecture())
    #define DisposeGXCloseSpoolFileUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCloseSpoolFileUPP(theSpoolFile, closeOptions, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXCloseSpoolFileProcInfo, (theSpoolFile), (closeOptions))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCloseSpoolFileProc(userRoutine)                    NewGXCloseSpoolFileUPP(userRoutine)
#define CallGXCloseSpoolFileProc(userRoutine, theSpoolFile, closeOptions) InvokeGXCloseSpoolFileUPP(theSpoolFile, closeOptions, userRoutine)
#define Send_GXCloseSpoolFile(theSpoolFile, closeOptions) \
          MacSendMessage(0x00000018, theSpoolFile, closeOptions)
 
#define Forward_GXCloseSpoolFile(theSpoolFile, closeOptions) \
          ForwardThisMessage((void *) (theSpoolFile), (void *) (closeOptions))

typedef CALLBACK_API_C( OSErr , GXStartJobProcPtr )(StringPtr docName, long pageCount);
typedef STACK_UPP_TYPE(GXStartJobProcPtr)                       GXStartJobUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXStartJobUPP)
    NewGXStartJobUPP               (GXStartJobProcPtr       userRoutine);

    EXTERN_API(void)
    DisposeGXStartJobUPP           (GXStartJobUPP           userUPP);

    EXTERN_API(OSErr)
    InvokeGXStartJobUPP            (StringPtr               docName,
                                    long                    pageCount,
                                    GXStartJobUPP           userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXStartJobProcInfo = 0x000003E1 };                    /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXStartJobUPP(userRoutine)                           (GXStartJobUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXStartJobProcInfo, GetCurrentArchitecture())
    #define DisposeGXStartJobUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define InvokeGXStartJobUPP(docName, pageCount, userUPP)        (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXStartJobProcInfo, (docName), (pageCount))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXStartJobProc(userRoutine)                          NewGXStartJobUPP(userRoutine)
#define CallGXStartJobProc(userRoutine, docName, pageCount)     InvokeGXStartJobUPP(docName, pageCount, userRoutine)
#define Send_GXStartJob(docName, pageCount) \
          MacSendMessage(0x00000019, docName, pageCount)
 
#define Forward_GXStartJob(docName, pageCount) \
          ForwardThisMessage((void *) (docName), (void *) (pageCount))

typedef CALLBACK_API_C( OSErr , GXFinishJobProcPtr )(void );
typedef STACK_UPP_TYPE(GXFinishJobProcPtr)                      GXFinishJobUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFinishJobUPP)
    NewGXFinishJobUPP              (GXFinishJobProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeGXFinishJobUPP          (GXFinishJobUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeGXFinishJobUPP           (GXFinishJobUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFinishJobProcInfo = 0x00000021 };                   /* 2_bytes Func() */
    #define NewGXFinishJobUPP(userRoutine)                          (GXFinishJobUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFinishJobProcInfo, GetCurrentArchitecture())
    #define DisposeGXFinishJobUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFinishJobUPP(userUPP)                           (OSErr)CALL_ZERO_PARAMETER_UPP((userUPP), uppGXFinishJobProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFinishJobProc(userRoutine)                         NewGXFinishJobUPP(userRoutine)
#define CallGXFinishJobProc(userRoutine)                        InvokeGXFinishJobUPP(userRoutine)
#define Send_GXFinishJob() MacSendMessage(0x0000001A)
#define Forward_GXFinishJob() ForwardThisMessage((void *) (0))

typedef CALLBACK_API_C( OSErr , GXStartPageProcPtr )(gxFormat theFormat, long numViewPorts, gxViewPort *viewPortList);
typedef STACK_UPP_TYPE(GXStartPageProcPtr)                      GXStartPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXStartPageUPP)
    NewGXStartPageUPP              (GXStartPageProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeGXStartPageUPP          (GXStartPageUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeGXStartPageUPP           (gxFormat                theFormat,
                                    long                    numViewPorts,
                                    gxViewPort *            viewPortList,
                                    GXStartPageUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXStartPageProcInfo = 0x00000FE1 };                   /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXStartPageUPP(userRoutine)                          (GXStartPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXStartPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXStartPageUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeGXStartPageUPP(theFormat, numViewPorts, viewPortList, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXStartPageProcInfo, (theFormat), (numViewPorts), (viewPortList))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXStartPageProc(userRoutine)                         NewGXStartPageUPP(userRoutine)
#define CallGXStartPageProc(userRoutine, theFormat, numViewPorts, viewPortList) InvokeGXStartPageUPP(theFormat, numViewPorts, viewPortList, userRoutine)
#define Send_GXStartPage(theFormat, numViewPorts, viewPortList) \
          MacSendMessage(0x0000001B, theFormat, numViewPorts, viewPortList)
 
#define Forward_GXStartPage(theFormat, numViewPorts, viewPortList) \
          ForwardThisMessage((void *) (theFormat), (void *) (numViewPorts), (void *) (viewPortList))

typedef CALLBACK_API_C( OSErr , GXFinishPageProcPtr )(void );
typedef STACK_UPP_TYPE(GXFinishPageProcPtr)                     GXFinishPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFinishPageUPP)
    NewGXFinishPageUPP             (GXFinishPageProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeGXFinishPageUPP         (GXFinishPageUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeGXFinishPageUPP          (GXFinishPageUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFinishPageProcInfo = 0x00000021 };                  /* 2_bytes Func() */
    #define NewGXFinishPageUPP(userRoutine)                         (GXFinishPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFinishPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXFinishPageUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFinishPageUPP(userUPP)                          (OSErr)CALL_ZERO_PARAMETER_UPP((userUPP), uppGXFinishPageProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFinishPageProc(userRoutine)                        NewGXFinishPageUPP(userRoutine)
#define CallGXFinishPageProc(userRoutine)                       InvokeGXFinishPageUPP(userRoutine)
#define Send_GXFinishPage() MacSendMessage(0x0000001C)
#define Forward_GXFinishPage() ForwardThisMessage((void *) (0))

typedef CALLBACK_API_C( OSErr , GXPrintPageProcPtr )(gxFormat theFormat, gxShape thePage);
typedef STACK_UPP_TYPE(GXPrintPageProcPtr)                      GXPrintPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXPrintPageUPP)
    NewGXPrintPageUPP              (GXPrintPageProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeGXPrintPageUPP          (GXPrintPageUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeGXPrintPageUPP           (gxFormat                theFormat,
                                    gxShape                 thePage,
                                    GXPrintPageUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXPrintPageProcInfo = 0x000003E1 };                   /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXPrintPageUPP(userRoutine)                          (GXPrintPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXPrintPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXPrintPageUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeGXPrintPageUPP(theFormat, thePage, userUPP)       (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXPrintPageProcInfo, (theFormat), (thePage))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXPrintPageProc(userRoutine)                         NewGXPrintPageUPP(userRoutine)
#define CallGXPrintPageProc(userRoutine, theFormat, thePage)    InvokeGXPrintPageUPP(theFormat, thePage, userRoutine)
#define Send_GXPrintPage(theFormat, thePage) \
          MacSendMessage(0x0000001D, theFormat, thePage)
 
#define Forward_GXPrintPage(theFormat, thePage) \
          ForwardThisMessage((void *) (theFormat), (void *) (thePage))

typedef CALLBACK_API_C( OSErr , GXSetupImageDataProcPtr )(void *imageData);
typedef STACK_UPP_TYPE(GXSetupImageDataProcPtr)                 GXSetupImageDataUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXSetupImageDataUPP)
    NewGXSetupImageDataUPP         (GXSetupImageDataProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXSetupImageDataUPP     (GXSetupImageDataUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeGXSetupImageDataUPP      (void *                  imageData,
                                    GXSetupImageDataUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXSetupImageDataProcInfo = 0x000000E1 };              /* 2_bytes Func(4_bytes) */
    #define NewGXSetupImageDataUPP(userRoutine)                     (GXSetupImageDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXSetupImageDataProcInfo, GetCurrentArchitecture())
    #define DisposeGXSetupImageDataUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeGXSetupImageDataUPP(imageData, userUPP)           (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXSetupImageDataProcInfo, (imageData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXSetupImageDataProc(userRoutine)                    NewGXSetupImageDataUPP(userRoutine)
#define CallGXSetupImageDataProc(userRoutine, imageData)        InvokeGXSetupImageDataUPP(imageData, userRoutine)
#define Send_GXSetupImageData(imageData) \
          MacSendMessage(0x0000001E, imageData)
 
#define Forward_GXSetupImageData(imageData) \
          ForwardThisMessage((void *) (imageData))

typedef CALLBACK_API_C( OSErr , GXImageJobProcPtr )(gxSpoolFile theSpoolFile, long *closeOptions);
typedef STACK_UPP_TYPE(GXImageJobProcPtr)                       GXImageJobUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXImageJobUPP)
    NewGXImageJobUPP               (GXImageJobProcPtr       userRoutine);

    EXTERN_API(void)
    DisposeGXImageJobUPP           (GXImageJobUPP           userUPP);

    EXTERN_API(OSErr)
    InvokeGXImageJobUPP            (gxSpoolFile             theSpoolFile,
                                    long *                  closeOptions,
                                    GXImageJobUPP           userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXImageJobProcInfo = 0x000003E1 };                    /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXImageJobUPP(userRoutine)                           (GXImageJobUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXImageJobProcInfo, GetCurrentArchitecture())
    #define DisposeGXImageJobUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define InvokeGXImageJobUPP(theSpoolFile, closeOptions, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXImageJobProcInfo, (theSpoolFile), (closeOptions))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXImageJobProc(userRoutine)                          NewGXImageJobUPP(userRoutine)
#define CallGXImageJobProc(userRoutine, theSpoolFile, closeOptions) InvokeGXImageJobUPP(theSpoolFile, closeOptions, userRoutine)
#define Send_GXImageJob(theSpoolFile, closeOptions) \
          MacSendMessage(0x0000001F, theSpoolFile, closeOptions)
 
#define Forward_GXImageJob(theSpoolFile, closeOptions) \
          ForwardThisMessage((void *) (theSpoolFile), (void *) (closeOptions))

typedef CALLBACK_API_C( OSErr , GXImageDocumentProcPtr )(gxSpoolFile theSpoolFile, void *imageData);
typedef STACK_UPP_TYPE(GXImageDocumentProcPtr)                  GXImageDocumentUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXImageDocumentUPP)
    NewGXImageDocumentUPP          (GXImageDocumentProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeGXImageDocumentUPP      (GXImageDocumentUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeGXImageDocumentUPP       (gxSpoolFile             theSpoolFile,
                                    void *                  imageData,
                                    GXImageDocumentUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXImageDocumentProcInfo = 0x000003E1 };               /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXImageDocumentUPP(userRoutine)                      (GXImageDocumentUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXImageDocumentProcInfo, GetCurrentArchitecture())
    #define DisposeGXImageDocumentUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeGXImageDocumentUPP(theSpoolFile, imageData, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXImageDocumentProcInfo, (theSpoolFile), (imageData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXImageDocumentProc(userRoutine)                     NewGXImageDocumentUPP(userRoutine)
#define CallGXImageDocumentProc(userRoutine, theSpoolFile, imageData) InvokeGXImageDocumentUPP(theSpoolFile, imageData, userRoutine)
#define Send_GXImageDocument(theSpoolFile, imageData) \
          MacSendMessage(0x00000020, theSpoolFile, imageData)
 
#define Forward_GXImageDocument(theSpoolFile, imageData) \
          ForwardThisMessage((void *) (theSpoolFile), (void *) (imageData))

typedef CALLBACK_API_C( OSErr , GXImagePageProcPtr )(gxSpoolFile theSpoolFile, long pageNumber, gxFormat theFormat, void *imageData);
typedef STACK_UPP_TYPE(GXImagePageProcPtr)                      GXImagePageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXImagePageUPP)
    NewGXImagePageUPP              (GXImagePageProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeGXImagePageUPP          (GXImagePageUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeGXImagePageUPP           (gxSpoolFile             theSpoolFile,
                                    long                    pageNumber,
                                    gxFormat                theFormat,
                                    void *                  imageData,
                                    GXImagePageUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXImagePageProcInfo = 0x00003FE1 };                   /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXImagePageUPP(userRoutine)                          (GXImagePageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXImagePageProcInfo, GetCurrentArchitecture())
    #define DisposeGXImagePageUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeGXImagePageUPP(theSpoolFile, pageNumber, theFormat, imageData, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppGXImagePageProcInfo, (theSpoolFile), (pageNumber), (theFormat), (imageData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXImagePageProc(userRoutine)                         NewGXImagePageUPP(userRoutine)
#define CallGXImagePageProc(userRoutine, theSpoolFile, pageNumber, theFormat, imageData) InvokeGXImagePageUPP(theSpoolFile, pageNumber, theFormat, imageData, userRoutine)
#define Send_GXImagePage(theSpoolFile, pageNumber, theFormat, imageData) \
          MacSendMessage(0x00000021, theSpoolFile, pageNumber, theFormat, imageData)
 
#define Forward_GXImagePage(theSpoolFile, pageNumber, theFormat, imageData) \
          ForwardThisMessage((void *) (theSpoolFile), (void *) (pageNumber), (void *) (theFormat), \
                                    (void *) (imageData))

typedef CALLBACK_API_C( OSErr , GXRenderPageProcPtr )(gxFormat theFormat, gxShape thePage, gxPageInfoRecord *pageInfo, void *imageData);
typedef STACK_UPP_TYPE(GXRenderPageProcPtr)                     GXRenderPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXRenderPageUPP)
    NewGXRenderPageUPP             (GXRenderPageProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeGXRenderPageUPP         (GXRenderPageUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeGXRenderPageUPP          (gxFormat                theFormat,
                                    gxShape                 thePage,
                                    gxPageInfoRecord *      pageInfo,
                                    void *                  imageData,
                                    GXRenderPageUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXRenderPageProcInfo = 0x00003FE1 };                  /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXRenderPageUPP(userRoutine)                         (GXRenderPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXRenderPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXRenderPageUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeGXRenderPageUPP(theFormat, thePage, pageInfo, imageData, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppGXRenderPageProcInfo, (theFormat), (thePage), (pageInfo), (imageData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXRenderPageProc(userRoutine)                        NewGXRenderPageUPP(userRoutine)
#define CallGXRenderPageProc(userRoutine, theFormat, thePage, pageInfo, imageData) InvokeGXRenderPageUPP(theFormat, thePage, pageInfo, imageData, userRoutine)
#define Send_GXRenderPage(theFormat, thePage, pageInfo, imageData) \
          MacSendMessage(0x00000022, theFormat, thePage, pageInfo, imageData)
 
#define Forward_GXRenderPage(theFormat, thePage, pageInfo, imageData) \
          ForwardThisMessage((void *) (theFormat), (void *) (thePage), (void *) (pageInfo), (void *) (imageData))

typedef CALLBACK_API_C( OSErr , GXCreateImageFileProcPtr )(FSSpecPtr pFileSpec, long imageFileOptions, long *theImageFile);
typedef STACK_UPP_TYPE(GXCreateImageFileProcPtr)                GXCreateImageFileUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCreateImageFileUPP)
    NewGXCreateImageFileUPP        (GXCreateImageFileProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCreateImageFileUPP    (GXCreateImageFileUPP    userUPP);

    EXTERN_API(OSErr)
    InvokeGXCreateImageFileUPP     (FSSpecPtr               pFileSpec,
                                    long                    imageFileOptions,
                                    long *                  theImageFile,
                                    GXCreateImageFileUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCreateImageFileProcInfo = 0x00000FE1 };             /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXCreateImageFileUPP(userRoutine)                    (GXCreateImageFileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCreateImageFileProcInfo, GetCurrentArchitecture())
    #define DisposeGXCreateImageFileUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCreateImageFileUPP(pFileSpec, imageFileOptions, theImageFile, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXCreateImageFileProcInfo, (pFileSpec), (imageFileOptions), (theImageFile))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCreateImageFileProc(userRoutine)                   NewGXCreateImageFileUPP(userRoutine)
#define CallGXCreateImageFileProc(userRoutine, pFileSpec, imageFileOptions, theImageFile) InvokeGXCreateImageFileUPP(pFileSpec, imageFileOptions, theImageFile, userRoutine)
#define Send_GXCreateImageFile(pFileSpec, imageFileOptions, theImageFile) \
          MacSendMessage(0x00000023, pFileSpec, imageFileOptions, theImageFile)
 
#define Forward_GXCreateImageFile(pFileSpec, imageFileOptions, theImageFile) \
          ForwardThisMessage((void *) (pFileSpec), (void *) (imageFileOptions), (void *) (theImageFile))

typedef CALLBACK_API_C( OSErr , GXOpenConnectionProcPtr )(void );
typedef STACK_UPP_TYPE(GXOpenConnectionProcPtr)                 GXOpenConnectionUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXOpenConnectionUPP)
    NewGXOpenConnectionUPP         (GXOpenConnectionProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXOpenConnectionUPP     (GXOpenConnectionUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeGXOpenConnectionUPP      (GXOpenConnectionUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXOpenConnectionProcInfo = 0x00000021 };              /* 2_bytes Func() */
    #define NewGXOpenConnectionUPP(userRoutine)                     (GXOpenConnectionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXOpenConnectionProcInfo, GetCurrentArchitecture())
    #define DisposeGXOpenConnectionUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeGXOpenConnectionUPP(userUPP)                      (OSErr)CALL_ZERO_PARAMETER_UPP((userUPP), uppGXOpenConnectionProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXOpenConnectionProc(userRoutine)                    NewGXOpenConnectionUPP(userRoutine)
#define CallGXOpenConnectionProc(userRoutine)                   InvokeGXOpenConnectionUPP(userRoutine)
#define Send_GXOpenConnection() MacSendMessage(0x00000024)
#define Forward_GXOpenConnection() ForwardThisMessage((void *) (0))

typedef CALLBACK_API_C( OSErr , GXCloseConnectionProcPtr )(void );
typedef STACK_UPP_TYPE(GXCloseConnectionProcPtr)                GXCloseConnectionUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCloseConnectionUPP)
    NewGXCloseConnectionUPP        (GXCloseConnectionProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCloseConnectionUPP    (GXCloseConnectionUPP    userUPP);

    EXTERN_API(OSErr)
    InvokeGXCloseConnectionUPP     (GXCloseConnectionUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCloseConnectionProcInfo = 0x00000021 };             /* 2_bytes Func() */
    #define NewGXCloseConnectionUPP(userRoutine)                    (GXCloseConnectionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCloseConnectionProcInfo, GetCurrentArchitecture())
    #define DisposeGXCloseConnectionUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCloseConnectionUPP(userUPP)                     (OSErr)CALL_ZERO_PARAMETER_UPP((userUPP), uppGXCloseConnectionProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCloseConnectionProc(userRoutine)                   NewGXCloseConnectionUPP(userRoutine)
#define CallGXCloseConnectionProc(userRoutine)                  InvokeGXCloseConnectionUPP(userRoutine)
#define Send_GXCloseConnection() MacSendMessage(0x00000025)
#define Forward_GXCloseConnection() ForwardThisMessage((void *) (0))

typedef CALLBACK_API_C( OSErr , GXStartSendPageProcPtr )(gxFormat theFormat);
typedef STACK_UPP_TYPE(GXStartSendPageProcPtr)                  GXStartSendPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXStartSendPageUPP)
    NewGXStartSendPageUPP          (GXStartSendPageProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeGXStartSendPageUPP      (GXStartSendPageUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeGXStartSendPageUPP       (gxFormat                theFormat,
                                    GXStartSendPageUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXStartSendPageProcInfo = 0x000000E1 };               /* 2_bytes Func(4_bytes) */
    #define NewGXStartSendPageUPP(userRoutine)                      (GXStartSendPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXStartSendPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXStartSendPageUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeGXStartSendPageUPP(theFormat, userUPP)            (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXStartSendPageProcInfo, (theFormat))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXStartSendPageProc(userRoutine)                     NewGXStartSendPageUPP(userRoutine)
#define CallGXStartSendPageProc(userRoutine, theFormat)         InvokeGXStartSendPageUPP(theFormat, userRoutine)
#define Send_GXStartSendPage(theFormat) MacSendMessage(0x00000026, theFormat)
#define Forward_GXStartSendPage(theFormat) ForwardThisMessage((void *) (theFormat))

typedef CALLBACK_API_C( OSErr , GXFinishSendPageProcPtr )(void );
typedef STACK_UPP_TYPE(GXFinishSendPageProcPtr)                 GXFinishSendPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFinishSendPageUPP)
    NewGXFinishSendPageUPP         (GXFinishSendPageProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXFinishSendPageUPP     (GXFinishSendPageUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeGXFinishSendPageUPP      (GXFinishSendPageUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFinishSendPageProcInfo = 0x00000021 };              /* 2_bytes Func() */
    #define NewGXFinishSendPageUPP(userRoutine)                     (GXFinishSendPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFinishSendPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXFinishSendPageUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFinishSendPageUPP(userUPP)                      (OSErr)CALL_ZERO_PARAMETER_UPP((userUPP), uppGXFinishSendPageProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFinishSendPageProc(userRoutine)                    NewGXFinishSendPageUPP(userRoutine)
#define CallGXFinishSendPageProc(userRoutine)                   InvokeGXFinishSendPageUPP(userRoutine)
#define Send_GXFinishSendPage() MacSendMessage(0x00000027)
#define Forward_GXFinishSendPage() ForwardThisMessage((void *) (0))

typedef CALLBACK_API_C( OSErr , GXWriteDataProcPtr )(Ptr data, long length);
typedef STACK_UPP_TYPE(GXWriteDataProcPtr)                      GXWriteDataUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXWriteDataUPP)
    NewGXWriteDataUPP              (GXWriteDataProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeGXWriteDataUPP          (GXWriteDataUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeGXWriteDataUPP           (Ptr                     data,
                                    long                    length,
                                    GXWriteDataUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXWriteDataProcInfo = 0x000003E1 };                   /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXWriteDataUPP(userRoutine)                          (GXWriteDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXWriteDataProcInfo, GetCurrentArchitecture())
    #define DisposeGXWriteDataUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeGXWriteDataUPP(data, length, userUPP)             (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXWriteDataProcInfo, (data), (length))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXWriteDataProc(userRoutine)                         NewGXWriteDataUPP(userRoutine)
#define CallGXWriteDataProc(userRoutine, data, length)          InvokeGXWriteDataUPP(data, length, userRoutine)
#define Send_GXWriteData(data, length) MacSendMessage(0x00000028, data, length)
#define Forward_GXWriteData(data, length) ForwardThisMessage((void *) (data), (void *) (length))

typedef CALLBACK_API_C( OSErr , GXBufferDataProcPtr )(Ptr data, long length, long bufferOptions);
typedef STACK_UPP_TYPE(GXBufferDataProcPtr)                     GXBufferDataUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXBufferDataUPP)
    NewGXBufferDataUPP             (GXBufferDataProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeGXBufferDataUPP         (GXBufferDataUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeGXBufferDataUPP          (Ptr                     data,
                                    long                    length,
                                    long                    bufferOptions,
                                    GXBufferDataUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXBufferDataProcInfo = 0x00000FE1 };                  /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXBufferDataUPP(userRoutine)                         (GXBufferDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXBufferDataProcInfo, GetCurrentArchitecture())
    #define DisposeGXBufferDataUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeGXBufferDataUPP(data, length, bufferOptions, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXBufferDataProcInfo, (data), (length), (bufferOptions))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXBufferDataProc(userRoutine)                        NewGXBufferDataUPP(userRoutine)
#define CallGXBufferDataProc(userRoutine, data, length, bufferOptions) InvokeGXBufferDataUPP(data, length, bufferOptions, userRoutine)
#define Send_GXBufferData(data, length, bufferOptions) \
          MacSendMessage(0x00000029, data, length, bufferOptions)
 
#define Forward_GXBufferData(data, length, bufferOptions) \
          ForwardThisMessage((void *) (data), (void *) (length), (void *) (bufferOptions))

typedef CALLBACK_API_C( OSErr , GXDumpBufferProcPtr )(gxPrintingBuffer *theBuffer);
typedef STACK_UPP_TYPE(GXDumpBufferProcPtr)                     GXDumpBufferUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDumpBufferUPP)
    NewGXDumpBufferUPP             (GXDumpBufferProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeGXDumpBufferUPP         (GXDumpBufferUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeGXDumpBufferUPP          (gxPrintingBuffer *      theBuffer,
                                    GXDumpBufferUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDumpBufferProcInfo = 0x000000E1 };                  /* 2_bytes Func(4_bytes) */
    #define NewGXDumpBufferUPP(userRoutine)                         (GXDumpBufferUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDumpBufferProcInfo, GetCurrentArchitecture())
    #define DisposeGXDumpBufferUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDumpBufferUPP(theBuffer, userUPP)               (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXDumpBufferProcInfo, (theBuffer))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDumpBufferProc(userRoutine)                        NewGXDumpBufferUPP(userRoutine)
#define CallGXDumpBufferProc(userRoutine, theBuffer)            InvokeGXDumpBufferUPP(theBuffer, userRoutine)
#define Send_GXDumpBuffer(theBuffer) MacSendMessage(0x0000002A, theBuffer)
#define Forward_GXDumpBuffer(theBuffer) ForwardThisMessage((void *) (theBuffer))

typedef CALLBACK_API_C( OSErr , GXFreeBufferProcPtr )(gxPrintingBuffer *theBuffer);
typedef STACK_UPP_TYPE(GXFreeBufferProcPtr)                     GXFreeBufferUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFreeBufferUPP)
    NewGXFreeBufferUPP             (GXFreeBufferProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeGXFreeBufferUPP         (GXFreeBufferUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeGXFreeBufferUPP          (gxPrintingBuffer *      theBuffer,
                                    GXFreeBufferUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFreeBufferProcInfo = 0x000000E1 };                  /* 2_bytes Func(4_bytes) */
    #define NewGXFreeBufferUPP(userRoutine)                         (GXFreeBufferUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFreeBufferProcInfo, GetCurrentArchitecture())
    #define DisposeGXFreeBufferUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFreeBufferUPP(theBuffer, userUPP)               (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXFreeBufferProcInfo, (theBuffer))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFreeBufferProc(userRoutine)                        NewGXFreeBufferUPP(userRoutine)
#define CallGXFreeBufferProc(userRoutine, theBuffer)            InvokeGXFreeBufferUPP(theBuffer, userRoutine)
#define Send_GXFreeBuffer(theBuffer) MacSendMessage(0x0000002B, theBuffer)
#define Forward_GXFreeBuffer(theBuffer) ForwardThisMessage((void *) (theBuffer))

typedef CALLBACK_API_C( OSErr , GXCheckStatusProcPtr )(Ptr data, long length, long statusType, gxOwnerSignature owner);
typedef STACK_UPP_TYPE(GXCheckStatusProcPtr)                    GXCheckStatusUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCheckStatusUPP)
    NewGXCheckStatusUPP            (GXCheckStatusProcPtr    userRoutine);

    EXTERN_API(void)
    DisposeGXCheckStatusUPP        (GXCheckStatusUPP        userUPP);

    EXTERN_API(OSErr)
    InvokeGXCheckStatusUPP         (Ptr                     data,
                                    long                    length,
                                    long                    statusType,
                                    gxOwnerSignature        owner,
                                    GXCheckStatusUPP        userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCheckStatusProcInfo = 0x00003FE1 };                 /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXCheckStatusUPP(userRoutine)                        (GXCheckStatusUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCheckStatusProcInfo, GetCurrentArchitecture())
    #define DisposeGXCheckStatusUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCheckStatusUPP(data, length, statusType, owner, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppGXCheckStatusProcInfo, (data), (length), (statusType), (owner))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCheckStatusProc(userRoutine)                       NewGXCheckStatusUPP(userRoutine)
#define CallGXCheckStatusProc(userRoutine, data, length, statusType, owner) InvokeGXCheckStatusUPP(data, length, statusType, owner, userRoutine)
#define Send_GXCheckStatus(data, length, statusType, owner) \
          MacSendMessage(0x0000002C, data, length, statusType, owner)
 
#define Forward_GXCheckStatus(data, length, statusType, owner) \
          ForwardThisMessage((void *) (data), (void *) (length), (void *) (statusType), (void *) (owner))

typedef CALLBACK_API_C( OSErr , GXGetDeviceStatusProcPtr )(Ptr cmdData, long cmdSize, Ptr responseData, long *responseSize, Str255 termination);
typedef STACK_UPP_TYPE(GXGetDeviceStatusProcPtr)                GXGetDeviceStatusUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXGetDeviceStatusUPP)
    NewGXGetDeviceStatusUPP        (GXGetDeviceStatusProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXGetDeviceStatusUPP    (GXGetDeviceStatusUPP    userUPP);

    EXTERN_API(OSErr)
    InvokeGXGetDeviceStatusUPP     (Ptr                     cmdData,
                                    long                    cmdSize,
                                    Ptr                     responseData,
                                    long *                  responseSize,
                                    Str255                  termination,
                                    GXGetDeviceStatusUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXGetDeviceStatusProcInfo = 0x0000FFE1 };             /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXGetDeviceStatusUPP(userRoutine)                    (GXGetDeviceStatusUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXGetDeviceStatusProcInfo, GetCurrentArchitecture())
    #define DisposeGXGetDeviceStatusUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeGXGetDeviceStatusUPP(cmdData, cmdSize, responseData, responseSize, termination, userUPP)  (OSErr)CALL_FIVE_PARAMETER_UPP((userUPP), uppGXGetDeviceStatusProcInfo, (cmdData), (cmdSize), (responseData), (responseSize), (termination))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXGetDeviceStatusProc(userRoutine)                   NewGXGetDeviceStatusUPP(userRoutine)
#define CallGXGetDeviceStatusProc(userRoutine, cmdData, cmdSize, responseData, responseSize, termination) InvokeGXGetDeviceStatusUPP(cmdData, cmdSize, responseData, responseSize, termination, userRoutine)
#define Send_GXGetDeviceStatus(cmdData, cmdSize, responseData, responseSize, termination) \
          MacSendMessage(0x0000002D, cmdData, cmdSize, responseData, responseSize, termination)
 
#define Forward_GXGetDeviceStatus(cmdData, cmdSize, responseData, responseSize, termination) \
          ForwardThisMessage((void *) (cmdData), (void *) (cmdSize), (void *) (responseData), \
                                    (void *) (responseSize), (void *) (termination))

typedef CALLBACK_API_C( OSErr , GXFetchTaggedDataProcPtr )(ResType theType, long id, Handle *dataHdl, gxOwnerSignature owner);
typedef STACK_UPP_TYPE(GXFetchTaggedDataProcPtr)                GXFetchTaggedDataUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFetchTaggedDataUPP)
    NewGXFetchTaggedDataUPP        (GXFetchTaggedDataProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXFetchTaggedDataUPP    (GXFetchTaggedDataUPP    userUPP);

    EXTERN_API(OSErr)
    InvokeGXFetchTaggedDataUPP     (ResType                 theType,
                                    long                    id,
                                    Handle *                dataHdl,
                                    gxOwnerSignature        owner,
                                    GXFetchTaggedDataUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFetchTaggedDataProcInfo = 0x00003FE1 };             /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXFetchTaggedDataUPP(userRoutine)                    (GXFetchTaggedDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFetchTaggedDataProcInfo, GetCurrentArchitecture())
    #define DisposeGXFetchTaggedDataUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFetchTaggedDataUPP(theType, id, dataHdl, owner, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppGXFetchTaggedDataProcInfo, (theType), (id), (dataHdl), (owner))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFetchTaggedDataProc(userRoutine)                   NewGXFetchTaggedDataUPP(userRoutine)
#define CallGXFetchTaggedDataProc(userRoutine, theType, id, dataHdl, owner) InvokeGXFetchTaggedDataUPP(theType, id, dataHdl, owner, userRoutine)
#define Send_GXFetchTaggedDriverData(tag, id, pHandle) Send_GXFetchTaggedData(tag, id, pHandle, 'drvr')
#define Forward_GXFetchTaggedDriverData(tag, id, pHandle) Forward_GXFetchTaggedData(tag, id, pHandle, 'drvr')
#define Send_GXFetchTaggedData(theType, id, dataHdl, owner) \
          MacSendMessage(0x0000002E, theType, id, dataHdl, owner)
 
#define Forward_GXFetchTaggedData(theType, id, dataHdl, owner) \
          ForwardThisMessage((void *) (theType), (void *) (id), (void *) (dataHdl), (void *) (owner))

typedef CALLBACK_API_C( OSErr , GXGetDTPMenuListProcPtr )(MenuHandle menuHdl);
typedef STACK_UPP_TYPE(GXGetDTPMenuListProcPtr)                 GXGetDTPMenuListUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXGetDTPMenuListUPP)
    NewGXGetDTPMenuListUPP         (GXGetDTPMenuListProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXGetDTPMenuListUPP     (GXGetDTPMenuListUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeGXGetDTPMenuListUPP      (MenuHandle              menuHdl,
                                    GXGetDTPMenuListUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXGetDTPMenuListProcInfo = 0x000000E1 };              /* 2_bytes Func(4_bytes) */
    #define NewGXGetDTPMenuListUPP(userRoutine)                     (GXGetDTPMenuListUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXGetDTPMenuListProcInfo, GetCurrentArchitecture())
    #define DisposeGXGetDTPMenuListUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeGXGetDTPMenuListUPP(menuHdl, userUPP)             (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXGetDTPMenuListProcInfo, (menuHdl))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXGetDTPMenuListProc(userRoutine)                    NewGXGetDTPMenuListUPP(userRoutine)
#define CallGXGetDTPMenuListProc(userRoutine, menuHdl)          InvokeGXGetDTPMenuListUPP(menuHdl, userRoutine)
#define Send_GXGetDTPMenuList(menuHdl) \
            MacSendMessage(0x0000002F, menuHdl)
 
#define Forward_GXGetDTPMenuList(menuHdl) \
            ForwardThisMessage((void *) (menuHdl))

typedef CALLBACK_API_C( OSErr , GXDTPMenuSelectProcPtr )(long id);
typedef STACK_UPP_TYPE(GXDTPMenuSelectProcPtr)                  GXDTPMenuSelectUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDTPMenuSelectUPP)
    NewGXDTPMenuSelectUPP          (GXDTPMenuSelectProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeGXDTPMenuSelectUPP      (GXDTPMenuSelectUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeGXDTPMenuSelectUPP       (long                    id,
                                    GXDTPMenuSelectUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDTPMenuSelectProcInfo = 0x000000E1 };               /* 2_bytes Func(4_bytes) */
    #define NewGXDTPMenuSelectUPP(userRoutine)                      (GXDTPMenuSelectUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDTPMenuSelectProcInfo, GetCurrentArchitecture())
    #define DisposeGXDTPMenuSelectUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDTPMenuSelectUPP(id, userUPP)                   (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXDTPMenuSelectProcInfo, (id))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDTPMenuSelectProc(userRoutine)                     NewGXDTPMenuSelectUPP(userRoutine)
#define CallGXDTPMenuSelectProc(userRoutine, id)                InvokeGXDTPMenuSelectUPP(id, userRoutine)
#define Send_GXDTPMenuSelect(id) \
            MacSendMessage(0x00000030, id)
 
#define Forward_GXDTPMenuSelect(id) \
            ForwardThisMessage((void *) (id))

typedef CALLBACK_API_C( OSErr , GXHandleAlertFilterProcPtr )(gxJob theJob, gxStatusRecord *pStatusRec, DialogPtr pDialog, EventRecord *theEvent, short *itemHit, Boolean *returnImmed);
typedef STACK_UPP_TYPE(GXHandleAlertFilterProcPtr)              GXHandleAlertFilterUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXHandleAlertFilterUPP)
    NewGXHandleAlertFilterUPP      (GXHandleAlertFilterProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXHandleAlertFilterUPP    (GXHandleAlertFilterUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXHandleAlertFilterUPP    (gxJob                  theJob,
                                    gxStatusRecord *        pStatusRec,
                                    DialogPtr               pDialog,
                                    EventRecord *           theEvent,
                                    short *                 itemHit,
                                    Boolean *               returnImmed,
                                    GXHandleAlertFilterUPP  userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXHandleAlertFilterProcInfo = 0x0003FFE1 };           /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXHandleAlertFilterUPP(userRoutine)                  (GXHandleAlertFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXHandleAlertFilterProcInfo, GetCurrentArchitecture())
    #define DisposeGXHandleAlertFilterUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define InvokeGXHandleAlertFilterUPP(theJob, pStatusRec, pDialog, theEvent, itemHit, returnImmed, userUPP)  (OSErr)CALL_SIX_PARAMETER_UPP((userUPP), uppGXHandleAlertFilterProcInfo, (theJob), (pStatusRec), (pDialog), (theEvent), (itemHit), (returnImmed))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXHandleAlertFilterProc(userRoutine)                 NewGXHandleAlertFilterUPP(userRoutine)
#define CallGXHandleAlertFilterProc(userRoutine, theJob, pStatusRec, pDialog, theEvent, itemHit, returnImmed) InvokeGXHandleAlertFilterUPP(theJob, pStatusRec, pDialog, theEvent, itemHit, returnImmed, userRoutine)
#define Send_GXHandleAlertFilter(theJob, pStatusRec, pDialog, theEvent, itemHit, returnImmed) \
            MacSendMessage(0x00000031, theJob, pStatusRec, pDialog, theEvent, itemHit, returnImmed)
 
#define Forward_GXHandleAlertFilter(theJob, pStatusRec, pDialog, theEvent, itemHit, returnImmed) \
            ForwardThisMessage((void *) (theJob), (void *) (pStatusRec), (void *) (pDialog), (void *) (theEvent), \
                                     (void *) (itemHit), (void *) (returnImmed))

typedef CALLBACK_API_C( OSErr , GXJobFormatModeQueryProcPtr )(gxQueryType theQuery, void *srcData, void *dstData);
typedef STACK_UPP_TYPE(GXJobFormatModeQueryProcPtr)             GXJobFormatModeQueryUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXJobFormatModeQueryUPP)
    NewGXJobFormatModeQueryUPP     (GXJobFormatModeQueryProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXJobFormatModeQueryUPP    (GXJobFormatModeQueryUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXJobFormatModeQueryUPP    (gxQueryType           theQuery,
                                    void *                  srcData,
                                    void *                  dstData,
                                    GXJobFormatModeQueryUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXJobFormatModeQueryProcInfo = 0x00000FE1 };          /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXJobFormatModeQueryUPP(userRoutine)                 (GXJobFormatModeQueryUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXJobFormatModeQueryProcInfo, GetCurrentArchitecture())
    #define DisposeGXJobFormatModeQueryUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define InvokeGXJobFormatModeQueryUPP(theQuery, srcData, dstData, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXJobFormatModeQueryProcInfo, (theQuery), (srcData), (dstData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXJobFormatModeQueryProc(userRoutine)                NewGXJobFormatModeQueryUPP(userRoutine)
#define CallGXJobFormatModeQueryProc(userRoutine, theQuery, srcData, dstData) InvokeGXJobFormatModeQueryUPP(theQuery, srcData, dstData, userRoutine)
#define Send_GXJobFormatModeQuery(theQuery, srcData, dstData) \
          MacSendMessage(0x00000032, theQuery, srcData, dstData)
 
#define Forward_GXJobFormatModeQuery(theQuery, srcData, dstData) \
          ForwardThisMessage((void *) (theQuery), (void *) (srcData), (void *) (dstData))

typedef CALLBACK_API_C( OSErr , GXWriteStatusToDTPWindowProcPtr )(gxStatusRecord *pStatusRec, gxDisplayRecord *pDisplay);
typedef STACK_UPP_TYPE(GXWriteStatusToDTPWindowProcPtr)         GXWriteStatusToDTPWindowUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXWriteStatusToDTPWindowUPP)
    NewGXWriteStatusToDTPWindowUPP    (GXWriteStatusToDTPWindowProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXWriteStatusToDTPWindowUPP    (GXWriteStatusToDTPWindowUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXWriteStatusToDTPWindowUPP    (gxStatusRecord *  pStatusRec,
                                    gxDisplayRecord *       pDisplay,
                                    GXWriteStatusToDTPWindowUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXWriteStatusToDTPWindowProcInfo = 0x000003E1 };      /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXWriteStatusToDTPWindowUPP(userRoutine)             (GXWriteStatusToDTPWindowUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXWriteStatusToDTPWindowProcInfo, GetCurrentArchitecture())
    #define DisposeGXWriteStatusToDTPWindowUPP(userUPP)             DisposeRoutineDescriptor(userUPP)
    #define InvokeGXWriteStatusToDTPWindowUPP(pStatusRec, pDisplay, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXWriteStatusToDTPWindowProcInfo, (pStatusRec), (pDisplay))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXWriteStatusToDTPWindowProc(userRoutine)            NewGXWriteStatusToDTPWindowUPP(userRoutine)
#define CallGXWriteStatusToDTPWindowProc(userRoutine, pStatusRec, pDisplay) InvokeGXWriteStatusToDTPWindowUPP(pStatusRec, pDisplay, userRoutine)
#define Send_GXWriteStatusToDTPWindow(pStatusRec, pDisplay) \
          MacSendMessage(0x00000033, pStatusRec, pDisplay)
 
#define Forward_GXWriteStatusToDTPWindow(pStatusRec, pDisplay) \
          ForwardThisMessage((void *) (pStatusRec), (void *) (pDisplay))

typedef CALLBACK_API_C( OSErr , GXInitializeStatusAlertProcPtr )(gxStatusRecord *pStatusRec, DialogPtr *pDialog);
typedef STACK_UPP_TYPE(GXInitializeStatusAlertProcPtr)          GXInitializeStatusAlertUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXInitializeStatusAlertUPP)
    NewGXInitializeStatusAlertUPP    (GXInitializeStatusAlertProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXInitializeStatusAlertUPP    (GXInitializeStatusAlertUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXInitializeStatusAlertUPP    (gxStatusRecord *   pStatusRec,
                                    DialogPtr *             pDialog,
                                    GXInitializeStatusAlertUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXInitializeStatusAlertProcInfo = 0x000003E1 };       /* 2_bytes Func(4_bytes, 4_bytes) */
    #define NewGXInitializeStatusAlertUPP(userRoutine)              (GXInitializeStatusAlertUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXInitializeStatusAlertProcInfo, GetCurrentArchitecture())
    #define DisposeGXInitializeStatusAlertUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define InvokeGXInitializeStatusAlertUPP(pStatusRec, pDialog, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppGXInitializeStatusAlertProcInfo, (pStatusRec), (pDialog))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXInitializeStatusAlertProc(userRoutine)             NewGXInitializeStatusAlertUPP(userRoutine)
#define CallGXInitializeStatusAlertProc(userRoutine, pStatusRec, pDialog) InvokeGXInitializeStatusAlertUPP(pStatusRec, pDialog, userRoutine)
#define Send_GXInitializeStatusAlert(pStatusRec, pDialog) \
          MacSendMessage(0x00000034, pStatusRec, pDialog)
 
#define Forward_GXInitializeStatusAlert(pStatusRec, pDialog) \
          ForwardThisMessage((void *) (pStatusRec), (void *) (pDialog))

typedef CALLBACK_API_C( OSErr , GXHandleAlertStatusProcPtr )(gxStatusRecord *pStatusRec);
typedef STACK_UPP_TYPE(GXHandleAlertStatusProcPtr)              GXHandleAlertStatusUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXHandleAlertStatusUPP)
    NewGXHandleAlertStatusUPP      (GXHandleAlertStatusProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXHandleAlertStatusUPP    (GXHandleAlertStatusUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXHandleAlertStatusUPP    (gxStatusRecord *       pStatusRec,
                                    GXHandleAlertStatusUPP  userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXHandleAlertStatusProcInfo = 0x000000E1 };           /* 2_bytes Func(4_bytes) */
    #define NewGXHandleAlertStatusUPP(userRoutine)                  (GXHandleAlertStatusUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXHandleAlertStatusProcInfo, GetCurrentArchitecture())
    #define DisposeGXHandleAlertStatusUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define InvokeGXHandleAlertStatusUPP(pStatusRec, userUPP)       (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXHandleAlertStatusProcInfo, (pStatusRec))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXHandleAlertStatusProc(userRoutine)                 NewGXHandleAlertStatusUPP(userRoutine)
#define CallGXHandleAlertStatusProc(userRoutine, pStatusRec)    InvokeGXHandleAlertStatusUPP(pStatusRec, userRoutine)
#define Send_GXHandleAlertStatus(pStatusRec) \
          MacSendMessage(0x00000035, pStatusRec)
 
#define Forward_GXHandleAlertStatus(pStatusRec) \
          ForwardThisMessage((void *) (pStatusRec))

typedef CALLBACK_API_C( OSErr , GXHandleAlertEventProcPtr )(gxStatusRecord *pStatusRec, DialogPtr pDialog, EventRecord *theEvent, short *response);
typedef STACK_UPP_TYPE(GXHandleAlertEventProcPtr)               GXHandleAlertEventUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXHandleAlertEventUPP)
    NewGXHandleAlertEventUPP       (GXHandleAlertEventProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXHandleAlertEventUPP    (GXHandleAlertEventUPP  userUPP);

    EXTERN_API(OSErr)
    InvokeGXHandleAlertEventUPP    (gxStatusRecord *        pStatusRec,
                                    DialogPtr               pDialog,
                                    EventRecord *           theEvent,
                                    short *                 response,
                                    GXHandleAlertEventUPP   userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXHandleAlertEventProcInfo = 0x00003FE1 };            /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXHandleAlertEventUPP(userRoutine)                   (GXHandleAlertEventUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXHandleAlertEventProcInfo, GetCurrentArchitecture())
    #define DisposeGXHandleAlertEventUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define InvokeGXHandleAlertEventUPP(pStatusRec, pDialog, theEvent, response, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppGXHandleAlertEventProcInfo, (pStatusRec), (pDialog), (theEvent), (response))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXHandleAlertEventProc(userRoutine)                  NewGXHandleAlertEventUPP(userRoutine)
#define CallGXHandleAlertEventProc(userRoutine, pStatusRec, pDialog, theEvent, response) InvokeGXHandleAlertEventUPP(pStatusRec, pDialog, theEvent, response, userRoutine)
#define Send_GXHandleAlertEvent(pStatusRec, pDialog, theEvent, response) \
          MacSendMessage(0x00000036, pStatusRec, pDialog, theEvent, response)
 
#define Forward_GXHandleAlertEvent(pStatusRec, pDialog, theEvent, response) \
          ForwardThisMessage((void *) (pStatusRec), (void *) (pDialog), \
                                    (void *) (theEvent), (void *) (response))

typedef CALLBACK_API_C( void , GXCleanupStartJobProcPtr )(void );
typedef STACK_UPP_TYPE(GXCleanupStartJobProcPtr)                GXCleanupStartJobUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCleanupStartJobUPP)
    NewGXCleanupStartJobUPP        (GXCleanupStartJobProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCleanupStartJobUPP    (GXCleanupStartJobUPP    userUPP);

    EXTERN_API(void)
    InvokeGXCleanupStartJobUPP     (GXCleanupStartJobUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCleanupStartJobProcInfo = 0x00000001 };             /* no_return_value Func() */
    #define NewGXCleanupStartJobUPP(userRoutine)                    (GXCleanupStartJobUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCleanupStartJobProcInfo, GetCurrentArchitecture())
    #define DisposeGXCleanupStartJobUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCleanupStartJobUPP(userUPP)                     CALL_ZERO_PARAMETER_UPP((userUPP), uppGXCleanupStartJobProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCleanupStartJobProc(userRoutine)                   NewGXCleanupStartJobUPP(userRoutine)
#define CallGXCleanupStartJobProc(userRoutine)                  InvokeGXCleanupStartJobUPP(userRoutine)
#define Send_GXCleanupStartJob() ((void) MacSendMessage(0x00000037))
#define Forward_GXCleanupStartJob() ((void) ForwardThisMessage((void *) (0)))

typedef CALLBACK_API_C( void , GXCleanupStartPageProcPtr )(void );
typedef STACK_UPP_TYPE(GXCleanupStartPageProcPtr)               GXCleanupStartPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCleanupStartPageUPP)
    NewGXCleanupStartPageUPP       (GXCleanupStartPageProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCleanupStartPageUPP    (GXCleanupStartPageUPP  userUPP);

    EXTERN_API(void)
    InvokeGXCleanupStartPageUPP    (GXCleanupStartPageUPP   userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCleanupStartPageProcInfo = 0x00000001 };            /* no_return_value Func() */
    #define NewGXCleanupStartPageUPP(userRoutine)                   (GXCleanupStartPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCleanupStartPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXCleanupStartPageUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCleanupStartPageUPP(userUPP)                    CALL_ZERO_PARAMETER_UPP((userUPP), uppGXCleanupStartPageProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCleanupStartPageProc(userRoutine)                  NewGXCleanupStartPageUPP(userRoutine)
#define CallGXCleanupStartPageProc(userRoutine)                 InvokeGXCleanupStartPageUPP(userRoutine)
#define Send_GXCleanupStartPage() ((void) MacSendMessage(0x00000038))
#define Forward_GXCleanupStartPage() ((void) ForwardThisMessage((void *) (0)))

typedef CALLBACK_API_C( void , GXCleanupOpenConnectionProcPtr )(void );
typedef STACK_UPP_TYPE(GXCleanupOpenConnectionProcPtr)          GXCleanupOpenConnectionUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCleanupOpenConnectionUPP)
    NewGXCleanupOpenConnectionUPP    (GXCleanupOpenConnectionProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCleanupOpenConnectionUPP    (GXCleanupOpenConnectionUPP userUPP);

    EXTERN_API(void)
    InvokeGXCleanupOpenConnectionUPP    (GXCleanupOpenConnectionUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCleanupOpenConnectionProcInfo = 0x00000001 };       /* no_return_value Func() */
    #define NewGXCleanupOpenConnectionUPP(userRoutine)              (GXCleanupOpenConnectionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCleanupOpenConnectionProcInfo, GetCurrentArchitecture())
    #define DisposeGXCleanupOpenConnectionUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCleanupOpenConnectionUPP(userUPP)               CALL_ZERO_PARAMETER_UPP((userUPP), uppGXCleanupOpenConnectionProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCleanupOpenConnectionProc(userRoutine)             NewGXCleanupOpenConnectionUPP(userRoutine)
#define CallGXCleanupOpenConnectionProc(userRoutine)            InvokeGXCleanupOpenConnectionUPP(userRoutine)
#define Send_GXCleanupOpenConnection() ((void) MacSendMessage(0x00000039))
#define Forward_GXCleanupOpenConnection() ((void) ForwardThisMessage((void *) (0)))

typedef CALLBACK_API_C( void , GXCleanupStartSendPageProcPtr )(void );
typedef STACK_UPP_TYPE(GXCleanupStartSendPageProcPtr)           GXCleanupStartSendPageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCleanupStartSendPageUPP)
    NewGXCleanupStartSendPageUPP    (GXCleanupStartSendPageProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCleanupStartSendPageUPP    (GXCleanupStartSendPageUPP userUPP);

    EXTERN_API(void)
    InvokeGXCleanupStartSendPageUPP    (GXCleanupStartSendPageUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCleanupStartSendPageProcInfo = 0x00000001 };        /* no_return_value Func() */
    #define NewGXCleanupStartSendPageUPP(userRoutine)               (GXCleanupStartSendPageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCleanupStartSendPageProcInfo, GetCurrentArchitecture())
    #define DisposeGXCleanupStartSendPageUPP(userUPP)               DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCleanupStartSendPageUPP(userUPP)                CALL_ZERO_PARAMETER_UPP((userUPP), uppGXCleanupStartSendPageProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCleanupStartSendPageProc(userRoutine)              NewGXCleanupStartSendPageUPP(userRoutine)
#define CallGXCleanupStartSendPageProc(userRoutine)             InvokeGXCleanupStartSendPageUPP(userRoutine)
#define Send_GXCleanupStartSendPage()  ((void) MacSendMessage(0x0000003A))
#define Forward_GXCleanupStartSendPage() ((void) ForwardThisMessage((void *) (0)))

typedef CALLBACK_API_C( OSErr , GXDefaultDesktopPrinterProcPtr )(Str31 dtpName);
typedef STACK_UPP_TYPE(GXDefaultDesktopPrinterProcPtr)          GXDefaultDesktopPrinterUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDefaultDesktopPrinterUPP)
    NewGXDefaultDesktopPrinterUPP    (GXDefaultDesktopPrinterProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXDefaultDesktopPrinterUPP    (GXDefaultDesktopPrinterUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXDefaultDesktopPrinterUPP    (Str31              dtpName,
                                    GXDefaultDesktopPrinterUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDefaultDesktopPrinterProcInfo = 0x000000E1 };       /* 2_bytes Func(4_bytes) */
    #define NewGXDefaultDesktopPrinterUPP(userRoutine)              (GXDefaultDesktopPrinterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDefaultDesktopPrinterProcInfo, GetCurrentArchitecture())
    #define DisposeGXDefaultDesktopPrinterUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDefaultDesktopPrinterUPP(dtpName, userUPP)      (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXDefaultDesktopPrinterProcInfo, (dtpName))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDefaultDesktopPrinterProc(userRoutine)             NewGXDefaultDesktopPrinterUPP(userRoutine)
#define CallGXDefaultDesktopPrinterProc(userRoutine, dtpName)   InvokeGXDefaultDesktopPrinterUPP(dtpName, userRoutine)
#define Send_GXDefaultDesktopPrinter(dtpName) MacSendMessage(0x0000003B, dtpName)
#define Forward_GXDefaultDesktopPrinter(dtpName) ForwardThisMessage((void *) (dtpName))

typedef CALLBACK_API_C( OSErr , GXCaptureOutputDeviceProcPtr )(Boolean capture);
typedef STACK_UPP_TYPE(GXCaptureOutputDeviceProcPtr)            GXCaptureOutputDeviceUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXCaptureOutputDeviceUPP)
    NewGXCaptureOutputDeviceUPP    (GXCaptureOutputDeviceProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXCaptureOutputDeviceUPP    (GXCaptureOutputDeviceUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXCaptureOutputDeviceUPP    (Boolean              capture,
                                    GXCaptureOutputDeviceUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXCaptureOutputDeviceProcInfo = 0x00000061 };         /* 2_bytes Func(1_byte) */
    #define NewGXCaptureOutputDeviceUPP(userRoutine)                (GXCaptureOutputDeviceUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXCaptureOutputDeviceProcInfo, GetCurrentArchitecture())
    #define DisposeGXCaptureOutputDeviceUPP(userUPP)                DisposeRoutineDescriptor(userUPP)
    #define InvokeGXCaptureOutputDeviceUPP(capture, userUPP)        (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXCaptureOutputDeviceProcInfo, (capture))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXCaptureOutputDeviceProc(userRoutine)               NewGXCaptureOutputDeviceUPP(userRoutine)
#define CallGXCaptureOutputDeviceProc(userRoutine, capture)     InvokeGXCaptureOutputDeviceUPP(capture, userRoutine)
#define Send_GXCaptureOutputDevice(capture) MacSendMessage(0x0000003C, capture)
#define Forward_GXCaptureOutputDevice(capture) ForwardThisMessage((void *) (capture))

typedef CALLBACK_API_C( OSErr , GXOpenConnectionRetryProcPtr )(ResType theType, void *aVoid, Boolean *retryopenPtr, OSErr anErr);
typedef STACK_UPP_TYPE(GXOpenConnectionRetryProcPtr)            GXOpenConnectionRetryUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXOpenConnectionRetryUPP)
    NewGXOpenConnectionRetryUPP    (GXOpenConnectionRetryProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXOpenConnectionRetryUPP    (GXOpenConnectionRetryUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXOpenConnectionRetryUPP    (ResType              theType,
                                    void *                  aVoid,
                                    Boolean *               retryopenPtr,
                                    OSErr                   anErr,
                                    GXOpenConnectionRetryUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXOpenConnectionRetryProcInfo = 0x00002FE1 };         /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 2_bytes) */
    #define NewGXOpenConnectionRetryUPP(userRoutine)                (GXOpenConnectionRetryUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXOpenConnectionRetryProcInfo, GetCurrentArchitecture())
    #define DisposeGXOpenConnectionRetryUPP(userUPP)                DisposeRoutineDescriptor(userUPP)
    #define InvokeGXOpenConnectionRetryUPP(theType, aVoid, retryopenPtr, anErr, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppGXOpenConnectionRetryProcInfo, (theType), (aVoid), (retryopenPtr), (anErr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXOpenConnectionRetryProc(userRoutine)               NewGXOpenConnectionRetryUPP(userRoutine)
#define CallGXOpenConnectionRetryProc(userRoutine, theType, aVoid, retryopenPtr, anErr) InvokeGXOpenConnectionRetryUPP(theType, aVoid, retryopenPtr, anErr, userRoutine)
#define Send_GXOpenConnectionRetry(theType, aVoid, retryopenPtr, anErr) \
          MacSendMessage(0x0000003D, theType, aVoid, retryopenPtr, anErr)
 
#define Forward_GXOpenConnectionRetry(theType, aVoid, retryopenPtr, anErr) \
          ForwardThisMessage((void *) (theType), (void *) (aVoid), (void *) (retryopenPtr), (void *) (anErr))

typedef CALLBACK_API_C( OSErr , GXExamineSpoolFileProcPtr )(gxSpoolFile theSpoolFile);
typedef STACK_UPP_TYPE(GXExamineSpoolFileProcPtr)               GXExamineSpoolFileUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXExamineSpoolFileUPP)
    NewGXExamineSpoolFileUPP       (GXExamineSpoolFileProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXExamineSpoolFileUPP    (GXExamineSpoolFileUPP  userUPP);

    EXTERN_API(OSErr)
    InvokeGXExamineSpoolFileUPP    (gxSpoolFile             theSpoolFile,
                                    GXExamineSpoolFileUPP   userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXExamineSpoolFileProcInfo = 0x000000E1 };            /* 2_bytes Func(4_bytes) */
    #define NewGXExamineSpoolFileUPP(userRoutine)                   (GXExamineSpoolFileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXExamineSpoolFileProcInfo, GetCurrentArchitecture())
    #define DisposeGXExamineSpoolFileUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define InvokeGXExamineSpoolFileUPP(theSpoolFile, userUPP)      (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXExamineSpoolFileProcInfo, (theSpoolFile))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXExamineSpoolFileProc(userRoutine)                  NewGXExamineSpoolFileUPP(userRoutine)
#define CallGXExamineSpoolFileProc(userRoutine, theSpoolFile)   InvokeGXExamineSpoolFileUPP(theSpoolFile, userRoutine)
#define Send_GXExamineSpoolFile(theSpoolFile) MacSendMessage(0x0000003E, theSpoolFile)
#define Forward_GXExamineSpoolFile(theSpoolFile) ForwardThisMessage((void *) (theSpoolFile))

typedef CALLBACK_API_C( OSErr , GXFinishSendPlaneProcPtr )(void );
typedef STACK_UPP_TYPE(GXFinishSendPlaneProcPtr)                GXFinishSendPlaneUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFinishSendPlaneUPP)
    NewGXFinishSendPlaneUPP        (GXFinishSendPlaneProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXFinishSendPlaneUPP    (GXFinishSendPlaneUPP    userUPP);

    EXTERN_API(OSErr)
    InvokeGXFinishSendPlaneUPP     (GXFinishSendPlaneUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFinishSendPlaneProcInfo = 0x00000021 };             /* 2_bytes Func() */
    #define NewGXFinishSendPlaneUPP(userRoutine)                    (GXFinishSendPlaneUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFinishSendPlaneProcInfo, GetCurrentArchitecture())
    #define DisposeGXFinishSendPlaneUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFinishSendPlaneUPP(userUPP)                     (OSErr)CALL_ZERO_PARAMETER_UPP((userUPP), uppGXFinishSendPlaneProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFinishSendPlaneProc(userRoutine)                   NewGXFinishSendPlaneUPP(userRoutine)
#define CallGXFinishSendPlaneProc(userRoutine)                  InvokeGXFinishSendPlaneUPP(userRoutine)
#define Send_GXFinishSendPlane() MacSendMessage(0x0000003F)
#define Forward_GXFinishSendPlane() ForwardThisMessage((void *) (0))

typedef CALLBACK_API_C( OSErr , GXDoesPaperFitProcPtr )(gxTrayIndex whichTray, gxPaperType paper, Boolean *fits);
typedef STACK_UPP_TYPE(GXDoesPaperFitProcPtr)                   GXDoesPaperFitUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXDoesPaperFitUPP)
    NewGXDoesPaperFitUPP           (GXDoesPaperFitProcPtr   userRoutine);

    EXTERN_API(void)
    DisposeGXDoesPaperFitUPP       (GXDoesPaperFitUPP       userUPP);

    EXTERN_API(OSErr)
    InvokeGXDoesPaperFitUPP        (gxTrayIndex             whichTray,
                                    gxPaperType             paper,
                                    Boolean *               fits,
                                    GXDoesPaperFitUPP       userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXDoesPaperFitProcInfo = 0x00000FE1 };                /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXDoesPaperFitUPP(userRoutine)                       (GXDoesPaperFitUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXDoesPaperFitProcInfo, GetCurrentArchitecture())
    #define DisposeGXDoesPaperFitUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define InvokeGXDoesPaperFitUPP(whichTray, paper, fits, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXDoesPaperFitProcInfo, (whichTray), (paper), (fits))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXDoesPaperFitProc(userRoutine)                      NewGXDoesPaperFitUPP(userRoutine)
#define CallGXDoesPaperFitProc(userRoutine, whichTray, paper, fits) InvokeGXDoesPaperFitUPP(whichTray, paper, fits, userRoutine)
#define Send_GXDoesPaperFit(whichTray, paper, fits) \
          MacSendMessage(0x00000040, whichTray, paper, fits)
 
#define Forward_GXDoesPaperFit(whichTray, paper, fits) \
          ForwardThisMessage((void *) (whichTray), (void *) (paper), (void *) (fits))

typedef CALLBACK_API_C( OSErr , GXChooserMessageProcPtr )(long message, long caller, StringPtr objName, StringPtr zoneName, ListHandle theList, long p2);
typedef STACK_UPP_TYPE(GXChooserMessageProcPtr)                 GXChooserMessageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXChooserMessageUPP)
    NewGXChooserMessageUPP         (GXChooserMessageProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXChooserMessageUPP     (GXChooserMessageUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeGXChooserMessageUPP      (long                    message,
                                    long                    caller,
                                    StringPtr               objName,
                                    StringPtr               zoneName,
                                    ListHandle              theList,
                                    long                    p2,
                                    GXChooserMessageUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXChooserMessageProcInfo = 0x0003FFE1 };              /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXChooserMessageUPP(userRoutine)                     (GXChooserMessageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXChooserMessageProcInfo, GetCurrentArchitecture())
    #define DisposeGXChooserMessageUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeGXChooserMessageUPP(message, caller, objName, zoneName, theList, p2, userUPP)  (OSErr)CALL_SIX_PARAMETER_UPP((userUPP), uppGXChooserMessageProcInfo, (message), (caller), (objName), (zoneName), (theList), (p2))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXChooserMessageProc(userRoutine)                    NewGXChooserMessageUPP(userRoutine)
#define CallGXChooserMessageProc(userRoutine, message, caller, objName, zoneName, theList, p2) InvokeGXChooserMessageUPP(message, caller, objName, zoneName, theList, p2, userRoutine)
#define Send_GXChooserMessage(message, caller, objName, zoneName, theList, p2) \
          MacSendMessage(0x00000041, message, caller, objName, zoneName, theList, p2)
 
#define Forward_GXChooserMessage(message, caller, objName, zoneName, theList, p2) \
          ForwardThisMessage((void *) (message), (void *) (caller), (void *) (objName), \
                                    (void *) (zoneName), (void *) (theList), (void *) (p2))

typedef CALLBACK_API_C( OSErr , GXFindPrinterProfileProcPtr )(gxPrinter thePrinter, void *searchData, long index, gxColorProfile *returnedProfile, long *numProfiles);
typedef STACK_UPP_TYPE(GXFindPrinterProfileProcPtr)             GXFindPrinterProfileUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFindPrinterProfileUPP)
    NewGXFindPrinterProfileUPP     (GXFindPrinterProfileProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXFindPrinterProfileUPP    (GXFindPrinterProfileUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXFindPrinterProfileUPP    (gxPrinter             thePrinter,
                                    void *                  searchData,
                                    long                    index,
                                    gxColorProfile *        returnedProfile,
                                    long *                  numProfiles,
                                    GXFindPrinterProfileUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFindPrinterProfileProcInfo = 0x0000FFE1 };          /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXFindPrinterProfileUPP(userRoutine)                 (GXFindPrinterProfileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFindPrinterProfileProcInfo, GetCurrentArchitecture())
    #define DisposeGXFindPrinterProfileUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFindPrinterProfileUPP(thePrinter, searchData, index, returnedProfile, numProfiles, userUPP)  (OSErr)CALL_FIVE_PARAMETER_UPP((userUPP), uppGXFindPrinterProfileProcInfo, (thePrinter), (searchData), (index), (returnedProfile), (numProfiles))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFindPrinterProfileProc(userRoutine)                NewGXFindPrinterProfileUPP(userRoutine)
#define CallGXFindPrinterProfileProc(userRoutine, thePrinter, searchData, index, returnedProfile, numProfiles) InvokeGXFindPrinterProfileUPP(thePrinter, searchData, index, returnedProfile, numProfiles, userRoutine)
#define Send_GXFindPrinterProfile(thePrinter, searchData, index, returnedProfile, numProfiles) \
          MacSendMessage(0x00000042, thePrinter, searchData, index, returnedProfile, numProfiles)
 
#define Forward_GXFindPrinterProfile(thePrinter, searchData, index, returnedProfile, numProfiles) \
          ForwardThisMessage((void *) (thePrinter), (void *) (searchData), (void *) (index), (void *) (returnedProfile), (void *) (numProfiles))

typedef CALLBACK_API_C( OSErr , GXFindFormatProfileProcPtr )(gxFormat theFormat, void *searchData, long index, gxColorProfile *returnedProfile, long *numProfiles);
typedef STACK_UPP_TYPE(GXFindFormatProfileProcPtr)              GXFindFormatProfileUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXFindFormatProfileUPP)
    NewGXFindFormatProfileUPP      (GXFindFormatProfileProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXFindFormatProfileUPP    (GXFindFormatProfileUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXFindFormatProfileUPP    (gxFormat               theFormat,
                                    void *                  searchData,
                                    long                    index,
                                    gxColorProfile *        returnedProfile,
                                    long *                  numProfiles,
                                    GXFindFormatProfileUPP  userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXFindFormatProfileProcInfo = 0x0000FFE1 };           /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewGXFindFormatProfileUPP(userRoutine)                  (GXFindFormatProfileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXFindFormatProfileProcInfo, GetCurrentArchitecture())
    #define DisposeGXFindFormatProfileUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define InvokeGXFindFormatProfileUPP(theFormat, searchData, index, returnedProfile, numProfiles, userUPP)  (OSErr)CALL_FIVE_PARAMETER_UPP((userUPP), uppGXFindFormatProfileProcInfo, (theFormat), (searchData), (index), (returnedProfile), (numProfiles))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXFindFormatProfileProc(userRoutine)                 NewGXFindFormatProfileUPP(userRoutine)
#define CallGXFindFormatProfileProc(userRoutine, theFormat, searchData, index, returnedProfile, numProfiles) InvokeGXFindFormatProfileUPP(theFormat, searchData, index, returnedProfile, numProfiles, userRoutine)
#define Send_GXFindFormatProfile(theFormat, searchData, index, returnedProfile, numProfiles) \
          MacSendMessage(0x00000043, theFormat, searchData, index, returnedProfile, numProfiles)
 
#define Forward_GXFindFormatProfile(theFormat, searchData, index, returnedProfile, numProfiles) \
          ForwardThisMessage((void *) (theFormat), (void *) (searchData), (void *) (index), (void *) (returnedProfile), \
                                    (void *) (numProfiles))

typedef CALLBACK_API_C( OSErr , GXSetPrinterProfileProcPtr )(gxPrinter thePrinter, gxColorProfile oldProfile, gxColorProfile newProfile);
typedef STACK_UPP_TYPE(GXSetPrinterProfileProcPtr)              GXSetPrinterProfileUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXSetPrinterProfileUPP)
    NewGXSetPrinterProfileUPP      (GXSetPrinterProfileProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXSetPrinterProfileUPP    (GXSetPrinterProfileUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXSetPrinterProfileUPP    (gxPrinter              thePrinter,
                                    gxColorProfile          oldProfile,
                                    gxColorProfile          newProfile,
                                    GXSetPrinterProfileUPP  userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXSetPrinterProfileProcInfo = 0x00000FE1 };           /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXSetPrinterProfileUPP(userRoutine)                  (GXSetPrinterProfileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXSetPrinterProfileProcInfo, GetCurrentArchitecture())
    #define DisposeGXSetPrinterProfileUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define InvokeGXSetPrinterProfileUPP(thePrinter, oldProfile, newProfile, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXSetPrinterProfileProcInfo, (thePrinter), (oldProfile), (newProfile))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXSetPrinterProfileProc(userRoutine)                 NewGXSetPrinterProfileUPP(userRoutine)
#define CallGXSetPrinterProfileProc(userRoutine, thePrinter, oldProfile, newProfile) InvokeGXSetPrinterProfileUPP(thePrinter, oldProfile, newProfile, userRoutine)
#define Send_GXSetPrinterProfile(thePrinter, oldProfile, newProfile) \
          MacSendMessage(0x00000044, thePrinter, oldProfile, newProfile)
 
#define Forward_GXSetPrinterProfile(thePrinter, oldProfile, newProfile) \
          ForwardThisMessage((void *) (thePrinter), (void *) (oldProfile), (void *) (newProfile))

typedef CALLBACK_API_C( OSErr , GXSetFormatProfileProcPtr )(gxFormat theFormat, gxColorProfile oldProfile, gxColorProfile newProfile);
typedef STACK_UPP_TYPE(GXSetFormatProfileProcPtr)               GXSetFormatProfileUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXSetFormatProfileUPP)
    NewGXSetFormatProfileUPP       (GXSetFormatProfileProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXSetFormatProfileUPP    (GXSetFormatProfileUPP  userUPP);

    EXTERN_API(OSErr)
    InvokeGXSetFormatProfileUPP    (gxFormat                theFormat,
                                    gxColorProfile          oldProfile,
                                    gxColorProfile          newProfile,
                                    GXSetFormatProfileUPP   userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXSetFormatProfileProcInfo = 0x00000FE1 };            /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXSetFormatProfileUPP(userRoutine)                   (GXSetFormatProfileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXSetFormatProfileProcInfo, GetCurrentArchitecture())
    #define DisposeGXSetFormatProfileUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define InvokeGXSetFormatProfileUPP(theFormat, oldProfile, newProfile, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXSetFormatProfileProcInfo, (theFormat), (oldProfile), (newProfile))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXSetFormatProfileProc(userRoutine)                  NewGXSetFormatProfileUPP(userRoutine)
#define CallGXSetFormatProfileProc(userRoutine, theFormat, oldProfile, newProfile) InvokeGXSetFormatProfileUPP(theFormat, oldProfile, newProfile, userRoutine)
#define Send_GXSetFormatProfile(theFormat, oldProfile, newProfile) \
          MacSendMessage(0x00000045, theFormat, oldProfile, newProfile)
 
#define Forward_GXSetFormatProfile(theFormat, oldProfile, newProfile) \
          ForwardThisMessage((void *) (theFormat), (void *) (oldProfile), (void *) (newProfile))

typedef CALLBACK_API_C( OSErr , GXHandleAltDestinationProcPtr )(Boolean *userCancels);
typedef STACK_UPP_TYPE(GXHandleAltDestinationProcPtr)           GXHandleAltDestinationUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXHandleAltDestinationUPP)
    NewGXHandleAltDestinationUPP    (GXHandleAltDestinationProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXHandleAltDestinationUPP    (GXHandleAltDestinationUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXHandleAltDestinationUPP    (Boolean *           userCancels,
                                    GXHandleAltDestinationUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXHandleAltDestinationProcInfo = 0x000000E1 };        /* 2_bytes Func(4_bytes) */
    #define NewGXHandleAltDestinationUPP(userRoutine)               (GXHandleAltDestinationUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXHandleAltDestinationProcInfo, GetCurrentArchitecture())
    #define DisposeGXHandleAltDestinationUPP(userUPP)               DisposeRoutineDescriptor(userUPP)
    #define InvokeGXHandleAltDestinationUPP(userCancels, userUPP)   (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppGXHandleAltDestinationProcInfo, (userCancels))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXHandleAltDestinationProc(userRoutine)              NewGXHandleAltDestinationUPP(userRoutine)
#define CallGXHandleAltDestinationProc(userRoutine, userCancels) InvokeGXHandleAltDestinationUPP(userCancels, userRoutine)
#define Send_GXHandleAltDestination(userCancels) MacSendMessage(0x00000046, userCancels)
#define Forward_GXHandleAltDestination(userCancels) ForwardThisMessage((void *) (userCancels))

typedef CALLBACK_API_C( OSErr , GXSetupPageImageDataProcPtr )(gxFormat theFormat, gxShape thePage, void *imageData);
typedef STACK_UPP_TYPE(GXSetupPageImageDataProcPtr)             GXSetupPageImageDataUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(GXSetupPageImageDataUPP)
    NewGXSetupPageImageDataUPP     (GXSetupPageImageDataProcPtr userRoutine);

    EXTERN_API(void)
    DisposeGXSetupPageImageDataUPP    (GXSetupPageImageDataUPP userUPP);

    EXTERN_API(OSErr)
    InvokeGXSetupPageImageDataUPP    (gxFormat              theFormat,
                                    gxShape                 thePage,
                                    void *                  imageData,
                                    GXSetupPageImageDataUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppGXSetupPageImageDataProcInfo = 0x00000FE1 };          /* 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGXSetupPageImageDataUPP(userRoutine)                 (GXSetupPageImageDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGXSetupPageImageDataProcInfo, GetCurrentArchitecture())
    #define DisposeGXSetupPageImageDataUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define InvokeGXSetupPageImageDataUPP(theFormat, thePage, imageData, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGXSetupPageImageDataProcInfo, (theFormat), (thePage), (imageData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGXSetupPageImageDataProc(userRoutine)                NewGXSetupPageImageDataUPP(userRoutine)
#define CallGXSetupPageImageDataProc(userRoutine, theFormat, thePage, imageData) InvokeGXSetupPageImageDataUPP(theFormat, thePage, imageData, userRoutine)
#define Send_GXSetupPageImageData(theFormat, thePage, imageData) \
          MacSendMessage(0x00000047, theFormat, thePage, imageData)
 
#define Forward_GXSetupPageImageData(theFormat, thePage, imageData) \
          ForwardThisMessage((void *) (theFormat), (void *) (thePage), (void *) (imageData))
/*******************************************************************
                Start of old "GXPrintingErrors.h/a/p" interface file.
        ********************************************************************/
enum {
    gxPrintingResultBase        = -510                          /*First QuickDraw GX printing error code.*/
};

/*RESULT CODES FOR QUICKDRAW GX PRINTING OPERATIONS*/
enum {
    gxAioTimeout                = (gxPrintingResultBase),       /*-510 : Timeout condition occurred during operation*/
    gxAioBadRqstState           = (gxPrintingResultBase - 1),   /*-511 : Async I/O request in invalid state for operation*/
    gxAioBadConn                = (gxPrintingResultBase - 2),   /*-512 : Invalid Async I/O connection refnum*/
    gxAioInvalidXfer            = (gxPrintingResultBase - 3),   /*-513 : Read data transfer structure contained bad values*/
    gxAioNoRqstBlks             = (gxPrintingResultBase - 4),   /*-514 : No available request blocks to process request*/
    gxAioNoDataXfer             = (gxPrintingResultBase - 5),   /*-515 : Data transfer structure pointer not specified*/
    gxAioTooManyAutos           = (gxPrintingResultBase - 6),   /*-516 : Auto status request already active*/
    gxAioNoAutoStat             = (gxPrintingResultBase - 7),   /*-517 : Connection not configured for auto status*/
    gxAioBadRqstID              = (gxPrintingResultBase - 8),   /*-518 : Invalid I/O request identifier*/
    gxAioCantKill               = (gxPrintingResultBase - 9),   /*-519 : Comm. protocol doesn't support I/O term*/
    gxAioAlreadyExists          = (gxPrintingResultBase - 10),  /*-520 : Protocol spec. data already specified*/
    gxAioCantFind               = (gxPrintingResultBase - 11),  /*-521 : Protocol spec. data does not exist*/
    gxAioDeviceDisconn          = (gxPrintingResultBase - 12),  /*-522 : Machine disconnected from printer*/
    gxAioNotImplemented         = (gxPrintingResultBase - 13),  /*-523 : Function not implemented*/
    gxAioOpenPending            = (gxPrintingResultBase - 14),  /*-524 : Opening a connection for protocol, but another open pending*/
    gxAioNoProtocolData         = (gxPrintingResultBase - 15),  /*-525 : No protocol specific data specified in request*/
    gxAioRqstKilled             = (gxPrintingResultBase - 16),  /*-526 : I/O request was terminated*/
    gxBadBaudRate               = (gxPrintingResultBase - 17),  /*-527 : Invalid baud rate specified*/
    gxBadParity                 = (gxPrintingResultBase - 18),  /*-528 : Invalid parity specified*/
    gxBadStopBits               = (gxPrintingResultBase - 19),  /*-529 : Invalid stop bits specified*/
    gxBadDataBits               = (gxPrintingResultBase - 20),  /*-530 : Invalid data bits specified*/
    gxBadPrinterName            = (gxPrintingResultBase - 21),  /*-531 : Bad printer name specified*/
    gxAioBadMsgType             = (gxPrintingResultBase - 22),  /*-532 : Bad masType field in transfer info structure*/
    gxAioCantFindDevice         = (gxPrintingResultBase - 23),  /*-533 : Cannot locate target device*/
    gxAioOutOfSeq               = (gxPrintingResultBase - 24),  /*-534 : Non-atomic SCSI requests submitted out of sequence*/
    gxPrIOAbortErr              = (gxPrintingResultBase - 25),  /*-535 : I/O operation aborted*/
    gxPrUserAbortErr            = (gxPrintingResultBase - 26),  /*-536 : User aborted*/
    gxCantAddPanelsNowErr       = (gxPrintingResultBase - 27),  /*-537 : Can only add panels during driver switch or dialog setup*/
    gxBadxdtlKeyErr             = (gxPrintingResultBase - 28),  /*-538 : Unknown key for xdtl - must be radiobutton, etc*/
    gxXdtlItemOutOfRangeErr     = (gxPrintingResultBase - 29),  /*-539 : Referenced item does not belong to panel*/
    gxNoActionButtonErr         = (gxPrintingResultBase - 30),  /*-540 : Action button is nil*/
    gxTitlesTooLongErr          = (gxPrintingResultBase - 31),  /*-541 : Length of buttons exceeds alert maximum width*/
    gxUnknownAlertVersionErr    = (gxPrintingResultBase - 32),  /*-542 : Bad version for printing alerts*/
    gxGBBufferTooSmallErr       = (gxPrintingResultBase - 33),  /*-543 : Buffer too small.*/
    gxInvalidPenTable           = (gxPrintingResultBase - 34),  /*-544 : Invalid vector driver pen table.*/
    gxIncompletePrintFileErr    = (gxPrintingResultBase - 35),  /*-545 : Print file was not completely spooled*/
    gxCrashedPrintFileErr       = (gxPrintingResultBase - 36),  /*-546 : Print file is corrupted*/
    gxInvalidPrintFileVersion   = (gxPrintingResultBase - 37),  /*-547 : Print file is incompatible with current QuickDraw GX version*/
    gxSegmentLoadFailedErr      = (gxPrintingResultBase - 38),  /*-548 : Segment loader error*/
    gxExtensionNotFoundErr      = (gxPrintingResultBase - 39),  /*-549 : Requested printing extension could not be found*/
    gxDriverVersionErr          = (gxPrintingResultBase - 40),  /*-550 : Driver too new for current version of QuickDraw GX*/
    gxImagingSystemVersionErr   = (gxPrintingResultBase - 41),  /*-551 : Imaging system too new for current version of QuickDraw GX*/
    gxFlattenVersionTooNew      = (gxPrintingResultBase - 42),  /*-552 : Flattened object format too new for current version of QDGX*/
    gxPaperTypeNotFound         = (gxPrintingResultBase - 43),  /*-553 : Requested papertype could not be found*/
    gxNoSuchPTGroup             = (gxPrintingResultBase - 44),  /*-554 : Requested papertype group could not be found*/
    gxNotEnoughPrinterMemory    = (gxPrintingResultBase - 45),  /*-555 : Printer does not have enough memory for fonts in document*/
    gxDuplicatePanelNameErr     = (gxPrintingResultBase - 46),  /*-556 : Attempt to add more than 10 panels with the same name*/
    gxExtensionVersionErr       = (gxPrintingResultBase - 47)   /*-557 : Extension too new for current version of QuickDraw GX*/
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

#endif /* __GXPRINTING__ */

