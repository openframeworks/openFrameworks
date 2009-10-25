/*
     File:       MacErrors.h
 
     Contains:   OSErr codes.
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MACERRORS__
#define __MACERRORS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
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
    paramErr                    = -50,                          /*error in user parameter list*/
    noHardwareErr               = -200,                         /*Sound Manager Error Returns*/
    notEnoughHardwareErr        = -201,                         /*Sound Manager Error Returns*/
    userCanceledErr             = -128,
    qErr                        = -1,                           /*queue element not found during deletion*/
    vTypErr                     = -2,                           /*invalid queue element*/
    corErr                      = -3,                           /*core routine number out of range*/
    unimpErr                    = -4,                           /*unimplemented core routine*/
    SlpTypeErr                  = -5,                           /*invalid queue element*/
    seNoDB                      = -8,                           /*no debugger installed to handle debugger command*/
    controlErr                  = -17,                          /*I/O System Errors*/
    statusErr                   = -18,                          /*I/O System Errors*/
    readErr                     = -19,                          /*I/O System Errors*/
    writErr                     = -20,                          /*I/O System Errors*/
    badUnitErr                  = -21,                          /*I/O System Errors*/
    unitEmptyErr                = -22,                          /*I/O System Errors*/
    openErr                     = -23,                          /*I/O System Errors*/
    closErr                     = -24,                          /*I/O System Errors*/
    dRemovErr                   = -25,                          /*tried to remove an open driver*/
    dInstErr                    = -26                           /*DrvrInstall couldn't find driver in resources*/
};

enum {
    abortErr                    = -27,                          /*IO call aborted by KillIO*/
    iIOAbortErr                 = -27,                          /*IO abort error (Printing Manager)*/
    notOpenErr                  = -28,                          /*Couldn't rd/wr/ctl/sts cause driver not opened*/
    unitTblFullErr              = -29,                          /*unit table has no more entries*/
    dceExtErr                   = -30,                          /*dce extension error*/
    slotNumErr                  = -360,                         /*invalid slot # error*/
    gcrOnMFMErr                 = -400,                         /*gcr format on high density media error*/
    dirFulErr                   = -33,                          /*Directory full*/
    dskFulErr                   = -34,                          /*disk full*/
    nsvErr                      = -35,                          /*no such volume*/
    ioErr                       = -36,                          /*I/O error (bummers)*/
    bdNamErr                    = -37,                          /*there may be no bad names in the final system!*/
    fnOpnErr                    = -38,                          /*File not open*/
    eofErr                      = -39,                          /*End of file*/
    posErr                      = -40,                          /*tried to position to before start of file (r/w)*/
    mFulErr                     = -41,                          /*memory full (open) or file won't fit (load)*/
    tmfoErr                     = -42,                          /*too many files open*/
    fnfErr                      = -43,                          /*File not found*/
    wPrErr                      = -44,                          /*diskette is write protected.*/
    fLckdErr                    = -45                           /*file is locked*/
};

enum {
    vLckdErr                    = -46,                          /*volume is locked*/
    fBsyErr                     = -47,                          /*File is busy (delete)*/
    dupFNErr                    = -48,                          /*duplicate filename (rename)*/
    opWrErr                     = -49,                          /*file already open with with write permission*/
    rfNumErr                    = -51,                          /*refnum error*/
    gfpErr                      = -52,                          /*get file position error*/
    volOffLinErr                = -53,                          /*volume not on line error (was Ejected)*/
    permErr                     = -54,                          /*permissions error (on file open)*/
    volOnLinErr                 = -55,                          /*drive volume already on-line at MountVol*/
    nsDrvErr                    = -56,                          /*no such drive (tried to mount a bad drive num)*/
    noMacDskErr                 = -57,                          /*not a mac diskette (sig bytes are wrong)*/
    extFSErr                    = -58,                          /*volume in question belongs to an external fs*/
    fsRnErr                     = -59,                          /*file system internal error:during rename the old entry was deleted but could not be restored.*/
    badMDBErr                   = -60,                          /*bad master directory block*/
    wrPermErr                   = -61,                          /*write permissions error*/
    dirNFErr                    = -120,                         /*Directory not found*/
    tmwdoErr                    = -121,                         /*No free WDCB available*/
    badMovErr                   = -122,                         /*Move into offspring error*/
    wrgVolTypErr                = -123,                         /*Wrong volume type error [operation not supported for MFS]*/
    volGoneErr                  = -124                          /*Server volume has been disconnected.*/
};

enum {
    fidNotFound                 = -1300,                        /*no file thread exists.*/
    fidExists                   = -1301,                        /*file id already exists*/
    notAFileErr                 = -1302,                        /*directory specified*/
    diffVolErr                  = -1303,                        /*files on different volumes*/
    catChangedErr               = -1304,                        /*the catalog has been modified*/
    desktopDamagedErr           = -1305,                        /*desktop database files are corrupted*/
    sameFileErr                 = -1306,                        /*can't exchange a file with itself*/
    badFidErr                   = -1307,                        /*file id is dangling or doesn't match with the file number*/
    notARemountErr              = -1308,                        /*when _Mount allows only remounts and doesn't get one*/
    fileBoundsErr               = -1309,                        /*file's EOF, offset, mark or size is too big*/
    fsDataTooBigErr             = -1310,                        /*file or volume is too big for system*/
    volVMBusyErr                = -1311,                        /*can't eject because volume is in use by VM*/
    badFCBErr                   = -1327,                        /*FCBRecPtr is not valid*/
    errFSUnknownCall            = -1400,                        /* selector is not recognized by this filesystem */
    errFSBadFSRef               = -1401,                        /* FSRef parameter is bad */
    errFSBadForkName            = -1402,                        /* Fork name parameter is bad */
    errFSBadBuffer              = -1403,                        /* A buffer parameter was bad */
    errFSBadForkRef             = -1404,                        /* A ForkRefNum parameter was bad */
    errFSBadInfoBitmap          = -1405,                        /* A CatalogInfoBitmap or VolumeInfoBitmap has reserved or invalid bits set */
    errFSMissingCatInfo         = -1406,                        /* A CatalogInfo parameter was NULL */
    errFSNotAFolder             = -1407,                        /* Expected a folder, got a file */
    errFSForkNotFound           = -1409,                        /* Named fork does not exist */
    errFSNameTooLong            = -1410,                        /* File/fork name is too long to create/rename */
    errFSMissingName            = -1411,                        /* A Unicode name parameter was NULL or nameLength parameter was zero */
    errFSBadPosMode             = -1412,                        /* Newline bits set in positionMode */
    errFSBadAllocFlags          = -1413,                        /* Invalid bits set in allocationFlags */
    errFSNoMoreItems            = -1417,                        /* Iteration ran out of items to return */
    errFSBadItemCount           = -1418,                        /* maximumItems was zero */
    errFSBadSearchParams        = -1419,                        /* Something wrong with CatalogSearch searchParams */
    errFSRefsDifferent          = -1420,                        /* FSCompareFSRefs; refs are for different objects */
    errFSForkExists             = -1421,                        /* Named fork already exists. */
    errFSBadIteratorFlags       = -1422,                        /* Flags passed to FSOpenIterator are bad */
    errFSIteratorNotFound       = -1423,                        /* Passed FSIterator is not an open iterator */
    errFSIteratorNotSupported   = -1424,                        /* The iterator's flags or container are not supported by this call */
    envNotPresent               = -5500,                        /*returned by glue.*/
    envBadVers                  = -5501,                        /*Version non-positive*/
    envVersTooBig               = -5502,                        /*Version bigger than call can handle*/
    fontDecError                = -64,                          /*error during font declaration*/
    fontNotDeclared             = -65,                          /*font not declared*/
    fontSubErr                  = -66,                          /*font substitution occurred*/
    fontNotOutlineErr           = -32615,                       /*bitmap font passed to routine that does outlines only*/
    firstDskErr                 = -84,                          /*I/O System Errors*/
    lastDskErr                  = -64,                          /*I/O System Errors*/
    noDriveErr                  = -64,                          /*drive not installed*/
    offLinErr                   = -65,                          /*r/w requested for an off-line drive*/
    noNybErr                    = -66                           /*couldn't find 5 nybbles in 200 tries*/
};

enum {
    noAdrMkErr                  = -67,                          /*couldn't find valid addr mark*/
    dataVerErr                  = -68,                          /*read verify compare failed*/
    badCksmErr                  = -69,                          /*addr mark checksum didn't check*/
    badBtSlpErr                 = -70,                          /*bad addr mark bit slip nibbles*/
    noDtaMkErr                  = -71,                          /*couldn't find a data mark header*/
    badDCksum                   = -72,                          /*bad data mark checksum*/
    badDBtSlp                   = -73,                          /*bad data mark bit slip nibbles*/
    wrUnderrun                  = -74,                          /*write underrun occurred*/
    cantStepErr                 = -75,                          /*step handshake failed*/
    tk0BadErr                   = -76,                          /*track 0 detect doesn't change*/
    initIWMErr                  = -77,                          /*unable to initialize IWM*/
    twoSideErr                  = -78,                          /*tried to read 2nd side on a 1-sided drive*/
    spdAdjErr                   = -79,                          /*unable to correctly adjust disk speed*/
    seekErr                     = -80,                          /*track number wrong on address mark*/
    sectNFErr                   = -81,                          /*sector number never found on a track*/
    fmt1Err                     = -82,                          /*can't find sector 0 after track format*/
    fmt2Err                     = -83,                          /*can't get enough sync*/
    verErr                      = -84,                          /*track failed to verify*/
    clkRdErr                    = -85,                          /*unable to read same clock value twice*/
    clkWrErr                    = -86,                          /*time written did not verify*/
    prWrErr                     = -87,                          /*parameter ram written didn't read-verify*/
    prInitErr                   = -88,                          /*InitUtil found the parameter ram uninitialized*/
    rcvrErr                     = -89,                          /*SCC receiver error (framing; parity; OR)*/
    breakRecd                   = -90                           /*Break received (SCC)*/
};

enum {
                                                                /*Scrap Manager errors*/
    noScrapErr                  = -100,                         /*No scrap exists error*/
    noTypeErr                   = -102                          /*No object of that type in scrap*/
};

enum {
                                                                /* ENET error codes */
    eLenErr                     = -92,                          /*Length error ddpLenErr*/
    eMultiErr                   = -91                           /*Multicast address error ddpSktErr*/
};

enum {
    ddpSktErr                   = -91,                          /*error in soket number*/
    ddpLenErr                   = -92,                          /*data length too big*/
    noBridgeErr                 = -93,                          /*no network bridge for non-local send*/
    lapProtErr                  = -94,                          /*error in attaching/detaching protocol*/
    excessCollsns               = -95,                          /*excessive collisions on write*/
    portNotPwr                  = -96,                          /*serial port not currently powered*/
    portInUse                   = -97,                          /*driver Open error code (port is in use)*/
    portNotCf                   = -98                           /*driver Open error code (parameter RAM not configured for this connection)*/
};

enum {
                                                                /* Memory Manager errors*/
    memROZWarn                  = -99,                          /*soft error in ROZ*/
    memROZError                 = -99,                          /*hard error in ROZ*/
    memROZErr                   = -99,                          /*hard error in ROZ*/
    memFullErr                  = -108,                         /*Not enough room in heap zone*/
    nilHandleErr                = -109,                         /*Master Pointer was NIL in HandleZone or other*/
    memWZErr                    = -111,                         /*WhichZone failed (applied to free block)*/
    memPurErr                   = -112,                         /*trying to purge a locked or non-purgeable block*/
    memAdrErr                   = -110,                         /*address was odd; or out of range*/
    memAZErr                    = -113,                         /*Address in zone check failed*/
    memPCErr                    = -114,                         /*Pointer Check failed*/
    memBCErr                    = -115,                         /*Block Check failed*/
    memSCErr                    = -116,                         /*Size Check failed*/
    memLockedErr                = -117                          /*trying to move a locked block (MoveHHi)*/
};

enum {
                                                                /* Printing Errors */
    iMemFullErr                 = -108,
    iIOAbort                    = -27
};


enum {
    resourceInMemory            = -188,                         /*Resource already in memory*/
    writingPastEnd              = -189,                         /*Writing past end of file*/
    inputOutOfBounds            = -190,                         /*Offset of Count out of bounds*/
    resNotFound                 = -192,                         /*Resource not found*/
    resFNotFound                = -193,                         /*Resource file not found*/
    addResFailed                = -194,                         /*AddResource failed*/
    addRefFailed                = -195,                         /*AddReference failed*/
    rmvResFailed                = -196,                         /*RmveResource failed*/
    rmvRefFailed                = -197,                         /*RmveReference failed*/
    resAttrErr                  = -198,                         /*attribute inconsistent with operation*/
    mapReadErr                  = -199,                         /*map inconsistent with operation*/
    CantDecompress              = -186,                         /*resource bent ("the bends") - can't decompress a compressed resource*/
    badExtResource              = -185,                         /*extended resource has a bad format.*/
    noMemForPictPlaybackErr     = -145,
    rgnOverflowErr              = -147,
    rgnTooBigError              = -147,
    pixMapTooDeepErr            = -148,
    insufficientStackErr        = -149,
    nsStackErr                  = -149
};

enum {
    evtNotEnb                   = 1                             /*event not enabled at PostEvent*/
};

/* OffScreen QuickDraw Errors */
enum {
    cMatchErr                   = -150,                         /*Color2Index failed to find an index*/
    cTempMemErr                 = -151,                         /*failed to allocate memory for temporary structures*/
    cNoMemErr                   = -152,                         /*failed to allocate memory for structure*/
    cRangeErr                   = -153,                         /*range error on colorTable request*/
    cProtectErr                 = -154,                         /*colorTable entry protection violation*/
    cDevErr                     = -155,                         /*invalid type of graphics device*/
    cResErr                     = -156,                         /*invalid resolution for MakeITable*/
    cDepthErr                   = -157,                         /*invalid pixel depth */
    rgnTooBigErr                = -500,
    updPixMemErr                = -125,                         /*insufficient memory to update a pixmap*/
    pictInfoVersionErr          = -11000,                       /*wrong version of the PictInfo structure*/
    pictInfoIDErr               = -11001,                       /*the internal consistancy check for the PictInfoID is wrong*/
    pictInfoVerbErr             = -11002,                       /*the passed verb was invalid*/
    cantLoadPickMethodErr       = -11003,                       /*unable to load the custom pick proc*/
    colorsRequestedErr          = -11004,                       /*the number of colors requested was illegal*/
    pictureDataErr              = -11005                        /*the picture data was invalid*/
};

/* ColorSync Result codes */
enum {
                                                                /* General Errors */
    cmProfileError              = -170,
    cmMethodError               = -171,
    cmMethodNotFound            = -175,                         /* CMM not present */
    cmProfileNotFound           = -176,                         /* Responder error */
    cmProfilesIdentical         = -177,                         /* Profiles the same */
    cmCantConcatenateError      = -178,                         /* Profile can't be concatenated */
    cmCantXYZ                   = -179,                         /* CMM cant handle XYZ space */
    cmCantDeleteProfile         = -180,                         /* Responder error */
    cmUnsupportedDataType       = -181,                         /* Responder error */
    cmNoCurrentProfile          = -182                          /* Responder error */
};


enum {
                                                                /*Sound Manager errors*/
    noHardware                  = noHardwareErr,                /*obsolete spelling*/
    notEnoughHardware           = notEnoughHardwareErr,         /*obsolete spelling*/
    queueFull                   = -203,                         /*Sound Manager Error Returns*/
    resProblem                  = -204,                         /*Sound Manager Error Returns*/
    badChannel                  = -205,                         /*Sound Manager Error Returns*/
    badFormat                   = -206,                         /*Sound Manager Error Returns*/
    notEnoughBufferSpace        = -207,                         /*could not allocate enough memory*/
    badFileFormat               = -208,                         /*was not type AIFF or was of bad format,corrupt*/
    channelBusy                 = -209,                         /*the Channel is being used for a PFD already*/
    buffersTooSmall             = -210,                         /*can not operate in the memory allowed*/
    channelNotBusy              = -211,
    noMoreRealTime              = -212,                         /*not enough CPU cycles left to add another task*/
    siVBRCompressionNotSupported = -213,                        /*vbr audio compression not supported for this operation*/
    siNoSoundInHardware         = -220,                         /*no Sound Input hardware*/
    siBadSoundInDevice          = -221,                         /*invalid index passed to SoundInGetIndexedDevice*/
    siNoBufferSpecified         = -222,                         /*returned by synchronous SPBRecord if nil buffer passed*/
    siInvalidCompression        = -223,                         /*invalid compression type*/
    siHardDriveTooSlow          = -224,                         /*hard drive too slow to record to disk*/
    siInvalidSampleRate         = -225,                         /*invalid sample rate*/
    siInvalidSampleSize         = -226,                         /*invalid sample size*/
    siDeviceBusyErr             = -227,                         /*input device already in use*/
    siBadDeviceName             = -228,                         /*input device could not be opened*/
    siBadRefNum                 = -229,                         /*invalid input device reference number*/
    siInputDeviceErr            = -230,                         /*input device hardware failure*/
    siUnknownInfoType           = -231,                         /*invalid info type selector (returned by driver)*/
    siUnknownQuality            = -232                          /*invalid quality selector (returned by driver)*/
};

/*Speech Manager errors*/
enum {
    noSynthFound                = -240,
    synthOpenFailed             = -241,
    synthNotReady               = -242,
    bufTooSmall                 = -243,
    voiceNotFound               = -244,
    incompatibleVoice           = -245,
    badDictFormat               = -246,
    badInputText                = -247
};

/* Midi Manager Errors: */
enum {
    midiNoClientErr             = -250,                         /*no client with that ID found*/
    midiNoPortErr               = -251,                         /*no port with that ID found*/
    midiTooManyPortsErr         = -252,                         /*too many ports already installed in the system*/
    midiTooManyConsErr          = -253,                         /*too many connections made*/
    midiVConnectErr             = -254,                         /*pending virtual connection created*/
    midiVConnectMade            = -255,                         /*pending virtual connection resolved*/
    midiVConnectRmvd            = -256,                         /*pending virtual connection removed*/
    midiNoConErr                = -257,                         /*no connection exists between specified ports*/
    midiWriteErr                = -258,                         /*MIDIWritePacket couldn't write to all connected ports*/
    midiNameLenErr              = -259,                         /*name supplied is longer than 31 characters*/
    midiDupIDErr                = -260,                         /*duplicate client ID*/
    midiInvalidCmdErr           = -261                          /*command not supported for port type*/
};


enum {
    nmTypErr                    = -299                          /*Notification Manager:wrong queue type*/
};


enum {
    siInitSDTblErr              = 1,                            /*slot int dispatch table could not be initialized.*/
    siInitVBLQsErr              = 2,                            /*VBLqueues for all slots could not be initialized.*/
    siInitSPTblErr              = 3,                            /*slot priority table could not be initialized.*/
    sdmJTInitErr                = 10,                           /*SDM Jump Table could not be initialized.*/
    sdmInitErr                  = 11,                           /*SDM could not be initialized.*/
    sdmSRTInitErr               = 12,                           /*Slot Resource Table could not be initialized.*/
    sdmPRAMInitErr              = 13,                           /*Slot PRAM could not be initialized.*/
    sdmPriInitErr               = 14                            /*Cards could not be initialized.*/
};

enum {
    smSDMInitErr                = -290,                         /*Error; SDM could not be initialized.*/
    smSRTInitErr                = -291,                         /*Error; Slot Resource Table could not be initialized.*/
    smPRAMInitErr               = -292,                         /*Error; Slot Resource Table could not be initialized.*/
    smPriInitErr                = -293,                         /*Error; Cards could not be initialized.*/
    smEmptySlot                 = -300,                         /*No card in slot*/
    smCRCFail                   = -301,                         /*CRC check failed for declaration data*/
    smFormatErr                 = -302,                         /*FHeader Format is not Apple's*/
    smRevisionErr               = -303,                         /*Wrong revison level*/
    smNoDir                     = -304,                         /*Directory offset is Nil*/
    smDisabledSlot              = -305,                         /*This slot is disabled (-305 use to be smLWTstBad)*/
    smNosInfoArray              = -306                          /*No sInfoArray. Memory Mgr error.*/
};


enum {
    smResrvErr                  = -307,                         /*Fatal reserved error. Resreved field <> 0.*/
    smUnExBusErr                = -308,                         /*Unexpected BusError*/
    smBLFieldBad                = -309,                         /*ByteLanes field was bad.*/
    smFHBlockRdErr              = -310,                         /*Error occurred during _sGetFHeader.*/
    smFHBlkDispErr              = -311,                         /*Error occurred during _sDisposePtr (Dispose of FHeader block).*/
    smDisposePErr               = -312,                         /*_DisposePointer error*/
    smNoBoardSRsrc              = -313,                         /*No Board sResource.*/
    smGetPRErr                  = -314,                         /*Error occurred during _sGetPRAMRec (See SIMStatus).*/
    smNoBoardId                 = -315,                         /*No Board Id.*/
    smInitStatVErr              = -316,                         /*The InitStatusV field was negative after primary or secondary init.*/
    smInitTblVErr               = -317,                         /*An error occurred while trying to initialize the Slot Resource Table.*/
    smNoJmpTbl                  = -318,                         /*SDM jump table could not be created.*/
    smReservedSlot              = -318,                         /*slot is reserved, VM should not use this address space.*/
    smBadBoardId                = -319,                         /*BoardId was wrong; re-init the PRAM record.*/
    smBusErrTO                  = -320,                         /*BusError time out.*/
                                                                /* These errors are logged in the  vendor status field of the sInfo record. */
    svTempDisable               = -32768L,                      /*Temporarily disable card but run primary init.*/
    svDisabled                  = -32640,                       /*Reserve range -32640 to -32768 for Apple temp disables.*/
    smBadRefId                  = -330,                         /*Reference Id not found in List*/
    smBadsList                  = -331,                         /*Bad sList: Id1 < Id2 < Id3 ...format is not followed.*/
    smReservedErr               = -332,                         /*Reserved field not zero*/
    smCodeRevErr                = -333                          /*Code revision is wrong*/
};

enum {
    smCPUErr                    = -334,                         /*Code revision is wrong*/
    smsPointerNil               = -335,                         /*LPointer is nil From sOffsetData. If this error occurs; check sInfo rec for more information.*/
    smNilsBlockErr              = -336,                         /*Nil sBlock error (Dont allocate and try to use a nil sBlock)*/
    smSlotOOBErr                = -337,                         /*Slot out of bounds error*/
    smSelOOBErr                 = -338,                         /*Selector out of bounds error*/
    smNewPErr                   = -339,                         /*_NewPtr error*/
    smBlkMoveErr                = -340,                         /*_BlockMove error*/
    smCkStatusErr               = -341,                         /*Status of slot = fail.*/
    smGetDrvrNamErr             = -342,                         /*Error occurred during _sGetDrvrName.*/
    smDisDrvrNamErr             = -343,                         /*Error occurred during _sDisDrvrName.*/
    smNoMoresRsrcs              = -344,                         /*No more sResources*/
    smsGetDrvrErr               = -345,                         /*Error occurred during _sGetDriver.*/
    smBadsPtrErr                = -346,                         /*Bad pointer was passed to sCalcsPointer*/
    smByteLanesErr              = -347,                         /*NumByteLanes was determined to be zero.*/
    smOffsetErr                 = -348,                         /*Offset was too big (temporary error*/
    smNoGoodOpens               = -349,                         /*No opens were successfull in the loop.*/
    smSRTOvrFlErr               = -350,                         /*SRT over flow.*/
    smRecNotFnd                 = -351                          /*Record not found in the SRT.*/
};


enum {
                                                                /*Dictionary Manager errors*/
    notBTree                    = -410,                         /*The file is not a dictionary.*/
    btNoSpace                   = -413,                         /*Can't allocate disk space.*/
    btDupRecErr                 = -414,                         /*Record already exists.*/
    btRecNotFnd                 = -415,                         /*Record cannot be found.*/
    btKeyLenErr                 = -416,                         /*Maximum key length is too long or equal to zero.*/
    btKeyAttrErr                = -417,                         /*There is no such a key attribute.*/
    unknownInsertModeErr        = -20000,                       /*There is no such an insert mode.*/
    recordDataTooBigErr         = -20001,                       /*The record data is bigger than buffer size (1024 bytes).*/
    invalidIndexErr             = -20002                        /*The recordIndex parameter is not valid.*/
};


/*
 * Error codes from FSM functions
 */
enum {
    fsmFFSNotFoundErr           = -431,                         /* Foreign File system does not exist - new Pack2 could return this error too */
    fsmBusyFFSErr               = -432,                         /* File system is busy, cannot be removed */
    fsmBadFFSNameErr            = -433,                         /* Name length not 1 <= length <= 31 */
    fsmBadFSDLenErr             = -434,                         /* FSD size incompatible with current FSM vers */
    fsmDuplicateFSIDErr         = -435,                         /* FSID already exists on InstallFS */
    fsmBadFSDVersionErr         = -436,                         /* FSM version incompatible with FSD */
    fsmNoAlternateStackErr      = -437,                         /* no alternate stack for HFS CI */
    fsmUnknownFSMMessageErr     = -438                          /* unknown message passed to FSM */
};


enum {
                                                                /* Edition Mgr errors*/
    editionMgrInitErr           = -450,                         /*edition manager not inited by this app*/
    badSectionErr               = -451,                         /*not a valid SectionRecord*/
    notRegisteredSectionErr     = -452,                         /*not a registered SectionRecord*/
    badEditionFileErr           = -453,                         /*edition file is corrupt*/
    badSubPartErr               = -454,                         /*can not use sub parts in this release*/
    multiplePublisherWrn        = -460,                         /*A Publisher is already registered for that container*/
    containerNotFoundWrn        = -461,                         /*could not find editionContainer at this time*/
    containerAlreadyOpenWrn     = -462,                         /*container already opened by this section*/
    notThePublisherWrn          = -463                          /*not the first registered publisher for that container*/
};

enum {
    teScrapSizeErr              = -501,                         /*scrap item too big for text edit record*/
    hwParamErr                  = -502,                         /*bad selector for _HWPriv*/
    driverHardwareGoneErr       = -503                          /*disk driver's hardware was disconnected*/
};

enum {
                                                                /*Process Manager errors*/
    procNotFound                = -600,                         /*no eligible process with specified descriptor*/
    memFragErr                  = -601,                         /*not enough room to launch app w/special requirements*/
    appModeErr                  = -602,                         /*memory mode is 32-bit, but app not 32-bit clean*/
    protocolErr                 = -603,                         /*app made module calls in improper order*/
    hardwareConfigErr           = -604,                         /*hardware configuration not correct for call*/
    appMemFullErr               = -605,                         /*application SIZE not big enough for launch*/
    appIsDaemon                 = -606,                         /*app is BG-only, and launch flags disallow this*/
    bufferIsSmall               = -607,                         /*error returns from Post and Accept */
    noOutstandingHLE            = -608,
    connectionInvalid           = -609,
    noUserInteractionAllowed    = -610                          /* no user interaction allowed */
};

enum {
                                                                /* More Process Manager errors */
    wrongApplicationPlatform    = -875,                         /* The application could not launch because the required platform is not available    */
    appVersionTooOld            = -876                          /* The application's creator and version are incompatible with the current version of Mac OS. */
};

/* Thread Manager Error Codes */
enum {
    threadTooManyReqsErr        = -617,
    threadNotFoundErr           = -618,
    threadProtocolErr           = -619
};

/*MemoryDispatch errors*/
enum {
    notEnoughMemoryErr          = -620,                         /*insufficient physical memory*/
    notHeldErr                  = -621,                         /*specified range of memory is not held*/
    cannotMakeContiguousErr     = -622,                         /*cannot make specified range contiguous*/
    notLockedErr                = -623,                         /*specified range of memory is not locked*/
    interruptsMaskedErr         = -624,                         /*don't call with interrupts masked*/
    cannotDeferErr              = -625,                         /*unable to defer additional functions*/
    noMMUErr                    = -626                          /*no MMU present*/
};

enum {
                                                                /* Database access error codes */
    rcDBNull                    = -800,
    rcDBValue                   = -801,
    rcDBError                   = -802,
    rcDBBadType                 = -803,
    rcDBBreak                   = -804,
    rcDBExec                    = -805,
    rcDBBadSessID               = -806,
    rcDBBadSessNum              = -807,                         /* bad session number for DBGetConnInfo */
    rcDBBadDDEV                 = -808,                         /* bad ddev specified on DBInit */
    rcDBAsyncNotSupp            = -809,                         /* ddev does not support async calls */
    rcDBBadAsyncPB              = -810,                         /* tried to kill a bad pb */
    rcDBNoHandler               = -811,                         /* no app handler for specified data type */
    rcDBWrongVersion            = -812,                         /* incompatible versions */
    rcDBPackNotInited           = -813                          /* attempt to call other routine before InitDBPack */
};


/*Help Mgr error range: -850 to -874*/
enum {
    hmHelpDisabled              = -850,                         /* Show Balloons mode was off, call to routine ignored */
    hmBalloonAborted            = -853,                         /* Returned if mouse was moving or mouse wasn't in window port rect */
    hmSameAsLastBalloon         = -854,                         /* Returned from HMShowMenuBalloon if menu & item is same as last time */
    hmHelpManagerNotInited      = -855,                         /* Returned from HMGetHelpMenuHandle if help menu not setup */
    hmSkippedBalloon            = -857,                         /* Returned from calls if helpmsg specified a skip balloon */
    hmWrongVersion              = -858,                         /* Returned if help mgr resource was the wrong version */
    hmUnknownHelpType           = -859,                         /* Returned if help msg record contained a bad type */
    hmOperationUnsupported      = -861,                         /* Returned from HMShowBalloon call if bad method passed to routine */
    hmNoBalloonUp               = -862,                         /* Returned from HMRemoveBalloon if no balloon was visible when call was made */
    hmCloseViewActive           = -863                          /* Returned from HMRemoveBalloon if CloseView was active */
};



enum {
                                                                /*PPC errors*/
    notInitErr                  = -900,                         /*PPCToolBox not initialized*/
    nameTypeErr                 = -902,                         /*Invalid or inappropriate locationKindSelector in locationName*/
    noPortErr                   = -903,                         /*Unable to open port or bad portRefNum.  If you're calling */
                                                                /* AESend, this is because your application does not have */
                                                                /* the isHighLevelEventAware bit set in your SIZE resource. */
    noGlobalsErr                = -904,                         /*The system is hosed, better re-boot*/
    localOnlyErr                = -905,                         /*Network activity is currently disabled*/
    destPortErr                 = -906,                         /*Port does not exist at destination*/
    sessTableErr                = -907,                         /*Out of session tables, try again later*/
    noSessionErr                = -908,                         /*Invalid session reference number*/
    badReqErr                   = -909,                         /*bad parameter or invalid state for operation*/
    portNameExistsErr           = -910,                         /*port is already open (perhaps in another app)*/
    noUserNameErr               = -911,                         /*user name unknown on destination machine*/
    userRejectErr               = -912,                         /*Destination rejected the session request*/
    noMachineNameErr            = -913,                         /*user hasn't named his Macintosh in the Network Setup Control Panel*/
    noToolboxNameErr            = -914,                         /*A system resource is missing, not too likely*/
    noResponseErr               = -915,                         /*unable to contact destination*/
    portClosedErr               = -916,                         /*port was closed*/
    sessClosedErr               = -917,                         /*session was closed*/
    badPortNameErr              = -919,                         /*PPCPortRec malformed*/
    noDefaultUserErr            = -922,                         /*user hasn't typed in owners name in Network Setup Control Pannel*/
    notLoggedInErr              = -923,                         /*The default userRefNum does not yet exist*/
    noUserRefErr                = -924,                         /*unable to create a new userRefNum*/
    networkErr                  = -925,                         /*An error has occurred in the network, not too likely*/
    noInformErr                 = -926,                         /*PPCStart failed because destination did not have inform pending*/
    authFailErr                 = -927,                         /*unable to authenticate user at destination*/
    noUserRecErr                = -928,                         /*Invalid user reference number*/
    badServiceMethodErr         = -930,                         /*illegal service type, or not supported*/
    badLocNameErr               = -931,                         /*location name malformed*/
    guestNotAllowedErr          = -932                          /*destination port requires authentication*/
};

/* Font Mgr errors*/
enum {
    kFMIterationCompleted       = -980L,
    kFMInvalidFontFamilyErr     = -981L,
    kFMInvalidFontErr           = -982L,
    kFMIterationScopeModifiedErr = -983L,
    kFMFontTableAccessErr       = -984L,
    kFMFontContainerAccessErr   = -985L
};

enum {
    noMaskFoundErr              = -1000                         /*Icon Utilties Error*/
};

enum {
    nbpBuffOvr                  = -1024,                        /*Buffer overflow in LookupName*/
    nbpNoConfirm                = -1025,
    nbpConfDiff                 = -1026,                        /*Name confirmed at different socket*/
    nbpDuplicate                = -1027,                        /*Duplicate name exists already*/
    nbpNotFound                 = -1028,                        /*Name not found on remove*/
    nbpNISErr                   = -1029                         /*Error trying to open the NIS*/
};

enum {
    aspBadVersNum               = -1066,                        /*Server cannot support this ASP version*/
    aspBufTooSmall              = -1067,                        /*Buffer too small*/
    aspNoMoreSess               = -1068,                        /*No more sessions on server*/
    aspNoServers                = -1069,                        /*No servers at that address*/
    aspParamErr                 = -1070,                        /*Parameter error*/
    aspServerBusy               = -1071,                        /*Server cannot open another session*/
    aspSessClosed               = -1072,                        /*Session closed*/
    aspSizeErr                  = -1073,                        /*Command block too big*/
    aspTooMany                  = -1074,                        /*Too many clients (server error)*/
    aspNoAck                    = -1075                         /*No ack on attention request (server err)*/
};

enum {
    reqFailed                   = -1096,
    tooManyReqs                 = -1097,
    tooManySkts                 = -1098,
    badATPSkt                   = -1099,
    badBuffNum                  = -1100,
    noRelErr                    = -1101,
    cbNotFound                  = -1102,
    noSendResp                  = -1103,
    noDataArea                  = -1104,
    reqAborted                  = -1105
};

/* ADSP Error Codes */
enum {
                                                                /* driver control ioResults */
    errRefNum                   = -1280,                        /* bad connection refNum */
    errAborted                  = -1279,                        /* control call was aborted */
    errState                    = -1278,                        /* bad connection state for this operation */
    errOpening                  = -1277,                        /* open connection request failed */
    errAttention                = -1276,                        /* attention message too long */
    errFwdReset                 = -1275,                        /* read terminated by forward reset */
    errDSPQueueSize             = -1274,                        /* DSP Read/Write Queue Too small */
    errOpenDenied               = -1273                         /* open connection request was denied */
};


/*--------------------------------------------------------------
        Apple event manager error messages
--------------------------------------------------------------*/
enum {
    errAECoercionFail           = -1700,                        /* bad parameter data or unable to coerce the data supplied */
    errAEDescNotFound           = -1701,
    errAECorruptData            = -1702,
    errAEWrongDataType          = -1703,
    errAENotAEDesc              = -1704,
    errAEBadListItem            = -1705,                        /* the specified list item does not exist */
    errAENewerVersion           = -1706,                        /* need newer version of the AppleEvent manager */
    errAENotAppleEvent          = -1707,                        /* the event is not in AppleEvent format */
    errAEEventNotHandled        = -1708,                        /* the AppleEvent was not handled by any handler */
    errAEReplyNotValid          = -1709,                        /* AEResetTimer was passed an invalid reply parameter */
    errAEUnknownSendMode        = -1710,                        /* mode wasn't NoReply, WaitReply, or QueueReply or Interaction level is unknown */
    errAEWaitCanceled           = -1711,                        /* in AESend, the user cancelled out of wait loop for reply or receipt */
    errAETimeout                = -1712,                        /* the AppleEvent timed out */
    errAENoUserInteraction      = -1713,                        /* no user interaction is allowed */
    errAENotASpecialFunction    = -1714,                        /* there is no special function for/with this keyword */
    errAEParamMissed            = -1715,                        /* a required parameter was not accessed */
    errAEUnknownAddressType     = -1716,                        /* the target address type is not known */
    errAEHandlerNotFound        = -1717,                        /* no handler in the dispatch tables fits the parameters to AEGetEventHandler or AEGetCoercionHandler */
    errAEReplyNotArrived        = -1718,                        /* the contents of the reply you are accessing have not arrived yet */
    errAEIllegalIndex           = -1719,                        /* index is out of range in a put operation */
    errAEImpossibleRange        = -1720,                        /* A range like 3rd to 2nd, or 1st to all. */
    errAEWrongNumberArgs        = -1721,                        /* Logical op kAENOT used with other than 1 term */
    errAEAccessorNotFound       = -1723,                        /* Accessor proc matching wantClass and containerType or wildcards not found */
    errAENoSuchLogical          = -1725,                        /* Something other than AND, OR, or NOT */
    errAEBadTestKey             = -1726,                        /* Test is neither typeLogicalDescriptor nor typeCompDescriptor */
    errAENotAnObjSpec           = -1727,                        /* Param to AEResolve not of type 'obj ' */
    errAENoSuchObject           = -1728,                        /* e.g.,: specifier asked for the 3rd, but there are only 2. Basically, this indicates a run-time resolution error. */
    errAENegativeCount          = -1729,                        /* CountProc returned negative value */
    errAEEmptyListContainer     = -1730,                        /* Attempt to pass empty list as container to accessor */
    errAEUnknownObjectType      = -1731,                        /* available only in version 1.0.1 or greater */
    errAERecordingIsAlreadyOn   = -1732,                        /* available only in version 1.0.1 or greater */
    errAEReceiveTerminate       = -1733,                        /* break out of all levels of AEReceive to the topmost (1.1 or greater) */
    errAEReceiveEscapeCurrent   = -1734,                        /* break out of only lowest level of AEReceive (1.1 or greater) */
    errAEEventFiltered          = -1735,                        /* event has been filtered, and should not be propogated (1.1 or greater) */
    errAEDuplicateHandler       = -1736,                        /* attempt to install handler in table for identical class and id (1.1 or greater) */
    errAEStreamBadNesting       = -1737,                        /* nesting violation while streaming */
    errAEStreamAlreadyConverted = -1738,                        /* attempt to convert a stream that has already been converted */
    errAEDescIsNull             = -1739                         /* attempting to perform an invalid operation on a null descriptor */
};

enum {
    errOSASystemError           = -1750,
    errOSAInvalidID             = -1751,
    errOSABadStorageType        = -1752,
    errOSAScriptError           = -1753,
    errOSABadSelector           = -1754,
    errOSASourceNotAvailable    = -1756,
    errOSANoSuchDialect         = -1757,
    errOSADataFormatObsolete    = -1758,
    errOSADataFormatTooNew      = -1759,
    errOSACorruptData           = errAECorruptData,
    errOSARecordingIsAlreadyOn  = errAERecordingIsAlreadyOn,
    errOSAComponentMismatch     = -1761,                        /* Parameters are from 2 different components */
    errOSACantOpenComponent     = -1762                         /* Can't connect to scripting system with that ID */
};



/* AppleEvent error definitions */
enum {
    errOffsetInvalid            = -1800,
    errOffsetIsOutsideOfView    = -1801,
    errTopOfDocument            = -1810,
    errTopOfBody                = -1811,
    errEndOfDocument            = -1812,
    errEndOfBody                = -1813
};

enum {
                                                                /* Drag Manager error codes */
    badDragRefErr               = -1850,                        /* unknown drag reference */
    badDragItemErr              = -1851,                        /* unknown drag item reference */
    badDragFlavorErr            = -1852,                        /* unknown flavor type */
    duplicateFlavorErr          = -1853,                        /* flavor type already exists */
    cantGetFlavorErr            = -1854,                        /* error while trying to get flavor data */
    duplicateHandlerErr         = -1855,                        /* handler already exists */
    handlerNotFoundErr          = -1856,                        /* handler not found */
    dragNotAcceptedErr          = -1857,                        /* drag was not accepted by receiver */
    unsupportedForPlatformErr   = -1858,                        /* call is for PowerPC only */
    noSuitableDisplaysErr       = -1859,                        /* no displays support translucency */
    badImageRgnErr              = -1860,                        /* bad translucent image region */
    badImageErr                 = -1861                         /* bad translucent image PixMap */
};


/* QuickTime errors */
enum {
    couldNotResolveDataRef      = -2000,
    badImageDescription         = -2001,
    badPublicMovieAtom          = -2002,
    cantFindHandler             = -2003,
    cantOpenHandler             = -2004,
    badComponentType            = -2005,
    noMediaHandler              = -2006,
    noDataHandler               = -2007,
    invalidMedia                = -2008,
    invalidTrack                = -2009,
    invalidMovie                = -2010,
    invalidSampleTable          = -2011,
    invalidDataRef              = -2012,
    invalidHandler              = -2013,
    invalidDuration             = -2014,
    invalidTime                 = -2015,
    cantPutPublicMovieAtom      = -2016,
    badEditList                 = -2017,
    mediaTypesDontMatch         = -2018,
    progressProcAborted         = -2019,
    movieToolboxUninitialized   = -2020,
    noRecordOfApp               = movieToolboxUninitialized,    /* replica */
    wfFileNotFound              = -2021,
    cantCreateSingleForkFile    = -2022,                        /* happens when file already exists */
    invalidEditState            = -2023,
    nonMatchingEditState        = -2024,
    staleEditState              = -2025,
    userDataItemNotFound        = -2026,
    maxSizeToGrowTooSmall       = -2027,
    badTrackIndex               = -2028,
    trackIDNotFound             = -2029,
    trackNotInMovie             = -2030,
    timeNotInTrack              = -2031,
    timeNotInMedia              = -2032,
    badEditIndex                = -2033,
    internalQuickTimeError      = -2034,
    cantEnableTrack             = -2035,
    invalidRect                 = -2036,
    invalidSampleNum            = -2037,
    invalidChunkNum             = -2038,
    invalidSampleDescIndex      = -2039,
    invalidChunkCache           = -2040,
    invalidSampleDescription    = -2041,
    dataNotOpenForRead          = -2042,
    dataNotOpenForWrite         = -2043,
    dataAlreadyOpenForWrite     = -2044,
    dataAlreadyClosed           = -2045,
    endOfDataReached            = -2046,
    dataNoDataRef               = -2047,
    noMovieFound                = -2048,
    invalidDataRefContainer     = -2049,
    badDataRefIndex             = -2050,
    noDefaultDataRef            = -2051,
    couldNotUseAnExistingSample = -2052,
    featureUnsupported          = -2053,
    noVideoTrackInMovieErr      = -2054,                        /* QT for Windows error */
    noSoundTrackInMovieErr      = -2055,                        /* QT for Windows error */
    soundSupportNotAvailableErr = -2056,                        /* QT for Windows error */
    unsupportedAuxiliaryImportData = -2057,
    auxiliaryExportDataUnavailable = -2058,
    samplesAlreadyInMediaErr    = -2059,
    noSourceTreeFoundErr        = -2060,
    sourceNotFoundErr           = -2061,
    movieTextNotFoundErr        = -2062,
    missingRequiredParameterErr = -2063,
    invalidSpriteWorldPropertyErr = -2064,
    invalidSpritePropertyErr    = -2065,
    gWorldsNotSameDepthAndSizeErr = -2066,
    invalidSpriteIndexErr       = -2067,
    invalidImageIndexErr        = -2068,
    invalidSpriteIDErr          = -2069
};

enum {
    internalComponentErr        = -2070,
    notImplementedMusicOSErr    = -2071,
    cantSendToSynthesizerOSErr  = -2072,
    cantReceiveFromSynthesizerOSErr = -2073,
    illegalVoiceAllocationOSErr = -2074,
    illegalPartOSErr            = -2075,
    illegalChannelOSErr         = -2076,
    illegalKnobOSErr            = -2077,
    illegalKnobValueOSErr       = -2078,
    illegalInstrumentOSErr      = -2079,
    illegalControllerOSErr      = -2080,
    midiManagerAbsentOSErr      = -2081,
    synthesizerNotRespondingOSErr = -2082,
    synthesizerOSErr            = -2083,
    illegalNoteChannelOSErr     = -2084,
    noteChannelNotAllocatedOSErr = -2085,
    tunePlayerFullOSErr         = -2086,
    tuneParseOSErr              = -2087,
    noExportProcAvailableErr    = -2089,
    videoOutputInUseErr         = -2090
};

enum {
    componentDllLoadErr         = -2091,                        /* Windows specific errors (when component is loading)*/
    componentDllEntryNotFoundErr = -2092,                       /* Windows specific errors (when component is loading)*/
    qtmlDllLoadErr              = -2093,                        /* Windows specific errors (when qtml is loading)*/
    qtmlDllEntryNotFoundErr     = -2094,                        /* Windows specific errors (when qtml is loading)*/
    qtmlUninitialized           = -2095,
    unsupportedOSErr            = -2096,
    unsupportedProcessorErr     = -2097
};

enum {
    cannotFindAtomErr           = -2101,
    notLeafAtomErr              = -2102,
    atomsNotOfSameTypeErr       = -2103,
    atomIndexInvalidErr         = -2104,
    duplicateAtomTypeAndIDErr   = -2105,
    invalidAtomErr              = -2106,
    invalidAtomContainerErr     = -2107,
    invalidAtomTypeErr          = -2108,
    cannotBeLeafAtomErr         = -2109,
    pathTooLongErr              = -2110,
    emptyPathErr                = -2111,
    noPathMappingErr            = -2112,
    pathNotVerifiedErr          = -2113,
    unknownFormatErr            = -2114,
    wackBadFileErr              = -2115,
    wackForkNotFoundErr         = -2116,
    wackBadMetaDataErr          = -2117,
    qfcbNotFoundErr             = -2118,
    qfcbNotCreatedErr           = -2119,
    AAPNotCreatedErr            = -2120,
    AAPNotFoundErr              = -2121,
    ASDBadHeaderErr             = -2122,
    ASDBadForkErr               = -2123,
    ASDEntryNotFoundErr         = -2124,
    fileOffsetTooBigErr         = -2125,
    notAllowedToSaveMovieErr    = -2126,
    qtNetworkAlreadyAllocatedErr = -2127,
    urlDataHHTTPProtocolErr     = -2129,
    urlDataHHTTPNoNetDriverErr  = -2130,
    urlDataHHTTPURLErr          = -2131,
    urlDataHHTTPRedirectErr     = -2132,
    urlDataHFTPProtocolErr      = -2133,
    urlDataHFTPShutdownErr      = -2134,
    urlDataHFTPBadUserErr       = -2135,
    urlDataHFTPBadPasswordErr   = -2136,
    urlDataHFTPServerErr        = -2137,
    urlDataHFTPDataConnectionErr = -2138,
    urlDataHFTPNoDirectoryErr   = -2139,
    urlDataHFTPQuotaErr         = -2140,
    urlDataHFTPPermissionsErr   = -2141,
    urlDataHFTPFilenameErr      = -2142,
    urlDataHFTPNoNetDriverErr   = -2143,
    urlDataHFTPBadNameListErr   = -2144,
    urlDataHFTPNeedPasswordErr  = -2145,
    urlDataHFTPNoPasswordErr    = -2146,
    urlDataHFTPServerDisconnectedErr = -2147,
    urlDataHFTPURLErr           = -2148,
    notEnoughDataErr            = -2149,
    qtActionNotHandledErr       = -2157,
    qtXMLParseErr               = -2158,
    qtXMLApplicationErr         = -2159
};


enum {
    digiUnimpErr                = -2201,                        /* feature unimplemented */
    qtParamErr                  = -2202,                        /* bad input parameter (out of range, etc) */
    matrixErr                   = -2203,                        /* bad matrix, digitizer did nothing */
    notExactMatrixErr           = -2204,                        /* warning of bad matrix, digitizer did its best */
    noMoreKeyColorsErr          = -2205,                        /* all key indexes in use */
    notExactSizeErr             = -2206,                        /* Can't do exact size requested */
    badDepthErr                 = -2207,                        /* Can't digitize into this depth */
    noDMAErr                    = -2208,                        /* Can't do DMA digitizing (i.e. can't go to requested dest */
    badCallOrderErr             = -2209,                        /* Usually due to a status call being called prior to being setup first */
    vdImageDescStaleErr         = -2210,                        /* The ImageDescription previously returned from VDGetImageDescription is no longer valid */
    vdHardwareGoneErr           = -2211,                        /* The hardware device providing video frames has been unplugged */
    vdDontHaveThatUniqueIDErr   = -2212                         /* The hardware device does not have the ID you are asking for */
};


/*  Kernel Error Codes  */
enum {
    kernelIncompleteErr         = -2401,
    kernelCanceledErr           = -2402,
    kernelOptionsErr            = -2403,
    kernelPrivilegeErr          = -2404,
    kernelUnsupportedErr        = -2405,
    kernelObjectExistsErr       = -2406,
    kernelWritePermissionErr    = -2407,
    kernelReadPermissionErr     = -2408,
    kernelExecutePermissionErr  = -2409,
    kernelDeletePermissionErr   = -2410,
    kernelExecutionLevelErr     = -2411,
    kernelAttributeErr          = -2412,
    kernelAsyncSendLimitErr     = -2413,
    kernelAsyncReceiveLimitErr  = -2414,
    kernelTimeoutErr            = -2415,
    kernelInUseErr              = -2416,
    kernelTerminatedErr         = -2417,
    kernelExceptionErr          = -2418,
    kernelIDErr                 = -2419,
    kernelAlreadyFreeErr        = -2421,
    kernelReturnValueErr        = -2422,
    kernelUnrecoverableErr      = -2499
};



enum {
                                                                /* Text Services Mgr error codes */
    tsmComponentNoErr           = 0,                            /* component result = no error */
    tsmUnsupScriptLanguageErr   = -2500,
    tsmInputMethodNotFoundErr   = -2501,
    tsmNotAnAppErr              = -2502,                        /* not an application error */
    tsmAlreadyRegisteredErr     = -2503,                        /* want to register again error */
    tsmNeverRegisteredErr       = -2504,                        /* app never registered error (not TSM aware) */
    tsmInvalidDocIDErr          = -2505,                        /* invalid TSM documentation id */
    tsmTSMDocBusyErr            = -2506,                        /* document is still active */
    tsmDocNotActiveErr          = -2507,                        /* document is NOT active */
    tsmNoOpenTSErr              = -2508,                        /* no open text service */
    tsmCantOpenComponentErr     = -2509,                        /* can't open the component */
    tsmTextServiceNotFoundErr   = -2510,                        /* no text service found */
    tsmDocumentOpenErr          = -2511,                        /* there are open documents */
    tsmUseInputWindowErr        = -2512,                        /* not TSM aware because we are using input window */
    tsmTSHasNoMenuErr           = -2513,                        /* the text service has no menu */
    tsmTSNotOpenErr             = -2514,                        /* text service is not open */
    tsmComponentAlreadyOpenErr  = -2515,                        /* text service already opened for the document */
    tsmInputMethodIsOldErr      = -2516,                        /* returned by GetDefaultInputMethod */
    tsmScriptHasNoIMErr         = -2517,                        /* script has no imput method or is using old IM */
    tsmUnsupportedTypeErr       = -2518,                        /* unSupported interface type error */
    tsmUnknownErr               = -2519,                        /* any other errors */
    tsmInvalidContext           = -2520,                        /* Invalid TSMContext specified in call */
    tsmNoHandler                = -2521,                        /* No Callback Handler exists for callback */
    tsmNoMoreTokens             = -2522,                        /* No more tokens are available for the source text */
    tsmNoStem                   = -2523,                        /* No stem exists for the token */
    tsmDefaultIsNotInputMethodErr = -2524                       /* Current Input source is KCHR or uchr, not Input Method  (GetDefaultInputMethod) */
};


enum {
                                                                /* Mixed Mode error codes */
    mmInternalError             = -2526
};

/* NameRegistry error codes */
enum {
    nrLockedErr                 = -2536,
    nrNotEnoughMemoryErr        = -2537,
    nrInvalidNodeErr            = -2538,
    nrNotFoundErr               = -2539,
    nrNotCreatedErr             = -2540,
    nrNameErr                   = -2541,
    nrNotSlotDeviceErr          = -2542,
    nrDataTruncatedErr          = -2543,
    nrPowerErr                  = -2544,
    nrPowerSwitchAbortErr       = -2545,
    nrTypeMismatchErr           = -2546,
    nrNotModifiedErr            = -2547,
    nrOverrunErr                = -2548,
    nrResultCodeBase            = -2549,
    nrPathNotFound              = -2550,                        /* a path component lookup failed */
    nrPathBufferTooSmall        = -2551,                        /* buffer for path is too small */
    nrInvalidEntryIterationOp   = -2552,                        /* invalid entry iteration operation */
    nrPropertyAlreadyExists     = -2553,                        /* property already exists */
    nrIterationDone             = -2554,                        /* iteration operation is done */
    nrExitedIteratorScope       = -2555,                        /* outer scope of iterator was exited */
    nrTransactionAborted        = -2556,                        /* transaction was aborted */
    nrCallNotSupported          = -2557                         /* This call is not available or supported on this machine */
};

/* Icon Services error codes */
enum {
    invalidIconRefErr           = -2580,                        /* The icon ref is not valid */
    noSuchIconErr               = -2581,                        /* The requested icon could not be found */
    noIconDataAvailableErr      = -2582                         /* The necessary icon data is not available */
};


/*        
    Dynamic AppleScript errors:

    These errors result from data-dependent conditions and are typically
    signaled at runtime.
*/
enum {
    errOSACantCoerce            = errAECoercionFail,            /* Signaled when a value can't be coerced to the desired type. */
    errOSACantAccess            = errAENoSuchObject,            /* Signaled when an object is not found in a container*/
    errOSACantAssign            = -10006,                       /* Signaled when an object cannot be set in a container.*/
    errOSAGeneralError          = -2700,                        /* Signaled by user scripts or applications when no actual error code is to be returned.*/
    errOSADivideByZero          = -2701,                        /* Signaled when there is an attempt to divide by zero*/
    errOSANumericOverflow       = -2702,                        /* Signaled when integer or real value is too large to be represented*/
    errOSACantLaunch            = -2703,                        /* Signaled when application can't be launched or when it is remote and program linking is not enabled*/
    errOSAAppNotHighLevelEventAware = -2704,                    /* Signaled when an application can't respond to AppleEvents*/
    errOSACorruptTerminology    = -2705,                        /* Signaled when an application's terminology resource is not readable*/
    errOSAStackOverflow         = -2706,                        /* Signaled when the runtime stack overflows*/
    errOSAInternalTableOverflow = -2707,                        /* Signaled when a runtime internal data structure overflows*/
    errOSADataBlockTooLarge     = -2708,                        /* Signaled when an intrinsic limitation is exceeded for the size of a value or data structure.*/
    errOSACantGetTerminology    = -2709,
    errOSACantCreate            = -2710
};

/*        
    Component-specific dynamic script errors:

    The range -2720 thru -2739 is reserved for component-specific runtime errors.
    (Note that error codes from different scripting components in this range will
    overlap.)
*/
/*        
    Static AppleScript errors:

    These errors comprise what are commonly thought of as parse and compile-
    time errors.  However, in a dynamic system (e.g. AppleScript) any or all
    of these may also occur at runtime.
*/
enum {
    errOSATypeError             = errAEWrongDataType,
    OSAMessageNotUnderstood     = errAEEventNotHandled,         /* Signaled when a message was sent to an object that didn't handle it*/
    OSAUndefinedHandler         = errAEHandlerNotFound,         /* Signaled when a function to be returned doesn't exist. */
    OSAIllegalAccess            = errAEAccessorNotFound,        /* Signaled when a container can never have the requested object*/
    OSAIllegalIndex             = errAEIllegalIndex,            /* Signaled when index was out of range. Specialization of errOSACantAccess*/
    OSAIllegalRange             = errAEImpossibleRange,         /* Signaled when a range is screwy. Specialization of errOSACantAccess*/
    OSAIllegalAssign            = -10003,                       /* Signaled when an object can never be set in a container*/
    OSASyntaxError              = -2740,                        /* Signaled when a syntax error occurs. (e.g. "Syntax error" or "<this> can't go after <that>")*/
    OSASyntaxTypeError          = -2741,                        /* Signaled when another form of syntax was expected. (e.g. "expected a <type> but found <this>")*/
    OSATokenTooLong             = -2742,                        /* Signaled when a name or number is too long to be parsed*/
    OSAMissingParameter         = errAEDescNotFound,            /* Signaled when a parameter is missing for a function invocation*/
    OSAParameterMismatch        = errAEWrongNumberArgs,         /* Signaled when function is called with the wrong number of parameters, or a parameter pattern cannot be matched*/
    OSADuplicateParameter       = -2750,                        /* Signaled when a formal parameter, local variable, or instance variable is specified more than once*/
    OSADuplicateProperty        = -2751,                        /* Signaled when a formal parameter, local variable, or instance variable is specified more than once.*/
    OSADuplicateHandler         = -2752,                        /* Signaled when more than one handler is defined with the same name in a scope where the language doesn't allow it*/
    OSAUndefinedVariable        = -2753,                        /* Signaled when a variable is accessed that has no value*/
    OSAInconsistentDeclarations = -2754,                        /* Signaled when a variable is declared inconsistently in the same scope, such as both local and global*/
    OSAControlFlowError         = -2755                         /* Signaled when illegal control flow occurs in an application (no catcher for throw, non-lexical loop exit, etc.)*/
};

/*        
    Component-specific AppleScript static errors:

    The range -2760 thru -2779 is reserved for component-specific parsing and
    compile-time errors. (Note that error codes from different scripting
    components in this range will overlap.)
*/
/*        
    Dialect-specific AppleScript errors:

    The range -2780 thru -2799 is reserved for dialect specific error codes for
    scripting components that support dialects. (Note that error codes from
    different scripting components in this range will overlap, as well as error
    codes from different dialects in the same scripting component.)
*/

/**************************************************************************
    Apple Script Error Codes
**************************************************************************/
/* Runtime errors: */
enum {
    errASCantConsiderAndIgnore  = -2720,
    errASCantCompareMoreThan32k = -2721,                        /* Parser/Compiler errors: */
    errASTerminologyNestingTooDeep = -2760,
    errASIllegalFormalParameter = -2761,
    errASParameterNotForEvent   = -2762,
    errASNoResultReturned       = -2763,                        /*    The range -2780 thru -2799 is reserved for dialect specific error codes. (Error codes from different dialects may overlap.) */
    errASInconsistentNames      = -2780                         /*    English errors: */
};


/* The preferred spelling for Code Fragment Manager errors:*/
enum {
    cfragFirstErrCode           = -2800,                        /* The first value in the range of CFM errors.*/
    cfragContextIDErr           = -2800,                        /* The context ID was not valid.*/
    cfragConnectionIDErr        = -2801,                        /* The connection ID was not valid.*/
    cfragNoSymbolErr            = -2802,                        /* The specified symbol was not found.*/
    cfragNoSectionErr           = -2803,                        /* The specified section was not found.*/
    cfragNoLibraryErr           = -2804,                        /* The named library was not found.*/
    cfragDupRegistrationErr     = -2805,                        /* The registration name was already in use.*/
    cfragFragmentFormatErr      = -2806,                        /* A fragment's container format is unknown.*/
    cfragUnresolvedErr          = -2807,                        /* A fragment had "hard" unresolved imports.*/
    cfragNoPositionErr          = -2808,                        /* The registration insertion point was not found.*/
    cfragNoPrivateMemErr        = -2809,                        /* Out of memory for internal bookkeeping.*/
    cfragNoClientMemErr         = -2810,                        /* Out of memory for fragment mapping or section instances.*/
    cfragNoIDsErr               = -2811,                        /* No more CFM IDs for contexts, connections, etc.*/
    cfragInitOrderErr           = -2812,                        /* */
    cfragImportTooOldErr        = -2813,                        /* An import library was too old for a client.*/
    cfragImportTooNewErr        = -2814,                        /* An import library was too new for a client.*/
    cfragInitLoopErr            = -2815,                        /* Circularity in required initialization order.*/
    cfragInitAtBootErr          = -2816,                        /* A boot library has an initialization function.  (System 7 only)*/
    cfragLibConnErr             = -2817,                        /* */
    cfragCFMStartupErr          = -2818,                        /* Internal error during CFM initialization.*/
    cfragCFMInternalErr         = -2819,                        /* An internal inconstistancy has been detected.*/
    cfragFragmentCorruptErr     = -2820,                        /* A fragment's container was corrupt (known format).*/
    cfragInitFunctionErr        = -2821,                        /* A fragment's initialization routine returned an error.*/
    cfragNoApplicationErr       = -2822,                        /* No application member found in the cfrg resource.*/
    cfragArchitectureErr        = -2823,                        /* A fragment has an unacceptable architecture.*/
    cfragFragmentUsageErr       = -2824,                        /* A semantic error in usage of the fragment.*/
    cfragFileSizeErr            = -2825,                        /* A file was too large to be mapped.*/
    cfragNotClosureErr          = -2826,                        /* The closure ID was actually a connection ID.*/
    cfragNoRegistrationErr      = -2827,                        /* The registration name was not found.*/
    cfragContainerIDErr         = -2828,                        /* The fragment container ID was not valid.*/
    cfragClosureIDErr           = -2829,                        /* The closure ID was not valid.*/
    cfragAbortClosureErr        = -2830,                        /* Used by notification handlers to abort a closure.*/
    cfragOutputLengthErr        = -2831,                        /* An output parameter is too small to hold the value.*/
    cfragLastErrCode            = -2899                         /* The last value in the range of CFM errors.*/
};

#if OLDROUTINENAMES
/* The old spelling for Code Fragment Manager errors, kept for compatibility:*/
enum {
    fragContextNotFound         = cfragContextIDErr,
    fragConnectionIDNotFound    = cfragConnectionIDErr,
    fragSymbolNotFound          = cfragNoSymbolErr,
    fragSectionNotFound         = cfragNoSectionErr,
    fragLibNotFound             = cfragNoLibraryErr,
    fragDupRegLibName           = cfragDupRegistrationErr,
    fragFormatUnknown           = cfragFragmentFormatErr,
    fragHadUnresolveds          = cfragUnresolvedErr,
    fragNoMem                   = cfragNoPrivateMemErr,
    fragNoAddrSpace             = cfragNoClientMemErr,
    fragNoContextIDs            = cfragNoIDsErr,
    fragObjectInitSeqErr        = cfragInitOrderErr,
    fragImportTooOld            = cfragImportTooOldErr,
    fragImportTooNew            = cfragImportTooNewErr,
    fragInitLoop                = cfragInitLoopErr,
    fragInitRtnUsageErr         = cfragInitAtBootErr,
    fragLibConnErr              = cfragLibConnErr,
    fragMgrInitErr              = cfragCFMStartupErr,
    fragConstErr                = cfragCFMInternalErr,
    fragCorruptErr              = cfragFragmentCorruptErr,
    fragUserInitProcErr         = cfragInitFunctionErr,
    fragAppNotFound             = cfragNoApplicationErr,
    fragArchError               = cfragArchitectureErr,
    fragInvalidFragmentUsage    = cfragFragmentUsageErr,
    fragLastErrCode             = cfragLastErrCode
};

#endif  /* OLDROUTINENAMES */

/*Component Manager & component errors*/
enum {
    invalidComponentID          = -3000,
    validInstancesExist         = -3001,
    componentNotCaptured        = -3002,
    componentDontRegister       = -3003,
    unresolvedComponentDLLErr   = -3004
};

/*Translation manager & Translation components*/
enum {
    invalidTranslationPathErr   = -3025,                        /*Source type to destination type not a valid path*/
    couldNotParseSourceFileErr  = -3026,                        /*Source document does not contain source type*/
    noTranslationPathErr        = -3030,
    badTranslationSpecErr       = -3031,
    noPrefAppErr                = -3032
};

enum {
    buf2SmallErr                = -3101,
    noMPPErr                    = -3102,
    ckSumErr                    = -3103,
    extractErr                  = -3104,
    readQErr                    = -3105,
    atpLenErr                   = -3106,
    atpBadRsp                   = -3107,
    recNotFnd                   = -3108,
    sktClosedErr                = -3109
};


/* OpenTransport errors*/
enum {
    kOTNoError                  = 0,                            /* No Error occurred                    */
    kOTOutOfMemoryErr           = -3211,                        /* OT ran out of memory, may be a temporary      */
    kOTNotFoundErr              = -3201,                        /* OT generic not found error               */
    kOTDuplicateFoundErr        = -3216,                        /* OT generic duplicate found error             */
    kOTBadAddressErr            = -3150,                        /* XTI2OSStatus(TBADADDR) A Bad address was specified          */
    kOTBadOptionErr             = -3151,                        /* XTI2OSStatus(TBADOPT) A Bad option was specified             */
    kOTAccessErr                = -3152,                        /* XTI2OSStatus(TACCES) Missing access permission           */
    kOTBadReferenceErr          = -3153,                        /* XTI2OSStatus(TBADF) Bad provider reference               */
    kOTNoAddressErr             = -3154,                        /* XTI2OSStatus(TNOADDR) No address was specified           */
    kOTOutStateErr              = -3155,                        /* XTI2OSStatus(TOUTSTATE) Call issued in wrong state           */
    kOTBadSequenceErr           = -3156,                        /* XTI2OSStatus(TBADSEQ) Sequence specified does not exist         */
    kOTSysErrorErr              = -3157,                        /* XTI2OSStatus(TSYSERR) A system error occurred            */
    kOTLookErr                  = -3158,                        /* XTI2OSStatus(TLOOK) An event occurred - call Look()         */
    kOTBadDataErr               = -3159,                        /* XTI2OSStatus(TBADDATA) An illegal amount of data was specified */
    kOTBufferOverflowErr        = -3160,                        /* XTI2OSStatus(TBUFOVFLW) Passed buffer not big enough          */
    kOTFlowErr                  = -3161,                        /* XTI2OSStatus(TFLOW) Provider is flow-controlled          */
    kOTNoDataErr                = -3162,                        /* XTI2OSStatus(TNODATA) No data available for reading          */
    kOTNoDisconnectErr          = -3163,                        /* XTI2OSStatus(TNODIS) No disconnect indication available         */
    kOTNoUDErrErr               = -3164,                        /* XTI2OSStatus(TNOUDERR) No Unit Data Error indication available */
    kOTBadFlagErr               = -3165,                        /* XTI2OSStatus(TBADFLAG) A Bad flag value was supplied          */
    kOTNoReleaseErr             = -3166,                        /* XTI2OSStatus(TNOREL) No orderly release indication available   */
    kOTNotSupportedErr          = -3167,                        /* XTI2OSStatus(TNOTSUPPORT) Command is not supported           */
    kOTStateChangeErr           = -3168,                        /* XTI2OSStatus(TSTATECHNG) State is changing - try again later     */
    kOTNoStructureTypeErr       = -3169,                        /* XTI2OSStatus(TNOSTRUCTYPE) Bad structure type requested for OTAlloc    */
    kOTBadNameErr               = -3170,                        /* XTI2OSStatus(TBADNAME) A bad endpoint name was supplied         */
    kOTBadQLenErr               = -3171,                        /* XTI2OSStatus(TBADQLEN) A Bind to an in-use addr with qlen > 0   */
    kOTAddressBusyErr           = -3172,                        /* XTI2OSStatus(TADDRBUSY) Address requested is already in use       */
    kOTIndOutErr                = -3173,                        /* XTI2OSStatus(TINDOUT) Accept failed because of pending listen  */
    kOTProviderMismatchErr      = -3174,                        /* XTI2OSStatus(TPROVMISMATCH) Tried to accept on incompatible endpoint   */
    kOTResQLenErr               = -3175,                        /* XTI2OSStatus(TRESQLEN)                            */
    kOTResAddressErr            = -3176,                        /* XTI2OSStatus(TRESADDR)                            */
    kOTQFullErr                 = -3177,                        /* XTI2OSStatus(TQFULL)                          */
    kOTProtocolErr              = -3178,                        /* XTI2OSStatus(TPROTO) An unspecified provider error occurred       */
    kOTBadSyncErr               = -3179,                        /* XTI2OSStatus(TBADSYNC) A synchronous call at interrupt time       */
    kOTCanceledErr              = -3180,                        /* XTI2OSStatus(TCANCELED) The command was cancelled            */
    kEPERMErr                   = -3200,                        /* Permission denied            */
    kENOENTErr                  = -3201,                        /* No such file or directory       */
    kENORSRCErr                 = -3202,                        /* No such resource               */
    kEINTRErr                   = -3203,                        /* Interrupted system service        */
    kEIOErr                     = -3204,                        /* I/O error                 */
    kENXIOErr                   = -3205,                        /* No such device or address       */
    kEBADFErr                   = -3208,                        /* Bad file number                 */
    kEAGAINErr                  = -3210,                        /* Try operation again later       */
    kENOMEMErr                  = -3211,                        /* Not enough space               */
    kEACCESErr                  = -3212,                        /* Permission denied            */
    kEFAULTErr                  = -3213,                        /* Bad address                   */
    kEBUSYErr                   = -3215,                        /* Device or resource busy          */
    kEEXISTErr                  = -3216,                        /* File exists                   */
    kENODEVErr                  = -3218,                        /* No such device               */
    kEINVALErr                  = -3221,                        /* Invalid argument               */
    kENOTTYErr                  = -3224,                        /* Not a character device          */
    kEPIPEErr                   = -3231,                        /* Broken pipe                   */
    kERANGEErr                  = -3233,                        /* Message size too large for STREAM  */
    kEWOULDBLOCKErr             = -3234,                        /* Call would block, so was aborted     */
    kEDEADLKErr                 = -3234,                        /* or a deadlock would occur       */
    kEALREADYErr                = -3236,                        /*                          */
    kENOTSOCKErr                = -3237,                        /* Socket operation on non-socket     */
    kEDESTADDRREQErr            = -3238,                        /* Destination address required      */
    kEMSGSIZEErr                = -3239,                        /* Message too long               */
    kEPROTOTYPEErr              = -3240,                        /* Protocol wrong type for socket     */
    kENOPROTOOPTErr             = -3241,                        /* Protocol not available          */
    kEPROTONOSUPPORTErr         = -3242,                        /* Protocol not supported          */
    kESOCKTNOSUPPORTErr         = -3243,                        /* Socket type not supported       */
    kEOPNOTSUPPErr              = -3244,                        /* Operation not supported on socket  */
    kEADDRINUSEErr              = -3247,                        /* Address already in use          */
    kEADDRNOTAVAILErr           = -3248,                        /* Can't assign requested address     */
    kENETDOWNErr                = -3249,                        /* Network is down                 */
    kENETUNREACHErr             = -3250,                        /* Network is unreachable          */
    kENETRESETErr               = -3251,                        /* Network dropped connection on reset    */
    kECONNABORTEDErr            = -3252,                        /* Software caused connection abort     */
    kECONNRESETErr              = -3253,                        /* Connection reset by peer          */
    kENOBUFSErr                 = -3254,                        /* No buffer space available       */
    kEISCONNErr                 = -3255,                        /* Socket is already connected         */
    kENOTCONNErr                = -3256,                        /* Socket is not connected          */
    kESHUTDOWNErr               = -3257,                        /* Can't send after socket shutdown     */
    kETOOMANYREFSErr            = -3258,                        /* Too many references: can't splice  */
    kETIMEDOUTErr               = -3259,                        /* Connection timed out             */
    kECONNREFUSEDErr            = -3260,                        /* Connection refused           */
    kEHOSTDOWNErr               = -3263,                        /* Host is down                */
    kEHOSTUNREACHErr            = -3264,                        /* No route to host               */
    kEPROTOErr                  = -3269,                        /* ooo fill out missing codes ooo     */
    kETIMEErr                   = -3270,                        /*                          */
    kENOSRErr                   = -3271,                        /*                          */
    kEBADMSGErr                 = -3272,                        /*                          */
    kECANCELErr                 = -3273,                        /*                          */
    kENOSTRErr                  = -3274,                        /*                          */
    kENODATAErr                 = -3275,                        /*                          */
    kEINPROGRESSErr             = -3276,                        /*                          */
    kESRCHErr                   = -3277,                        /*                          */
    kENOMSGErr                  = -3278,                        /*                          */
    kOTClientNotInittedErr      = -3279,                        /*                          */
    kOTPortHasDiedErr           = -3280,                        /*                          */
    kOTPortWasEjectedErr        = -3281,                        /*                          */
    kOTBadConfigurationErr      = -3282,                        /*                          */
    kOTConfigurationChangedErr  = -3283,                        /*                          */
    kOTUserRequestedErr         = -3284,                        /*                          */
    kOTPortLostConnection       = -3285                         /*                          */
};


/* Color Picker errors*/
enum {
    firstPickerError            = -4000,
    invalidPickerType           = firstPickerError,
    requiredFlagsDontMatch      = -4001,
    pickerResourceError         = -4002,
    cantLoadPicker              = -4003,
    cantCreatePickerWindow      = -4004,
    cantLoadPackage             = -4005,
    pickerCantLive              = -4006,
    colorSyncNotInstalled       = -4007,
    badProfileError             = -4008,
    noHelpForItem               = -4009
};



/* NSL error codes*/
enum {
    kNSL68kContextNotSupported  = -4170,                        /* no 68k allowed*/
    kNSLSchedulerError          = -4171,                        /* A custom thread routine encountered an error*/
    kNSLBadURLSyntax            = -4172,                        /* URL contains illegal characters*/
    kNSLNoCarbonLib             = -4173,
    kNSLUILibraryNotAvailable   = -4174,                        /* The NSL UI Library needs to be in the Extensions Folder*/
    kNSLNotImplementedYet       = -4175,
    kNSLErrNullPtrError         = -4176,
    kNSLSomePluginsFailedToLoad = -4177,                        /* (one or more plugins failed to load, but at least one did load; this error isn't fatal)*/
    kNSLNullNeighborhoodPtr     = -4178,                        /* (client passed a null neighborhood ptr)*/
    kNSLNoPluginsForSearch      = -4179,                        /* (no plugins will respond to search request; bad protocol(s)?)*/
    kNSLSearchAlreadyInProgress = -4180,                        /* (you can only have one ongoing search per clientRef)*/
    kNSLNoPluginsFound          = -4181,                        /* (manager didn't find any valid plugins to load)*/
    kNSLPluginLoadFailed        = -4182,                        /* (manager unable to load one of the plugins)*/
    kNSLBadProtocolTypeErr      = -4183,                        /* (client is trying to add a null protocol type)*/
    kNSLNullListPtr             = -4184,                        /* (client is trying to add items to a nil list)*/
    kNSLBadClientInfoPtr        = -4185,                        /* (nil ClientAsyncInfoPtr; no reference available)*/
    kNSLCannotContinueLookup    = -4186,                        /* (Can't continue lookup; error or bad state)*/
    kNSLBufferTooSmallForData   = -4187,                        /* (Client buffer too small for data from plugin)*/
    kNSLNoContextAvailable      = -4188,                        /* (ContinueLookup function ptr invalid)*/
    kNSLRequestBufferAlreadyInList = -4189,
    kNSLInvalidPluginSpec       = -4190,
    kNSLNoSupportForService     = -4191,
    kNSLBadNetConnection        = -4192,
    kNSLBadDataTypeErr          = -4193,
    kNSLBadServiceTypeErr       = -4194,
    kNSLBadReferenceErr         = -4195,
    kNSLNoElementsInList        = -4196,
    kNSLInsufficientOTVer       = -4197,
    kNSLInsufficientSysVer      = -4198,
    kNSLNotInitialized          = -4199,
    kNSLInitializationFailed    = -4200                         /* UNABLE TO INITIALIZE THE MANAGER!!!!! DO NOT CONTINUE!!!!*/
};



/* desktop printing error codes*/
enum {
    kDTPHoldJobErr              = -4200,
    kDTPStopQueueErr            = -4201,
    kDTPTryAgainErr             = -4202,
    kDTPAbortJobErr             = 128
};


/* ColorSync Result codes */
enum {
                                                                /* Profile Access Errors */
    cmElementTagNotFound        = -4200,
    cmIndexRangeErr             = -4201,                        /* Tag index out of range */
    cmCantDeleteElement         = -4202,
    cmFatalProfileErr           = -4203,
    cmInvalidProfile            = -4204,                        /* A Profile must contain a 'cs1 ' tag to be valid */
    cmInvalidProfileLocation    = -4205,                        /* Operation not supported for this profile location */
    cmCantCopyModifiedV1Profile = -4215,                        /* Illegal to copy version 1 profiles that have been modified */
                                                                /* Profile Search Errors */
    cmInvalidSearch             = -4206,                        /* Bad Search Handle */
    cmSearchError               = -4207,
    cmErrIncompatibleProfile    = -4208,                        /* Other ColorSync Errors */
    cmInvalidColorSpace         = -4209,                        /* Profile colorspace does not match bitmap type */
    cmInvalidSrcMap             = -4210,                        /* Source pix/bit map was invalid */
    cmInvalidDstMap             = -4211,                        /* Destination pix/bit map was invalid */
    cmNoGDevicesError           = -4212,                        /* Begin/End Matching -- no gdevices available */
    cmInvalidProfileComment     = -4213,                        /* Bad Profile comment during drawpicture */
    cmRangeOverFlow             = -4214,                        /* Color conversion warning that some output color values over/underflowed and were clipped */
    cmNamedColorNotFound        = -4216,                        /* NamedColor not found */
    cmCantGamutCheckError       = -4217                         /* Gammut checking not supported by this ColorWorld */
};

/* new Folder Manager error codes */
enum {
    badFolderDescErr            = -4270,
    duplicateFolderDescErr      = -4271,
    noMoreFolderDescErr         = -4272,
    invalidFolderTypeErr        = -4273,
    duplicateRoutingErr         = -4274,
    routingNotFoundErr          = -4275,
    badRoutingSizeErr           = -4276
};


/* Core Foundation errors*/
enum {
    coreFoundationUnknownErr    = -4960
};

/* ScrapMgr error codes (CarbonLib 1.0 and later)*/
enum {
    internalScrapErr            = -4988,
    duplicateScrapFlavorErr     = -4989,
    badScrapRefErr              = -4990,
    processStateIncorrectErr    = -4991,
    scrapPromiseNotKeptErr      = -4992,
    noScrapPromiseKeeperErr     = -4993,
    nilScrapFlavorDataErr       = -4994,
    scrapFlavorFlagsMismatchErr = -4995,
    scrapFlavorSizeMismatchErr  = -4996,
    illegalScrapFlavorFlagsErr  = -4997,
    illegalScrapFlavorTypeErr   = -4998,
    illegalScrapFlavorSizeErr   = -4999,
    scrapFlavorNotFoundErr      = -102,                         /* == noTypeErr*/
    needClearScrapErr           = -100                          /* == noScrapErr*/
};


enum {
                                                                /*  AFP Protocol Errors */
    afpAccessDenied             = -5000,                        /* Insufficient access privileges for operation */
    afpAuthContinue             = -5001,                        /* Further information required to complete AFPLogin call */
    afpBadUAM                   = -5002,                        /* Unknown user authentication method specified */
    afpBadVersNum               = -5003,                        /* Unknown AFP protocol version number specified */
    afpBitmapErr                = -5004,                        /* Bitmap contained bits undefined for call */
    afpCantMove                 = -5005,                        /* Move destination is offspring of source, or root was specified */
    afpDenyConflict             = -5006,                        /* Specified open/deny modes conflict with current open modes */
    afpDirNotEmpty              = -5007,                        /* Cannot delete non-empty directory */
    afpDiskFull                 = -5008,                        /* Insufficient free space on volume for operation */
    afpEofError                 = -5009,                        /* Read beyond logical end-of-file */
    afpFileBusy                 = -5010,                        /* Cannot delete an open file */
    afpFlatVol                  = -5011,                        /* Cannot create directory on specified volume */
    afpItemNotFound             = -5012,                        /* Unknown UserName/UserID or missing comment/APPL entry */
    afpLockErr                  = -5013,                        /* Some or all of requested range is locked by another user */
    afpMiscErr                  = -5014,                        /* Unexpected error encountered during execution */
    afpNoMoreLocks              = -5015,                        /* Maximum lock limit reached */
    afpNoServer                 = -5016,                        /* Server not responding */
    afpObjectExists             = -5017,                        /* Specified destination file or directory already exists */
    afpObjectNotFound           = -5018,                        /* Specified file or directory does not exist */
    afpParmErr                  = -5019,                        /* A specified parameter was out of allowable range */
    afpRangeNotLocked           = -5020,                        /* Tried to unlock range that was not locked by user */
    afpRangeOverlap             = -5021,                        /* Some or all of range already locked by same user */
    afpSessClosed               = -5022,                        /* Session closed*/
    afpUserNotAuth              = -5023,                        /* No AFPLogin call has successfully been made for this session */
    afpCallNotSupported         = -5024,                        /* Unsupported AFP call was made */
    afpObjectTypeErr            = -5025,                        /* File/Directory specified where Directory/File expected */
    afpTooManyFilesOpen         = -5026,                        /* Maximum open file count reached */
    afpServerGoingDown          = -5027,                        /* Server is shutting down */
    afpCantRename               = -5028,                        /* AFPRename cannot rename volume */
    afpDirNotFound              = -5029,                        /* Unknown directory specified */
    afpIconTypeError            = -5030,                        /* Icon size specified different from existing icon size */
    afpVolLocked                = -5031,                        /* Volume is Read-Only */
    afpObjectLocked             = -5032,                        /* Object is M/R/D/W inhibited*/
    afpContainsSharedErr        = -5033,                        /* the folder being shared contains a shared folder*/
    afpIDNotFound               = -5034,
    afpIDExists                 = -5035,
    afpDiffVolErr               = -5036,
    afpCatalogChanged           = -5037,
    afpSameObjectErr            = -5038,
    afpBadIDErr                 = -5039,
    afpPwdSameErr               = -5040,                        /* Someone tried to change their password to the same password on a mantadory password change */
    afpPwdTooShortErr           = -5041,                        /* The password being set is too short: there is a minimum length that must be met or exceeded */
    afpPwdExpiredErr            = -5042,                        /* The password being used is too old: this requires the user to change the password before log-in can continue */
    afpInsideSharedErr          = -5043,                        /* The folder being shared is inside a shared folder OR the folder contains a shared folder and is being moved into a shared folder */
                                                                /* OR the folder contains a shared folder and is being moved into the descendent of a shared folder.*/
    afpInsideTrashErr           = -5044,                        /* The folder being shared is inside the trash folder OR the shared folder is being moved into the trash folder */
                                                                /* OR the folder is being moved to the trash and it contains a shared folder */
    afpPwdNeedsChangeErr        = -5045,                        /* The password needs to be changed*/
    afpPwdPolicyErr             = -5046,                        /* Password does not conform to servers password policy */
    afpAlreadyLoggedInErr       = -5047,                        /* User has been authenticated but is already logged in from another machine (and that's not allowed on this server) */
    afpCallNotAllowed           = -5048                         /* The server knows what you wanted to do, but won't let you do it just now */
};

enum {
                                                                /*  AppleShare Client Errors */
    afpBadDirIDType             = -5060,
    afpCantMountMoreSrvre       = -5061,                        /* The Maximum number of server connections has been reached */
    afpAlreadyMounted           = -5062,                        /* The volume is already mounted */
    afpSameNodeErr              = -5063                         /* An Attempt was made to connect to a file server running on the same machine */
};



/*Text Engines, TSystemTextEngines, HIEditText error coded*/

enum {
    errUnknownAttributeTag      = -5240,
    errMarginWilllNotFit        = -5241,
    errNotInImagingMode         = -5242,
    errAlreadyInImagingMode     = -5243,
    errEngineNotFound           = -5244,
    errIteratorReachedEnd       = -5245,
    errInvalidRange             = -5246,
    errOffsetNotOnElementBounday = -5247,
    errNoHiliteText             = -5248,
    errEmptyScrap               = -5249,
    errReadOnlyText             = -5250,
    errUnknownElement           = -5251,
    errNonContiuousAttribute    = -5252,
    errCannotUndo               = -5253
};


/* HTMLRendering OSStaus codes*/
enum {
    hrHTMLRenderingLibNotInstalledErr = -5360,
    hrMiscellaneousExceptionErr = -5361,
    hrUnableToResizeHandleErr   = -5362
};


/* IAExtractor result codes */
enum {
    errIANoErr                  = 0,
    errIAUnknownErr             = -5380,
    errIAAllocationErr          = -5381,
    errIAParamErr               = -5382,
    errIANoMoreItems            = -5383,
    errIABufferTooSmall         = -5384,
    errIACanceled               = -5385,
    errIAInvalidDocument        = -5386,
    errIATextExtractionErr      = -5387,
    errIAEndOfTextRun           = -5388
};


/* QuickTime Streaming Errors */
enum {
    qtsBadSelectorErr           = -5400,
    qtsBadStateErr              = -5401,
    qtsBadDataErr               = -5402,                        /* something is wrong with the data */
    qtsUnsupportedDataTypeErr   = -5403,
    qtsUnsupportedRateErr       = -5404,
    qtsUnsupportedFeatureErr    = -5405,
    qtsTooMuchDataErr           = -5406,
    qtsUnknownValueErr          = -5407,
    qtsTimeoutErr               = -5408,
    qtsConnectionFailedErr      = -5420,
    qtsAddressBusyErr           = -5421
};


enum {
                                                                /*Gestalt error codes*/
    gestaltUnknownErr           = -5550,                        /*value returned if Gestalt doesn't know the answer*/
    gestaltUndefSelectorErr     = -5551,                        /*undefined selector was passed to Gestalt*/
    gestaltDupSelectorErr       = -5552,                        /*tried to add an entry that already existed*/
    gestaltLocationErr          = -5553                         /*gestalt function ptr wasn't in sysheap*/
};


/* Menu Manager error codes*/
enum {
    menuPropertyInvalidErr      = -5603,                        /* invalid property creator */
    menuPropertyInvalid         = menuPropertyInvalidErr,       /* "menuPropertyInvalid" is deprecated */
    menuPropertyNotFoundErr     = -5604,                        /* specified property wasn't found */
    menuNotFoundErr             = -5620,                        /* specified menu or menu ID wasn't found */
    menuUsesSystemDefErr        = -5621,                        /* GetMenuDefinition failed because the menu uses the system MDEF */
    menuItemNotFoundErr         = -5622,                        /* specified menu item wasn't found*/
    menuInvalidErr              = -5623                         /* menu is invalid*/
};


/* Window Manager error codes*/
enum {
    errInvalidWindowPtr         = -5600,                        /* tried to pass a bad WindowRef argument*/
    errInvalidWindowRef         = -5600,                        /* tried to pass a bad WindowRef argument*/
    errUnsupportedWindowAttributesForClass = -5601,             /* tried to create a window with WindowAttributes not supported by the WindowClass*/
    errWindowDoesNotHaveProxy   = -5602,                        /* tried to do something requiring a proxy to a window which doesn't have a proxy*/
    errInvalidWindowProperty    = -5603,                        /* tried to access a property tag with private creator*/
    errWindowPropertyNotFound   = -5604,                        /* tried to get a nonexistent property*/
    errUnrecognizedWindowClass  = -5605,                        /* tried to create a window with a bad WindowClass*/
    errCorruptWindowDescription = -5606,                        /* tried to load a corrupt window description (size or version fields incorrect)*/
    errUserWantsToDragWindow    = -5607,                        /* if returned from TrackWindowProxyDrag, you should call DragWindow on the window*/
    errWindowsAlreadyInitialized = -5608,                       /* tried to call InitFloatingWindows twice, or called InitWindows and then floating windows*/
    errFloatingWindowsNotInitialized = -5609,                   /* called HideFloatingWindows or ShowFloatingWindows without calling InitFloatingWindows*/
    errWindowNotFound           = -5610                         /* returned from FindWindowOfClass*/
};


/* Dialog Mgr error codes*/
enum {
    dialogNoTimeoutErr          = -5640
};


/* NavigationLib error codes*/
enum {
    kNavInvalidSystemConfigErr  = -5696,
    kNavCustomControlMessageFailedErr = -5697,
    kNavInvalidCustomControlMessageErr = -5698,
    kNavMissingKindStringErr    = -5699
};


/* Collection Manager errors */
enum {
    collectionItemLockedErr     = -5750,
    collectionItemNotFoundErr   = -5751,
    collectionIndexRangeErr     = -5752,
    collectionVersionErr        = -5753
};


/* QuickTime Streaming Server Errors */
enum {
    kQTSSUnknownErr             = -6150
};


enum {
                                                                /* Display Manager error codes (-6220...-6269)*/
    kDMGenErr                   = -6220,                        /*Unexpected Error*/
                                                                /* Mirroring-Specific Errors */
    kDMMirroringOnAlready       = -6221,                        /*Returned by all calls that need mirroring to be off to do their thing.*/
    kDMWrongNumberOfDisplays    = -6222,                        /*Can only handle 2 displays for now.*/
    kDMMirroringBlocked         = -6223,                        /*DMBlockMirroring() has been called.*/
    kDMCantBlock                = -6224,                        /*Mirroring is already on, can't Block now (call DMUnMirror() first).*/
    kDMMirroringNotOn           = -6225,                        /*Returned by all calls that need mirroring to be on to do their thing.*/
                                                                /* Other Display Manager Errors */
    kSysSWTooOld                = -6226,                        /*Missing critical pieces of System Software.*/
    kDMSWNotInitializedErr      = -6227,                        /*Required software not initialized (eg windowmanager or display mgr).*/
    kDMDriverNotDisplayMgrAwareErr = -6228,                     /*Video Driver does not support display manager.*/
    kDMDisplayNotFoundErr       = -6229,                        /*Could not find item (will someday remove).*/
    kDMNotFoundErr              = -6229,                        /*Could not find item.*/
    kDMDisplayAlreadyInstalledErr = -6230,                      /*Attempt to add an already installed display.*/
    kDMMainDisplayCannotMoveErr = -6231,                        /*Trying to move main display (or a display mirrored to it) */
    kDMNoDeviceTableclothErr    = -6231,                        /*obsolete*/
    kDMFoundErr                 = -6232                         /*Did not proceed because we found an item*/
};


/*
    Language Analysis error codes
*/
enum {
    laTooSmallBufferErr         = -6984,                        /* output buffer is too small to store any result */
    laEnvironmentBusyErr        = -6985,                        /* specified environment is used */
    laEnvironmentNotFoundErr    = -6986,                        /* can't fint the specified environment */
    laEnvironmentExistErr       = -6987,                        /* same name environment is already exists */
    laInvalidPathErr            = -6988,                        /* path is not correct */
    laNoMoreMorphemeErr         = -6989,                        /* nothing to read*/
    laFailAnalysisErr           = -6990,                        /* analysis failed*/
    laTextOverFlowErr           = -6991,                        /* text is too long*/
    laDictionaryNotOpenedErr    = -6992,                        /* the dictionary is not opened*/
    laDictionaryUnknownErr      = -6993,                        /* can't use this dictionary with this environment*/
    laDictionaryTooManyErr      = -6994,                        /* too many dictionaries*/
    laPropertyValueErr          = -6995,                        /* Invalid property value*/
    laPropertyUnknownErr        = -6996,                        /* the property is unknown to this environment*/
    laPropertyIsReadOnlyErr     = -6997,                        /* the property is read only*/
    laPropertyNotFoundErr       = -6998,                        /* can't find the property*/
    laPropertyErr               = -6999,                        /* Error in properties*/
    laEngineNotFoundErr         = -7000                         /* can't find the engine*/
};


enum {
    kUSBNoErr                   = 0,
    kUSBNoTran                  = 0,
    kUSBNoDelay                 = 0,
    kUSBPending                 = 1
};

/*
   
   USB Hardware Errors 
   Note pipe stalls are communication 
   errors. The affected pipe can not 
   be used until USBClearPipeStallByReference  
   is used.
   kUSBEndpointStallErr is returned in 
   response to a stall handshake 
   from a device. The device has to be 
   cleared before a USBClearPipeStallByReference 
   can be used.
*/
enum {
    kUSBNotSent2Err             = -6901,                        /*  Transaction not sent */
    kUSBNotSent1Err             = -6902,                        /*  Transaction not sent */
    kUSBBufUnderRunErr          = -6903,                        /*  Host hardware failure on data out, PCI busy? */
    kUSBBufOvrRunErr            = -6904,                        /*  Host hardware failure on data in, PCI busy? */
    kUSBRes2Err                 = -6905,
    kUSBRes1Err                 = -6906,
    kUSBUnderRunErr             = -6907,                        /*  Less data than buffer */
    kUSBOverRunErr              = -6908,                        /*  Packet too large or more data than buffer */
    kUSBWrongPIDErr             = -6909,                        /*  Pipe stall, Bad or wrong PID */
    kUSBPIDCheckErr             = -6910,                        /*  Pipe stall, PID CRC error */
    kUSBNotRespondingErr        = -6911,                        /*  Pipe stall, No device, device hung */
    kUSBEndpointStallErr        = -6912,                        /*  Device didn't understand */
    kUSBDataToggleErr           = -6913,                        /*  Pipe stall, Bad data toggle */
    kUSBBitstufErr              = -6914,                        /*  Pipe stall, bitstuffing */
    kUSBCRCErr                  = -6915,                        /*  Pipe stall, bad CRC */
    kUSBLinkErr                 = -6916
};


/*
   
   USB Manager Errors 
*/
enum {
    kUSBQueueFull               = -6948,                        /* Internal queue maxxed  */
    kUSBNotHandled              = -6987,                        /* Notification was not handled   (same as NotFound)*/
    kUSBUnknownNotification     = -6949,                        /* Notification type not defined  */
    kUSBBadDispatchTable        = -6950                         /* Improper driver dispatch table     */
};


/* USB Services Errors */
enum {
    kUSBQueueAborted            = -6970,                        /* Pipe zero stall cleared.*/
    kUSBTimedOut                = -6971,                        /* Transaction timed out. */
    kUSBDeviceDisconnected      = -6972,                        /* Disconnected during suspend or reset */
    kUSBDeviceNotSuspended      = -6973,                        /* device is not suspended for resume */
    kUSBDeviceSuspended         = -6974,                        /* Device is suspended */
    kUSBInvalidBuffer           = -6975,                        /* bad buffer, usually nil */
    kUSBDevicePowerProblem      = -6976,                        /*  Device has a power problem */
    kUSBDeviceBusy              = -6977,                        /*  Device is already being configured */
    kUSBUnknownInterfaceErr     = -6978,                        /*  Interface ref not recognised */
    kUSBPipeStalledError        = -6979,                        /*  Pipe has stalled, error needs to be cleared */
    kUSBPipeIdleError           = -6980,                        /*  Pipe is Idle, it will not accept transactions */
    kUSBNoBandwidthError        = -6981,                        /*  Not enough bandwidth available */
    kUSBAbortedError            = -6982,                        /*  Pipe aborted */
    kUSBFlagsError              = -6983,                        /*  Unused flags not zeroed */
    kUSBCompletionError         = -6984,                        /*  no completion routine specified */
    kUSBPBLengthError           = -6985,                        /*  pbLength too small */
    kUSBPBVersionError          = -6986,                        /*  Wrong pbVersion */
    kUSBNotFound                = -6987,                        /*  Not found */
    kUSBOutOfMemoryErr          = -6988,                        /*  Out of memory */
    kUSBDeviceErr               = -6989,                        /*  Device error */
    kUSBNoDeviceErr             = -6990,                        /*  No device*/
    kUSBAlreadyOpenErr          = -6991,                        /*  Already open */
    kUSBTooManyTransactionsErr  = -6992,                        /*  Too many transactions */
    kUSBUnknownRequestErr       = -6993,                        /*  Unknown request */
    kUSBRqErr                   = -6994,                        /*  Request error */
    kUSBIncorrectTypeErr        = -6995,                        /*  Incorrect type */
    kUSBTooManyPipesErr         = -6996,                        /*  Too many pipes */
    kUSBUnknownPipeErr          = -6997,                        /*  Pipe ref not recognised */
    kUSBUnknownDeviceErr        = -6998,                        /*  device ref not recognised */
    kUSBInternalErr             = -6999                         /* Internal error */
};


/*
    DictionaryMgr error codes
*/
enum {
    dcmParamErr                 = -7100,                        /* bad parameter*/
    dcmNotDictionaryErr         = -7101,                        /* not dictionary*/
    dcmBadDictionaryErr         = -7102,                        /* invalid dictionary*/
    dcmPermissionErr            = -7103,                        /* invalid permission*/
    dcmDictionaryNotOpenErr     = -7104,                        /* dictionary not opened*/
    dcmDictionaryBusyErr        = -7105,                        /* dictionary is busy*/
    dcmBlockFullErr             = -7107,                        /* dictionary block full*/
    dcmNoRecordErr              = -7108,                        /* no such record*/
    dcmDupRecordErr             = -7109,                        /* same record already exist*/
    dcmNecessaryFieldErr        = -7110,                        /* lack required/identify field*/
    dcmBadFieldInfoErr          = -7111,                        /* incomplete information*/
    dcmBadFieldTypeErr          = -7112,                        /* no such field type supported*/
    dcmNoFieldErr               = -7113,                        /* no such field exist*/
    dcmBadKeyErr                = -7115,                        /* bad key information*/
    dcmTooManyKeyErr            = -7116,                        /* too many key field*/
    dcmBadDataSizeErr           = -7117,                        /* too big data size*/
    dcmBadFindMethodErr         = -7118,                        /* no such find method supported*/
    dcmBadPropertyErr           = -7119,                        /* no such property exist*/
    dcmProtectedErr             = -7121,                        /* need keyword to use dictionary*/
    dcmNoAccessMethodErr        = -7122,                        /* no such AccessMethod*/
    dcmBadFeatureErr            = -7124,                        /* invalid AccessMethod feature*/
    dcmIterationCompleteErr     = -7126,                        /* no more item in iterator*/
    dcmBufferOverflowErr        = -7127                         /* data is larger than buffer size*/
};


/* ATSUnicode error codes*/
enum {
    kATSUInvalidTextLayoutErr   = -8790,                        /*    An attempt was made to use a ATSUTextLayout */
                                                                /*    which hadn't been initialized or is otherwise */
                                                                /*    in an invalid state. */
    kATSUInvalidStyleErr        = -8791,                        /*    An attempt was made to use a ATSUStyle which  */
                                                                /*    hadn't been properly allocated or is otherwise  */
                                                                /*    in an invalid state.  */
    kATSUInvalidTextRangeErr    = -8792,                        /*    An attempt was made to extract information   */
                                                                /*    from or perform an operation on a ATSUTextLayout */
                                                                /*    for a range of text not covered by the ATSUTextLayout.  */
    kATSUFontsMatched           = -8793,                        /*    This is not an error code but is returned by    */
                                                                /*    ATSUMatchFontsToText() when changes need to    */
                                                                /*    be made to the fonts associated with the text.  */
    kATSUFontsNotMatched        = -8794,                        /*    This value is returned by ATSUMatchFontsToText()    */
                                                                /*    when the text contains Unicode characters which    */
                                                                /*    cannot be represented by any installed font.  */
    kATSUNoCorrespondingFontErr = -8795,                        /*    This value is retrned by font ID conversion */
                                                                /*    routines ATSUFONDtoFontID() and ATSUFontIDtoFOND() */
                                                                /*    to indicate that the input font ID is valid but */
                                                                /*    there is no conversion possible.  For example, */
                                                                /*    data-fork fonts can only be used with ATSUI not */
                                                                /*    the FontManager, and so converting an ATSUIFontID */
                                                                /*    for such a font will fail.   */
    kATSUInvalidFontErr         = -8796,                        /*    Used when an attempt was made to use an invalid font ID.*/
    kATSUInvalidAttributeValueErr = -8797,                      /*    Used when an attempt was made to use an attribute with */
                                                                /*    a bad or undefined value.  */
    kATSUInvalidAttributeSizeErr = -8798,                       /*    Used when an attempt was made to use an attribute with a */
                                                                /*    bad size.  */
    kATSUInvalidAttributeTagErr = -8799,                        /*    Used when an attempt was made to use a tag value that*/
                                                                /*    was not appropriate for the function call it was used.  */
    kATSUInvalidCacheErr        = -8800,                        /*    Used when an attempt was made to read in style data */
                                                                /*    from an invalid cache.  Either the format of the */
                                                                /*    cached data doesn't match that used by Apple Type */
                                                                /*    Services for Unicode(tm) Imaging, or the cached data */
                                                                /*    is corrupt.  */
    kATSUNotSetErr              = -8801,                        /*    Used when the client attempts to retrieve an attribute, */
                                                                /*    font feature, or font variation from a style when it */
                                                                /*    hadn't been set.  In such a case, the default value will*/
                                                                /*    be returned for the attribute's value.*/
    kATSUNoStyleRunsAssignedErr = -8802,                        /*    Used when an attempt was made to measure, highlight or draw*/
                                                                /*    a ATSUTextLayout object that has no styleRuns associated with it.*/
    kATSUQuickDrawTextErr       = -8803,                        /*    Used when QuickDraw Text encounters an error rendering or measuring*/
                                                                /*    a line of ATSUI text.*/
    kATSULowLevelErr            = -8804,                        /*    Used when an error was encountered within the low level ATS */
                                                                /*    mechanism performing an operation requested by ATSUI.*/
    kATSUNoFontCmapAvailableErr = -8805,                        /*    Used when no CMAP table can be accessed or synthesized for the */
                                                                /*    font passed into a SetAttributes Font call.*/
    kATSUNoFontScalerAvailableErr = -8806,                      /*    Used when no font scaler is available for the font passed*/
                                                                /*    into a SetAttributes Font call.*/
    kATSUCoordinateOverflowErr  = -8807,                        /*    Used to indicate the coordinates provided to an ATSUI routine caused*/
                                                                /*    a coordinate overflow (i.e. > 32K).*/
    kATSULineBreakInWord        = -8808,                        /*    This is not an error code but is returned by ATSUBreakLine to */
                                                                /*    indicate that the returned offset is within a word since there was*/
                                                                /*    only less than one word that could fit the requested width.*/
    kATSULastErr                = -8809                         /*    The last ATSUI error code.*/
};


/* Error & status codes for general text and text encoding conversion*/

enum {
                                                                /* general text errors*/
    kTextUnsupportedEncodingErr = -8738,                        /* specified encoding not supported for this operation*/
    kTextMalformedInputErr      = -8739,                        /* in DBCS, for example, high byte followed by invalid low byte*/
    kTextUndefinedElementErr    = -8740,                        /* text conversion errors*/
    kTECMissingTableErr         = -8745,
    kTECTableChecksumErr        = -8746,
    kTECTableFormatErr          = -8747,
    kTECCorruptConverterErr     = -8748,                        /* invalid converter object reference*/
    kTECNoConversionPathErr     = -8749,
    kTECBufferBelowMinimumSizeErr = -8750,                      /* output buffer too small to allow processing of first input text element*/
    kTECArrayFullErr            = -8751,                        /* supplied name buffer or TextRun, TextEncoding, or UnicodeMapping array is too small*/
    kTECBadTextRunErr           = -8752,
    kTECPartialCharErr          = -8753,                        /* input buffer ends in the middle of a multibyte character, conversion stopped*/
    kTECUnmappableElementErr    = -8754,
    kTECIncompleteElementErr    = -8755,                        /* text element may be incomplete or is too long for internal buffers*/
    kTECDirectionErr            = -8756,                        /* direction stack overflow, etc.*/
    kTECGlobalsUnavailableErr   = -8770,                        /* globals have already been deallocated (premature TERM)*/
    kTECItemUnavailableErr      = -8771,                        /* item (e.g. name) not available for specified region (& encoding if relevant)*/
                                                                /* text conversion status codes*/
    kTECUsedFallbacksStatus     = -8783,
    kTECNeedFlushStatus         = -8784,
    kTECOutputBufferFullStatus  = -8785,                        /* output buffer has no room for conversion of next input text element (partial conversion)*/
                                                                /* deprecated error & status codes for low-level converter*/
    unicodeChecksumErr          = -8769,
    unicodeNoTableErr           = -8768,
    unicodeVariantErr           = -8767,
    unicodeFallbacksErr         = -8766,
    unicodePartConvertErr       = -8765,
    unicodeBufErr               = -8764,
    unicodeCharErr              = -8763,
    unicodeElementErr           = -8762,
    unicodeNotFoundErr          = -8761,
    unicodeTableFormatErr       = -8760,
    unicodeDirectionErr         = -8759,
    unicodeContextualErr        = -8758,
    unicodeTextEncodingDataErr  = -8757
};


/* UTCUtils Status Codes */
enum {
    kUTCUnderflowErr            = -8850,
    kUTCOverflowErr             = -8851,
    kIllegalClockValueErr       = -8852
};


/* QuickTime errors (Image Compression Manager) */
enum {
    codecErr                    = -8960,
    noCodecErr                  = -8961,
    codecUnimpErr               = -8962,
    codecSizeErr                = -8963,
    codecScreenBufErr           = -8964,
    codecImageBufErr            = -8965,
    codecSpoolErr               = -8966,
    codecAbortErr               = -8967,
    codecWouldOffscreenErr      = -8968,
    codecBadDataErr             = -8969,
    codecDataVersErr            = -8970,
    codecExtensionNotFoundErr   = -8971,
    scTypeNotFoundErr           = codecExtensionNotFoundErr,
    codecConditionErr           = -8972,
    codecOpenErr                = -8973,
    codecCantWhenErr            = -8974,
    codecCantQueueErr           = -8975,
    codecNothingToBlitErr       = -8976,
    codecNoMemoryPleaseWaitErr  = -8977,
    codecDisabledErr            = -8978,                        /* codec disabled itself -- pass codecFlagReenable to reset*/
    codecNeedToFlushChainErr    = -8979,
    lockPortBitsBadSurfaceErr   = -8980,
    lockPortBitsWindowMovedErr  = -8981,
    lockPortBitsWindowResizedErr = -8982,
    lockPortBitsWindowClippedErr = -8983,
    lockPortBitsBadPortErr      = -8984,
    lockPortBitsSurfaceLostErr  = -8985,
    codecParameterDialogConfirm = -8986,
    codecNeedAccessKeyErr       = -8987,                        /* codec needs password in order to decompress*/
    codecOffscreenFailedErr     = -8988,
    codecDroppedFrameErr        = -8989,                        /* returned from ImageCodecDrawBand */
    directXObjectAlreadyExists  = -8990,
    lockPortBitsWrongGDeviceErr = -8991,
    codecOffscreenFailedPleaseRetryErr = -8992,
    badCodecCharacterizationErr = -8993,
    noThumbnailFoundErr         = -8994
};


/* PCCard error codes */
enum {
    kBadAdapterErr              = -9050,                        /* invalid adapter number*/
    kBadAttributeErr            = -9051,                        /* specified attributes field value is invalid*/
    kBadBaseErr                 = -9052,                        /* specified base system memory address is invalid*/
    kBadEDCErr                  = -9053,                        /* specified EDC generator specified is invalid*/
    kBadIRQErr                  = -9054,                        /* specified IRQ level is invalid*/
    kBadOffsetErr               = -9055,                        /* specified PC card memory array offset is invalid*/
    kBadPageErr                 = -9056,                        /* specified page is invalid*/
    kBadSizeErr                 = -9057,                        /* specified size is invalid*/
    kBadSocketErr               = -9058,                        /* specified logical or physical socket number is invalid*/
    kBadTypeErr                 = -9059,                        /* specified window or interface type is invalid*/
    kBadVccErr                  = -9060,                        /* specified Vcc power level index is invalid*/
    kBadVppErr                  = -9061,                        /* specified Vpp1 or Vpp2 power level index is invalid*/
    kBadWindowErr               = -9062,                        /* specified window is invalid*/
    kBadArgLengthErr            = -9063,                        /* ArgLength argument is invalid*/
    kBadArgsErr                 = -9064,                        /* values in argument packet are invalid*/
    kBadHandleErr               = -9065,                        /* clientHandle is invalid*/
    kBadCISErr                  = -9066,                        /* CIS on card is invalid*/
    kBadSpeedErr                = -9067,                        /* specified speed is unavailable*/
    kReadFailureErr             = -9068,                        /* unable to complete read request*/
    kWriteFailureErr            = -9069,                        /* unable to complete write request*/
    kGeneralFailureErr          = -9070,                        /* an undefined error has occurred*/
    kNoCardErr                  = -9071,                        /* no PC card in the socket*/
    kUnsupportedFunctionErr     = -9072,                        /* function is not supported by this implementation*/
    kUnsupportedModeErr         = -9073,                        /* mode is not supported*/
    kBusyErr                    = -9074,                        /* unable to process request at this time - try later*/
    kWriteProtectedErr          = -9075,                        /* media is write-protected*/
    kConfigurationLockedErr     = -9076,                        /* a configuration has already been locked*/
    kInUseErr                   = -9077,                        /* requested resource is being used by a client*/
    kNoMoreItemsErr             = -9078,                        /* there are no more of the requested item*/
    kOutOfResourceErr           = -9079,                        /* Card Services has exhausted the resource*/
    kNoCardSevicesSocketsErr    = -9080,
    kInvalidRegEntryErr         = -9081,
    kBadLinkErr                 = -9082,
    kBadDeviceErr               = -9083,
    k16BitCardErr               = -9084,
    kCardBusCardErr             = -9085,
    kPassCallToChainErr         = -9086,
    kCantConfigureCardErr       = -9087,
    kPostCardEventErr           = -9088,                        /* _PCCSLPostCardEvent failed and dropped an event */
    kInvalidDeviceNumber        = -9089,
    kUnsupportedVsErr           = -9090,                        /* Unsupported Voltage Sense */
    kInvalidCSClientErr         = -9091,                        /* Card Services ClientID is not registered */
    kBadTupleDataErr            = -9092,                        /* Data in tuple is invalid */
    kBadCustomIFIDErr           = -9093,                        /* Custom interface ID is invalid */
    kNoIOWindowRequestedErr     = -9094,                        /* Request I/O window before calling configuration */
    kNoMoreTimerClientsErr      = -9095,                        /* All timer callbacks are in use */
    kNoMoreInterruptSlotsErr    = -9096,                        /* All internal Interrupt slots are in use */
    kNoClientTableErr           = -9097,                        /* The client table has not be initialized yet */
    kUnsupportedCardErr         = -9098,                        /* Card not supported by generic enabler*/
    kNoCardEnablersFoundErr     = -9099,                        /* No Enablers were found*/
    kNoEnablerForCardErr        = -9100,                        /* No Enablers were found that can support the card*/
    kNoCompatibleNameErr        = -9101,                        /* There is no compatible driver name for this device*/
    kClientRequestDenied        = -9102,                        /* CS Clients should return this code inorder to */
                                                                /*   deny a request-type CS Event                */
    kNotReadyErr                = -9103,                        /* PC Card failed to go ready */
    kTooManyIOWindowsErr        = -9104,                        /* device requested more than one I/O window */
    kAlreadySavedStateErr       = -9105,                        /* The state has been saved on previous call */
    kAttemptDupCardEntryErr     = -9106,                        /* The Enabler was asked to create a duplicate card entry */
    kCardPowerOffErr            = -9107,                        /* Power to the card has been turned off */
    kNotZVCapableErr            = -9108,                        /* This socket does not support Zoomed Video */
    kNoCardBusCISErr            = -9109                         /* No valid CIS exists for this CardBus card */
};


/* QuickTime errors (Sequence Grabber) */
enum {
    noDeviceForChannel          = -9400,
    grabTimeComplete            = -9401,
    cantDoThatInCurrentMode     = -9402,
    notEnoughMemoryToGrab       = -9403,
    notEnoughDiskSpaceToGrab    = -9404,
    couldntGetRequiredComponent = -9405,
    badSGChannel                = -9406,
    seqGrabInfoNotAvailable     = -9407,
    deviceCantMeetRequest       = -9408
};

/* QuickTime errors (Movie Controller) */
enum {
    badControllerHeight         = -9994,
    editingNotAllowed           = -9995,
    controllerBoundsNotExact    = -9996,
    cannotSetWidthOfAttachedController = -9997,
    controllerHasFixedHeight    = -9998,
    cannotMoveAttachedController = -9999
};

/* AERegistry Errors */
enum {
    errAEBadKeyForm             = -10002,
    errAECantHandleClass        = -10010,
    errAECantSupplyType         = -10009,
    errAECantUndo               = -10015,
    errAEEventFailed            = -10000,
    errAEIndexTooLarge          = -10007,
    errAEInTransaction          = -10011,
    errAELocalOnly              = -10016,
    errAENoSuchTransaction      = -10012,
    errAENotAnElement           = -10008,
    errAENotASingleObject       = -10014,
    errAENotModifiable          = -10003,
    errAENoUserSelection        = -10013,
    errAEPrivilegeError         = -10004,
    errAEReadDenied             = -10005,
    errAETypeError              = -10001,
    errAEWriteDenied            = -10006,
    errAENotAnEnumMember        = -10023,                       /* enumerated value in SetData is not allowed for this property */
    errAECantPutThatThere       = -10024,                       /* in make new, duplicate, etc. class can't be an element of container */
    errAEPropertiesClash        = -10025                        /* illegal combination of properties settings for Set Data, make new, or duplicate */
};

/* TELErr */
enum {
    telGenericError             = -1,
    telNoErr                    = 0,
    telNoTools                  = 8,                            /* no telephone tools found in extension folder */
    telBadTermErr               = -10001,                       /* invalid TELHandle or handle not found*/
    telBadDNErr                 = -10002,                       /* TELDNHandle not found or invalid */
    telBadCAErr                 = -10003,                       /* TELCAHandle not found or invalid */
    telBadHandErr               = -10004,                       /* bad handle specified */
    telBadProcErr               = -10005,                       /* bad msgProc specified */
    telCAUnavail                = -10006,                       /* a CA is not available */
    telNoMemErr                 = -10007,                       /* no memory to allocate handle */
    telNoOpenErr                = -10008,                       /* unable to open terminal */
    telBadHTypeErr              = -10010,                       /* bad hook type specified */
    telHTypeNotSupp             = -10011,                       /* hook type not supported by this tool */
    telBadLevelErr              = -10012,                       /* bad volume level setting */
    telBadVTypeErr              = -10013,                       /* bad volume type error */
    telVTypeNotSupp             = -10014,                       /* volume type not supported by this tool*/
    telBadAPattErr              = -10015,                       /* bad alerting pattern specified */
    telAPattNotSupp             = -10016,                       /* alerting pattern not supported by tool*/
    telBadIndex                 = -10017,                       /* bad index specified */
    telIndexNotSupp             = -10018,                       /* index not supported by this tool */
    telBadStateErr              = -10019,                       /* bad device state specified */
    telStateNotSupp             = -10020,                       /* device state not supported by tool */
    telBadIntExt                = -10021,                       /* bad internal external error */
    telIntExtNotSupp            = -10022,                       /* internal external type not supported by this tool */
    telBadDNDType               = -10023,                       /* bad DND type specified */
    telDNDTypeNotSupp           = -10024,                       /* DND type is not supported by this tool */
    telFeatNotSub               = -10030,                       /* feature not subscribed */
    telFeatNotAvail             = -10031,                       /* feature subscribed but not available */
    telFeatActive               = -10032,                       /* feature already active */
    telFeatNotSupp              = -10033,                       /* feature program call not supported by this tool */
    telConfLimitErr             = -10040,                       /* limit specified is too high for this configuration */
    telConfNoLimit              = -10041,                       /* no limit was specified but required*/
    telConfErr                  = -10042,                       /* conference was not prepared */
    telConfRej                  = -10043,                       /* conference request was rejected */
    telTransferErr              = -10044,                       /* transfer not prepared */
    telTransferRej              = -10045,                       /* transfer request rejected */
    telCBErr                    = -10046,                       /* call back feature not set previously */
    telConfLimitExceeded        = -10047,                       /* attempt to exceed switch conference limits */
    telBadDNType                = -10050,                       /* DN type invalid */
    telBadPageID                = -10051,                       /* bad page ID specified*/
    telBadIntercomID            = -10052,                       /* bad intercom ID specified */
    telBadFeatureID             = -10053,                       /* bad feature ID specified */
    telBadFwdType               = -10054,                       /* bad fwdType specified */
    telBadPickupGroupID         = -10055,                       /* bad pickup group ID specified */
    telBadParkID                = -10056,                       /* bad park id specified */
    telBadSelect                = -10057,                       /* unable to select or deselect DN */
    telBadBearerType            = -10058,                       /* bad bearerType specified */
    telBadRate                  = -10059,                       /* bad rate specified */
    telDNTypeNotSupp            = -10060,                       /* DN type not supported by tool */
    telFwdTypeNotSupp           = -10061,                       /* forward type not supported by tool */
    telBadDisplayMode           = -10062,                       /* bad display mode specified */
    telDisplayModeNotSupp       = -10063,                       /* display mode not supported by tool */
    telNoCallbackRef            = -10064,                       /* no call back reference was specified, but is required */
    telAlreadyOpen              = -10070,                       /* terminal already open */
    telStillNeeded              = -10071,                       /* terminal driver still needed by someone else */
    telTermNotOpen              = -10072,                       /* terminal not opened via TELOpenTerm */
    telCANotAcceptable          = -10080,                       /* CA not "acceptable" */
    telCANotRejectable          = -10081,                       /* CA not "rejectable" */
    telCANotDeflectable         = -10082,                       /* CA not "deflectable" */
    telPBErr                    = -10090,                       /* parameter block error, bad format */
    telBadFunction              = -10091,                       /* bad msgCode specified */
                                                                /*    telNoTools        = -10101,        unable to find any telephone tools */
    telNoSuchTool               = -10102,                       /* unable to find tool with name specified */
    telUnknownErr               = -10103,                       /* unable to set config */
    telNoCommFolder             = -10106,                       /* Communications/Extensions f not found */
    telInitFailed               = -10107,                       /* initialization failed */
    telBadCodeResource          = -10108,                       /* code resource not found */
    telDeviceNotFound           = -10109,                       /* device not found */
    telBadProcID                = -10110,                       /* invalid procID */
    telValidateFailed           = -10111,                       /* telValidate failed */
    telAutoAnsNotOn             = -10112,                       /* autoAnswer in not turned on */
    telDetAlreadyOn             = -10113,                       /* detection is already turned on */
    telBadSWErr                 = -10114,                       /* Software not installed properly */
    telBadSampleRate            = -10115,                       /* incompatible sample rate */
    telNotEnoughdspBW           = -10116                        /* not enough real-time for allocation */
};


enum {
                                                                /*Power Manager Errors*/
    pmBusyErr                   = -13000,                       /*Power Mgr never ready to start handshake*/
    pmReplyTOErr                = -13001,                       /*Timed out waiting for reply*/
    pmSendStartErr              = -13002,                       /*during send, pmgr did not start hs*/
    pmSendEndErr                = -13003,                       /*during send, pmgr did not finish hs*/
    pmRecvStartErr              = -13004,                       /*during receive, pmgr did not start hs*/
    pmRecvEndErr                = -13005                        /*during receive, pmgr did not finish hs configured for this connection*/
};

/*Power Manager 2.0 Errors*/
enum {
    kPowerHandlerExistsForDeviceErr = -13006,
    kPowerHandlerNotFoundForDeviceErr = -13007,
    kPowerHandlerNotFoundForProcErr = -13008,
    kPowerMgtMessageNotHandled  = -13009,
    kPowerMgtRequestDenied      = -13010,
    kCantReportProcessorTemperatureErr = -13013,
    kProcessorTempRoutineRequiresMPLib2 = -13014,
    kNoSuchPowerSource          = -13020
};


/* Debugging library errors */
enum {
    debuggingExecutionContextErr = -13880,                      /* routine cannot be called at this time */
    debuggingDuplicateSignatureErr = -13881,                    /* componentSignature already registered */
    debuggingDuplicateOptionErr = -13882,                       /* optionSelectorNum already registered */
    debuggingInvalidSignatureErr = -13883,                      /* componentSignature not registered */
    debuggingInvalidOptionErr   = -13884,                       /* optionSelectorNum is not registered */
    debuggingInvalidNameErr     = -13885,                       /* componentName or optionName is invalid (NULL) */
    debuggingNoCallbackErr      = -13886,                       /* debugging component has no callback */
    debuggingNoMatchErr         = -13887                        /* debugging component or option not found at this index */
};


/* HID device driver error codes */
enum {
    kHIDVersionIncompatibleErr  = -13909,
    kHIDDeviceNotReady          = -13910                        /* The device is still initializing, try again later*/
};


/* HID error codes */
enum {
    kHIDSuccess                 = 0,
    kHIDInvalidRangePageErr     = -13923,
    kHIDReportIDZeroErr         = -13924,
    kHIDReportCountZeroErr      = -13925,
    kHIDReportSizeZeroErr       = -13926,
    kHIDUnmatchedDesignatorRangeErr = -13927,
    kHIDUnmatchedStringRangeErr = -13928,
    kHIDInvertedUsageRangeErr   = -13929,
    kHIDUnmatchedUsageRangeErr  = -13930,
    kHIDInvertedPhysicalRangeErr = -13931,
    kHIDInvertedLogicalRangeErr = -13932,
    kHIDBadLogicalMaximumErr    = -13933,
    kHIDBadLogicalMinimumErr    = -13934,
    kHIDUsagePageZeroErr        = -13935,
    kHIDEndOfDescriptorErr      = -13936,
    kHIDNotEnoughMemoryErr      = -13937,
    kHIDBadParameterErr         = -13938,
    kHIDNullPointerErr          = -13939,
    kHIDInvalidReportLengthErr  = -13940,
    kHIDInvalidReportTypeErr    = -13941,
    kHIDBadLogPhysValuesErr     = -13942,
    kHIDIncompatibleReportErr   = -13943,
    kHIDInvalidPreparsedDataErr = -13944,
    kHIDNotValueArrayErr        = -13945,
    kHIDUsageNotFoundErr        = -13946,
    kHIDValueOutOfRangeErr      = -13947,
    kHIDBufferTooSmallErr       = -13948,
    kHIDNullStateErr            = -13949,
    kHIDBaseError               = -13950
};


/* MacTextEditor error codes */
enum {
    kTXNEndIterationErr         = -22000,
    kTXNCannotAddFrameErr       = -22001,
    kTXNInvalidFrameIDErr       = -22002,
    kTXNIllegalToCrossDataBoundariesErr = -22003,
    kTXNUserCanceledOperationErr = -22004,
    kTXNBadDefaultFileTypeWarning = -22005,
    kTXNCannotSetAutoIndentErr  = -22006,
    kTXNRunIndexOutofBoundsErr  = -22007,
    kTXNNoMatchErr              = -22008,
    kTXNAttributeTagInvalidForRunErr = -22009,                  /*dataValue is set to this per invalid tag*/
    kTXNSomeOrAllTagsInvalidForRunErr = -22010,
    kTXNInvalidRunIndex         = -22011,
    kTXNAlreadyInitializedErr   = -22012,
    kTXNCannotTurnTSMOffWhenUsingUnicodeErr = -22013,
    kTXNCopyNotAllowedInEchoModeErr = -22014,
    kTXNDataTypeNotAllowedErr   = -22015
};




/*Possible errors from the PrinterStatus bottleneck*/
enum {
    printerStatusOpCodeNotSupportedErr = -25280
};


/* Keychain Manager error codes */
enum {
    errKCNotAvailable           = -25291,
    errKCReadOnly               = -25292,
    errKCAuthFailed             = -25293,
    errKCNoSuchKeychain         = -25294,
    errKCInvalidKeychain        = -25295,
    errKCDuplicateKeychain      = -25296,
    errKCDuplicateCallback      = -25297,
    errKCInvalidCallback        = -25298,
    errKCDuplicateItem          = -25299,
    errKCItemNotFound           = -25300,
    errKCBufferTooSmall         = -25301,
    errKCDataTooLarge           = -25302,
    errKCNoSuchAttr             = -25303,
    errKCInvalidItemRef         = -25304,
    errKCInvalidSearchRef       = -25305,
    errKCNoSuchClass            = -25306,
    errKCNoDefaultKeychain      = -25307,
    errKCInteractionNotAllowed  = -25308,
    errKCReadOnlyAttr           = -25309,
    errKCWrongKCVersion         = -25310,
    errKCKeySizeNotAllowed      = -25311,
    errKCNoStorageModule        = -25312,
    errKCNoCertificateModule    = -25313,
    errKCNoPolicyModule         = -25314,
    errKCInteractionRequired    = -25315,
    errKCDataNotAvailable       = -25316,
    errKCDataNotModifiable      = -25317,
    errKCCreateChainFailed      = -25318
};


/* UnicodeUtilities error & status codes*/
enum {
    kUCOutputBufferTooSmall     = -25340,                       /* Output buffer too small for Unicode string result*/
    kUCTextBreakLocatorMissingType = -25341                     /* Unicode text break error*/
};



/* Multiprocessing API error codes*/
enum {
    kMPIterationEndErr          = -29275,
    kMPPrivilegedErr            = -29276,
    kMPProcessCreatedErr        = -29288,
    kMPProcessTerminatedErr     = -29289,
    kMPTaskCreatedErr           = -29290,
    kMPTaskBlockedErr           = -29291,
    kMPTaskStoppedErr           = -29292,                       /* A convention used with MPThrowException.*/
    kMPBlueBlockingErr          = -29293,
    kMPDeletedErr               = -29295,
    kMPTimeoutErr               = -29296,
    kMPTaskAbortedErr           = -29297,
    kMPInsufficientResourcesErr = -29298,
    kMPInvalidIDErr             = -29299
};

/* StringCompare error codes (in TextUtils range)*/
enum {
    kCollateAttributesNotFoundErr = -29500,
    kCollateInvalidOptions      = -29501,
    kCollateMissingUnicodeTableErr = -29502,
    kCollateUnicodeConvertFailedErr = -29503,
    kCollatePatternNotFoundErr  = -29504,
    kCollateInvalidChar         = -29505,
    kCollateBufferTooSmall      = -29506,
    kCollateInvalidCollationRef = -29507
};


/* FontSync OSStatus Codes */
enum {
    kFNSInvalidReferenceErr     = -29580,                       /* ref. was NULL or otherwise bad */
    kFNSBadReferenceVersionErr  = -29581,                       /* ref. version is out of known range */
    kFNSInvalidProfileErr       = -29582,                       /* profile is NULL or otherwise bad */
    kFNSBadProfileVersionErr    = -29583,                       /* profile version is out of known range */
    kFNSDuplicateReferenceErr   = -29584,                       /* the ref. being added is already in the profile */
    kFNSMismatchErr             = -29585,                       /* reference didn't match or wasn't found in profile */
    kFNSInsufficientDataErr     = -29586,                       /* insufficient data for the operation */
    kFNSBadFlattenedSizeErr     = -29587,                       /* flattened size didn't match input or was too small */
    kFNSNameNotFoundErr         = -29589                        /* The name with the requested paramters was not found */
};



/* MacLocales error codes*/
enum {
    kLocalesBufferTooSmallErr   = -30001,
    kLocalesTableFormatErr      = -30002,
    kLocalesDefaultDisplayStatus = -30029                       /* Requested display locale unavailable, used default*/
};


/* Settings Manager (formerly known as Location Manager) Errors */
enum {
    kALMInternalErr             = -30049,
    kALMGroupNotFoundErr        = -30048,
    kALMNoSuchModuleErr         = -30047,
    kALMModuleCommunicationErr  = -30046,
    kALMDuplicateModuleErr      = -30045,
    kALMInstallationErr         = -30044,
    kALMDeferSwitchErr          = -30043,
    kALMRebootFlagsLevelErr     = -30042
};

enum {
    kALMLocationNotFoundErr     = kALMGroupNotFoundErr          /* Old name */
};


/* SoundSprocket Error Codes */
enum {
    kSSpInternalErr             = -30340,
    kSSpVersionErr              = -30341,
    kSSpCantInstallErr          = -30342,
    kSSpParallelUpVectorErr     = -30343,
    kSSpScaleToZeroErr          = -30344
};


/* NetSprocket Error Codes */
enum {
    kNSpInitializationFailedErr = -30360,
    kNSpAlreadyInitializedErr   = -30361,
    kNSpTopologyNotSupportedErr = -30362,
    kNSpPipeFullErr             = -30364,
    kNSpHostFailedErr           = -30365,
    kNSpProtocolNotAvailableErr = -30366,
    kNSpInvalidGameRefErr       = -30367,
    kNSpInvalidParameterErr     = -30369,
    kNSpOTNotPresentErr         = -30370,
    kNSpOTVersionTooOldErr      = -30371,
    kNSpMemAllocationErr        = -30373,
    kNSpAlreadyAdvertisingErr   = -30374,
    kNSpNotAdvertisingErr       = -30376,
    kNSpInvalidAddressErr       = -30377,
    kNSpFreeQExhaustedErr       = -30378,
    kNSpRemovePlayerFailedErr   = -30379,
    kNSpAddressInUseErr         = -30380,
    kNSpFeatureNotImplementedErr = -30381,
    kNSpNameRequiredErr         = -30382,
    kNSpInvalidPlayerIDErr      = -30383,
    kNSpInvalidGroupIDErr       = -30384,
    kNSpNoPlayersErr            = -30385,
    kNSpNoGroupsErr             = -30386,
    kNSpNoHostVolunteersErr     = -30387,
    kNSpCreateGroupFailedErr    = -30388,
    kNSpAddPlayerFailedErr      = -30389,
    kNSpInvalidDefinitionErr    = -30390,
    kNSpInvalidProtocolRefErr   = -30391,
    kNSpInvalidProtocolListErr  = -30392,
    kNSpTimeoutErr              = -30393,
    kNSpGameTerminatedErr       = -30394,
    kNSpConnectFailedErr        = -30395,
    kNSpSendFailedErr           = -30396,
    kNSpMessageTooBigErr        = -30397,
    kNSpCantBlockErr            = -30398,
    kNSpJoinFailedErr           = -30399
};


/* InputSprockets error codes */
enum {
    kISpInternalErr             = -30420,
    kISpSystemListErr           = -30421,
    kISpBufferToSmallErr        = -30422,
    kISpElementInListErr        = -30423,
    kISpElementNotInListErr     = -30424,
    kISpSystemInactiveErr       = -30425,
    kISpDeviceInactiveErr       = -30426,
    kISpSystemActiveErr         = -30427,
    kISpDeviceActiveErr         = -30428,
    kISpListBusyErr             = -30429
};

/* DrawSprockets error/warning codes */
enum {
    kDSpNotInitializedErr       = -30440L,
    kDSpSystemSWTooOldErr       = -30441L,
    kDSpInvalidContextErr       = -30442L,
    kDSpInvalidAttributesErr    = -30443L,
    kDSpContextAlreadyReservedErr = -30444L,
    kDSpContextNotReservedErr   = -30445L,
    kDSpContextNotFoundErr      = -30446L,
    kDSpFrameRateNotReadyErr    = -30447L,
    kDSpConfirmSwitchWarning    = -30448L,
    kDSpInternalErr             = -30449L,
    kDSpStereoContextErr        = -30450L
};


/*
   ***************************************************************************
   Find By Content errors are assigned in the range -30500 to -30539, inclusive.
   ***************************************************************************
*/
enum {
    kFBCvTwinExceptionErr       = -30500,                       /*no telling what it was*/
    kFBCnoIndexesFound          = -30501,
    kFBCallocFailed             = -30502,                       /*probably low memory*/
    kFBCbadParam                = -30503,
    kFBCfileNotIndexed          = -30504,
    kFBCbadIndexFile            = -30505,                       /*bad FSSpec, or bad data in file*/
    kFBCcompactionFailed        = -30506,                       /*V-Twin exception caught*/
    kFBCvalidationFailed        = -30507,                       /*V-Twin exception caught*/
    kFBCindexingFailed          = -30508,                       /*V-Twin exception caught*/
    kFBCcommitFailed            = -30509,                       /*V-Twin exception caught*/
    kFBCdeletionFailed          = -30510,                       /*V-Twin exception caught*/
    kFBCmoveFailed              = -30511,                       /*V-Twin exception caught*/
    kFBCtokenizationFailed      = -30512,                       /*couldn't read from document or query*/
    kFBCmergingFailed           = -30513,                       /*couldn't merge index files*/
    kFBCindexCreationFailed     = -30514,                       /*couldn't create index*/
    kFBCaccessorStoreFailed     = -30515,
    kFBCaddDocFailed            = -30516,
    kFBCflushFailed             = -30517,
    kFBCindexNotFound           = -30518,
    kFBCnoSearchSession         = -30519,
    kFBCindexingCanceled        = -30520,
    kFBCaccessCanceled          = -30521,
    kFBCindexFileDestroyed      = -30522,
    kFBCindexNotAvailable       = -30523,
    kFBCsearchFailed            = -30524,
    kFBCsomeFilesNotIndexed     = -30525,
    kFBCillegalSessionChange    = -30526,                       /*tried to add/remove vols to a session*/
                                                                /*that has hits*/
    kFBCanalysisNotAvailable    = -30527,
    kFBCbadIndexFileVersion     = -30528,
    kFBCsummarizationCanceled   = -30529,
    kFBCindexDiskIOFailed       = -30530,
    kFBCbadSearchSession        = -30531,
    kFBCnoSuchHit               = -30532
};


/* QuickTime VR Errors */
enum {
    notAQTVRMovieErr            = -30540,
    constraintReachedErr        = -30541,
    callNotSupportedByNodeErr   = -30542,
    selectorNotSupportedByNodeErr = -30543,
    invalidNodeIDErr            = -30544,
    invalidViewStateErr         = -30545,
    timeNotInViewErr            = -30546,
    propertyNotSupportedByNodeErr = -30547,
    settingNotSupportedByNodeErr = -30548,
    limitReachedErr             = -30549,
    invalidNodeFormatErr        = -30550,
    invalidHotSpotIDErr         = -30551,
    noMemoryNodeFailedInitialize = -30552,
    streamingNodeNotReadyErr    = -30553,
    qtvrLibraryLoadErr          = -30554,
    qtvrUninitialized           = -30555
};


/* Appearance Manager Error Codes */
enum {
    themeInvalidBrushErr        = -30560,                       /* pattern index invalid */
    themeProcessRegisteredErr   = -30561,
    themeProcessNotRegisteredErr = -30562,
    themeBadTextColorErr        = -30563,
    themeHasNoAccentsErr        = -30564,
    themeBadCursorIndexErr      = -30565,
    themeScriptFontNotFoundErr  = -30566,                       /* theme font requested for uninstalled script system */
    themeMonitorDepthNotSupportedErr = -30567                   /* theme not supported at monitor depth */
};


/*------------------------------------------------------------------------------------------------------*/
/*  Control Mgr Errors are in the range -30580 .. -30599                                                            */
/*------------------------------------------------------------------------------------------------------*/
enum {
    errMessageNotSupported      = -30580,
    errDataNotSupported         = -30581,
    errControlDoesntSupportFocus = -30582,
    errWindowDoesntSupportFocus = -30583,
    errUnknownControl           = -30584,
    errCouldntSetFocus          = -30585,
    errNoRootControl            = -30586,
    errRootAlreadyExists        = -30587,
    errInvalidPartCode          = -30588,
    errControlsAlreadyExist     = -30589,
    errControlIsNotEmbedder     = -30590,
    errDataSizeMismatch         = -30591,
    errControlHiddenOrDisabled  = -30592,
    errWindowRegionCodeInvalid  = -30593,
    errCantEmbedIntoSelf        = -30594,
    errCantEmbedRoot            = -30595,
    errItemNotControl           = -30596,
    controlInvalidDataVersionErr = -30597,
    controlPropertyInvalid      = -5603,
    controlPropertyNotFoundErr  = -5604,
    controlHandleInvalidErr     = -30599
};




/* URLAccess Error Codes */
enum {
    kURLInvalidURLReferenceError = -30770,
    kURLProgressAlreadyDisplayedError = -30771,
    kURLDestinationExistsError  = -30772,
    kURLInvalidURLError         = -30773,
    kURLUnsupportedSchemeError  = -30774,
    kURLServerBusyError         = -30775,
    kURLAuthenticationError     = -30776,
    kURLPropertyNotYetKnownError = -30777,
    kURLUnknownPropertyError    = -30778,
    kURLPropertyBufferTooSmallError = -30779,
    kURLUnsettablePropertyError = -30780,
    kURLInvalidCallError        = -30781,
    kURLFileEmptyError          = -30783,
    kURLExtensionFailureError   = -30785,
    kURLInvalidConfigurationError = -30786,
    kURLAccessNotAvailableError = -30787,
    kURL68kNotSupportedError    = -30788
};


enum {
    badComponentInstance        = (long)0x80008001,
    badComponentSelector        = (long)0x80008002
};



enum {
    dsBusError                  = 1,                            /*bus error*/
    dsAddressErr                = 2,                            /*address error*/
    dsIllInstErr                = 3,                            /*illegal instruction error*/
    dsZeroDivErr                = 4,                            /*zero divide error*/
    dsChkErr                    = 5,                            /*check trap error*/
    dsOvflowErr                 = 6,                            /*overflow trap error*/
    dsPrivErr                   = 7,                            /*privilege violation error*/
    dsTraceErr                  = 8,                            /*trace mode error*/
    dsLineAErr                  = 9,                            /*line 1010 trap error*/
    dsLineFErr                  = 10,                           /*line 1111 trap error*/
    dsMiscErr                   = 11,                           /*miscellaneous hardware exception error*/
    dsCoreErr                   = 12,                           /*unimplemented core routine error*/
    dsIrqErr                    = 13,                           /*uninstalled interrupt error*/
    dsIOCoreErr                 = 14,                           /*IO Core Error*/
    dsLoadErr                   = 15,                           /*Segment Loader Error*/
    dsFPErr                     = 16,                           /*Floating point error*/
    dsNoPackErr                 = 17,                           /*package 0 not present*/
    dsNoPk1                     = 18,                           /*package 1 not present*/
    dsNoPk2                     = 19                            /*package 2 not present*/
};

enum {
    dsNoPk3                     = 20,                           /*package 3 not present*/
    dsNoPk4                     = 21,                           /*package 4 not present*/
    dsNoPk5                     = 22,                           /*package 5 not present*/
    dsNoPk6                     = 23,                           /*package 6 not present*/
    dsNoPk7                     = 24,                           /*package 7 not present*/
    dsMemFullErr                = 25,                           /*out of memory!*/
    dsBadLaunch                 = 26,                           /*can't launch file*/
    dsFSErr                     = 27,                           /*file system map has been trashed*/
    dsStknHeap                  = 28,                           /*stack has moved into application heap*/
    negZcbFreeErr               = 33,                           /*ZcbFree has gone negative*/
    dsFinderErr                 = 41,                           /*can't load the Finder error*/
    dsBadSlotInt                = 51,                           /*unserviceable slot interrupt*/
    dsBadSANEOpcode             = 81,                           /*bad opcode given to SANE Pack4*/
    dsBadPatchHeader            = 83,                           /*SetTrapAddress saw the "come-from" header*/
    menuPrgErr                  = 84,                           /*happens when a menu is purged*/
    dsMBarNFnd                  = 85,                           /*Menu Manager Errors*/
    dsHMenuFindErr              = 86,                           /*Menu Manager Errors*/
    dsWDEFNotFound              = 87,                           /*could not load WDEF*/
    dsCDEFNotFound              = 88,                           /*could not load CDEF*/
    dsMDEFNotFound              = 89                            /*could not load MDEF*/
};

enum {
    dsNoFPU                     = 90,                           /*an FPU instruction was executed and the machine doesn't have one*/
    dsNoPatch                   = 98,                           /*Can't patch for particular Model Mac*/
    dsBadPatch                  = 99,                           /*Can't load patch resource*/
    dsParityErr                 = 101,                          /*memory parity error*/
    dsOldSystem                 = 102,                          /*System is too old for this ROM*/
    ds32BitMode                 = 103,                          /*booting in 32-bit on a 24-bit sys*/
    dsNeedToWriteBootBlocks     = 104,                          /*need to write new boot blocks*/
    dsNotEnoughRAMToBoot        = 105,                          /*must have at least 1.5MB of RAM to boot 7.0*/
    dsBufPtrTooLow              = 106,                          /*bufPtr moved too far during boot*/
    dsVMDeferredFuncTableFull   = 112,                          /*VM's DeferUserFn table is full*/
    dsVMBadBackingStore         = 113,                          /*Error occurred while reading or writing the VM backing-store file*/
    dsCantHoldSystemHeap        = 114,                          /*Unable to hold the system heap during boot*/
    dsSystemRequiresPowerPC     = 116,                          /*Startup disk requires PowerPC*/
    dsGibblyMovedToDisabledFolder = 117,                        /* For debug builds only, signals that active gibbly was disabled during boot. */
    dsUnBootableSystem          = 118,                          /* Active system file will not boot on this system because it was designed only to boot from a CD. */
    dsMustUseFCBAccessors       = 119,                          /* FCBSPtr and FSFCBLen are invalid - must use FSM FCB accessor functions */
    dsMacOSROMVersionTooOld     = 120,                          /* The version of the "Mac OS ROM" file is too old to be used with the installed version of system software */
    dsLostConnectionToNetworkDisk = 121,                        /* Lost communication with Netboot server */
    dsWriteToSupervisorStackGuardPage = 128,                    /*the supervisor stack overflowed into its guard page */
    dsReinsert                  = 30,                           /*request user to reinsert off-line volume*/
    shutDownAlert               = 42,                           /*handled like a shutdown error*/
    dsShutDownOrRestart         = 20000,                        /*user choice between ShutDown and Restart*/
    dsSwitchOffOrRestart        = 20001,                        /*user choice between switching off and Restart*/
    dsForcedQuit                = 20002,                        /*allow the user to ExitToShell, return if Cancel*/
    dsRemoveDisk                = 20003,                        /*request user to remove disk from manual eject drive*/
    dsDirtyDisk                 = 20004,                        /*request user to return a manually-ejected dirty disk*/
    dsShutDownOrResume          = 20109,                        /*allow user to return to Finder or ShutDown*/
    dsSCSIWarn                  = 20010,                        /*Portable SCSI adapter warning.*/
    dsMBSysError                = 29200,                        /*Media Bay replace warning.*/
    dsMBFlpySysError            = 29201,                        /*Media Bay, floppy replace warning.*/
    dsMBATASysError             = 29202,                        /*Media Bay, ATA replace warning.*/
    dsMBATAPISysError           = 29203,                        /*Media Bay, ATAPI replace warning...*/
    dsMBExternFlpySysError      = 29204,                        /*Media Bay, external floppy drive reconnect warning*/
    dsPCCardATASysError         = 29205                         /*PCCard has been ejected while still in use. */
};

/*
    System Errors that are used after MacsBug is loaded to put up dialogs since these should not 
    cause MacsBug to stop, they must be in the range (30, 42, 16384-32767) negative numbers add 
    to an existing dialog without putting up a whole new dialog 
*/
enum {
    dsNoExtsMacsBug             = -1,                           /*not a SysErr, just a placeholder */
    dsNoExtsDisassembler        = -2,                           /*not a SysErr, just a placeholder */
    dsMacsBugInstalled          = -10,                          /*say "MacsBug Installed"*/
    dsDisassemblerInstalled     = -11,                          /*say "Disassembler Installed"*/
    dsExtensionsDisabled        = -13,                          /*say "Extensions Disabled"*/
    dsGreeting                  = 40,                           /*welcome to Macintosh greeting*/
    dsSysErr                    = 32767,                        /*general system error*/
                                                                /*old names here for compatibility's sake*/
    WDEFNFnd                    = dsWDEFNotFound
};

enum {
    CDEFNFnd                    = dsCDEFNotFound,
    dsNotThe1                   = 31,                           /*not the disk I wanted*/
    dsBadStartupDisk            = 42,                           /*unable to mount boot volume (sad Mac only)*/
    dsSystemFileErr             = 43,                           /*can't find System file to open (sad Mac only)*/
    dsHD20Installed             = -12,                          /*say "HD20 Startup"*/
    mBarNFnd                    = -126,                         /*system error code for MBDF not found*/
    fsDSIntErr                  = -127,                         /*non-hardware Internal file system error*/
    hMenuFindErr                = -127,                         /*could not find HMenu's parent in MenuKey (wrong error code - obsolete)*/
    userBreak                   = -490,                         /*user debugger break*/
    strUserBreak                = -491,                         /*user debugger break; display string on stack*/
    exUserBreak                 = -492                          /*user debugger break; execute debugger commands on stack*/
};


enum {
                                                                /* DS Errors which are specific to the new runtime model introduced with PowerPC */
    dsBadLibrary                = 1010,                         /* Bad shared library */
    dsMixedModeFailure          = 1011                          /* Internal Mixed Mode Failure */
};


                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SysError(__D0)
                                                                                            #endif
EXTERN_API( void )
SysError                        (short                  errorCode)                          ONEWORDINLINE(0xA9C9);




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

#endif /* __MACERRORS__ */

