/*
     File:       MacWindows.h
 
     Contains:   Window Manager Interfaces
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1997-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MACWINDOWS__
#define __MACWINDOWS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __ALIASES__
#include <Aliases.h>
#endif

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif

#ifndef __COLLECTIONS__
#include <Collections.h>
#endif

#ifndef __DRAG__
#include <Drag.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __QDOFFSCREEN__
#include <QDOffscreen.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __TEXTCOMMON__
#include <TextCommon.h>
#endif

#ifndef __ICONS__
#include <Icons.h>
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

/*                                                                                                      */
/* Current documentation for the Mac OS Window Manager is available on the web:                         */
/*  <http://developer.apple.com/techpubs/macos8/HumanInterfaceToolbox/WindowManager/windowmanager.html> */
/*                                                                                                      */
/*--------------------------------------------------------------------------------------*/
/* o Property Types                                                                     */
/*--------------------------------------------------------------------------------------*/
typedef OSType                          PropertyCreator;
typedef OSType                          PropertyTag;
/*--------------------------------------------------------------------------------------*/
/* o Window Classes                                                                     */
/*--------------------------------------------------------------------------------------*/

typedef UInt32 WindowClass;
enum {
    kAlertWindowClass           = 1L,                           /* "I need your attention now."*/
    kMovableAlertWindowClass    = 2L,                           /* "I need your attention now, but I'm kind enough to let you switch out of this app to do other things."*/
    kModalWindowClass           = 3L,                           /* system modal, not draggable*/
    kMovableModalWindowClass    = 4L,                           /* application modal, draggable*/
    kFloatingWindowClass        = 5L,                           /* floats above all other application windows*/
    kDocumentWindowClass        = 6L,                           /* document windows*/
    kDesktopWindowClass         = 7L,                           /* desktop window (usually only one of these exists) - OS X only in CarbonLib 1.0*/
    kUtilityWindowClass         = 8L,                           /* Available in CarbonLib 1.1 and later, and in Mac OS X*/
    kHelpWindowClass            = 10L,                          /* Available in CarbonLib 1.1 and later, and in Mac OS X*/
    kSheetWindowClass           = 11L,                          /* Available in CarbonLib 1.3 and later, and in Mac OS X*/
    kToolbarWindowClass         = 12L,                          /* Available in CarbonLib 1.1 and later, and in Mac OS X*/
    kPlainWindowClass           = 13L,                          /* Available in CarbonLib 1.2.5 and later, and Mac OS X*/
    kOverlayWindowClass         = 14L,                          /* Available in Mac OS X*/
    kSheetAlertWindowClass      = 15L,                          /* Available in CarbonLib 1.3 and later, and in Mac OS X 10.1 and later*/
    kAltPlainWindowClass        = 16L,                          /* Available in CarbonLib 1.3 and later, and in Mac OS X 10.1 and later*/
    kDrawerWindowClass          = 20L,                          /* Available in Mac OS X 10.2 or later*/
    kAllWindowClasses           = (unsigned long)0xFFFFFFFF     /* for use with GetFrontWindowOfClass, FindWindowOfClass, GetNextWindowOfClass*/
};


/*--------------------------------------------------------------------------------------*/
/* o Window Attributes                                                                  */
/*--------------------------------------------------------------------------------------*/

typedef UInt32                          WindowAttributes;
enum {
    kWindowNoAttributes         = 0L,                           /* no attributes*/
    kWindowCloseBoxAttribute    = (1L << 0),                    /* window has a close box*/
    kWindowHorizontalZoomAttribute = (1L << 1),                 /* window has horizontal zoom box*/
    kWindowVerticalZoomAttribute = (1L << 2),                   /* window has vertical zoom box*/
    kWindowFullZoomAttribute    = (kWindowVerticalZoomAttribute | kWindowHorizontalZoomAttribute),
    kWindowCollapseBoxAttribute = (1L << 3),                    /* window has a collapse box*/
    kWindowResizableAttribute   = (1L << 4),                    /* window is resizable*/
    kWindowSideTitlebarAttribute = (1L << 5),                   /* window wants a titlebar on the side    (floating window class only)*/
    kWindowNoUpdatesAttribute   = (1L << 16),                   /* this window receives no update events*/
    kWindowNoActivatesAttribute = (1L << 17),                   /* this window receives no activate events*/
    kWindowNoBufferingAttribute = (1L << 20),                   /* this window is not buffered (Mac OS X only)*/
    kWindowStandardDocumentAttributes = (kWindowCloseBoxAttribute | kWindowFullZoomAttribute | kWindowCollapseBoxAttribute | kWindowResizableAttribute),
    kWindowStandardFloatingAttributes = (kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute)
};

/*--------------------------------------------------------------------------------------*/
/* o Window Definition Type                                                             */
/*--------------------------------------------------------------------------------------*/
enum {
    kWindowDefProcType          = FOUR_CHAR_CODE('WDEF')
};

/*--------------------------------------------------------------------------------------*/
/* o Mac OS 7.5 Window Definition Resource IDs                                          */
/*--------------------------------------------------------------------------------------*/
enum {
    kStandardWindowDefinition   = 0,                            /* for document windows and dialogs*/
    kRoundWindowDefinition      = 1,                            /* old da-style window*/
    kFloatingWindowDefinition   = 124                           /* for floating windows*/
};

/*--------------------------------------------------------------------------------------*/
/* o Variant Codes                                                                      */
/*--------------------------------------------------------------------------------------*/
enum {
                                                                /* for use with kStandardWindowDefinition */
    kDocumentWindowVariantCode  = 0,
    kModalDialogVariantCode     = 1,
    kPlainDialogVariantCode     = 2,
    kShadowDialogVariantCode    = 3,
    kMovableModalDialogVariantCode = 5,
    kAlertVariantCode           = 7,
    kMovableAlertVariantCode    = 9,                            /* for use with kFloatingWindowDefinition */
    kSideFloaterVariantCode     = 8
};


/*--------------------------------------------------------------------------------------*/
/* o DefProc IDs                                                                        */
/*--------------------------------------------------------------------------------------*/
enum {
                                                                /* classic ids */
    documentProc                = 0,
    dBoxProc                    = 1,
    plainDBox                   = 2,
    altDBoxProc                 = 3,
    noGrowDocProc               = 4,
    movableDBoxProc             = 5,
    zoomDocProc                 = 8,
    zoomNoGrow                  = 12,
    rDocProc                    = 16,                           /* floating window defproc ids */
    floatProc                   = 1985,
    floatGrowProc               = 1987,
    floatZoomProc               = 1989,
    floatZoomGrowProc           = 1991,
    floatSideProc               = 1993,
    floatSideGrowProc           = 1995,
    floatSideZoomProc           = 1997,
    floatSideZoomGrowProc       = 1999
};

enum {
                                                                /* Resource IDs for theme-savvy window defprocs */
    kWindowDocumentDefProcResID = 64,
    kWindowDialogDefProcResID   = 65,
    kWindowUtilityDefProcResID  = 66,
    kWindowUtilitySideTitleDefProcResID = 67
};

enum {
                                                                /* Proc IDs for theme-savvy windows */
    kWindowDocumentProc         = 1024,
    kWindowGrowDocumentProc     = 1025,
    kWindowVertZoomDocumentProc = 1026,
    kWindowVertZoomGrowDocumentProc = 1027,
    kWindowHorizZoomDocumentProc = 1028,
    kWindowHorizZoomGrowDocumentProc = 1029,
    kWindowFullZoomDocumentProc = 1030,
    kWindowFullZoomGrowDocumentProc = 1031
};


enum {
                                                                /* Proc IDs for theme-savvy dialogs */
    kWindowPlainDialogProc      = 1040,
    kWindowShadowDialogProc     = 1041,
    kWindowModalDialogProc      = 1042,
    kWindowMovableModalDialogProc = 1043,
    kWindowAlertProc            = 1044,
    kWindowMovableAlertProc     = 1045
};


enum {
                                                                /* procIDs available from Mac OS 8.1 (Appearance 1.0.1) forward */
    kWindowMovableModalGrowProc = 1046
};


enum {
                                                                /* Proc IDs for top title bar theme-savvy floating windows */
    kWindowFloatProc            = 1057,
    kWindowFloatGrowProc        = 1059,
    kWindowFloatVertZoomProc    = 1061,
    kWindowFloatVertZoomGrowProc = 1063,
    kWindowFloatHorizZoomProc   = 1065,
    kWindowFloatHorizZoomGrowProc = 1067,
    kWindowFloatFullZoomProc    = 1069,
    kWindowFloatFullZoomGrowProc = 1071
};


enum {
                                                                /* Proc IDs for side title bar theme-savvy floating windows */
    kWindowFloatSideProc        = 1073,
    kWindowFloatSideGrowProc    = 1075,
    kWindowFloatSideVertZoomProc = 1077,
    kWindowFloatSideVertZoomGrowProc = 1079,
    kWindowFloatSideHorizZoomProc = 1081,
    kWindowFloatSideHorizZoomGrowProc = 1083,
    kWindowFloatSideFullZoomProc = 1085,
    kWindowFloatSideFullZoomGrowProc = 1087
};

/*--------------------------------------------------------------------------------------*/
/* o System 7 Window Positioning Constants                                              */
/*                                                                                      */
/* Passed into StandardAlert and used in 'WIND', 'DLOG', and 'ALRT' templates           */
/* StandardAlert uses zero to specify the default position. Other calls use zero to     */
/* specify "no position".  Do not pass these constants to RepositionWindow.  Do not     */
/* store these constants in the BasicWindowDescription of a 'wind' resource.            */
/*--------------------------------------------------------------------------------------*/

enum {
    kWindowNoPosition           = 0x0000,
    kWindowDefaultPosition      = 0x0000,                       /* used by StandardAlert*/
    kWindowCenterMainScreen     = 0x280A,
    kWindowAlertPositionMainScreen = 0x300A,
    kWindowStaggerMainScreen    = 0x380A,
    kWindowCenterParentWindow   = 0xA80A,
    kWindowAlertPositionParentWindow = 0xB00A,
    kWindowStaggerParentWindow  = 0xB80A,
    kWindowCenterParentWindowScreen = 0x680A,
    kWindowAlertPositionParentWindowScreen = 0x700A,
    kWindowStaggerParentWindowScreen = 0x780A
};


/*--------------------------------------------------------------------------------------*/
/* o Window Positioning Methods                                                         */
/*                                                                                      */
/* Positioning methods passed to RepositionWindow.                                      */
/* Do not use them in WIND, ALRT, DLOG templates.                                       */
/* Do not confuse these constants with the constants above                              */
/*--------------------------------------------------------------------------------------*/

typedef UInt32 WindowPositionMethod;
enum {
    kWindowCenterOnMainScreen   = 0x00000001,
    kWindowCenterOnParentWindow = 0x00000002,
    kWindowCenterOnParentWindowScreen = 0x00000003,
    kWindowCascadeOnMainScreen  = 0x00000004,
    kWindowCascadeOnParentWindow = 0x00000005,
    kWindowCascadeOnParentWindowScreen = 0x00000006,
    kWindowAlertPositionOnMainScreen = 0x00000007,
    kWindowAlertPositionOnParentWindow = 0x00000008,
    kWindowAlertPositionOnParentWindowScreen = 0x00000009
};


/*--------------------------------------------------------------------------------------*/
/* o GetWindowRegion Types                                                              */
/*--------------------------------------------------------------------------------------*/
typedef UInt16                          WindowRegionCode;
enum {
                                                                /* Region values to pass into GetWindowRegion & GetWindowBounds */
    kWindowTitleBarRgn          = 0,
    kWindowTitleTextRgn         = 1,
    kWindowCloseBoxRgn          = 2,
    kWindowZoomBoxRgn           = 3,
    kWindowDragRgn              = 5,
    kWindowGrowRgn              = 6,
    kWindowCollapseBoxRgn       = 7,
    kWindowTitleProxyIconRgn    = 8,                            /* Mac OS 8.5 forward*/
    kWindowStructureRgn         = 32,
    kWindowContentRgn           = 33,                           /* Content area of the window; empty when the window is collapsed*/
    kWindowUpdateRgn            = 34,                           /* Carbon forward*/
    kWindowGlobalPortRgn        = 40                            /* Carbon forward - bounds of the window's port in global coordinates; not affected by CollapseWindow*/
};

/* GetWindowRegionRec - a pointer to this is passed in WDEF param for kWindowMsgGetRegion*/

struct GetWindowRegionRec {
    RgnHandle                       winRgn;
    WindowRegionCode                regionCode;
};
typedef struct GetWindowRegionRec       GetWindowRegionRec;

typedef GetWindowRegionRec *            GetWindowRegionPtr;
typedef GetWindowRegionRec *            GetWindowRegionRecPtr;
/*--------------------------------------------------------------------------------------*/
/* o WDEF Message Types                                                                 */
/*--------------------------------------------------------------------------------------*/
/*
   SetupWindowProxyDragImageRec - setup the proxy icon drag image
   Both regions are allocated and disposed by the Window Manager.
   The GWorld is disposed of by the Window Manager, but the WDEF must allocate
   it.  See Technote on Drag Manager 1.1 additions for more information and sample code for
   setting up drag images.
*/


struct SetupWindowProxyDragImageRec {
    GWorldPtr                       imageGWorld;                /* locked GWorld containing the drag image - output - can be NULL*/
    RgnHandle                       imageRgn;                   /* image clip region, contains the portion of the image which gets blitted to screen - preallocated output - if imageGWorld is NULL, this is ignored*/
    RgnHandle                       outlineRgn;                 /* the outline region used on shallow monitors - preallocated output - must always be non-empty*/
};
typedef struct SetupWindowProxyDragImageRec SetupWindowProxyDragImageRec;
/* MeasureWindowTitleRec - a pointer to this is passed in WDEF param for kWindowMsgMeasureTitle*/

struct MeasureWindowTitleRec {
                                                                /* output parameters (filled in by the WDEF)*/
    SInt16                          fullTitleWidth;             /* text + proxy icon width*/
    SInt16                          titleTextWidth;             /* text width*/
};
typedef struct MeasureWindowTitleRec    MeasureWindowTitleRec;
typedef MeasureWindowTitleRec *         MeasureWindowTitleRecPtr;

/*--------------------------------------------------------------------------------------*/
/* o Standard Window Kinds                                                              */
/*--------------------------------------------------------------------------------------*/
enum {
    dialogKind                  = 2,
    userKind                    = 8,
    kDialogWindowKind           = 2,
    kApplicationWindowKind      = 8
};


/*--------------------------------------------------------------------------------------*/
/* o FindWindow Result Codes                                                            */
/*--------------------------------------------------------------------------------------*/

typedef SInt16 WindowPartCode;
enum {
    inDesk                      = 0,
    inNoWindow                  = 0,
    inMenuBar                   = 1,
    inSysWindow                 = 2,
    inContent                   = 3,
    inDrag                      = 4,
    inGrow                      = 5,
    inGoAway                    = 6,
    inZoomIn                    = 7,
    inZoomOut                   = 8,
    inCollapseBox               = 11,                           /* Mac OS 8.0 forward*/
    inProxyIcon                 = 12                            /* Mac OS 8.5 forward*/
};


/*--------------------------------------------------------------------------------------*/
/* o Window Definition Hit Test Result Codes                                            */
/*--------------------------------------------------------------------------------------*/

typedef SInt16 WindowDefPartCode;
enum {
    wNoHit                      = 0,
    wInContent                  = 1,
    wInDrag                     = 2,
    wInGrow                     = 3,
    wInGoAway                   = 4,
    wInZoomIn                   = 5,
    wInZoomOut                  = 6,
    wInCollapseBox              = 9,                            /* Mac OS 8.0 forward*/
    wInProxyIcon                = 10                            /* Mac OS 8.5 forward*/
};

/*--------------------------------------------------------------------------------------*/
/* o Window Definition Messages                                                         */
/*--------------------------------------------------------------------------------------*/

enum {
    kWindowMsgDraw              = 0,
    kWindowMsgHitTest           = 1,
    kWindowMsgCalculateShape    = 2,
    kWindowMsgInitialize        = 3,
    kWindowMsgCleanUp           = 4,
    kWindowMsgDrawGrowOutline   = 5,
    kWindowMsgDrawGrowBox       = 6
};

/* Messages available from Mac OS 8.0 forward*/
enum {
    kWindowMsgGetFeatures       = 7,
    kWindowMsgGetRegion         = 8
};

/* Messages available from Mac OS 8.5 forward*/
enum {
    kWindowMsgDragHilite        = 9,                            /* parameter boolean indicating on or off*/
    kWindowMsgModified          = 10,                           /* parameter boolean indicating saved (false) or modified (true)*/
    kWindowMsgDrawInCurrentPort = 11,                           /* same as kWindowMsgDraw, but must draw in current port*/
    kWindowMsgSetupProxyDragImage = 12,                         /* parameter pointer to SetupWindowProxyDragImageRec*/
    kWindowMsgStateChanged      = 13,                           /* something about the window's state has changed*/
    kWindowMsgMeasureTitle      = 14                            /* measure and return the ideal title width*/
};

/* old names*/
enum {
    wDraw                       = 0,
    wHit                        = 1,
    wCalcRgns                   = 2,
    wNew                        = 3,
    wDispose                    = 4,
    wGrow                       = 5,
    wDrawGIcon                  = 6
};

/*--------------------------------------------------------------------------------------*/
/* o State-changed Flags for kWindowMsgStateChanged                                     */
/*--------------------------------------------------------------------------------------*/
enum {
    kWindowStateTitleChanged    = (1 << 0)
};

/*--------------------------------------------------------------------------------------*/
/* o Window Feature Bits                                                                */
/*--------------------------------------------------------------------------------------*/
enum {
    kWindowCanGrow              = (1 << 0),
    kWindowCanZoom              = (1 << 1),
    kWindowCanCollapse          = (1 << 2),
    kWindowIsModal              = (1 << 3),
    kWindowCanGetWindowRegion   = (1 << 4),
    kWindowIsAlert              = (1 << 5),
    kWindowHasTitleBar          = (1 << 6)
};

/* Feature bits available from Mac OS 8.5 forward*/
enum {
    kWindowSupportsDragHilite   = (1 << 7),                     /* window definition supports kWindowMsgDragHilite*/
    kWindowSupportsModifiedBit  = (1 << 8),                     /* window definition supports kWindowMsgModified*/
    kWindowCanDrawInCurrentPort = (1 << 9),                     /* window definition supports kWindowMsgDrawInCurrentPort*/
    kWindowCanSetupProxyDragImage = (1 << 10),                  /* window definition supports kWindowMsgSetupProxyDragImage*/
    kWindowCanMeasureTitle      = (1 << 11),                    /* window definition supports kWindowMsgMeasureTitle*/
    kWindowWantsDisposeAtProcessDeath = (1 << 12),              /* window definition wants a Dispose message for windows still extant during ExitToShell*/
    kWindowDefSupportsColorGrafPort = 0x40000002                /* window definition does not need the monochrome GrafPort hack during kWindowMsgCalculateShape*/
};


/*--------------------------------------------------------------------------------------*/
/* o Desktop Pattern Resource ID                                                        */
/*--------------------------------------------------------------------------------------*/
enum {
    deskPatID                   = 16
};



/*--------------------------------------------------------------------------------------*/
/* o Window Color Part Codes                                                            */
/*--------------------------------------------------------------------------------------*/
enum {
    wContentColor               = 0,
    wFrameColor                 = 1,
    wTextColor                  = 2,
    wHiliteColor                = 3,
    wTitleBarColor              = 4
};


/*--------------------------------------------------------------------------------------*/
/*  o Region Dragging Constants                                                         */
/*--------------------------------------------------------------------------------------*/

enum {
    kMouseUpOutOfSlop           = (long)0x80008000
};


/*--------------------------------------------------------------------------------------*/
/* o Window Color Table                                                                 */
/*--------------------------------------------------------------------------------------*/

struct WinCTab {
    long                            wCSeed;                     /* reserved */
    short                           wCReserved;                 /* reserved */
    short                           ctSize;                     /* usually 4 for windows */
    ColorSpec                       ctTable[5];
};
typedef struct WinCTab                  WinCTab;

typedef WinCTab *                       WCTabPtr;
typedef WCTabPtr *                      WCTabHandle;
/*--------------------------------------------------------------------------------------*/
/* o WindowRecord                                                                       */
/*--------------------------------------------------------------------------------------*/
#if !OPAQUE_TOOLBOX_STRUCTS
typedef struct WindowRecord             WindowRecord;
typedef WindowRecord *                  WindowPeek;

struct WindowRecord {
    GrafPort                        port;
    short                           windowKind;
    Boolean                         visible;
    Boolean                         hilited;
    Boolean                         goAwayFlag;
    Boolean                         spareFlag;
    RgnHandle                       strucRgn;
    RgnHandle                       contRgn;
    RgnHandle                       updateRgn;
    Handle                          windowDefProc;
    Handle                          dataHandle;
    StringHandle                    titleHandle;
    short                           titleWidth;
    Handle                          controlList;
    WindowPeek                      nextWindow;
    PicHandle                       windowPic;
    long                            refCon;
};

#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

/*--------------------------------------------------------------------------------------*/
/* o Color WindowRecord                                                                 */
/*--------------------------------------------------------------------------------------*/
#if !OPAQUE_TOOLBOX_STRUCTS
typedef struct CWindowRecord            CWindowRecord;
typedef CWindowRecord *                 CWindowPeek;

struct CWindowRecord {
    CGrafPort                       port;
    short                           windowKind;
    Boolean                         visible;
    Boolean                         hilited;
    Boolean                         goAwayFlag;
    Boolean                         spareFlag;
    RgnHandle                       strucRgn;
    RgnHandle                       contRgn;
    RgnHandle                       updateRgn;
    Handle                          windowDefProc;
    Handle                          dataHandle;
    StringHandle                    titleHandle;
    short                           titleWidth;
    Handle                          controlList;
    CWindowPeek                     nextWindow;
    PicHandle                       windowPic;
    long                            refCon;
};

#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

/*--------------------------------------------------------------------------------------*/
/* o AuxWinHandle                                                                       */
/*--------------------------------------------------------------------------------------*/
#if !OPAQUE_TOOLBOX_STRUCTS
typedef struct AuxWinRec                AuxWinRec;
typedef AuxWinRec *                     AuxWinPtr;
typedef AuxWinPtr *                     AuxWinHandle;

struct AuxWinRec {
    AuxWinHandle                    awNext;                     /*handle to next AuxWinRec*/
    WindowPtr                       awOwner;                    /*ptr to window */
    CTabHandle                      awCTable;                   /*color table for this window*/
    Handle                          reserved;
    long                            awFlags;                    /*reserved for expansion*/
    CTabHandle                      awReserved;                 /*reserved for expansion*/
    long                            awRefCon;                   /*user Constant*/
};

#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

/*--------------------------------------------------------------------------------------*/
/*  o BasicWindowDescription                                                            */
/*                                                                                      */
/*  Contains statically-sized basic attributes of the window, for storage in a          */
/*  collection item.                                                                    */
/*--------------------------------------------------------------------------------------*/
/* constants for the version field*/
enum {
    kWindowDefinitionVersionOne = 1,
    kWindowDefinitionVersionTwo = 2
};

/* constants for the stateflags bit field */
enum {
    kWindowIsCollapsedState     = (1 << 0L)
};


struct BasicWindowDescription {
    UInt32                          descriptionSize;            /* sizeof(BasicWindowDescription)*/

    Rect                            windowContentRect;          /* location on screen*/
    Rect                            windowZoomRect;             /* location on screen when zoomed out*/
    UInt32                          windowRefCon;               /* the refcon - __avoid saving stale pointers here__  */
    UInt32                          windowStateFlags;           /* window state bit flags*/
    WindowPositionMethod            windowPositionMethod;       /* method last used by RepositionWindow to position the window (if any)*/

    UInt32                          windowDefinitionVersion;
    union {
        struct {
            SInt16                          windowDefProc;      /* defProc and variant*/
            Boolean                         windowHasCloseBox;
        }                                 versionOne;

        struct {
            WindowClass                     windowClass;        /* the class*/
            WindowAttributes                windowAttributes;   /* the attributes*/
        }                                 versionTwo;

    }                                 windowDefinition;
};
typedef struct BasicWindowDescription   BasicWindowDescription;
/*  the window manager stores the default collection items using these IDs*/
enum {
    kStoredWindowSystemTag      = FOUR_CHAR_CODE('appl'),       /* Only Apple collection items will be of this tag*/
    kStoredBasicWindowDescriptionID = FOUR_CHAR_CODE('sbas'),   /* BasicWindowDescription*/
    kStoredWindowPascalTitleID  = FOUR_CHAR_CODE('s255')        /* pascal title string*/
};

/*--------------------------------------------------------------------------------------*/
/* o Window Class Ordering                                                              */
/*                                                                                      */
/*  Special cases for the "behind" parameter in window creation calls.                  */
/*--------------------------------------------------------------------------------------*/
#define kFirstWindowOfClass ((WindowRef)-1L)
#define kLastWindowOfClass ((WindowRef)0L)
/*--------------------------------------------------------------------------------------*/
/* o Zoom Information Handle                                                            */
/*--------------------------------------------------------------------------------------*/

struct WStateData {
    Rect                            userState;                  /*user zoom state*/
    Rect                            stdState;                   /*standard zoom state*/
};
typedef struct WStateData               WStateData;
typedef WStateData *                    WStateDataPtr;
typedef WStateDataPtr *                 WStateDataHandle;
/*--------------------------------------------------------------------------------------*/
/* o MixedMode & ProcPtrs                                                               */
/*--------------------------------------------------------------------------------------*/
typedef CALLBACK_API( long , WindowDefProcPtr )(short varCode, WindowRef window, short message, long param);
typedef CALLBACK_API( void , DeskHookProcPtr )(Boolean mouseClick, EventRecord *theEvent);
/*
    WARNING: DeskHookProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef CALLBACK_API( OSStatus , WindowPaintProcPtr )(GDHandle device, GrafPtr qdContext, WindowRef window, RgnHandle inClientPaintRgn, RgnHandle outSystemPaintRgn, void *refCon);
typedef STACK_UPP_TYPE(WindowDefProcPtr)                        WindowDefUPP;
typedef REGISTER_UPP_TYPE(DeskHookProcPtr)                      DeskHookUPP;
typedef STACK_UPP_TYPE(WindowPaintProcPtr)                      WindowPaintUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(WindowDefUPP)
    NewWindowDefUPP                (WindowDefProcPtr        userRoutine);

#if CALL_NOT_IN_CARBON
    EXTERN_API(DeskHookUPP)
    NewDeskHookUPP                 (DeskHookProcPtr         userRoutine);

#endif  /* CALL_NOT_IN_CARBON */

    EXTERN_API(WindowPaintUPP)
    NewWindowPaintUPP              (WindowPaintProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeWindowDefUPP            (WindowDefUPP            userUPP);

#if CALL_NOT_IN_CARBON
    EXTERN_API(void)
    DisposeDeskHookUPP             (DeskHookUPP             userUPP);

#endif  /* CALL_NOT_IN_CARBON */

    EXTERN_API(void)
    DisposeWindowPaintUPP          (WindowPaintUPP          userUPP);

    EXTERN_API(long)
    InvokeWindowDefUPP             (short                   varCode,
                                    WindowRef               window,
                                    short                   message,
                                    long                    param,
                                    WindowDefUPP            userUPP);

#if CALL_NOT_IN_CARBON
    EXTERN_API(void)
    InvokeDeskHookUPP              (Boolean                 mouseClick,
                                    EventRecord *           theEvent,
                                    DeskHookUPP             userUPP);

#endif  /* CALL_NOT_IN_CARBON */

    EXTERN_API(OSStatus)
    InvokeWindowPaintUPP           (GDHandle                device,
                                    GrafPtr                 qdContext,
                                    WindowRef               window,
                                    RgnHandle               inClientPaintRgn,
                                    RgnHandle               outSystemPaintRgn,
                                    void *                  refCon,
                                    WindowPaintUPP          userUPP);

#else
    enum { uppWindowDefProcInfo = 0x00003BB0 };                     /* pascal 4_bytes Func(2_bytes, 4_bytes, 2_bytes, 4_bytes) */
    enum { uppDeskHookProcInfo = 0x00130802 };                      /* register no_return_value Func(1_byte:D0, 4_bytes:A0) */
    enum { uppWindowPaintProcInfo = 0x0003FFF0 };                   /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewWindowDefUPP(userRoutine)                            (WindowDefUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppWindowDefProcInfo, GetCurrentArchitecture())
    #define NewDeskHookUPP(userRoutine)                             (DeskHookUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDeskHookProcInfo, GetCurrentArchitecture())
    #define NewWindowPaintUPP(userRoutine)                          (WindowPaintUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppWindowPaintProcInfo, GetCurrentArchitecture())
    #define DisposeWindowDefUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define DisposeDeskHookUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define DisposeWindowPaintUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeWindowDefUPP(varCode, window, message, param, userUPP)  (long)CALL_FOUR_PARAMETER_UPP((userUPP), uppWindowDefProcInfo, (varCode), (window), (message), (param))
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeDeskHookUPP(__D0, __A0, __A1)
    void InvokeDeskHookUPP(Boolean mouseClick, EventRecord * theEvent, DeskHookUPP userUPP) = 0x4E91;
    #else
        #define InvokeDeskHookUPP(mouseClick, theEvent, userUPP)        CALL_TWO_PARAMETER_UPP((userUPP), uppDeskHookProcInfo, (mouseClick), (theEvent))
    #endif
    #define InvokeWindowPaintUPP(device, qdContext, window, inClientPaintRgn, outSystemPaintRgn, refCon, userUPP)  (OSStatus)CALL_SIX_PARAMETER_UPP((userUPP), uppWindowPaintProcInfo, (device), (qdContext), (window), (inClientPaintRgn), (outSystemPaintRgn), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewWindowDefProc(userRoutine)                           NewWindowDefUPP(userRoutine)
#define NewDeskHookProc(userRoutine)                            NewDeskHookUPP(userRoutine)
#define NewWindowPaintProc(userRoutine)                         NewWindowPaintUPP(userRoutine)
#define CallWindowDefProc(userRoutine, varCode, window, message, param) InvokeWindowDefUPP(varCode, window, message, param, userRoutine)
#define CallDeskHookProc(userRoutine, mouseClick, theEvent)     InvokeDeskHookUPP(mouseClick, theEvent, userRoutine)
#define CallWindowPaintProc(userRoutine, device, qdContext, window, inClientPaintRgn, outSystemPaintRgn, refCon) InvokeWindowPaintUPP(device, qdContext, window, inClientPaintRgn, outSystemPaintRgn, refCon, userRoutine)
/*--------------------------------------------------------------------------------------*/
/* o Window Definition Spec.  Used in Carbon to specify the code that defines a window. */
/*--------------------------------------------------------------------------------------*/
enum {
    kWindowDefProcPtr           = 0                             /* raw proc-ptr based access*/
};


typedef UInt32                          WindowDefType;

struct WindowDefSpec {
    WindowDefType                   defType;
    union {
        WindowDefUPP                    defProc;
    }                                 u;
};
typedef struct WindowDefSpec            WindowDefSpec;
/*--------------------------------------------------------------------------------------*/
/* o Window Creation & Persistence                                                      */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( WindowRef )
GetNewCWindow                   (short                  windowID,
                                 void *                 wStorage,
                                 WindowRef              behind)                             ONEWORDINLINE(0xAA46);

EXTERN_API( WindowRef )
NewWindow                       (void *                 wStorage,
                                 const Rect *           boundsRect,
                                 ConstStr255Param       title,
                                 Boolean                visible,
                                 short                  theProc,
                                 WindowRef              behind,
                                 Boolean                goAwayFlag,
                                 long                   refCon)                             ONEWORDINLINE(0xA913);

EXTERN_API( WindowRef )
GetNewWindow                    (short                  windowID,
                                 void *                 wStorage,
                                 WindowRef              behind)                             ONEWORDINLINE(0xA9BD);

EXTERN_API( WindowRef )
NewCWindow                      (void *                 wStorage,
                                 const Rect *           boundsRect,
                                 ConstStr255Param       title,
                                 Boolean                visible,
                                 short                  procID,
                                 WindowRef              behind,
                                 Boolean                goAwayFlag,
                                 long                   refCon)                             ONEWORDINLINE(0xAA45);

EXTERN_API( void )
DisposeWindow                   (WindowRef              window)                             ONEWORDINLINE(0xA914);

#if CALL_NOT_IN_CARBON
#if TARGET_OS_MAC
    #define MacCloseWindow CloseWindow
#endif
EXTERN_API( void )
MacCloseWindow                  (WindowRef              window)                             ONEWORDINLINE(0xA92D);


/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( OSStatus )
CreateNewWindow                 (WindowClass            windowClass,
                                 WindowAttributes       attributes,
                                 const Rect *           contentBounds,
                                 WindowRef *            outWindow);

/* Routines available from Mac OS 8.5 forward*/

/* Create a window from a 'wind' resource*/
EXTERN_API( OSStatus )
CreateWindowFromResource        (SInt16                 resID,
                                 WindowRef *            outWindow);

/* window persistence*/
EXTERN_API( OSStatus )
StoreWindowIntoCollection       (WindowRef              window,
                                 Collection             collection);

EXTERN_API( OSStatus )
CreateWindowFromCollection      (Collection             collection,
                                 WindowRef *            outWindow);

/* window refcounting*/
EXTERN_API( OSStatus )
GetWindowOwnerCount             (WindowRef              window,
                                 UInt32 *               outCount);

EXTERN_API( OSStatus )
CloneWindow                     (WindowRef              window);


/*--------------------------------------------------------------------------------------*/
/* o Custom Windows                                                                     */
/*--------------------------------------------------------------------------------------*/
/* Routines available from Mac OS 8.1 forward when linking to CarbonLib*/

EXTERN_API( OSStatus )
CreateCustomWindow              (const WindowDefSpec *  def,
                                 WindowClass            windowClass,
                                 WindowAttributes       attributes,
                                 const Rect *           contentBounds,
                                 WindowRef *            outWindow);

EXTERN_API( OSStatus )
ReshapeCustomWindow             (WindowRef              window);

/*--------------------------------------------------------------------------------------*/
/* o Window Metainformation Accessors                                                   */
/*--------------------------------------------------------------------------------------*/

/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/
EXTERN_API( OSStatus )
GetWindowClass                  (WindowRef              window,
                                 WindowClass *          outClass);

EXTERN_API( OSStatus )
GetWindowAttributes             (WindowRef              window,
                                 WindowAttributes *     outAttributes);

/*
   Routines available from Mac OS 9.0 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/
EXTERN_API( OSStatus )
ChangeWindowAttributes          (WindowRef              window,
                                 WindowAttributes       setTheseAttributes,
                                 WindowAttributes       clearTheseAttributes);

/*--------------------------------------------------------------------------------------*/
/* o Floating Windows                                                                   */
/*--------------------------------------------------------------------------------------*/
/*
   Routines available from Mac OS 8.6 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/

EXTERN_API( OSStatus )
ShowFloatingWindows             (void);

EXTERN_API( OSStatus )
HideFloatingWindows             (void);

EXTERN_API( Boolean )
AreFloatingWindowsVisible       (void);



/*--------------------------------------------------------------------------------------*/
/* o Background Image                                                                   */
/*--------------------------------------------------------------------------------------*/
/* SetWinColor is not available in Carbon.*/
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
SetWinColor                     (WindowRef              window,
                                 WCTabHandle            newColorTable)                      ONEWORDINLINE(0xAA41);

/* SetDeskCPat is not available in Carbon.*/
EXTERN_API( void )
SetDeskCPat                     (PixPatHandle           deskPixPat)                         ONEWORDINLINE(0xAA47);

/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/
#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( OSStatus )
SetWindowContentColor           (WindowRef              window,
                                 const RGBColor *       color);

EXTERN_API( OSStatus )
GetWindowContentColor           (WindowRef              window,
                                 RGBColor *             color);

/* Routines available from Mac OS 8.5 forward*/
EXTERN_API( OSStatus )
GetWindowContentPattern         (WindowRef              window,
                                 PixPatHandle           outPixPat);

EXTERN_API( OSStatus )
SetWindowContentPattern         (WindowRef              window,
                                 PixPatHandle           pixPat);

/* Routines available from Mac OS 9.0 forward*/

typedef OptionBits                      WindowPaintProcOptions;
enum {
    kWindowPaintProcOptionsNone = 0
};

EXTERN_API( OSStatus )
InstallWindowContentPaintProc   (WindowRef              window,
                                 WindowPaintUPP         paintProc,
                                 WindowPaintProcOptions  options,
                                 void *                 refCon) /* can be NULL */;

/*--------------------------------------------------------------------------------------*/
/* o Scrolling Routines                                                                 */
/*--------------------------------------------------------------------------------------*/

typedef UInt32 ScrollWindowOptions;
enum {
    kScrollWindowNoOptions      = 0,
    kScrollWindowInvalidate     = (1L << 0),                    /* add the exposed area to the window's update region*/
    kScrollWindowEraseToPortBackground = (1L << 1)              /* erase the exposed area using the background color/pattern of the window's grafport*/
};


/* Routines available from Mac OS 8.1 forward when linking to CarbonLib*/

EXTERN_API( OSStatus )
ScrollWindowRect                (WindowRef              inWindow,
                                 const Rect *           inScrollRect,
                                 SInt16                 inHPixels,
                                 SInt16                 inVPixels,
                                 ScrollWindowOptions    inOptions,
                                 RgnHandle              outExposedRgn) /* can be NULL */;

EXTERN_API( OSStatus )
ScrollWindowRegion              (WindowRef              inWindow,
                                 RgnHandle              inScrollRgn,
                                 SInt16                 inHPixels,
                                 SInt16                 inVPixels,
                                 ScrollWindowOptions    inOptions,
                                 RgnHandle              outExposedRgn) /* can be NULL */;



/*--------------------------------------------------------------------------------------*/
/* o Low-Level Region & Painting Routines                                               */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
ClipAbove                       (WindowRef              window)                             ONEWORDINLINE(0xA90B);

/* SaveOld/DrawNew are not available in Carbon.  Use ReshapeCustomWindow instead.*/
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
SaveOld                         (WindowRef              window)                             ONEWORDINLINE(0xA90E);

EXTERN_API( void )
DrawNew                         (WindowRef              window,
                                 Boolean                update)                             ONEWORDINLINE(0xA90F);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( void )
PaintOne                        (WindowRef              window, /* can be NULL */
                                 RgnHandle              clobberedRgn)                       ONEWORDINLINE(0xA90C);

EXTERN_API( void )
PaintBehind                     (WindowRef              startWindow, /* can be NULL */
                                 RgnHandle              clobberedRgn)                       ONEWORDINLINE(0xA90D);

EXTERN_API( void )
CalcVis                         (WindowRef              window)                             ONEWORDINLINE(0xA909);

EXTERN_API( void )
CalcVisBehind                   (WindowRef              startWindow, /* can be NULL */
                                 RgnHandle              clobberedRgn)                       ONEWORDINLINE(0xA90A);

EXTERN_API( Boolean )
CheckUpdate                     (EventRecord *          theEvent)                           ONEWORDINLINE(0xA911);


/*--------------------------------------------------------------------------------------*/
/* o Window List                                                                        */
/*--------------------------------------------------------------------------------------*/
#if TARGET_OS_MAC
    #define MacFindWindow FindWindow
#endif
EXTERN_API( WindowPartCode )
MacFindWindow                   (Point                  thePoint,
                                 WindowRef *            window)                             ONEWORDINLINE(0xA92C);

EXTERN_API( WindowRef )
FrontWindow                     (void)                                                      ONEWORDINLINE(0xA924);

EXTERN_API( void )
BringToFront                    (WindowRef              window)                             ONEWORDINLINE(0xA920);

EXTERN_API( void )
SendBehind                      (WindowRef              window,
                                 WindowRef              behindWindow)                       ONEWORDINLINE(0xA921);

EXTERN_API( void )
SelectWindow                    (WindowRef              window)                             ONEWORDINLINE(0xA91F);

/*
   Routines available from Mac OS 8.6 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/

EXTERN_API( WindowRef )
FrontNonFloatingWindow          (void);

/* Routines available from Mac OS 8.1 forward when linking to CarbonLib*/

EXTERN_API( WindowRef )
GetNextWindowOfClass            (WindowRef              inWindow,
                                 WindowClass            inWindowClass,
                                 Boolean                mustBeVisible);

EXTERN_API( WindowPtr )
GetFrontWindowOfClass           (WindowClass            inWindowClass,
                                 Boolean                mustBeVisible);

EXTERN_API( OSStatus )
FindWindowOfClass               (const Point *          where,
                                 WindowClass            inWindowClass,
                                 WindowRef *            outWindow,
                                 SInt16 *               outWindowPart);


/*--------------------------------------------------------------------------------------*/
/* o Misc Low-Level stuff                                                               */
/*--------------------------------------------------------------------------------------*/
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
InitWindows                     (void)                                                      ONEWORDINLINE(0xA912);

/*  The window manager port does not exist in Carbon.   */
/*  We are investigating replacement technologies.      */
EXTERN_API( void )
GetWMgrPort                     (GrafPtr *              wPort)                              ONEWORDINLINE(0xA910);

EXTERN_API( void )
GetCWMgrPort                    (CGrafPtr *             wMgrCPort)                          ONEWORDINLINE(0xAA48);

/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/
#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( Boolean )
IsValidWindowPtr                (WindowPtr              possibleWindow);

/*
   Routines available from Mac OS 8.6 forward
   InitFloatingWindows is not available in Carbon;
   window ordering is always active for Carbon clients
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
InitFloatingWindows             (void);


/*--------------------------------------------------------------------------------------*/
/* o Various & Sundry Window Accessors                                                  */
/*--------------------------------------------------------------------------------------*/
#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( void )
HiliteWindow                    (WindowRef              window,
                                 Boolean                fHilite)                            ONEWORDINLINE(0xA91C);

EXTERN_API( void )
SetWRefCon                      (WindowRef              window,
                                 long                   data)                               ONEWORDINLINE(0xA918);

EXTERN_API( long )
GetWRefCon                      (WindowRef              window)                             ONEWORDINLINE(0xA917);

EXTERN_API( void )
SetWindowPic                    (WindowRef              window,
                                 PicHandle              pic)                                ONEWORDINLINE(0xA92E);

EXTERN_API( PicHandle )
GetWindowPic                    (WindowRef              window)                             ONEWORDINLINE(0xA92F);

EXTERN_API( short )
GetWVariant                     (WindowRef              window)                             ONEWORDINLINE(0xA80A);

/* Routines available from Mac OS 8.0 (Appearance 1.0) forward*/
EXTERN_API( OSStatus )
GetWindowFeatures               (WindowRef              window,
                                 UInt32 *               outFeatures)                        THREEWORDINLINE(0x303C, 0x0013, 0xAA74);

EXTERN_API( OSStatus )
GetWindowRegion                 (WindowRef              window,
                                 WindowRegionCode       inRegionCode,
                                 RgnHandle              ioWinRgn)                           THREEWORDINLINE(0x303C, 0x0014, 0xAA74);



/*--------------------------------------------------------------------------------------*/
/* o Update Events                                                                      */
/*--------------------------------------------------------------------------------------*/
/*
   These aren't present in Carbon. Please use the InvalWindowRect, etc. routines
   below instead.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
InvalRect                       (const Rect *           badRect)                            ONEWORDINLINE(0xA928);

EXTERN_API( void )
InvalRgn                        (RgnHandle              badRgn)                             ONEWORDINLINE(0xA927);

EXTERN_API( void )
ValidRect                       (const Rect *           goodRect)                           ONEWORDINLINE(0xA92A);

EXTERN_API( void )
ValidRgn                        (RgnHandle              goodRgn)                            ONEWORDINLINE(0xA929);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( void )
BeginUpdate                     (WindowRef              window)                             ONEWORDINLINE(0xA922);

EXTERN_API( void )
EndUpdate                       (WindowRef              window)                             ONEWORDINLINE(0xA923);

/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/

EXTERN_API( OSStatus )
InvalWindowRgn                  (WindowRef              window,
                                 RgnHandle              region);

EXTERN_API( OSStatus )
InvalWindowRect                 (WindowRef              window,
                                 const Rect *           bounds);

EXTERN_API( OSStatus )
ValidWindowRgn                  (WindowRef              window,
                                 RgnHandle              region);

EXTERN_API( OSStatus )
ValidWindowRect                 (WindowRef              window,
                                 const Rect *           bounds);


/*--------------------------------------------------------------------------------------*/
/* o DrawGrowIcon                                                                       */
/*                                                                                      */
/*  DrawGrowIcon is deprecated from Mac OS 8.0 forward.  Theme-savvy window defprocs    */
/*  include the grow box in the window frame.                                           */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
DrawGrowIcon                    (WindowRef              window)                             ONEWORDINLINE(0xA904);

/*--------------------------------------------------------------------------------------*/
/* o Window Titles                                                                      */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
SetWTitle                       (WindowRef              window,
                                 ConstStr255Param       title)                              ONEWORDINLINE(0xA91A);

EXTERN_API( void )
GetWTitle                       (WindowRef              window,
                                 Str255                 title)                              ONEWORDINLINE(0xA919);

/*--------------------------------------------------------------------------------------*/
/* o Window Proxies                                                                     */
/*--------------------------------------------------------------------------------------*/
/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/

EXTERN_API( OSStatus )
SetWindowProxyFSSpec            (WindowRef              window,
                                 const FSSpec *         inFile);

EXTERN_API( OSStatus )
GetWindowProxyFSSpec            (WindowRef              window,
                                 FSSpec *               outFile);

EXTERN_API( OSStatus )
SetWindowProxyAlias             (WindowRef              window,
                                 AliasHandle            alias);

EXTERN_API( OSStatus )
GetWindowProxyAlias             (WindowRef              window,
                                 AliasHandle *          alias);

EXTERN_API( OSStatus )
SetWindowProxyCreatorAndType    (WindowRef              window,
                                 OSType                 fileCreator,
                                 OSType                 fileType,
                                 SInt16                 vRefNum);

EXTERN_API( OSStatus )
GetWindowProxyIcon              (WindowRef              window,
                                 IconRef *              outIcon);

EXTERN_API( OSStatus )
SetWindowProxyIcon              (WindowRef              window,
                                 IconRef                icon);

EXTERN_API( OSStatus )
RemoveWindowProxy               (WindowRef              window);

EXTERN_API( OSStatus )
BeginWindowProxyDrag            (WindowRef              window,
                                 DragReference *        outNewDrag,
                                 RgnHandle              outDragOutlineRgn);

EXTERN_API( OSStatus )
EndWindowProxyDrag              (WindowRef              window,
                                 DragReference          theDrag);

EXTERN_API( OSStatus )
TrackWindowProxyFromExistingDrag (WindowRef             window,
                                 Point                  startPt,
                                 DragReference          drag,
                                 RgnHandle              inDragOutlineRgn);

EXTERN_API( OSStatus )
TrackWindowProxyDrag            (WindowRef              window,
                                 Point                  startPt);

EXTERN_API( Boolean )
IsWindowModified                (WindowRef              window);

EXTERN_API( OSStatus )
SetWindowModified               (WindowRef              window,
                                 Boolean                modified);

EXTERN_API( Boolean )
IsWindowPathSelectClick         (WindowRef              window,
                                 const EventRecord *    event);

EXTERN_API( OSStatus )
WindowPathSelect                (WindowRef              window,
                                 MenuHandle             menu, /* can be NULL */
                                 SInt32 *               outMenuResult);


/*--------------------------------------------------------------------------------------*/
/*  o HiliteWindowFrameForDrag                                                          */
/*                                                                                      */
/*  If you call ShowDragHilite and HideDragHilite, you don't need to use this routine.  */
/*  If you implement custom drag hiliting, you should call HiliteWindowFrameForDrag     */
/*  when the drag is tracking inside a window with drag-hilited content.                */
/*--------------------------------------------------------------------------------------*/
/* Routines available from Mac OS 8.5 forward*/

EXTERN_API( OSStatus )
HiliteWindowFrameForDrag        (WindowRef              window,
                                 Boolean                hilited)                            TWOWORDINLINE(0x7019, 0xA829);


/*--------------------------------------------------------------------------------------*/
/* o Window Transitions                                                                 */
/*                                                                                      */
/*  TransitionWindow displays a window with accompanying animation and sound.           */
/*--------------------------------------------------------------------------------------*/
/* Routines available from Mac OS 8.5 forward*/


typedef UInt32 WindowTransitionEffect;
enum {
    kWindowZoomTransitionEffect = 1                             /* Finder-like zoom rectangles, use with Show or Open transition actions*/
};


typedef UInt32 WindowTransitionAction;
enum {
    kWindowShowTransitionAction = 1,                            /* param is rect in global coordinates from which to start the animation*/
    kWindowHideTransitionAction = 2                             /* param is rect in global coordinates at which to end the animation*/
};

EXTERN_API( OSStatus )
TransitionWindow                (WindowRef              window,
                                 WindowTransitionEffect  effect,
                                 WindowTransitionAction  action,
                                 const Rect *           rect) /* can be NULL */;


/*--------------------------------------------------------------------------------------*/
/* o Window Positioning                                                                 */
/*--------------------------------------------------------------------------------------*/

#if TARGET_OS_MAC
    #define MacMoveWindow MoveWindow
#endif
EXTERN_API( void )
MacMoveWindow                   (WindowRef              window,
                                 short                  hGlobal,
                                 short                  vGlobal,
                                 Boolean                front)                              ONEWORDINLINE(0xA91B);

EXTERN_API( void )
SizeWindow                      (WindowRef              window,
                                 short                  w,
                                 short                  h,
                                 Boolean                fUpdate)                            ONEWORDINLINE(0xA91D);


/* Note: bBox can only be NULL on Carbon or OS X systems */
EXTERN_API( long )
GrowWindow                      (WindowRef              window,
                                 Point                  startPt,
                                 const Rect *           bBox) /* can be NULL */             ONEWORDINLINE(0xA92B);

/* Note: boundsRect can only be NULL on Carbon or OS X systems */
EXTERN_API( void )
DragWindow                      (WindowRef              window,
                                 Point                  startPt,
                                 const Rect *           boundsRect) /* can be NULL */       ONEWORDINLINE(0xA925);

EXTERN_API( void )
ZoomWindow                      (WindowRef              window,
                                 WindowPartCode         partCode,
                                 Boolean                front)                              ONEWORDINLINE(0xA83A);

/* Routines available from Mac OS 8.0 (Appearance 1.0) forward*/

EXTERN_API( Boolean )
IsWindowCollapsable             (WindowRef              window)                             THREEWORDINLINE(0x303C, 0x000F, 0xAA74);

EXTERN_API( Boolean )
IsWindowCollapsed               (WindowRef              window)                             THREEWORDINLINE(0x303C, 0x0010, 0xAA74);

EXTERN_API( OSStatus )
CollapseWindow                  (WindowRef              window,
                                 Boolean                collapse)                           THREEWORDINLINE(0x303C, 0x0011, 0xAA74);

EXTERN_API( OSStatus )
CollapseAllWindows              (Boolean                collapse)                           THREEWORDINLINE(0x303C, 0x0012, 0xAA74);

/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/

EXTERN_API( OSStatus )
GetWindowBounds                 (WindowRef              window,
                                 WindowRegionCode       regionCode,
                                 Rect *                 globalBounds);

/* Note: newContentRect can only be NULL on Carbon or OS X systems */
EXTERN_API( Boolean )
ResizeWindow                    (WindowRef              window,
                                 Point                  startPoint,
                                 const Rect *           sizeConstraints, /* can be NULL */
                                 Rect *                 newContentRect) /* can be NULL */;

/* Routines available from Mac OS 8.5 forward*/

EXTERN_API( OSStatus )
SetWindowBounds                 (WindowRef              window,
                                 WindowRegionCode       regionCode,
                                 const Rect *           globalBounds);

EXTERN_API( OSStatus )
RepositionWindow                (WindowRef              window,
                                 WindowRef              parentWindow,
                                 WindowPositionMethod   method);


EXTERN_API( OSStatus )
MoveWindowStructure             (WindowRef              window,
                                 short                  hGlobal,
                                 short                  vGlobal);

EXTERN_API( Boolean )
IsWindowInStandardState         (WindowRef              window,
                                 Point *                idealSize,
                                 Rect *                 idealStandardState);

EXTERN_API( OSStatus )
ZoomWindowIdeal                 (WindowRef              window,
                                 SInt16                 partCode,
                                 Point *                ioIdealSize);

EXTERN_API( OSStatus )
GetWindowIdealUserState         (WindowRef              window,
                                 Rect *                 userState);

EXTERN_API( OSStatus )
SetWindowIdealUserState         (WindowRef              window,
                                 Rect *                 userState);


/*--------------------------------------------------------------------------------------*/
/* o Window Visibility                                                                  */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( void )
HideWindow                      (WindowRef              window)                             ONEWORDINLINE(0xA916);

#if TARGET_OS_MAC
    #define MacShowWindow ShowWindow
#endif
EXTERN_API( void )
MacShowWindow                   (WindowRef              window)                             ONEWORDINLINE(0xA915);

EXTERN_API( void )
ShowHide                        (WindowRef              window,
                                 Boolean                showFlag)                           ONEWORDINLINE(0xA908);


/*--------------------------------------------------------------------------------------*/
/* o Window Properties                                                                  */
/*--------------------------------------------------------------------------------------*/
/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib
*/

EXTERN_API( OSStatus )
GetWindowProperty               (WindowRef              window,
                                 PropertyCreator        propertyCreator,
                                 PropertyTag            propertyTag,
                                 UInt32                 bufferSize,
                                 UInt32 *               actualSize, /* can be NULL */
                                 void *                 propertyBuffer);

EXTERN_API( OSStatus )
GetWindowPropertySize           (WindowRef              window,
                                 PropertyCreator        creator,
                                 PropertyTag            tag,
                                 UInt32 *               size);

EXTERN_API( OSStatus )
SetWindowProperty               (WindowRef              window,
                                 PropertyCreator        propertyCreator,
                                 PropertyTag            propertyTag,
                                 UInt32                 propertySize,
                                 void *                 propertyBuffer);

EXTERN_API( OSStatus )
RemoveWindowProperty            (WindowRef              window,
                                 PropertyCreator        propertyCreator,
                                 PropertyTag            propertyTag);


/* Routines available from Mac OS 8.1 forward when linking to CarbonLib*/

enum {
    kWindowPropertyPersistent   = 0x00000001                    /* whether this property gets saved when flattening the window */
};

EXTERN_API( OSStatus )
GetWindowPropertyAttributes     (WindowRef              window,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32 *               attributes);

EXTERN_API( OSStatus )
ChangeWindowPropertyAttributes  (WindowRef              window,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32                 attributesToSet,
                                 UInt32                 attributesToClear);

/*--------------------------------------------------------------------------------------*/
/* o Utilities                                                                          */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( long )
PinRect                         (const Rect *           theRect,
                                 Point                  thePt)                              ONEWORDINLINE(0xA94E);


EXTERN_API( RgnHandle )
GetGrayRgn                      (void)                                                      TWOWORDINLINE(0x2EB8, 0x09EE);


/*--------------------------------------------------------------------------------------*/
/* o Window Part Tracking                                                               */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( Boolean )
TrackBox                        (WindowRef              window,
                                 Point                  thePt,
                                 WindowPartCode         partCode)                           ONEWORDINLINE(0xA83B);

EXTERN_API( Boolean )
TrackGoAway                     (WindowRef              window,
                                 Point                  thePt)                              ONEWORDINLINE(0xA91E);


/*--------------------------------------------------------------------------------------*/
/* o Region Dragging                                                                    */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( long )
DragGrayRgn                     (RgnHandle              theRgn,
                                 Point                  startPt,
                                 const Rect *           limitRect,
                                 const Rect *           slopRect,
                                 short                  axis,
                                 DragGrayRgnUPP         actionProc)                         ONEWORDINLINE(0xA905);

EXTERN_API( long )
DragTheRgn                      (RgnHandle              theRgn,
                                 Point                  startPt,
                                 const Rect *           limitRect,
                                 const Rect *           slopRect,
                                 short                  axis,
                                 DragGrayRgnUPP         actionProc)                         ONEWORDINLINE(0xA926);


/*--------------------------------------------------------------------------------------*/
/*  o GetAuxWin                                                                         */
/*                                                                                      */
/*  GetAuxWin is not available in Carbon                                                */
/*--------------------------------------------------------------------------------------*/
#if !OPAQUE_TOOLBOX_STRUCTS
#if CALL_NOT_IN_CARBON
EXTERN_API( Boolean )
GetAuxWin                       (WindowRef              window,
                                 AuxWinHandle *         awHndl)                             ONEWORDINLINE(0xAA42);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

/*--------------------------------------------------------------------------------------*/
/* o C Glue                                                                             */
/*--------------------------------------------------------------------------------------*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
setwtitle                       (WindowRef              window,
                                 const char *           title);

EXTERN_API_C( Boolean )
trackgoaway                     (WindowRef              window,
                                 Point *                thePt);

EXTERN_API_C( short )
findwindow                      (Point *                thePoint,
                                 WindowRef *            window);

EXTERN_API_C( void )
getwtitle                       (WindowRef              window,
                                 char *                 title);

EXTERN_API_C( long )
growwindow                      (WindowRef              window,
                                 Point *                startPt,
                                 const Rect *           bBox);

EXTERN_API_C( WindowRef )
newwindow                       (void *                 wStorage,
                                 const Rect *           boundsRect,
                                 const char *           title,
                                 Boolean                visible,
                                 short                  theProc,
                                 WindowRef              behind,
                                 Boolean                goAwayFlag,
                                 long                   refCon);

EXTERN_API_C( WindowRef )
newcwindow                      (void *                 wStorage,
                                 const Rect *           boundsRect,
                                 const char *           title,
                                 Boolean                visible,
                                 short                  procID,
                                 WindowRef              behind,
                                 Boolean                goAwayFlag,
                                 long                   refCon);

EXTERN_API_C( long )
pinrect                         (const Rect *           theRect,
                                 Point *                thePt);

EXTERN_API_C( Boolean )
trackbox                        (WindowRef              window,
                                 Point *                thePt,
                                 short                  partCode);

EXTERN_API_C( long )
draggrayrgn                     (RgnHandle              theRgn,
                                 Point *                startPt,
                                 const Rect *           boundsRect,
                                 const Rect *           slopRect,
                                 short                  axis,
                                 DragGrayRgnUPP         actionProc);

EXTERN_API_C( void )
dragwindow                      (WindowRef              window,
                                 Point *                startPt,
                                 const Rect *           boundsRect);

#endif  /* CALL_NOT_IN_CARBON */

/*--------------------------------------------------------------------------------------*/
/* o WindowRecord Accessors                                                             */
/*--------------------------------------------------------------------------------------*/
/*
    CHAPTER XVI - the Opacity Wars
    
    A long, long time ago, in a metaverse far, far away...
    
    On an ancient gravestone by the side of the road is a hastily-scrawled
    epitaph, written in an archaic dialect.  With the help of your robot guide,
    you translate it:

    "Shared data structures are difficult/impossible to coordinate in a preemptively
    multitasking OS.  To allow Apple to provide a fully reentrant toolbox in
    the future, while still maintaining a reasonable migration path for existing
    codebases and developer knowledge, the decision has been made to make all
    toolbox objects opaque."
    
    You hear a vague rumbling sound in the distance.  Your guide suggests backing
    away, but you ignore it, wishing to admire the ornamentation on the gravestone.
    
    Not the best decision.  The grave ERUPTS, a dusty maelstrom filling your
    senses, fragment of dirt and bone flying through the air.  You are thrown
    violently to the ground, shocked but conscious.
    
    The dust clears quickly, revealing a wizened man sitting on the gravestone.
    He wears a robe and speaks in deep, passionate tones:
    
    "N.B. If you attempt to access fields of the WindowRecord, ControlRecord,
    DialogRecord, or MenuInfo directly in Mac OS X, you _will_ crash.  To avoid
    this unpleasant situation, please use the accessor macros which correspond to
    the routines within the ACCESSOR_CALLS_ARE_FUNCTIONS conditional."
    
    Yes, this is another change in direction.  But you're sort 'o used to that
    by now.  And this time, there is no turning back.
*/
#if !OPAQUE_TOOLBOX_STRUCTS && !ACCESSOR_CALLS_ARE_FUNCTIONS
#ifdef __cplusplus
inline CGrafPtr     GetWindowPort(WindowRef w)                  { return (CGrafPtr) w;                                                  }
inline void         SetPortWindowPort(WindowRef w)              { MacSetPort( (GrafPtr) GetWindowPort(w)); }
inline SInt16       GetWindowKind(WindowRef w)                  { return ( *(SInt16 *)  (((UInt8 *) w) + sizeof(GrafPort)));            }
inline void         SetWindowKind(WindowRef w, SInt16 wKind)    { *(SInt16 *)   (((UInt8 *) w) + sizeof(GrafPort)) = wKind;             }
#if TARGET_OS_MAC
inline Boolean      IsWindowVisible(WindowRef w)                { return *(Boolean *)   (((UInt8 *) w) + sizeof(GrafPort) + 0x2);       }
#endif
inline Boolean      MacIsWindowVisible(WindowRef w)             { return *(Boolean *)   (((UInt8 *) w) + sizeof(GrafPort) + 0x2);       }
inline Boolean      IsWindowHilited(WindowRef w)                { return *(Boolean *)   (((UInt8 *) w) + sizeof(GrafPort) + 0x3);       }
inline Boolean      GetWindowGoAwayFlag(WindowRef w)            { return *(Boolean *)   (((UInt8 *) w) + sizeof(GrafPort) + 0x4);       }
inline Boolean      GetWindowZoomFlag(WindowRef w)              { return *(Boolean *)   (((UInt8 *) w) + sizeof(GrafPort) + 0x5);       }
inline void         GetWindowStructureRgn(WindowRef w, RgnHandle r) { MacCopyRgn( *(RgnHandle *)(((UInt8 *) w) + sizeof(GrafPort) + 0x6), r );  }
inline void         GetWindowContentRgn(WindowRef w, RgnHandle r)   { MacCopyRgn( *(RgnHandle *)(((UInt8 *) w) + sizeof(GrafPort) + 0xA), r );  }
inline void         GetWindowUpdateRgn(WindowRef w, RgnHandle r)    { MacCopyRgn( *(RgnHandle *)(((UInt8 *) w) + sizeof(GrafPort) + 0xE), r );  }
inline SInt16       GetWindowTitleWidth(WindowRef w)                { return *(SInt16 *)(((UInt8 *) w) + sizeof(GrafPort) + 0x1E);          }
#if TARGET_OS_MAC
inline WindowRef    GetNextWindow(WindowRef w)                      { return *(WindowRef *) (((UInt8 *) w) + sizeof(GrafPort) + 0x24);      }
#endif
inline WindowRef    MacGetNextWindow(WindowRef w)                   { return *(WindowRef *) (((UInt8 *) w) + sizeof(GrafPort) + 0x24);      }

inline void GetWindowStandardState(WindowRef w, Rect *r)
{   Rect *stateRects = (  (Rect *) (**(Handle *) (((UInt8 *) w) + sizeof(GrafPort) + 0x16)));
if (stateRects != NULL) *r = stateRects[1];     }
inline void SetWindowStandardState(WindowRef w, const Rect *r)
{   Rect *stateRects = (  (Rect *) (**(Handle *) (((UInt8 *) w) + sizeof(GrafPort) + 0x16)));
    if (stateRects != NULL) stateRects[1] = *r;     }
inline void GetWindowUserState(WindowRef w, Rect *r)
{   Rect *stateRects = (  (Rect *) (**(Handle *) (((UInt8 *) w) + sizeof(GrafPort) + 0x16)));
    if (stateRects != NULL) *r = stateRects[0]; }
inline void SetWindowUserState(WindowRef w, const Rect *r)
{ Rect *stateRects = (  (Rect *) (**(Handle *) (((UInt8 *) w) + sizeof(GrafPort) + 0x16)));
    if (stateRects != NULL) stateRects[0] = *r; }
inline Handle       GetWindowDataHandle(WindowRef w)                {   return  (((WindowPeek) (w))->dataHandle);               }
inline void         SetWindowDataHandle(WindowRef w, Handle data)   {   (((WindowPeek) (w))->dataHandle) = ((Handle) (data));   }
#else
#if TARGET_OS_MAC
#define IsWindowVisible MacIsWindowVisible
#define GetNextWindow MacGetNextWindow
#endif
#define ShowHideWindow(w)                       ShowHide(w)
#define SetPortWindowPort(w)                    MacSetPort( (GrafPtr) GetWindowPort(w) )
#define GetWindowPort(w)                        ( (CGrafPtr) w)
#define GetWindowKind(w)                        ( *(SInt16 *)   (((UInt8 *) w) + sizeof(GrafPort)))
#define SetWindowKind(w, wKind)                 ( *(SInt16 *)   (((UInt8 *) w) + sizeof(GrafPort)) = wKind )
#define MacIsWindowVisible(w)                   ( *(Boolean *)  (((UInt8 *) w) + sizeof(GrafPort) + 0x2))
#define IsWindowHilited(w)                      ( *(Boolean *)  (((UInt8 *) w) + sizeof(GrafPort) + 0x3))
#define GetWindowGoAwayFlag(w)                  ( *(Boolean *)  (((UInt8 *) w) + sizeof(GrafPort) + 0x4))
#define GetWindowZoomFlag(w)                    ( *(Boolean *)  (((UInt8 *) w) + sizeof(GrafPort) + 0x5))
#define GetWindowStructureRgn(w, aRgnHandle)    MacCopyRgn( *(RgnHandle *)(((UInt8 *) w) + sizeof(GrafPort) + 0x6), aRgnHandle )
#define GetWindowContentRgn(w, aRgnHandle)      MacCopyRgn( *(RgnHandle *)(((UInt8 *) w) + sizeof(GrafPort) + 0xA), aRgnHandle )

#define GetWindowUpdateRgn(w, aRgnHandle)       MacCopyRgn( *(RgnHandle *)(((UInt8 *) w) + sizeof(GrafPort) + 0xE), aRgnHandle )

#define GetWindowTitleWidth(w)                  ( *(SInt16 *)       (((UInt8 *) w) + sizeof(GrafPort) + 0x1E))
#define MacGetNextWindow(w)                     ( *(WindowRef *)    (((UInt8 *) w) + sizeof(GrafPort) + 0x24))

#define GetWindowStandardState(w, aRectPtr) do { Rect *stateRects = (  (Rect *) (**(Handle *) (((UInt8 *) w) + sizeof(GrafPort) + 0x16)));  \
                                                                if (stateRects != NULL) *aRectPtr = stateRects[1]; } while (false)
#define SetWindowStandardState(w, aRectPtr) do { Rect *stateRects = (  (Rect *) (**(Handle *) (((UInt8 *) w) + sizeof(GrafPort) + 0x16)));  \
                                                                if (stateRects != NULL) stateRects[1] = *aRectPtr; } while (false)
#define GetWindowUserState(w, aRectPtr)     do { Rect *stateRects = (  (Rect *) (**(Handle *) (((UInt8 *) w) + sizeof(GrafPort) + 0x16)));  \
                                                                if (stateRects != NULL) *aRectPtr = stateRects[0]; } while (false)
#define SetWindowUserState(w, aRectPtr)     do { Rect *stateRects = (  (Rect *) (**(Handle *) (((UInt8 *) w) + sizeof(GrafPort) + 0x16)));  \
                                                                if (stateRects != NULL) stateRects[0] = *aRectPtr; } while (false)
#define GetWindowDataHandle(windowRef)              (((WindowPeek) (windowRef))->dataHandle)
#define SetWindowDataHandle(windowRef, data)        (((WindowPeek) (windowRef))->dataHandle) = ((Handle) (data))

#endif  /* defined(__cplusplus) */

#endif  /* !OPAQUE_TOOLBOX_STRUCTS && !ACCESSOR_CALLS_ARE_FUNCTIONS */

#if ACCESSOR_CALLS_ARE_FUNCTIONS
/*                                                                                          */
/*  Direct modification of the close box and zoom box "flags" is not supported in Carbon.   */
/*                                                                                          */
/*  GetWindowGoAwayFlag                                                                     */
/*  GetWindowSpareFlag                                                                      */
/*  SetWindowGoAwayFlag                                                                     */
/*  SetWindowSpareFlag                                                                      */
/*                                                                                          */
/*  Use GetWindowAttributes and ChangeWindowAttributes if you must dynamically              */
/*  access the zoom box and close box attributes.                                           */
/*                                                                                          */
/* Still available as transitional API in PreCarbon.o, since GetWindowAttributes is not implemented there. */
EXTERN_API( Boolean )
GetWindowGoAwayFlag             (WindowRef              window);

EXTERN_API( Boolean )
GetWindowSpareFlag              (WindowRef              window);

/* Getters */
EXTERN_API( WindowRef )
GetWindowList                   (void);

EXTERN_API( CGrafPtr )
GetWindowPort                   (WindowRef              window);

EXTERN_API( short )
GetWindowKind                   (WindowRef              window);

#if TARGET_OS_MAC
    #define MacIsWindowVisible IsWindowVisible
#endif
EXTERN_API( Boolean )
MacIsWindowVisible              (WindowRef              window);

EXTERN_API( Boolean )
IsWindowHilited                 (WindowRef              window);

EXTERN_API( Boolean )
IsWindowUpdatePending           (WindowRef              window);

#if TARGET_OS_MAC
    #define MacGetNextWindow GetNextWindow
#endif
EXTERN_API( WindowRef )
MacGetNextWindow                (WindowRef              window);

EXTERN_API( Rect *)
GetWindowStandardState          (WindowRef              window,
                                 Rect *                 rect);

EXTERN_API( Rect *)
GetWindowUserState              (WindowRef              window,
                                 Rect *                 rect);

/* Setters */
EXTERN_API( void )
SetWindowKind                   (WindowRef              window,
                                 short                  kind);

EXTERN_API( void )
SetWindowStandardState          (WindowRef              window,
                                 const Rect *           rect);

EXTERN_API( void )
SetWindowUserState              (WindowRef              window,
                                 const Rect *           rect);

/* Utilities */
/* set the current QuickDraw port to the port associated with the window */
EXTERN_API( void )
SetPortWindowPort               (WindowRef              window);

EXTERN_API( Rect *)
GetWindowPortBounds             (WindowRef              window,
                                 Rect *                 bounds);

/* GetWindowFromPort is needed to 'cast up' to a WindowRef from a GrafPort */
EXTERN_API( WindowRef )
GetWindowFromPort               (CGrafPtr               port);

/* To prevent upward dependencies, GetDialogFromWindow() is defined in the Dialogs interface: */
/*      pascal DialogPtr        GetDialogFromWindow(WindowRef window); */
#endif  /* ACCESSOR_CALLS_ARE_FUNCTIONS */









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

#endif /* __MACWINDOWS__ */

