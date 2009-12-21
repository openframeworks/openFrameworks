/*
     File:       ColorPickerComponents.h
 
     Contains:   Color Picker Component Interfaces.
 
     Version:    Technology: Mac OS 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __COLORPICKERCOMPONENTS__
#define __COLORPICKERCOMPONENTS__

#ifndef __COLORPICKER__
#include <ColorPicker.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __BALLOONS__
#include <Balloons.h>
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
    kPickerComponentType        = FOUR_CHAR_CODE('cpkr')
};

enum {
    kPickerInit                 = 0,
    kPickerTestGraphicsWorld    = 1,
    kPickerGetDialog            = 2,
    kPickerGetItemList          = 3,
    kPickerGetColor             = 4,
    kPickerSetColor             = 5,
    kPickerEvent                = 6,
    kPickerEdit                 = 7,
    kPickerSetVisibility        = 8,
    kPickerDisplay              = 9,
    kPickerItemHit              = 10,
    kPickerSetBaseItem          = 11,
    kPickerGetProfile           = 12,
    kPickerSetProfile           = 13,
    kPickerGetPrompt            = 14,
    kPickerSetPrompt            = 15,
    kPickerGetIconData          = 16,
    kPickerGetEditMenuState     = 17,
    kPickerSetOrigin            = 18,
    kPickerExtractHelpItem      = 19,
    kPickerSetColorChangedProc  = 20,
    kNPickerGetColor            = 21,
    kNPickerSetColor            = 22,
    kNPickerGetProfile          = 23,
    kNPickerSetProfile          = 24,
    kNPickerSetColorChangedProc = 25
};

/* These structs were moved here from the ColorPicker header.*/
enum {
    kPickerDidNothing           = 0,                            /* was kDidNothing */
    kPickerColorChanged         = 1,                            /* was kColorChanged */
    kPickerOkHit                = 2,                            /* was kOkHit */
    kPickerCancelHit            = 3,                            /* was kCancelHit */
    kPickerNewPickerChosen      = 4,                            /* was kNewPickerChosen */
    kPickerApplItemHit          = 5                             /* was kApplItemHit */
};

typedef short                           PickerAction;
enum {
    kOriginalColor              = 0,
    kNewColor                   = 1
};

typedef short                           PickerColorType;
enum {
    kPickerCut                  = 0,                            /* was kCut */
    kPickerCopy                 = 1,                            /* was kCopy */
    kPickerPaste                = 2,                            /* was kPaste */
    kPickerClear                = 3,                            /* was kClear */
    kPickerUndo                 = 4                             /* was kUndo */
};

typedef short                           PickerEditOperation;
enum {
    kPickerMouseDown            = 0,                            /* was kMouseDown */
    kPickerKeyDown              = 1,                            /* was kKeyDown */
    kPickerFieldEntered         = 2,                            /* was kFieldEntered */
    kPickerFieldLeft            = 3,                            /* was kFieldLeft */
    kPickerCutOp                = 4,                            /* was kCutOp */
    kPickerCopyOp               = 5,                            /* was kCopyOp */
    kPickerPasteOp              = 6,                            /* was kPasteOp */
    kPickerClearOp              = 7,                            /* was kClearOp */
    kPickerUndoOp               = 8                             /* was kUndoOp */
};

typedef short                           PickerItemModifier;
/* These are for the flags field in the picker's 'thng' resource. */
enum {
    kPickerCanDoColor           = 1,                            /* was CanDoColor */
    kPickerCanDoBlackWhite      = 2,                            /* was CanDoBlackWhite */
    kPickerAlwaysModifiesPalette = 4,                           /* was AlwaysModifiesPalette */
    kPickerMayModifyPalette     = 8,                            /* was MayModifyPalette */
    kPickerIsColorSyncAware     = 16,                           /* was PickerIsColorSyncAware */
    kPickerCanDoSystemDialog    = 32,                           /* was CanDoSystemDialog */
    kPickerCanDoApplDialog      = 64,                           /* was CanDoApplDialog */
    kPickerHasOwnDialog         = 128,                          /* was HasOwnDialog */
    kPickerCanDetach            = 256,                          /* was CanDetach */
    kPickerIsColorSync2Aware    = 512                           /* was PickerIsColorSync2Aware */
};

enum {
    kPickerNoForcast            = 0,                            /* was kNoForcast */
    kPickerMenuChoice           = 1,                            /* was kMenuChoice */
    kPickerDialogAccept         = 2,                            /* was kDialogAccept */
    kPickerDialogCancel         = 3,                            /* was kDialogCancel */
    kPickerLeaveFocus           = 4,                            /* was kLeaveFocus */
    kPickerSwitch               = 5,
    kPickerNormalKeyDown        = 6,                            /* was kNormalKeyDown */
    kPickerNormalMouseDown      = 7                             /* was kNormalMouseDown */
};

typedef short                           PickerEventForcaster;

struct PickerIconData {
    short                           scriptCode;
    short                           iconSuiteID;
    ResType                         helpResType;
    short                           helpResID;
};
typedef struct PickerIconData           PickerIconData;

struct PickerInitData {
    DialogPtr                       pickerDialog;
    DialogPtr                       choicesDialog;
    long                            flags;
    Picker                          yourself;
};
typedef struct PickerInitData           PickerInitData;

struct PickerMenuState {
    Boolean                         cutEnabled;
    Boolean                         copyEnabled;
    Boolean                         pasteEnabled;
    Boolean                         clearEnabled;
    Boolean                         undoEnabled;
    SInt8                           filler;
    Str255                          undoString;
};
typedef struct PickerMenuState          PickerMenuState;

struct SystemDialogInfo {
    long                            flags;
    long                            pickerType;
    DialogPlacementSpec             placeWhere;
    Point                           dialogOrigin;
    PickerMenuItemInfo              mInfo;
};
typedef struct SystemDialogInfo         SystemDialogInfo;

struct PickerDialogInfo {
    long                            flags;
    long                            pickerType;
    Point *                         dialogOrigin;
    PickerMenuItemInfo              mInfo;
};
typedef struct PickerDialogInfo         PickerDialogInfo;

struct ApplicationDialogInfo {
    long                            flags;
    long                            pickerType;
    DialogPtr                       theDialog;
    Point                           pickerOrigin;
    PickerMenuItemInfo              mInfo;
};
typedef struct ApplicationDialogInfo    ApplicationDialogInfo;

struct PickerEventData {
    EventRecord *                   event;
    PickerAction                    action;
    short                           itemHit;
    Boolean                         handled;
    SInt8                           filler;
    ColorChangedUPP                 colorProc;
    long                            colorProcData;
    PickerEventForcaster            forcast;
};
typedef struct PickerEventData          PickerEventData;

struct PickerEditData {
    PickerEditOperation             theEdit;
    PickerAction                    action;
    Boolean                         handled;
    SInt8                           filler;
};
typedef struct PickerEditData           PickerEditData;

struct PickerItemHitData {
    short                           itemHit;
    PickerItemModifier              iMod;
    PickerAction                    action;
    ColorChangedUPP                 colorProc;
    long                            colorProcData;
    Point                           where;
};
typedef struct PickerItemHitData        PickerItemHitData;

struct PickerHelpItemInfo {
    long                            options;
    Point                           tip;
    Rect                            altRect;
    short                           theProc;
    short                           helpVariant;
    HMMessageRecord                 helpMessage;
};
typedef struct PickerHelpItemInfo       PickerHelpItemInfo;
#if OLDROUTINENAMES
enum {
    kInitPicker                 = kPickerInit,
    kTestGraphicsWorld          = kPickerTestGraphicsWorld,
    kGetDialog                  = kPickerGetDialog,
    kGetItemList                = kPickerGetItemList,
    kGetColor                   = kPickerGetColor,
    kSetColor                   = kPickerSetColor,
    kEvent                      = kPickerEvent,
    kEdit                       = kPickerEdit,
    kSetVisibility              = kPickerSetVisibility,
    kDrawPicker                 = kPickerDisplay,
    kItemHit                    = kPickerItemHit,
    kSetBaseItem                = kPickerSetBaseItem,
    kGetProfile                 = kPickerGetProfile,
    kSetProfile                 = kPickerSetProfile,
    kGetPrompt                  = kPickerGetPrompt,
    kSetPrompt                  = kPickerSetPrompt,
    kGetIconData                = kPickerGetIconData,
    kGetEditMenuState           = kPickerGetEditMenuState,
    kSetOrigin                  = kPickerSetOrigin,
    kExtractHelpItem            = kPickerExtractHelpItem
};

enum {
    kDidNothing                 = kPickerDidNothing,
    kColorChanged               = kPickerColorChanged,
    kOkHit                      = kPickerOkHit,
    kCancelHit                  = kPickerCancelHit,
    kNewPickerChosen            = kPickerNewPickerChosen,
    kApplItemHit                = kPickerApplItemHit
};

enum {
    kCut                        = kPickerCut,
    kCopy                       = kPickerCopy,
    kPaste                      = kPickerPaste,
    kClear                      = kPickerClear,
    kUndo                       = kPickerUndo
};

enum {
    kMouseDown                  = kPickerMouseDown,
    kKeyDown                    = kPickerKeyDown,
    kFieldEntered               = kPickerFieldEntered,
    kFieldLeft                  = kPickerFieldLeft,
    kCutOp                      = kPickerCutOp,
    kCopyOp                     = kPickerCopyOp,
    kPasteOp                    = kPickerPasteOp,
    kClearOp                    = kPickerClearOp,
    kUndoOp                     = kPickerUndoOp
};

enum {
    kNoForcast                  = kPickerNoForcast,
    kMenuChoice                 = kPickerMenuChoice,
    kDialogAccept               = kPickerDialogAccept,
    kDialogCancel               = kPickerDialogCancel,
    kLeaveFocus                 = kPickerLeaveFocus,
    kNormalKeyDown              = kPickerNormalKeyDown,
    kNormalMouseDown            = kPickerNormalMouseDown
};


typedef short                           ColorType;
typedef short                           EditOperation;
typedef short                           ItemModifier;
typedef short                           EventForcaster;

struct EventData {
    EventRecord *                   event;
    PickerAction                    action;
    short                           itemHit;
    Boolean                         handled;
    SInt8                           filler;
    ColorChangedUPP                 colorProc;
    long                            colorProcData;
    EventForcaster                  forcast;
};
typedef struct EventData                EventData;

struct EditData {
    EditOperation                   theEdit;
    PickerAction                    action;
    Boolean                         handled;
    SInt8                           filler;
};
typedef struct EditData                 EditData;

struct ItemHitData {
    short                           itemHit;
    ItemModifier                    iMod;
    PickerAction                    action;
    ColorChangedUPP                 colorProc;
    long                            colorProcData;
    Point                           where;
};
typedef struct ItemHitData              ItemHitData;

struct HelpItemInfo {
    long                            options;
    Point                           tip;
    Rect                            altRect;
    short                           theProc;
    short                           helpVariant;
    HMMessageRecord                 helpMessage;
};
typedef struct HelpItemInfo             HelpItemInfo;
#endif  /* OLDROUTINENAMES */

typedef CALLBACK_API( ComponentResult , PickerOpenProcPtr )(long storage, ComponentInstance self);
typedef CALLBACK_API( ComponentResult , PickerCloseProcPtr )(long storage, ComponentInstance self);
typedef CALLBACK_API( ComponentResult , PickerCanDoProcPtr )(long storage, short selector);
typedef CALLBACK_API( ComponentResult , PickerVersionProcPtr )(long storage);
typedef CALLBACK_API( ComponentResult , PickerRegisterProcPtr )(long storage);
typedef CALLBACK_API( ComponentResult , PickerSetTargetProcPtr )(long storage, ComponentInstance topOfCallChain);
typedef STACK_UPP_TYPE(PickerOpenProcPtr)                       PickerOpenUPP;
typedef STACK_UPP_TYPE(PickerCloseProcPtr)                      PickerCloseUPP;
typedef STACK_UPP_TYPE(PickerCanDoProcPtr)                      PickerCanDoUPP;
typedef STACK_UPP_TYPE(PickerVersionProcPtr)                    PickerVersionUPP;
typedef STACK_UPP_TYPE(PickerRegisterProcPtr)                   PickerRegisterUPP;
typedef STACK_UPP_TYPE(PickerSetTargetProcPtr)                  PickerSetTargetUPP;
EXTERN_API( ComponentResult )
PickerInit                      (long                   storage,
                                 PickerInitData *       data)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0000, 0x7000, 0xA82A);

typedef CALLBACK_API( ComponentResult , PickerInitProcPtr )(long storage, PickerInitData *data);
EXTERN_API( ComponentResult )
PickerTestGraphicsWorld         (long                   storage,
                                 PickerInitData *       data)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);

typedef CALLBACK_API( ComponentResult , PickerTestGraphicsWorldProcPtr )(long storage, PickerInitData *data);
EXTERN_API( ComponentResult )
PickerGetDialog                 (long                   storage)                            FIVEWORDINLINE(0x2F3C, 0x0000, 0x0002, 0x7000, 0xA82A);

typedef CALLBACK_API( ComponentResult , PickerGetDialogProcPtr )(long storage);
EXTERN_API( ComponentResult )
PickerGetItemList               (long                   storage)                            FIVEWORDINLINE(0x2F3C, 0x0000, 0x0003, 0x7000, 0xA82A);

typedef CALLBACK_API( ComponentResult , PickerGetItemListProcPtr )(long storage);
EXTERN_API( ComponentResult )
PickerGetColor                  (long                   storage,
                                 PickerColorType        whichColor,
                                 PMColorPtr             color)                              FIVEWORDINLINE(0x2F3C, 0x0006, 0x0004, 0x7000, 0xA82A);

typedef CALLBACK_API( ComponentResult , PickerGetColorProcPtr )(long storage, PickerColorType whichColor, PMColorPtr color);
typedef STACK_UPP_TYPE(PickerInitProcPtr)                       PickerInitUPP;
typedef STACK_UPP_TYPE(PickerTestGraphicsWorldProcPtr)          PickerTestGraphicsWorldUPP;
typedef STACK_UPP_TYPE(PickerGetDialogProcPtr)                  PickerGetDialogUPP;
typedef STACK_UPP_TYPE(PickerGetItemListProcPtr)                PickerGetItemListUPP;
typedef STACK_UPP_TYPE(PickerGetColorProcPtr)                   PickerGetColorUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(PickerOpenUPP)
    NewPickerOpenUPP               (PickerOpenProcPtr       userRoutine);

    EXTERN_API(PickerCloseUPP)
    NewPickerCloseUPP              (PickerCloseProcPtr      userRoutine);

    EXTERN_API(PickerCanDoUPP)
    NewPickerCanDoUPP              (PickerCanDoProcPtr      userRoutine);

    EXTERN_API(PickerVersionUPP)
    NewPickerVersionUPP            (PickerVersionProcPtr    userRoutine);

    EXTERN_API(PickerRegisterUPP)
    NewPickerRegisterUPP           (PickerRegisterProcPtr   userRoutine);

    EXTERN_API(PickerSetTargetUPP)
    NewPickerSetTargetUPP          (PickerSetTargetProcPtr  userRoutine);

    EXTERN_API(PickerInitUPP)
    NewPickerInitUPP               (PickerInitProcPtr       userRoutine);

    EXTERN_API(PickerTestGraphicsWorldUPP)
    NewPickerTestGraphicsWorldUPP    (PickerTestGraphicsWorldProcPtr userRoutine);

    EXTERN_API(PickerGetDialogUPP)
    NewPickerGetDialogUPP          (PickerGetDialogProcPtr  userRoutine);

    EXTERN_API(PickerGetItemListUPP)
    NewPickerGetItemListUPP        (PickerGetItemListProcPtr userRoutine);

    EXTERN_API(PickerGetColorUPP)
    NewPickerGetColorUPP           (PickerGetColorProcPtr   userRoutine);

    EXTERN_API(void)
    DisposePickerOpenUPP           (PickerOpenUPP           userUPP);

    EXTERN_API(void)
    DisposePickerCloseUPP          (PickerCloseUPP          userUPP);

    EXTERN_API(void)
    DisposePickerCanDoUPP          (PickerCanDoUPP          userUPP);

    EXTERN_API(void)
    DisposePickerVersionUPP        (PickerVersionUPP        userUPP);

    EXTERN_API(void)
    DisposePickerRegisterUPP       (PickerRegisterUPP       userUPP);

    EXTERN_API(void)
    DisposePickerSetTargetUPP      (PickerSetTargetUPP      userUPP);

    EXTERN_API(void)
    DisposePickerInitUPP           (PickerInitUPP           userUPP);

    EXTERN_API(void)
    DisposePickerTestGraphicsWorldUPP    (PickerTestGraphicsWorldUPP userUPP);

    EXTERN_API(void)
    DisposePickerGetDialogUPP      (PickerGetDialogUPP      userUPP);

    EXTERN_API(void)
    DisposePickerGetItemListUPP    (PickerGetItemListUPP    userUPP);

    EXTERN_API(void)
    DisposePickerGetColorUPP       (PickerGetColorUPP       userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerOpenUPP            (long                    storage,
                                    ComponentInstance       self,
                                    PickerOpenUPP           userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerCloseUPP           (long                    storage,
                                    ComponentInstance       self,
                                    PickerCloseUPP          userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerCanDoUPP           (long                    storage,
                                    short                   selector,
                                    PickerCanDoUPP          userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerVersionUPP         (long                    storage,
                                    PickerVersionUPP        userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerRegisterUPP        (long                    storage,
                                    PickerRegisterUPP       userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerSetTargetUPP       (long                    storage,
                                    ComponentInstance       topOfCallChain,
                                    PickerSetTargetUPP      userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerInitUPP            (long                    storage,
                                    PickerInitData *        data,
                                    PickerInitUPP           userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerTestGraphicsWorldUPP    (long               storage,
                                    PickerInitData *        data,
                                    PickerTestGraphicsWorldUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerGetDialogUPP       (long                    storage,
                                    PickerGetDialogUPP      userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerGetItemListUPP     (long                    storage,
                                    PickerGetItemListUPP    userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerGetColorUPP        (long                    storage,
                                    PickerColorType         whichColor,
                                    PMColorPtr              color,
                                    PickerGetColorUPP       userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppPickerOpenProcInfo = 0x000003F0 };                    /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerCloseProcInfo = 0x000003F0 };                   /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerCanDoProcInfo = 0x000002F0 };                   /* pascal 4_bytes Func(4_bytes, 2_bytes) */
    enum { uppPickerVersionProcInfo = 0x000000F0 };                 /* pascal 4_bytes Func(4_bytes) */
    enum { uppPickerRegisterProcInfo = 0x000000F0 };                /* pascal 4_bytes Func(4_bytes) */
    enum { uppPickerSetTargetProcInfo = 0x000003F0 };               /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerInitProcInfo = 0x000003F0 };                    /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerTestGraphicsWorldProcInfo = 0x000003F0 };       /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerGetDialogProcInfo = 0x000000F0 };               /* pascal 4_bytes Func(4_bytes) */
    enum { uppPickerGetItemListProcInfo = 0x000000F0 };             /* pascal 4_bytes Func(4_bytes) */
    enum { uppPickerGetColorProcInfo = 0x00000EF0 };                /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes) */
    #define NewPickerOpenUPP(userRoutine)                           (PickerOpenUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerOpenProcInfo, GetCurrentArchitecture())
    #define NewPickerCloseUPP(userRoutine)                          (PickerCloseUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerCloseProcInfo, GetCurrentArchitecture())
    #define NewPickerCanDoUPP(userRoutine)                          (PickerCanDoUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerCanDoProcInfo, GetCurrentArchitecture())
    #define NewPickerVersionUPP(userRoutine)                        (PickerVersionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerVersionProcInfo, GetCurrentArchitecture())
    #define NewPickerRegisterUPP(userRoutine)                       (PickerRegisterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerRegisterProcInfo, GetCurrentArchitecture())
    #define NewPickerSetTargetUPP(userRoutine)                      (PickerSetTargetUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerSetTargetProcInfo, GetCurrentArchitecture())
    #define NewPickerInitUPP(userRoutine)                           (PickerInitUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerInitProcInfo, GetCurrentArchitecture())
    #define NewPickerTestGraphicsWorldUPP(userRoutine)              (PickerTestGraphicsWorldUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerTestGraphicsWorldProcInfo, GetCurrentArchitecture())
    #define NewPickerGetDialogUPP(userRoutine)                      (PickerGetDialogUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerGetDialogProcInfo, GetCurrentArchitecture())
    #define NewPickerGetItemListUPP(userRoutine)                    (PickerGetItemListUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerGetItemListProcInfo, GetCurrentArchitecture())
    #define NewPickerGetColorUPP(userRoutine)                       (PickerGetColorUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerGetColorProcInfo, GetCurrentArchitecture())
    #define DisposePickerOpenUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposePickerCloseUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposePickerCanDoUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposePickerVersionUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposePickerRegisterUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposePickerSetTargetUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposePickerInitUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposePickerTestGraphicsWorldUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define DisposePickerGetDialogUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposePickerGetItemListUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposePickerGetColorUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define InvokePickerOpenUPP(storage, self, userUPP)             (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerOpenProcInfo, (storage), (self))
    #define InvokePickerCloseUPP(storage, self, userUPP)            (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerCloseProcInfo, (storage), (self))
    #define InvokePickerCanDoUPP(storage, selector, userUPP)        (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerCanDoProcInfo, (storage), (selector))
    #define InvokePickerVersionUPP(storage, userUPP)                (ComponentResult)CALL_ONE_PARAMETER_UPP((userUPP), uppPickerVersionProcInfo, (storage))
    #define InvokePickerRegisterUPP(storage, userUPP)               (ComponentResult)CALL_ONE_PARAMETER_UPP((userUPP), uppPickerRegisterProcInfo, (storage))
    #define InvokePickerSetTargetUPP(storage, topOfCallChain, userUPP)  (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerSetTargetProcInfo, (storage), (topOfCallChain))
    #define InvokePickerInitUPP(storage, data, userUPP)             (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerInitProcInfo, (storage), (data))
    #define InvokePickerTestGraphicsWorldUPP(storage, data, userUPP)  (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerTestGraphicsWorldProcInfo, (storage), (data))
    #define InvokePickerGetDialogUPP(storage, userUPP)              (ComponentResult)CALL_ONE_PARAMETER_UPP((userUPP), uppPickerGetDialogProcInfo, (storage))
    #define InvokePickerGetItemListUPP(storage, userUPP)            (ComponentResult)CALL_ONE_PARAMETER_UPP((userUPP), uppPickerGetItemListProcInfo, (storage))
    #define InvokePickerGetColorUPP(storage, whichColor, color, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppPickerGetColorProcInfo, (storage), (whichColor), (color))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewPickerOpenProc(userRoutine)                          NewPickerOpenUPP(userRoutine)
#define NewPickerCloseProc(userRoutine)                         NewPickerCloseUPP(userRoutine)
#define NewPickerCanDoProc(userRoutine)                         NewPickerCanDoUPP(userRoutine)
#define NewPickerVersionProc(userRoutine)                       NewPickerVersionUPP(userRoutine)
#define NewPickerRegisterProc(userRoutine)                      NewPickerRegisterUPP(userRoutine)
#define NewPickerSetTargetProc(userRoutine)                     NewPickerSetTargetUPP(userRoutine)
#define NewPickerInitProc(userRoutine)                          NewPickerInitUPP(userRoutine)
#define NewPickerTestGraphicsWorldProc(userRoutine)             NewPickerTestGraphicsWorldUPP(userRoutine)
#define NewPickerGetDialogProc(userRoutine)                     NewPickerGetDialogUPP(userRoutine)
#define NewPickerGetItemListProc(userRoutine)                   NewPickerGetItemListUPP(userRoutine)
#define NewPickerGetColorProc(userRoutine)                      NewPickerGetColorUPP(userRoutine)
#define CallPickerOpenProc(userRoutine, storage, self)          InvokePickerOpenUPP(storage, self, userRoutine)
#define CallPickerCloseProc(userRoutine, storage, self)         InvokePickerCloseUPP(storage, self, userRoutine)
#define CallPickerCanDoProc(userRoutine, storage, selector)     InvokePickerCanDoUPP(storage, selector, userRoutine)
#define CallPickerVersionProc(userRoutine, storage)             InvokePickerVersionUPP(storage, userRoutine)
#define CallPickerRegisterProc(userRoutine, storage)            InvokePickerRegisterUPP(storage, userRoutine)
#define CallPickerSetTargetProc(userRoutine, storage, topOfCallChain) InvokePickerSetTargetUPP(storage, topOfCallChain, userRoutine)
#define CallPickerInitProc(userRoutine, storage, data)          InvokePickerInitUPP(storage, data, userRoutine)
#define CallPickerTestGraphicsWorldProc(userRoutine, storage, data) InvokePickerTestGraphicsWorldUPP(storage, data, userRoutine)
#define CallPickerGetDialogProc(userRoutine, storage)           InvokePickerGetDialogUPP(storage, userRoutine)
#define CallPickerGetItemListProc(userRoutine, storage)         InvokePickerGetItemListUPP(storage, userRoutine)
#define CallPickerGetColorProc(userRoutine, storage, whichColor, color) InvokePickerGetColorUPP(storage, whichColor, color, userRoutine)
#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerSetColor                  (long                   storage,
                                 PickerColorType        whichColor,
                                 PMColorPtr             color)                              FIVEWORDINLINE(0x2F3C, 0x0006, 0x0005, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerSetColorProcPtr )(long storage, PickerColorType whichColor, PMColorPtr color);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerEvent                     (long                   storage,
                                 PickerEventData *      data)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0006, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerEventProcPtr )(long storage, PickerEventData *data);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerEdit                      (long                   storage,
                                 PickerEditData *       data)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerEditProcPtr )(long storage, PickerEditData *data);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerSetVisibility             (long                   storage,
                                 Boolean                visible)                            FIVEWORDINLINE(0x2F3C, 0x0002, 0x0008, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerSetVisibilityProcPtr )(long storage, Boolean visible);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerDisplay                   (long                   storage)                            FIVEWORDINLINE(0x2F3C, 0x0000, 0x0009, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerDisplayProcPtr )(long storage);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerItemHit                   (long                   storage,
                                 PickerItemHitData *    data)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x000A, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerItemHitProcPtr )(long storage, PickerItemHitData *data);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerSetBaseItem               (long                   storage,
                                 short                  baseItem)                           FIVEWORDINLINE(0x2F3C, 0x0002, 0x000B, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerSetBaseItemProcPtr )(long storage, short baseItem);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerGetProfile                (long                   storage)                            FIVEWORDINLINE(0x2F3C, 0x0000, 0x000C, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerGetProfileProcPtr )(long storage);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerSetProfile                (long                   storage,
                                 CMProfileHandle        profile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x000D, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerSetProfileProcPtr )(long storage, CMProfileHandle profile);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerGetPrompt                 (long                   storage,
                                 Str255                 prompt)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x000E, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerGetPromptProcPtr )(long storage, Str255 prompt);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerSetPrompt                 (long                   storage,
                                 ConstStr255Param       prompt)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x000F, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerSetPromptProcPtr )(long storage, ConstStr255Param prompt);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerGetIconData               (long                   storage,
                                 PickerIconData *       data)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0010, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerGetIconDataProcPtr )(long storage, PickerIconData *data);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerGetEditMenuState          (long                   storage,
                                 PickerMenuState *      mState)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0011, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerGetEditMenuStateProcPtr )(long storage, PickerMenuState *mState);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerSetOrigin                 (long                   storage,
                                 Point                  where)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerSetOriginProcPtr )(long storage, Point where);
/*  Below are the ColorPicker 2.1 routines.*/


#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerSetColorChangedProc       (long                   storage,
                                 ColorChangedUPP        colorProc,
                                 long                   colorProcData)                      FIVEWORDINLINE(0x2F3C, 0x0008, 0x0014, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerSetColorChangedProcProcPtr )(long storage, NColorChangedUPP colorProc, long colorProcData);
/* New Color Picker 2.1 messages.  If you don't wish to support these you should already be... */
/* returning a badComponentSelector in your main entry routine.  They have new selectors*/
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
NPickerGetColor                 (long                   storage,
                                 PickerColorType        whichColor,
                                 NPMColor *             color)                              FIVEWORDINLINE(0x2F3C, 0x0006, 0x0015, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , NPickerGetColorProcPtr )(long storage, PickerColorType whichColor, NPMColor *color);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
NPickerSetColor                 (long                   storage,
                                 PickerColorType        whichColor,
                                 NPMColor *             color)                              FIVEWORDINLINE(0x2F3C, 0x0006, 0x0016, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , NPickerSetColorProcPtr )(long storage, PickerColorType whichColor, NPMColor *color);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
NPickerGetProfile               (long                   storage,
                                 CMProfileRef *         profile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0017, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , NPickerGetProfileProcPtr )(long storage, CMProfileRef *profile);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
NPickerSetProfile               (long                   storage,
                                 CMProfileRef           profile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0018, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , NPickerSetProfileProcPtr )(long storage, CMProfileRef profile);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
NPickerSetColorChangedProc      (long                   storage,
                                 NColorChangedUPP       colorProc,
                                 long                   colorProcData)                      FIVEWORDINLINE(0x2F3C, 0x0008, 0x0019, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , NPickerSetColorChangedProcProcPtr )(long storage, NColorChangedUPP colorProc, long colorProcData);
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
PickerExtractHelpItem           (long                   storage,
                                 short                  itemNo,
                                 short                  whichMsg,
                                 PickerHelpItemInfo *   helpInfo)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0013, 0x7000, 0xA82A);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( ComponentResult , PickerExtractHelpItemProcPtr )(long storage, short itemNo, short whichMsg, PickerHelpItemInfo *helpInfo);
typedef STACK_UPP_TYPE(PickerSetColorProcPtr)                   PickerSetColorUPP;
typedef STACK_UPP_TYPE(PickerEventProcPtr)                      PickerEventUPP;
typedef STACK_UPP_TYPE(PickerEditProcPtr)                       PickerEditUPP;
typedef STACK_UPP_TYPE(PickerSetVisibilityProcPtr)              PickerSetVisibilityUPP;
typedef STACK_UPP_TYPE(PickerDisplayProcPtr)                    PickerDisplayUPP;
typedef STACK_UPP_TYPE(PickerItemHitProcPtr)                    PickerItemHitUPP;
typedef STACK_UPP_TYPE(PickerSetBaseItemProcPtr)                PickerSetBaseItemUPP;
typedef STACK_UPP_TYPE(PickerGetProfileProcPtr)                 PickerGetProfileUPP;
typedef STACK_UPP_TYPE(PickerSetProfileProcPtr)                 PickerSetProfileUPP;
typedef STACK_UPP_TYPE(PickerGetPromptProcPtr)                  PickerGetPromptUPP;
typedef STACK_UPP_TYPE(PickerSetPromptProcPtr)                  PickerSetPromptUPP;
typedef STACK_UPP_TYPE(PickerGetIconDataProcPtr)                PickerGetIconDataUPP;
typedef STACK_UPP_TYPE(PickerGetEditMenuStateProcPtr)           PickerGetEditMenuStateUPP;
typedef STACK_UPP_TYPE(PickerSetOriginProcPtr)                  PickerSetOriginUPP;
typedef STACK_UPP_TYPE(PickerSetColorChangedProcProcPtr)        PickerSetColorChangedProcUPP;
typedef STACK_UPP_TYPE(NPickerGetColorProcPtr)                  NPickerGetColorUPP;
typedef STACK_UPP_TYPE(NPickerSetColorProcPtr)                  NPickerSetColorUPP;
typedef STACK_UPP_TYPE(NPickerGetProfileProcPtr)                NPickerGetProfileUPP;
typedef STACK_UPP_TYPE(NPickerSetProfileProcPtr)                NPickerSetProfileUPP;
typedef STACK_UPP_TYPE(NPickerSetColorChangedProcProcPtr)       NPickerSetColorChangedProcUPP;
typedef STACK_UPP_TYPE(PickerExtractHelpItemProcPtr)            PickerExtractHelpItemUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(PickerSetColorUPP)
    NewPickerSetColorUPP           (PickerSetColorProcPtr   userRoutine);

    EXTERN_API(PickerEventUPP)
    NewPickerEventUPP              (PickerEventProcPtr      userRoutine);

    EXTERN_API(PickerEditUPP)
    NewPickerEditUPP               (PickerEditProcPtr       userRoutine);

    EXTERN_API(PickerSetVisibilityUPP)
    NewPickerSetVisibilityUPP      (PickerSetVisibilityProcPtr userRoutine);

    EXTERN_API(PickerDisplayUPP)
    NewPickerDisplayUPP            (PickerDisplayProcPtr    userRoutine);

    EXTERN_API(PickerItemHitUPP)
    NewPickerItemHitUPP            (PickerItemHitProcPtr    userRoutine);

    EXTERN_API(PickerSetBaseItemUPP)
    NewPickerSetBaseItemUPP        (PickerSetBaseItemProcPtr userRoutine);

    EXTERN_API(PickerGetProfileUPP)
    NewPickerGetProfileUPP         (PickerGetProfileProcPtr userRoutine);

    EXTERN_API(PickerSetProfileUPP)
    NewPickerSetProfileUPP         (PickerSetProfileProcPtr userRoutine);

    EXTERN_API(PickerGetPromptUPP)
    NewPickerGetPromptUPP          (PickerGetPromptProcPtr  userRoutine);

    EXTERN_API(PickerSetPromptUPP)
    NewPickerSetPromptUPP          (PickerSetPromptProcPtr  userRoutine);

    EXTERN_API(PickerGetIconDataUPP)
    NewPickerGetIconDataUPP        (PickerGetIconDataProcPtr userRoutine);

    EXTERN_API(PickerGetEditMenuStateUPP)
    NewPickerGetEditMenuStateUPP    (PickerGetEditMenuStateProcPtr userRoutine);

    EXTERN_API(PickerSetOriginUPP)
    NewPickerSetOriginUPP          (PickerSetOriginProcPtr  userRoutine);

    EXTERN_API(PickerSetColorChangedProcUPP)
    NewPickerSetColorChangedProcUPP    (PickerSetColorChangedProcProcPtr userRoutine);

    EXTERN_API(NPickerGetColorUPP)
    NewNPickerGetColorUPP          (NPickerGetColorProcPtr  userRoutine);

    EXTERN_API(NPickerSetColorUPP)
    NewNPickerSetColorUPP          (NPickerSetColorProcPtr  userRoutine);

    EXTERN_API(NPickerGetProfileUPP)
    NewNPickerGetProfileUPP        (NPickerGetProfileProcPtr userRoutine);

    EXTERN_API(NPickerSetProfileUPP)
    NewNPickerSetProfileUPP        (NPickerSetProfileProcPtr userRoutine);

    EXTERN_API(NPickerSetColorChangedProcUPP)
    NewNPickerSetColorChangedProcUPP    (NPickerSetColorChangedProcProcPtr userRoutine);

    EXTERN_API(PickerExtractHelpItemUPP)
    NewPickerExtractHelpItemUPP    (PickerExtractHelpItemProcPtr userRoutine);

    EXTERN_API(void)
    DisposePickerSetColorUPP       (PickerSetColorUPP       userUPP);

    EXTERN_API(void)
    DisposePickerEventUPP          (PickerEventUPP          userUPP);

    EXTERN_API(void)
    DisposePickerEditUPP           (PickerEditUPP           userUPP);

    EXTERN_API(void)
    DisposePickerSetVisibilityUPP    (PickerSetVisibilityUPP userUPP);

    EXTERN_API(void)
    DisposePickerDisplayUPP        (PickerDisplayUPP        userUPP);

    EXTERN_API(void)
    DisposePickerItemHitUPP        (PickerItemHitUPP        userUPP);

    EXTERN_API(void)
    DisposePickerSetBaseItemUPP    (PickerSetBaseItemUPP    userUPP);

    EXTERN_API(void)
    DisposePickerGetProfileUPP     (PickerGetProfileUPP     userUPP);

    EXTERN_API(void)
    DisposePickerSetProfileUPP     (PickerSetProfileUPP     userUPP);

    EXTERN_API(void)
    DisposePickerGetPromptUPP      (PickerGetPromptUPP      userUPP);

    EXTERN_API(void)
    DisposePickerSetPromptUPP      (PickerSetPromptUPP      userUPP);

    EXTERN_API(void)
    DisposePickerGetIconDataUPP    (PickerGetIconDataUPP    userUPP);

    EXTERN_API(void)
    DisposePickerGetEditMenuStateUPP    (PickerGetEditMenuStateUPP userUPP);

    EXTERN_API(void)
    DisposePickerSetOriginUPP      (PickerSetOriginUPP      userUPP);

    EXTERN_API(void)
    DisposePickerSetColorChangedProcUPP    (PickerSetColorChangedProcUPP userUPP);

    EXTERN_API(void)
    DisposeNPickerGetColorUPP      (NPickerGetColorUPP      userUPP);

    EXTERN_API(void)
    DisposeNPickerSetColorUPP      (NPickerSetColorUPP      userUPP);

    EXTERN_API(void)
    DisposeNPickerGetProfileUPP    (NPickerGetProfileUPP    userUPP);

    EXTERN_API(void)
    DisposeNPickerSetProfileUPP    (NPickerSetProfileUPP    userUPP);

    EXTERN_API(void)
    DisposeNPickerSetColorChangedProcUPP    (NPickerSetColorChangedProcUPP userUPP);

    EXTERN_API(void)
    DisposePickerExtractHelpItemUPP    (PickerExtractHelpItemUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerSetColorUPP        (long                    storage,
                                    PickerColorType         whichColor,
                                    PMColorPtr              color,
                                    PickerSetColorUPP       userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerEventUPP           (long                    storage,
                                    PickerEventData *       data,
                                    PickerEventUPP          userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerEditUPP            (long                    storage,
                                    PickerEditData *        data,
                                    PickerEditUPP           userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerSetVisibilityUPP    (long                   storage,
                                    Boolean                 visible,
                                    PickerSetVisibilityUPP  userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerDisplayUPP         (long                    storage,
                                    PickerDisplayUPP        userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerItemHitUPP         (long                    storage,
                                    PickerItemHitData *     data,
                                    PickerItemHitUPP        userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerSetBaseItemUPP     (long                    storage,
                                    short                   baseItem,
                                    PickerSetBaseItemUPP    userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerGetProfileUPP      (long                    storage,
                                    PickerGetProfileUPP     userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerSetProfileUPP      (long                    storage,
                                    CMProfileHandle         profile,
                                    PickerSetProfileUPP     userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerGetPromptUPP       (long                    storage,
                                    Str255                  prompt,
                                    PickerGetPromptUPP      userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerSetPromptUPP       (long                    storage,
                                    ConstStr255Param        prompt,
                                    PickerSetPromptUPP      userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerGetIconDataUPP     (long                    storage,
                                    PickerIconData *        data,
                                    PickerGetIconDataUPP    userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerGetEditMenuStateUPP    (long                storage,
                                    PickerMenuState *       mState,
                                    PickerGetEditMenuStateUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerSetOriginUPP       (long                    storage,
                                    Point                   where,
                                    PickerSetOriginUPP      userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerSetColorChangedProcUPP    (long             storage,
                                    NColorChangedUPP        colorProc,
                                    long                    colorProcData,
                                    PickerSetColorChangedProcUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokeNPickerGetColorUPP       (long                    storage,
                                    PickerColorType         whichColor,
                                    NPMColor *              color,
                                    NPickerGetColorUPP      userUPP);

    EXTERN_API(ComponentResult)
    InvokeNPickerSetColorUPP       (long                    storage,
                                    PickerColorType         whichColor,
                                    NPMColor *              color,
                                    NPickerSetColorUPP      userUPP);

    EXTERN_API(ComponentResult)
    InvokeNPickerGetProfileUPP     (long                    storage,
                                    CMProfileRef *          profile,
                                    NPickerGetProfileUPP    userUPP);

    EXTERN_API(ComponentResult)
    InvokeNPickerSetProfileUPP     (long                    storage,
                                    CMProfileRef            profile,
                                    NPickerSetProfileUPP    userUPP);

    EXTERN_API(ComponentResult)
    InvokeNPickerSetColorChangedProcUPP    (long            storage,
                                    NColorChangedUPP        colorProc,
                                    long                    colorProcData,
                                    NPickerSetColorChangedProcUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokePickerExtractHelpItemUPP    (long                 storage,
                                    short                   itemNo,
                                    short                   whichMsg,
                                    PickerHelpItemInfo *    helpInfo,
                                    PickerExtractHelpItemUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppPickerSetColorProcInfo = 0x00000EF0 };                /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes) */
    enum { uppPickerEventProcInfo = 0x000003F0 };                   /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerEditProcInfo = 0x000003F0 };                    /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerSetVisibilityProcInfo = 0x000001F0 };           /* pascal 4_bytes Func(4_bytes, 1_byte) */
    enum { uppPickerDisplayProcInfo = 0x000000F0 };                 /* pascal 4_bytes Func(4_bytes) */
    enum { uppPickerItemHitProcInfo = 0x000003F0 };                 /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerSetBaseItemProcInfo = 0x000002F0 };             /* pascal 4_bytes Func(4_bytes, 2_bytes) */
    enum { uppPickerGetProfileProcInfo = 0x000000F0 };              /* pascal 4_bytes Func(4_bytes) */
    enum { uppPickerSetProfileProcInfo = 0x000003F0 };              /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerGetPromptProcInfo = 0x000003F0 };               /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerSetPromptProcInfo = 0x000003F0 };               /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerGetIconDataProcInfo = 0x000003F0 };             /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerGetEditMenuStateProcInfo = 0x000003F0 };        /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerSetOriginProcInfo = 0x000003F0 };               /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPickerSetColorChangedProcProcInfo = 0x00000FF0 };     /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppNPickerGetColorProcInfo = 0x00000EF0 };               /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes) */
    enum { uppNPickerSetColorProcInfo = 0x00000EF0 };               /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes) */
    enum { uppNPickerGetProfileProcInfo = 0x000003F0 };             /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppNPickerSetProfileProcInfo = 0x000003F0 };             /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppNPickerSetColorChangedProcProcInfo = 0x00000FF0 };    /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppPickerExtractHelpItemProcInfo = 0x00003AF0 };         /* pascal 4_bytes Func(4_bytes, 2_bytes, 2_bytes, 4_bytes) */
    #define NewPickerSetColorUPP(userRoutine)                       (PickerSetColorUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerSetColorProcInfo, GetCurrentArchitecture())
    #define NewPickerEventUPP(userRoutine)                          (PickerEventUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerEventProcInfo, GetCurrentArchitecture())
    #define NewPickerEditUPP(userRoutine)                           (PickerEditUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerEditProcInfo, GetCurrentArchitecture())
    #define NewPickerSetVisibilityUPP(userRoutine)                  (PickerSetVisibilityUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerSetVisibilityProcInfo, GetCurrentArchitecture())
    #define NewPickerDisplayUPP(userRoutine)                        (PickerDisplayUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerDisplayProcInfo, GetCurrentArchitecture())
    #define NewPickerItemHitUPP(userRoutine)                        (PickerItemHitUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerItemHitProcInfo, GetCurrentArchitecture())
    #define NewPickerSetBaseItemUPP(userRoutine)                    (PickerSetBaseItemUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerSetBaseItemProcInfo, GetCurrentArchitecture())
    #define NewPickerGetProfileUPP(userRoutine)                     (PickerGetProfileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerGetProfileProcInfo, GetCurrentArchitecture())
    #define NewPickerSetProfileUPP(userRoutine)                     (PickerSetProfileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerSetProfileProcInfo, GetCurrentArchitecture())
    #define NewPickerGetPromptUPP(userRoutine)                      (PickerGetPromptUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerGetPromptProcInfo, GetCurrentArchitecture())
    #define NewPickerSetPromptUPP(userRoutine)                      (PickerSetPromptUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerSetPromptProcInfo, GetCurrentArchitecture())
    #define NewPickerGetIconDataUPP(userRoutine)                    (PickerGetIconDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerGetIconDataProcInfo, GetCurrentArchitecture())
    #define NewPickerGetEditMenuStateUPP(userRoutine)               (PickerGetEditMenuStateUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerGetEditMenuStateProcInfo, GetCurrentArchitecture())
    #define NewPickerSetOriginUPP(userRoutine)                      (PickerSetOriginUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerSetOriginProcInfo, GetCurrentArchitecture())
    #define NewPickerSetColorChangedProcUPP(userRoutine)            (PickerSetColorChangedProcUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerSetColorChangedProcProcInfo, GetCurrentArchitecture())
    #define NewNPickerGetColorUPP(userRoutine)                      (NPickerGetColorUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppNPickerGetColorProcInfo, GetCurrentArchitecture())
    #define NewNPickerSetColorUPP(userRoutine)                      (NPickerSetColorUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppNPickerSetColorProcInfo, GetCurrentArchitecture())
    #define NewNPickerGetProfileUPP(userRoutine)                    (NPickerGetProfileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppNPickerGetProfileProcInfo, GetCurrentArchitecture())
    #define NewNPickerSetProfileUPP(userRoutine)                    (NPickerSetProfileUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppNPickerSetProfileProcInfo, GetCurrentArchitecture())
    #define NewNPickerSetColorChangedProcUPP(userRoutine)           (NPickerSetColorChangedProcUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppNPickerSetColorChangedProcProcInfo, GetCurrentArchitecture())
    #define NewPickerExtractHelpItemUPP(userRoutine)                (PickerExtractHelpItemUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPickerExtractHelpItemProcInfo, GetCurrentArchitecture())
    #define DisposePickerSetColorUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposePickerEventUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposePickerEditUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposePickerSetVisibilityUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define DisposePickerDisplayUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposePickerItemHitUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposePickerSetBaseItemUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposePickerGetProfileUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposePickerSetProfileUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposePickerGetPromptUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposePickerSetPromptUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposePickerGetIconDataUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposePickerGetEditMenuStateUPP(userUPP)               DisposeRoutineDescriptor(userUPP)
    #define DisposePickerSetOriginUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposePickerSetColorChangedProcUPP(userUPP)            DisposeRoutineDescriptor(userUPP)
    #define DisposeNPickerGetColorUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeNPickerSetColorUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeNPickerGetProfileUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeNPickerSetProfileUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeNPickerSetColorChangedProcUPP(userUPP)           DisposeRoutineDescriptor(userUPP)
    #define DisposePickerExtractHelpItemUPP(userUPP)                DisposeRoutineDescriptor(userUPP)
    #define InvokePickerSetColorUPP(storage, whichColor, color, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppPickerSetColorProcInfo, (storage), (whichColor), (color))
    #define InvokePickerEventUPP(storage, data, userUPP)            (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerEventProcInfo, (storage), (data))
    #define InvokePickerEditUPP(storage, data, userUPP)             (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerEditProcInfo, (storage), (data))
    #define InvokePickerSetVisibilityUPP(storage, visible, userUPP)  (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerSetVisibilityProcInfo, (storage), (visible))
    #define InvokePickerDisplayUPP(storage, userUPP)                (ComponentResult)CALL_ONE_PARAMETER_UPP((userUPP), uppPickerDisplayProcInfo, (storage))
    #define InvokePickerItemHitUPP(storage, data, userUPP)          (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerItemHitProcInfo, (storage), (data))
    #define InvokePickerSetBaseItemUPP(storage, baseItem, userUPP)  (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerSetBaseItemProcInfo, (storage), (baseItem))
    #define InvokePickerGetProfileUPP(storage, userUPP)             (ComponentResult)CALL_ONE_PARAMETER_UPP((userUPP), uppPickerGetProfileProcInfo, (storage))
    #define InvokePickerSetProfileUPP(storage, profile, userUPP)    (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerSetProfileProcInfo, (storage), (profile))
    #define InvokePickerGetPromptUPP(storage, prompt, userUPP)      (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerGetPromptProcInfo, (storage), (prompt))
    #define InvokePickerSetPromptUPP(storage, prompt, userUPP)      (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerSetPromptProcInfo, (storage), (prompt))
    #define InvokePickerGetIconDataUPP(storage, data, userUPP)      (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerGetIconDataProcInfo, (storage), (data))
    #define InvokePickerGetEditMenuStateUPP(storage, mState, userUPP)  (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerGetEditMenuStateProcInfo, (storage), (mState))
    #define InvokePickerSetOriginUPP(storage, where, userUPP)       (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppPickerSetOriginProcInfo, (storage), (where))
    #define InvokePickerSetColorChangedProcUPP(storage, colorProc, colorProcData, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppPickerSetColorChangedProcProcInfo, (storage), (colorProc), (colorProcData))
    #define InvokeNPickerGetColorUPP(storage, whichColor, color, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppNPickerGetColorProcInfo, (storage), (whichColor), (color))
    #define InvokeNPickerSetColorUPP(storage, whichColor, color, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppNPickerSetColorProcInfo, (storage), (whichColor), (color))
    #define InvokeNPickerGetProfileUPP(storage, profile, userUPP)   (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppNPickerGetProfileProcInfo, (storage), (profile))
    #define InvokeNPickerSetProfileUPP(storage, profile, userUPP)   (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppNPickerSetProfileProcInfo, (storage), (profile))
    #define InvokeNPickerSetColorChangedProcUPP(storage, colorProc, colorProcData, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppNPickerSetColorChangedProcProcInfo, (storage), (colorProc), (colorProcData))
    #define InvokePickerExtractHelpItemUPP(storage, itemNo, whichMsg, helpInfo, userUPP)  (ComponentResult)CALL_FOUR_PARAMETER_UPP((userUPP), uppPickerExtractHelpItemProcInfo, (storage), (itemNo), (whichMsg), (helpInfo))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewPickerSetColorProc(userRoutine)                      NewPickerSetColorUPP(userRoutine)
#define NewPickerEventProc(userRoutine)                         NewPickerEventUPP(userRoutine)
#define NewPickerEditProc(userRoutine)                          NewPickerEditUPP(userRoutine)
#define NewPickerSetVisibilityProc(userRoutine)                 NewPickerSetVisibilityUPP(userRoutine)
#define NewPickerDisplayProc(userRoutine)                       NewPickerDisplayUPP(userRoutine)
#define NewPickerItemHitProc(userRoutine)                       NewPickerItemHitUPP(userRoutine)
#define NewPickerSetBaseItemProc(userRoutine)                   NewPickerSetBaseItemUPP(userRoutine)
#define NewPickerGetProfileProc(userRoutine)                    NewPickerGetProfileUPP(userRoutine)
#define NewPickerSetProfileProc(userRoutine)                    NewPickerSetProfileUPP(userRoutine)
#define NewPickerGetPromptProc(userRoutine)                     NewPickerGetPromptUPP(userRoutine)
#define NewPickerSetPromptProc(userRoutine)                     NewPickerSetPromptUPP(userRoutine)
#define NewPickerGetIconDataProc(userRoutine)                   NewPickerGetIconDataUPP(userRoutine)
#define NewPickerGetEditMenuStateProc(userRoutine)              NewPickerGetEditMenuStateUPP(userRoutine)
#define NewPickerSetOriginProc(userRoutine)                     NewPickerSetOriginUPP(userRoutine)
#define NewPickerSetColorChangedProcProc(userRoutine)           NewPickerSetColorChangedProcUPP(userRoutine)
#define NewNPickerGetColorProc(userRoutine)                     NewNPickerGetColorUPP(userRoutine)
#define NewNPickerSetColorProc(userRoutine)                     NewNPickerSetColorUPP(userRoutine)
#define NewNPickerGetProfileProc(userRoutine)                   NewNPickerGetProfileUPP(userRoutine)
#define NewNPickerSetProfileProc(userRoutine)                   NewNPickerSetProfileUPP(userRoutine)
#define NewNPickerSetColorChangedProcProc(userRoutine)          NewNPickerSetColorChangedProcUPP(userRoutine)
#define NewPickerExtractHelpItemProc(userRoutine)               NewPickerExtractHelpItemUPP(userRoutine)
#define CallPickerSetColorProc(userRoutine, storage, whichColor, color) InvokePickerSetColorUPP(storage, whichColor, color, userRoutine)
#define CallPickerEventProc(userRoutine, storage, data)         InvokePickerEventUPP(storage, data, userRoutine)
#define CallPickerEditProc(userRoutine, storage, data)          InvokePickerEditUPP(storage, data, userRoutine)
#define CallPickerSetVisibilityProc(userRoutine, storage, visible) InvokePickerSetVisibilityUPP(storage, visible, userRoutine)
#define CallPickerDisplayProc(userRoutine, storage)             InvokePickerDisplayUPP(storage, userRoutine)
#define CallPickerItemHitProc(userRoutine, storage, data)       InvokePickerItemHitUPP(storage, data, userRoutine)
#define CallPickerSetBaseItemProc(userRoutine, storage, baseItem) InvokePickerSetBaseItemUPP(storage, baseItem, userRoutine)
#define CallPickerGetProfileProc(userRoutine, storage)          InvokePickerGetProfileUPP(storage, userRoutine)
#define CallPickerSetProfileProc(userRoutine, storage, profile) InvokePickerSetProfileUPP(storage, profile, userRoutine)
#define CallPickerGetPromptProc(userRoutine, storage, prompt)   InvokePickerGetPromptUPP(storage, prompt, userRoutine)
#define CallPickerSetPromptProc(userRoutine, storage, prompt)   InvokePickerSetPromptUPP(storage, prompt, userRoutine)
#define CallPickerGetIconDataProc(userRoutine, storage, data)   InvokePickerGetIconDataUPP(storage, data, userRoutine)
#define CallPickerGetEditMenuStateProc(userRoutine, storage, mState) InvokePickerGetEditMenuStateUPP(storage, mState, userRoutine)
#define CallPickerSetOriginProc(userRoutine, storage, where)    InvokePickerSetOriginUPP(storage, where, userRoutine)
#define CallPickerSetColorChangedProcProc(userRoutine, storage, colorProc, colorProcData) InvokePickerSetColorChangedProcUPP(storage, colorProc, colorProcData, userRoutine)
#define CallNPickerGetColorProc(userRoutine, storage, whichColor, color) InvokeNPickerGetColorUPP(storage, whichColor, color, userRoutine)
#define CallNPickerSetColorProc(userRoutine, storage, whichColor, color) InvokeNPickerSetColorUPP(storage, whichColor, color, userRoutine)
#define CallNPickerGetProfileProc(userRoutine, storage, profile) InvokeNPickerGetProfileUPP(storage, profile, userRoutine)
#define CallNPickerSetProfileProc(userRoutine, storage, profile) InvokeNPickerSetProfileUPP(storage, profile, userRoutine)
#define CallNPickerSetColorChangedProcProc(userRoutine, storage, colorProc, colorProcData) InvokeNPickerSetColorChangedProcUPP(storage, colorProc, colorProcData, userRoutine)
#define CallPickerExtractHelpItemProc(userRoutine, storage, itemNo, whichMsg, helpInfo) InvokePickerExtractHelpItemUPP(storage, itemNo, whichMsg, helpInfo, userRoutine)
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

#endif /* __COLORPICKERCOMPONENTS__ */

