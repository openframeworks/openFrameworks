/*
     File:       Palettes.h
 
     Contains:   Palette Manager Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1987-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PALETTES__
#define __PALETTES__

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
    pmCourteous                 = 0,                            /*Record use of color on each device touched.*/
    pmDithered                  = 0x0001,
    pmTolerant                  = 0x0002,                       /*render ciRGB if ciTolerance is exceeded by best match.*/
    pmAnimated                  = 0x0004,                       /*reserve an index on each device touched and render ciRGB.*/
    pmExplicit                  = 0x0008,                       /*no reserve, no render, no record; stuff index into port.*/
    pmWhite                     = 0x0010,
    pmBlack                     = 0x0020,
    pmInhibitG2                 = 0x0100,
    pmInhibitC2                 = 0x0200,
    pmInhibitG4                 = 0x0400,
    pmInhibitC4                 = 0x0800,
    pmInhibitG8                 = 0x1000,
    pmInhibitC8                 = 0x2000,                       /* NSetPalette Update Constants */
    pmNoUpdates                 = 0x8000,                       /*no updates*/
    pmBkUpdates                 = 0xA000,                       /*background updates only*/
    pmFgUpdates                 = 0xC000,                       /*foreground updates only*/
    pmAllUpdates                = 0xE000                        /*all updates*/
};


struct ColorInfo {
    RGBColor                        ciRGB;                      /*true RGB values*/
    short                           ciUsage;                    /*color usage*/
    short                           ciTolerance;                /*tolerance value*/
    short                           ciDataFields[3];            /*private fields*/
};
typedef struct ColorInfo                ColorInfo;
typedef ColorInfo *                     ColorInfoPtr;
typedef ColorInfoPtr *                  ColorInfoHandle;

struct Palette {
    short                           pmEntries;                  /*entries in pmTable*/
    short                           pmDataFields[7];            /*private fields*/
    ColorInfo                       pmInfo[1];
};
typedef struct Palette                  Palette;
typedef Palette *                       PalettePtr;
typedef PalettePtr *                    PaletteHandle;
EXTERN_API( void )
InitPalettes                    (void)                                                      ONEWORDINLINE(0xAA90);

EXTERN_API( PaletteHandle )
NewPalette                      (short                  entries,
                                 CTabHandle             srcColors,
                                 short                  srcUsage,
                                 short                  srcTolerance)                       ONEWORDINLINE(0xAA91);

EXTERN_API( PaletteHandle )
GetNewPalette                   (short                  PaletteID)                          ONEWORDINLINE(0xAA92);

EXTERN_API( void )
DisposePalette                  (PaletteHandle          srcPalette)                         ONEWORDINLINE(0xAA93);

EXTERN_API( void )
ActivatePalette                 (WindowPtr              srcWindow)                          ONEWORDINLINE(0xAA94);

EXTERN_API( void )
SetPalette                      (WindowPtr              dstWindow,
                                 PaletteHandle          srcPalette,
                                 Boolean                cUpdates)                           ONEWORDINLINE(0xAA95);

EXTERN_API( void )
NSetPalette                     (WindowPtr              dstWindow,
                                 PaletteHandle          srcPalette,
                                 short                  nCUpdates)                          ONEWORDINLINE(0xAA95);

EXTERN_API( PaletteHandle )
GetPalette                      (WindowPtr              srcWindow)                          ONEWORDINLINE(0xAA96);

EXTERN_API( void )
CopyPalette                     (PaletteHandle          srcPalette,
                                 PaletteHandle          dstPalette,
                                 short                  srcEntry,
                                 short                  dstEntry,
                                 short                  dstLength)                          ONEWORDINLINE(0xAAA1);

EXTERN_API( void )
PmForeColor                     (short                  dstEntry)                           ONEWORDINLINE(0xAA97);

EXTERN_API( void )
PmBackColor                     (short                  dstEntry)                           ONEWORDINLINE(0xAA98);

EXTERN_API( void )
AnimateEntry                    (WindowPtr              dstWindow,
                                 short                  dstEntry,
                                 const RGBColor *       srcRGB)                             ONEWORDINLINE(0xAA99);

#if TARGET_OS_MAC
    #define MacAnimatePalette AnimatePalette
#endif
EXTERN_API( void )
MacAnimatePalette               (WindowPtr              dstWindow,
                                 CTabHandle             srcCTab,
                                 short                  srcIndex,
                                 short                  dstEntry,
                                 short                  dstLength)                          ONEWORDINLINE(0xAA9A);

EXTERN_API( void )
GetEntryColor                   (PaletteHandle          srcPalette,
                                 short                  srcEntry,
                                 RGBColor *             dstRGB)                             ONEWORDINLINE(0xAA9B);

EXTERN_API( void )
SetEntryColor                   (PaletteHandle          dstPalette,
                                 short                  dstEntry,
                                 const RGBColor *       srcRGB)                             ONEWORDINLINE(0xAA9C);

EXTERN_API( void )
GetEntryUsage                   (PaletteHandle          srcPalette,
                                 short                  srcEntry,
                                 short *                dstUsage,
                                 short *                dstTolerance)                       ONEWORDINLINE(0xAA9D);

EXTERN_API( void )
SetEntryUsage                   (PaletteHandle          dstPalette,
                                 short                  dstEntry,
                                 short                  srcUsage,
                                 short                  srcTolerance)                       ONEWORDINLINE(0xAA9E);

EXTERN_API( void )
CTab2Palette                    (CTabHandle             srcCTab,
                                 PaletteHandle          dstPalette,
                                 short                  srcUsage,
                                 short                  srcTolerance)                       ONEWORDINLINE(0xAA9F);

EXTERN_API( void )
Palette2CTab                    (PaletteHandle          srcPalette,
                                 CTabHandle             dstCTab)                            ONEWORDINLINE(0xAAA0);

EXTERN_API( long )
Entry2Index                     (short                  entry)                              TWOWORDINLINE(0x7000, 0xAAA2);

EXTERN_API( void )
RestoreDeviceClut               (GDHandle               gd)                                 TWOWORDINLINE(0x7002, 0xAAA2);

#if TARGET_OS_MAC
    #define MacResizePalette ResizePalette
#endif
EXTERN_API( void )
MacResizePalette                (PaletteHandle          p,
                                 short                  size)                               TWOWORDINLINE(0x7003, 0xAAA2);

EXTERN_API( void )
SaveFore                        (ColorSpec *            c)                                  THREEWORDINLINE(0x303C, 0x040D, 0xAAA2);

EXTERN_API( void )
SaveBack                        (ColorSpec *            c)                                  THREEWORDINLINE(0x303C, 0x040E, 0xAAA2);

EXTERN_API( void )
RestoreFore                     (const ColorSpec *      c)                                  THREEWORDINLINE(0x303C, 0x040F, 0xAAA2);

EXTERN_API( void )
RestoreBack                     (const ColorSpec *      c)                                  THREEWORDINLINE(0x303C, 0x0410, 0xAAA2);

EXTERN_API( OSErr )
SetDepth                        (GDHandle               gd,
                                 short                  depth,
                                 short                  whichFlags,
                                 short                  flags)                              THREEWORDINLINE(0x303C, 0x0A13, 0xAAA2);

EXTERN_API( short )
HasDepth                        (GDHandle               gd,
                                 short                  depth,
                                 short                  whichFlags,
                                 short                  flags)                              THREEWORDINLINE(0x303C, 0x0A14, 0xAAA2);

EXTERN_API( short )
PMgrVersion                     (void)                                                      TWOWORDINLINE(0x7015, 0xAAA2);

EXTERN_API( void )
SetPaletteUpdates               (PaletteHandle          p,
                                 short                  updates)                            THREEWORDINLINE(0x303C, 0x0616, 0xAAA2);

EXTERN_API( short )
GetPaletteUpdates               (PaletteHandle          p)                                  THREEWORDINLINE(0x303C, 0x0417, 0xAAA2);

EXTERN_API( Boolean )
GetGray                         (GDHandle               device,
                                 const RGBColor *       backGround,
                                 RGBColor *             foreGround)                         THREEWORDINLINE(0x303C, 0x0C19, 0xAAA2);




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

#endif /* __PALETTES__ */

