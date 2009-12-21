/*
     File:       fp.h
 
     Contains:   FPCE Floating-Point Definitions and Declarations.
 
     Version:    Technology: MathLib v2
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1987-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FP__
#define __FP__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif


/********************************************************************************
*                                                                               *
*    A collection of numerical functions designed to facilitate a wide          *
*    range of numerical programming as required by C9X.                         *
*                                                                               *
*    The <fp.h> declares many functions in support of numerical programming.    *
*    It provides a superset of <math.h> and <SANE.h> functions.  Some           *
*    functionality previously found in <SANE.h> and not in the FPCE <fp.h>      *
*    can be found in this <fp.h> under the heading "__NOEXTENSIONS__".          *
*                                                                               *
*    All of these functions are IEEE 754 aware and treat exceptions, NaNs,      *
*    positive and negative zero and infinity consistent with the floating-      *
*    point standard.                                                            *
*                                                                               *
********************************************************************************/



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

/********************************************************************************
*                                                                               *
*                            Efficient types                                    *
*                                                                               *
*    float_t         Most efficient type at least as wide as float              *
*    double_t        Most efficient type at least as wide as double             *
*                                                                               *
*      CPU            float_t(bits)                double_t(bits)               *
*    --------        -----------------            -----------------             *
*    PowerPC          float(32)                    double(64)                   *
*    68K              long double(80/96)           long double(80/96)           *
*    x86              long double(80)              long double(80)              *
*                                                                               *
********************************************************************************/
#if defined(__MWERKS__) && defined(__cmath__)
/* these types were already defined in MSL's math.h */
#else
#if TARGET_CPU_PPC
typedef float                           float_t;
typedef double                          double_t;
#elif TARGET_CPU_68K
typedef long double                     float_t;
typedef long double                     double_t;
#elif TARGET_CPU_X86
#if NeXT || TARGET_OS_MAC
typedef double                          float_t;
typedef double                          double_t;
#else
typedef long double                     float_t;
typedef long double                     double_t;
#endif  /* NeXT || TARGET_OS_MAC */

#elif TARGET_CPU_MIPS
typedef double                          float_t;
typedef double                          double_t;
#elif TARGET_CPU_ALPHA
typedef double                          float_t;
typedef double                          double_t;
#elif TARGET_CPU_SPARC
typedef double                          float_t;
typedef double                          double_t;
#else
#error unsupported CPU
#endif  /*  */

/********************************************************************************
*                                                                               *
*                              Define some constants.                           *
*                                                                               *
*    HUGE_VAL            IEEE 754 value of infinity.                            *
*    INFINITY            IEEE 754 value of infinity.                            *
*    NAN                 A generic NaN (Not A Number).                          *
*    DECIMAL_DIG         Satisfies the constraint that the conversion from      *
*                        double to decimal and back is the identity function.   *
*                                                                               *
********************************************************************************/
#if TARGET_OS_MAC
  #if TARGET_RT_MAC_MACHO
    #define   HUGE_VAL                1e500 /* compatible with bsd math.h */
  #else
    #define   HUGE_VAL                __inf()
  #endif
  #define     INFINITY                __inf()
  #define     NAN                     nan("255")
#else
  #define     NAN                     sqrt(-1)
#endif

#if TARGET_CPU_PPC
    #define      DECIMAL_DIG              17 /* does not exist for double-double */
#elif TARGET_CPU_68K
    #define      DECIMAL_DIG              21
#endif      
#endif  /* __MWERKS__ && __cmath__ */
#if TARGET_OS_MAC
/* MSL already defines these */
#if !defined(__MWERKS__) || !defined(__cmath__)
/********************************************************************************
*                                                                               *
*                            Trigonometric functions                            *
*                                                                               *
*   acos        result is in [0,pi].                                            *
*   asin        result is in [-pi/2,pi/2].                                      *
*   atan        result is in [-pi/2,pi/2].                                      *
*   atan2       Computes the arc tangent of y/x in [-pi,pi] using the sign of   *
*               both arguments to determine the quadrant of the computed value. *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) cos(double_t x);

EXTERN_API_C( double_t ) sin(double_t x);

EXTERN_API_C( double_t ) tan(double_t x);

EXTERN_API_C( double_t ) acos(double_t x);

EXTERN_API_C( double_t ) asin(double_t x);

EXTERN_API_C( double_t ) atan(double_t x);

EXTERN_API_C( double_t ) atan2(double_t y, double_t x);



/********************************************************************************
*                                                                               *
*                              Hyperbolic functions                             *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) cosh(double_t x);

EXTERN_API_C( double_t ) sinh(double_t x);

EXTERN_API_C( double_t ) tanh(double_t x);

EXTERN_API_C( double_t ) acosh(double_t x);

EXTERN_API_C( double_t ) asinh(double_t x);

EXTERN_API_C( double_t ) atanh(double_t x);



/********************************************************************************
*                                                                               *
*                              Exponential functions                            *
*                                                                               *
*   expm1       expm1(x) = exp(x) - 1.  But, for small enough arguments,        *
*               expm1(x) is expected to be more accurate than exp(x) - 1.       *
*   frexp       Breaks a floating-point number into a normalized fraction       *
*               and an integral power of 2.  It stores the integer in the       *
*               object pointed by *exponent.                                    *
*   ldexp       Multiplies a floating-point number by an integer power of 2.    *
*   log1p       log1p = log(1 + x). But, for small enough arguments,            *
*               log1p is expected to be more accurate than log(1 + x).          *
*   logb        Extracts the exponent of its argument, as a signed integral     *
*               value. A subnormal argument is treated as though it were first  *
*               normalized. Thus:                                               *
*                                  1   <=   x * 2^(-logb(x))   <   2            *
*   modf        Returns fractional part of x as function result and returns     *
*               integral part of x via iptr. Note C9X uses double not double_t. *
*   scalb       Computes x * 2^n efficently.  This is not normally done by      *
*               computing 2^n explicitly.                                       *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) exp(double_t x);

EXTERN_API_C( double_t ) expm1(double_t x);

EXTERN_API_C( double_t ) exp2(double_t x);

EXTERN_API_C( double_t ) frexp(double_t x, int *exponent);

EXTERN_API_C( double_t ) ldexp(double_t x, int n);

EXTERN_API_C( double_t ) log(double_t x);

EXTERN_API_C( double_t ) log2(double_t x);

EXTERN_API_C( double_t ) log1p(double_t x);

EXTERN_API_C( double_t ) log10(double_t x);

EXTERN_API_C( double_t ) logb(double_t x);

#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( long double ) modfl(long double x, long double *iptrl);

#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( double_t ) modf(double_t x, double_t *iptr);

EXTERN_API_C( float ) modff(float x, float *iptrf);


EXTERN_API_C( double_t ) scalb(double_t x, long n);

/********************************************************************************
*                                                                               *
*                     Power and absolute value functions                        *
*                                                                               *
*   hypot       Computes the square root of the sum of the squares of its       *
*               arguments, without undue overflow or underflow.                 *
*   pow         Returns x raised to the power of y.  Result is more accurate    *
*               than using exp(log(x)*y).                                       *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) fabs(double_t x);

EXTERN_API_C( double_t ) hypot(double_t x, double_t y);

EXTERN_API_C( double_t ) pow(double_t x, double_t y);

EXTERN_API_C( double_t ) sqrt(double_t x);



/********************************************************************************
*                                                                               *
*                        Gamma and Error functions                              *
*                                                                               *
*   erf         The error function.                                             *
*   erfc        Complementary error function.                                   *
*   gamma       The gamma function.                                             *
*   lgamma      Computes the base-e logarithm of the absolute value of          *
*               gamma of its argument x, for x > 0.                             *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) erf(double_t x);

EXTERN_API_C( double_t ) erfc(double_t x);

EXTERN_API_C( double_t ) gamma(double_t x);

EXTERN_API_C( double_t ) lgamma(double_t x);



/********************************************************************************
*                                                                               *
*                        Nearest integer functions                              *
*                                                                               *
*   rint        Rounds its argument to an integral value in floating point      *
*               format, honoring the current rounding direction.                *
*                                                                               *
*   nearbyint   Differs from rint only in that it does not raise the inexact    *
*               exception. It is the nearbyint function recommended by the      *
*               IEEE floating-point standard 854.                               *
*                                                                               *
*   rinttol     Rounds its argument to the nearest long int using the current   *
*               rounding direction.  NOTE: if the rounded value is outside      *
*               the range of long int, then the result is undefined.            *
*                                                                               *
*   round       Rounds the argument to the nearest integral value in floating   *
*               point format similar to the Fortran "anint" function. That is:  *
*               add half to the magnitude and chop.                             *
*                                                                               *
*   roundtol    Similar to the Fortran function nint or to the Pascal round.    *
*               NOTE: if the rounded value is outside the range of long int,    *
*               then the result is undefined.                                   *
*                                                                               *
*   trunc       Computes the integral value, in floating format, nearest to     *
*               but no larger in magnitude than its argument.   NOTE: on 68K    *
*               compilers when using -elems881, trunc must return an int        *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) ceil(double_t x);

EXTERN_API_C( double_t ) floor(double_t x);

EXTERN_API_C( double_t ) rint(double_t x);

EXTERN_API_C( double_t ) nearbyint(double_t x);

EXTERN_API_C( long ) rinttol(double_t x);

EXTERN_API_C( double_t ) round(double_t x);

EXTERN_API_C( long ) roundtol(double_t round);

#if TARGET_RT_MAC_68881
#if CALL_NOT_IN_CARBON
EXTERN_API_C( int ) trunc(double_t x);

#endif  /* CALL_NOT_IN_CARBON */

#else
EXTERN_API_C( double_t ) trunc(double_t x);

#endif  /* TARGET_RT_MAC_68881 */


/********************************************************************************
*                                                                               *
*                            Remainder functions                                *
*                                                                               *
*   remainder       IEEE 754 floating point standard for remainder.             *
*   remquo          SANE remainder.  It stores into 'quotient' the 7 low-order  *
*                   bits of the integer quotient x/y, such that:                *
*                       -127 <= quotient <= 127.                                *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) fmod(double_t x, double_t y);

EXTERN_API_C( double_t ) remainder(double_t x, double_t y);

EXTERN_API_C( double_t ) remquo(double_t x, double_t y, int *quo);



/********************************************************************************
*                                                                               *
*                             Auxiliary functions                               *
*                                                                               *
*   copysign        Produces a value with the magnitude of its first argument   *
*                   and sign of its second argument.  NOTE: the order of the    *
*                   arguments matches the recommendation of the IEEE 754        *
*                   floating point standard,  which is opposite from the SANE   *
*                   copysign function.                                          *
*                                                                               *
*   nan             The call 'nan("n-char-sequence")' returns a quiet NaN       *
*                   with content indicated through tagp in the selected         *
*                   data type format.                                           *
*                                                                               *
*   nextafter       Computes the next representable value after 'x' in the      *
*                   direction of 'y'.  if x == y, then y is returned.           *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) copysign(double_t x, double_t y);

#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( long double ) nanl(const char *tagp);

#else
#define nanl(tagp) ((long double)nan(tagp))
#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( double ) nan(const char *tagp);

EXTERN_API_C( float ) nanf(const char *tagp);

#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( long double ) nextafterl(long double x, long double y);

#else
#define nextafterl(x,y) ((long double)nextafter((double)x,(double)y))
#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( double ) nextafterd(double x, double y);

EXTERN_API_C( float ) nextafterf(float x, float y);



/********************************************************************************
*                                                                               *
*                              Inquiry macros                                   *
*                                                                               *
*   fpclassify      Returns one of the FP_* values.                             *
*   isnormal        Non-zero if and only if the argument x is normalized.       *
*   isfinite        Non-zero if and only if the argument x is finite.           *
*   isnan           Non-zero if and only if the argument x is a NaN.            *
*   signbit         Non-zero if and only if the sign of the argument x is       *
*                   negative.  This includes, NaNs, infinities and zeros.       *
*                                                                               *
********************************************************************************/
enum {
    FP_SNAN                     = 0,                            /*      signaling NaN                         */
    FP_QNAN                     = 1,                            /*      quiet NaN                             */
    FP_INFINITE                 = 2,                            /*      + or - infinity                       */
    FP_ZERO                     = 3,                            /*      + or - zero                           */
    FP_NORMAL                   = 4,                            /*      all normal numbers                    */
    FP_SUBNORMAL                = 5                             /*      denormal numbers                      */
};




#if TARGET_RT_MAC_MACHO
#define __fpclassify(x) __fpclassifyd((double)x)
#define __isnormal(x)   __isnormald((double)x)
#define __isfinite(x)   __isfinited((double)x)
#define __isnan(x)      __isnand((double)x)
#define __signbit(x)    __signbitd((double)x)
#endif

#define      fpclassify(x)    ( ( sizeof ( x ) == sizeof(long double) ) ?      \
                              __fpclassify  ( x ) :                            \
                                ( sizeof ( x ) == sizeof(double) ) ?           \
                              __fpclassifyd ( x ) :                            \
                              __fpclassifyf ( x ) )
#define      isnormal(x)      ( ( sizeof ( x ) == sizeof(long double) ) ?      \
                              __isnormal ( x ) :                               \
                                ( sizeof ( x ) == sizeof(double) ) ?           \
                              __isnormald ( x ) :                              \
                              __isnormalf ( x ) )
#define      isfinite(x)      ( ( sizeof ( x ) == sizeof(long double) ) ?      \
                              __isfinite ( x ) :                               \
                                ( sizeof ( x ) == sizeof(double) ) ?           \
                              __isfinited ( x ) :                              \
                              __isfinitef ( x ) )
#define      isnan(x)         ( ( sizeof ( x ) == sizeof(long double) ) ?      \
                              __isnan ( x ) :                                  \
                              ( sizeof ( x ) == sizeof(double) ) ?             \
                              __isnand ( x ) :                                 \
                              __isnanf ( x ) )
#define      signbit(x)       ( ( sizeof ( x ) == sizeof(long double) ) ?      \
                              __signbit ( x ) :                                \
                              ( sizeof ( x ) == sizeof(double) ) ?             \
                              __signbitd ( x ) :                               \
                              __signbitf ( x ) )


#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( long ) __fpclassify(long double x);

#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( long ) __fpclassifyd(double x);

EXTERN_API_C( long ) __fpclassifyf(float x);

#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( long ) __isnormal(long double x);

#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( long ) __isnormald(double x);

EXTERN_API_C( long ) __isnormalf(float x);

#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( long ) __isfinite(long double x);

#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( long ) __isfinited(double x);

EXTERN_API_C( long ) __isfinitef(float x);

#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( long ) __isnan(long double x);

#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( long ) __isnand(double x);

EXTERN_API_C( long ) __isnanf(float x);

#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( long ) __signbit(long double x);

#endif  /* !TARGET_RT_MAC_MACHO */

EXTERN_API_C( long ) __signbitd(double x);

EXTERN_API_C( long ) __signbitf(float x);

EXTERN_API_C( double_t ) __inf(void );



/********************************************************************************
*                                                                               *
*                      Max, Min and Positive Difference                         *
*                                                                               *
*   fdim        Determines the 'positive difference' between its arguments:     *
*               { x - y, if x > y }, { +0, if x <= y }.  If one argument is     *
*               NaN, then fdim returns that NaN.  if both arguments are NaNs,   *
*               then fdim returns the first argument.                           *
*                                                                               *
*   fmax        Returns the maximum of the two arguments.  Corresponds to the   *
*               max function in FORTRAN.  NaN arguments are treated as missing  *
*               data.  If one argument is NaN and the other is a number, then   *
*               the number is returned.  If both are NaNs then the first        *
*               argument is returned.                                           *
*                                                                               *
*   fmin        Returns the minimum of the two arguments.  Corresponds to the   *
*               min function in FORTRAN.  NaN arguments are treated as missing  *
*               data.  If one argument is NaN and the other is a number, then   *
*               the number is returned.  If both are NaNs then the first        *
*               argument is returned.                                           *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) fdim(double_t x, double_t y);

EXTERN_API_C( double_t ) fmax(double_t x, double_t y);

EXTERN_API_C( double_t ) fmin(double_t x, double_t y);

#endif /* !defined(__MWERKS__) || !defined(__cmath__) */

/*******************************************************************************
*                                Constants                                     *
*******************************************************************************/
extern const double_t pi;
/********************************************************************************
*                                                                               *
*                              Non NCEG extensions                              *
*                                                                               *
********************************************************************************/
#ifndef __NOEXTENSIONS__
/********************************************************************************
*                                                                               *
*                              Financial functions                              *
*                                                                               *
*   compound        Computes the compound interest factor "(1 + rate)^periods"  *
*                   more accurately than the straightforward computation with   *
*                   the Power function.  This is SANE's compound function.      *
*                                                                               *
*   annuity         Computes the present value factor for an annuity            *
*                   "(1 - (1 + rate)^(-periods)) /rate" more accurately than    *
*                   the straightforward computation with the Power function.    *
*                   This is SANE's annuity function.                            *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) compound(double_t rate, double_t periods);

EXTERN_API_C( double_t ) annuity(double_t rate, double_t periods);



/********************************************************************************
*                                                                               *
*                              Random function                                  *
*                                                                               *
*   randomx         A pseudorandom number generator.  It uses the iteration:    *
*                               (7^5*x)mod(2^31-1)                              *
*                                                                               *
********************************************************************************/
EXTERN_API_C( double_t ) randomx(double_t *x);



/*******************************************************************************
*                              Relational operator                             *
*******************************************************************************/
/*      relational operator      */
typedef short                           relop;
enum {
    GREATERTHAN                 = 0,
    LESSTHAN                    = 1,
    EQUALTO                     = 2,
    UNORDERED                   = 3
};

#if !defined(__MWERKS__) || !defined(__cmath__)
EXTERN_API_C( relop ) relation(double_t x, double_t y);

#endif /* !defined(__MWERKS__) || !defined(__cmath__) */


/********************************************************************************
*                                                                               *
*                         Binary to decimal conversions                         *
*                                                                               *
*   SIGDIGLEN   Significant decimal digits.                                     *
*                                                                               *
*   decimal     A record which provides an intermediate unpacked form for       *
*               programmers who wish to do their own parsing of numeric input   *
*               or formatting of numeric output.                                *
*                                                                               *
*   decform     Controls each conversion to a decimal string.  The style field  *
*               is either FLOATDECIMAL or FIXEDDECIMAL. If FLOATDECIMAL, the    *
*               value of the field digits is the number of significant digits.  *
*               If FIXEDDECIMAL value of the field digits is the number of      *
*               digits to the right of the decimal point.                       *
*                                                                               *
*   num2dec     Converts a double_t to a decimal record using a decform.        *
*   dec2num     Converts a decimal record d to a double_t value.                *
*   dec2str     Converts a decform and decimal to a string using a decform.     *
*   str2dec     Converts a string to a decimal struct.                          *
*   dec2d       Similar to dec2num except a double is returned (68k only).      *
*   dec2f       Similar to dec2num except a float is returned.                  *
*   dec2s       Similar to dec2num except a short is returned.                  *
*   dec2l       Similar to dec2num except a long is returned.                   *
*                                                                               *
********************************************************************************/
#if TARGET_CPU_PPC
    #define SIGDIGLEN      36  
#elif TARGET_CPU_68K
    #define SIGDIGLEN      20
#elif TARGET_CPU_X86
    #define SIGDIGLEN      20
#endif
#define      DECSTROUTLEN   80               /* max length for dec2str output */
#define      FLOATDECIMAL   ((char)(0))
#define      FIXEDDECIMAL   ((char)(1))
struct decimal {
    char                            sgn;                        /* sign 0 for +, 1 for - */
    char                            unused;
    short                           exp;                        /* decimal exponent */
    struct {
        unsigned char                   length;
        unsigned char                   text[SIGDIGLEN];        /* significant digits */
        unsigned char                   unused;
    }                               sig;
};
typedef struct decimal decimal;

struct decform {
    char                            style;                      /*  FLOATDECIMAL or FIXEDDECIMAL */
    char                            unused;
    short                           digits;
};
typedef struct decform decform;
EXTERN_API_C( void ) num2dec(const decform *f, double_t x, decimal *d);

EXTERN_API_C( double_t ) dec2num(const decimal *d);

EXTERN_API_C( void ) dec2str(const decform *f, const decimal *d, char *s);

EXTERN_API_C( void ) str2dec(const char *s, short *ix, decimal *d, short *vp);

#if TARGET_CPU_68K
#if CALL_NOT_IN_CARBON
EXTERN_API_C( double ) dec2d(const decimal *d);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_CPU_68K */

EXTERN_API_C( float ) dec2f(const decimal *d);

EXTERN_API_C( short ) dec2s(const decimal *d);

EXTERN_API_C( long ) dec2l(const decimal *d);




/********************************************************************************
*                                                                               *
*                         68k-only Transfer Function Prototypes                 *
*                                                                               *
********************************************************************************/
#if TARGET_CPU_68K
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void ) x96tox80(const extended96 *x, extended80 *x80);

EXTERN_API_C( void ) x80tox96(const extended80 *x80, extended96 *x);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_CPU_68K */

#endif  /* !defined(__NOEXTENSIONS__) */

/********************************************************************************
*                                                                               *
*                         PowerPC-only Function Prototypes                      *
*                                                                               *
********************************************************************************/

#if TARGET_CPU_PPC
#if !TARGET_RT_MAC_MACHO
#ifndef __MWERKS__  /* Metrowerks does not support double double */
EXTERN_API_C( long double ) cosl(long double x);

EXTERN_API_C( long double ) sinl(long double x);

EXTERN_API_C( long double ) tanl(long double x);

EXTERN_API_C( long double ) acosl(long double x);

EXTERN_API_C( long double ) asinl(long double x);

EXTERN_API_C( long double ) atanl(long double x);

EXTERN_API_C( long double ) atan2l(long double y, long double x);

EXTERN_API_C( long double ) coshl(long double x);

EXTERN_API_C( long double ) sinhl(long double x);

EXTERN_API_C( long double ) tanhl(long double x);

EXTERN_API_C( long double ) acoshl(long double x);

EXTERN_API_C( long double ) asinhl(long double x);

EXTERN_API_C( long double ) atanhl(long double x);

EXTERN_API_C( long double ) expl(long double x);

EXTERN_API_C( long double ) expm1l(long double x);

EXTERN_API_C( long double ) exp2l(long double x);

EXTERN_API_C( long double ) frexpl(long double x, int *exponent);

EXTERN_API_C( long double ) ldexpl(long double x, int n);

EXTERN_API_C( long double ) logl(long double x);

EXTERN_API_C( long double ) log1pl(long double x);

EXTERN_API_C( long double ) log10l(long double x);

EXTERN_API_C( long double ) log2l(long double x);

EXTERN_API_C( long double ) logbl(long double x);

EXTERN_API_C( long double ) scalbl(long double x, long n);

EXTERN_API_C( long double ) fabsl(long double x);

EXTERN_API_C( long double ) hypotl(long double x, long double y);

EXTERN_API_C( long double ) powl(long double x, long double y);

EXTERN_API_C( long double ) sqrtl(long double x);

EXTERN_API_C( long double ) erfl(long double x);

EXTERN_API_C( long double ) erfcl(long double x);

EXTERN_API_C( long double ) gammal(long double x);

EXTERN_API_C( long double ) lgammal(long double x);

EXTERN_API_C( long double ) ceill(long double x);

EXTERN_API_C( long double ) floorl(long double x);

EXTERN_API_C( long double ) rintl(long double x);

EXTERN_API_C( long double ) nearbyintl(long double x);

EXTERN_API_C( long ) rinttoll(long double x);

EXTERN_API_C( long double ) roundl(long double x);

EXTERN_API_C( long ) roundtoll(long double round);

EXTERN_API_C( long double ) truncl(long double x);

EXTERN_API_C( long double ) remainderl(long double x, long double y);

EXTERN_API_C( long double ) remquol(long double x, long double y, int *quo);

EXTERN_API_C( long double ) copysignl(long double x, long double y);

EXTERN_API_C( long double ) fdiml(long double x, long double y);

EXTERN_API_C( long double ) fmaxl(long double x, long double y);

EXTERN_API_C( long double ) fminl(long double x, long double y);

#endif /* __MWERKS__ */
#else
#define cosl(x)          ((long double) cos((double_t)(x)))
#define sinl(x)          ((long double) sin((double_t)(x)))
#define tanl(x)          ((long double) tan((double_t)(x)))
#define acosl(x)         ((long double) acos((double_t)(x)))
#define asinl(x)         ((long double) asin((double_t)(x)))
#define atanl(x)         ((long double) atan((double_t)(x)))
#define atan2l(y,x)      ((long double) atan2((double_t)(y),(double_t)(x)))
#define coshl(x)         ((long double) cosh((double_t)(x)))
#define sinhl(x)         ((long double) sinh((double_t)(x)))
#define tanhl(x)         ((long double) tanh((double_t)(x)))
#define acoshl(x)        ((long double) acosh((double_t)(x)))
#define asinhl(x)        ((long double) asinh((double_t)(x)))
#define atanhl(x)        ((long double) atanh((double_t)(x)))
#define expl(x)          ((long double) exp((double_t)(x)))
#define expm1l(x)        ((long double) expm1((double_t)(x)))
#define exp2l(x)         ((long double) exp2((double_t)(x)))
#define frexpl(x,exp)    ((long double) frexp((double_t)(x),(exp)))
#define ldexpl(x,n)      ((long double) ldexp((double_t)(x),(n)))
#define logl(x)          ((long double) log((double_t)(x)))
#define log1pl(x)        ((long double) log1p((double_t)(x)))
#define log10l(x)        ((long double) log10((double_t)(x)))
#define log2l(x)         ((long double) log2((double_t)(x)))
#define logbl(x)         ((long double) logb((double_t)(x)))
#define scalbl(x,n)      ((long double) scalb((double_t)(x),(n)))
#define fabsl(x)         ((long double) fabs((double_t)(x)))
#define hypotl(x,y)      ((long double) hypot((double_t)(x),(double_t)(y)))
#define powl(x,y)        ((long double) pow((double_t)(x),(double_t)(y)))
#define sqrtl(x)         ((long double) sqrt((double_t)(x)))
#define erfl(x)          ((long double) erf((double_t)(x)))
#define erfcl(x)         ((long double) erfc((double_t)(x)))
#define gammal(x)        ((long double) gamma((double_t)(x)))
#define lgammal(x)       ((long double) lgamma((double_t)(x)))
#define ceill(x)         ((long double) ceil((double_t)(x)))
#define floorl(x)        ((long double) floor((double_t)(x)))
#define rintl(x)         ((long double) rint((double_t)(x)))
#define nearbyintl(x)    ((long double) nearbyint((double_t)(x)))
#define rinttoll(x)      (rinttol((double_t)(x)))
#define roundl(x)        ((long double) round((double_t)(x)))
#define roundtoll(x)     (roundtol((double_t)(x)))
#define truncl(x)        ((long double) trunc((double_t)(x)))
#define remainderl(x,y)  ((long double) remainder((double_t)(x),(double_t)(y)))
#define remquol(x,y,quo) ((long double) remquo((double_t)(x),(double_t)(y),(quo)))
#define copysignl(x,y)   ((long double) copysign((double_t)(x),(double_t)(y)))
#define fdiml(x,y)       ((long double) fdim((double_t)(x),(double_t)(y)))
#define fmaxl(x,y)       ((long double) fmax((double_t)(x),(double_t)(y)))
#define fminl(x,y)       ((long double) fmin((double_t)(x),(double_t)(y)))
#endif  /* !TARGET_RT_MAC_MACHO */

#ifndef __NOEXTENSIONS__
#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( relop ) relationl(long double x, long double y);

EXTERN_API_C( void ) num2decl(const decform *f, long double x, decimal *d);

EXTERN_API_C( long double ) dec2numl(const decimal *d);

#else

       #define relationl(x,y) (relation((double_t)x,(double_t)y))
     #define num2decl(f,x,d) (num2dec(f,(double_t)x,d))
     #define dec2numl(d) ((long double)dec2num(d))
      
#endif  /* !TARGET_RT_MAC_MACHO */

#endif  /* !defined(__NOEXTENSIONS__) */

#endif  /* TARGET_CPU_PPC */

#endif  /* TARGET_OS_MAC */

#ifndef __NOEXTENSIONS__
#if !TARGET_RT_MAC_MACHO
EXTERN_API_C( void ) x80told(const extended80 *x80, long double *x);

EXTERN_API_C( void ) ldtox80(const long double *x, extended80 *x80);

#else

   #define x80told(x80,ld) (*(ld) = x80tod(x80))
  #define ldtox80(ld,x80) do { double d = (double) *(ld); dtox80(&d, (x80)); } while (0)
 
#endif  /* !TARGET_RT_MAC_MACHO */

/*    
        MathLib v2 has two new transfer functions: x80tod and dtox80.  They can 
        be used to directly transform 68k 80-bit extended data types to double
        and back for PowerPC based machines without using the functions
        x80told or ldtox80.  Double rounding may occur. 
    */
EXTERN_API_C( double ) x80tod(const extended80 *x80);

EXTERN_API_C( void ) dtox80(const double *x, extended80 *x80);

#endif  /* !defined(__NOEXTENSIONS__) */


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

#endif /* __FP__ */

