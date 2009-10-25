/*
     File:       MacHelp.h
 
     Contains:   Macintosh Help Package Interfaces.
 
     Version:    Technology: CarbonLib 1.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1998-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MACHELP__
#define __MACHELP__

#ifndef __CONTROLS__
#include <Controls.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __MACWINDOWS__
#include <MacWindows.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __MOVIES__
#include <Movies.h>
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

/*----------------------------------------------------------------------------------*/
/* Help Manager constants, etc.                                                     */
/*----------------------------------------------------------------------------------*/
#ifndef __BALLOONS__
enum {
    kMacHelpVersion             = 0x0003,
    hmBalloonHelpVersion        = kMacHelpVersion               /* Backwards compatibility */
};

#endif  /* !defined(__BALLOONS__) */


typedef SInt16 HMContentRequest;
enum {
    kHMSupplyContent            = 0,
    kHMDisposeContent           = 1
};


typedef UInt32 HMContentType;
enum {
    kHMNoContent                = FOUR_CHAR_CODE('none'),
    kHMPascalStrContent         = FOUR_CHAR_CODE('pstr'),
    kHMPictResContent           = FOUR_CHAR_CODE('pict'),
    kHMStringResContent         = FOUR_CHAR_CODE('str#'),
    kHMTEHandleContent          = FOUR_CHAR_CODE('txth'),
    kHMPictHandleContent        = FOUR_CHAR_CODE('pcth'),
    kHMTextResContent           = FOUR_CHAR_CODE('text'),
    kHMStrResContent            = FOUR_CHAR_CODE('str '),
    kHMMovieContent             = FOUR_CHAR_CODE('moov')
};


typedef SInt16 HMTagDisplaySide;
enum {
    kHMDefaultSide              = 0,
    kHMTopSide                  = 1,
    kHMLeftSide                 = 2,
    kHMBottomSide               = 3,
    kHMRightSide                = 4,
    kHMTopLeftCorner            = 5,
    kHMTopRightCorner           = 6,
    kHMLeftTopCorner            = 7,
    kHMLeftBottomCorner         = 8,
    kHMBottomLeftCorner         = 9,
    kHMBottomRightCorner        = 10,
    kHMRightTopCorner           = 11,
    kHMRightBottomCorner        = 12
};


typedef SInt16 HMContentProvidedType;
enum {
    kHMContentProvided          = 0,
    kHMContentNotProvided       = 1,
    kHMContentNotProvidedDontPropagate = 2
};

enum {
    kHMMinimumContentIndex      = 0,
    kHMMaximumContentIndex      = 1
};

enum {
    kHMIllegalContentForMinimumState = -1000                    /* *** Need real numbers here*/
};

enum {
    kHMHelpPropertyContentTag   = FOUR_CHAR_CODE('help')
};

#ifndef __BALLOONS__

struct HMStringResType {
    short                           hmmResID;
    short                           hmmIndex;
};
typedef struct HMStringResType          HMStringResType;
#endif  /* !defined(__BALLOONS__) */


struct HMHelpContent {
    HMContentType                   contentType;
    union {
        Str255                          tagString;              /* Pascal String*/
        SInt16                          tagPictID;              /* 'PICT' resource ID*/
        HMStringResType                 tagStringRes;           /* STR# resource ID and index*/
        TEHandle                        tagTEHandle;            /* TextEdit handle*/
        PicHandle                       tagPictHandle;          /* Picture handle*/
        SInt16                          tagTextRes;             /* TEXT/styl resource ID*/
        SInt16                          tagStrRes;              /* STR resource ID*/
        Movie                           tagMovie;               /* QuickTime movie*/
    }                                 u;
};
typedef struct HMHelpContent            HMHelpContent;

struct HMHelpContentRec {
    SInt32                          version;
    Rect                            absHotRect;
    HMTagDisplaySide                tagSide;
    HMHelpContent                   content[2];
};
typedef struct HMHelpContentRec         HMHelpContentRec;

typedef HMHelpContentRec *              HMHelpContentPtr;
/*------------------------------------------------------------------------------------------*/
/* Callback procs                                       */
/*------------------------------------------------------------------------------------------ */
typedef CALLBACK_API( OSStatus , HMControlContentProcPtr )(ControlRef inControl, Point inGlobalMouse, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentPtr ioHelpContent);
typedef CALLBACK_API( OSStatus , HMWindowContentProcPtr )(WindowPtr inWindow, Point inGlobalMouse, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentPtr ioHelpContent);
typedef CALLBACK_API( OSStatus , HMMenuTitleContentProcPtr )(MenuHandle inMenu, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentPtr ioHelpContent);
typedef CALLBACK_API( OSStatus , HMMenuItemContentProcPtr )(const MenuTrackingData *inTrackingData, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentPtr ioHelpContent);
typedef STACK_UPP_TYPE(HMControlContentProcPtr)                 HMControlContentUPP;
typedef STACK_UPP_TYPE(HMWindowContentProcPtr)                  HMWindowContentUPP;
typedef STACK_UPP_TYPE(HMMenuTitleContentProcPtr)               HMMenuTitleContentUPP;
typedef STACK_UPP_TYPE(HMMenuItemContentProcPtr)                HMMenuItemContentUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(HMControlContentUPP)
    NewHMControlContentUPP         (HMControlContentProcPtr userRoutine);

    EXTERN_API(HMWindowContentUPP)
    NewHMWindowContentUPP          (HMWindowContentProcPtr  userRoutine);

    EXTERN_API(HMMenuTitleContentUPP)
    NewHMMenuTitleContentUPP       (HMMenuTitleContentProcPtr userRoutine);

    EXTERN_API(HMMenuItemContentUPP)
    NewHMMenuItemContentUPP        (HMMenuItemContentProcPtr userRoutine);

    EXTERN_API(void)
    DisposeHMControlContentUPP     (HMControlContentUPP     userUPP);

    EXTERN_API(void)
    DisposeHMWindowContentUPP      (HMWindowContentUPP      userUPP);

    EXTERN_API(void)
    DisposeHMMenuTitleContentUPP    (HMMenuTitleContentUPP  userUPP);

    EXTERN_API(void)
    DisposeHMMenuItemContentUPP    (HMMenuItemContentUPP    userUPP);

    EXTERN_API(OSStatus)
    InvokeHMControlContentUPP      (ControlRef              inControl,
                                    Point                   inGlobalMouse,
                                    HMContentRequest        inRequest,
                                    HMContentProvidedType * outContentProvided,
                                    HMHelpContentPtr        ioHelpContent,
                                    HMControlContentUPP     userUPP);

    EXTERN_API(OSStatus)
    InvokeHMWindowContentUPP       (WindowPtr               inWindow,
                                    Point                   inGlobalMouse,
                                    HMContentRequest        inRequest,
                                    HMContentProvidedType * outContentProvided,
                                    HMHelpContentPtr        ioHelpContent,
                                    HMWindowContentUPP      userUPP);

    EXTERN_API(OSStatus)
    InvokeHMMenuTitleContentUPP    (MenuHandle              inMenu,
                                    HMContentRequest        inRequest,
                                    HMContentProvidedType * outContentProvided,
                                    HMHelpContentPtr        ioHelpContent,
                                    HMMenuTitleContentUPP   userUPP);

    EXTERN_API(OSStatus)
    InvokeHMMenuItemContentUPP     (const MenuTrackingData * inTrackingData,
                                    HMContentRequest        inRequest,
                                    HMContentProvidedType * outContentProvided,
                                    HMHelpContentPtr        ioHelpContent,
                                    HMMenuItemContentUPP    userUPP);

#else
    enum { uppHMControlContentProcInfo = 0x0000FBF0 };              /* pascal 4_bytes Func(4_bytes, 4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppHMWindowContentProcInfo = 0x0000FBF0 };               /* pascal 4_bytes Func(4_bytes, 4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppHMMenuTitleContentProcInfo = 0x00003EF0 };            /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppHMMenuItemContentProcInfo = 0x00003EF0 };             /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    #define NewHMControlContentUPP(userRoutine)                     (HMControlContentUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppHMControlContentProcInfo, GetCurrentArchitecture())
    #define NewHMWindowContentUPP(userRoutine)                      (HMWindowContentUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppHMWindowContentProcInfo, GetCurrentArchitecture())
    #define NewHMMenuTitleContentUPP(userRoutine)                   (HMMenuTitleContentUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppHMMenuTitleContentProcInfo, GetCurrentArchitecture())
    #define NewHMMenuItemContentUPP(userRoutine)                    (HMMenuItemContentUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppHMMenuItemContentProcInfo, GetCurrentArchitecture())
    #define DisposeHMControlContentUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeHMWindowContentUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeHMMenuTitleContentUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define DisposeHMMenuItemContentUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeHMControlContentUPP(inControl, inGlobalMouse, inRequest, outContentProvided, ioHelpContent, userUPP)  (OSStatus)CALL_FIVE_PARAMETER_UPP((userUPP), uppHMControlContentProcInfo, (inControl), (inGlobalMouse), (inRequest), (outContentProvided), (ioHelpContent))
    #define InvokeHMWindowContentUPP(inWindow, inGlobalMouse, inRequest, outContentProvided, ioHelpContent, userUPP)  (OSStatus)CALL_FIVE_PARAMETER_UPP((userUPP), uppHMWindowContentProcInfo, (inWindow), (inGlobalMouse), (inRequest), (outContentProvided), (ioHelpContent))
    #define InvokeHMMenuTitleContentUPP(inMenu, inRequest, outContentProvided, ioHelpContent, userUPP)  (OSStatus)CALL_FOUR_PARAMETER_UPP((userUPP), uppHMMenuTitleContentProcInfo, (inMenu), (inRequest), (outContentProvided), (ioHelpContent))
    #define InvokeHMMenuItemContentUPP(inTrackingData, inRequest, outContentProvided, ioHelpContent, userUPP)  (OSStatus)CALL_FOUR_PARAMETER_UPP((userUPP), uppHMMenuItemContentProcInfo, (inTrackingData), (inRequest), (outContentProvided), (ioHelpContent))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewHMControlContentProc(userRoutine)                    NewHMControlContentUPP(userRoutine)
#define NewHMWindowContentProc(userRoutine)                     NewHMWindowContentUPP(userRoutine)
#define NewHMMenuTitleContentProc(userRoutine)                  NewHMMenuTitleContentUPP(userRoutine)
#define NewHMMenuItemContentProc(userRoutine)                   NewHMMenuItemContentUPP(userRoutine)
#define CallHMControlContentProc(userRoutine, inControl, inGlobalMouse, inRequest, outContentProvided, ioHelpContent) InvokeHMControlContentUPP(inControl, inGlobalMouse, inRequest, outContentProvided, ioHelpContent, userRoutine)
#define CallHMWindowContentProc(userRoutine, inWindow, inGlobalMouse, inRequest, outContentProvided, ioHelpContent) InvokeHMWindowContentUPP(inWindow, inGlobalMouse, inRequest, outContentProvided, ioHelpContent, userRoutine)
#define CallHMMenuTitleContentProc(userRoutine, inMenu, inRequest, outContentProvided, ioHelpContent) InvokeHMMenuTitleContentUPP(inMenu, inRequest, outContentProvided, ioHelpContent, userRoutine)
#define CallHMMenuItemContentProc(userRoutine, inTrackingData, inRequest, outContentProvided, ioHelpContent) InvokeHMMenuItemContentUPP(inTrackingData, inRequest, outContentProvided, ioHelpContent, userRoutine)
/*------------------------------------------------------------------------------------------*/
/* API                                                                                      */
/*------------------------------------------------------------------------------------------*/
/* Installing/Retrieving Content */
EXTERN_API( OSStatus )
HMSetControlHelpContent         (ControlRef             inControl,
                                 const HMHelpContentRec * inContent);

EXTERN_API( OSStatus )
HMGetControlHelpContent         (ControlRef             inControl,
                                 HMHelpContentRec *     outContent);

EXTERN_API( OSStatus )
HMSetWindowHelpContent          (WindowRef              inWindow,
                                 const HMHelpContentRec * inContent);

EXTERN_API( OSStatus )
HMGetWindowHelpContent          (WindowRef              inWindow,
                                 HMHelpContentRec *     outContent);

EXTERN_API( OSStatus )
HMSetMenuItemHelpContent        (MenuRef                inMenu,
                                 MenuItemIndex          inItem,
                                 const HMHelpContentRec * inContent);

EXTERN_API( OSStatus )
HMGetMenuItemHelpContent        (MenuRef                inMenu,
                                 MenuItemIndex          inItem,
                                 HMHelpContentRec *     outContent);

/* Installing/Retrieving Content Callbacks */
EXTERN_API( OSStatus )
HMInstallControlContentCallback (ControlRef             inControl,
                                 HMControlContentUPP    inContentUPP);

EXTERN_API( OSStatus )
HMInstallWindowContentCallback  (WindowRef              inWindow,
                                 HMWindowContentUPP     inContentUPP);

EXTERN_API( OSStatus )
HMInstallMenuTitleContentCallback (MenuRef              inMenu,
                                 HMMenuTitleContentUPP  inContentUPP);

EXTERN_API( OSStatus )
HMInstallMenuItemContentCallback (MenuRef               inMenu,
                                 HMMenuItemContentUPP   inContentUPP);


EXTERN_API( OSStatus )
HMGetControlContentCallback     (ControlRef             inControl,
                                 HMControlContentUPP *  outContentUPP);

EXTERN_API( OSStatus )
HMGetWindowContentCallback      (WindowRef              inWindow,
                                 HMWindowContentUPP *   outContentUPP);

EXTERN_API( OSStatus )
HMGetMenuTitleContentCallback   (MenuRef                inMenu,
                                 HMMenuTitleContentUPP * outContentUPP);

EXTERN_API( OSStatus )
HMGetMenuItemContentCallback    (MenuRef                inMenu,
                                 HMMenuItemContentUPP * outContentUPP);

/* Enabling and Disabling Help Tags */
EXTERN_API( Boolean )
HMAreHelpTagsDisplayed          (void);

EXTERN_API( OSStatus )
HMSetHelpTagsDisplayed          (Boolean                inDisplayTags);

EXTERN_API( OSStatus )
HMSetTagDelay                   (Duration               inDelay);

EXTERN_API( OSStatus )
HMGetTagDelay                   (Duration *             outDelay);

/* Compatibility */
EXTERN_API( OSStatus )
HMSetMenuHelpFromBalloonRsrc    (MenuRef                inMenu,
                                 SInt16                 inHmnuRsrcID);

EXTERN_API( OSStatus )
HMSetDialogHelpFromBalloonRsrc  (DialogRef              inDialog,
                                 SInt16                 inHdlgRsrcID,
                                 SInt16                 inItemStart);


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

#endif /* __MACHELP__ */

