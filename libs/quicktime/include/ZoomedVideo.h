/*
     File:       ZoomedVideo.h
 
     Contains:   PC Card Family Zoomed Video Driver Interface
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1996-2001 by Apple Computer, Inc.  All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ZOOMEDVIDEO__
#define __ZOOMEDVIDEO__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __DEVICES__
#include <Devices.h>
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

#define kZVDriverName "\p.ZVPort"

/* Control codes */

enum {
    cscZVVideoEnable            = 2,                            /* Used to enable or disable ZV Video*/
    cscZVSetCurrentAnalogValue  = 3,                            /* Used to set brightness, contrast, etc.*/
    cscZVSetInput               = 9,                            /* Set video source to selected input*/
    cscZVSetInputStandard       = 11,                           /* Set the input to NTSC, PAL, or SECAM*/
    cscZVGetMaxSourceRect       = 12                            /* Get Maximum Source Rect*/
};

/* Status codes*/
enum {
    cscZVGetVideoEnable         = 2,                            /* Indicates whether ZV Video is enabled*/
    cscZVGetCurrentAnalogValue  = 3,                            /* Used to get brightness, contrast, etc.*/
    cscZVGetDefaultAnalogValue  = 4,
    cscZVGetVSyncState          = 5,                            /* Used to look for a Vertical Sync on ZV Video*/
    cscZVGetInfo                = 6,                            /* Returns the ZV Information*/
    cscZVGetInputFlags          = 7,                            /* Returns the input flags.*/
    cscZVGetNumberOfInputs      = 8,                            /* Returns the number of video inputs*/
    cscZVGetInput               = 9,                            /* Zero-based input number*/
    cscZVGetInputFormat         = 10                            /* Returns whether input is compsite/s-video*/
};


/*
  -----------------------------------------------------------------
   Additional parameters for csInfoZV control call
    A pointer to ZVInfo is passed in csParam[0] (and csParam[1])
    which must be filled by the driver in response to this call.
*/

typedef UInt32                          ZVFeatures;
enum {
    kZVHasAudio                 = (1 << 0),
    kZVHasTVTuner               = (1 << 1),
    kZVHasContrast              = (1 << 16),
    kZVHasBrightness            = (1 << 17),
    kZVHasSharpness             = (1 << 18),
    kZVHasSaturation            = (1 << 19),
    kZVHasHue                   = (1 << 20)
};



struct ZVInfo {
    ZVFeatures                      features;
    UInt32                          currentFlags;
    Rect                            activeRect;
    Boolean                         isInterlaced;
    SInt8                           filler;
    UInt32                          reserved1;
    UInt32                          reserved2;
};
typedef struct ZVInfo                   ZVInfo;

struct ZVFlagRecord {
    Boolean                         csFlag;
    SInt8                           filler;
};
typedef struct ZVFlagRecord             ZVFlagRecord;
/*
  -----------------------------------------------------------------
   Additional definitions for "AnalogCtlZV" control calls
    A pointer to a ZVSetAnalogControlParam is passed to the driver
    whenever the system needs to adjust one of the analog settings.
*/

typedef UInt16                          ZVAnalogControlSelector;
enum {
    kZVContrast                 = 1,                            /* Range:  0x0 <= no change to image, larger values increase the contrast*/
    kZVBrightness               = 2,                            /* Range:  0x0 <= darkest setting, 0xffff = lightest setting*/
    kZVSharpness                = 3,                            /* Range:  0x0 <= no sharpness filtering, 0xffff <= full sharpness filtering*/
    kZVSaturation               = 4,                            /* Range:  0x0 <= min saturation, 0xffff <= max saturation       */
    kZVHue                      = 5,                            /* Range:  0x0 <= -180. shift in hue, 0xffff <= 179. shift, 0x8000 <=0. shift*/
    kZVBlackLevel               = 6,                            /* Range:  0x0 <= max black, 0xffff <= min black level*/
    kZVWhiteLevel               = 7                             /* Range:  0x0 <= min white, 0xffff <= max white level*/
};


struct ZVAnalogControlRecord {
    ZVAnalogControlSelector         whichControl;
    UInt16                          value;
};
typedef struct ZVAnalogControlRecord    ZVAnalogControlRecord;

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

#endif /* __ZOOMEDVIDEO__ */

