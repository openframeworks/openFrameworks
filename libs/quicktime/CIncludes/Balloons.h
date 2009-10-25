/*
     File:       Balloons.h
 
     Contains:   Balloon Help Package Interfaces.
 
     Version:    Technology: System 7.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __BALLOONS__
#define __BALLOONS__

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

/*
   Carbon clients should use MacHelp. The definitions below will NOT work for Carbon and
   are only defined for those files that need to build pre-Carbon applications.
*/
/* o.constants*/


typedef SInt16 BalloonVariant;
enum {
    kTopLeftTipPointsLeftVariant = 0,
    kTopLeftTipPointsUpVariant  = 1,
    kTopRightTipPointsUpVariant = 2,
    kTopRightTipPointsRightVariant = 3,
    kBottomRightTipPointsRightVariant = 4,
    kBottomRightTipPointsDownVariant = 5,
    kBottomLeftTipPointsDownVariant = 6,
    kBottomLeftTipPointsLeftVariant = 7,
    kBalloonVariantCount        = 8
};


#ifndef __MACHELP__
enum {
    hmBalloonHelpVersion        = 0x0002                        /* The real version of the Help Manager */
};

#endif  /* !defined(__MACHELP__) */

enum {
    kHMHelpMenuID               = -16490,                       /* Resource ID and menu ID of help menu */
    kHMAboutHelpItem            = 1,                            /* help menu item number of About Balloon Help... */
    kHMShowBalloonsItem         = 3                             /* help menu item number of Show/Hide Balloons */
};

enum {
    kHMHelpID                   = -5696,                        /* ID of various Help Mgr package resources (in Pack14 range) */
    kBalloonWDEFID              = 126                           /* Resource ID of the WDEF proc used in standard balloons */
};

enum {
                                                                /* Dialog item template type constant */
    helpItem                    = 1                             /* key value in DITL template that corresponds to the help item */
};

enum {
                                                                /* Options for Help Manager resources in 'hmnu', 'hdlg', 'hrct', 'hovr', & 'hfdr' resources */
    hmDefaultOptions            = 0,                            /* default options for help manager resources */
    hmUseSubID                  = 1,                            /* treat resID's in resources as subID's of driver base ID (for Desk Accessories) */
    hmAbsoluteCoords            = 2                             /* ignore window port origin and treat rectangles as absolute coords (local to window) */
};

enum {
    hmSaveBitsNoWindow          = 4,                            /* don't create a window, just blast bits on screen. No update event is generated */
    hmSaveBitsWindow            = 8,                            /* create a window, but restore bits behind window when window goes away & generate update event */
    hmMatchInTitle              = 16                            /* for hwin resources, match string anywhere in window title string */
};

enum {
                                                                /* Constants for Help Types in 'hmnu', 'hdlg', 'hrct', 'hovr', & 'hfdr' resources */
    kHMStringItem               = 1,                            /* pstring used in resource */
    kHMPictItem                 = 2,                            /* 'PICT' ResID used in resource */
    kHMStringResItem            = 3,                            /* 'STR#' ResID & index used in resource */
    kHMTEResItem                = 6,                            /* Styled Text Edit ResID used in resource ('TEXT' & 'styl') */
    kHMSTRResItem               = 7,                            /* 'STR ' ResID used in resource */
    kHMSkipItem                 = 256,                          /* don't display a balloon */
    kHMCompareItem              = 512,                          /* Compare pstring in menu item w/ PString in resource item ('hmnu' only) */
    kHMNamedResourceItem        = 1024,                         /* Use pstring in menu item to get 'STR#', 'PICT', or 'STR ' resource ('hmnu' only) */
    kHMTrackCntlItem            = 2048                          /* Reserved */
};

enum {
                                                                /* Constants for hmmHelpType's when filling out HMMessageRecord */
    khmmString                  = 1,                            /* help message contains a PString */
    khmmPict                    = 2,                            /* help message contains a resource ID to a 'PICT' resource */
    khmmStringRes               = 3,                            /* help message contains a res ID & index to a 'STR#' resource */
    khmmTEHandle                = 4,                            /* help message contains a Text Edit handle */
    khmmPictHandle              = 5,                            /* help message contains a Picture handle */
    khmmTERes                   = 6,                            /* help message contains a res ID to 'TEXT' & 'styl' resources */
    khmmSTRRes                  = 7,                            /* help message contains a res ID to a 'STR ' resource */
    kHMEnabledItem              = 0                             /* item is enabled, but not checked or control value = 0 */
};

enum {
                                                                /* ResTypes for Styled TE Handles in Resources */
    kHMTETextResType            = FOUR_CHAR_CODE('TEXT'),       /* Resource Type of text data for styled TE record w/o style info */
    kHMTEStyleResType           = FOUR_CHAR_CODE('styl')        /* Resource Type of style information for styled TE record */
};

enum {
    kHMDisabledItem             = 1,                            /* item is disabled, grayed in menus or disabled in dialogs */
    kHMCheckedItem              = 2,                            /* item is enabled, and checked or control value = 1 */
    kHMOtherItem                = 3,                            /* item is enabled, and control value > 1 */
                                                                /* Method parameters to pass to HMShowBalloon */
    kHMRegularWindow            = 0,                            /* Create a regular window floating above all windows */
    kHMSaveBitsNoWindow         = 1,                            /* Just save the bits and draw (for MDEF calls) */
    kHMSaveBitsWindow           = 2                             /* Regular window, save bits behind, AND generate update event */
};

enum {
                                                                /* Resource Types for whichType parameter used when extracting 'hmnu' & 'hdlg' messages */
    kHMMenuResType              = FOUR_CHAR_CODE('hmnu'),       /* ResType of help resource for supporting menus */
    kHMDialogResType            = FOUR_CHAR_CODE('hdlg'),       /* ResType of help resource for supporting dialogs */
    kHMWindListResType          = FOUR_CHAR_CODE('hwin'),       /* ResType of help resource for supporting windows */
    kHMRectListResType          = FOUR_CHAR_CODE('hrct'),       /* ResType of help resource for rectangles in windows */
    kHMOverrideResType          = FOUR_CHAR_CODE('hovr'),       /* ResType of help resource for overriding system balloons */
    kHMFinderApplResType        = FOUR_CHAR_CODE('hfdr')        /* ResType of help resource for custom balloon in Finder */
};

#ifndef __MACHELP__

struct HMStringResType {
    short                           hmmResID;
    short                           hmmIndex;
};
typedef struct HMStringResType          HMStringResType;
#endif  /* !defined(__MACHELP__) */


struct HMMessageRecord {
    SInt16                          hmmHelpType;
    union {
        Str255                          hmmString;
        SInt16                          hmmPict;
        TEHandle                        hmmTEHandle;
        HMStringResType                 hmmStringRes;
        SInt16                          hmmPictRes;
        PicHandle                       hmmPictHandle;
        SInt16                          hmmTERes;
        SInt16                          hmmSTRRes;
    }                                 u;
};
typedef struct HMMessageRecord          HMMessageRecord;

typedef HMMessageRecord *               HMMessageRecPtr;
typedef CALLBACK_API( OSErr , TipFunctionProcPtr )(Point tip, RgnHandle structure, Rect *r, BalloonVariant *balloonVariant);
typedef STACK_UPP_TYPE(TipFunctionProcPtr)                      TipFunctionUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(TipFunctionUPP)
    NewTipFunctionUPP              (TipFunctionProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeTipFunctionUPP          (TipFunctionUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeTipFunctionUPP           (Point                   tip,
                                    RgnHandle               structure,
                                    Rect *                  r,
                                    BalloonVariant *        balloonVariant,
                                    TipFunctionUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppTipFunctionProcInfo = 0x00003FE0 };                   /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewTipFunctionUPP(userRoutine)                          (TipFunctionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTipFunctionProcInfo, GetCurrentArchitecture())
    #define DisposeTipFunctionUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeTipFunctionUPP(tip, structure, r, balloonVariant, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppTipFunctionProcInfo, (tip), (structure), (r), (balloonVariant))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewTipFunctionProc(userRoutine)                         NewTipFunctionUPP(userRoutine)
#define CallTipFunctionProc(userRoutine, tip, structure, r, balloonVariant) InvokeTipFunctionUPP(tip, structure, r, balloonVariant, userRoutine)
/*  Public Interfaces  */
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
HMGetHelpMenuHandle             (MenuHandle *           mh)                                 THREEWORDINLINE(0x303C, 0x0200, 0xA830);

EXTERN_API( OSErr )
HMShowBalloon                   (const HMMessageRecord * inHelpMessage,
                                 Point                  inTip,
                                 Rect *                 inHotRect, /* can be NULL */
                                 TipFunctionUPP         inTipProc,
                                 SInt16                 inWindowProcID,
                                 BalloonVariant         inBalloonVariant,
                                 SInt16                 inMethod)                           THREEWORDINLINE(0x303C, 0x0B01, 0xA830);

EXTERN_API( OSErr )
HMShowMenuBalloon               (SInt16                 itemNum,
                                 SInt16                 itemMenuID,
                                 SInt32                 itemFlags,
                                 SInt32                 itemReserved,
                                 Point                  tip,
                                 Rect *                 alternateRect, /* can be NULL */
                                 TipFunctionUPP         tipProc,
                                 SInt16                 theProc,
                                 BalloonVariant         balloonVariant)                     THREEWORDINLINE(0x303C, 0x0E05, 0xA830);

EXTERN_API( OSErr )
HMRemoveBalloon                 (void)                                                      THREEWORDINLINE(0x303C, 0x0002, 0xA830);

EXTERN_API( OSErr )
HMGetIndHelpMsg                 (ResType                inWhichResType,
                                 SInt16                 inWhichResID,
                                 SInt16                 inMessageIndex,
                                 SInt16                 inMessageState,
                                 UInt32 *               outOptions,
                                 Point *                outTip,
                                 Rect *                 outHotRect,
                                 SInt16 *               outWindowProcID,
                                 BalloonVariant *       outBalloonVariant,
                                 HMMessageRecord *      outHelpMessage,
                                 SInt16 *               outMessageCount)                    THREEWORDINLINE(0x303C, 0x1306, 0xA830);

EXTERN_API( Boolean )
HMIsBalloon                     (void)                                                      THREEWORDINLINE(0x303C, 0x0007, 0xA830);

EXTERN_API( Boolean )
HMGetBalloons                   (void)                                                      THREEWORDINLINE(0x303C, 0x0003, 0xA830);

EXTERN_API( OSErr )
HMSetBalloons                   (Boolean                flag)                               THREEWORDINLINE(0x303C, 0x0104, 0xA830);

EXTERN_API( OSErr )
HMSetFont                       (SInt16                 font)                               THREEWORDINLINE(0x303C, 0x0108, 0xA830);

EXTERN_API( OSErr )
HMSetFontSize                   (UInt16                 fontSize)                           THREEWORDINLINE(0x303C, 0x0109, 0xA830);

EXTERN_API( OSErr )
HMGetFont                       (SInt16 *               font)                               THREEWORDINLINE(0x303C, 0x020A, 0xA830);

EXTERN_API( OSErr )
HMGetFontSize                   (UInt16 *               fontSize)                           THREEWORDINLINE(0x303C, 0x020B, 0xA830);

EXTERN_API( OSErr )
HMSetDialogResID                (SInt16                 resID)                              THREEWORDINLINE(0x303C, 0x010C, 0xA830);

EXTERN_API( OSErr )
HMSetMenuResID                  (SInt16                 menuID,
                                 SInt16                 resID)                              THREEWORDINLINE(0x303C, 0x020D, 0xA830);

EXTERN_API( OSErr )
HMBalloonRect                   (const HMMessageRecord * inMessage,
                                 Rect *                 outRect)                            THREEWORDINLINE(0x303C, 0x040E, 0xA830);

EXTERN_API( OSErr )
HMBalloonPict                   (const HMMessageRecord * inMessage,
                                 PicHandle *            outPict)                            THREEWORDINLINE(0x303C, 0x040F, 0xA830);

EXTERN_API( OSErr )
HMScanTemplateItems             (SInt16                 whichID,
                                 SInt16                 whichResFile,
                                 ResType                whichType)                          THREEWORDINLINE(0x303C, 0x0410, 0xA830);

EXTERN_API( OSErr )
HMExtractHelpMsg                (ResType                inType,
                                 SInt16                 inResID,
                                 SInt16                 inMessageIndex,
                                 SInt16                 inMessageState,
                                 HMMessageRecord *      outMessage)                         THREEWORDINLINE(0x303C, 0x0711, 0xA830);

EXTERN_API( OSErr )
HMGetDialogResID                (SInt16 *               resID)                              THREEWORDINLINE(0x303C, 0x0213, 0xA830);

EXTERN_API( OSErr )
HMGetMenuResID                  (SInt16                 menuID,
                                 SInt16 *               resID)                              THREEWORDINLINE(0x303C, 0x0314, 0xA830);

EXTERN_API( OSErr )
HMGetBalloonWindow              (WindowPtr *            window)                             THREEWORDINLINE(0x303C, 0x0215, 0xA830);

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

#endif /* __BALLOONS__ */

