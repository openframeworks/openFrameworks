/*
     File:       CMConversions.h
 
     Contains:   ColorSync Conversion Component API
 
     Version:    Technology: ColorSync 2.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CMCONVERSIONS__
#define __CMCONVERSIONS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __CMAPPLICATION__
#include <CMApplication.h>
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

enum {
    CMConversionInterfaceVersion = 1
};

/* Component function selectors */
enum {
    kCMXYZToLab                 = 0,
    kCMLabToXYZ                 = 1,
    kCMXYZToLuv                 = 2,
    kCMLuvToXYZ                 = 3,
    kCMXYZToYxy                 = 4,
    kCMYxyToXYZ                 = 5,
    kCMRGBToHLS                 = 6,
    kCMHLSToRGB                 = 7,
    kCMRGBToHSV                 = 8,
    kCMHSVToRGB                 = 9,
    kCMRGBToGRAY                = 10,
    kCMXYZToFixedXYZ            = 11,
    kCMFixedXYZToXYZ            = 12
};

#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
CMXYZToLab                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 const CMXYZColor *     white,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x0010, 0x0000, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMLabToXYZ                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 const CMXYZColor *     white,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x0010, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMXYZToLuv                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 const CMXYZColor *     white,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x0010, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMLuvToXYZ                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 const CMXYZColor *     white,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x0010, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMXYZToYxy                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMYxyToXYZ                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMRGBToHLS                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMHLSToRGB                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMRGBToHSV                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0008, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMHSVToRGB                      (ComponentInstance      ci,
                                 const CMColor *        src,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0009, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMRGBToGray                     (ComponentInstance      ci,
                                 const CMColor *        src,
                                 CMColor *              dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMXYZToFixedXYZ                 (ComponentInstance      ci,
                                 const CMXYZColor *     src,
                                 CMFixedXYZColor *      dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
CMFixedXYZToXYZ                 (ComponentInstance      ci,
                                 const CMFixedXYZColor * src,
                                 CMXYZColor *           dst,
                                 unsigned long          count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x000C, 0x7000, 0xA82A);


#endif  /* CALL_NOT_IN_CARBON */


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

#endif /* __CMCONVERSIONS__ */

