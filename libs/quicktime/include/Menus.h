/*
     File:       Menus.h
 
     Contains:   Menu Manager Interfaces.
 
     Version:    Technology: Mac OS 9.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MENUS__
#define __MENUS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __FONTS__
#include <Fonts.h>
#endif

#ifndef __TEXTCOMMON__
#include <TextCommon.h>
#endif

#ifndef __PROCESSES__
#include <Processes.h>
#endif

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
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


/*--------------------------------------------------------------------------------------*/
/*  o Menu Constants                                                                    */
/*--------------------------------------------------------------------------------------*/
enum {
    noMark                      = 0                             /*mark symbol for MarkItem*/
};

enum {
                                                                /* menu defProc messages */
    kMenuDrawMsg                = 0,
    kMenuSizeMsg                = 2,
    kMenuPopUpMsg               = 3,
    kMenuCalcItemMsg            = 5,
    kMenuThemeSavvyMsg          = 7,                            /* is your MDEF theme-savvy?  If so, return hex 7473 in the whichItem parameter*/
    mDrawMsg                    = 0,
    mSizeMsg                    = 2,
    mPopUpMsg                   = 3,                            /* position the popup menu rect appropriately */
    mCalcItemMsg                = 5
};

#if CALL_NOT_IN_CARBON
/*
   Carbon MDEFs must replace mChooseMsg with the new messages kMenuFindItemMsg and kMenuHiliteItemMsg. 
   mDrawItemMsg was used by the popup menu control before 8.5, but is no longer used. 
*/
enum {
    mChooseMsg                  = 1,
    mDrawItemMsg                = 4,
    kMenuChooseMsg              = 1,
    kMenuDrawItemMsg            = 4
};

#endif  /* CALL_NOT_IN_CARBON */

enum {
    kThemeSavvyMenuResponse     = 0x7473
};

/* these MDEF messages are only supported in Carbon*/
enum {
    kMenuInitMsg                = 8,
    kMenuDisposeMsg             = 9,
    kMenuFindItemMsg            = 10,
    kMenuHiliteItemMsg          = 11
};

enum {
    textMenuProc                = 0,
    hMenuCmd                    = 27,                           /*itemCmd == 0x001B ==> hierarchical menu*/
    hierMenu                    = -1,                           /*a hierarchical menu - for InsertMenu call*/
    kInsertHierarchicalMenu     = -1,                           /*a better name for hierMenu */
    mctAllItems                 = -98,                          /*search for all Items for the given ID*/
    mctLastIDIndic              = -99                           /*last color table entry has this in ID field*/
};

/* Constants for use with MacOS 8.0 (Appearance 1.0) and later*/
enum {
    kMenuStdMenuProc            = 63,
    kMenuStdMenuBarProc         = 63
};

enum {
    kMenuNoModifiers            = 0,                            /* Mask for no modifiers*/
    kMenuShiftModifier          = (1 << 0),                     /* Mask for shift key modifier*/
    kMenuOptionModifier         = (1 << 1),                     /* Mask for option key modifier*/
    kMenuControlModifier        = (1 << 2),                     /* Mask for control key modifier*/
    kMenuNoCommandModifier      = (1 << 3)                      /* Mask for no command key modifier*/
};

enum {
    kMenuNoIcon                 = 0,                            /* No icon*/
    kMenuIconType               = 1,                            /* Type for ICON*/
    kMenuShrinkIconType         = 2,                            /* Type for ICON plotted 16 x 16*/
    kMenuSmallIconType          = 3,                            /* Type for SICN*/
    kMenuColorIconType          = 4,                            /* Type for cicn*/
    kMenuIconSuiteType          = 5,                            /* Type for Icon Suite*/
    kMenuIconRefType            = 6                             /* Type for Icon Ref*/
};

enum {
    kMenuAttrExcludesMarkColumn = (1 << 0),                     /* No space is allocated for the mark character */
    kMenuAttrAutoDisable        = (1 << 2)                      /* Menu title is automatically disabled when all items are disabled */
};

typedef OptionBits                      MenuAttributes;
enum {
    kMenuItemAttrSubmenuParentChoosable = (1 << 2)              /* Parent item of a submenu is still selectable by the user */
};

typedef OptionBits                      MenuItemAttributes;

typedef UInt32 MenuTrackingMode;
enum {
    kMenuTrackingModeMouse      = 1,                            /* Menus are being tracked using the mouse*/
    kMenuTrackingModeKeyboard   = 2                             /* Menus are being tracked using the keyboard*/
};

/*--------------------------------------------------------------------------------------*/
/*  o Menu Types                                                                        */
/*--------------------------------------------------------------------------------------*/
typedef SInt16                          MenuID;
typedef UInt16                          MenuItemIndex;
typedef UInt32                          MenuCommand;
#if !OPAQUE_TOOLBOX_STRUCTS

struct MenuInfo {
    MenuID                          menuID;
    short                           menuWidth;
    short                           menuHeight;
    Handle                          menuProc;
    long                            enableFlags;
    Str255                          menuData;
};
typedef struct MenuInfo                 MenuInfo;

typedef MenuInfo *                      MenuPtr;
typedef MenuPtr *                       MenuHandle;
#else
typedef struct OpaqueMenuHandle*        MenuHandle;
#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

/* MenuRef and MenuHandle are equivalent. Use either. We don't care.*/
typedef MenuHandle                      MenuRef;

struct MCEntry {
    MenuID                          mctID;                      /*menu ID.  ID = 0 is the menu bar*/
    short                           mctItem;                    /*menu Item. Item = 0 is a title*/
    RGBColor                        mctRGB1;                    /*usage depends on ID and Item*/
    RGBColor                        mctRGB2;                    /*usage depends on ID and Item*/
    RGBColor                        mctRGB3;                    /*usage depends on ID and Item*/
    RGBColor                        mctRGB4;                    /*usage depends on ID and Item*/
    short                           mctReserved;                /*reserved for internal use*/
};
typedef struct MCEntry                  MCEntry;
typedef MCEntry *                       MCEntryPtr;

typedef MCEntry                         MCTable[1];
typedef MCEntry *                       MCTablePtr;
typedef MCTablePtr *                    MCTableHandle;

struct MenuCRsrc {
    short                           numEntries;                 /*number of entries*/
    MCTable                         mcEntryRecs;                /*ARRAY [1..numEntries] of MCEntry*/
};
typedef struct MenuCRsrc                MenuCRsrc;
typedef MenuCRsrc *                     MenuCRsrcPtr;
typedef MenuCRsrcPtr *                  MenuCRsrcHandle;
#if TARGET_OS_WIN32
/* QuickTime 3.0 */

struct MenuAccessKeyRec {
    short                           count;
    long                            flags;
    unsigned char                   keys[1];
};
typedef struct MenuAccessKeyRec         MenuAccessKeyRec;

typedef MenuAccessKeyRec *              MenuAccessKeyPtr;
typedef MenuAccessKeyPtr *              MenuAccessKeyHandle;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
SetMenuItemHotKey               (MenuRef                menu,
                                 short                  itemID,
                                 char                   hotKey,
                                 long                   flags);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_WIN32 */


struct MenuTrackingData {
    MenuRef                         menu;
    MenuItemIndex                   itemSelected;
    MenuItemIndex                   itemUnderMouse;
    Rect                            itemRect;
    SInt32                          virtualMenuTop;
    SInt32                          virtualMenuBottom;
};
typedef struct MenuTrackingData         MenuTrackingData;
typedef MenuTrackingData *              MenuTrackingDataPtr;

struct HiliteMenuItemData {
    MenuItemIndex                   previousItem;
    MenuItemIndex                   newItem;
};
typedef struct HiliteMenuItemData       HiliteMenuItemData;
typedef HiliteMenuItemData *            HiliteMenuItemDataPtr;
/*--------------------------------------------------------------------------------------*/
/*  o Menu ProcPtrs                                                                     */
/*                                                                                      */
/*  All of these procs are considered deprecated.  Developers interested in portability */
/*  to Carbon should avoid them entirely, if at all possible.                           */
/*--------------------------------------------------------------------------------------*/
typedef CALLBACK_API( void , MenuDefProcPtr )(short message, MenuRef theMenu, Rect *menuRect, Point hitPt, short *whichItem);
typedef STACK_UPP_TYPE(MenuDefProcPtr)                          MenuDefUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(MenuDefUPP)
    NewMenuDefUPP                  (MenuDefProcPtr          userRoutine);

    EXTERN_API(void)
    DisposeMenuDefUPP              (MenuDefUPP              userUPP);

    EXTERN_API(void)
    InvokeMenuDefUPP               (short                   message,
                                    MenuRef                 theMenu,
                                    Rect *                  menuRect,
                                    Point                   hitPt,
                                    short *                 whichItem,
                                    MenuDefUPP              userUPP);

#else
    enum { uppMenuDefProcInfo = 0x0000FF80 };                       /* pascal no_return_value Func(2_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewMenuDefUPP(userRoutine)                              (MenuDefUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMenuDefProcInfo, GetCurrentArchitecture())
    #define DisposeMenuDefUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define InvokeMenuDefUPP(message, theMenu, menuRect, hitPt, whichItem, userUPP)  CALL_FIVE_PARAMETER_UPP((userUPP), uppMenuDefProcInfo, (message), (theMenu), (menuRect), (hitPt), (whichItem))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewMenuDefProc(userRoutine)                             NewMenuDefUPP(userRoutine)
#define CallMenuDefProc(userRoutine, message, theMenu, menuRect, hitPt, whichItem) InvokeMenuDefUPP(message, theMenu, menuRect, hitPt, whichItem, userRoutine)
typedef CALLBACK_API( long , MenuBarDefProcPtr )(short selector, short message, short parameter1, long parameter2);
typedef CALLBACK_API( void , MenuHookProcPtr )(void );
typedef CALLBACK_API( short , MBarHookProcPtr )(Rect *menuRect);
typedef STACK_UPP_TYPE(MenuBarDefProcPtr)                       MenuBarDefUPP;
typedef STACK_UPP_TYPE(MenuHookProcPtr)                         MenuHookUPP;
typedef STACK_UPP_TYPE(MBarHookProcPtr)                         MBarHookUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(MenuBarDefUPP)
    NewMenuBarDefUPP               (MenuBarDefProcPtr       userRoutine);

    EXTERN_API(MenuHookUPP)
    NewMenuHookUPP                 (MenuHookProcPtr         userRoutine);

    EXTERN_API(MBarHookUPP)
    NewMBarHookUPP                 (MBarHookProcPtr         userRoutine);

    EXTERN_API(void)
    DisposeMenuBarDefUPP           (MenuBarDefUPP           userUPP);

    EXTERN_API(void)
    DisposeMenuHookUPP             (MenuHookUPP             userUPP);

    EXTERN_API(void)
    DisposeMBarHookUPP             (MBarHookUPP             userUPP);

    EXTERN_API(long)
    InvokeMenuBarDefUPP            (short                   selector,
                                    short                   message,
                                    short                   parameter1,
                                    long                    parameter2,
                                    MenuBarDefUPP           userUPP);

    EXTERN_API(void)
    InvokeMenuHookUPP              (MenuHookUPP             userUPP);

    EXTERN_API(short)
    InvokeMBarHookUPP              (Rect *                  menuRect,
                                    MBarHookUPP             userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppMenuBarDefProcInfo = 0x00003AB0 };                    /* pascal 4_bytes Func(2_bytes, 2_bytes, 2_bytes, 4_bytes) */
    enum { uppMenuHookProcInfo = 0x00000000 };                      /* pascal no_return_value Func() */
    enum { uppMBarHookProcInfo = 0x000000CF };                      /* SPECIAL_CASE_PROCINFO(12) */
    #define NewMenuBarDefUPP(userRoutine)                           (MenuBarDefUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMenuBarDefProcInfo, GetCurrentArchitecture())
    #define NewMenuHookUPP(userRoutine)                             (MenuHookUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMenuHookProcInfo, GetCurrentArchitecture())
    #define NewMBarHookUPP(userRoutine)                             (MBarHookUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMBarHookProcInfo, GetCurrentArchitecture())
    #define DisposeMenuBarDefUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeMenuHookUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define DisposeMBarHookUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define InvokeMenuBarDefUPP(selector, message, parameter1, parameter2, userUPP)  (long)CALL_FOUR_PARAMETER_UPP((userUPP), uppMenuBarDefProcInfo, (selector), (message), (parameter1), (parameter2))
    #define InvokeMenuHookUPP(userUPP)                              CALL_ZERO_PARAMETER_UPP((userUPP), uppMenuHookProcInfo)
    #define InvokeMBarHookUPP(menuRect, userUPP)                    (short)CALL_ONE_PARAMETER_UPP((userUPP), uppMBarHookProcInfo, (menuRect))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewMenuBarDefProc(userRoutine)                          NewMenuBarDefUPP(userRoutine)
#define NewMenuHookProc(userRoutine)                            NewMenuHookUPP(userRoutine)
#define NewMBarHookProc(userRoutine)                            NewMBarHookUPP(userRoutine)
#define CallMenuBarDefProc(userRoutine, selector, message, parameter1, parameter2) InvokeMenuBarDefUPP(selector, message, parameter1, parameter2, userRoutine)
#define CallMenuHookProc(userRoutine)                           InvokeMenuHookUPP(userRoutine)
#define CallMBarHookProc(userRoutine, menuRect)                 InvokeMBarHookUPP(menuRect, userRoutine)
enum {
    kMenuDefProcPtr             = 0                             /* raw proc-ptr access based on old MDEF */
};


typedef UInt32                          MenuDefType;

struct MenuDefSpec {
    MenuDefType                     defType;
    union {
        MenuDefUPP                      defProc;
    }                                 u;
};
typedef struct MenuDefSpec              MenuDefSpec;
typedef MenuDefSpec *                   MenuDefSpecPtr;
/*--------------------------------------------------------------------------------------*/
/*  o Menu Manager Initialization                                                       */
/*--------------------------------------------------------------------------------------*/
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
InitProcMenu                    (short                  resID)                              ONEWORDINLINE(0xA808);

EXTERN_API( void )
InitMenus                       (void)                                                      ONEWORDINLINE(0xA930);

/*--------------------------------------------------------------------------------------*/
/*  o Menu Manipulation                                                                 */
/*--------------------------------------------------------------------------------------*/
#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( MenuRef )
NewMenu                         (MenuID                 menuID,
                                 ConstStr255Param       menuTitle)                          ONEWORDINLINE(0xA931);

#if TARGET_OS_MAC
    #define MacGetMenu GetMenu
#endif
EXTERN_API( MenuRef )
MacGetMenu                      (short                  resourceID)                         ONEWORDINLINE(0xA9BF);

EXTERN_API( void )
DisposeMenu                     (MenuRef                theMenu)                            ONEWORDINLINE(0xA932);

EXTERN_API( void )
CalcMenuSize                    (MenuRef                theMenu)                            ONEWORDINLINE(0xA948);

EXTERN_API( short )
CountMenuItems                  (MenuRef                theMenu)                            ONEWORDINLINE(0xA950);

/* CountMItems() has been renamed to CountMenuItems() in Carbon */
#if CALL_NOT_IN_CARBON
EXTERN_API( short )
CountMItems                     (MenuRef                theMenu)                            ONEWORDINLINE(0xA950);

#if CALL_NOT_IN_CARBON
#define CountMenuItems( menu )  CountMItems( menu )
#endif /* CALL_NOT_IN_CARBON */

/* Routines available in Mac OS 8.5 and later */

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( OSStatus )
GetMenuFont                     (MenuRef                menu,
                                 SInt16 *               outFontID,
                                 UInt16 *               outFontSize);

EXTERN_API( OSStatus )
SetMenuFont                     (MenuRef                menu,
                                 SInt16                 inFontID,
                                 UInt16                 inFontSize);

EXTERN_API( Boolean )
GetMenuExcludesMarkColumn       (MenuRef                menu);

EXTERN_API( OSStatus )
SetMenuExcludesMarkColumn       (MenuRef                menu,
                                 Boolean                excludesMark);

/* Routines available in Carbon only*/

EXTERN_API( OSStatus )
RegisterMenuDefinition          (SInt16                 inResID,
                                 MenuDefSpecPtr         inDefSpec);

EXTERN_API( OSStatus )
CreateNewMenu                   (MenuID                 menuID,
                                 MenuAttributes         menuAttributes,
                                 MenuRef *              outMenuRef);

EXTERN_API( OSStatus )
CreateCustomMenu                (const MenuDefSpec *    defSpec,
                                 MenuID                 menuID,
                                 MenuAttributes         menuAttributes,
                                 MenuRef *              outMenuRef);

/*--------------------------------------------------------------------------------------*/
/*  o Menu Item Insertion                                                               */
/*--------------------------------------------------------------------------------------*/
#if TARGET_OS_MAC
    #define MacAppendMenu AppendMenu
#endif
EXTERN_API( void )
MacAppendMenu                   (MenuRef                menu,
                                 ConstStr255Param       data)                               ONEWORDINLINE(0xA933);

EXTERN_API( void )
InsertResMenu                   (MenuRef                theMenu,
                                 ResType                theType,
                                 short                  afterItem)                          ONEWORDINLINE(0xA951);

EXTERN_API( void )
AppendResMenu                   (MenuRef                theMenu,
                                 ResType                theType)                            ONEWORDINLINE(0xA94D);

#if TARGET_OS_MAC
    #define MacInsertMenuItem InsertMenuItem
#endif
EXTERN_API( void )
MacInsertMenuItem               (MenuRef                theMenu,
                                 ConstStr255Param       itemString,
                                 short                  afterItem)                          ONEWORDINLINE(0xA826);

EXTERN_API( void )
DeleteMenuItem                  (MenuRef                theMenu,
                                 short                  item)                               ONEWORDINLINE(0xA952);

EXTERN_API( void )
InsertFontResMenu               (MenuRef                theMenu,
                                 short                  afterItem,
                                 short                  scriptFilter)                       THREEWORDINLINE(0x303C, 0x0400, 0xA825);

EXTERN_API( void )
InsertIntlResMenu               (MenuRef                theMenu,
                                 ResType                theType,
                                 short                  afterItem,
                                 short                  scriptFilter)                       THREEWORDINLINE(0x303C, 0x0601, 0xA825);

/* Routines available in Mac OS 8.5 and later*/

EXTERN_API( OSStatus )
AppendMenuItemText              (MenuRef                menu,
                                 ConstStr255Param       inString);

EXTERN_API( OSStatus )
InsertMenuItemText              (MenuRef                menu,
                                 ConstStr255Param       inString,
                                 MenuItemIndex          afterItem);

/* Routines available in Carbon and later*/

/*--------------------------------------------------------------------------------------*/
/*  o Menu Events                                                                       */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( long )
MenuKey                         (CharParameter          ch)                                 ONEWORDINLINE(0xA93E);

EXTERN_API( long )
MenuSelect                      (Point                  startPt)                            ONEWORDINLINE(0xA93D);

EXTERN_API( long )
PopUpMenuSelect                 (MenuRef                menu,
                                 short                  top,
                                 short                  left,
                                 short                  popUpItem)                          ONEWORDINLINE(0xA80B);

EXTERN_API( long )
MenuChoice                      (void)                                                      ONEWORDINLINE(0xAA66);

/* Routines available in Mac OS 8.0 (Appearance 1.0) and later*/
EXTERN_API( UInt32 )
MenuEvent                       (const EventRecord *    inEvent)                            THREEWORDINLINE(0x303C, 0x020C, 0xA825);


/*--------------------------------------------------------------------------------------*/
/*  o Menu Bar                                                                          */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( short )
GetMBarHeight                   (void)                                                      TWOWORDINLINE(0x3EB8, 0x0BAA);

#if TARGET_OS_MAC
    #define MacDrawMenuBar DrawMenuBar
#endif
EXTERN_API( void )
MacDrawMenuBar                  (void)                                                      ONEWORDINLINE(0xA937);

EXTERN_API( void )
InvalMenuBar                    (void)                                                      ONEWORDINLINE(0xA81D);

EXTERN_API( void )
HiliteMenu                      (MenuID                 menuID)                             ONEWORDINLINE(0xA938);

EXTERN_API( Handle )
GetNewMBar                      (short                  menuBarID)                          ONEWORDINLINE(0xA9C0);

EXTERN_API( Handle )
GetMenuBar                      (void)                                                      ONEWORDINLINE(0xA93B);

EXTERN_API( void )
SetMenuBar                      (Handle                 menuList)                           ONEWORDINLINE(0xA93C);

EXTERN_API( MenuRef )
GetMenuHandle                   (MenuID                 menuID)                             ONEWORDINLINE(0xA949);

#if TARGET_OS_MAC
    #define MacInsertMenu InsertMenu
#endif
EXTERN_API( void )
MacInsertMenu                   (MenuRef                theMenu,
                                 MenuID                 beforeID)                           ONEWORDINLINE(0xA935);

#if TARGET_OS_MAC
    #define MacDeleteMenu DeleteMenu
#endif
EXTERN_API( void )
MacDeleteMenu                   (MenuID                 menuID)                             ONEWORDINLINE(0xA936);

EXTERN_API( void )
ClearMenuBar                    (void)                                                      ONEWORDINLINE(0xA934);

EXTERN_API( void )
SetMenuFlashCount               (short                  count)                              ONEWORDINLINE(0xA94A);

/* SetMenuFlash() has been renamed to SetMenuFlashCount() in Carbon */
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
SetMenuFlash                    (short                  count)                              ONEWORDINLINE(0xA94A);

#if CALL_NOT_IN_CARBON
#define SetMenuFlashCount( count )  SetMenuFlash( count )
#endif /* CALL_NOT_IN_CARBON */
#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( void )
FlashMenuBar                    (MenuID                 menuID)                             ONEWORDINLINE(0xA94C);

/* These are obsolete because Carbon does not support desk accessories.*/
#if CALL_NOT_IN_CARBON
EXTERN_API( Boolean )
SystemEdit                      (short                  editCmd)                            ONEWORDINLINE(0xA9C2);

EXTERN_API( void )
SystemMenu                      (long                   menuResult)                         ONEWORDINLINE(0xA9B5);

/* Routines available in Mac OS 8.5 and later*/
#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( Boolean )
IsMenuBarVisible                (void);

EXTERN_API( void )
ShowMenuBar                     (void);

EXTERN_API( void )
HideMenuBar                     (void);

/*--------------------------------------------------------------------------------------*/
/*  o Menu Item Accessors                                                               */
/*--------------------------------------------------------------------------------------*/
#if TARGET_OS_MAC
    #define MacCheckMenuItem CheckMenuItem
#endif
EXTERN_API( void )
MacCheckMenuItem                (MenuRef                theMenu,
                                 short                  item,
                                 Boolean                checked)                            ONEWORDINLINE(0xA945);

/* CheckItem() has been renamed to CheckMenuItem() in Carbon */
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
CheckItem                       (MenuRef                theMenu,
                                 short                  item,
                                 Boolean                checked)                            ONEWORDINLINE(0xA945);

#if TARGET_OS_MAC && CALL_NOT_IN_CARBON
#define CheckMenuItem( menu, item, checked ) CheckItem( menu, item, checked )
#endif
#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( void )
SetMenuItemText                 (MenuRef                theMenu,
                                 short                  item,
                                 ConstStr255Param       itemString)                         ONEWORDINLINE(0xA947);

EXTERN_API( void )
GetMenuItemText                 (MenuRef                theMenu,
                                 short                  item,
                                 Str255                 itemString)                         ONEWORDINLINE(0xA946);

EXTERN_API( void )
SetItemMark                     (MenuRef                theMenu,
                                 short                  item,
                                 CharParameter          markChar)                           ONEWORDINLINE(0xA944);

EXTERN_API( void )
GetItemMark                     (MenuRef                theMenu,
                                 short                  item,
                                 CharParameter *        markChar)                           ONEWORDINLINE(0xA943);

EXTERN_API( void )
SetItemCmd                      (MenuRef                theMenu,
                                 short                  item,
                                 CharParameter          cmdChar)                            ONEWORDINLINE(0xA84F);

EXTERN_API( void )
GetItemCmd                      (MenuRef                theMenu,
                                 short                  item,
                                 CharParameter *        cmdChar)                            ONEWORDINLINE(0xA84E);

EXTERN_API( void )
SetItemIcon                     (MenuRef                theMenu,
                                 short                  item,
                                 short                  iconIndex)                          ONEWORDINLINE(0xA940);

/* icon is returned in high byte of 16-bit iconIndex */
EXTERN_API( void )
GetItemIcon                     (MenuRef                theMenu,
                                 short                  item,
                                 short *                iconIndex)                          ONEWORDINLINE(0xA93F);

EXTERN_API( void )
SetItemStyle                    (MenuRef                theMenu,
                                 short                  item,
                                 StyleParameter         chStyle)                            ONEWORDINLINE(0xA942);

EXTERN_API( void )
GetItemStyle                    (MenuRef                theMenu,
                                 short                  item,
                                 Style *                chStyle);

/* These APIs are not supported in Carbon. Please use EnableMenuItem and */
/* DisableMenuItem (available back through Mac OS 8.5) instead.          */
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
DisableItem                     (MenuRef                theMenu,
                                 short                  item)                               ONEWORDINLINE(0xA93A);

EXTERN_API( void )
EnableItem                      (MenuRef                theMenu,
                                 short                  item)                               ONEWORDINLINE(0xA939);

/* Routines available in Mac OS 8.0 (Appearance 1.0) and later*/

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( OSErr )
SetMenuItemCommandID            (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 MenuCommand            inCommandID)                        THREEWORDINLINE(0x303C, 0x0502, 0xA825);

EXTERN_API( OSErr )
GetMenuItemCommandID            (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 MenuCommand *          outCommandID)                       THREEWORDINLINE(0x303C, 0x0503, 0xA825);

EXTERN_API( OSErr )
SetMenuItemModifiers            (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 UInt8                  inModifiers)                        THREEWORDINLINE(0x303C, 0x0404, 0xA825);

EXTERN_API( OSErr )
GetMenuItemModifiers            (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 UInt8 *                outModifiers)                       THREEWORDINLINE(0x303C, 0x0505, 0xA825);

EXTERN_API( OSErr )
SetMenuItemIconHandle           (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 UInt8                  inIconType,
                                 Handle                 inIconHandle)                       THREEWORDINLINE(0x303C, 0x0606, 0xA825);

EXTERN_API( OSErr )
GetMenuItemIconHandle           (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 UInt8 *                outIconType,
                                 Handle *               outIconHandle)                      THREEWORDINLINE(0x303C, 0x0707, 0xA825);

EXTERN_API( OSErr )
SetMenuItemTextEncoding         (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 TextEncoding           inScriptID)                         THREEWORDINLINE(0x303C, 0x0408, 0xA825);

EXTERN_API( OSErr )
GetMenuItemTextEncoding         (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 TextEncoding *         outScriptID)                        THREEWORDINLINE(0x303C, 0x0509, 0xA825);

EXTERN_API( OSErr )
SetMenuItemHierarchicalID       (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 MenuID                 inHierID)                           THREEWORDINLINE(0x303C, 0x040D, 0xA825);

EXTERN_API( OSErr )
GetMenuItemHierarchicalID       (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 MenuID *               outHierID)                          THREEWORDINLINE(0x303C, 0x050E, 0xA825);

EXTERN_API( OSErr )
SetMenuItemFontID               (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 SInt16                 inFontID)                           THREEWORDINLINE(0x303C, 0x040F, 0xA825);

EXTERN_API( OSErr )
GetMenuItemFontID               (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 SInt16 *               outFontID)                          THREEWORDINLINE(0x303C, 0x0510, 0xA825);

EXTERN_API( OSErr )
SetMenuItemRefCon               (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 UInt32                 inRefCon)                           THREEWORDINLINE(0x303C, 0x050A, 0xA825);

EXTERN_API( OSErr )
GetMenuItemRefCon               (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 UInt32 *               outRefCon)                          THREEWORDINLINE(0x303C, 0x050B, 0xA825);

/* Please use the menu item property APIs in Carbon.*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
SetMenuItemRefCon2              (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 UInt32                 inRefCon2)                          THREEWORDINLINE(0x303C, 0x0511, 0xA825);

EXTERN_API( OSErr )
GetMenuItemRefCon2              (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 UInt32 *               outRefCon2)                         THREEWORDINLINE(0x303C, 0x0512, 0xA825);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( OSErr )
SetMenuItemKeyGlyph             (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 SInt16                 inGlyph)                            THREEWORDINLINE(0x303C, 0x0513, 0xA825);

EXTERN_API( OSErr )
GetMenuItemKeyGlyph             (MenuRef                inMenu,
                                 SInt16                 inItem,
                                 SInt16 *               outGlyph)                           THREEWORDINLINE(0x303C, 0x0514, 0xA825);

/* Routines available in Mac OS 8.5 and later (supporting enabling/disabling of > 31 items)*/

#if TARGET_OS_MAC
    #define MacEnableMenuItem EnableMenuItem
#endif
EXTERN_API( void )
MacEnableMenuItem               (MenuRef                theMenu,
                                 MenuItemIndex          item);

EXTERN_API( void )
DisableMenuItem                 (MenuRef                theMenu,
                                 MenuItemIndex          item);

EXTERN_API( Boolean )
IsMenuItemEnabled               (MenuRef                menu,
                                 MenuItemIndex          item);

EXTERN_API( void )
EnableMenuItemIcon              (MenuRef                theMenu,
                                 MenuItemIndex          item)                               THREEWORDINLINE(0x303C, 0x0019, 0xA825);

EXTERN_API( void )
DisableMenuItemIcon             (MenuRef                theMenu,
                                 MenuItemIndex          item)                               THREEWORDINLINE(0x303C, 0x0020, 0xA825);

EXTERN_API( Boolean )
IsMenuItemIconEnabled           (MenuRef                menu,
                                 MenuItemIndex          item)                               THREEWORDINLINE(0x303C, 0x0018, 0xA825);

/*--------------------------------------------------------------------------------------*/
/*  o Menu Item Color Tables                                                            */
/*                                                                                      */
/*  Menu color manipulation is considered deprecated with the advent of the Appearance  */
/*  Manager.  Avoid using these routines if possible                                    */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
DeleteMCEntries                 (MenuID                 menuID,
                                 short                  menuItem)                           ONEWORDINLINE(0xAA60);

EXTERN_API( MCTableHandle )
GetMCInfo                       (void)                                                      ONEWORDINLINE(0xAA61);

EXTERN_API( void )
SetMCInfo                       (MCTableHandle          menuCTbl)                           ONEWORDINLINE(0xAA62);

EXTERN_API( void )
DisposeMCInfo                   (MCTableHandle          menuCTbl)                           ONEWORDINLINE(0xAA63);

EXTERN_API( MCEntryPtr )
GetMCEntry                      (MenuID                 menuID,
                                 short                  menuItem)                           ONEWORDINLINE(0xAA64);

EXTERN_API( void )
SetMCEntries                    (short                  numEntries,
                                 MCTablePtr             menuCEntries)                       ONEWORDINLINE(0xAA65);


/*--------------------------------------------------------------------------------------*/
/* o Properties  (Mac OS 8.5 and later)                                                 */
/*                                                                                      */
/* With the following property APIs, you can attach any piece of data you'd like to a   */
/* menu or menu item. Passing zero for the item number parameter indicates you'd like   */
/* to attach the data to the menu itself, and not to any specific menu item.            */
/*--------------------------------------------------------------------------------------*/
enum {
    kMenuPropertyPersistent     = 0x00000001                    /* whether this property gets saved when flattening the menu*/
};

EXTERN_API( OSStatus )
GetMenuItemProperty             (MenuRef                menu,
                                 MenuItemIndex          item,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32                 bufferSize,
                                 UInt32 *               actualSize,
                                 void *                 propertyBuffer);

EXTERN_API( OSStatus )
GetMenuItemPropertySize         (MenuRef                menu,
                                 MenuItemIndex          item,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32 *               size);

EXTERN_API( OSStatus )
SetMenuItemProperty             (MenuRef                menu,
                                 MenuItemIndex          item,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32                 propertySize,
                                 const void *           propertyData);

EXTERN_API( OSStatus )
RemoveMenuItemProperty          (MenuRef                menu,
                                 MenuItemIndex          item,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag);

EXTERN_API( OSStatus )
GetMenuItemPropertyAttributes   (MenuRef                menu,
                                 MenuItemIndex          item,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32 *               attributes);

EXTERN_API( OSStatus )
ChangeMenuItemPropertyAttributes (MenuRef               menu,
                                 MenuItemIndex          item,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 UInt32                 attributesToSet,
                                 UInt32                 attributesToClear);

/*--------------------------------------------------------------------------------------*/
/*  o Attributes (Carbon and later)                                                     */
/*                                                                                      */
/*  Each menu and menu item has attribute flags.                                        */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSStatus )
GetMenuAttributes               (MenuRef                menu,
                                 MenuAttributes *       outAttributes);

EXTERN_API( OSStatus )
ChangeMenuAttributes            (MenuRef                menu,
                                 MenuAttributes         setTheseAttributes,
                                 MenuAttributes         clearTheseAttributes);

EXTERN_API( OSStatus )
GetMenuItemAttributes           (MenuRef                menu,
                                 MenuItemIndex          item,
                                 MenuItemAttributes *   outAttributes);

EXTERN_API( OSStatus )
ChangeMenuItemAttributes        (MenuRef                menu,
                                 MenuItemIndex          item,
                                 MenuItemAttributes     setTheseAttributes,
                                 MenuItemAttributes     clearTheseAttributes);

/*--------------------------------------------------------------------------------------*/
/*  o Mass menu item enabling and disabling (Carbon and later)                          */
/*                                                                                      */
/*  Useful when rewriting code that whacks the enableFlags field directly.              */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( void )
DisableAllMenuItems             (MenuRef                theMenu);

EXTERN_API( void )
EnableAllMenuItems              (MenuRef                theMenu);

EXTERN_API( Boolean )
MenuHasEnabledItems             (MenuRef                theMenu);

/*--------------------------------------------------------------------------------------*/
/*  o Menu tracking status (Carbon and later)                                           */
/*                                                                                      */
/*  Get info about the selected menu item during menu tracking. Replaces direct access  */
/*  to low-mem globals that used to hold this info.                                     */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSStatus )
GetMenuTrackingData             (MenuRef                theMenu,
                                 MenuTrackingData *     outData);

/*--------------------------------------------------------------------------------------*/
/*  o.Universal command ID access (Carbon and later)                                    */
/*                                                                                      */
/*  These APIs allow you to operate on menu items strictly by command ID, with no       */
/*  knowledge of a menu item's index.                                                   */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( ItemCount )
CountMenuItemsWithCommandID     (MenuRef                menu,
                                 MenuCommand            commandID);

EXTERN_API( OSStatus )
GetIndMenuItemWithCommandID     (MenuRef                menu,
                                 MenuCommand            commandID,
                                 UInt32                 itemIndex,
                                 MenuRef *              outMenu,
                                 MenuItemIndex *        outIndex);

EXTERN_API( void )
EnableMenuCommand               (MenuRef                theMenu,
                                 MenuCommand            commandID);

EXTERN_API( void )
DisableMenuCommand              (MenuRef                theMenu,
                                 MenuCommand            commandID);

EXTERN_API( Boolean )
IsMenuCommandEnabled            (MenuRef                menu,
                                 MenuCommand            commandID);

EXTERN_API( OSStatus )
GetMenuCommandProperty          (MenuRef                menu,
                                 MenuCommand            commandID,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 ByteCount              bufferSize,
                                 ByteCount *            actualSize,
                                 void *                 propertyBuffer);

EXTERN_API( OSStatus )
GetMenuCommandPropertySize      (MenuRef                menu,
                                 MenuCommand            commandID,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 ByteCount *            size);

EXTERN_API( OSStatus )
SetMenuCommandProperty          (MenuRef                menu,
                                 MenuCommand            commandID,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag,
                                 ByteCount              propertySize,
                                 const void *           propertyData);

EXTERN_API( OSStatus )
RemoveMenuCommandProperty       (MenuRef                menu,
                                 MenuCommand            commandID,
                                 OSType                 propertyCreator,
                                 OSType                 propertyTag);


/*--------------------------------------------------------------------------------------*/
/*  o.Standard font menu (Carbon and later)                                             */
/*                                                                                      */
/*  These APIs allow you to create and use the standard font menu.                      */
/*--------------------------------------------------------------------------------------*/
enum {
    kHierarchicalFontMenuOption = 0x00000001
};

EXTERN_API( OSStatus )
CreateStandardFontMenu          (MenuRef                menu,
                                 MenuItemIndex          afterItem,
                                 MenuID                 firstHierMenuID,
                                 OptionBits             options,
                                 ItemCount *            outHierMenuCount);

EXTERN_API( OSStatus )
UpdateStandardFontMenu          (MenuRef                menu,
                                 ItemCount *            outHierMenuCount);

EXTERN_API( OSStatus )
GetFontFamilyFromMenuSelection  (MenuRef                menu,
                                 MenuItemIndex          item,
                                 FMFontFamily *         outFontFamily,
                                 FMFontStyle *          outStyle);

/*--------------------------------------------------------------------------------------*/
/*  o Contextual Menu routines and constants                                            */
/*  available with Conxtextual Menu extension 1.0 and later                             */
/*--------------------------------------------------------------------------------------*/
/* Gestalt Selector for classic 68K apps only. */
/* CFM apps should weak link and check the symbols. */
enum {
    gestaltContextualMenuAttr   = FOUR_CHAR_CODE('cmnu'),
    gestaltContextualMenuUnusedBit = 0,
    gestaltContextualMenuTrapAvailable = 1
};

/* Values indicating what kind of help the application supports */
enum {
    kCMHelpItemNoHelp           = 0,
    kCMHelpItemAppleGuide       = 1,
    kCMHelpItemOtherHelp        = 2
};

/* Values indicating what was chosen from the menu */
enum {
    kCMNothingSelected          = 0,
    kCMMenuItemSelected         = 1,
    kCMShowHelpSelected         = 3
};

EXTERN_API( OSStatus )
InitContextualMenus             (void)                                                      TWOWORDINLINE(0x7001, 0xAA72);

EXTERN_API( Boolean )
IsShowContextualMenuClick       (const EventRecord *    inEvent)                            TWOWORDINLINE(0x7002, 0xAA72);

EXTERN_API( OSStatus )
ContextualMenuSelect            (MenuRef                inMenu,
                                 Point                  inGlobalLocation,
                                 Boolean                inReserved,
                                 UInt32                 inHelpType,
                                 ConstStr255Param       inHelpItemString,
                                 const AEDesc *         inSelection,
                                 UInt32 *               outUserSelectionType,
                                 SInt16 *               outMenuID,
                                 MenuItemIndex *        outMenuItem)                        TWOWORDINLINE(0x7003, 0xAA72);

EXTERN_API( Boolean )
ProcessIsContextualMenuClient   (ProcessSerialNumber *  inPSN)                              TWOWORDINLINE(0x7004, 0xAA72);


/*--------------------------------------------------------------------------------------*/
/*  o Contextual Menu Plugin Notes                                                      */
/*                                                                                      */
/*  For Mac OS X, we will support a new type of Contextual Menu Plugin: the CFM-based   */
/*  plugin. Each plugin must be a separate file in the Contextual Menu Items subfolder  */
/*  of the system folder. It must export two functions and has the option of exporting  */
/*  a third; these three functions are virtually identical to the methods that must be  */
/*  supported by a SOM-based plugin.                                                        */
/*                                                                                      */
/*  The required symbols must be named "ExamineContext" and "HandleSelection".          */
/*  The optional symbol must be named "PostMenuCleanup".                                */
/*                                                                                      */
/*  The ExamineContext routine must have the following prototype:                       */
/*      pascal OSStatus ExamineContext( const AEDesc* inContext,                        */
/*                                      AEDescList* outCommandPairs );                  */
/*                                                                                      */
/*  The HandleSelection routine must have the following prototype:                      */
/*      pascal OSStatus HandleSelection(    const AEDesc* inContext,                    */
/*                                          SInt32 inCommandID );                       */
/*                                                                                      */
/*  The PostMenuCleanup routine must have the following prototype:                      */
/*      pascal void PostMenuCleanup(     void );                                            */
/*--------------------------------------------------------------------------------------*/

#if CALL_NOT_IN_CARBON
EXTERN_API_C( MenuRef )
newmenu                         (MenuID                 menuID,
                                 const char *           menuTitle);

EXTERN_API_C( void )
appendmenu                      (MenuRef                menu,
                                 const char *           data);

EXTERN_API_C( void )
insertmenuitem                  (MenuRef                theMenu,
                                 const char *           itemString,
                                 short                  afterItem);

EXTERN_API_C( long )
menuselect                      (const Point *          startPt);

EXTERN_API_C( void )
setmenuitemtext                 (MenuRef                menu,
                                 short                  item,
                                 const char *           itemString);

EXTERN_API_C( void )
getmenuitemtext                 (MenuRef                menu,
                                 short                  item,
                                 char *                 itemString);

#endif  /* CALL_NOT_IN_CARBON */

#if OLDROUTINENAMES
#define AddResMenu(theMenu, theType) AppendResMenu(theMenu, theType)
#define InsMenuItem(theMenu, itemString, afterItem) InsertMenuItem(theMenu, itemString, afterItem)
#define DelMenuItem( theMenu, item ) DeleteMenuItem( theMenu, item )
#if TARGET_OS_MAC
#define SetItem MacSetItem
#define GetItem MacGetItem
#endif
#define MacSetItem(theMenu, item, itemString) SetMenuItemText(theMenu, item, itemString)
#define MacGetItem(theMenu, item, itemString) GetMenuItemText(theMenu, item, itemString)
#define GetMHandle(menuID) GetMenuHandle(menuID)
#define DelMCEntries(menuID, menuItem) DeleteMCEntries(menuID, menuItem)
#define DispMCInfo(menuCTbl) DisposeMCInfo(menuCTbl)
#if CALL_NOT_IN_CARBON
#define addresmenu(menu, data) appendresmenu(menu, data)
#define getitem(menu, item, itemString) getmenuitemtext(menu, item, itemString)
#define setitem(menu, item, itemString) setmenuitemtext(menu, item, itemString)
#define insmenuitem(theMenu, itemString, afterItem) insertmenuitem(theMenu, itemString, afterItem)
#endif
#endif  /* OLDROUTINENAMES */

#if ACCESSOR_CALLS_ARE_FUNCTIONS
/* Getters */
EXTERN_API( MenuID )
GetMenuID                       (MenuRef                menu);

EXTERN_API( SInt16 )
GetMenuWidth                    (MenuRef                menu);

EXTERN_API( SInt16 )
GetMenuHeight                   (MenuRef                menu);

EXTERN_API( StringPtr )
GetMenuTitle                    (MenuRef                menu,
                                 Str255                 title);

EXTERN_API( OSStatus )
GetMenuDefinition               (MenuRef                menu,
                                 MenuDefSpecPtr         outDefSpec);

/* Setters */
EXTERN_API( void )
SetMenuID                       (MenuRef                menu,
                                 MenuID                 menuID);

EXTERN_API( void )
SetMenuWidth                    (MenuRef                menu,
                                 SInt16                 width);

EXTERN_API( void )
SetMenuHeight                   (MenuRef                menu,
                                 SInt16                 height);

EXTERN_API( OSStatus )
SetMenuTitle                    (MenuRef                menu,
                                 ConstStr255Param       title);

EXTERN_API( OSStatus )
SetMenuDefinition               (MenuRef                menu,
                                 const MenuDefSpec *    defSpec);

#endif  /* ACCESSOR_CALLS_ARE_FUNCTIONS */



#if TARGET_OS_WIN32
#endif  /* TARGET_OS_WIN32 */


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

#endif /* __MENUS__ */

