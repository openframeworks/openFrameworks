/*
     File:       CMApplication.h
 
     Contains:   Color Matching Interfaces
 
     Version:    Technology: ColorSync 3.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1992-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CMAPPLICATION__
#define __CMAPPLICATION__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __CMICCPROFILE__
#include <CMICCProfile.h>
#endif

#ifndef __DISPLAYS__
#include <Displays.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif

#if TARGET_API_MAC_OS8
#ifndef __PRINTING__
#include <Printing.h>
#endif

#endif  /* TARGET_API_MAC_OS8 */


#if TARGET_OS_WIN32
#include <windows.h>
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
    kDefaultCMMSignature        = FOUR_CHAR_CODE('appl')
};

/* Macintosh 68K trap word */
enum {
    cmTrap                      = 0xABEE
};


/* PicComment IDs */
enum {
    cmBeginProfile              = 220,
    cmEndProfile                = 221,
    cmEnableMatching            = 222,
    cmDisableMatching           = 223,
    cmComment                   = 224
};

/* PicComment selectors for cmComment */
enum {
    cmBeginProfileSel           = 0,
    cmContinueProfileSel        = 1,
    cmEndProfileSel             = 2,
    cmProfileIdentifierSel      = 3
};


/* Defines for version 1.0 CMProfileSearchRecord.fieldMask */
enum {
    cmMatchCMMType              = 0x00000001,
    cmMatchApplProfileVersion   = 0x00000002,
    cmMatchDataType             = 0x00000004,
    cmMatchDeviceType           = 0x00000008,
    cmMatchDeviceManufacturer   = 0x00000010,
    cmMatchDeviceModel          = 0x00000020,
    cmMatchDeviceAttributes     = 0x00000040,
    cmMatchFlags                = 0x00000080,
    cmMatchOptions              = 0x00000100,
    cmMatchWhite                = 0x00000200,
    cmMatchBlack                = 0x00000400
};

/* Defines for version 2.0 CMSearchRecord.searchMask */
enum {
    cmMatchAnyProfile           = 0x00000000,
    cmMatchProfileCMMType       = 0x00000001,
    cmMatchProfileClass         = 0x00000002,
    cmMatchDataColorSpace       = 0x00000004,
    cmMatchProfileConnectionSpace = 0x00000008,
    cmMatchManufacturer         = 0x00000010,
    cmMatchModel                = 0x00000020,
    cmMatchAttributes           = 0x00000040,
    cmMatchProfileFlags         = 0x00000080
};


/* deviceType values for ColorSync 1.0 Device Profile access */
enum {
    cmSystemDevice              = FOUR_CHAR_CODE('sys '),
    cmGDevice                   = FOUR_CHAR_CODE('gdev')
};

/* Commands for CMFlattenUPP(...) */
enum {
    cmOpenReadSpool             = 1,
    cmOpenWriteSpool            = 2,
    cmReadSpool                 = 3,
    cmWriteSpool                = 4,
    cmCloseSpool                = 5
};

/* Flags for PostScript-related functions */
enum {
    cmPS7bit                    = 1,
    cmPS8bit                    = 2
};

/* Flags for profile embedding functions */
enum {
    cmEmbedWholeProfile         = 0x00000000,
    cmEmbedProfileIdentifier    = 0x00000001
};

/* Commands for CMAccessUPP(...) */
enum {
    cmOpenReadAccess            = 1,
    cmOpenWriteAccess           = 2,
    cmReadAccess                = 3,
    cmWriteAccess               = 4,
    cmCloseAccess               = 5,
    cmCreateNewAccess           = 6,
    cmAbortWriteAccess          = 7,
    cmBeginAccess               = 8,
    cmEndAccess                 = 9
};


/* Abstract data type for memory-based Profile */
typedef struct OpaqueCMProfileRef*      CMProfileRef;
/* Abstract data type for Profile search result */
typedef struct OpaqueCMProfileSearchRef*  CMProfileSearchRef;
/* Abstract data type for BeginMatching(...) reference */
typedef struct OpaqueCMMatchRef*        CMMatchRef;
/* Abstract data type for ColorWorld reference */
typedef struct OpaqueCMWorldRef*        CMWorldRef;
/* Caller-supplied flatten function */
typedef CALLBACK_API( OSErr , CMFlattenProcPtr )(long command, long *size, void *data, void *refCon);
/* Caller-supplied progress function for Bitmap & PixMap matching routines */
typedef CALLBACK_API( Boolean , CMBitmapCallBackProcPtr )(long progress, void *refCon);
/* Caller-supplied progress function for NCMMConcatInit & NCMMNewLinkProfile routines */
typedef CALLBACK_API( Boolean , CMConcatCallBackProcPtr )(long progress, void *refCon);
/* Caller-supplied filter function for Profile search */
typedef CALLBACK_API( Boolean , CMProfileFilterProcPtr )(CMProfileRef prof, void *refCon);
/* Caller-supplied function for profile access */
typedef CALLBACK_API( OSErr , CMProfileAccessProcPtr )(long command, long offset, long *size, void *data, void *refCon);
typedef STACK_UPP_TYPE(CMFlattenProcPtr)                        CMFlattenUPP;
typedef STACK_UPP_TYPE(CMBitmapCallBackProcPtr)                 CMBitmapCallBackUPP;
typedef STACK_UPP_TYPE(CMConcatCallBackProcPtr)                 CMConcatCallBackUPP;
typedef STACK_UPP_TYPE(CMProfileFilterProcPtr)                  CMProfileFilterUPP;
typedef STACK_UPP_TYPE(CMProfileAccessProcPtr)                  CMProfileAccessUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(CMFlattenUPP)
    NewCMFlattenUPP                (CMFlattenProcPtr        userRoutine);

    EXTERN_API(CMBitmapCallBackUPP)
    NewCMBitmapCallBackUPP         (CMBitmapCallBackProcPtr userRoutine);

    EXTERN_API(CMConcatCallBackUPP)
    NewCMConcatCallBackUPP         (CMConcatCallBackProcPtr userRoutine);

    EXTERN_API(CMProfileFilterUPP)
    NewCMProfileFilterUPP          (CMProfileFilterProcPtr  userRoutine);

    EXTERN_API(CMProfileAccessUPP)
    NewCMProfileAccessUPP          (CMProfileAccessProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeCMFlattenUPP            (CMFlattenUPP            userUPP);

    EXTERN_API(void)
    DisposeCMBitmapCallBackUPP     (CMBitmapCallBackUPP     userUPP);

    EXTERN_API(void)
    DisposeCMConcatCallBackUPP     (CMConcatCallBackUPP     userUPP);

    EXTERN_API(void)
    DisposeCMProfileFilterUPP      (CMProfileFilterUPP      userUPP);

    EXTERN_API(void)
    DisposeCMProfileAccessUPP      (CMProfileAccessUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeCMFlattenUPP             (long                    command,
                                    long *                  size,
                                    void *                  data,
                                    void *                  refCon,
                                    CMFlattenUPP            userUPP);

    EXTERN_API(Boolean)
    InvokeCMBitmapCallBackUPP      (long                    progress,
                                    void *                  refCon,
                                    CMBitmapCallBackUPP     userUPP);

    EXTERN_API(Boolean)
    InvokeCMConcatCallBackUPP      (long                    progress,
                                    void *                  refCon,
                                    CMConcatCallBackUPP     userUPP);

    EXTERN_API(Boolean)
    InvokeCMProfileFilterUPP       (CMProfileRef            prof,
                                    void *                  refCon,
                                    CMProfileFilterUPP      userUPP);

    EXTERN_API(OSErr)
    InvokeCMProfileAccessUPP       (long                    command,
                                    long                    offset,
                                    long *                  size,
                                    void *                  data,
                                    void *                  refCon,
                                    CMProfileAccessUPP      userUPP);

#else
    enum { uppCMFlattenProcInfo = 0x00003FE0 };                     /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppCMBitmapCallBackProcInfo = 0x000003D0 };              /* pascal 1_byte Func(4_bytes, 4_bytes) */
    enum { uppCMConcatCallBackProcInfo = 0x000003D0 };              /* pascal 1_byte Func(4_bytes, 4_bytes) */
    enum { uppCMProfileFilterProcInfo = 0x000003D0 };               /* pascal 1_byte Func(4_bytes, 4_bytes) */
    enum { uppCMProfileAccessProcInfo = 0x0000FFE0 };               /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewCMFlattenUPP(userRoutine)                            (CMFlattenUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCMFlattenProcInfo, GetCurrentArchitecture())
    #define NewCMBitmapCallBackUPP(userRoutine)                     (CMBitmapCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCMBitmapCallBackProcInfo, GetCurrentArchitecture())
    #define NewCMConcatCallBackUPP(userRoutine)                     (CMConcatCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCMConcatCallBackProcInfo, GetCurrentArchitecture())
    #define NewCMProfileFilterUPP(userRoutine)                      (CMProfileFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCMProfileFilterProcInfo, GetCurrentArchitecture())
    #define NewCMProfileAccessUPP(userRoutine)                      (CMProfileAccessUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCMProfileAccessProcInfo, GetCurrentArchitecture())
    #define DisposeCMFlattenUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define DisposeCMBitmapCallBackUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeCMConcatCallBackUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeCMProfileFilterUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeCMProfileAccessUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeCMFlattenUPP(command, size, data, refCon, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppCMFlattenProcInfo, (command), (size), (data), (refCon))
    #define InvokeCMBitmapCallBackUPP(progress, refCon, userUPP)    (Boolean)CALL_TWO_PARAMETER_UPP((userUPP), uppCMBitmapCallBackProcInfo, (progress), (refCon))
    #define InvokeCMConcatCallBackUPP(progress, refCon, userUPP)    (Boolean)CALL_TWO_PARAMETER_UPP((userUPP), uppCMConcatCallBackProcInfo, (progress), (refCon))
    #define InvokeCMProfileFilterUPP(prof, refCon, userUPP)         (Boolean)CALL_TWO_PARAMETER_UPP((userUPP), uppCMProfileFilterProcInfo, (prof), (refCon))
    #define InvokeCMProfileAccessUPP(command, offset, size, data, refCon, userUPP)  (OSErr)CALL_FIVE_PARAMETER_UPP((userUPP), uppCMProfileAccessProcInfo, (command), (offset), (size), (data), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewCMFlattenProc(userRoutine)                           NewCMFlattenUPP(userRoutine)
#define NewCMBitmapCallBackProc(userRoutine)                    NewCMBitmapCallBackUPP(userRoutine)
#define NewCMConcatCallBackProc(userRoutine)                    NewCMConcatCallBackUPP(userRoutine)
#define NewCMProfileFilterProc(userRoutine)                     NewCMProfileFilterUPP(userRoutine)
#define NewCMProfileAccessProc(userRoutine)                     NewCMProfileAccessUPP(userRoutine)
#define CallCMFlattenProc(userRoutine, command, size, data, refCon) InvokeCMFlattenUPP(command, size, data, refCon, userRoutine)
#define CallCMBitmapCallBackProc(userRoutine, progress, refCon) InvokeCMBitmapCallBackUPP(progress, refCon, userRoutine)
#define CallCMConcatCallBackProc(userRoutine, progress, refCon) InvokeCMConcatCallBackUPP(progress, refCon, userRoutine)
#define CallCMProfileFilterProc(userRoutine, prof, refCon)      InvokeCMProfileFilterUPP(prof, refCon, userRoutine)
#define CallCMProfileAccessProc(userRoutine, command, offset, size, data, refCon) InvokeCMProfileAccessUPP(command, offset, size, data, refCon, userRoutine)
typedef long                            CMError;
/* For 1.0 and 2.0 profile header variants */
/* CMAppleProfileHeader */

union CMAppleProfileHeader {
    CMHeader                        cm1;
    CM2Header                       cm2;
};
typedef union CMAppleProfileHeader      CMAppleProfileHeader;
/* Param for CWConcatColorWorld(...) */

struct CMConcatProfileSet {
    UInt16                          keyIndex;                   /* Zero-based */
    UInt16                          count;                      /* Min 1 */
    CMProfileRef                    profileSet[1];              /* Variable. Ordered from Source -> Dest */
};
typedef struct CMConcatProfileSet       CMConcatProfileSet;

/* NCMConcatProfileSpec Tags */
enum {
    kNoTransform                = 0,                            /* Not used */
    kUseAtoB                    = 1,                            /* Use 'A2B*' tag from this profile or equivalent */
    kUseBtoA                    = 2,                            /* Use 'B2A*' tag from this profile or equivalent */
    kUseBtoB                    = 3,                            /* Use 'pre*' tag from this profile or equivalent */
                                                                /* For typical device profiles the following synonyms may be useful */
    kDeviceToPCS                = kUseAtoB,                     /* Device Dependent to Device Independent */
    kPCSToDevice                = kUseBtoA,                     /* Device Independent to Device Dependent */
    kPCSToPCS                   = kUseBtoB,                     /* Independent, through device's gamut */
    kUseProfileIntent           = (long)0xFFFFFFFF              /* For renderingIntent in NCMConcatProfileSpec    */
};

/* NCMConcatProfileSpec */

struct NCMConcatProfileSpec {
    UInt32                          renderingIntent;            /* renderingIntent override     */
    UInt32                          transformTag;               /* transformTag, defined above    */
    CMProfileRef                    profile;                    /* profile */
};
typedef struct NCMConcatProfileSpec     NCMConcatProfileSpec;
/* NCMConcatProfileSet */

struct NCMConcatProfileSet {
    OSType                          cmm;                        /* e.g. 'KCMS', 'appl', ...  uniquely ids the cmm, or 0000 */
    UInt32                          flags;                      /* specify quality, lookup only, no gamut checking ... */
    UInt32                          flagsMask;                  /* which bits of 'flags' to use to override profile */
    UInt32                          profileCount;               /* how many ProfileSpecs in the following set */
    NCMConcatProfileSpec            profileSpecs[1];            /* Variable. Ordered from Source -> Dest */
};
typedef struct NCMConcatProfileSet      NCMConcatProfileSet;
/* ColorSync color data types */

struct CMRGBColor {
    UInt16                          red;                        /* 0..65535 */
    UInt16                          green;
    UInt16                          blue;
};
typedef struct CMRGBColor               CMRGBColor;

struct CMCMYKColor {
    UInt16                          cyan;                       /* 0..65535 */
    UInt16                          magenta;
    UInt16                          yellow;
    UInt16                          black;
};
typedef struct CMCMYKColor              CMCMYKColor;

struct CMCMYColor {
    UInt16                          cyan;                       /* 0..65535 */
    UInt16                          magenta;
    UInt16                          yellow;
};
typedef struct CMCMYColor               CMCMYColor;

struct CMHLSColor {
    UInt16                          hue;                        /* 0..65535. Fraction of circle. Red at 0 */
    UInt16                          lightness;                  /* 0..65535 */
    UInt16                          saturation;                 /* 0..65535 */
};
typedef struct CMHLSColor               CMHLSColor;

struct CMHSVColor {
    UInt16                          hue;                        /* 0..65535. Fraction of circle. Red at 0 */
    UInt16                          saturation;                 /* 0..65535 */
    UInt16                          value;                      /* 0..65535 */
};
typedef struct CMHSVColor               CMHSVColor;

struct CMLabColor {
    UInt16                          L;                          /* 0..65535 maps to 0..100 */
    UInt16                          a;                          /* 0..65535 maps to -128..127.996 */
    UInt16                          b;                          /* 0..65535 maps to -128..127.996 */
};
typedef struct CMLabColor               CMLabColor;

struct CMLuvColor {
    UInt16                          L;                          /* 0..65535 maps to 0..100 */
    UInt16                          u;                          /* 0..65535 maps to -128..127.996 */
    UInt16                          v;                          /* 0..65535 maps to -128..127.996 */
};
typedef struct CMLuvColor               CMLuvColor;

struct CMYxyColor {
    UInt16                          capY;                       /* 0..65535 maps to 0..1 */
    UInt16                          x;                          /* 0..65535 maps to 0..1 */
    UInt16                          y;                          /* 0..65535 maps to 0..1 */
};
typedef struct CMYxyColor               CMYxyColor;

struct CMGrayColor {
    UInt16                          gray;                       /* 0..65535 */
};
typedef struct CMGrayColor              CMGrayColor;

struct CMMultichannel5Color {
    UInt8                           components[5];              /* 0..255 */
};
typedef struct CMMultichannel5Color     CMMultichannel5Color;

struct CMMultichannel6Color {
    UInt8                           components[6];              /* 0..255 */
};
typedef struct CMMultichannel6Color     CMMultichannel6Color;

struct CMMultichannel7Color {
    UInt8                           components[7];              /* 0..255 */
};
typedef struct CMMultichannel7Color     CMMultichannel7Color;

struct CMMultichannel8Color {
    UInt8                           components[8];              /* 0..255 */
};
typedef struct CMMultichannel8Color     CMMultichannel8Color;

struct CMNamedColor {
    UInt32                          namedColorIndex;            /* 0..a lot */
};
typedef struct CMNamedColor             CMNamedColor;

union CMColor {
    CMRGBColor                      rgb;
    CMHSVColor                      hsv;
    CMHLSColor                      hls;
    CMXYZColor                      XYZ;
    CMLabColor                      Lab;
    CMLuvColor                      Luv;
    CMYxyColor                      Yxy;
    CMCMYKColor                     cmyk;
    CMCMYColor                      cmy;
    CMGrayColor                     gray;
    CMMultichannel5Color            mc5;
    CMMultichannel6Color            mc6;
    CMMultichannel7Color            mc7;
    CMMultichannel8Color            mc8;
    CMNamedColor                    namedColor;
};
typedef union CMColor                   CMColor;

struct CMProfileSearchRecord {
    CMHeader                        header;
    UInt32                          fieldMask;
    UInt32                          reserved[2];
};
typedef struct CMProfileSearchRecord    CMProfileSearchRecord;
typedef CMProfileSearchRecord *         CMProfileSearchRecordPtr;
typedef CMProfileSearchRecordPtr *      CMProfileSearchRecordHandle;
/* Search definition for 2.0 */

struct CMSearchRecord {
    OSType                          CMMType;
    OSType                          profileClass;
    OSType                          dataColorSpace;
    OSType                          profileConnectionSpace;
    UInt32                          deviceManufacturer;
    UInt32                          deviceModel;
    UInt32                          deviceAttributes[2];
    UInt32                          profileFlags;
    UInt32                          searchMask;
    CMProfileFilterUPP              filter;
};
typedef struct CMSearchRecord           CMSearchRecord;
/* CMMInfo structure */

struct CMMInfo {
    UInt32                          dataSize;                   /* Size of this structure - compatibility*/
    OSType                          CMMType;                    /* Signature, e.g. 'KCMS'*/
    OSType                          CMMMfr;                     /* Vendor, e.g. 'appl'*/
    UInt32                          CMMVersion;                 /* cmm version number*/
    unsigned char                   ASCIIName[32];              /* pascal string - name*/
    unsigned char                   ASCIIDesc[256];             /* pascal string - description or copyright*/
    UniCharCount                    UniCodeNameCount;           /* count of UniChars in following array*/
    UniChar                         UniCodeName[32];            /* the name in UniCode chars*/
    UniCharCount                    UniCodeDescCount;           /* count of UniChars in following array*/
    UniChar                         UniCodeDesc[256];           /* the description in UniCode chars*/
};
typedef struct CMMInfo                  CMMInfo;
/* GetCWInfo structures */

struct CMMInfoRecord {
    OSType                          CMMType;
    long                            CMMVersion;
};
typedef struct CMMInfoRecord            CMMInfoRecord;

struct CMCWInfoRecord {
    UInt32                          cmmCount;
    CMMInfoRecord                   cmmInfo[2];
};
typedef struct CMCWInfoRecord           CMCWInfoRecord;
/* profile identifier structures */

struct CMProfileIdentifier {
    CM2Header                       profileHeader;
    CMDateTime                      calibrationDate;
    UInt32                          ASCIIProfileDescriptionLen;
    char                            ASCIIProfileDescription[1]; /* variable length */
};
typedef struct CMProfileIdentifier      CMProfileIdentifier;
typedef CMProfileIdentifier *           CMProfileIdentifierPtr;
/* packing formats */
enum {
    cmNoColorPacking            = 0x0000,
    cmWord5ColorPacking         = 0x0500,
    cmWord565ColorPacking       = 0x0600,
    cmLong8ColorPacking         = 0x0800,
    cmLong10ColorPacking        = 0x0A00,
    cmAlphaFirstPacking         = 0x1000,
    cmOneBitDirectPacking       = 0x0B00,
    cmAlphaLastPacking          = 0x0000,
    cm8_8ColorPacking           = 0x2800,
    cm16_8ColorPacking          = 0x2000,
    cm24_8ColorPacking          = 0x2100,
    cm32_8ColorPacking          = cmLong8ColorPacking,
    cm40_8ColorPacking          = 0x2200,
    cm48_8ColorPacking          = 0x2300,
    cm56_8ColorPacking          = 0x2400,
    cm64_8ColorPacking          = 0x2500,
    cm32_16ColorPacking         = 0x2600,
    cm48_16ColorPacking         = 0x2900,
    cm64_16ColorPacking         = 0x2A00,
    cm32_32ColorPacking         = 0x2700,
    cmLittleEndianPacking       = 0x4000,
    cmReverseChannelPacking     = 0x8000
};

/* colorspace masks */
enum {
    cmColorSpaceSpaceMask       = 0x0000007F,
    cmColorSpaceAlphaMask       = 0x00000080,
    cmColorSpaceSpaceAndAlphaMask = 0x000000FF,
    cmColorSpacePackingMask     = 0x0000FF00,
    cmColorSpaceReservedMask    = (long)0xFFFF0000
};

/* general colorspaces */
enum {
    cmNoSpace                   = 0,
    cmRGBSpace                  = 1,
    cmCMYKSpace                 = 2,
    cmHSVSpace                  = 3,
    cmHLSSpace                  = 4,
    cmYXYSpace                  = 5,
    cmXYZSpace                  = 6,
    cmLUVSpace                  = 7,
    cmLABSpace                  = 8,
    cmReservedSpace1            = 9,
    cmGraySpace                 = 10,
    cmReservedSpace2            = 11,
    cmGamutResultSpace          = 12,
    cmNamedIndexedSpace         = 16,
    cmMCFiveSpace               = 17,
    cmMCSixSpace                = 18,
    cmMCSevenSpace              = 19,
    cmMCEightSpace              = 20,
    cmAlphaSpace                = 0x80,
    cmRGBASpace                 = cmRGBSpace + cmAlphaSpace,
    cmGrayASpace                = cmGraySpace + cmAlphaSpace
};

/* supported CMBitmapColorSpaces - Each of the following is a */
/* combination of a general colospace and a packing formats. */
/* Each can also be or'd with cmReverseChannelPacking. */
enum {
    cmGray8Space                = cmGraySpace + cm8_8ColorPacking,
    cmGrayA16Space              = cmGrayASpace + cm16_8ColorPacking,
    cmGray16Space               = cmGraySpace,
    cmGrayA32Space              = cmGrayASpace,
    cmGray16LSpace              = cmGraySpace + cmLittleEndianPacking,
    cmGrayA32LSpace             = cmGrayASpace + cmLittleEndianPacking,
    cmRGB16Space                = cmRGBSpace + cmWord5ColorPacking,
    cmRGB16LSpace               = cmRGBSpace + cmWord5ColorPacking + cmLittleEndianPacking,
    cmRGB565Space               = cmRGBSpace + cmWord565ColorPacking,
    cmRGB565LSpace              = cmRGBSpace + cmWord565ColorPacking + cmLittleEndianPacking,
    cmRGB24Space                = cmRGBSpace + cm24_8ColorPacking,
    cmRGB32Space                = cmRGBSpace + cm32_8ColorPacking,
    cmRGB48Space                = cmRGBSpace + cm48_16ColorPacking,
    cmRGB48LSpace               = cmRGBSpace + cm48_16ColorPacking + cmLittleEndianPacking,
    cmARGB32Space               = cmRGBASpace + cm32_8ColorPacking + cmAlphaFirstPacking,
    cmARGB64Space               = cmRGBASpace + cm64_16ColorPacking + cmAlphaFirstPacking,
    cmARGB64LSpace              = cmRGBASpace + cm64_16ColorPacking + cmAlphaFirstPacking + cmLittleEndianPacking,
    cmRGBA32Space               = cmRGBASpace + cm32_8ColorPacking + cmAlphaLastPacking,
    cmRGBA64Space               = cmRGBASpace + cm64_16ColorPacking + cmAlphaLastPacking,
    cmRGBA64LSpace              = cmRGBASpace + cm64_16ColorPacking + cmAlphaLastPacking + cmLittleEndianPacking,
    cmCMYK32Space               = cmCMYKSpace + cm32_8ColorPacking,
    cmCMYK64Space               = cmCMYKSpace + cm64_16ColorPacking,
    cmCMYK64LSpace              = cmCMYKSpace + cm64_16ColorPacking + cmLittleEndianPacking,
    cmHSV32Space                = cmHSVSpace + cmLong10ColorPacking,
    cmHLS32Space                = cmHLSSpace + cmLong10ColorPacking,
    cmYXY32Space                = cmYXYSpace + cmLong10ColorPacking,
    cmXYZ24Space                = cmXYZSpace + cm24_8ColorPacking,
    cmXYZ32Space                = cmXYZSpace + cmLong10ColorPacking,
    cmXYZ48Space                = cmXYZSpace + cm48_16ColorPacking,
    cmXYZ48LSpace               = cmXYZSpace + cm48_16ColorPacking + cmLittleEndianPacking,
    cmLUV32Space                = cmLUVSpace + cmLong10ColorPacking,
    cmLAB24Space                = cmLABSpace + cm24_8ColorPacking,
    cmLAB32Space                = cmLABSpace + cmLong10ColorPacking,
    cmLAB48Space                = cmLABSpace + cm48_16ColorPacking,
    cmLAB48LSpace               = cmLABSpace + cm48_16ColorPacking + cmLittleEndianPacking,
    cmGamutResult1Space         = cmOneBitDirectPacking + cmGamutResultSpace,
    cmNamedIndexed32Space       = cm32_32ColorPacking + cmNamedIndexedSpace,
    cmNamedIndexed32LSpace      = cm32_32ColorPacking + cmNamedIndexedSpace + cmLittleEndianPacking,
    cmMCFive8Space              = cm40_8ColorPacking + cmMCFiveSpace,
    cmMCSix8Space               = cm48_8ColorPacking + cmMCSixSpace,
    cmMCSeven8Space             = cm56_8ColorPacking + cmMCSevenSpace,
    cmMCEight8Space             = cm64_8ColorPacking + cmMCEightSpace
};



typedef UInt32                          CMBitmapColorSpace;

struct CMBitmap {
    char *                          image;
    long                            width;
    long                            height;
    long                            rowBytes;
    long                            pixelSize;
    CMBitmapColorSpace              space;
    long                            user1;
    long                            user2;
};
typedef struct CMBitmap                 CMBitmap;

/* Classic Print Manager Stuff */
#if TARGET_OS_MAC
enum {
    enableColorMatchingOp       = 12,
    registerProfileOp           = 13
};

#endif  /* TARGET_OS_MAC */

/* Profile Locations */
enum {
    CS_MAX_PATH                 = 256
};

enum {
    cmNoProfileBase             = 0,
    cmFileBasedProfile          = 1,
    cmHandleBasedProfile        = 2,
    cmPtrBasedProfile           = 3,
    cmProcedureBasedProfile     = 4,
    cmPathBasedProfile          = 5,
    cmBufferBasedProfile        = 6
};


struct CMFileLocation {
    FSSpec                          spec;
};
typedef struct CMFileLocation           CMFileLocation;

struct CMHandleLocation {
    Handle                          h;
};
typedef struct CMHandleLocation         CMHandleLocation;

struct CMPtrLocation {
    Ptr                             p;
};
typedef struct CMPtrLocation            CMPtrLocation;

struct CMProcedureLocation {
    CMProfileAccessUPP              proc;
    void *                          refCon;
};
typedef struct CMProcedureLocation      CMProcedureLocation;

struct CMPathLocation {
    char                            path[256];
};
typedef struct CMPathLocation           CMPathLocation;

struct CMBufferLocation {
    void *                          buffer;
    UInt32                          size;
};
typedef struct CMBufferLocation         CMBufferLocation;

union CMProfLoc {
    CMFileLocation                  fileLoc;
    CMHandleLocation                handleLoc;
    CMPtrLocation                   ptrLoc;
    CMProcedureLocation             procLoc;
    CMPathLocation                  pathLoc;
    CMBufferLocation                bufferLoc;
};
typedef union CMProfLoc                 CMProfLoc;

struct CMProfileLocation {
    short                           locType;
    CMProfLoc                       u;
};
typedef struct CMProfileLocation        CMProfileLocation;
#if TARGET_OS_MAC
enum {
    cmOriginalProfileLocationSize = 72,
    cmCurrentProfileLocationSize = 2 + CS_MAX_PATH
};

#else
enum {
    cmOriginalProfileLocationSize = 2 + CS_MAX_PATH,
    cmCurrentProfileLocationSize = 2 + CS_MAX_PATH
};

#endif  /* TARGET_OS_MAC */

/* Struct and enums used for Profile iteration */
enum {
    cmProfileIterateDataVersion1 = 0x00010000,
    cmProfileIterateDataVersion2 = 0x00020000
};


struct CMProfileIterateData {
    UInt32                          dataVersion;                /* cmProfileIterateDataVersion2 */
    CM2Header                       header;
    ScriptCode                      code;
    Str255                          name;
    CMProfileLocation               location;
    UniCharCount                    uniCodeNameCount;
    UniChar *                       uniCodeName;
    unsigned char *                 asciiName;
    CMMakeAndModel *                makeAndModel;
};
typedef struct CMProfileIterateData     CMProfileIterateData;
/* Caller-supplied callback function for Profile & CMM iteration */
typedef CALLBACK_API( OSErr , CMProfileIterateProcPtr )(CMProfileIterateData *iterateData, void *refCon);
typedef CALLBACK_API( OSErr , CMMIterateProcPtr )(CMMInfo *iterateData, void *refCon);
typedef STACK_UPP_TYPE(CMProfileIterateProcPtr)                 CMProfileIterateUPP;
typedef STACK_UPP_TYPE(CMMIterateProcPtr)                       CMMIterateUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(CMProfileIterateUPP)
    NewCMProfileIterateUPP         (CMProfileIterateProcPtr userRoutine);

    EXTERN_API(CMMIterateUPP)
    NewCMMIterateUPP               (CMMIterateProcPtr       userRoutine);

    EXTERN_API(void)
    DisposeCMProfileIterateUPP     (CMProfileIterateUPP     userUPP);

    EXTERN_API(void)
    DisposeCMMIterateUPP           (CMMIterateUPP           userUPP);

    EXTERN_API(OSErr)
    InvokeCMProfileIterateUPP      (CMProfileIterateData *  iterateData,
                                    void *                  refCon,
                                    CMProfileIterateUPP     userUPP);

    EXTERN_API(OSErr)
    InvokeCMMIterateUPP            (CMMInfo *               iterateData,
                                    void *                  refCon,
                                    CMMIterateUPP           userUPP);

#else
    enum { uppCMProfileIterateProcInfo = 0x000003E0 };              /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    enum { uppCMMIterateProcInfo = 0x000003E0 };                    /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    #define NewCMProfileIterateUPP(userRoutine)                     (CMProfileIterateUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCMProfileIterateProcInfo, GetCurrentArchitecture())
    #define NewCMMIterateUPP(userRoutine)                           (CMMIterateUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCMMIterateProcInfo, GetCurrentArchitecture())
    #define DisposeCMProfileIterateUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeCMMIterateUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define InvokeCMProfileIterateUPP(iterateData, refCon, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppCMProfileIterateProcInfo, (iterateData), (refCon))
    #define InvokeCMMIterateUPP(iterateData, refCon, userUPP)       (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppCMMIterateProcInfo, (iterateData), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewCMProfileIterateProc(userRoutine)                    NewCMProfileIterateUPP(userRoutine)
#define NewCMMIterateProc(userRoutine)                          NewCMMIterateUPP(userRoutine)
#define CallCMProfileIterateProc(userRoutine, iterateData, refCon) InvokeCMProfileIterateUPP(iterateData, refCon, userRoutine)
#define CallCMMIterateProc(userRoutine, iterateData, refCon)    InvokeCMMIterateUPP(iterateData, refCon, userRoutine)
/* Profile file and element access */
EXTERN_API( CMError )
CMNewProfile                    (CMProfileRef *         prof,
                                 const CMProfileLocation * theProfile)                      FOURWORDINLINE(0x203C, 0x0008, 0x001B, 0xABEE);

EXTERN_API( CMError )
CMOpenProfile                   (CMProfileRef *         prof,
                                 const CMProfileLocation * theProfile)                      FOURWORDINLINE(0x203C, 0x0008, 0x001C, 0xABEE);

EXTERN_API( CMError )
CMCloseProfile                  (CMProfileRef           prof)                               FOURWORDINLINE(0x203C, 0x0004, 0x001D, 0xABEE);

EXTERN_API( CMError )
CMUpdateProfile                 (CMProfileRef           prof)                               FOURWORDINLINE(0x203C, 0x0004, 0x0034, 0xABEE);

EXTERN_API( CMError )
CMCopyProfile                   (CMProfileRef *         targetProf,
                                 const CMProfileLocation * targetLocation,
                                 CMProfileRef           srcProf)                            FOURWORDINLINE(0x203C, 0x000C, 0x0025, 0xABEE);

EXTERN_API( CMError )
CMValidateProfile               (CMProfileRef           prof,
                                 Boolean *              valid,
                                 Boolean *              preferredCMMnotfound)               FOURWORDINLINE(0x203C, 0x000C, 0x0026, 0xABEE);

EXTERN_API( CMError )
CMGetProfileLocation            (CMProfileRef           prof,
                                 CMProfileLocation *    theProfile)                         FOURWORDINLINE(0x203C, 0x0008, 0x003C, 0xABEE);

EXTERN_API( CMError )
NCMGetProfileLocation           (CMProfileRef           prof,
                                 CMProfileLocation *    theProfile,
                                 UInt32 *               locationSize)                       FOURWORDINLINE(0x203C, 0x000C, 0x0059, 0xABEE);

EXTERN_API( CMError )
CMFlattenProfile                (CMProfileRef           prof,
                                 UInt32                 flags,
                                 CMFlattenUPP           proc,
                                 void *                 refCon,
                                 Boolean *              preferredCMMnotfound)               FOURWORDINLINE(0x203C, 0x0014, 0x0031, 0xABEE);

#if TARGET_OS_MAC
EXTERN_API( CMError )
CMUnflattenProfile              (FSSpec *               resultFileSpec,
                                 CMFlattenUPP           proc,
                                 void *                 refCon,
                                 Boolean *              preferredCMMnotfound)               FOURWORDINLINE(0x203C, 0x0010, 0x0032, 0xABEE);

#endif  /* TARGET_OS_MAC */

EXTERN_API( CMError )
CMGetProfileHeader              (CMProfileRef           prof,
                                 CMAppleProfileHeader * header)                             FOURWORDINLINE(0x203C, 0x0008, 0x0039, 0xABEE);

EXTERN_API( CMError )
CMSetProfileHeader              (CMProfileRef           prof,
                                 const CMAppleProfileHeader * header)                       FOURWORDINLINE(0x203C, 0x0008, 0x003A, 0xABEE);

EXTERN_API( CMError )
CMProfileElementExists          (CMProfileRef           prof,
                                 OSType                 tag,
                                 Boolean *              found)                              FOURWORDINLINE(0x203C, 0x000C, 0x001E, 0xABEE);

EXTERN_API( CMError )
CMCountProfileElements          (CMProfileRef           prof,
                                 UInt32 *               elementCount)                       FOURWORDINLINE(0x203C, 0x0008, 0x001F, 0xABEE);

EXTERN_API( CMError )
CMGetProfileElement             (CMProfileRef           prof,
                                 OSType                 tag,
                                 UInt32 *               elementSize,
                                 void *                 elementData)                        FOURWORDINLINE(0x203C, 0x0010, 0x0020, 0xABEE);

EXTERN_API( CMError )
CMSetProfileElement             (CMProfileRef           prof,
                                 OSType                 tag,
                                 UInt32                 elementSize,
                                 void *                 elementData)                        FOURWORDINLINE(0x203C, 0x0010, 0x0023, 0xABEE);

EXTERN_API( CMError )
CMSetProfileElementSize         (CMProfileRef           prof,
                                 OSType                 tag,
                                 UInt32                 elementSize)                        FOURWORDINLINE(0x203C, 0x000C, 0x0038, 0xABEE);

EXTERN_API( CMError )
CMSetProfileElementReference    (CMProfileRef           prof,
                                 OSType                 elementTag,
                                 OSType                 referenceTag)                       FOURWORDINLINE(0x203C, 0x000C, 0x0035, 0xABEE);

EXTERN_API( CMError )
CMGetPartialProfileElement      (CMProfileRef           prof,
                                 OSType                 tag,
                                 UInt32                 offset,
                                 UInt32 *               byteCount,
                                 void *                 elementData)                        FOURWORDINLINE(0x203C, 0x0014, 0x0036, 0xABEE);

EXTERN_API( CMError )
CMSetPartialProfileElement      (CMProfileRef           prof,
                                 OSType                 tag,
                                 UInt32                 offset,
                                 UInt32                 byteCount,
                                 void *                 elementData)                        FOURWORDINLINE(0x203C, 0x0014, 0x0037, 0xABEE);

EXTERN_API( CMError )
CMGetIndProfileElementInfo      (CMProfileRef           prof,
                                 UInt32                 index,
                                 OSType *               tag,
                                 UInt32 *               elementSize,
                                 Boolean *              refs)                               FOURWORDINLINE(0x203C, 0x0014, 0x0021, 0xABEE);

EXTERN_API( CMError )
CMGetIndProfileElement          (CMProfileRef           prof,
                                 UInt32                 index,
                                 UInt32 *               elementSize,
                                 void *                 elementData)                        FOURWORDINLINE(0x203C, 0x0010, 0x0022, 0xABEE);

EXTERN_API( CMError )
CMRemoveProfileElement          (CMProfileRef           prof,
                                 OSType                 tag)                                FOURWORDINLINE(0x203C, 0x0008, 0x0024, 0xABEE);

EXTERN_API( CMError )
CMGetScriptProfileDescription   (CMProfileRef           prof,
                                 Str255                 name,
                                 ScriptCode *           code)                               FOURWORDINLINE(0x203C, 0x000C, 0x003E, 0xABEE);

EXTERN_API( CMError )
CMGetProfileDescriptions        (CMProfileRef           prof,
                                 char *                 aName,
                                 UInt32 *               aCount,
                                 Str255                 mName,
                                 ScriptCode *           mCode,
                                 UniChar *              uName,
                                 UniCharCount *         uCount)                             FOURWORDINLINE(0x203C, 0x001A, 0x0067, 0xABEE);

EXTERN_API( CMError )
CMSetProfileDescriptions        (CMProfileRef           prof,
                                 const char *           aName,
                                 UInt32                 aCount,
                                 ConstStr255Param       mName,
                                 ScriptCode             mCode,
                                 const UniChar *        uName,
                                 UniCharCount           uCount)                             FOURWORDINLINE(0x203C, 0x001A, 0x0068, 0xABEE);

EXTERN_API( CMError )
CMCloneProfileRef               (CMProfileRef           prof)                               FOURWORDINLINE(0x203C, 0x0004, 0x0042, 0xABEE);

EXTERN_API( CMError )
CMGetProfileRefCount            (CMProfileRef           prof,
                                 long *                 count)                              FOURWORDINLINE(0x203C, 0x0008, 0x0043, 0xABEE);

EXTERN_API( CMError )
CMProfileModified               (CMProfileRef           prof,
                                 Boolean *              modified)                           FOURWORDINLINE(0x203C, 0x0008, 0x0044, 0xABEE);


/* named Color access functions */
EXTERN_API( CMError )
CMGetNamedColorInfo             (CMProfileRef           prof,
                                 UInt32 *               deviceChannels,
                                 OSType *               deviceColorSpace,
                                 OSType *               PCSColorSpace,
                                 UInt32 *               count,
                                 StringPtr              prefix,
                                 StringPtr              suffix)                             FOURWORDINLINE(0x203C, 0x001C, 0x0046, 0xABEE);

EXTERN_API( CMError )
CMGetNamedColorValue            (CMProfileRef           prof,
                                 StringPtr              name,
                                 CMColor *              deviceColor,
                                 CMColor *              PCSColor)                           FOURWORDINLINE(0x203C, 0x0010, 0x0047, 0xABEE);

EXTERN_API( CMError )
CMGetIndNamedColorValue         (CMProfileRef           prof,
                                 UInt32                 index,
                                 CMColor *              deviceColor,
                                 CMColor *              PCSColor)                           FOURWORDINLINE(0x203C, 0x0010, 0x0048, 0xABEE);

EXTERN_API( CMError )
CMGetNamedColorIndex            (CMProfileRef           prof,
                                 StringPtr              name,
                                 UInt32 *               index)                              FOURWORDINLINE(0x203C, 0x000C, 0x0049, 0xABEE);

EXTERN_API( CMError )
CMGetNamedColorName             (CMProfileRef           prof,
                                 UInt32                 index,
                                 StringPtr              name)                               FOURWORDINLINE(0x203C, 0x000C, 0x004A, 0xABEE);


/* General-purpose matching functions */
EXTERN_API( CMError )
NCWNewColorWorld                (CMWorldRef *           cw,
                                 CMProfileRef           src,
                                 CMProfileRef           dst)                                FOURWORDINLINE(0x203C, 0x000C, 0x0014, 0xABEE);

EXTERN_API( CMError )
CWConcatColorWorld              (CMWorldRef *           cw,
                                 CMConcatProfileSet *   profileSet)                         FOURWORDINLINE(0x203C, 0x0008, 0x0015, 0xABEE);

EXTERN_API( CMError )
CWNewLinkProfile                (CMProfileRef *         prof,
                                 const CMProfileLocation * targetLocation,
                                 CMConcatProfileSet *   profileSet)                         FOURWORDINLINE(0x203C, 0x000C, 0x0033, 0xABEE);

EXTERN_API( CMError )
NCWConcatColorWorld             (CMWorldRef *           cw,
                                 NCMConcatProfileSet *  profileSet,
                                 CMConcatCallBackUPP    proc,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x0008, 0x0061, 0xABEE);

EXTERN_API( CMError )
NCWNewLinkProfile               (CMProfileRef *         prof,
                                 const CMProfileLocation * targetLocation,
                                 NCMConcatProfileSet *  profileSet,
                                 CMConcatCallBackUPP    proc,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x000C, 0x0062, 0xABEE);

EXTERN_API( void )
CWDisposeColorWorld             (CMWorldRef             cw)                                 FOURWORDINLINE(0x203C, 0x0004, 0x0001, 0xABEE);

EXTERN_API( CMError )
CWMatchColors                   (CMWorldRef             cw,
                                 CMColor *              myColors,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0002, 0xABEE);

EXTERN_API( CMError )
CWCheckColors                   (CMWorldRef             cw,
                                 CMColor *              myColors,
                                 UInt32                 count,
                                 UInt32 *               result)                             FOURWORDINLINE(0x203C, 0x0010, 0x0003, 0xABEE);

EXTERN_API( CMError )
CWMatchBitmap                   (CMWorldRef             cw,
                                 CMBitmap *             bitmap,
                                 CMBitmapCallBackUPP    progressProc,
                                 void *                 refCon,
                                 CMBitmap *             matchedBitmap)                      FOURWORDINLINE(0x203C, 0x0010, 0x002C, 0xABEE);

EXTERN_API( CMError )
CWCheckBitmap                   (CMWorldRef             cw,
                                 const CMBitmap *       bitmap,
                                 CMBitmapCallBackUPP    progressProc,
                                 void *                 refCon,
                                 CMBitmap *             resultBitmap)                       FOURWORDINLINE(0x203C, 0x0014, 0x002D, 0xABEE);


/* Quickdraw-specific matching */
EXTERN_API( CMError )
CWMatchPixMap                   (CMWorldRef             cw,
                                 PixMap *               myPixMap,
                                 CMBitmapCallBackUPP    progressProc,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x0010, 0x0004, 0xABEE);

EXTERN_API( CMError )
CWCheckPixMap                   (CMWorldRef             cw,
                                 PixMap *               myPixMap,
                                 CMBitmapCallBackUPP    progressProc,
                                 void *                 refCon,
                                 BitMap *               resultBitMap)                       FOURWORDINLINE(0x203C, 0x0014, 0x0007, 0xABEE);

#if TARGET_API_MAC_OS8 || TARGET_API_MAC_CARBON
EXTERN_API( CMError )
NCMBeginMatching                (CMProfileRef           src,
                                 CMProfileRef           dst,
                                 CMMatchRef *           myRef)                              FOURWORDINLINE(0x203C, 0x000C, 0x0016, 0xABEE);

EXTERN_API( void )
CMEndMatching                   (CMMatchRef             myRef)                              FOURWORDINLINE(0x203C, 0x0004, 0x000B, 0xABEE);

EXTERN_API( void )
NCMDrawMatchedPicture           (PicHandle              myPicture,
                                 CMProfileRef           dst,
                                 Rect *                 myRect)                             FOURWORDINLINE(0x203C, 0x000C, 0x0017, 0xABEE);

EXTERN_API( void )
CMEnableMatchingComment         (Boolean                enableIt)                           FOURWORDINLINE(0x203C, 0x0002, 0x000D, 0xABEE);

EXTERN_API( CMError )
NCMUseProfileComment            (CMProfileRef           prof,
                                 UInt32                 flags)                              FOURWORDINLINE(0x203C, 0x0008, 0x003B, 0xABEE);

#endif  /* TARGET_API_MAC_OS8 || TARGET_API_MAC_CARBON */

#if TARGET_OS_WIN32
#if CALL_NOT_IN_CARBON
EXTERN_API( CMError )
CWMatchHBITMAP                  (CMWorldRef             cw,
                                 HBITMAP                hBitmap,
                                 CMBitmapCallBackUPP    progressProc,
                                 void *                 refCon);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_WIN32 */

EXTERN_API( CMError )
CMCreateProfileIdentifier       (CMProfileRef           prof,
                                 CMProfileIdentifierPtr  ident,
                                 UInt32 *               size)                               FOURWORDINLINE(0x203C, 0x000C, 0x0041, 0xABEE);


/* System Profile access */
EXTERN_API( CMError )
CMGetSystemProfile              (CMProfileRef *         prof)                               FOURWORDINLINE(0x203C, 0x0004, 0x0018, 0xABEE);

EXTERN_API( CMError )
CMSetSystemProfile              (const FSSpec *         profileFileSpec)                    FOURWORDINLINE(0x203C, 0x0004, 0x0019, 0xABEE);

EXTERN_API( CMError )
NCMSetSystemProfile             (const CMProfileLocation * profLoc)                         FOURWORDINLINE(0x203C, 0x0004, 0x0064, 0xABEE);

EXTERN_API( CMError )
CMGetDefaultProfileBySpace      (OSType                 dataColorSpace,
                                 CMProfileRef *         prof)                               FOURWORDINLINE(0x203C, 0x0008, 0x005A, 0xABEE);

EXTERN_API( CMError )
CMSetDefaultProfileBySpace      (OSType                 dataColorSpace,
                                 CMProfileRef           prof)                               FOURWORDINLINE(0x203C, 0x0008, 0x005B, 0xABEE);

#if TARGET_OS_MAC
EXTERN_API( CMError )
CMGetProfileByAVID              (AVIDType               theAVID,
                                 CMProfileRef *         prof)                               FOURWORDINLINE(0x203C, 0x0008, 0x005C, 0xABEE);

EXTERN_API( CMError )
CMSetProfileByAVID              (AVIDType               theAVID,
                                 CMProfileRef           prof)                               FOURWORDINLINE(0x203C, 0x0008, 0x005D, 0xABEE);

#endif  /* TARGET_OS_MAC */

/* Profile Use enumerations */
enum {
    cmInputUse                  = FOUR_CHAR_CODE('inpt'),
    cmOutputUse                 = FOUR_CHAR_CODE('outp'),
    cmDisplayUse                = FOUR_CHAR_CODE('dply'),
    cmProofUse                  = FOUR_CHAR_CODE('pruf')
};

/* Profile access by Use */
EXTERN_API( CMError )
CMGetDefaultProfileByUse        (OSType                 use,
                                 CMProfileRef *         prof)                               FOURWORDINLINE(0x203C, 0x0008, 0x0069, 0xABEE);

EXTERN_API( CMError )
CMSetDefaultProfileByUse        (OSType                 use,
                                 CMProfileRef           prof)                               FOURWORDINLINE(0x203C, 0x0008, 0x0079, 0xABEE);

/* Profile Management */
EXTERN_API( CMError )
CMNewProfileSearch              (CMSearchRecord *       searchSpec,
                                 void *                 refCon,
                                 UInt32 *               count,
                                 CMProfileSearchRef *   searchResult)                       FOURWORDINLINE(0x203C, 0x0010, 0x0027, 0xABEE);

EXTERN_API( CMError )
CMUpdateProfileSearch           (CMProfileSearchRef     search,
                                 void *                 refCon,
                                 UInt32 *               count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0028, 0xABEE);

EXTERN_API( void )
CMDisposeProfileSearch          (CMProfileSearchRef     search)                             FOURWORDINLINE(0x203C, 0x0004, 0x0029, 0xABEE);

EXTERN_API( CMError )
CMSearchGetIndProfile           (CMProfileSearchRef     search,
                                 UInt32                 index,
                                 CMProfileRef *         prof)                               FOURWORDINLINE(0x203C, 0x000C, 0x002A, 0xABEE);

EXTERN_API( CMError )
CMSearchGetIndProfileFileSpec   (CMProfileSearchRef     search,
                                 UInt32                 index,
                                 FSSpec *               profileFile)                        FOURWORDINLINE(0x203C, 0x000C, 0x002B, 0xABEE);

EXTERN_API( CMError )
CMProfileIdentifierFolderSearch (CMProfileIdentifierPtr  ident,
                                 UInt32 *               matchedCount,
                                 CMProfileSearchRef *   searchResult)                       FOURWORDINLINE(0x203C, 0x000C, 0x003F, 0xABEE);

EXTERN_API( CMError )
CMProfileIdentifierListSearch   (CMProfileIdentifierPtr  ident,
                                 CMProfileRef *         profileList,
                                 UInt32                 listSize,
                                 UInt32 *               matchedCount,
                                 CMProfileRef *         matchedList)                        FOURWORDINLINE(0x203C, 0x0014, 0x0040, 0xABEE);

EXTERN_API( CMError )
CMIterateColorSyncFolder        (CMProfileIterateUPP    proc,
                                 UInt32 *               seed,
                                 UInt32 *               count,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x0010, 0x0058, 0xABEE);

EXTERN_API( CMError )
NCMUnflattenProfile             (CMProfileLocation *    targetLocation,
                                 CMFlattenUPP           proc,
                                 void *                 refCon,
                                 Boolean *              preferredCMMnotfound)               FOURWORDINLINE(0x203C, 0x0010, 0x0065, 0xABEE);

/* Utilities */
#if TARGET_OS_MAC
EXTERN_API( CMError )
CMGetColorSyncFolderSpec        (short                  vRefNum,
                                 Boolean                createFolder,
                                 short *                foundVRefNum,
                                 long *                 foundDirID)                         FOURWORDINLINE(0x203C, 0x000C, 0x0011, 0xABEE);

#endif  /* TARGET_OS_MAC */

#if TARGET_OS_WIN32 || TARGET_OS_UNIX
#if CALL_NOT_IN_CARBON
EXTERN_API( CMError )
CMGetColorSyncFolderPath        (Boolean                createFolder,
                                 char *                 lpBuffer,
                                 UInt32                 uSize);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_WIN32 || TARGET_OS_UNIX */

EXTERN_API( CMError )
CMGetCWInfo                     (CMWorldRef             cw,
                                 CMCWInfoRecord *       info)                               FOURWORDINLINE(0x203C, 0x0008, 0x001A, 0xABEE);

#if TARGET_API_MAC_OS8
#if CALL_NOT_IN_CARBON
EXTERN_API( CMError )
CMConvertProfile2to1            (CMProfileRef           profv2,
                                 CMProfileHandle *      profv1)                             FOURWORDINLINE(0x203C, 0x0008, 0x0045, 0xABEE);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_API_MAC_OS8 */

EXTERN_API( CMError )
CMGetPreferredCMM               (OSType *               cmmType,
                                 Boolean *              preferredCMMnotfound)               FOURWORDINLINE(0x203C, 0x0008, 0x005E, 0xABEE);

EXTERN_API( CMError )
CMIterateCMMInfo                (CMMIterateUPP          proc,
                                 UInt32 *               count,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x000C, 0x0063, 0xABEE);

EXTERN_API( CMError )
CMGetColorSyncVersion           (UInt32 *               version)                            FOURWORDINLINE(0x203C, 0x0004, 0x0066, 0xABEE);

EXTERN_API( CMError )
CMLaunchControlPanel            (UInt32                 flags);

/* ColorSpace conversion functions */
EXTERN_API( CMError )
CMConvertXYZToLab               (const CMColor *        src,
                                 const CMXYZColor *     white,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x0010, 0x004B, 0xABEE);

EXTERN_API( CMError )
CMConvertLabToXYZ               (const CMColor *        src,
                                 const CMXYZColor *     white,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x0010, 0x004C, 0xABEE);

EXTERN_API( CMError )
CMConvertXYZToLuv               (const CMColor *        src,
                                 const CMXYZColor *     white,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x0010, 0x004D, 0xABEE);

EXTERN_API( CMError )
CMConvertLuvToXYZ               (const CMColor *        src,
                                 const CMXYZColor *     white,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x0010, 0x004E, 0xABEE);

EXTERN_API( CMError )
CMConvertXYZToYxy               (const CMColor *        src,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x004F, 0xABEE);

EXTERN_API( CMError )
CMConvertYxyToXYZ               (const CMColor *        src,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0050, 0xABEE);

EXTERN_API( CMError )
CMConvertRGBToHLS               (const CMColor *        src,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0051, 0xABEE);

EXTERN_API( CMError )
CMConvertHLSToRGB               (const CMColor *        src,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0052, 0xABEE);

EXTERN_API( CMError )
CMConvertRGBToHSV               (const CMColor *        src,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0053, 0xABEE);

EXTERN_API( CMError )
CMConvertHSVToRGB               (const CMColor *        src,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0054, 0xABEE);

EXTERN_API( CMError )
CMConvertRGBToGray              (const CMColor *        src,
                                 CMColor *              dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0055, 0xABEE);

EXTERN_API( CMError )
CMConvertXYZToFixedXYZ          (const CMXYZColor *     src,
                                 CMFixedXYZColor *      dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0056, 0xABEE);

EXTERN_API( CMError )
CMConvertFixedXYZToXYZ          (const CMFixedXYZColor * src,
                                 CMXYZColor *           dst,
                                 UInt32                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x0057, 0xABEE);


/* PS-related */
EXTERN_API( CMError )
CMGetPS2ColorSpace              (CMProfileRef           srcProf,
                                 UInt32                 flags,
                                 CMFlattenUPP           proc,
                                 void *                 refCon,
                                 Boolean *              preferredCMMnotfound)               FOURWORDINLINE(0x203C, 0x0014, 0x002E, 0xABEE);

EXTERN_API( CMError )
CMGetPS2ColorRenderingIntent    (CMProfileRef           srcProf,
                                 UInt32                 flags,
                                 CMFlattenUPP           proc,
                                 void *                 refCon,
                                 Boolean *              preferredCMMnotfound)               FOURWORDINLINE(0x203C, 0x0014, 0x002F, 0xABEE);

EXTERN_API( CMError )
CMGetPS2ColorRendering          (CMProfileRef           srcProf,
                                 CMProfileRef           dstProf,
                                 UInt32                 flags,
                                 CMFlattenUPP           proc,
                                 void *                 refCon,
                                 Boolean *              preferredCMMnotfound)               FOURWORDINLINE(0x203C, 0x0018, 0x0030, 0xABEE);

EXTERN_API( CMError )
CMGetPS2ColorRenderingVMSize    (CMProfileRef           srcProf,
                                 CMProfileRef           dstProf,
                                 UInt32 *               vmSize,
                                 Boolean *              preferredCMMnotfound)               FOURWORDINLINE(0x203C, 0x0010, 0x003D, 0xABEE);


/* ColorSync 1.0 functions which have parallel 2.0 counterparts */
#if TARGET_API_MAC_OS8
#if CALL_NOT_IN_CARBON
EXTERN_API( CMError )
CWNewColorWorld                 (CMWorldRef *           cw,
                                 CMProfileHandle        src,
                                 CMProfileHandle        dst)                                FOURWORDINLINE(0x203C, 0x000C, 0x0000, 0xABEE);

EXTERN_API( CMError )
ConcatenateProfiles             (CMProfileHandle        thru,
                                 CMProfileHandle        dst,
                                 CMProfileHandle *      newDst)                             FOURWORDINLINE(0x203C, 0x000C, 0x000C, 0xABEE);

EXTERN_API( CMError )
CMBeginMatching                 (CMProfileHandle        src,
                                 CMProfileHandle        dst,
                                 CMMatchRef *           myRef)                              FOURWORDINLINE(0x203C, 0x000C, 0x000A, 0xABEE);

EXTERN_API( void )
CMDrawMatchedPicture            (PicHandle              myPicture,
                                 CMProfileHandle        dst,
                                 Rect *                 myRect)                             FOURWORDINLINE(0x203C, 0x000C, 0x0009, 0xABEE);

EXTERN_API( CMError )
CMUseProfileComment             (CMProfileHandle        profile)                            FOURWORDINLINE(0x203C, 0x0004, 0x0008, 0xABEE);

EXTERN_API( void )
CMGetProfileName                (CMProfileHandle        myProfile,
                                 CMIString *            IStringResult)                      FOURWORDINLINE(0x203C, 0x0008, 0x000E, 0xABEE);

EXTERN_API( long )
CMGetProfileAdditionalDataOffset (CMProfileHandle       myProfile)                          FOURWORDINLINE(0x203C, 0x0004, 0x000F, 0xABEE);


/* ProfileResponder functions */
EXTERN_API( CMError )
GetProfile                      (OSType                 deviceType,
                                 long                   refNum,
                                 CMProfileHandle        aProfile,
                                 CMProfileHandle *      returnedProfile)                    FOURWORDINLINE(0x203C, 0x0010, 0x0005, 0xABEE);

EXTERN_API( CMError )
SetProfile                      (OSType                 deviceType,
                                 long                   refNum,
                                 CMProfileHandle        newProfile)                         FOURWORDINLINE(0x203C, 0x000C, 0x0006, 0xABEE);

EXTERN_API( CMError )
SetProfileDescription           (OSType                 deviceType,
                                 long                   refNum,
                                 long                   deviceData,
                                 CMProfileHandle        hProfile)                           FOURWORDINLINE(0x203C, 0x0010, 0x0010, 0xABEE);

EXTERN_API( CMError )
GetIndexedProfile               (OSType                 deviceType,
                                 long                   refNum,
                                 CMProfileSearchRecordHandle  search,
                                 CMProfileHandle *      returnProfile,
                                 long *                 index)                              FOURWORDINLINE(0x203C, 0x0014, 0x0012, 0xABEE);

EXTERN_API( CMError )
DeleteDeviceProfile             (OSType                 deviceType,
                                 long                   refNum,
                                 CMProfileHandle        deleteMe)                           FOURWORDINLINE(0x203C, 0x000C, 0x0013, 0xABEE);


#endif  /* CALL_NOT_IN_CARBON */

#if OLDROUTINENAMES
/* constants */

typedef CMFlattenProcPtr                CMFlattenProc;
typedef CMBitmapCallBackProcPtr         CMBitmapCallBackProc;
typedef CMProfileFilterProcPtr          CMProfileFilterProc;
enum {
    CMTrap                      = cmTrap,
    CMBeginProfile              = cmBeginProfile,
    CMEndProfile                = cmEndProfile,
    CMEnableMatching            = cmEnableMatching,
    CMDisableMatching           = cmDisableMatching
};

/* 1.0 Error codes, for compatibility with older applications. 1.0 CMM's may return obsolete error codes */
enum {
    CMNoError                   = 0,                            /*    obsolete name, use noErr */
    CMProfileError              = cmProfileError,
    CMMethodError               = cmMethodError,
    CMMemFullError              = -172,                         /*    obsolete, 2.0 uses memFullErr */
    CMUnimplementedError        = -173,                         /*    obsolete, 2.0 uses unimpErr */
    CMParamError                = -174,                         /*    obsolete, 2.0 uses paramErr */
    CMMethodNotFound            = cmMethodNotFound,
    CMProfileNotFound           = cmProfileNotFound,
    CMProfilesIdentical         = cmProfilesIdentical,
    CMCantConcatenateError      = cmCantConcatenateError,
    CMCantXYZ                   = cmCantXYZ,
    CMCantDeleteProfile         = cmCantDeleteProfile,
    CMUnsupportedDataType       = cmUnsupportedDataType,
    CMNoCurrentProfile          = cmNoCurrentProfile
};

enum {
    qdSystemDevice              = cmSystemDevice,
    qdGDevice                   = cmGDevice
};


enum {
    kMatchCMMType               = cmMatchCMMType,
    kMatchApplProfileVersion    = cmMatchApplProfileVersion,
    kMatchDataType              = cmMatchDataType,
    kMatchDeviceType            = cmMatchDeviceType,
    kMatchDeviceManufacturer    = cmMatchDeviceManufacturer,
    kMatchDeviceModel           = cmMatchDeviceModel,
    kMatchDeviceAttributes      = cmMatchDeviceAttributes,
    kMatchFlags                 = cmMatchFlags,
    kMatchOptions               = cmMatchOptions,
    kMatchWhite                 = cmMatchWhite,
    kMatchBlack                 = cmMatchBlack
};

/* types */
typedef CMCMYKColor                     CMYKColor;
typedef CMWorldRef                      CWorld;
typedef long *                          CMGamutResult;
/* functions */
#define EndMatching(myRef)                                                      CMEndMatching(myRef)
#define EnableMatching(enableIt)                                                CMEnableMatchingComment(enableIt)
#define GetColorSyncFolderSpec(vRefNum, createFolder, foundVRefNum, foundDirID) CMGetColorSyncFolderSpec(vRefNum, createFolder, foundVRefNum, foundDirID)
#define BeginMatching(src, dst, myRef)                                          CMBeginMatching(src, dst, myRef)
#define DrawMatchedPicture(myPicture, dst, myRect)                              CMDrawMatchedPicture(myPicture, dst, myRect)
#define UseProfile(profile)                                                     CMUseProfileComment(profile)
#define GetProfileName(myProfile, IStringResult)                                CMGetProfileName(myProfile, IStringResult)
#define GetProfileAdditionalDataOffset(myProfile)                               CMGetProfileAdditionalDataOffset(myProfile)
#endif  /* OLDROUTINENAMES */

/* Deprecated stuff*/

/* PrGeneral parameter blocks */

struct TEnableColorMatchingBlk {
    short                           iOpCode;
    short                           iError;
    long                            lReserved;
    THPrint                         hPrint;
    Boolean                         fEnableIt;
    SInt8                           filler;
};
typedef struct TEnableColorMatchingBlk  TEnableColorMatchingBlk;

struct TRegisterProfileBlk {
    short                           iOpCode;
    short                           iError;
    long                            lReserved;
    THPrint                         hPrint;
    Boolean                         fRegisterIt;
    SInt8                           filler;
};
typedef struct TRegisterProfileBlk      TRegisterProfileBlk;
#endif  /* TARGET_API_MAC_OS8 */


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

#endif /* __CMAPPLICATION__ */

