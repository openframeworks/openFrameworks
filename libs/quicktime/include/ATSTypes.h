/*
     File:       ATSTypes.h
 
     Contains:   Public interfaces for Apple Type Services components.
 
     Version:    Technology: Post-Sonata, Post-Carbon 1.01
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1997-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ATSTYPES__
#define __ATSTYPES__

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

/* ----------------------------------------------------------------------------------------- */
/* CONSTANTS */
/* Miscellaneous constants */
enum {
    kATSItalicQDSkew            = (1 << 16) / 4,                /* fixed value of 0.25 */
    kATSRadiansFactor           = 1144                          /* fixed value of approx. pi/180 (0.0174560546875) */
};

/* Glyph outline path constants used in ATSGetGlyphPaths */
enum {
    kATSCubicCurveType          = 0x0001,
    kATSQuadCurveType           = 0x0002
};

/* --------------------------------------------------------------------------- */
/* TYPES */
/* --------------------------------------------------------------------------- */
typedef UInt16                          GlyphID;
typedef UInt16                          ATSCurveType;
/*
    The StyleGroupPtr contains all of the style attributes associated with a run
    of glyphs.
*/
typedef struct AttributeGroup *ATSStyleGroupPtr;

struct ATSStyleSetting {
    ATSStyleGroupPtr                style;                      /* The style attribute group */
    UInt32                          styleFlags;                 /* Flag bits for this style */
};
typedef struct ATSStyleSetting          ATSStyleSetting;
/* Mapping3x3 is the basic matrix type. The third column actually contains Fract numbers */

struct Mapping3x3 {
    Fixed                           map[3][3];
};
typedef struct Mapping3x3               Mapping3x3;
/* Rendering metrics type */

struct ATSGlyphRenderingMetrics {
    FixedPoint                      deviceAdvance;
    FixedPoint                      topLeft;
    UInt16                          height;
    UInt16                          width;
    FixedPoint                      sideBearing;
    FixedPoint                      otherSideBearing;
};
typedef struct ATSGlyphRenderingMetrics ATSGlyphRenderingMetrics;
/* Fractional metrics types */

struct ATSGlyphFractionalMetrics {
    FixedPoint                      advance;
    FixedPoint                      sideBearing;
    FixedPoint                      otherSideBearing;
};
typedef struct ATSGlyphFractionalMetrics ATSGlyphFractionalMetrics;

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

#endif /* __ATSTYPES__ */

