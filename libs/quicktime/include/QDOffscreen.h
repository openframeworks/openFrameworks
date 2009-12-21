/*
     File:       QDOffscreen.h
 
     Contains:   Quickdraw Offscreen GWorld Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QDOFFSCREEN__
#define __QDOFFSCREEN__

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
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
    pixPurgeBit                 = 0,
    noNewDeviceBit              = 1,
    useTempMemBit               = 2,
    keepLocalBit                = 3,
    useDistantHdwrMemBit        = 4,
    useLocalHdwrMemBit          = 5,
    pixelsPurgeableBit          = 6,
    pixelsLockedBit             = 7,
    mapPixBit                   = 16,
    newDepthBit                 = 17,
    alignPixBit                 = 18,
    newRowBytesBit              = 19,
    reallocPixBit               = 20,
    clipPixBit                  = 28,
    stretchPixBit               = 29,
    ditherPixBit                = 30,
    gwFlagErrBit                = 31
};

enum {
    pixPurge                    = 1L << pixPurgeBit,
    noNewDevice                 = 1L << noNewDeviceBit,
    useTempMem                  = 1L << useTempMemBit,
    keepLocal                   = 1L << keepLocalBit,
    useDistantHdwrMem           = 1L << useDistantHdwrMemBit,
    useLocalHdwrMem             = 1L << useLocalHdwrMemBit,
    pixelsPurgeable             = 1L << pixelsPurgeableBit,
    pixelsLocked                = 1L << pixelsLockedBit,
    kAllocDirectDrawSurface     = 1L << 14,
    mapPix                      = 1L << mapPixBit,
    newDepth                    = 1L << newDepthBit,
    alignPix                    = 1L << alignPixBit,
    newRowBytes                 = 1L << newRowBytesBit,
    reallocPix                  = 1L << reallocPixBit,
    clipPix                     = 1L << clipPixBit,
    stretchPix                  = 1L << stretchPixBit,
    ditherPix                   = 1L << ditherPixBit,
    gwFlagErr                   = 1L << gwFlagErrBit
};

typedef unsigned long                   GWorldFlags;
/* Type definition of a GWorldPtr */
typedef CGrafPtr                        GWorldPtr;
EXTERN_API( QDErr )
NewGWorld                       (GWorldPtr *            offscreenGWorld,
                                 short                  PixelDepth,
                                 const Rect *           boundsRect,
                                 CTabHandle             cTable, /* can be NULL */
                                 GDHandle               aGDevice, /* can be NULL */
                                 GWorldFlags            flags)                              FOURWORDINLINE(0x203C, 0x0016, 0x0000, 0xAB1D);

/* GDevice attribute bits for Carbon and QuickTime 3.0*/
enum {
    deviceIsIndirect            = (1L << 0),
    deviceNeedsLock             = (1L << 1),
    deviceIsStatic              = (1L << 2),
    deviceIsExternalBuffer      = (1L << 3),
    deviceIsDDSurface           = (1L << 4),
    deviceIsDCISurface          = (1L << 5),
    deviceIsGDISurface          = (1L << 6),
    deviceIsAScreen             = (1L << 7),
    deviceIsOverlaySurface      = (1L << 8)
};

#if TARGET_OS_WIN32
#if CALL_NOT_IN_CARBON
EXTERN_API( void *)
GetGDeviceSurface               (GDHandle               gdh);

EXTERN_API( unsigned long )
GetGDeviceAttributes            (GDHandle               gdh);

/* to allocate non-mac-rgb GWorlds use QTNewGWorld (ImageCompression.h) */
EXTERN_API( QDErr )
NewGWorldFromHBITMAP            (GWorldPtr *            offscreenGWorld,
                                 CTabHandle             cTable,
                                 GDHandle               aGDevice,
                                 GWorldFlags            flags,
                                 void *                 newHBITMAP,
                                 void *                 newHDC);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_WIN32 */

#if CALL_NOT_IN_CARBON
EXTERN_API( QDErr )
NewGWorldFromPtr                (GWorldPtr *            offscreenGWorld,
                                 unsigned long          PixelFormat,
                                 const Rect *           boundsRect,
                                 CTabHandle             cTable,
                                 GDHandle               aGDevice,
                                 GWorldFlags            flags,
                                 Ptr                    newBuffer,
                                 long                   rowBytes);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( Boolean )
LockPixels                      (PixMapHandle           pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x0001, 0xAB1D);

EXTERN_API( void )
UnlockPixels                    (PixMapHandle           pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x0002, 0xAB1D);

EXTERN_API( GWorldFlags )
UpdateGWorld                    (GWorldPtr *            offscreenGWorld,
                                 short                  pixelDepth,
                                 const Rect *           boundsRect,
                                 CTabHandle             cTable,
                                 GDHandle               aGDevice, /* can be NULL */
                                 GWorldFlags            flags)                              FOURWORDINLINE(0x203C, 0x0016, 0x0003, 0xAB1D);

EXTERN_API( void )
DisposeGWorld                   (GWorldPtr              offscreenGWorld)                    FOURWORDINLINE(0x203C, 0x0004, 0x0004, 0xAB1D);

EXTERN_API( void )
GetGWorld                       (CGrafPtr *             port,
                                 GDHandle *             gdh)                                FOURWORDINLINE(0x203C, 0x0008, 0x0005, 0xAB1D);

EXTERN_API( void )
SetGWorld                       (CGrafPtr               port,
                                 GDHandle               gdh)                                FOURWORDINLINE(0x203C, 0x0008, 0x0006, 0xAB1D);

EXTERN_API( void )
CTabChanged                     (CTabHandle             ctab)                               FOURWORDINLINE(0x203C, 0x0004, 0x0007, 0xAB1D);

EXTERN_API( void )
PixPatChanged                   (PixPatHandle           ppat)                               FOURWORDINLINE(0x203C, 0x0004, 0x0008, 0xAB1D);

EXTERN_API( void )
PortChanged                     (GrafPtr                port)                               FOURWORDINLINE(0x203C, 0x0004, 0x0009, 0xAB1D);

EXTERN_API( void )
GDeviceChanged                  (GDHandle               gdh)                                FOURWORDINLINE(0x203C, 0x0004, 0x000A, 0xAB1D);

EXTERN_API( void )
AllowPurgePixels                (PixMapHandle           pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x000B, 0xAB1D);

EXTERN_API( void )
NoPurgePixels                   (PixMapHandle           pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x000C, 0xAB1D);

EXTERN_API( GWorldFlags )
GetPixelsState                  (PixMapHandle           pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x000D, 0xAB1D);

EXTERN_API( void )
SetPixelsState                  (PixMapHandle           pm,
                                 GWorldFlags            state)                              FOURWORDINLINE(0x203C, 0x0008, 0x000E, 0xAB1D);

EXTERN_API( Ptr )
GetPixBaseAddr                  (PixMapHandle           pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x000F, 0xAB1D);

EXTERN_API( long )
GetPixRowBytes                  (PixMapHandle           pm)                                 FOURWORDINLINE(0x203C, 0x0004, 0x0018, 0xAB1D);

EXTERN_API( QDErr )
NewScreenBuffer                 (const Rect *           globalRect,
                                 Boolean                purgeable,
                                 GDHandle *             gdh,
                                 PixMapHandle *         offscreenPixMap)                    FOURWORDINLINE(0x203C, 0x000E, 0x0010, 0xAB1D);

EXTERN_API( void )
DisposeScreenBuffer             (PixMapHandle           offscreenPixMap)                    FOURWORDINLINE(0x203C, 0x0004, 0x0011, 0xAB1D);

EXTERN_API( GDHandle )
GetGWorldDevice                 (GWorldPtr              offscreenGWorld)                    FOURWORDINLINE(0x203C, 0x0004, 0x0012, 0xAB1D);

EXTERN_API( Boolean )
QDDone                          (GrafPtr                port)                               FOURWORDINLINE(0x203C, 0x0004, 0x0013, 0xAB1D);

EXTERN_API( long )
OffscreenVersion                (void)                                                      TWOWORDINLINE(0x7014, 0xAB1D);

EXTERN_API( QDErr )
NewTempScreenBuffer             (const Rect *           globalRect,
                                 Boolean                purgeable,
                                 GDHandle *             gdh,
                                 PixMapHandle *         offscreenPixMap)                    FOURWORDINLINE(0x203C, 0x000E, 0x0015, 0xAB1D);

EXTERN_API( Boolean )
PixMap32Bit                     (PixMapHandle           pmHandle)                           FOURWORDINLINE(0x203C, 0x0004, 0x0016, 0xAB1D);

EXTERN_API( PixMapHandle )
GetGWorldPixMap                 (GWorldPtr              offscreenGWorld)                    FOURWORDINLINE(0x203C, 0x0004, 0x0017, 0xAB1D);


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

#endif /* __QDOFFSCREEN__ */

