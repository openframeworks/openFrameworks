/*
     File:       ATSUnicode.h
 
     Contains:   Public interfaces for Apple Type Services for Unicode Imaging
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1997-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ATSUNICODE__
#define __ATSUNICODE__

#ifndef __ATSLAYOUTTYPES__
#include <ATSLayoutTypes.h>
#endif

#ifndef __SFNTLAYOUTTYPES__
#include <SFNTLayoutTypes.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MACMEMORY__
#include <MacMemory.h>
#endif

#ifndef __FONTS__
#include <Fonts.h>
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

/************/
/*  Types   */
/************/
/*
   UniCharArrayHandle and UniCharArrayOffset are defined in the
   TextCommon interface file.
*/

/*
    ATSUTextMeasurement is specific to ATSUI in that measurement
    quantities are in fractional Fixed format instead of shorts
    used in QuickDraw Text.  This provides exact outline metrics and
    line specifications such as line width, ascent, descent, and so on.  
*/

typedef Fixed                           ATSUTextMeasurement;
/*
    ATSUFontID indicates a particular font family and face.  
    ATSUFontID's are not guaranteed to remain constant across 
    reboots.  Clients should use the font's unique name to 
    get a font token to store in documents which is 
    guaranteed to remain constant across reboots. 
*/
typedef FMFont                          ATSUFontID;
/*
    ATSUFontFeatureType and ATSUFontFeatureSelector are used 
    to identify font features.  
*/
typedef UInt16                          ATSUFontFeatureType;
typedef UInt16                          ATSUFontFeatureSelector;
/*
    ATSUFontVariationAxis and ATSUFontVariationValue are used 
    in connection with font variations.  
*/
typedef FourCharCode                    ATSUFontVariationAxis;
typedef Fixed                           ATSUFontVariationValue;
/*
    ATSUTextLayout is used to store the attribute information 
    associated with a contiguous block of UniChar's (UInt16's) 
    in memory.  It's typed to be an opaque structure.  
*/

typedef struct OpaqueATSUTextLayout*    ATSUTextLayout;
/*
    ATSUStyle is used to store a set of individual attributes, 
    font features, and font variations.  It's typed to be 
    an opaque structure.  
*/
typedef struct OpaqueATSUStyle*         ATSUStyle;
/*
    ATSUAttributeTag is used to indicate the particular type 
    of attribute under consideration:  font, size, color, 
    and so on.  
    Each style run may have at most one attribute with a 
    given ATSUAttributeTag (i.e., a style run can't have 
    more than one font or size) but may have none.  
*/
typedef UInt32                          ATSUAttributeTag;
/*
    ATSUAttributeValuePtr is used to provide generic access 
    to storage of attribute values, which vary in size.
*/
typedef void *                          ATSUAttributeValuePtr;
/*
    ATSUAttributeInfo is used to provide a tag/size pairing.  
    This makes it possible to provide the client information   
    about all the attributes for a given range of text.  This   
    structure is only used to return to the client information   
    about a complete set of attributes.  An array of   
    ATSUAttributeInfos is passed as a parameter so that the   
    client can find out what attributes are set and what their   
    individual sizes are; with that information, they can then   
    query about the values of the attributes they're interested   
    in.  Because arrays of ATSUAttributeInfos are used as parameters   
    to functions, they have to be of a fixed size, hence the   
    value is not included in the structure.  
*/

struct ATSUAttributeInfo {
    ATSUAttributeTag                fTag;
    ByteCount                       fValueSize;
};
typedef struct ATSUAttributeInfo        ATSUAttributeInfo;
/*
    ATSUCaret contains the complete information needed to render a
    caret.  fX and fY is the position of one of the caret's ends
    relative to the origin position of the line the caret belongs.
    fDeltaX and fDeltaY is the position of the caret's other end.
    Hence, to draw a caret, simply call MoveTo(fX, fY) followed by
    LineTo(fDeltaX, fDeltaY) or equivalent.  The ATSUCaret will
    contain the positions needed to draw carets on angled lines
    and reflect angled carets and leading/trailing split caret
    appearances.
*/

struct ATSUCaret {
    Fixed                           fX;
    Fixed                           fY;
    Fixed                           fDeltaX;
    Fixed                           fDeltaY;
};
typedef struct ATSUCaret                ATSUCaret;
/*
    ATSUCursorMovementType currently can take three values 
    (kATSUByCharacter, kATSUByCluster, and kATSUByWord) 
    and is used to indicate how much to move the cursor.  
*/

typedef UInt16                          ATSUCursorMovementType;
/*
    ATSUVerticalCharacterType currently can take two values 
    (kATSUStronglyVertical, and kATSUStronglyHorizontal) and 
    is used to indicate whether text is to be laid out as 
    vertical glyphs or horizontal glyphs.  
*/
typedef UInt16                          ATSUVerticalCharacterType;
/*
    ATSUStyleComparison is an enumeration with four values 
    (kATUStyleUnequal, ATSUStyleContains, kATSUStyleEquals, 
    and kATSUStyleContainedBy), and is used by ATSUCompareStyles() 
    to indicate if the first style parameter contains as a 
    proper subset, is equal to, or is contained by the second 
    style parameter.
*/
typedef UInt16                          ATSUStyleComparison;
/*
    ATSUFontFallbackMethod type defines the type of heap or memory callback
    method ATSUI is to follow iin all its permanent memory allocations for
    it clients.
*/
typedef UInt16                          ATSUFontFallbackMethod;
/*
    ATSUMemoryCallbacks is a union struct that allows the ATSUI 
    client to specify a specific heap for ATSUI use or allocation
    callbacks of which ATSUI is to use each time ATSUI performs a
    memory operation (alloc, grow, free).
*/
typedef CALLBACK_API_C( void *, ATSUCustomAllocFunc )(void *refCon, ByteCount howMuch);
typedef CALLBACK_API_C( void , ATSUCustomFreeFunc )(void *refCon, void *doomedBlock);
typedef CALLBACK_API_C( void *, ATSUCustomGrowFunc )(void *refCon, void *oldBlock, ByteCount oldSize, ByteCount newSize);

union ATSUMemoryCallbacks {
    struct {
        ATSUCustomAllocFunc             Alloc;
        ATSUCustomFreeFunc              Free;
        ATSUCustomGrowFunc              Grow;
        void *                          memoryRefCon;
    }                                 callbacks;

    THz                             heapToUse;
};
typedef union ATSUMemoryCallbacks       ATSUMemoryCallbacks;
/*
    ATSUHeapSpec provides allows the ATSUI client a means of
    specifying the heap ATSUI should allocate it's dynamic memory
    from or that ATSUI should use the memory callback provided by
    the client.
*/

typedef UInt16                          ATSUHeapSpec;
/*
    ATSUMemorySetting is used to store the results from a ATSUSetMemoryAlloc
    or a ATSUGetCurrentMemorySetting call.  It can also be used to change the
    current ATSUMemorySetting by passing it into the ATSUSetCurrentMemorySetting
    call.
*/
typedef struct OpaqueATSUMemorySetting*  ATSUMemorySetting;
/*******************************************************************************/
/*  ATSUI Attribute tags:  Apple reserves values 0 to 65,535 (0 to 0x0000FFFF) */
/*  ATSUI clients may create their own tags with any other value               */
/*******************************************************************************/
/*  Line Control Attribute Tags */
enum {
    kATSULineWidthTag           = 1L,                           /*    Type:       ATSUTextMeasurement*/
                                                                /*    Default value: 0*/
    kATSULineRotationTag        = 2L,                           /*    Type:       Fixed (fixed value in degrees in right-handed coordinate system)*/
                                                                /*    Default value: 0*/
    kATSULineDirectionTag       = 3L,                           /*    Type:       Boolean; values 0 or 1 (see below for value identities)*/
                                                                /*    Default value: GetSysDirection() ? kATSURightToLeftBaseDirection : kATSULeftToRightBaseDirection*/
    kATSULineJustificationFactorTag = 4L,                       /*    Type:       Fract between 0 and 1*/
                                                                /*    Default value: kATSUNoJustification*/
    kATSULineFlushFactorTag     = 5L,                           /*    Type:       Fract between 0 and 1 */
                                                                /*    Default value: kATSUStartAlignment*/
    kATSULineBaselineValuesTag  = 6L,                           /*    Type:       BslnBaselineRecord*/
                                                                /*    Default value: All zeros.  Calculated from other style attributes (e.g., font and point size)*/
    kATSULineLayoutOptionsTag   = 7L,                           /*    Type:       UInt32*/
                                                                /*    Default value: kATSLineNoLayoutOptions - other options listed in ATSLayoutTypes.h*/
    kATSULineAscentTag          = 8L,                           /*    Type:       ATSUTextMeasurement*/
                                                                /*    Default value: kATSUseLineHeight*/
    kATSULineDescentTag         = 9L,                           /*    Type:       ATSUTextMeasurement*/
                                                                /*    Default value: kATSUseLineHeight*/
    kATSULineLanguageTag        = 10L,                          /*    Type:       RegionCode - region values listed in script.h interface file*/
                                                                /*    Default value: kTextRegionDontCare*/
    kATSULineTextLocatorTag     = 11L,                          /*    Type:       TextBreakLocatorRef*/
                                                                /*    Default value: NULL - set Region derived locator or the default Text Utilities locator*/
    kATSUMaxLineTag             = 12L                           /*    This is just for convenience - the upper limit of the ATSUTextLayout attribute tags */
};

/*  Run Style Attribute Tags */
enum {
                                                                /* QuickDraw compatibility tags */
    kATSUQDBoldfaceTag          = 256L,                         /*    Type:       Boolean    */
                                                                /*    Default value: false*/
    kATSUQDItalicTag            = 257L,                         /*    Type:       Boolean       */
                                                                /*    Default value: false*/
    kATSUQDUnderlineTag         = 258L,                         /*    Type:       Boolean    */
                                                                /*    Default value: false*/
    kATSUQDCondensedTag         = 259L,                         /*    Type:       Boolean    */
                                                                /*    Default value: false*/
    kATSUQDExtendedTag          = 260L,                         /*    Type:       Boolean    */
                                                                /*    Default value: false*/
                                                                /* Common run tags */
    kATSUFontTag                = 261L,                         /*    Type:       ATSUFontID */
                                                                /*    Default value: GetScriptVariable( smSystemScript, smScriptAppFond )*/
    kATSUSizeTag                = 262L,                         /*    Type:       Fixed  */
                                                                /*    Default value: GetScriptVariable( smSystemScript, smScriptAppFondSize )    */
    kATSUColorTag               = 263L,                         /*    Type:       RGBColor   */
                                                                /*    Default value: (0, 0, 0)*/
                                                                /*    Less common run tags */
    kATSULanguageTag            = 264L,                         /*    Type:       RegionCode - region values listed in script.h interface file*/
                                                                /*    Default value: GetScriptManagerVariable( smRegionCode )*/
    kATSUVerticalCharacterTag   = 265L,                         /*    Type:       ATSUVerticalCharacterType  */
                                                                /*    Default value: kATSUStronglyHorizontal*/
    kATSUImposeWidthTag         = 266L,                         /*    Type:       ATSUTextMeasurement*/
                                                                /*    Default value: all glyphs use their own font defined advance widths*/
    kATSUBeforeWithStreamShiftTag = 267L,                       /*    Type:       Fixed*/
                                                                /*    Default value: 0*/
    kATSUAfterWithStreamShiftTag = 268L,                        /*    Type:       Fixed*/
                                                                /*    Default value: 0*/
    kATSUCrossStreamShiftTag    = 269L,                         /*    Type:       Fixed*/
                                                                /*    Default value: 0*/
    kATSUTrackingTag            = 270L,                         /*    Type:       Fixed*/
                                                                /*    Default value: kATSNoTracking*/
    kATSUHangingInhibitFactorTag = 271L,                        /*    Type:       Fract between 0 and 1*/
                                                                /*    Default value: 0*/
    kATSUKerningInhibitFactorTag = 272L,                        /*    Type:       Fract between 0 and 1*/
                                                                /*    Default value: 0*/
    kATSUDecompositionFactorTag = 273L,                         /*    Type:       Fixed (-1.0 -> 1.0)*/
                                                                /*    Default value: 0*/
    kATSUBaselineClassTag       = 274L,                         /*    Type:       BslnBaselineClass  (see SFNTLayoutTypes.h)*/
                                                                /*    Default value: kBSLNRomanBaseline - set to kBSLNNoBaselineOverride to use intrinsic baselines*/
    kATSUPriorityJustOverrideTag = 275L,                        /*    Type:       ATSJustPriorityWidthDeltaOverrides (see ATSLayoutTypes.h)*/
                                                                /*    Default value: all zeros*/
    kATSUNoLigatureSplitTag     = 276L,                         /*    Type:       Boolean*/
                                                                /*    Default value: false - ligatures and compound characters have divisable components.*/
    kATSUNoCaretAngleTag        = 277L,                         /*    Type:       Boolean*/
                                                                /*    Default value: false - use the character's angularity to determine its boundaries*/
    kATSUSuppressCrossKerningTag = 278L,                        /*    Type:       Boolean*/
                                                                /*    Default value: false - do not suppress automatic cross kerning (defined by font)*/
    kATSUNoOpticalAlignmentTag  = 279L,                         /*    Type:       Boolean*/
                                                                /*    Default value: false - do not suppress character's automatic optical positional alignment*/
    kATSUForceHangingTag        = 280L,                         /*    Type:       Boolean*/
                                                                /*    Default value: false - do not force the character's to hang beyond the line boundaries*/
    kATSUNoSpecialJustificationTag = 281L,                      /*    Type:       Boolean*/
                                                                /*    Default value: false - perform post-compensation justification if needed*/
    kATSUStyleTextLocatorTag    = 282L,                         /*    Type:       TextBreakLocatorRef*/
                                                                /*    Default value: NULL - region derived locator or the default Text Utilities locator*/
    kATSUMaxStyleTag            = 283L,                         /*    This is just for convenience - the upper limit of the ATSUStyle attribute tags */
    kATSUMaxATSUITagValue       = 65535L                        /*    This is the maximum Apple ATSUI reserved tag value.  Client defined tags must be larger.*/
};

/********************************/
/*  Enumerations and constants  */
/********************************/
/* Cursor movement */
enum {
    kATSUByCharacter            = 0,
    kATSUByCluster              = 1,
    kATSUByWord                 = 2
};

/* Vertical text types */
enum {
    kATSUStronglyHorizontal     = 0,
    kATSUStronglyVertical       = 1
};

/* Line direction types (used for kATSULineDirectionTag values) */
enum {
    kATSULeftToRightBaseDirection = 0,                          /*    Impose left-to-right or top-to-bottom dominant direction */
    kATSURightToLeftBaseDirection = 1                           /*    Impose right-to-left or bottom-to-top dominant direction */
};

/* Style comparison types */
enum {
    kATSUStyleUnequal           = 0,
    kATSUStyleContains          = 1,
    kATSUStyleEquals            = 2,
    kATSUStyleContainedBy       = 3
};

/* Font Fallback methods */
enum {
    kATSUDefaultFontFallbacks   = 0,
    kATSULastResortOnlyFallback = 1,
    kATSUSequentialFallbacksPreferred = 2,
    kATSUSequentialFallbacksExclusive = 3
};

/* ATSUI heap or memory allocation specifiers (of type ATSUHeapSpec) */
enum {
    kATSUUseCurrentHeap         = 0,
    kATSUUseAppHeap             = 1,
    kATSUUseSpecificHeap        = 2,
    kATSUUseCallbacks           = 3
};

/* LineFlushFactor convenience defined values */
#define kATSUStartAlignment         ((Fract) 0x00000000L)
#define kATSUEndAlignment           ((Fract) 0x40000000L)
#define kATSUCenterAlignment        ((Fract) 0x20000000L)
/* LineJustificationFactor convenience defined values */
#define kATSUNoJustification        ((Fract) 0x00000000L)
#define kATSUFullJustification      ((Fract) 0x40000000L)
/* Other constants  */
enum {
    kATSUInvalidFontID          = 0
};


enum {
    kATSUUseLineControlWidth    = 0x7FFFFFFF
};


enum {
    kATSUNoSelector             = 0x0000FFFF
};


enum {
    kATSUUseGrafPortPenLoc      = (long)0xFFFFFFFF,
    kATSUClearAll               = (long)0xFFFFFFFF
};


enum {
    kATSUFromTextBeginning      = (long)0xFFFFFFFF,
    kATSUToTextEnd              = (long)0xFFFFFFFF
};


/****************/
/*  Functions   */
/****************/

/*  Clipboard support, flattened style version 0 (is is advised to not use these routines and perform your own flattening)  */
EXTERN_API_C( OSStatus )
ATSUCopyToHandle                (ATSUStyle              iStyle,
                                 Handle                 oStyleHandle);

EXTERN_API_C( OSStatus )
ATSUPasteFromHandle             (ATSUStyle              iStyle,
                                 Handle                 iStyleHandle);

/*  Memory allocation specification functions   */
EXTERN_API_C( OSStatus )
ATSUCreateMemorySetting         (ATSUHeapSpec           iHeapSpec,
                                 ATSUMemoryCallbacks *  iMemoryCallbacks,
                                 ATSUMemorySetting *    oMemorySetting);

EXTERN_API_C( OSStatus )
ATSUSetCurrentMemorySetting     (ATSUMemorySetting      iMemorySetting);

EXTERN_API_C( ATSUMemorySetting )
ATSUGetCurrentMemorySetting     (void);

EXTERN_API_C( OSStatus )
ATSUDisposeMemorySetting        (ATSUMemorySetting      iMemorySetting);

/*  Basic style functions   */
EXTERN_API_C( OSStatus )
ATSUCreateStyle                 (ATSUStyle *            oStyle);

EXTERN_API_C( OSStatus )
ATSUCreateAndCopyStyle          (ATSUStyle              iStyle,
                                 ATSUStyle *            oStyle);

EXTERN_API_C( OSStatus )
ATSUDisposeStyle                (ATSUStyle              iStyle);

EXTERN_API_C( OSStatus )
ATSUSetStyleRefCon              (ATSUStyle              iStyle,
                                 UInt32                 iRefCon);

EXTERN_API_C( OSStatus )
ATSUGetStyleRefCon              (ATSUStyle              iStyle,
                                 UInt32 *               oRefCon);

/*  Style comparison        */
EXTERN_API_C( OSStatus )
ATSUCompareStyles               (ATSUStyle              iFirstStyle,
                                 ATSUStyle              iSecondStyle,
                                 ATSUStyleComparison *  oComparison);

/*  Attribute manipulations */
EXTERN_API_C( OSStatus )
ATSUCopyAttributes              (ATSUStyle              iSourceStyle,
                                 ATSUStyle              iDestinationStyle);

EXTERN_API_C( OSStatus )
ATSUOverwriteAttributes         (ATSUStyle              iSourceStyle,
                                 ATSUStyle              iDestinationStyle);

EXTERN_API_C( OSStatus )
ATSUUnderwriteAttributes        (ATSUStyle              iSourceStyle,
                                 ATSUStyle              iDestinationStyle);

/*  Empty styles    */
EXTERN_API_C( OSStatus )
ATSUClearStyle                  (ATSUStyle              iStyle);

EXTERN_API_C( OSStatus )
ATSUStyleIsEmpty                (ATSUStyle              iStyle,
                                 Boolean *              oIsClear);

/*  Get and set attributes */
EXTERN_API_C( OSStatus )
ATSUCalculateBaselineDeltas     (ATSUStyle              iStyle,
                                 BslnBaselineClass      iBaselineClass,
                                 BslnBaselineRecord     oBaselineDeltas);

EXTERN_API_C( OSStatus )
ATSUSetAttributes               (ATSUStyle              iStyle,
                                 ItemCount              iAttributeCount,
                                 ATSUAttributeTag       iTag[],
                                 ByteCount              iValueSize[],
                                 ATSUAttributeValuePtr  iValue[]);

EXTERN_API_C( OSStatus )
ATSUGetAttribute                (ATSUStyle              iStyle,
                                 ATSUAttributeTag       iTag,
                                 ByteCount              iExpectedValueSize,
                                 ATSUAttributeValuePtr  oValue,
                                 ByteCount *            oActualValueSize);

EXTERN_API_C( OSStatus )
ATSUGetAllAttributes            (ATSUStyle              iStyle,
                                 ATSUAttributeInfo      oAttributeInfoArray[],
                                 ItemCount              iTagValuePairArraySize,
                                 ItemCount *            oTagValuePairCount);

EXTERN_API_C( OSStatus )
ATSUClearAttributes             (ATSUStyle              iStyle,
                                 ItemCount              iTagCount,
                                 ATSUAttributeTag       iTag[]);

/*  Font features   */
EXTERN_API_C( OSStatus )
ATSUSetFontFeatures             (ATSUStyle              iStyle,
                                 ItemCount              iFeatureCount,
                                 ATSUFontFeatureType    iType[],
                                 ATSUFontFeatureSelector  iSelector[]);

EXTERN_API_C( OSStatus )
ATSUGetFontFeature              (ATSUStyle              iStyle,
                                 ItemCount              iFeatureIndex,
                                 ATSUFontFeatureType *  oFeatureType,
                                 ATSUFontFeatureSelector * oFeatureSelector);

EXTERN_API_C( OSStatus )
ATSUGetAllFontFeatures          (ATSUStyle              iStyle,
                                 ItemCount              iMaximumFeatureCount,
                                 ATSUFontFeatureType    oFeatureType[],
                                 ATSUFontFeatureSelector  oFeatureSelector[],
                                 ItemCount *            oActualFeatureCount);

EXTERN_API_C( OSStatus )
ATSUClearFontFeatures           (ATSUStyle              iStyle,
                                 ItemCount              iFeatureCount,
                                 ATSUFontFeatureType    iType[],
                                 ATSUFontFeatureSelector  iSelector[]);

/*  Font variations */
EXTERN_API_C( OSStatus )
ATSUSetVariations               (ATSUStyle              iStyle,
                                 ItemCount              iVariationCount,
                                 ATSUFontVariationAxis  iAxes[],
                                 ATSUFontVariationValue  iValue[]);

EXTERN_API_C( OSStatus )
ATSUGetFontVariationValue       (ATSUStyle              iStyle,
                                 ATSUFontVariationAxis  iFontVariationAxis,
                                 ATSUFontVariationValue * oFontVariationValue);

EXTERN_API_C( OSStatus )
ATSUGetAllFontVariations        (ATSUStyle              iStyle,
                                 ItemCount              iVariationCount,
                                 ATSUFontVariationAxis  oVariationAxes[],
                                 ATSUFontVariationValue  oFontVariationValues[],
                                 ItemCount *            oActualVariationCount);

EXTERN_API_C( OSStatus )
ATSUClearFontVariations         (ATSUStyle              iStyle,
                                 ItemCount              iAxisCount,
                                 ATSUFontVariationAxis  iAxis[]);

/*  Basic text-layout functions */
EXTERN_API_C( OSStatus )
ATSUCreateTextLayout            (ATSUTextLayout *       oTextLayout);

EXTERN_API_C( OSStatus )
ATSUCreateAndCopyTextLayout     (ATSUTextLayout         iTextLayout,
                                 ATSUTextLayout *       oTextLayout);

EXTERN_API_C( OSStatus )
ATSUCreateTextLayoutWithTextPtr (ConstUniCharArrayPtr   iText,
                                 UniCharArrayOffset     iTextOffset,
                                 UniCharCount           iTextLength,
                                 UniCharCount           iTextTotalLength,
                                 ItemCount              iNumberOfRuns,
                                 UniCharCount           iRunLengths[],
                                 ATSUStyle              iStyles[],
                                 ATSUTextLayout *       oTextLayout);

EXTERN_API_C( OSStatus )
ATSUCreateTextLayoutWithTextHandle (UniCharArrayHandle  iText,
                                 UniCharArrayOffset     iTextOffset,
                                 UniCharCount           iTextLength,
                                 UniCharCount           iTextTotalLength,
                                 ItemCount              iNumberOfRuns,
                                 UniCharCount           iRunLengths[],
                                 ATSUStyle              iStyles[],
                                 ATSUTextLayout *       oTextLayout);

EXTERN_API_C( OSStatus )
ATSUClearLayoutCache            (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineStart);

EXTERN_API_C( OSStatus )
ATSUDisposeTextLayout           (ATSUTextLayout         iTextLayout);

EXTERN_API_C( OSStatus )
ATSUSetTextLayoutRefCon         (ATSUTextLayout         iTextLayout,
                                 UInt32                 iRefCon);

EXTERN_API_C( OSStatus )
ATSUGetTextLayoutRefCon         (ATSUTextLayout         iTextLayout,
                                 UInt32 *               oRefCon);

/*  Glyph bounds retrieval  */
EXTERN_API_C( OSStatus )
ATSUGetGlyphBounds              (ATSUTextLayout         iTextLayout,
                                 ATSUTextMeasurement    iTextBasePointX,
                                 ATSUTextMeasurement    iTextBasePointY,
                                 UniCharArrayOffset     iBoundsCharStart,
                                 UniCharCount           iBoundsCharLength,
                                 UInt16                 iTypeOfBounds,
                                 ItemCount              iMaxNumberOfBounds,
                                 ATSTrapezoid           oGlyphBounds[],
                                 ItemCount *            oActualNumberOfBounds);

/*  Idle processing */
EXTERN_API_C( OSStatus )
ATSUIdle                        (ATSUTextLayout         iTextLayout);

/*  Text location   */
EXTERN_API_C( OSStatus )
ATSUSetTextPointerLocation      (ATSUTextLayout         iTextLayout,
                                 ConstUniCharArrayPtr   iText,
                                 UniCharArrayOffset     iTextOffset,
                                 UniCharCount           iTextLength,
                                 UniCharCount           iTextTotalLength);

EXTERN_API_C( OSStatus )
ATSUSetTextHandleLocation       (ATSUTextLayout         iTextLayout,
                                 UniCharArrayHandle     iText,
                                 UniCharArrayOffset     iTextOffset,
                                 UniCharCount           iTextLength,
                                 UniCharCount           iTextTotalLength);

EXTERN_API_C( OSStatus )
ATSUGetTextLocation             (ATSUTextLayout         iTextLayout,
                                 void **                oText,
                                 Boolean *              oTextIsStoredInHandle,
                                 UniCharArrayOffset *   oOffset,
                                 UniCharCount *         oTextLength,
                                 UniCharCount *         oTextTotalLength);

/*  Text manipulation   */
EXTERN_API_C( OSStatus )
ATSUTextDeleted                 (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iDeletedRangeStart,
                                 UniCharCount           iDeletedRangeLength);

EXTERN_API_C( OSStatus )
ATSUTextInserted                (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iInsertionLocation,
                                 UniCharCount           iInsertionLength);

EXTERN_API_C( OSStatus )
ATSUTextMoved                   (ATSUTextLayout         iTextLayout,
                                 ConstUniCharArrayPtr   iNewLocation);

/*  Layout controls */
EXTERN_API_C( OSStatus )
ATSUCopyLayoutControls          (ATSUTextLayout         iSourceTextLayout,
                                 ATSUTextLayout         iDestTextLayout);

EXTERN_API_C( OSStatus )
ATSUSetLayoutControls           (ATSUTextLayout         iTextLayout,
                                 ItemCount              iAttributeCount,
                                 ATSUAttributeTag       iTag[],
                                 ByteCount              iValueSize[],
                                 ATSUAttributeValuePtr  iValue[]);

EXTERN_API_C( OSStatus )
ATSUGetLayoutControl            (ATSUTextLayout         iTextLayout,
                                 ATSUAttributeTag       iTag,
                                 ByteCount              iExpectedValueSize,
                                 ATSUAttributeValuePtr  oValue,
                                 ByteCount *            oActualValueSize);

EXTERN_API_C( OSStatus )
ATSUGetAllLayoutControls        (ATSUTextLayout         iTextLayout,
                                 ATSUAttributeInfo      oAttributeInfoArray[],
                                 ItemCount              iTagValuePairArraySize,
                                 ItemCount *            oTagValuePairCount);

EXTERN_API_C( OSStatus )
ATSUClearLayoutControls         (ATSUTextLayout         iTextLayout,
                                 ItemCount              iTagCount,
                                 ATSUAttributeTag       iTag[]);

/*  Single line layout controls */
EXTERN_API_C( OSStatus )
ATSUCopyLineControls            (ATSUTextLayout         iSourceTextLayout,
                                 UniCharArrayOffset     iSourceLineStart,
                                 ATSUTextLayout         iDestTextLayout,
                                 UniCharArrayOffset     iDestLineStart);

EXTERN_API_C( OSStatus )
ATSUSetLineControls             (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineStart,
                                 ItemCount              iAttributeCount,
                                 ATSUAttributeTag       iTag[],
                                 ByteCount              iValueSize[],
                                 ATSUAttributeValuePtr  iValue[]);

EXTERN_API_C( OSStatus )
ATSUGetLineControl              (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineStart,
                                 ATSUAttributeTag       iTag,
                                 ByteCount              iExpectedValueSize,
                                 ATSUAttributeValuePtr  oValue,
                                 ByteCount *            oActualValueSize);

EXTERN_API_C( OSStatus )
ATSUGetAllLineControls          (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineStart,
                                 ATSUAttributeInfo      oAttributeInfoArray[],
                                 ItemCount              iTagValuePairArraySize,
                                 ItemCount *            oTagValuePairCount);

EXTERN_API_C( OSStatus )
ATSUClearLineControls           (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineStart,
                                 ItemCount              iTagCount,
                                 ATSUAttributeTag       iTag[]);

/*  Style run processing    */
EXTERN_API_C( OSStatus )
ATSUSetRunStyle                 (ATSUTextLayout         iTextLayout,
                                 ATSUStyle              iStyle,
                                 UniCharArrayOffset     iRunStart,
                                 UniCharCount           iRunLength);

EXTERN_API_C( OSStatus )
ATSUGetRunStyle                 (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iOffset,
                                 ATSUStyle *            oStyle,
                                 UniCharArrayOffset *   oRunStart,
                                 UniCharCount *         oRunLength);

EXTERN_API_C( OSStatus )
ATSUGetContinuousAttributes     (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iOffset,
                                 UniCharCount           iLength,
                                 ATSUStyle              oStyle);

/*  Drawing and measuring   */
EXTERN_API_C( OSStatus )
ATSUDrawText                    (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineOffset,
                                 UniCharCount           iLineLength,
                                 ATSUTextMeasurement    iLocationX,
                                 ATSUTextMeasurement    iLocationY);

EXTERN_API_C( OSStatus )
ATSUMeasureText                 (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineStart,
                                 UniCharCount           iLineLength,
                                 ATSUTextMeasurement *  oTextBefore,
                                 ATSUTextMeasurement *  oTextAfter,
                                 ATSUTextMeasurement *  oAscent,
                                 ATSUTextMeasurement *  oDescent);

EXTERN_API_C( OSStatus )
ATSUMeasureTextImage            (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineOffset,
                                 UniCharCount           iLineLength,
                                 ATSUTextMeasurement    iLocationX,
                                 ATSUTextMeasurement    iLocationY,
                                 Rect *                 oTextImageRect);

/*  Highlighting    */
EXTERN_API_C( OSStatus )
ATSUHighlightText               (ATSUTextLayout         iTextLayout,
                                 ATSUTextMeasurement    iTextBasePointX,
                                 ATSUTextMeasurement    iTextBasePointY,
                                 UniCharArrayOffset     iHighlightStart,
                                 UniCharCount           iHighlightLength);

EXTERN_API_C( OSStatus )
ATSUUnhighlightText             (ATSUTextLayout         iTextLayout,
                                 ATSUTextMeasurement    iTextBasePointX,
                                 ATSUTextMeasurement    iTextBasePointY,
                                 UniCharArrayOffset     iHighlightStart,
                                 UniCharCount           iHighlightLength);

EXTERN_API_C( OSStatus )
ATSUGetTextHighlight            (ATSUTextLayout         iTextLayout,
                                 ATSUTextMeasurement    iTextBasePointX,
                                 ATSUTextMeasurement    iTextBasePointY,
                                 UniCharArrayOffset     iHighlightStart,
                                 UniCharCount           iHighlightLength,
                                 RgnHandle              oHighlightRegion);

/*  Hit-testing */
EXTERN_API_C( OSStatus )
ATSUPositionToOffset            (ATSUTextLayout         iTextLayout,
                                 ATSUTextMeasurement    iLocationX,
                                 ATSUTextMeasurement    iLocationY,
                                 UniCharArrayOffset *   ioPrimaryOffset,
                                 Boolean *              oIsLeading,
                                 UniCharArrayOffset *   oSecondaryOffset);

EXTERN_API_C( OSStatus )
ATSUOffsetToPosition            (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iOffset,
                                 Boolean                iIsLeading,
                                 ATSUCaret *            oMainCaret,
                                 ATSUCaret *            oSecondCaret,
                                 Boolean *              oCaretIsSplit);

/*  Cursor movement */
EXTERN_API_C( OSStatus )
ATSUNextCursorPosition          (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iOldOffset,
                                 ATSUCursorMovementType  iMovementType,
                                 UniCharArrayOffset *   oNewOffset);

EXTERN_API_C( OSStatus )
ATSUPreviousCursorPosition      (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iOldOffset,
                                 ATSUCursorMovementType  iMovementType,
                                 UniCharArrayOffset *   oNewOffset);

EXTERN_API_C( OSStatus )
ATSURightwardCursorPosition     (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iOldOffset,
                                 ATSUCursorMovementType  iMovementType,
                                 UniCharArrayOffset *   oNewOffset);

EXTERN_API_C( OSStatus )
ATSULeftwardCursorPosition      (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iOldOffset,
                                 ATSUCursorMovementType  iMovementType,
                                 UniCharArrayOffset *   oNewOffset);

/*  Line breaking   */
EXTERN_API_C( OSStatus )
ATSUBreakLine                   (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineStart,
                                 ATSUTextMeasurement    iLineWidth,
                                 Boolean                iUseAsSoftLineBreak,
                                 UniCharArrayOffset *   oLineBreak);

EXTERN_API_C( OSStatus )
ATSUSetSoftLineBreak            (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iLineBreak);

EXTERN_API_C( OSStatus )
ATSUGetSoftLineBreaks           (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iRangeStart,
                                 UniCharCount           iRangeLength,
                                 ItemCount              iMaximumBreaks,
                                 UniCharArrayOffset     oBreaks[],
                                 ItemCount *            oBreakCount);

EXTERN_API_C( OSStatus )
ATSUClearSoftLineBreaks         (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iRangeStart,
                                 UniCharCount           iRangeLength);

/*  Font matching   */
EXTERN_API_C( OSStatus )
ATSUSetFontFallbacks            (ItemCount              iFontFallbacksCount,
                                 ATSUFontID             iFontIDs[],
                                 ATSUFontFallbackMethod  iFontFallbackMethod);

EXTERN_API_C( OSStatus )
ATSUGetFontFallbacks            (ItemCount              iMaxFontFallbacksCount,
                                 ATSUFontID             oFontIDs[],
                                 ATSUFontFallbackMethod * oFontFallbackMethod,
                                 ItemCount *            oActualFallbacksCount);

EXTERN_API_C( OSStatus )
ATSUMatchFontsToText            (ATSUTextLayout         iTextLayout,
                                 UniCharArrayOffset     iTextStart,
                                 UniCharCount           iTextLength,
                                 ATSUFontID *           oFontID,
                                 UniCharArrayOffset *   oChangedOffset,
                                 UniCharCount *         oChangedLength);

EXTERN_API_C( OSStatus )
ATSUSetTransientFontMatching    (ATSUTextLayout         iTextLayout,
                                 Boolean                iTransientFontMatching);

EXTERN_API_C( OSStatus )
ATSUGetTransientFontMatching    (ATSUTextLayout         iTextLayout,
                                 Boolean *              oTransientFontMatching);

/*  Font ID's   */
EXTERN_API_C( OSStatus )
ATSUFontCount                   (ItemCount *            oFontCount);

EXTERN_API_C( OSStatus )
ATSUGetFontIDs                  (ATSUFontID             oFontIDs[],
                                 ItemCount              iArraySize,
                                 ItemCount *            oFontCount);

EXTERN_API_C( OSStatus )
ATSUFONDtoFontID                (short                  iFONDNumber,
                                 Style                  iFONDStyle,
                                 ATSUFontID *           oFontID);

EXTERN_API_C( OSStatus )
ATSUFontIDtoFOND                (ATSUFontID             iFontID,
                                 short *                oFONDNumber,
                                 Style *                oFONDStyle);

/*  Font names  */
EXTERN_API_C( OSStatus )
ATSUCountFontNames              (ATSUFontID             iFontID,
                                 ItemCount *            oFontNameCount);

EXTERN_API_C( OSStatus )
ATSUGetIndFontName              (ATSUFontID             iFontID,
                                 ItemCount              iFontNameIndex,
                                 ByteCount              iMaximumNameLength,
                                 Ptr                    oName,
                                 ByteCount *            oActualNameLength,
                                 FontNameCode *         oFontNameCode,
                                 FontPlatformCode *     oFontNamePlatform,
                                 FontScriptCode *       oFontNameScript,
                                 FontLanguageCode *     oFontNameLanguage);

EXTERN_API_C( OSStatus )
ATSUFindFontName                (ATSUFontID             iFontID,
                                 FontNameCode           iFontNameCode,
                                 FontPlatformCode       iFontNamePlatform,
                                 FontScriptCode         iFontNameScript,
                                 FontLanguageCode       iFontNameLanguage,
                                 ByteCount              iMaximumNameLength,
                                 Ptr                    oName,
                                 ByteCount *            oActualNameLength,
                                 ItemCount *            oFontNameIndex);

EXTERN_API_C( OSStatus )
ATSUFindFontFromName            (Ptr                    iName,
                                 ByteCount              iNameLength,
                                 FontNameCode           iFontNameCode,
                                 FontPlatformCode       iFontNamePlatform,
                                 FontScriptCode         iFontNameScript,
                                 FontLanguageCode       iFontNameLanguage,
                                 ATSUFontID *           oFontID);

/*  Font features   */
EXTERN_API_C( OSStatus )
ATSUCountFontFeatureTypes       (ATSUFontID             iFontID,
                                 ItemCount *            oTypeCount);

EXTERN_API_C( OSStatus )
ATSUCountFontFeatureSelectors   (ATSUFontID             iFontID,
                                 ATSUFontFeatureType    iType,
                                 ItemCount *            oSelectorCount);

EXTERN_API_C( OSStatus )
ATSUGetFontFeatureTypes         (ATSUFontID             iFontID,
                                 ItemCount              iMaximumTypes,
                                 ATSUFontFeatureType    oTypes[],
                                 ItemCount *            oActualTypeCount);

EXTERN_API_C( OSStatus )
ATSUGetFontFeatureSelectors     (ATSUFontID             iFontID,
                                 ATSUFontFeatureType    iType,
                                 ItemCount              iMaximumSelectors,
                                 ATSUFontFeatureSelector  oSelectors[],
                                 Boolean                oSelectorIsOnByDefault[],
                                 ItemCount *            oActualSelectorCount,
                                 Boolean *              oIsMutuallyExclusive);

EXTERN_API_C( OSStatus )
ATSUGetFontFeatureNameCode      (ATSUFontID             iFontID,
                                 ATSUFontFeatureType    iType,
                                 ATSUFontFeatureSelector  iSelector,
                                 FontNameCode *         oNameCode);

/*  Font tracking value & names */
EXTERN_API_C( OSStatus )
ATSUCountFontTracking           (ATSUFontID             iFontID,
                                 ATSUVerticalCharacterType  iCharacterOrientation,
                                 ItemCount *            oTrackingCount);

EXTERN_API_C( OSStatus )
ATSUGetIndFontTracking          (ATSUFontID             iFontID,
                                 ATSUVerticalCharacterType  iCharacterOrientation,
                                 ItemCount              iTrackIndex,
                                 Fixed *                oFontTrackingValue,
                                 FontNameCode *         oNameCode);

/*  Font variations */
EXTERN_API_C( OSStatus )
ATSUCountFontVariations         (ATSUFontID             iFontID,
                                 ItemCount *            oVariationCount);

EXTERN_API_C( OSStatus )
ATSUGetIndFontVariation         (ATSUFontID             iFontID,
                                 ItemCount              iVariationIndex,
                                 ATSUFontVariationAxis * oATSUFontVariationAxis,
                                 ATSUFontVariationValue * oMinimumValue,
                                 ATSUFontVariationValue * oMaximumValue,
                                 ATSUFontVariationValue * oDefaultValue);

EXTERN_API_C( OSStatus )
ATSUGetFontVariationNameCode    (ATSUFontID             iFontID,
                                 ATSUFontVariationAxis  iAxis,
                                 FontNameCode *         oNameCode);

/*  Font Instances  */
EXTERN_API_C( OSStatus )
ATSUCountFontInstances          (ATSUFontID             iFontID,
                                 ItemCount *            oInstances);

EXTERN_API_C( OSStatus )
ATSUGetFontInstance             (ATSUFontID             iFontID,
                                 ItemCount              iFontInstanceIndex,
                                 ItemCount              iMaximumVariations,
                                 ATSUFontVariationAxis  oAxes[],
                                 ATSUFontVariationValue  oValues[],
                                 ItemCount *            oActualVariationCount);

EXTERN_API_C( OSStatus )
ATSUGetFontInstanceNameCode     (ATSUFontID             iFontID,
                                 ItemCount              iInstanceIndex,
                                 FontNameCode *         oNameCode);



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

#endif /* __ATSUNICODE__ */

