/*
     File:       QD3DStyle.h
 
     Contains:   Q3Style types and routines
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DSTYLE__
#define __QD3DSTYLE__

#ifndef __QD3D__
#include <QD3D.h>
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

#if PRAGMA_ENUM_ALWAYSINT
    #if defined(__fourbyteints__) && !__fourbyteints__ 
        #define __QD3DSTYLE__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DSTYLE__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                 Style Base Class Routines                                **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3ObjectType )
Q3Style_GetType                 (TQ3StyleObject         style);

EXTERN_API_C( TQ3Status )
Q3Style_Submit                  (TQ3StyleObject         style,
                                 TQ3ViewObject          view);


/******************************************************************************
 **                                                                          **
 **                              Subdivision                                 **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3SubdivisionMethod {
    kQ3SubdivisionMethodConstant = 0,
    kQ3SubdivisionMethodWorldSpace = 1,
    kQ3SubdivisionMethodScreenSpace = 2
};
typedef enum TQ3SubdivisionMethod TQ3SubdivisionMethod;



struct TQ3SubdivisionStyleData {
    TQ3SubdivisionMethod            method;
    float                           c1;
    float                           c2;
};
typedef struct TQ3SubdivisionStyleData  TQ3SubdivisionStyleData;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StyleObject )
Q3SubdivisionStyle_New          (const TQ3SubdivisionStyleData * data);

EXTERN_API_C( TQ3Status )
Q3SubdivisionStyle_Submit       (const TQ3SubdivisionStyleData * data,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3SubdivisionStyle_SetData      (TQ3StyleObject         subdiv,
                                 const TQ3SubdivisionStyleData * data);

EXTERN_API_C( TQ3Status )
Q3SubdivisionStyle_GetData      (TQ3StyleObject         subdiv,
                                 TQ3SubdivisionStyleData * data);


/******************************************************************************
 **                                                                          **
 **                             Pick ID                                      **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3StyleObject )
Q3PickIDStyle_New               (unsigned long          id);

EXTERN_API_C( TQ3Status )
Q3PickIDStyle_Submit            (unsigned long          id,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3PickIDStyle_Get               (TQ3StyleObject         pickIDObject,
                                 unsigned long *        id);

EXTERN_API_C( TQ3Status )
Q3PickIDStyle_Set               (TQ3StyleObject         pickIDObject,
                                 unsigned long          id);


/******************************************************************************
 **                                                                          **
 **                             Pick Parts                                   **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3PickPartsMasks {
    kQ3PickPartsObject          = 0,
    kQ3PickPartsMaskFace        = 1 << 0,
    kQ3PickPartsMaskEdge        = 1 << 1,
    kQ3PickPartsMaskVertex      = 1 << 2
};
typedef enum TQ3PickPartsMasks TQ3PickPartsMasks;


typedef unsigned long                   TQ3PickParts;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StyleObject )
Q3PickPartsStyle_New            (TQ3PickParts           parts);

EXTERN_API_C( TQ3Status )
Q3PickPartsStyle_Submit         (TQ3PickParts           parts,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3PickPartsStyle_Get            (TQ3StyleObject         pickPartsObject,
                                 TQ3PickParts *         parts);

EXTERN_API_C( TQ3Status )
Q3PickPartsStyle_Set            (TQ3StyleObject         pickPartsObject,
                                 TQ3PickParts           parts);


/******************************************************************************
 **                                                                          **
 **                     Receive Shadows                                      **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3StyleObject )
Q3ReceiveShadowsStyle_New       (TQ3Boolean             receives);

EXTERN_API_C( TQ3Status )
Q3ReceiveShadowsStyle_Submit    (TQ3Boolean             receives,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3ReceiveShadowsStyle_Get       (TQ3StyleObject         styleObject,
                                 TQ3Boolean *           receives);

EXTERN_API_C( TQ3Status )
Q3ReceiveShadowsStyle_Set       (TQ3StyleObject         styleObject,
                                 TQ3Boolean             receives);


/******************************************************************************
 **                                                                          **
 **                         Fill Styles                                      **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3FillStyle {
    kQ3FillStyleFilled          = 0,
    kQ3FillStyleEdges           = 1,
    kQ3FillStylePoints          = 2
};
typedef enum TQ3FillStyle TQ3FillStyle;


#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StyleObject )
Q3FillStyle_New                 (TQ3FillStyle           fillStyle);

EXTERN_API_C( TQ3Status )
Q3FillStyle_Submit              (TQ3FillStyle           fillStyle,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3FillStyle_Get                 (TQ3StyleObject         styleObject,
                                 TQ3FillStyle *         fillStyle);

EXTERN_API_C( TQ3Status )
Q3FillStyle_Set                 (TQ3StyleObject         styleObject,
                                 TQ3FillStyle           fillStyle);


/******************************************************************************
 **                                                                          **
 **                         Backfacing Styles                                **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3BackfacingStyle {
    kQ3BackfacingStyleBoth      = 0,
    kQ3BackfacingStyleRemove    = 1,
    kQ3BackfacingStyleFlip      = 2
};
typedef enum TQ3BackfacingStyle TQ3BackfacingStyle;

#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StyleObject )
Q3BackfacingStyle_New           (TQ3BackfacingStyle     backfacingStyle);

EXTERN_API_C( TQ3Status )
Q3BackfacingStyle_Submit        (TQ3BackfacingStyle     backfacingStyle,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3BackfacingStyle_Get           (TQ3StyleObject         backfacingObject,
                                 TQ3BackfacingStyle *   backfacingStyle);

EXTERN_API_C( TQ3Status )
Q3BackfacingStyle_Set           (TQ3StyleObject         backfacingObject,
                                 TQ3BackfacingStyle     backfacingStyle);


/******************************************************************************
 **                                                                          **
 **                         Interpolation Types                              **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3InterpolationStyle {
    kQ3InterpolationStyleNone   = 0,
    kQ3InterpolationStyleVertex = 1,
    kQ3InterpolationStylePixel  = 2
};
typedef enum TQ3InterpolationStyle TQ3InterpolationStyle;

#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StyleObject )
Q3InterpolationStyle_New        (TQ3InterpolationStyle  interpolationStyle);

EXTERN_API_C( TQ3Status )
Q3InterpolationStyle_Submit     (TQ3InterpolationStyle  interpolationStyle,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3InterpolationStyle_Get        (TQ3StyleObject         interpolationObject,
                                 TQ3InterpolationStyle * interpolationStyle);

EXTERN_API_C( TQ3Status )
Q3InterpolationStyle_Set        (TQ3StyleObject         interpolationObject,
                                 TQ3InterpolationStyle  interpolationStyle);


/******************************************************************************
 **                                                                          **
 **                             Highlight Style                              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3StyleObject )
Q3HighlightStyle_New            (TQ3AttributeSet        highlightAttribute);

EXTERN_API_C( TQ3Status )
Q3HighlightStyle_Submit         (TQ3AttributeSet        highlightAttribute,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3HighlightStyle_Get            (TQ3StyleObject         highlight,
                                 TQ3AttributeSet *      highlightAttribute);

EXTERN_API_C( TQ3Status )
Q3HighlightStyle_Set            (TQ3StyleObject         highlight,
                                 TQ3AttributeSet        highlightAttribute);


/******************************************************************************
 **                                                                          **
 **                         FrontFacing Direction Styles                     **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3OrientationStyle {
    kQ3OrientationStyleCounterClockwise = 0,
    kQ3OrientationStyleClockwise = 1
};
typedef enum TQ3OrientationStyle TQ3OrientationStyle;

#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StyleObject )
Q3OrientationStyle_New          (TQ3OrientationStyle    frontFacingDirection);

EXTERN_API_C( TQ3Status )
Q3OrientationStyle_Submit       (TQ3OrientationStyle    frontFacingDirection,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3OrientationStyle_Get          (TQ3StyleObject         frontFacingDirectionObject,
                                 TQ3OrientationStyle *  frontFacingDirection);

EXTERN_API_C( TQ3Status )
Q3OrientationStyle_Set          (TQ3StyleObject         frontFacingDirectionObject,
                                 TQ3OrientationStyle    frontFacingDirection);


/******************************************************************************
 **                                                                          **
 **                             AntiAlias Style                              **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3AntiAliasModeMasks {
    kQ3AntiAliasModeMaskEdges   = 1 << 0,
    kQ3AntiAliasModeMaskFilled  = 1 << 1
};
typedef enum TQ3AntiAliasModeMasks TQ3AntiAliasModeMasks;

typedef unsigned long                   TQ3AntiAliasMode;

struct TQ3AntiAliasStyleData {
    TQ3Switch                       state;
    TQ3AntiAliasMode                mode;
    float                           quality;
};
typedef struct TQ3AntiAliasStyleData    TQ3AntiAliasStyleData;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StyleObject )
Q3AntiAliasStyle_New            (const TQ3AntiAliasStyleData * data);

EXTERN_API_C( TQ3Status )
Q3AntiAliasStyle_Submit         (const TQ3AntiAliasStyleData * data,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3AntiAliasStyle_GetData        (TQ3StyleObject         styleObject,
                                 TQ3AntiAliasStyleData * data);

EXTERN_API_C( TQ3Status )
Q3AntiAliasStyle_SetData        (TQ3StyleObject         styleObject,
                                 const TQ3AntiAliasStyleData * data);


/******************************************************************************
 **                                                                          **
 **                                 Fog Style                                **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3FogMode {
    kQ3FogModeLinear            = 0,
    kQ3FogModeExponential       = 1,
    kQ3FogModeExponentialSquared = 2,
    kQ3FogModeAlpha             = 3
};
typedef enum TQ3FogMode TQ3FogMode;


struct TQ3FogStyleData {
    TQ3Switch                       state;
    TQ3FogMode                      mode;
    float                           fogStart;
    float                           fogEnd;
    float                           density;
    TQ3ColorARGB                    color;
};
typedef struct TQ3FogStyleData          TQ3FogStyleData;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StyleObject )
Q3FogStyle_New                  (const TQ3FogStyleData * data);

EXTERN_API_C( TQ3Status )
Q3FogStyle_Submit               (const TQ3FogStyleData * data,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3FogStyle_GetData              (TQ3StyleObject         styleObject,
                                 TQ3FogStyleData *      data);

EXTERN_API_C( TQ3Status )
Q3FogStyle_SetData              (TQ3StyleObject         styleObject,
                                 const TQ3FogStyleData * data);




#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DSTYLE__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DSTYLE__RESTORE_PACKED_ENUMS)
    #pragma options(pack_enums)
#endif

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

#endif /* __QD3DSTYLE__ */

