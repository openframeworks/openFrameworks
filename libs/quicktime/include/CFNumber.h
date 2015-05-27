/*
     File:       CFNumber.h
 
     Contains:   CoreFoundation numbers
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFNUMBER__
#define __CFNUMBER__

#ifndef __CFBASE__
#include <CFBase.h>
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

#if PRAGMA_ENUM_ALWAYSINT
    #if defined(__fourbyteints__) && !__fourbyteints__ 
        #define __CFNUMBER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __CFNUMBER__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

typedef const struct __CFBoolean * CFBooleanRef;
extern const CFBooleanRef kCFBooleanTrue;
extern const CFBooleanRef kCFBooleanFalse;
EXTERN_API_C( CFTypeID )
CFBooleanGetTypeID              (void);


/* Returns the value of the given CFBoolean instance. */
EXTERN_API_C( Boolean )
CFBooleanGetValue               (CFBooleanRef           boolean);



enum CFNumberType {
                                                                /* Types from MacTypes.h */
    kCFNumberSInt8Type          = 1,
    kCFNumberSInt16Type         = 2,
    kCFNumberSInt32Type         = 3,
    kCFNumberSInt64Type         = 4,
    kCFNumberFloat32Type        = 5,
    kCFNumberFloat64Type        = 6,                            /* 64-bit IEEE 754 */
                                                                /* Basic C types */
    kCFNumberCharType           = 7,
    kCFNumberShortType          = 8,
    kCFNumberIntType            = 9,
    kCFNumberLongType           = 10,
    kCFNumberLongLongType       = 11,
    kCFNumberFloatType          = 12,
    kCFNumberDoubleType         = 13,                           /* Other */
    kCFNumberCFIndexType        = 14,
    kCFNumberMaxType            = 14
};
typedef enum CFNumberType CFNumberType;

typedef const struct __CFNumber * CFNumberRef;
extern const CFNumberRef kCFNumberPositiveInfinity;
extern const CFNumberRef kCFNumberNegativeInfinity;
extern const CFNumberRef kCFNumberNaN;
EXTERN_API_C( CFTypeID )
CFNumberGetTypeID               (void);


/* Creates a CFNumber with the given value. The type of number pointed
to by the valuePtr is specified by type. If type is a floating point
type and the value represents one of the infinities or NaN, the
well-defined CFNumber for that value is returned. If either of valuePtr
or type is an invalid value, the result it undefined. */
EXTERN_API_C( CFNumberRef )
CFNumberCreate                  (CFAllocatorRef         allocator,
                                 CFNumberType           theType,
                                 const void *           valuePtr);


/* Returns the storage format of the CFNumber's value.  Note that
this is not necessarily the type provided in CFNumberCreate(). */
EXTERN_API_C( CFNumberType )
CFNumberGetType                 (CFNumberRef            number);


/* Returns the size in bytes of the type of the number. */
EXTERN_API_C( CFIndex )
CFNumberGetByteSize             (CFNumberRef            number);


/* Returns TRUE if the type of the CFNumber's value is one of the defined floating point types. */
EXTERN_API_C( Boolean )
CFNumberIsFloatType             (CFNumberRef            number);


/* Copies the CFNumber's value into the space pointed to by
valuePtr, as the specified type. If conversion needs to take
place, the conversion rules follow human expectation and not
C's promotion and truncation rules.  If the conversion is
lossy, or the value is out of range, FALSE is returned. Best
attempt at conversion will still be in *valuePtr.  */
EXTERN_API_C( Boolean )
CFNumberGetValue                (CFNumberRef            number,
                                 CFNumberType           numType,
                                 void *                 valuePtr);


/* Compares the two CFNumber instances. If conversion of the
types of the values is needed, the conversion and comparison
follow human expectations and not C's promotion and comparison
rules. Negative zero compares less than positive zero.
Positive infinity compares greater than everything except
itself, to which it compares equal. Negative infinity compares
less than everything except itself, to which it compares equal.
Unlike standard practice, if both numbers are NaN, then they
compare equal; if only one of the numbers is NaN, then the NaN
compares greater than the other number if it is negative, and
smaller than the other number if it is positive. [Note that in
CFEqual() with two CFNumbers, if either or both of the numbers
is NaN, FALSE is returned.] */
EXTERN_API_C( CFComparisonResult )
CFNumberCompare                 (CFNumberRef            number,
                                 CFNumberRef            otherNumber,
                                 void *                 context);


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __CFNUMBER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__CFNUMBER__RESTORE_PACKED_ENUMS)
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

#endif /* __CFNUMBER__ */

