/*
     File:       FSM.h
 
     Contains:   HFS External File System Interfaces.
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FSM__
#define __FSM__

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __HFSVOLUMES__
#include <HFSVolumes.h>
#endif

#ifndef __PROCESSES__
#include <Processes.h>
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

/* VolMount declarations are now in Files.* */

/*
 * Miscellaneous file system values not in Files.*
 */
enum {
    fsUsrCNID                   = 16,                           /* First assignable directory or file number */
                                                                /*    File system trap word attribute bits */
    kHFSBit                     = 9,                            /* HFS call: bit 9 */
    kHFSMask                    = 0x0200,
    kAsyncBit                   = 10,                           /* Asynchronous call: bit 10 */
    kAsyncMask                  = 0x0400
};


/*
 * HFSCIProc selectCode values
 * Note: The trap attribute bits (the HFS bit and the asynchronous bit)
 * may be set in these selectCode values.
 */
enum {
    kFSMOpen                    = 0xA000,
    kFSMClose                   = 0xA001,
    kFSMRead                    = 0xA002,
    kFSMWrite                   = 0xA003,
    kFSMGetVolInfo              = 0xA007,
    kFSMCreate                  = 0xA008,
    kFSMDelete                  = 0xA009,
    kFSMOpenRF                  = 0xA00A,
    kFSMRename                  = 0xA00B,
    kFSMGetFileInfo             = 0xA00C,
    kFSMSetFileInfo             = 0xA00D,
    kFSMUnmountVol              = 0xA00E,
    kFSMMountVol                = 0xA00F,
    kFSMAllocate                = 0xA010,
    kFSMGetEOF                  = 0xA011,
    kFSMSetEOF                  = 0xA012,
    kFSMFlushVol                = 0xA013,
    kFSMGetVol                  = 0xA014,
    kFSMSetVol                  = 0xA015,
    kFSMEject                   = 0xA017,
    kFSMGetFPos                 = 0xA018,
    kFSMOffline                 = 0xA035,
    kFSMSetFilLock              = 0xA041,
    kFSMRstFilLock              = 0xA042,
    kFSMSetFilType              = 0xA043,
    kFSMSetFPos                 = 0xA044,
    kFSMFlushFile               = 0xA045,                       /*    The File System HFSDispatch selectCodes */
    kFSMOpenWD                  = 0x0001,
    kFSMCloseWD                 = 0x0002,
    kFSMCatMove                 = 0x0005,
    kFSMDirCreate               = 0x0006,
    kFSMGetWDInfo               = 0x0007,
    kFSMGetFCBInfo              = 0x0008,
    kFSMGetCatInfo              = 0x0009,
    kFSMSetCatInfo              = 0x000A,
    kFSMSetVolInfo              = 0x000B,
    kFSMLockRng                 = 0x0010,
    kFSMUnlockRng               = 0x0011,
    kFSMXGetVolInfo             = 0x0012,
    kFSMCreateFileIDRef         = 0x0014,
    kFSMDeleteFileIDRef         = 0x0015,
    kFSMResolveFileIDRef        = 0x0016,
    kFSMExchangeFiles           = 0x0017,
    kFSMCatSearch               = 0x0018,
    kFSMOpenDF                  = 0x001A,
    kFSMMakeFSSpec              = 0x001B,                       /*    The Desktop Manager HFSDispatch selectCodes */
    kFSMDTGetPath               = 0x0020,
    kFSMDTCloseDown             = 0x0021,
    kFSMDTAddIcon               = 0x0022,
    kFSMDTGetIcon               = 0x0023,
    kFSMDTGetIconInfo           = 0x0024,
    kFSMDTAddAPPL               = 0x0025,
    kFSMDTRemoveAPPL            = 0x0026,
    kFSMDTGetAPPL               = 0x0027,
    kFSMDTSetComment            = 0x0028,
    kFSMDTRemoveComment         = 0x0029,
    kFSMDTGetComment            = 0x002A,
    kFSMDTFlush                 = 0x002B,
    kFSMDTReset                 = 0x002C,
    kFSMDTGetInfo               = 0x002D,
    kFSMDTOpenInform            = 0x002E,
    kFSMDTDelete                = 0x002F,                       /*    The AppleShare HFSDispatch selectCodes */
    kFSMGetVolParms             = 0x0030,
    kFSMGetLogInInfo            = 0x0031,
    kFSMGetDirAccess            = 0x0032,
    kFSMSetDirAccess            = 0x0033,
    kFSMMapID                   = 0x0034,
    kFSMMapName                 = 0x0035,
    kFSMCopyFile                = 0x0036,
    kFSMMoveRename              = 0x0037,
    kFSMOpenDeny                = 0x0038,
    kFSMOpenRFDeny              = 0x0039,
    kFSMGetXCatInfo             = 0x003A,
    kFSMGetVolMountInfoSize     = 0x003F,
    kFSMGetVolMountInfo         = 0x0040,
    kFSMVolumeMount             = 0x0041,
    kFSMShare                   = 0x0042,
    kFSMUnShare                 = 0x0043,
    kFSMGetUGEntry              = 0x0044,
    kFSMGetForeignPrivs         = 0x0060,
    kFSMSetForeignPrivs         = 0x0061,                       /* The HFSPlus    API SelectCodes    */
    kFSMGetVolumeInfo           = 0x001D,
    kFSMSetVolumeInfo           = 0x001E,
    kFSMReadFork                = 0x0051,
    kFSMWriteFork               = 0x0052,
    kFSMGetForkPosition         = 0x0053,
    kFSMSetForkPosition         = 0x0054,
    kFSMGetForkSize             = 0x0055,
    kFSMSetForkSize             = 0x0056,
    kFSMAllocateFork            = 0x0057,
    kFSMFlushFork               = 0x0058,
    kFSMCloseFork               = 0x0059,
    kFSMGetForkCBInfo           = 0x005A,
    kFSMCloseIterator           = 0x005B,
    kFSMGetCatalogInfoBulk      = 0x005C,
    kFSMCatalogSearch           = 0x005D,
    kFSMMakeFSRef               = 0x006E,
    kFSMCreateFileUnicode       = 0x0070,
    kFSMCreateDirUnicode        = 0x0071,
    kFSMDeleteObject            = 0x0072,
    kFSMMoveObject              = 0x0073,
    kFSMRenameUnicode           = 0x0074,
    kFSMExchangeObjects         = 0x0075,
    kFSMGetCatalogInfo          = 0x0076,
    kFSMSetCatalogInfo          = 0x0077,
    kFSMOpenIterator            = 0x0078,
    kFSMOpenFork                = 0x0079,
    kFSMMakeFSRefUnicode        = 0x007A,
    kFSMCompareFSRefs           = 0x007C,
    kFSMCreateFork              = 0x007D,
    kFSMDeleteFork              = 0x007E,
    kFSMIterateForks            = 0x007F
};

/*
 * UTDetermineVol status values
 */
enum {
    dtmvError                   = 0,                            /* param error */
    dtmvFullPathame             = 1,                            /* determined by full pathname */
    dtmvVRefNum                 = 2,                            /* determined by volume refNum */
    dtmvWDRefNum                = 3,                            /* determined by working directory refNum */
    dtmvDriveNum                = 4,                            /* determined by drive number */
    dtmvDefault                 = 5                             /* determined by default volume */
};


/*
 * UTGetBlock options
 */
enum {
    gbDefault                   = 0,                            /* default value - read if not found */
                                                                /*    bits and masks */
    gbReadBit                   = 0,                            /* read block from disk (forced read) */
    gbReadMask                  = 0x0001,
    gbExistBit                  = 1,                            /* get existing cache block */
    gbExistMask                 = 0x0002,
    gbNoReadBit                 = 2,                            /* don't read block from disk if not found in cache */
    gbNoReadMask                = 0x0004,
    gbReleaseBit                = 3,                            /* release block immediately after GetBlock */
    gbReleaseMask               = 0x0008
};


/*
 * UTReleaseBlock options
 */
enum {
    rbDefault                   = 0,                            /* default value - just mark the buffer not in-use */
                                                                /*    bits and masks */
    rbWriteBit                  = 0,                            /* force write buffer to disk */
    rbWriteMask                 = 0x0001,
    rbTrashBit                  = 1,                            /* trash buffer contents after release */
    rbTrashMask                 = 0x0002,
    rbDirtyBit                  = 2,                            /* mark buffer dirty */
    rbDirtyMask                 = 0x0004,
    rbFreeBit                   = 3,                            /* free the buffer (save in the hash) */
    rbFreeMask                  = 0x000A                        /* rbFreeMask (rbFreeBit + rbTrashBit) works as rbTrash on < System 7.0 RamCache; on >= System 7.0, rbfreeMask overrides rbTrash */
};


/*
 * UTFlushCache options
 */
enum {
    fcDefault                   = 0,                            /* default value - pass this fcOption to just flush any dirty buffers */
                                                                /*    bits and masks */
    fcTrashBit                  = 0,                            /* (don't pass this as fcOption, use only for testing bit) */
    fcTrashMask                 = 0x0001,                       /* pass this fcOption value to flush and trash cache blocks */
    fcFreeBit                   = 1,                            /* (don't pass this as fcOption, use only for testing bit) */
    fcFreeMask                  = 0x0003                        /* pass this fcOption to flush and free cache blocks (Note: both fcTrash and fcFree bits are set) */
};


/*
 * UTCacheReadIP and UTCacheWriteIP cacheOption bits and masks are the ioPosMode
 * bits and masks in Files.x
 */

/*
 * Cache routine internal error codes
 */
enum {
    chNoBuf                     = 1,                            /* no free cache buffers (all in use) */
    chInUse                     = 2,                            /* requested block in use */
    chnotfound                  = 3,                            /* requested block not found */
    chNotInUse                  = 4                             /* block being released was not in use */
};


/*
 * FCBRec.fcbFlags bits
 */
enum {
    fcbWriteBit                 = 0,                            /* Data can be written to this file */
    fcbWriteMask                = 0x01,
    fcbResourceBit              = 1,                            /* This file is a resource fork */
    fcbResourceMask             = 0x02,
    fcbWriteLockedBit           = 2,                            /* File has a locked byte range */
    fcbWriteLockedMask          = 0x04,
    fcbLargeFileBit             = 3,                            /* File may grow beyond 2GB; cache uses file blocks, not bytes */
    fcbLargeFileMask            = 0x08,
    fcbSharedWriteBit           = 4,                            /* File is open for shared write access */
    fcbSharedWriteMask          = 0x10,
    fcbFileLockedBit            = 5,                            /* File is locked (write-protected) */
    fcbFileLockedMask           = 0x20,
    fcbOwnClumpBit              = 6,                            /* File has clump size specified in FCB */
    fcbOwnClumpMask             = 0x40,
    fcbModifiedBit              = 7,                            /* File has changed since it was last flushed */
    fcbModifiedMask             = 0x80
};


/*
 *  ForkControlBlock.moreFlags bits
 */
enum {
    fcbIteratorBit              = 0,                            /* FCB belongs to an FSIterator */
    fcbIteratorMask             = 0x0001,
    fcbUseForkIDBit             = 1,                            /* Use the FCB's forkID instead of fcbResourceBit */
    fcbUseForkIDMask            = 0x0002
};


/*
 *  HFS Component Interface constants
 */

/*
 * compInterfMask bits specific to HFS component
 */
enum {
    hfsCIDoesHFSBit             = 23,                           /* set if file system supports HFS calls */
    hfsCIDoesHFSMask            = 0x00800000,
    hfsCIDoesAppleShareBit      = 22,                           /* set if AppleShare calls supported */
    hfsCIDoesAppleShareMask     = 0x00400000,
    hfsCIDoesDeskTopBit         = 21,                           /* set if Desktop Database calls supported */
    hfsCIDoesDeskTopMask        = 0x00200000,
    hfsCIDoesDynamicLoadBit     = 20,                           /* set if dynamically loading code resource */
    hfsCIDoesDynamicLoadMask    = 0x00100000,                   /*       supported */
    hfsCIResourceLoadedBit      = 19,                           /* set if code resource already loaded */
    hfsCIResourceLoadedMask     = 0x00080000,
    hfsCIHasHLL2PProcBit        = 18,                           /* set if FFS' log2PhyProc */
    hfsCIHasHLL2PProcMask       = 0x00040000,                   /* is written in a high level language. (i.e., uses Pascal calling convention) */
    hfsCIWantsDTSupportBit      = 17,                           /* set if FFS wants the Mac OS's Desktop Manager code to handle */
    hfsCIWantsDTSupportMask     = 0x00020000                    /* all Desktop Manager requests to its volumes. */
};


/*
 *  Disk Initialization Component Interface constants
 */

/*
 * compInterfMask bits specific to Disk Initialization component
 */
enum {
    diCIHasExtFormatParamsBit   = 18,                           /* set if file system needs extended format */
    diCIHasExtFormatParamsMask  = 0x00040000,                   /*       parameters */
    diCIHasMultiVolTypesBit     = 17,                           /* set if file system supports more than one */
    diCIHasMultiVolTypesMask    = 0x00020000,                   /*       volume type */
    diCIDoesSparingBit          = 16,                           /* set if file system supports disk sparing */
    diCIDoesSparingMask         = 0x00010000,
    diCILiveBit                 = 0,                            /* set if file system is candidate for current */
    diCILiveMask                = 0x00000001                    /*       formatting operation (set by PACK2) */
};


/*
 * Disk Initialization Component Function selectors
 */
enum {
    diCILoad                    = 1,                            /* Make initialization code memory resident */
    diCIUnload                  = 2,                            /* Make initialization code purgeable */
    diCIEvaluateSizeChoices     = 3,                            /* Evaluate size choices */
    diCIExtendedZero            = 4,                            /* Write an empty volume directory */
    diCIValidateVolName         = 5,                            /* Validate volume name */
    diCIGetVolTypeInfo          = 6,                            /* get volume type info */
    diCIGetFormatString         = 7,                            /* get dialog format string */
    diCIGetExtFormatParams      = 8,                            /* get extended format parameters */
    diCIGetDefectList           = 9                             /* return the defect list for the indicated disk - reserved for future use */
};


/*
 * Constants used in the DICIEvaluateSizeRec and FormatListRec
 */
enum {
    diCIFmtListMax              = 8,                            /* maximum number of format list entries in DICIEvaluateSizeRec.numSizeEntries */
                                                                /*    bits in FormatListRec.formatFlags: */
    diCIFmtFlagsValidBit        = 7,                            /* set if sec, side, tracks valid */
    diCIFmtFlagsValidMask       = 0x80,
    diCIFmtFlagsCurrentBit      = 6,                            /* set if current disk has this fmt */
    diCIFmtFlagsCurrentMask     = 0x40,                         /*    bits in FormatListRec.sizeListFlags: */
    diCISizeListOKBit           = 15,                           /* set if this disk size usable */
    diCISizeListOKMask          = 0x8000
};


/*
 * DICIGetFormatStringRec.stringKind format strings
 */
enum {
    diCIAlternateFormatStr      = 1,                            /* get alternate format  string (Balloon Help) */
    diCISizePresentationStr     = 2                             /* get size presentation string (for dialog) */
};


/*
 * Error codes returned by Disk Sparing
 */
enum {
    diCIUserCancelErr           = 1,                            /* user cancelled the disk init */
    diCICriticalSectorBadErr    = 20,                           /* critical sectors are bad (hopeless)    */
    diCISparingFailedErr        = 21,                           /* disk cannot be spared */
    diCITooManyBadSectorsErr    = 22,                           /* too many bad sectors */
    diCIUnknownVolTypeErr       = 23,                           /* the volume type passed in diCIExtendedZero paramBlock is not supported */
    diCIVolSizeMismatchErr      = 24,                           /* specified volume size doesn't match with formatted disk size */
    diCIUnknownDICallErr        = 25,                           /* bogus DI function call selector */
    diCINoSparingErr            = 26,                           /* disk is bad but the target FS doesn't do disk sparing */
    diCINoExtendInfoErr         = 27,                           /* missing file system specific extra parameter in diCIExtendedZero call */
    diCINoMessageTextErr        = 28                            /* missing message text in DIReformat call */
};


/*
 *  File System Manager constants
 */

/*
 * Miscellaneous constants used by FSM
 */
enum {
    fsdVersion1                 = 1,                            /* current version of FSD record */
    fsmIgnoreFSID               = 0xFFFE,                       /* this FSID should be ignored by the driver */
    fsmGenericFSID              = 0xFFFF                        /* unknown foreign file system ID */
};


/*
 * compInterfMask bits common to all FSM components
 */
enum {
    fsmComponentEnableBit       = 31,                           /* set if FSM component interface is enabled */
    fsmComponentEnableMask      = (long)0x80000000,
    fsmComponentBusyBit         = 30,                           /* set if FSM component interface is busy */
    fsmComponentBusyMask        = 0x40000000
};


/*
 * Selectors for GetFSInfo
 */
enum {
    fsmGetFSInfoByIndex         = -1,                           /* get fs info by index */
    fsmGetFSInfoByFSID          = 0,                            /* get fs info by FSID */
    fsmGetFSInfoByRefNum        = 1                             /* get fs info by file/vol refnum */
};


/*
 * InformFSM messages
 */
enum {
    fsmNopMessage               = 0,                            /* nop */
    fsmDrvQElChangedMessage     = 1,                            /* DQE has changed */
    fsmGetFSIconMessage         = 2                             /* Get FFS's disk icon */
};


/*
 * Messages passed to the fileSystemCommProc
 */
enum {
    ffsNopMessage               = 0,                            /* nop, should always return noErr */
    ffsGetIconMessage           = 1,                            /* return disk icon and mask */
    ffsIDDiskMessage            = 2,                            /* identify the about-to-be-mounted volume */
    ffsLoadMessage              = 3,                            /* load in the FFS */
    ffsUnloadMessage            = 4,                            /* unload the FFS */
    ffsIDVolMountMessage        = 5,                            /* identify a VolMountInfo record */
    ffsInformMessage            = 6,                            /* FFS defined message */
    ffsGetIconInfoMessage       = 7
};



/*
 *  HFS Utility routine records
 */

/*
 * record used by UTGetPathComponentName
 */

struct ParsePathRec {
    StringPtr                       namePtr;                    /* pathname to parse */
    short                           startOffset;                /* where to start parsing */
    short                           componentLength;            /* the length of the pathname component parsed */
    SignedByte                      moreName;                   /* non-zero if there are more components after this one */
    SignedByte                      foundDelimiter;             /* non-zero if parsing stopped because a colon (:) delimiter was found */
};
typedef struct ParsePathRec             ParsePathRec;
typedef ParsePathRec *                  ParsePathRecPtr;

struct WDCBRec {
    VCBPtr                          wdVCBPtr;                   /* Pointer to VCB of this working directory */
    long                            wdDirID;                    /* Directory ID number of this working directory */
    long                            wdCatHint;                  /* Hint for finding this working directory */
    long                            wdProcID;                   /* Process that created this working directory */
};
typedef struct WDCBRec                  WDCBRec;
typedef WDCBRec *                       WDCBRecPtr;

struct FCBRec {
    unsigned long                   fcbFlNm;                    /* FCB file number. Non-zero marks FCB used */
    SignedByte                      fcbFlags;                   /* FCB flags */
    SignedByte                      fcbTypByt;                  /* File type byte */
    unsigned short                  fcbSBlk;                    /* File start block (in alloc size blks) */
    unsigned long                   fcbEOF;                     /* Logical length or EOF in bytes */
    unsigned long                   fcbPLen;                    /* Physical file length in bytes */
    unsigned long                   fcbCrPs;                    /* Current position within file */
    VCBPtr                          fcbVPtr;                    /* Pointer to the corresponding VCB */
    Ptr                             fcbBfAdr;                   /* File's buffer address */
    unsigned short                  fcbFlPos;                   /* Directory block this file is in */
                                                                /* FCB Extensions for HFS */
    unsigned long                   fcbClmpSize;                /* Number of bytes per clump */
    Ptr                             fcbBTCBPtr;                 /* Pointer to B*-Tree control block for file */
    HFSExtentRecord                 fcbExtRec;                  /* First 3 file extents */
    OSType                          fcbFType;                   /* File's 4 Finder Type bytes */
    unsigned long                   fcbCatPos;                  /* Catalog hint for use on Close */
    unsigned long                   fcbDirID;                   /* Parent Directory ID */
    Str31                           fcbCName;                   /* CName of open file */
};
typedef struct FCBRec                   FCBRec;
typedef FCBRec *                        FCBRecPtr;

struct ForkControlBlock {
    unsigned long                   fcbFlNm;                    /* FCB file number. Non-zero marks FCB used */
    SignedByte                      fcbFlags;                   /* FCB flags */
    SignedByte                      fcbTypByt;                  /* File type byte */
    unsigned short                  fcbSBlk;                    /* File start block (in alloc size blks) */
    unsigned long                   fcbEOF;                     /* Logical length or EOF in bytes */
    unsigned long                   fcbPLen;                    /* Physical file length in bytes */
    unsigned long                   fcbCrPs;                    /* Current position within file */
    VCBPtr                          fcbVPtr;                    /* Pointer to the corresponding VCB */
    Ptr                             fcbBfAdr;                   /* File's buffer address */
    unsigned short                  fcbFlPos;                   /* Directory block this file is in */
                                                                /* FCB Extensions for HFS */
    unsigned long                   fcbClmpSize;                /* Number of bytes per clump */
    Ptr                             fcbBTCBPtr;                 /* Pointer to B*-Tree control block for file */
    HFSExtentRecord                 fcbExtRec;                  /* First 3 file extents */
    OSType                          fcbFType;                   /* File's 4 Finder Type bytes */
    unsigned long                   fcbCatPos;                  /* Catalog hint for use on Close */
    unsigned long                   fcbDirID;                   /* Parent Directory ID */
    Str31                           fcbCName;                   /* CName of open file */

                                                                /*    New fields start here*/

    unsigned short                  moreFlags;                  /* more flags, align following fields*/

                                                                /*    Old ExtendedFCB fields*/

    ProcessSerialNumber             processID;                  /* Process Mgr process that opened the file (used to clean up at process death).*/
    HFSPlusExtentRecord             extents;                    /* extents for HFS+ volumes*/

                                                                /*    New fields for HFS Plus APIs*/

    UInt64                          endOfFile;                  /* logical size of this fork*/
    UInt64                          physicalEOF;                /* amount of space physically allocated to this fork*/
    UInt64                          currentPosition;            /* default offset for next read/write*/
    UInt32                          forkID;
    Ptr                             searchListPtr;              /* reserved for File Manager's use*/
    UInt8                           reserved1[8];               /* reserved*/
};
typedef struct ForkControlBlock         ForkControlBlock;
typedef ForkControlBlock *              ForkControlBlockPtr;
/*
 *  IteratorControlBlock - a ForkControlBlock used by a FSIterator
 */

struct IteratorControlBlock {
    unsigned long                   containerID;                /* directory ID of iterator's container*/
    unsigned short                  flags;                      /* reserved*/
    UInt8                           user1[14];                  /* 14 bytes iterator's use*/
    VCBPtr                          vcbPtr;                     /* pointer to the iterator's VCB*/
    unsigned long                   reserved2;                  /* reserved*/
    UInt8                           user2[6];                   /* 6 bytes for iterator's use*/
    unsigned long                   reserved3;                  /* reserved*/
    UInt8                           user3[12];                  /* 12 bytes for iterator's use*/
    OSType                          cbType;                     /* must be 'fold'*/
    UInt8                           user4[40];                  /* 40 bytes for iterator's use*/
    unsigned short                  moreFlags;                  /* must be fcbIteratorMask*/
    ProcessSerialNumber             processID;                  /* Process Mgr process that opened the iterator (used to clean up at process death).*/
    UInt8                           user5[92];                  /* 92 bytes for iterator's use*/
    Ptr                             searchListPtr;              /* reserved for File Manager's use*/
    UInt8                           reserved1[8];               /* reserved*/
};
typedef struct IteratorControlBlock     IteratorControlBlock;
typedef IteratorControlBlock *          IteratorControlBlockPtr;
/*
 *  HFS Component Interface records
 */
typedef CALLBACK_API( OSErr , Lg2PhysProcPtr )(void *fsdGlobalPtr, VCBPtr volCtrlBlockPtr, FCBRecPtr fileCtrlBlockPtr, short fileRefNum, unsigned long filePosition, unsigned long reqCount, unsigned long *volOffset, unsigned long *contiguousBytes);
typedef STACK_UPP_TYPE(Lg2PhysProcPtr)                          Lg2PhysUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(Lg2PhysUPP)
    NewLg2PhysUPP                  (Lg2PhysProcPtr          userRoutine);

    EXTERN_API(void)
    DisposeLg2PhysUPP              (Lg2PhysUPP              userUPP);

    EXTERN_API(OSErr)
    InvokeLg2PhysUPP               (void *                  fsdGlobalPtr,
                                    VCBPtr                  volCtrlBlockPtr,
                                    FCBRecPtr               fileCtrlBlockPtr,
                                    short                   fileRefNum,
                                    unsigned long           filePosition,
                                    unsigned long           reqCount,
                                    unsigned long *         volOffset,
                                    unsigned long *         contiguousBytes,
                                    Lg2PhysUPP              userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppLg2PhysProcInfo = 0x003FEFE0 };                       /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewLg2PhysUPP(userRoutine)                              (Lg2PhysUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppLg2PhysProcInfo, GetCurrentArchitecture())
    #define DisposeLg2PhysUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define InvokeLg2PhysUPP(fsdGlobalPtr, volCtrlBlockPtr, fileCtrlBlockPtr, fileRefNum, filePosition, reqCount, volOffset, contiguousBytes, userUPP)  (OSErr)CALL_EIGHT_PARAMETER_UPP((userUPP), uppLg2PhysProcInfo, (fsdGlobalPtr), (volCtrlBlockPtr), (fileCtrlBlockPtr), (fileRefNum), (filePosition), (reqCount), (volOffset), (contiguousBytes))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewLg2PhysProc(userRoutine)                             NewLg2PhysUPP(userRoutine)
#define CallLg2PhysProc(userRoutine, fsdGlobalPtr, volCtrlBlockPtr, fileCtrlBlockPtr, fileRefNum, filePosition, reqCount, volOffset, contiguousBytes) InvokeLg2PhysUPP(fsdGlobalPtr, volCtrlBlockPtr, fileCtrlBlockPtr, fileRefNum, filePosition, reqCount, volOffset, contiguousBytes, userRoutine)
typedef CALLBACK_API( OSErr , HFSCIProcPtr )(VCBPtr theVCB, short selectCode, void *paramBlock, void *fsdGlobalPtr, short fsid);
typedef STACK_UPP_TYPE(HFSCIProcPtr)                            HFSCIUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(HFSCIUPP)
    NewHFSCIUPP                    (HFSCIProcPtr            userRoutine);

    EXTERN_API(void)
    DisposeHFSCIUPP                (HFSCIUPP                userUPP);

    EXTERN_API(OSErr)
    InvokeHFSCIUPP                 (VCBPtr                  theVCB,
                                    short                   selectCode,
                                    void *                  paramBlock,
                                    void *                  fsdGlobalPtr,
                                    short                   fsid,
                                    HFSCIUPP                userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppHFSCIProcInfo = 0x0000BEE0 };                         /* pascal 2_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes, 2_bytes) */
    #define NewHFSCIUPP(userRoutine)                                (HFSCIUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppHFSCIProcInfo, GetCurrentArchitecture())
    #define DisposeHFSCIUPP(userUPP)                                DisposeRoutineDescriptor(userUPP)
    #define InvokeHFSCIUPP(theVCB, selectCode, paramBlock, fsdGlobalPtr, fsid, userUPP)  (OSErr)CALL_FIVE_PARAMETER_UPP((userUPP), uppHFSCIProcInfo, (theVCB), (selectCode), (paramBlock), (fsdGlobalPtr), (fsid))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewHFSCIProc(userRoutine)                               NewHFSCIUPP(userRoutine)
#define CallHFSCIProc(userRoutine, theVCB, selectCode, paramBlock, fsdGlobalPtr, fsid) InvokeHFSCIUPP(theVCB, selectCode, paramBlock, fsdGlobalPtr, fsid, userRoutine)

struct HFSCIRec {
    long                            compInterfMask;             /* component flags */
    HFSCIUPP                        compInterfProc;             /* pointer to file system call processing code */
    Lg2PhysUPP                      log2PhyProc;                /* pointer to Lg2PhysProc() code */
    Ptr                             stackTop;                   /* file system stack top */
    long                            stackSize;                  /* file system stack size */
    Ptr                             stackPtr;                   /* current file system stack pointer */
    long                            reserved3;                  /* --reserved, must be zero-- */
    long                            idSector;                   /* Sector you need to ID a local volume. For networked volumes, this must be -1 */
    long                            reserved2;                  /* --reserved, must be zero-- */
    long                            reserved1;                  /* --reserved, must be zero-- */
};
typedef struct HFSCIRec                 HFSCIRec;
typedef HFSCIRec *                      HFSCIRecPtr;
/*
 *  Disk Initialization Component Interface records
 */
typedef CALLBACK_API( OSErr , DICIProcPtr )(short whatFunction, void *paramBlock, void *fsdGlobalPtr);
typedef STACK_UPP_TYPE(DICIProcPtr)                             DICIUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(DICIUPP)
    NewDICIUPP                     (DICIProcPtr             userRoutine);

    EXTERN_API(void)
    DisposeDICIUPP                 (DICIUPP                 userUPP);

    EXTERN_API(OSErr)
    InvokeDICIUPP                  (short                   whatFunction,
                                    void *                  paramBlock,
                                    void *                  fsdGlobalPtr,
                                    DICIUPP                 userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppDICIProcInfo = 0x00000FA0 };                          /* pascal 2_bytes Func(2_bytes, 4_bytes, 4_bytes) */
    #define NewDICIUPP(userRoutine)                                 (DICIUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDICIProcInfo, GetCurrentArchitecture())
    #define DisposeDICIUPP(userUPP)                                 DisposeRoutineDescriptor(userUPP)
    #define InvokeDICIUPP(whatFunction, paramBlock, fsdGlobalPtr, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppDICIProcInfo, (whatFunction), (paramBlock), (fsdGlobalPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewDICIProc(userRoutine)                                NewDICIUPP(userRoutine)
#define CallDICIProc(userRoutine, whatFunction, paramBlock, fsdGlobalPtr) InvokeDICIUPP(whatFunction, paramBlock, fsdGlobalPtr, userRoutine)

struct DICIRec {
    long                            compInterfMask;             /* component flags */
    DICIUPP                         compInterfProc;             /* pointer to call processing code */
    short                           maxVolNameLength;           /* maximum length of your volume name */
    unsigned short                  blockSize;                  /* your file system's block size */
    long                            reserved3;                  /* --reserved, must be zero-- */
    long                            reserved2;                  /* --reserved, must be zero-- */
    long                            reserved1;                  /* --reserved, must be zero-- */
};
typedef struct DICIRec                  DICIRec;
typedef DICIRec *                       DICIRecPtr;
/*
 * FormatListRec as returned by the .Sony disk driver's
 * Return Format List status call (csCode = 6).
 * If the status call to get this list for a drive is not
 * implemented by the driver, then a list with one entry
 * is contructed from the drive queue element for the drive.
 */

struct FormatListRec {
    unsigned long                   volSize;                    /* disk capacity in SECTORs */
    SignedByte                      formatFlags;                /* flags */
    SignedByte                      sectorsPerTrack;            /* sectors per track side */
    unsigned short                  tracks;                     /* number of tracks */
};
typedef struct FormatListRec            FormatListRec;
typedef FormatListRec *                 FormatListRecPtr;
/*
 * SizeListRec built from FormatListRecs as described above.
 */

struct SizeListRec {
    short                           sizeListFlags;              /* flags as set by external file system */
    FormatListRec                   sizeEntry;                  /* disk driver format list record */
};
typedef struct SizeListRec              SizeListRec;
typedef SizeListRec *                   SizeListRecPtr;
/*
 * paramBlock for the diCIEvaluateSize call
 */

struct DICIEvaluateSizeRec {
    short                           defaultSizeIndex;           /* default size for this FS */
    short                           numSizeEntries;             /* number of size entries */
    short                           driveNumber;                /* drive number */
    SizeListRecPtr                  sizeListPtr;                /* ptr to size entry table */
    unsigned short                  sectorSize;                 /* bytes per sector */
};
typedef struct DICIEvaluateSizeRec      DICIEvaluateSizeRec;
typedef DICIEvaluateSizeRec *           DICIEvaluateSizeRecPtr;
/*
 * paramBlock for the diCIExtendedZero call
 */

struct DICIExtendedZeroRec {
    short                           driveNumber;                /* drive number */
    StringPtr                       volNamePtr;                 /* ptr to volume name string */
    short                           fsid;                       /* file system ID */
    short                           volTypeSelector;            /* volume type selector, if supports more than 1 type */
    unsigned short                  numDefectBlocks;            /* number of bad logical blocks */
    unsigned short                  defectListSize;             /* size of the defect list buffer in bytes */
    Ptr                             defectListPtr;              /* pointer to defect list buffer */
    unsigned long                   volSize;                    /* size of volume in SECTORs */
    unsigned short                  sectorSize;                 /* bytes per sector */
    Ptr                             extendedInfoPtr;            /* ptr to extended info */
};
typedef struct DICIExtendedZeroRec      DICIExtendedZeroRec;
typedef DICIExtendedZeroRec *           DICIExtendedZeroRecPtr;
/*
 * paramBlock for the diCIValidateVolName call
 */

struct DICIValidateVolNameRec {
    char                            theChar;                    /* the character to validate */
    Boolean                         hasMessageBuffer;           /* false if no message */
    short                           charOffset;                 /* position of the current character (first char = 1) */
    StringPtr                       messageBufferPtr;           /* pointer to message buffer or nil */
    short                           charByteType;               /* theChar's byte type (smSingleByte, smFirstByte, or smLastByte) */
};
typedef struct DICIValidateVolNameRec   DICIValidateVolNameRec;
typedef DICIValidateVolNameRec *        DICIValidateVolNameRecPtr;
/*
 * paramBlock for the diCIGetVolTypeInfo call
 */

struct DICIGetVolTypeInfoRec {
    unsigned long                   volSize;                    /* size of volume in SECTORs */
    unsigned short                  sectorSize;                 /* bytes per sector */
    short                           numVolTypes;                /* number of volume types supported */
    Str31                           volTypesBuffer[4];          /* 4 string buffers */
};
typedef struct DICIGetVolTypeInfoRec    DICIGetVolTypeInfoRec;
typedef DICIGetVolTypeInfoRec *         DICIGetVolTypeInfoRecPtr;
/*
 * paramBlock for the diCIGetFormatString call
 */

struct DICIGetFormatStringRec {
    unsigned long                   volSize;                    /* volume size in SECTORs */
    unsigned short                  sectorSize;                 /* sector size */
    short                           volTypeSelector;            /* volume type selector */
    short                           stringKind;                 /* sub-function = type of string */
    Str255                          stringBuffer;               /* string buffer */
};
typedef struct DICIGetFormatStringRec   DICIGetFormatStringRec;
typedef DICIGetFormatStringRec *        DICIGetFormatStringRecPtr;
/*
 * paramBlock for the diCIGetExtendedFormatParams call
 */

struct DICIGetExtendedFormatRec {
    short                           driveNumber;                /* drive number */
    short                           volTypeSelector;            /* volume type selector or 0 */
    unsigned long                   volSize;                    /* size of volume in SECTORs */
    unsigned short                  sectorSize;                 /* bytes per sector */
    FSSpecPtr                       fileSystemSpecPtr;          /* pointer to the foreign file system's FSSpec */
    Ptr                             extendedInfoPtr;            /* pointer to extended parameter structure */
};
typedef struct DICIGetExtendedFormatRec DICIGetExtendedFormatRec;
typedef DICIGetExtendedFormatRec *      DICIGetExtendedFormatRecPtr;
/*
 *  File System Manager records
 */

typedef CALLBACK_API( OSErr , FSDCommProcPtr )(short message, void *paramBlock, void *globalsPtr);
typedef STACK_UPP_TYPE(FSDCommProcPtr)                          FSDCommUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(FSDCommUPP)
    NewFSDCommUPP                  (FSDCommProcPtr          userRoutine);

    EXTERN_API(void)
    DisposeFSDCommUPP              (FSDCommUPP              userUPP);

    EXTERN_API(OSErr)
    InvokeFSDCommUPP               (short                   message,
                                    void *                  paramBlock,
                                    void *                  globalsPtr,
                                    FSDCommUPP              userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppFSDCommProcInfo = 0x00000FA0 };                       /* pascal 2_bytes Func(2_bytes, 4_bytes, 4_bytes) */
    #define NewFSDCommUPP(userRoutine)                              (FSDCommUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppFSDCommProcInfo, GetCurrentArchitecture())
    #define DisposeFSDCommUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define InvokeFSDCommUPP(message, paramBlock, globalsPtr, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppFSDCommProcInfo, (message), (paramBlock), (globalsPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewFSDCommProc(userRoutine)                             NewFSDCommUPP(userRoutine)
#define CallFSDCommProc(userRoutine, message, paramBlock, globalsPtr) InvokeFSDCommUPP(message, paramBlock, globalsPtr, userRoutine)
typedef struct FSDRec                   FSDRec;

struct FSDRec {
    struct FSDRec *                 fsdLink;                    /* ptr to next */
    short                           fsdLength;                  /* length of this FSD in BYTES */
    short                           fsdVersion;                 /* version number */
    short                           fileSystemFSID;             /* file system id */
    Str31                           fileSystemName;             /* file system name */
    FSSpec                          fileSystemSpec;             /* foreign file system's FSSpec */
    Ptr                             fileSystemGlobalsPtr;       /* ptr to file system globals */
    FSDCommUPP                      fileSystemCommProc;         /* communication proc with the FFS */
    long                            reserved3;                  /* --reserved, must be zero-- */
    long                            reserved2;                  /* --reserved, must be zero-- */
    long                            reserved1;                  /* --reserved, must be zero-- */
    HFSCIRec                        fsdHFSCI;                   /* HFS component interface    */
    DICIRec                         fsdDICI;                    /* Disk Initialization component interface */
};


typedef FSDRec *                        FSDRecPtr;

struct FSMGetIconInfoRec {
    long                            theIcon[32];                /* The ICN# structure */
    long                            theMask[32];                /* The mask for the icon above */
    Str255                          whereStr;
};
typedef struct FSMGetIconInfoRec        FSMGetIconInfoRec;
typedef FSMGetIconInfoRec *             FSMGetIconInfoRecPtr;
/*
 * paramBlock for ffsGetIconMessage and fsmGetFSIconMessage
 */

struct FSMGetIconRec {
    short                           refNum;                     /* target drive num or volume refnum */
    FSMGetIconInfoRecPtr            iconBufferPtr;              /* pointer to icon buffer */
    long                            requestSize;                /* requested size of the icon buffer */
    long                            actualSize;                 /* actual size of the icon data returned */
    SInt8                           iconType;                   /* kind of icon */
    Boolean                         isEjectable;                /* true if the device is ejectable */
    DrvQElPtr                       driveQElemPtr;              /* pointer to DQE */
    FSSpecPtr                       fileSystemSpecPtr;          /* pointer to foreign file system's FSSpec */
    long                            reserved1;                  /* --reserved, must be zero-- */
};
typedef struct FSMGetIconRec            FSMGetIconRec;
typedef FSMGetIconRec *                 FSMGetIconRecPtr;
#if CALL_NOT_IN_CARBON
/*
 *  HFS Utility routine prototypes
 */
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
UTAllocateFCB                   (short *                fileRefNum,
                                 FCBRecPtr *            fileCtrlBlockPtr)                   TWOWORDINLINE(0x7000, 0xA824);

EXTERN_API( OSErr )
UTReleaseFCB                    (short                  fileRefNum)                         TWOWORDINLINE(0x7001, 0xA824);

EXTERN_API( OSErr )
UTLocateFCB                     (VCBPtr                 volCtrlBlockPtr,
                                 unsigned long          fileNum,
                                 StringPtr              namePtr,
                                 short *                fileRefNum,
                                 FCBRecPtr *            fileCtrlBlockPtr)                   TWOWORDINLINE(0x7002, 0xA824);

EXTERN_API( OSErr )
UTLocateNextFCB                 (VCBPtr                 volCtrlBlockPtr,
                                 unsigned long          fileNum,
                                 StringPtr              namePtr,
                                 short *                fileRefNum,
                                 FCBRecPtr *            fileCtrlBlockPtr)                   TWOWORDINLINE(0x7003, 0xA824);

EXTERN_API( OSErr )
UTIndexFCB                      (VCBPtr                 volCtrlBlockPtr,
                                 short *                fileRefNum,
                                 FCBRecPtr *            fileCtrlBlockPtr)                   TWOWORDINLINE(0x7004, 0xA824);

EXTERN_API( OSErr )
UTResolveFCB                    (short                  fileRefNum,
                                 FCBRecPtr *            fileCtrlBlockPtr)                   TWOWORDINLINE(0x7005, 0xA824);

EXTERN_API( OSErr )
UTAllocateVCB                   (unsigned short *       sysVCBLength,
                                 VCBPtr *               volCtrlBlockPtr,
                                 unsigned short         addSize)                            TWOWORDINLINE(0x7006, 0xA824);

EXTERN_API( OSErr )
UTAddNewVCB                     (short                  driveNum,
                                 short *                vRefNum,
                                 VCBPtr                 volCtrlBlockPtr)                    TWOWORDINLINE(0x7007, 0xA824);

EXTERN_API( OSErr )
UTDisposeVCB                    (VCBPtr                 volCtrlBlockPtr)                    TWOWORDINLINE(0x7008, 0xA824);

EXTERN_API( OSErr )
UTLocateVCBByRefNum             (short                  refNum,
                                 short *                vRefNum,
                                 VCBPtr *               volCtrlBlockPtr)                    TWOWORDINLINE(0x7009, 0xA824);

EXTERN_API( OSErr )
UTLocateVCBByName               (StringPtr              namePtr,
                                 short *                moreMatches,
                                 short *                vRefNum,
                                 VCBPtr *               volCtrlBlockPtr)                    TWOWORDINLINE(0x700A, 0xA824);

EXTERN_API( OSErr )
UTLocateNextVCB                 (StringPtr              namePtr,
                                 short *                moreMatches,
                                 short *                vRefNum,
                                 VCBPtr *               volCtrlBlockPtr)                    TWOWORDINLINE(0x700B, 0xA824);

EXTERN_API( OSErr )
UTAllocateWDCB                  (WDPBPtr                paramBlock)                         TWOWORDINLINE(0x700C, 0xA824);

EXTERN_API( OSErr )
UTReleaseWDCB                   (short                  wdRefNum)                           TWOWORDINLINE(0x700D, 0xA824);

EXTERN_API( OSErr )
UTResolveWDCB                   (long                   procID,
                                 short                  wdIndex,
                                 short                  wdRefNum,
                                 WDCBRecPtr *           wdCtrlBlockPtr)                     TWOWORDINLINE(0x700E, 0xA824);

EXTERN_API( OSErr )
UTFindDrive                     (short                  driveNum,
                                 DrvQElPtr *            driveQElementPtr)                   TWOWORDINLINE(0x700F, 0xA824);

EXTERN_API( OSErr )
UTAdjustEOF                     (short                  fileRefNum)                         TWOWORDINLINE(0x7010, 0xA824);

EXTERN_API( OSErr )
UTSetDefaultVol                 (long                   nodeHint,
                                 unsigned long          dirID,
                                 short                  refNum)                             TWOWORDINLINE(0x7011, 0xA824);

EXTERN_API( OSErr )
UTGetDefaultVol                 (WDPBPtr                paramBlock)                         TWOWORDINLINE(0x7012, 0xA824);

EXTERN_API( OSErr )
UTEjectVol                      (VCBPtr                 volCtrlBlockPtr)                    TWOWORDINLINE(0x702B, 0xA824);

EXTERN_API( OSErr )
UTCheckWDRefNum                 (short                  wdRefNum)                           TWOWORDINLINE(0x7013, 0xA824);

EXTERN_API( OSErr )
UTCheckFileRefNum               (short                  fileRefNum)                         TWOWORDINLINE(0x7014, 0xA824);

EXTERN_API( OSErr )
UTCheckVolRefNum                (short                  vRefNum)                            TWOWORDINLINE(0x7015, 0xA824);

EXTERN_API( OSErr )
UTCheckPermission               (VCBPtr                 volCtrlBlockPtr,
                                 short *                modByte,
                                 unsigned long          fileNum,
                                 ParmBlkPtr             paramBlock)                         TWOWORDINLINE(0x7016, 0xA824);

EXTERN_API( OSErr )
UTCheckVolOffline               (short                  vRefNum)                            TWOWORDINLINE(0x7017, 0xA824);

EXTERN_API( OSErr )
UTCheckVolModifiable            (short                  vRefNum)                            TWOWORDINLINE(0x7018, 0xA824);

EXTERN_API( OSErr )
UTCheckFileModifiable           (short                  fileRefNum)                         TWOWORDINLINE(0x7019, 0xA824);

EXTERN_API( OSErr )
UTCheckDirBusy                  (VCBPtr                 volCtrlBlockPtr,
                                 unsigned long          dirID)                              TWOWORDINLINE(0x701A, 0xA824);

EXTERN_API( OSErr )
UTParsePathname                 (short *                volNamelength,
                                 StringPtr              namePtr)                            TWOWORDINLINE(0x701B, 0xA824);

EXTERN_API( OSErr )
UTGetPathComponentName          (ParsePathRecPtr        parseRec)                           TWOWORDINLINE(0x701C, 0xA824);

EXTERN_API( OSErr )
UTDetermineVol                  (ParmBlkPtr             paramBlock,
                                 short *                status,
                                 short *                moreMatches,
                                 short *                vRefNum,
                                 VCBPtr *               volCtrlBlockPtr)                    TWOWORDINLINE(0x701D, 0xA824);

EXTERN_API( OSErr )
UTGetBlock                      (short                  refNum,
                                 void *                 log2PhyProc,
                                 unsigned long          blockNum,
                                 short                  gbOption,
                                 Ptr *                  buffer)                             TWOWORDINLINE(0x701F, 0xA824);

EXTERN_API( OSErr )
UTReleaseBlock                  (Ptr                    buffer,
                                 short                  rbOption)                           TWOWORDINLINE(0x7020, 0xA824);

EXTERN_API( OSErr )
UTFlushCache                    (short                  refNum,
                                 short                  fcOption)                           TWOWORDINLINE(0x7021, 0xA824);

EXTERN_API( OSErr )
UTMarkDirty                     (Ptr                    buffer)                             TWOWORDINLINE(0x7023, 0xA824);

EXTERN_API( OSErr )
UTTrashVolBlocks                (VCBPtr                 volCtrlBlockPtr)                    TWOWORDINLINE(0x7024, 0xA824);

EXTERN_API( OSErr )
UTTrashFileBlocks               (VCBPtr                 volCtrlBlockPtr,
                                 unsigned long          fileNum)                            TWOWORDINLINE(0x7025, 0xA824);

EXTERN_API( OSErr )
UTTrashBlocks                   (unsigned long          beginPosition,
                                 unsigned long          byteCount,
                                 VCBPtr                 volCtrlBlockPtr,
                                 short                  fileRefNum,
                                 short                  tbOption)                           TWOWORDINLINE(0x7026, 0xA824);

EXTERN_API( OSErr )
UTCacheReadIP                   (void *                 log2PhyProc,
                                 unsigned long          filePosition,
                                 Ptr                    ioBuffer,
                                 short                  fileRefNum,
                                 unsigned long          reqCount,
                                 unsigned long *        actCount,
                                 short                  cacheOption)                        TWOWORDINLINE(0x7027, 0xA824);

EXTERN_API( OSErr )
UTCacheWriteIP                  (void *                 log2PhyProc,
                                 unsigned long          filePosition,
                                 Ptr                    ioBuffer,
                                 short                  fileRefNum,
                                 unsigned long          reqCount,
                                 unsigned long *        actCount,
                                 short                  cacheOption)                        TWOWORDINLINE(0x7028, 0xA824);

EXTERN_API( OSErr )
UTBlockInFQHashP                (short                  vRefNum,
                                 unsigned long          diskBlock)                          TWOWORDINLINE(0x702C, 0xA824);

EXTERN_API( OSErr )
UTVolCacheReadIP                (VCBPtr                 volCtrlBlockPtr,
                                 unsigned long          blockPosition,
                                 Ptr                    ioBuffer,
                                 unsigned long          reqCount,
                                 unsigned long *        actCount,
                                 short                  cacheOption)                        TWOWORDINLINE(0x7034, 0xA824);

EXTERN_API( OSErr )
UTVolCacheWriteIP               (VCBPtr                 volCtrlBlockPtr,
                                 unsigned long          blockPosition,
                                 Ptr                    ioBuffer,
                                 unsigned long          reqCount,
                                 unsigned long *        actCount,
                                 short                  cacheOption)                        TWOWORDINLINE(0x7035, 0xA824);

EXTERN_API( OSErr )
UTResolveFileRefNum             (FCBRecPtr              fileCtrlBlockPtr,
                                 short *                fileRefNum)                         TWOWORDINLINE(0x7036, 0xA824);

EXTERN_API( OSErr )
UTCheckFCB                      (FCBRecPtr              fileCtrlBlockPtr)                   TWOWORDINLINE(0x7037, 0xA824);

EXTERN_API( OSErr )
UTCheckForkPermissions          (VCBPtr                 volCtrlBlockPtr,
                                 UInt32                 fileNum,
                                 UInt32                 forkID,
                                 Boolean                fileLocked,
                                 SInt8                  permissions,
                                 Boolean                useSearchList,
                                 SInt8 *                fcbFlags,
                                 SInt16 *               openForkRefNum)                     TWOWORDINLINE(0x7038, 0xA824);

EXTERN_API( OSErr )
UTAddFCBToSearchList            (FCBRecPtr              fileCtrlBlockPtr)                   TWOWORDINLINE(0x7039, 0xA824);

EXTERN_API( OSErr )
UTRemoveFCBFromSearchList       (FCBRecPtr              fileCtrlBlockPtr)                   TWOWORDINLINE(0x703A, 0xA824);

EXTERN_API( OSErr )
UTLocateFCBInSearchList         (VCBPtr                 volCtrlBlockPtr,
                                 unsigned long          fileNum,
                                 short *                fileRefNum,
                                 FCBRecPtr *            fileCtrlBlockPtr)                   TWOWORDINLINE(0x703B, 0xA824);

EXTERN_API( OSErr )
UTGetForkControlBlockSize       (UInt32 *               fcbSize)                            TWOWORDINLINE(0x703C, 0xA824);


/*
 *  File System Manager call prototypes
 */
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 InstallFS(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
InstallFS                       (FSDRecPtr              fsdPtr)                             TWOWORDINLINE(0x7000, 0xA0AC);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter __D0 RemoveFS(__A0)
                                                                                            #endif
EXTERN_API( OSErr )
RemoveFS                        (short                  fsid)                               TWOWORDINLINE(0x7001, 0xA0AC);

EXTERN_API( OSErr )
SetFSInfo                       (short                  fsid,
                                 short                  bufSize,
                                 FSDRecPtr              fsdPtr);

EXTERN_API( OSErr )
GetFSInfo                       (short                  selector,
                                 short                  key,
                                 short *                bufSize,
                                 FSDRecPtr              fsdPtr);

EXTERN_API( OSErr )
InformFSM                       (short                  theMessage,
                                 void *                 paramBlock);

EXTERN_API( OSErr )
InformFFS                       (short                  fsid,
                                 void *                 paramBlock);

#endif  /* CALL_NOT_IN_CARBON */

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

#endif /* __FSM__ */

