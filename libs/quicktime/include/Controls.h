/*
     File:       Controls.h
 
     Contains:   Control Manager interfaces
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CONTROLS__
#define __CONTROLS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __TEXTEDIT__
#include <TextEdit.h>
#endif

#ifndef __DRAG__
#include <Drag.h>
#endif

#ifndef __ICONS__
#include <Icons.h>
#endif

#ifndef __COLLECTIONS__
#include <Collections.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
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
    kControlDefProcType         = FOUR_CHAR_CODE('CDEF'),
    kControlTemplateResourceType = FOUR_CHAR_CODE('CNTL'),
    kControlColorTableResourceType = FOUR_CHAR_CODE('cctb'),
    kControlDefProcResourceType = FOUR_CHAR_CODE('CDEF')
};

/*------------------------------------------------------------------------------------------------------*/
/*  o Format of a 'CNTL' resource                                                                       */
/*------------------------------------------------------------------------------------------------------*/

struct ControlTemplate {
    Rect                            controlRect;
    SInt16                          controlValue;
    Boolean                         controlVisible;
    UInt8                           fill;
    SInt16                          controlMaximum;
    SInt16                          controlMinimum;
    SInt16                          controlDefProcID;
    SInt32                          controlReference;
    Str255                          controlTitle;
};
typedef struct ControlTemplate          ControlTemplate;
typedef ControlTemplate *               ControlTemplatePtr;
typedef ControlTemplatePtr *            ControlTemplateHandle;


#if !TARGET_OS_MAC
/*
  ---------------------------------------------------------------------------------------------------------
   o NON-MAC COMPATIBILITY CODES (QuickTime 3.0)
  ---------------------------------------------------------------------------------------------------------
*/

typedef UInt32                          ControlNotification;
enum {
    controlNotifyNothing        = FOUR_CHAR_CODE('nada'),       /* No (null) notification*/
    controlNotifyClick          = FOUR_CHAR_CODE('clik'),       /* Control was clicked*/
    controlNotifyFocus          = FOUR_CHAR_CODE('focu'),       /* Control got keyboard focus*/
    controlNotifyKey            = FOUR_CHAR_CODE('key ')        /* Control got a keypress*/
};

typedef UInt32                          ControlCapabilities;
enum {
    kControlCanAutoInvalidate   = 1L << 0                       /* Control component automatically invalidates areas left behind after hide/move operation.*/
};

/* procID's for our added "controls"*/
enum {
    staticTextProc              = 256,                          /* static text*/
    editTextProc                = 272,                          /* editable text*/
    iconProc                    = 288,                          /* icon*/
    userItemProc                = 304,                          /* user drawn item*/
    pictItemProc                = 320                           /* pict*/
};

#endif  /* !TARGET_OS_MAC */

/*------------------------------------------------------------------------------------------------------*/
/*  o ControlRef                                                                                        */
/*------------------------------------------------------------------------------------------------------*/
#if !OPAQUE_TOOLBOX_STRUCTS
typedef struct ControlRecord            ControlRecord;
typedef ControlRecord *                 ControlPtr;
typedef ControlPtr *                    ControlRef;
#else
typedef struct OpaqueControlRef*        ControlRef;
#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

/* ControlHandle is obsolete. Use ControlRef.*/
typedef ControlRef                      ControlHandle;
typedef SInt16                          ControlPartCode;
/*------------------------------------------------------------------------------------------------------*/
/* o Control ActionProcPtr                                                                              */
/*------------------------------------------------------------------------------------------------------*/
typedef CALLBACK_API( void , ControlActionProcPtr )(ControlRef theControl, ControlPartCode partCode);
typedef STACK_UPP_TYPE(ControlActionProcPtr)                    ControlActionUPP;
/*------------------------------------------------------------------------------------------------------*/
/*  o ControlRecord                                                                                     */
/*------------------------------------------------------------------------------------------------------*/
#if !OPAQUE_TOOLBOX_STRUCTS

struct ControlRecord {
    ControlRef                      nextControl;
    WindowPtr                       contrlOwner;
    Rect                            contrlRect;
    UInt8                           contrlVis;
    UInt8                           contrlHilite;
    SInt16                          contrlValue;
    SInt16                          contrlMin;
    SInt16                          contrlMax;
    Handle                          contrlDefProc;
    Handle                          contrlData;
    ControlActionUPP                contrlAction;
    SInt32                          contrlRfCon;
    Str255                          contrlTitle;
};

#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

/*------------------------------------------------------------------------------------------------------*/
/* o Control ActionProcPtr : Epilogue                                                                   */
/*------------------------------------------------------------------------------------------------------*/
#if OPAQUE_UPP_TYPES
    EXTERN_API(ControlActionUPP)
    NewControlActionUPP            (ControlActionProcPtr    userRoutine);

    EXTERN_API(void)
    DisposeControlActionUPP        (ControlActionUPP        userUPP);

    EXTERN_API(void)
    InvokeControlActionUPP         (ControlRef              theControl,
                                    ControlPartCode         partCode,
                                    ControlActionUPP        userUPP);

#else
    enum { uppControlActionProcInfo = 0x000002C0 };                 /* pascal no_return_value Func(4_bytes, 2_bytes) */
    #define NewControlActionUPP(userRoutine)                        (ControlActionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlActionProcInfo, GetCurrentArchitecture())
    #define DisposeControlActionUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define InvokeControlActionUPP(theControl, partCode, userUPP)   CALL_TWO_PARAMETER_UPP((userUPP), uppControlActionProcInfo, (theControl), (partCode))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewControlActionProc(userRoutine)                       NewControlActionUPP(userRoutine)
#define CallControlActionProc(userRoutine, theControl, partCode) InvokeControlActionUPP(theControl, partCode, userRoutine)
/*------------------------------------------------------------------------------------------------------*/
/*  o Control Color Table                                                                               */
/*------------------------------------------------------------------------------------------------------*/
enum {
    cFrameColor                 = 0,
    cBodyColor                  = 1,
    cTextColor                  = 2,
    cThumbColor                 = 3,
    kNumberCtlCTabEntries       = 4
};


struct CtlCTab {
    SInt32                          ccSeed;
    SInt16                          ccRider;
    SInt16                          ctSize;
    ColorSpec                       ctTable[4];
};
typedef struct CtlCTab                  CtlCTab;

typedef CtlCTab *                       CCTabPtr;
typedef CCTabPtr *                      CCTabHandle;
/*------------------------------------------------------------------------------------------------------*/
/*  o Auxiliary Control Record                                                                          */
/*------------------------------------------------------------------------------------------------------*/
#if !OPAQUE_TOOLBOX_STRUCTS

struct AuxCtlRec {
    Handle                          acNext;
    ControlRef                      acOwner;
    CCTabHandle                     acCTable;
    SInt16                          acFlags;
    SInt32                          acReserved;
    SInt32                          acRefCon;
};
typedef struct AuxCtlRec                AuxCtlRec;

typedef AuxCtlRec *                     AuxCtlPtr;
typedef AuxCtlPtr *                     AuxCtlHandle;
#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

/*--------------------------------------------------------------------------------------*/
/*  o Control Variants                                                                  */
/*--------------------------------------------------------------------------------------*/
typedef SInt16                          ControlVariant;
enum {
    kControlNoVariant           = 0,                            /* No variant*/
    kControlUsesOwningWindowsFontVariant = 1 << 3               /* Control uses owning windows font to display text*/
};


/*--------------------------------------------------------------------------------------*/
/*  o Control Part Codes                                                                */
/*--------------------------------------------------------------------------------------*/
/* Basic part codes */
enum {
    kControlNoPart              = 0,
    kControlIndicatorPart       = 129,
    kControlDisabledPart        = 254,
    kControlInactivePart        = 255
};

/* Use this constant in Get/SetControlData when the data referred to is not         */
/* specific to a part, but rather the entire control, e.g. the list handle of a     */
/* list box control.                                                                */
enum {
    kControlEntireControl       = 0
};

/*  Meta-Parts                                                                          */
/*                                                                                      */
/*  If you haven't guessed from looking at other toolbox headers. We like the word      */
/*  'meta'. It's cool. So here's one more for you. A meta-part is a part used in a call */
/*  to the GetControlRegion API. These parts are parts that might be defined by a       */
/*  control, but should not be returned from calls like TestControl, et al. They define */
/*  a region of a control, presently the structure and the content region. The content  */
/*  region is only defined by controls that can embed other controls. It is the area    */
/*  that embedded content can live.                                                     */
/*                                                                                      */
/*  Along with these parts, you can also pass in normal part codes to get the regions   */
/*  of the parts. Not all controls fully support this at the time this was written.     */
enum {
    kControlStructureMetaPart   = -1,
    kControlContentMetaPart     = -2
};

/* focusing part codes */
enum {
    kControlFocusNoPart         = 0,                            /* tells control to clear its focus*/
    kControlFocusNextPart       = -1,                           /* tells control to focus on the next part*/
    kControlFocusPrevPart       = -2                            /* tells control to focus on the previous part*/
};

typedef SInt16                          ControlFocusPart;
/*------------------------------------------------------------------------------------------------------*/
/*  o Control Collection Tags                                                                           */
/*------------------------------------------------------------------------------------------------------*/
/*  These are standard tags that you will find in a flattened Control's Collection.                     */
/*                                                                                                      */
/*  All tags at ID zero in a flattened Control's Collection is reserved for Control Manager use.        */
/*  Custom control definitions should use other IDs.                                                    */
/*                                                                                                      */
/*  Most of these tags are interpreted when you call CreateCustomControl; the Control Manager will      */
/*  put value in the right place before calling the Control Definition with the initialization message. */
/*  Other tags are only interpreted when calling UnflattenControl.                                      */
enum {
    kControlCollectionTagBounds = FOUR_CHAR_CODE('boun'),       /* Rect - the bounding rectangle*/
    kControlCollectionTagValue  = FOUR_CHAR_CODE('valu'),       /* SInt32 - the value*/
    kControlCollectionTagMinimum = FOUR_CHAR_CODE('min '),      /* SInt32 - the minimum*/
    kControlCollectionTagMaximum = FOUR_CHAR_CODE('max '),      /* SInt32 - the maximum*/
    kControlCollectionTagViewSize = FOUR_CHAR_CODE('view'),     /* SInt32 - the view size*/
    kControlCollectionTagVisibility = FOUR_CHAR_CODE('visi'),   /* Boolean - the visible state*/
    kControlCollectionTagRefCon = FOUR_CHAR_CODE('refc'),       /* SInt32 - the refCon*/
    kControlCollectionTagTitle  = FOUR_CHAR_CODE('titl')        /* arbitrarily sized character array - the title*/
};

/*  The following are additional tags which are only interpreted by UnflattenControl. */
enum {
    kControlCollectionTagSubControls = FOUR_CHAR_CODE('subc')   /* data for all of a control's subcontrols*/
};


/*------------------------------------------------------------------------------------------------------*/
/*  o Control Image Content                                                                             */
/*------------------------------------------------------------------------------------------------------*/
enum {
    kControlContentTextOnly     = 0,
    kControlNoContent           = 0,
    kControlContentIconSuiteRes = 1,
    kControlContentCIconRes     = 2,
    kControlContentPictRes      = 3,
    kControlContentICONRes      = 4,
    kControlContentIconSuiteHandle = 129,
    kControlContentCIconHandle  = 130,
    kControlContentPictHandle   = 131,
    kControlContentIconRef      = 132,
    kControlContentICON         = 133
};

typedef SInt16                          ControlContentType;

struct ControlButtonContentInfo {
    ControlContentType              contentType;
    union {
        SInt16                          resID;
        CIconHandle                     cIconHandle;
        Handle                          iconSuite;
        IconRef                         iconRef;
        PicHandle                       picture;
        Handle                          ICONHandle;
    }                                 u;
};
typedef struct ControlButtonContentInfo ControlButtonContentInfo;
typedef ControlButtonContentInfo *      ControlButtonContentInfoPtr;

typedef ControlButtonContentInfo        ControlImageContentInfo;
typedef ControlButtonContentInfo *      ControlImageContentInfoPtr;
/*------------------------------------------------------------------------------------------------------*/
/*  o Control Key Script Behavior                                                                       */
/*------------------------------------------------------------------------------------------------------*/
enum {
    kControlKeyScriptBehaviorAllowAnyScript = FOUR_CHAR_CODE('any '), /* leaves the current keyboard alone and allows user to change the keyboard.*/
    kControlKeyScriptBehaviorPrefersRoman = FOUR_CHAR_CODE('prmn'), /* switches the keyboard to roman, but allows them to change it as desired.*/
    kControlKeyScriptBehaviorRequiresRoman = FOUR_CHAR_CODE('rrmn') /* switches the keyboard to roman and prevents the user from changing it.*/
};

typedef UInt32                          ControlKeyScriptBehavior;
/*------------------------------------------------------------------------------------------------------*/
/*  o Control Font Style                                                                                */
/*------------------------------------------------------------------------------------------------------*/
/*    SPECIAL FONT USAGE NOTES: You can specify the font to use for many control types.
    The constants below are meta-font numbers which you can use to set a particular
    control's font usage. There are essentially two modes you can use: 1) default,
    which is essentially the same as it always has been, i.e. it uses the system font, unless
    directed to use the window font via a control variant. 2) you can specify to use
    the big or small system font in a generic manner. The Big system font is the font
    used in menus, etc. Chicago has filled that role for some time now. Small system
    font is currently Geneva 10. The meta-font number implies the size and style.

    NOTE:       Not all font attributes are used by all controls. Most, in fact, ignore
                the fore and back color (Static Text is the only one that does, for
                backwards compatibility). Also size, face, and addFontSize are ignored
                when using the meta-font numbering.
*/
/* Meta-font numbering - see note above */
enum {
    kControlFontBigSystemFont   = -1,                           /* force to big system font*/
    kControlFontSmallSystemFont = -2,                           /* force to small system font*/
    kControlFontSmallBoldSystemFont = -3,                       /* force to small bold system font*/
    kControlFontViewSystemFont  = -4                            /* force to views system font (DataBrowser control only)*/
};

/* Add these masks together to set the flags field of a ControlFontStyleRec */
/* They specify which fields to apply to the text. It is important to make  */
/* sure that you specify only the fields that you wish to set.              */
enum {
    kControlUseFontMask         = 0x0001,
    kControlUseFaceMask         = 0x0002,
    kControlUseSizeMask         = 0x0004,
    kControlUseForeColorMask    = 0x0008,
    kControlUseBackColorMask    = 0x0010,
    kControlUseModeMask         = 0x0020,
    kControlUseJustMask         = 0x0040,
    kControlUseAllMask          = 0x00FF,
    kControlAddFontSizeMask     = 0x0100
};

/* AddToMetaFont indicates that we want to start with a standard system     */
/* font, but then we'd like to add the other attributes. Normally, the meta */
/* font ignores all other flags                                             */
enum {
    kControlAddToMetaFontMask   = 0x0200                        /* Available in Appearance 1.1 or later*/
};


struct ControlFontStyleRec {
    SInt16                          flags;
    SInt16                          font;
    SInt16                          size;
    SInt16                          style;
    SInt16                          mode;
    SInt16                          just;
    RGBColor                        foreColor;
    RGBColor                        backColor;
};
typedef struct ControlFontStyleRec      ControlFontStyleRec;

typedef ControlFontStyleRec *           ControlFontStylePtr;
/*------------------------------------------------------------------------------------------------------*/
/*  o Click Activation Results                                                                          */
/*------------------------------------------------------------------------------------------------------*/
/*  These are for use with GetControlClickActivation. The enumerated values should be pretty            */
/*  self-explanatory, but just in case:                                                                 */
/*  o Activate/DoNotActivate indicates whether or not to change the owning window's z-ordering before   */
/*      processing the click. If activation is requested, you may also want to immediately redraw the   */
/*      newly exposed portion of the window.                                                            */
/*  o Ignore/Handle Click indicates whether or not to call an appropriate click handling API (like      */
/*      HandleControlClick) in respose to the event.                                                    */
enum {
    kDoNotActivateAndIgnoreClick = 0,                           /* probably never used. here for completeness.*/
    kDoNotActivateAndHandleClick = 1,                           /* let the control handle the click while the window is still in the background.*/
    kActivateAndIgnoreClick     = 2,                            /* control doesn't want to respond directly to the click, but window should still be brought forward.*/
    kActivateAndHandleClick     = 3                             /* control wants to respond to the click, but only after the window has been activated.*/
};

typedef UInt32                          ClickActivationResult;
/*------------------------------------------------------------------------------------------------------*/
/*  o Common data tags for Get/SetControlData                                                           */
/*------------------------------------------------------------------------------------------------------*/
enum {
    kControlFontStyleTag        = FOUR_CHAR_CODE('font'),
    kControlKeyFilterTag        = FOUR_CHAR_CODE('fltr')
};


/*------------------------------------------------------------------------------------------------------*/
/*  o Control Feature Bits                                                                              */
/*------------------------------------------------------------------------------------------------------*/
enum {
                                                                /* Control feature bits - returned by GetControlFeatures */
    kControlSupportsGhosting    = 1 << 0,
    kControlSupportsEmbedding   = 1 << 1,
    kControlSupportsFocus       = 1 << 2,
    kControlWantsIdle           = 1 << 3,
    kControlWantsActivate       = 1 << 4,
    kControlHandlesTracking     = 1 << 5,
    kControlSupportsDataAccess  = 1 << 6,
    kControlHasSpecialBackground = 1 << 7,
    kControlGetsFocusOnClick    = 1 << 8,
    kControlSupportsCalcBestRect = 1 << 9,
    kControlSupportsLiveFeedback = 1 << 10,
    kControlHasRadioBehavior    = 1 << 11,                      /* Available in Appearance 1.0.1 or later*/
    kControlSupportsDragAndDrop = 1 << 12,                      /* Available in Carbon*/
    kControlAutoToggles         = 1 << 14,                      /* Available in Appearance 1.1 or later*/
    kControlSupportsGetRegion   = 1 << 17,                      /* Available in Appearance 1.1 or later*/
    kControlSupportsFlattening  = 1 << 19,                      /* Available in Carbon*/
    kControlSupportsSetCursor   = 1 << 20,                      /* Available in Carbon*/
    kControlSupportsContextualMenus = 1 << 21,                  /* Available in Carbon*/
    kControlSupportsClickActivation = 1 << 22                   /* Available in Carbon*/
};

/*------------------------------------------------------------------------------------------------------*/
/*  o Control Messages                                                                                  */
/*------------------------------------------------------------------------------------------------------*/
enum {
    drawCntl                    = 0,
    testCntl                    = 1,
    calcCRgns                   = 2,
    initCntl                    = 3,
    dispCntl                    = 4,
    posCntl                     = 5,
    thumbCntl                   = 6,
    dragCntl                    = 7,
    autoTrack                   = 8,
    calcCntlRgn                 = 10,
    calcThumbRgn                = 11,
    drawThumbOutline            = 12,
    kControlMsgDrawGhost        = 13,
    kControlMsgCalcBestRect     = 14,                           /* Calculate best fitting rectangle for control*/
    kControlMsgHandleTracking   = 15,
    kControlMsgFocus            = 16,                           /* param indicates action.*/
    kControlMsgKeyDown          = 17,
    kControlMsgIdle             = 18,
    kControlMsgGetFeatures      = 19,
    kControlMsgSetData          = 20,
    kControlMsgGetData          = 21,
    kControlMsgActivate         = 22,
    kControlMsgSetUpBackground  = 23,
    kControlMsgCalcValueFromPos = 26,
    kControlMsgTestNewMsgSupport = 27,                          /* See if this control supports new messaging*/
    kControlMsgSubValueChanged  = 25,                           /* Available in Appearance 1.0.1 or later*/
    kControlMsgSubControlAdded  = 28,                           /* Available in Appearance 1.0.1 or later*/
    kControlMsgSubControlRemoved = 29,                          /* Available in Appearance 1.0.1 or later*/
    kControlMsgApplyTextColor   = 30,                           /* Available in Appearance 1.1 or later*/
    kControlMsgGetRegion        = 31,                           /* Available in Appearance 1.1 or later*/
    kControlMsgFlatten          = 32,                           /* Available in Carbon. Param is Collection.*/
    kControlMsgSetCursor        = 33,                           /* Available in Carbon. Param is ControlSetCursorRec*/
    kControlMsgDragEnter        = 38,                           /* Available in Carbon. Param is DragRef, result is boolean indicating acceptibility of drag.*/
    kControlMsgDragLeave        = 39,                           /* Available in Carbon. As above.*/
    kControlMsgDragWithin       = 40,                           /* Available in Carbon. As above.*/
    kControlMsgDragReceive      = 41,                           /* Available in Carbon. Param is DragRef, result is OSStatus indicating success/failure.*/
    kControlMsgDisplayDebugInfo = 46,                           /* Available in Carbon on X.*/
    kControlMsgContextualMenuClick = 47,                        /* Available in Carbon. Param is ControlContextualMenuClickRec*/
    kControlMsgGetClickActivation = 48                          /* Available in Carbon. Param is ControlClickActivationRec*/
};

typedef SInt16                          ControlDefProcMessage;
/*--------------------------------------------------------------------------------------*/
/*  o Constants for drawCntl message (passed in param)                                  */
/*--------------------------------------------------------------------------------------*/
enum {
    kDrawControlEntireControl   = 0,
    kDrawControlIndicatorOnly   = 129
};

/*--------------------------------------------------------------------------------------*/
/*  o Constants for dragCntl message (passed in param)                                  */
/*--------------------------------------------------------------------------------------*/
enum {
    kDragControlEntireControl   = 0,
    kDragControlIndicator       = 1
};

/*--------------------------------------------------------------------------------------*/
/*  o Drag Constraint Structure for thumbCntl message (passed in param)                 */
/*--------------------------------------------------------------------------------------*/

struct IndicatorDragConstraint {
    Rect                            limitRect;
    Rect                            slopRect;
    DragConstraint                  axis;
};
typedef struct IndicatorDragConstraint  IndicatorDragConstraint;
typedef IndicatorDragConstraint *       IndicatorDragConstraintPtr;
/*--------------------------------------------------------------------------------------*/
/*  CDEF should return as result of kControlMsgTestNewMsgSupport                        */
/*--------------------------------------------------------------------------------------*/
enum {
    kControlSupportsNewMessages = FOUR_CHAR_CODE(' ok ')
};

/*--------------------------------------------------------------------------------------*/
/*  This structure is passed into a CDEF when called with the kControlMsgHandleTracking */
/*  message                                                                             */
/*--------------------------------------------------------------------------------------*/

struct ControlTrackingRec {
    Point                           startPt;
    EventModifiers                  modifiers;
    ControlActionUPP                action;
};
typedef struct ControlTrackingRec       ControlTrackingRec;

typedef ControlTrackingRec *            ControlTrackingPtr;
/*--------------------------------------------------------------------------------------*/
/* This structure is passed into a CDEF when called with the kControlMsgKeyDown message */
/*--------------------------------------------------------------------------------------*/

struct ControlKeyDownRec {
    EventModifiers                  modifiers;
    SInt16                          keyCode;
    SInt16                          charCode;
};
typedef struct ControlKeyDownRec        ControlKeyDownRec;

typedef ControlKeyDownRec *             ControlKeyDownPtr;
/*--------------------------------------------------------------------------------------*/
/* This structure is passed into a CDEF when called with the kControlMsgGetData or      */
/* kControlMsgSetData message                                                           */
/*--------------------------------------------------------------------------------------*/

struct ControlDataAccessRec {
    ResType                         tag;
    ResType                         part;
    Size                            size;
    Ptr                             dataPtr;
};
typedef struct ControlDataAccessRec     ControlDataAccessRec;

typedef ControlDataAccessRec *          ControlDataAccessPtr;
/*--------------------------------------------------------------------------------------*/
/* This structure is passed into a CDEF when called with the kControlCalcBestRect msg   */
/*--------------------------------------------------------------------------------------*/

struct ControlCalcSizeRec {
    SInt16                          height;
    SInt16                          width;
    SInt16                          baseLine;
};
typedef struct ControlCalcSizeRec       ControlCalcSizeRec;

typedef ControlCalcSizeRec *            ControlCalcSizePtr;
/*--------------------------------------------------------------------------------------*/
/* This structure is passed into a CDEF when called with the kControlMsgSetUpBackground */
/* message is sent                                                                      */
/*--------------------------------------------------------------------------------------*/

struct ControlBackgroundRec {
    SInt16                          depth;
    Boolean                         colorDevice;
};
typedef struct ControlBackgroundRec     ControlBackgroundRec;

typedef ControlBackgroundRec *          ControlBackgroundPtr;
/*--------------------------------------------------------------------------------------*/
/* This structure is passed into a CDEF when called with the kControlMsgApplyTextColor  */
/* message is sent                                                                      */
/*--------------------------------------------------------------------------------------*/

struct ControlApplyTextColorRec {
    SInt16                          depth;
    Boolean                         colorDevice;
    Boolean                         active;
};
typedef struct ControlApplyTextColorRec ControlApplyTextColorRec;

typedef ControlApplyTextColorRec *      ControlApplyTextColorPtr;
/*--------------------------------------------------------------------------------------*/
/* This structure is passed into a CDEF when called with the kControlMsgGetRegion       */
/* message is sent                                                                      */
/*--------------------------------------------------------------------------------------*/

struct ControlGetRegionRec {
    RgnHandle                       region;
    ControlPartCode                 part;
};
typedef struct ControlGetRegionRec      ControlGetRegionRec;

typedef ControlGetRegionRec *           ControlGetRegionPtr;
/*--------------------------------------------------------------------------------------*/
/* This structure is passed into a CDEF when the kControlMsgSetCursor message is sent   */
/* Only sent on Carbon                                                                  */
/*--------------------------------------------------------------------------------------*/

struct ControlSetCursorRec {
    Point                           localPoint;
    EventModifiers                  modifiers;
    Boolean                         cursorWasSet;               /* your CDEF must set this to true if you set the cursor, or false otherwise*/
};
typedef struct ControlSetCursorRec      ControlSetCursorRec;

typedef ControlSetCursorRec *           ControlSetCursorPtr;
/*--------------------------------------------------------------------------------------*/
/* This structure is passed into a CDEF when the kControlMsgContextualMenuClick message */
/* is sent                                                                              */
/* Only sent on Carbon                                                                  */
/*--------------------------------------------------------------------------------------*/

struct ControlContextualMenuClickRec {
    Point                           localPoint;
    Boolean                         menuDisplayed;              /* your CDEF must set this to true if you displayed a menu, or false otherwise*/
};
typedef struct ControlContextualMenuClickRec ControlContextualMenuClickRec;

typedef ControlContextualMenuClickRec * ControlContextualMenuClickPtr;
/*--------------------------------------------------------------------------------------*/
/* This structure is passed into a CDEF when the kControlMsgGetClickActivation message  */
/* is sent                                                                              */
/* Only sent on Carbon                                                                  */
/*--------------------------------------------------------------------------------------*/

struct ControlClickActivationRec {
    Point                           localPoint;
    EventModifiers                  modifiers;
    ClickActivationResult           result;                     /* your CDEF must pass the desired result back*/
};
typedef struct ControlClickActivationRec ControlClickActivationRec;

typedef ControlClickActivationRec *     ControlClickActivationPtr;
/*--------------------------------------------------------------------------------------*/
/*  o 'CDEF' entrypoint                                                                 */
/*--------------------------------------------------------------------------------------*/
typedef CALLBACK_API( SInt32 , ControlDefProcPtr )(SInt16 varCode, ControlRef theControl, ControlDefProcMessage message, SInt32 param);
typedef STACK_UPP_TYPE(ControlDefProcPtr)                       ControlDefUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(ControlDefUPP)
    NewControlDefUPP               (ControlDefProcPtr       userRoutine);

    EXTERN_API(void)
    DisposeControlDefUPP           (ControlDefUPP           userUPP);

    EXTERN_API(SInt32)
    InvokeControlDefUPP            (SInt16                  varCode,
                                    ControlRef              theControl,
                                    ControlDefProcMessage   message,
                                    SInt32                  param,
                                    ControlDefUPP           userUPP);

#else
    enum { uppControlDefProcInfo = 0x00003BB0 };                    /* pascal 4_bytes Func(2_bytes, 4_bytes, 2_bytes, 4_bytes) */
    #define NewControlDefUPP(userRoutine)                           (ControlDefUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlDefProcInfo, GetCurrentArchitecture())
    #define DisposeControlDefUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define InvokeControlDefUPP(varCode, theControl, message, param, userUPP)  (SInt32)CALL_FOUR_PARAMETER_UPP((userUPP), uppControlDefProcInfo, (varCode), (theControl), (message), (param))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewControlDefProc(userRoutine)                          NewControlDefUPP(userRoutine)
#define CallControlDefProc(userRoutine, varCode, theControl, message, param) InvokeControlDefUPP(varCode, theControl, message, param, userRoutine)
/*--------------------------------------------------------------------------------------*/
/*  Control Key Filter                                                                  */
/*--------------------------------------------------------------------------------------*/
/* Certain controls can have a keyfilter attached to them.                              */
/* Definition of a key filter for intercepting and possibly changing keystrokes         */
/* which are destined for a control.                                                    */
/* Key Filter Result Codes                                                          */
/* The filter proc should return one of the two constants below. If                 */
/* kKeyFilterBlockKey is returned, the key is blocked and never makes it to the     */
/* control. If kKeyFilterPassKey is returned, the control receives the keystroke.   */
enum {
    kControlKeyFilterBlockKey   = 0,
    kControlKeyFilterPassKey    = 1
};

typedef SInt16                          ControlKeyFilterResult;
typedef CALLBACK_API( ControlKeyFilterResult , ControlKeyFilterProcPtr )(ControlRef theControl, SInt16 *keyCode, SInt16 *charCode, EventModifiers *modifiers);
typedef STACK_UPP_TYPE(ControlKeyFilterProcPtr)                 ControlKeyFilterUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(ControlKeyFilterUPP)
    NewControlKeyFilterUPP         (ControlKeyFilterProcPtr userRoutine);

    EXTERN_API(void)
    DisposeControlKeyFilterUPP     (ControlKeyFilterUPP     userUPP);

    EXTERN_API(ControlKeyFilterResult)
    InvokeControlKeyFilterUPP      (ControlRef              theControl,
                                    SInt16 *                keyCode,
                                    SInt16 *                charCode,
                                    EventModifiers *        modifiers,
                                    ControlKeyFilterUPP     userUPP);

#else
    enum { uppControlKeyFilterProcInfo = 0x00003FE0 };              /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewControlKeyFilterUPP(userRoutine)                     (ControlKeyFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlKeyFilterProcInfo, GetCurrentArchitecture())
    #define DisposeControlKeyFilterUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeControlKeyFilterUPP(theControl, keyCode, charCode, modifiers, userUPP)  (ControlKeyFilterResult)CALL_FOUR_PARAMETER_UPP((userUPP), uppControlKeyFilterProcInfo, (theControl), (keyCode), (charCode), (modifiers))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewControlKeyFilterProc(userRoutine)                    NewControlKeyFilterUPP(userRoutine)
#define CallControlKeyFilterProc(userRoutine, theControl, keyCode, charCode, modifiers) InvokeControlKeyFilterUPP(theControl, keyCode, charCode, modifiers, userRoutine)

/*--------------------------------------------------------------------------------------*/
/*  o DragGrayRgn Constatns                                                             */
/*                                                                                      */
/*   For DragGrayRgnUPP used in TrackControl()                                          */
/*--------------------------------------------------------------------------------------*/
enum {
    noConstraint                = kNoConstraint,
    hAxisOnly                   = 1,
    vAxisOnly                   = 2
};

/*--------------------------------------------------------------------------------------*/
/*  o Control Creation/Deletion/Persistence                                             */
/*--------------------------------------------------------------------------------------*/
/*  CreateCustomControl is only available as part of Carbon                             */
enum {
    kControlDefProcPtr          = 0                             /* raw proc-ptr based access*/
};

typedef UInt32                          ControlDefType;

struct ControlDefSpec {
    ControlDefType                  defType;
    union {
        ControlDefUPP                   defProc;
    }                                 u;
};
typedef struct ControlDefSpec           ControlDefSpec;
EXTERN_API( OSStatus )
CreateCustomControl             (WindowRef              owningWindow,
                                 const Rect *           contBounds,
                                 const ControlDefSpec * def,
                                 Collection             initData,
                                 ControlRef *           outControl);

EXTERN_API( ControlRef )
NewControl                      (WindowPtr              owningWindow,
                                 const Rect *           boundsRect,
                                 ConstStr255Param       controlTitle,
                                 Boolean                initiallyVisible,
                                 SInt16                 initialValue,
                                 SInt16                 minimumValue,
                                 SInt16                 maximumValue,
                                 SInt16                 procID,
                                 SInt32                 controlReference)                   ONEWORDINLINE(0xA954);

EXTERN_API( ControlRef )
GetNewControl                   (SInt16                 resourceID,
                                 WindowPtr              owningWindow)                       ONEWORDINLINE(0xA9BE);

EXTERN_API( void )
DisposeControl                  (ControlRef             theControl)                         ONEWORDINLINE(0xA955);

EXTERN_API( void )
KillControls                    (WindowPtr              theWindow)                          ONEWORDINLINE(0xA956);

#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
FlattenControl                  (ControlRef             control,
                                 Boolean                flattenSubControls,
                                 Collection             collection);

EXTERN_API( OSStatus )
UnflattenControl                (WindowRef              window,
                                 Collection             collection,
                                 ControlRef *           outControl);

#endif  /* CALL_NOT_IN_CARBON */

/*--------------------------------------------------------------------------------------*/
/*  o Control Definition Registration                                                   */
/*--------------------------------------------------------------------------------------*/
/*  In GetNewControl on Carbon, the Control Manager needs to know how to map the procID */
/*  to a ControlDefSpec. With RegisterControlDefinition, your application can inform    */
/*  the Control Manager which ControlDefSpec to call when it sees a request to use a    */
/*  'CDEF' of a particular resource ID.                                                 */
/*                                                                                      */
/*  Since custom control definitions receive their initialization data in a Collection, */
/*  you must also provide a procedure to convert the bounds, min, max, and other fields */
/*  from the 'CNTL' resource into a Collection. If you don't provide a conversion proc, */
/*  your control will receive an empty collection when it is sent the initialization    */
/*  message.                                                                                */
/*  If you want the value, min, visibility, etc. to be given to the control, you must   */
/*  add the appropriate tagged data to the collection. See the Control Collection Tags  */
/*  above.                                                                              */
/*  RegisterControlDefinition is only available as part of Carbon                       */
typedef CALLBACK_API( OSStatus , ControlCNTLToCollectionProcPtr )(const Rect *bounds, SInt16 value, Boolean visible, SInt16 max, SInt16 min, SInt16 procID, SInt32 refCon, ConstStr255Param title, Collection collection);
typedef STACK_UPP_TYPE(ControlCNTLToCollectionProcPtr)          ControlCNTLToCollectionUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(ControlCNTLToCollectionUPP)
    NewControlCNTLToCollectionUPP    (ControlCNTLToCollectionProcPtr userRoutine);

    EXTERN_API(void)
    DisposeControlCNTLToCollectionUPP    (ControlCNTLToCollectionUPP userUPP);

    EXTERN_API(OSStatus)
    InvokeControlCNTLToCollectionUPP    (const Rect *       bounds,
                                    SInt16                  value,
                                    Boolean                 visible,
                                    SInt16                  max,
                                    SInt16                  min,
                                    SInt16                  procID,
                                    SInt32                  refCon,
                                    ConstStr255Param        title,
                                    Collection              collection,
                                    ControlCNTLToCollectionUPP userUPP);

#else
    enum { uppControlCNTLToCollectionProcInfo = 0x00FEA6F0 };       /* pascal 4_bytes Func(4_bytes, 2_bytes, 1_byte, 2_bytes, 2_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewControlCNTLToCollectionUPP(userRoutine)              (ControlCNTLToCollectionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppControlCNTLToCollectionProcInfo, GetCurrentArchitecture())
    #define DisposeControlCNTLToCollectionUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define InvokeControlCNTLToCollectionUPP(bounds, value, visible, max, min, procID, refCon, title, collection, userUPP)  (OSStatus)CALL_NINE_PARAMETER_UPP((userUPP), uppControlCNTLToCollectionProcInfo, (bounds), (value), (visible), (max), (min), (procID), (refCon), (title), (collection))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewControlCNTLToCollectionProc(userRoutine)             NewControlCNTLToCollectionUPP(userRoutine)
#define CallControlCNTLToCollectionProc(userRoutine, bounds, value, visible, max, min, procID, refCon, title, collection) InvokeControlCNTLToCollectionUPP(bounds, value, visible, max, min, procID, refCon, title, collection, userRoutine)
EXTERN_API( OSStatus )
RegisterControlDefinition       (SInt16                 CDEFResID,
                                 const ControlDefSpec * def,
                                 ControlCNTLToCollectionUPP  conversionProc);



/*--------------------------------------------------------------------------------------*/
/*  o Control Visible State                                                             */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
HiliteControl                   (ControlRef             theControl,
                                 ControlPartCode        hiliteState)                        ONEWORDINLINE(0xA95D);

EXTERN_API( void )
ShowControl                     (ControlRef             theControl)                         ONEWORDINLINE(0xA957);

EXTERN_API( void )
HideControl                     (ControlRef             theControl)                         ONEWORDINLINE(0xA958);


/* following state routines available only with Appearance 1.0 and later*/
EXTERN_API( Boolean )
IsControlActive                 (ControlRef             inControl)                          THREEWORDINLINE(0x303C, 0x0005, 0xAA73);

EXTERN_API( Boolean )
IsControlVisible                (ControlRef             inControl)                          THREEWORDINLINE(0x303C, 0x0006, 0xAA73);

EXTERN_API( OSErr )
ActivateControl                 (ControlRef             inControl)                          THREEWORDINLINE(0x303C, 0x0007, 0xAA73);

EXTERN_API( OSErr )
DeactivateControl               (ControlRef             inControl)                          THREEWORDINLINE(0x303C, 0x0008, 0xAA73);

EXTERN_API( OSErr )
SetControlVisibility            (ControlRef             inControl,
                                 Boolean                inIsVisible,
                                 Boolean                inDoDraw)                           THREEWORDINLINE(0x303C, 0x001E, 0xAA73);


/*--------------------------------------------------------------------------------------*/
/*  o Control Imaging                                                                   */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
DrawControls                    (WindowPtr              theWindow)                          ONEWORDINLINE(0xA969);

EXTERN_API( void )
Draw1Control                    (ControlRef             theControl)                         ONEWORDINLINE(0xA96D);

#define DrawOneControl(theControl) Draw1Control(theControl)

EXTERN_API( void )
UpdateControls                  (WindowPtr              theWindow,
                                 RgnHandle              updateRegion)                       ONEWORDINLINE(0xA953);


/* following imaging routines available only with Appearance 1.0 and later*/
EXTERN_API( OSErr )
GetBestControlRect              (ControlRef             inControl,
                                 Rect *                 outRect,
                                 SInt16 *               outBaseLineOffset)                  THREEWORDINLINE(0x303C, 0x001B, 0xAA73);

EXTERN_API( OSErr )
SetControlFontStyle             (ControlRef             inControl,
                                 const ControlFontStyleRec * inStyle)                       THREEWORDINLINE(0x303C, 0x001C, 0xAA73);

EXTERN_API( void )
DrawControlInCurrentPort        (ControlRef             inControl)                          THREEWORDINLINE(0x303C, 0x0018, 0xAA73);

EXTERN_API( OSErr )
SetUpControlBackground          (ControlRef             inControl,
                                 SInt16                 inDepth,
                                 Boolean                inIsColorDevice)                    THREEWORDINLINE(0x303C, 0x001D, 0xAA73);

/* SetUpControlTextColor is available in Appearance 1.1 or later.*/
EXTERN_API( OSErr )
SetUpControlTextColor           (ControlRef             inControl,
                                 SInt16                 inDepth,
                                 Boolean                inIsColorDevice);


/*--------------------------------------------------------------------------------------*/
/*  o Control Mousing                                                                   */
/*--------------------------------------------------------------------------------------*/
/*
    NOTE ON CONTROL ACTION PROCS

    When using the TrackControl() call when tracking an indicator, the actionProc parameter
    (type ControlActionUPP) should be replaced by a parameter of type DragGrayRgnUPP
    (see Quickdraw.h).

    If, however, you are using the live feedback variants of scroll bars or sliders, you
    can pass a ControlActionUPP in when tracking the indicator as well. This functionality
    is available in Appearance 1.0 or later.
*/
EXTERN_API( ControlPartCode )
TrackControl                    (ControlRef             theControl,
                                 Point                  startPoint,
                                 ControlActionUPP       actionProc) /* can be NULL */       ONEWORDINLINE(0xA968);

EXTERN_API( void )
DragControl                     (ControlRef             theControl,
                                 Point                  startPoint,
                                 const Rect *           limitRect,
                                 const Rect *           slopRect,
                                 DragConstraint         axis)                               ONEWORDINLINE(0xA967);

EXTERN_API( ControlPartCode )
TestControl                     (ControlRef             theControl,
                                 Point                  testPoint)                          ONEWORDINLINE(0xA966);

EXTERN_API( ControlPartCode )
FindControl                     (Point                  testPoint,
                                 WindowPtr              theWindow,
                                 ControlRef *           theControl)                         ONEWORDINLINE(0xA96C);

/* The following mousing routines available only with Appearance 1.0 and later  */
/*                                                                              */
/* HandleControlClick is preferable to TrackControl when running under          */
/* Appearance 1.0 as you can pass in modifiers, which some of the new controls  */
/* use, such as edit text and list boxes.                                       */
EXTERN_API( ControlRef )
FindControlUnderMouse           (Point                  inWhere,
                                 WindowPtr              inWindow,
                                 SInt16 *               outPart)                            THREEWORDINLINE(0x303C, 0x0009, 0xAA73);

EXTERN_API( ControlPartCode )
HandleControlClick              (ControlRef             inControl,
                                 Point                  inWhere,
                                 EventModifiers         inModifiers,
                                 ControlActionUPP       inAction) /* can be NULL */         THREEWORDINLINE(0x303C, 0x000A, 0xAA73);

/* Contextual Menu support in the Control Manager is only available on Carbon.  */
/* If the control didn't display a contextual menu (possibly because the point  */
/* was in a non-interesting part), the menuDisplayed output parameter will be   */
/* false. If the control did display a menu, menuDisplayed will be true.        */
/* This in on Carbon only                                                       */
EXTERN_API( OSStatus )
HandleControlContextualMenuClick (ControlRef            inControl,
                                 Point                  inWhere,
                                 Boolean *              menuDisplayed);

/* Some complex controls (like Data Browser) require proper sequencing of       */
/* window activation and click processing. In some cases, the control might     */
/* want the window to be left inactive yet still handle the click, or vice-     */
/* versa. The GetControlClickActivation routine lets a control client ask the   */
/* control how it wishes to behave for a particular click.                      */
/* This in on Carbon only.                                                      */
EXTERN_API( OSStatus )
GetControlClickActivation       (ControlRef             inControl,
                                 Point                  inWhere,
                                 EventModifiers         inModifiers,
                                 ClickActivationResult * outResult);


/*--------------------------------------------------------------------------------------*/
/*  o Control Events (available only with Appearance 1.0 and later)                     */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( SInt16 )
HandleControlKey                (ControlRef             inControl,
                                 SInt16                 inKeyCode,
                                 SInt16                 inCharCode,
                                 EventModifiers         inModifiers)                        THREEWORDINLINE(0x303C, 0x000B, 0xAA73);

EXTERN_API( void )
IdleControls                    (WindowPtr              inWindow)                           THREEWORDINLINE(0x303C, 0x000C, 0xAA73);



/*--------------------------------------------------------------------------------------*/
/* o Control Mouse Tracking (available with Carbon)                                     */
/*--------------------------------------------------------------------------------------*/
/* The HandleControlSetCursor routine requests that a given control set the cursor to   */
/* something appropriate based on the mouse location.                                   */
/* If the control didn't want to set the cursor (because the point was in a             */
/* non-interesting part), the cursorWasSet output parameter will be false. If the       */
/* control did set the cursor, cursorWasSet will be true.                               */
/* Carbon only.                                                                         */
EXTERN_API( OSStatus )
HandleControlSetCursor          (ControlRef             control,
                                 Point                  localPoint,
                                 EventModifiers         modifiers,
                                 Boolean *              cursorWasSet);

/*--------------------------------------------------------------------------------------*/
/*  o Control Positioning                                                               */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
MoveControl                     (ControlRef             theControl,
                                 SInt16                 h,
                                 SInt16                 v)                                  ONEWORDINLINE(0xA959);

EXTERN_API( void )
SizeControl                     (ControlRef             theControl,
                                 SInt16                 w,
                                 SInt16                 h)                                  ONEWORDINLINE(0xA95C);

/*--------------------------------------------------------------------------------------*/
/*  o Control Title                                                                     */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
SetControlTitle                 (ControlRef             theControl,
                                 ConstStr255Param       title)                              ONEWORDINLINE(0xA95F);

EXTERN_API( void )
GetControlTitle                 (ControlRef             theControl,
                                 Str255                 title)                              ONEWORDINLINE(0xA95E);

/*--------------------------------------------------------------------------------------*/
/*  o Control Value                                                                     */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( SInt16 )
GetControlValue                 (ControlRef             theControl)                         ONEWORDINLINE(0xA960);

EXTERN_API( void )
SetControlValue                 (ControlRef             theControl,
                                 SInt16                 newValue)                           ONEWORDINLINE(0xA963);

EXTERN_API( SInt16 )
GetControlMinimum               (ControlRef             theControl)                         ONEWORDINLINE(0xA961);

EXTERN_API( void )
SetControlMinimum               (ControlRef             theControl,
                                 SInt16                 newMinimum)                         ONEWORDINLINE(0xA964);

EXTERN_API( SInt16 )
GetControlMaximum               (ControlRef             theControl)                         ONEWORDINLINE(0xA962);

EXTERN_API( void )
SetControlMaximum               (ControlRef             theControl,
                                 SInt16                 newMaximum)                         ONEWORDINLINE(0xA965);


/* proportional scrolling/32-bit value support is new with Appearance 1.1*/

EXTERN_API( SInt32 )
GetControlViewSize              (ControlRef             theControl);

EXTERN_API( void )
SetControlViewSize              (ControlRef             theControl,
                                 SInt32                 newViewSize);

EXTERN_API( SInt32 )
GetControl32BitValue            (ControlRef             theControl);

EXTERN_API( void )
SetControl32BitValue            (ControlRef             theControl,
                                 SInt32                 newValue);

EXTERN_API( SInt32 )
GetControl32BitMaximum          (ControlRef             theControl);

EXTERN_API( void )
SetControl32BitMaximum          (ControlRef             theControl,
                                 SInt32                 newMaximum);

EXTERN_API( SInt32 )
GetControl32BitMinimum          (ControlRef             theControl);

EXTERN_API( void )
SetControl32BitMinimum          (ControlRef             theControl,
                                 SInt32                 newMinimum);

/*
    IsValidControlHandle will tell you if the handle you pass in belongs to a control
    the Control Manager knows about. It does not sanity check the data in the control.
*/

EXTERN_API( Boolean )
IsValidControlHandle            (ControlRef             theControl);

/*--------------------------------------------------------------------------------------*/
/* o Control IDs                                                                        */
/* Carbon only.                                                                         */
/*--------------------------------------------------------------------------------------*/

struct ControlID {
    OSType                          signature;
    SInt32                          id;
};
typedef struct ControlID                ControlID;
EXTERN_API( OSStatus )
SetControlID                    (ControlRef             inControl,
                                 const ControlID *      inID);

EXTERN_API( OSStatus )
GetControlID                    (ControlRef             inControl,
                                 ControlID *            outID);

EXTERN_API( OSStatus )
GetControlByID                  (WindowRef              inWindow,
                                 const ControlID *      inID,
                                 ControlRef *           outControl);

/*--------------------------------------------------------------------------------------*/
/* o Properties                                                                         */
/*--------------------------------------------------------------------------------------*/
enum {
    kControlPropertyPersistent  = 0x00000001                    /* whether this property gets saved when flattening the control*/
};

EXTERN_API( OSStatus )
GetControlProperty              (ControlRef             control,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32                 bufferSize,
                                 UInt32 *               actualSize,
                                 void *                 propertyBuffer);

EXTERN_API( OSStatus )
GetControlPropertySize          (ControlRef             control,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32 *               size);

EXTERN_API( OSStatus )
SetControlProperty              (ControlRef             control,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32                 propertySize,
                                 void *                 propertyData);

EXTERN_API( OSStatus )
RemoveControlProperty           (ControlRef             control,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag);

EXTERN_API( OSStatus )
GetControlPropertyAttributes    (ControlRef             control,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32 *               attributes);

EXTERN_API( OSStatus )
ChangeControlPropertyAttributes (ControlRef             control,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32                 attributesToSet,
                                 UInt32                 attributesToClear);

/*--------------------------------------------------------------------------------------*/
/*  o Control Regions (Appearance 1.1 or later)                                         */
/*                                                                                      */
/*  See the discussion on meta-parts in this header for more information                */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSStatus )
GetControlRegion                (ControlRef             inControl,
                                 ControlPartCode        inPart,
                                 RgnHandle              outRegion);



/*--------------------------------------------------------------------------------------*/
/*  o Control Variant                                                                   */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( ControlVariant )
GetControlVariant               (ControlRef             theControl)                         ONEWORDINLINE(0xA809);

/*--------------------------------------------------------------------------------------*/
/*  o Control Action                                                                    */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
SetControlAction                (ControlRef             theControl,
                                 ControlActionUPP       actionProc)                         ONEWORDINLINE(0xA96B);

EXTERN_API( ControlActionUPP )
GetControlAction                (ControlRef             theControl)                         ONEWORDINLINE(0xA96A);

/*--------------------------------------------------------------------------------------*/
/* o Control Accessors                                                                  */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
SetControlReference             (ControlRef             theControl,
                                 SInt32                 data)                               ONEWORDINLINE(0xA95B);

EXTERN_API( SInt32 )
GetControlReference             (ControlRef             theControl)                         ONEWORDINLINE(0xA95A);

#if !OPAQUE_TOOLBOX_STRUCTS
EXTERN_API( Boolean )
GetAuxiliaryControlRecord       (ControlRef             theControl,
                                 AuxCtlHandle *         acHndl)                             ONEWORDINLINE(0xAA44);

#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

#if CALL_NOT_IN_CARBON
EXTERN_API( void )
SetControlColor                 (ControlRef             theControl,
                                 CCTabHandle            newColorTable)                      ONEWORDINLINE(0xAA43);

/*--------------------------------------------------------------------------------------*/
/*  o Control Hierarchy (Appearance 1.0 and later only)                                 */
/*--------------------------------------------------------------------------------------*/
#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( SInt32 )
SendControlMessage              (ControlRef             inControl,
                                 SInt16                 inMessage,
                                 SInt32                 inParam)                            THREEWORDINLINE(0x303C, 0xFFFE, 0xAA73);

EXTERN_API( OSErr )
DumpControlHierarchy            (WindowPtr              inWindow,
                                 const FSSpec *         inDumpFile)                         THREEWORDINLINE(0x303C, 0xFFFF, 0xAA73);

EXTERN_API( OSErr )
CreateRootControl               (WindowPtr              inWindow,
                                 ControlRef *           outControl)                         THREEWORDINLINE(0x303C, 0x0001, 0xAA73);

EXTERN_API( OSErr )
GetRootControl                  (WindowPtr              inWindow,
                                 ControlRef *           outControl)                         THREEWORDINLINE(0x303C, 0x0002, 0xAA73);

EXTERN_API( OSErr )
EmbedControl                    (ControlRef             inControl,
                                 ControlRef             inContainer)                        THREEWORDINLINE(0x303C, 0x0003, 0xAA73);

EXTERN_API( OSErr )
AutoEmbedControl                (ControlRef             inControl,
                                 WindowPtr              inWindow)                           THREEWORDINLINE(0x303C, 0x0004, 0xAA73);

EXTERN_API( OSErr )
GetSuperControl                 (ControlRef             inControl,
                                 ControlRef *           outParent)                          THREEWORDINLINE(0x303C, 0x0015, 0xAA73);

EXTERN_API( OSErr )
CountSubControls                (ControlRef             inControl,
                                 UInt16 *               outNumChildren)                     THREEWORDINLINE(0x303C, 0x0016, 0xAA73);

EXTERN_API( OSErr )
GetIndexedSubControl            (ControlRef             inControl,
                                 UInt16                 inIndex,
                                 ControlRef *           outSubControl)                      THREEWORDINLINE(0x303C, 0x0017, 0xAA73);

EXTERN_API( OSErr )
SetControlSupervisor            (ControlRef             inControl,
                                 ControlRef             inBoss)                             THREEWORDINLINE(0x303C, 0x001A, 0xAA73);



/*--------------------------------------------------------------------------------------*/
/*  o Keyboard Focus (available only with Appearance 1.0 and later)                     */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSErr )
GetKeyboardFocus                (WindowPtr              inWindow,
                                 ControlRef *           outControl)                         THREEWORDINLINE(0x303C, 0x000D, 0xAA73);

EXTERN_API( OSErr )
SetKeyboardFocus                (WindowPtr              inWindow,
                                 ControlRef             inControl,
                                 ControlFocusPart       inPart)                             THREEWORDINLINE(0x303C, 0x000E, 0xAA73);

EXTERN_API( OSErr )
AdvanceKeyboardFocus            (WindowPtr              inWindow)                           THREEWORDINLINE(0x303C, 0x000F, 0xAA73);

EXTERN_API( OSErr )
ReverseKeyboardFocus            (WindowPtr              inWindow)                           THREEWORDINLINE(0x303C, 0x0010, 0xAA73);

EXTERN_API( OSErr )
ClearKeyboardFocus              (WindowPtr              inWindow)                           THREEWORDINLINE(0x303C, 0x0019, 0xAA73);



/*--------------------------------------------------------------------------------------*/
/*  o Control Data (available only with Appearance 1.0 and later)                       */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( OSErr )
GetControlFeatures              (ControlRef             inControl,
                                 UInt32 *               outFeatures)                        THREEWORDINLINE(0x303C, 0x0011, 0xAA73);

EXTERN_API( OSErr )
SetControlData                  (ControlRef             inControl,
                                 ControlPartCode        inPart,
                                 ResType                inTagName,
                                 Size                   inSize,
                                 void *                 inData)                             THREEWORDINLINE(0x303C, 0x0012, 0xAA73);

EXTERN_API( OSErr )
GetControlData                  (ControlRef             inControl,
                                 ControlPartCode        inPart,
                                 ResType                inTagName,
                                 Size                   inBufferSize,
                                 void *                 inBuffer,
                                 Size *                 outActualSize)                      THREEWORDINLINE(0x303C, 0x0013, 0xAA73);

EXTERN_API( OSErr )
GetControlDataSize              (ControlRef             inControl,
                                 ControlPartCode        inPart,
                                 ResType                inTagName,
                                 Size *                 outMaxSize)                         THREEWORDINLINE(0x303C, 0x0014, 0xAA73);


/*--------------------------------------------------------------------------------------*/
/*  o Control Drag & Drop                                                               */
/*      Carbon only.                                                                    */
/*--------------------------------------------------------------------------------------*/
/* some simple redefinitions of the messages found in the Drag header*/
enum {
    kDragTrackingEnterControl   = kDragTrackingEnterWindow,
    kDragTrackingInControl      = kDragTrackingInWindow,
    kDragTrackingLeaveControl   = kDragTrackingLeaveWindow
};

EXTERN_API( OSStatus )
HandleControlDragTracking       (ControlRef             inControl,
                                 DragTrackingMessage    inMessage,
                                 DragReference          inDrag,
                                 Boolean *              outLikesDrag);

EXTERN_API( OSStatus )
HandleControlDragReceive        (ControlRef             inControl,
                                 DragReference          inDrag);

EXTERN_API( OSStatus )
SetControlDragTrackingEnabled   (ControlRef             theControl,
                                 Boolean                tracks);

EXTERN_API( OSStatus )
IsControlDragTrackingEnabled    (ControlRef             theControl,
                                 Boolean *              tracks);

EXTERN_API( OSStatus )
SetAutomaticControlDragTrackingEnabledForWindow (WindowPtr  theWindow,
                                 Boolean                tracks);

EXTERN_API( OSStatus )
IsAutomaticControlDragTrackingEnabledForWindow (WindowPtr  theWindow,
                                 Boolean *              tracks);


#if !TARGET_OS_MAC
/*--------------------------------------------------------------------------------------*/
/*  o QuickTime 3.0 Win32/unix notification mechanism                                   */
/*--------------------------------------------------------------------------------------*/
/* Proc used to notify window that something happened to the control*/
typedef CALLBACK_API_C( void , ControlNotificationProcPtr )(WindowPtr theWindow, ControlRef theControl, ControlNotification notification, long param1, long param2);
/*
   Proc used to prefilter events before handled by control.  A client of a control calls
   CTRLSetPreFilterProc() to have the control call this proc before handling the event.
   If the proc returns TRUE, the control can go ahead and handle the event.
*/
typedef CALLBACK_API_C( Boolean , PreFilterEventProc )(ControlRef theControl, EventRecord *theEvent);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( long )
GetControlComponentInstance     (ControlRef             theControl);

EXTERN_API_C( ControlRef )
GetControlHandleFromCookie      (long                   cookie);

EXTERN_API_C( void )
SetControlDefProc               (short                  resID,
                                 ControlDefProcPtr      proc);

#endif  /* CALL_NOT_IN_CARBON */


typedef ControlNotificationProcPtr      ControlNotificationUPP;
#endif  /* !TARGET_OS_MAC */

/*--------------------------------------------------------------------------------------*/
/*  o C Glue                                                                            */
/*--------------------------------------------------------------------------------------*/
#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
dragcontrol                     (ControlRef             theControl,
                                 Point *                startPt,
                                 const Rect *           limitRect,
                                 const Rect *           slopRect,
                                 short                  axis);

EXTERN_API_C( ControlRef )
newcontrol                      (WindowPtr              theWindow,
                                 const Rect *           boundsRect,
                                 const char *           title,
                                 Boolean                visible,
                                 short                  value,
                                 short                  min,
                                 short                  max,
                                 short                  procID,
                                 long                   refCon);

EXTERN_API_C( short )
findcontrol                     (Point *                thePoint,
                                 WindowPtr              theWindow,
                                 ControlRef *           theControl);

EXTERN_API_C( void )
getcontroltitle                 (ControlRef             theControl,
                                 char *                 title);

EXTERN_API_C( void )
setcontroltitle                 (ControlRef             theControl,
                                 const char *           title);

EXTERN_API_C( short )
trackcontrol                    (ControlRef             theControl,
                                 Point *                thePoint,
                                 ControlActionUPP       actionProc);

EXTERN_API_C( short )
testcontrol                     (ControlRef             theControl,
                                 Point *                thePt);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if OLDROUTINENAMES
/*--------------------------------------------------------------------------------------*/
/*  o OLDROUTINENAMES                                                                   */
/*--------------------------------------------------------------------------------------*/
enum {
    useWFont                    = kControlUsesOwningWindowsFontVariant
};

enum {
    inThumb                     = kControlIndicatorPart,
    kNoHiliteControlPart        = kControlNoPart,
    kInIndicatorControlPart     = kControlIndicatorPart,
    kReservedControlPart        = kControlDisabledPart,
    kControlInactiveControlPart = kControlInactivePart
};


#define SetCTitle(theControl, title) SetControlTitle(theControl, title)
#define GetCTitle(theControl, title) GetControlTitle(theControl, title)
#define UpdtControl(theWindow, updateRgn) UpdateControls(theWindow, updateRgn)
#define SetCtlValue(theControl, theValue) SetControlValue(theControl, theValue)
#define GetCtlValue(theControl) GetControlValue(theControl)
#define SetCtlMin(theControl, minValue) SetControlMinimum(theControl, minValue)
#define GetCtlMin(theControl) GetControlMinimum(theControl)
#define SetCtlMax(theControl, maxValue) SetControlMaximum(theControl, maxValue)
#define GetCtlMax(theControl) GetControlMaximum(theControl)
#define GetAuxCtl(theControl, acHndl) GetAuxiliaryControlRecord(theControl, acHndl)
#define SetCRefCon(theControl, data) SetControlReference(theControl, data)
#define GetCRefCon(theControl) GetControlReference(theControl)
#define SetCtlAction(theControl, actionProc) SetControlAction(theControl, actionProc)
#define GetCtlAction(theControl) GetControlAction(theControl)
#define SetCtlColor(theControl, newColorTable) SetControlColor(theControl, newColorTable)
#define GetCVariant(theControl) GetControlVariant(theControl)
#define getctitle(theControl, title) getcontroltitle(theControl, title)
#define setctitle(theControl, title) setcontroltitle(theControl, title)
#endif  /* OLDROUTINENAMES */

#if ACCESSOR_CALLS_ARE_FUNCTIONS
/* Getters */
EXTERN_API( Rect *)
GetControlBounds                (ControlRef             control,
                                 Rect *                 bounds);

EXTERN_API( Boolean )
IsControlHilited                (ControlRef             control);

EXTERN_API( UInt16 )
GetControlHilite                (ControlRef             control);

EXTERN_API( WindowPtr )
GetControlOwner                 (ControlRef             control);

EXTERN_API( Handle )
GetControlDataHandle            (ControlRef             control);

EXTERN_API( MenuHandle )
GetControlPopupMenuHandle       (ControlRef             control);

EXTERN_API( short )
GetControlPopupMenuID           (ControlRef             control);

/* Setters */
EXTERN_API( void )
SetControlDataHandle            (ControlRef             control,
                                 Handle                 dataHandle);

EXTERN_API( void )
SetControlBounds                (ControlRef             control,
                                 const Rect *           bounds);

EXTERN_API( void )
SetControlPopupMenuHandle       (ControlRef             control,
                                 MenuHandle             popupMenu);

EXTERN_API( void )
SetControlPopupMenuID           (ControlRef             control,
                                 short                  menuID);

#endif  /* ACCESSOR_CALLS_ARE_FUNCTIONS */

#if !OPAQUE_TOOLBOX_STRUCTS && !ACCESSOR_CALLS_ARE_FUNCTIONS
#define GetControlListFromWindow(theWindow)     ( *(ControlRef *) (((UInt8 *) theWindow) + sizeof(GrafPort) + 0x20))
#define GetControlOwningWindowControlList(theWindow)        ( *(ControlRef *) (((UInt8 *) theWindow) + sizeof(GrafPort) + 0x20))
#endif  /* !OPAQUE_TOOLBOX_STRUCTS && !ACCESSOR_CALLS_ARE_FUNCTIONS */



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

#endif /* __CONTROLS__ */

