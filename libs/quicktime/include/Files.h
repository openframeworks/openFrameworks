/*
     File:       Files.h
 
     Contains:   File Manager (MFS, HFS, and HFS+) Interfaces.
 
     Version:    Technology: Mac OS 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FILES__
#define __FILES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __OSUTILS__
#include <OSUtils.h>
#endif

#ifndef __TEXTCOMMON__
#include <TextCommon.h>
#endif

#ifndef __UTCUTILS__
#include <UTCUtils.h>
#endif


/* Finder constants were moved to Finder.* */
#ifndef __FINDER__
#include <Finder.h>
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

/* HFSUniStr255 is the Unicode equivalent of Str255 */

struct HFSUniStr255 {
    UInt16                          length;                     /* number of unicode characters */
    UniChar                         unicode[255];               /* unicode characters */
};
typedef struct HFSUniStr255             HFSUniStr255;

typedef const HFSUniStr255 *            ConstHFSUniStr255Param;
enum {
    fsCurPerm                   = 0x00,                         /* open access permissions in ioPermssn */
    fsRdPerm                    = 0x01,
    fsWrPerm                    = 0x02,
    fsRdWrPerm                  = 0x03,
    fsRdWrShPerm                = 0x04,
    fsRdDenyPerm                = 0x10,                         /* for use with OpenDeny and OpenRFDeny */
    fsWrDenyPerm                = 0x20                          /* for use with OpenDeny and OpenRFDeny */
};

enum {
    fsRtParID                   = 1,
    fsRtDirID                   = 2
};

enum {
    fsAtMark                    = 0,                            /* positioning modes in ioPosMode */
    fsFromStart                 = 1,
    fsFromLEOF                  = 2,
    fsFromMark                  = 3
};

enum {
                                                                /* ioPosMode flags */
    pleaseCacheBit              = 4,                            /* please cache this request */
    pleaseCacheMask             = 0x0010,
    noCacheBit                  = 5,                            /* please don't cache this request */
    noCacheMask                 = 0x0020,
    rdVerifyBit                 = 6,                            /* read verify mode */
    rdVerifyMask                = 0x0040,
    rdVerify                    = 64,                           /* old name of rdVerifyMask */
    forceReadBit                = 6,
    forceReadMask               = 0x0040,
    newLineBit                  = 7,                            /* newline mode */
    newLineMask                 = 0x0080,
    newLineCharMask             = 0xFF00                        /* newline character */
};


enum {
                                                                /* CatSearch Search bitmask Constants */
    fsSBPartialName             = 1,
    fsSBFullName                = 2,
    fsSBFlAttrib                = 4,
    fsSBFlFndrInfo              = 8,
    fsSBFlLgLen                 = 32,
    fsSBFlPyLen                 = 64,
    fsSBFlRLgLen                = 128,
    fsSBFlRPyLen                = 256,
    fsSBFlCrDat                 = 512,
    fsSBFlMdDat                 = 1024,
    fsSBFlBkDat                 = 2048,
    fsSBFlXFndrInfo             = 4096,
    fsSBFlParID                 = 8192,
    fsSBNegate                  = 16384,
    fsSBDrUsrWds                = 8,
    fsSBDrNmFls                 = 16,
    fsSBDrCrDat                 = 512,
    fsSBDrMdDat                 = 1024,
    fsSBDrBkDat                 = 2048,
    fsSBDrFndrInfo              = 4096,
    fsSBDrParID                 = 8192
};

enum {
                                                                /* CatSearch Search bit value Constants */
    fsSBPartialNameBit          = 0,                            /*ioFileName points to a substring*/
    fsSBFullNameBit             = 1,                            /*ioFileName points to a match string*/
    fsSBFlAttribBit             = 2,                            /*search includes file attributes*/
    fsSBFlFndrInfoBit           = 3,                            /*search includes finder info*/
    fsSBFlLgLenBit              = 5,                            /*search includes data logical length*/
    fsSBFlPyLenBit              = 6,                            /*search includes data physical length*/
    fsSBFlRLgLenBit             = 7,                            /*search includes resource logical length*/
    fsSBFlRPyLenBit             = 8,                            /*search includes resource physical length*/
    fsSBFlCrDatBit              = 9,                            /*search includes create date*/
    fsSBFlMdDatBit              = 10,                           /*search includes modification date*/
    fsSBFlBkDatBit              = 11,                           /*search includes backup date*/
    fsSBFlXFndrInfoBit          = 12,                           /*search includes extended finder info*/
    fsSBFlParIDBit              = 13,                           /*search includes file's parent ID*/
    fsSBNegateBit               = 14,                           /*return all non-matches*/
    fsSBDrUsrWdsBit             = 3,                            /*search includes directory finder info*/
    fsSBDrNmFlsBit              = 4,                            /*search includes directory valence*/
    fsSBDrCrDatBit              = 9,                            /*directory-named version of fsSBFlCrDatBit*/
    fsSBDrMdDatBit              = 10,                           /*directory-named version of fsSBFlMdDatBit*/
    fsSBDrBkDatBit              = 11,                           /*directory-named version of fsSBFlBkDatBit*/
    fsSBDrFndrInfoBit           = 12,                           /*directory-named version of fsSBFlXFndrInfoBit*/
    fsSBDrParIDBit              = 13                            /*directory-named version of fsSBFlParIDBit*/
};

enum {
                                                                /* vMAttrib (GetVolParms) bit position constants */
    bLimitFCBs                  = 31,
    bLocalWList                 = 30,
    bNoMiniFndr                 = 29,
    bNoVNEdit                   = 28,
    bNoLclSync                  = 27,
    bTrshOffLine                = 26,
    bNoSwitchTo                 = 25,
    bNoDeskItems                = 20,
    bNoBootBlks                 = 19,
    bAccessCntl                 = 18,
    bNoSysDir                   = 17,
    bHasExtFSVol                = 16,
    bHasOpenDeny                = 15,
    bHasCopyFile                = 14,
    bHasMoveRename              = 13,
    bHasDesktopMgr              = 12,
    bHasShortName               = 11,
    bHasFolderLock              = 10,
    bHasPersonalAccessPrivileges = 9,
    bHasUserGroupList           = 8,
    bHasCatSearch               = 7,
    bHasFileIDs                 = 6,
    bHasBTreeMgr                = 5,
    bHasBlankAccessPrivileges   = 4,
    bSupportsAsyncRequests      = 3,                            /* asynchronous requests to this volume are handled correctly at any time*/
    bSupportsTrashVolumeCache   = 2
};

enum {
                                                                /* vMExtendedAttributes (GetVolParms) bit position constants */
    bIsEjectable                = 0,                            /* volume is in an ejectable disk drive */
    bSupportsHFSPlusAPIs        = 1,                            /* volume supports HFS Plus APIs directly (not through compatibility layer) */
    bSupportsFSCatalogSearch    = 2,                            /* volume supports FSCatalogSearch */
    bSupportsFSExchangeObjects  = 3,                            /* volume supports FSExchangeObjects */
    bSupports2TBFiles           = 4,                            /* volume supports supports 2 terabyte files */
    bSupportsLongNames          = 5,                            /* volume supports file/directory/volume names longer than 31 characters */
    bSupportsMultiScriptNames   = 6,                            /* volume supports file/directory/volume names with characters from multiple script systems */
    bSupportsNamedForks         = 7,                            /* volume supports forks beyond the data and resource forks */
    bSupportsSubtreeIterators   = 8,                            /* volume supports recursive iterators not at the volume root */
    bL2PCanMapFileBlocks        = 9,                            /* volume supports Lg2Phys SPI correctly */
    bAllowCDiDataHandler        = 17                            /* allow QuickTime's CDi data handler to examine this volume */
};

enum {
                                                                /* Desktop Database icon Constants */
    kLargeIcon                  = 1,
    kLarge4BitIcon              = 2,
    kLarge8BitIcon              = 3,
    kSmallIcon                  = 4,
    kSmall4BitIcon              = 5,
    kSmall8BitIcon              = 6
};

enum {
    kLargeIconSize              = 256,
    kLarge4BitIconSize          = 512,
    kLarge8BitIconSize          = 1024,
    kSmallIconSize              = 64,
    kSmall4BitIconSize          = 128,
    kSmall8BitIconSize          = 256
};

enum {
                                                                /* Large Volume Constants */
    kWidePosOffsetBit           = 8,
    kUseWidePositioning         = (1 << kWidePosOffsetBit),
    kMaximumBlocksIn4GB         = 0x007FFFFF
};

enum {
                                                                /* Foreign Privilege Model Identifiers */
    fsUnixPriv                  = 1
};

enum {
                                                                /* Authentication Constants */
    kNoUserAuthentication       = 1,
    kPassword                   = 2,
    kEncryptPassword            = 3,
    kTwoWayEncryptPassword      = 6
};


/* mapping codes (ioObjType) for MapName & MapID */
enum {
    kOwnerID2Name               = 1,
    kGroupID2Name               = 2,
    kOwnerName2ID               = 3,
    kGroupName2ID               = 4,                            /* types of oj object to be returned (ioObjType) for _GetUGEntry */
    kReturnNextUser             = 1,
    kReturnNextGroup            = 2,
    kReturnNextUG               = 3
};

/* vcbFlags bits */
enum {
    kVCBFlagsIdleFlushBit       = 3,                            /* Set if volume should be flushed at idle time */
    kVCBFlagsIdleFlushMask      = 0x0008,
    kVCBFlagsHFSPlusAPIsBit     = 4,                            /* Set if volume implements HFS Plus APIs itself (not via emulation) */
    kVCBFlagsHFSPlusAPIsMask    = 0x0010,
    kVCBFlagsHardwareGoneBit    = 5,                            /* Set if disk driver returned a hardwareGoneErr to Read or Write */
    kVCBFlagsHardwareGoneMask   = 0x0020,
    kVCBFlagsVolumeDirtyBit     = 15,                           /* Set if volume information has changed since the last FlushVol */
    kVCBFlagsVolumeDirtyMask    = 0x8000
};

/* ioFlAttrib bits returned by PBGetCatInfo */
enum {
                                                                /* file and directory attributes in ioFlAttrib */
    kioFlAttribLockedBit        = 0,                            /* Set if file or directory is locked */
    kioFlAttribLockedMask       = 0x01,
    kioFlAttribResOpenBit       = 2,                            /* Set if resource fork is open */
    kioFlAttribResOpenMask      = 0x04,
    kioFlAttribDataOpenBit      = 3,                            /* Set if data fork is open */
    kioFlAttribDataOpenMask     = 0x08,
    kioFlAttribDirBit           = 4,                            /* Set if this is a directory */
    kioFlAttribDirMask          = 0x10,
    ioDirFlg                    = 4,                            /* Set if this is a directory (old name) */
    ioDirMask                   = 0x10,
    kioFlAttribCopyProtBit      = 6,                            /* Set if AppleShare server "copy-protects" the file */
    kioFlAttribCopyProtMask     = 0x40,
    kioFlAttribFileOpenBit      = 7,                            /* Set if file (either fork) is open */
    kioFlAttribFileOpenMask     = 0x80,                         /* ioFlAttrib for directories only */
    kioFlAttribInSharedBit      = 2,                            /* Set if the directory is within a shared area of the directory hierarchy */
    kioFlAttribInSharedMask     = 0x04,
    kioFlAttribMountedBit       = 3,                            /* Set if the directory is a share point that is mounted by some user */
    kioFlAttribMountedMask      = 0x08,
    kioFlAttribSharePointBit    = 5,                            /* Set if the directory is a share point */
    kioFlAttribSharePointMask   = 0x20
};

/* ioFCBFlags bits returned by PBGetFCBInfo */
enum {
    kioFCBWriteBit              = 8,                            /* Data can be written to this file */
    kioFCBWriteMask             = 0x0100,
    kioFCBResourceBit           = 9,                            /* This file is a resource fork */
    kioFCBResourceMask          = 0x0200,
    kioFCBWriteLockedBit        = 10,                           /* File has a locked byte range */
    kioFCBWriteLockedMask       = 0x0400,
    kioFCBLargeFileBit          = 11,                           /* File may grow beyond 2GB; cache uses file blocks, not bytes */
    kioFCBLargeFileMask         = 0x0800,
    kioFCBSharedWriteBit        = 12,                           /* File is open for shared write access */
    kioFCBSharedWriteMask       = 0x1000,
    kioFCBFileLockedBit         = 13,                           /* File is locked (write-protected) */
    kioFCBFileLockedMask        = 0x2000,
    kioFCBOwnClumpBit           = 14,                           /* File has clump size specified in FCB */
    kioFCBOwnClumpMask          = 0x4000,
    kioFCBModifiedBit           = 15,                           /* File has changed since it was last flushed */
    kioFCBModifiedMask          = 0x8000
};

/* ioACUser bits returned by PBGetCatInfo */
/* Note: you must clear ioACUser before calling PBGetCatInfo because some file systems do not use this field */
enum {
    kioACUserNoSeeFolderBit     = 0,                            /* Set if user does not have See Folder privileges */
    kioACUserNoSeeFolderMask    = 0x01,
    kioACUserNoSeeFilesBit      = 1,                            /* Set if user does not have See Files privileges */
    kioACUserNoSeeFilesMask     = 0x02,
    kioACUserNoMakeChangesBit   = 2,                            /* Set if user does not have Make Changes privileges */
    kioACUserNoMakeChangesMask  = 0x04,
    kioACUserNotOwnerBit        = 7,                            /* Set if user is not owner of the directory */
    kioACUserNotOwnerMask       = 0x80
};

/* Folder and File values of access privileges in ioACAccess */
enum {
    kioACAccessOwnerBit         = 31,                           /* User is owner of directory */
    kioACAccessOwnerMask        = (long)0x80000000,
    kioACAccessBlankAccessBit   = 28,                           /* Directory has blank access privileges */
    kioACAccessBlankAccessMask  = 0x10000000,
    kioACAccessUserWriteBit     = 26,                           /* User has write privileges */
    kioACAccessUserWriteMask    = 0x04000000,
    kioACAccessUserReadBit      = 25,                           /* User has read privileges */
    kioACAccessUserReadMask     = 0x02000000,
    kioACAccessUserSearchBit    = 24,                           /* User has search privileges */
    kioACAccessUserSearchMask   = 0x01000000,
    kioACAccessEveryoneWriteBit = 18,                           /* Everyone has write privileges */
    kioACAccessEveryoneWriteMask = 0x00040000,
    kioACAccessEveryoneReadBit  = 17,                           /* Everyone has read privileges */
    kioACAccessEveryoneReadMask = 0x00020000,
    kioACAccessEveryoneSearchBit = 16,                          /* Everyone has search privileges */
    kioACAccessEveryoneSearchMask = 0x00010000,
    kioACAccessGroupWriteBit    = 10,                           /* Group has write privileges */
    kioACAccessGroupWriteMask   = 0x00000400,
    kioACAccessGroupReadBit     = 9,                            /* Group has read privileges */
    kioACAccessGroupReadMask    = 0x00000200,
    kioACAccessGroupSearchBit   = 8,                            /* Group has search privileges */
    kioACAccessGroupSearchMask  = 0x00000100,
    kioACAccessOwnerWriteBit    = 2,                            /* Owner has write privileges */
    kioACAccessOwnerWriteMask   = 0x00000004,
    kioACAccessOwnerReadBit     = 1,                            /* Owner has read privileges */
    kioACAccessOwnerReadMask    = 0x00000002,
    kioACAccessOwnerSearchBit   = 0,                            /* Owner has search privileges */
    kioACAccessOwnerSearchMask  = 0x00000001,
    kfullPrivileges             = 0x00070007,                   /* all privileges for everybody and owner*/
    kownerPrivileges            = 0x00000007                    /* all privileges for owner only*/
};

/* values of user IDs and group IDs */
enum {
    knoUser                     = 0,
    kadministratorUser          = 1
};

enum {
    knoGroup                    = 0
};



struct GetVolParmsInfoBuffer {
    short                           vMVersion;                  /*version number*/
    long                            vMAttrib;                   /*bit vector of attributes (see vMAttrib constants)*/
    Handle                          vMLocalHand;                /*handle to private data*/
    long                            vMServerAdr;                /*AppleTalk server address or zero*/
                                                                /*       vMVersion 1 GetVolParmsInfoBuffer ends here */
    long                            vMVolumeGrade;              /*approx. speed rating or zero if unrated*/
    short                           vMForeignPrivID;            /*foreign privilege model supported or zero if none*/
                                                                /*       vMVersion 2 GetVolParmsInfoBuffer ends here */
    long                            vMExtendedAttributes;       /*extended attribute bits (see vMExtendedAttributes constants)*/
                                                                /*       vMVersion 3 GetVolParmsInfoBuffer ends here */
};
typedef struct GetVolParmsInfoBuffer    GetVolParmsInfoBuffer;
typedef union ParamBlockRec             ParamBlockRec;

typedef ParamBlockRec *                 ParmBlkPtr;
typedef CALLBACK_API( void , IOCompletionProcPtr )(ParmBlkPtr paramBlock);
/*
    WARNING: IOCompletionProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(IOCompletionProcPtr)                  IOCompletionUPP;

struct IOParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioRefNum;                   /*refNum for I/O operation*/
    SInt8                           ioVersNum;                  /*version number*/
    SInt8                           ioPermssn;                  /*Open: permissions (byte)*/
    Ptr                             ioMisc;                     /*Rename: new name (GetEOF,SetEOF: logical end of file) (Open: optional ptr to buffer) (SetFileType: new type)*/
    Ptr                             ioBuffer;                   /*data buffer Ptr*/
    long                            ioReqCount;                 /*requested byte count; also = ioNewDirID*/
    long                            ioActCount;                 /*actual byte count completed*/
    short                           ioPosMode;                  /*initial file positioning*/
    long                            ioPosOffset;                /*file position offset*/
};
typedef struct IOParam                  IOParam;
typedef IOParam *                       IOParamPtr;

struct FileParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioFRefNum;                  /*reference number*/
    SInt8                           ioFVersNum;                 /*version number*/
    SInt8                           filler1;
    short                           ioFDirIndex;                /*GetFInfo directory index*/
    SInt8                           ioFlAttrib;                 /*GetFInfo: in-use bit=7, lock bit=0*/
    SInt8                           ioFlVersNum;                /*file version number*/
    FInfo                           ioFlFndrInfo;               /*user info*/
    unsigned long                   ioFlNum;                    /*GetFInfo: file number; TF- ioDirID*/
    unsigned short                  ioFlStBlk;                  /*start file block (0 if none)*/
    long                            ioFlLgLen;                  /*logical length (EOF)*/
    long                            ioFlPyLen;                  /*physical length*/
    unsigned short                  ioFlRStBlk;                 /*start block rsrc fork*/
    long                            ioFlRLgLen;                 /*file logical length rsrc fork*/
    long                            ioFlRPyLen;                 /*file physical length rsrc fork*/
    unsigned long                   ioFlCrDat;                  /*file creation date& time (32 bits in secs)*/
    unsigned long                   ioFlMdDat;                  /*last modified date and time*/
};
typedef struct FileParam                FileParam;
typedef FileParam *                     FileParamPtr;

struct VolumeParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    long                            filler2;
    short                           ioVolIndex;                 /*volume index number*/
    unsigned long                   ioVCrDate;                  /*creation date and time*/
    unsigned long                   ioVLsBkUp;                  /*last backup date and time*/
    unsigned short                  ioVAtrb;                    /*volume attrib*/
    unsigned short                  ioVNmFls;                   /*number of files in directory*/
    unsigned short                  ioVDirSt;                   /*start block of file directory*/
    short                           ioVBlLn;                    /*GetVolInfo: length of dir in blocks*/
    unsigned short                  ioVNmAlBlks;                /*for compatibilty ioVNmAlBlks * ioVAlBlkSiz <= 2 GB*/
    unsigned long                   ioVAlBlkSiz;                /*for compatibilty ioVAlBlkSiz is <= $0000FE00 (65,024)*/
    unsigned long                   ioVClpSiz;                  /*GetVolInfo: bytes to allocate at a time*/
    unsigned short                  ioAlBlSt;                   /*starting disk(512-byte) block in block map*/
    unsigned long                   ioVNxtFNum;                 /*GetVolInfo: next free file number*/
    unsigned short                  ioVFrBlk;                   /*GetVolInfo: # free alloc blks for this vol*/
};
typedef struct VolumeParam              VolumeParam;
typedef VolumeParam *                   VolumeParamPtr;

struct CntrlParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioCRefNum;                  /*refNum for I/O operation*/
    short                           csCode;                     /*word for control status code*/
    short                           csParam[11];                /*operation-defined parameters*/
};
typedef struct CntrlParam               CntrlParam;
typedef CntrlParam *                    CntrlParamPtr;

struct SlotDevParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioSRefNum;
    SInt8                           ioSVersNum;
    SInt8                           ioSPermssn;
    Ptr                             ioSMix;
    short                           ioSFlags;
    SInt8                           ioSlot;
    SInt8                           ioID;
};
typedef struct SlotDevParam             SlotDevParam;
typedef SlotDevParam *                  SlotDevParamPtr;

struct MultiDevParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioMRefNum;
    SInt8                           ioMVersNum;
    SInt8                           ioMPermssn;
    Ptr                             ioMMix;
    short                           ioMFlags;
    Ptr                             ioSEBlkPtr;
};
typedef struct MultiDevParam            MultiDevParam;
typedef MultiDevParam *                 MultiDevParamPtr;

union ParamBlockRec {
    IOParam                         ioParam;
    FileParam                       fileParam;
    VolumeParam                     volumeParam;
    CntrlParam                      cntrlParam;
    SlotDevParam                    slotDevParam;
    MultiDevParam                   multiDevParam;
};


struct HFileInfo {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioFRefNum;
    SInt8                           ioFVersNum;
    SInt8                           filler1;
    short                           ioFDirIndex;
    SInt8                           ioFlAttrib;
    SInt8                           ioACUser;
    FInfo                           ioFlFndrInfo;
    long                            ioDirID;
    unsigned short                  ioFlStBlk;
    long                            ioFlLgLen;
    long                            ioFlPyLen;
    unsigned short                  ioFlRStBlk;
    long                            ioFlRLgLen;
    long                            ioFlRPyLen;
    unsigned long                   ioFlCrDat;
    unsigned long                   ioFlMdDat;
    unsigned long                   ioFlBkDat;
    FXInfo                          ioFlXFndrInfo;
    long                            ioFlParID;
    long                            ioFlClpSiz;
};
typedef struct HFileInfo                HFileInfo;

struct DirInfo {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioFRefNum;
    SInt8                           ioFVersNum;
    SInt8                           filler1;
    short                           ioFDirIndex;
    SInt8                           ioFlAttrib;
    SInt8                           ioACUser;
    DInfo                           ioDrUsrWds;
    long                            ioDrDirID;
    unsigned short                  ioDrNmFls;
    short                           filler3[9];
    unsigned long                   ioDrCrDat;
    unsigned long                   ioDrMdDat;
    unsigned long                   ioDrBkDat;
    DXInfo                          ioDrFndrInfo;
    long                            ioDrParID;
};
typedef struct DirInfo                  DirInfo;

union CInfoPBRec {
    HFileInfo                       hFileInfo;
    DirInfo                         dirInfo;
};
typedef union CInfoPBRec                CInfoPBRec;

typedef CInfoPBRec *                    CInfoPBPtr;

struct XCInfoPBRec {
    QElemPtr                        qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    ProcPtr                         ioCompletion;               /* --> A pointer to a completion routine */
    volatile OSErr                  ioResult;                   /* --> The result code of the function */
    StringPtr                       ioNamePtr;                  /* --> Pointer to pathname to object */
    short                           ioVRefNum;                  /* --> A volume specification */
    long                            filler1;
    StringPtr                       ioShortNamePtr;             /* <-> A pointer to the short name string buffer - required! */
    short                           filler2;
    short                           ioPDType;                   /* <-- The ProDOS file type */
    long                            ioPDAuxType;                /* <-- The ProDOS aux type */
    long                            filler3[2];
    long                            ioDirID;                    /* --> A directory ID */
};
typedef struct XCInfoPBRec              XCInfoPBRec;

typedef XCInfoPBRec *                   XCInfoPBPtr;
/* Catalog position record */

struct CatPositionRec {
    long                            initialize;
    short                           priv[6];
};
typedef struct CatPositionRec           CatPositionRec;

struct FSSpec {
    short                           vRefNum;
    long                            parID;
    StrFileName                     name;                       /* a Str63 on MacOS*/
};
typedef struct FSSpec                   FSSpec;
typedef FSSpec *                        FSSpecPtr;
typedef FSSpecPtr *                     FSSpecHandle;
/* pointer to array of FSSpecs */
typedef FSSpecPtr                       FSSpecArrayPtr;
/* 
    The only difference between "const FSSpec*" and "ConstFSSpecPtr" is 
    that as a parameter, ConstFSSpecPtr is allowed to be NULL 
*/

typedef const FSSpec *                  ConstFSSpecPtr;
/* 
    The following are structures to be filled out with the _PBGetVolMountInfo call
    and passed back into the _PBVolumeMount call for external file system mounts. 
*/
/* the "signature" of the file system */
typedef OSType                          VolumeType;
enum {
                                                                /* the signature for AppleShare */
    AppleShareMediaType         = FOUR_CHAR_CODE('afpm')
};

/*
    VolMount stuff was once in FSM.*
*/

struct VolMountInfoHeader {
    short                           length;                     /* length of location data (including self) */
    VolumeType                      media;                      /* type of media.  Variable length data follows */
};
typedef struct VolMountInfoHeader       VolMountInfoHeader;

typedef VolMountInfoHeader *            VolMountInfoPtr;
/* The new volume mount info record.  The old one is included for compatibility. 
    the new record allows access by foriegn filesystems writers to the flags 
    portion of the record. This portion is now public.  
*/

struct VolumeMountInfoHeader {
    short                           length;                     /* length of location data (including self) */
    VolumeType                      media;                      /* type of media (must be registered with Apple) */
    short                           flags;                      /* volume mount flags. Variable length data follows */
};
typedef struct VolumeMountInfoHeader    VolumeMountInfoHeader;
typedef VolumeMountInfoHeader *         VolumeMountInfoHeaderPtr;
/* volume mount flags */
enum {
    volMountNoLoginMsgFlagBit   = 0,                            /* Input to VolumeMount: If set, the file system */
    volMountNoLoginMsgFlagMask  = 0x0001,                       /*  should suppresss any log-in message/greeting dialog */
    volMountExtendedFlagsBit    = 7,                            /* Input to VolumeMount: If set, the mount info is a */
    volMountExtendedFlagsMask   = 0x0080,                       /*  AFPXVolMountInfo record for 3.7 AppleShare Client */
    volMountInteractBit         = 15,                           /* Input to VolumeMount: If set, it's OK for the file system */
    volMountInteractMask        = 0x8000,                       /*  to perform user interaction to mount the volume */
    volMountChangedBit          = 14,                           /* Output from VoumeMount: If set, the volume was mounted, but */
    volMountChangedMask         = 0x4000,                       /*  the volume mounting information record needs to be updated. */
    volMountFSReservedMask      = 0x00FF,                       /* bits 0-7 are defined by each file system for its own use */
    volMountSysReservedMask     = 0xFF00                        /* bits 8-15 are reserved for Apple system use */
};




struct AFPVolMountInfo {
    short                           length;                     /* length of location data (including self) */
    VolumeType                      media;                      /* type of media */
    short                           flags;                      /* bits for no messages, no reconnect */
    SInt8                           nbpInterval;                /* NBP Interval parameter (IM2, p.322) */
    SInt8                           nbpCount;                   /* NBP Interval parameter (IM2, p.322) */
    short                           uamType;                    /* User Authentication Method */
    short                           zoneNameOffset;             /* short positive offset from start of struct to Zone Name */
    short                           serverNameOffset;           /* offset to pascal Server Name string */
    short                           volNameOffset;              /* offset to pascal Volume Name string */
    short                           userNameOffset;             /* offset to pascal User Name string */
    short                           userPasswordOffset;         /* offset to pascal User Password string */
    short                           volPasswordOffset;          /* offset to pascal Volume Password string */
    char                            AFPData[144];               /* variable length data may follow */
};
typedef struct AFPVolMountInfo          AFPVolMountInfo;
typedef AFPVolMountInfo *               AFPVolMountInfoPtr;


/* AFPXVolMountInfo is the new AFP volume mount info record, requires the 3.7 AppleShare Client */

struct AFPXVolMountInfo {
    short                           length;                     /* length of location data (including self) */
    VolumeType                      media;                      /* type of media */
    short                           flags;                      /* bits for no messages, no reconnect */
    SInt8                           nbpInterval;                /* NBP Interval parameter (IM2, p.322) */
    SInt8                           nbpCount;                   /* NBP Interval parameter (IM2, p.322) */
    short                           uamType;                    /* User Authentication Method type */
    short                           zoneNameOffset;             /* short positive offset from start of struct to Zone Name */
    short                           serverNameOffset;           /* offset to pascal Server Name string */
    short                           volNameOffset;              /* offset to pascal Volume Name string */
    short                           userNameOffset;             /* offset to pascal User Name string */
    short                           userPasswordOffset;         /* offset to pascal User Password string */
    short                           volPasswordOffset;          /* offset to pascal Volume Password string */
    short                           extendedFlags;              /* extended flags word */
    short                           uamNameOffset;              /* offset to a pascal UAM name string */
    short                           alternateAddressOffset;     /* offset to Alternate Addresses in tagged format */
    char                            AFPData[176];               /* variable length data may follow */
};
typedef struct AFPXVolMountInfo         AFPXVolMountInfo;
typedef AFPXVolMountInfo *              AFPXVolMountInfoPtr;
enum {
    kAFPExtendedFlagsAlternateAddressMask = 1                   /*  bit in AFPXVolMountInfo.extendedFlags that means alternateAddressOffset is used*/
};


enum {
                                                                /* constants for use in AFPTagData.fType field*/
    kAFPTagTypeIP               = 0x01,                         /* 4 byte IP address (MSB first)            */
    kAFPTagTypeIPPort           = 0x02,                         /* 4 byte IP address, 2 byte port (MSB first)     */
    kAFPTagTypeDDP              = 0x03,                         /* Net,Node,Socket Sent by the server, currently unused by the client */
    kAFPTagTypeDNS              = 0x04                          /* DNS name in  address:port format   (total length variable up to 254 chars of dns name)          */
};


enum {
                                                                /* constants for use in AFPTagData.fLength field*/
    kAFPTagLengthIP             = 0x06,
    kAFPTagLengthIPPort         = 0x08,
    kAFPTagLengthDDP            = 0x06
};


struct AFPTagData {
    UInt8                           fLength;                    /* length of this data tag including the fLength field */
    UInt8                           fType;
    UInt8                           fData[1];                   /* variable length data */
};
typedef struct AFPTagData               AFPTagData;

struct AFPAlternateAddress {
                                                                /* ooo.NOTE: fVersion was missing in 3.2 Universal Interfaces*/
    UInt8                           fVersion;                   /* version of the structure (currently 0x00)*/
    UInt8                           fAddressCount;
    UInt8                           fAddressList[1];            /* actually variable length packed set of AFPTagData */
};
typedef struct AFPAlternateAddress      AFPAlternateAddress;

struct DTPBRec {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioDTRefNum;                 /* desktop refnum */
    short                           ioIndex;
    long                            ioTagInfo;
    Ptr                             ioDTBuffer;
    long                            ioDTReqCount;
    long                            ioDTActCount;
    SInt8                           ioFiller1;
    SInt8                           ioIconType;
    short                           ioFiller2;
    long                            ioDirID;
    OSType                          ioFileCreator;
    OSType                          ioFileType;
    long                            ioFiller3;
    long                            ioDTLgLen;
    long                            ioDTPyLen;
    short                           ioFiller4[14];
    long                            ioAPPLParID;
};
typedef struct DTPBRec                  DTPBRec;

typedef DTPBRec *                       DTPBPtr;


struct HIOParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioRefNum;
    SInt8                           ioVersNum;
    SInt8                           ioPermssn;
    Ptr                             ioMisc;
    Ptr                             ioBuffer;
    long                            ioReqCount;
    long                            ioActCount;
    short                           ioPosMode;
    long                            ioPosOffset;
};
typedef struct HIOParam                 HIOParam;
typedef HIOParam *                      HIOParamPtr;

struct HFileParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioFRefNum;
    SInt8                           ioFVersNum;
    SInt8                           filler1;
    short                           ioFDirIndex;
    SInt8                           ioFlAttrib;
    SInt8                           ioFlVersNum;
    FInfo                           ioFlFndrInfo;
    long                            ioDirID;
    unsigned short                  ioFlStBlk;
    long                            ioFlLgLen;
    long                            ioFlPyLen;
    unsigned short                  ioFlRStBlk;
    long                            ioFlRLgLen;
    long                            ioFlRPyLen;
    unsigned long                   ioFlCrDat;
    unsigned long                   ioFlMdDat;
};
typedef struct HFileParam               HFileParam;
typedef HFileParam *                    HFileParamPtr;

struct HVolumeParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    long                            filler2;
    short                           ioVolIndex;
    unsigned long                   ioVCrDate;
    unsigned long                   ioVLsMod;
    short                           ioVAtrb;
    unsigned short                  ioVNmFls;
    unsigned short                  ioVBitMap;
    unsigned short                  ioAllocPtr;
    unsigned short                  ioVNmAlBlks;
    unsigned long                   ioVAlBlkSiz;
    unsigned long                   ioVClpSiz;
    unsigned short                  ioAlBlSt;
    unsigned long                   ioVNxtCNID;
    unsigned short                  ioVFrBlk;
    unsigned short                  ioVSigWord;
    short                           ioVDrvInfo;
    short                           ioVDRefNum;
    short                           ioVFSID;
    unsigned long                   ioVBkUp;
    short                           ioVSeqNum;
    unsigned long                   ioVWrCnt;
    unsigned long                   ioVFilCnt;
    unsigned long                   ioVDirCnt;
    long                            ioVFndrInfo[8];
};
typedef struct HVolumeParam             HVolumeParam;
typedef HVolumeParam *                  HVolumeParamPtr;

struct XIOParam {
    QElemPtr                        qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    IOCompletionUPP                 ioCompletion;
    volatile OSErr                  ioResult;
    StringPtr                       ioNamePtr;
    short                           ioVRefNum;
    short                           ioRefNum;
    SInt8                           ioVersNum;
    SInt8                           ioPermssn;
    Ptr                             ioMisc;
    Ptr                             ioBuffer;
    long                            ioReqCount;
    long                            ioActCount;
    short                           ioPosMode;                  /* must have kUseWidePositioning bit set */
    wide                            ioWPosOffset;               /* wide positioning offset */
};
typedef struct XIOParam                 XIOParam;
typedef XIOParam *                      XIOParamPtr;

struct XVolumeParam {
    QElemPtr                        qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    IOCompletionUPP                 ioCompletion;
    volatile OSErr                  ioResult;
    StringPtr                       ioNamePtr;
    short                           ioVRefNum;
    unsigned long                   ioXVersion;                 /* this XVolumeParam version (0) */
    short                           ioVolIndex;
    unsigned long                   ioVCrDate;
    unsigned long                   ioVLsMod;
    short                           ioVAtrb;
    unsigned short                  ioVNmFls;
    unsigned short                  ioVBitMap;
    unsigned short                  ioAllocPtr;
    unsigned short                  ioVNmAlBlks;
    unsigned long                   ioVAlBlkSiz;
    unsigned long                   ioVClpSiz;
    unsigned short                  ioAlBlSt;
    unsigned long                   ioVNxtCNID;
    unsigned short                  ioVFrBlk;
    unsigned short                  ioVSigWord;
    short                           ioVDrvInfo;
    short                           ioVDRefNum;
    short                           ioVFSID;
    unsigned long                   ioVBkUp;
    short                           ioVSeqNum;
    unsigned long                   ioVWrCnt;
    unsigned long                   ioVFilCnt;
    unsigned long                   ioVDirCnt;
    long                            ioVFndrInfo[8];
    UInt64                          ioVTotalBytes;              /* total number of bytes on volume */
    UInt64                          ioVFreeBytes;               /* number of free bytes on volume */
};
typedef struct XVolumeParam             XVolumeParam;
typedef XVolumeParam *                  XVolumeParamPtr;

struct AccessParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           filler3;
    short                           ioDenyModes;                /*access rights data*/
    short                           filler4;
    SInt8                           filler5;
    SInt8                           ioACUser;                   /*access rights for directory only*/
    long                            filler6;
    long                            ioACOwnerID;                /*owner ID*/
    long                            ioACGroupID;                /*group ID*/
    long                            ioACAccess;                 /*access rights*/
    long                            ioDirID;
};
typedef struct AccessParam              AccessParam;
typedef AccessParam *                   AccessParamPtr;

struct ObjParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           filler7;
    short                           ioObjType;                  /*function code*/
    StringPtr                       ioObjNamePtr;               /*ptr to returned creator/group name*/
    long                            ioObjID;                    /*creator/group ID*/
};
typedef struct ObjParam                 ObjParam;
typedef ObjParam *                      ObjParamPtr;

struct CopyParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           ioDstVRefNum;               /*destination vol identifier*/
    short                           filler8;
    StringPtr                       ioNewName;                  /*ptr to destination pathname*/
    StringPtr                       ioCopyName;                 /*ptr to optional name*/
    long                            ioNewDirID;                 /*destination directory ID*/
    long                            filler14;
    long                            filler15;
    long                            ioDirID;
};
typedef struct CopyParam                CopyParam;
typedef CopyParam *                     CopyParamPtr;

struct WDParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    short                           filler9;
    short                           ioWDIndex;
    long                            ioWDProcID;
    short                           ioWDVRefNum;
    short                           filler10;
    long                            filler11;
    long                            filler12;
    long                            filler13;
    long                            ioWDDirID;
};
typedef struct WDParam                  WDParam;
typedef WDParam *                       WDParamPtr;

struct FIDParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    long                            filler14;
    StringPtr                       ioDestNamePtr;              /* dest file name */
    long                            filler15;
    long                            ioDestDirID;                /* dest file's directory id */
    long                            filler16;
    long                            filler17;
    long                            ioSrcDirID;                 /* source file's directory id */
    short                           filler18;
    long                            ioFileID;                   /* file ID */
};
typedef struct FIDParam                 FIDParam;
typedef FIDParam *                      FIDParamPtr;

struct ForeignPrivParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    long                            ioFiller21;
    long                            ioFiller22;
    Ptr                             ioForeignPrivBuffer;
    long                            ioForeignPrivActCount;
    long                            ioForeignPrivReqCount;
    long                            ioFiller23;
    long                            ioForeignPrivDirID;
    long                            ioForeignPrivInfo1;
    long                            ioForeignPrivInfo2;
    long                            ioForeignPrivInfo3;
    long                            ioForeignPrivInfo4;
};
typedef struct ForeignPrivParam         ForeignPrivParam;
typedef ForeignPrivParam *              ForeignPrivParamPtr;

struct CSParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/
    FSSpecPtr                       ioMatchPtr;                 /* match array */
    long                            ioReqMatchCount;            /* maximum allowable matches */
    long                            ioActMatchCount;            /* actual match count */
    long                            ioSearchBits;               /* search criteria selector */
    CInfoPBPtr                      ioSearchInfo1;              /* search values and range lower bounds */
    CInfoPBPtr                      ioSearchInfo2;              /* search values and range upper bounds */
    long                            ioSearchTime;               /* length of time to run search */
    CatPositionRec                  ioCatPosition;              /* current position in the catalog */
    Ptr                             ioOptBuffer;                /* optional performance enhancement buffer */
    long                            ioOptBufSize;               /* size of buffer pointed to by ioOptBuffer */
};
typedef struct CSParam                  CSParam;
typedef CSParam *                       CSParamPtr;

union HParamBlockRec {
    HIOParam                        ioParam;
    HFileParam                      fileParam;
    HVolumeParam                    volumeParam;
    AccessParam                     accessParam;
    ObjParam                        objParam;
    CopyParam                       copyParam;
    WDParam                         wdParam;
    FIDParam                        fidParam;
    CSParam                         csParam;
    ForeignPrivParam                foreignPrivParam;
};
typedef union HParamBlockRec            HParamBlockRec;



typedef HParamBlockRec *                HParmBlkPtr;


struct CMovePBRec {
    QElemPtr                        qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    IOCompletionUPP                 ioCompletion;
    volatile OSErr                  ioResult;
    StringPtr                       ioNamePtr;
    short                           ioVRefNum;
    long                            filler1;
    StringPtr                       ioNewName;
    long                            filler2;
    long                            ioNewDirID;
    long                            filler3[2];
    long                            ioDirID;
};
typedef struct CMovePBRec               CMovePBRec;

typedef CMovePBRec *                    CMovePBPtr;

struct WDPBRec {
    QElemPtr                        qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    IOCompletionUPP                 ioCompletion;
    volatile OSErr                  ioResult;
    StringPtr                       ioNamePtr;
    short                           ioVRefNum;
    short                           filler1;
    short                           ioWDIndex;
    long                            ioWDProcID;
    short                           ioWDVRefNum;
    short                           filler2[7];
    long                            ioWDDirID;
};
typedef struct WDPBRec                  WDPBRec;

typedef WDPBRec *                       WDPBPtr;

struct FCBPBRec {
    QElemPtr                        qLink;
    short                           qType;
    short                           ioTrap;
    Ptr                             ioCmdAddr;
    IOCompletionUPP                 ioCompletion;
    volatile OSErr                  ioResult;
    StringPtr                       ioNamePtr;
    short                           ioVRefNum;
    short                           ioRefNum;
    short                           filler;
    short                           ioFCBIndx;
    short                           filler1;
    long                            ioFCBFlNm;
    short                           ioFCBFlags;
    unsigned short                  ioFCBStBlk;
    long                            ioFCBEOF;
    long                            ioFCBPLen;
    long                            ioFCBCrPs;
    short                           ioFCBVRefNum;
    long                            ioFCBClpSiz;
    long                            ioFCBParID;
};
typedef struct FCBPBRec                 FCBPBRec;

typedef FCBPBRec *                      FCBPBPtr;

struct VCB {
    QElemPtr                        qLink;
    short                           qType;
    short                           vcbFlags;
    unsigned short                  vcbSigWord;
    unsigned long                   vcbCrDate;
    unsigned long                   vcbLsMod;
    short                           vcbAtrb;
    unsigned short                  vcbNmFls;
    short                           vcbVBMSt;
    short                           vcbAllocPtr;
    unsigned short                  vcbNmAlBlks;
    long                            vcbAlBlkSiz;
    long                            vcbClpSiz;
    short                           vcbAlBlSt;
    long                            vcbNxtCNID;
    unsigned short                  vcbFreeBks;
    Str27                           vcbVN;
    short                           vcbDrvNum;
    short                           vcbDRefNum;
    short                           vcbFSID;
    short                           vcbVRefNum;
    Ptr                             vcbMAdr;
    Ptr                             vcbBufAdr;
    short                           vcbMLen;
    short                           vcbDirIndex;
    short                           vcbDirBlk;
    unsigned long                   vcbVolBkUp;
    unsigned short                  vcbVSeqNum;
    long                            vcbWrCnt;
    long                            vcbXTClpSiz;
    long                            vcbCTClpSiz;
    unsigned short                  vcbNmRtDirs;
    long                            vcbFilCnt;
    long                            vcbDirCnt;
    long                            vcbFndrInfo[8];
    unsigned short                  vcbVCSize;
    unsigned short                  vcbVBMCSiz;
    unsigned short                  vcbCtlCSiz;
    unsigned short                  vcbXTAlBlks;
    unsigned short                  vcbCTAlBlks;
    short                           vcbXTRef;
    short                           vcbCTRef;
    Ptr                             vcbCtlBuf;
    long                            vcbDirIDM;
    short                           vcbOffsM;
};
typedef struct VCB                      VCB;
typedef VCB *                           VCBPtr;

struct DrvQEl {
    QElemPtr                        qLink;
    short                           qType;
    short                           dQDrive;
    short                           dQRefNum;
    short                           dQFSID;
    unsigned short                  dQDrvSz;
    unsigned short                  dQDrvSz2;
};
typedef struct DrvQEl                   DrvQEl;
typedef DrvQEl *                        DrvQElPtr;
#if OPAQUE_UPP_TYPES
    EXTERN_API(IOCompletionUPP)
    NewIOCompletionUPP             (IOCompletionProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeIOCompletionUPP         (IOCompletionUPP         userUPP);

    EXTERN_API(void)
    InvokeIOCompletionUPP          (ParmBlkPtr              paramBlock,
                                    IOCompletionUPP         userUPP);

#else
    enum { uppIOCompletionProcInfo = 0x00009802 };                  /* register no_return_value Func(4_bytes:A0) */
    #define NewIOCompletionUPP(userRoutine)                         (IOCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIOCompletionProcInfo, GetCurrentArchitecture())
    #define DisposeIOCompletionUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeIOCompletionUPP(__A0, __A1)
    void InvokeIOCompletionUPP(ParmBlkPtr paramBlock, IOCompletionUPP userUPP) = 0x4E91;
    #else
        #define InvokeIOCompletionUPP(paramBlock, userUPP)              CALL_ONE_PARAMETER_UPP((userUPP), uppIOCompletionProcInfo, (paramBlock))
    #endif
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewIOCompletionProc(userRoutine)                        NewIOCompletionUPP(userRoutine)
#define CallIOCompletionProc(userRoutine, paramBlock)           InvokeIOCompletionUPP(paramBlock, userRoutine)

/*
   PBOpenSync(), PBOpenAsync(), PBOpenImmed() were moved to Devices.h
   PBCloseSync(), PBCloseAsync(), PBCloseImmed() were moved to Devices.h
   PBReadSync(), PBReadAsync(), PBReadImmed() were moved to Devices.h
   PBWriteSync(), PBWriteAsync(), PBWriteImmed() were moved to Devices.h
*/


#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetVInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetVInfoSync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA007);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetVInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetVInfoAsync(ParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA407);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBXGetVolInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBXGetVolInfoSync(XVolumeParamPtr paramBlock)                           TWOWORDINLINE(0x7012, 0xA060);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBXGetVolInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBXGetVolInfoAsync(XVolumeParamPtr paramBlock)                          TWOWORDINLINE(0x7012, 0xA460);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetVolSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetVolSync(ParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA014);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetVolAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetVolAsync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA414);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetVolSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetVolSync(ParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA015);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetVolAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetVolAsync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA415);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBFlushVolSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBFlushVolSync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA013);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBFlushVolAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBFlushVolAsync(ParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA413);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHTrashVolumeCachesSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHTrashVolumeCachesSync(ParmBlkPtr paramBlock)                         ONEWORDINLINE(0xA213);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCreateSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCreateSync(ParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA008);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCreateAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCreateAsync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA408);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDeleteSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDeleteSync(ParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA009);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDeleteAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDeleteAsync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA409);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenDFSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBOpenDFSync(ParmBlkPtr paramBlock)                                     TWOWORDINLINE(0x701A, 0xA060);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenDFAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBOpenDFAsync(ParmBlkPtr paramBlock)                                    TWOWORDINLINE(0x701A, 0xA460);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenRFSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBOpenRFSync(ParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA00A);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenRFAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBOpenRFAsync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA40A);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBRenameSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBRenameSync(ParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA00B);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBRenameAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBRenameAsync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA40B);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetFInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetFInfoSync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA00C);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetFInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetFInfoAsync(ParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA40C);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetFInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetFInfoSync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA00D);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetFInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetFInfoAsync(ParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA40D);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetFLockSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetFLockSync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA041);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetFLockAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetFLockAsync(ParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA441);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBRstFLockSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBRstFLockSync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA042);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBRstFLockAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBRstFLockAsync(ParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA442);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetFVersSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetFVersSync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA043);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetFVersAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetFVersAsync(ParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA443);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBAllocateSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBAllocateSync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA010);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBAllocateAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBAllocateAsync(ParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA410);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetEOFSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetEOFSync(ParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA011);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetEOFAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetEOFAsync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA411);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetEOFSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetEOFSync(ParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA012);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetEOFAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetEOFAsync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA412);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetFPosSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetFPosSync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA018);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetFPosAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetFPosAsync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA418);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetFPosSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetFPosSync(ParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA044);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetFPosAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetFPosAsync(ParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA444);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBFlushFileSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBFlushFileSync(ParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA045);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBFlushFileAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBFlushFileAsync(ParmBlkPtr paramBlock)                                 ONEWORDINLINE(0xA445);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBMountVol(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBMountVol(ParmBlkPtr paramBlock)                                       ONEWORDINLINE(0xA00F);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBUnmountVol(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBUnmountVol(ParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA00E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBUnmountVolImmed(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBUnmountVolImmed(ParmBlkPtr paramBlock)                                ONEWORDINLINE(0xA20E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBEject(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBEject(ParmBlkPtr paramBlock)                                          ONEWORDINLINE(0xA017);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOffLine(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBOffLine(ParmBlkPtr paramBlock)                                        ONEWORDINLINE(0xA035);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCatSearchSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCatSearchSync(CSParamPtr paramBlock)                                  TWOWORDINLINE(0x7018, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCatSearchAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCatSearchAsync(CSParamPtr paramBlock)                                 TWOWORDINLINE(0x7018, 0xA660);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
SetVol                          (ConstStr63Param        volName, /* can be NULL */
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( OSErr )
UnmountVol                      (ConstStr63Param        volName, /* can be NULL */
                                 short                  vRefNum);

EXTERN_API( OSErr )
Eject                           (ConstStr63Param        volName, /* can be NULL */
                                 short                  vRefNum);

EXTERN_API( OSErr )
FlushVol                        (ConstStr63Param        volName, /* can be NULL */
                                 short                  vRefNum);

EXTERN_API( OSErr )
HSetVol                         (ConstStr63Param        volName, /* can be NULL */
                                 short                  vRefNum,
                                 long                   dirID);

/* AddDrive() was moved to Devices.h*/

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
FSOpen                          (ConstStr255Param       fileName,
                                 short                  vRefNum,
                                 short *                refNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OpenDF                          (ConstStr255Param       fileName,
                                 short                  vRefNum,
                                 short *                refNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( OSErr )
FSClose                         (short                  refNum);

EXTERN_API( OSErr )
FSRead                          (short                  refNum,
                                 long *                 count,
                                 void *                 buffPtr);

EXTERN_API( OSErr )
FSWrite                         (short                  refNum,
                                 long *                 count,
                                 const void *           buffPtr);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
GetVInfo                        (short                  drvNum,
                                 StringPtr              volName,
                                 short *                vRefNum,
                                 long *                 freeBytes);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
GetFInfo                        (ConstStr255Param       fileName,
                                 short                  vRefNum,
                                 FInfo *                fndrInfo);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
GetVol                          (StringPtr              volName,
                                 short *                vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
Create                          (ConstStr255Param       fileName,
                                 short                  vRefNum,
                                 OSType                 creator,
                                 OSType                 fileType);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
FSDelete                        (ConstStr255Param       fileName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OpenRF                          (ConstStr255Param       fileName,
                                 short                  vRefNum,
                                 short *                refNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
Rename                          (ConstStr255Param       oldName,
                                 short                  vRefNum,
                                 ConstStr255Param       newName);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
SetFInfo                        (ConstStr255Param       fileName,
                                 short                  vRefNum,
                                 const FInfo *          fndrInfo);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
SetFLock                        (ConstStr255Param       fileName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
RstFLock                        (ConstStr255Param       fileName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( OSErr )
Allocate                        (short                  refNum,
                                 long *                 count);

EXTERN_API( OSErr )
GetEOF                          (short                  refNum,
                                 long *                 logEOF);

EXTERN_API( OSErr )
SetEOF                          (short                  refNum,
                                 long                   logEOF);

EXTERN_API( OSErr )
GetFPos                         (short                  refNum,
                                 long *                 filePos);

EXTERN_API( OSErr )
SetFPos                         (short                  refNum,
                                 short                  posMode,
                                 long                   posOff);

EXTERN_API( OSErr )
GetVRefNum                      (short                  fileRefNum,
                                 short *                vRefNum);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
fsopen                          (const char *           fileName,
                                 short                  vRefNum,
                                 short *                refNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
getvinfo                        (short                  drvNum,
                                 char *                 volName,
                                 short *                vRefNum,
                                 long *                 freeBytes);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
getfinfo                        (const char *           fileName,
                                 short                  vRefNum,
                                 FInfo *                fndrInfo);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
getvol                          (char *                 volName,
                                 short *                vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
setvol                          (const char *           volName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
unmountvol                      (const char *           volName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
eject                           (const char *           volName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
flushvol                        (const char *           volName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
create                          (const char *           fileName,
                                 short                  vRefNum,
                                 OSType                 creator,
                                 OSType                 fileType);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
fsdelete                        (const char *           fileName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
openrf                          (const char *           fileName,
                                 short                  vRefNum,
                                 short *                refNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
fsrename                        (const char *           oldName,
                                 short                  vRefNum,
                                 const char *           newName);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
setfinfo                        (const char *           fileName,
                                 short                  vRefNum,
                                 const FInfo *          fndrInfo);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
setflock                        (const char *           fileName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSErr )
rstflock                        (const char *           fileName,
                                 short                  vRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenWDSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBOpenWDSync(WDPBPtr paramBlock)                                        TWOWORDINLINE(0x7001, 0xA260);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenWDAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBOpenWDAsync(WDPBPtr paramBlock)                                       TWOWORDINLINE(0x7001, 0xA660);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCloseWDSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCloseWDSync(WDPBPtr paramBlock)                                       TWOWORDINLINE(0x7002, 0xA260);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCloseWDAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCloseWDAsync(WDPBPtr paramBlock)                                      TWOWORDINLINE(0x7002, 0xA660);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHSetVolSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHSetVolSync(WDPBPtr paramBlock)                                       ONEWORDINLINE(0xA215);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHSetVolAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHSetVolAsync(WDPBPtr paramBlock)                                      ONEWORDINLINE(0xA615);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetVolSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetVolSync(WDPBPtr paramBlock)                                       ONEWORDINLINE(0xA214);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetVolAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetVolAsync(WDPBPtr paramBlock)                                      ONEWORDINLINE(0xA614);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCatMoveSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCatMoveSync(CMovePBPtr paramBlock)                                    TWOWORDINLINE(0x7005, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCatMoveAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCatMoveAsync(CMovePBPtr paramBlock)                                   TWOWORDINLINE(0x7005, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDirCreateSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDirCreateSync(HParmBlkPtr paramBlock)                                 TWOWORDINLINE(0x7006, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDirCreateAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDirCreateAsync(HParmBlkPtr paramBlock)                                TWOWORDINLINE(0x7006, 0xA660);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetWDInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetWDInfoSync(WDPBPtr paramBlock)                                     TWOWORDINLINE(0x7007, 0xA260);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetWDInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetWDInfoAsync(WDPBPtr paramBlock)                                    TWOWORDINLINE(0x7007, 0xA660);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetFCBInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetFCBInfoSync(FCBPBPtr paramBlock)                                   TWOWORDINLINE(0x7008, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetFCBInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetFCBInfoAsync(FCBPBPtr paramBlock)                                  TWOWORDINLINE(0x7008, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetCatInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetCatInfoSync(CInfoPBPtr paramBlock)                                 TWOWORDINLINE(0x7009, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetCatInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetCatInfoAsync(CInfoPBPtr paramBlock)                                TWOWORDINLINE(0x7009, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetCatInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetCatInfoSync(CInfoPBPtr paramBlock)                                 TWOWORDINLINE(0x700A, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetCatInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetCatInfoAsync(CInfoPBPtr paramBlock)                                TWOWORDINLINE(0x700A, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBAllocContigSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBAllocContigSync(ParmBlkPtr paramBlock)                                ONEWORDINLINE(0xA210);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBAllocContigAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBAllocContigAsync(ParmBlkPtr paramBlock)                               ONEWORDINLINE(0xA610);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBLockRangeSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBLockRangeSync(ParmBlkPtr paramBlock)                                  TWOWORDINLINE(0x7010, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBLockRangeAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBLockRangeAsync(ParmBlkPtr paramBlock)                                 TWOWORDINLINE(0x7010, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBUnlockRangeSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBUnlockRangeSync(ParmBlkPtr paramBlock)                                TWOWORDINLINE(0x7011, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBUnlockRangeAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBUnlockRangeAsync(ParmBlkPtr paramBlock)                               TWOWORDINLINE(0x7011, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetVInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetVInfoSync(HParmBlkPtr paramBlock)                                  TWOWORDINLINE(0x700B, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetVInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetVInfoAsync(HParmBlkPtr paramBlock)                                 TWOWORDINLINE(0x700B, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetVInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetVInfoSync(HParmBlkPtr paramBlock)                                 ONEWORDINLINE(0xA207);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetVInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetVInfoAsync(HParmBlkPtr paramBlock)                                ONEWORDINLINE(0xA607);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenSync(HParmBlkPtr paramBlock)                                     ONEWORDINLINE(0xA200);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenAsync(HParmBlkPtr paramBlock)                                    ONEWORDINLINE(0xA600);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenRFSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenRFSync(HParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA20A);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenRFAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenRFAsync(HParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA60A);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenDFSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenDFSync(HParmBlkPtr paramBlock)                                   TWOWORDINLINE(0x701A, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenDFAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenDFAsync(HParmBlkPtr paramBlock)                                  TWOWORDINLINE(0x701A, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHCreateSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHCreateSync(HParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA208);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHCreateAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHCreateAsync(HParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA608);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHDeleteSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHDeleteSync(HParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA209);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHDeleteAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHDeleteAsync(HParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA609);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHRenameSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHRenameSync(HParmBlkPtr paramBlock)                                   ONEWORDINLINE(0xA20B);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHRenameAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHRenameAsync(HParmBlkPtr paramBlock)                                  ONEWORDINLINE(0xA60B);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHRstFLockSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHRstFLockSync(HParmBlkPtr paramBlock)                                 ONEWORDINLINE(0xA242);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHRstFLockAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHRstFLockAsync(HParmBlkPtr paramBlock)                                ONEWORDINLINE(0xA642);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHSetFLockSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHSetFLockSync(HParmBlkPtr paramBlock)                                 ONEWORDINLINE(0xA241);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHSetFLockAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHSetFLockAsync(HParmBlkPtr paramBlock)                                ONEWORDINLINE(0xA641);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetFInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetFInfoSync(HParmBlkPtr paramBlock)                                 ONEWORDINLINE(0xA20C);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetFInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetFInfoAsync(HParmBlkPtr paramBlock)                                ONEWORDINLINE(0xA60C);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHSetFInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHSetFInfoSync(HParmBlkPtr paramBlock)                                 ONEWORDINLINE(0xA20D);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHSetFInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHSetFInfoAsync(HParmBlkPtr paramBlock)                                ONEWORDINLINE(0xA60D);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBMakeFSSpecSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBMakeFSSpecSync(HParmBlkPtr paramBlock)                                TWOWORDINLINE(0x701B, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBMakeFSSpecAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBMakeFSSpecAsync(HParmBlkPtr paramBlock)                               TWOWORDINLINE(0x701B, 0xA660);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( void ) FInitQueue(void )                                                             ONEWORDINLINE(0xA016);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( QHdrPtr )
GetFSQHdr                       (void)                                                      THREEWORDINLINE(0x2EBC, 0x0000, 0x0360);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( QHdrPtr )
GetVCBQHdr                      (void)                                                      THREEWORDINLINE(0x2EBC, 0x0000, 0x0356);

/* GetDrvQHdr was moved to Devices.h*/

EXTERN_API( OSErr )
HGetVol                         (StringPtr              volName,
                                 short *                vRefNum,
                                 long *                 dirID);

EXTERN_API( OSErr )
HOpen                           (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName,
                                 SInt8                  permission,
                                 short *                refNum);

EXTERN_API( OSErr )
HOpenDF                         (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName,
                                 SInt8                  permission,
                                 short *                refNum);

EXTERN_API( OSErr )
HOpenRF                         (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName,
                                 SInt8                  permission,
                                 short *                refNum);

EXTERN_API( OSErr )
AllocContig                     (short                  refNum,
                                 long *                 count);

EXTERN_API( OSErr )
HCreate                         (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName,
                                 OSType                 creator,
                                 OSType                 fileType);

EXTERN_API( OSErr )
DirCreate                       (short                  vRefNum,
                                 long                   parentDirID,
                                 ConstStr255Param       directoryName,
                                 long *                 createdDirID);

EXTERN_API( OSErr )
HDelete                         (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName);

EXTERN_API( OSErr )
HGetFInfo                       (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName,
                                 FInfo *                fndrInfo);

EXTERN_API( OSErr )
HSetFInfo                       (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName,
                                 const FInfo *          fndrInfo);

EXTERN_API( OSErr )
HSetFLock                       (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName);

EXTERN_API( OSErr )
HRstFLock                       (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName);

EXTERN_API( OSErr )
HRename                         (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       oldName,
                                 ConstStr255Param       newName);

EXTERN_API( OSErr )
CatMove                         (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       oldName,
                                 long                   newDirID,
                                 ConstStr255Param       newName);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OpenWD                          (short                  vRefNum,
                                 long                   dirID,
                                 long                   procID,
                                 short *                wdRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
CloseWD                         (short                  wdRefNum);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
GetWDInfo                       (short                  wdRefNum,
                                 short *                vRefNum,
                                 long *                 dirID,
                                 long *                 procID);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

/*  shared environment  */
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetVolParmsSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetVolParmsSync(HParmBlkPtr paramBlock)                              TWOWORDINLINE(0x7030, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetVolParmsAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetVolParmsAsync(HParmBlkPtr paramBlock)                             TWOWORDINLINE(0x7030, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetLogInInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetLogInInfoSync(HParmBlkPtr paramBlock)                             TWOWORDINLINE(0x7031, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetLogInInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetLogInInfoAsync(HParmBlkPtr paramBlock)                            TWOWORDINLINE(0x7031, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetDirAccessSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetDirAccessSync(HParmBlkPtr paramBlock)                             TWOWORDINLINE(0x7032, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHGetDirAccessAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHGetDirAccessAsync(HParmBlkPtr paramBlock)                            TWOWORDINLINE(0x7032, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHSetDirAccessSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHSetDirAccessSync(HParmBlkPtr paramBlock)                             TWOWORDINLINE(0x7033, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHSetDirAccessAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHSetDirAccessAsync(HParmBlkPtr paramBlock)                            TWOWORDINLINE(0x7033, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHMapIDSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHMapIDSync(HParmBlkPtr paramBlock)                                    TWOWORDINLINE(0x7034, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHMapIDAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHMapIDAsync(HParmBlkPtr paramBlock)                                   TWOWORDINLINE(0x7034, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHMapNameSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHMapNameSync(HParmBlkPtr paramBlock)                                  TWOWORDINLINE(0x7035, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHMapNameAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHMapNameAsync(HParmBlkPtr paramBlock)                                 TWOWORDINLINE(0x7035, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHCopyFileSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHCopyFileSync(HParmBlkPtr paramBlock)                                 TWOWORDINLINE(0x7036, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHCopyFileAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHCopyFileAsync(HParmBlkPtr paramBlock)                                TWOWORDINLINE(0x7036, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHMoveRenameSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHMoveRenameSync(HParmBlkPtr paramBlock)                               TWOWORDINLINE(0x7037, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHMoveRenameAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHMoveRenameAsync(HParmBlkPtr paramBlock)                              TWOWORDINLINE(0x7037, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenDenySync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenDenySync(HParmBlkPtr paramBlock)                                 TWOWORDINLINE(0x7038, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenDenyAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenDenyAsync(HParmBlkPtr paramBlock)                                TWOWORDINLINE(0x7038, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenRFDenySync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenRFDenySync(HParmBlkPtr paramBlock)                               TWOWORDINLINE(0x7039, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBHOpenRFDenyAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBHOpenRFDenyAsync(HParmBlkPtr paramBlock)                              TWOWORDINLINE(0x7039, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetXCatInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetXCatInfoSync(XCInfoPBPtr paramBlock)                               TWOWORDINLINE(0x703A, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetXCatInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetXCatInfoAsync(XCInfoPBPtr paramBlock)                              TWOWORDINLINE(0x703A, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBExchangeFilesSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBExchangeFilesSync(HParmBlkPtr paramBlock)                             TWOWORDINLINE(0x7017, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBExchangeFilesAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBExchangeFilesAsync(HParmBlkPtr paramBlock)                            TWOWORDINLINE(0x7017, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCreateFileIDRefSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCreateFileIDRefSync(HParmBlkPtr paramBlock)                           TWOWORDINLINE(0x7014, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCreateFileIDRefAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBCreateFileIDRefAsync(HParmBlkPtr paramBlock)                          TWOWORDINLINE(0x7014, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBResolveFileIDRefSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBResolveFileIDRefSync(HParmBlkPtr paramBlock)                          TWOWORDINLINE(0x7016, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBResolveFileIDRefAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBResolveFileIDRefAsync(HParmBlkPtr paramBlock)                         TWOWORDINLINE(0x7016, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDeleteFileIDRefSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDeleteFileIDRefSync(HParmBlkPtr paramBlock)                           TWOWORDINLINE(0x7015, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDeleteFileIDRefAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDeleteFileIDRefAsync(HParmBlkPtr paramBlock)                          TWOWORDINLINE(0x7015, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetForeignPrivsSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetForeignPrivsSync(HParmBlkPtr paramBlock)                           TWOWORDINLINE(0x7060, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetForeignPrivsAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetForeignPrivsAsync(HParmBlkPtr paramBlock)                          TWOWORDINLINE(0x7060, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetForeignPrivsSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetForeignPrivsSync(HParmBlkPtr paramBlock)                           TWOWORDINLINE(0x7061, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetForeignPrivsAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetForeignPrivsAsync(HParmBlkPtr paramBlock)                          TWOWORDINLINE(0x7061, 0xA660);

/*  Desktop Manager  */
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetPath(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetPath(DTPBPtr paramBlock)                                         TWOWORDINLINE(0x7020, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTCloseDown(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTCloseDown(DTPBPtr paramBlock)                                       TWOWORDINLINE(0x7021, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTAddIconSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTAddIconSync(DTPBPtr paramBlock)                                     TWOWORDINLINE(0x7022, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTAddIconAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTAddIconAsync(DTPBPtr paramBlock)                                    TWOWORDINLINE(0x7022, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetIconSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetIconSync(DTPBPtr paramBlock)                                     TWOWORDINLINE(0x7023, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetIconAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetIconAsync(DTPBPtr paramBlock)                                    TWOWORDINLINE(0x7023, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetIconInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetIconInfoSync(DTPBPtr paramBlock)                                 TWOWORDINLINE(0x7024, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetIconInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetIconInfoAsync(DTPBPtr paramBlock)                                TWOWORDINLINE(0x7024, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTAddAPPLSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTAddAPPLSync(DTPBPtr paramBlock)                                     TWOWORDINLINE(0x7025, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTAddAPPLAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTAddAPPLAsync(DTPBPtr paramBlock)                                    TWOWORDINLINE(0x7025, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTRemoveAPPLSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTRemoveAPPLSync(DTPBPtr paramBlock)                                  TWOWORDINLINE(0x7026, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTRemoveAPPLAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTRemoveAPPLAsync(DTPBPtr paramBlock)                                 TWOWORDINLINE(0x7026, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetAPPLSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetAPPLSync(DTPBPtr paramBlock)                                     TWOWORDINLINE(0x7027, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetAPPLAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetAPPLAsync(DTPBPtr paramBlock)                                    TWOWORDINLINE(0x7027, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTSetCommentSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTSetCommentSync(DTPBPtr paramBlock)                                  TWOWORDINLINE(0x7028, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTSetCommentAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTSetCommentAsync(DTPBPtr paramBlock)                                 TWOWORDINLINE(0x7028, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTRemoveCommentSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTRemoveCommentSync(DTPBPtr paramBlock)                               TWOWORDINLINE(0x7029, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTRemoveCommentAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTRemoveCommentAsync(DTPBPtr paramBlock)                              TWOWORDINLINE(0x7029, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetCommentSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetCommentSync(DTPBPtr paramBlock)                                  TWOWORDINLINE(0x702A, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetCommentAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetCommentAsync(DTPBPtr paramBlock)                                 TWOWORDINLINE(0x702A, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTFlushSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTFlushSync(DTPBPtr paramBlock)                                       TWOWORDINLINE(0x702B, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTFlushAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTFlushAsync(DTPBPtr paramBlock)                                      TWOWORDINLINE(0x702B, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTResetSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTResetSync(DTPBPtr paramBlock)                                       TWOWORDINLINE(0x702C, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTResetAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTResetAsync(DTPBPtr paramBlock)                                      TWOWORDINLINE(0x702C, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetInfoSync(DTPBPtr paramBlock)                                     TWOWORDINLINE(0x702D, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTGetInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTGetInfoAsync(DTPBPtr paramBlock)                                    TWOWORDINLINE(0x702D, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTOpenInform(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTOpenInform(DTPBPtr paramBlock)                                      TWOWORDINLINE(0x702E, 0xA060);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTDeleteSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTDeleteSync(DTPBPtr paramBlock)                                      TWOWORDINLINE(0x702F, 0xA060);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDTDeleteAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBDTDeleteAsync(DTPBPtr paramBlock)                                     TWOWORDINLINE(0x702F, 0xA460);

/*  VolumeMount traps  */
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetVolMountInfoSize(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetVolMountInfoSize(ParmBlkPtr paramBlock)                            TWOWORDINLINE(0x703F, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetVolMountInfo(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetVolMountInfo(ParmBlkPtr paramBlock)                                TWOWORDINLINE(0x7040, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBVolumeMount(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBVolumeMount(ParmBlkPtr paramBlock)                                    TWOWORDINLINE(0x7041, 0xA260);

/*  FSp traps  */
EXTERN_API( OSErr )
FSMakeFSSpec                    (short                  vRefNum,
                                 long                   dirID,
                                 ConstStr255Param       fileName,
                                 FSSpec *               spec)                               TWOWORDINLINE(0x7001, 0xAA52);

EXTERN_API( OSErr )
FSpOpenDF                       (const FSSpec *         spec,
                                 SInt8                  permission,
                                 short *                refNum)                             TWOWORDINLINE(0x7002, 0xAA52);

EXTERN_API( OSErr )
FSpOpenRF                       (const FSSpec *         spec,
                                 SInt8                  permission,
                                 short *                refNum)                             TWOWORDINLINE(0x7003, 0xAA52);

EXTERN_API( OSErr )
FSpCreate                       (const FSSpec *         spec,
                                 OSType                 creator,
                                 OSType                 fileType,
                                 ScriptCode             scriptTag)                          TWOWORDINLINE(0x7004, 0xAA52);

EXTERN_API( OSErr )
FSpDirCreate                    (const FSSpec *         spec,
                                 ScriptCode             scriptTag,
                                 long *                 createdDirID)                       TWOWORDINLINE(0x7005, 0xAA52);

EXTERN_API( OSErr )
FSpDelete                       (const FSSpec *         spec)                               TWOWORDINLINE(0x7006, 0xAA52);

EXTERN_API( OSErr )
FSpGetFInfo                     (const FSSpec *         spec,
                                 FInfo *                fndrInfo)                           TWOWORDINLINE(0x7007, 0xAA52);

EXTERN_API( OSErr )
FSpSetFInfo                     (const FSSpec *         spec,
                                 const FInfo *          fndrInfo)                           TWOWORDINLINE(0x7008, 0xAA52);

EXTERN_API( OSErr )
FSpSetFLock                     (const FSSpec *         spec)                               TWOWORDINLINE(0x7009, 0xAA52);

EXTERN_API( OSErr )
FSpRstFLock                     (const FSSpec *         spec)                               TWOWORDINLINE(0x700A, 0xAA52);

EXTERN_API( OSErr )
FSpRename                       (const FSSpec *         spec,
                                 ConstStr255Param       newName)                            TWOWORDINLINE(0x700B, 0xAA52);

EXTERN_API( OSErr )
FSpCatMove                      (const FSSpec *         source,
                                 const FSSpec *         dest)                               TWOWORDINLINE(0x700C, 0xAA52);

EXTERN_API( OSErr )
FSpExchangeFiles                (const FSSpec *         source,
                                 const FSSpec *         dest)                               TWOWORDINLINE(0x700F, 0xAA52);


                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBShareSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBShareSync(HParmBlkPtr paramBlock)                                     TWOWORDINLINE(0x7042, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBShareAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBShareAsync(HParmBlkPtr paramBlock)                                    TWOWORDINLINE(0x7042, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBUnshareSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBUnshareSync(HParmBlkPtr paramBlock)                                   TWOWORDINLINE(0x7043, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBUnshareAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBUnshareAsync(HParmBlkPtr paramBlock)                                  TWOWORDINLINE(0x7043, 0xA660);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetUGEntrySync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetUGEntrySync(HParmBlkPtr paramBlock)                                TWOWORDINLINE(0x7044, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetUGEntryAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetUGEntryAsync(HParmBlkPtr paramBlock)                               TWOWORDINLINE(0x7044, 0xA660);




#if TARGET_CPU_68K
/*
    PBGetAltAccess and PBSetAltAccess are obsolete and will not be supported 
    on PowerPC. Equivalent functionality is provided by the routines 
    PBGetForeignPrivs and PBSetForeignPrivs.
*/
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetAltAccessSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetAltAccessSync(HParmBlkPtr paramBlock)                              TWOWORDINLINE(0x7060, 0xA060);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetAltAccessAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBGetAltAccessAsync(HParmBlkPtr paramBlock)                             TWOWORDINLINE(0x7060, 0xA460);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetAltAccessSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetAltAccessSync(HParmBlkPtr paramBlock)                              TWOWORDINLINE(0x7061, 0xA060);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetAltAccessAsync(__A0)
                                                                                            #endif
EXTERN_API( OSErr ) PBSetAltAccessAsync(HParmBlkPtr paramBlock)                             TWOWORDINLINE(0x7061, 0xA460);

#endif  /* CALL_NOT_IN_CARBON */

#define PBSetAltAccess(pb, async) ((async) ? PBSetAltAccessAsync(pb) : PBSetAltAccessSync(pb))
#define PBGetAltAccess(pb, async) ((async) ? PBGetAltAccessAsync(pb) : PBGetAltAccessSync(pb))
#endif  /* TARGET_CPU_68K */


/*
    The PBxxx() routines are obsolete.  
    
    Use the PBxxxSync() or PBxxxAsync() version instead.
*/
#define PBGetVInfo(pb, async) ((async) ? PBGetVInfoAsync(pb) : PBGetVInfoSync(pb))
#define PBXGetVolInfo(pb, async) ((async) ? PBXGetVolInfoAsync(pb) : PBXGetVolInfoSync(pb))
#define PBGetVol(pb, async) ((async) ? PBGetVolAsync(pb) : PBGetVolSync(pb))
#define PBSetVol(pb, async) ((async) ? PBSetVolAsync(pb) : PBSetVolSync(pb))
#define PBFlushVol(pb, async) ((async) ? PBFlushVolAsync(pb) : PBFlushVolSync(pb))
#define PBCreate(pb, async) ((async) ? PBCreateAsync(pb) : PBCreateSync(pb))
#define PBDelete(pb, async) ((async) ? PBDeleteAsync(pb) : PBDeleteSync(pb))
#define PBOpenDF(pb, async) ((async) ? PBOpenDFAsync(pb) : PBOpenDFSync(pb))
#define PBOpenRF(pb, async) ((async) ? PBOpenRFAsync(pb) : PBOpenRFSync(pb))
#define PBRename(pb, async) ((async) ? PBRenameAsync(pb) : PBRenameSync(pb))
#define PBGetFInfo(pb, async) ((async) ? PBGetFInfoAsync(pb) : PBGetFInfoSync(pb))
#define PBSetFInfo(pb, async) ((async) ? PBSetFInfoAsync(pb) : PBSetFInfoSync(pb))
#define PBSetFLock(pb, async) ((async) ? PBSetFLockAsync(pb) : PBSetFLockSync(pb))
#define PBRstFLock(pb, async) ((async) ? PBRstFLockAsync(pb) : PBRstFLockSync(pb))
#define PBSetFVers(pb, async) ((async) ? PBSetFVersAsync(pb) : PBSetFVersSync(pb))
#define PBAllocate(pb, async) ((async) ? PBAllocateAsync(pb) : PBAllocateSync(pb))
#define PBGetEOF(pb, async) ((async) ? PBGetEOFAsync(pb) : PBGetEOFSync(pb))
#define PBSetEOF(pb, async) ((async) ? PBSetEOFAsync(pb) : PBSetEOFSync(pb))
#define PBGetFPos(pb, async) ((async) ? PBGetFPosAsync(pb) : PBGetFPosSync(pb))
#define PBSetFPos(pb, async) ((async) ? PBSetFPosAsync(pb) : PBSetFPosSync(pb))
#define PBFlushFile(pb, async) ((async) ? PBFlushFileAsync(pb) : PBFlushFileSync(pb))
#define PBCatSearch(pb, async) ((async) ? PBCatSearchAsync(pb) : PBCatSearchSync(pb))
#define PBOpenWD(pb, async) ((async) ? PBOpenWDAsync(pb) : PBOpenWDSync(pb))
#define PBCloseWD(pb, async) ((async) ? PBCloseWDAsync(pb) : PBCloseWDSync(pb))
#define PBHSetVol(pb, async) ((async) ? PBHSetVolAsync(pb) : PBHSetVolSync(pb))
#define PBHGetVol(pb, async) ((async) ? PBHGetVolAsync(pb) : PBHGetVolSync(pb))
#define PBCatMove(pb, async) ((async) ? PBCatMoveAsync(pb) : PBCatMoveSync(pb))
#define PBDirCreate(pb, async) ((async) ? PBDirCreateAsync(pb) : PBDirCreateSync(pb))
#define PBGetWDInfo(pb, async) ((async) ? PBGetWDInfoAsync(pb) : PBGetWDInfoSync(pb))
#define PBGetFCBInfo(pb, async) ((async) ? PBGetFCBInfoAsync(pb) : PBGetFCBInfoSync(pb))
#define PBGetCatInfo(pb, async) ((async) ? PBGetCatInfoAsync(pb) : PBGetCatInfoSync(pb))
#define PBSetCatInfo(pb, async) ((async) ? PBSetCatInfoAsync(pb) : PBSetCatInfoSync(pb))
#define PBAllocContig(pb, async) ((async) ? PBAllocContigAsync(pb) : PBAllocContigSync(pb))
#define PBLockRange(pb, async) ((async) ? PBLockRangeAsync(pb) : PBLockRangeSync(pb))
#define PBUnlockRange(pb, async) ((async) ? PBUnlockRangeAsync(pb) : PBUnlockRangeSync(pb))
#define PBSetVInfo(pb, async) ((async) ? PBSetVInfoAsync(pb) : PBSetVInfoSync(pb))
#define PBHGetVInfo(pb, async) ((async) ? PBHGetVInfoAsync(pb) : PBHGetVInfoSync(pb))
#define PBHOpen(pb, async) ((async) ? PBHOpenAsync(pb) : PBHOpenSync(pb))
#define PBHOpenRF(pb, async) ((async) ? PBHOpenRFAsync(pb) : PBHOpenRFSync(pb))
#define PBHOpenDF(pb, async) ((async) ? PBHOpenDFAsync(pb) : PBHOpenDFSync(pb))
#define PBHCreate(pb, async) ((async) ? PBHCreateAsync(pb) : PBHCreateSync(pb))
#define PBHDelete(pb, async) ((async) ? PBHDeleteAsync(pb) : PBHDeleteSync(pb))
#define PBHRename(pb, async) ((async) ? PBHRenameAsync(pb) : PBHRenameSync(pb))
#define PBHRstFLock(pb, async) ((async) ? PBHRstFLockAsync(pb) : PBHRstFLockSync(pb))
#define PBHSetFLock(pb, async) ((async) ? PBHSetFLockAsync(pb) : PBHSetFLockSync(pb))
#define PBHGetFInfo(pb, async) ((async) ? PBHGetFInfoAsync(pb) : PBHGetFInfoSync(pb))
#define PBHSetFInfo(pb, async) ((async) ? PBHSetFInfoAsync(pb) : PBHSetFInfoSync(pb))
#define PBMakeFSSpec(pb, async) ((async) ? PBMakeFSSpecAsync(pb) : PBMakeFSSpecSync(pb))
#define PBHGetVolParms(pb, async) ((async) ? PBHGetVolParmsAsync(pb) : PBHGetVolParmsSync(pb))
#define PBHGetLogInInfo(pb, async) ((async) ? PBHGetLogInInfoAsync(pb) : PBHGetLogInInfoSync(pb))
#define PBHGetDirAccess(pb, async) ((async) ? PBHGetDirAccessAsync(pb) : PBHGetDirAccessSync(pb))
#define PBHSetDirAccess(pb, async) ((async) ? PBHSetDirAccessAsync(pb) : PBHSetDirAccessSync(pb))
#define PBHMapID(pb, async) ((async) ? PBHMapIDAsync(pb) : PBHMapIDSync(pb))
#define PBHMapName(pb, async) ((async) ? PBHMapNameAsync(pb) : PBHMapNameSync(pb))
#define PBHCopyFile(pb, async) ((async) ? PBHCopyFileAsync(pb) : PBHCopyFileSync(pb))
#define PBHMoveRename(pb, async) ((async) ? PBHMoveRenameAsync(pb) : PBHMoveRenameSync(pb))
#define PBHOpenDeny(pb, async) ((async) ? PBHOpenDenyAsync(pb) : PBHOpenDenySync(pb))
#define PBHOpenRFDeny(pb, async) ((async) ? PBHOpenRFDenyAsync(pb) : PBHOpenRFDenySync(pb))
#define PBExchangeFiles(pb, async) ((async) ? PBExchangeFilesAsync(pb) : PBExchangeFilesSync(pb))
#define PBCreateFileIDRef(pb, async) ((async) ? PBCreateFileIDRefAsync(pb) : PBCreateFileIDRefSync(pb))
#define PBResolveFileIDRef(pb, async) ((async) ? PBResolveFileIDRefAsync(pb) : PBResolveFileIDRefSync(pb))
#define PBDeleteFileIDRef(pb, async) ((async) ? PBDeleteFileIDRefAsync(pb) : PBDeleteFileIDRefSync(pb))
#define PBGetForeignPrivs(pb, async) ((async) ? PBGetForeignPrivsAsync(pb) : PBGetForeignPrivsSync(pb))
#define PBSetForeignPrivs(pb, async) ((async) ? PBSetForeignPrivsAsync(pb) : PBSetForeignPrivsSync(pb))
#define PBDTAddIcon(pb, async) ((async) ? PBDTAddIconAsync(pb) : PBDTAddIconSync(pb))
#define PBDTGetIcon(pb, async) ((async) ? PBDTGetIconAsync(pb) : PBDTGetIconSync(pb))
#define PBDTGetIconInfo(pb, async) ((async) ? PBDTGetIconInfoAsync(pb) : PBDTGetIconInfoSync(pb))
#define PBDTAddAPPL(pb, async) ((async) ? PBDTAddAPPLAsync(pb) : PBDTAddAPPLSync(pb))
#define PBDTRemoveAPPL(pb, async) ((async) ? PBDTRemoveAPPLAsync(pb) : PBDTRemoveAPPLSync(pb))
#define PBDTGetAPPL(pb, async) ((async) ? PBDTGetAPPLAsync(pb) : PBDTGetAPPLSync(pb))
#define PBDTSetComment(pb, async) ((async) ? PBDTSetCommentAsync(pb) : PBDTSetCommentSync(pb))
#define PBDTRemoveComment(pb, async) ((async) ? PBDTRemoveCommentAsync(pb) : PBDTRemoveCommentSync(pb))
#define PBDTGetComment(pb, async) ((async) ? PBDTGetCommentAsync(pb) : PBDTGetCommentSync(pb))
#define PBDTFlush(pb, async) ((async) ? PBDTFlushAsync(pb) : PBDTFlushSync(pb))
#define PBDTReset(pb, async) ((async) ? PBDTResetAsync(pb) : PBDTResetSync(pb))
#define PBDTGetInfo(pb, async) ((async) ? PBDTGetInfoAsync(pb) : PBDTGetInfoSync(pb))
#define PBDTDelete(pb, async) ((async) ? PBDTDeleteAsync(pb) : PBDTDeleteSync(pb))


typedef SInt16                          FSVolumeRefNum;
enum {
    kFSInvalidVolumeRefNum      = 0
};

#if TARGET_OS_WIN32

struct FSRef {
    UInt8                           hidden[600];                /* private to File Manager; oo need symbolic constant */
};
typedef struct FSRef                    FSRef;
#else

struct FSRef {
    UInt8                           hidden[80];                 /* private to File Manager; oo need symbolic constant */
};
typedef struct FSRef                    FSRef;
#endif  /* TARGET_OS_WIN32 */

typedef FSRef *                         FSRefPtr;
/*  CatalogInfoBitmap describes which fields of the CatalogInfo you wish to get or set.*/

typedef UInt32                          FSCatalogInfoBitmap;
enum {
    kFSCatInfoNone              = 0x00000000,
    kFSCatInfoTextEncoding      = 0x00000001,
    kFSCatInfoNodeFlags         = 0x00000002,                   /* Locked (bit 0) and directory (bit 4) only */
    kFSCatInfoVolume            = 0x00000004,
    kFSCatInfoParentDirID       = 0x00000008,
    kFSCatInfoNodeID            = 0x00000010,
    kFSCatInfoCreateDate        = 0x00000020,
    kFSCatInfoContentMod        = 0x00000040,
    kFSCatInfoAttrMod           = 0x00000080,
    kFSCatInfoAccessDate        = 0x00000100,
    kFSCatInfoBackupDate        = 0x00000200,
    kFSCatInfoPermissions       = 0x00000400,                   /* Should this be finer granularity? */
    kFSCatInfoFinderInfo        = 0x00000800,
    kFSCatInfoFinderXInfo       = 0x00001000,
    kFSCatInfoValence           = 0x00002000,                   /* Folders only, zero for files */
    kFSCatInfoDataSizes         = 0x00004000,                   /* Data fork logical and physical size */
    kFSCatInfoRsrcSizes         = 0x00008000,                   /* Resource fork logical and physical size */
    kFSCatInfoSharingFlags      = 0x00010000,                   /* sharingFlags: kioFlAttribMountedBit, kioFlAttribSharePointBit */
    kFSCatInfoUserPrivs         = 0x00020000,                   /* userPrivileges */
    kFSCatInfoAllDates          = 0x000003E0,
    kFSCatInfoGettableInfo      = 0x0003FFFF,
    kFSCatInfoSettableInfo      = 0x00001FE3,                   /* flags, dates, permissions, Finder info, text encoding */
    kFSCatInfoReserved          = (long)0xFFFC0000              /* bits that are currently reserved */
};

/*  Constants for nodeFlags field of FSCatalogInfo*/
enum {
    kFSNodeLockedBit            = 0,
    kFSNodeLockedMask           = 0x0001,
    kFSNodeIsDirectoryBit       = 4,
    kFSNodeIsDirectoryMask      = 0x0010
};


struct FSCatalogInfo {
    UInt16                          nodeFlags;                  /* node flags */
    FSVolumeRefNum                  volume;                     /* object's volume ref */
    UInt32                          parentDirID;                /* parent directory's ID */
    UInt32                          nodeID;                     /* file/directory ID */
    UInt8                           sharingFlags;               /* kioFlAttribMountedBit and kioFlAttribSharePointBit */
    UInt8                           userPrivileges;             /* user's effective AFP privileges (same as ioACUser) */
    UInt8                           reserved1;
    UInt8                           reserved2;
    UTCDateTime                     createDate;                 /* date and time of creation */
    UTCDateTime                     contentModDate;             /* date and time of last fork modification */
    UTCDateTime                     attributeModDate;           /* date and time of last attribute modification */
    UTCDateTime                     accessDate;                 /* date and time of last access (for Mac OS X) */
    UTCDateTime                     backupDate;                 /* date and time of last backup */

    UInt32                          permissions[4];             /* permissions (for Mac OS X) */

    UInt8                           finderInfo[16];             /* Finder information part 1 */
    UInt8                           extFinderInfo[16];          /* Finder information part 2 */

    UInt64                          dataLogicalSize;            /* files only */
    UInt64                          dataPhysicalSize;           /* files only */
    UInt64                          rsrcLogicalSize;            /* files only */
    UInt64                          rsrcPhysicalSize;           /* files only */

    UInt32                          valence;                    /* folders only */
    TextEncoding                    textEncodingHint;
};
typedef struct FSCatalogInfo            FSCatalogInfo;
typedef FSCatalogInfo *                 FSCatalogInfoPtr;

struct FSRefParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    ConstStringPtr                  ioNamePtr;                  /*ptr to Vol:FileName string*/
    short                           ioVRefNum;                  /*volume refnum (DrvNum for Eject and MountVol)*/

    SInt16                          reserved1;                  /* was ioRefNum */
    UInt8                           reserved2;                  /* was ioVersNum */
    UInt8                           reserved3;                  /* was ioPermssn */

    const FSRef *                   ref;                        /* Input ref; the target of the call */
    FSCatalogInfoBitmap             whichInfo;
    FSCatalogInfo *                 catInfo;
    UniCharCount                    nameLength;                 /* input name length for create/rename */
    const UniChar *                 name;                       /* input name for create/rename */
    long                            ioDirID;
    FSSpec *                        spec;
    FSRef *                         parentRef;                  /* ref of directory to move another ref to */
    FSRef *                         newRef;                     /* Output ref */
    TextEncoding                    textEncodingHint;           /* for Rename, MakeFSRefUnicode */
    HFSUniStr255 *                  outName;                    /* Output name for GetCatalogInfo */
};
typedef struct FSRefParam               FSRefParam;
typedef FSRefParam *                    FSRefParamPtr;
typedef struct OpaqueFSIterator*        FSIterator;
enum {
    kFSIterateFlat              = 0,                            /* Immediate children of container only */
    kFSIterateSubtree           = 1,                            /* Entire subtree rooted at container */
    kFSIterateReserved          = (long)0xFFFFFFFE
};


typedef OptionBits                      FSIteratorFlags;
enum {
                                                                /* CatalogSearch constants */
    fsSBNodeID                  = 0x00008000,                   /* search by range of nodeID */
    fsSBAttributeModDate        = 0x00010000,                   /* search by range of attributeModDate */
    fsSBAccessDate              = 0x00020000,                   /* search by range of accessDate */
    fsSBPermissions             = 0x00040000,                   /* search by value/mask of permissions */
    fsSBNodeIDBit               = 15,
    fsSBAttributeModDateBit     = 16,
    fsSBAccessDateBit           = 17,
    fsSBPermissionsBit          = 18
};


struct FSSearchParams {
    Duration                        searchTime;                 /* a Time Manager duration */
    OptionBits                      searchBits;                 /* which fields to search on */
    UniCharCount                    searchNameLength;
    const UniChar *                 searchName;
    FSCatalogInfo *                 searchInfo1;                /* values and lower bounds */
    FSCatalogInfo *                 searchInfo2;                /* masks and upper bounds */
};
typedef struct FSSearchParams           FSSearchParams;
typedef FSSearchParams *                FSSearchParamsPtr;

struct FSCatalogBulkParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    Boolean                         containerChanged;           /* true if container changed since last iteration */
    UInt8                           reserved;                   /* make following fields 4-byte aligned */

    FSIteratorFlags                 iteratorFlags;
    FSIterator                      iterator;
    const FSRef *                   container;                  /* directory/volume to iterate */
    ItemCount                       maximumItems;
    ItemCount                       actualItems;
    FSCatalogInfoBitmap             whichInfo;
    FSCatalogInfo *                 catalogInfo;                /* returns an array */
    FSRef *                         refs;                       /* returns an array */
    FSSpec *                        specs;                      /* returns an array */
    HFSUniStr255 *                  names;                      /* returns an array */
    const FSSearchParams *          searchParams;
};
typedef struct FSCatalogBulkParam       FSCatalogBulkParam;
typedef FSCatalogBulkParam *            FSCatalogBulkParamPtr;

typedef UInt16                          FSAllocationFlags;
enum {
    kFSAllocDefaultFlags        = 0x0000,                       /* as much as possible, not contiguous */
    kFSAllocAllOrNothingMask    = 0x0001,                       /* allocate all of the space, or nothing */
    kFSAllocContiguousMask      = 0x0002,                       /* new space must be one contiguous piece */
    kFSAllocNoRoundUpMask       = 0x0004,                       /* don't round up allocation to clump size */
    kFSAllocReservedMask        = 0xFFF8                        /* these bits are reserved and must not be set */
};


struct FSForkIOParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    void *                          reserved1;                  /* was ioNamePtr */
    SInt16                          reserved2;                  /* was ioVRefNum */
    SInt16                          forkRefNum;                 /* same as ioRefNum */
    UInt8                           reserved3;                  /* was ioVersNum */
    SInt8                           permissions;                /* desired access to the fork */
    const FSRef *                   ref;                        /* which object to open */


    Ptr                             buffer;                     /*data buffer Ptr*/
    UInt32                          requestCount;               /*requested byte count*/
    UInt32                          actualCount;                /*actual byte count completed*/
    UInt16                          positionMode;               /*initial file positioning*/
    SInt64                          positionOffset;             /*file position offset*/

    FSAllocationFlags               allocationFlags;
    UInt64                          allocationAmount;

    UniCharCount                    forkNameLength;             /* input; length of fork name */
    const UniChar *                 forkName;                   /* input; name of fork */

    CatPositionRec                  forkIterator;
    HFSUniStr255 *                  outForkName;                /* output; name of fork */
};
typedef struct FSForkIOParam            FSForkIOParam;
typedef FSForkIOParam *                 FSForkIOParamPtr;

struct FSForkInfo {
    SInt8                           flags;                      /* copy of FCB flags */
    SInt8                           permissions;
    FSVolumeRefNum                  volume;
    UInt32                          reserved2;
    UInt32                          nodeID;                     /* file or directory ID */
    UInt32                          forkID;                     /* fork ID */
    UInt64                          currentPosition;
    UInt64                          logicalEOF;
    UInt64                          physicalEOF;
    UInt64                          process;                    /* should be ProcessSerialNumber */
};
typedef struct FSForkInfo               FSForkInfo;
typedef FSForkInfo *                    FSForkInfoPtr;

struct FSForkCBInfoParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    SInt16                          desiredRefNum;              /* 0 to iterate, non-0 for specific refnum */
    SInt16                          volumeRefNum;               /* volume to match, or 0 for all volumes */
    SInt16                          iterator;                   /* 0 to start iteration */
    SInt16                          actualRefNum;               /* actual refnum found */

    FSRef *                         ref;
    FSForkInfo *                    forkInfo;
    HFSUniStr255 *                  forkName;
};
typedef struct FSForkCBInfoParam        FSForkCBInfoParam;
typedef FSForkCBInfoParam *             FSForkCBInfoParamPtr;

typedef UInt32                          FSVolumeInfoBitmap;
enum {
    kFSVolInfoNone              = 0x0000,
    kFSVolInfoCreateDate        = 0x0001,
    kFSVolInfoModDate           = 0x0002,
    kFSVolInfoBackupDate        = 0x0004,
    kFSVolInfoCheckedDate       = 0x0008,
    kFSVolInfoFileCount         = 0x0010,
    kFSVolInfoDirCount          = 0x0020,
    kFSVolInfoSizes             = 0x0040,                       /* totalBytes and freeBytes */
    kFSVolInfoBlocks            = 0x0080,                       /* blockSize, totalBlocks, freeBlocks */
    kFSVolInfoNextAlloc         = 0x0100,
    kFSVolInfoRsrcClump         = 0x0200,
    kFSVolInfoDataClump         = 0x0400,
    kFSVolInfoNextID            = 0x0800,
    kFSVolInfoFinderInfo        = 0x1000,
    kFSVolInfoFlags             = 0x2000,
    kFSVolInfoFSInfo            = 0x4000,                       /* filesystemID, signature */
    kFSVolInfoDriveInfo         = 0x8000,                       /* driveNumber, driverRefNum */
    kFSVolInfoGettableInfo      = 0xFFFF,                       /* This seems like it is here just for completeness */
    kFSVolInfoSettableInfo      = 0x3004                        /* backup date, Finder info, flags */
};



struct FSVolumeInfo {
                                                                /* Dates -- zero means "never" or "unknown" */
    UTCDateTime                     createDate;
    UTCDateTime                     modifyDate;
    UTCDateTime                     backupDate;
    UTCDateTime                     checkedDate;

                                                                /* File/Folder counts -- return zero if unknown */
    UInt32                          fileCount;                  /* total files on volume */
    UInt32                          folderCount;                /* total folders on volume */
                                                                /* Note: no root directory counts */

    UInt64                          totalBytes;                 /* total number of bytes on volume */
    UInt64                          freeBytes;                  /* number of free bytes on volume */

                                                                /* HFS and HFS Plus specific.  Set fields to zero if not appropriate */
    UInt32                          blockSize;                  /* size (in bytes) of allocation blocks */
    UInt32                          totalBlocks;                /* number of allocation blocks in volume */
    UInt32                          freeBlocks;                 /* number of unused allocation blocks */
    UInt32                          nextAllocation;             /* start of next allocation search */
    UInt32                          rsrcClumpSize;              /* default resource fork clump size */
    UInt32                          dataClumpSize;              /* default data fork clump size */
    UInt32                          nextCatalogID;              /* next unused catalog node ID ooo OYG ooo need to make HFSVolumes.h work Should be HFSCatalogNodeID*/
    UInt8                           finderInfo[32];             /* information used by Finder */

                                                                /* Identifying information */
    UInt16                          flags;                      /* ioVAtrb */
    UInt16                          filesystemID;               /* ioVFSID */
    UInt16                          signature;                  /* ioVSigWord, unique within an FSID */
    UInt16                          driveNumber;                /* ioVDrvInfo */
    short                           driverRefNum;               /* ioVDRefNum */
};
typedef struct FSVolumeInfo             FSVolumeInfo;
typedef FSVolumeInfo *                  FSVolumeInfoPtr;

struct FSVolumeInfoParam {
    QElemPtr                        qLink;                      /*queue link in header*/
    short                           qType;                      /*type byte for safety check*/
    short                           ioTrap;                     /*FS: the Trap*/
    Ptr                             ioCmdAddr;                  /*FS: address to dispatch to*/
    IOCompletionUPP                 ioCompletion;               /*completion routine addr (0 for synch calls)*/
    volatile OSErr                  ioResult;                   /*result code*/
    StringPtr                       ioNamePtr;                  /* unused */
    FSVolumeRefNum                  ioVRefNum;                  /* volume refnum */

    UInt32                          volumeIndex;                /* index, or 0 to use ioVRefNum */
    FSVolumeInfoBitmap              whichInfo;                  /* which volumeInfo fields to get/set */
    FSVolumeInfo *                  volumeInfo;                 /* information about the volume */
    HFSUniStr255 *                  volumeName;                 /* output; pointer to volume name */
    FSRef *                         ref;                        /* volume's FSRef */
};
typedef struct FSVolumeInfoParam        FSVolumeInfoParam;
typedef FSVolumeInfoParam *             FSVolumeInfoParamPtr;
/*
    MakeFSRef
    Create an FSRef for an existing object specified by a combination
    of volume refnum, parent directory, and pathname.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ioNamePtr       A pointer to a pathname
    ->  ioVRefNum       A volume specification
    ->  ioDirID         A directory ID
    <-  newRef          A pointer to an FSRef
*/
EXTERN_API( OSErr )
FSpMakeFSRef                    (const FSSpec *         source,
                                 FSRef *                newRef)                             THREEWORDINLINE(0x303C, 0x041A, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBMakeFSRefSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBMakeFSRefSync                 (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x706E, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBMakeFSRefAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBMakeFSRefAsync                (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x706E, 0xA660);


/*
    MakeFSRefUnicode
    Create an FSRef for an existing object specified by 
    Parent FSRef and Unicode name.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             A pointer to the parent directory FSRef
    ->  name            A pointer to Unicde name
    ->  nameLength      The length of the Unicode Name
    ->  textEncodingHint A suggested text encoding to use for the name
    <-  newRef          A pointer to an FSRef
*/
EXTERN_API( OSErr )
FSMakeFSRefUnicode              (const FSRef *          parentRef,
                                 UniCharCount           nameLength,
                                 const UniChar *        name,
                                 TextEncoding           textEncodingHint,
                                 FSRef *                newRef)                             THREEWORDINLINE(0x303C, 0x0A1B, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBMakeFSRefUnicodeSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBMakeFSRefUnicodeSync          (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x707A, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBMakeFSRefUnicodeAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBMakeFSRefUnicodeAsync         (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x707A, 0xA660);


/*
    CompareFSRefs
    Test whether two FSRefs refer to the same file or directory.
    If they do, noErr is returned.  Otherwise, an appropriate error
    (such as errFSRefsDifferent) is returned.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             A pointer to the first FSRef
    ->  parentRef       A pointer to the second FSRef
*/
EXTERN_API( OSErr )
FSCompareFSRefs                 (const FSRef *          ref1,
                                 const FSRef *          ref2)                               THREEWORDINLINE(0x303C, 0x0435, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCompareFSRefsSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCompareFSRefsSync             (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x707C, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBCompareFSRefsAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBCompareFSRefsAsync            (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x707C, 0xA660);


/*
    CreateFileUnicode
    Creates a new file.  The input filename is in Unicode.
    You can optionally set catalog info for the file.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The directory where the file is to be created
    ->  whichInfo       Which catalog info fields to set
    ->  catInfo         The values for catalog info fields to set; may be NULL
    ->  nameLength      Number of Unicode characters in the file's name
    ->  name            A pointer to the Unicode name
    <-  spec            A pointer to the FSSpec for the new directory; may be NULL
    <-  newRef          A pointer to the FSRef for the new file; may be NULL
*/
EXTERN_API( OSErr )
FSCreateFileUnicode             (const FSRef *          parentRef,
                                 UniCharCount           nameLength,
                                 const UniChar *        name,
                                 FSCatalogInfoBitmap    whichInfo,
                                 const FSCatalogInfo *  catalogInfo, /* can be NULL */
                                 FSRef *                newRef, /* can be NULL */
                                 FSSpec *               newSpec) /* can be NULL */          THREEWORDINLINE(0x303C, 0x0E1C, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCreateFileUnicodeSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCreateFileUnicodeSync         (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7070, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBCreateFileUnicodeAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBCreateFileUnicodeAsync        (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7070, 0xA660);


/*
    CreateDirectoryUnicode
    Creates a new directory.  The input directory name is in Unicode.
    You can optionally set catalog info for the directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The parent directory where the directory is to be created
    ->  whichInfo       Which catalog info fields to set
    ->  catInfo         The values for catalog info fields to set; may be NULL
    ->  nameLength      Number of Unicode characters in the directory's name
    ->  name            A pointer to the Unicode name
    <-  ioDirID         The DirID of the new directory
    <-  spec            A pointer to the FSSpec for the new directory; may be NULL
    <-  newRef          A pointer to the FSRef for the new directory; may be NULL
*/
EXTERN_API( OSErr )
FSCreateDirectoryUnicode        (const FSRef *          parentRef,
                                 UniCharCount           nameLength,
                                 const UniChar *        name,
                                 FSCatalogInfoBitmap    whichInfo,
                                 const FSCatalogInfo *  catalogInfo, /* can be NULL */
                                 FSRef *                newRef, /* can be NULL */
                                 FSSpec *               newSpec, /* can be NULL */
                                 UInt32 *               newDirID) /* can be NULL */         THREEWORDINLINE(0x303C, 0x101D, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCreateDirectoryUnicodeSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCreateDirectoryUnicodeSync    (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7071, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBCreateDirectoryUnicodeAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBCreateDirectoryUnicodeAsync   (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7071, 0xA660);


/*
    DeleteObject
    Deletes an existing file or directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory to be deleted
*/
EXTERN_API( OSErr )
FSDeleteObject                  (const FSRef *          ref)                                THREEWORDINLINE(0x303C, 0x021E, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDeleteObjectSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBDeleteObjectSync              (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7072, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBDeleteObjectAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBDeleteObjectAsync             (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7072, 0xA660);


/*
    MoveObject
    Move an existing file or directory into a different directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory to be moved
    ->  parentRef       The file or directory will be moved into this directory
    <-  newRef          A new FSRef for the file or directory in its new location;
                        optional, may be NULL
    NOTE: Moving an object may change its FSRef.  If you want to continue to
    refer to the object, you should pass a non-NULL pointer in newRef and use
    that returned FSRef to access the object after the move.  The FSRef passed
    in "ref" may or may not be usable to access the object after it is moved.
    "newRef" may point to the same storage as "parentRef" or "ref".
*/
EXTERN_API( OSErr )
FSMoveObject                    (const FSRef *          ref,
                                 const FSRef *          destDirectory,
                                 FSRef *                newRef) /* can be NULL */           THREEWORDINLINE(0x303C, 0x061F, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBMoveObjectSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBMoveObjectSync                (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7073, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBMoveObjectAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBMoveObjectAsync               (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7073, 0xA660);


/*
    ExchangeObjects
    swap the contents of two files.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The first file 
    ->  parentRef       The second file 
*/
EXTERN_API( OSErr )
FSExchangeObjects               (const FSRef *          ref,
                                 const FSRef *          destRef)                            THREEWORDINLINE(0x303C, 0x0421, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBExchangeObjectsSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBExchangeObjectsSync           (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7075, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBExchangeObjectsAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBExchangeObjectsAsync          (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7075, 0xA660);


/*
    RenameUnicode
    Change the name of an existing file or directory.  The new name is in
    Unicode.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory to be moved
    ->  nameLength      Number of Unicode characters in the new name
    ->  name            A pointer to the new Unicode name
    ->  textEncodingHint A suggested text encoding to use for the name
    <-  newRef          A new FSRef for the file or directory; may be NULL
    NOTE: Renaming an object may change its FSRef.  If you want to continue to
    refer to the object, you should pass a non-NULL pointer in newRef and use
    that returned FSRef to access the object after the rename.  The FSRef passed
    in "ref" may or may not be usable to access the object after it is renamed.
    "newRef" may point to the same storage as "ref".
*/
EXTERN_API( OSErr )
FSRenameUnicode                 (const FSRef *          ref,
                                 UniCharCount           nameLength,
                                 const UniChar *        name,
                                 TextEncoding           textEncodingHint,
                                 FSRef *                newRef) /* can be NULL */           THREEWORDINLINE(0x303C, 0x0A20, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBRenameUnicodeSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBRenameUnicodeSync             (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7074, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBRenameUnicodeAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBRenameUnicodeAsync            (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7074, 0xA660);


/*
    GetCatalogInfo
    Returns various information about a given file or directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory whose information is to be returned
    ->  whichInfo       Which catalog info fields to get
    <-  catInfo         The returned values of catalog info fields; may be NULL
    <-  spec            A pointer to the FSSpec for the object; may be NULL
    <-  parentRef       A pointer to the FSRef for the object's parent directory; may be NULL
    <-  outName         The Unicode name is returned here.  This pointer may be NULL.
    Note: All of the outputs are optional; if you don't want that particular output, just
    set its pointer to NULL.  This is the call to use to map from an FSRef to an FSSpec.
*/
EXTERN_API( OSErr )
FSGetCatalogInfo                (const FSRef *          ref,
                                 FSCatalogInfoBitmap    whichInfo,
                                 FSCatalogInfo *        catalogInfo, /* can be NULL */
                                 HFSUniStr255 *         outName, /* can be NULL */
                                 FSSpec *               fsSpec, /* can be NULL */
                                 FSRef *                parentRef) /* can be NULL */        THREEWORDINLINE(0x303C, 0x0C22, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetCatalogInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBGetCatalogInfoSync            (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7076, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBGetCatalogInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBGetCatalogInfoAsync           (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7076, 0xA660);


/*
    SetCatalogInfo
    Set catalog information about a given file or directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory whose information is to be changed
    ->  whichInfo       Which catalog info fields to set
    ->  catInfo         The new values of catalog info fields
    Note: Only some of the catalog info fields may be set.  The settable fields
    are given by the constant kFSCatInfoSettableInfo; no other bits may be set in
    whichInfo.
*/
EXTERN_API( OSErr )
FSSetCatalogInfo                (const FSRef *          ref,
                                 FSCatalogInfoBitmap    whichInfo,
                                 const FSCatalogInfo *  catalogInfo)                        THREEWORDINLINE(0x303C, 0x0623, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetCatalogInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBSetCatalogInfoSync            (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7077, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBSetCatalogInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBSetCatalogInfoAsync           (FSRefParam *           paramBlock)                         TWOWORDINLINE(0x7077, 0xA660);


/*
    OpenIterator
    Creates an FSIterator to iterate over a directory or subtree.  The
    iterator can then be passed to GetCatalogInfoBulk or CatalogSearch.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    <-  iterator        The returned FSIterator
    ->  iteratorFlags   Controls whether the iterator iterates over subtrees
                        or just the immediate children of the container.
    ->  container       An FSRef for the directory to iterate (or root of
                        the subtree to iterate).
*/
EXTERN_API( OSErr )
FSOpenIterator                  (const FSRef *          container,
                                 FSIteratorFlags        iteratorFlags,
                                 FSIterator *           iterator)                           THREEWORDINLINE(0x303C, 0x0624, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenIteratorSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBOpenIteratorSync              (FSCatalogBulkParam *   paramBlock)                         TWOWORDINLINE(0x7078, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBOpenIteratorAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBOpenIteratorAsync             (FSCatalogBulkParam *   paramBlock)                         TWOWORDINLINE(0x7078, 0xA660);


/*
    CloseIterator
    Invalidates and disposes an FSIterator.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  iterator        The returned FSIterator
*/
EXTERN_API( OSErr )
FSCloseIterator                 (FSIterator             iterator)                           THREEWORDINLINE(0x303C, 0x0225, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCloseIteratorSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCloseIteratorSync             (FSCatalogBulkParam *   paramBlock)                         TWOWORDINLINE(0x705B, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBCloseIteratorAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBCloseIteratorAsync            (FSCatalogBulkParam *   paramBlock)                         TWOWORDINLINE(0x705B, 0xA660);


/*
    GetCatalogInfoBulk
    Iterates over catalog objects and returns information about them.
    For now, iterator must have been created with kFSIterateFlat option.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  iterator        The iterator
    ->  maximumItems    The maximum number of items to return
    <-  actualItems     The actual number of items returned
    <-  containerChanged Set to true if the container's contents changed
    ->  whichInfo       The catalog information fields to return for each item
    <-  catalogInfo     An array of catalog information; one for each returned item
    <-  refs            An array of FSRefs; one for each returned item
    <-  specs           An array of FSSpecs; one for each returned item
    <-  names           An array of filenames; one for each returned item
    Note: The catalogInfo, refs, specs, names, and containerChanged are all optional outputs;
    if you don't want that particular output, set its pointer to NULL.
*/
EXTERN_API( OSErr )
FSGetCatalogInfoBulk            (FSIterator             iterator,
                                 ItemCount              maximumObjects,
                                 ItemCount *            actualObjects,
                                 Boolean *              containerChanged, /* can be NULL */
                                 FSCatalogInfoBitmap    whichInfo,
                                 FSCatalogInfo *        catalogInfos, /* can be NULL */
                                 FSRef *                refs, /* can be NULL */
                                 FSSpec *               specs, /* can be NULL */
                                 HFSUniStr255 *         names) /* can be NULL */            THREEWORDINLINE(0x303C, 0x1226, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetCatalogInfoBulkSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBGetCatalogInfoBulkSync        (FSCatalogBulkParam *   paramBlock)                         TWOWORDINLINE(0x705C, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBGetCatalogInfoBulkAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBGetCatalogInfoBulkAsync       (FSCatalogBulkParam *   paramBlock)                         TWOWORDINLINE(0x705C, 0xA660);

/*
    CatalogSearch
    Iterates over catalog objects, searching for objects that match given
    search criteria.  Returns various information about matching objects.
    For now, iterator must have been created with kFSIterateSubtree option
    and the container must have been the root directory of a volume.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  iterator        The iterator
    ->  maximumItems    The maximum number of items to return
    <-  actualItems     The actual number of items returned
    <-  containerChanged Set to true if the container's contents changed
    ->  whichInfo       The catalog information fields to return for each item
    <-  catalogInfo     An array of catalog information; one for each returned item
    <-  refs            An array of FSRefs; one for each returned item
    <-  specs           An array of FSSpecs; one for each returned item
    <-  names           An array of filenames; one for each returned item
    ->  searchParams    The criteria that controls the matching, including timeout, a bitmap
                        controlling the fields to compare, and the (Unicode) name to compare.
    Note: The catalogInfo, refs, specs, and names are all optional outputs; if you don't want
    that particular output, set its pointer to NULL.
*/
EXTERN_API( OSErr )
FSCatalogSearch                 (FSIterator             iterator,
                                 const FSSearchParams * searchCriteria,
                                 ItemCount              maximumObjects,
                                 ItemCount *            actualObjects,
                                 Boolean *              containerChanged, /* can be NULL */
                                 FSCatalogInfoBitmap    whichInfo,
                                 FSCatalogInfo *        catalogInfos, /* can be NULL */
                                 FSRef *                refs, /* can be NULL */
                                 FSSpec *               specs, /* can be NULL */
                                 HFSUniStr255 *         names) /* can be NULL */            THREEWORDINLINE(0x303C, 0x1427, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCatalogSearchSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCatalogSearchSync             (FSCatalogBulkParam *   paramBlock)                         TWOWORDINLINE(0x705D, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBCatalogSearchAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBCatalogSearchAsync            (FSCatalogBulkParam *   paramBlock)                         TWOWORDINLINE(0x705D, 0xA660);


/*
    CreateFork
    Create a named fork for a file or directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory
    ->  forkNameLength  The length of the fork name (in Unicode characters)
    ->  forkName        The name of the fork to open (in Unicode)
*/
EXTERN_API( OSErr )
FSCreateFork                    (const FSRef *          ref,
                                 UniCharCount           forkNameLength,
                                 const UniChar *        forkName) /* can be NULL */         THREEWORDINLINE(0x303C, 0x0636, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCreateForkSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCreateForkSync                (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x707D, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBCreateForkAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBCreateForkAsync               (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x707D, 0xA660);


/*
    DeleteFork
    Delete a named fork of a file or directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory
    ->  forkNameLength  The length of the fork name (in Unicode characters)
    ->  forkName        The name of the fork to open (in Unicode)
*/
EXTERN_API( OSErr )
FSDeleteFork                    (const FSRef *          ref,
                                 UniCharCount           forkNameLength,
                                 const UniChar *        forkName) /* can be NULL */         THREEWORDINLINE(0x303C, 0x0637, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBDeleteForkSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBDeleteForkSync                (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x707E, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBDeleteForkAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBDeleteForkAsync               (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x707E, 0xA660);


/*
    IterateForks
    Return the names and sizes of the forks of a file or directory.
    One fork is returned per call.
    ->  ioCompletion    A pointer to a completion routine.
    <-  ioResult        The result code of the function.
    ->  ref             The file or directory containing the forks.
    <-  positionOffset  The length of the fork, in bytes.
    <-  allocationAmount The space allocated to the fork (physical length).
    <-  outForkName     The name of the fork in Unicode.
    <>  forkIterator    Maintains state between calls for a given FSRef.
                        Before the first call, set the initialize field to zero.
*/
EXTERN_API( OSErr )
FSIterateForks                  (const FSRef *          ref,
                                 CatPositionRec *       forkIterator,
                                 HFSUniStr255 *         forkName, /* can be NULL */
                                 SInt64 *               forkSize, /* can be NULL */
                                 UInt64 *               forkPhysicalSize) /* can be NULL */ THREEWORDINLINE(0x303C, 0x0A38, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBIterateForksSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBIterateForksSync              (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x707F, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBIterateForksAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBIterateForksAsync             (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x707F, 0xA660);


/*
    OpenFork
    Open a fork for reading and/or writing.  Allows the opened fork
    to grow beyond 2GB in size.  All volumes should support data and
    resource forks.  Other named forks may be supported by some
    volumes.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory containing the fork to open
    ->  forkNameLength  The length of the fork name (in Unicode characters)
    ->  forkName        The name of the fork to open (in Unicode)
    ->  permissions     The access (read and/or write) you want
    <-  forkRefNum      The reference number for accessing the open fork
*/
EXTERN_API( OSErr )
FSOpenFork                      (const FSRef *          ref,
                                 UniCharCount           forkNameLength,
                                 const UniChar *        forkName, /* can be NULL */
                                 SInt8                  permissions,
                                 SInt16 *               forkRefNum)                         THREEWORDINLINE(0x303C, 0x0928, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBOpenForkSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBOpenForkSync                  (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7079, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBOpenForkAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBOpenForkAsync                 (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7079, 0xA660);


/*
    ReadFork
    Read data from a fork opened via OpenFork.  The first byte to read is
    indicated by a combination of positionMode and positionOffset.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to read from
    <-  buffer          Pointer to buffer where data will be returned
    ->  requestCount    The number of bytes to read
    <-  actualCount     The number of bytes actually read
    ->  positionMode    The base location for start of read
    ->  positionOffset  The offset from base location for start of read
*/
EXTERN_API( OSErr )
FSReadFork                      (SInt16                 forkRefNum,
                                 UInt16                 positionMode,
                                 SInt64                 positionOffset,
                                 ByteCount              requestCount,
                                 void *                 buffer,
                                 ByteCount *            actualCount) /* can be NULL */      THREEWORDINLINE(0x303C, 0x0A29, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBReadForkSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBReadForkSync                  (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7051, 0xA2A8);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBReadForkAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBReadForkAsync                 (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7051, 0xA6A8);


/*
    WriteFork
    Write data to a fork opened via OpenFork.  The first byte to write is
    indicated by a combination of positionMode and positionOffset.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to write to
    ->  buffer          Pointer to data to write
    ->  requestCount    The number of bytes to write
    <-  actualCount     The number of bytes actually written
    ->  positionMode    The base location for start of write
    ->  positionOffset  The offset from base location for start of write
*/
EXTERN_API( OSErr )
FSWriteFork                     (SInt16                 forkRefNum,
                                 UInt16                 positionMode,
                                 SInt64                 positionOffset,
                                 ByteCount              requestCount,
                                 void *                 buffer,
                                 ByteCount *            actualCount) /* can be NULL */      THREEWORDINLINE(0x303C, 0x0A2A, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBWriteForkSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBWriteForkSync                 (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7052, 0xA2A8);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBWriteForkAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBWriteForkAsync                (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7052, 0xA6A8);


/*
    GetForkPosition
    Get the current (default) position of a fork that was
    opened via OpenFork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    <-  positionOffset  The current position of the fork
*/
EXTERN_API( OSErr )
FSGetForkPosition               (SInt16                 forkRefNum,
                                 SInt64 *               position)                           THREEWORDINLINE(0x303C, 0x032B, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetForkPositionSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBGetForkPositionSync           (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7053, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBGetForkPositionAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBGetForkPositionAsync          (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7053, 0xA660);


/*
    SetForkPosition
    Set the current (default) position of a fork that was
    opened via OpenFork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    ->  positionMode    The base location for the new position
    ->  positionOffset  The offset of the new position from the base
*/
EXTERN_API( OSErr )
FSSetForkPosition               (SInt16                 forkRefNum,
                                 UInt16                 positionMode,
                                 SInt64                 positionOffset)                     THREEWORDINLINE(0x303C, 0x042C, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetForkPositionSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBSetForkPositionSync           (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7054, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBSetForkPositionAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBSetForkPositionAsync          (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7054, 0xA660);


/*
    GetForkSize
    Get the current logical size (end-of-file) of an open fork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    <-  positionOffset  The logical size of the fork, in bytes
*/
EXTERN_API( OSErr )
FSGetForkSize                   (SInt16                 forkRefNum,
                                 SInt64 *               forkSize)                           THREEWORDINLINE(0x303C, 0x032D, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetForkSizeSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBGetForkSizeSync               (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7055, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBGetForkSizeAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBGetForkSizeAsync              (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7055, 0xA660);


/*
    SetForkSize
    Set the logical size (end-of-file) of an open fork.  This
    may cause space to be allocated or deallocated.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    ->  positionMode    The base location for the new size
    ->  positionOffset  The offset of the new size from the base
*/
EXTERN_API( OSErr )
FSSetForkSize                   (SInt16                 forkRefNum,
                                 UInt16                 positionMode,
                                 SInt64                 positionOffset)                     THREEWORDINLINE(0x303C, 0x042E, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetForkSizeSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBSetForkSizeSync               (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7056, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBSetForkSizeAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBSetForkSizeAsync              (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7056, 0xA660);


/*
    AllocateFork
    Allocate space to an open fork.  Typically, the space to be
    allocated is beyond the current size of the fork, to reserve
    space so the file will be able to grow later.  Some volume
    formats are unable to allocate space beyond the logical size
    of the fork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    ->  positionMode    The base location for start of allocation
    ->  positionOffset  The offset of the start of allocation
    ->  allocationFlags Zero or more of the following flags:
        kFSAllocContiguousMask
                Any newly allocated space must be one contiguous piece.
        kFSAllocAllOrNothingMask
                All of the request space must be available, or the call
                will fail.  (If not set, the call may succeed even though
                some of the requested space wasn't allocated.)
        kFSAllocNoRoundUpMask
                Do not allocate additional space.  (If not set, a volume
                may allocate additional space in order to reduce fragmentation.)
    <>  allocationAmount    The number of bytes to allocate
                            On output, the number of bytes actually added
*/
EXTERN_API( OSErr )
FSAllocateFork                  (SInt16                 forkRefNum,
                                 FSAllocationFlags      flags,
                                 UInt16                 positionMode,
                                 SInt64                 positionOffset,
                                 UInt64                 requestCount,
                                 UInt64 *               actualCount) /* can be NULL */      THREEWORDINLINE(0x303C, 0x092F, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBAllocateForkSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBAllocateForkSync              (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7057, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBAllocateForkAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBAllocateForkAsync             (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7057, 0xA660);


/*
    FlushFork
    Flush a fork.  Any data written to this fork refnum is flushed to the device.
    The volume's control structures are also flushed to the device.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to flush
*/
EXTERN_API( OSErr )
FSFlushFork                     (SInt16                 forkRefNum)                         THREEWORDINLINE(0x303C, 0x0130, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBFlushForkSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBFlushForkSync                 (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7058, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBFlushForkAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBFlushForkAsync                (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7058, 0xA660);


/*
    CloseFork
    Flush and close a fork.  Any data written to this fork refnum is flushed
    to the device.  The volume's control structures are also flushed to the device.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to close
*/
EXTERN_API( OSErr )
FSCloseFork                     (SInt16                 forkRefNum)                         THREEWORDINLINE(0x303C, 0x0131, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBCloseForkSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBCloseForkSync                 (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7059, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBCloseForkAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBCloseForkAsync                (FSForkIOParam *        paramBlock)                         TWOWORDINLINE(0x7059, 0xA660);


/*
    GetForkCBInfo
    Return information about an open fork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    <>  desiredRefNum   If non-zero on input, then get information for this refnum;
                        unchanged on output.  If zero on input, iterate over all open
                        forks (possibly limited to a single volume); on output, contains
                        the fork's refnum.
    ->  volumeRefNum    Used when desiredRefNum is zero on input.  Set to 0 to iterate over all
                        volumes, or set to a FSVolumeRefNum to limit iteration to that volume.
    <>  iterator        Used when desiredRefNum is zero on input.  Set to 0 before iterating.
                        Pass the iterator returned by the previous call to continue iterating.
    <-  actualRefNum    The refnum of the open fork.
    <-  ref             The FSRef for the file or directory that contains the fork.
    <-  forkInfo        Various information about the open fork.
    <-  outForkName     The name of the fork
    Note: the foundRefNum, ref, forkInfo, and fork name outputs are all optional; if you don't want
    a particular output, then set its pointer to NULL.  If forkName is NULL, then forkNameLength
    will be undefined.
    Note: Returning the forkInfo generally does not require a disk access.  Returning the
    ref or forkName may cause disk access for some volume formats.
*/
EXTERN_API( OSErr )
FSGetForkCBInfo                 (SInt16                 desiredRefNum,
                                 FSVolumeRefNum         volume,
                                 SInt16 *               iterator, /* can be NULL */
                                 SInt16 *               actualRefNum, /* can be NULL */
                                 FSForkInfo *           forkInfo, /* can be NULL */
                                 FSRef *                ref, /* can be NULL */
                                 HFSUniStr255 *         outForkName) /* can be NULL */      THREEWORDINLINE(0x303C, 0x0C32, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetForkCBInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBGetForkCBInfoSync             (FSForkCBInfoParam *    paramBlock)                         TWOWORDINLINE(0x705A, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBGetForkCBInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBGetForkCBInfoAsync            (FSForkCBInfoParam *    paramBlock)                         TWOWORDINLINE(0x705A, 0xA660);


/*
    GetVolumeInfo
    Returns various information about a given volume, or indexing over all volumes.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    <>  ioVRefNum       On input, the volume reference number or drive number whose
                        information is to be returned (if volumeIndex is 0); same
                        as "volume" input to FSGetVolumeInfo.
                        On output, the actual volume reference number; same as
                        "actualVolume" output of FSGetVolumeInfo.
    ->  volumeIndex     The index of the desired volume, or 0 to use ioVRefNum
    ->  whichInfo       Which volInfo info fields to get
    <-  volumeInfo      The returned values of Volume info fields; may be NULL
    <-  name            The Unicode name is returned here.  This pointer may be NULL.
    Note: All of the outputs are optional; if you don't want that particular output, just
    set it's pointer to NULL.
*/
EXTERN_API( OSErr )
FSGetVolumeInfo                 (FSVolumeRefNum         volume,
                                 ItemCount              volumeIndex,
                                 FSVolumeRefNum *       actualVolume, /* can be NULL */
                                 FSVolumeInfoBitmap     whichInfo,
                                 FSVolumeInfo *         info, /* can be NULL */
                                 HFSUniStr255 *         volumeName, /* can be NULL */
                                 FSRef *                rootDirectory) /* can be NULL */    THREEWORDINLINE(0x303C, 0x0D33, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBGetVolumeInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBGetVolumeInfoSync             (FSVolumeInfoParam *    paramBlock)                         TWOWORDINLINE(0x701D, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBGetVolumeInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBGetVolumeInfoAsync            (FSVolumeInfoParam *    paramBlock)                         TWOWORDINLINE(0x701D, 0xA660);


/*
    SetVolumeInfo
    Set information about a given volume.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ioVRefNum       The volume whose information is to be changed
    ->  whichInfo       Which catalog info fields to set
    ->  volumeInfo      The new values of volume info fields
    Note: Only some of the volume info fields may be set.  The settable fields
    are given by the constant kFSVolInfoSettableInfo; no other bits may be set in
    whichInfo.
*/
EXTERN_API( OSErr )
FSSetVolumeInfo                 (FSVolumeRefNum         volume,
                                 FSVolumeInfoBitmap     whichInfo,
                                 const FSVolumeInfo *   info)                               THREEWORDINLINE(0x303C, 0x0534, 0xAA52);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 PBSetVolumeInfoSync(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
PBSetVolumeInfoSync             (FSVolumeInfoParam *    paramBlock)                         TWOWORDINLINE(0x701E, 0xA260);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter PBSetVolumeInfoAsync(__A0)
                                                                                            #endif
EXTERN_API( void )
PBSetVolumeInfoAsync            (FSVolumeInfoParam *    paramBlock)                         TWOWORDINLINE(0x701E, 0xA660);

/*
    FSGetDataForkName
    Returns the constant for the name of the data fork (the empty string)
*/
EXTERN_API( OSErr )
FSGetDataForkName               (HFSUniStr255 *         dataForkName)                       THREEWORDINLINE(0x303C, 0x0218, 0xAA52);

/*
    FSGetResourceForkName
    Returns the constant for the name of the resource fork
    (currently "RESOURCE_FORK").
*/
EXTERN_API( OSErr )
FSGetResourceForkName           (HFSUniStr255 *         resourceForkName)                   THREEWORDINLINE(0x303C, 0x0219, 0xAA52);




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

#endif /* __FILES__ */

