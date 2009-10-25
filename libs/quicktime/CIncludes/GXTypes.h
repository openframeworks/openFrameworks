/*
     File:       GXTypes.h
 
     Contains:   QuickDraw GX object and constant definitions
 
     Version:    Technology: Quickdraw GX 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __GXTYPES__
#define __GXTYPES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __FIXMATH__
#include <FixMath.h>
#endif

#ifndef __GXMATH__
#include <GXMath.h>
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



typedef struct OpaquegxShape*           gxShape;
typedef struct OpaquegxStyle*           gxStyle;
typedef struct OpaquegxInk*             gxInk;
typedef struct OpaquegxTransform*       gxTransform;
typedef struct OpaquegxTag*             gxTag;
typedef struct OpaquegxColorSet*        gxColorSet;
typedef struct OpaquegxColorProfile*    gxColorProfile;
typedef struct OpaquegxGraphicsClient*  gxGraphicsClient;
typedef struct OpaquegxViewGroup*       gxViewGroup;
typedef struct OpaquegxViewPort*        gxViewPort;
typedef struct OpaquegxViewDevice*      gxViewDevice;

typedef long                            gxColorSpace;
/* gxShape enumerations */
enum {
    gxEmptyType                 = 1,
    gxPointType                 = 2,
    gxLineType                  = 3,
    gxCurveType                 = 4,
    gxRectangleType             = 5,
    gxPolygonType               = 6,
    gxPathType                  = 7,
    gxBitmapType                = 8,
    gxTextType                  = 9,
    gxGlyphType                 = 10,
    gxLayoutType                = 11,
    gxFullType                  = 12,
    gxPictureType               = 13
};

typedef long                            gxShapeType;
enum {
    gxNoFill                    = 0,
    gxOpenFrameFill             = 1,
    gxFrameFill                 = gxOpenFrameFill,
    gxClosedFrameFill           = 2,
    gxHollowFill                = gxClosedFrameFill,
    gxEvenOddFill               = 3,
    gxSolidFill                 = gxEvenOddFill,
    gxWindingFill               = 4,
    gxInverseEvenOddFill        = 5,
    gxInverseSolidFill          = gxInverseEvenOddFill,
    gxInverseFill               = gxInverseEvenOddFill,
    gxInverseWindingFill        = 6
};

typedef long                            gxShapeFill;
enum {
    gxNoAttributes              = 0,
    gxDirectShape               = 0x0001,
    gxRemoteShape               = 0x0002,
    gxCachedShape               = 0x0004,
    gxLockedShape               = 0x0008,
    gxGroupShape                = 0x0010,
    gxMapTransformShape         = 0x0020,
    gxUniqueItemsShape          = 0x0040,
    gxIgnorePlatformShape       = 0x0080,
    gxNoMetricsGridShape        = 0x0100,
    gxDiskShape                 = 0x0200,
    gxMemoryShape               = 0x0400
};

typedef long                            gxShapeAttribute;
/* gxShape editing enumerations */
enum {
    gxBreakNeitherEdit          = 0,
    gxBreakLeftEdit             = 0x0001,
    gxBreakRightEdit            = 0x0002,
    gxRemoveDuplicatePointsEdit = 0x0004
};

/* if the new first (or last) point exactly matches the point before it in */
/* the same contour, then remove it) */
typedef long                            gxEditShapeFlag;
enum {
    gxSelectToEnd               = -1
};

#define gxSetToNil  (void *)(-1)
#define gxAnyNumber     1
enum {
    gxCounterclockwiseDirection = 0,
    gxClockwiseDirection        = 1
};

typedef long                            gxContourDirection;
/* gxShape structures */
/* The type 'gxPoint' is defined in "GXMath.h" */

struct gxLine {
    gxPoint                         first;
    gxPoint                         last;
};
typedef struct gxLine                   gxLine;

struct gxCurve {
    gxPoint                         first;
    gxPoint                         control;
    gxPoint                         last;
};
typedef struct gxCurve                  gxCurve;

struct gxRectangle {
    Fixed                           left;
    Fixed                           top;
    Fixed                           right;
    Fixed                           bottom;
};
typedef struct gxRectangle              gxRectangle;

struct gxPolygon {
    long                            vectors;
    gxPoint                         vector[1];
};
typedef struct gxPolygon                gxPolygon;

struct gxPolygons {
    long                            contours;
    gxPolygon                       contour[1];
};
typedef struct gxPolygons               gxPolygons;

struct gxPath {
    long                            vectors;
    long                            controlBits[1];
    gxPoint                         vector[1];
};
typedef struct gxPath                   gxPath;

struct gxPaths {
    long                            contours;
    gxPath                          contour[1];
};
typedef struct gxPaths                  gxPaths;
struct gxBitmap {
    char            *image;     /* pointer to pixels */
    long            width;      /* width in pixels */
    long            height;     /* height in pixels */
    long            rowBytes;   /* width in bytes */
    long            pixelSize;      /* physical bits per pixel */
    gxColorSpace    space;
    gxColorSet      set;
    gxColorProfile  profile;
};
typedef struct gxBitmap gxBitmap;

struct gxLongRectangle {
    long                            left;
    long                            top;
    long                            right;
    long                            bottom;
};
typedef struct gxLongRectangle          gxLongRectangle;
/* gxStyle enumerations */
enum {
    gxCenterFrameStyle          = 0,
    gxSourceGridStyle           = 0x0001,
    gxDeviceGridStyle           = 0x0002,
    gxInsideFrameStyle          = 0x0004,
    gxOutsideFrameStyle         = 0x0008,
    gxAutoInsetStyle            = 0x0010
};


typedef long                            gxStyleAttribute;
enum {
    gxBendDash                  = 0x0001,
    gxBreakDash                 = 0x0002,
    gxClipDash                  = 0x0004,
    gxLevelDash                 = 0x0008,
    gxAutoAdvanceDash           = 0x0010
};

typedef long                            gxDashAttribute;
enum {
    gxPortAlignPattern          = 0x0001,
    gxPortMapPattern            = 0x0002
};

typedef long                            gxPatternAttribute;
enum {
    gxSharpJoin                 = 0x0000,
    gxCurveJoin                 = 0x0001,
    gxLevelJoin                 = 0x0002,
    gxSnapJoin                  = 0x0004
};

typedef long                            gxJoinAttribute;
enum {
    gxLevelStartCap             = 0x0001,
    gxLevelEndCap               = 0x0002
};

typedef long                            gxCapAttribute;
enum {
    gxAutoAdvanceText           = 0x0001,
    gxNoContourGridText         = 0x0002,
    gxNoMetricsGridText         = 0x0004,
    gxAnchorPointsText          = 0x0008,
    gxVerticalText              = 0x0010,
    gxNoOpticalScaleText        = 0x0020
};

typedef long                            gxTextAttribute;
enum {
    gxLeftJustify               = 0,
    gxCenterJustify             = fract1 / 2,
    gxRightJustify              = fract1,
    gxFillJustify               = -1
};

enum {
    gxUnderlineAdvanceLayer     = 0x0001,                       /* a gxLine is drawn through the advances */
    gxSkipWhiteSpaceLayer       = 0x0002,                       /* except characters describing white space */
    gxUnderlineIntervalLayer    = 0x0004,                       /* (+ gxStringLayer) a gxLine is drawn through the gaps between advances */
    gxUnderlineContinuationLayer = 0x0008,                      /* (+ gxStringLayer) join this underline with another face */
    gxWhiteLayer                = 0x0010,                       /* the layer draws to white instead of black */
    gxClipLayer                 = 0x0020,                       /* the characters define a clip */
    gxStringLayer               = 0x0040                        /* all characters in run are combined */
};

typedef long                            gxLayerFlag;
/* gxStyle structures */

struct gxFaceLayer {
    gxShapeFill                     outlineFill;                /* outline framed or filled */
    gxLayerFlag                     flags;                      /* various additional effects */
    gxStyle                         outlineStyle;               /* outline */
    gxTransform                     outlineTransform;           /* italic, condense, extend */
    gxPoint                         boldOutset;                 /* bold */
};
typedef struct gxFaceLayer              gxFaceLayer;

struct gxTextFace {
    long                            faceLayers;                 /* layer to implement shadow */
    gxMapping                       advanceMapping;             /* algorithmic change to advance width */
    gxFaceLayer                     faceLayer[1];               /* zero or more face layers describing the face */
};
typedef struct gxTextFace               gxTextFace;

struct gxJoinRecord {
    gxJoinAttribute                 attributes;
    gxShape                         join;
    Fixed                           miter;
};
typedef struct gxJoinRecord             gxJoinRecord;

struct gxDashRecord {
    gxDashAttribute                 attributes;
    gxShape                         dash;                       /* similar to pattern, except rotated to gxLine slope */
    Fixed                           advance;                    /* specifies repeating frequency of dash */
    Fract                           phase;                      /* specifies offset into the gxPath to start dashing */
    Fixed                           scale;                      /* specifies height of dash to be mapped to penWidth */
};
typedef struct gxDashRecord             gxDashRecord;

struct gxPatternRecord {
    gxPatternAttribute              attributes;
    gxShape                         pattern;
    gxPoint                         u;
    gxPoint                         v;
};
typedef struct gxPatternRecord          gxPatternRecord;

struct gxCapRecord {
    gxCapAttribute                  attributes;
    gxShape                         startCap;
    gxShape                         endCap;
};
typedef struct gxCapRecord              gxCapRecord;

/* gxInk enumerations */
enum {
    gxPortAlignDitherInk        = 0x0001,
    gxForceDitherInk            = 0x0002,
    gxSuppressDitherInk         = 0x0004,
    gxSuppressHalftoneInk       = 0x0008
};


typedef long                            gxInkAttribute;
enum {
    gxNoMode                    = 0,
    gxCopyMode                  = 1,
    gxAddMode                   = 2,
    gxBlendMode                 = 3,
    gxMigrateMode               = 4,
    gxMinimumMode               = 5,
    gxMaximumMode               = 6,
    gxHighlightMode             = 7,
    gxAndMode                   = 8,
    gxOrMode                    = 9,
    gxXorMode                   = 10,
    gxRampAndMode               = 11,
    gxRampOrMode                = 12,
    gxRampXorMode               = 13,
    gxOverMode                  = 14,                           /* Alpha channel modes          */
    gxAtopMode                  = 15,                           /* Note: In England = Beta channel modes   */
    gxExcludeMode               = 16,
    gxFadeMode                  = 17
};

typedef unsigned char                   gxComponentMode;
enum {
    gxRejectSourceTransfer      = 0x0001,                       /* at least one component must be out of range       */
    gxRejectDeviceTransfer      = 0x0002,                       /* at least one component must be out of range       */
    gxSingleComponentTransfer   = 0x0004                        /* duplicate gxTransferComponent[0] for all components in transfer */
};

typedef long                            gxTransferFlag;
enum {
    gxOverResultComponent       = 0x01,                         /* & result gxColor with 0xFFFF before clamping   */
    gxReverseComponent          = 0x02                          /* reverse source and device before moding       */
};

typedef unsigned char                   gxComponentFlag;
/* gxInk structures */

struct gxTransferComponent {
    gxComponentMode                 mode;                       /* how the component is operated upon */
    gxComponentFlag                 flags;                      /* flags for each component   */
    gxColorValue                    sourceMinimum;
    gxColorValue                    sourceMaximum;              /* input filter range */
    gxColorValue                    deviceMinimum;
    gxColorValue                    deviceMaximum;              /* output filter range */
    gxColorValue                    clampMinimum;
    gxColorValue                    clampMaximum;               /* output clamping range */
    gxColorValue                    operand;                    /* ratio for blend, step for migrate, gxColor for highlight   */
};
typedef struct gxTransferComponent      gxTransferComponent;
struct gxTransferMode {
    gxColorSpace        space;          /* the gxColor-space the transfer mode is to operate in */
    gxColorSet          set;
    gxColorProfile      profile;
    Fixed               sourceMatrix[5][4];
    Fixed               deviceMatrix[5][4];
    Fixed               resultMatrix[5][4];
    gxTransferFlag      flags;
    gxTransferComponent component[4];  /* how each component is operated upon           */
};
typedef struct gxTransferMode gxTransferMode;

/* gxColor space enumerations */
enum {
    gxNoColorPacking            = 0x0000,                       /* 16 bits per channel */
    gxAlphaSpace                = 0x0080,                       /* space includes alpha channel */
    gxWord5ColorPacking         = 0x0500,                       /* 5 bits per channel, right-justified */
    gxLong8ColorPacking         = 0x0800,                       /* 8 bits per channel, right-justified */
    gxLong10ColorPacking        = 0x0A00,                       /* 10 bits per channel, right-justified */
    gxAlphaFirstPacking         = 0x1000                        /* alpha channel is the first field in the packed space */
};

enum {
    gxNoSpace                   = 0,
    gxRGBSpace                  = 1,
    gxCMYKSpace                 = 2,
    gxHSVSpace                  = 3,
    gxHLSSpace                  = 4,
    gxYXYSpace                  = 5,
    gxXYZSpace                  = 6,
    gxLUVSpace                  = 7,
    gxLABSpace                  = 8,
    gxYIQSpace                  = 9,
    gxNTSCSpace                 = gxYIQSpace,
    gxPALSpace                  = gxYIQSpace,
    gxGraySpace                 = 10,
    gxIndexedSpace              = 11,
    gxRGBASpace                 = gxRGBSpace + gxAlphaSpace,
    gxGrayASpace                = gxGraySpace + gxAlphaSpace,
    gxRGB16Space                = gxWord5ColorPacking + gxRGBSpace,
    gxRGB32Space                = gxLong8ColorPacking + gxRGBSpace,
    gxARGB32Space               = gxLong8ColorPacking + gxAlphaFirstPacking + gxRGBASpace,
    gxCMYK32Space               = gxLong8ColorPacking + gxCMYKSpace,
    gxHSV32Space                = gxLong10ColorPacking + gxHSVSpace,
    gxHLS32Space                = gxLong10ColorPacking + gxHLSSpace,
    gxYXY32Space                = gxLong10ColorPacking + gxYXYSpace,
    gxXYZ32Space                = gxLong10ColorPacking + gxXYZSpace,
    gxLUV32Space                = gxLong10ColorPacking + gxLUVSpace,
    gxLAB32Space                = gxLong10ColorPacking + gxLABSpace,
    gxYIQ32Space                = gxLong10ColorPacking + gxYIQSpace,
    gxNTSC32Space               = gxYIQ32Space,
    gxPAL32Space                = gxYIQ32Space
};

/* gxColor space structures */

struct gxRGBColor {
    gxColorValue                    red;
    gxColorValue                    green;
    gxColorValue                    blue;
};
typedef struct gxRGBColor               gxRGBColor;

struct gxRGBAColor {
    gxColorValue                    red;
    gxColorValue                    green;
    gxColorValue                    blue;
    gxColorValue                    alpha;
};
typedef struct gxRGBAColor              gxRGBAColor;

struct gxHSVColor {
    gxColorValue                    hue;
    gxColorValue                    saturation;
    gxColorValue                    value;
};
typedef struct gxHSVColor               gxHSVColor;

struct gxHLSColor {
    gxColorValue                    hue;
    gxColorValue                    lightness;
    gxColorValue                    saturation;
};
typedef struct gxHLSColor               gxHLSColor;

struct gxCMYKColor {
    gxColorValue                    cyan;
    gxColorValue                    magenta;
    gxColorValue                    yellow;
    gxColorValue                    black;
};
typedef struct gxCMYKColor              gxCMYKColor;

struct gxXYZColor {
    gxColorValue                    x;
    gxColorValue                    y;
    gxColorValue                    z;
};
typedef struct gxXYZColor               gxXYZColor;

struct gxYXYColor {
    gxColorValue                    capY;
    gxColorValue                    x;
    gxColorValue                    y;
};
typedef struct gxYXYColor               gxYXYColor;

struct gxLUVColor {
    gxColorValue                    l;
    gxColorValue                    u;
    gxColorValue                    v;
};
typedef struct gxLUVColor               gxLUVColor;

struct gxLABColor {
    gxColorValue                    l;
    gxColorValue                    a;
    gxColorValue                    b;
};
typedef struct gxLABColor               gxLABColor;

struct gxYIQColor {
    gxColorValue                    y;
    gxColorValue                    i;
    gxColorValue                    q;
};
typedef struct gxYIQColor               gxYIQColor;

struct gxGrayAColor {
    gxColorValue                    gray;
    gxColorValue                    alpha;
};
typedef struct gxGrayAColor             gxGrayAColor;

typedef long                            gxColorIndex;
struct gxIndexedColor {
    gxColorIndex    index;
    gxColorSet      set;
};
typedef struct gxIndexedColor gxIndexedColor;

struct gxColor {
    gxColorSpace                    space;
    gxColorProfile                  profile;
    union {
        gxCMYKColor                     cmyk;
        gxRGBColor                      rgb;
        gxRGBAColor                     rgba;
        gxHSVColor                      hsv;
        gxHLSColor                      hls;
        gxXYZColor                      xyz;
        gxYXYColor                      yxy;
        gxLUVColor                      luv;
        gxLABColor                      lab;
        gxYIQColor                      yiq;
        gxColorValue                    gray;
        gxGrayAColor                    graya;
        unsigned short                  pixel16;
        unsigned long                   pixel32;
        gxIndexedColor                  indexed;
        gxColorValue                    component[4];
    }                                 element;
};
typedef struct gxColor                  gxColor;

/* gxColorSet structures */

union gxSetColor {
    gxCMYKColor                     cmyk;
    gxRGBColor                      rgb;
    gxRGBAColor                     rgba;
    gxHSVColor                      hsv;
    gxHLSColor                      hls;
    gxXYZColor                      xyz;
    gxYXYColor                      yxy;
    gxLUVColor                      luv;
    gxLABColor                      lab;
    gxYIQColor                      yiq;
    gxColorValue                    gray;
    gxGrayAColor                    graya;
    unsigned short                  pixel16;
    unsigned long                   pixel32;
    gxColorValue                    component[4];
};
typedef union gxSetColor                gxSetColor;
/* gxTransform enumerations */
/* parts of a gxShape considered in hit testing: */
enum {
    gxNoPart                    = 0,                            /* (in order of evaluation) */
    gxBoundsPart                = 0x0001,
    gxGeometryPart              = 0x0002,
    gxPenPart                   = 0x0004,
    gxCornerPointPart           = 0x0008,
    gxControlPointPart          = 0x0010,
    gxEdgePart                  = 0x0020,
    gxJoinPart                  = 0x0040,
    gxStartCapPart              = 0x0080,
    gxEndCapPart                = 0x0100,
    gxDashPart                  = 0x0200,
    gxPatternPart               = 0x0400,
    gxGlyphBoundsPart           = gxJoinPart,
    gxGlyphFirstPart            = gxStartCapPart,
    gxGlyphLastPart             = gxEndCapPart,
    gxSideBearingPart           = gxDashPart,
    gxAnyPart                   = gxBoundsPart | gxGeometryPart | gxPenPart | gxCornerPointPart | gxControlPointPart | gxEdgePart | gxJoinPart | gxStartCapPart | gxEndCapPart | gxDashPart | gxPatternPart
};


typedef long                            gxShapePart;
/* gxTransform structures */

struct gxHitTestInfo {
    gxShapePart                     what;                       /* which part of gxShape */
    long                            index;                      /* control gxPoint index */
    Fixed                           distance;                   /* how far from gxPoint or outside of area click was */

                                                                /* these fields are only set by GXHitTestPicture */
    gxShape                         which;
    gxShape                         containerPicture;           /* picture which contains gxShape hit */
    long                            containerIndex;             /* the index within that picture  */
    long                            totalIndex;                 /* the total index within the root picture */
};
typedef struct gxHitTestInfo            gxHitTestInfo;
/* gxViewPort enumerations */
enum {
    gxGrayPort                  = 0x0001,
    gxAlwaysGridPort            = 0x0002,
    gxEnableMatchPort           = 0x0004
};


typedef long                            gxPortAttribute;
/* gxViewDevice enumerations */
enum {
    gxDirectDevice              = 0x01,                         /* for the device gxBitmap baseAddr pointer */
    gxRemoteDevice              = 0x02,
    gxInactiveDevice            = 0x04
};

typedef long                            gxDeviceAttribute;
enum {
    gxRoundDot                  = 1,
    gxSpiralDot                 = 2,
    gxSquareDot                 = 3,
    gxLineDot                   = 4,
    gxEllipticDot               = 5,
    gxTriangleDot               = 6,
    gxDispersedDot              = 7,
    gxCustomDot                 = 8
};

typedef long                            gxDotType;
/* gxViewPort structures */
enum {
    gxNoTint                    = 0,
    gxLuminanceTint             = 1,                            /* use the luminance of the gxColor */
    gxAverageTint               = 2,                            /* add all the components and divide by the number of components */
    gxMixtureTint               = 3,                            /* find the closest gxColor on the axis between the foreground and background */
    gxComponent1Tint            = 4,                            /* use the value of the first component of the gxColor */
    gxComponent2Tint            = 5,                            /* ... etc. */
    gxComponent3Tint            = 6,
    gxComponent4Tint            = 7
};

typedef long                            gxTintType;

struct gxHalftone {
    Fixed                           angle;
    Fixed                           frequency;
    gxDotType                       method;
    gxTintType                      tinting;
    gxColor                         dotColor;
    gxColor                         backgroundColor;
    gxColorSpace                    tintSpace;
};
typedef struct gxHalftone               gxHalftone;

struct gxHalftoneMatrix {
    Fixed                           dpiX;                       /* intended resolution */
    Fixed                           dpiY;
    long                            width;                      /* width of matrix (in device pixels) */
    long                            height;                     /* height of matrix (in device pixels) */
    long                            tileShift;                  /* shift amount (in samples) for rectangular tiling */
    unsigned short                  samples[1];                 /* samples from 0..MAX(halftone tintSpace) */
};
typedef struct gxHalftoneMatrix         gxHalftoneMatrix;
/* gxViewGroup enumerations */
#define gxAllViewDevices        ((gxViewGroup) 0)
#define gxScreenViewDevices     ((gxViewGroup) 1)
/* graphics stream constants and structures */
enum {
    gxOpenReadSpool             = 1,
    gxOpenWriteSpool            = 2,
    gxReadSpool                 = 3,
    gxWriteSpool                = 4,
    gxCloseSpool                = 5
};


typedef long                            gxSpoolCommand;
typedef unsigned char                   gxGraphicsOpcode;
typedef struct gxSpoolBlock             gxSpoolBlock;
typedef CALLBACK_API_C( long , gxSpoolProcPtr )(gxSpoolCommand command, gxSpoolBlock *block);
typedef STACK_UPP_TYPE(gxSpoolProcPtr)                          gxSpoolUPP;

struct gxSpoolBlock {
    gxSpoolUPP                      spoolProcedure;             /* these fields are read only */
    void *                          buffer;                     /* source/destination pointer to data */
    long                            bufferSize;                 /* how many bytes for the system to read (flatten) / write (unflatten) */
                                                                /* these fields are written to (but are not read from) */
    long                            count;                      /* how many bytes for the caller to read (unflatten) /write (flatten) */
    long                            operationSize;              /* operation size (including operand byte) */
    long                            operationOffset;            /* the data offset, if any, within the current operation */
    gxGraphicsOpcode                lastTypeOpcode;             /* type of last created object */
    gxGraphicsOpcode                currentOperation;           /* operation emitted by flatten, or intrepreted by last unflatten */
    gxGraphicsOpcode                currentOperand;             /* e.g., gxTransformTypeOpcode, gxInkTagOpcode */
    unsigned char                   compressed;                 /* one of: gxTwoBitCompressionValues */
};

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(gxSpoolUPP)
    NewgxSpoolUPP                  (gxSpoolProcPtr          userRoutine);

    EXTERN_API(void)
    DisposegxSpoolUPP              (gxSpoolUPP              userUPP);

    EXTERN_API(long)
    InvokegxSpoolUPP               (gxSpoolCommand          command,
                                    gxSpoolBlock *          block,
                                    gxSpoolUPP              userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppgxSpoolProcInfo = 0x000003F1 };                       /* 4_bytes Func(4_bytes, 4_bytes) */
    #define NewgxSpoolUPP(userRoutine)                              (gxSpoolUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppgxSpoolProcInfo, GetCurrentArchitecture())
    #define DisposegxSpoolUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define InvokegxSpoolUPP(command, block, userUPP)               (long)CALL_TWO_PARAMETER_UPP((userUPP), uppgxSpoolProcInfo, (command), (block))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewgxSpoolProc(userRoutine)                             NewgxSpoolUPP(userRoutine)
#define CallgxSpoolProc(userRoutine, command, block)            InvokegxSpoolUPP(command, block, userRoutine)

enum {
    gxFontListFlatten           = 0x01,                         /* if set, generate a gxTag containing list of each gxFont referenced */
    gxFontGlyphsFlatten         = 0x02,                         /* if set, generate a gxTag containing the list of glyphs referenced inside the gxFont */
    gxFontVariationsFlatten     = 0x04,                         /* if set, append the gxTag with separate [variation] coordinates */
    gxBitmapAliasFlatten        = 0x08                          /* if set, open bitmap alias files and flatten out their image data */
};


typedef long                            gxFlattenFlag;
/* gxGraphicsClient constants */
#define gxGraphicsSystemClient      (gxGraphicsClient)(-1)
enum {
    gxStaticHeapClient          = 0x0001
};

typedef long                            gxClientAttribute;
/* graphics patching constants */
enum {
    gxOriginalGraphicsFunction  = -1,
    gxOriginalGraphicsIdentifier = FOUR_CHAR_CODE('grfx')
};


struct gxBitmapDataSourceAlias {
    unsigned long                   fileOffset;                 /* file offset (in bytes) of top-left pixel */
    unsigned long                   aliasRecordSize;            /* size of alias record below */
    unsigned char                   aliasRecord[1];             /* the actual alias record data */
};
typedef struct gxBitmapDataSourceAlias  gxBitmapDataSourceAlias;
enum {
    gxBitmapFileAliasTagType    = FOUR_CHAR_CODE('bfil'),
    gxPICTFileAliasTagType      = FOUR_CHAR_CODE('pict'),
    gxBitmapFileAliasImageValue = 1L
};

typedef struct OpaquegxFont*            gxFont;
enum {
    gxNoPlatform                = 0,
    gxNoScript                  = 0,
    gxNoLanguage                = 0,
    gxNoFontName                = 0,
    gxGlyphPlatform             = -1
};

enum {
    gxUnicodePlatform           = 1,
    gxMacintoshPlatform         = 2,
    gxReservedPlatform          = 3,
    gxMicrosoftPlatform         = 4,
    gxCustomPlatform            = 5
};


typedef long                            gxFontPlatform;
enum {
    gxUnicodeDefaultSemantics   = 1,
    gxUnicodeV1_1Semantics      = 2,
    gxISO10646_1993Semantics    = 3
};

enum {
    gxRomanScript               = 1,
    gxJapaneseScript            = 2,
    gxTraditionalChineseScript  = 3,
    gxChineseScript             = gxTraditionalChineseScript,
    gxKoreanScript              = 4,
    gxArabicScript              = 5,
    gxHebrewScript              = 6,
    gxGreekScript               = 7,
    gxCyrillicScript            = 8,
    gxRussian                   = gxCyrillicScript,
    gxRSymbolScript             = 9,
    gxDevanagariScript          = 10,
    gxGurmukhiScript            = 11,
    gxGujaratiScript            = 12,
    gxOriyaScript               = 13,
    gxBengaliScript             = 14,
    gxTamilScript               = 15,
    gxTeluguScript              = 16,
    gxKannadaScript             = 17,
    gxMalayalamScript           = 18,
    gxSinhaleseScript           = 19,
    gxBurmeseScript             = 20,
    gxKhmerScript               = 21,
    gxThaiScript                = 22,
    gxLaotianScript             = 23,
    gxGeorgianScript            = 24,
    gxArmenianScript            = 25,
    gxSimpleChineseScript       = 26,
    gxTibetanScript             = 27,
    gxMongolianScript           = 28,
    gxGeezScript                = 29,
    gxEthiopicScript            = gxGeezScript,
    gxAmharicScript             = gxGeezScript,
    gxSlavicScript              = 30,
    gxEastEuropeanRomanScript   = gxSlavicScript,
    gxVietnameseScript          = 31,
    gxExtendedArabicScript      = 32,
    gxSindhiScript              = gxExtendedArabicScript,
    gxUninterpretedScript       = 33
};

enum {
    gxMicrosoftSymbolScript     = 1,
    gxMicrosoftStandardScript   = 2
};

enum {
    gxCustom8BitScript          = 1,
    gxCustom816BitScript        = 2,
    gxCustom16BitScript         = 3
};

typedef long                            gxFontScript;
enum {
    gxEnglishLanguage           = 1,
    gxFrenchLanguage            = 2,
    gxGermanLanguage            = 3,
    gxItalianLanguage           = 4,
    gxDutchLanguage             = 5,
    gxSwedishLanguage           = 6,
    gxSpanishLanguage           = 7,
    gxDanishLanguage            = 8,
    gxPortugueseLanguage        = 9,
    gxNorwegianLanguage         = 10,
    gxHebrewLanguage            = 11,
    gxJapaneseLanguage          = 12,
    gxArabicLanguage            = 13,
    gxFinnishLanguage           = 14,
    gxGreekLanguage             = 15,
    gxIcelandicLanguage         = 16,
    gxMalteseLanguage           = 17,
    gxTurkishLanguage           = 18,
    gxCroatianLanguage          = 19,
    gxTradChineseLanguage       = 20,
    gxUrduLanguage              = 21,
    gxHindiLanguage             = 22,
    gxThaiLanguage              = 23,
    gxKoreanLanguage            = 24,
    gxLithuanianLanguage        = 25,
    gxPolishLanguage            = 26,
    gxHungarianLanguage         = 27,
    gxEstonianLanguage          = 28,
    gxLettishLanguage           = 29,
    gxLatvianLanguage           = gxLettishLanguage,
    gxSaamiskLanguage           = 30,
    gxLappishLanguage           = gxSaamiskLanguage,
    gxFaeroeseLanguage          = 31,
    gxFarsiLanguage             = 32,
    gxPersianLanguage           = gxFarsiLanguage,
    gxRussianLanguage           = 33,
    gxSimpChineseLanguage       = 34,
    gxFlemishLanguage           = 35,
    gxIrishLanguage             = 36,
    gxAlbanianLanguage          = 37,
    gxRomanianLanguage          = 38,
    gxCzechLanguage             = 39,
    gxSlovakLanguage            = 40,
    gxSlovenianLanguage         = 41,
    gxYiddishLanguage           = 42,
    gxSerbianLanguage           = 43,
    gxMacedonianLanguage        = 44,
    gxBulgarianLanguage         = 45,
    gxUkrainianLanguage         = 46,
    gxByelorussianLanguage      = 47,
    gxUzbekLanguage             = 48,
    gxKazakhLanguage            = 49,
    gxAzerbaijaniLanguage       = 50,
    gxAzerbaijanArLanguage      = 51,
    gxArmenianLanguage          = 52,
    gxGeorgianLanguage          = 53,
    gxMoldavianLanguage         = 54,
    gxKirghizLanguage           = 55,
    gxTajikiLanguage            = 56,
    gxTurkmenLanguage           = 57,
    gxMongolianLanguage         = 58,
    gxMongolianCyrLanguage      = 59,
    gxPashtoLanguage            = 60,
    gxKurdishLanguage           = 61,
    gxKashmiriLanguage          = 62,
    gxSindhiLanguage            = 63,
    gxTibetanLanguage           = 64,
    gxNepaliLanguage            = 65,
    gxSanskritLanguage          = 66,
    gxMarathiLanguage           = 67,
    gxBengaliLanguage           = 68,
    gxAssameseLanguage          = 69,
    gxGujaratiLanguage          = 70,
    gxPunjabiLanguage           = 71,
    gxOriyaLanguage             = 72,
    gxMalayalamLanguage         = 73,
    gxKannadaLanguage           = 74,
    gxTamilLanguage             = 75,
    gxTeluguLanguage            = 76,
    gxSinhaleseLanguage         = 77,
    gxBurmeseLanguage           = 78,
    gxKhmerLanguage             = 79,
    gxLaoLanguage               = 80,
    gxVietnameseLanguage        = 81,
    gxIndonesianLanguage        = 82,
    gxTagalogLanguage           = 83,
    gxMalayRomanLanguage        = 84,
    gxMalayArabicLanguage       = 85,
    gxAmharicLanguage           = 86,
    gxTigrinyaLanguage          = 87,
    gxGallaLanguage             = 88,
    gxOromoLanguage             = gxGallaLanguage,
    gxSomaliLanguage            = 89,
    gxSwahiliLanguage           = 90,
    gxRuandaLanguage            = 91,
    gxRundiLanguage             = 92,
    gxChewaLanguage             = 93,
    gxMalagasyLanguage          = 94,
    gxEsperantoLanguage         = 95,
    gxWelshLanguage             = 129,
    gxBasqueLanguage            = 130,
    gxCatalanLanguage           = 131,
    gxLatinLanguage             = 132,
    gxQuechuaLanguage           = 133,
    gxGuaraniLanguage           = 134,
    gxAymaraLanguage            = 135,
    gxTatarLanguage             = 136,
    gxUighurLanguage            = 137,
    gxDzongkhaLanguage          = 138,
    gxJavaneseRomLanguage       = 139,
    gxSundaneseRomLanguage      = 140
};

typedef long                            gxFontLanguage;
enum {
    gxCopyrightFontName         = 1,
    gxFamilyFontName            = 2,
    gxStyleFontName             = 3,
    gxUniqueFontName            = 4,
    gxFullFontName              = 5,
    gxVersionFontName           = 6,
    gxPostscriptFontName        = 7,
    gxTrademarkFontName         = 8,
    gxManufacturerFontName      = 9,
    gxLastReservedFontName      = 256
};

typedef long                            gxFontName;
typedef long                            gxFontTableTag;
typedef long                            gxFontVariationTag;
typedef long                            gxFontFormatTag;
typedef long                            gxFontStorageTag;
typedef gxFontVariationTag              gxFontDescriptorTag;

struct gxFontVariation {
    gxFontVariationTag              name;
    Fixed                           value;
};
typedef struct gxFontVariation          gxFontVariation;

typedef gxFontVariation                 gxFontDescriptor;

struct gxFontFeatureSetting {
    unsigned short                  setting;
    unsigned short                  nameID;
};
typedef struct gxFontFeatureSetting     gxFontFeatureSetting;
enum {
    gxSystemFontAttribute       = 0x0001,
    gxReadOnlyFontAttribute     = 0x0002
};


typedef long                            gxFontAttribute;
enum {
    gxMutuallyExclusiveFeature  = 0x00008000
};

typedef long                            gxFontFeatureFlag;
typedef long                            gxFontFeature;
enum {
    gxResourceFontStorage       = FOUR_CHAR_CODE('rsrc'),
    gxHandleFontStorage         = FOUR_CHAR_CODE('hndl'),
    gxFileFontStorage           = FOUR_CHAR_CODE('bass'),
    gxNfntFontStorage           = FOUR_CHAR_CODE('nfnt')
};

typedef void *                          gxFontStorageReference;
typedef unsigned short                  gxGlyphcode;
/* single glyph in a font */
/* byte offset within backing store */
typedef long                            gxByteOffset;
/* The actual constants for feature types and selectors have been moved to a library. */
typedef unsigned short                  gxRunFeatureType;
typedef unsigned short                  gxRunFeatureSelector;
/* If tracking is not desired, specify the following value in the track field in the
        gxRunControls record (note that a track of 0 does *not* mean to turn tracking off;
        rather, it means to use normal tracking). */
enum {
    gxNoTracking                = gxNegativeInfinity
};

/* The special "gxNoStake" value is returned by the GXGetLayoutBreakOffset call to
        indicate the absence of a character offset that is stable with respect to
        metamorphosis and contextual kerning. */
enum {
    gxNoStake                   = -1
};

/* A glyph's behavior with respect to other glyphs on its line is defined in part by its
        gxBaselineType. These types allow correct alignment of the baselines of all glyphs on
        the line. */
enum {
    gxRomanBaseline             = 0,
    gxIdeographicCenterBaseline = 1,
    gxIdeographicLowBaseline    = 2,
    gxHangingBaseline           = 3,
    gxMathBaseline              = 4,
    gxLastBaseline              = 31,
    gxNumberOfBaselineTypes     = gxLastBaseline + 1,
    gxNoOverrideBaseline        = 255
};

typedef unsigned long                   gxBaselineType;
typedef Fixed                           gxBaselineDeltas[32];
/* gxJustificationPriority defines the point during the justification process at which a
    glyph will begin to receive deltas before and after itself. */
enum {
    gxKashidaPriority           = 0,
    gxWhiteSpacePriority        = 1,
    gxInterCharPriority         = 2,
    gxNullJustificationPriority = 3,
    gxNumberOfJustificationPriorities = 4
};

typedef unsigned char                   gxJustificationPriority;
/* gxJustificationFlags are used to control which fields of a gxWidthDeltaRecord are to
        be overridden and which are not if a gxPriorityJustificationOverride or
        gxGlyphJustificationOverride (qq.v.) is specified. */
enum {
    gxOverridePriority          = 0x8000,                       /* use priority value from override */
    gxOverrideLimits            = 0x4000,                       /* use limits values from override */
    gxOverrideUnlimited         = 0x2000,                       /* use unlimited flag from override */
    gxUnlimitedGapAbsorption    = 0x1000,                       /* glyph can take unlimited gap */
    gxJustificationPriorityMask = 0x000F,                       /* justification priority */
    gxAllJustificationFlags     = (gxOverridePriority | gxOverrideLimits | gxOverrideUnlimited | gxUnlimitedGapAbsorption | gxJustificationPriorityMask)
};

typedef unsigned short                  gxJustificationFlags;
/* The directional behavior of a glyph can be overridden using a gxDirectionOverride. */
enum {
    gxNoDirectionOverride       = 0,
    gxImposeLeftToRight         = 1,
    gxImposeRightToLeft         = 2,
    gxImposeArabic              = 3
};

typedef unsigned short                  gxDirectionOverride;
/* gxRunControlFlags describe the nonparametric layout options contained in a gxStyle. */
enum {
    gxNoCaretAngle              = 0x40000000,
    gxImposeWidth               = 0x20000000,
    gxNoCrossKerning            = 0x10000000,
    gxNoOpticalAlignment        = 0x08000000,
    gxForceHanging              = 0x04000000,
    gxNoSpecialJustification    = 0x02000000,
    gxDirectionOverrideMask     = 0x00000003,
    gxNoLigatureSplits          = (long)0x80000000
};

enum {
    gxAllRunControlFlags        = (gxNoLigatureSplits | gxNoCaretAngle | gxImposeWidth | gxNoCrossKerning | gxNoOpticalAlignment | gxForceHanging | gxNoSpecialJustification | gxDirectionOverrideMask)
};

typedef unsigned long                   gxRunControlFlags;
/* gxHighlightType is used to distinguish various highlighting methods, both in terms of
        character offset based vs. visual based, and in terms of angled sides vs. non-angled
        sides. */
enum {
    gxHighlightStraight         = 0,                            /* straight-edged simple highlighting */
    gxHighlightAverageAngle     = 1                             /* takes average of two edge angles */
};

typedef unsigned long                   gxHighlightType;
/* gxCaretType is used to control whether the caret that is returned from GXGetLayoutCaret
        is a split caret or a (keyboard-syncronized) single caret. */
enum {
    gxSplitCaretType            = 0,                            /* returns Mac-style split caret (default) */
    gxLeftRightKeyboardCaret    = 1,                            /* single caret in left-right position */
    gxRightLeftKeyboardCaret    = 2                             /* single caret in right-left position */
};

typedef unsigned long                   gxCaretType;
/* gxLayoutOffsetState describes the characteristics of a given gxByteOffset in some
        layout. It is returned by the GXGetOffsetGlyphs call. Note that the
        gxOffsetInsideLigature value is returned in addition to the 8/16 (or invalid)
        indication. */
enum {
    gxOffset8_8                 = 0,
    gxOffset8_16                = 1,
    gxOffset16_8                = 2,
    gxOffset16_16               = 3,
    gxOffsetInvalid             = 4
};

enum {
    gxOffsetInsideLigature      = 0x8000
};

typedef unsigned short                  gxLayoutOffsetState;
/* gxLayoutOptionsFlags are single-bit flags contained in a gxLayoutOptions record. We
    also define here some utility constants that are useful in setting various fields in
    the gxLayoutOptions record. */
enum {
    gxNoLayoutOptions           = 0,
    gxLineIsDisplayOnly         = 0x00000001,
    gxKeepSpacesInMargin        = 0x00000002,
    gxLimitReorderingToTwoLevels = 0x00000004,
    gxLineLeftEdgeNotAtMargin   = 0x00000008,
    gxLineRightEdgeNotAtMargin  = 0x00000010,
    gxAllLayoutOptionsFlags     = gxLineIsDisplayOnly | gxKeepSpacesInMargin | gxLimitReorderingToTwoLevels | gxLineLeftEdgeNotAtMargin | gxLineRightEdgeNotAtMargin,
    gxMaxRunLevel               = 15,
    gxFlushLeft                 = 0,
    gxFlushCenter               = fract1 / 2,
    gxFlushRight                = fract1,
    gxNoJustification           = 0,
    gxFullJustification         = fract1
};

typedef unsigned long                   gxLayoutOptionsFlags;
/* A gxRunFeature describes a feature and a level for that feature. */

struct gxRunFeature {
    gxRunFeatureType                featureType;
    gxRunFeatureSelector            featureSelector;
};
typedef struct gxRunFeature             gxRunFeature;
/* A gxWidthDeltaRecord contains all of the information needed to describe the behavior of one
        class of glyphs during the justification process. */

struct gxWidthDeltaRecord {
    Fixed                           beforeGrowLimit;            /* ems AW can grow by at most on LT */
    Fixed                           beforeShrinkLimit;          /* ems AW can shrink by at most on LT */
    Fixed                           afterGrowLimit;             /* ems AW can grow by at most on RB */
    Fixed                           afterShrinkLimit;           /* ems AW can shrink by at most on RB */
    gxJustificationFlags            growFlags;                  /* flags controlling grow case */
    gxJustificationFlags            shrinkFlags;                /* flags controlling shrink case */
};
typedef struct gxWidthDeltaRecord       gxWidthDeltaRecord;
/* A gxPriorityJustificationOverride contains an array of WidthDeltaRecords, one for each
        gxJustificationPriority. */

struct gxPriorityJustificationOverride {
    gxWidthDeltaRecord              deltas[4];                  /* overrides for each of the priorities */
};
typedef struct gxPriorityJustificationOverride gxPriorityJustificationOverride;
/* A gxGlyphJustificationOverride contains a gxWidthDeltaRecord that is to be used for a
        specific glyph in a specific run (this limitation is because glyphcodes vary by font). */

struct gxGlyphJustificationOverride {
    gxGlyphcode                     glyph;
    gxWidthDeltaRecord              override;
};
typedef struct gxGlyphJustificationOverride gxGlyphJustificationOverride;

/* gxRunControls contains flags, shifts, imposed widths and overrides for a run. */
/* NOTE: a value of "gxNoTracking" (see above) in track disables tracking */

struct gxRunControls {
    gxRunControlFlags               flags;
    Fixed                           beforeWithStreamShift;
    Fixed                           afterWithStreamShift;
    Fixed                           crossStreamShift;
    Fixed                           imposedWidth;
    Fixed                           track;
    Fract                           hangingInhibitFactor;
    Fract                           kerningInhibitFactor;
    Fixed                           decompositionAdjustmentFactor;
    gxBaselineType                  baselineType;
};
typedef struct gxRunControls            gxRunControls;
/* A gxGlyphSubstitution describes one client-provided substitution that occurs after all
        other automatic glyph changes have happened. */

struct gxGlyphSubstitution {
    gxGlyphcode                     originalGlyph;              /* Whenever you see this glyph... */
    gxGlyphcode                     substituteGlyph;            /* ...change it to this one. */
};
typedef struct gxGlyphSubstitution      gxGlyphSubstitution;
/* gxKerningAdjustmentFactors specify an adjustment to automatic kerning. The adjustment
        is ax + b where x is the automatic kerning value, a is scaleFactor, and b is
        adjustmentPointSizeFactor times the run's point size. */

struct gxKerningAdjustmentFactors {
    Fract                           scaleFactor;
    Fixed                           adjustmentPointSizeFactor;
};
typedef struct gxKerningAdjustmentFactors gxKerningAdjustmentFactors;
/* A gxKerningAdjustment identifies with- and cross-stream kerning adjustments
        for specific glyph pairs. */

struct gxKerningAdjustment {
    gxGlyphcode                     firstGlyph;
    gxGlyphcode                     secondGlyph;
    gxKerningAdjustmentFactors      withStreamFactors;
    gxKerningAdjustmentFactors      crossStreamFactors;
};
typedef struct gxKerningAdjustment      gxKerningAdjustment;
/* A value of gxResetCrossStreamFactor in crossStreamFactors.adjustmentPointSizeFactor
        will reset the cross-stream kerning to the baseline. */
enum {
    gxResetCrossStreamFactor    = gxNegativeInfinity
};

/* gxLayoutHitInfo contains the output from the GXHitTestLayout call. */

struct gxLayoutHitInfo {
    Fixed                           firstPartialDist;
    Fixed                           lastPartialDist;
    gxByteOffset                    hitSideOffset;
    gxByteOffset                    nonHitSideOffset;
    Boolean                         leadingEdge;
    Boolean                         inLoose;
};
typedef struct gxLayoutHitInfo          gxLayoutHitInfo;
/* A gxLineBaselineRecord contains the deltas from 0 to all the different baselines for
        the layout. It can be filled via a call to GetBaselineDeltas (q.v.). */

struct gxLineBaselineRecord {
    gxBaselineDeltas                deltas;
};
typedef struct gxLineBaselineRecord     gxLineBaselineRecord;
/* The gxLayoutOptions type contains information about the layout characteristics of the
        whole line. */

struct gxLayoutOptions {
    Fixed                           width;
    Fract                           flush;
    Fract                           just;
    gxLayoutOptionsFlags            flags;
    gxLineBaselineRecord *          baselineRec;
};
typedef struct gxLayoutOptions          gxLayoutOptions;
enum {
    gxNewObjectOpcode           = 0x00,                         /* create new object */
    gxSetDataOpcode             = 0x40,                         /* add reference to current object */
    gxSetDefaultOpcode          = 0x80,                         /* replace current default with this object */
    gxReservedOpcode            = 0xC0,                         /* (may be used in future expansion) */
    gxNextOpcode                = 0xFF                          /* used by currentOperand field to say opcode is coming */
};

/* new object types (new object opcode) */
enum {
    gxHeaderTypeOpcode          = 0x00,                         /* byte following new object uses bottom 6 bits for type */
                                                                /* gxShape types use values 1 (gxEmptyType) through 13 (gxPictureType) */
    gxStyleTypeOpcode           = 0x28,
    gxInkTypeOpcode             = 0x29,
    gxTransformTypeOpcode       = 0x2A,
    gxColorProfileTypeOpcode    = 0x2B,
    gxColorSetTypeOpcode        = 0x2C,
    gxTagTypeOpcode             = 0x2D,
    gxBitImageOpcode            = 0x2E,
    gxFontNameTypeOpcode        = 0x2F,
    gxTrailerTypeOpcode         = 0x3F
};

/* fields of objects (set data opcodes) */
enum {
    gxShapeAttributesOpcode     = 0,
    gxShapeTagOpcode            = 1,
    gxShapeFillOpcode           = 2
};

enum {
    gxOmitPathPositionXMask     = 0xC0,
    gxOmitPathPositionYMask     = 0x30,
    gxOmitPathDeltaXMask        = 0x0C,
    gxOmitPathDeltaYMask        = 0x03
};

enum {
    gxOmitPathPositionXShift    = 6,
    gxOmitPathPositionYShift    = 4,
    gxOmitPathDeltaXShift       = 2,
    gxOmitPathDeltaYShift       = 0
};

enum {
    gxOmitBitmapImageMask       = 0xC0,
    gxOmitBitmapWidthMask       = 0x30,
    gxOmitBitmapHeightMask      = 0x0C,
    gxOmitBitmapRowBytesMask    = 0x03
};

enum {
    gxOmitBitmapImageShift      = 6,
    gxOmitBitmapWidthShift      = 4,
    gxOmitBitmapHeightShift     = 2,
    gxOmitBitmapRowBytesShift   = 0
};

enum {
    gxOmitBitmapPixelSizeMask   = 0xC0,
    gxOmitBitmapSpaceMask       = 0x30,
    gxOmitBitmapSetMask         = 0x0C,
    gxOmitBitmapProfileMask     = 0x03
};

enum {
    gxOmitBitmapPixelSizeShift  = 6,
    gxOmitBitmapSpaceShift      = 4,
    gxOmitBitmapSetShift        = 2,
    gxOmitBitmapProfileShift    = 0
};

enum {
    gxOmitBitmapPositionXMask   = 0xC0,
    gxOmitBitmapPositionYMask   = 0x30
};

enum {
    gxOmitBitmapPositionXShift  = 6,
    gxOmitBitmapPositionYShift  = 4
};

enum {
    gxOmitBitImageRowBytesMask  = 0xC0,
    gxOmitBitImageHeightMask    = 0x30,
    gxOmitBitImageDataMask      = 0x08
};

enum {
    gxOmitBitImageRowBytesShift = 6,
    gxOmitBitImageHeightShift   = 4,
    gxOmitBitImageDataShift     = 3
};

enum {
    gxCopyBitImageBytesOpcode   = 0x00,
    gxRepeatBitImageBytesOpcode = 0x40,
    gxLookupBitImageBytesOpcode = 0x80,
    gxRepeatBitImageScanOpcode  = 0xC0
};

enum {
    gxOmitTextCharactersMask    = 0xC0,
    gxOmitTextPositionXMask     = 0x30,
    gxOmitTextPositionYMask     = 0x0C,
    gxOmitTextDataMask          = 0x02
};

enum {
    gxOmitTextCharactersShift   = 6,
    gxOmitTextPositionXShift    = 4,
    gxOmitTextPositionYShift    = 2,
    gxOmitTextDataShift         = 1
};

enum {
    gxOmitGlyphCharactersMask   = 0xC0,
    gxOmitGlyphLengthMask       = 0x30,
    gxOmitGlyphRunNumberMask    = 0x0C,
    gxOmitGlyphOnePositionMask  = 0x02,
    gxOmitGlyphDataMask         = 0x01
};

enum {
    gxOmitGlyphCharactersShift  = 6,
    gxOmitGlyphLengthShift      = 4,
    gxOmitGlyphRunNumberShift   = 2,
    gxOmitGlyphOnePositionShift = 1,
    gxOmitGlyphDataShift        = 0
};

enum {
    gxOmitGlyphPositionsMask    = 0xC0,
    gxOmitGlyphAdvancesMask     = 0x20,
    gxOmitGlyphTangentsMask     = 0x18,
    gxOmitGlyphRunsMask         = 0x04,
    gxOmitGlyphStylesMask       = 0x03
};

enum {
    gxOmitGlyphPositionsShift   = 6,
    gxOmitGlyphAdvancesShift    = 5,
    gxOmitGlyphTangentsShift    = 3,
    gxOmitGlyphRunsShift        = 2,
    gxOmitGlyphStylesShift      = 0
};

enum {
    gxOmitLayoutLengthMask      = 0xC0,
    gxOmitLayoutPositionXMask   = 0x30,
    gxOmitLayoutPositionYMask   = 0x0C,
    gxOmitLayoutDataMask        = 0x02
};

enum {
    gxOmitLayoutLengthShift     = 6,
    gxOmitLayoutPositionXShift  = 4,
    gxOmitLayoutPositionYShift  = 2,
    gxOmitLayoutDataShift       = 1
};

enum {
    gxOmitLayoutWidthMask       = 0xC0,
    gxOmitLayoutFlushMask       = 0x30,
    gxOmitLayoutJustMask        = 0x0C,
    gxOmitLayoutOptionsMask     = 0x03
};

enum {
    gxOmitLayoutWidthShift      = 6,
    gxOmitLayoutFlushShift      = 4,
    gxOmitLayoutJustShift       = 2,
    gxOmitLayoutOptionsShift    = 0
};

enum {
    gxOmitLayoutStyleRunNumberMask = 0xC0,
    gxOmitLayoutLevelRunNumberMask = 0x30,
    gxOmitLayoutHasBaselineMask = 0x08,
    gxOmitLayoutStyleRunsMask   = 0x04,
    gxOmitLayoutStylesMask      = 0x03
};

enum {
    gxOmitLayoutStyleRunNumberShift = 6,
    gxOmitLayoutLevelRunNumberShift = 4,
    gxOmitLayoutHasBaselineShift = 3,
    gxOmitLayoutStyleRunsShift  = 2,
    gxOmitLayoutStylesShift     = 0
};

enum {
    gxOmitLayoutLevelRunsMask   = 0x80,
    gxOmitLayoutLevelsMask      = 0x40
};

enum {
    gxOmitLayoutLevelRunsShift  = 7,
    gxOmitLayoutLevelsShift     = 6
};

enum {
    gxInkAttributesOpcode       = 0,
    gxInkTagOpcode              = 1,
    gxInkColorOpcode            = 2,
    gxInkTransferModeOpcode     = 3
};

enum {
    gxOmitColorsSpaceMask       = 0xC0,
    gxOmitColorsProfileMask     = 0x30,
    gxOmitColorsComponentsMask  = 0x0F,
    gxOmitColorsIndexMask       = 0x0C,
    gxOmitColorsIndexSetMask    = 0x03
};

enum {
    gxOmitColorsSpaceShift      = 6,
    gxOmitColorsProfileShift    = 4,
    gxOmitColorsComponentsShift = 0,
    gxOmitColorsIndexShift      = 2,
    gxOmitColorsIndexSetShift   = 0
};

enum {
    gxOmitTransferSpaceMask     = 0xC0,
    gxOmitTransferSetMask       = 0x30,
    gxOmitTransferProfileMask   = 0x0C
};

enum {
    gxOmitTransferSpaceShift    = 6,
    gxOmitTransferSetShift      = 4,
    gxOmitTransferProfileShift  = 2
};

enum {
    gxOmitTransferSourceMatrixMask = 0xC0,
    gxOmitTransferDeviceMatrixMask = 0x30,
    gxOmitTransferResultMatrixMask = 0x0C,
    gxOmitTransferFlagsMask     = 0x03
};

enum {
    gxOmitTransferSourceMatrixShift = 6,
    gxOmitTransferDeviceMatrixShift = 4,
    gxOmitTransferResultMatrixShift = 2,
    gxOmitTransferFlagsShift    = 0
};

enum {
    gxOmitTransferComponentModeMask = 0x80,
    gxOmitTransferComponentFlagsMask = 0x40,
    gxOmitTransferComponentSourceMinimumMask = 0x30,
    gxOmitTransferComponentSourceMaximumMask = 0x0C,
    gxOmitTransferComponentDeviceMinimumMask = 0x03
};

enum {
    gxOmitTransferComponentModeShift = 7,
    gxOmitTransferComponentFlagsShift = 6,
    gxOmitTransferComponentSourceMinimumShift = 4,
    gxOmitTransferComponentSourceMaximumShift = 2,
    gxOmitTransferComponentDeviceMinimumShift = 0
};

enum {
    gxOmitTransferComponentDeviceMaximumMask = 0xC0,
    gxOmitTransferComponentClampMinimumMask = 0x30,
    gxOmitTransferComponentClampMaximumMask = 0x0C,
    gxOmitTransferComponentOperandMask = 0x03
};

enum {
    gxOmitTransferComponentDeviceMaximumShift = 6,
    gxOmitTransferComponentClampMinimumShift = 4,
    gxOmitTransferComponentClampMaximumShift = 2,
    gxOmitTransferComponentOperandShift = 0
};

enum {
    gxStyleAttributesOpcode     = 0,
    gxStyleTagOpcode            = 1,
    gxStyleCurveErrorOpcode     = 2,
    gxStylePenOpcode            = 3,
    gxStyleJoinOpcode           = 4,
    gxStyleDashOpcode           = 5,
    gxStyleCapsOpcode           = 6,
    gxStylePatternOpcode        = 7,
    gxStyleTextAttributesOpcode = 8,
    gxStyleTextSizeOpcode       = 9,
    gxStyleFontOpcode           = 10,
    gxStyleTextFaceOpcode       = 11,
    gxStylePlatformOpcode       = 12,
    gxStyleFontVariationsOpcode = 13,
    gxStyleRunControlsOpcode    = 14,
    gxStyleRunPriorityJustOverrideOpcode = 15,
    gxStyleRunGlyphJustOverridesOpcode = 16,
    gxStyleRunGlyphSubstitutionsOpcode = 17,
    gxStyleRunFeaturesOpcode    = 18,
    gxStyleRunKerningAdjustmentsOpcode = 19,
    gxStyleJustificationOpcode  = 20
};

enum {
    gxOmitDashAttributesMask    = 0xC0,
    gxOmitDashShapeMask         = 0x30,
    gxOmitDashAdvanceMask       = 0x0C,
    gxOmitDashPhaseMask         = 0x03
};

enum {
    gxOmitDashAttributesShift   = 6,
    gxOmitDashShapeShift        = 4,
    gxOmitDashAdvanceShift      = 2,
    gxOmitDashPhaseShift        = 0
};

enum {
    gxOmitDashScaleMask         = 0xC0
};

enum {
    gxOmitDashScaleShift        = 6
};

enum {
    gxOmitPatternAttributesMask = 0xC0,
    gxOmitPatternShapeMask      = 0x30,
    gxOmitPatternUXMask         = 0x0C,
    gxOmitPatternUYMask         = 0x03
};

enum {
    gxOmitPatternAttributesShift = 6,
    gxOmitPatternShapeShift     = 4,
    gxOmitPatternUXShift        = 2,
    gxOmitPatternUYShift        = 0
};

enum {
    gxOmitPatternVXMask         = 0xC0,
    gxOmitPatternVYMask         = 0x30
};

enum {
    gxOmitPatternVXShift        = 6,
    gxOmitPatternVYShift        = 4
};

enum {
    gxOmitJoinAttributesMask    = 0xC0,
    gxOmitJoinShapeMask         = 0x30,
    gxOmitJoinMiterMask         = 0x0C
};

enum {
    gxOmitJoinAttributesShift   = 6,
    gxOmitJoinShapeShift        = 4,
    gxOmitJoinMiterShift        = 2
};

enum {
    gxOmitCapAttributesMask     = 0xC0,
    gxOmitCapStartShapeMask     = 0x30,
    gxOmitCapEndShapeMask       = 0x0C
};

enum {
    gxOmitCapAttributesShift    = 6,
    gxOmitCapStartShapeShift    = 4,
    gxOmitCapEndShapeShift      = 2
};

enum {
    gxOmitFaceLayersMask        = 0xC0,
    gxOmitFaceMappingMask       = 0x30
};

enum {
    gxOmitFaceLayersShift       = 6,
    gxOmitFaceMappingShift      = 4
};

enum {
    gxOmitFaceLayerFillMask     = 0xC0,
    gxOmitFaceLayerFlagsMask    = 0x30,
    gxOmitFaceLayerStyleMask    = 0x0C,
    gxOmitFaceLayerTransformMask = 0x03
};

enum {
    gxOmitFaceLayerFillShift    = 6,
    gxOmitFaceLayerFlagsShift   = 4,
    gxOmitFaceLayerStyleShift   = 2,
    gxOmitFaceLayerTransformShift = 0
};

enum {
    gxOmitFaceLayerBoldXMask    = 0xC0,
    gxOmitFaceLayerBoldYMask    = 0x30
};

enum {
    gxOmitFaceLayerBoldXShift   = 6,
    gxOmitFaceLayerBoldYShift   = 4
};

enum {
    gxColorSetReservedOpcode    = 0,
    gxColorSetTagOpcode         = 1
};

enum {
    gxColorProfileReservedOpcode = 0,
    gxColorProfileTagOpcode     = 1
};

enum {
    gxTransformReservedOpcode   = 0,
    gxTransformTagOpcode        = 1,
    gxTransformClipOpcode       = 2,
    gxTransformMappingOpcode    = 3,
    gxTransformPartMaskOpcode   = 4,
    gxTransformToleranceOpcode  = 5
};

enum {
    gxTypeOpcode                = 0,
    gxSizeOpcode                = 1
};

/* used by currentOperand when currentOperation is gxNextOpcode */
/*    format of top byte:
xx yyyyyy   xx == 0x00, 0x40, 0x80, 0xC0: defines graphics operation (see gxGraphicsOperationOpcode)
            yyyyyy == size of operation in bytes
            if (yyyyyy == 0), byte size follows. If byte following == 0, word size follows; if == 0, long follows
            word and long, if present, are specified in high-endian order (first byte is largest magnitude)
            
format of byte following size specifiers, if any:
xx yyyyyy   xx == 0x00, 0x40, 0x80, 0xC0: defines compression level (0 == none, 0xC0 == most)
            exact method of compression is defined by type of data
            yyyyyy == data type selector (0 to 63): see gxGraphicsNewOpcode, __DataOpcode
*/
enum {
    gxOpcodeShift               = 6,
    gxObjectSizeMask            = 0x3F,
    gxCompressionShift          = 6,
    gxObjectTypeMask            = 0x3F,
    gxBitImageOpcodeMask        = 0xC0,
    gxBitImageCountMask         = 0x3F,
    gxBitImageOpcodeShift       = 6
};

enum {
    gxNoCompression             = 0,
    gxWordCompression           = 1,
    gxByteCompression           = 2,
    gxOmitCompression           = 3,
    gxCompressionMask           = 0x03
};

/*    the following structures define how primitives without a public geometry
    are stored (their format mirrors that of the New call to create them)   */

struct gxFlatFontName {
    unsigned char                   name;                       /* gxFontName */
    unsigned char                   platform;                   /* gxFontPlatform */
    unsigned char                   script;                     /* gxFontScript */
    unsigned char                   language;                   /* gxFontLanguage */
    short                           length;                     /* byte length */
};
typedef struct gxFlatFontName           gxFlatFontName;
enum {
    gxFlatFontListItemTag       = FOUR_CHAR_CODE('flst')
};


struct gxFlatFontListItem {
    gxFont                          fontID;                     /*** if we get rid of this, remove #include "font types.h", above */
    unsigned char                   name;                       /* gxFontName */
    unsigned char                   platform;                   /* gxFontPlatform */
    unsigned char                   script;                     /* gxFontScript */
    unsigned char                   language;                   /* gxFontLanguage */
    short                           length;                     /* byte length of the name that follows */
    unsigned short                  glyphCount;                 /* CountFontGlyphs or 0 if gxFontGlyphsFlatten is false */
    unsigned short                  axisCount;                  /* CountFontVariations or 0 if gxFontVariationsFlatten is false */
    unsigned short                  variationCount;             /* number of bitsVariationPairs that follow the (optional) glyphBits */
};
typedef struct gxFlatFontListItem       gxFlatFontListItem;

struct gxFlatFontList {
    long                            count;
    gxFlatFontListItem              items[1];
};
typedef struct gxFlatFontList           gxFlatFontList;

struct gxFlattenHeader {
    Fixed                           version;
    unsigned char                   flatFlags;
    SInt8                           padding;
};
typedef struct gxFlattenHeader          gxFlattenHeader;
enum {
    gxOmitPictureShapeMask      = 0xC0,
    gxOmitOverrideStyleMask     = 0x30,
    gxOmitOverrideInkMask       = 0x0C,
    gxOmitOverrideTransformMask = 0x03
};

enum {
    gxOmitPictureShapeShift     = 0x06,
    gxOmitOverrideStyleShift    = 0x04,
    gxOmitOverrideInkShift      = 0x02,
    gxOmitOverrideTransformShift = 0x00
};

enum {
    gxPostScriptTag             = FOUR_CHAR_CODE('post'),
    gxPostControlTag            = FOUR_CHAR_CODE('psct')
};

enum {
    gxNoSave                    = 1,                            /* don't do save-restore around PS data */
    gxPSContinueNext            = 2                             /* next shape is continuation of this shape's PS -- only obeyed if gxNoSave is true */
};


struct gxPostControl {
    long                            flags;                      /* PostScript state flags */
};
typedef struct gxPostControl            gxPostControl;
enum {
    gxDashSynonymTag            = FOUR_CHAR_CODE('sdsh')
};


struct gxDashSynonym {
    long                            size;                       /* number of elements in array */
    Fixed                           dashLength[1];              /* Array of dash lengths */
};
typedef struct gxDashSynonym            gxDashSynonym;
enum {
    gxLineCapSynonymTag         = FOUR_CHAR_CODE('lcap')
};

enum {
    gxButtCap                   = 0,
    gxRoundCap                  = 1,
    gxSquareCap                 = 2,
    gxTriangleCap               = 3
};

/* gxLine cap type */

typedef long                            gxLineCapSynonym;
enum {
    gxCubicSynonymTag           = FOUR_CHAR_CODE('cubx')
};

enum {
    gxIgnoreFlag                = 0x0000,                       /* Ignore this word, get next one */
    gxLineToFlag                = 0x0001,                       /* Draw a gxLine to gxPoint following this flag */
    gxCurveToFlag               = 0x0002,                       /* Draw a gxCurve through the 3 points following this flag */
    gxMoveToFlag                = 0x0003,                       /* Start a new contour at the gxPoint following this flag */
    gxClosePathFlag             = 0x0004                        /* Close the contour */
};

typedef long                            gxCubicSynonym;
enum {
    gxCubicInstructionMask      = 0x000F                        /* low four bits are gxPoint instructions */
};

/* Low four bits are instruction (moveto, lineto, curveto, closepath) */
typedef short                           gxCubicSynonymFlags;
enum {
    gxPatternSynonymTag         = FOUR_CHAR_CODE('ptrn')
};

enum {
    gxHatch                     = 0,
    gxCrossHatch                = 1
};


struct gxPatternSynonym {
    long                            patternType;                /* one of the gxPatterns: gxHatch or gxCrossHatch */
    Fixed                           angle;                      /* angle at which pattern is drawn */
    Fixed                           spacing;                    /* distance between two parallel pattern lines */
    Fixed                           thickness;                  /* thickness of the pattern */
    gxPoint                         anchorPoint;                /* gxPoint with with respect to which pattern position is calculated */
};
typedef struct gxPatternSynonym         gxPatternSynonym;
enum {
    gxURLTag                    = FOUR_CHAR_CODE('urlt')
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

#endif /* __GXTYPES__ */

