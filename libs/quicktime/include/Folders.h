/*
     File:       Folders.h
 
     Contains:   Folder Manager Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FOLDERS__
#define __FOLDERS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __FILES__
#include <Files.h>
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
    kOnSystemDisk               = -32768L,                      /* previously was 0x8000 but that is an unsigned value whereas vRefNum is signed*/
    kOnAppropriateDisk          = -32767                        /* Generally, the same as kOnSystemDisk, but it's clearer that this isn't always the 'boot' disk.*/
};

enum {
    kCreateFolder               = true,
    kDontCreateFolder           = false
};

enum {
    kSystemFolderType           = FOUR_CHAR_CODE('macs'),       /* the system folder */
    kDesktopFolderType          = FOUR_CHAR_CODE('desk'),       /* the desktop folder; objects in this folder show on the desk top. */
    kSystemDesktopFolderType    = FOUR_CHAR_CODE('sdsk'),       /* the desktop folder at the root of the hard drive, never the redirected user desktop folder */
    kTrashFolderType            = FOUR_CHAR_CODE('trsh'),       /* the trash folder; objects in this folder show up in the trash */
    kSystemTrashFolderType      = FOUR_CHAR_CODE('strs'),       /* the trash folder at the root of the drive, never the redirected user trash folder */
    kWhereToEmptyTrashFolderType = FOUR_CHAR_CODE('empt'),      /* the "empty trash" folder; Finder starts empty from here down */
    kPrintMonitorDocsFolderType = FOUR_CHAR_CODE('prnt'),       /* Print Monitor documents */
    kStartupFolderType          = FOUR_CHAR_CODE('strt'),       /* Finder objects (applications, documents, DAs, aliases, to...) to open at startup go here */
    kShutdownFolderType         = FOUR_CHAR_CODE('shdf'),       /* Finder objects (applications, documents, DAs, aliases, to...) to open at shutdown go here */
    kAppleMenuFolderType        = FOUR_CHAR_CODE('amnu'),       /* Finder objects to put into the Apple menu go here */
    kControlPanelFolderType     = FOUR_CHAR_CODE('ctrl'),       /* Control Panels go here (may contain INITs) */
    kSystemControlPanelFolderType = FOUR_CHAR_CODE('sctl'),     /* System control panels folder - never the redirected one, always "Control Panels" inside the System Folder */
    kExtensionFolderType        = FOUR_CHAR_CODE('extn'),       /* System extensions go here */
    kFontsFolderType            = FOUR_CHAR_CODE('font'),       /* Fonts go here */
    kPreferencesFolderType      = FOUR_CHAR_CODE('pref'),       /* preferences for applications go here */
    kSystemPreferencesFolderType = FOUR_CHAR_CODE('sprf'),      /* System-type Preferences go here - this is always the system's preferences folder, never a logged in user's */
    kTemporaryFolderType        = FOUR_CHAR_CODE('temp')        /* temporary files go here (deleted periodically, but don't rely on it.) */
};

EXTERN_API( OSErr )
FindFolder                      (short                  vRefNum,
                                 OSType                 folderType,
                                 Boolean                createFolder,
                                 short *                foundVRefNum,
                                 long *                 foundDirID)                         TWOWORDINLINE(0x7000, 0xA823);

EXTERN_API( OSErr )
FindFolderExtended              (short                  vol,
                                 OSType                 foldType,
                                 Boolean                createFolder,
                                 UInt32                 flags,
                                 void *                 data,
                                 short *                vRefNum,
                                 long *                 dirID)                              THREEWORDINLINE(0x303C, 0x0B2C, 0xA823);

EXTERN_API( OSErr )
ReleaseFolder                   (short                  vRefNum,
                                 OSType                 folderType)                         TWOWORDINLINE(0x700B, 0xA823);


#if !TARGET_OS_MAC
/* Since non-mac targets don't know about VRef's or DirID's, the Ex version returns
   the found folder path.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
FindFolderEx                    (short                  vRefNum,
                                 OSType                 folderType,
                                 Boolean                createFolder,
                                 short *                foundVRefNum,
                                 long *                 foundDirID,
                                 char *                 foundFolder);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !TARGET_OS_MAC */

/******************************************/
/* Extensible Folder Manager declarations */
/******************************************/

/****************************/
/* Folder Manager constants */
/****************************/

enum {
    kExtensionDisabledFolderType = FOUR_CHAR_CODE('extD'),
    kControlPanelDisabledFolderType = FOUR_CHAR_CODE('ctrD'),
    kSystemExtensionDisabledFolderType = FOUR_CHAR_CODE('macD'),
    kStartupItemsDisabledFolderType = FOUR_CHAR_CODE('strD'),
    kShutdownItemsDisabledFolderType = FOUR_CHAR_CODE('shdD'),
    kApplicationsFolderType     = FOUR_CHAR_CODE('apps'),
    kDocumentsFolderType        = FOUR_CHAR_CODE('docs')
};

enum {
                                                                /* new constants */
    kVolumeRootFolderType       = FOUR_CHAR_CODE('root'),       /* root folder of a volume */
    kChewableItemsFolderType    = FOUR_CHAR_CODE('flnt'),       /* items deleted at boot */
    kApplicationSupportFolderType = FOUR_CHAR_CODE('asup'),     /* third-party items and folders */
    kTextEncodingsFolderType    = FOUR_CHAR_CODE('tex '),       /* encoding tables */
    kStationeryFolderType       = FOUR_CHAR_CODE('odst'),       /* stationery */
    kOpenDocFolderType          = FOUR_CHAR_CODE('odod'),       /* OpenDoc root */
    kOpenDocShellPlugInsFolderType = FOUR_CHAR_CODE('odsp'),    /* OpenDoc Shell Plug-Ins in OpenDoc folder */
    kEditorsFolderType          = FOUR_CHAR_CODE('oded'),       /* OpenDoc editors in MacOS Folder */
    kOpenDocEditorsFolderType   = FOUR_CHAR_CODE('odf '),       /* OpenDoc subfolder of Editors folder */
    kOpenDocLibrariesFolderType = FOUR_CHAR_CODE('odlb'),       /* OpenDoc libraries folder */
    kGenEditorsFolderType       = FOUR_CHAR_CODE('edi '),       /* CKH general editors folder at root level of Sys folder */
    kHelpFolderType             = FOUR_CHAR_CODE('hlp '),       /* CKH help folder currently at root of system folder */
    kInternetPlugInFolderType   = FOUR_CHAR_CODE('net '),       /* CKH internet plug ins for browsers and stuff */
    kModemScriptsFolderType     = FOUR_CHAR_CODE('mod '),       /* CKH modem scripts, get 'em OUT of the Extensions folder */
    kPrinterDescriptionFolderType = FOUR_CHAR_CODE('ppdf'),     /* CKH new folder at root of System folder for printer descs. */
    kPrinterDriverFolderType    = FOUR_CHAR_CODE('prd '),       /* CKH new folder at root of System folder for printer drivers */
    kScriptingAdditionsFolderType = FOUR_CHAR_CODE('scr '),     /* CKH at root of system folder */
    kSharedLibrariesFolderType  = FOUR_CHAR_CODE('lib '),       /* CKH for general shared libs. */
    kVoicesFolderType           = FOUR_CHAR_CODE('fvoc'),       /* CKH macintalk can live here */
    kControlStripModulesFolderType = FOUR_CHAR_CODE('sdev'),    /* CKH for control strip modules */
    kAssistantsFolderType       = FOUR_CHAR_CODE('ast '),       /* SJF for Assistants (MacOS Setup Assistant, etc) */
    kUtilitiesFolderType        = FOUR_CHAR_CODE('uti '),       /* SJF for Utilities folder */
    kAppleExtrasFolderType      = FOUR_CHAR_CODE('aex '),       /* SJF for Apple Extras folder */
    kContextualMenuItemsFolderType = FOUR_CHAR_CODE('cmnu'),    /* SJF for Contextual Menu items */
    kMacOSReadMesFolderType     = FOUR_CHAR_CODE('mor '),       /* SJF for MacOS ReadMes folder */
    kALMModulesFolderType       = FOUR_CHAR_CODE('walk'),       /* EAS for Location Manager Module files except type 'thng' (within kExtensionFolderType) */
    kALMPreferencesFolderType   = FOUR_CHAR_CODE('trip'),       /* EAS for Location Manager Preferences (within kPreferencesFolderType; contains kALMLocationsFolderType) */
    kALMLocationsFolderType     = FOUR_CHAR_CODE('fall'),       /* EAS for Location Manager Locations (within kALMPreferencesFolderType) */
    kColorSyncProfilesFolderType = FOUR_CHAR_CODE('prof'),      /* for ColorSync(tm) Profiles */
    kThemesFolderType           = FOUR_CHAR_CODE('thme'),       /* for Theme data files */
    kFavoritesFolderType        = FOUR_CHAR_CODE('favs'),       /* Favorties folder for Navigation Services */
    kInternetFolderType         = FOUR_CHAR_CODE('int '),       /* Internet folder (root level of startup volume) */
    kAppearanceFolderType       = FOUR_CHAR_CODE('appr'),       /* Appearance folder (root of system folder) */
    kSoundSetsFolderType        = FOUR_CHAR_CODE('snds'),       /* Sound Sets folder (in Appearance folder) */
    kDesktopPicturesFolderType  = FOUR_CHAR_CODE('dtp '),       /* Desktop Pictures folder (in Appearance folder) */
    kInternetSearchSitesFolderType = FOUR_CHAR_CODE('issf'),    /* Internet Search Sites folder */
    kFindSupportFolderType      = FOUR_CHAR_CODE('fnds'),       /* Find support folder */
    kFindByContentFolderType    = FOUR_CHAR_CODE('fbcf'),       /* Find by content folder */
    kInstallerLogsFolderType    = FOUR_CHAR_CODE('ilgf'),       /* Installer Logs folder */
    kScriptsFolderType          = FOUR_CHAR_CODE('scr '),       /* Scripts folder */
    kFolderActionsFolderType    = FOUR_CHAR_CODE('fasf'),       /* Folder Actions Scripts folder */
    kLauncherItemsFolderType    = FOUR_CHAR_CODE('laun'),       /* Launcher Items folder */
    kRecentApplicationsFolderType = FOUR_CHAR_CODE('rapp'),     /* Recent Applications folder */
    kRecentDocumentsFolderType  = FOUR_CHAR_CODE('rdoc'),       /* Recent Documents folder */
    kRecentServersFolderType    = FOUR_CHAR_CODE('rsvr'),       /* Recent Servers folder */
    kSpeakableItemsFolderType   = FOUR_CHAR_CODE('spki'),       /* Speakable Items folder */
    kKeychainFolderType         = FOUR_CHAR_CODE('kchn'),       /* Keychain folder */
    kQuickTimeExtensionsFolderType = FOUR_CHAR_CODE('qtex'),    /* QuickTime Extensions Folder (in Extensions folder) */
    kDisplayExtensionsFolderType = FOUR_CHAR_CODE('dspl'),      /* Display Extensions Folder (in Extensions folder) */
    kMultiprocessingFolderType  = FOUR_CHAR_CODE('mpxf'),       /* Multiprocessing Folder (in Extensions folder) */
    kPrintingPlugInsFolderType  = FOUR_CHAR_CODE('pplg')        /* Printing Plug-Ins Folder (in Extensions folder) */
};

enum {
    kLocalesFolderType          = FOUR_CHAR_CODE('loc '),       /* PKE for Locales folder */
    kFindByContentPluginsFolderType = FOUR_CHAR_CODE('fbcp')    /* Find By Content Plug-ins */
};

enum {
    kUsersFolderType            = FOUR_CHAR_CODE('usrs'),       /* "Users" folder, contains one folder for each user. */
    kCurrentUserFolderType      = FOUR_CHAR_CODE('cusr'),       /* The folder for the currently logged on user. */
    kCurrentUserRemoteFolderLocation = FOUR_CHAR_CODE('rusf'),  /* The remote folder for the currently logged on user */
    kCurrentUserRemoteFolderType = FOUR_CHAR_CODE('rusr'),      /* The remote folder location for the currently logged on user */
    kSharedUserDataFolderType   = FOUR_CHAR_CODE('sdat'),       /* A Shared "Documents" folder, readable & writeable by all users */
    kVolumeSettingsFolderType   = FOUR_CHAR_CODE('vsfd')        /* Volume specific user information goes here */
};

/* FolderDescFlags values */
enum {
    kCreateFolderAtBoot         = 0x00000002,
    kFolderCreatedInvisible     = 0x00000004,
    kFolderCreatedNameLocked    = 0x00000008,
    kFolderCreatedAdminPrivs    = 0x00000010
};

enum {
    kFolderInUserFolder         = 0x00000020,
    kFolderTrackedByAlias       = 0x00000040,
    kFolderInRemoteUserFolderIfAvailable = 0x00000080,
    kFolderNeverMatchedInIdentifyFolder = 0x00000100,
    kFolderMustStayOnSameVolume = 0x00000200,
    kFolderInLocalOrRemoteUserFolder = kFolderInUserFolder | kFolderInRemoteUserFolderIfAvailable
};

typedef UInt32                          FolderDescFlags;
/* FolderClass values */
enum {
    kRelativeFolder             = FOUR_CHAR_CODE('relf'),
    kSpecialFolder              = FOUR_CHAR_CODE('spcf')
};

typedef OSType                          FolderClass;
/* special folder locations */
enum {
    kBlessedFolder              = FOUR_CHAR_CODE('blsf'),
    kRootFolder                 = FOUR_CHAR_CODE('rotf')
};

enum {
    kCurrentUserFolderLocation  = FOUR_CHAR_CODE('cusf')        /*    the magic 'Current User' folder location*/
};

typedef OSType                          FolderType;
typedef OSType                          FolderLocation;


struct FolderDesc {
    Size                            descSize;
    FolderType                      foldType;
    FolderDescFlags                 flags;
    FolderClass                     foldClass;
    FolderType                      foldLocation;
    OSType                          badgeSignature;
    OSType                          badgeType;
    UInt32                          reserved;
    StrFileName                     name;                       /* Str63 on MacOS*/
};
typedef struct FolderDesc               FolderDesc;
typedef FolderDesc *                    FolderDescPtr;


typedef UInt32                          RoutingFlags;

struct FolderRouting {
    Size                            descSize;
    OSType                          fileType;
    FolderType                      routeFromFolder;
    FolderType                      routeToFolder;
    RoutingFlags                    flags;
};
typedef struct FolderRouting            FolderRouting;
typedef FolderRouting *                 FolderRoutingPtr;
/* routing constants */
/*  These are bits in the .flags field of the FindFolderUserRedirectionGlobals struct*/
enum {
                                                                /*    Set this bit to 1 in the .flags field of a FindFolderUserRedirectionGlobals*/
                                                                /*    structure if the userName in the struct should be used as the current*/
                                                                /*    "User" name*/
    kFindFolderRedirectionFlagUseDistinctUserFoldersBit = 0,    /*    Set this bit to 1 and the currentUserFolderVRefNum and currentUserFolderDirID*/
                                                                /*    fields of the user record will get used instead of finding the user folder*/
                                                                /*    with the userName field.*/
    kFindFolderRedirectionFlagUseGivenVRefAndDirIDAsUserFolderBit = 1, /*    Set this bit to 1 and the remoteUserFolderVRefNum and remoteUserFolderDirID*/
                                                                /*    fields of the user record will get used instead of finding the user folder*/
                                                                /*    with the userName field.*/
    kFindFolderRedirectionFlagsUseGivenVRefNumAndDirIDAsRemoteUserFolderBit = 2
};


struct FindFolderUserRedirectionGlobals {
    UInt32                          version;
    UInt32                          flags;

    Str31                           userName;
    short                           userNameScript;

    short                           currentUserFolderVRefNum;
    long                            currentUserFolderDirID;

    short                           remoteUserFolderVRefNum;
    long                            remoteUserFolderDirID;
};
typedef struct FindFolderUserRedirectionGlobals FindFolderUserRedirectionGlobals;
typedef FindFolderUserRedirectionGlobals * FindFolderUserRedirectionGlobalsPtr;
enum {
    kFolderManagerUserRedirectionGlobalsCurrentVersion = 1
};

/*
    These are passed into FindFolderExtended(), FindFolderInternalExtended(), and
    FindFolderNewInstallerEntryExtended() in the flags field. 
*/
enum {
    kFindFolderExtendedFlagsDoNotFollowAliasesBit = 0,
    kFindFolderExtendedFlagsDoNotUseUserFolderBit = 1,
    kFindFolderExtendedFlagsUseOtherUserRecord = 0x01000000
};

typedef CALLBACK_API( OSStatus , FolderManagerNotificationProcPtr )(OSType message, void *arg, void *userRefCon);
typedef STACK_UPP_TYPE(FolderManagerNotificationProcPtr)        FolderManagerNotificationUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(FolderManagerNotificationUPP)
    NewFolderManagerNotificationUPP    (FolderManagerNotificationProcPtr userRoutine);

    EXTERN_API(void)
    DisposeFolderManagerNotificationUPP    (FolderManagerNotificationUPP userUPP);

    EXTERN_API(OSStatus)
    InvokeFolderManagerNotificationUPP    (OSType           message,
                                    void *                  arg,
                                    void *                  userRefCon,
                                    FolderManagerNotificationUPP userUPP);

#else
    enum { uppFolderManagerNotificationProcInfo = 0x00000FF0 };     /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewFolderManagerNotificationUPP(userRoutine)            (FolderManagerNotificationUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppFolderManagerNotificationProcInfo, GetCurrentArchitecture())
    #define DisposeFolderManagerNotificationUPP(userUPP)            DisposeRoutineDescriptor(userUPP)
    #define InvokeFolderManagerNotificationUPP(message, arg, userRefCon, userUPP)  (OSStatus)CALL_THREE_PARAMETER_UPP((userUPP), uppFolderManagerNotificationProcInfo, (message), (arg), (userRefCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewFolderManagerNotificationProc(userRoutine)           NewFolderManagerNotificationUPP(userRoutine)
#define CallFolderManagerNotificationProc(userRoutine, message, arg, userRefCon) InvokeFolderManagerNotificationUPP(message, arg, userRefCon, userRoutine)
enum {
    kFolderManagerNotificationMessageUserLogIn = FOUR_CHAR_CODE('log+'), /*    Sent by system & third party software after a user logs in.  arg should point to a valid FindFolderUserRedirectionGlobals structure or nil for the owner*/
    kFolderManagerNotificationMessagePreUserLogIn = FOUR_CHAR_CODE('logj'), /*    Sent by system & third party software before a user logs in.  arg should point to a valid FindFolderUserRedirectionGlobals structure or nil for the owner*/
    kFolderManagerNotificationMessageUserLogOut = FOUR_CHAR_CODE('log-'), /*    Sent by system & third party software before a user logs out.  arg should point to a valid FindFolderUserRedirectionGlobals structure or nil for the owner*/
    kFolderManagerNotificationMessagePostUserLogOut = FOUR_CHAR_CODE('logp'), /*    Sent by system & third party software after a user logs out.  arg should point to a valid FindFolderUserRedirectionGlobals structure or nil for the owner*/
    kFolderManagerNotificationDiscardCachedData = FOUR_CHAR_CODE('dche') /*    Sent by third party software when the entire Folder Manager cache should be flushed*/
};


/*  These get used in the options parameter of FolderManagerRegisterNotificationProc()*/
enum {
    kDoNotRemoveWhenCurrentApplicationQuitsBit = 0,
    kDoNotRemoveWheCurrentApplicationQuitsBit = kDoNotRemoveWhenCurrentApplicationQuitsBit /*    Going away soon, use kDoNotRemoveWheCurrentApplicationQuitsBit*/
};

/*  These get used in the options parameter of FolderManagerCallNotificationProcs()*/
enum {
    kStopIfAnyNotificationProcReturnsErrorBit = 31
};

/***************************/
/* Folder Manager routines */
/***************************/
/* Folder Manager administration routines */
EXTERN_API( OSErr )
AddFolderDescriptor             (FolderType             foldType,
                                 FolderDescFlags        flags,
                                 FolderClass            foldClass,
                                 FolderLocation         foldLocation,
                                 OSType                 badgeSignature,
                                 OSType                 badgeType,
                                 ConstStrFileNameParam  name,
                                 Boolean                replaceFlag)                        TWOWORDINLINE(0x7020, 0xA823);

EXTERN_API( OSErr )
GetFolderDescriptor             (FolderType             foldType,
                                 Size                   descSize,
                                 FolderDesc *           foldDesc)                           TWOWORDINLINE(0x7023, 0xA823);

EXTERN_API( OSErr )
GetFolderTypes                  (UInt32                 requestedTypeCount,
                                 UInt32 *               totalTypeCount,
                                 FolderType *           theTypes)                           TWOWORDINLINE(0x7024, 0xA823);

EXTERN_API( OSErr )
RemoveFolderDescriptor          (FolderType             foldType)                           TWOWORDINLINE(0x7021, 0xA823);

/* legacy routines */
EXTERN_API( OSErr )
GetFolderName                   (short                  vRefNum,
                                 OSType                 foldType,
                                 short *                foundVRefNum,
                                 StrFileName            name)                               TWOWORDINLINE(0x700E, 0xA823);

/* routing routines */
EXTERN_API( OSErr )
AddFolderRouting                (OSType                 fileType,
                                 FolderType             routeFromFolder,
                                 FolderType             routeToFolder,
                                 RoutingFlags           flags,
                                 Boolean                replaceFlag)                        THREEWORDINLINE(0x303C, 0x0926, 0xA823);

EXTERN_API( OSErr )
RemoveFolderRouting             (OSType                 fileType,
                                 FolderType             routeFromFolder)                    THREEWORDINLINE(0x303C, 0x0427, 0xA823);

EXTERN_API( OSErr )
FindFolderRouting               (OSType                 fileType,
                                 FolderType             routeFromFolder,
                                 FolderType *           routeToFolder,
                                 RoutingFlags *         flags)                              THREEWORDINLINE(0x303C, 0x0822, 0xA823);

EXTERN_API( OSErr )
GetFolderRoutings               (UInt32                 requestedRoutingCount,
                                 UInt32 *               totalRoutingCount,
                                 Size                   routingSize,
                                 FolderRouting *        theRoutings)                        THREEWORDINLINE(0x303C, 0x081E, 0xA823);

EXTERN_API( OSErr )
InvalidateFolderDescriptorCache (short                  vRefNum,
                                 long                   dirID)                              THREEWORDINLINE(0x303C, 0x0325, 0xA823);

EXTERN_API( OSErr )
IdentifyFolder                  (short                  vRefNum,
                                 long                   dirID,
                                 FolderType *           foldType)                           THREEWORDINLINE(0x303C, 0x051F, 0xA823);


EXTERN_API( OSErr )
FolderManagerRegisterNotificationProc (FolderManagerNotificationUPP  notificationProc,
                                 void *                 refCon,
                                 UInt32                 options)                            THREEWORDINLINE(0x303C, 0x062F, 0xA823);

EXTERN_API( OSErr )
FolderManagerUnregisterNotificationProc (FolderManagerNotificationUPP  notificationProc,
                                 void *                 refCon)                             THREEWORDINLINE(0x303C, 0x0430, 0xA823);

EXTERN_API( OSStatus )
FolderManagerRegisterCallNotificationProcs (OSType      message,
                                 void *                 arg,
                                 UInt32                 options)                            THREEWORDINLINE(0x303C, 0x0631, 0xA823);

/*******************************/
/* MultiUser (At Ease) globals */
/*******************************/
/*
   This structure has been through some evolution since the early days of At Ease 1.0.  The structure
   has been expanded (and developers should assume that it will continue this way into the future).  Older
   fields have been obsoleted as the features have changed in newer versions of the code.
*/

/* Some fields in here are really only valid for the network version of Macintosh Manager*/


struct MultiUserGestalt {
                                                                /*    Version 1 fields.*/
    short                           giVersion;                  /* structure version: 0 = invalid, 6 = OS 9*/
    short                           giReserved0;                /* [OBSOLETE with v3] giIsActive: if true then At Ease is currently running*/
    short                           giReserved1;                /* [OBSOLETE] if true then auto create alias*/
    short                           giReserved2;                /* [OBSOLETE with v6]  if true then request floppy on new saves*/
    short                           giReserved3;                /* [OBSOLETE] if true then hypercard stacks are shown on Applications panel*/
    FSSpec                          giReserved4;                /* [OBSOLETE with v6] location of At Ease Items folder*/

                                                                /*    Version 2 fields.*/
    short                           giDocsVRefNum;              /* vrefnum of user's documents location (only valid if not on floppy)*/
    long                            giDocsDirID;                /* directory id of user's documents folder (only valid if not on floppy)*/
    short                           giForceSaves;               /* true if user is forced to save to their documents folder*/
    short                           giForceOpens;               /* true if user is forced to open from their documents folder*/
    Str31                           giSetupName;                /* name of current setup*/
    Str31                           giUserName;                 /* name of current user*/
    Str31                           giFrontAppName;             /* name of the frontmost application*/
    short                           giReserved5;                /* [OBSOLETE with v6] true if user has Go To Finder menu item*/
    short                           giIsOn;                     /* true if Multiple Users/Macintosh Manager is on right now*/

                                                                /*    Version 3 fields.*/
                                                                /*  There were no additional fields for version 3.x*/

                                                                /*    Version 4 fields.*/
    short                           giUserLoggedInType;         /* 0 = normal user, 1 = workgroup admin, 2 = global admin*/
    char                            giUserEncryptPwd[16];       /* encrypted user password (our digest form)*/
    short                           giUserEnvironment;          /* 0 = panels, 1 = normal Finder, 2 = limited/restricted Finder*/
    long                            giReserved6;                /* [OBSOLETE]*/
    long                            giReserved7;                /* [OBSOLETE]*/
    Boolean                         giDisableScrnShots;         /* true if screen shots are not allowed*/

                                                                /*    Version 5 fields.*/
    Boolean                         giSupportsAsyncFSCalls;     /* Finder uses this to tell if our patches support async trap patches*/
    short                           giPrefsVRefNum;             /* vrefnum of preferences*/
    long                            giPrefsDirID;               /* dirID of the At Ease Items folder on preferences volume*/
    unsigned long                   giUserLogInTime;            /* time in seconds we've been logged in (0 or 1 mean not logged in)*/
    Boolean                         giUsingPrintQuotas;         /* true if logged in user is using printer quotas*/
    Boolean                         giUsingDiskQuotas;          /* true if logged in user has disk quotas active*/

                                                                /* Version 6 fields - As of Mac OS 9's "Multiple Users 1.0"*/
    Boolean                         giInSystemAccess;           /* true if system is in System Access (i.e., owner logged in)*/
    Boolean                         giUserFolderEnabled;        /* true if FindFolder is redirecting folders (uses giUserName for user)*/
    short                           giReserved8;
    long                            giReserved9;
    Boolean                         giInLoginScreen;            /* true if no user has logged in (including owner)*/


                                                                /* May have more fields added in future, so never check for sizeof(GestaltRec)*/

};
typedef struct MultiUserGestalt         MultiUserGestalt;
typedef MultiUserGestalt *              MultiUserGestaltPtr;
typedef MultiUserGestaltPtr *           MultiUserGestaltHandle;



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

#endif /* __FOLDERS__ */

