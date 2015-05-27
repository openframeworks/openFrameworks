/*
     File:       ImageCompression.h
 
     Contains:   QuickTime Interfaces.
 
     Version:    Technology: QuickTime 6.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __IMAGECOMPRESSION__
#define __IMAGECOMPRESSION__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __QDOFFSCREEN__
#include <QDOffscreen.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __STANDARDFILE__
#include <StandardFile.h>
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


struct MatrixRecord {
    Fixed                           matrix[3][3];
};
typedef struct MatrixRecord             MatrixRecord;
typedef MatrixRecord *                  MatrixRecordPtr;
enum {
    kRawCodecType               = FOUR_CHAR_CODE('raw '),
    kCinepakCodecType           = FOUR_CHAR_CODE('cvid'),
    kGraphicsCodecType          = FOUR_CHAR_CODE('smc '),
    kAnimationCodecType         = FOUR_CHAR_CODE('rle '),
    kVideoCodecType             = FOUR_CHAR_CODE('rpza'),
    kComponentVideoCodecType    = FOUR_CHAR_CODE('yuv2'),
    kJPEGCodecType              = FOUR_CHAR_CODE('jpeg'),
    kMotionJPEGACodecType       = FOUR_CHAR_CODE('mjpa'),
    kMotionJPEGBCodecType       = FOUR_CHAR_CODE('mjpb'),
    kSGICodecType               = FOUR_CHAR_CODE('.SGI'),
    kPlanarRGBCodecType         = FOUR_CHAR_CODE('8BPS'),
    kMacPaintCodecType          = FOUR_CHAR_CODE('PNTG'),
    kGIFCodecType               = FOUR_CHAR_CODE('gif '),
    kPhotoCDCodecType           = FOUR_CHAR_CODE('kpcd'),
    kQuickDrawGXCodecType       = FOUR_CHAR_CODE('qdgx'),
    kAVRJPEGCodecType           = FOUR_CHAR_CODE('avr '),
    kOpenDMLJPEGCodecType       = FOUR_CHAR_CODE('dmb1'),
    kBMPCodecType               = FOUR_CHAR_CODE('WRLE'),
    kWindowsRawCodecType        = FOUR_CHAR_CODE('WRAW'),
    kVectorCodecType            = FOUR_CHAR_CODE('path'),
    kQuickDrawCodecType         = FOUR_CHAR_CODE('qdrw'),
    kWaterRippleCodecType       = FOUR_CHAR_CODE('ripl'),
    kFireCodecType              = FOUR_CHAR_CODE('fire'),
    kCloudCodecType             = FOUR_CHAR_CODE('clou'),
    kH261CodecType              = FOUR_CHAR_CODE('h261'),
    kH263CodecType              = FOUR_CHAR_CODE('h263'),
    kDVCNTSCCodecType           = FOUR_CHAR_CODE('dvc '),       /* DV - NTSC and DVCPRO NTSC (available in QuickTime 6.0 or later)*/
                                                                /* NOTE: kDVCProNTSCCodecType is deprecated.  */
                                                                /* Use kDVCNTSCCodecType instead -- as far as the codecs are concerned, */
                                                                /* the two data formats are identical.*/
    kDVCPALCodecType            = FOUR_CHAR_CODE('dvcp'),
    kDVCProPALCodecType         = FOUR_CHAR_CODE('dvpp'),       /* available in QuickTime 6.0 or later*/
    kBaseCodecType              = FOUR_CHAR_CODE('base'),
    kFLCCodecType               = FOUR_CHAR_CODE('flic'),
    kTargaCodecType             = FOUR_CHAR_CODE('tga '),
    kPNGCodecType               = FOUR_CHAR_CODE('png '),
    kTIFFCodecType              = FOUR_CHAR_CODE('tiff'),       /* NOTE: despite what might seem obvious from the two constants*/
                                                                /* below and their names, they really are correct. 'yuvu' really */
                                                                /* does mean signed, and 'yuvs' really does mean unsigned. Really. */
    kComponentVideoSigned       = FOUR_CHAR_CODE('yuvu'),
    kComponentVideoUnsigned     = FOUR_CHAR_CODE('yuvs'),
    kCMYKCodecType              = FOUR_CHAR_CODE('cmyk'),
    kMicrosoftVideo1CodecType   = FOUR_CHAR_CODE('msvc'),
    kSorensonCodecType          = FOUR_CHAR_CODE('SVQ1'),
    kSorenson3CodecType         = FOUR_CHAR_CODE('SVQ3'),       /* available in QuickTime 5 and later*/
    kIndeo4CodecType            = FOUR_CHAR_CODE('IV41'),
    kMPEG4VisualCodecType       = FOUR_CHAR_CODE('mp4v'),
    k64ARGBCodecType            = FOUR_CHAR_CODE('b64a'),
    k48RGBCodecType             = FOUR_CHAR_CODE('b48r'),
    k32AlphaGrayCodecType       = FOUR_CHAR_CODE('b32a'),
    k16GrayCodecType            = FOUR_CHAR_CODE('b16g'),
    kMpegYUV420CodecType        = FOUR_CHAR_CODE('myuv'),
    kYUV420CodecType            = FOUR_CHAR_CODE('y420'),
    kSorensonYUV9CodecType      = FOUR_CHAR_CODE('syv9'),
    k422YpCbCr8CodecType        = FOUR_CHAR_CODE('2vuy'),       /* Component Y'CbCr 8-bit 4:2:2  */
    k444YpCbCr8CodecType        = FOUR_CHAR_CODE('v308'),       /* Component Y'CbCr 8-bit 4:4:4  */
    k4444YpCbCrA8CodecType      = FOUR_CHAR_CODE('v408'),       /* Component Y'CbCrA 8-bit 4:4:4:4 */
    k422YpCbCr16CodecType       = FOUR_CHAR_CODE('v216'),       /* Component Y'CbCr 10,12,14,16-bit 4:2:2*/
    k422YpCbCr10CodecType       = FOUR_CHAR_CODE('v210'),       /* Component Y'CbCr 10-bit 4:2:2 */
    k444YpCbCr10CodecType       = FOUR_CHAR_CODE('v410'),       /* Component Y'CbCr 10-bit 4:4:4 */
    k4444YpCbCrA8RCodecType     = FOUR_CHAR_CODE('r408')        /* Component Y'CbCrA 8-bit 4:4:4:4, rendering format. full range alpha, zero biased yuv*/
};


/* one source effects */
enum {
    kBlurImageFilterType        = FOUR_CHAR_CODE('blur'),
    kSharpenImageFilterType     = FOUR_CHAR_CODE('shrp'),
    kEdgeDetectImageFilterType  = FOUR_CHAR_CODE('edge'),
    kEmbossImageFilterType      = FOUR_CHAR_CODE('embs'),
    kConvolveImageFilterType    = FOUR_CHAR_CODE('genk'),
    kAlphaGainImageFilterType   = FOUR_CHAR_CODE('gain'),
    kRGBColorBalanceImageFilterType = FOUR_CHAR_CODE('rgbb'),
    kHSLColorBalanceImageFilterType = FOUR_CHAR_CODE('hslb'),
    kColorSyncImageFilterType   = FOUR_CHAR_CODE('sync'),
    kFilmNoiseImageFilterType   = FOUR_CHAR_CODE('fmns'),
    kSolarizeImageFilterType    = FOUR_CHAR_CODE('solr'),
    kColorTintImageFilterType   = FOUR_CHAR_CODE('tint'),
    kLensFlareImageFilterType   = FOUR_CHAR_CODE('lens'),
    kBrightnessContrastImageFilterType = FOUR_CHAR_CODE('brco')
};

/* two source effects */
enum {
    kAlphaCompositorTransitionType = FOUR_CHAR_CODE('blnd'),
    kCrossFadeTransitionType    = FOUR_CHAR_CODE('dslv'),
    kChannelCompositeEffectType = FOUR_CHAR_CODE('chan'),
    kChromaKeyTransitionType    = FOUR_CHAR_CODE('ckey'),
    kImplodeTransitionType      = FOUR_CHAR_CODE('mplo'),
    kExplodeTransitionType      = FOUR_CHAR_CODE('xplo'),
    kGradientTransitionType     = FOUR_CHAR_CODE('matt'),
    kPushTransitionType         = FOUR_CHAR_CODE('push'),
    kSlideTransitionType        = FOUR_CHAR_CODE('slid'),
    kWipeTransitionType         = FOUR_CHAR_CODE('smpt'),
    kIrisTransitionType         = FOUR_CHAR_CODE('smp2'),
    kRadialTransitionType       = FOUR_CHAR_CODE('smp3'),
    kMatrixTransitionType       = FOUR_CHAR_CODE('smp4'),
    kZoomTransitionType         = FOUR_CHAR_CODE('zoom')
};

/* three source effects */
enum {
    kTravellingMatteEffectType  = FOUR_CHAR_CODE('trav')
};


/* Supported by QTNewGWorld in QuickTime 4.0 and later */
enum {
    kCMYKPixelFormat            = FOUR_CHAR_CODE('cmyk'),       /* CMYK, 8-bit */
    k64ARGBPixelFormat          = FOUR_CHAR_CODE('b64a'),       /* ARGB, 16-bit big-endian samples */
    k48RGBPixelFormat           = FOUR_CHAR_CODE('b48r'),       /* RGB, 16-bit big-endian samples */
    k32AlphaGrayPixelFormat     = FOUR_CHAR_CODE('b32a'),       /* AlphaGray, 16-bit big-endian samples */
    k16GrayPixelFormat          = FOUR_CHAR_CODE('b16g'),       /* Grayscale, 16-bit big-endian samples */
    k422YpCbCr8PixelFormat      = FOUR_CHAR_CODE('2vuy')        /* Component Y'CbCr 8-bit 4:2:2, ordered Cb Y'0 Cr Y'1 */
};

/* Supported by QTNewGWorld in QuickTime 4.1.2 and later */
enum {
    k4444YpCbCrA8PixelFormat    = FOUR_CHAR_CODE('v408'),       /* Component Y'CbCrA 8-bit 4:4:4:4, ordered Cb Y' Cr A */
    k4444YpCbCrA8RPixelFormat   = FOUR_CHAR_CODE('r408')        /* Component Y'CbCrA 8-bit 4:4:4:4, rendering format. full range alpha, zero biased yuv, ordered A Y' Cb Cr */
};

/* Supported by QTNewGWorld in QuickTime 6.0 and later */
enum {
    kYUV420PixelFormat          = FOUR_CHAR_CODE('y420')        /* Planar Component Y'CbCr 8-bit 4:2:0.  PixMap baseAddr points to a big-endian PlanarPixmapInfoYUV420 struct; see ImageCodec.i. */
};


/* These are the bits that are set in the Component flags, and also in the codecInfo struct. */
enum {
    codecInfoDoes1              = (1L << 0),                    /* codec can work with 1-bit pixels */
    codecInfoDoes2              = (1L << 1),                    /* codec can work with 2-bit pixels */
    codecInfoDoes4              = (1L << 2),                    /* codec can work with 4-bit pixels */
    codecInfoDoes8              = (1L << 3),                    /* codec can work with 8-bit pixels */
    codecInfoDoes16             = (1L << 4),                    /* codec can work with 16-bit pixels */
    codecInfoDoes32             = (1L << 5),                    /* codec can work with 32-bit pixels */
    codecInfoDoesDither         = (1L << 6),                    /* codec can do ditherMode */
    codecInfoDoesStretch        = (1L << 7),                    /* codec can stretch to arbitrary sizes */
    codecInfoDoesShrink         = (1L << 8),                    /* codec can shrink to arbitrary sizes */
    codecInfoDoesMask           = (1L << 9),                    /* codec can mask to clipping regions */
    codecInfoDoesTemporal       = (1L << 10),                   /* codec can handle temporal redundancy */
    codecInfoDoesDouble         = (1L << 11),                   /* codec can stretch to double size exactly */
    codecInfoDoesQuad           = (1L << 12),                   /* codec can stretch to quadruple size exactly */
    codecInfoDoesHalf           = (1L << 13),                   /* codec can shrink to half size */
    codecInfoDoesQuarter        = (1L << 14),                   /* codec can shrink to quarter size */
    codecInfoDoesRotate         = (1L << 15),                   /* codec can rotate on decompress */
    codecInfoDoesHorizFlip      = (1L << 16),                   /* codec can flip horizontally on decompress */
    codecInfoDoesVertFlip       = (1L << 17),                   /* codec can flip vertically on decompress */
    codecInfoHasEffectParameterList = (1L << 18),               /* codec implements get effects parameter list call, once was codecInfoDoesSkew */
    codecInfoDoesBlend          = (1L << 19),                   /* codec can blend on decompress */
    codecInfoDoesWarp           = (1L << 20),                   /* codec can warp arbitrarily on decompress */
    codecInfoDoesRecompress     = (1L << 21),                   /* codec can recompress image without accumulating errors */
    codecInfoDoesSpool          = (1L << 22),                   /* codec can spool image data */
    codecInfoDoesRateConstrain  = (1L << 23)                    /* codec can data rate constrain */
};


enum {
    codecInfoDepth1             = (1L << 0),                    /* compressed data at 1 bpp depth available */
    codecInfoDepth2             = (1L << 1),                    /* compressed data at 2 bpp depth available */
    codecInfoDepth4             = (1L << 2),                    /* compressed data at 4 bpp depth available */
    codecInfoDepth8             = (1L << 3),                    /* compressed data at 8 bpp depth available */
    codecInfoDepth16            = (1L << 4),                    /* compressed data at 16 bpp depth available */
    codecInfoDepth32            = (1L << 5),                    /* compressed data at 32 bpp depth available */
    codecInfoDepth24            = (1L << 6),                    /* compressed data at 24 bpp depth available */
    codecInfoDepth33            = (1L << 7),                    /* compressed data at 1 bpp monochrome depth  available */
    codecInfoDepth34            = (1L << 8),                    /* compressed data at 2 bpp grayscale depth available */
    codecInfoDepth36            = (1L << 9),                    /* compressed data at 4 bpp grayscale depth available */
    codecInfoDepth40            = (1L << 10),                   /* compressed data at 8 bpp grayscale depth available */
    codecInfoStoresClut         = (1L << 11),                   /* compressed data can have custom cluts */
    codecInfoDoesLossless       = (1L << 12),                   /* compressed data can be stored in lossless format */
    codecInfoSequenceSensitive  = (1L << 13)                    /* compressed data is sensitive to out of sequence decoding */
};


/* input sequence flags*/
enum {
    codecFlagUseImageBuffer     = (1L << 0),                    /* decompress*/
    codecFlagUseScreenBuffer    = (1L << 1),                    /* decompress*/
    codecFlagUpdatePrevious     = (1L << 2),                    /* compress*/
    codecFlagNoScreenUpdate     = (1L << 3),                    /* decompress*/
    codecFlagWasCompressed      = (1L << 4),                    /* compress*/
    codecFlagDontOffscreen      = (1L << 5),                    /* decompress*/
    codecFlagUpdatePreviousComp = (1L << 6),                    /* compress*/
    codecFlagForceKeyFrame      = (1L << 7),                    /* compress*/
    codecFlagOnlyScreenUpdate   = (1L << 8),                    /* decompress*/
    codecFlagLiveGrab           = (1L << 9),                    /* compress*/
    codecFlagDiffFrame          = (1L << 9),                    /* decompress*/
    codecFlagDontUseNewImageBuffer = (1L << 10),                /* decompress*/
    codecFlagInterlaceUpdate    = (1L << 11),                   /* decompress*/
    codecFlagCatchUpDiff        = (1L << 12),                   /* decompress*/
    codecFlagSupportDisable     = (1L << 13),                   /* decompress*/
    codecFlagReenable           = (1L << 14)                    /* decompress*/
};


/* output sequence flags*/
enum {
    codecFlagOutUpdateOnNextIdle = (1L << 9),
    codecFlagOutUpdateOnDataSourceChange = (1L << 10),
    codecFlagSequenceSensitive  = (1L << 11),
    codecFlagOutUpdateOnTimeChange = (1L << 12),
    codecFlagImageBufferNotSourceImage = (1L << 13),
    codecFlagUsedNewImageBuffer = (1L << 14),
    codecFlagUsedImageBuffer    = (1L << 15)
};



enum {
                                                                /* The minimum data size for spooling in or out data */
    codecMinimumDataSize        = 32768L
};



enum {
    compressorComponentType     = FOUR_CHAR_CODE('imco'),       /* the type for "Components" which compress images */
    decompressorComponentType   = FOUR_CHAR_CODE('imdc')        /* the type for "Components" which decompress images */
};


typedef Component                       CompressorComponent;
typedef Component                       DecompressorComponent;
typedef Component                       CodecComponent;
#define anyCodec                ((CodecComponent)0)
#define bestSpeedCodec          ((CodecComponent)-1)
#define bestFidelityCodec       ((CodecComponent)-2)
#define bestCompressionCodec    ((CodecComponent)-3)
typedef OSType                          CodecType;
typedef unsigned short                  CodecFlags;
typedef unsigned long                   CodecQ;
enum {
    codecLosslessQuality        = 0x00000400,
    codecMaxQuality             = 0x000003FF,
    codecMinQuality             = 0x00000000,
    codecLowQuality             = 0x00000100,
    codecNormalQuality          = 0x00000200,
    codecHighQuality            = 0x00000300
};

enum {
    codecLockBitsShieldCursor   = (1 << 0)                      /* shield cursor */
};

enum {
    codecCompletionSource       = (1 << 0),                     /* asynchronous codec is done with source data */
    codecCompletionDest         = (1 << 1),                     /* asynchronous codec is done with destination data */
    codecCompletionDontUnshield = (1 << 2),                     /* on dest complete don't unshield cursor */
    codecCompletionWentOffscreen = (1 << 3),                    /* codec used offscreen buffer */
    codecCompletionUnlockBits   = (1 << 4),                     /* on dest complete, call ICMSequenceUnlockBits */
    codecCompletionForceChainFlush = (1 << 5),                  /* ICM needs to flush the whole chain */
    codecCompletionDropped      = (1 << 6)                      /* codec decided to drop this frame */
};

enum {
    codecProgressOpen           = 0,
    codecProgressUpdatePercent  = 1,
    codecProgressClose          = 2
};

typedef CALLBACK_API( OSErr , ICMDataProcPtr )(Ptr *dataP, long bytesNeeded, long refcon);
typedef CALLBACK_API( OSErr , ICMFlushProcPtr )(Ptr data, long bytesAdded, long refcon);
typedef CALLBACK_API( void , ICMCompletionProcPtr )(OSErr result, short flags, long refcon);
typedef CALLBACK_API( OSErr , ICMProgressProcPtr )(short message, Fixed completeness, long refcon);
typedef CALLBACK_API( void , StdPixProcPtr )(PixMap *src, Rect *srcRect, MatrixRecord *matrix, short mode, RgnHandle mask, PixMap *matte, Rect *matteRect, short flags);
typedef CALLBACK_API( void , QDPixProcPtr )(PixMap *src, Rect *srcRect, MatrixRecord *matrix, short mode, RgnHandle mask, PixMap *matte, Rect *matteRect, short flags);
typedef CALLBACK_API( void , ICMAlignmentProcPtr )(Rect *rp, long refcon);
typedef CALLBACK_API( void , ICMCursorShieldedProcPtr )(const Rect *r, void *refcon, long flags);
typedef CALLBACK_API( void , ICMMemoryDisposedProcPtr )(Ptr memoryBlock, void *refcon);
typedef void *                          ICMCursorNotify;
typedef CALLBACK_API( OSErr , ICMConvertDataFormatProcPtr )(void *refCon, long flags, Handle desiredFormat, Handle sourceDataFormat, void *srcData, long srcDataSize, void **dstData, long *dstDataSize);
typedef STACK_UPP_TYPE(ICMDataProcPtr)                          ICMDataUPP;
typedef STACK_UPP_TYPE(ICMFlushProcPtr)                         ICMFlushUPP;
typedef STACK_UPP_TYPE(ICMCompletionProcPtr)                    ICMCompletionUPP;
typedef STACK_UPP_TYPE(ICMProgressProcPtr)                      ICMProgressUPP;
typedef STACK_UPP_TYPE(StdPixProcPtr)                           StdPixUPP;
typedef STACK_UPP_TYPE(QDPixProcPtr)                            QDPixUPP;
typedef STACK_UPP_TYPE(ICMAlignmentProcPtr)                     ICMAlignmentUPP;
typedef STACK_UPP_TYPE(ICMCursorShieldedProcPtr)                ICMCursorShieldedUPP;
typedef STACK_UPP_TYPE(ICMMemoryDisposedProcPtr)                ICMMemoryDisposedUPP;
typedef STACK_UPP_TYPE(ICMConvertDataFormatProcPtr)             ICMConvertDataFormatUPP;
typedef long                            ImageSequence;
typedef long                            ImageSequenceDataSource;
typedef long                            ImageTranscodeSequence;
typedef long                            ImageFieldSequence;

struct ICMProgressProcRecord {
    ICMProgressUPP                  progressProc;
    long                            progressRefCon;
};
typedef struct ICMProgressProcRecord    ICMProgressProcRecord;
typedef ICMProgressProcRecord *         ICMProgressProcRecordPtr;

struct ICMCompletionProcRecord {
    ICMCompletionUPP                completionProc;
    long                            completionRefCon;
};
typedef struct ICMCompletionProcRecord  ICMCompletionProcRecord;
typedef ICMCompletionProcRecord *       ICMCompletionProcRecordPtr;

struct ICMDataProcRecord {
    ICMDataUPP                      dataProc;
    long                            dataRefCon;
};
typedef struct ICMDataProcRecord        ICMDataProcRecord;
typedef ICMDataProcRecord *             ICMDataProcRecordPtr;

struct ICMFlushProcRecord {
    ICMFlushUPP                     flushProc;
    long                            flushRefCon;
};
typedef struct ICMFlushProcRecord       ICMFlushProcRecord;
typedef ICMFlushProcRecord *            ICMFlushProcRecordPtr;

struct ICMAlignmentProcRecord {
    ICMAlignmentUPP                 alignmentProc;
    long                            alignmentRefCon;
};
typedef struct ICMAlignmentProcRecord   ICMAlignmentProcRecord;
typedef ICMAlignmentProcRecord *        ICMAlignmentProcRecordPtr;

struct DataRateParams {
    long                            dataRate;
    long                            dataOverrun;
    long                            frameDuration;
    long                            keyFrameRate;
    CodecQ                          minSpatialQuality;
    CodecQ                          minTemporalQuality;
};
typedef struct DataRateParams           DataRateParams;
typedef DataRateParams *                DataRateParamsPtr;

struct ImageDescription {
    long                            idSize;                     /* total size of ImageDescription including extra data ( CLUTs and other per sequence data ) */
    CodecType                       cType;                      /* what kind of codec compressed this data */
    long                            resvd1;                     /* reserved for Apple use */
    short                           resvd2;                     /* reserved for Apple use */
    short                           dataRefIndex;               /* set to zero  */
    short                           version;                    /* which version is this data */
    short                           revisionLevel;              /* what version of that codec did this */
    long                            vendor;                     /* whose  codec compressed this data */
    CodecQ                          temporalQuality;            /* what was the temporal quality factor  */
    CodecQ                          spatialQuality;             /* what was the spatial quality factor */
    short                           width;                      /* how many pixels wide is this data */
    short                           height;                     /* how many pixels high is this data */
    Fixed                           hRes;                       /* horizontal resolution */
    Fixed                           vRes;                       /* vertical resolution */
    long                            dataSize;                   /* if known, the size of data for this image descriptor */
    short                           frameCount;                 /* number of frames this description applies to */
    Str31                           name;                       /* name of codec ( in case not installed )  */
    short                           depth;                      /* what depth is this data (1-32) or ( 33-40 grayscale ) */
    short                           clutID;                     /* clut id or if 0 clut follows  or -1 if no clut */
};
typedef struct ImageDescription         ImageDescription;
typedef ImageDescription *              ImageDescriptionPtr;
typedef ImageDescriptionPtr *           ImageDescriptionHandle;

struct CodecInfo {
    Str31                           typeName;                   /* name of the codec type i.e.: 'Apple Image Compression' */
    short                           version;                    /* version of the codec data that this codec knows about */
    short                           revisionLevel;              /* revision level of this codec i.e: 0x00010001 (1.0.1) */
    long                            vendor;                     /* Maker of this codec i.e: 'appl' */
    long                            decompressFlags;            /* codecInfo flags for decompression capabilities */
    long                            compressFlags;              /* codecInfo flags for compression capabilities */
    long                            formatFlags;                /* codecInfo flags for compression format details */
    UInt8                           compressionAccuracy;        /* measure (1-255) of accuracy of this codec for compress (0 if unknown) */
    UInt8                           decompressionAccuracy;      /* measure (1-255) of accuracy of this codec for decompress (0 if unknown) */
    unsigned short                  compressionSpeed;           /* ( millisecs for compressing 320x240 on base mac II) (0 if unknown)  */
    unsigned short                  decompressionSpeed;         /* ( millisecs for decompressing 320x240 on mac II)(0 if unknown)  */
    UInt8                           compressionLevel;           /* measure (1-255) of compression level of this codec (0 if unknown)  */
    UInt8                           resvd;                      /* pad */
    short                           minimumHeight;              /* minimum height of image (block size) */
    short                           minimumWidth;               /* minimum width of image (block size) */
    short                           decompressPipelineLatency;  /* in milliseconds ( for asynchronous codecs ) */
    short                           compressPipelineLatency;    /* in milliseconds ( for asynchronous codecs ) */
    long                            privateData;
};
typedef struct CodecInfo                CodecInfo;

struct CodecNameSpec {
    CodecComponent                  codec;
    CodecType                       cType;
    Str31                           typeName;
    Handle                          name;
};
typedef struct CodecNameSpec            CodecNameSpec;

struct CodecNameSpecList {
    short                           count;
    CodecNameSpec                   list[1];
};
typedef struct CodecNameSpecList        CodecNameSpecList;
typedef CodecNameSpecList *             CodecNameSpecListPtr;
enum {
    defaultDither               = 0,
    forceDither                 = 1,
    suppressDither              = 2,
    useColorMatching            = 4
};

enum {
    callStdBits                 = 1,
    callOldBits                 = 2,
    noDefaultOpcodes            = 4
};

enum {
    graphicsModeStraightAlpha   = 256,
    graphicsModePreWhiteAlpha   = 257,
    graphicsModePreBlackAlpha   = 258,
    graphicsModeComposition     = 259,
    graphicsModeStraightAlphaBlend = 260,
    graphicsModePreMulColorAlpha = 261
};

enum {
    evenField1ToEvenFieldOut    = 1 << 0,
    evenField1ToOddFieldOut     = 1 << 1,
    oddField1ToEvenFieldOut     = 1 << 2,
    oddField1ToOddFieldOut      = 1 << 3,
    evenField2ToEvenFieldOut    = 1 << 4,
    evenField2ToOddFieldOut     = 1 << 5,
    oddField2ToEvenFieldOut     = 1 << 6,
    oddField2ToOddFieldOut      = 1 << 7
};

enum {
    icmFrameTimeHasVirtualStartTimeAndDuration = 1 << 0
};


struct ICMFrameTimeRecord {
    wide                            value;                      /* frame time*/
    long                            scale;                      /* timescale of value/duration fields*/
    void *                          base;                       /* timebase*/

    long                            duration;                   /* duration frame is to be displayed (0 if unknown)*/
    Fixed                           rate;                       /* rate of timebase relative to wall-time*/

    long                            recordSize;                 /* total number of bytes in ICMFrameTimeRecord*/

    long                            frameNumber;                /* number of frame, zero if not known*/

    long                            flags;

    wide                            virtualStartTime;           /* conceptual start time*/
    long                            virtualDuration;            /* conceptual duration*/
};
typedef struct ICMFrameTimeRecord       ICMFrameTimeRecord;

typedef ICMFrameTimeRecord *            ICMFrameTimePtr;
#ifndef __QTUUID__
#define __QTUUID__ 1
/* QuickTime flavor of universally unique identifier (uuid)*/

struct QTUUID {
    UInt32                          data1;
    UInt16                          data2;
    UInt16                          data3;
    UInt8                           data4[8];
};
typedef struct QTUUID                   QTUUID;

typedef QTUUID                          QTMediaContextID;
#endif  /* !defined(__QTUUID__) */

#if OPAQUE_UPP_TYPES
    EXTERN_API(ICMDataUPP)
    NewICMDataUPP                  (ICMDataProcPtr          userRoutine);

    EXTERN_API(ICMFlushUPP)
    NewICMFlushUPP                 (ICMFlushProcPtr         userRoutine);

    EXTERN_API(ICMCompletionUPP)
    NewICMCompletionUPP            (ICMCompletionProcPtr    userRoutine);

    EXTERN_API(ICMProgressUPP)
    NewICMProgressUPP              (ICMProgressProcPtr      userRoutine);

    EXTERN_API(StdPixUPP)
    NewStdPixUPP                   (StdPixProcPtr           userRoutine);

    EXTERN_API(QDPixUPP)
    NewQDPixUPP                    (QDPixProcPtr            userRoutine);

    EXTERN_API(ICMAlignmentUPP)
    NewICMAlignmentUPP             (ICMAlignmentProcPtr     userRoutine);

    EXTERN_API(ICMCursorShieldedUPP)
    NewICMCursorShieldedUPP        (ICMCursorShieldedProcPtr userRoutine);

    EXTERN_API(ICMMemoryDisposedUPP)
    NewICMMemoryDisposedUPP        (ICMMemoryDisposedProcPtr userRoutine);

    EXTERN_API(ICMConvertDataFormatUPP)
    NewICMConvertDataFormatUPP     (ICMConvertDataFormatProcPtr userRoutine);

    EXTERN_API(void)
    DisposeICMDataUPP              (ICMDataUPP              userUPP);

    EXTERN_API(void)
    DisposeICMFlushUPP             (ICMFlushUPP             userUPP);

    EXTERN_API(void)
    DisposeICMCompletionUPP        (ICMCompletionUPP        userUPP);

    EXTERN_API(void)
    DisposeICMProgressUPP          (ICMProgressUPP          userUPP);

    EXTERN_API(void)
    DisposeStdPixUPP               (StdPixUPP               userUPP);

    EXTERN_API(void)
    DisposeQDPixUPP                (QDPixUPP                userUPP);

    EXTERN_API(void)
    DisposeICMAlignmentUPP         (ICMAlignmentUPP         userUPP);

    EXTERN_API(void)
    DisposeICMCursorShieldedUPP    (ICMCursorShieldedUPP    userUPP);

    EXTERN_API(void)
    DisposeICMMemoryDisposedUPP    (ICMMemoryDisposedUPP    userUPP);

    EXTERN_API(void)
    DisposeICMConvertDataFormatUPP    (ICMConvertDataFormatUPP userUPP);

    EXTERN_API(OSErr)
    InvokeICMDataUPP               (Ptr *                   dataP,
                                    long                    bytesNeeded,
                                    long                    refcon,
                                    ICMDataUPP              userUPP);

    EXTERN_API(OSErr)
    InvokeICMFlushUPP              (Ptr                     data,
                                    long                    bytesAdded,
                                    long                    refcon,
                                    ICMFlushUPP             userUPP);

    EXTERN_API(void)
    InvokeICMCompletionUPP         (OSErr                   result,
                                    short                   flags,
                                    long                    refcon,
                                    ICMCompletionUPP        userUPP);

    EXTERN_API(OSErr)
    InvokeICMProgressUPP           (short                   message,
                                    Fixed                   completeness,
                                    long                    refcon,
                                    ICMProgressUPP          userUPP);

    EXTERN_API(void)
    InvokeStdPixUPP                (PixMap *                src,
                                    Rect *                  srcRect,
                                    MatrixRecord *          matrix,
                                    short                   mode,
                                    RgnHandle               mask,
                                    PixMap *                matte,
                                    Rect *                  matteRect,
                                    short                   flags,
                                    StdPixUPP               userUPP);

    EXTERN_API(void)
    InvokeQDPixUPP                 (PixMap *                src,
                                    Rect *                  srcRect,
                                    MatrixRecord *          matrix,
                                    short                   mode,
                                    RgnHandle               mask,
                                    PixMap *                matte,
                                    Rect *                  matteRect,
                                    short                   flags,
                                    QDPixUPP                userUPP);

    EXTERN_API(void)
    InvokeICMAlignmentUPP          (Rect *                  rp,
                                    long                    refcon,
                                    ICMAlignmentUPP         userUPP);

    EXTERN_API(void)
    InvokeICMCursorShieldedUPP     (const Rect *            r,
                                    void *                  refcon,
                                    long                    flags,
                                    ICMCursorShieldedUPP    userUPP);

    EXTERN_API(void)
    InvokeICMMemoryDisposedUPP     (Ptr                     memoryBlock,
                                    void *                  refcon,
                                    ICMMemoryDisposedUPP    userUPP);

    EXTERN_API(OSErr)
    InvokeICMConvertDataFormatUPP    (void *                refCon,
                                    long                    flags,
                                    Handle                  desiredFormat,
                                    Handle                  sourceDataFormat,
                                    void *                  srcData,
                                    long                    srcDataSize,
                                    void **                 dstData,
                                    long *                  dstDataSize,
                                    ICMConvertDataFormatUPP userUPP);

#else
    enum { uppICMDataProcInfo = 0x00000FE0 };                       /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppICMFlushProcInfo = 0x00000FE0 };                      /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppICMCompletionProcInfo = 0x00000E80 };                 /* pascal no_return_value Func(2_bytes, 2_bytes, 4_bytes) */
    enum { uppICMProgressProcInfo = 0x00000FA0 };                   /* pascal 2_bytes Func(2_bytes, 4_bytes, 4_bytes) */
    enum { uppStdPixProcInfo = 0x002FEFC0 };                        /* pascal no_return_value Func(4_bytes, 4_bytes, 4_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes, 2_bytes) */
    enum { uppQDPixProcInfo = 0x002FEFC0 };                         /* pascal no_return_value Func(4_bytes, 4_bytes, 4_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes, 2_bytes) */
    enum { uppICMAlignmentProcInfo = 0x000003C0 };                  /* pascal no_return_value Func(4_bytes, 4_bytes) */
    enum { uppICMCursorShieldedProcInfo = 0x00000FC0 };             /* pascal no_return_value Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppICMMemoryDisposedProcInfo = 0x000003C0 };             /* pascal no_return_value Func(4_bytes, 4_bytes) */
    enum { uppICMConvertDataFormatProcInfo = 0x003FFFE0 };          /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewICMDataUPP(userRoutine)                              (ICMDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppICMDataProcInfo, GetCurrentArchitecture())
    #define NewICMFlushUPP(userRoutine)                             (ICMFlushUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppICMFlushProcInfo, GetCurrentArchitecture())
    #define NewICMCompletionUPP(userRoutine)                        (ICMCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppICMCompletionProcInfo, GetCurrentArchitecture())
    #define NewICMProgressUPP(userRoutine)                          (ICMProgressUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppICMProgressProcInfo, GetCurrentArchitecture())
    #define NewStdPixUPP(userRoutine)                               (StdPixUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppStdPixProcInfo, GetCurrentArchitecture())
    #define NewQDPixUPP(userRoutine)                                (QDPixUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppQDPixProcInfo, GetCurrentArchitecture())
    #define NewICMAlignmentUPP(userRoutine)                         (ICMAlignmentUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppICMAlignmentProcInfo, GetCurrentArchitecture())
    #define NewICMCursorShieldedUPP(userRoutine)                    (ICMCursorShieldedUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppICMCursorShieldedProcInfo, GetCurrentArchitecture())
    #define NewICMMemoryDisposedUPP(userRoutine)                    (ICMMemoryDisposedUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppICMMemoryDisposedProcInfo, GetCurrentArchitecture())
    #define NewICMConvertDataFormatUPP(userRoutine)                 (ICMConvertDataFormatUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppICMConvertDataFormatProcInfo, GetCurrentArchitecture())
    #define DisposeICMDataUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define DisposeICMFlushUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define DisposeICMCompletionUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeICMProgressUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposeStdPixUPP(userUPP)                               DisposeRoutineDescriptor(userUPP)
    #define DisposeQDPixUPP(userUPP)                                DisposeRoutineDescriptor(userUPP)
    #define DisposeICMAlignmentUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeICMCursorShieldedUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeICMMemoryDisposedUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeICMConvertDataFormatUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define InvokeICMDataUPP(dataP, bytesNeeded, refcon, userUPP)   (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppICMDataProcInfo, (dataP), (bytesNeeded), (refcon))
    #define InvokeICMFlushUPP(data, bytesAdded, refcon, userUPP)    (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppICMFlushProcInfo, (data), (bytesAdded), (refcon))
    #define InvokeICMCompletionUPP(result, flags, refcon, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppICMCompletionProcInfo, (result), (flags), (refcon))
    #define InvokeICMProgressUPP(message, completeness, refcon, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppICMProgressProcInfo, (message), (completeness), (refcon))
    #define InvokeStdPixUPP(src, srcRect, matrix, mode, mask, matte, matteRect, flags, userUPP)  CALL_EIGHT_PARAMETER_UPP((userUPP), uppStdPixProcInfo, (src), (srcRect), (matrix), (mode), (mask), (matte), (matteRect), (flags))
    #define InvokeQDPixUPP(src, srcRect, matrix, mode, mask, matte, matteRect, flags, userUPP)  CALL_EIGHT_PARAMETER_UPP((userUPP), uppQDPixProcInfo, (src), (srcRect), (matrix), (mode), (mask), (matte), (matteRect), (flags))
    #define InvokeICMAlignmentUPP(rp, refcon, userUPP)              CALL_TWO_PARAMETER_UPP((userUPP), uppICMAlignmentProcInfo, (rp), (refcon))
    #define InvokeICMCursorShieldedUPP(r, refcon, flags, userUPP)   CALL_THREE_PARAMETER_UPP((userUPP), uppICMCursorShieldedProcInfo, (r), (refcon), (flags))
    #define InvokeICMMemoryDisposedUPP(memoryBlock, refcon, userUPP)  CALL_TWO_PARAMETER_UPP((userUPP), uppICMMemoryDisposedProcInfo, (memoryBlock), (refcon))
    #define InvokeICMConvertDataFormatUPP(refCon, flags, desiredFormat, sourceDataFormat, srcData, srcDataSize, dstData, dstDataSize, userUPP)  (OSErr)CALL_EIGHT_PARAMETER_UPP((userUPP), uppICMConvertDataFormatProcInfo, (refCon), (flags), (desiredFormat), (sourceDataFormat), (srcData), (srcDataSize), (dstData), (dstDataSize))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewICMDataProc(userRoutine)                             NewICMDataUPP(userRoutine)
#define NewICMFlushProc(userRoutine)                            NewICMFlushUPP(userRoutine)
#define NewICMCompletionProc(userRoutine)                       NewICMCompletionUPP(userRoutine)
#define NewICMProgressProc(userRoutine)                         NewICMProgressUPP(userRoutine)
#define NewStdPixProc(userRoutine)                              NewStdPixUPP(userRoutine)
#define NewQDPixProc(userRoutine)                               NewQDPixUPP(userRoutine)
#define NewICMAlignmentProc(userRoutine)                        NewICMAlignmentUPP(userRoutine)
#define NewICMCursorShieldedProc(userRoutine)                   NewICMCursorShieldedUPP(userRoutine)
#define NewICMMemoryDisposedProc(userRoutine)                   NewICMMemoryDisposedUPP(userRoutine)
#define NewICMConvertDataFormatProc(userRoutine)                NewICMConvertDataFormatUPP(userRoutine)
#define CallICMDataProc(userRoutine, dataP, bytesNeeded, refcon) InvokeICMDataUPP(dataP, bytesNeeded, refcon, userRoutine)
#define CallICMFlushProc(userRoutine, data, bytesAdded, refcon) InvokeICMFlushUPP(data, bytesAdded, refcon, userRoutine)
#define CallICMCompletionProc(userRoutine, result, flags, refcon) InvokeICMCompletionUPP(result, flags, refcon, userRoutine)
#define CallICMProgressProc(userRoutine, message, completeness, refcon) InvokeICMProgressUPP(message, completeness, refcon, userRoutine)
#define CallStdPixProc(userRoutine, src, srcRect, matrix, mode, mask, matte, matteRect, flags) InvokeStdPixUPP(src, srcRect, matrix, mode, mask, matte, matteRect, flags, userRoutine)
#define CallQDPixProc(userRoutine, src, srcRect, matrix, mode, mask, matte, matteRect, flags) InvokeQDPixUPP(src, srcRect, matrix, mode, mask, matte, matteRect, flags, userRoutine)
#define CallICMAlignmentProc(userRoutine, rp, refcon)           InvokeICMAlignmentUPP(rp, refcon, userRoutine)
#define CallICMCursorShieldedProc(userRoutine, r, refcon, flags) InvokeICMCursorShieldedUPP(r, refcon, flags, userRoutine)
#define CallICMMemoryDisposedProc(userRoutine, memoryBlock, refcon) InvokeICMMemoryDisposedUPP(memoryBlock, refcon, userRoutine)
#define CallICMConvertDataFormatProc(userRoutine, refCon, flags, desiredFormat, sourceDataFormat, srcData, srcDataSize, dstData, dstDataSize) InvokeICMConvertDataFormatUPP(refCon, flags, desiredFormat, sourceDataFormat, srcData, srcDataSize, dstData, dstDataSize, userRoutine)

EXTERN_API( OSErr )
CodecManagerVersion             (long *                 version)                            TWOWORDINLINE(0x7000, 0xAAA3);

EXTERN_API( OSErr )
GetCodecNameList                (CodecNameSpecListPtr * list,
                                 short                  showAll)                            TWOWORDINLINE(0x7001, 0xAAA3);

EXTERN_API( OSErr )
DisposeCodecNameList            (CodecNameSpecListPtr   list)                               TWOWORDINLINE(0x700F, 0xAAA3);

EXTERN_API( OSErr )
GetCodecInfo                    (CodecInfo *            info,
                                 CodecType              cType,
                                 CodecComponent         codec)                              TWOWORDINLINE(0x7003, 0xAAA3);

EXTERN_API( OSErr )
GetMaxCompressionSize           (PixMapHandle           src,
                                 const Rect *           srcRect,
                                 short                  colorDepth,
                                 CodecQ                 quality,
                                 CodecType              cType,
                                 CompressorComponent    codec,
                                 long *                 size)                               TWOWORDINLINE(0x7004, 0xAAA3);

EXTERN_API( OSErr )
GetCSequenceMaxCompressionSize  (ImageSequence          seqID,
                                 PixMapHandle           src,
                                 long *                 size)                               FOURWORDINLINE(0x203C, 0x000C, 0x0074, 0xAAA3);

EXTERN_API( OSErr )
GetCompressionTime              (PixMapHandle           src,
                                 const Rect *           srcRect,
                                 short                  colorDepth,
                                 CodecType              cType,
                                 CompressorComponent    codec,
                                 CodecQ *               spatialQuality,
                                 CodecQ *               temporalQuality,
                                 unsigned long *        compressTime)                       TWOWORDINLINE(0x7005, 0xAAA3);

EXTERN_API( OSErr )
CompressImage                   (PixMapHandle           src,
                                 const Rect *           srcRect,
                                 CodecQ                 quality,
                                 CodecType              cType,
                                 ImageDescriptionHandle  desc,
                                 Ptr                    data)                               TWOWORDINLINE(0x7006, 0xAAA3);

EXTERN_API( OSErr )
FCompressImage                  (PixMapHandle           src,
                                 const Rect *           srcRect,
                                 short                  colorDepth,
                                 CodecQ                 quality,
                                 CodecType              cType,
                                 CompressorComponent    codec,
                                 CTabHandle             ctable,
                                 CodecFlags             flags,
                                 long                   bufferSize,
                                 ICMFlushProcRecordPtr  flushProc,
                                 ICMProgressProcRecordPtr  progressProc,
                                 ImageDescriptionHandle  desc,
                                 Ptr                    data)                               TWOWORDINLINE(0x7007, 0xAAA3);

EXTERN_API( OSErr )
DecompressImage                 (Ptr                    data,
                                 ImageDescriptionHandle  desc,
                                 PixMapHandle           dst,
                                 const Rect *           srcRect,
                                 const Rect *           dstRect,
                                 short                  mode,
                                 RgnHandle              mask)                               TWOWORDINLINE(0x7008, 0xAAA3);

EXTERN_API( OSErr )
FDecompressImage                (Ptr                    data,
                                 ImageDescriptionHandle  desc,
                                 PixMapHandle           dst,
                                 const Rect *           srcRect,
                                 MatrixRecordPtr        matrix,
                                 short                  mode,
                                 RgnHandle              mask,
                                 PixMapHandle           matte,
                                 const Rect *           matteRect,
                                 CodecQ                 accuracy,
                                 DecompressorComponent  codec,
                                 long                   bufferSize,
                                 ICMDataProcRecordPtr   dataProc,
                                 ICMProgressProcRecordPtr  progressProc)                    TWOWORDINLINE(0x7009, 0xAAA3);

EXTERN_API( OSErr )
CompressSequenceBegin           (ImageSequence *        seqID,
                                 PixMapHandle           src,
                                 PixMapHandle           prev,
                                 const Rect *           srcRect,
                                 const Rect *           prevRect,
                                 short                  colorDepth,
                                 CodecType              cType,
                                 CompressorComponent    codec,
                                 CodecQ                 spatialQuality,
                                 CodecQ                 temporalQuality,
                                 long                   keyFrameRate,
                                 CTabHandle             ctable,
                                 CodecFlags             flags,
                                 ImageDescriptionHandle  desc)                              TWOWORDINLINE(0x700A, 0xAAA3);

EXTERN_API( OSErr )
CompressSequenceFrame           (ImageSequence          seqID,
                                 PixMapHandle           src,
                                 const Rect *           srcRect,
                                 CodecFlags             flags,
                                 Ptr                    data,
                                 long *                 dataSize,
                                 UInt8 *                similarity,
                                 ICMCompletionProcRecordPtr  asyncCompletionProc)           TWOWORDINLINE(0x700B, 0xAAA3);

EXTERN_API( OSErr )
DecompressSequenceBegin         (ImageSequence *        seqID,
                                 ImageDescriptionHandle  desc,
                                 CGrafPtr               port,
                                 GDHandle               gdh,
                                 const Rect *           srcRect,
                                 MatrixRecordPtr        matrix,
                                 short                  mode,
                                 RgnHandle              mask,
                                 CodecFlags             flags,
                                 CodecQ                 accuracy,
                                 DecompressorComponent  codec)                              TWOWORDINLINE(0x700D, 0xAAA3);

EXTERN_API( OSErr )
DecompressSequenceBeginS        (ImageSequence *        seqID,
                                 ImageDescriptionHandle  desc,
                                 Ptr                    data,
                                 long                   dataSize,
                                 CGrafPtr               port,
                                 GDHandle               gdh,
                                 const Rect *           srcRect,
                                 MatrixRecordPtr        matrix,
                                 short                  mode,
                                 RgnHandle              mask,
                                 CodecFlags             flags,
                                 CodecQ                 accuracy,
                                 DecompressorComponent  codec)                              FOURWORDINLINE(0x203C, 0x0030, 0x005D, 0xAAA3);

EXTERN_API( OSErr )
DecompressSequenceFrame         (ImageSequence          seqID,
                                 Ptr                    data,
                                 CodecFlags             inFlags,
                                 CodecFlags *           outFlags,
                                 ICMCompletionProcRecordPtr  asyncCompletionProc)           TWOWORDINLINE(0x700E, 0xAAA3);

EXTERN_API( OSErr )
DecompressSequenceFrameS        (ImageSequence          seqID,
                                 Ptr                    data,
                                 long                   dataSize,
                                 CodecFlags             inFlags,
                                 CodecFlags *           outFlags,
                                 ICMCompletionProcRecordPtr  asyncCompletionProc)           FOURWORDINLINE(0x203C, 0x0016, 0x0047, 0xAAA3);

EXTERN_API( OSErr )
DecompressSequenceFrameWhen     (ImageSequence          seqID,
                                 Ptr                    data,
                                 long                   dataSize,
                                 CodecFlags             inFlags,
                                 CodecFlags *           outFlags,
                                 ICMCompletionProcRecordPtr  asyncCompletionProc,
                                 const ICMFrameTimeRecord * frameTime)                      FOURWORDINLINE(0x203C, 0x001A, 0x005E, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceFlush                 (ImageSequence          seqID)                              FOURWORDINLINE(0x203C, 0x0004, 0x005F, 0xAAA3);

EXTERN_API( OSErr )
SetDSequenceMatrix              (ImageSequence          seqID,
                                 MatrixRecordPtr        matrix)                             TWOWORDINLINE(0x7010, 0xAAA3);

EXTERN_API( OSErr )
GetDSequenceMatrix              (ImageSequence          seqID,
                                 MatrixRecordPtr        matrix)                             FOURWORDINLINE(0x203C, 0x0008, 0x0091, 0xAAA3);

EXTERN_API( OSErr )
SetDSequenceMatte               (ImageSequence          seqID,
                                 PixMapHandle           matte,
                                 const Rect *           matteRect)                          TWOWORDINLINE(0x7011, 0xAAA3);

EXTERN_API( OSErr )
SetDSequenceMask                (ImageSequence          seqID,
                                 RgnHandle              mask)                               TWOWORDINLINE(0x7012, 0xAAA3);

EXTERN_API( OSErr )
SetDSequenceTransferMode        (ImageSequence          seqID,
                                 short                  mode,
                                 const RGBColor *       opColor)                            TWOWORDINLINE(0x7013, 0xAAA3);

EXTERN_API( OSErr )
SetDSequenceDataProc            (ImageSequence          seqID,
                                 ICMDataProcRecordPtr   dataProc,
                                 long                   bufferSize)                         TWOWORDINLINE(0x7014, 0xAAA3);

EXTERN_API( OSErr )
SetDSequenceAccuracy            (ImageSequence          seqID,
                                 CodecQ                 accuracy)                           TWOWORDINLINE(0x7034, 0xAAA3);

EXTERN_API( OSErr )
SetDSequenceSrcRect             (ImageSequence          seqID,
                                 const Rect *           srcRect)                            TWOWORDINLINE(0x7035, 0xAAA3);

EXTERN_API( OSErr )
SetDSequenceFlags               (ImageSequence          seqID,
                                 long                   flags,
                                 long                   flagsMask)                          FOURWORDINLINE(0x203C, 0x000C, 0x0057, 0xAAA3);

enum {
    codecDSequenceDisableOverlaySurface = (1L << 5),
    codecDSequenceSingleField   = (1L << 6),
    codecDSequenceBidirectionalPrediction = (1L << 7),
    codecDSequenceFlushInsteadOfDirtying = (1L << 8),
    codecDSequenceEnableSubPixelPositioning = (1L << 9)
};

typedef CodecComponent *                CodecComponentPtr;
typedef CodecComponentPtr *             CodecComponentHandle;
/* selectors for ICMSequenceGet/SetInfo*/
enum {
    kICMSequenceTaskWeight      = FOUR_CHAR_CODE('twei'),       /* data is pointer to UInt32*/
    kICMSequenceTaskName        = FOUR_CHAR_CODE('tnam'),       /* data is pointer to OSType*/
    kICMSequenceUserPreferredCodecs = FOUR_CHAR_CODE('punt')    /* data is pointer to CodecComponentHandle*/
};


EXTERN_API( OSErr )
ICMSequenceGetInfo              (ImageSequence          seqID,
                                 OSType                 which,
                                 void *                 data)                               FOURWORDINLINE(0x203C, 0x000C, 0x0089, 0xAAA3);

EXTERN_API( OSErr )
ICMSequenceSetInfo              (ImageSequence          seqID,
                                 OSType                 which,
                                 void *                 data,
                                 Size                   dataSize)                           FOURWORDINLINE(0x203C, 0x0010, 0x009D, 0xAAA3);

EXTERN_API( OSErr )
GetDSequenceImageBuffer         (ImageSequence          seqID,
                                 GWorldPtr *            gworld)                             TWOWORDINLINE(0x7015, 0xAAA3);

EXTERN_API( OSErr )
GetDSequenceScreenBuffer        (ImageSequence          seqID,
                                 GWorldPtr *            gworld)                             TWOWORDINLINE(0x7016, 0xAAA3);

EXTERN_API( OSErr )
SetCSequenceQuality             (ImageSequence          seqID,
                                 CodecQ                 spatialQuality,
                                 CodecQ                 temporalQuality)                    TWOWORDINLINE(0x7017, 0xAAA3);

EXTERN_API( OSErr )
SetCSequencePrev                (ImageSequence          seqID,
                                 PixMapHandle           prev,
                                 const Rect *           prevRect)                           TWOWORDINLINE(0x7018, 0xAAA3);

EXTERN_API( OSErr )
SetCSequenceFlushProc           (ImageSequence          seqID,
                                 ICMFlushProcRecordPtr  flushProc,
                                 long                   bufferSize)                         TWOWORDINLINE(0x7033, 0xAAA3);

EXTERN_API( OSErr )
SetCSequenceKeyFrameRate        (ImageSequence          seqID,
                                 long                   keyFrameRate)                       TWOWORDINLINE(0x7036, 0xAAA3);

EXTERN_API( OSErr )
GetCSequenceKeyFrameRate        (ImageSequence          seqID,
                                 long *                 keyFrameRate)                       FOURWORDINLINE(0x203C, 0x0008, 0x004B, 0xAAA3);

EXTERN_API( OSErr )
GetCSequencePrevBuffer          (ImageSequence          seqID,
                                 GWorldPtr *            gworld)                             TWOWORDINLINE(0x7019, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceBusy                  (ImageSequence          seqID)                              TWOWORDINLINE(0x701A, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceEnd                   (ImageSequence          seqID)                              TWOWORDINLINE(0x701B, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceEquivalentImageDescription (ImageSequence     seqID,
                                 ImageDescriptionHandle  newDesc,
                                 Boolean *              equivalent)                         FOURWORDINLINE(0x203C, 0x000C, 0x0065, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceEquivalentImageDescriptionS (ImageSequence    seqID,
                                 ImageDescriptionHandle  newDesc,
                                 Boolean *              equivalent,
                                 Boolean *              canSwitch)                          FOURWORDINLINE(0x203C, 0x0010, 0x009F, 0xAAA3);

EXTERN_API( OSErr )
ReplaceDSequenceImageDescription (ImageSequence         seqID,
                                 ImageDescriptionHandle  newDesc)                           FOURWORDINLINE(0x203C, 0x0008, 0x00A0, 0xAAA3);

EXTERN_API( OSErr )
GetCompressedImageSize          (ImageDescriptionHandle  desc,
                                 Ptr                    data,
                                 long                   bufferSize,
                                 ICMDataProcRecordPtr   dataProc,
                                 long *                 dataSize)                           TWOWORDINLINE(0x701C, 0xAAA3);

EXTERN_API( OSErr )
GetSimilarity                   (PixMapHandle           src,
                                 const Rect *           srcRect,
                                 ImageDescriptionHandle  desc,
                                 Ptr                    data,
                                 Fixed *                similarity)                         TWOWORDINLINE(0x701D, 0xAAA3);

enum {
    kImageDescriptionSampleFormat = FOUR_CHAR_CODE('idfm'),     /* image description extension describing sample format*/
    kImageDescriptionClassicAtomFormat = FOUR_CHAR_CODE('atom'), /* sample contains classic atom structure (ie, GX codec and Curve codec)*/
    kImageDescriptionQTAtomFormat = FOUR_CHAR_CODE('qtat'),     /* sample contains QT atom structure*/
    kImageDescriptionEffectDataFormat = FOUR_CHAR_CODE('fxat'), /* sample describes an effect (as QTAtoms)*/
    kImageDescriptionPrivateDataFormat = FOUR_CHAR_CODE('priv'), /* sample is in a private codec specific format*/
    kImageDescriptionAlternateCodec = FOUR_CHAR_CODE('subs'),   /* image description extension containing the OSType of a substitute codec should the main codec not be available*/
    kImageDescriptionColorSpace = FOUR_CHAR_CODE('cspc')        /* image description extension containing an OSType naming the native pixel format of an image (only used for pixel formats not supported by classic Color QuickDraw)*/
};

EXTERN_API( OSErr )
GetImageDescriptionCTable       (ImageDescriptionHandle  desc,
                                 CTabHandle *           ctable)                             TWOWORDINLINE(0x701E, 0xAAA3);

EXTERN_API( OSErr )
SetImageDescriptionCTable       (ImageDescriptionHandle  desc,
                                 CTabHandle             ctable)                             TWOWORDINLINE(0x701F, 0xAAA3);

EXTERN_API( OSErr )
GetImageDescriptionExtension    (ImageDescriptionHandle  desc,
                                 Handle *               extension,
                                 long                   idType,
                                 long                   index)                              TWOWORDINLINE(0x7020, 0xAAA3);

EXTERN_API( OSErr )
AddImageDescriptionExtension    (ImageDescriptionHandle  desc,
                                 Handle                 extension,
                                 long                   idType)                             TWOWORDINLINE(0x7021, 0xAAA3);

EXTERN_API( OSErr )
RemoveImageDescriptionExtension (ImageDescriptionHandle  desc,
                                 long                   idType,
                                 long                   index)                              FOURWORDINLINE(0x203C, 0x000C, 0x003A, 0xAAA3);

EXTERN_API( OSErr )
CountImageDescriptionExtensionType (ImageDescriptionHandle  desc,
                                 long                   idType,
                                 long *                 count)                              FOURWORDINLINE(0x203C, 0x000C, 0x003B, 0xAAA3);

EXTERN_API( OSErr )
GetNextImageDescriptionExtensionType (ImageDescriptionHandle  desc,
                                 long *                 idType)                             FOURWORDINLINE(0x203C, 0x0008, 0x003C, 0xAAA3);

EXTERN_API( OSErr )
FindCodec                       (CodecType              cType,
                                 CodecComponent         specCodec,
                                 CompressorComponent *  compressor,
                                 DecompressorComponent * decompressor)                      TWOWORDINLINE(0x7023, 0xAAA3);

EXTERN_API( OSErr )
CompressPicture                 (PicHandle              srcPicture,
                                 PicHandle              dstPicture,
                                 CodecQ                 quality,
                                 CodecType              cType)                              TWOWORDINLINE(0x7024, 0xAAA3);

EXTERN_API( OSErr )
FCompressPicture                (PicHandle              srcPicture,
                                 PicHandle              dstPicture,
                                 short                  colorDepth,
                                 CTabHandle             ctable,
                                 CodecQ                 quality,
                                 short                  doDither,
                                 short                  compressAgain,
                                 ICMProgressProcRecordPtr  progressProc,
                                 CodecType              cType,
                                 CompressorComponent    codec)                              TWOWORDINLINE(0x7025, 0xAAA3);

EXTERN_API( OSErr )
CompressPictureFile             (short                  srcRefNum,
                                 short                  dstRefNum,
                                 CodecQ                 quality,
                                 CodecType              cType)                              TWOWORDINLINE(0x7026, 0xAAA3);

EXTERN_API( OSErr )
FCompressPictureFile            (short                  srcRefNum,
                                 short                  dstRefNum,
                                 short                  colorDepth,
                                 CTabHandle             ctable,
                                 CodecQ                 quality,
                                 short                  doDither,
                                 short                  compressAgain,
                                 ICMProgressProcRecordPtr  progressProc,
                                 CodecType              cType,
                                 CompressorComponent    codec)                              TWOWORDINLINE(0x7027, 0xAAA3);

EXTERN_API( OSErr )
GetPictureFileHeader            (short                  refNum,
                                 Rect *                 frame,
                                 OpenCPicParams *       header)                             TWOWORDINLINE(0x7028, 0xAAA3);

EXTERN_API( OSErr )
DrawPictureFile                 (short                  refNum,
                                 const Rect *           frame,
                                 ICMProgressProcRecordPtr  progressProc)                    TWOWORDINLINE(0x7029, 0xAAA3);

EXTERN_API( OSErr )
DrawTrimmedPicture              (PicHandle              srcPicture,
                                 const Rect *           frame,
                                 RgnHandle              trimMask,
                                 short                  doDither,
                                 ICMProgressProcRecordPtr  progressProc)                    TWOWORDINLINE(0x702E, 0xAAA3);

EXTERN_API( OSErr )
DrawTrimmedPictureFile          (short                  srcRefnum,
                                 const Rect *           frame,
                                 RgnHandle              trimMask,
                                 short                  doDither,
                                 ICMProgressProcRecordPtr  progressProc)                    TWOWORDINLINE(0x702F, 0xAAA3);

EXTERN_API( OSErr )
MakeThumbnailFromPicture        (PicHandle              picture,
                                 short                  colorDepth,
                                 PicHandle              thumbnail,
                                 ICMProgressProcRecordPtr  progressProc)                    TWOWORDINLINE(0x702A, 0xAAA3);

EXTERN_API( OSErr )
MakeThumbnailFromPictureFile    (short                  refNum,
                                 short                  colorDepth,
                                 PicHandle              thumbnail,
                                 ICMProgressProcRecordPtr  progressProc)                    TWOWORDINLINE(0x702B, 0xAAA3);

EXTERN_API( OSErr )
MakeThumbnailFromPixMap         (PixMapHandle           src,
                                 const Rect *           srcRect,
                                 short                  colorDepth,
                                 PicHandle              thumbnail,
                                 ICMProgressProcRecordPtr  progressProc)                    TWOWORDINLINE(0x702C, 0xAAA3);

EXTERN_API( OSErr )
TrimImage                       (ImageDescriptionHandle  desc,
                                 Ptr                    inData,
                                 long                   inBufferSize,
                                 ICMDataProcRecordPtr   dataProc,
                                 Ptr                    outData,
                                 long                   outBufferSize,
                                 ICMFlushProcRecordPtr  flushProc,
                                 Rect *                 trimRect,
                                 ICMProgressProcRecordPtr  progressProc)                    TWOWORDINLINE(0x702D, 0xAAA3);

EXTERN_API( OSErr )
ConvertImage                    (ImageDescriptionHandle  srcDD,
                                 Ptr                    srcData,
                                 short                  colorDepth,
                                 CTabHandle             ctable,
                                 CodecQ                 accuracy,
                                 CodecQ                 quality,
                                 CodecType              cType,
                                 CodecComponent         codec,
                                 ImageDescriptionHandle  dstDD,
                                 Ptr                    dstData)                            TWOWORDINLINE(0x7030, 0xAAA3);

EXTERN_API( OSErr )
GetCompressedPixMapInfo         (PixMapPtr              pix,
                                 ImageDescriptionHandle * desc,
                                 Ptr *                  data,
                                 long *                 bufferSize,
                                 ICMDataProcRecord *    dataProc,
                                 ICMProgressProcRecord * progressProc)                      TWOWORDINLINE(0x7037, 0xAAA3);

EXTERN_API( OSErr )
SetCompressedPixMapInfo         (PixMapPtr              pix,
                                 ImageDescriptionHandle  desc,
                                 Ptr                    data,
                                 long                   bufferSize,
                                 ICMDataProcRecordPtr   dataProc,
                                 ICMProgressProcRecordPtr  progressProc)                    TWOWORDINLINE(0x7038, 0xAAA3);

EXTERN_API( void )
StdPix                          (PixMapPtr              src,
                                 const Rect *           srcRect,
                                 MatrixRecordPtr        matrix,
                                 short                  mode,
                                 RgnHandle              mask,
                                 PixMapPtr              matte,
                                 const Rect *           matteRect,
                                 short                  flags)                              TWOWORDINLINE(0x700C, 0xAAA3);

EXTERN_API( OSErr )
TransformRgn                    (MatrixRecordPtr        matrix,
                                 RgnHandle              rgn)                                TWOWORDINLINE(0x7039, 0xAAA3);

/***********
    preview stuff
***********/
EXTERN_API( void )
SFGetFilePreview                (Point                  where,
                                 ConstStr255Param       prompt,
                                 FileFilterUPP          fileFilter,
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList,
                                 DlgHookUPP             dlgHook,
                                 SFReply *              reply)                              TWOWORDINLINE(0x7041, 0xAAA3);

EXTERN_API( void )
SFPGetFilePreview               (Point                  where,
                                 ConstStr255Param       prompt,
                                 FileFilterUPP          fileFilter,
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList,
                                 DlgHookUPP             dlgHook,
                                 SFReply *              reply,
                                 short                  dlgID,
                                 ModalFilterUPP         filterProc)                         TWOWORDINLINE(0x7042, 0xAAA3);

EXTERN_API( void )
StandardGetFilePreview          (FileFilterUPP          fileFilter,
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList,
                                 StandardFileReply *    reply)                              TWOWORDINLINE(0x7043, 0xAAA3);

EXTERN_API( void )
CustomGetFilePreview            (FileFilterYDUPP        fileFilter,
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList,
                                 StandardFileReply *    reply,
                                 short                  dlgID,
                                 Point                  where,
                                 DlgHookYDUPP           dlgHook,
                                 ModalFilterYDUPP       filterProc,
                                 ActivationOrderListPtr  activeList,
                                 ActivateYDUPP          activateProc,
                                 void *                 yourDataPtr)                        TWOWORDINLINE(0x7044, 0xAAA3);

EXTERN_API( OSErr )
MakeFilePreview                 (short                  resRefNum,
                                 ICMProgressProcRecordPtr  progress)                        TWOWORDINLINE(0x7045, 0xAAA3);

EXTERN_API( OSErr )
AddFilePreview                  (short                  resRefNum,
                                 OSType                 previewType,
                                 Handle                 previewData)                        TWOWORDINLINE(0x7046, 0xAAA3);

enum {
    sfpItemPreviewAreaUser      = 11,
    sfpItemPreviewStaticText    = 12,
    sfpItemPreviewDividerUser   = 13,
    sfpItemCreatePreviewButton  = 14,
    sfpItemShowPreviewButton    = 15
};


struct PreviewResourceRecord {
    unsigned long                   modDate;
    short                           version;
    OSType                          resType;
    short                           resID;
};
typedef struct PreviewResourceRecord    PreviewResourceRecord;

typedef PreviewResourceRecord *         PreviewResourcePtr;
typedef PreviewResourcePtr *            PreviewResource;
EXTERN_API( void )
AlignScreenRect                 (Rect *                 rp,
                                 ICMAlignmentProcRecordPtr  alignmentProc)                  FOURWORDINLINE(0x203C, 0x0008, 0x004C, 0xAAA3);

EXTERN_API( void )
AlignWindow                     (WindowRef              wp,
                                 Boolean                front,
                                 const Rect *           alignmentRect,
                                 ICMAlignmentProcRecordPtr  alignmentProc)                  FOURWORDINLINE(0x203C, 0x000E, 0x004D, 0xAAA3);

EXTERN_API( void )
DragAlignedWindow               (WindowRef              wp,
                                 Point                  startPt,
                                 Rect *                 boundsRect,
                                 Rect *                 alignmentRect,
                                 ICMAlignmentProcRecordPtr  alignmentProc)                  FOURWORDINLINE(0x203C, 0x0014, 0x004E, 0xAAA3);

EXTERN_API( long )
DragAlignedGrayRgn              (RgnHandle              theRgn,
                                 Point                  startPt,
                                 Rect *                 boundsRect,
                                 Rect *                 slopRect,
                                 short                  axis,
                                 UniversalProcPtr       actionProc,
                                 Rect *                 alignmentRect,
                                 ICMAlignmentProcRecordPtr  alignmentProc)                  FOURWORDINLINE(0x203C, 0x001E, 0x004F, 0xAAA3);

EXTERN_API( OSErr )
SetCSequenceDataRateParams      (ImageSequence          seqID,
                                 DataRateParamsPtr      params)                             FOURWORDINLINE(0x203C, 0x0008, 0x0050, 0xAAA3);

EXTERN_API( OSErr )
SetCSequenceFrameNumber         (ImageSequence          seqID,
                                 long                   frameNumber)                        FOURWORDINLINE(0x203C, 0x0008, 0x0051, 0xAAA3);

EXTERN_API( OSErr )
SetCSequencePreferredPacketSize (ImageSequence          seqID,
                                 long                   preferredPacketSizeInBytes)         FOURWORDINLINE(0x203C, 0x0008, 0x0078, 0xAAA3);

EXTERN_API( OSErr )
NewImageGWorld                  (GWorldPtr *            gworld,
                                 ImageDescriptionHandle  idh,
                                 GWorldFlags            flags)                              FOURWORDINLINE(0x203C, 0x000C, 0x0052, 0xAAA3);

EXTERN_API( OSErr )
GetCSequenceDataRateParams      (ImageSequence          seqID,
                                 DataRateParamsPtr      params)                             FOURWORDINLINE(0x203C, 0x0008, 0x0053, 0xAAA3);

EXTERN_API( OSErr )
GetCSequenceFrameNumber         (ImageSequence          seqID,
                                 long *                 frameNumber)                        FOURWORDINLINE(0x203C, 0x0008, 0x0054, 0xAAA3);

EXTERN_API( OSErr )
GetBestDeviceRect               (GDHandle *             gdh,
                                 Rect *                 rp)                                 FOURWORDINLINE(0x203C, 0x0008, 0x0055, 0xAAA3);

EXTERN_API( OSErr )
SetSequenceProgressProc         (ImageSequence          seqID,
                                 ICMProgressProcRecord * progressProc)                      FOURWORDINLINE(0x203C, 0x0008, 0x0056, 0xAAA3);

EXTERN_API( OSErr )
GDHasScale                      (GDHandle               gdh,
                                 short                  depth,
                                 Fixed *                scale)                              FOURWORDINLINE(0x203C, 0x000A, 0x005A, 0xAAA3);

EXTERN_API( OSErr )
GDGetScale                      (GDHandle               gdh,
                                 Fixed *                scale,
                                 short *                flags)                              FOURWORDINLINE(0x203C, 0x000C, 0x005B, 0xAAA3);

EXTERN_API( OSErr )
GDSetScale                      (GDHandle               gdh,
                                 Fixed                  scale,
                                 short                  flags)                              FOURWORDINLINE(0x203C, 0x000A, 0x005C, 0xAAA3);

EXTERN_API( OSErr )
ICMShieldSequenceCursor         (ImageSequence          seqID)                              FOURWORDINLINE(0x203C, 0x0004, 0x0062, 0xAAA3);

EXTERN_API( void )
ICMDecompressComplete           (ImageSequence          seqID,
                                 OSErr                  err,
                                 short                  flag,
                                 ICMCompletionProcRecordPtr  completionRtn)                 FOURWORDINLINE(0x203C, 0x000C, 0x0063, 0xAAA3);

EXTERN_API( OSErr )
ICMDecompressCompleteS          (ImageSequence          seqID,
                                 OSErr                  err,
                                 short                  flag,
                                 ICMCompletionProcRecordPtr  completionRtn)                 FOURWORDINLINE(0x203C, 0x000C, 0x0082, 0xAAA3);

EXTERN_API( OSErr )
ICMSequenceLockBits             (ImageSequence          seqID,
                                 PixMapPtr              dst,
                                 long                   flags)                              FOURWORDINLINE(0x203C, 0x000C, 0x007C, 0xAAA3);

EXTERN_API( OSErr )
ICMSequenceUnlockBits           (ImageSequence          seqID,
                                 long                   flags)                              FOURWORDINLINE(0x203C, 0x0008, 0x007D, 0xAAA3);

enum {
    kICMPixelFormatIsPlanarMask = 0x0F,                         /* these bits in formatFlags indicate how many planes there are; they're 0 if chunky*/
    kICMPixelFormatIsIndexed    = (1L << 4),
    kICMPixelFormatIsSupportedByQD = (1L << 5),
    kICMPixelFormatIsMonochrome = (1L << 6),
    kICMPixelFormatHasAlphaChannel = (1L << 7)
};


struct ICMPixelFormatInfo {
    long                            size;                       /* caller MUST fill this in with sizeof(ICMPixelFormatInfo) before calling ICMGet/SetPixelFormatInfo*/
    unsigned long                   formatFlags;
    short                           bitsPerPixel[14];           /* list each plane's bits per pixel separately if planar*/
                                                                /* new field for QuickTime 4.1*/
    Fixed                           defaultGammaLevel;
                                                                /* new fields for QuickTime 6.0*/
    short                           horizontalSubsampling[14];  /* per plane; use 1 if plane is not subsampled*/
    short                           verticalSubsampling[14];    /* per plane; use 1 if plane is not subsampled*/
};
typedef struct ICMPixelFormatInfo       ICMPixelFormatInfo;
typedef ICMPixelFormatInfo *            ICMPixelFormatInfoPtr;
/* IMPORTANT: Fill in theInfo->size with sizeof(ICMPixelFormatInfo) before calling ICMGetPixelFormatInfo */
EXTERN_API( OSErr )
ICMGetPixelFormatInfo           (OSType                 PixelFormat,
                                 ICMPixelFormatInfoPtr  theInfo)                            FOURWORDINLINE(0x203C, 0x0008, 0x0083, 0xAAA3);

/* IMPORTANT: Fill in theInfo->size with sizeof(ICMPixelFormatInfo) before calling ICMSetPixelFormatInfo */
EXTERN_API( OSErr )
ICMSetPixelFormatInfo           (OSType                 PixelFormat,
                                 ICMPixelFormatInfoPtr  theInfo)                            FOURWORDINLINE(0x203C, 0x0008, 0x008A, 0xAAA3);

enum {
    kICMGetChainUltimateParent  = 0,
    kICMGetChainParent          = 1,
    kICMGetChainChild           = 2,
    kICMGetChainUltimateChild   = 3
};

EXTERN_API( OSErr )
ICMSequenceGetChainMember       (ImageSequence          seqID,
                                 ImageSequence *        retSeqID,
                                 long                   flags)                              FOURWORDINLINE(0x203C, 0x000C, 0x007E, 0xAAA3);

EXTERN_API( OSErr )
SetDSequenceTimeCode            (ImageSequence          seqID,
                                 void *                 timeCodeFormat,
                                 void *                 timeCodeTime)                       FOURWORDINLINE(0x203C, 0x000C, 0x0064, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceNewMemory             (ImageSequence          seqID,
                                 Ptr *                  data,
                                 Size                   dataSize,
                                 long                   dataUse,
                                 ICMMemoryDisposedUPP   memoryGoneProc,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x0018, 0x0066, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceDisposeMemory         (ImageSequence          seqID,
                                 Ptr                    data)                               FOURWORDINLINE(0x203C, 0x0008, 0x0067, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceNewDataSource         (ImageSequence          seqID,
                                 ImageSequenceDataSource * sourceID,
                                 OSType                 sourceType,
                                 long                   sourceInputNumber,
                                 Handle                 dataDescription,
                                 ICMConvertDataFormatUPP  transferProc,
                                 void *                 refCon)                             FOURWORDINLINE(0x203C, 0x001C, 0x0068, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceDisposeDataSource     (ImageSequenceDataSource  sourceID)                         FOURWORDINLINE(0x203C, 0x0004, 0x0069, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceSetSourceData         (ImageSequenceDataSource  sourceID,
                                 void *                 data,
                                 long                   dataSize)                           FOURWORDINLINE(0x203C, 0x000C, 0x006A, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceChangedSourceData     (ImageSequenceDataSource  sourceID)                         FOURWORDINLINE(0x203C, 0x0004, 0x006B, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceSetSourceDataQueue    (ImageSequenceDataSource  sourceID,
                                 QHdrPtr                dataQueue)                          FOURWORDINLINE(0x203C, 0x0008, 0x007B, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceGetDataSource         (ImageSequence          seqID,
                                 ImageSequenceDataSource * sourceID,
                                 OSType                 sourceType,
                                 long                   sourceInputNumber)                  FOURWORDINLINE(0x203C, 0x0010, 0x007F, 0xAAA3);

EXTERN_API( OSErr )
PtInDSequenceData               (ImageSequence          seqID,
                                 void *                 data,
                                 Size                   dataSize,
                                 Point                  where,
                                 Boolean *              hit)                                FOURWORDINLINE(0x203C, 0x0014, 0x006C, 0xAAA3);

EXTERN_API( OSErr )
HitTestDSequenceData            (ImageSequence          seqID,
                                 void *                 data,
                                 Size                   dataSize,
                                 Point                  where,
                                 long *                 hit,
                                 long                   hitFlags)                           FOURWORDINLINE(0x203C, 0x0018, 0x0087, 0xAAA3);

EXTERN_API( OSErr )
GetGraphicsImporterForFile      (const FSSpec *         theFile,
                                 ComponentInstance *    gi)                                 FOURWORDINLINE(0x203C, 0x0008, 0x006E, 0xAAA3);

EXTERN_API( OSErr )
GetGraphicsImporterForDataRef   (Handle                 dataRef,
                                 OSType                 dataRefType,
                                 ComponentInstance *    gi)                                 FOURWORDINLINE(0x203C, 0x000C, 0x0077, 0xAAA3);

enum {
    kDontUseValidateToFindGraphicsImporter = 1L << 0
};

EXTERN_API( OSErr )
GetGraphicsImporterForFileWithFlags (const FSSpec *     theFile,
                                 ComponentInstance *    gi,
                                 long                   flags)                              FOURWORDINLINE(0x203C, 0x000C, 0x0084, 0xAAA3);

EXTERN_API( OSErr )
GetGraphicsImporterForDataRefWithFlags (Handle          dataRef,
                                 OSType                 dataRefType,
                                 ComponentInstance *    gi,
                                 long                   flags)                              FOURWORDINLINE(0x203C, 0x0010, 0x0085, 0xAAA3);

EXTERN_API( OSErr )
QTGetFileNameExtension          (ConstStrFileNameParam  fileName,
                                 OSType                 fileType,
                                 OSType *               extension)                          FOURWORDINLINE(0x203C, 0x000C, 0x0061, 0xAAA3);


EXTERN_API( OSErr )
ImageTranscodeSequenceBegin     (ImageTranscodeSequence * its,
                                 ImageDescriptionHandle  srcDesc,
                                 OSType                 destType,
                                 ImageDescriptionHandle * dstDesc,
                                 void *                 data,
                                 long                   dataSize)                           FOURWORDINLINE(0x203C, 0x0018, 0x006F, 0xAAA3);

EXTERN_API( OSErr )
ImageTranscodeSequenceEnd       (ImageTranscodeSequence  its)                               FOURWORDINLINE(0x203C, 0x0004, 0x0070, 0xAAA3);

EXTERN_API( OSErr )
ImageTranscodeFrame             (ImageTranscodeSequence  its,
                                 void *                 srcData,
                                 long                   srcDataSize,
                                 void **                dstData,
                                 long *                 dstDataSize)                        FOURWORDINLINE(0x203C, 0x0014, 0x0071, 0xAAA3);

EXTERN_API( OSErr )
ImageTranscodeDisposeFrameData  (ImageTranscodeSequence  its,
                                 void *                 dstData)                            FOURWORDINLINE(0x203C, 0x0008, 0x0072, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceInvalidate            (ImageSequence          seqID,
                                 RgnHandle              invalRgn)                           FOURWORDINLINE(0x203C, 0x0008, 0x0073, 0xAAA3);

EXTERN_API( OSErr )
CDSequenceSetTimeBase           (ImageSequence          seqID,
                                 void *                 base)                               FOURWORDINLINE(0x203C, 0x0008, 0x0079, 0xAAA3);

EXTERN_API( OSErr )
ImageFieldSequenceBegin         (ImageFieldSequence *   ifs,
                                 ImageDescriptionHandle  desc1,
                                 ImageDescriptionHandle  desc2,
                                 ImageDescriptionHandle  descOut)                           FOURWORDINLINE(0x203C, 0x0010, 0x006D, 0xAAA3);

EXTERN_API( OSErr )
ImageFieldSequenceExtractCombine (ImageFieldSequence    ifs,
                                 long                   fieldFlags,
                                 void *                 data1,
                                 long                   dataSize1,
                                 void *                 data2,
                                 long                   dataSize2,
                                 void *                 outputData,
                                 long *                 outDataSize)                        FOURWORDINLINE(0x203C, 0x0020, 0x0075, 0xAAA3);

EXTERN_API( OSErr )
ImageFieldSequenceEnd           (ImageFieldSequence     ifs)                                FOURWORDINLINE(0x203C, 0x0004, 0x0076, 0xAAA3);

enum {
    kICMTempThenAppMemory       = 1L << 12,
    kICMAppThenTempMemory       = 1L << 13
};

EXTERN_API( OSErr )
QTNewGWorld                     (GWorldPtr *            offscreenGWorld,
                                 OSType                 PixelFormat,
                                 const Rect *           boundsRect,
                                 CTabHandle             cTable,
                                 GDHandle               aGDevice,
                                 GWorldFlags            flags)                              FOURWORDINLINE(0x203C, 0x0018, 0x0080, 0xAAA3);

EXTERN_API( OSErr )
QTNewGWorldFromPtr              (GWorldPtr *            gw,
                                 OSType                 pixelFormat,
                                 const Rect *           boundsRect,
                                 CTabHandle             cTable,
                                 GDHandle               aGDevice,
                                 GWorldFlags            flags,
                                 void *                 baseAddr,
                                 long                   rowBytes)                           FOURWORDINLINE(0x203C, 0x0020, 0x008B, 0xAAA3);

EXTERN_API( GWorldFlags )
QTUpdateGWorld                  (GWorldPtr *            offscreenGWorld,
                                 OSType                 PixelFormat,
                                 const Rect *           boundsRect,
                                 CTabHandle             cTable,
                                 GDHandle               aGDevice,
                                 GWorldFlags            flags)                              FOURWORDINLINE(0x203C, 0x0018, 0x0081, 0xAAA3);

EXTERN_API( OSErr )
MakeImageDescriptionForPixMap   (PixMapHandle           pixmap,
                                 ImageDescriptionHandle * idh)                              FOURWORDINLINE(0x203C, 0x0008, 0x007A, 0xAAA3);

EXTERN_API( OSErr )
MakeImageDescriptionForEffect   (OSType                 effectType,
                                 ImageDescriptionHandle * idh)                              FOURWORDINLINE(0x203C, 0x0008, 0x0092, 0xAAA3);

EXTERN_API( short )
QTGetPixelSize                  (OSType                 PixelFormat)                        FOURWORDINLINE(0x203C, 0x0004, 0x0086, 0xAAA3);

EXTERN_API( short )
QTGetPixelFormatDepthForImageDescription (OSType        pixelFormat)                        FOURWORDINLINE(0x203C, 0x0004, 0x00A3, 0xAAA3);

EXTERN_API( long )
QTGetPixMapPtrRowBytes          (PixMapPtr              pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x008D, 0xAAA3);

EXTERN_API( long )
QTGetPixMapHandleRowBytes       (PixMapHandle           pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x008E, 0xAAA3);

EXTERN_API( OSErr )
QTSetPixMapPtrRowBytes          (PixMapPtr              pm,
                                 long                   rowBytes)                           FOURWORDINLINE(0x203C, 0x0008, 0x0090, 0xAAA3);

EXTERN_API( OSErr )
QTSetPixMapHandleRowBytes       (PixMapHandle           pm,
                                 long                   rowBytes)                           FOURWORDINLINE(0x203C, 0x0008, 0x008F, 0xAAA3);

enum {
    kQTUsePlatformDefaultGammaLevel = 0,                        /* When decompressing into this PixMap, gamma-correct to the platform's standard gamma. */
    kQTUseSourceGammaLevel      = -1L,                          /* When decompressing into this PixMap, don't perform gamma-correction. */
    kQTCCIR601VideoGammaLevel   = 0x00023333                    /* 2.2, standard television video gamma.*/
};

EXTERN_API( Fixed )
QTGetPixMapPtrGammaLevel        (PixMapPtr              pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x0093, 0xAAA3);

EXTERN_API( OSErr )
QTSetPixMapPtrGammaLevel        (PixMapPtr              pm,
                                 Fixed                  gammaLevel)                         FOURWORDINLINE(0x203C, 0x0008, 0x0094, 0xAAA3);

EXTERN_API( Fixed )
QTGetPixMapHandleGammaLevel     (PixMapHandle           pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x0096, 0xAAA3);

EXTERN_API( OSErr )
QTSetPixMapHandleGammaLevel     (PixMapHandle           pm,
                                 Fixed                  gammaLevel)                         FOURWORDINLINE(0x203C, 0x0008, 0x0097, 0xAAA3);

EXTERN_API( Fixed )
QTGetPixMapPtrRequestedGammaLevel (PixMapPtr            pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x0098, 0xAAA3);

EXTERN_API( OSErr )
QTSetPixMapPtrRequestedGammaLevel (PixMapPtr            pm,
                                 Fixed                  requestedGammaLevel)                FOURWORDINLINE(0x203C, 0x0008, 0x0099, 0xAAA3);

EXTERN_API( Fixed )
QTGetPixMapHandleRequestedGammaLevel (PixMapHandle      pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x009A, 0xAAA3);

EXTERN_API( OSErr )
QTSetPixMapHandleRequestedGammaLevel (PixMapHandle      pm,
                                 Fixed                  requestedGammaLevel)                FOURWORDINLINE(0x203C, 0x0008, 0x009B, 0xAAA3);

EXTERN_API( OSErr )
QuadToQuadMatrix                (const Fixed *          source,
                                 const Fixed *          dest,
                                 MatrixRecord *         map)                                FOURWORDINLINE(0x203C, 0x000C, 0x0095, 0xAAA3);





enum {
    identityMatrixType          = 0x00,                         /* result if matrix is identity */
    translateMatrixType         = 0x01,                         /* result if matrix translates */
    scaleMatrixType             = 0x02,                         /* result if matrix scales */
    scaleTranslateMatrixType    = 0x03,                         /* result if matrix scales and translates */
    linearMatrixType            = 0x04,                         /* result if matrix is general 2 x 2 */
    linearTranslateMatrixType   = 0x05,                         /* result if matrix is general 2 x 2 and translates */
    perspectiveMatrixType       = 0x06                          /* result if matrix is general 3 x 3 */
};


typedef unsigned short                  MatrixFlags;
EXTERN_API( short )
GetMatrixType                   (const MatrixRecord *   m)                                  TWOWORDINLINE(0x7014, 0xABC2);

EXTERN_API( void )
CopyMatrix                      (const MatrixRecord *   m1,
                                 MatrixRecord *         m2)                                 TWOWORDINLINE(0x7020, 0xABC2);

EXTERN_API( Boolean )
EqualMatrix                     (const MatrixRecord *   m1,
                                 const MatrixRecord *   m2)                                 TWOWORDINLINE(0x7021, 0xABC2);

EXTERN_API( void )
SetIdentityMatrix               (MatrixRecord *         matrix)                             TWOWORDINLINE(0x7015, 0xABC2);

EXTERN_API( void )
TranslateMatrix                 (MatrixRecord *         m,
                                 Fixed                  deltaH,
                                 Fixed                  deltaV)                             TWOWORDINLINE(0x7019, 0xABC2);

EXTERN_API( void )
RotateMatrix                    (MatrixRecord *         m,
                                 Fixed                  degrees,
                                 Fixed                  aboutX,
                                 Fixed                  aboutY)                             TWOWORDINLINE(0x7016, 0xABC2);

EXTERN_API( void )
ScaleMatrix                     (MatrixRecord *         m,
                                 Fixed                  scaleX,
                                 Fixed                  scaleY,
                                 Fixed                  aboutX,
                                 Fixed                  aboutY)                             TWOWORDINLINE(0x7017, 0xABC2);

EXTERN_API( void )
SkewMatrix                      (MatrixRecord *         m,
                                 Fixed                  skewX,
                                 Fixed                  skewY,
                                 Fixed                  aboutX,
                                 Fixed                  aboutY)                             TWOWORDINLINE(0x7018, 0xABC2);

EXTERN_API( OSErr )
TransformFixedPoints            (const MatrixRecord *   m,
                                 FixedPoint *           fpt,
                                 long                   count)                              TWOWORDINLINE(0x7022, 0xABC2);

EXTERN_API( OSErr )
TransformPoints                 (const MatrixRecord *   mp,
                                 Point *                pt1,
                                 long                   count)                              TWOWORDINLINE(0x7023, 0xABC2);

EXTERN_API( Boolean )
TransformFixedRect              (const MatrixRecord *   m,
                                 FixedRect *            fr,
                                 FixedPoint *           fpp)                                TWOWORDINLINE(0x7024, 0xABC2);

EXTERN_API( Boolean )
TransformRect                   (const MatrixRecord *   m,
                                 Rect *                 r,
                                 FixedPoint *           fpp)                                TWOWORDINLINE(0x7025, 0xABC2);

EXTERN_API( Boolean )
InverseMatrix                   (const MatrixRecord *   m,
                                 MatrixRecord *         im)                                 TWOWORDINLINE(0x701C, 0xABC2);

EXTERN_API( void )
ConcatMatrix                    (const MatrixRecord *   a,
                                 MatrixRecord *         b)                                  TWOWORDINLINE(0x701B, 0xABC2);

EXTERN_API( void )
RectMatrix                      (MatrixRecord *         matrix,
                                 const Rect *           srcRect,
                                 const Rect *           dstRect)                            TWOWORDINLINE(0x701E, 0xABC2);

EXTERN_API( void )
MapMatrix                       (MatrixRecord *         matrix,
                                 const Rect *           fromRect,
                                 const Rect *           toRect)                             TWOWORDINLINE(0x701D, 0xABC2);







EXTERN_API( void )
CompAdd                         (wide *                 src,
                                 wide *                 dst)                                TWOWORDINLINE(0x7001, 0xABC2);

EXTERN_API( void )
CompSub                         (wide *                 src,
                                 wide *                 dst)                                TWOWORDINLINE(0x7002, 0xABC2);

EXTERN_API( void )
CompNeg                         (wide *                 dst)                                TWOWORDINLINE(0x7003, 0xABC2);

EXTERN_API( void )
CompShift                       (wide *                 src,
                                 short                  shift)                              TWOWORDINLINE(0x7004, 0xABC2);

EXTERN_API( void )
CompMul                         (long                   src1,
                                 long                   src2,
                                 wide *                 dst)                                TWOWORDINLINE(0x7005, 0xABC2);

EXTERN_API( long )
CompDiv                         (wide *                 numerator,
                                 long                   denominator,
                                 long *                 remainder)                          TWOWORDINLINE(0x7006, 0xABC2);

EXTERN_API( void )
CompFixMul                      (wide *                 compSrc,
                                 Fixed                  fixSrc,
                                 wide *                 compDst)                            TWOWORDINLINE(0x7007, 0xABC2);

EXTERN_API( void )
CompMulDiv                      (wide *                 co,
                                 long                   mul,
                                 long                   divisor)                            TWOWORDINLINE(0x7008, 0xABC2);

EXTERN_API( void )
CompMulDivTrunc                 (wide *                 co,
                                 long                   mul,
                                 long                   divisor,
                                 long *                 remainder)                          TWOWORDINLINE(0x700C, 0xABC2);

EXTERN_API( long )
CompCompare                     (const wide *           a,
                                 const wide *           minusb)                             TWOWORDINLINE(0x7009, 0xABC2);

EXTERN_API( unsigned long )
CompSquareRoot                  (const wide *           src)                                TWOWORDINLINE(0x7011, 0xABC2);

EXTERN_API( Fixed )
FixMulDiv                       (Fixed                  src,
                                 Fixed                  mul,
                                 Fixed                  divisor)                            TWOWORDINLINE(0x700A, 0xABC2);

EXTERN_API( Fixed )
UnsignedFixMulDiv               (Fixed                  src,
                                 Fixed                  mul,
                                 Fixed                  divisor)                            TWOWORDINLINE(0x700D, 0xABC2);

EXTERN_API( Fract )
FracSinCos                      (Fixed                  degree,
                                 Fract *                cosOut)                             TWOWORDINLINE(0x700B, 0xABC2);

EXTERN_API( Fixed )
FixExp2                         (Fixed                  src)                                TWOWORDINLINE(0x700E, 0xABC2);

EXTERN_API( Fixed )
FixLog2                         (Fixed                  src)                                TWOWORDINLINE(0x700F, 0xABC2);

EXTERN_API( Fixed )
FixPow                          (Fixed                  base,
                                 Fixed                  exp)                                TWOWORDINLINE(0x7010, 0xABC2);




typedef ComponentInstance               GraphicsImportComponent;
enum {
    GraphicsImporterComponentType = FOUR_CHAR_CODE('grip')
};

enum {
    graphicsImporterUsesImageDecompressor = 1L << 23
};

enum {
    quickTimeImageFileImageDescriptionAtom = FOUR_CHAR_CODE('idsc'),
    quickTimeImageFileImageDataAtom = FOUR_CHAR_CODE('idat'),
    quickTimeImageFileMetaDataAtom = FOUR_CHAR_CODE('meta'),
    quickTimeImageFileColorSyncProfileAtom = FOUR_CHAR_CODE('iicc')
};

enum {
    graphicsImporterDrawsAllPixels = 0,
    graphicsImporterDoesntDrawAllPixels = 1,
    graphicsImporterDontKnowIfDrawAllPixels = 2
};

/* Flags for GraphicsImportSetFlags */
enum {
    kGraphicsImporterDontDoGammaCorrection = 1L << 0,
    kGraphicsImporterTrustResolutionFromFile = 1L << 1,
    kGraphicsImporterEnableSubPixelPositioning = 1L << 2
};

enum {
    kGraphicsExportGroup        = FOUR_CHAR_CODE('expo'),
    kGraphicsExportFileType     = FOUR_CHAR_CODE('ftyp'),
    kGraphicsExportMIMEType     = FOUR_CHAR_CODE('mime'),
    kGraphicsExportExtension    = FOUR_CHAR_CODE('ext '),
    kGraphicsExportDescription  = FOUR_CHAR_CODE('desc')
};

/* User data types for layers of Photoshop files */
enum {
    kQTPhotoshopLayerMode       = FOUR_CHAR_CODE('lmod'),       /* OSType */
    kQTPhotoshopLayerOpacity    = FOUR_CHAR_CODE('lopa'),       /* UInt8, 0 = transparent .. 255 = opaque */
    kQTPhotoshopLayerClipping   = FOUR_CHAR_CODE('lclp'),       /* UInt8, 0 = base, 1 = non-base */
    kQTPhotoshopLayerFlags      = FOUR_CHAR_CODE('lflg'),       /* UInt8 */
    kQTPhotoshopLayerName       = FOUR_CHAR_CODE(' lnm'),       /* Text */
    kQTPhotoshopLayerUnicodeName = FOUR_CHAR_CODE('luni')       /* Unicode characters, not terminated */
};

/* User data returned by graphics importers to suggest intended use for indexed images */
enum {
    kQTIndexedImageType         = FOUR_CHAR_CODE('nth?'),       /* 1 or more OSTypes, such as the following values: */
    kQTIndexedImageIsThumbnail  = FOUR_CHAR_CODE('n=th'),       /* The image at this index is a thumbnail. */
    kQTIndexedImageIsLayer      = FOUR_CHAR_CODE('n=ly'),       /* The image at this index is a layer. */
    kQTIndexedImageIsPage       = FOUR_CHAR_CODE('n=pg'),       /* The image at this index is a page. */
    kQTIndexedImageIsMultiResolution = FOUR_CHAR_CODE('n=rs')   /* The image at this index is one of several identical images at different resolutions. */
};

/* Other user data types returned by graphics importers */
enum {
    kQTTIFFUserDataPrefix       = 0x74690000,                   /* Added to some tag values in TIFF IFDs to generate user data codes.  (0x7469 is 'ti'.) */
                                                                /* For example, YCbCrPositioning is tag 0x0213, so its user data code is 0x74690213. */
    kQTTIFFExifUserDataPrefix   = 0x65780000,                   /* Added to tag values in Exif IFDs to generate user data codes.  (0x6578 is 'ex'.) */
                                                                /* For example, DateTimeOriginal is tag 0x9003, so its user data code is 0x65789003. */
    kQTTIFFExifGPSUserDataPrefix = 0x67700000,                  /* Added to tag values in Exif GPS IFDs to generate user data codes.  (0x6770 is 'gp'.) */
                                                                /* For example, GPSAltitude is tag 0x0006, so its user data code is 0x6770006. */
    kQTAlphaMode                = FOUR_CHAR_CODE('almo'),       /* UInt32; eg, graphicsModeStraightAlpha or graphicsModePreBlackAlpha */
    kQTAlphaModePreMulColor     = FOUR_CHAR_CODE('almp'),       /* RGBColor; used if kQTAlphaMode is graphicsModePreMulColorAlpha */
    kUserDataIPTC               = FOUR_CHAR_CODE('iptc')
};

/* Found in TIFF and Exif JPEG files */
enum {
    kQTTIFFUserDataOrientation  = 0x74690112,                   /* 1 SHORT */
    kQTTIFFUserDataTransferFunction = 0x7469012D,               /* n SHORTs */
    kQTTIFFUserDataWhitePoint   = 0x7469013E,                   /* 2 RATIONALs */
    kQTTIFFUserDataPrimaryChromaticities = 0x7469013F,          /* 6 RATIONALs */
    kQTTIFFUserDataTransferRange = 0x74690156,                  /* 6 SHORTs */
    kQTTIFFUserDataYCbCrPositioning = 0x74690213,               /* 1 SHORT */
    kQTTIFFUserDataReferenceBlackWhite = 0x74690214             /* n LONGs */
};

/* Found in GeoTIFF files; defined in the GeoTIFF 1.0 spec */
enum {
    kQTTIFFUserDataModelPixelScale = 0x7469830E,                /* 3 DOUBLEs */
    kQTTIFFUserDataModelTransformation = 0x746985D8,            /* 16 DOUBLEs */
    kQTTIFFUserDataModelTiepoint = 0x74698482,                  /* n DOUBLEs */
    kQTTIFFUserDataGeoKeyDirectory = 0x746987AF,                /* n SHORTs */
    kQTTIFFUserDataGeoDoubleParams = 0x746987B0,                /* n DOUBLEs */
    kQTTIFFUserDataGeoAsciiParams = 0x746987B1,                 /* n ASCIIs */
    kQTTIFFUserDataIntergraphMatrix = 0x74698480                /* 16 or 17 DOUBLEs */
};

/* Found in Exif TIFF and Exif JPEG files; defined in the Exif 2.1 spec */
enum {
    kQTExifUserDataExifVersion  = 0x65789000,                   /* 4 bytes (import only) */
    kQTExifUserDataFlashPixVersion = 0x6578A000,                /* 4 bytes */
    kQTExifUserDataColorSpace   = 0x6578A001,                   /* 1 SHORT */
    kQTExifUserDataComponentsConfiguration = 0x65789101,        /* 4 bytes */
    kQTExifUserDataCompressedBitsPerPixel = 0x65789102,         /* 1 RATIONAL */
    kQTExifUserDataPixelXDimension = 0x6578A002,                /* 1 SHORT or LONG */
    kQTExifUserDataPixelYDimension = 0x6578A003,                /* 1 SHORT or LONG */
    kQTExifUserDataMakerNote    = 0x6578927C,                   /* n bytes */
    kQTExifUserDataUserComment  = 0x6578928C,                   /* n bytes */
    kQTExifUserDataRelatedSoundFile = 0x6578A004,               /* 13 ASCIIs*/
    kQTExifUserDataDateTimeOriginal = 0x65789003,               /* 20 ASCIIs */
    kQTExifUserDataDateTimeDigitized = 0x65789004,              /* 20 ASCIIs */
    kQTExifUserDataSubSecTime   = 0x65789290,                   /* n ASCIIs */
    kQTExifUserDataSubSecTimeOriginal = 0x65789291,             /* n ASCIIs */
    kQTExifUserDataSubSecTimeDigitized = 0x65789292,            /* n ASCIIs */
    kQTExifUserDataExposureTime = 0x6578829A,                   /* 1 RATIONAL */
    kQTExifUserDataFNumber      = 0x6578829D,                   /* 1 RATIONAL */
    kQTExifUserDataExposureProgram = 0x65788822,                /* 1 SHORT */
    kQTExifUserDataSpectralSensitivity = 0x65788824,            /* n ASCIIs */
    kQTExifUserDataISOSpeedRatings = 0x65788827,                /* n SHORTs */
    kQTExifUserDataShutterSpeedValue = 0x65789201,              /* 1 SIGNED RATIONAL */
    kQTExifUserDataApertureValue = 0x65789202,                  /* 1 RATIONAL */
    kQTExifUserDataBrightnessValue = 0x65789203,                /* 1 SIGNED RATIONAL */
    kQTExifUserDataExposureBiasValue = 0x65789204,              /* 1 SIGNED RATIONAL */
    kQTExifUserDataMaxApertureValue = 0x65789205,               /* 1 RATIONAL */
    kQTExifUserDataSubjectDistance = 0x65789206,                /* 1 RATIONAL */
    kQTExifUserDataMeteringMode = 0x65789207,                   /* 1 SHORT */
    kQTExifUserDataLightSource  = 0x65789208,                   /* 1 SHORT */
    kQTExifUserDataFlash        = 0x65789209,                   /* 1 SHORT */
    kQTExifUserDataFocalLength  = 0x6578920A,                   /* 1 RATIONAL */
    kQTExifUserDataFlashEnergy  = 0x6578A20B,                   /* 1 RATIONAL */
    kQTExifUserDataFocalPlaneXResolution = 0x6578A20E,          /* 1 RATIONAL */
    kQTExifUserDataFocalPlaneYResolution = 0x6578A20F,          /* 1 RATIONAL */
    kQTExifUserDataFocalPlaneResolutionUnit = 0x6578A210,       /* 1 SHORT */
    kQTExifUserDataSubjectLocation = 0x6578A214,                /* 1 SHORT */
    kQTExifUserDataExposureIndex = 0x6578A215,                  /* 1 RATIONAL */
    kQTExifUserDataSensingMethod = 0x6578A217,                  /* 1 SHORT */
    kQTExifUserDataFileSource   = 0x6578A300,                   /* 1 UNDEFINED */
    kQTExifUserDataSceneType    = 0x6578A301                    /* 1 UNDEFINED */
};

/* Found in some Exif TIFF and Exif JPEG files; defined in the Exif 2.1 spec */
enum {
    kQTExifUserDataGPSVersionID = 0x06770000,                   /* 4 BYTEs */
    kQTExifUserDataGPSLatitudeRef = 0x06770001,                 /* 2 ASCIIs*/
    kQTExifUserDataGPSLatitude  = 0x06770002,                   /* 3 RATIONALs */
    kQTExifUserDataGPSLongitudeRef = 0x06770003,                /* 2 ASCIIs */
    kQTExifUserDataGPSLongitude = 0x06770004,                   /* 3 RATIONALs */
    kQTExifUserDataGPSAltitudeRef = 0x06770005,                 /* 1 BYTE */
    kQTExifUserDataGPSAltitude  = 0x06770006,                   /* 1 RATIONAL */
    kQTExifUserDataGPSTimeStamp = 0x06770007,                   /* 3 RATIONALs */
    kQTExifUserDataGPSSatellites = 0x06770008,                  /* n ASCIIs */
    kQTExifUserDataGPSStatus    = 0x06770009,                   /* 2 ASCIIs */
    kQTExifUserDataGPSMeasureMode = 0x0677000A,                 /* 2 ASCIIs */
    kQTExifUserDataGPSDOP       = 0x0677000B,                   /* 1 RATIONAL */
    kQTExifUserDataGPSSpeedRef  = 0x0677000C,                   /* 2 ASCIIs */
    kQTExifUserDataGPSSpeed     = 0x0677000D,                   /* 1 RATIONAL */
    kQTExifUserDataGPSTrackRef  = 0x0677000E,                   /* 2 ASCIIs */
    kQTExifUserDataGPSTrack     = 0x0677000F,                   /* 1 RATIONAL */
    kQTExifUserDataGPSImgDirectionRef = 0x06770010,             /* 2 ASCIIs */
    kQTExifUserDataGPSImgDirection = 0x06770011,                /* 1 RATIONAL */
    kQTExifUserDataGPSMapDatum  = 0x06770012,                   /* n ASCII */
    kQTExifUserDataGPSDestLatitudeRef = 0x06770013,             /* 2 ASCIIs */
    kQTExifUserDataGPSDestLatitude = 0x06770014,                /* 3 RATIONALs */
    kQTExifUserDataGPSDestLongitudeRef = 0x06770015,            /* 2 ASCIIs */
    kQTExifUserDataGPSDestLongitude = 0x06770016,               /* 3 RATIONALs */
    kQTExifUserDataGPSDestBearingRef = 0x06770017,              /* 2 ASCIIs */
    kQTExifUserDataGPSDestBearing = 0x06770018,                 /* 1 RATIONAL */
    kQTExifUserDataGPSDestDistanceRef = 0x06770019,             /* 2 ASCIIs */
    kQTExifUserDataGPSDestDistance = 0x0677001A                 /* 1 RATIONAL */
};


/** These are GraphicsImport procedures **/
EXTERN_API( ComponentResult )
GraphicsImportSetDataReference  (GraphicsImportComponent  ci,
                                 Handle                 dataRef,
                                 OSType                 dataReType)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDataReference  (GraphicsImportComponent  ci,
                                 Handle *               dataRef,
                                 OSType *               dataReType)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetDataFile       (GraphicsImportComponent  ci,
                                 const FSSpec *         theFile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDataFile       (GraphicsImportComponent  ci,
                                 FSSpec *               theFile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetDataHandle     (GraphicsImportComponent  ci,
                                 Handle                 h)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDataHandle     (GraphicsImportComponent  ci,
                                 Handle *               h)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetImageDescription (GraphicsImportComponent  ci,
                                 ImageDescriptionHandle * desc)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDataOffsetAndSize (GraphicsImportComponent  ci,
                                 unsigned long *        offset,
                                 unsigned long *        size)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x0008, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportReadData          (GraphicsImportComponent  ci,
                                 void *                 dataPtr,
                                 unsigned long          dataOffset,
                                 unsigned long          dataSize)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x0009, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetClip           (GraphicsImportComponent  ci,
                                 RgnHandle              clipRgn)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetClip           (GraphicsImportComponent  ci,
                                 RgnHandle *            clipRgn)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetSourceRect     (GraphicsImportComponent  ci,
                                 const Rect *           sourceRect)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x000C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetSourceRect     (GraphicsImportComponent  ci,
                                 Rect *                 sourceRect)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetNaturalBounds  (GraphicsImportComponent  ci,
                                 Rect *                 naturalBounds)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x000E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportDraw              (GraphicsImportComponent  ci)                               FIVEWORDINLINE(0x2F3C, 0x0000, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetGWorld         (GraphicsImportComponent  ci,
                                 CGrafPtr               port,
                                 GDHandle               gd)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetGWorld         (GraphicsImportComponent  ci,
                                 CGrafPtr *             port,
                                 GDHandle *             gd)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetMatrix         (GraphicsImportComponent  ci,
                                 const MatrixRecord *   matrix)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetMatrix         (GraphicsImportComponent  ci,
                                 MatrixRecord *         matrix)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0013, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetBoundsRect     (GraphicsImportComponent  ci,
                                 const Rect *           bounds)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0014, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetBoundsRect     (GraphicsImportComponent  ci,
                                 Rect *                 bounds)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0015, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSaveAsPicture     (GraphicsImportComponent  ci,
                                 const FSSpec *         fss,
                                 ScriptCode             scriptTag)                          FIVEWORDINLINE(0x2F3C, 0x0006, 0x0016, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetGraphicsMode   (GraphicsImportComponent  ci,
                                 long                   graphicsMode,
                                 const RGBColor *       opColor)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0017, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetGraphicsMode   (GraphicsImportComponent  ci,
                                 long *                 graphicsMode,
                                 RGBColor *             opColor)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0018, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetQuality        (GraphicsImportComponent  ci,
                                 CodecQ                 quality)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0019, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetQuality        (GraphicsImportComponent  ci,
                                 CodecQ *               quality)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSaveAsQuickTimeImageFile (GraphicsImportComponent  ci,
                                 const FSSpec *         fss,
                                 ScriptCode             scriptTag)                          FIVEWORDINLINE(0x2F3C, 0x0006, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetDataReferenceOffsetAndLimit (GraphicsImportComponent  ci,
                                 unsigned long          offset,
                                 unsigned long          limit)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x001C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDataReferenceOffsetAndLimit (GraphicsImportComponent  ci,
                                 unsigned long *        offset,
                                 unsigned long *        limit)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x001D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetAliasedDataReference (GraphicsImportComponent  ci,
                                 Handle *               dataRef,
                                 OSType *               dataRefType)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x001E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportValidate          (GraphicsImportComponent  ci,
                                 Boolean *              valid)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x001F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetMetaData       (GraphicsImportComponent  ci,
                                 void *                 userData)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0020, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetMIMETypeList   (GraphicsImportComponent  ci,
                                 void *                 qtAtomContainerPtr)                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0021, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportDoesDrawAllPixels (GraphicsImportComponent  ci,
                                 short *                drawsAllPixels)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0022, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetAsPicture      (GraphicsImportComponent  ci,
                                 PicHandle *            picture)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0023, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportExportImageFile   (GraphicsImportComponent  ci,
                                 OSType                 fileType,
                                 OSType                 fileCreator,
                                 const FSSpec *         fss,
                                 ScriptCode             scriptTag)                          FIVEWORDINLINE(0x2F3C, 0x000E, 0x0024, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetExportImageTypeList (GraphicsImportComponent  ci,
                                 void *                 qtAtomContainerPtr)                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0025, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportDoExportImageFileDialog (GraphicsImportComponent  ci,
                                 const FSSpec *         inDefaultSpec,
                                 StringPtr              prompt,
                                 ModalFilterYDUPP       filterProc,
                                 OSType *               outExportedType,
                                 FSSpec *               outExportedSpec,
                                 ScriptCode *           outScriptTag)                       FIVEWORDINLINE(0x2F3C, 0x0018, 0x0026, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetExportSettingsAsAtomContainer (GraphicsImportComponent  ci,
                                 void *                 qtAtomContainerPtr)                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0027, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetExportSettingsFromAtomContainer (GraphicsImportComponent  ci,
                                 void *                 qtAtomContainer)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0028, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetProgressProc   (GraphicsImportComponent  ci,
                                 ICMProgressProcRecordPtr  progressProc)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0029, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetProgressProc   (GraphicsImportComponent  ci,
                                 ICMProgressProcRecordPtr  progressProc)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x002A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetImageCount     (GraphicsImportComponent  ci,
                                 unsigned long *        imageCount)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x002B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetImageIndex     (GraphicsImportComponent  ci,
                                 unsigned long          imageIndex)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x002C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetImageIndex     (GraphicsImportComponent  ci,
                                 unsigned long *        imageIndex)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x002D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDataOffsetAndSize64 (GraphicsImportComponent  ci,
                                 wide *                 offset,
                                 wide *                 size)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x002E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportReadData64        (GraphicsImportComponent  ci,
                                 void *                 dataPtr,
                                 const wide *           dataOffset,
                                 unsigned long          dataSize)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x002F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetDataReferenceOffsetAndLimit64 (GraphicsImportComponent  ci,
                                 const wide *           offset,
                                 const wide *           limit)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0030, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDataReferenceOffsetAndLimit64 (GraphicsImportComponent  ci,
                                 wide *                 offset,
                                 wide *                 limit)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0031, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDefaultMatrix  (GraphicsImportComponent  ci,
                                 MatrixRecord *         defaultMatrix)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0032, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDefaultClip    (GraphicsImportComponent  ci,
                                 RgnHandle *            defaultRgn)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0033, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDefaultGraphicsMode (GraphicsImportComponent  ci,
                                 long *                 defaultGraphicsMode,
                                 RGBColor *             defaultOpColor)                     FIVEWORDINLINE(0x2F3C, 0x0008, 0x0034, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDefaultSourceRect (GraphicsImportComponent  ci,
                                 Rect *                 defaultSourceRect)                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0035, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetColorSyncProfile (GraphicsImportComponent  ci,
                                 Handle *               profile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0036, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetDestRect       (GraphicsImportComponent  ci,
                                 const Rect *           destRect)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0037, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetDestRect       (GraphicsImportComponent  ci,
                                 Rect *                 destRect)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0038, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetFlags          (GraphicsImportComponent  ci,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0039, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportGetFlags          (GraphicsImportComponent  ci,
                                 long *                 flags)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x003A, 0x7000, 0xA82A);

/* 2 private selectors */
EXTERN_API( ComponentResult )
GraphicsImportGetBaseDataOffsetAndSize64 (GraphicsImportComponent  ci,
                                 wide *                 offset,
                                 wide *                 size)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x003D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImportSetImageIndexToThumbnail (GraphicsImportComponent  ci)                        FIVEWORDINLINE(0x2F3C, 0x0000, 0x003E, 0x7000, 0xA82A);



typedef ComponentInstance               GraphicsExportComponent;
enum {
    GraphicsExporterComponentType = FOUR_CHAR_CODE('grex'),
    kBaseGraphicsExporterSubType = FOUR_CHAR_CODE('base')
};

enum {
    graphicsExporterIsBaseExporter = 1L << 0,
    graphicsExporterCanTranscode = 1L << 1,
    graphicsExporterUsesImageCompressor = 1L << 2
};


struct QTResolutionSettings {
    Fixed                           horizontalResolution;
    Fixed                           verticalResolution;
};
typedef struct QTResolutionSettings     QTResolutionSettings;

struct QTTargetDataSize {
    unsigned long                   targetDataSize;
};
typedef struct QTTargetDataSize         QTTargetDataSize;

struct QTThumbnailSettings {
    long                            enableThumbnail;            /* a thoroughly padded Boolean*/
    long                            maxThumbnailWidth;          /* set to zero to let someone else decide*/
    long                            maxThumbnailHeight;         /* set to zero to let someone else decide*/
};
typedef struct QTThumbnailSettings      QTThumbnailSettings;
enum {
    kQTResolutionSettings       = FOUR_CHAR_CODE('reso'),
    kQTTargetDataSize           = FOUR_CHAR_CODE('dasz'),
    kQTDontRecompress           = FOUR_CHAR_CODE('dntr'),
    kQTInterlaceStyle           = FOUR_CHAR_CODE('ilac'),
    kQTColorSyncProfile         = FOUR_CHAR_CODE('iccp'),
    kQTThumbnailSettings        = FOUR_CHAR_CODE('thum'),
    kQTEnableExif               = FOUR_CHAR_CODE('exif'),       /* UInt8 (boolean)*/
    kQTMetaData                 = FOUR_CHAR_CODE('meta')
};

enum {
    kQTTIFFCompressionMethod    = FOUR_CHAR_CODE('tifc'),       /* UInt32*/
    kQTTIFFCompression_None     = 1,
    kQTTIFFCompression_PackBits = 32773L,
    kQTTIFFLittleEndian         = FOUR_CHAR_CODE('tife')        /* UInt8 (boolean)*/
};

enum {
    kQTPNGFilterPreference      = FOUR_CHAR_CODE('pngf'),       /* UInt32*/
    kQTPNGFilterBestForColorType = FOUR_CHAR_CODE('bflt'),
    kQTPNGFilterNone            = 0,
    kQTPNGFilterSub             = 1,
    kQTPNGFilterUp              = 2,
    kQTPNGFilterAverage         = 3,
    kQTPNGFilterPaeth           = 4,
    kQTPNGFilterAdaptivePerRow  = FOUR_CHAR_CODE('aflt'),
    kQTPNGInterlaceStyle        = FOUR_CHAR_CODE('ilac'),       /* UInt32*/
    kQTPNGInterlaceNone         = 0,
    kQTPNGInterlaceAdam7        = 1
};


/** These are GraphicsExport procedures **/
/* To use: set the input and output (and other settings as desired) and call GEDoExport. */
EXTERN_API( ComponentResult )
GraphicsExportDoExport          (GraphicsExportComponent  ci,
                                 unsigned long *        actualSizeWritten)                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);

/* Used for internal communication between the base and format-specific graphics exporter: */
EXTERN_API( ComponentResult )
GraphicsExportCanTranscode      (GraphicsExportComponent  ci,
                                 Boolean *              canTranscode)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportDoTranscode       (GraphicsExportComponent  ci)                               FIVEWORDINLINE(0x2F3C, 0x0000, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportCanUseCompressor  (GraphicsExportComponent  ci,
                                 Boolean *              canUseCompressor,
                                 void *                 codecSettingsAtomContainerPtr)      FIVEWORDINLINE(0x2F3C, 0x0008, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportDoUseCompressor   (GraphicsExportComponent  ci,
                                 void *                 codecSettingsAtomContainer,
                                 ImageDescriptionHandle * outDesc)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportDoStandaloneExport (GraphicsExportComponent  ci)                              FIVEWORDINLINE(0x2F3C, 0x0000, 0x0006, 0x7000, 0xA82A);

/* Queries applications can make of a format-specific graphics exporter: */
EXTERN_API( ComponentResult )
GraphicsExportGetDefaultFileTypeAndCreator (GraphicsExportComponent  ci,
                                 OSType *               fileType,
                                 OSType *               fileCreator)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetDefaultFileNameExtension (GraphicsExportComponent  ci,
                                 OSType *               fileNameExtension)                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0008, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetMIMETypeList   (GraphicsExportComponent  ci,
                                 void *                 qtAtomContainerPtr)                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0009, 0x7000, 0xA82A);

/* (1 unused selector) */
/* Graphics exporter settings: */
EXTERN_API( ComponentResult )
GraphicsExportRequestSettings   (GraphicsExportComponent  ci,
                                 ModalFilterYDUPP       filterProc,
                                 void *                 yourDataProc)                       FIVEWORDINLINE(0x2F3C, 0x0008, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetSettingsFromAtomContainer (GraphicsExportComponent  ci,
                                 void *                 qtAtomContainer)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x000C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetSettingsAsAtomContainer (GraphicsExportComponent  ci,
                                 void *                 qtAtomContainerPtr)                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetSettingsAsText (GraphicsExportComponent  ci,
                                 Handle *               theText)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x000E, 0x7000, 0xA82A);

/* Graphics exporters may implement some or none of the following: */
EXTERN_API( ComponentResult )
GraphicsExportSetDontRecompress (GraphicsExportComponent  ci,
                                 Boolean                dontRecompress)                     FIVEWORDINLINE(0x2F3C, 0x0002, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetDontRecompress (GraphicsExportComponent  ci,
                                 Boolean *              dontRecompress)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetInterlaceStyle (GraphicsExportComponent  ci,
                                 unsigned long          interlaceStyle)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInterlaceStyle (GraphicsExportComponent  ci,
                                 unsigned long *        interlaceStyle)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetMetaData       (GraphicsExportComponent  ci,
                                 void *                 userData)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0013, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetMetaData       (GraphicsExportComponent  ci,
                                 void *                 userData)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0014, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetTargetDataSize (GraphicsExportComponent  ci,
                                 unsigned long          targetDataSize)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0015, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetTargetDataSize (GraphicsExportComponent  ci,
                                 unsigned long *        targetDataSize)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0016, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetCompressionMethod (GraphicsExportComponent  ci,
                                 long                   compressionMethod)                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0017, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetCompressionMethod (GraphicsExportComponent  ci,
                                 long *                 compressionMethod)                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0018, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetCompressionQuality (GraphicsExportComponent  ci,
                                 CodecQ                 spatialQuality)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0019, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetCompressionQuality (GraphicsExportComponent  ci,
                                 CodecQ *               spatialQuality)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetResolution     (GraphicsExportComponent  ci,
                                 Fixed                  horizontalResolution,
                                 Fixed                  verticalResolution)                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetResolution     (GraphicsExportComponent  ci,
                                 Fixed *                horizontalResolution,
                                 Fixed *                verticalResolution)                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x001C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetDepth          (GraphicsExportComponent  ci,
                                 long                   depth)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x001D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetDepth          (GraphicsExportComponent  ci,
                                 long *                 depth)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x001E, 0x7000, 0xA82A);

/* (2 unused selectors) */
EXTERN_API( ComponentResult )
GraphicsExportSetColorSyncProfile (GraphicsExportComponent  ci,
                                 Handle                 colorSyncProfile)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0021, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetColorSyncProfile (GraphicsExportComponent  ci,
                                 Handle *               colorSyncProfile)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0022, 0x7000, 0xA82A);

/* Always implemented by the base graphics exporter: */
EXTERN_API( ComponentResult )
GraphicsExportSetProgressProc   (GraphicsExportComponent  ci,
                                 ICMProgressProcRecordPtr  progressProc)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0023, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetProgressProc   (GraphicsExportComponent  ci,
                                 ICMProgressProcRecordPtr  progressProc)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0024, 0x7000, 0xA82A);

/* Sources for the input image: */
EXTERN_API( ComponentResult )
GraphicsExportSetInputDataReference (GraphicsExportComponent  ci,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 ImageDescriptionHandle  desc)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0025, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputDataReference (GraphicsExportComponent  ci,
                                 Handle *               dataRef,
                                 OSType *               dataRefType)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x0026, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetInputFile      (GraphicsExportComponent  ci,
                                 const FSSpec *         theFile,
                                 ImageDescriptionHandle  desc)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0027, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputFile      (GraphicsExportComponent  ci,
                                 FSSpec *               theFile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0028, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetInputHandle    (GraphicsExportComponent  ci,
                                 Handle                 h,
                                 ImageDescriptionHandle  desc)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0029, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputHandle    (GraphicsExportComponent  ci,
                                 Handle *               h)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x002A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetInputPtr       (GraphicsExportComponent  ci,
                                 Ptr                    p,
                                 unsigned long          size,
                                 ImageDescriptionHandle  desc)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x002B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputPtr       (GraphicsExportComponent  ci,
                                 Ptr *                  p,
                                 unsigned long *        size)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x002C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetInputGraphicsImporter (GraphicsExportComponent  ci,
                                 GraphicsImportComponent  grip)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x002D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputGraphicsImporter (GraphicsExportComponent  ci,
                                 GraphicsImportComponent * grip)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x002E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetInputPicture   (GraphicsExportComponent  ci,
                                 PicHandle              picture)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x002F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputPicture   (GraphicsExportComponent  ci,
                                 PicHandle *            picture)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0030, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetInputGWorld    (GraphicsExportComponent  ci,
                                 GWorldPtr              gworld)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0031, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputGWorld    (GraphicsExportComponent  ci,
                                 GWorldPtr *            gworld)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0032, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetInputPixmap    (GraphicsExportComponent  ci,
                                 PixMapHandle           pixmap)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0033, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputPixmap    (GraphicsExportComponent  ci,
                                 PixMapHandle *         pixmap)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0034, 0x7000, 0xA82A);

/* Only applicable when the input is a data reference, file, handle or ptr: */
EXTERN_API( ComponentResult )
GraphicsExportSetInputOffsetAndLimit (GraphicsExportComponent  ci,
                                 unsigned long          offset,
                                 unsigned long          limit)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0035, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputOffsetAndLimit (GraphicsExportComponent  ci,
                                 unsigned long *        offset,
                                 unsigned long *        limit)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0036, 0x7000, 0xA82A);

/* Used by format-specific graphics exporters when transcoding: */
EXTERN_API( ComponentResult )
GraphicsExportMayExporterReadInputData (GraphicsExportComponent  ci,
                                 Boolean *              mayReadInputData)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0037, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputDataSize  (GraphicsExportComponent  ci,
                                 unsigned long *        size)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0038, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportReadInputData     (GraphicsExportComponent  ci,
                                 void *                 dataPtr,
                                 unsigned long          dataOffset,
                                 unsigned long          dataSize)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x0039, 0x7000, 0xA82A);

/* Used by format-specific graphics exporters, especially when doing standalone export: */
EXTERN_API( ComponentResult )
GraphicsExportGetInputImageDescription (GraphicsExportComponent  ci,
                                 ImageDescriptionHandle * desc)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x003A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputImageDimensions (GraphicsExportComponent  ci,
                                 Rect *                 dimensions)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x003B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetInputImageDepth (GraphicsExportComponent  ci,
                                 long *                 inputDepth)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x003C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportDrawInputImage    (GraphicsExportComponent  ci,
                                 CGrafPtr               gw,
                                 GDHandle               gd,
                                 const Rect *           srcRect,
                                 const Rect *           dstRect)                            FIVEWORDINLINE(0x2F3C, 0x0010, 0x003D, 0x7000, 0xA82A);

/* Destinations for the output image: */
EXTERN_API( ComponentResult )
GraphicsExportSetOutputDataReference (GraphicsExportComponent  ci,
                                 Handle                 dataRef,
                                 OSType                 dataRefType)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x003E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetOutputDataReference (GraphicsExportComponent  ci,
                                 Handle *               dataRef,
                                 OSType *               dataRefType)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x003F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetOutputFile     (GraphicsExportComponent  ci,
                                 const FSSpec *         theFile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0040, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetOutputFile     (GraphicsExportComponent  ci,
                                 FSSpec *               theFile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0041, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetOutputHandle   (GraphicsExportComponent  ci,
                                 Handle                 h)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0042, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetOutputHandle   (GraphicsExportComponent  ci,
                                 Handle *               h)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0043, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetOutputOffsetAndMaxSize (GraphicsExportComponent  ci,
                                 unsigned long          offset,
                                 unsigned long          maxSize,
                                 Boolean                truncateFile)                       FIVEWORDINLINE(0x2F3C, 0x000A, 0x0044, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetOutputOffsetAndMaxSize (GraphicsExportComponent  ci,
                                 unsigned long *        offset,
                                 unsigned long *        maxSize,
                                 Boolean *              truncateFile)                       FIVEWORDINLINE(0x2F3C, 0x000C, 0x0045, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetOutputFileTypeAndCreator (GraphicsExportComponent  ci,
                                 OSType                 fileType,
                                 OSType                 fileCreator)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x0046, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetOutputFileTypeAndCreator (GraphicsExportComponent  ci,
                                 OSType *               fileType,
                                 OSType *               fileCreator)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x0047, 0x7000, 0xA82A);

/* Used by format-specific graphics exporters: */
EXTERN_API( ComponentResult )
GraphicsExportWriteOutputData   (GraphicsExportComponent  ci,
                                 const void *           dataPtr,
                                 unsigned long          dataSize)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0048, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportSetOutputMark     (GraphicsExportComponent  ci,
                                 unsigned long          mark)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0049, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetOutputMark     (GraphicsExportComponent  ci,
                                 unsigned long *        mark)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x004A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportReadOutputData    (GraphicsExportComponent  ci,
                                 void *                 dataPtr,
                                 unsigned long          dataOffset,
                                 unsigned long          dataSize)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x004B, 0x7000, 0xA82A);

/* Allows embedded thumbnail creation, if supported. */
EXTERN_API( ComponentResult )
GraphicsExportSetThumbnailEnabled (GraphicsExportComponent  ci,
                                 Boolean                enableThumbnail,
                                 long                   maxThumbnailWidth,
                                 long                   maxThumbnailHeight)                 FIVEWORDINLINE(0x2F3C, 0x000A, 0x004C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetThumbnailEnabled (GraphicsExportComponent  ci,
                                 Boolean *              thumbnailEnabled,
                                 long *                 maxThumbnailWidth,
                                 long *                 maxThumbnailHeight)                 FIVEWORDINLINE(0x2F3C, 0x000C, 0x004D, 0x7000, 0xA82A);

/* Allows export of Exif files, if supported.  This disables Exif-incompatible settings such as grayscale JPEG and compressed TIFF, and enables export of Exif metadata. */
EXTERN_API( ComponentResult )
GraphicsExportSetExifEnabled    (GraphicsExportComponent  ci,
                                 Boolean                enableExif)                         FIVEWORDINLINE(0x2F3C, 0x0002, 0x004E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsExportGetExifEnabled    (GraphicsExportComponent  ci,
                                 Boolean *              exifEnabled)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x004F, 0x7000, 0xA82A);



typedef ComponentInstance               ImageTranscoderComponent;
enum {
    ImageTranscodererComponentType = FOUR_CHAR_CODE('imtc')
};


/** These are ImageTranscoder procedures **/
EXTERN_API( ComponentResult )
ImageTranscoderBeginSequence    (ImageTranscoderComponent  itc,
                                 ImageDescriptionHandle  srcDesc,
                                 ImageDescriptionHandle * dstDesc,
                                 void *                 data,
                                 long                   dataSize)                           FIVEWORDINLINE(0x2F3C, 0x0010, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ImageTranscoderConvert          (ImageTranscoderComponent  itc,
                                 void *                 srcData,
                                 long                   srcDataSize,
                                 void **                dstData,
                                 long *                 dstDataSize)                        FIVEWORDINLINE(0x2F3C, 0x0010, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ImageTranscoderDisposeData      (ImageTranscoderComponent  itc,
                                 void *                 dstData)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ImageTranscoderEndSequence      (ImageTranscoderComponent  itc)                             FIVEWORDINLINE(0x2F3C, 0x0000, 0x0004, 0x7000, 0xA82A);

/* UPP call backs */

/* selectors for component calls */
enum {
    kGraphicsImportSetDataReferenceSelect      = 0x0001,
    kGraphicsImportGetDataReferenceSelect      = 0x0002,
    kGraphicsImportSetDataFileSelect           = 0x0003,
    kGraphicsImportGetDataFileSelect           = 0x0004,
    kGraphicsImportSetDataHandleSelect         = 0x0005,
    kGraphicsImportGetDataHandleSelect         = 0x0006,
    kGraphicsImportGetImageDescriptionSelect   = 0x0007,
    kGraphicsImportGetDataOffsetAndSizeSelect  = 0x0008,
    kGraphicsImportReadDataSelect              = 0x0009,
    kGraphicsImportSetClipSelect               = 0x000A,
    kGraphicsImportGetClipSelect               = 0x000B,
    kGraphicsImportSetSourceRectSelect         = 0x000C,
    kGraphicsImportGetSourceRectSelect         = 0x000D,
    kGraphicsImportGetNaturalBoundsSelect      = 0x000E,
    kGraphicsImportDrawSelect                  = 0x000F,
    kGraphicsImportSetGWorldSelect             = 0x0010,
    kGraphicsImportGetGWorldSelect             = 0x0011,
    kGraphicsImportSetMatrixSelect             = 0x0012,
    kGraphicsImportGetMatrixSelect             = 0x0013,
    kGraphicsImportSetBoundsRectSelect         = 0x0014,
    kGraphicsImportGetBoundsRectSelect         = 0x0015,
    kGraphicsImportSaveAsPictureSelect         = 0x0016,
    kGraphicsImportSetGraphicsModeSelect       = 0x0017,
    kGraphicsImportGetGraphicsModeSelect       = 0x0018,
    kGraphicsImportSetQualitySelect            = 0x0019,
    kGraphicsImportGetQualitySelect            = 0x001A,
    kGraphicsImportSaveAsQuickTimeImageFileSelect = 0x001B,
    kGraphicsImportSetDataReferenceOffsetAndLimitSelect = 0x001C,
    kGraphicsImportGetDataReferenceOffsetAndLimitSelect = 0x001D,
    kGraphicsImportGetAliasedDataReferenceSelect = 0x001E,
    kGraphicsImportValidateSelect              = 0x001F,
    kGraphicsImportGetMetaDataSelect           = 0x0020,
    kGraphicsImportGetMIMETypeListSelect       = 0x0021,
    kGraphicsImportDoesDrawAllPixelsSelect     = 0x0022,
    kGraphicsImportGetAsPictureSelect          = 0x0023,
    kGraphicsImportExportImageFileSelect       = 0x0024,
    kGraphicsImportGetExportImageTypeListSelect = 0x0025,
    kGraphicsImportDoExportImageFileDialogSelect = 0x0026,
    kGraphicsImportGetExportSettingsAsAtomContainerSelect = 0x0027,
    kGraphicsImportSetExportSettingsFromAtomContainerSelect = 0x0028,
    kGraphicsImportSetProgressProcSelect       = 0x0029,
    kGraphicsImportGetProgressProcSelect       = 0x002A,
    kGraphicsImportGetImageCountSelect         = 0x002B,
    kGraphicsImportSetImageIndexSelect         = 0x002C,
    kGraphicsImportGetImageIndexSelect         = 0x002D,
    kGraphicsImportGetDataOffsetAndSize64Select = 0x002E,
    kGraphicsImportReadData64Select            = 0x002F,
    kGraphicsImportSetDataReferenceOffsetAndLimit64Select = 0x0030,
    kGraphicsImportGetDataReferenceOffsetAndLimit64Select = 0x0031,
    kGraphicsImportGetDefaultMatrixSelect      = 0x0032,
    kGraphicsImportGetDefaultClipSelect        = 0x0033,
    kGraphicsImportGetDefaultGraphicsModeSelect = 0x0034,
    kGraphicsImportGetDefaultSourceRectSelect  = 0x0035,
    kGraphicsImportGetColorSyncProfileSelect   = 0x0036,
    kGraphicsImportSetDestRectSelect           = 0x0037,
    kGraphicsImportGetDestRectSelect           = 0x0038,
    kGraphicsImportSetFlagsSelect              = 0x0039,
    kGraphicsImportGetFlagsSelect              = 0x003A,
    kGraphicsImportGetBaseDataOffsetAndSize64Select = 0x003D,
    kGraphicsImportSetImageIndexToThumbnailSelect = 0x003E,
    kGraphicsExportDoExportSelect              = 0x0001,
    kGraphicsExportCanTranscodeSelect          = 0x0002,
    kGraphicsExportDoTranscodeSelect           = 0x0003,
    kGraphicsExportCanUseCompressorSelect      = 0x0004,
    kGraphicsExportDoUseCompressorSelect       = 0x0005,
    kGraphicsExportDoStandaloneExportSelect    = 0x0006,
    kGraphicsExportGetDefaultFileTypeAndCreatorSelect = 0x0007,
    kGraphicsExportGetDefaultFileNameExtensionSelect = 0x0008,
    kGraphicsExportGetMIMETypeListSelect       = 0x0009,
    kGraphicsExportRequestSettingsSelect       = 0x000B,
    kGraphicsExportSetSettingsFromAtomContainerSelect = 0x000C,
    kGraphicsExportGetSettingsAsAtomContainerSelect = 0x000D,
    kGraphicsExportGetSettingsAsTextSelect     = 0x000E,
    kGraphicsExportSetDontRecompressSelect     = 0x000F,
    kGraphicsExportGetDontRecompressSelect     = 0x0010,
    kGraphicsExportSetInterlaceStyleSelect     = 0x0011,
    kGraphicsExportGetInterlaceStyleSelect     = 0x0012,
    kGraphicsExportSetMetaDataSelect           = 0x0013,
    kGraphicsExportGetMetaDataSelect           = 0x0014,
    kGraphicsExportSetTargetDataSizeSelect     = 0x0015,
    kGraphicsExportGetTargetDataSizeSelect     = 0x0016,
    kGraphicsExportSetCompressionMethodSelect  = 0x0017,
    kGraphicsExportGetCompressionMethodSelect  = 0x0018,
    kGraphicsExportSetCompressionQualitySelect = 0x0019,
    kGraphicsExportGetCompressionQualitySelect = 0x001A,
    kGraphicsExportSetResolutionSelect         = 0x001B,
    kGraphicsExportGetResolutionSelect         = 0x001C,
    kGraphicsExportSetDepthSelect              = 0x001D,
    kGraphicsExportGetDepthSelect              = 0x001E,
    kGraphicsExportSetColorSyncProfileSelect   = 0x0021,
    kGraphicsExportGetColorSyncProfileSelect   = 0x0022,
    kGraphicsExportSetProgressProcSelect       = 0x0023,
    kGraphicsExportGetProgressProcSelect       = 0x0024,
    kGraphicsExportSetInputDataReferenceSelect = 0x0025,
    kGraphicsExportGetInputDataReferenceSelect = 0x0026,
    kGraphicsExportSetInputFileSelect          = 0x0027,
    kGraphicsExportGetInputFileSelect          = 0x0028,
    kGraphicsExportSetInputHandleSelect        = 0x0029,
    kGraphicsExportGetInputHandleSelect        = 0x002A,
    kGraphicsExportSetInputPtrSelect           = 0x002B,
    kGraphicsExportGetInputPtrSelect           = 0x002C,
    kGraphicsExportSetInputGraphicsImporterSelect = 0x002D,
    kGraphicsExportGetInputGraphicsImporterSelect = 0x002E,
    kGraphicsExportSetInputPictureSelect       = 0x002F,
    kGraphicsExportGetInputPictureSelect       = 0x0030,
    kGraphicsExportSetInputGWorldSelect        = 0x0031,
    kGraphicsExportGetInputGWorldSelect        = 0x0032,
    kGraphicsExportSetInputPixmapSelect        = 0x0033,
    kGraphicsExportGetInputPixmapSelect        = 0x0034,
    kGraphicsExportSetInputOffsetAndLimitSelect = 0x0035,
    kGraphicsExportGetInputOffsetAndLimitSelect = 0x0036,
    kGraphicsExportMayExporterReadInputDataSelect = 0x0037,
    kGraphicsExportGetInputDataSizeSelect      = 0x0038,
    kGraphicsExportReadInputDataSelect         = 0x0039,
    kGraphicsExportGetInputImageDescriptionSelect = 0x003A,
    kGraphicsExportGetInputImageDimensionsSelect = 0x003B,
    kGraphicsExportGetInputImageDepthSelect    = 0x003C,
    kGraphicsExportDrawInputImageSelect        = 0x003D,
    kGraphicsExportSetOutputDataReferenceSelect = 0x003E,
    kGraphicsExportGetOutputDataReferenceSelect = 0x003F,
    kGraphicsExportSetOutputFileSelect         = 0x0040,
    kGraphicsExportGetOutputFileSelect         = 0x0041,
    kGraphicsExportSetOutputHandleSelect       = 0x0042,
    kGraphicsExportGetOutputHandleSelect       = 0x0043,
    kGraphicsExportSetOutputOffsetAndMaxSizeSelect = 0x0044,
    kGraphicsExportGetOutputOffsetAndMaxSizeSelect = 0x0045,
    kGraphicsExportSetOutputFileTypeAndCreatorSelect = 0x0046,
    kGraphicsExportGetOutputFileTypeAndCreatorSelect = 0x0047,
    kGraphicsExportWriteOutputDataSelect       = 0x0048,
    kGraphicsExportSetOutputMarkSelect         = 0x0049,
    kGraphicsExportGetOutputMarkSelect         = 0x004A,
    kGraphicsExportReadOutputDataSelect        = 0x004B,
    kGraphicsExportSetThumbnailEnabledSelect   = 0x004C,
    kGraphicsExportGetThumbnailEnabledSelect   = 0x004D,
    kGraphicsExportSetExifEnabledSelect        = 0x004E,
    kGraphicsExportGetExifEnabledSelect        = 0x004F,
    kImageTranscoderBeginSequenceSelect        = 0x0001,
    kImageTranscoderConvertSelect              = 0x0002,
    kImageTranscoderDisposeDataSelect          = 0x0003,
    kImageTranscoderEndSequenceSelect          = 0x0004
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

#endif /* __IMAGECOMPRESSION__ */

