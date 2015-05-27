/*
     File:       Math64.h
 
     Contains:   64-bit integer math Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MATH64__
#define __MATH64__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

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


/*--------------------------------------------------------------------------------
                These routines are intended to provide C software support for
                64 bit integer types.  Their behavior should mimic anticipated
                64 bit hardware. This implementation should replace use of the
                "wide" type found in PowerPC.

    The following routines are available for performing math on 64-bit integers:
    
    S64Max
                Returns the largest representable SInt64.
    S64Min
                Returns the smallest (i.e. most negative) SInt64.  Note: the negative
                (absolute value) of this number is not representable in an SInt64.
                That means that S64Negate(S64Min) is not representable (in fact,
                it returns S64Min).
    S64Add
                Adds two integers, producing an integer result.  If an overflow
                occurs the result is congruent mod (2^64) as if the operands and
                result were unsigned.  No overflow is signaled.
    
    S64Subtract
                Subtracts two integers, producing an integer result.  If an overflow
                occurs the result is congruent mod (2^64) as if the operands and
                result were unsigned.  No overflow is signaled.

    S64Negate
                Returns the additive inverse of a signed number (i.e. it returns
                0 - the number).  S64Negate (S64Min) is not representable (in fact,
                it returns S64Min).
    
    S64Absolute
                Returns the absolute value of the number (i.e. the number if
                it is positive, or 0 - the number if it is negative).
                See S64Negate above.
                
    S64Multiply
                Multiplies two signed numbers, producing a signed result.  Overflow
                is ignored and the low-order part of the product is returned.  The
                sign of the result is not guaranteed to be correct if the magnitude
                of the product is not representable.
                
    S64Div
                Divides dividend by divisor, returning the quotient.
                
    S64Mod
                Returns the remainder of divide of dividend by divisor.  The sign of
                the remainder is the same as the sign of the dividend (i.e., it takes
                the absolute values of the operands, does the division, then fixes
                the sign of the quotient and remainder).

    S64Divide
                Divides dividend by divisor, returning the quotient.  The remainder
                is returned in *remainder if remainder (the pointer) is non-NULL.
                The sign of the remainder is the same as the sign of the dividend
                (i.e. it takes the absolute values of the operands, does the division,
                then fixes the sign of the quotient and remainder).  If the divisor
                is zero, then S64Max() will be returned (or S64Min() if the dividend
                is negative), and the remainder will be the dividend; no error is
                reported.
    
    S64Set
                Given an SInt32, returns an SInt64 with the same value.  Use this
                routine instead of coding 64-bit constants (at least when the
                constant will fit in an SInt32).
    
    S64SetU
                Given a UInt32, returns a SInt64 with the same value.
                
    S64Set
                Given an SInt64, returns an SInt32 by discarding the high-order
                32 bits.
    
    S64Compare
                Given two signed numbers, left and right, returns an
                SInt32 that compares with zero the same way left compares with
                right.  If you wanted to perform a comparison on 64-bit integers
                of the form:
                        operand_1 <operation> operand_2
                then you could use an expression of the form:
                        xxxS64Compare(operand_1,operand_2) <operation> 0
                to test for the same condition.
                
                CAUTION: DO NOT depend on the exact value returned by this routine.
                Only the sign (i.e. positive, zero, or negative) of the result is
                guaranteed.

    S64And, S64Or, S64Eor and S64Not
    
                Return Boolean (1 or 0) depending on the outcome of the logical
                operation.

    S64BitwiseAnd, S64BitwiseOr, S64BitwiseEor and S64BitwiseNot
    
                Return the Bitwise result.
                
    S64ShiftRight and S64ShiftLeft
    
                The lower 7 bits of the shift argument determines the amount of 
                shifting.  S64ShiftRight is an arithmetic shift while U64ShiftRight
                is a logical shift.

    SInt64ToLongDouble
                
                Converts SInt64 to long double.  Note all SInt64s fit exactly into 
                long doubles, thus, the binary -> decimal conversion routines
                in fp.h can be used to achieve SInt64 -> long double -> decimal
                conversions.
                
    LongDoubleToSInt64
    
                Converts a long double to a SInt64.  Any decimal string that fits
                into a SInt64 can be converted exactly into a long double, using the
                conversion routines found in fp.h.  Then this routine can be used
                to complete the conversion to SInt64.
                
    SInt64ToWide
    
                Converts a SInt64 to a wide struct.  If SInt64 is implemented
                as a typedef of wide, the marco does nothing. If SInt64 is 
                implememnted as a long long, it casts the long long into a 
                wide struct.
    
    WideToSInt64
    
                Converts a wide struct into a SInt64.  If SInt64 is implemented
                as a typedef of wide, the marco does nothing. If SInt64 is 
                implememnted as a long long, it reads the struct into a long long.
    
    
    The corresponding UInt64 routines are also included.
    
--------------------------------------------------------------------------------*/

#if TYPE_LONGLONG

#ifdef __MRC__
    #define S64Max() 9223372036854775807LL
#else
   #define S64Max() 9223372036854775807
#endif
#define S64Min() (-S64Max() - 1)
#define S64Add(x, y) ((SInt64) (x) + (SInt64) (y))
#define S64Subtract(x, y) ((SInt64) (x) - (SInt64) (y))
#define S64Negate(x) (-(SInt64) (x))
#define S64Absolute(x) absll((SInt64) (x))
#define S64Multiply(x, y) ((SInt64) (x) * (SInt64) (y))
#define S64Div(x, y) ((SInt64) (x) / (SInt64) (y))
#define S64Mod(x, y) ((SInt64) (x) % (SInt64) (y))
#define S64Set(x) ((SInt64) (x))
#define S64SetU(x) ((SInt64) (x))
#define S32Set(x) ((SInt32) (x))

#define S64And(x, y) ((Boolean)((SInt64) (x) && (SInt64) (y)))
#define S64Or(x, y) ((Boolean)((SInt64) (x) || (SInt64) (y)))
#define S64Eor(x, y) ((Boolean)((SInt64) (x) ^ (SInt64) (y)))
#define S64Not(x) ((Boolean)(!(SInt64) (x)))
#define S64BitwiseAnd(x, y) ((SInt64) (x) & (SInt64) (y))
#define S64BitwiseOr(x, y) ((SInt64) (x) | (SInt64) (y))
#define S64BitwiseEor(x, y) (((SInt64) (x) & (~(SInt64) (y))) | ((~(SInt64) (x)) & (SInt64) (y)))
#define S64BitwiseNot(x) (~(SInt64) (x))
#define S64ShiftRight(x, y) ((SInt64) (x) >> (UInt32) (y))
#define S64ShiftLeft(x, y) ((SInt64) (x) << (UInt32) (y))
#define SInt64ToLongDouble(x) ((long double)(x))
#define LongDoubleToSInt64(x) ((SInt64)(x))


#ifdef __MRC__
    #define U64Max() 0xffffffffffffffffULL
#else
   #define U64Max() 0xffffffffffffffff
#endif
#define U64Add(x, y) ((UInt64) (x) + (UInt64) (y))
#define U64Subtract(x, y) ((UInt64) (x) - (UInt64) (y))
#define U64Multiply(x, y) ((UInt64) (x) * (UInt64) (y))
#define U64Div(x, y) ((UInt64) (x) / (UInt64) (y))
#define U64Mod(x, y) ((UInt64) (x) % (UInt64) (y))
#define U64Set(x) ((UInt64) (x))
#define U64SetU(x) ((UInt64) (x))
#define U32SetU(x) ((UInt32) (x))

#define U64And(x, y) ((Boolean)((UInt64) (x) && (UInt64) (y)))
#define U64Or(x, y) ((Boolean)((UInt64) (x) || (UInt64) (y)))
#define U64Eor(x, y) ((Boolean)((UInt64) (x) ^ (UInt64) (y)))
#define U64Not(x) ((Boolean)(!(UInt64) (x)))
#define U64BitwiseAnd(x, y) ((UInt64) (x) & (UInt64) (y))
#define U64BitwiseOr(x, y) ((UInt64) (x) | (UInt64) (y))
#define U64BitwiseEor(x, y) (((UInt64) (x) & (~(UInt64) (y))) | ((~(UInt64) (x)) & (UInt64) (y)))
#define U64BitwiseNot(x) (~(UInt64) (x))
#define U64ShiftRight(x, y) ((UInt64) (x) >> (UInt32) (y))
#define U64ShiftLeft(x, y) ((UInt64) (x) << (UInt32) (y))
#define UInt64ToLongDouble(x) ((long double)(x))
#define LongDoubleToUInt64(x) ((UInt64)(x))
#define UInt64ToSInt64(x) ((SInt64)(x))
#define SInt64ToUInt64(x) ((UInt64)(x))

#else
EXTERN_API_C( SInt64 ) S64Max(void );

EXTERN_API_C( SInt64 ) S64Min(void );

EXTERN_API_C( SInt64 ) S64Add(SInt64 x, SInt64 y);

EXTERN_API_C( SInt64 ) S64Subtract(SInt64 left, SInt64 right);

EXTERN_API_C( SInt64 ) S64Negate(SInt64 value);

EXTERN_API_C( SInt64 ) S64Absolute(SInt64 value);

EXTERN_API_C( SInt64 ) S64Multiply(SInt64 xparam, SInt64 yparam);

#define S64Div(dividend, divisor) S64Divide(dividend, divisor, NULL)

#if CALL_NOT_IN_CARBON
EXTERN_API_C( SInt64 ) S64Mod(SInt64 dividend, SInt64 divisor);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API_C( SInt64 ) S64Divide(SInt64 dividend, SInt64 divisor, SInt64 *remainder);

EXTERN_API_C( SInt64 ) S64Set(SInt32 value);

EXTERN_API_C( SInt64 ) S64SetU(UInt32 value);

EXTERN_API_C( SInt32 ) S32Set(SInt64 value);

EXTERN_API_C( Boolean ) S64And(SInt64 left, SInt64 right);

EXTERN_API_C( Boolean ) S64Or(SInt64 left, SInt64 right);

EXTERN_API_C( Boolean ) S64Eor(SInt64 left, SInt64 right);

EXTERN_API_C( Boolean ) S64Not(SInt64 value);

EXTERN_API_C( SInt64 ) S64BitwiseAnd(SInt64 left, SInt64 right);

EXTERN_API_C( SInt64 ) S64BitwiseOr(SInt64 left, SInt64 right);

EXTERN_API_C( SInt64 ) S64BitwiseEor(SInt64 left, SInt64 right);

EXTERN_API_C( SInt64 ) S64BitwiseNot(SInt64 value);

EXTERN_API_C( SInt64 ) S64ShiftRight(SInt64 value, UInt32 shift);

EXTERN_API_C( SInt64 ) S64ShiftLeft(SInt64 value, UInt32 shift);

#if !TARGET_RT_MAC_MACHO
/*
    "long double" means 128 bit type on PowerPC and 80-bit type on 68K
*/
EXTERN_API_C( long double ) SInt64ToLongDouble(SInt64 value);

EXTERN_API_C( SInt64 ) LongDoubleToSInt64(long double value);

#else

#define SInt64ToLongDouble(x) ((long double)(x))
#define LongDoubleToSInt64(x) ((SInt64)(x))

#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( UInt64 ) U64Max(void );

EXTERN_API_C( UInt64 ) U64Add(UInt64 x, UInt64 y);

EXTERN_API_C( UInt64 ) U64Subtract(UInt64 left, UInt64 right);

EXTERN_API_C( UInt64 ) U64Multiply(UInt64 xparam, UInt64 yparam);

#define U64Div(dividend, divisor) U64Divide(dividend, divisor, NULL)

#if CALL_NOT_IN_CARBON
EXTERN_API_C( UInt64 ) U64Mod(UInt64 dividend, UInt64 divisor);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API_C( UInt64 ) U64Divide(UInt64 dividend, UInt64 divisor, UInt64 *remainder);

EXTERN_API_C( UInt64 ) U64Set(SInt32 value);

EXTERN_API_C( UInt64 ) U64SetU(UInt32 value);

EXTERN_API_C( UInt32 ) U32SetU(UInt64 value);

EXTERN_API_C( Boolean ) U64And(UInt64 left, UInt64 right);

EXTERN_API_C( Boolean ) U64Or(UInt64 left, UInt64 right);

EXTERN_API_C( Boolean ) U64Eor(UInt64 left, UInt64 right);

EXTERN_API_C( Boolean ) U64Not(UInt64 value);

EXTERN_API_C( UInt64 ) U64BitwiseAnd(UInt64 left, UInt64 right);

EXTERN_API_C( UInt64 ) U64BitwiseOr(UInt64 left, UInt64 right);

EXTERN_API_C( UInt64 ) U64BitwiseEor(UInt64 left, UInt64 right);

EXTERN_API_C( UInt64 ) U64BitwiseNot(UInt64 value);

EXTERN_API_C( UInt64 ) U64ShiftRight(UInt64 value, UInt32 shift);

EXTERN_API_C( UInt64 ) U64ShiftLeft(UInt64 value, UInt32 shift);

#if !TARGET_RT_MAC_MACHO
/*
    "long double" means 128 bit type on PowerPC and 80-bit type on 68K
*/
EXTERN_API_C( long double ) UInt64ToLongDouble(UInt64 value);

EXTERN_API_C( UInt64 ) LongDoubleToUInt64(long double value);

#else

#define UInt64ToLongDouble(x) ((long double)(x))
#define LongDoubleToUInt64(x) ((UInt64)(x))

#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( SInt64 ) UInt64ToSInt64(UInt64 value);

EXTERN_API_C( UInt64 ) SInt64ToUInt64(SInt64 value);

#endif  /* TYPE_LONGLONG */

EXTERN_API_C( SInt32 ) S64Compare(SInt64 left, SInt64 right);

EXTERN_API_C( SInt32 ) U64Compare(UInt64 left, UInt64 right);


// sam says you want these to be dispatched routines
#if TARGET_OS_WIN32 && TYPE_LONGLONG
#define S64Compare(left,right) ((int)(left - right))
#define U64Compare(left,right) ((int)(left - right))
#endif


/* 
   Functions to convert between [Unsigned]Wide and [S|U]Int64 types.
  
   These functions are necessary if source code which uses both
   wide and SInt64 is to compile under a compiler that supports
   long long.
*/
#if TYPE_LONGLONG 
   #define SInt64ToWide(x)         (*((wide*)(&x)))
   #define WideToSInt64(x)         (*((SInt64*)(&x)))
 #define UInt64ToUnsignedWide(x) (*((UnsignedWide*)(&x)))
   #define UnsignedWideToUInt64(x) (*((UInt64*)(&x)))
#else
   #define SInt64ToWide(x)         (x)
    #define WideToSInt64(x)         (x)
    #define UInt64ToUnsignedWide(x) (x)
    #define UnsignedWideToUInt64(x) (x)
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

#endif /* __MATH64__ */

