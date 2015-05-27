/*
     File:       Icons.h
 
     Contains:   Icon Utilities and Icon Services Interfaces.
 
     Version:    Technology: Mac OS 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc. All rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ICONS__
#define __ICONS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __CODEFRAGMENTS__
#include <CodeFragments.h>
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

/* The following are icons for which there are both icon suites and SICNs. */
/* Avoid using icon resources if possible. Use IconServices instead. */
enum {
    kGenericDocumentIconResource = -4000,
    kGenericStationeryIconResource = -3985,
    kGenericEditionFileIconResource = -3989,
    kGenericApplicationIconResource = -3996,
    kGenericDeskAccessoryIconResource = -3991,
    kGenericFolderIconResource  = -3999,
    kPrivateFolderIconResource  = -3994,
    kFloppyIconResource         = -3998,
    kTrashIconResource          = -3993,
    kGenericRAMDiskIconResource = -3988,
    kGenericCDROMIconResource   = -3987
};

/* The following are icons for which there are SICNs only. */
/* Avoid using icon resources if possible. Use IconServices instead. */
enum {
    kDesktopIconResource        = -3992,
    kOpenFolderIconResource     = -3997,
    kGenericHardDiskIconResource = -3995,
    kGenericFileServerIconResource = -3972,
    kGenericSuitcaseIconResource = -3970,
    kGenericMoverObjectIconResource = -3969
};

/* The following are icons for which there are icon suites only. */
/* Avoid using icon resources if possible. Use IconServices instead. */
enum {
    kGenericPreferencesIconResource = -3971,
    kGenericQueryDocumentIconResource = -16506,
    kGenericExtensionIconResource = -16415,
    kSystemFolderIconResource   = -3983,
    kHelpIconResource           = -20271,
    kAppleMenuFolderIconResource = -3982
};

/* Obsolete. Use named constants defined above. */
enum {
    genericDocumentIconResource = kGenericDocumentIconResource,
    genericStationeryIconResource = kGenericStationeryIconResource,
    genericEditionFileIconResource = kGenericEditionFileIconResource,
    genericApplicationIconResource = kGenericApplicationIconResource,
    genericDeskAccessoryIconResource = kGenericDeskAccessoryIconResource,
    genericFolderIconResource   = kGenericFolderIconResource,
    privateFolderIconResource   = kPrivateFolderIconResource,
    floppyIconResource          = kFloppyIconResource,
    trashIconResource           = kTrashIconResource,
    genericRAMDiskIconResource  = kGenericRAMDiskIconResource,
    genericCDROMIconResource    = kGenericCDROMIconResource,
    desktopIconResource         = kDesktopIconResource,
    openFolderIconResource      = kOpenFolderIconResource,
    genericHardDiskIconResource = kGenericHardDiskIconResource,
    genericFileServerIconResource = kGenericFileServerIconResource,
    genericSuitcaseIconResource = kGenericSuitcaseIconResource,
    genericMoverObjectIconResource = kGenericMoverObjectIconResource,
    genericPreferencesIconResource = kGenericPreferencesIconResource,
    genericQueryDocumentIconResource = kGenericQueryDocumentIconResource,
    genericExtensionIconResource = kGenericExtensionIconResource,
    systemFolderIconResource    = kSystemFolderIconResource,
    appleMenuFolderIconResource = kAppleMenuFolderIconResource
};

/* Avoid using icon resources if possible. Use IconServices instead. */
enum {
    kStartupFolderIconResource  = -3981,
    kOwnedFolderIconResource    = -3980,
    kDropFolderIconResource     = -3979,
    kSharedFolderIconResource   = -3978,
    kMountedFolderIconResource  = -3977,
    kControlPanelFolderIconResource = -3976,
    kPrintMonitorFolderIconResource = -3975,
    kPreferencesFolderIconResource = -3974,
    kExtensionsFolderIconResource = -3973,
    kFontsFolderIconResource    = -3968,
    kFullTrashIconResource      = -3984
};

/* Obsolete. Use named constants defined above. */
enum {
    startupFolderIconResource   = kStartupFolderIconResource,
    ownedFolderIconResource     = kOwnedFolderIconResource,
    dropFolderIconResource      = kDropFolderIconResource,
    sharedFolderIconResource    = kSharedFolderIconResource,
    mountedFolderIconResource   = kMountedFolderIconResource,
    controlPanelFolderIconResource = kControlPanelFolderIconResource,
    printMonitorFolderIconResource = kPrintMonitorFolderIconResource,
    preferencesFolderIconResource = kPreferencesFolderIconResource,
    extensionsFolderIconResource = kExtensionsFolderIconResource,
    fontsFolderIconResource     = kFontsFolderIconResource,
    fullTrashIconResource       = kFullTrashIconResource
};

/* The following icon types can only be used as an icon element */
/* inside a 'icns' icon family */
enum {
    kThumbnail32BitData         = FOUR_CHAR_CODE('it32'),
    kThumbnail8BitMask          = FOUR_CHAR_CODE('t8mk')
};

enum {
    kHuge1BitMask               = FOUR_CHAR_CODE('ich#'),
    kHuge4BitData               = FOUR_CHAR_CODE('ich4'),
    kHuge8BitData               = FOUR_CHAR_CODE('ich8'),
    kHuge32BitData              = FOUR_CHAR_CODE('ih32'),
    kHuge8BitMask               = FOUR_CHAR_CODE('h8mk')
};

/* The following icon types can be used as a resource type */
/* or as an icon element type inside a 'icns' icon family */
enum {
    kLarge1BitMask              = FOUR_CHAR_CODE('ICN#'),
    kLarge4BitData              = FOUR_CHAR_CODE('icl4'),
    kLarge8BitData              = FOUR_CHAR_CODE('icl8'),
    kLarge32BitData             = FOUR_CHAR_CODE('il32'),
    kLarge8BitMask              = FOUR_CHAR_CODE('l8mk'),
    kSmall1BitMask              = FOUR_CHAR_CODE('ics#'),
    kSmall4BitData              = FOUR_CHAR_CODE('ics4'),
    kSmall8BitData              = FOUR_CHAR_CODE('ics8'),
    kSmall32BitData             = FOUR_CHAR_CODE('is32'),
    kSmall8BitMask              = FOUR_CHAR_CODE('s8mk'),
    kMini1BitMask               = FOUR_CHAR_CODE('icm#'),
    kMini4BitData               = FOUR_CHAR_CODE('icm4'),
    kMini8BitData               = FOUR_CHAR_CODE('icm8')
};

/*  Icon Variants */
/* These can be used as an element of an 'icns' icon family */
/* or as a parameter to GetIconRefVariant */
enum {
    kTileIconVariant            = FOUR_CHAR_CODE('tile'),
    kRolloverIconVariant        = FOUR_CHAR_CODE('over'),
    kDropIconVariant            = FOUR_CHAR_CODE('drop'),
    kOpenIconVariant            = FOUR_CHAR_CODE('open'),
    kOpenDropIconVariant        = FOUR_CHAR_CODE('odrp')
};

/* Obsolete. Use names defined above. */
enum {
    large1BitMask               = kLarge1BitMask,
    large4BitData               = kLarge4BitData,
    large8BitData               = kLarge8BitData,
    small1BitMask               = kSmall1BitMask,
    small4BitData               = kSmall4BitData,
    small8BitData               = kSmall8BitData,
    mini1BitMask                = kMini1BitMask,
    mini4BitData                = kMini4BitData,
    mini8BitData                = kMini8BitData
};

/* Alignment type values. */
enum {
    kAlignNone                  = 0x00,
    kAlignVerticalCenter        = 0x01,
    kAlignTop                   = 0x02,
    kAlignBottom                = 0x03,
    kAlignHorizontalCenter      = 0x04,
    kAlignAbsoluteCenter        = kAlignVerticalCenter | kAlignHorizontalCenter,
    kAlignCenterTop             = kAlignTop | kAlignHorizontalCenter,
    kAlignCenterBottom          = kAlignBottom | kAlignHorizontalCenter,
    kAlignLeft                  = 0x08,
    kAlignCenterLeft            = kAlignVerticalCenter | kAlignLeft,
    kAlignTopLeft               = kAlignTop | kAlignLeft,
    kAlignBottomLeft            = kAlignBottom | kAlignLeft,
    kAlignRight                 = 0x0C,
    kAlignCenterRight           = kAlignVerticalCenter | kAlignRight,
    kAlignTopRight              = kAlignTop | kAlignRight,
    kAlignBottomRight           = kAlignBottom | kAlignRight
};

/* Obsolete. Use names defined above. */
enum {
    atNone                      = kAlignNone,
    atVerticalCenter            = kAlignVerticalCenter,
    atTop                       = kAlignTop,
    atBottom                    = kAlignBottom,
    atHorizontalCenter          = kAlignHorizontalCenter,
    atAbsoluteCenter            = kAlignAbsoluteCenter,
    atCenterTop                 = kAlignCenterTop,
    atCenterBottom              = kAlignCenterBottom,
    atLeft                      = kAlignLeft,
    atCenterLeft                = kAlignCenterLeft,
    atTopLeft                   = kAlignTopLeft,
    atBottomLeft                = kAlignBottomLeft,
    atRight                     = kAlignRight,
    atCenterRight               = kAlignCenterRight,
    atTopRight                  = kAlignTopRight,
    atBottomRight               = kAlignBottomRight
};

typedef SInt16                          IconAlignmentType;
/* Transform type values. */
enum {
    kTransformNone              = 0x00,
    kTransformDisabled          = 0x01,
    kTransformOffline           = 0x02,
    kTransformOpen              = 0x03,
    kTransformLabel1            = 0x0100,
    kTransformLabel2            = 0x0200,
    kTransformLabel3            = 0x0300,
    kTransformLabel4            = 0x0400,
    kTransformLabel5            = 0x0500,
    kTransformLabel6            = 0x0600,
    kTransformLabel7            = 0x0700,
    kTransformSelected          = 0x4000,
    kTransformSelectedDisabled  = kTransformSelected | kTransformDisabled,
    kTransformSelectedOffline   = kTransformSelected | kTransformOffline,
    kTransformSelectedOpen      = kTransformSelected | kTransformOpen
};

/* Obsolete. Use names defined above. */
enum {
    ttNone                      = kTransformNone,
    ttDisabled                  = kTransformDisabled,
    ttOffline                   = kTransformOffline,
    ttOpen                      = kTransformOpen,
    ttLabel1                    = kTransformLabel1,
    ttLabel2                    = kTransformLabel2,
    ttLabel3                    = kTransformLabel3,
    ttLabel4                    = kTransformLabel4,
    ttLabel5                    = kTransformLabel5,
    ttLabel6                    = kTransformLabel6,
    ttLabel7                    = kTransformLabel7,
    ttSelected                  = kTransformSelected,
    ttSelectedDisabled          = kTransformSelectedDisabled,
    ttSelectedOffline           = kTransformSelectedOffline,
    ttSelectedOpen              = kTransformSelectedOpen
};

typedef SInt16                          IconTransformType;
/* Selector mask values. */
enum {
    kSelectorLarge1Bit          = 0x00000001,
    kSelectorLarge4Bit          = 0x00000002,
    kSelectorLarge8Bit          = 0x00000004,
    kSelectorLarge32Bit         = 0x00000008,
    kSelectorLarge8BitMask      = 0x00000010,
    kSelectorSmall1Bit          = 0x00000100,
    kSelectorSmall4Bit          = 0x00000200,
    kSelectorSmall8Bit          = 0x00000400,
    kSelectorSmall32Bit         = 0x00000800,
    kSelectorSmall8BitMask      = 0x00001000,
    kSelectorMini1Bit           = 0x00010000,
    kSelectorMini4Bit           = 0x00020000,
    kSelectorMini8Bit           = 0x00040000,
    kSelectorHuge1Bit           = 0x01000000,
    kSelectorHuge4Bit           = 0x02000000,
    kSelectorHuge8Bit           = 0x04000000,
    kSelectorHuge32Bit          = 0x08000000,
    kSelectorHuge8BitMask       = 0x10000000,
    kSelectorAllLargeData       = 0x000000FF,
    kSelectorAllSmallData       = 0x0000FF00,
    kSelectorAllMiniData        = 0x00FF0000,
    kSelectorAllHugeData        = (long)0xFF000000,
    kSelectorAll1BitData        = kSelectorLarge1Bit | kSelectorSmall1Bit | kSelectorMini1Bit | kSelectorHuge1Bit,
    kSelectorAll4BitData        = kSelectorLarge4Bit | kSelectorSmall4Bit | kSelectorMini4Bit | kSelectorHuge4Bit,
    kSelectorAll8BitData        = kSelectorLarge8Bit | kSelectorSmall8Bit | kSelectorMini8Bit | kSelectorHuge8Bit,
    kSelectorAll32BitData       = kSelectorLarge32Bit | kSelectorSmall32Bit | kSelectorHuge32Bit,
    kSelectorAllAvailableData   = (long)0xFFFFFFFF
};


/* Obsolete. Use names defined above. */
enum {
    svLarge1Bit                 = kSelectorLarge1Bit,
    svLarge4Bit                 = kSelectorLarge4Bit,
    svLarge8Bit                 = kSelectorLarge8Bit,
    svSmall1Bit                 = kSelectorSmall1Bit,
    svSmall4Bit                 = kSelectorSmall4Bit,
    svSmall8Bit                 = kSelectorSmall8Bit,
    svMini1Bit                  = kSelectorMini1Bit,
    svMini4Bit                  = kSelectorMini4Bit,
    svMini8Bit                  = kSelectorMini8Bit,
    svAllLargeData              = kSelectorAllLargeData,
    svAllSmallData              = kSelectorAllSmallData,
    svAllMiniData               = kSelectorAllMiniData,
    svAll1BitData               = kSelectorAll1BitData,
    svAll4BitData               = kSelectorAll4BitData,
    svAll8BitData               = kSelectorAll8BitData,
    svAllAvailableData          = kSelectorAllAvailableData
};

typedef UInt32                          IconSelectorValue;
typedef CALLBACK_API( OSErr , IconActionProcPtr )(ResType theType, Handle *theIcon, void *yourDataPtr);
typedef CALLBACK_API( Handle , IconGetterProcPtr )(ResType theType, void *yourDataPtr);
typedef STACK_UPP_TYPE(IconActionProcPtr)                       IconActionUPP;
typedef STACK_UPP_TYPE(IconGetterProcPtr)                       IconGetterUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(IconActionUPP)
    NewIconActionUPP               (IconActionProcPtr       userRoutine);

    EXTERN_API(IconGetterUPP)
    NewIconGetterUPP               (IconGetterProcPtr       userRoutine);

    EXTERN_API(void)
    DisposeIconActionUPP           (IconActionUPP           userUPP);

    EXTERN_API(void)
    DisposeIconGetterUPP           (IconGetterUPP           userUPP);

    EXTERN_API(OSErr)
    InvokeIconActionUPP            (ResType                 theType,
                                    Handle *                theIcon,
                                    void *                  yourDataPtr,
                                    IconActionUPP           userUPP);

    EXTERN_API(Handle)
    InvokeIconGetterUPP            (ResType                 theType,
                                    void *                  yourDataPtr,
                                    IconGetterUPP           userUPP);

#else
    enum { uppIconActionProcInfo = 0x00000FE0 };                    /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppIconGetterProcInfo = 0x000003F0 };                    /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    #define NewIconActionUPP(userRoutine)                           (IconActionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIconActionProcInfo, GetCurrentArchitecture())
    #define NewIconGetterUPP(userRoutine)                           (IconGetterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIconGetterProcInfo, GetCurrentArchitecture())
    #define DisposeIconActionUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeIconGetterUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define InvokeIconActionUPP(theType, theIcon, yourDataPtr, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppIconActionProcInfo, (theType), (theIcon), (yourDataPtr))
    #define InvokeIconGetterUPP(theType, yourDataPtr, userUPP)      (Handle)CALL_TWO_PARAMETER_UPP((userUPP), uppIconGetterProcInfo, (theType), (yourDataPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewIconActionProc(userRoutine)                          NewIconActionUPP(userRoutine)
#define NewIconGetterProc(userRoutine)                          NewIconGetterUPP(userRoutine)
#define CallIconActionProc(userRoutine, theType, theIcon, yourDataPtr) InvokeIconActionUPP(theType, theIcon, yourDataPtr, userRoutine)
#define CallIconGetterProc(userRoutine, theType, yourDataPtr)   InvokeIconGetterUPP(theType, yourDataPtr, userRoutine)
typedef IconGetterProcPtr               IconGetter;
typedef IconActionProcPtr               IconAction;
/* CIconHandle, GetCIcon(), PlotCIcon(), and DisposeCIcon() moved here from Quickdraw.h*/

struct CIcon {
    PixMap                          iconPMap;                   /*the icon's pixMap*/
    BitMap                          iconMask;                   /*the icon's mask*/
    BitMap                          iconBMap;                   /*the icon's bitMap*/
    Handle                          iconData;                   /*the icon's data*/
    SInt16                          iconMaskData[1];            /*icon's mask and BitMap data*/
};
typedef struct CIcon                    CIcon;
typedef CIcon *                         CIconPtr;
typedef CIconPtr *                      CIconHandle;
EXTERN_API( CIconHandle )
GetCIcon                        (SInt16                 iconID)                             ONEWORDINLINE(0xAA1E);

EXTERN_API( void )
PlotCIcon                       (const Rect *           theRect,
                                 CIconHandle            theIcon)                            ONEWORDINLINE(0xAA1F);

EXTERN_API( void )
DisposeCIcon                    (CIconHandle            theIcon)                            ONEWORDINLINE(0xAA25);


/* GetIcon and PlotIcon moved here from ToolUtils*/
EXTERN_API( Handle )
GetIcon                         (SInt16                 iconID)                             ONEWORDINLINE(0xA9BB);

EXTERN_API( void )
PlotIcon                        (const Rect *           theRect,
                                 Handle                 theIcon)                            ONEWORDINLINE(0xA94B);



/*
    Note:   IconSuiteRef and IconCacheRef should be an abstract types, 
            but too much source code already relies on them being of type Handle.
*/

typedef Handle                          IconSuiteRef;
typedef Handle                          IconCacheRef;
/* IconRefs are 32-bit values identifying cached icon data. IconRef 0 is invalid.*/
typedef struct OpaqueIconRef*           IconRef;
EXTERN_API( OSErr )
PlotIconID                      (const Rect *           theRect,
                                 IconAlignmentType      align,
                                 IconTransformType      transform,
                                 SInt16                 theResID)                           THREEWORDINLINE(0x303C, 0x0500, 0xABC9);

EXTERN_API( OSErr )
NewIconSuite                    (IconSuiteRef *         theIconSuite)                       THREEWORDINLINE(0x303C, 0x0207, 0xABC9);

EXTERN_API( OSErr )
AddIconToSuite                  (Handle                 theIconData,
                                 IconSuiteRef           theSuite,
                                 ResType                theType)                            THREEWORDINLINE(0x303C, 0x0608, 0xABC9);

EXTERN_API( OSErr )
GetIconFromSuite                (Handle *               theIconData,
                                 IconSuiteRef           theSuite,
                                 ResType                theType)                            THREEWORDINLINE(0x303C, 0x0609, 0xABC9);

EXTERN_API( OSErr )
ForEachIconDo                   (IconSuiteRef           theSuite,
                                 IconSelectorValue      selector,
                                 IconActionUPP          action,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x303C, 0x080A, 0xABC9);

EXTERN_API( OSErr )
GetIconSuite                    (IconSuiteRef *         theIconSuite,
                                 SInt16                 theResID,
                                 IconSelectorValue      selector)                           THREEWORDINLINE(0x303C, 0x0501, 0xABC9);

EXTERN_API( OSErr )
DisposeIconSuite                (IconSuiteRef           theIconSuite,
                                 Boolean                disposeData)                        THREEWORDINLINE(0x303C, 0x0302, 0xABC9);

EXTERN_API( OSErr )
PlotIconSuite                   (const Rect *           theRect,
                                 IconAlignmentType      align,
                                 IconTransformType      transform,
                                 IconSuiteRef           theIconSuite)                       THREEWORDINLINE(0x303C, 0x0603, 0xABC9);

EXTERN_API( OSErr )
MakeIconCache                   (IconCacheRef *         theCache,
                                 IconGetterUPP          makeIcon,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x303C, 0x0604, 0xABC9);

EXTERN_API( OSErr )
LoadIconCache                   (const Rect *           theRect,
                                 IconAlignmentType      align,
                                 IconTransformType      transform,
                                 IconCacheRef           theIconCache)                       THREEWORDINLINE(0x303C, 0x0606, 0xABC9);

EXTERN_API( OSErr )
PlotIconMethod                  (const Rect *           theRect,
                                 IconAlignmentType      align,
                                 IconTransformType      transform,
                                 IconGetterUPP          theMethod,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x303C, 0x0805, 0xABC9);

EXTERN_API( OSErr )
GetLabel                        (SInt16                 labelNumber,
                                 RGBColor *             labelColor,
                                 Str255                 labelString)                        THREEWORDINLINE(0x303C, 0x050B, 0xABC9);

EXTERN_API( Boolean )
PtInIconID                      (Point                  testPt,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 SInt16                 iconID)                             THREEWORDINLINE(0x303C, 0x060D, 0xABC9);

EXTERN_API( Boolean )
PtInIconSuite                   (Point                  testPt,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 IconSuiteRef           theIconSuite)                       THREEWORDINLINE(0x303C, 0x070E, 0xABC9);

EXTERN_API( Boolean )
PtInIconMethod                  (Point                  testPt,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 IconGetterUPP          theMethod,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x303C, 0x090F, 0xABC9);

EXTERN_API( Boolean )
RectInIconID                    (const Rect *           testRect,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 SInt16                 iconID)                             THREEWORDINLINE(0x303C, 0x0610, 0xABC9);

EXTERN_API( Boolean )
RectInIconSuite                 (const Rect *           testRect,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 IconSuiteRef           theIconSuite)                       THREEWORDINLINE(0x303C, 0x0711, 0xABC9);

EXTERN_API( Boolean )
RectInIconMethod                (const Rect *           testRect,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 IconGetterUPP          theMethod,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x303C, 0x0912, 0xABC9);

EXTERN_API( OSErr )
IconIDToRgn                     (RgnHandle              theRgn,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 SInt16                 iconID)                             THREEWORDINLINE(0x303C, 0x0613, 0xABC9);

EXTERN_API( OSErr )
IconSuiteToRgn                  (RgnHandle              theRgn,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 IconSuiteRef           theIconSuite)                       THREEWORDINLINE(0x303C, 0x0714, 0xABC9);

EXTERN_API( OSErr )
IconMethodToRgn                 (RgnHandle              theRgn,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 IconGetterUPP          theMethod,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x303C, 0x0915, 0xABC9);

EXTERN_API( OSErr )
SetSuiteLabel                   (IconSuiteRef           theSuite,
                                 SInt16                 theLabel)                           THREEWORDINLINE(0x303C, 0x0316, 0xABC9);

EXTERN_API( SInt16 )
GetSuiteLabel                   (IconSuiteRef           theSuite)                           THREEWORDINLINE(0x303C, 0x0217, 0xABC9);

EXTERN_API( OSErr )
GetIconCacheData                (IconCacheRef           theCache,
                                 void **                theData)                            THREEWORDINLINE(0x303C, 0x0419, 0xABC9);

EXTERN_API( OSErr )
SetIconCacheData                (IconCacheRef           theCache,
                                 void *                 theData)                            THREEWORDINLINE(0x303C, 0x041A, 0xABC9);

EXTERN_API( OSErr )
GetIconCacheProc                (IconCacheRef           theCache,
                                 IconGetterUPP *        theProc)                            THREEWORDINLINE(0x303C, 0x041B, 0xABC9);

EXTERN_API( OSErr )
SetIconCacheProc                (IconCacheRef           theCache,
                                 IconGetterUPP          theProc)                            THREEWORDINLINE(0x303C, 0x041C, 0xABC9);

EXTERN_API( OSErr )
PlotIconHandle                  (const Rect *           theRect,
                                 IconAlignmentType      align,
                                 IconTransformType      transform,
                                 Handle                 theIcon)                            THREEWORDINLINE(0x303C, 0x061D, 0xABC9);

EXTERN_API( OSErr )
PlotSICNHandle                  (const Rect *           theRect,
                                 IconAlignmentType      align,
                                 IconTransformType      transform,
                                 Handle                 theSICN)                            THREEWORDINLINE(0x303C, 0x061E, 0xABC9);

EXTERN_API( OSErr )
PlotCIconHandle                 (const Rect *           theRect,
                                 IconAlignmentType      align,
                                 IconTransformType      transform,
                                 CIconHandle            theCIcon)                           THREEWORDINLINE(0x303C, 0x061F, 0xABC9);











/*
   IconServices is an efficient mechanism to share icon data amongst multiple 
   clients. It avoids duplication of data; it provides efficient caching, 
   releasing memory when the icon data is no longer needed; it can provide
   the appropriate icon for any filesystem object; it can provide commonly 
   used icons (caution, note, help...); it is Appearance-savvy: the icons
   are switched when appropriate.
   IconServices refer to cached icon data using IconRef, a 32-bit opaque
   value. IconRefs are reference counted. When there are no more "owners" 
   of an IconRef, the memory used by the icon bitmap is disposed of.
   Two files of same type and creator with no custom icon will have the same IconRef.
   Files with custom icons will have their own IconRef.
*/

/*
   Use the special creator kSystemIconsCreator to get "standard" icons 
   that are not associated with a file, such as the help icon.
   Note that all lowercase creators are reserved by Apple.
*/
enum {
    kSystemIconsCreator         = FOUR_CHAR_CODE('macs')
};


/*
   Type of the predefined/generic icons. For example, the call:
      err = GetIconRef(kOnSystemDisk, kSystemIconsCreator, kHelpIcon, &iconRef);
   will retun in iconRef the IconRef for the standard help icon.
*/

/* Generic Finder icons */
enum {
    kClipboardIcon              = FOUR_CHAR_CODE('CLIP'),
    kClippingUnknownTypeIcon    = FOUR_CHAR_CODE('clpu'),
    kClippingPictureTypeIcon    = FOUR_CHAR_CODE('clpp'),
    kClippingTextTypeIcon       = FOUR_CHAR_CODE('clpt'),
    kClippingSoundTypeIcon      = FOUR_CHAR_CODE('clps'),
    kDesktopIcon                = FOUR_CHAR_CODE('desk'),
    kFinderIcon                 = FOUR_CHAR_CODE('FNDR'),
    kFontSuitcaseIcon           = FOUR_CHAR_CODE('FFIL'),
    kFullTrashIcon              = FOUR_CHAR_CODE('ftrh'),
    kGenericApplicationIcon     = FOUR_CHAR_CODE('APPL'),
    kGenericCDROMIcon           = FOUR_CHAR_CODE('cddr'),
    kGenericControlPanelIcon    = FOUR_CHAR_CODE('APPC'),
    kGenericControlStripModuleIcon = FOUR_CHAR_CODE('sdev'),
    kGenericComponentIcon       = FOUR_CHAR_CODE('thng'),
    kGenericDeskAccessoryIcon   = FOUR_CHAR_CODE('APPD'),
    kGenericDocumentIcon        = FOUR_CHAR_CODE('docu'),
    kGenericEditionFileIcon     = FOUR_CHAR_CODE('edtf'),
    kGenericExtensionIcon       = FOUR_CHAR_CODE('INIT'),
    kGenericFileServerIcon      = FOUR_CHAR_CODE('srvr'),
    kGenericFontIcon            = FOUR_CHAR_CODE('ffil'),
    kGenericFontScalerIcon      = FOUR_CHAR_CODE('sclr'),
    kGenericFloppyIcon          = FOUR_CHAR_CODE('flpy'),
    kGenericHardDiskIcon        = FOUR_CHAR_CODE('hdsk'),
    kGenericRemovableMediaIcon  = FOUR_CHAR_CODE('rmov'),
    kGenericMoverObjectIcon     = FOUR_CHAR_CODE('movr'),
    kGenericPCCardIcon          = FOUR_CHAR_CODE('pcmc'),
    kGenericPreferencesIcon     = FOUR_CHAR_CODE('pref'),
    kGenericQueryDocumentIcon   = FOUR_CHAR_CODE('qery'),
    kGenericRAMDiskIcon         = FOUR_CHAR_CODE('ramd'),
    kGenericSharedLibaryIcon    = FOUR_CHAR_CODE('shlb'),
    kGenericStationeryIcon      = FOUR_CHAR_CODE('sdoc'),
    kGenericSuitcaseIcon        = FOUR_CHAR_CODE('suit'),
    kGenericWORMIcon            = FOUR_CHAR_CODE('worm'),
    kInternationResourcesIcon   = FOUR_CHAR_CODE('ifil'),
    kKeyboardLayoutIcon         = FOUR_CHAR_CODE('kfil'),
    kSoundFileIcon              = FOUR_CHAR_CODE('sfil'),
    kSystemSuitcaseIcon         = FOUR_CHAR_CODE('zsys'),
    kTrashIcon                  = FOUR_CHAR_CODE('trsh'),
    kTrueTypeFontIcon           = FOUR_CHAR_CODE('tfil'),
    kTrueTypeFlatFontIcon       = FOUR_CHAR_CODE('sfnt'),
    kTrueTypeMultiFlatFontIcon  = FOUR_CHAR_CODE('ttcf')
};

/* Internet locations */
enum {
    kInternetLocationHTTPIcon   = FOUR_CHAR_CODE('ilht'),
    kInternetLocationFTPIcon    = FOUR_CHAR_CODE('ilft'),
    kInternetLocationAppleShareIcon = FOUR_CHAR_CODE('ilaf'),
    kInternetLocationAppleTalkZoneIcon = FOUR_CHAR_CODE('ilat'),
    kInternetLocationFileIcon   = FOUR_CHAR_CODE('ilfi'),
    kInternetLocationMailIcon   = FOUR_CHAR_CODE('ilma'),
    kInternetLocationNewsIcon   = FOUR_CHAR_CODE('ilnw'),
    kInternetLocationNSLNeighborhoodIcon = FOUR_CHAR_CODE('ilns'),
    kInternetLocationGenericIcon = FOUR_CHAR_CODE('ilge')
};

/* Folders */
enum {
    kGenericFolderIcon          = FOUR_CHAR_CODE('fldr'),
    kDropFolderIcon             = FOUR_CHAR_CODE('dbox'),
    kMountedFolderIcon          = FOUR_CHAR_CODE('mntd'),
    kOpenFolderIcon             = FOUR_CHAR_CODE('ofld'),
    kOwnedFolderIcon            = FOUR_CHAR_CODE('ownd'),
    kPrivateFolderIcon          = FOUR_CHAR_CODE('prvf'),
    kSharedFolderIcon           = FOUR_CHAR_CODE('shfl')
};

/* Sharing Privileges icons */
enum {
    kSharingPrivsNotApplicableIcon = FOUR_CHAR_CODE('shna'),
    kSharingPrivsReadOnlyIcon   = FOUR_CHAR_CODE('shro'),
    kSharingPrivsReadWriteIcon  = FOUR_CHAR_CODE('shrw'),
    kSharingPrivsUnknownIcon    = FOUR_CHAR_CODE('shuk'),
    kSharingPrivsWritableIcon   = FOUR_CHAR_CODE('writ')
};


/* Users and Groups icons */
enum {
    kUserFolderIcon             = FOUR_CHAR_CODE('ufld'),
    kWorkgroupFolderIcon        = FOUR_CHAR_CODE('wfld'),
    kGuestUserIcon              = FOUR_CHAR_CODE('gusr'),
    kUserIcon                   = FOUR_CHAR_CODE('user'),
    kOwnerIcon                  = FOUR_CHAR_CODE('susr'),
    kGroupIcon                  = FOUR_CHAR_CODE('grup')
};

/* Special folders */
enum {
    kAppleExtrasFolderIcon      = FOUR_CHAR_CODE('aex '),
    kAppleMenuFolderIcon        = FOUR_CHAR_CODE('amnu'),
    kApplicationsFolderIcon     = FOUR_CHAR_CODE('apps'),
    kApplicationSupportFolderIcon = FOUR_CHAR_CODE('asup'),
    kAssistantsFolderIcon       = FOUR_CHAR_CODE('ast '),
    kContextualMenuItemsFolderIcon = FOUR_CHAR_CODE('cmnu'),
    kControlPanelDisabledFolderIcon = FOUR_CHAR_CODE('ctrD'),
    kControlPanelFolderIcon     = FOUR_CHAR_CODE('ctrl'),
    kControlStripModulesFolderIcon = FOUR_CHAR_CODE('sdv '),
    kDocumentsFolderIcon        = FOUR_CHAR_CODE('docs'),
    kExtensionsDisabledFolderIcon = FOUR_CHAR_CODE('extD'),
    kExtensionsFolderIcon       = FOUR_CHAR_CODE('extn'),
    kFavoritesFolderIcon        = FOUR_CHAR_CODE('favs'),
    kFontsFolderIcon            = FOUR_CHAR_CODE('font'),
    kHelpFolderIcon             = FOUR_CHAR_CODE('hlp '),
    kInternetFolderIcon         = FOUR_CHAR_CODE('int '),
    kInternetPlugInFolderIcon   = FOUR_CHAR_CODE('net '),
    kLocalesFolderIcon          = FOUR_CHAR_CODE('loc '),
    kMacOSReadMeFolderIcon      = FOUR_CHAR_CODE('mor '),
    kPreferencesFolderIcon      = FOUR_CHAR_CODE('prf '),
    kPrinterDescriptionFolderIcon = FOUR_CHAR_CODE('ppdf'),
    kPrinterDriverFolderIcon    = FOUR_CHAR_CODE('prd '),
    kPrintMonitorFolderIcon     = FOUR_CHAR_CODE('prnt'),
    kRecentApplicationsFolderIcon = FOUR_CHAR_CODE('rapp'),
    kRecentDocumentsFolderIcon  = FOUR_CHAR_CODE('rdoc'),
    kRecentServersFolderIcon    = FOUR_CHAR_CODE('rsrv'),
    kScriptingAdditionsFolderIcon = FOUR_CHAR_CODE('scr '),
    kSharedLibrariesFolderIcon  = FOUR_CHAR_CODE('lib '),
    kScriptsFolderIcon          = FOUR_CHAR_CODE('scr '),
    kShutdownItemsDisabledFolderIcon = FOUR_CHAR_CODE('shdD'),
    kShutdownItemsFolderIcon    = FOUR_CHAR_CODE('shdf'),
    kSpeakableItemsFolder       = FOUR_CHAR_CODE('spki'),
    kStartupItemsDisabledFolderIcon = FOUR_CHAR_CODE('strD'),
    kStartupItemsFolderIcon     = FOUR_CHAR_CODE('strt'),
    kSystemExtensionDisabledFolderIcon = FOUR_CHAR_CODE('macD'),
    kSystemFolderIcon           = FOUR_CHAR_CODE('macs'),
    kTextEncodingsFolderIcon    = FOUR_CHAR_CODE('tex '),
    kAppearanceFolderIcon       = FOUR_CHAR_CODE('appr'),
    kUtilitiesFolderIcon        = FOUR_CHAR_CODE('uti '),
    kVoicesFolderIcon           = FOUR_CHAR_CODE('fvoc'),
    kColorSyncFolderIcon        = FOUR_CHAR_CODE('prof'),
    kInternetSearchSitesFolderIcon = FOUR_CHAR_CODE('issf'),
    kUsersFolderIcon            = FOUR_CHAR_CODE('usr ')
};

/* Badges */
enum {
    kAppleScriptBadgeIcon       = FOUR_CHAR_CODE('scrp'),
    kLockedBadgeIcon            = FOUR_CHAR_CODE('lbdg'),
    kMountedBadgeIcon           = FOUR_CHAR_CODE('mbdg'),
    kSharedBadgeIcon            = FOUR_CHAR_CODE('sbdg'),
    kAliasBadgeIcon             = FOUR_CHAR_CODE('abdg')
};

/* Alert icons */
enum {
    kAlertNoteIcon              = FOUR_CHAR_CODE('note'),
    kAlertCautionIcon           = FOUR_CHAR_CODE('caut'),
    kAlertStopIcon              = FOUR_CHAR_CODE('stop')
};

/* Networking icons */
enum {
    kAppleTalkIcon              = FOUR_CHAR_CODE('atlk'),
    kAppleTalkZoneIcon          = FOUR_CHAR_CODE('atzn'),
    kAFPServerIcon              = FOUR_CHAR_CODE('afps'),
    kFTPServerIcon              = FOUR_CHAR_CODE('ftps'),
    kHTTPServerIcon             = FOUR_CHAR_CODE('htps'),
    kGenericNetworkIcon         = FOUR_CHAR_CODE('gnet'),
    kIPFileServerIcon           = FOUR_CHAR_CODE('isrv')
};

/* Other icons */
enum {
    kAppleLogoIcon              = FOUR_CHAR_CODE('capl'),
    kAppleMenuIcon              = FOUR_CHAR_CODE('sapl'),
    kBackwardArrowIcon          = FOUR_CHAR_CODE('baro'),
    kFavoriteItemsIcon          = FOUR_CHAR_CODE('favr'),
    kForwardArrowIcon           = FOUR_CHAR_CODE('faro'),
    kGridIcon                   = FOUR_CHAR_CODE('grid'),
    kHelpIcon                   = FOUR_CHAR_CODE('help'),
    kKeepArrangedIcon           = FOUR_CHAR_CODE('arng'),
    kLockedIcon                 = FOUR_CHAR_CODE('lock'),
    kNoFilesIcon                = FOUR_CHAR_CODE('nfil'),
    kNoFolderIcon               = FOUR_CHAR_CODE('nfld'),
    kNoWriteIcon                = FOUR_CHAR_CODE('nwrt'),
    kProtectedApplicationFolderIcon = FOUR_CHAR_CODE('papp'),
    kProtectedSystemFolderIcon  = FOUR_CHAR_CODE('psys'),
    kRecentItemsIcon            = FOUR_CHAR_CODE('rcnt'),
    kShortcutIcon               = FOUR_CHAR_CODE('shrt'),
    kSortAscendingIcon          = FOUR_CHAR_CODE('asnd'),
    kSortDescendingIcon         = FOUR_CHAR_CODE('dsnd'),
    kUnlockedIcon               = FOUR_CHAR_CODE('ulck'),
    kConnectToIcon              = FOUR_CHAR_CODE('cnct')
};



/*  IconServicesUsageFlags */
typedef UInt32                          IconServicesUsageFlags;
enum {
    kIconServicesNormalUsageFlag = 0
};


/*
    IconFamily 'icns' resources contain an entire IconFamily (all sizes and depths).  
   For custom icons, icns IconFamily resources of the custom icon resource ID are fetched first before
   the classic custom icons (individual 'ics#, ICN#, etc) are fetched.  If the fetch of the icns resource
   succeeds then the icns is looked at exclusively for the icon data.
   For custom icons, new icon features such as 32-bit deep icons are only fetched from the icns resource.
   This is to avoid incompatibilities with cut & paste of new style icons with an older version of the
   MacOS Finder.
   DriverGestalt is called with code kdgMediaIconSuite by IconServices after calling FSM to determine a
   driver icon for a particular device.  The result of the kdgMediaIconSuite call to DriverGestalt should
   be a pointer an an IconFamily.  In this manner driver vendors can provide rich, detailed drive icons
   instead of the 1-bit variety previously supported.
*/
enum {
    kIconFamilyType             = FOUR_CHAR_CODE('icns')
};



struct IconFamilyElement {
    OSType                          elementType;                /* 'ICN#', 'icl8', etc...*/
    Size                            elementSize;                /* Size of this element*/
    unsigned char                   elementData[1];
};
typedef struct IconFamilyElement        IconFamilyElement;

struct IconFamilyResource {
    OSType                          resourceType;               /* Always 'icns'*/
    Size                            resourceSize;               /* Total size of this resource*/
    IconFamilyElement               elements[1];

};
typedef struct IconFamilyResource       IconFamilyResource;

typedef IconFamilyResource *            IconFamilyPtr;
typedef IconFamilyPtr *                 IconFamilyHandle;
/*
  ==============================================================================
   Initialization and Termination
  ==============================================================================
*/

/*
   IconServicesInit
   
   Call this routine once per classic 68K application initialization.
   This routine does not need to be called at boot time.
*/

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
IconServicesInit                (CFragInitBlockPtr      initBlockPtr)                       TWOWORDINLINE(0x7015, 0xAA75);


/*
   IconServicesTerminate:
   
   Call this routine once from the termination of a classic 68K application.
   This routine does not need to be called at boot time.
*/

EXTERN_API( void )
IconServicesTerminate           (void)                                                      TWOWORDINLINE(0x7016, 0xAA75);

#endif  /* CALL_NOT_IN_CARBON */

/*
  ==============================================================================
   Converting data structures
  ==============================================================================
*/


/*
   IconRefToIconFamily
   This routines returns a new IconFamily that contains the data corresponding
   to the specified IconRef.
*/

EXTERN_API( OSErr )
IconRefToIconFamily             (IconRef                theIconRef,
                                 IconSelectorValue      whichIcons,
                                 IconFamilyHandle *     iconFamily)                         TWOWORDINLINE(0x7024, 0xAA75);


/*
   IconFamilyToIconSuite
   This routine transfers the data from an icon family handle into an icon suite.
*/

EXTERN_API( OSErr )
IconFamilyToIconSuite           (IconFamilyHandle       iconFamily,
                                 IconSelectorValue      whichIcons,
                                 IconSuiteRef *         iconSuite)                          TWOWORDINLINE(0x7025, 0xAA75);


/*
   IconSuiteToIconFamily
   This routine transfers the data in an icon suite into an icon family.
*/

EXTERN_API( OSErr )
IconSuiteToIconFamily           (IconSuiteRef           iconSuite,
                                 IconSelectorValue      whichIcons,
                                 IconFamilyHandle *     iconFamily)                         TWOWORDINLINE(0x7026, 0xAA75);


/*
   SetIconFamilyData
   Change the data of an icon family. The data is copied.
   The type can be one of the icon type, or 'PICT'.
   The data will be compressed if needed.
*/

EXTERN_API( OSErr )
SetIconFamilyData               (IconFamilyHandle       iconFamily,
                                 OSType                 iconType,
                                 Handle                 h)                                  TWOWORDINLINE(0x7030, 0xAA75);


/*
   GetIconFamilyData
   Return a copy of the data in the icon family.
   The type can be one of the icon type, or 'PICT'
   The data will be returned uncompressed.
   The handle (h) will be resized as appropriate. If no data of the 
   requested type is present, the handle size will be set to 0.
*/

EXTERN_API( OSErr )
GetIconFamilyData               (IconFamilyHandle       iconFamily,
                                 OSType                 iconType,
                                 Handle                 h)                                  TWOWORDINLINE(0x7031, 0xAA75);


/*
  ==============================================================================
   Reference counting
  ==============================================================================
*/


/*
   GetIconRefOwners
   
   This routine returns the reference count for the IconRef, or number of owners.
   
   A valid IconRef always has at least one owner.
*/

EXTERN_API( OSErr )
GetIconRefOwners                (IconRef                theIconRef,
                                 UInt16 *               owners)                             TWOWORDINLINE(0x700B, 0xAA75);


/*
   AcquireIconRef
   This routine increments the reference count for the IconRef
*/

EXTERN_API( OSErr )
AcquireIconRef                  (IconRef                theIconRef)                         TWOWORDINLINE(0x7027, 0xAA75);



/*
   ReleaseIconRef
   
   This routine decrements the reference count for the IconRef.
   
   When the reference count reaches 0, all memory allocated for the icon
   is disposed. Any subsequent use of the IconRef is invalid.
*/

EXTERN_API( OSErr )
ReleaseIconRef                  (IconRef                theIconRef)                         TWOWORDINLINE(0x7028, 0xAA75);



/*
  ==============================================================================
   Getting an IconRef
  ==============================================================================
*/


/*
   GetIconRefFromFile
   
   This routine returns an icon ref for the specified file, folder or volume.
   The label information is provided separately, since two files with the same icon 
   but a different label would share the same iconRef. The label can be used in 
   PlotIconRef() for example.
   
   Use this routine if you have no information about the file system object. If 
   you have already done a GetCatInfo on the file and want to save some I/O, 
   call GetIconRefFromFolder() if you know it's a folder with no custom icon or 
   call GetIconRef() if it's a file with no custom icon.
   This routine increments the reference count of the returned IconRef. Call 
   ReleaseIconRef() when you're done with it.
*/

EXTERN_API( OSErr )
GetIconRefFromFile              (const FSSpec *         theFile,
                                 IconRef *              theIconRef,
                                 SInt16 *               theLabel)                           TWOWORDINLINE(0x7002, 0xAA75);



/*
   GetIconRef
   
   This routine returns an icon ref for an icon in the desktop database or
   for a registered icon.
   The system registers a set of icon such as the help icon with the creator 
   code kSystemIconsCreator. See above for a list of the registered system types.
   The vRefNum is used as a hint on where to look for the icon first. Use 
   kOnSystemDisk if you don't know what to pass.
   This routine increments the reference count of the returned IconRef. Call 
   ReleaseIconRef() when you're done with it.
*/

EXTERN_API( OSErr )
GetIconRef                      (SInt16                 vRefNum,
                                 OSType                 creator,
                                 OSType                 iconType,
                                 IconRef *              theIconRef)                         TWOWORDINLINE(0x7021, 0xAA75);



/*
   GetIconRefFromFolder
   
   This routine returns an icon ref for a folder with no custom icon.
   Use the more generic, but slightly slower, GetIconRefFromFile() if
   you don't already have the necessary info about the file.
   Attributes should be CInfoPBRec.dirInfo.ioFlAttrib for this folder.
   Access privileges should be CInfoPBRec.dirInfo.ioACUser for this folder.
   This routine increments the reference count of the IconRef. Call ReleaseIconRef() 
   when you're done with it.
*/

EXTERN_API( OSErr )
GetIconRefFromFolder            (SInt16                 vRefNum,
                                 SInt32                 parentFolderID,
                                 SInt32                 folderID,
                                 SInt8                  attributes,
                                 SInt8                  accessPrivileges,
                                 IconRef *              theIconRef)                         TWOWORDINLINE(0x7022, 0xAA75);



/*
  ==============================================================================
   Adding and modifying IconRef
  ==============================================================================
*/


/*
   RegisterIconRefFromIconFamily
   This routine adds a new entry to the IconRef registry. Other clients will be 
   able to access it using the (creator, iconType) pair specified here.
   Lower-case creators are reserved for the system.
   Consider using RegisterIconRefFromResource() if possible, since the data 
   registered using RegisterIconRefFromFamily() cannot be purged.
   The iconFamily data is copied and the caller is reponsible for disposing of it.
   This routine increments the reference count of the IconRef. Call ReleaseIconRef() 
   when you're done with it.
*/

EXTERN_API( OSErr )
RegisterIconRefFromIconFamily   (OSType                 creator,
                                 OSType                 iconType,
                                 IconFamilyHandle       iconFamily,
                                 IconRef *              theIconRef)                         TWOWORDINLINE(0x701C, 0xAA75);


/*
   RegisterIconRefFromResource
   
   Registers an IconRef from a resouce file.  
   Lower-case creators are reserved for the system.
   The icon data to be fetched is either classic icon data or an icon family.  
   The 'icns' icon family is searched for before the classic icon data.
   This routine increments the reference count of the IconRef. Call ReleaseIconRef() 
   when you're done with it.
*/

EXTERN_API( OSErr )
RegisterIconRefFromResource     (OSType                 creator,
                                 OSType                 iconType,
                                 const FSSpec *         resourceFile,
                                 SInt16                 resourceID,
                                 IconRef *              theIconRef)                         TWOWORDINLINE(0x7023, 0xAA75);


/*
   UnregisterIconRef
   
   Removes the specified icon from the icon cache (if there are no users of it).  
   If some clients are using this iconRef, then the IconRef will be removed when the 
   last user calls ReleaseIconRef.
*/

EXTERN_API( OSErr )
UnregisterIconRef               (OSType                 creator,
                                 OSType                 iconType)                           TWOWORDINLINE(0x7008, 0xAA75);



/*
   UpdateIconRef
   
   Call this routine to force an update of the data for iconRef.
   
   For example after changing an icon in the desktop database or changing the custom 
   icon of a file. Note that after _adding_ a custom icon to file or folder, you 
   need to call GetIconRefFromFile() to get a new IconRef specific to this file. 
   
   This routine does nothing if the IconRef is a registered icon.
*/

EXTERN_API( OSErr )
UpdateIconRef                   (IconRef                theIconRef)                         TWOWORDINLINE(0x7009, 0xAA75);



/*
   OverrideIconRefFromResource
   
   This routines replaces the bitmaps of the specified IconRef with the ones
   in the specified resource file.
*/

EXTERN_API( OSErr )
OverrideIconRefFromResource     (IconRef                theIconRef,
                                 const FSSpec *         resourceFile,
                                 SInt16                 resourceID)                         TWOWORDINLINE(0x702A, 0xAA75);



/*
   OverrideIconRef
   
   This routines replaces the bitmaps of the specified IconRef with the ones
   from the new IconRef.
*/

EXTERN_API( OSErr )
OverrideIconRef                 (IconRef                oldIconRef,
                                 IconRef                newIconRef)                         TWOWORDINLINE(0x702B, 0xAA75);


/*
   RemoveIconRefOverride
   This routine remove an override if one was applied to the icon and 
   reverts back to the original bitmap data.
*/

EXTERN_API( OSErr )
RemoveIconRefOverride           (IconRef                theIconRef)                         TWOWORDINLINE(0x701E, 0xAA75);



/*
  ==============================================================================
   Creating composite IconRef
  ==============================================================================
*/


/*
   CompositeIconRef
   
   Superimposes an IconRef on top of another one
*/

EXTERN_API( OSErr )
CompositeIconRef                (IconRef                backgroundIconRef,
                                 IconRef                foregroundIconRef,
                                 IconRef *              compositeIconRef)                   TWOWORDINLINE(0x7014, 0xAA75);


/*
   IsIconRefComposite
   Indicates if a given icon ref is a composite of two other icon refs (and which ones)
   If it isn't a composite, backgroundIconRef and foreGroundIconRef will be 0.
*/

EXTERN_API( OSErr )
IsIconRefComposite              (IconRef                compositeIconRef,
                                 IconRef *              backgroundIconRef,
                                 IconRef *              foregroundIconRef)                  TWOWORDINLINE(0x701A, 0xAA75);



/*
  ==============================================================================
   Using IconRef
  ==============================================================================
*/

/*
   IsValidIconRef
   Return true if the iconRef passed in is a valid icon ref
*/

EXTERN_API( Boolean )
IsValidIconRef                  (IconRef                theIconRef)                         TWOWORDINLINE(0x7032, 0xAA75);


/*
   PlotIconRef
   
   This routine plots the IconRef.  It mostly takes the same parameters as 
   PlotIconSuite. Pass kIconServicesNormalUsageFlag as a default value for 
   IconServicesUsageFlags.
*/

EXTERN_API( OSErr )
PlotIconRef                     (const Rect *           theRect,
                                 IconAlignmentType      align,
                                 IconTransformType      transform,
                                 IconServicesUsageFlags  theIconServicesUsageFlags,
                                 IconRef                theIconRef)                         TWOWORDINLINE(0x700E, 0xAA75);


/*
   PtInIconRef
   
   This routine indicates if testPt would hit the icon designated by iconRef.
   It mostly takes the same parameters as PtInIconSuite.
   Pass kIconServicesNormalUsageFlag as a default value for IconServicesUsageFlags.
*/


EXTERN_API( Boolean )
PtInIconRef                     (const Point *          testPt,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 IconServicesUsageFlags  theIconServicesUsageFlags,
                                 IconRef                theIconRef)                         TWOWORDINLINE(0x700F, 0xAA75);


/*
   RectInIconRef
   
   This routine indicates if testRect would intersect the icon designated by iconRef.
   It mostly takes the same parameters as RectInIconSuite.
   Pass kIconServicesNormalUsageFlag as a default value for IconServicesUsageFlags.
*/


EXTERN_API( Boolean )
RectInIconRef                   (const Rect *           testRect,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 IconServicesUsageFlags  iconServicesUsageFlags,
                                 IconRef                theIconRef)                         TWOWORDINLINE(0x7010, 0xAA75);


/*
   IconRefToRgn
   
   This routine returns a region for the icon.
   It mostly takes the same parameters as IconSuiteToRgn.
   Pass kIconServicesNormalUsageFlag as a default value for IconServicesUsageFlags.
*/

EXTERN_API( OSErr )
IconRefToRgn                    (RgnHandle              theRgn,
                                 const Rect *           iconRect,
                                 IconAlignmentType      align,
                                 IconServicesUsageFlags  iconServicesUsageFlags,
                                 IconRef                theIconRef)                         TWOWORDINLINE(0x7011, 0xAA75);


/*
   GetIconSizesFromIconRef
   
   This routine returns an IconSelectorValue indicating the depths and sizes of 
   icon data which are actually available.  It takes an IconSelectorValue 
   indicating which sizes/depths the caller is interested and returns an 
   IconSelectorValue indicating which sizes/depths exist.
   
   Caution:
   
   This is potentially an extremely expensive call as IconServices may be forced 
   to attempt fetching the data for the IconRef's sizes/depths which may result 
   in hitting the local disk or even the network to obtain the data to determine 
   which sizes/depths actually exist.
   Pass kIconServicesNormalUsageFlag as a default value for IconServicesUsageFlags.
*/

EXTERN_API( OSErr )
GetIconSizesFromIconRef         (IconSelectorValue      iconSelectorInput,
                                 IconSelectorValue *    iconSelectorOutputPtr,
                                 IconServicesUsageFlags  iconServicesUsageFlags,
                                 IconRef                theIconRef)                         TWOWORDINLINE(0x7012, 0xAA75);



/*
  ==============================================================================
   Flushing IconRef data
  ==============================================================================
*/


/*
   FlushIconRefs
   
   Making this call will dispose of all the data for the specified icons if 
   the data can be reacquired, for example if the data is provided from a resource.
   '****' is a wildcard for all types or all creators.
*/

EXTERN_API( OSErr )
FlushIconRefs                   (OSType                 creator,
                                 OSType                 iconType)                           TWOWORDINLINE(0x7029, 0xAA75);


/*
   FlushIconRefsByVolume
   
   This routine disposes of the data for the icons related to the indicated volume
   if this data can be reacquired, for example if the data is provided from a 
   resource.
*/

EXTERN_API( OSErr )
FlushIconRefsByVolume           (SInt16                 vRefNum)                            TWOWORDINLINE(0x7018, 0xAA75);



/*
  ==============================================================================
   Controling custom icons
  ==============================================================================
*/


/*
   SetCustomIconsEnabled
   
   Enable or disable custom icons on the specified volume.
*/

EXTERN_API( OSErr )
SetCustomIconsEnabled           (SInt16                 vRefNum,
                                 Boolean                enableCustomIcons)                  TWOWORDINLINE(0x701F, 0xAA75);


/*
   GetCustomIconsEnabled
   
   Return true if custom icons are enabled on the specified volume, false otherwise.
*/

EXTERN_API( OSErr )
GetCustomIconsEnabled           (SInt16                 vRefNum,
                                 Boolean *              customIconsEnabled)                 TWOWORDINLINE(0x7020, 0xAA75);


/*
   IsIconRefMaskEmpty
   Returns true if the mask for this icon is blank
*/

EXTERN_API( Boolean )
IsIconRefMaskEmpty              (IconRef                iconRef)                            TWOWORDINLINE(0x7033, 0xAA75);


/*
   GetIconRefVariant
   Icon variants allows different images to be used with different icon state.
   For example, the 'open' variant for a folder could be represented with
   an open folder.
   Given an icon ref and a desired variant, this routine returns an icon
   ref (which may be the same as the input icon ref) and a transformation 
   which should be used with PlotIconRef() to render the icon appropriately.
   The returned icon ref should be used to do hit-testing.
*/
EXTERN_API( IconRef )
GetIconRefVariant               (IconRef                inIconRef,
                                 OSType                 inVariant,
                                 IconTransformType *    outTransform)                       TWOWORDINLINE(0x7034, 0xAA75);



/*
  ==============================================================================
   Icon files (.icns files)
  ==============================================================================
*/


/*
   RegisterIconRefFromIconFile
   This routine adds a new entry to the IconRef registry. Other clients will be 
   able to access it using the (creator, iconType) pair specified here.
   Lower-case creators are reserved for the system.
   If the creator is kSystemIconsCreator and the iconType is 0, a new IconRef
   is always returned. Otherwise, if the creator and type have already been
   registered, the previously registered IconRef is returned.
   This routine increments the reference count of the IconRef. Call ReleaseIconRef() 
   when you're done with it.
*/

EXTERN_API( OSErr )
RegisterIconRefFromIconFile     (OSType                 creator,
                                 OSType                 iconType,
                                 const FSSpec *         iconFile,
                                 IconRef *              theIconRef)                         TWOWORDINLINE(0x7035, 0xAA75);


/*
   ReadIconFile
   Read the specified icon file into the icon family handle.
   The caller is responsible for disposing the iconFamily
*/

EXTERN_API( OSErr )
ReadIconFile                    (const FSSpec *         iconFile,
                                 IconFamilyHandle *     iconFamily)                         TWOWORDINLINE(0x7036, 0xAA75);


/*
   WriteIconFile
   Write the iconFamily handle to the specified file
*/

EXTERN_API( OSErr )
WriteIconFile                   (IconFamilyHandle       iconFamily,
                                 const FSSpec *         iconFile)                           TWOWORDINLINE(0x7037, 0xAA75);


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

#endif /* __ICONS__ */

