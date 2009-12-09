/*
     File:       ToolUtils.h
 
     Contains:   Toolbox Utilities Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TOOLUTILS__
#define __TOOLUTILS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __OSUTILS__
#include <OSUtils.h>
#endif


#ifndef __FIXMATH__
#include <FixMath.h>
#endif

#ifndef __ICONS__
#include <Icons.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __TEXTUTILS__
#include <TextUtils.h>
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
------------------------------------------------------------------------------------
    Note: 
    
    The following routines that used to be in this header file, have moved to
    more appropriate headers.  
    
        FixMath.h:      FixMul
                        FixRatio
                        FixRound
        
        Icons.h:        GetIcon
                        PlotIcon
                        
        Quickdraw.h:    AngleFromSlope
                        DeltaPoint
                        GetCursor
                        GetIndPattern
                        GetPattern
                        GetPicture
                        PackBits
                        ScreenRes
                        ShieldCursor
                        SlopeFromAngle
                        UnpackBits
                        
        TextUtils.h:    Munger
                        GetIndString
                        GetString
                        NewString
                        SetString
------------------------------------------------------------------------------------
*/

EXTERN_API( Boolean )
BitTst                          (const void *           bytePtr,
                                 long                   bitNum)                             ONEWORDINLINE(0xA85D);

EXTERN_API( void )
BitSet                          (void *                 bytePtr,
                                 long                   bitNum)                             ONEWORDINLINE(0xA85E);

EXTERN_API( void )
BitClr                          (void *                 bytePtr,
                                 long                   bitNum)                             ONEWORDINLINE(0xA85F);

EXTERN_API( long )
BitAnd                          (long                   value1,
                                 long                   value2)                             ONEWORDINLINE(0xA858);

EXTERN_API( long )
BitOr                           (long                   value1,
                                 long                   value2)                             ONEWORDINLINE(0xA85B);

EXTERN_API( long )
BitXor                          (long                   value1,
                                 long                   value2)                             ONEWORDINLINE(0xA859);

EXTERN_API( long )
BitNot                          (long                   value)                              ONEWORDINLINE(0xA85A);

EXTERN_API( long )
BitShift                        (long                   value,
                                 short                  count)                              ONEWORDINLINE(0xA85C);

#if TARGET_CPU_68K

struct Int64Bit {
    SInt32                          hiLong;
    UInt32                          loLong;
};
typedef struct Int64Bit                 Int64Bit;
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
LongMul                         (long                   a,
                                 long                   b,
                                 Int64Bit *             result)                             ONEWORDINLINE(0xA867);

#else
    #define LongMul(a, b, result) ((void) WideMultiply((a), (b), (wide*)(result)))
#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_CPU_68K */

#define HiWord(x) ((short)((long)(x) >> 16))
#define LoWord(x) ((short)(x))


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

#endif /* __TOOLUTILS__ */

