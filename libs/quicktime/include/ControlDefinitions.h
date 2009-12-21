/*
     File:       ControlDefinitions.h
 
     Contains:   Definitions of controls used by Control Mgr
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CONTROLDEFINITIONS__
#define __CONTROLDEFINITIONS__

#ifndef __APPEARANCE__
#include <Appearance.h>
#endif

#ifndef __CONTROLS__
#include <Controls.h>
#endif

#ifndef __LISTS__
#include <Lists.h>
#endif

#ifndef __MACHELP__
#include <MacHelp.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
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

/*------------------------------------------------------------------------------------------------------*/
/*  o Resource Types                                                                                    */
/*------------------------------------------------------------------------------------------------------*/

enum {
    kControlTabListResType      = FOUR_CHAR_CODE('tab#'),       /* used for tab control (Appearance 1.0 and later)*/
    kControlListDescResType     = FOUR_CHAR_CODE('ldes')        /* used for list box control (Appearance 1.0 and later)*/
};

/*--------------------------------------------------------------------------------------*/
/*  o Check Box Values                                                                  */
/*--------------------------------------------------------------------------------------*/
enum {
    kControlCheckBoxUncheckedValue = 0,
    kControlCheckBoxCheckedValue = 1,
    kControlCheckBoxMixedValue  = 2
};

/*--------------------------------------------------------------------------------------*/
/*  o Radio Button Values                                                               */
/*--------------------------------------------------------------------------------------*/
enum {
    kControlRadioButtonUncheckedValue = 0,
    kControlRadioButtonCheckedValue = 1,
    kControlRadioButtonMixedValue = 2
};

/*--------------------------------------------------------------------------------------*/
/*  o Pop-Up Menu Control Constants                                                     */
/*--------------------------------------------------------------------------------------*/
/* Variant codes for the System 7 pop-up menu*/
enum {
    popupFixedWidth             = 1 << 0,
    popupVariableWidth          = 1 << 1,
    popupUseAddResMenu          = 1 << 2,
    popupUseWFont               = 1 << 3
};

/* Menu label styles for the System 7 pop-up menu*/
enum {
    popupTitleBold              = 1 << 8,
    popupTitleItalic            = 1 << 9,
    popupTitleUnderline         = 1 << 10,
    popupTitleOutline           = 1 << 11,
    popupTitleShadow            = 1 << 12,
    popupTitleCondense          = 1 << 13,
    popupTitleExtend            = 1 << 14,
    popupTitleNoStyle           = 1 << 15
};

/* Menu label justifications for the System 7 pop-up menu*/
enum {
    popupTitleLeftJust          = 0x00000000,
    popupTitleCenterJust        = 0x00000001,
    popupTitleRightJust         = 0x000000FF
};

/*------------------------------------------------------------------------------------------------------*/
/*  o PopUp Menu Private Data Structure                                                                 */
/*------------------------------------------------------------------------------------------------------*/
#if !OPAQUE_TOOLBOX_STRUCTS

struct PopupPrivateData {
    MenuHandle                      mHandle;
    SInt16                          mID;
};
typedef struct PopupPrivateData         PopupPrivateData;
typedef PopupPrivateData *              PopupPrivateDataPtr;
typedef PopupPrivateDataPtr *           PopupPrivateDataHandle;
#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

/*------------------------------------------------------------------------------------------------------*/
/*  o Control Definition ID's                                                                           */
/*------------------------------------------------------------------------------------------------------*/
/* Standard System 7 procIDs*/

enum {
    pushButProc                 = 0,
    checkBoxProc                = 1,
    radioButProc                = 2,
    scrollBarProc               = 16,
    popupMenuProc               = 1008
};

/*--------------------------------------------------------------------------------------*/
/*  o Control Part Codes                                                                */
/*--------------------------------------------------------------------------------------*/
enum {
    kControlLabelPart           = 1,
    kControlMenuPart            = 2,
    kControlTrianglePart        = 4,
    kControlEditTextPart        = 5,                            /* Appearance 1.0 and later*/
    kControlPicturePart         = 6,                            /* Appearance 1.0 and later*/
    kControlIconPart            = 7,                            /* Appearance 1.0 and later*/
    kControlClockPart           = 8,                            /* Appearance 1.0 and later*/
    kControlListBoxPart         = 24,                           /* Appearance 1.0 and later*/
    kControlListBoxDoubleClickPart = 25,                        /* Appearance 1.0 and later*/
    kControlImageWellPart       = 26,                           /* Appearance 1.0 and later*/
    kControlRadioGroupPart      = 27,                           /* Appearance 1.0.2 and later*/
    kControlButtonPart          = 10,
    kControlCheckBoxPart        = 11,
    kControlRadioButtonPart     = 11,
    kControlUpButtonPart        = 20,
    kControlDownButtonPart      = 21,
    kControlPageUpPart          = 22,
    kControlPageDownPart        = 23,
    kControlClockHourDayPart    = 9,                            /* Appearance 1.1 and later*/
    kControlClockMinuteMonthPart = 10,                          /* Appearance 1.1 and later*/
    kControlClockSecondYearPart = 11,                           /* Appearance 1.1 and later*/
    kControlClockAMPMPart       = 12,                           /* Appearance 1.1 and later*/
    kControlDataBrowserPart     = 24,                           /* CarbonLib 1.0 and later*/
    kControlDataBrowserDraggedPart = 25                         /* CarbonLib 1.0 and later*/
};



/*------------------------------------------------------------------------------------------------------*/
/*  o Control Types and ID's available only with Appearance 1.0 and later                               */
/*------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
/*  o BEVEL BUTTON INTERFACE (CDEF 2)                                                   */
/*--------------------------------------------------------------------------------------*/
/*  Bevel buttons allow you to control the content type (pict/icon/etc.), the behavior  */
/* (pushbutton/toggle/sticky), and the bevel size. You also have the option of          */
/*  attaching a menu to it. When a menu is present, you can specify which way the       */
/*  popup arrow is facing (down or right).                                              */
/*                                                                                      */
/*  This is all made possible by overloading the Min, Max, and Value parameters for the */
/*  control, as well as adjusting the variant. Here's the breakdown of what goes where: */
/*                                                                                      */
/*  Parameter                   What Goes Here                                          */
/*  -------------------         ----------------------------------------------------    */
/*  Min                         Hi Byte = Behavior, Lo Byte = content type.             */
/*  Max                         ResID for resource-based content types.                 */
/*  Value                       MenuID to attach, 0 = no menu, please.                  */
/*                                                                                      */
/*  The variant is broken down into two halfs. The low 2 bits control the bevel type.   */
/*  Bit 2 controls the popup arrow direction (if a menu is present) and bit 3 controls  */
/*  whether or not to use the control's owning window's font.                           */
/*                                                                                      */
/*  Constants for all you need to put this together are below. The values for behaviors */
/*  are set up so that you can simply add them to the content type and pass them into   */
/*  the Min parameter of NewControl.                                                    */
/*                                                                                      */
/*  An example call:                                                                    */
/*                                                                                      */
/*  control = NewControl( window, &bounds, "\p", true, 0, kContentIconSuiteRes +        */
/*                          kBehaviorToggles, myIconSuiteID, bevelButtonSmallBevelProc, */
/*                          0L );                                                       */
/*                                                                                      */
/*  Attaching a menu:                                                                   */
/*                                                                                      */
/*  control = NewControl( window, &bounds, "\p", true, kMyMenuID, kContentIconSuiteRes, */
/*          myIconSuiteID, bevelButtonSmallBevelProc + kBevelButtonMenuOnRight, 0L );   */
/*                                                                                      */
/*  This will attach menu ID kMyMenuID to the button, with the popup arrow facing right.*/
/*  This also puts the menu up to the right of the button. You can also specify that a  */
/*  menu can have multiple items checked at once by adding kBehaviorMultiValueMenus     */
/*  into the Min parameter. If you do use multivalue menus, the GetBevelButtonMenuValue */
/*  helper function will return the last item chosen from the menu, whether or not it   */
/*  was checked.                                                                        */
/*                                                                                      */
/*  NOTE:   Bevel buttons with menus actually have *two* values. The value of the       */
/*          button (on/off), and the value of the menu. The menu value can be gotten    */
/*          with the GetBevelButtonMenuValue helper function.                           */
/*                                                                                      */
/*  Handle-based Content                                                                */
/*  --------------------                                                                */
/*  You can create your control and then set the content to an existing handle to an    */
/*  icon suite, etc. using the macros below. Please keep in mind that resource-based    */
/*  content is owned by the control, handle-based content is owned by you. The CDEF will*/
/*  not try to dispose of handle-based content. If you are changing the content type of */
/*  the button on the fly, you must make sure that if you are replacing a handle-       */
/*  based content with a resource-based content to properly dispose of the handle,      */
/*  else a memory leak will ensue.                                                      */
/*                                                                                      */
/* Bevel Button Proc IDs */
enum {
    kControlBevelButtonSmallBevelProc = 32,
    kControlBevelButtonNormalBevelProc = 33,
    kControlBevelButtonLargeBevelProc = 34
};

/* Add these variant codes to kBevelButtonSmallBevelProc to change the type of button */
enum {
    kControlBevelButtonSmallBevelVariant = 0,
    kControlBevelButtonNormalBevelVariant = (1 << 0),
    kControlBevelButtonLargeBevelVariant = (1 << 1),
    kControlBevelButtonMenuOnRightVariant = (1 << 2)
};

/* Bevel Thicknesses */

typedef UInt16 ControlBevelThickness;
enum {
    kControlBevelButtonSmallBevel = 0,
    kControlBevelButtonNormalBevel = 1,
    kControlBevelButtonLargeBevel = 2
};

/* Behaviors of bevel buttons. These are set up so you can add  */
/* them together with the content types.                        */
enum {
    kControlBehaviorPushbutton  = 0,
    kControlBehaviorToggles     = 0x0100,
    kControlBehaviorSticky      = 0x0200,
    kControlBehaviorSingleValueMenu = 0,
    kControlBehaviorMultiValueMenu = 0x4000,                    /* only makes sense when a menu is attached.*/
    kControlBehaviorOffsetContents = 0x8000
};

/* Behaviors for 1.0.1 or later */
enum {
    kControlBehaviorCommandMenu = 0x2000                        /* menu holds commands, not choices. Overrides multi-value bit.*/
};


typedef UInt16                          ControlBevelButtonBehavior;
typedef UInt16                          ControlBevelButtonMenuBehavior;
/* Bevel Button Menu Placements */

typedef UInt16 ControlBevelButtonMenuPlacement;
enum {
    kControlBevelButtonMenuOnBottom = 0,
    kControlBevelButtonMenuOnRight = (1 << 2)
};


/* Graphic Alignments */

typedef SInt16 ControlButtonGraphicAlignment;
enum {
    kControlBevelButtonAlignSysDirection = -1,                  /* only left or right*/
    kControlBevelButtonAlignCenter = 0,
    kControlBevelButtonAlignLeft = 1,
    kControlBevelButtonAlignRight = 2,
    kControlBevelButtonAlignTop = 3,
    kControlBevelButtonAlignBottom = 4,
    kControlBevelButtonAlignTopLeft = 5,
    kControlBevelButtonAlignBottomLeft = 6,
    kControlBevelButtonAlignTopRight = 7,
    kControlBevelButtonAlignBottomRight = 8
};

/* Text Alignments */

typedef SInt16 ControlButtonTextAlignment;
enum {
    kControlBevelButtonAlignTextSysDirection = teFlushDefault,
    kControlBevelButtonAlignTextCenter = teCenter,
    kControlBevelButtonAlignTextFlushRight = teFlushRight,
    kControlBevelButtonAlignTextFlushLeft = teFlushLeft
};

/* Text Placements */

typedef SInt16 ControlButtonTextPlacement;
enum {
    kControlBevelButtonPlaceSysDirection = -1,                  /* if graphic on right, then on left*/
    kControlBevelButtonPlaceNormally = 0,
    kControlBevelButtonPlaceToRightOfGraphic = 1,
    kControlBevelButtonPlaceToLeftOfGraphic = 2,
    kControlBevelButtonPlaceBelowGraphic = 3,
    kControlBevelButtonPlaceAboveGraphic = 4
};


/* Data tags supported by the bevel button controls */
enum {
    kControlBevelButtonContentTag = FOUR_CHAR_CODE('cont'),     /* ButtonContentInfo*/
    kControlBevelButtonTransformTag = FOUR_CHAR_CODE('tran'),   /* IconTransformType*/
    kControlBevelButtonTextAlignTag = FOUR_CHAR_CODE('tali'),   /* ButtonTextAlignment*/
    kControlBevelButtonTextOffsetTag = FOUR_CHAR_CODE('toff'),  /* SInt16*/
    kControlBevelButtonGraphicAlignTag = FOUR_CHAR_CODE('gali'), /* ButtonGraphicAlignment*/
    kControlBevelButtonGraphicOffsetTag = FOUR_CHAR_CODE('goff'), /* Point*/
    kControlBevelButtonTextPlaceTag = FOUR_CHAR_CODE('tplc'),   /* ButtonTextPlacement*/
    kControlBevelButtonMenuValueTag = FOUR_CHAR_CODE('mval'),   /* SInt16*/
    kControlBevelButtonMenuHandleTag = FOUR_CHAR_CODE('mhnd'),  /* MenuHandle*/
    kControlBevelButtonCenterPopupGlyphTag = FOUR_CHAR_CODE('pglc') /* Boolean: true = center, false = bottom right*/
};

/* These are tags in 1.0.1 or later */
enum {
    kControlBevelButtonLastMenuTag = FOUR_CHAR_CODE('lmnu'),    /* SInt16: menuID of last menu item selected from*/
    kControlBevelButtonMenuDelayTag = FOUR_CHAR_CODE('mdly')    /* SInt32: ticks to delay before menu appears*/
};

/* tags available with Appearance 1.1 or later */
enum {
                                                                /* Boolean: True = if an icon of the ideal size for*/
                                                                /* the button isn't available, scale a larger or*/
                                                                /* smaller icon to the ideal size. False = don't*/
                                                                /* scale; draw a smaller icon or clip a larger icon.*/
                                                                /* Default is false. Only applies to IconSuites and*/
    kControlBevelButtonScaleIconTag = FOUR_CHAR_CODE('scal')    /* IconRefs.*/
};

/* Helper routines are available only thru the shared library/glue. */
EXTERN_API( OSErr )
GetBevelButtonMenuValue         (ControlHandle          inButton,
                                 SInt16 *               outValue);

EXTERN_API( OSErr )
SetBevelButtonMenuValue         (ControlHandle          inButton,
                                 SInt16                 inValue);

EXTERN_API( OSErr )
GetBevelButtonMenuHandle        (ControlHandle          inButton,
                                 MenuHandle *           outHandle);

EXTERN_API( OSErr )
GetBevelButtonContentInfo       (ControlHandle          inButton,
                                 ControlButtonContentInfoPtr  outContent);

EXTERN_API( OSErr )
SetBevelButtonContentInfo       (ControlHandle          inButton,
                                 ControlButtonContentInfoPtr  inContent);

EXTERN_API( OSErr )
SetBevelButtonTransform         (ControlHandle          inButton,
                                 IconTransformType      transform);

EXTERN_API( OSErr )
SetBevelButtonGraphicAlignment  (ControlHandle          inButton,
                                 ControlButtonGraphicAlignment  inAlign,
                                 SInt16                 inHOffset,
                                 SInt16                 inVOffset);

EXTERN_API( OSErr )
SetBevelButtonTextAlignment     (ControlHandle          inButton,
                                 ControlButtonTextAlignment  inAlign,
                                 SInt16                 inHOffset);

EXTERN_API( OSErr )
SetBevelButtonTextPlacement     (ControlHandle          inButton,
                                 ControlButtonTextPlacement  inWhere);


/*--------------------------------------------------------------------------------------*/
/*  o SLIDER (CDEF 3)                                                                   */
/*--------------------------------------------------------------------------------------*/
/*  There are several variants that control the behavior of the slider control. Any     */
/*  combination of the following three constants can be added to the basic CDEF ID      */
/*  (kSliderProc).                                                                      */
/*                                                                                      */
/*  Variants:                                                                           */
/*                                                                                      */
/*      kSliderLiveFeedback     Slider does not use "ghosted" indicator when tracking.  */
/*                              ActionProc is called (set via SetControlAction) as the  */
/*                              indicator is dragged. The value is updated so that the  */
/*                              actionproc can adjust some other property based on the  */
/*                              value each time the action proc is called. If no action */
/*                              proc is installed, it reverts to the ghost indicator.   */
/*                                                                                      */
/*      kSliderHasTickMarks     Slider is drawn with 'tick marks'. The control          */
/*                              rectangle must be large enough to accomidate the tick   */
/*                              marks.                                                  */
/*                                                                                      */
/*      kSliderReverseDirection Slider thumb points in opposite direction than normal.  */
/*                              If the slider is vertical, the thumb will point to the  */
/*                              left, if the slider is horizontal, the thumb will point */
/*                              upwards.                                                */
/*                                                                                      */
/*      kSliderNonDirectional   This option overrides the kSliderReverseDirection and   */
/*                              kSliderHasTickMarks variants. It creates an indicator   */
/*                              which is rectangular and doesn't point in any direction */
/*                              like the normal indicator does.                         */
/* Slider proc ID and variants */
enum {
    kControlSliderProc          = 48,
    kControlSliderLiveFeedback  = (1 << 0),
    kControlSliderHasTickMarks  = (1 << 1),
    kControlSliderReverseDirection = (1 << 2),
    kControlSliderNonDirectional = (1 << 3)
};

/*--------------------------------------------------------------------------------------*/
/*  o DISCLOSURE TRIANGLE (CDEF 4)                                                      */
/*--------------------------------------------------------------------------------------*/
/*  This control can be used as either left or right facing. It can also handle its own */
/*  tracking if you wish. This means that when the 'autotoggle' variant is used, if the */
/*  user clicks the control, it's state will change automatically from open to closed   */
/*  and vice-versa depending on its initial state. After a successful call to Track-    */
/*  Control, you can just check the current value to see what state it was switched to. */
/* Triangle proc IDs */
enum {
    kControlTriangleProc        = 64,
    kControlTriangleLeftFacingProc = 65,
    kControlTriangleAutoToggleProc = 66,
    kControlTriangleLeftFacingAutoToggleProc = 67
};

/* Tagged data supported by disclosure triangles */
enum {
    kControlTriangleLastValueTag = FOUR_CHAR_CODE('last')       /* SInt16*/
};

/* Helper routines are available only thru the shared library/glue. */
EXTERN_API( OSErr )
SetDisclosureTriangleLastValue  (ControlHandle          inTabControl,
                                 SInt16                 inValue);

/*--------------------------------------------------------------------------------------*/
/*  o PROGRESS INDICATOR (CDEF 5)                                                       */
/*--------------------------------------------------------------------------------------*/
/*  This CDEF implements both determinate and indeterminate progress bars. To switch,   */
/*  just use SetControlData to set the indeterminate flag to make it indeterminate call */
/*  IdleControls to step thru the animation. IdleControls should be called at least     */
/*  once during your event loop.                                                        */
/*                                                                                      */
/* Progress Bar proc IDs */
enum {
    kControlProgressBarProc     = 80
};

/* Tagged data supported by progress bars */
enum {
    kControlProgressBarIndeterminateTag = FOUR_CHAR_CODE('inde') /* Boolean*/
};

/*--------------------------------------------------------------------------------------*/
/*  o LITTLE ARROWS (CDEF 6)                                                            */
/*--------------------------------------------------------------------------------------*/
/*  This control implements the little up and down arrows you'd see in the Memory       */
/*  control panel for adjusting the cache size.                                         */
/* Little Arrows proc IDs */
enum {
    kControlLittleArrowsProc    = 96
};

/*--------------------------------------------------------------------------------------*/
/*  o CHASING ARROWS (CDEF 7)                                                           */
/*--------------------------------------------------------------------------------------*/
/*  To animate this control, make sure to call IdleControls repeatedly.                 */
/*                                                                                      */
/* Chasing Arrows proc IDs */
enum {
    kControlChasingArrowsProc   = 112
};

/*--------------------------------------------------------------------------------------*/
/*  o TABS (CDEF 8)                                                                     */
/*--------------------------------------------------------------------------------------*/
/*  Tabs use an auxiliary resource (tab#) to hold tab information such as the tab name  */
/*  and an icon suite ID for each tab.                                                  */
/*                                                                                      */
/*  The ID of the tab# resource that you wish to associate with a tab control should    */
/*  be passed in as the Value parameter of the control. If you are using GetNewControl, */
/*  then the Value slot in the CNTL resource should have the ID of the 'tab#' resource  */
/*  on creation.                                                                        */
/*                                                                                      */
/*  Passing zero in for the tab# resource tells the control not to read in a tab# res.  */
/*  You can then use SetControlMaximum to add tabs, followed by a call to SetControlData*/
/*  with the kControlTabInfoTag, passing in a pointer to a ControlTabInfoRec. This sets */
/*  the name and optionally an icon for a tab.                                          */
/* Tabs proc IDs */
enum {
    kControlTabLargeProc        = 128,                          /* Large tab size, north facing   */
    kControlTabSmallProc        = 129,                          /* Small tab size, north facing   */
    kControlTabLargeNorthProc   = 128,                          /* Large tab size, north facing   */
    kControlTabSmallNorthProc   = 129,                          /* Small tab size, north facing   */
    kControlTabLargeSouthProc   = 130,                          /* Large tab size, south facing   */
    kControlTabSmallSouthProc   = 131,                          /* Small tab size, south facing   */
    kControlTabLargeEastProc    = 132,                          /* Large tab size, east facing    */
    kControlTabSmallEastProc    = 133,                          /* Small tab size, east facing    */
    kControlTabLargeWestProc    = 134,                          /* Large tab size, west facing    */
    kControlTabSmallWestProc    = 135                           /* Small tab size, west facing    */
};

/* Tagged data supported by tabs */
enum {
    kControlTabContentRectTag   = FOUR_CHAR_CODE('rect'),       /* Rect*/
    kControlTabEnabledFlagTag   = FOUR_CHAR_CODE('enab'),       /* Boolean*/
    kControlTabFontStyleTag     = kControlFontStyleTag          /* ControlFontStyleRec*/
};

/* New tags in 1.0.1 or later */
enum {
    kControlTabInfoTag          = FOUR_CHAR_CODE('tabi')        /* ControlTabInfoRec*/
};

enum {
    kControlTabInfoVersionZero  = 0
};


struct ControlTabInfoRec {
    SInt16                          version;                    /* version of this structure.*/
    SInt16                          iconSuiteID;                /* icon suite to use. Zero indicates no icon*/
    Str255                          name;                       /* name to be displayed on the tab*/
};
typedef struct ControlTabInfoRec        ControlTabInfoRec;
/* Helper routines are available only thru the shared library/glue. */
EXTERN_API( OSErr )
GetTabContentRect               (ControlHandle          inTabControl,
                                 Rect *                 outContentRect);

EXTERN_API( OSErr )
SetTabEnabled                   (ControlHandle          inTabControl,
                                 SInt16                 inTabToHilite,
                                 Boolean                inEnabled);

/*--------------------------------------------------------------------------------------*/
/*  o VISUAL SEPARATOR (CDEF 9)                                                         */
/*--------------------------------------------------------------------------------------*/
/*  Separator lines determine their orientation (horizontal or vertical) automatically  */
/*  based on the relative height and width of their contrlRect.                         */
/* Visual separator proc IDs */
enum {
    kControlSeparatorLineProc   = 144
};

/*--------------------------------------------------------------------------------------*/
/*  o GROUP BOX (CDEF 10)                                                               */
/*--------------------------------------------------------------------------------------*/
/*  The group box CDEF can be use in several ways. It can have no title, a text title,  */
/*  a check box as the title, or a popup button as a title. There are two versions of   */
/*  group boxes, primary and secondary, which look slightly different.                  */
/* Group Box proc IDs */
enum {
    kControlGroupBoxTextTitleProc = 160,
    kControlGroupBoxCheckBoxProc = 161,
    kControlGroupBoxPopupButtonProc = 162,
    kControlGroupBoxSecondaryTextTitleProc = 164,
    kControlGroupBoxSecondaryCheckBoxProc = 165,
    kControlGroupBoxSecondaryPopupButtonProc = 166
};

/* Tagged data supported by group box */
enum {
    kControlGroupBoxMenuHandleTag = FOUR_CHAR_CODE('mhan'),     /* MenuHandle (popup title only)*/
    kControlGroupBoxFontStyleTag = kControlFontStyleTag         /* ControlFontStyleRec*/
};

/* tags available with Appearance 1.1 or later */
enum {
    kControlGroupBoxTitleRectTag = FOUR_CHAR_CODE('trec')       /* Rect. Rectangle that the title text/control is drawn in. (get only)*/
};

/*--------------------------------------------------------------------------------------*/
/*  o IMAGE WELL (CDEF 11)                                                              */
/*--------------------------------------------------------------------------------------*/
/*  Image Wells allow you to control the content type (pict/icon/etc.) shown in the     */
/*  well.                                                                               */
/*                                                                                      */
/*  This is made possible by overloading the Min and Value parameters for the control.  */
/*                                                                                      */
/*  Parameter                   What Goes Here                                          */
/*  -------------------         --------------------------------------------------      */
/*  Min                         content type (see constants for bevel buttons)          */
/*  Value                       Resource ID of content type, if resource-based.         */
/*                                                                                      */
/*                                                                                      */
/*  Handle-based Content                                                                */
/*  --------------------                                                                */
/*  You can create your control and then set the content to an existing handle to an    */
/*  icon suite, etc. using the macros below. Please keep in mind that resource-based    */
/*  content is owned by the control, handle-based content is owned by you. The CDEF will*/
/*  not try to dispose of handle-based content. If you are changing the content type of */
/*  the button on the fly, you must make sure that if you are replacing a handle-       */
/*  based content with a resource-based content to properly dispose of the handle,      */
/*  else a memory leak will ensue.                                                      */
/*                                                                                      */
/* Image Well proc IDs */
enum {
    kControlImageWellProc       = 176
};

/* Tagged data supported by image wells */
enum {
    kControlImageWellContentTag = FOUR_CHAR_CODE('cont'),       /* ButtonContentInfo*/
    kControlImageWellTransformTag = FOUR_CHAR_CODE('tran')      /* IconTransformType*/
};

/* Helper routines are available only thru the shared library/glue. */
EXTERN_API( OSErr )
GetImageWellContentInfo         (ControlHandle          inButton,
                                 ControlButtonContentInfoPtr  outContent);

EXTERN_API( OSErr )
SetImageWellContentInfo         (ControlHandle          inButton,
                                 ControlButtonContentInfoPtr  inContent);

EXTERN_API( OSErr )
SetImageWellTransform           (ControlHandle          inButton,
                                 IconTransformType      inTransform);

/*--------------------------------------------------------------------------------------*/
/*  o POPUP ARROW (CDEF 12)                                                             */
/*--------------------------------------------------------------------------------------*/
/*  The popup arrow CDEF is used to draw the small arrow normally associated with a     */
/*  popup control. The arrow can point in four directions, and a small or large version */
/*  can be used. This control is provided to allow clients to draw the arrow in a       */
/*  normalized fashion which will take advantage of themes automatically.               */
/*                                                                                      */
/* Popup Arrow proc IDs */
enum {
    kControlPopupArrowEastProc  = 192,
    kControlPopupArrowWestProc  = 193,
    kControlPopupArrowNorthProc = 194,
    kControlPopupArrowSouthProc = 195,
    kControlPopupArrowSmallEastProc = 196,
    kControlPopupArrowSmallWestProc = 197,
    kControlPopupArrowSmallNorthProc = 198,
    kControlPopupArrowSmallSouthProc = 199
};

/* Popup Arrow Orientations */
enum {
    kControlPopupArrowOrientationEast = 0,
    kControlPopupArrowOrientationWest = 1,
    kControlPopupArrowOrientationNorth = 2,
    kControlPopupArrowOrientationSouth = 3
};


typedef UInt16                          ControlPopupArrowOrientation;
/*--------------------------------------------------------------------------------------*/
/*  o PLACARD (CDEF 14)                                                                 */
/*--------------------------------------------------------------------------------------*/
/* Placard proc IDs */
enum {
    kControlPlacardProc         = 224
};

/*--------------------------------------------------------------------------------------*/
/*  o CLOCK (CDEF 15)                                                                   */
/*--------------------------------------------------------------------------------------*/
/*  NOTE:   You can specify more options in the Value paramter when creating the clock. */
/*          See below.                                                                  */
/*                                                                                      */
/*  NOTE:   Under Appearance 1.1, the clock control knows and returns more part codes.  */
/*          The new clock-specific part codes are defined with the other control parts. */
/*          Besides these clock-specific parts, we also return kControlUpButtonPart     */
/*          and kControlDownButtonPart when they hit the up and down arrows.            */
/*          The new part codes give you more flexibility for focusing and hit testing.  */
/*                                                                                      */
/*          The original kControlClockPart is still valid. When hit testing, it means   */
/*          that some non-editable area of the clock's whitespace has been clicked.     */
/*          When focusing a currently unfocused clock, it changes the focus to the      */
/*          first part; it is the same as passing kControlFocusNextPart. When           */
/*          re-focusing a focused clock, it will not change the focus at all.           */
/* Clock proc IDs */
enum {
    kControlClockTimeProc       = 240,
    kControlClockTimeSecondsProc = 241,
    kControlClockDateProc       = 242,
    kControlClockMonthYearProc  = 243
};

/* Clock Types */

typedef UInt16 ControlClockType;
enum {
    kControlClockTypeHourMinute = 0,
    kControlClockTypeHourMinuteSecond = 1,
    kControlClockTypeMonthDay   = 2,
    kControlClockTypeMonthDayYear = 3
};

/* Clock Flags */
/*  These flags can be passed into 'value' field on creation of the control.            */
/*  Value is set to 0 after control is created.                                         */

typedef UInt32 ControlClockFlags;
enum {
    kControlClockFlagStandard   = 0,                            /* editable, non-live*/
    kControlClockNoFlags        = 0,
    kControlClockFlagDisplayOnly = 1,                           /* add this to become non-editable*/
    kControlClockIsDisplayOnly  = 1,
    kControlClockFlagLive       = 2,                            /* automatically shows current time on idle. only valid with display only.*/
    kControlClockIsLive         = 2
};

/* Tagged data supported by clocks */
enum {
    kControlClockLongDateTag    = FOUR_CHAR_CODE('date'),       /* LongDateRec*/
    kControlClockFontStyleTag   = kControlFontStyleTag          /* ControlFontStyleRec*/
};

/*--------------------------------------------------------------------------------------*/
/*  o USER PANE (CDEF 16)                                                               */
/*--------------------------------------------------------------------------------------*/
/* User Pane proc IDs */
enum {
    kControlUserPaneProc        = 256
};

/* Tagged data supported by user panes */
/* Currently, they are all proc ptrs for doing things like drawing and hit testing, etc. */
enum {
    kControlUserItemDrawProcTag = FOUR_CHAR_CODE('uidp'),       /* UserItemUPP*/
    kControlUserPaneDrawProcTag = FOUR_CHAR_CODE('draw'),       /* ControlUserPaneDrawingUPP*/
    kControlUserPaneHitTestProcTag = FOUR_CHAR_CODE('hitt'),    /* ControlUserPaneHitTestUPP*/
    kControlUserPaneTrackingProcTag = FOUR_CHAR_CODE('trak'),   /* ControlUserPaneTrackingUPP*/
    kControlUserPaneIdleProcTag = FOUR_CHAR_CODE('idle'),       /* ControlUserPaneIdleUPP*/
    kControlUserPaneKeyDownProcTag = FOUR_CHAR_CODE('keyd'),    /* ControlUserPaneKeyDownUPP*/
    kControlUserPaneActivateProcTag = FOUR_CHAR_CODE('acti'),   /* ControlUserPaneActivateUPP*/
    kControlUserPaneFocusProcTag = FOUR_CHAR_CODE('foci'),      /* ControlUserPaneFocusUPP*/
    kControlUserPaneBackgroundProcTag = FOUR_CHAR_CODE('back')  /* ControlUserPaneBackgroundUPP*/
};

typedef CALLBACK_API( void , ControlUserPaneDrawProcPtr )(ControlHandle control, SInt16 part);
typedef CALLBACK_API( ControlPartCode , ControlUserPaneHitTestProcPtr )(ControlHandle control, Point where);
typedef CALLBACK_API( ControlPartCode , ControlUserPaneTrackingProcPtr )(ControlHandle control, Point startPt, ControlActionUPP actionProc);
typedef CALLBACK_API( void , ControlUserPaneIdleProcPtr )(ControlHandle control);
typedef CALLBACK_API( ControlPartCode , ControlUserPaneKeyDownProcPtr )(ControlHandle control, SInt16 keyCode, SInt16 charCode, SInt16 modifiers);
typedef CALLBACK_API( void , ControlUserPaneActivateProcPtr )(ControlHandle control, Boolean activating);
typedef CALLBACK_API( ControlPartCode , ControlUserPaneFocusProcPtr )(ControlHandle control, ControlFocusPart action);
typedef CALLBACK_API( void , ControlUserPaneBackgroundProcPtr )(ControlHandle control, ControlBackgroundPtr info);
typedef STACK_UPP_TYPE(ControlUserPaneDrawProcPtr)              ControlUserPaneDrawUPP;
typedef STACK_UPP_TYPE(ControlUserPaneHitTestProcPtr)           ControlUserPaneHitTestUPP;
typedef STACK_UPP_TYPE(ControlUserPaneTrackingProcPtr)          ControlUserPaneTrackingUPP;
typedef STACK_UPP_TYPE(ControlUserPaneIdleProcPtr)              ControlUserPaneIdleUPP;
typedef STACK_UPP_TYPE(ControlUserPaneKeyDownProcPtr)           ControlUserPaneKeyDownUPP;
typedef STACK_UPP_TYPE(ControlUserPaneActivateProcPtr)          ControlUserPaneActivateUPP;
typedef STACK_UPP_TYPE(ControlUserPaneFocusProcPtr)             ControlUserPaneFocusUPP;
typedef STACK_UPP_TYPE(ControlUserPaneBackgroundProcPtr)        ControlUserPaneBackgroundUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(ControlUserPaneDrawUPP)
    NewControlUserPaneDrawUPP      (ControlUserPaneDrawProcPtr userRoutine);

    EXTERN_API(ControlUserPaneHitTestUPP)
    NewControlUserPaneHitTestUPP    (ControlUserPaneHitTestProcPtr userRoutine);

    EXTERN_API(ControlUserPaneTrackingUPP)
    NewControlUserPaneTrackingUPP    (ControlUserPaneTrackingProcPtr userRoutine);

    EXTERN_API(ControlUserPaneIdleUPP)
    NewControlUserPaneIdleUPP      (ControlUserPaneIdleProcPtr userRoutine);

    EXTERN_API(ControlUserPaneKeyDownUPP)
    NewControlUserPaneKeyDownUPP    (ControlUserPaneKeyDownProcPtr userRoutine);

    EXTERN_API(ControlUserPaneActivateUPP)
    NewControlUserPaneActivateUPP    (ControlUserPaneActivateProcPtr userRoutine);

    EXTERN_API(ControlUserPaneFocusUPP)
    NewControlUserPaneFocusUPP     (ControlUserPaneFocusProcPtr userRoutine);

    EXTERN_API(ControlUserPaneBackgroundUPP)
    NewControlUserPaneBackgroundUPP    (ControlUserPaneBackgroundProcPtr userRoutine);

    EXTERN_API(void)
    DisposeControlUserPaneDrawUPP    (ControlUserPaneDrawUPP userUPP);

    EXTERN_API(void)
    DisposeControlUserPaneHitTestUPP    (ControlUserPaneHitTestUPP userUPP);

    EXTERN_API(void)
    DisposeControlUserPaneTrackingUPP    (ControlUserPaneTrackingUPP userUPP);

    EXTERN_API(void)
    DisposeControlUserPaneIdleUPP    (ControlUserPaneIdleUPP userUPP);

    EXTERN_API(void)
    DisposeControlUserPaneKeyDownUPP    (ControlUserPaneKeyDownUPP userUPP);

    EXTERN_API(void)
    DisposeControlUserPaneActivateUPP    (ControlUserPaneActivateUPP userUPP);

    EXTERN_API(void)
    DisposeControlUserPaneFocusUPP    (ControlUserPaneFocusUPP userUPP);

    EXTERN_API(void)
    DisposeControlUserPaneBackgroundUPP    (ControlUserPaneBackgroundUPP userUPP);

    EXTERN_API(void)
    InvokeControlUserPaneDrawUPP    (ControlHandle          control,
                                    SInt16                  part,
                                    ControlUserPaneDrawUPP  userUPP);

    EXTERN_API(ControlPartCode)
    InvokeControlUserPaneHitTestUPP    (ControlHandle       control,
                                    Point                   where,
                                    ControlUserPaneHitTestUPP userUPP);

    EXTERN_API(ControlPartCode)
    InvokeControlUserPaneTrackingUPP    (ControlHandle      control,
                                    Point                   startPt,
                                    ControlActionUPP        actionProc,
                                    ControlUserPaneTrackingUPP userUPP);

    EXTERN_API(void)
    InvokeControlUserPaneIdleUPP    (ControlHandle          control,
                                    ControlUserPaneIdleUPP  userUPP);

    EXTERN_API(ControlPartCode)
    InvokeControlUserPaneKeyDownUPP    (ControlHandle       control,
                                    SInt16                  keyCode,
                                    SInt16                  charCode,
                                    SInt16                  modifiers,
                                    ControlUserPaneKeyDownUPP userUPP);

    EXTERN_API(void)
    InvokeControlUserPaneActivateUPP    (ControlHandle      control,
                                    Boolean                 activating,
                                    ControlUserPaneActivateUPP userUPP);

    EXTERN_API(ControlPartCode)
    InvokeControlUserPaneFocusUPP    (ControlHandle         control,
                                    ControlFocusPart        action,
                                    ControlUserPaneFocusUPP userUPP);

    EXTERN_API(void)
    InvokeControlUserPaneBackgroundUPP    (ControlHandle    control,
                                    ControlBackgroundPtr    info,
                                    ControlUserPaneBackgroundUPP userUPP);

#else
    enum { uppControlUserPaneDrawProcInfo = 0x000002C0 };           /* pascal no_return_value Func(4_bytes, 2_bytes) */
    enum { uppControlUserPaneHitTestProcInfo = 0x000003E0 };        /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    enum { uppControlUserPaneTrackingProcInfo = 0x00000FE0 };       /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppControlUserPaneIdleProcInfo = 0x000000C0 };           /* pascal no_return_value Func(4_bytes) */
    enum { uppControlUserPaneKeyDownProcInfo = 0x00002AE0 };        /* pascal 2_bytes Func(4_bytes, 2_bytes, 2_bytes, 2_bytes) */
    enum { uppControlUserPaneActivateProcInfo = 0x000001C0 };       /* pascal no_return_value Func(4_bytes, 1_byte) */
    enum { uppControlUserPaneFocusProcInfo = 0x000002E0 };          /* pascal 2_bytes Func(4_bytes, 2_bytes) */
    enum { uppControlUserPaneBackgroundProcInfo = 0x000003C0 };     /* pascal no_return_value Func(4_bytes, 4_bytes) */
    #define NewControlUserPaneDrawUPP(userRoutine)                  (ControlUserPaneDrawUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlUserPaneDrawProcInfo, GetCurrentArchitecture())
    #define NewControlUserPaneHitTestUPP(userRoutine)               (ControlUserPaneHitTestUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlUserPaneHitTestProcInfo, GetCurrentArchitecture())
    #define NewControlUserPaneTrackingUPP(userRoutine)              (ControlUserPaneTrackingUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlUserPaneTrackingProcInfo, GetCurrentArchitecture())
    #define NewControlUserPaneIdleUPP(userRoutine)                  (ControlUserPaneIdleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlUserPaneIdleProcInfo, GetCurrentArchitecture())
    #define NewControlUserPaneKeyDownUPP(userRoutine)               (ControlUserPaneKeyDownUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlUserPaneKeyDownProcInfo, GetCurrentArchitecture())
    #define NewControlUserPaneActivateUPP(userRoutine)              (ControlUserPaneActivateUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlUserPaneActivateProcInfo, GetCurrentArchitecture())
    #define NewControlUserPaneFocusUPP(userRoutine)                 (ControlUserPaneFocusUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlUserPaneFocusProcInfo, GetCurrentArchitecture())
    #define NewControlUserPaneBackgroundUPP(userRoutine)            (ControlUserPaneBackgroundUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlUserPaneBackgroundProcInfo, GetCurrentArchitecture())
    #define DisposeControlUserPaneDrawUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define DisposeControlUserPaneHitTestUPP(userUPP)               DisposeRoutineDescriptor(userUPP)
    #define DisposeControlUserPaneTrackingUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define DisposeControlUserPaneIdleUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define DisposeControlUserPaneKeyDownUPP(userUPP)               DisposeRoutineDescriptor(userUPP)
    #define DisposeControlUserPaneActivateUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define DisposeControlUserPaneFocusUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define DisposeControlUserPaneBackgroundUPP(userUPP)            DisposeRoutineDescriptor(userUPP)
    #define InvokeControlUserPaneDrawUPP(control, part, userUPP)    CALL_TWO_PARAMETER_UPP((userUPP), uppControlUserPaneDrawProcInfo, (control), (part))
    #define InvokeControlUserPaneHitTestUPP(control, where, userUPP)  (ControlPartCode)CALL_TWO_PARAMETER_UPP((userUPP), uppControlUserPaneHitTestProcInfo, (control), (where))
    #define InvokeControlUserPaneTrackingUPP(control, startPt, actionProc, userUPP)  (ControlPartCode)CALL_THREE_PARAMETER_UPP((userUPP), uppControlUserPaneTrackingProcInfo, (control), (startPt), (actionProc))
    #define InvokeControlUserPaneIdleUPP(control, userUPP)          CALL_ONE_PARAMETER_UPP((userUPP), uppControlUserPaneIdleProcInfo, (control))
    #define InvokeControlUserPaneKeyDownUPP(control, keyCode, charCode, modifiers, userUPP)  (ControlPartCode)CALL_FOUR_PARAMETER_UPP((userUPP), uppControlUserPaneKeyDownProcInfo, (control), (keyCode), (charCode), (modifiers))
    #define InvokeControlUserPaneActivateUPP(control, activating, userUPP)  CALL_TWO_PARAMETER_UPP((userUPP), uppControlUserPaneActivateProcInfo, (control), (activating))
    #define InvokeControlUserPaneFocusUPP(control, action, userUPP)  (ControlPartCode)CALL_TWO_PARAMETER_UPP((userUPP), uppControlUserPaneFocusProcInfo, (control), (action))
    #define InvokeControlUserPaneBackgroundUPP(control, info, userUPP)  CALL_TWO_PARAMETER_UPP((userUPP), uppControlUserPaneBackgroundProcInfo, (control), (info))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewControlUserPaneDrawProc(userRoutine)                 NewControlUserPaneDrawUPP(userRoutine)
#define NewControlUserPaneHitTestProc(userRoutine)              NewControlUserPaneHitTestUPP(userRoutine)
#define NewControlUserPaneTrackingProc(userRoutine)             NewControlUserPaneTrackingUPP(userRoutine)
#define NewControlUserPaneIdleProc(userRoutine)                 NewControlUserPaneIdleUPP(userRoutine)
#define NewControlUserPaneKeyDownProc(userRoutine)              NewControlUserPaneKeyDownUPP(userRoutine)
#define NewControlUserPaneActivateProc(userRoutine)             NewControlUserPaneActivateUPP(userRoutine)
#define NewControlUserPaneFocusProc(userRoutine)                NewControlUserPaneFocusUPP(userRoutine)
#define NewControlUserPaneBackgroundProc(userRoutine)           NewControlUserPaneBackgroundUPP(userRoutine)
#define CallControlUserPaneDrawProc(userRoutine, control, part) InvokeControlUserPaneDrawUPP(control, part, userRoutine)
#define CallControlUserPaneHitTestProc(userRoutine, control, where) InvokeControlUserPaneHitTestUPP(control, where, userRoutine)
#define CallControlUserPaneTrackingProc(userRoutine, control, startPt, actionProc) InvokeControlUserPaneTrackingUPP(control, startPt, actionProc, userRoutine)
#define CallControlUserPaneIdleProc(userRoutine, control)       InvokeControlUserPaneIdleUPP(control, userRoutine)
#define CallControlUserPaneKeyDownProc(userRoutine, control, keyCode, charCode, modifiers) InvokeControlUserPaneKeyDownUPP(control, keyCode, charCode, modifiers, userRoutine)
#define CallControlUserPaneActivateProc(userRoutine, control, activating) InvokeControlUserPaneActivateUPP(control, activating, userRoutine)
#define CallControlUserPaneFocusProc(userRoutine, control, action) InvokeControlUserPaneFocusUPP(control, action, userRoutine)
#define CallControlUserPaneBackgroundProc(userRoutine, control, info) InvokeControlUserPaneBackgroundUPP(control, info, userRoutine)
/*
  ------------------------------------------------------------------------------------------
    o EDIT TEXT (CDEF 17)
  ------------------------------------------------------------------------------------------
*/
/* Edit Text proc IDs */
enum {
    kControlEditTextProc        = 272,
    kControlEditTextPasswordProc = 274
};

/* proc IDs available with Appearance 1.1 or later */
enum {
    kControlEditTextInlineInputProc = 276                       /* Can't combine with the other variants*/
};

/* Tagged data supported by edit text */
enum {
    kControlEditTextStyleTag    = kControlFontStyleTag,         /* ControlFontStyleRec*/
    kControlEditTextTextTag     = FOUR_CHAR_CODE('text'),       /* Buffer of chars - you supply the buffer*/
    kControlEditTextTEHandleTag = FOUR_CHAR_CODE('than'),       /* The TEHandle of the text edit record*/
    kControlEditTextKeyFilterTag = kControlKeyFilterTag,
    kControlEditTextSelectionTag = FOUR_CHAR_CODE('sele'),      /* EditTextSelectionRec*/
    kControlEditTextPasswordTag = FOUR_CHAR_CODE('pass')        /* The clear text password text*/
};

/* tags available with Appearance 1.1 or later */
enum {
    kControlEditTextKeyScriptBehaviorTag = FOUR_CHAR_CODE('kscr'), /* ControlKeyScriptBehavior. Defaults to "PrefersRoman" for password fields,*/
                                                                /*       or "AllowAnyScript" for non-password fields.*/
    kControlEditTextLockedTag   = FOUR_CHAR_CODE('lock'),       /* Boolean. Locking disables editability.*/
    kControlEditTextFixedTextTag = FOUR_CHAR_CODE('ftxt'),      /* Like the normal text tag, but fixes inline input first*/
    kControlEditTextValidationProcTag = FOUR_CHAR_CODE('vali'), /* ControlEditTextValidationUPP. Called when a key filter can't be: after cut, paste, etc.*/
    kControlEditTextInlinePreUpdateProcTag = FOUR_CHAR_CODE('prup'), /* TSMTEPreUpdateUPP and TSMTEPostUpdateUpp. For use with inline input variant...*/
    kControlEditTextInlinePostUpdateProcTag = FOUR_CHAR_CODE('poup') /* ...The refCon parameter will contain the ControlHandle.*/
};

/* Structure for getting the edit text selection */

struct ControlEditTextSelectionRec {
    SInt16                          selStart;
    SInt16                          selEnd;
};
typedef struct ControlEditTextSelectionRec ControlEditTextSelectionRec;

typedef ControlEditTextSelectionRec *   ControlEditTextSelectionPtr;
typedef CALLBACK_API( void , ControlEditTextValidationProcPtr )(ControlHandle control);
typedef STACK_UPP_TYPE(ControlEditTextValidationProcPtr)        ControlEditTextValidationUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(ControlEditTextValidationUPP)
    NewControlEditTextValidationUPP    (ControlEditTextValidationProcPtr userRoutine);

    EXTERN_API(void)
    DisposeControlEditTextValidationUPP    (ControlEditTextValidationUPP userUPP);

    EXTERN_API(void)
    InvokeControlEditTextValidationUPP    (ControlHandle    control,
                                    ControlEditTextValidationUPP userUPP);

#else
    enum { uppControlEditTextValidationProcInfo = 0x000000C0 };     /* pascal no_return_value Func(4_bytes) */
    #define NewControlEditTextValidationUPP(userRoutine)            (ControlEditTextValidationUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlEditTextValidationProcInfo, GetCurrentArchitecture())
    #define DisposeControlEditTextValidationUPP(userUPP)            DisposeRoutineDescriptor(userUPP)
    #define InvokeControlEditTextValidationUPP(control, userUPP)    CALL_ONE_PARAMETER_UPP((userUPP), uppControlEditTextValidationProcInfo, (control))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewControlEditTextValidationProc(userRoutine)           NewControlEditTextValidationUPP(userRoutine)
#define CallControlEditTextValidationProc(userRoutine, control) InvokeControlEditTextValidationUPP(control, userRoutine)
/*--------------------------------------------------------------------------------------*/
/*  o STATIC TEXT (CDEF 18)                                                             */
/*--------------------------------------------------------------------------------------*/
/* Static Text proc IDs */
enum {
    kControlStaticTextProc      = 288
};

/* Tagged data supported by static text */
enum {
    kControlStaticTextStyleTag  = kControlFontStyleTag,         /* ControlFontStyleRec*/
    kControlStaticTextTextTag   = FOUR_CHAR_CODE('text'),       /* Copy of text*/
    kControlStaticTextTextHeightTag = FOUR_CHAR_CODE('thei')    /* SInt16*/
};

/* Tags available with appearance 1.1 or later */
enum {
    kControlStaticTextTruncTag  = FOUR_CHAR_CODE('trun')        /* TruncCode (-1 means no truncation)*/
};

/*--------------------------------------------------------------------------------------*/
/*  o PICTURE CONTROL (CDEF 19)                                                         */
/*--------------------------------------------------------------------------------------*/
/*  Value parameter should contain the ID of the picture you wish to display when       */
/*  creating controls of this type. If you don't want the control tracked at all, use   */
/*  the 'no track' variant.                                                             */
/* Picture control proc IDs */
enum {
    kControlPictureProc         = 304,
    kControlPictureNoTrackProc  = 305                           /* immediately returns kControlPicturePart*/
};

/*--------------------------------------------------------------------------------------*/
/*  o ICON CONTROL (CDEF 20)                                                            */
/*--------------------------------------------------------------------------------------*/
/*  Value parameter should contain the ID of the ICON or cicn you wish to display when  */
/*  creating controls of this type. If you don't want the control tracked at all, use   */
/*  the 'no track' variant.                                                             */
/* Icon control proc IDs */
enum {
    kControlIconProc            = 320,
    kControlIconNoTrackProc     = 321,                          /* immediately returns kControlIconPart*/
    kControlIconSuiteProc       = 322,
    kControlIconSuiteNoTrackProc = 323                          /* immediately returns kControlIconPart*/
};

enum {
                                                                /* icon ref controls may have either an icon, color icon, icon suite, or icon ref.*/
                                                                /* for data other than icon, you must set the data by passing a*/
                                                                /* ControlButtonContentInfo to SetControlData*/
    kControlIconRefProc         = 324,
    kControlIconRefNoTrackProc  = 325                           /* immediately returns kControlIconPart*/
};

/* Tagged data supported by icon controls */
enum {
    kControlIconTransformTag    = FOUR_CHAR_CODE('trfm'),       /* IconTransformType*/
    kControlIconAlignmentTag    = FOUR_CHAR_CODE('algn')        /* IconAlignmentType*/
};

/* Tags available with appearance 1.1 or later */
enum {
    kControlIconResourceIDTag   = FOUR_CHAR_CODE('ires'),       /* SInt16 resource ID of icon to use*/
    kControlIconContentTag      = FOUR_CHAR_CODE('cont')        /* accepts a ControlButtonContentInfo*/
};

/*--------------------------------------------------------------------------------------*/
/*  o WINDOW HEADER (CDEF 21)                                                           */
/*--------------------------------------------------------------------------------------*/
/* Window Header proc IDs */
enum {
    kControlWindowHeaderProc    = 336,                          /* normal header*/
    kControlWindowListViewHeaderProc = 337                      /* variant for list views - no bottom line*/
};

/*--------------------------------------------------------------------------------------*/
/*  o LIST BOX (CDEF 22)                                                                */
/*--------------------------------------------------------------------------------------*/
/*  Lists use an auxiliary resource to define their format. The resource type used is   */
/*  'ldes' and a definition for it can be found in Appearance.r. The resource ID for    */
/*  the ldes is passed in the 'value' parameter when creating the control. You may pass */
/*  zero in value. This tells the List Box control to not use a resource. The list will */
/*  be created with default values, and will use the standard LDEF (0). You can change  */
/*  the list by getting the list handle. You can set the LDEF to use by using the tag   */
/*  below (kControlListBoxLDEFTag)                                                      */
/* List Box proc IDs */
enum {
    kControlListBoxProc         = 352,
    kControlListBoxAutoSizeProc = 353
};

/* Tagged data supported by list box */
enum {
    kControlListBoxListHandleTag = FOUR_CHAR_CODE('lhan'),      /* ListHandle*/
    kControlListBoxKeyFilterTag = kControlKeyFilterTag,         /* ControlKeyFilterUPP*/
    kControlListBoxFontStyleTag = kControlFontStyleTag          /* ControlFontStyleRec*/
};

/* New tags in 1.0.1 or later */
enum {
    kControlListBoxDoubleClickTag = FOUR_CHAR_CODE('dblc'),     /* Boolean. Was last click a double-click?*/
    kControlListBoxLDEFTag      = FOUR_CHAR_CODE('ldef')        /* SInt16. ID of LDEF to use.*/
};

/*--------------------------------------------------------------------------------------*/
/*  o PUSH BUTTON (CDEF 23)                                                             */
/*--------------------------------------------------------------------------------------*/
/*  The new standard checkbox and radio button controls support a "mixed" value that    */
/*  indicates that the current setting contains a mixed set of on and off values. The   */
/*  control value used to display this indication is defined in Controls.h:             */
/*                                                                                      */
/*      kControlCheckBoxMixedValue = 2                                                  */
/*                                                                                      */
/*  Two new variants of the standard pushbutton have been added to the standard control */
/*  suite that draw a color icon next to the control title. One variant draws the icon  */
/*  on the left side, the other draws it on the right side (when the system justifica-  */
/*  tion is right to left, these are reversed).                                         */
/*                                                                                      */
/*  When either of the icon pushbuttons are created, the contrlMax field of the control */
/*  record is used to determine the ID of the 'cicn' resource drawn in the pushbutton.  */
/*                                                                                      */
/*  In addition, a push button can now be told to draw with a default outline using the */
/*  SetControlData routine with the kPushButtonDefaultTag below.                        */
/*                                                                                      */
/*  A push button may also be marked using the kControlPushButtonCancelTag. This has    */
/*  no visible representation, but does cause the button to play the CancelButton theme */
/*  sound instead of the regular pushbutton theme sound when pressed.                   */
/*                                                                                      */
/* Theme Push Button/Check Box/Radio Button proc IDs */
enum {
    kControlPushButtonProc      = 368,
    kControlCheckBoxProc        = 369,
    kControlRadioButtonProc     = 370,
    kControlPushButLeftIconProc = 374,                          /* Standard pushbutton with left-side icon*/
    kControlPushButRightIconProc = 375                          /* Standard pushbutton with right-side icon*/
};

/* Variants with Appearance 1.1 or later */
enum {
    kControlCheckBoxAutoToggleProc = 371,
    kControlRadioButtonAutoToggleProc = 372
};

/* Tagged data supported by standard buttons */
enum {
    kControlPushButtonDefaultTag = FOUR_CHAR_CODE('dflt'),      /* default ring flag*/
    kControlPushButtonCancelTag = FOUR_CHAR_CODE('cncl')        /* cancel button flag (1.1 and later)*/
};

/*--------------------------------------------------------------------------------------*/
/*  o SCROLL BAR (CDEF 24)                                                              */
/*--------------------------------------------------------------------------------------*/
/*  This is the new Appearance scroll bar.                                              */
/*                                                                                      */
/* Theme Scroll Bar proc IDs */
enum {
    kControlScrollBarProc       = 384,                          /* normal scroll bar*/
    kControlScrollBarLiveProc   = 386                           /* live scrolling variant*/
};

/*--------------------------------------------------------------------------------------*/
/*  o POPUP BUTTON (CDEF 25)                                                            */
/*--------------------------------------------------------------------------------------*/
/*  This is the new Appearance Popup Button. It takes the same variants and does the    */
/*  same overloading as the previous popup menu control. There are some differences:    */
/*                                                                                      */
/*  Passing in a menu ID of -12345 causes the popup not to try and get the menu from a  */
/*  resource. Instead, you can build the menu and later stuff the menuhandle field in   */
/*  the popup data information.                                                         */
/*                                                                                      */
/*  You can pass -1 in the Max parameter to have the control calculate the width of the */
/*  title on its own instead of guessing and then tweaking to get it right. It adds the */
/*  appropriate amount of space between the title and the popup.                        */
/*                                                                                      */
/* Theme Popup Button proc IDs */
enum {
    kControlPopupButtonProc     = 400,
    kControlPopupFixedWidthVariant = 1 << 0,
    kControlPopupVariableWidthVariant = 1 << 1,
    kControlPopupUseAddResMenuVariant = 1 << 2,
    kControlPopupUseWFontVariant = kControlUsesOwningWindowsFontVariant
};

/* These tags are available in 1.0.1 or later of Appearance */
enum {
    kControlPopupButtonMenuHandleTag = FOUR_CHAR_CODE('mhan'),  /* MenuHandle*/
    kControlPopupButtonMenuIDTag = FOUR_CHAR_CODE('mnid')       /* SInt16*/
};

/* These tags are available in 1.1 or later of Appearance */
enum {
    kControlPopupButtonExtraHeightTag = FOUR_CHAR_CODE('exht')  /* SInt16 extra vertical whitespace within the button*/
};


/*--------------------------------------------------------------------------------------*/
/*  o RADIO GROUP (CDEF 26)                                                             */
/*--------------------------------------------------------------------------------------*/
/*  This control implements a radio group. It is an embedding control and can therefore */
/*  only be used when a control hierarchy is established for its owning window. You     */
/*  should only embed radio buttons within it. As radio buttons are embedded into it,   */
/*  the group sets up its value, min, and max to represent the number of embedded items.*/
/*  The current value of the control is the index of the sub-control that is the current*/
/*  'on' radio button. To get the current radio button control handle, you can use the  */
/*  control manager call GetIndSubControl, passing in the value of the radio group.     */
/*                                                                                      */
/*  NOTE: This control is only available with Appearance 1.0.1.                         */
/* Radio Group Proc ID */
enum {
    kControlRadioGroupProc      = 416
};

/*--------------------------------------------------------------------------------------*/
/*  o SCROLL TEXT BOX (CDEF 27)                                                         */
/*--------------------------------------------------------------------------------------*/
/*  This control implements a scrolling box of (non-editable) text. This is useful for  */
/*  credits in about boxes, etc.                                                        */
/*  The standard version of this control has a scroll bar, but the autoscrolling        */
/*  variant does not. The autoscrolling variant needs two pieces of information to      */
/*  work: delay (in ticks) before the scrolling starts, and time (in ticks) between     */
/*  scrolls. It will scroll one pixel at a time, unless changed via SetControlData.     */
/*                                                                                      */
/*  Parameter                   What Goes Here                                          */
/*  -------------------         ----------------------------------------------------    */
/*  Value                       Resource ID of 'TEXT'/'styl' content.                   */
/*  Min                         Scroll start delay (in ticks)                       .   */
/*  Max                         Delay (in ticks) between scrolls.                       */
/*                                                                                      */
/*  NOTE: This control is only available with Appearance 1.1.                           */
/* Scroll Text Box Proc IDs */
enum {
    kControlScrollTextBoxProc   = 432,
    kControlScrollTextBoxAutoScrollProc = 433
};

/* Tagged data supported by Scroll Text Box */
enum {
    kControlScrollTextBoxDelayBeforeAutoScrollTag = FOUR_CHAR_CODE('stdl'), /* UInt32 (ticks until autoscrolling starts)*/
    kControlScrollTextBoxDelayBetweenAutoScrollTag = FOUR_CHAR_CODE('scdl'), /* UInt32 (ticks between scrolls)*/
    kControlScrollTextBoxAutoScrollAmountTag = FOUR_CHAR_CODE('samt'), /* UInt16 (pixels per scroll) -- defaults to 1*/
    kControlScrollTextBoxContentsTag = FOUR_CHAR_CODE('tres')   /* SInt16 (resource ID of 'TEXT'/'styl') -- write only!*/
};

#if OLDROUTINENAMES
/*--------------------------------------------------------------------------------------*/
/*  o OLDROUTINENAMES                                                                   */
/*--------------------------------------------------------------------------------------*/
enum {
    kControlCheckboxUncheckedValue = kControlCheckBoxUncheckedValue,
    kControlCheckboxCheckedValue = kControlCheckBoxCheckedValue,
    kControlCheckboxMixedValue  = kControlCheckBoxMixedValue
};

enum {
    inLabel                     = kControlLabelPart,
    inMenu                      = kControlMenuPart,
    inTriangle                  = kControlTrianglePart,
    inButton                    = kControlButtonPart,
    inCheckBox                  = kControlCheckBoxPart,
    inUpButton                  = kControlUpButtonPart,
    inDownButton                = kControlDownButtonPart,
    inPageUp                    = kControlPageUpPart,
    inPageDown                  = kControlPageDownPart
};

enum {
    kInLabelControlPart         = kControlLabelPart,
    kInMenuControlPart          = kControlMenuPart,
    kInTriangleControlPart      = kControlTrianglePart,
    kInButtonControlPart        = kControlButtonPart,
    kInCheckBoxControlPart      = kControlCheckBoxPart,
    kInUpButtonControlPart      = kControlUpButtonPart,
    kInDownButtonControlPart    = kControlDownButtonPart,
    kInPageUpControlPart        = kControlPageUpPart,
    kInPageDownControlPart      = kControlPageDownPart
};


#endif  /* OLDROUTINENAMES */





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

#endif /* __CONTROLDEFINITIONS__ */

