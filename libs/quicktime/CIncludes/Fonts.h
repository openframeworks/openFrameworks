/*
     File:       Fonts.h
 
     Contains:   Public interface to the Font Manager.
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FONTS__
#define __FONTS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
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
    #pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

enum {
    systemFont                  = 0,
    applFont                    = 1
};

/* kPlatformDefaultGuiFontID is used in QuickTime 3.0. */
#if TARGET_OS_MAC
enum {
    kPlatformDefaultGuiFontID   = applFont
};

#else
enum {
    kPlatformDefaultGuiFontID   = -1
};

#endif  /* TARGET_OS_MAC */

/*  The following font constants are deprecated; use GetFNum() to look up the font ID by name. */
enum {
    kFontIDNewYork              = 2,
    kFontIDGeneva               = 3,
    kFontIDMonaco               = 4,
    kFontIDVenice               = 5,
    kFontIDLondon               = 6,
    kFontIDAthens               = 7,
    kFontIDSanFrancisco         = 8,
    kFontIDToronto              = 9,
    kFontIDCairo                = 11,
    kFontIDLosAngeles           = 12,
    kFontIDTimes                = 20,
    kFontIDHelvetica            = 21,
    kFontIDCourier              = 22,
    kFontIDSymbol               = 23,
    kFontIDMobile               = 24
};

enum {
    commandMark                 = 17,
    checkMark                   = 18,
    diamondMark                 = 19,
    appleMark                   = 20
};

enum {
    propFont                    = 36864L,
    prpFntH                     = 36865L,
    prpFntW                     = 36866L,
    prpFntHW                    = 36867L,
    fixedFont                   = 45056L,
    fxdFntH                     = 45057L,
    fxdFntW                     = 45058L,
    fxdFntHW                    = 45059L,
    fontWid                     = 44208L
};


struct FMInput {
    short                           family;
    short                           size;
    Style                           face;
    Boolean                         needBits;
    short                           device;
    Point                           numer;
    Point                           denom;
};
typedef struct FMInput                  FMInput;

struct FMOutput {
    short                           errNum;
    Handle                          fontHandle;
    UInt8                           boldPixels;
    UInt8                           italicPixels;
    UInt8                           ulOffset;
    UInt8                           ulShadow;
    UInt8                           ulThick;
    UInt8                           shadowPixels;
    SInt8                           extra;
    UInt8                           ascent;
    UInt8                           descent;
    UInt8                           widMax;
    SInt8                           leading;
    SInt8                           curStyle;
    Point                           numer;
    Point                           denom;
};
typedef struct FMOutput                 FMOutput;
typedef FMOutput *                      FMOutputPtr;

typedef FMOutputPtr                     FMOutPtr;

struct FontRec {
    short                           fontType;                   /*font type*/
    short                           firstChar;                  /*ASCII code of first character*/
    short                           lastChar;                   /*ASCII code of last character*/
    short                           widMax;                     /*maximum character width*/
    short                           kernMax;                    /*negative of maximum character kern*/
    short                           nDescent;                   /*negative of descent*/
    short                           fRectWidth;                 /*width of font rectangle*/
    short                           fRectHeight;                /*height of font rectangle*/
    unsigned short                  owTLoc;                     /*offset to offset/width table*/
    short                           ascent;                     /*ascent*/
    short                           descent;                    /*descent*/
    short                           leading;                    /*leading*/
    short                           rowWords;                   /*row width of bit image / 2 */
};
typedef struct FontRec                  FontRec;
typedef FontRec *                       FontRecPtr;
typedef FontRecPtr *                    FontRecHdl;

struct FMetricRec {
    Fixed                           ascent;                     /*base line to top*/
    Fixed                           descent;                    /*base line to bottom*/
    Fixed                           leading;                    /*leading between lines*/
    Fixed                           widMax;                     /*maximum character width*/
    Handle                          wTabHandle;                 /*handle to font width table*/
};
typedef struct FMetricRec               FMetricRec;
typedef FMetricRec *                    FMetricRecPtr;
typedef FMetricRecPtr *                 FMetricRecHandle;

struct WidEntry {
    short                           widStyle;                   /*style entry applies to*/
};
typedef struct WidEntry                 WidEntry;

struct WidTable {
    short                           numWidths;                  /*number of entries - 1*/
};
typedef struct WidTable                 WidTable;

struct AsscEntry {
    short                           fontSize;
    short                           fontStyle;
    short                           fontID;                     /*font resource ID*/
};
typedef struct AsscEntry                AsscEntry;

struct FontAssoc {
    short                           numAssoc;                   /*number of entries - 1*/
};
typedef struct FontAssoc                FontAssoc;

struct StyleTable {
    short                           fontClass;
    long                            offset;
    long                            reserved;
    char                            indexes[48];
};
typedef struct StyleTable               StyleTable;

struct NameTable {
    short                           stringCount;
    Str255                          baseFontName;
};
typedef struct NameTable                NameTable;

struct KernPair {
    char                            kernFirst;                  /*1st character of kerned pair*/
    char                            kernSecond;                 /*2nd character of kerned pair*/
    short                           kernWidth;                  /*kerning in 1pt fixed format*/
};
typedef struct KernPair                 KernPair;

struct KernEntry {
    short                           kernStyle;                  /*style the entry applies to*/
    short                           kernLength;                 /*length of this entry*/
};
typedef struct KernEntry                KernEntry;

struct KernTable {
    short                           numKerns;                   /*number of kerning entries*/
};
typedef struct KernTable                KernTable;

struct WidthTable {
    Fixed                           tabData[256];               /*character widths*/
    Handle                          tabFont;                    /*font record used to build table*/
    long                            sExtra;                     /*space extra used for table*/
    long                            style;                      /*extra due to style*/
    short                           fID;                        /*font family ID*/
    short                           fSize;                      /*font size request*/
    short                           face;                       /*style (face) request*/
    short                           device;                     /*device requested*/
    Point                           inNumer;                    /*scale factors requested*/
    Point                           inDenom;                    /*scale factors requested*/
    short                           aFID;                       /*actual font family ID for table*/
    Handle                          fHand;                      /*family record used to build up table*/
    Boolean                         usedFam;                    /*used fixed point family widths*/
    UInt8                           aFace;                      /*actual face produced*/
    short                           vOutput;                    /*vertical scale output value*/
    short                           hOutput;                    /*horizontal scale output value*/
    short                           vFactor;                    /*vertical scale output value*/
    short                           hFactor;                    /*horizontal scale output value*/
    short                           aSize;                      /*actual size of actual font used*/
    short                           tabSize;                    /*total size of table*/
};
typedef struct WidthTable               WidthTable;
typedef WidthTable *                    WidthTablePtr;
typedef WidthTablePtr *                 WidthTableHdl;

struct FamRec {
    short                           ffFlags;                    /*flags for family*/
    short                           ffFamID;                    /*family ID number*/
    short                           ffFirstChar;                /*ASCII code of 1st character*/
    short                           ffLastChar;                 /*ASCII code of last character*/
    short                           ffAscent;                   /*maximum ascent for 1pt font*/
    short                           ffDescent;                  /*maximum descent for 1pt font*/
    short                           ffLeading;                  /*maximum leading for 1pt font*/
    short                           ffWidMax;                   /*maximum widMax for 1pt font*/
    long                            ffWTabOff;                  /*offset to width table*/
    long                            ffKernOff;                  /*offset to kerning table*/
    long                            ffStylOff;                  /*offset to style mapping table*/
    short                           ffProperty[9];              /*style property info*/
    short                           ffIntl[2];                  /*for international use*/
    short                           ffVersion;                  /*version number*/
};
typedef struct FamRec                   FamRec;

typedef SInt16                          FontPointSize;
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
InitFonts                       (void)                                                      ONEWORDINLINE(0xA8FE);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( void )
GetFontName                     (short                  familyID,
                                 Str255                 name)                               ONEWORDINLINE(0xA8FF);

EXTERN_API( void )
GetFNum                         (ConstStr255Param       name,
                                 short *                familyID)                           ONEWORDINLINE(0xA900);

EXTERN_API( Boolean )
RealFont                        (short                  fontNum,
                                 short                  size)                               ONEWORDINLINE(0xA902);

#if CALL_NOT_IN_CARBON
EXTERN_API( void )
SetFontLock                     (Boolean                lockFlag)                           ONEWORDINLINE(0xA903);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( FMOutPtr )
FMSwapFont                      (const FMInput *        inRec)                              ONEWORDINLINE(0xA901);

EXTERN_API( void )
SetFScaleDisable                (Boolean                fscaleDisable)                      ONEWORDINLINE(0xA834);

EXTERN_API( void )
FontMetrics                     (FMetricRecPtr          theMetrics)                         ONEWORDINLINE(0xA835);

EXTERN_API( void )
SetFractEnable                  (Boolean                fractEnable)                        ONEWORDINLINE(0xA814);

EXTERN_API( short )
GetDefFontSize                  (void)                                                      FIVEWORDINLINE(0x3EB8, 0x0BA8, 0x6604, 0x3EBC, 0x000C);

EXTERN_API( Boolean )
IsOutline                       (Point                  numer,
                                 Point                  denom)                              TWOWORDINLINE(0x7000, 0xA854);

EXTERN_API( void )
SetOutlinePreferred             (Boolean                outlinePreferred)                   TWOWORDINLINE(0x7001, 0xA854);

EXTERN_API( Boolean )
GetOutlinePreferred             (void)                                                      TWOWORDINLINE(0x7009, 0xA854);

EXTERN_API( OSErr )
OutlineMetrics                  (short                  byteCount,
                                 const void *           textPtr,
                                 Point                  numer,
                                 Point                  denom,
                                 short *                yMax,
                                 short *                yMin,
                                 FixedPtr               awArray,
                                 FixedPtr               lsbArray,
                                 RectPtr                boundsArray)                        TWOWORDINLINE(0x7008, 0xA854);

EXTERN_API( void )
SetPreserveGlyph                (Boolean                preserveGlyph)                      TWOWORDINLINE(0x700A, 0xA854);

EXTERN_API( Boolean )
GetPreserveGlyph                (void)                                                      TWOWORDINLINE(0x700B, 0xA854);

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
FlushFonts                      (void)                                                      TWOWORDINLINE(0x700C, 0xA854);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API_C( void )
getfnum                         (const char *           theName,
                                 short *                familyID);

EXTERN_API_C( void )
getfontname                     (short                  familyID,
                                 char *                 theName);

EXTERN_API( short )
GetSysFont                      (void)                                                      TWOWORDINLINE(0x3EB8, 0x0BA6);

EXTERN_API( short )
GetAppFont                      (void)                                                      TWOWORDINLINE(0x3EB8, 0x0984);

/*--------------------------------------------------------------------------------------*/
/*  Extended font data functions (available only with Mac OS 8.5 or later)              */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSStatus )
SetAntiAliasedTextEnabled       (Boolean                iEnable,
                                 SInt16                 iMinFontSize)                       TWOWORDINLINE(0x7011, 0xA854);

EXTERN_API( Boolean )
IsAntiAliasedTextEnabled        (SInt16 *               oMinFontSize)                       TWOWORDINLINE(0x7012, 0xA854);

EXTERN_API( void )
QDTextBounds                    (short                  byteCount,
                                 const void *           textAddr,
                                 Rect *                 bounds)                             TWOWORDINLINE(0x7013, 0xA854);

EXTERN_API( OSErr )
FetchFontInfo                   (SInt16                 fontID,
                                 SInt16                 fontSize,
                                 SInt16                 fontStyle,
                                 FontInfo *             info)                               TWOWORDINLINE(0x7014, 0xA854);

/*--------------------------------------------------------------------------------------*/
/*  Font access and data management functions (available only with Mac OS 9.0 or later) */
/*--------------------------------------------------------------------------------------*/
typedef UInt32                          FMGeneration;
/* The FMFontFamily reference represents a collection of fonts with the same design     */
/* characteristics. It replaces the standard QuickDraw font identifer and may be used   */
/* with all QuickDraw functions including GetFontName and TextFont. It cannot be used   */
/* with the Resource Manager to access information from a FOND resource handle. A font  */
/* reference does not imply a particular script system, nor is the character encoding   */
/* of a font family determined by an arithmetic mapping of its value.                   */
typedef SInt16                          FMFontFamily;
typedef SInt16                          FMFontStyle;
typedef SInt16                          FMFontSize;
/* The font family is a collection of fonts, each of which is identified by an FMFont   */
/* reference that maps to a single object registered with the font database. The font   */
/* references associated with the font family consist of individual outline and         */
/* bitmapped fonts that may be used with the font access routines of the Font Manager   */
/* and ATS Unicode. */
typedef UInt32                          FMFont;

struct FMFontFamilyInstance {
    FMFontFamily                    fontFamily;
    FMFontStyle                     fontStyle;
};
typedef struct FMFontFamilyInstance     FMFontFamilyInstance;

struct FMFontFamilyIterator {
    UInt32                          reserved[16];
};
typedef struct FMFontFamilyIterator     FMFontFamilyIterator;

struct FMFontIterator {
    UInt32                          reserved[16];
};
typedef struct FMFontIterator           FMFontIterator;

struct FMFontFamilyInstanceIterator {
    UInt32                          reserved[16];
};
typedef struct FMFontFamilyInstanceIterator FMFontFamilyInstanceIterator;
enum {
    kInvalidGeneration          = 0L,
    kInvalidFontFamily          = -1,
    kInvalidFont                = 0L,
    kFMCurrentFilterFormat      = 0L,
    kFMDefaultOptions           = kNilOptions,
    kFMUseGlobalScopeOption     = 0x00000001
};


typedef UInt32 FMFilterSelector;
enum {
    kFMInvalidFilterSelector    = 0L,
    kFMFontTechnologyFilterSelector = 1L,
    kFMFontContainerFilterSelector = 2L,
    kFMGenerationFilterSelector = 3L,
    kFMFontFamilyCallbackFilterSelector = 4L,
    kFMFontCallbackFilterSelector = 5L
};

enum {
    kFMTrueTypeFontTechnology   = FOUR_CHAR_CODE('true'),
    kFMPostScriptFontTechnology = FOUR_CHAR_CODE('typ1')
};

typedef CALLBACK_API( OSStatus , FMFontFamilyCallbackFilterProcPtr )(FMFontFamily iFontFamily, void *iRefCon);
typedef CALLBACK_API( OSStatus , FMFontCallbackFilterProcPtr )(FMFont iFont, void *iRefCon);
typedef STACK_UPP_TYPE(FMFontFamilyCallbackFilterProcPtr)       FMFontFamilyCallbackFilterUPP;
typedef STACK_UPP_TYPE(FMFontCallbackFilterProcPtr)             FMFontCallbackFilterUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(FMFontFamilyCallbackFilterUPP)
    NewFMFontFamilyCallbackFilterUPP    (FMFontFamilyCallbackFilterProcPtr userRoutine);

    EXTERN_API(FMFontCallbackFilterUPP)
    NewFMFontCallbackFilterUPP     (FMFontCallbackFilterProcPtr userRoutine);

    EXTERN_API(void)
    DisposeFMFontFamilyCallbackFilterUPP    (FMFontFamilyCallbackFilterUPP userUPP);

    EXTERN_API(void)
    DisposeFMFontCallbackFilterUPP    (FMFontCallbackFilterUPP userUPP);

    EXTERN_API(OSStatus)
    InvokeFMFontFamilyCallbackFilterUPP    (FMFontFamily    iFontFamily,
                                    void *                  iRefCon,
                                    FMFontFamilyCallbackFilterUPP userUPP);

    EXTERN_API(OSStatus)
    InvokeFMFontCallbackFilterUPP    (FMFont                iFont,
                                    void *                  iRefCon,
                                    FMFontCallbackFilterUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppFMFontFamilyCallbackFilterProcInfo = 0x000003B0 };    /* pascal 4_bytes Func(2_bytes, 4_bytes) */
    enum { uppFMFontCallbackFilterProcInfo = 0x000003F0 };          /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    #define NewFMFontFamilyCallbackFilterUPP(userRoutine)           (FMFontFamilyCallbackFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppFMFontFamilyCallbackFilterProcInfo, GetCurrentArchitecture())
    #define NewFMFontCallbackFilterUPP(userRoutine)                 (FMFontCallbackFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppFMFontCallbackFilterProcInfo, GetCurrentArchitecture())
    #define DisposeFMFontFamilyCallbackFilterUPP(userUPP)           DisposeRoutineDescriptor(userUPP)
    #define DisposeFMFontCallbackFilterUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define InvokeFMFontFamilyCallbackFilterUPP(iFontFamily, iRefCon, userUPP)  (OSStatus)CALL_TWO_PARAMETER_UPP((userUPP), uppFMFontFamilyCallbackFilterProcInfo, (iFontFamily), (iRefCon))
    #define InvokeFMFontCallbackFilterUPP(iFont, iRefCon, userUPP)  (OSStatus)CALL_TWO_PARAMETER_UPP((userUPP), uppFMFontCallbackFilterProcInfo, (iFont), (iRefCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewFMFontFamilyCallbackFilterProc(userRoutine)          NewFMFontFamilyCallbackFilterUPP(userRoutine)
#define NewFMFontCallbackFilterProc(userRoutine)                NewFMFontCallbackFilterUPP(userRoutine)
#define CallFMFontFamilyCallbackFilterProc(userRoutine, iFontFamily, iRefCon) InvokeFMFontFamilyCallbackFilterUPP(iFontFamily, iRefCon, userRoutine)
#define CallFMFontCallbackFilterProc(userRoutine, iFont, iRefCon) InvokeFMFontCallbackFilterUPP(iFont, iRefCon, userRoutine)

struct FMFilter {
    UInt32                          format;
    FMFilterSelector                selector;
    union {
        FourCharCode                    fontTechnologyFilter;
        FSSpec                          fontContainerFilter;
        FMGeneration                    generationFilter;
        FMFontFamilyCallbackFilterUPP   fontFamilyCallbackFilter;
        FMFontCallbackFilterUPP         fontCallbackFilter;
    }                                 filter;
};
typedef struct FMFilter                 FMFilter;
/* Enumeration */
EXTERN_API_C( OSStatus )
FMCreateFontFamilyIterator      (const FMFilter *       iFilter, /* can be NULL */
                                 void *                 iRefCon,
                                 OptionBits             iOptions,
                                 FMFontFamilyIterator * ioIterator);

EXTERN_API_C( OSStatus )
FMDisposeFontFamilyIterator     (FMFontFamilyIterator * ioIterator);

EXTERN_API_C( OSStatus )
FMResetFontFamilyIterator       (const FMFilter *       iFilter, /* can be NULL */
                                 void *                 iRefCon,
                                 OptionBits             iOptions,
                                 FMFontFamilyIterator * ioIterator);

EXTERN_API_C( OSStatus )
FMGetNextFontFamily             (FMFontFamilyIterator * ioIterator,
                                 FMFontFamily *         oFontFamily);

EXTERN_API_C( OSStatus )
FMCreateFontIterator            (const FMFilter *       iFilter, /* can be NULL */
                                 void *                 iRefCon,
                                 OptionBits             iOptions,
                                 FMFontIterator *       ioIterator);

EXTERN_API_C( OSStatus )
FMDisposeFontIterator           (FMFontIterator *       ioIterator);

EXTERN_API_C( OSStatus )
FMResetFontIterator             (const FMFilter *       iFilter, /* can be NULL */
                                 void *                 iRefCon,
                                 OptionBits             iOptions,
                                 FMFontIterator *       ioIterator);

EXTERN_API_C( OSStatus )
FMGetNextFont                   (FMFontIterator *       ioIterator,
                                 FMFont *               oFont);

/* Font families */
EXTERN_API_C( OSStatus )
FMCreateFontFamilyInstanceIterator (FMFontFamily        iFontFamily,
                                 FMFontFamilyInstanceIterator * ioIterator);

EXTERN_API_C( OSStatus )
FMDisposeFontFamilyInstanceIterator (FMFontFamilyInstanceIterator * ioIterator);

EXTERN_API_C( OSStatus )
FMResetFontFamilyInstanceIterator (FMFontFamily         iFontFamily,
                                 FMFontFamilyInstanceIterator * ioIterator);

EXTERN_API_C( OSStatus )
FMGetNextFontFamilyInstance     (FMFontFamilyInstanceIterator * ioIterator,
                                 FMFont *               oFont,
                                 FMFontStyle *          oStyle, /* can be NULL */
                                 FMFontSize *           oSize) /* can be NULL */;

EXTERN_API_C( FMFontFamily )
FMGetFontFamilyFromName         (ConstStr255Param       iName);

EXTERN_API_C( OSStatus )
FMGetFontFamilyName             (FMFontFamily           iFontFamily,
                                 Str255                 oName);

EXTERN_API_C( OSStatus )
FMGetFontFamilyTextEncoding     (FMFontFamily           iFontFamily,
                                 TextEncoding *         oTextEncoding);

EXTERN_API_C( OSStatus )
FMGetFontFamilyGeneration       (FMFontFamily           iFontFamily,
                                 FMGeneration *         oGeneration);

/* Fonts */
EXTERN_API_C( OSStatus )
FMGetFontFormat                 (FMFont                 iFont,
                                 FourCharCode *         oFormat);

EXTERN_API_C( OSStatus )
FMGetFontTableDirectory         (FMFont                 iFont,
                                 ByteCount              iLength,
                                 void *                 iBuffer,
                                 ByteCount *            oActualLength) /* can be NULL */;

EXTERN_API_C( OSStatus )
FMGetFontTable                  (FMFont                 iFont,
                                 FourCharCode           iTag,
                                 ByteOffset             iOffset,
                                 ByteCount              iLength,
                                 void *                 iBuffer,
                                 ByteCount *            oActualLength) /* can be NULL */;

EXTERN_API_C( OSStatus )
FMGetFontGeneration             (FMFont                 iFont,
                                 FMGeneration *         oGeneration);

EXTERN_API_C( OSStatus )
FMGetFontContainer              (FMFont                 iFont,
                                 FSSpec *               oFontContainer);

/* Conversion */
EXTERN_API_C( OSStatus )
FMGetFontFromFontFamilyInstance (FMFontFamily           iFontFamily,
                                 FMFontStyle            iStyle,
                                 FMFont *               oFont,
                                 FMFontStyle *          oIntrinsicStyle) /* can be NULL */;

EXTERN_API_C( OSStatus )
FMGetFontFamilyInstanceFromFont (FMFont                 iFont,
                                 FMFontFamily *         oFontFamily,
                                 FMFontStyle *          oStyle);

/* Activation */
EXTERN_API_C( OSStatus )
FMActivateFonts                 (const FSSpec *         iFontContainer,
                                 const FMFilter *       iFilter, /* can be NULL */
                                 void *                 iRefCon,
                                 OptionBits             iOptions);

EXTERN_API_C( OSStatus )
FMDeactivateFonts               (const FSSpec *         iFontContainer,
                                 const FMFilter *       iFilter, /* can be NULL */
                                 void *                 iRefCon,
                                 OptionBits             iOptions);

EXTERN_API_C( FMGeneration )
FMGetGeneration                 (void);


typedef FMFontFamily                    FontFamilyID;
/*--------------------------------------------------------------------------------------*/
#if OLDROUTINENAMES
enum {
    newYork                     = kFontIDNewYork,
    geneva                      = kFontIDGeneva,
    monaco                      = kFontIDMonaco,
    venice                      = kFontIDVenice,
    london                      = kFontIDLondon,
    athens                      = kFontIDAthens,
    sanFran                     = kFontIDSanFrancisco,
    toronto                     = kFontIDToronto,
    cairo                       = kFontIDCairo,
    losAngeles                  = kFontIDLosAngeles,
    times                       = kFontIDTimes,
    helvetica                   = kFontIDHelvetica,
    courier                     = kFontIDCourier,
    symbol                      = kFontIDSymbol,
    mobile                      = kFontIDMobile
};

#endif  /* OLDROUTINENAMES */

/*--------------------------------------------------------------------------------------*/

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

#endif /* __FONTS__ */

