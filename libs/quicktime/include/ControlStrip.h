/*
     File:       ControlStrip.h
 
     Contains:   Control Strip (for Powerbooks and Duos) Interfaces.
 
     Version:    Technology: ControlStrip 1.4
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1992-2001 by Apple Computer, Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CONTROLSTRIP__
#define __CONTROLSTRIP__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
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

/*********************************************************************************************

    messages passed to the modules

*********************************************************************************************/
enum {
    sdevInitModule              = 0,                            /* initialize the module*/
    sdevCloseModule             = 1,                            /* clean up before being closed*/
    sdevFeatures                = 2,                            /* return feature bits*/
    sdevGetDisplayWidth         = 3,                            /* returns the width of the module's display*/
    sdevPeriodicTickle          = 4,                            /* periodic tickle when nothing else is happening*/
    sdevDrawStatus              = 5,                            /* update the interface in the Control Strip*/
    sdevMouseClick              = 6,                            /* user clicked on the module's display area in the Control Strip*/
    sdevSaveSettings            = 7,                            /* saved any changed settings in module's preferences file*/
    sdevShowBalloonHelp         = 8                             /* puts up a help balloon, if the module has one to display*/
};

/*********************************************************************************************

    Features supported by the module.  If a bit is set, it means that feature is supported.
    All undefined bits are reserved for future use by Apple, and should be set to zero.

*********************************************************************************************/
enum {
    sdevWantMouseClicks         = 0,                            /* notify the module of mouseDown events*/
    sdevDontAutoTrack           = 1,                            /* call the module to do mouse tracking*/
    sdevHasCustomHelp           = 2,                            /* module provides its own help messages*/
    sdevKeepModuleLocked        = 3                             /* module needs to be locked in the heap*/
};

/*********************************************************************************************

    Result values returned by the sdevPeriodicTickle and sdevIconMouseClick selectors.
    If a bit is set, the module can request that a specific function is performed by
    the Control Strip.  A result of zero will do nothing.  All undefined bits are reserved
    for future use by Apple, and should be set to zero.

*********************************************************************************************/
enum {
    sdevResizeDisplay           = 0,                            /* resize the module's display*/
    sdevNeedToSave              = 1,                            /* need to save changed settings, when convenient*/
    sdevHelpStateChange         = 2,                            /* need to update the help message because of a state change*/
    sdevCloseNow                = 3                             /* close a module because it doesn't want to stay around*/
};


/*********************************************************************************************

    miscellaneous

*********************************************************************************************/
enum {
    sdevFileType                = FOUR_CHAR_CODE('sdev')        /* module's file type*/
};

enum {
    sdevMenuItemMark            = 0xA5                          /* 'o': 'checkmark' to use in popup menus*/
};


/*  direction values for SBDrawBarGraph*/

enum {
    BarGraphSlopeLeft           = -1,                           /* max end of sloping bar graph is on the left*/
    BarGraphFlatRight           = 0,                            /* max end of flat bar graph is on the right*/
    BarGraphSlopeRight          = 1                             /* max end of sloping bar graph is on the right*/
};

/*********************************************************************************************

    utility routines to provide standard interface elements and support for common functions

*********************************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API( Boolean )
SBIsControlStripVisible         (void)                                                      TWOWORDINLINE(0x7000, 0xAAF2);

EXTERN_API( void )
SBShowHideControlStrip          (Boolean                showIt)                             THREEWORDINLINE(0x303C, 0x0101, 0xAAF2);

EXTERN_API( Boolean )
SBSafeToAccessStartupDisk       (void)                                                      TWOWORDINLINE(0x7002, 0xAAF2);

EXTERN_API( short )
SBOpenModuleResourceFile        (OSType                 fileCreator)                        THREEWORDINLINE(0x303C, 0x0203, 0xAAF2);

EXTERN_API( OSErr )
SBLoadPreferences               (ConstStr255Param       prefsResourceName,
                                 Handle *               preferences)                        THREEWORDINLINE(0x303C, 0x0404, 0xAAF2);

EXTERN_API( OSErr )
SBSavePreferences               (ConstStr255Param       prefsResourceName,
                                 Handle                 preferences)                        THREEWORDINLINE(0x303C, 0x0405, 0xAAF2);

EXTERN_API( void )
SBGetDetachedIndString          (StringPtr              theString,
                                 Handle                 stringList,
                                 short                  whichString)                        THREEWORDINLINE(0x303C, 0x0506, 0xAAF2);

EXTERN_API( OSErr )
SBGetDetachIconSuite            (Handle *               theIconSuite,
                                 short                  theResID,
                                 unsigned long          selector)                           THREEWORDINLINE(0x303C, 0x0507, 0xAAF2);

EXTERN_API( short )
SBTrackPopupMenu                (const Rect *           moduleRect,
                                 MenuHandle             theMenu)                            THREEWORDINLINE(0x303C, 0x0408, 0xAAF2);

EXTERN_API( short )
SBTrackSlider                   (const Rect *           moduleRect,
                                 short                  ticksOnSlider,
                                 short                  initialValue)                       THREEWORDINLINE(0x303C, 0x0409, 0xAAF2);

EXTERN_API( OSErr )
SBShowHelpString                (const Rect *           moduleRect,
                                 StringPtr              helpString)                         THREEWORDINLINE(0x303C, 0x040A, 0xAAF2);

EXTERN_API( short )
SBGetBarGraphWidth              (short                  barCount)                           THREEWORDINLINE(0x303C, 0x010B, 0xAAF2);

EXTERN_API( void )
SBDrawBarGraph                  (short                  level,
                                 short                  barCount,
                                 short                  direction,
                                 Point                  barGraphTopLeft)                    THREEWORDINLINE(0x303C, 0x050C, 0xAAF2);

EXTERN_API( void )
SBModalDialogInContext          (ModalFilterUPP         filterProc,
                                 short *                itemHit)                            THREEWORDINLINE(0x303C, 0x040D, 0xAAF2);

/* The following routines are available in Control Strip 1.2 and later. */
EXTERN_API( OSErr )
SBGetControlStripFontID         (short *                fontID)                             THREEWORDINLINE(0x303C, 0x020E, 0xAAF2);

EXTERN_API( OSErr )
SBSetControlStripFontID         (short                  fontID)                             THREEWORDINLINE(0x303C, 0x010F, 0xAAF2);

EXTERN_API( OSErr )
SBGetControlStripFontSize       (short *                fontSize)                           THREEWORDINLINE(0x303C, 0x0210, 0xAAF2);

EXTERN_API( OSErr )
SBSetControlStripFontSize       (short                  fontSize)                           THREEWORDINLINE(0x303C, 0x0111, 0xAAF2);

EXTERN_API( OSErr )
SBGetShowHideHotKey             (short *                modifiers,
                                 unsigned char *        keyCode)                            THREEWORDINLINE(0x303C, 0x0412, 0xAAF2);

EXTERN_API( OSErr )
SBSetShowHideHotKey             (short                  modifiers,
                                 unsigned char          keyCode)                            THREEWORDINLINE(0x303C, 0x0213, 0xAAF2);

EXTERN_API( OSErr )
SBIsShowHideHotKeyEnabled       (Boolean *              enabled)                            THREEWORDINLINE(0x303C, 0x0214, 0xAAF2);

EXTERN_API( OSErr )
SBEnableShowHideHotKey          (Boolean                enabled)                            THREEWORDINLINE(0x303C, 0x0115, 0xAAF2);

/* The following routines are available in Control Strip 1.4 and later. */
EXTERN_API( short )
SBHitTrackSlider                (const Rect *           moduleRect,
                                 short                  ticksOnSlider,
                                 short                  initialValue,
                                 Boolean *              hit)                                THREEWORDINLINE(0x303C, 0x0616, 0xAAF2);


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

#endif /* __CONTROLSTRIP__ */

