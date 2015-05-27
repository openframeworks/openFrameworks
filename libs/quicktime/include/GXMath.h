/*
     File:       GXMath.h
 
     Contains:   QuickDraw GX math routine interfaces.
 
     Version:    Technology: Quickdraw GX 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __GXMATH__
#define __GXMATH__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FIXMATH__
#include <FixMath.h>
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

#if defined(__MWERKS__) && TARGET_CPU_68K
    #pragma push
    #pragma pointers_in_D0
#endif
 


struct gxPoint {
    Fixed                           x;
    Fixed                           y;
};
typedef struct gxPoint                  gxPoint;

typedef unsigned short                  gxColorValue;

struct gxPolar {
    Fixed                           radius;
    Fixed                           angle;
};
typedef struct gxPolar                  gxPolar;

struct gxMapping {
    Fixed                           map[3][3];
};
typedef struct gxMapping                gxMapping;
#define gxColorValue1           ((gxColorValue) 0x0000FFFF)
#define  gxPositiveInfinity     ((long) 0x7FFFFFFFL)
#define gxNegativeInfinity      ((long) 0x80000000L)

#if CALL_NOT_IN_CARBON
EXTERN_API_C( gxMapping *)
CopyToMapping                   (gxMapping *            target,
                                 const gxMapping *      source)                             THREEWORDINLINE(0x303C, 0x0031, 0xA832);

EXTERN_API_C( gxMapping *)
InvertMapping                   (gxMapping *            target,
                                 const gxMapping *      source)                             THREEWORDINLINE(0x303C, 0x0032, 0xA832);

EXTERN_API_C( gxMapping *)
MapMapping                      (gxMapping *            target,
                                 const gxMapping *      source)                             THREEWORDINLINE(0x303C, 0x0033, 0xA832);

EXTERN_API_C( gxMapping *)
MoveMapping                     (gxMapping *            target,
                                 Fixed                  hOffset,
                                 Fixed                  vOffset)                            THREEWORDINLINE(0x303C, 0x0034, 0xA832);

EXTERN_API_C( gxMapping *)
MoveMappingTo                   (gxMapping *            target,
                                 Fixed                  hPosition,
                                 Fixed                  vPosition)                          THREEWORDINLINE(0x303C, 0x0035, 0xA832);

EXTERN_API_C( gxMapping *)
NormalizeMapping                (gxMapping *            target)                             THREEWORDINLINE(0x303C, 0x0036, 0xA832);

EXTERN_API_C( gxMapping *)
RotateMapping                   (gxMapping *            target,
                                 Fixed                  angle,
                                 Fixed                  xCenter,
                                 Fixed                  yCenter)                            THREEWORDINLINE(0x303C, 0x0037, 0xA832);

EXTERN_API_C( gxMapping *)
ScaleMapping                    (gxMapping *            target,
                                 Fixed                  hFactor,
                                 Fixed                  vFactor,
                                 Fixed                  xCenter,
                                 Fixed                  yCenter)                            THREEWORDINLINE(0x303C, 0x0038, 0xA832);

EXTERN_API_C( gxMapping *)
ResetMapping                    (gxMapping *            target)                             THREEWORDINLINE(0x303C, 0x0039, 0xA832);

EXTERN_API_C( gxMapping *)
SkewMapping                     (gxMapping *            target,
                                 Fixed                  skewX,
                                 Fixed                  skewY,
                                 Fixed                  xCenter,
                                 Fixed                  yCenter)                            THREEWORDINLINE(0x303C, 0x003A, 0xA832);

EXTERN_API_C( void )
MapPoints                       (const gxMapping *      source,
                                 long                   count,
                                 gxPoint                theVector[])                          THREEWORDINLINE(0x303C, 0x003B, 0xA832);

EXTERN_API_C( short )
FirstBit                        (unsigned long          x)                                  THREEWORDINLINE(0x303C, 0x003C, 0xA832);

EXTERN_API_C( short )
WideScale                       (const wide *           source)                             THREEWORDINLINE(0x303C, 0x003D, 0xA832);

EXTERN_API_C( short )
LinearRoot                      (Fixed                  first,
                                 Fixed                  last,
                                 Fract                  t[])                                  THREEWORDINLINE(0x303C, 0x003E, 0xA832);

EXTERN_API_C( short )
QuadraticRoot                   (Fixed                  first,
                                 Fixed                  control,
                                 Fixed                  last,
                                 Fract                  t[])                                  THREEWORDINLINE(0x303C, 0x003F, 0xA832);

EXTERN_API_C( gxPoint *)
PolarToPoint                    (const gxPolar *        ra,
                                 gxPoint *              xy)                                 THREEWORDINLINE(0x303C, 0x0040, 0xA832);

EXTERN_API_C( gxPolar *)
PointToPolar                    (const gxPoint *        xy,
                                 gxPolar *              ra)                                 THREEWORDINLINE(0x303C, 0x0041, 0xA832);

EXTERN_API_C( Fract )
FractCubeRoot                   (Fract                  source)                             THREEWORDINLINE(0x303C, 0x0042, 0xA832);

EXTERN_API_C( Fract )
FractDivide                     (Fract                  dividend,
                                 Fract                  divisor)                            THREEWORDINLINE(0x303C, 0x0043, 0xA832);

EXTERN_API_C( Fract )
FractMultiply                   (Fract                  multiplicand,
                                 Fract                  multiplier)                         THREEWORDINLINE(0x303C, 0x0044, 0xA832);

EXTERN_API_C( Fract )
FractSineCosine                 (Fixed                  degrees,
                                 Fract *                cosine)                             THREEWORDINLINE(0x303C, 0x0045, 0xA832);

EXTERN_API_C( Fract )
FractSquareRoot                 (Fract                  source)                             THREEWORDINLINE(0x303C, 0x0046, 0xA832);

EXTERN_API_C( Fixed )
FixedDivide                     (Fixed                  dividend,
                                 Fixed                  divisor)                            THREEWORDINLINE(0x303C, 0x0047, 0xA832);

EXTERN_API_C( Fixed )
FixedMultiply                   (Fixed                  multiplicand,
                                 Fixed                  multiplier)                         THREEWORDINLINE(0x303C, 0x0048, 0xA832);

/* This next call is (source * multiplier / divisor) -- it avoids underflow, overflow by using wides */
EXTERN_API_C( long )
MultiplyDivide                  (long                   source,
                                 long                   multiplier,
                                 long                   divisor)                            THREEWORDINLINE(0x303C, 0x0049, 0xA832);

EXTERN_API_C( unsigned long )
Magnitude                       (long                   deltaX,
                                 long                   deltaY)                             THREEWORDINLINE(0x303C, 0x004A, 0xA832);

EXTERN_API_C( long )
VectorMultiplyDivide            (long                   count,
                                 const long *           vector1,
                                 long                   step1,
                                 const long *           vector2,
                                 long                   step2,
                                 long                   divisor)                            THREEWORDINLINE(0x303C, 0x004B, 0xA832);




/* wide operations are defined within FixMath.h for PowerPC */
#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_OS_MAC && TARGET_CPU_68K
#if CALL_NOT_IN_CARBON
EXTERN_API_C( wide *)
WideAdd                         (wide *                 target,
                                 const wide *           source)                             THREEWORDINLINE(0x303C, 0x004C, 0xA832);

EXTERN_API_C( short )
WideCompare                     (const wide *           target,
                                 const wide *           source)                             THREEWORDINLINE(0x303C, 0x004D, 0xA832);

EXTERN_API_C( wide *)
WideNegate                      (wide *                 target)                             THREEWORDINLINE(0x303C, 0x004E, 0xA832);

EXTERN_API_C( wide *)
WideShift                       (wide *                 target,
                                 long                   shift)                              THREEWORDINLINE(0x303C, 0x004F, 0xA832);

EXTERN_API_C( unsigned long )
WideSquareRoot                  (const wide *           source)                             THREEWORDINLINE(0x303C, 0x0050, 0xA832);

EXTERN_API_C( wide *)
WideSubtract                    (wide *                 target,
                                 const wide *           source)                             THREEWORDINLINE(0x303C, 0x0051, 0xA832);

EXTERN_API_C( wide *)
WideMultiply                    (long                   multiplicand,
                                 long                   multiplier,
                                 wide *                 target)                             THREEWORDINLINE(0x303C, 0x0052, 0xA832);

/* returns the quotient */
EXTERN_API_C( long )
WideDivide                      (const wide *           dividend,
                                 long                   divisor,
                                 long *                 remainder)                          THREEWORDINLINE(0x303C, 0x0053, 0xA832);

/* quotient replaces dividend */
EXTERN_API_C( wide *)
WideWideDivide                  (wide *                 dividend,
                                 long                   divisor,
                                 long *                 remainder)                          THREEWORDINLINE(0x303C, 0x0055, 0xA832);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_MAC && TARGET_CPU_68K */



#if CALL_NOT_IN_CARBON
EXTERN_API_C( wide *)
VectorMultiply                  (long                   count,
                                 const long *           vector1,
                                 long                   step1,
                                 const long *           vector2,
                                 long                   step2,
                                 wide *                 dot)                                THREEWORDINLINE(0x303C, 0x0054, 0xA832);

EXTERN_API_C( unsigned long )
RandomBits                      (long                   count,
                                 long                   focus)                              THREEWORDINLINE(0x303C, 0x0056, 0xA832);

EXTERN_API_C( void )
SetRandomSeed                   (const wide *           seed)                               THREEWORDINLINE(0x303C, 0x0057, 0xA832);

EXTERN_API_C( wide *)
GetRandomSeed                   (wide *                 seed)                               THREEWORDINLINE(0x303C, 0x0058, 0xA832);

#endif  /* CALL_NOT_IN_CARBON */

#define FixedRound(a)       ((short)((Fixed)(a) + fixed1/2 >> 16))
#define FixedSquareRoot(a)  ((Fixed)FractSquareRoot(a) + 64 >> 7)
#define FixedTruncate(a)        ((short)((Fixed)(a) >> 16))
#define FixedToFract(a)     ((Fract)(a) << 14)
#define FractToFixed(a)     ((Fixed)(a) + 8192L >> 14)
#define FixedToInt(a)      ((short)((Fixed)(a) + fixed1/2 >> 16))
#define IntToFixed(a)      ((Fixed)(a) << 16)
#define FixedToFloat(a)  ((float)(a) / fixed1)
#define FloatToFixed(a)      ((Fixed)((float)(a) * fixed1))
#define FractToFloat(a)  ((float)(a) / fract1)
#define FloatToFract(a)  ((Fract)((float)(a) * fract1))
#define ColorToFract(a)  (((Fract) (a) << 14) + ((Fract)(a) + 2 >> 2))
#define FractToColor(a)  ((gxColorValue) ((a) - ((a) >> 16) + 8191 >> 14))
#ifndef ff  /* ff is already defined on some platforms */
#define ff(a)              IntToFixed(a)
#define fl(a)              FloatToFixed(a)
#endif

 
#if defined(__MWERKS__) && TARGET_CPU_68K
    #pragma pop
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

#endif /* __GXMATH__ */

