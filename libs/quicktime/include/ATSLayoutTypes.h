/*
     File:       ATSLayoutTypes.h
 
     Contains:   Apple Text Services layout public structures and constants.
 
     Version:    Technology: Allegro
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ATSLAYOUTTYPES__
#define __ATSLAYOUTTYPES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __SFNTTYPES__
#include <SFNTTypes.h>
#endif

#ifndef __SFNTLAYOUTTYPES__
#include <SFNTLayoutTypes.h>
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

/* --------------------------------------------------------------------------- */
/* CONSTANTS */
/* --------------------------------------------------------------------------- */
/* Miscellaneous constants */
enum {
    kATSItalicQDSkew            = (1 << 16) / 4,                /* fixed value of 0.25 */
    kATSRadiansFactor           = 1144,                         /* fixed value of approx. pi/180 (0.0174560546875) */
    kATSUseLineHeight           = 0x7FFFFFFF,                   /* assignment to use natural line ascent/descent values */
    kATSNoTracking              = (long)0x80000000              /* negativeInfinity */
};

/* LineOptions flags */
enum {
    kATSLineNoLayoutOptions     = 0x00000000,                   /* no options */
    kATSLineIsDisplayOnly       = 0x00000001,                   /* specifies to optimize for displaying text only */
    kATSLineHasNoHangers        = 0x00000002,                   /* specifies that no hangers to be formed on the line */
    kATSLineHasNoOpticalAlignment = 0x00000004,                 /* specifies that no optical alignment to be performed on the line */
    kATSLineKeepSpacesOutOfMargin = 0x00000008,                 /* specifies that space charcters should not be treated as hangers */
    kATSLineNoSpecialJustification = 0x00000010,                /* specifies no post-compensation justification is to be performed */
    kATSLineLastNoJustification = 0x00000020,                   /* specifies that if the line is the last of a paragraph, it will not get justified */
    kATSLineFractDisable        = 0x00000040,                   /* specifies that the displayed line glyphs will adjust for device metrics */
    kATSLineImposeNoAngleForEnds = 0x00000080,                  /* specifies that the carets at the ends of the line will be guarenteed to be perpendicular to the baseline */
    kATSLineFillOutToWidth      = 0x00000100,                   /* highlights for the line end characters will be extended to 0 and the specified line width */
    kATSLineTabAdjustEnabled    = 0x00000200,                   /* specifies that the tab character width will be automatically adjusted to fit the specified line width */
    kATSLineAppleReserved       = (long)0xFFFFFC00              /* these bits are reserved by Apple and will result in a invalid value error if attemped to set */
};

/* For accessing glyph bounds */
enum {
    kATSUseCaretOrigins         = 0,
    kATSUseDeviceOrigins        = 1,
    kATSUseFractionalOrigins    = 2,
    kATSUseOriginFlags          = 3
};

/* --------------------------------------------------------------------------- */
/* TYPES */
/* --------------------------------------------------------------------------- */
typedef UInt32                          ATSLineLayoutOptions;
/*
    Each individual glyph is assigned an index of its needed style
    which could be changed during the layout process (in that case, the associated style's 
    ATSStyleSetting styleFlags field would have its kStyleSynthesized bit set).
    
|*
    The ATSTrapezoid structure supplies a convenient container
    for glyph bounds in trapezoidal form.
*/

struct ATSTrapezoid {
    FixedPoint                      upperLeft;
    FixedPoint                      upperRight;
    FixedPoint                      lowerRight;
    FixedPoint                      lowerLeft;
};
typedef struct ATSTrapezoid             ATSTrapezoid;
/*
    The JustWidthDeltaEntryOverride structure specifies values for the grow and shrink case during
    justification, both on the left and on the right. It also contains flags.  This particular structure
    is used for passing justification overrides to LLC.  For further sfnt resource 'just' table
    constants and structures, see SFNTLayoutTypes.h.
*/

struct ATSJustWidthDeltaEntryOverride {
    Fixed                           beforeGrowLimit;            /* ems AW can grow by at most on LT */
    Fixed                           beforeShrinkLimit;          /* ems AW can shrink by at most on LT */
    Fixed                           afterGrowLimit;             /* ems AW can grow by at most on RB */
    Fixed                           afterShrinkLimit;           /* ems AW can shrink by at most on RB */
    JustificationFlags              growFlags;                  /* flags controlling grow case */
    JustificationFlags              shrinkFlags;                /* flags controlling shrink case */
};
typedef struct ATSJustWidthDeltaEntryOverride ATSJustWidthDeltaEntryOverride;
/* The JustPriorityOverrides type is an array of 4 width delta records, one per priority level override. */

typedef ATSJustWidthDeltaEntryOverride  ATSJustPriorityWidthDeltaOverrides[4];
/* --------------------------------------------------------------------------- */

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

#endif /* __ATSLAYOUTTYPES__ */

