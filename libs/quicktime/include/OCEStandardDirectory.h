/*
     File:       OCEStandardDirectory.h
 
     Contains:   Apple Open Collaboration Environment Standard Directory Interfaces.
 
     Version:    Technology: AOCE Toolbox 1.02
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OCESTANDARDDIRECTORY__
#define __OCESTANDARDDIRECTORY__

#ifndef REZ
#ifndef __ICONS__
#include <Icons.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif


#ifndef __OCE__
#include <OCE.h>
#endif

#ifndef __OCEAUTHDIR__
#include <OCEAuthDir.h>
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

#endif /* REZ */

/* generic icon suites */
#define genericDirectoryIconResource            -16721  /* icl8, icl4, ICN#, ics#, ics4, ics8, sicn  */
#define genericLockedDirectoryIconResource      -16716  /* icl8, icl4, ICN#, ics#, ics4, ics8, sicn  */
#define genericRecordIconResource               -16722  /* icl8, icl4, ICN#, ics#, ics4, ics8, sicn  */
#define genericAttributeIconResource            -16723  /* icl8, icl4, ICN#, ics#, ics4, ics8, sicn  */
#define genericTemplateIconResource             -16746  /* icl8, icl4, ICN#, ics#, ics4, ics8 */
/* standard icon suites */
#define directoryFolderIconResource             -16720  /* icl8, icl4, ICN#, ics#, ics4, ics8, sicn  */
#define lockedDirectoryFolderIconResource       -16719  /* icl8, icl4, ICN#, ics#, ics4, ics8, sicn  */
#define personalDirectoryIconResource           -16718  /* icl8, icl4, ICN#, ics#, ics4, ics8, sicn  */
#define directoriesIconResource                 -16717  /* icl8, icl4, ICN#, ics#, ics4, ics8, sicn  */
#define preferredPersonalDirectoryIconResource  -16724  /* icl8, icl4, ICN#, ics#, ics4, ics8, sicn  */
/* icon IDs for spinning arrows */
#define kFirstSpinnerIcon                       -16745
#define kLastSpinnerIcon                        -16738
/* resource types */
#define kSDPPanelResourceType                   'panl'
#define kSDPFindPanelResourceType               'find'
/* Standard FindPanel resource */
#define kStandardFindLayout                     -16700
#ifndef REZ
/* Prompt For Identity structures */
enum {
    kSDPGuestBit                = 0,
    kSDPSpecificIdentityBit     = 1,
    kSDPLocalIdentityBit        = 2
};

/* Values of SDPIdentityKind */
enum {
    kSDPGuestMask               = 1 << kSDPGuestBit,
    kSDPSpecificIdentityMask    = 1 << kSDPSpecificIdentityBit,
    kSDPLocalIdentityMask       = 1 << kSDPLocalIdentityBit
};

typedef unsigned short                  SDPIdentityKind;
enum {
    kSDPSuggestionOnly          = 0,
    kSDPRestrictToDirectory     = 1,
    kSDPRestrictToRecord        = 2
};

typedef unsigned short                  SDPLoginFilterKind;


/* Panel Structures */
/*
While the panel is in operation, four selection states may exist.
    1) kSDPNothingSelected means nothing is selected.
    2) kContainSelected means a volume, folder, catalog, dnode, or PAB is selected.
    3) kSDPLockedContainerSelected means one of the above, but no access privledges.
    4) kSDPRecordSelected means that a record is currently selected.
*/
/* Values of SDPSelectionState */
enum {
    kSDPNothingSelected         = 0,
    kSDPLockedContainerSelected = 1,
    kSDPContainerSelected       = 2,
    kSDPRecordSelected          = 3,
    kSDPRecordAliasSelected     = 4,
    kSDPContainerAliasSelected  = 5
};

typedef unsigned short                  SDPSelectionState;


/*
This type informs the caller of the action the user took, either as the result
of an event (as returned by SDPPanelEvent) or when SDPOpenSelectedItem is called.

kSDPProcessed means that the event (or call to SDPOpenSelectedItem) resulted in no
state change.

kSDPSelectedAnItem indicates that the user wants to select the currently-hilited
record. This is returned, for example, when a user double-clicks on a record.

kSDPChangedSelection implies that the user clicked on a new item (which may mean
that no item is selected).
*/
/* Values of SDPPanelState */
enum {
    kSDPProcessed               = 0,
    kSDPSelectedAnItem          = 1,
    kSDPChangedSelection        = 2
};

typedef unsigned short                  SDPPanelState;
/*
Your application may read any of the fields in a SDPPanelRecord, but it should
use the appropriate routines to make changes to the records with the exception
of the refCon field which your application may read or write at will.  Private
information follows the SDPPanelRecord, so the handle must not be re-sized.
*/

struct SDPPanelRecord {
    Rect                            bounds;
    Boolean                         visible;
    Boolean                         enabled;
    Boolean                         focused;
    Byte                            padByte;
    AuthIdentity                    identity;
    long                            refCon;
    Rect                            listRect;
    Rect                            popupRect;
    short                           numberOfRows;
    short                           rowHeight;
    Boolean                         pabMode;
    Boolean                         filler1;
};
typedef struct SDPPanelRecord           SDPPanelRecord;

typedef SDPPanelRecord *                SDPPanelPtr;
typedef SDPPanelPtr *                   SDPPanelHandle;
typedef CALLBACK_API( void , PanelBusyProcPtr )(SDPPanelHandle Panel, Boolean busy);
typedef STACK_UPP_TYPE(PanelBusyProcPtr)                        PanelBusyUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(PanelBusyUPP)
    NewPanelBusyUPP                (PanelBusyProcPtr        userRoutine);

    EXTERN_API(void)
    DisposePanelBusyUPP            (PanelBusyUPP            userUPP);

    EXTERN_API(void)
    InvokePanelBusyUPP             (SDPPanelHandle          Panel,
                                    Boolean                 busy,
                                    PanelBusyUPP            userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppPanelBusyProcInfo = 0x000001C0 };                     /* pascal no_return_value Func(4_bytes, 1_byte) */
    #define NewPanelBusyUPP(userRoutine)                            (PanelBusyUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPanelBusyProcInfo, GetCurrentArchitecture())
    #define DisposePanelBusyUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define InvokePanelBusyUPP(Panel, busy, userUPP)                CALL_TWO_PARAMETER_UPP((userUPP), uppPanelBusyProcInfo, (Panel), (busy))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewPanelBusyProc(userRoutine)                           NewPanelBusyUPP(userRoutine)
#define CallPanelBusyProc(userRoutine, Panel, busy)             InvokePanelBusyUPP(Panel, busy, userRoutine)
typedef PanelBusyUPP                    PanelBusyProc;
/* Find Panel Structures */
enum {
    kSDPItemIsSelectedBit       = 0,
    kSDPFindTextExistsBit       = 1
};

/* Values of SDPFindPanelState */
enum {
    kSDPItemIsSelectedMask      = 1 << kSDPItemIsSelectedBit,
    kSDPFindTextExistsMask      = 1 << kSDPFindTextExistsBit
};

typedef unsigned short                  SDPFindPanelState;
/* Values of SDPFindPanelFocus */
enum {
    kSDPFindPanelNoFocus        = 0,
    kSDPFindPanelListHasFocus   = 1,
    kSDPFindPanelTextHasFocus   = 2
};

typedef unsigned short                  SDPFindPanelFocus;



struct SDPFindPanelRecord {
    Point                           upperLeft;
    Boolean                         visible;
    Boolean                         enabled;
    Boolean                         nowFinding;
    Byte                            padByte;
    SDPFindPanelFocus               currentFocus;
    AuthIdentity                    identity;
    short                           simultaneousSearchCount;
    long                            refCon;
};
typedef struct SDPFindPanelRecord       SDPFindPanelRecord;

typedef SDPFindPanelRecord *            SDPFindPanelPtr;
typedef SDPFindPanelPtr *               SDPFindPanelHandle;
/* Values of SDPFindPanelResult */
enum {
    kSDPSelectedAFindItem       = 0,
    kSDPFindSelectionChanged    = 1,
    kSDPFindCompleted           = 2,
    kSDPTextStateChanged        = 3,
    kSDPFocusChanged            = 4,
    kSDPSelectionAndFocusChanged = 5,
    kSDPMenuChanged             = 6,
    kSDPSelectionAndMenuChanged = 7,
    kSDPProcessedFind           = 8
};

typedef unsigned short                  SDPFindPanelResult;
typedef PackedPathNamePtr *             PackedRStringListHandle;
typedef CALLBACK_API( void , FindPanelBusyProcPtr )(SDPFindPanelHandle findPanel, Boolean busy);
typedef STACK_UPP_TYPE(FindPanelBusyProcPtr)                    FindPanelBusyUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(FindPanelBusyUPP)
    NewFindPanelBusyUPP            (FindPanelBusyProcPtr    userRoutine);

    EXTERN_API(void)
    DisposeFindPanelBusyUPP        (FindPanelBusyUPP        userUPP);

    EXTERN_API(void)
    InvokeFindPanelBusyUPP         (SDPFindPanelHandle      findPanel,
                                    Boolean                 busy,
                                    FindPanelBusyUPP        userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppFindPanelBusyProcInfo = 0x000001C0 };                 /* pascal no_return_value Func(4_bytes, 1_byte) */
    #define NewFindPanelBusyUPP(userRoutine)                        (FindPanelBusyUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppFindPanelBusyProcInfo, GetCurrentArchitecture())
    #define DisposeFindPanelBusyUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define InvokeFindPanelBusyUPP(findPanel, busy, userUPP)        CALL_TWO_PARAMETER_UPP((userUPP), uppFindPanelBusyProcInfo, (findPanel), (busy))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewFindPanelBusyProc(userRoutine)                       NewFindPanelBusyUPP(userRoutine)
#define CallFindPanelBusyProc(userRoutine, findPanel, busy)     InvokeFindPanelBusyUPP(findPanel, busy, userRoutine)
typedef FindPanelBusyUPP                FindPanelBusyProc;
/* Prompt For Identity Routines */
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
SDPPromptForID                  (AuthIdentity *         id,
                                 ConstStr255Param       guestPrompt,
                                 ConstStr255Param       specificIDPrompt,
                                 ConstStr255Param       localIDPrompt,
                                 const RString *        recordType,
                                 SDPIdentityKind        permittedKinds,
                                 SDPIdentityKind *      selectedKind,
                                 const RecordID *       loginFilter,
                                 SDPLoginFilterKind     filterKind)                         FOURWORDINLINE(0x203C, 0x0010, 0x0388, 0xAA5D);



/*
SDPNewPanel creates a new panel. You supply the window in which the panel
is to live, the bounds for the panel (which includes both the menu and the list),
whether or not the panel is to be initially visible, the initial RLI (nil for
catalogs and volumes), the types of records that will be shown (only a single
(non-nil) type which may contain wildcards), the identity by which to browse
(for access control reasons), and a refCon which is  available to the caller.
*/
EXTERN_API( OSErr )
SDPNewPanel                     (SDPPanelHandle *       newPanel,
                                 WindowPtr              window,
                                 const Rect *           bounds,
                                 Boolean                visible,
                                 Boolean                enabled,
                                 const PackedRLI *      initialRLI,
                                 const RStringPtr *     typesList,
                                 unsigned long          typeCount,
                                 AuthIdentity           identity,
                                 DirEnumChoices         enumFlags,
                                 DirMatchWith           matchTypeHow,
                                 long                   refCon)                             FOURWORDINLINE(0x203C, 0x0015, 0x0064, 0xAA5D);


/*
SDPSetIdentity Sets the identity used for browsing.  NOTE: This call is likely to go
away if the new authentication scheme works.
*/
EXTERN_API( OSErr )
SDPSetIdentity                  (SDPPanelHandle         panel,
                                 AuthIdentity           identity)                           FOURWORDINLINE(0x203C, 0x0004, 0x0073, 0xAA5D);




/*
SDPGetNewPanel is similar to SDPNewPanel above, except it takes a resource id of a
'panl' resource.
*/
EXTERN_API( OSErr )
SDPGetNewPanel                  (SDPPanelHandle *       newPanel,
                                 short                  resourceID,
                                 WindowPtr              window,
                                 const PackedRLI *      initialRLI,
                                 AuthIdentity           identity)                           FOURWORDINLINE(0x203C, 0x0009, 0x0065, 0xAA5D);


/*
Call this when you're completely done with a panel. It deallocates all of the
associated data structures.
*/
EXTERN_API( OSErr )
SDPDisposePanel                 (SDPPanelHandle         panel)                              FOURWORDINLINE(0x203C, 0x0002, 0x0066, 0xAA5D);


/*
If the panel is visible, it makes it invisible by hiding the menu, turning off
drawing of the list, and erasing and  invaling the list's rectangle.
*/
EXTERN_API( OSErr )
SDPHidePanel                    (SDPPanelHandle         panel)                              FOURWORDINLINE(0x203C, 0x0002, 0x0067, 0xAA5D);


/* If the panel is invisible, it makes it visible and draws it. */
EXTERN_API( OSErr )
SDPShowPanel                    (SDPPanelHandle         panel)                              FOURWORDINLINE(0x203C, 0x0002, 0x0068, 0xAA5D);


/* Disables the list and menu so that it won't accept any commands. */
EXTERN_API( OSErr )
SDPEnablePanel                  (SDPPanelHandle         panel,
                                 Boolean                enable)                             FOURWORDINLINE(0x203C, 0x0003, 0x0069, 0xAA5D);

/*
Move the upper-left-hand corner of the panel to (h,v), given in local coordinates of
the panel's window.
*/
EXTERN_API( OSErr )
SDPMovePanel                    (SDPPanelHandle         panel,
                                 short                  h,
                                 short                  v)                                  FOURWORDINLINE(0x203C, 0x0004, 0x006B, 0xAA5D);


/*
Resizes the panel to have the given width and height (keeping the upper-left-hand
corner in a fixed position).
*/
EXTERN_API( OSErr )
SDPSizePanel                    (SDPPanelHandle         panel,
                                 short                  width,
                                 short                  height)                             FOURWORDINLINE(0x203C, 0x0004, 0x006C, 0xAA5D);


/*
This routine simulates a double-click on the selected item (if there is no selected
item, then it does nothing), and it returns the result of that "double-click" via the
whatHappened parameter. If a container is selected, then that container will be
opened and kMovedDownTheHierarchy is returned. If, however, the user is at the lowest
level in the hierarchy, then kSDPSelectedAnItem is returned.
*/
EXTERN_API( OSErr )
SDPOpenSelectedItem             (SDPPanelHandle         panel,
                                 SDPPanelState *        whatHappened)                       FOURWORDINLINE(0x203C, 0x0004, 0x006D, 0xAA5D);


/*
Returns whether a record is selected, something else is selected, or nothing is
selected.
*/
EXTERN_API( OSErr )
SDPGetPanelSelectionState       (SDPPanelHandle         panel,
                                 SDPSelectionState *    itsState)                           FOURWORDINLINE(0x203C, 0x0004, 0x006E, 0xAA5D);


/*
Returns the size of the currently-selected DSSpec, or zero if a record is
not selected.  It is safe to do a NewPtr (SDPGetPanelSelectionSize (...))
*/
EXTERN_API( OSErr )
SDPGetPanelSelectionSize        (SDPPanelHandle         panel,
                                 unsigned short *       dsSpecSize)                         FOURWORDINLINE(0x203C, 0x0004, 0x0072, 0xAA5D);


/*
Returns the currently-selected DSSpec, or a zero-lengthed
DSSpec if a record is not selected. It is assumed that the selection
buffer allocated is large enough.
*/
EXTERN_API( OSErr )
SDPGetPanelSelection            (SDPPanelHandle         panel,
                                 PackedDSSpec *         selection)                          FOURWORDINLINE(0x203C, 0x0004, 0x006F, 0xAA5D);


/*
Forces browsing to the specified RLI. If prli is the current RLI, then it does
nothing.
*/
EXTERN_API( OSErr )
SDPSetPath                      (SDPPanelHandle         panel,
                                 const PackedRLI *      prli)                               FOURWORDINLINE(0x203C, 0x0004, 0x0070, 0xAA5D);


/*
This is the main driver for the panel.  You should pass all events to SDPPanelEvent
including NULL events.  If you have more than 1 panel, you will need to
call SDPPanelEvent for each panel.  NOTE:  For Update Events you will also need to call
SDPUpdatePanel as described below.
*/
EXTERN_API( OSErr )
SDPPanelEvent                   (SDPPanelHandle         panel,
                                 const EventRecord *    theEvent,
                                 SDPPanelState *        whatHappened)                       FOURWORDINLINE(0x203C, 0x0006, 0x0071, 0xAA5D);



/*
SDPUpdatePanel should be called in response to an update event.  Generally you will
have first called BeginUpdate, and pass the windows visRgn as theRgn.  If NULL
is passed for theRgn the entire panel is re-drawn.  NOTE:  Drawing is not cliped to
theRgn, if this is desired, you must first call SetClip.
*/
EXTERN_API( OSErr )
SDPUpdatePanel                  (SDPPanelHandle         panel,
                                 RgnHandle              theRgn)                             FOURWORDINLINE(0x203C, 0x0004, 0x006A, 0xAA5D);


/*
SDPSelectString scrolls and selects the closest matching string at the current
level.  This is the same behavior as if the user typed in the given string.
*/
EXTERN_API( OSErr )
SDPSelectString                 (SDPPanelHandle         panel,
                                 const RString *        name)                               FOURWORDINLINE(0x203C, 0x0004, 0x0074, 0xAA5D);

/*
SDPGetPathLength returns the length in bytes required to hold the current path name
in RLI format.  This corresponds to the path name in the popup menu.
*/
EXTERN_API( OSErr )
SDPGetPathLength                (SDPPanelHandle         panel,
                                 unsigned short *       pathNameLength)                     FOURWORDINLINE(0x203C, 0x0004, 0x0075, 0xAA5D);

/* SDPGetPath returns the current rli. */
EXTERN_API( OSErr )
SDPGetPath                      (SDPPanelHandle         panel,
                                 PackedRLI *            prli,
                                 short *                dsRefNum)                           FOURWORDINLINE(0x203C, 0x0006, 0x0076, 0xAA5D);

/*
SDPSetFocus will draw the focus rectangle or erase the focus rectangle depending upon
the focus boolean.
*/
EXTERN_API( OSErr )
SDPSetFocus                     (SDPPanelHandle         panel,
                                 Boolean                focus)                              FOURWORDINLINE(0x203C, 0x0003, 0x0077, 0xAA5D);

/*
SDPSetPanelBalloonHelp allows clients to specify a STR# resource id to use for 
balloon help.  Balloon help is unavailable until this call is made.
*/
EXTERN_API( OSErr )
SDPSetPanelBalloonHelp          (SDPPanelHandle         panel,
                                 short                  balloonHelpID)                      FOURWORDINLINE(0x203C, 0x0003, 0x0078, 0xAA5D);

/*
SDPInstallPanelBusyProc allows clients to install a CB that will be called while
the panel is aynchronously busy.
*/
EXTERN_API( OSErr )
SDPInstallPanelBusyProc         (SDPPanelHandle         panel,
                                 PanelBusyProc          busyProc)                           FOURWORDINLINE(0x203C, 0x0004, 0x0079, 0xAA5D);

EXTERN_API( OSErr )
SDPNewFindPanel                 (SDPFindPanelHandle *   newPanel,
                                 WindowPtr              window,
                                 Point                  upperLeft,
                                 short                  layoutResourceID,
                                 Boolean                visible,
                                 Boolean                enabled,
                                 const RStringPtr *     typesList,
                                 long                   typeCount,
                                 DirMatchWith           matchTypeHow,
                                 AuthIdentity           identity,
                                 short                  simultaneousSearchCount,
                                 SDPFindPanelFocus      initialFocus,
                                 long                   refCon)                             FOURWORDINLINE(0x203C, 0x0014, 0x08FC, 0xAA5D);

EXTERN_API( OSErr )
SDPDisposeFindPanel             (SDPFindPanelHandle     findPanel)                          FOURWORDINLINE(0x203C, 0x0002, 0x08FD, 0xAA5D);

EXTERN_API( OSErr )
SDPStartFind                    (SDPFindPanelHandle     findPanel)                          FOURWORDINLINE(0x203C, 0x0002, 0x08FE, 0xAA5D);

EXTERN_API( OSErr )
SDPStopFind                     (SDPFindPanelHandle     findPanel)                          FOURWORDINLINE(0x203C, 0x0002, 0x08FF, 0xAA5D);

EXTERN_API( OSErr )
SDPFindPanelEvent               (SDPFindPanelHandle     findPanel,
                                 const EventRecord *    event,
                                 SDPFindPanelResult *   whatHappened)                       FOURWORDINLINE(0x203C, 0x0006, 0x0900, 0xAA5D);

EXTERN_API( OSErr )
SDPUpdateFindPanel              (SDPFindPanelHandle     findPanel,
                                 RgnHandle              theRgn)                             FOURWORDINLINE(0x203C, 0x0004, 0x0901, 0xAA5D);

EXTERN_API( OSErr )
SDPShowFindPanel                (SDPFindPanelHandle     findPanel)                          FOURWORDINLINE(0x203C, 0x0002, 0x0902, 0xAA5D);

EXTERN_API( OSErr )
SDPHideFindPanel                (SDPFindPanelHandle     findPanel)                          FOURWORDINLINE(0x203C, 0x0002, 0x0903, 0xAA5D);

EXTERN_API( OSErr )
SDPMoveFindPanel                (SDPFindPanelHandle     findPanel,
                                 short                  h,
                                 short                  v)                                  FOURWORDINLINE(0x203C, 0x0004, 0x0904, 0xAA5D);

EXTERN_API( OSErr )
SDPEnableFindPanel              (SDPFindPanelHandle     findPanel,
                                 Boolean                enabled)                            FOURWORDINLINE(0x203C, 0x0003, 0x0905, 0xAA5D);

EXTERN_API( OSErr )
SDPSetFindPanelFocus            (SDPFindPanelHandle     findPanel,
                                 SDPFindPanelFocus      newFocus)                           FOURWORDINLINE(0x203C, 0x0003, 0x0906, 0xAA5D);

EXTERN_API( OSErr )
SDPGetFindPanelState            (SDPFindPanelHandle     findPanel,
                                 SDPFindPanelState *    itsState)                           FOURWORDINLINE(0x203C, 0x0004, 0x0907, 0xAA5D);

EXTERN_API( OSErr )
SDPGetFindPanelSelectionSize    (SDPFindPanelHandle     findPanel,
                                 unsigned short *       size)                               FOURWORDINLINE(0x203C, 0x0004, 0x0908, 0xAA5D);

EXTERN_API( OSErr )
SDPGetFindPanelSelection        (SDPFindPanelHandle     findPanel,
                                 PackedDSSpec *         selection)                          FOURWORDINLINE(0x203C, 0x0004, 0x0909, 0xAA5D);

EXTERN_API( OSErr )
SDPSetFindPanelBalloonHelp      (SDPFindPanelHandle     findPanel,
                                 short                  balloonHelpID)                      FOURWORDINLINE(0x203C, 0x0003, 0x090A, 0xAA5D);

EXTERN_API( OSErr )
SDPSetFindIdentity              (SDPFindPanelHandle     findPanel,
                                 AuthIdentity           identity)                           FOURWORDINLINE(0x203C, 0x0004, 0x090B, 0xAA5D);

EXTERN_API( OSErr )
SDPInstallFindPanelBusyProc     (SDPFindPanelHandle     findPanel,
                                 FindPanelBusyProc      busyProc)                           FOURWORDINLINE(0x203C, 0x0004, 0x090C, 0xAA5D);



EXTERN_API( OSErr )
SDPGetIconByType                (const RString *        recordType,
                                 IconSelectorValue      whichIcons,
                                 Handle *               iconSuite)                          THREEWORDINLINE(0x303C, 0x0400, 0xAA5C);

EXTERN_API( OSErr )
SDPGetDSSpecIcon                (const PackedDSSpec *   packedDSSpec,
                                 IconSelectorValue      whichIcons,
                                 Handle *               iconSuite)                          THREEWORDINLINE(0x303C, 0x0401, 0xAA5C);

EXTERN_API( OSErr )
SDPGetCategories                (PackedRStringListHandle * categories,
                                 PackedRStringListHandle * displayNames)                    THREEWORDINLINE(0x303C, 0x0402, 0xAA5C);

EXTERN_API( OSErr )
SDPGetCategoryTypes             (const RString *        category,
                                 PackedRStringListHandle * types)                           THREEWORDINLINE(0x303C, 0x0403, 0xAA5C);




EXTERN_API( OSErr )
SDPResolveAliasFile             (FSSpecPtr              fileSpec,
                                 PackedDSSpecHandle     resolvedDSSpec,
                                 AuthIdentity           identity,
                                 Boolean                mayPromptUser)                      FOURWORDINLINE(0x203C, 0x0007, 0x0E74, 0xAA5D);

EXTERN_API( OSErr )
SDPResolveAliasDSSpec           (PackedDSSpecHandle     theAliasDSSpec,
                                 AuthIdentity           identity,
                                 Boolean                mayPromptUser)                      FOURWORDINLINE(0x203C, 0x0005, 0x0E75, 0xAA5D);

EXTERN_API( OSErr )
SDPRepairPersonalDirectory      (FSSpec *               pd,
                                 Boolean                showProgress)                       FOURWORDINLINE(0x203C, 0x0003, 0x1A2C, 0xAA5D);


#endif /* REZ */
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

#endif /* __OCESTANDARDDIRECTORY__ */

