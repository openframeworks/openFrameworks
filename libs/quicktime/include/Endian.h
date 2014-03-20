/*
     File:       Endian.h
 
     Contains:   QuickTime Interfaces
 
     Version:    Technology: QuickTime 3.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1997-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ENDIAN__
#define __ENDIAN__

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

/*
    This file provides Endian Flipping routines for dealing with converting data
    between Big-Endian and Little-Endian machines.  These routines are useful
    when writing code to compile for both Big and Little Endian machines and  
    which must handle other endian number formats, such as reading or writing 
    to a file or network packet.
    
    These routines are named as follows:
    
        Endian<U><W>_<S>to<D>

    where
        <U> is whether the integer is signed ('S') or unsigned ('U')
        <W> is integer bit width: 16, 32, or 64 
        <S> is the source endian format: 'B' for big, 'L' for little, or 'N' for native
        <D> is the destination endian format: 'B' for big, 'L' for little, or 'N' for native
    
    For example, to convert a Big Endian 32-bit unsigned integer to the current native format use:
        
        long i = EndianU32_BtoN(data);
        
    This file is set up so that the function macro to nothing when the target runtime already
    is the desired format (e.g. on Big Endian machines, EndianU32_BtoN() macros away).
            
    If long long's are not supported, you cannot get 64-bit quantities as a single value.
    The macros are not defined in that case.
    
    
    
                                <<< W A R N I N G >>>
    
    It is very important not to put any autoincrements inside the macros.  This 
    will produce erroneous results because each time the address is accessed in the macro, 
    the increment occurs.
    
 */
/*
   Note: These functions are currently not implemented in any library
         and are only listed here as function prototypes to document the macros
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( SInt16 )
EndianS16_BtoN                  (SInt16                 value);

EXTERN_API_C( SInt16 )
EndianS16_NtoB                  (SInt16                 value);

EXTERN_API_C( SInt16 )
EndianS16_LtoN                  (SInt16                 value);

EXTERN_API_C( SInt16 )
EndianS16_NtoL                  (SInt16                 value);

EXTERN_API_C( SInt16 )
EndianS16_LtoB                  (SInt16                 value);

EXTERN_API_C( SInt16 )
EndianS16_BtoL                  (SInt16                 value);

EXTERN_API_C( UInt16 )
EndianU16_BtoN                  (UInt16                 value);

EXTERN_API_C( UInt16 )
EndianU16_NtoB                  (UInt16                 value);

EXTERN_API_C( UInt16 )
EndianU16_LtoN                  (UInt16                 value);

EXTERN_API_C( UInt16 )
EndianU16_NtoL                  (UInt16                 value);

EXTERN_API_C( UInt16 )
EndianU16_LtoB                  (UInt16                 value);

EXTERN_API_C( UInt16 )
EndianU16_BtoL                  (UInt16                 value);

EXTERN_API_C( SInt32 )
EndianS32_BtoN                  (SInt32                 value);

EXTERN_API_C( SInt32 )
EndianS32_NtoB                  (SInt32                 value);

EXTERN_API_C( SInt32 )
EndianS32_LtoN                  (SInt32                 value);

EXTERN_API_C( SInt32 )
EndianS32_NtoL                  (SInt32                 value);

EXTERN_API_C( SInt32 )
EndianS32_LtoB                  (SInt32                 value);

EXTERN_API_C( SInt32 )
EndianS32_BtoL                  (SInt32                 value);

EXTERN_API_C( UInt32 )
EndianU32_BtoN                  (UInt32                 value);

EXTERN_API_C( UInt32 )
EndianU32_NtoB                  (UInt32                 value);

EXTERN_API_C( UInt32 )
EndianU32_LtoN                  (UInt32                 value);

EXTERN_API_C( UInt32 )
EndianU32_NtoL                  (UInt32                 value);

EXTERN_API_C( UInt32 )
EndianU32_LtoB                  (UInt32                 value);

EXTERN_API_C( UInt32 )
EndianU32_BtoL                  (UInt32                 value);

#endif  /* CALL_NOT_IN_CARBON */

#if !TYPE_LONGLONG
/*
   Note: If these Int64 functions ever were implemented in a library,
         we would need two libraries, one for compilers that
         support long long and one for other compilers.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( SInt64 )
EndianS64_BtoN                  (SInt64                 value);

EXTERN_API_C( SInt64 )
EndianS64_NtoB                  (SInt64                 value);

EXTERN_API_C( SInt64 )
EndianS64_LtoN                  (SInt64                 value);

EXTERN_API_C( SInt64 )
EndianS64_NtoL                  (SInt64                 value);

EXTERN_API_C( SInt64 )
EndianS64_LtoB                  (SInt64                 value);

EXTERN_API_C( SInt64 )
EndianS64_BtoL                  (SInt64                 value);

EXTERN_API_C( UInt64 )
EndianU64_BtoN                  (UInt64                 value);

EXTERN_API_C( UInt64 )
EndianU64_NtoB                  (UInt64                 value);

EXTERN_API_C( UInt64 )
EndianU64_LtoN                  (UInt64                 value);

EXTERN_API_C( UInt64 )
EndianU64_NtoL                  (UInt64                 value);

EXTERN_API_C( UInt64 )
EndianU64_LtoB                  (UInt64                 value);

EXTERN_API_C( UInt64 )
EndianU64_BtoL                  (UInt64                 value);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !TYPE_LONGLONG */

/*
   These types are used for structures that contain data that is
   always in BigEndian format.  This extra typing prevents little
   endian code from directly changing the data, thus saving much
   time in the debugger.
*/

#if TARGET_RT_LITTLE_ENDIAN

struct BigEndianLong {
    long                            bigEndianValue;
};
typedef struct BigEndianLong            BigEndianLong;

struct BigEndianUnsignedLong {
    unsigned long                   bigEndianValue;
};
typedef struct BigEndianUnsignedLong    BigEndianUnsignedLong;

struct BigEndianShort {
    short                           bigEndianValue;
};
typedef struct BigEndianShort           BigEndianShort;

struct BigEndianUnsignedShort {
    unsigned short                  bigEndianValue;
};
typedef struct BigEndianUnsignedShort   BigEndianUnsignedShort;

struct BigEndianFixed {
    Fixed                           bigEndianValue;
};
typedef struct BigEndianFixed           BigEndianFixed;

struct BigEndianUnsignedFixed {
    UnsignedFixed                   bigEndianValue;
};
typedef struct BigEndianUnsignedFixed   BigEndianUnsignedFixed;

struct BigEndianOSType {
    OSType                          bigEndianValue;
};
typedef struct BigEndianOSType          BigEndianOSType;
#else

typedef long                            BigEndianLong;
typedef unsigned long                   BigEndianUnsignedLong;
typedef short                           BigEndianShort;
typedef unsigned short                  BigEndianUnsignedShort;
typedef Fixed                           BigEndianFixed;
typedef UnsignedFixed                   BigEndianUnsignedFixed;
typedef OSType                          BigEndianOSType;
#endif  /* TARGET_RT_LITTLE_ENDIAN */


/*
    Macro away no-op functions
*/
#if TARGET_RT_BIG_ENDIAN
 #define EndianS16_BtoN(value)               (value)
    #define EndianS16_NtoB(value)               (value)
    #define EndianU16_BtoN(value)               (value)
    #define EndianU16_NtoB(value)               (value)
    #define EndianS32_BtoN(value)               (value)
    #define EndianS32_NtoB(value)               (value)
    #define EndianU32_BtoN(value)               (value)
    #define EndianU32_NtoB(value)               (value)
    #define EndianS64_BtoN(value)               (value)
    #define EndianS64_NtoB(value)               (value)
    #define EndianU64_BtoN(value)               (value)
    #define EndianU64_NtoB(value)               (value)
#else
  #define EndianS16_LtoN(value)               (value)
    #define EndianS16_NtoL(value)               (value)
    #define EndianU16_LtoN(value)               (value)
    #define EndianU16_NtoL(value)               (value)
    #define EndianS32_LtoN(value)               (value)
    #define EndianS32_NtoL(value)               (value)
    #define EndianU32_LtoN(value)               (value)
    #define EndianU32_NtoL(value)               (value)
    #define EndianS64_LtoN(value)               (value)
    #define EndianS64_NtoL(value)               (value)
    #define EndianU64_LtoN(value)               (value)
    #define EndianU64_NtoL(value)               (value)
#endif



/*
    Map native to actual
*/
#if TARGET_RT_BIG_ENDIAN
   #define EndianS16_LtoN(value)               EndianS16_LtoB(value)
  #define EndianS16_NtoL(value)               EndianS16_BtoL(value)
  #define EndianU16_LtoN(value)               EndianU16_LtoB(value)
  #define EndianU16_NtoL(value)               EndianU16_BtoL(value)
  #define EndianS32_LtoN(value)               EndianS32_LtoB(value)
  #define EndianS32_NtoL(value)               EndianS32_BtoL(value)
  #define EndianU32_LtoN(value)               EndianU32_LtoB(value)
  #define EndianU32_NtoL(value)               EndianU32_BtoL(value)
  #define EndianS64_LtoN(value)               EndianS64_LtoB(value)
  #define EndianS64_NtoL(value)               EndianS64_BtoL(value)
  #define EndianU64_LtoN(value)               EndianU64_LtoB(value)
  #define EndianU64_NtoL(value)               EndianU64_BtoL(value)
#else
    #define EndianS16_BtoN(value)               EndianS16_BtoL(value)
  #define EndianS16_NtoB(value)               EndianS16_LtoB(value)
  #define EndianU16_BtoN(value)               EndianU16_BtoL(value)
  #define EndianU16_NtoB(value)               EndianU16_LtoB(value)
  #define EndianS32_BtoN(value)               EndianS32_BtoL(value)
  #define EndianS32_NtoB(value)               EndianS32_LtoB(value)
  #define EndianU32_BtoN(value)               EndianU32_BtoL(value)
  #define EndianU32_NtoB(value)               EndianU32_LtoB(value)
  #define EndianS64_BtoN(value)               EndianS64_BtoL(value)
  #define EndianS64_NtoB(value)               EndianS64_LtoB(value)
  #define EndianU64_BtoN(value)               EndianU64_BtoL(value)
  #define EndianU64_NtoB(value)               EndianU64_LtoB(value)
#endif



/*
    Implement ~LtoB and ~BtoL
*/
#define EndianS16_LtoB(value)              ((SInt16)Endian16_Swap(value))
#define EndianS16_BtoL(value)                ((SInt16)Endian16_Swap(value))
#define EndianU16_LtoB(value)                ((UInt16)Endian16_Swap(value))
#define EndianU16_BtoL(value)                ((UInt16)Endian16_Swap(value))
#define EndianS32_LtoB(value)                ((SInt32)Endian32_Swap(value))
#define EndianS32_BtoL(value)                ((SInt32)Endian32_Swap(value))
#define EndianU32_LtoB(value)                ((UInt32)Endian32_Swap(value))
#define EndianU32_BtoL(value)                ((UInt32)Endian32_Swap(value))
#define EndianS64_LtoB(value)                ((SInt64)Endian64_Swap((UInt64)value))
#define EndianS64_BtoL(value)                ((SInt64)Endian64_Swap((UInt64)value))
#define EndianU64_LtoB(value)                ((UInt64)Endian64_Swap(value))
#define EndianU64_BtoL(value)                ((UInt64)Endian64_Swap(value))




/*
    Implement low level ~_Swap functions.
  
       extern UInt16 Endian16_Swap(UInt16 value);
     extern UInt32 Endian32_Swap(UInt32 value);
     extern UInt64 Endian64_Swap(UInt64 value);
     
   Note: Depending on the processor, you might want to implement
        these as function calls instead of macros.
   
*/
#if TARGET_CPU_68K && TARGET_OS_MAC
    #pragma parameter __D0 Endian16_Swap(__D0)
 pascal UInt16 Endian16_Swap(UInt16 value)
      = { 0xE158 };
  
   #pragma parameter __D0 Endian32_Swap (__D0)
    pascal UInt32 Endian32_Swap(UInt32 value)
      = { 0xE158, 0x4840, 0xE158 };
#else
    #define Endian16_Swap(value)                 \
         (((((UInt16)value)<<8) & 0xFF00)   | \
          ((((UInt16)value)>>8) & 0x00FF))
  
   #define Endian32_Swap(value)                     \
         (((((UInt32)value)<<24) & 0xFF000000)  | \
          ((((UInt32)value)<< 8) & 0x00FF0000)  | \
          ((((UInt32)value)>> 8) & 0x0000FF00)  | \
          ((((UInt32)value)>>24) & 0x000000FF))
#endif


#if TYPE_LONGLONG
 #if TARGET_OS_WIN32
        /* the inline macros crash MSDEV's optimizer on Windows. */
        extern UInt64 Endian64_Swap(UInt64 value);
 #elif defined(__MWERKS__) && (__MWERKS__ < 0x1800)
     /* older Metrowerks compilers errored on LL suffix */
      #define Endian64_Swap(value)                             \
             (((((UInt64)value)<<56) & 0xFF00000000000000)  | \
              ((((UInt64)value)<<40) & 0x00FF000000000000)  | \
              ((((UInt64)value)<<24) & 0x0000FF0000000000)  | \
              ((((UInt64)value)<< 8) & 0x000000FF00000000)  | \
              ((((UInt64)value)>> 8) & 0x00000000FF000000)  | \
              ((((UInt64)value)>>24) & 0x0000000000FF0000)  | \
              ((((UInt64)value)>>40) & 0x000000000000FF00)  | \
              ((((UInt64)value)>>56) & 0x00000000000000FF))
 #else
      #define Endian64_Swap(value)                             \
             (((((UInt64)value)<<56) & 0xFF00000000000000ULL)  | \
               ((((UInt64)value)<<40) & 0x00FF000000000000ULL)  | \
               ((((UInt64)value)<<24) & 0x0000FF0000000000ULL)  | \
               ((((UInt64)value)<< 8) & 0x000000FF00000000ULL)  | \
               ((((UInt64)value)>> 8) & 0x00000000FF000000ULL)  | \
               ((((UInt64)value)>>24) & 0x0000000000FF0000ULL)  | \
               ((((UInt64)value)>>40) & 0x000000000000FF00ULL)  | \
               ((((UInt64)value)>>56) & 0x00000000000000FFULL))
  #endif
#else
   /* 
        Note: When using C compilers that don't support "long long",
             Endian64_Swap must be implemented as glue. 
  */
 #ifdef __cplusplus
     inline static UInt64 Endian64_Swap(UInt64 value)
       {
          UInt64 temp;
           ((UnsignedWide*)&temp)->lo = Endian32_Swap(((UnsignedWide*)&value)->hi);
           ((UnsignedWide*)&temp)->hi = Endian32_Swap(((UnsignedWide*)&value)->lo);
           return temp;
       }
  #else
      extern UInt64 Endian64_Swap(UInt64 value);
 #endif
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

#endif /* __ENDIAN__ */

