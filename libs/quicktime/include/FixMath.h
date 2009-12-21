/*
     File:       FixMath.h
 
     Contains:   Fixed Math Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FIXMATH__
#define __FIXMATH__

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


#define fixed1              ((Fixed) 0x00010000L)
#define fract1              ((Fract) 0x40000000L)
#define positiveInfinity    ((long)  0x7FFFFFFFL)
#define negativeInfinity    ((long)  0x80000000L)
/*
    FixRatio, FixMul, and FixRound were previously in ToolUtils.h
*/
EXTERN_API( Fixed )
FixRatio                        (short                  numer,
                                 short                  denom)                              ONEWORDINLINE(0xA869);

EXTERN_API( Fixed )
FixMul                          (Fixed                  a,
                                 Fixed                  b)                                  ONEWORDINLINE(0xA868);

EXTERN_API( short )
FixRound                        (Fixed                  x)                                  ONEWORDINLINE(0xA86C);

EXTERN_API( Fract )
Fix2Frac                        (Fixed                  x)                                  ONEWORDINLINE(0xA841);

EXTERN_API( long )
Fix2Long                        (Fixed                  x)                                  ONEWORDINLINE(0xA840);

EXTERN_API( Fixed )
Long2Fix                        (long                   x)                                  ONEWORDINLINE(0xA83F);

EXTERN_API( Fixed )
Frac2Fix                        (Fract                  x)                                  ONEWORDINLINE(0xA842);

EXTERN_API( Fract )
FracMul                         (Fract                  x,
                                 Fract                  y)                                  ONEWORDINLINE(0xA84A);

EXTERN_API( Fixed )
FixDiv                          (Fixed                  x,
                                 Fixed                  y)                                  ONEWORDINLINE(0xA84D);

EXTERN_API( Fract )
FracDiv                         (Fract                  x,
                                 Fract                  y)                                  ONEWORDINLINE(0xA84B);

EXTERN_API( Fract )
FracSqrt                        (Fract                  x)                                  ONEWORDINLINE(0xA849);

EXTERN_API( Fract )
FracSin                         (Fixed                  x)                                  ONEWORDINLINE(0xA848);

EXTERN_API( Fract )
FracCos                         (Fixed                  x)                                  ONEWORDINLINE(0xA847);

EXTERN_API( Fixed )
FixATan2                        (long                   x,
                                 long                   y)                                  ONEWORDINLINE(0xA818);

/*
    Frac2X, Fix2X, X2Fix, and X2Frac translate to and from
    the floating point type "extended" (that's what the X is for).
    On the original Mac this was 80-bits and the functions could be
    accessed via A-Traps.  When the 68881 co-processor was added,
    it used 96-bit floating point types, so the A-Traps could not 
    be used.  When PowerPC was added, it used 64-bit floating point
    types, so yet another prototype was added.
*/
#if TARGET_CPU_68K
#if TARGET_RT_MAC_68881
#if CALL_NOT_IN_CARBON
EXTERN_API( long double )
Frac2X                          (Fract                  x);

EXTERN_API( long double )
Fix2X                           (Fixed                  x);

EXTERN_API( Fixed )
X2Fix                           (long double            x);

EXTERN_API( Fract )
X2Frac                          (long double            x);

#endif  /* CALL_NOT_IN_CARBON */

#else
#if CALL_NOT_IN_CARBON
EXTERN_API( long double )
Frac2X                          (Fract                  x)                                  ONEWORDINLINE(0xA845);

EXTERN_API( long double )
Fix2X                           (Fixed                  x)                                  ONEWORDINLINE(0xA843);

EXTERN_API( Fixed )
X2Fix                           (long double            x)                                  ONEWORDINLINE(0xA844);

EXTERN_API( Fract )
X2Frac                          (long double            x)                                  ONEWORDINLINE(0xA846);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_RT_MAC_68881 */

#else
EXTERN_API( double )
Frac2X                          (Fract                  x);

EXTERN_API( double )
Fix2X                           (Fixed                  x);

EXTERN_API( Fixed )
X2Fix                           (double                 x);

EXTERN_API( Fract )
X2Frac                          (double                 x);

#endif  /* TARGET_CPU_68K */

/* QuickTime 3.0 makes these Wide routines available on other platforms*/
#if TARGET_CPU_PPC || !TARGET_OS_MAC || TARGET_CPU_X86
EXTERN_API_C( short )
WideCompare                     (const wide *           target,
                                 const wide *           source);

EXTERN_API_C( wide *)
WideAdd                         (wide *                 target,
                                 const wide *           source);

EXTERN_API_C( wide *)
WideSubtract                    (wide *                 target,
                                 const wide *           source);

EXTERN_API_C( wide *)
WideNegate                      (wide *                 target);

EXTERN_API_C( wide *)
WideShift                       (wide *                 target,
                                 long                   shift);

EXTERN_API_C( unsigned long )
WideSquareRoot                  (const wide *           source);

EXTERN_API_C( wide *)
WideMultiply                    (long                   multiplicand,
                                 long                   multiplier,
                                 wide *                 target);

/* returns the quotient */
EXTERN_API_C( long )
WideDivide                      (const wide *           dividend,
                                 long                   divisor,
                                 long *                 remainder);

/* quotient replaces dividend */
EXTERN_API_C( wide *)
WideWideDivide                  (wide *                 dividend,
                                 long                   divisor,
                                 long *                 remainder);

EXTERN_API_C( wide *)
WideBitShift                    (wide *                 src,
                                 long                   shift);

#endif  /* TARGET_CPU_PPC || !TARGET_OS_MAC || TARGET_CPU_X86 */



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

#endif /* __FIXMATH__ */

