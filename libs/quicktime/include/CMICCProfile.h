/*
     File:       CMICCProfile.h
 
     Contains:   ICC Profile Format Definitions
 
     Version:    Technology: ColorSync 2.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CMICCPROFILE__
#define __CMICCPROFILE__

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

/* ICC Profile version constants  */
enum {
    cmICCProfileVersion2        = 0x02000000,
    cmICCProfileVersion21       = 0x02100000,
    cmCS2ProfileVersion         = cmICCProfileVersion2,
    cmCS1ProfileVersion         = 0x00000100                    /* ColorSync 1.0 profile version */
};

/* Current Major version number */
enum {
    cmProfileMajorVersionMask   = (long)0xFF000000,
    cmCurrentProfileMajorVersion = 0x02000000
};

/* magic cookie number for anonymous file ID */
enum {
    cmMagicNumber               = FOUR_CHAR_CODE('acsp')
};


/************************************************************************/
/*************** ColorSync 2.0 profile specification ********************/
/************************************************************************/
/**** flags field  ****/
enum {
    cmICCReservedFlagsMask      = 0x0000FFFF,                   /* these bits of the flags field are defined and reserved by ICC */
    cmEmbeddedMask              = 0x00000001,                   /* if bit 0 is 0 then not embedded profile, if 1 then embedded profile */
    cmEmbeddedUseMask           = 0x00000002,                   /* if bit 1 is 0 then ok to use anywhere, if 1 then ok to use as embedded profile only */
    cmCMSReservedFlagsMask      = (long)0xFFFF0000,             /* these bits of the flags field are defined and reserved by CMS vendor */
    cmQualityMask               = 0x00030000,                   /* if bits 16-17 is 0 then normal, if 1 then draft, if 2 then best */
    cmInterpolationMask         = 0x00040000,                   /* if bit 18 is 0 then interpolation, if 1 then lookup only */
    cmGamutCheckingMask         = 0x00080000                    /* if bit 19 is 0 then create gamut checking info, if 1 then no gamut checking info */
};

/* copyright-protection flag options */
enum {
    cmEmbeddedProfile           = 0,                            /* 0 is not embedded profile, 1 is embedded profile */
    cmEmbeddedUse               = 1                             /* 0 is to use anywhere, 1 is to use as embedded profile only */
};

/* speed and quality flag options */
enum {
    cmNormalMode                = 0,                            /* it uses the least significent two bits in the high word of flag */
    cmDraftMode                 = 1,                            /* it should be evaulated like this: right shift 16 bits first, mask off the */
    cmBestMode                  = 2                             /* high 14 bits, and then compare with the enum to determine the option value */
};


/**** deviceAttributes fields ****/
/* deviceAttributes[0] is defined by and reserved for device vendors */
/* deviceAttributes[1] is defined by and reserved for ICC */
/* The following bits of deviceAttributes[1] are currently defined */
enum {
    cmReflectiveTransparentMask = 0x00000001,                   /* if bit 0 is 0 then reflective media, if 1 then transparency media */
    cmGlossyMatteMask           = 0x00000002                    /* if bit 1 is 0 then glossy, if 1 then matte */
};

/* device/media attributes element values  */
enum {
    cmReflective                = 0,                            /* if bit 0 is 0 then reflective media, if 1 then transparency media */
    cmGlossy                    = 1                             /* if bit 1 is 0 then glossy, if 1 then matte */
};


/**** renderingIntent field ****/
enum {
    cmPerceptual                = 0,                            /* Photographic images */
    cmRelativeColorimetric      = 1,                            /* Logo Colors */
    cmSaturation                = 2,                            /* Business graphics */
    cmAbsoluteColorimetric      = 3                             /* Logo Colors */
};



/* data type element values */
enum {
    cmAsciiData                 = 0,
    cmBinaryData                = 1
};

/* screen encodings  */
enum {
    cmPrtrDefaultScreens        = 0,                            /* Use printer default screens.  0 is false, 1 is ture */
    cmLinesPer                  = 1                             /* 0 is LinesPerCm, 1 is LinesPerInch */
};

/* 2.0 tag type information */
enum {
    cmNumHeaderElements         = 10
};

/* public tags */
enum {
    cmAToB0Tag                  = FOUR_CHAR_CODE('A2B0'),
    cmAToB1Tag                  = FOUR_CHAR_CODE('A2B1'),
    cmAToB2Tag                  = FOUR_CHAR_CODE('A2B2'),
    cmBlueColorantTag           = FOUR_CHAR_CODE('bXYZ'),
    cmBlueTRCTag                = FOUR_CHAR_CODE('bTRC'),
    cmBToA0Tag                  = FOUR_CHAR_CODE('B2A0'),
    cmBToA1Tag                  = FOUR_CHAR_CODE('B2A1'),
    cmBToA2Tag                  = FOUR_CHAR_CODE('B2A2'),
    cmCalibrationDateTimeTag    = FOUR_CHAR_CODE('calt'),
    cmCharTargetTag             = FOUR_CHAR_CODE('targ'),
    cmCopyrightTag              = FOUR_CHAR_CODE('cprt'),
    cmDeviceMfgDescTag          = FOUR_CHAR_CODE('dmnd'),
    cmDeviceModelDescTag        = FOUR_CHAR_CODE('dmdd'),
    cmGamutTag                  = FOUR_CHAR_CODE('gamt'),
    cmGrayTRCTag                = FOUR_CHAR_CODE('kTRC'),
    cmGreenColorantTag          = FOUR_CHAR_CODE('gXYZ'),
    cmGreenTRCTag               = FOUR_CHAR_CODE('gTRC'),
    cmLuminanceTag              = FOUR_CHAR_CODE('lumi'),
    cmMeasurementTag            = FOUR_CHAR_CODE('meas'),
    cmMediaBlackPointTag        = FOUR_CHAR_CODE('bkpt'),
    cmMediaWhitePointTag        = FOUR_CHAR_CODE('wtpt'),
    cmNamedColorTag             = FOUR_CHAR_CODE('ncol'),
    cmNamedColor2Tag            = FOUR_CHAR_CODE('ncl2'),
    cmPreview0Tag               = FOUR_CHAR_CODE('pre0'),
    cmPreview1Tag               = FOUR_CHAR_CODE('pre1'),
    cmPreview2Tag               = FOUR_CHAR_CODE('pre2'),
    cmProfileDescriptionTag     = FOUR_CHAR_CODE('desc'),
    cmProfileSequenceDescTag    = FOUR_CHAR_CODE('pseq'),
    cmPS2CRD0Tag                = FOUR_CHAR_CODE('psd0'),
    cmPS2CRD1Tag                = FOUR_CHAR_CODE('psd1'),
    cmPS2CRD2Tag                = FOUR_CHAR_CODE('psd2'),
    cmPS2CRD3Tag                = FOUR_CHAR_CODE('psd3'),
    cmPS2CSATag                 = FOUR_CHAR_CODE('ps2s'),
    cmPS2RenderingIntentTag     = FOUR_CHAR_CODE('ps2i'),
    cmRedColorantTag            = FOUR_CHAR_CODE('rXYZ'),
    cmRedTRCTag                 = FOUR_CHAR_CODE('rTRC'),
    cmScreeningDescTag          = FOUR_CHAR_CODE('scrd'),
    cmScreeningTag              = FOUR_CHAR_CODE('scrn'),
    cmTechnologyTag             = FOUR_CHAR_CODE('tech'),
    cmUcrBgTag                  = FOUR_CHAR_CODE('bfd '),
    cmViewingConditionsDescTag  = FOUR_CHAR_CODE('vued'),
    cmViewingConditionsTag      = FOUR_CHAR_CODE('view')
};

/* custom tags */
enum {
    cmPS2CRDVMSizeTag           = FOUR_CHAR_CODE('psvm'),
    cmVideoCardGammaTag         = FOUR_CHAR_CODE('vcgt'),
    cmMakeAndModelTag           = FOUR_CHAR_CODE('mmod')
};

/* technology tag descriptions */
enum {
    cmTechnologyFilmScanner     = FOUR_CHAR_CODE('fscn'),
    cmTechnologyReflectiveScanner = FOUR_CHAR_CODE('rscn'),
    cmTechnologyInkJetPrinter   = FOUR_CHAR_CODE('ijet'),
    cmTechnologyThermalWaxPrinter = FOUR_CHAR_CODE('twax'),
    cmTechnologyElectrophotographicPrinter = FOUR_CHAR_CODE('epho'),
    cmTechnologyElectrostaticPrinter = FOUR_CHAR_CODE('esta'),
    cmTechnologyDyeSublimationPrinter = FOUR_CHAR_CODE('dsub'),
    cmTechnologyPhotographicPaperPrinter = FOUR_CHAR_CODE('rpho'),
    cmTechnologyFilmWriter      = FOUR_CHAR_CODE('fprn'),
    cmTechnologyVideoMonitor    = FOUR_CHAR_CODE('vidm'),
    cmTechnologyVideoCamera     = FOUR_CHAR_CODE('vidc'),
    cmTechnologyProjectionTelevision = FOUR_CHAR_CODE('pjtv'),
    cmTechnologyCRTDisplay      = FOUR_CHAR_CODE('CRT '),
    cmTechnologyPMDisplay       = FOUR_CHAR_CODE('PMD '),
    cmTechnologyAMDisplay       = FOUR_CHAR_CODE('AMD '),
    cmTechnologyPhotoCD         = FOUR_CHAR_CODE('KPCD'),
    cmTechnologyPhotoImageSetter = FOUR_CHAR_CODE('imgs'),
    cmTechnologyGravure         = FOUR_CHAR_CODE('grav'),
    cmTechnologyOffsetLithography = FOUR_CHAR_CODE('offs'),
    cmTechnologySilkscreen      = FOUR_CHAR_CODE('silk'),
    cmTechnologyFlexography     = FOUR_CHAR_CODE('flex')
};

/* public type signatures */
enum {
    cmSigCurveType              = FOUR_CHAR_CODE('curv'),
    cmSigDataType               = FOUR_CHAR_CODE('data'),
    cmSigDateTimeType           = FOUR_CHAR_CODE('dtim'),
    cmSigLut16Type              = FOUR_CHAR_CODE('mft2'),
    cmSigLut8Type               = FOUR_CHAR_CODE('mft1'),
    cmSigMeasurementType        = FOUR_CHAR_CODE('meas'),
    cmSigNamedColorType         = FOUR_CHAR_CODE('ncol'),
    cmSigNamedColor2Type        = FOUR_CHAR_CODE('ncl2'),
    cmSigProfileDescriptionType = FOUR_CHAR_CODE('desc'),
    cmSigScreeningType          = FOUR_CHAR_CODE('scrn'),
    cmSigS15Fixed16Type         = FOUR_CHAR_CODE('sf32'),
    cmSigSignatureType          = FOUR_CHAR_CODE('sig '),
    cmSigTextType               = FOUR_CHAR_CODE('text'),
    cmSigU16Fixed16Type         = FOUR_CHAR_CODE('uf32'),
    cmSigU1Fixed15Type          = FOUR_CHAR_CODE('uf16'),
    cmSigUInt32Type             = FOUR_CHAR_CODE('ui32'),
    cmSigUInt64Type             = FOUR_CHAR_CODE('ui64'),
    cmSigUInt8Type              = FOUR_CHAR_CODE('ui08'),
    cmSigUnicodeTextType        = FOUR_CHAR_CODE('utxt'),
    cmSigViewingConditionsType  = FOUR_CHAR_CODE('view'),
    cmSigXYZType                = FOUR_CHAR_CODE('XYZ ')
};

/* custom type signatures */
enum {
    cmSigVideoCardGammaType     = FOUR_CHAR_CODE('vcgt'),
    cmSigMakeAndModelType       = FOUR_CHAR_CODE('mmod')
};


/* Measurement type encodings */
/* Measurement Flare */
enum {
    cmFlare0                    = 0x00000000,
    cmFlare100                  = 0x00000001
};

/* Measurement Geometry */
enum {
    cmGeometryUnknown           = 0x00000000,
    cmGeometry045or450          = 0x00000001,
    cmGeometry0dord0            = 0x00000002
};

/* Standard Observer    */
enum {
    cmStdobsUnknown             = 0x00000000,
    cmStdobs1931TwoDegrees      = 0x00000001,
    cmStdobs1964TenDegrees      = 0x00000002
};

/* Standard Illuminant */
enum {
    cmIlluminantUnknown         = 0x00000000,
    cmIlluminantD50             = 0x00000001,
    cmIlluminantD65             = 0x00000002,
    cmIlluminantD93             = 0x00000003,
    cmIlluminantF2              = 0x00000004,
    cmIlluminantD55             = 0x00000005,
    cmIlluminantA               = 0x00000006,
    cmIlluminantEquiPower       = 0x00000007,
    cmIlluminantF8              = 0x00000008
};

/* Spot Function Value */
enum {
    cmSpotFunctionUnknown       = 0,
    cmSpotFunctionDefault       = 1,
    cmSpotFunctionRound         = 2,
    cmSpotFunctionDiamond       = 3,
    cmSpotFunctionEllipse       = 4,
    cmSpotFunctionLine          = 5,
    cmSpotFunctionSquare        = 6,
    cmSpotFunctionCross         = 7
};

/* Color Space Signatures */
enum {
    cmXYZData                   = FOUR_CHAR_CODE('XYZ '),
    cmLabData                   = FOUR_CHAR_CODE('Lab '),
    cmLuvData                   = FOUR_CHAR_CODE('Luv '),
    cmYxyData                   = FOUR_CHAR_CODE('Yxy '),
    cmRGBData                   = FOUR_CHAR_CODE('RGB '),
    cmGrayData                  = FOUR_CHAR_CODE('GRAY'),
    cmHSVData                   = FOUR_CHAR_CODE('HSV '),
    cmHLSData                   = FOUR_CHAR_CODE('HLS '),
    cmCMYKData                  = FOUR_CHAR_CODE('CMYK'),
    cmCMYData                   = FOUR_CHAR_CODE('CMY '),
    cmMCH5Data                  = FOUR_CHAR_CODE('MCH5'),
    cmMCH6Data                  = FOUR_CHAR_CODE('MCH6'),
    cmMCH7Data                  = FOUR_CHAR_CODE('MCH7'),
    cmMCH8Data                  = FOUR_CHAR_CODE('MCH8'),
    cm3CLRData                  = FOUR_CHAR_CODE('3CLR'),
    cm4CLRData                  = FOUR_CHAR_CODE('4CLR'),
    cm5CLRData                  = FOUR_CHAR_CODE('5CLR'),
    cm6CLRData                  = FOUR_CHAR_CODE('6CLR'),
    cm7CLRData                  = FOUR_CHAR_CODE('7CLR'),
    cm8CLRData                  = FOUR_CHAR_CODE('8CLR'),
    cmNamedData                 = FOUR_CHAR_CODE('NAME')
};

/* profileClass enumerations */
enum {
    cmInputClass                = FOUR_CHAR_CODE('scnr'),
    cmDisplayClass              = FOUR_CHAR_CODE('mntr'),
    cmOutputClass               = FOUR_CHAR_CODE('prtr'),
    cmLinkClass                 = FOUR_CHAR_CODE('link'),
    cmAbstractClass             = FOUR_CHAR_CODE('abst'),
    cmColorSpaceClass           = FOUR_CHAR_CODE('spac'),
    cmNamedColorClass           = FOUR_CHAR_CODE('nmcl')
};

/* platform enumerations */
enum {
    cmMacintosh                 = FOUR_CHAR_CODE('APPL'),
    cmMicrosoft                 = FOUR_CHAR_CODE('MSFT'),
    cmSolaris                   = FOUR_CHAR_CODE('SUNW'),
    cmSiliconGraphics           = FOUR_CHAR_CODE('SGI '),
    cmTaligent                  = FOUR_CHAR_CODE('TGNT')
};

/* ColorSync 1.0 elements */
enum {
    cmCS1ChromTag               = FOUR_CHAR_CODE('chrm'),
    cmCS1TRCTag                 = FOUR_CHAR_CODE('trc '),
    cmCS1NameTag                = FOUR_CHAR_CODE('name'),
    cmCS1CustTag                = FOUR_CHAR_CODE('cust')
};

/* General element data types */

struct CMDateTime {
    unsigned short                  year;
    unsigned short                  month;
    unsigned short                  dayOfTheMonth;
    unsigned short                  hours;
    unsigned short                  minutes;
    unsigned short                  seconds;
};
typedef struct CMDateTime               CMDateTime;

struct CMFixedXYZColor {
    Fixed                           X;
    Fixed                           Y;
    Fixed                           Z;
};
typedef struct CMFixedXYZColor          CMFixedXYZColor;

typedef unsigned short                  CMXYZComponent;

struct CMXYZColor {
    CMXYZComponent                  X;
    CMXYZComponent                  Y;
    CMXYZComponent                  Z;
};
typedef struct CMXYZColor               CMXYZColor;

struct CM2Header {
    unsigned long                   size;                       /* This is the total size of the Profile */
    OSType                          CMMType;                    /* CMM signature,  Registered with CS2 consortium  */
    unsigned long                   profileVersion;             /* Version of CMProfile format */
    OSType                          profileClass;               /* input, display, output, devicelink, abstract, or color conversion profile type */
    OSType                          dataColorSpace;             /* color space of data */
    OSType                          profileConnectionSpace;     /* profile connection color space */
    CMDateTime                      dateTime;                   /* date and time of profile creation */
    OSType                          CS2profileSignature;        /* 'acsp' constant ColorSync 2.0 file ID */
    OSType                          platform;                   /* primary profile platform, Registered with CS2 consortium */
    unsigned long                   flags;                      /* profile flags */
    OSType                          deviceManufacturer;         /* Registered with ICC consortium */
    unsigned long                   deviceModel;                /* Registered with ICC consortium */
    unsigned long                   deviceAttributes[2];        /* Attributes[0] is for device vendors, [1] is for ICC */
    unsigned long                   renderingIntent;            /* preferred rendering intent of tagged object */
    CMFixedXYZColor                 white;                      /* profile illuminant */
    OSType                          creator;                    /* profile creator */
    char                            reserved[44];               /* reserved for future use */
};
typedef struct CM2Header                CM2Header;

struct CMTagRecord {
    OSType                          tag;                        /* Registered with CS2 consortium */
    unsigned long                   elementOffset;              /* Relative to start of CMProfile */
    unsigned long                   elementSize;
};
typedef struct CMTagRecord              CMTagRecord;

struct CMTagElemTable {
    unsigned long                   count;
    CMTagRecord                     tagList[1];                 /* Variable size */
};
typedef struct CMTagElemTable           CMTagElemTable;
/* External 0x02002001 CMProfile */

struct CM2Profile {
    CM2Header                       header;
    CMTagElemTable                  tagTable;
    char                            elemData[1];                /* Tagged element storage. Variable size */
};
typedef struct CM2Profile               CM2Profile;
typedef CM2Profile *                    CM2ProfilePtr;
typedef CM2ProfilePtr *                 CM2ProfileHandle;
/* Tag Type Definitions */

struct CMCurveType {
    OSType                          typeDescriptor;             /* 'curv' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   countValue;                 /* number of entries in table that follows */
    unsigned short                  data[1];                    /* Tagged element storage. Variable size */
};
typedef struct CMCurveType              CMCurveType;

struct CMDataType {
    OSType                          typeDescriptor;             /* 'data' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   dataFlag;                   /* 0 = ASCII, 1 = binary */
    char                            data[1];                    /* Tagged element storage. Variable size */
};
typedef struct CMDataType               CMDataType;

struct CMDateTimeType {
    OSType                          typeDescriptor;             /* 'dtim' */
    unsigned long                   reserved;
    CMDateTime                      dateTime;
};
typedef struct CMDateTimeType           CMDateTimeType;

struct CMLut16Type {
    OSType                          typeDescriptor;             /* 'mft2' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned char                   inputChannels;              /* Number of input channels */
    unsigned char                   outputChannels;             /* Number of output channels */
    unsigned char                   gridPoints;                 /* Number of clutTable grid points */
    unsigned char                   reserved2;                  /* fill with 0x00 */
    Fixed                           matrix[3][3];               /* */
    unsigned short                  inputTableEntries;          /* */
    unsigned short                  outputTableEntries;         /* */
    unsigned short                  inputTable[1];              /* Variable size */
    unsigned short                  CLUT[1];                    /* Variable size */
    unsigned short                  outputTable[1];             /* Variable size */
};
typedef struct CMLut16Type              CMLut16Type;

struct CMLut8Type {
    OSType                          typeDescriptor;             /* 'mft1' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned char                   inputChannels;              /* */
    unsigned char                   outputChannels;             /* */
    unsigned char                   gridPoints;                 /* */
    unsigned char                   reserved2;                  /* fill with 0x00 */
    Fixed                           matrix[3][3];               /* */
    unsigned char                   inputTable[256];            /* fixed size of 256 */
    unsigned char                   CLUT[2];                    /* Variable size */
    unsigned char                   outputTable[256];           /* fixed size of 256 */
};
typedef struct CMLut8Type               CMLut8Type;

struct CMMeasurementType {
    OSType                          typeDescriptor;             /* 'meas' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   standardObserver;           /* 0 : unknown, 1 : CIE 1931, 2 : CIE 1964 */
    CMFixedXYZColor                 backingXYZ;                 /* absolute XYZ values of backing */
    unsigned long                   geometry;                   /* 0 : unknown, 1 : 0/45 or 45/0, 2 :0/d or d/0 */
    unsigned long                   flare;                      /* 0 : 0%, 1 : 100% flare */
    unsigned long                   illuminant;                 /* standard illuminant */
};
typedef struct CMMeasurementType        CMMeasurementType;

struct CMNamedColorType {
    OSType                          typeDescriptor;             /* 'ncol' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   vendorFlag;                 /* */
    unsigned long                   count;                      /* count of named colors in array that follows */
    unsigned char                   prefixName[1];              /* Variable size, max = 32, to access fields after this one, have to count bytes */
    unsigned char                   suffixName[1];              /* Variable size, max = 32 */
    char                            data[1];                    /* varaible size data as explained below */
};
typedef struct CMNamedColorType         CMNamedColorType;
/*    
    A variable size array of structs appears as the last block of data
    in the above struct, CMNamedColorType.  The data structure
    is as follows: (example in C)
    
    struct {                                             
        unsigned char   rootName[1];                 * Variable size, max = 32 
        unsigned char   colorCoords[1];              * Variable size  
    } colorName[1];                                  * Variable size  
*/

struct CMNamedColor2Type {
    OSType                          typeDescriptor;             /* 'ncl2' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   vendorFlag;                 /* lower 16 bits reserved for ICC use */
    unsigned long                   count;                      /* count of named colors in array that follows */
    unsigned long                   deviceChannelCount;         /* number of device channels, 0 indicates no device value available */
    unsigned char                   prefixName[32];             /* 32 byte field.  7 bit ASCII null terminated */
    unsigned char                   suffixName[32];             /* 32 byte field.  7 bit ASCII null terminated */
    char                            data[1];                    /* varaible size data as definced below */
};
typedef struct CMNamedColor2Type        CMNamedColor2Type;

struct CMNamedColor2EntryType {
    unsigned char                   rootName[32];               /* 32 byte field.  7 bit ASCII null terminated */
    unsigned short                  PCSColorCoords[3];          /* Lab or XYZ color */
    unsigned short                  DeviceColorCoords[1];       /* Variable size */
};
typedef struct CMNamedColor2EntryType   CMNamedColor2EntryType;

struct CMTextDescriptionType {
    OSType                          typeDescriptor;             /* 'desc' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   ASCIICount;                 /* the count of "bytes" */
    unsigned char                   ASCIIName[2];               /* Variable size, to access fields after this one, have to count bytes */
    unsigned long                   UniCodeCode;
    unsigned long                   UniCodeCount;               /* the count of characters, each character has two bytes */
    unsigned char                   UniCodeName[2];             /* Variable size */
    short                           ScriptCodeCode;
    unsigned char                   ScriptCodeCount;            /* the count of "bytes" */
    unsigned char                   ScriptCodeName[2];          /* Variable size */
};
typedef struct CMTextDescriptionType    CMTextDescriptionType;

struct CMTextType {
    OSType                          typeDescriptor;             /* 'text' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned char                   text[1];                    /* count of text is obtained from tag size element */
};
typedef struct CMTextType               CMTextType;

struct CMUnicodeTextType {
    OSType                          typeDescriptor;             /* 'utxt' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned short                  text[1];                    /* count of text is obtained from tag size element */
};
typedef struct CMUnicodeTextType        CMUnicodeTextType;

struct CMScreeningType {
    OSType                          typeDescriptor;             /* 'scrn' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   screeningFlag;              /* bit 0 : use printer default screens, bit 1 : inch/cm */
    unsigned long                   channelCount;
    char                            data[1];                    /* varaible size data as explained below */
};
typedef struct CMScreeningType          CMScreeningType;
/*
    A variable size array of structs appears as the last block of data
    in the above struct, CMScreeningType.  The data structure
    is as follows: (example in C)
    
    struct {
        Fixed           frequency;
        Fixed           angle;
        unsigned long   sportFunction;
     }  channelScreening[1];                        * Variable size 
*/

struct CMSignatureType {
    OSType                          typeDescriptor;             /* 'sig ' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    OSType                          signature;
};
typedef struct CMSignatureType          CMSignatureType;

struct CMS15Fixed16ArrayType {
    OSType                          typeDescriptor;             /* 'sf32' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    Fixed                           value[1];                   /* Variable size */
};
typedef struct CMS15Fixed16ArrayType    CMS15Fixed16ArrayType;

struct CMU16Fixed16ArrayType {
    OSType                          typeDescriptor;             /* 'uf32' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   value[1];                   /* Variable size */
};
typedef struct CMU16Fixed16ArrayType    CMU16Fixed16ArrayType;

struct CMUInt16ArrayType {
    OSType                          typeDescriptor;             /* 'ui16' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned short                  value[1];                   /* Variable size */
};
typedef struct CMUInt16ArrayType        CMUInt16ArrayType;

struct CMUInt32ArrayType {
    OSType                          typeDescriptor;             /* 'ui32' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   value[1];                   /* Variable size */
};
typedef struct CMUInt32ArrayType        CMUInt32ArrayType;

struct CMUInt64ArrayType {
    OSType                          typeDescriptor;             /* 'ui64' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   value[1];                   /* Variable size (x2) */
};
typedef struct CMUInt64ArrayType        CMUInt64ArrayType;

struct CMUInt8ArrayType {
    OSType                          typeDescriptor;             /* 'ui08' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned char                   value[1];                   /* Variable size */
};
typedef struct CMUInt8ArrayType         CMUInt8ArrayType;

struct CMViewingConditionsType {
    OSType                          typeDescriptor;             /* 'view' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    CMFixedXYZColor                 illuminant;                 /* absolute XYZs of illuminant  in cd/m^2 */
    CMFixedXYZColor                 surround;                   /* absolute XYZs of surround in cd/m^2 */
    unsigned long                   stdIlluminant;              /* see definitions of std illuminants */
};
typedef struct CMViewingConditionsType  CMViewingConditionsType;

struct CMXYZType {
    OSType                          typeDescriptor;             /* 'XYZ ' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    CMFixedXYZColor                 XYZ[1];                     /* variable size XYZ tristimulus values */
};
typedef struct CMXYZType                CMXYZType;
/* Profile sequence description type */

struct CMProfileSequenceDescType {
    OSType                          typeDescriptor;             /* 'pseq ' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   count;                      /* Number of descriptions */
    char                            data[1];                    /* varaible size data as explained below */
};
typedef struct CMProfileSequenceDescType CMProfileSequenceDescType;
/*
    A variable size array of structs appears as the last block of data
    in the above struct, CMProfileSequenceDescType.  The data structure
    is as follows: (example in C)
    
    struct {                                             
        OSType          deviceMfg;                   * Device Manufacturer 
        OSType          deviceModel;                 * Decvice Model 
        unsigned long   attributes[2];               * Device attributes 
        OSType          technology;                  * Technology signature 
        unsigned long   mfgDescASCIICount;           * the count of "bytes" 
        unsigned char   mfgDescASCIIName[2];         * Variable size 
        unsigned long   mfgDescUniCodeCode;          
        unsigned long   mfgDescUniCodeCount;         * the count of characters, each character has two bytes 
        unsigned char   mfgDescUniCodeName[2];       * Variable size 
        unsigned long   mfgDescScriptCodeCode;       
        unsigned long   mfgDescScriptCodeCount;      * the count of "bytes" 
        unsigned char   mfgDescScriptCodeName[2];    * Variable size 
        unsigned long   modelDescASCIICount;         * the count of "bytes" 
        unsigned char   modelDescASCIIName[2];       * Variable size 
        unsigned long   modelDescUniCodeCode;        
        unsigned long   modelDescUniCodeCount;       * the count of characters, each character has two bytes 
        unsigned char   modelDescUniCodeName[2];     * Variable size 
        short           modelDescScriptCodeCode;     
        unsigned char   modelDescScriptCodeCount;    * the count of "bytes" 
        SInt8           filler;                      * For proper alignment across languages 
        unsigned char   modelDescScriptCodeName[2];  * Variable size 
    }   profileDescription[1];                       
*/

/* Under color removal, black generation type */

struct CMUcrBgType {
    OSType                          typeDescriptor;             /* 'bfd  ' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   ucrCount;                   /* Number of UCR entries */
    unsigned short                  ucrValues[1];               /* variable size */
    unsigned long                   bgCount;                    /* Number of BG entries */
    unsigned short                  bgValues[1];                /* variable size */
    unsigned char                   ucrbgASCII[1];              /* null terminated ASCII string */
};
typedef struct CMUcrBgType              CMUcrBgType;

struct CMIntentCRDVMSize {
    long                            renderingIntent;            /* rendering intent */
    unsigned long                   VMSize;                     /* VM size taken up by the CRD */
};
typedef struct CMIntentCRDVMSize        CMIntentCRDVMSize;

struct CMPS2CRDVMSizeType {
    OSType                          typeDescriptor;             /* 'psvm' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    unsigned long                   count;                      /* number of intent entries */
    CMIntentCRDVMSize               intentCRD[1];               /* variable size */
};
typedef struct CMPS2CRDVMSizeType       CMPS2CRDVMSizeType;
/* Video Card Gamma type */
enum {
    cmVideoCardGammaTableType   = 0,
    cmVideoCardGammaFormulaType = 1
};



struct CMVideoCardGammaTable {
    unsigned short                  channels;                   /* # of gamma channels (1 or 3) */
    unsigned short                  entryCount;                 /* 1-based number of entries per channel */
    unsigned short                  entrySize;                  /* size on bytes of each entry */
    char                            data[1];                    /* variable size data follows */
};
typedef struct CMVideoCardGammaTable    CMVideoCardGammaTable;

struct CMVideoCardGammaFormula {
    Fixed                           redGamma;                   /* must be > 0.0 */
    Fixed                           redMin;                     /* must be > 0.0 and < 1.0 */
    Fixed                           redMax;                     /* must be > 0.0 and < 1.0 */
    Fixed                           greenGamma;                 /* must be > 0.0 */
    Fixed                           greenMin;                   /* must be > 0.0 and < 1.0 */
    Fixed                           greenMax;                   /* must be > 0.0 and < 1.0 */
    Fixed                           blueGamma;                  /* must be > 0.0 */
    Fixed                           blueMin;                    /* must be > 0.0 and < 1.0 */
    Fixed                           blueMax;                    /* must be > 0.0 and < 1.0 */
};
typedef struct CMVideoCardGammaFormula  CMVideoCardGammaFormula;

struct CMVideoCardGamma {
    unsigned long                   tagType;
    union {
        CMVideoCardGammaTable           table;
        CMVideoCardGammaFormula         formula;
    }                                 u;
};
typedef struct CMVideoCardGamma         CMVideoCardGamma;

struct CMVideoCardGammaType {
    OSType                          typeDescriptor;             /* 'vcgt' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    CMVideoCardGamma                gamma;
};
typedef struct CMVideoCardGammaType     CMVideoCardGammaType;

struct CMMakeAndModel {
    OSType                          manufacturer;
    unsigned long                   model;
    unsigned long                   serialNumber;
    unsigned long                   manufactureDate;
    unsigned long                   reserved1;                  /* fill with 0x00 */
    unsigned long                   reserved2;                  /* fill with 0x00 */
    unsigned long                   reserved3;                  /* fill with 0x00 */
    unsigned long                   reserved4;                  /* fill with 0x00 */
};
typedef struct CMMakeAndModel           CMMakeAndModel;

struct CMMakeAndModelType {
    OSType                          typeDescriptor;             /* 'mmod' */
    unsigned long                   reserved;                   /* fill with 0x00 */
    CMMakeAndModel                  makeAndModel;
};
typedef struct CMMakeAndModelType       CMMakeAndModelType;
/************************************************************************/
/*************** ColorSync 1.0 profile specification ********************/
/************************************************************************/
enum {
    cmGrayResponse              = 0,
    cmRedResponse               = 1,
    cmGreenResponse             = 2,
    cmBlueResponse              = 3,
    cmCyanResponse              = 4,
    cmMagentaResponse           = 5,
    cmYellowResponse            = 6,
    cmUcrResponse               = 7,
    cmBgResponse                = 8,
    cmOnePlusLastResponse       = 9
};


/* Device types */
enum {
    cmMonitorDevice             = FOUR_CHAR_CODE('mntr'),
    cmScannerDevice             = FOUR_CHAR_CODE('scnr'),
    cmPrinterDevice             = FOUR_CHAR_CODE('prtr')
};



struct CMIString {
    ScriptCode                      theScript;
    Str63                           theString;
};
typedef struct CMIString                CMIString;
/* Profile options */
enum {
    cmPerceptualMatch           = 0x0000,                       /* Default. For photographic images */
    cmColorimetricMatch         = 0x0001,                       /* Exact matching when possible */
    cmSaturationMatch           = 0x0002                        /* For solid colors */
};

/* Profile flags */
enum {
    cmNativeMatchingPreferred   = 0x00000001,                   /* Default to native not preferred */
    cmTurnOffCache              = 0x00000002                    /* Default to turn on CMM cache */
};


typedef long                            CMMatchOption;
typedef long                            CMMatchFlag;

struct CMHeader {
    unsigned long                   size;
    OSType                          CMMType;
    unsigned long                   applProfileVersion;
    OSType                          dataType;
    OSType                          deviceType;
    OSType                          deviceManufacturer;
    unsigned long                   deviceModel;
    unsigned long                   deviceAttributes[2];
    unsigned long                   profileNameOffset;
    unsigned long                   customDataOffset;
    CMMatchFlag                     flags;
    CMMatchOption                   options;
    CMXYZColor                      white;
    CMXYZColor                      black;
};
typedef struct CMHeader                 CMHeader;

struct CMProfileChromaticities {
    CMXYZColor                      red;
    CMXYZColor                      green;
    CMXYZColor                      blue;
    CMXYZColor                      cyan;
    CMXYZColor                      magenta;
    CMXYZColor                      yellow;
};
typedef struct CMProfileChromaticities  CMProfileChromaticities;

struct CMProfileResponse {
    unsigned short                  counts[9];
    unsigned short                  data[1];                    /* Variable size */
};
typedef struct CMProfileResponse        CMProfileResponse;

struct CMProfile {
    CMHeader                        header;
    CMProfileChromaticities         profile;
    CMProfileResponse               response;
    CMIString                       profileName;
    char                            customData[1];              /* Variable size */
};
typedef struct CMProfile                CMProfile;
typedef CMProfile *                     CMProfilePtr;
typedef CMProfilePtr *                  CMProfileHandle;
#if OLDROUTINENAMES
enum {
    kCMApplProfileVersion       = cmCS1ProfileVersion
};

enum {
    grayResponse                = cmGrayResponse,
    redResponse                 = cmRedResponse,
    greenResponse               = cmGreenResponse,
    blueResponse                = cmBlueResponse,
    cyanResponse                = cmCyanResponse,
    magentaResponse             = cmMagentaResponse,
    yellowResponse              = cmYellowResponse,
    ucrResponse                 = cmUcrResponse,
    bgResponse                  = cmBgResponse,
    onePlusLastResponse         = cmOnePlusLastResponse
};

enum {
    rgbData                     = cmRGBData,
    cmykData                    = cmCMYKData,
    grayData                    = cmGrayData,
    xyzData                     = cmXYZData
};

enum {
    XYZData                     = cmXYZData
};

enum {
    monitorDevice               = cmMonitorDevice,
    scannerDevice               = cmScannerDevice,
    printerDevice               = cmPrinterDevice
};

enum {
    CMNativeMatchingPreferred   = cmNativeMatchingPreferred,    /* Default to native not preferred */
    CMTurnOffCache              = cmTurnOffCache                /* Default to turn on CMM cache */
};

enum {
    CMPerceptualMatch           = cmPerceptualMatch,            /* Default. For photographic images */
    CMColorimetricMatch         = cmColorimetricMatch,          /* Exact matching when possible */
    CMSaturationMatch           = cmSaturationMatch             /* For solid colors */
};


typedef unsigned short                  XYZComponent;
typedef CMXYZColor                      XYZColor;
typedef unsigned short                  CMResponseData;
typedef CMIString                       IString;
typedef long                            CMResponseColor;
typedef CMResponseColor                 responseColor;
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

#endif /* __CMICCPROFILE__ */

