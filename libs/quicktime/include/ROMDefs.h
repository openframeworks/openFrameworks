/*
     File:       ROMDefs.h
 
     Contains:   NuBus card ROM Definitions.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1986-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ROMDEFS__
#define __ROMDEFS__



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
    appleFormat                 = 1,                            /*Format of Declaration Data (IEEE will assign real value)*/
    romRevision                 = 1,                            /*Revision of Declaration Data Format*/
    romRevRange                 = 9,                            /*Revision of Declaration Data Format [1..9]*/
    testPattern                 = 1519594439L,                  /*FHeader long word test pattern*/
    sCodeRev                    = 2,                            /*Revision of code (For sExec)*/
    sExec2                      = 2,
    sCPU68000                   = 1,                            /*CPU type = 68000*/
    sCPU68020                   = 2,                            /*CPU type = 68020*/
    sCPU68030                   = 3,                            /*CPU type = 68030*/
    sCPU68040                   = 4,                            /*CPU type = 68040*/
    sMacOS68000                 = 1,                            /*Mac OS, CPU type = 68000*/
    sMacOS68020                 = 2,                            /*Mac OS, CPU type = 68020*/
    sMacOS68030                 = 3,                            /*Mac OS, CPU type = 68030*/
    sMacOS68040                 = 4,                            /*Mac OS, CPU type = 68040*/
    board                       = 0,                            /*Board sResource - Required on all boards*/
    displayVideoAppleTFB        = 16843009L,                    /*Video with Apple parameters for TFB card.*/
    displayVideoAppleGM         = 16843010L,                    /*Video with Apple parameters for GM card.*/
    networkEtherNetApple3Com    = 33620225L,                    /*Ethernet with apple parameters for 3-Comm card.*/
    testSimpleAppleAny          = -2147417856L,                 /*A simple test sResource.*/
    endOfList                   = 255,                          /*End of list*/
    defaultTO                   = 100                           /*100 retries.*/
};


enum {
                                                                /* sResource flags for sRsrc_Flags */
    fOpenAtStart                = 1,                            /* set => open the driver at start time, else do not */
    f32BitMode                  = 2                             /* set => a 32-bit address will be put into dctlDevBase (IM Devices 2-54) */
};

enum {
    sRsrcType                   = 1,                            /*Type of sResource*/
    sRsrcName                   = 2,                            /*Name of sResource*/
    sRsrcIcon                   = 3,                            /*Icon*/
    sRsrcDrvrDir                = 4,                            /*Driver directory*/
    sRsrcLoadDir                = 5,                            /*Load directory*/
    sRsrcBootRec                = 6,                            /*sBoot record*/
    sRsrcFlags                  = 7,                            /*sResource Flags*/
    sRsrcHWDevId                = 8,                            /*Hardware Device Id*/
    minorBaseOS                 = 10,                           /*Offset to base of sResource in minor space.*/
    minorLength                 = 11,                           /*Length of sResource's address space in standard slot space.*/
    majorBaseOS                 = 12,                           /*Offset to base of sResource in Major space.*/
    majorLength                 = 13,                           /*Length of sResource in super slot space.*/
    sRsrcTest                   = 14,                           /*sBlock diagnostic code*/
    sRsrccicn                   = 15,                           /*Color icon*/
    sRsrcicl8                   = 16,                           /*8-bit (indexed) icon*/
    sRsrcicl4                   = 17,                           /*4-bit (indexed) icon*/
    sDRVRDir                    = 16,                           /*sDriver directory*/
    sGammaDir                   = 64,                           /*sGamma directory*/
    sRsrcVidNames               = 65,                           /*Video mode name directory*/
    sRsrcDock                   = 80,                           /*spID for Docking Handlers*/
    sDiagRec                    = 85,                           /*spID for board diagnostics*/
    sVidAuxParams               = 123,                          /*more video info for Display Manager -- timing information*/
    sDebugger                   = 124,                          /*DatLstEntry for debuggers indicating video anamolies*/
    sVidAttributes              = 125,                          /*video attributes data field (optional,word)*/
    fLCDScreen                  = 0,                            /* bit 0 - when set is LCD, else is CRT*/
    fBuiltInDisplay             = 1,                            /*      1 - when set is built-in (in the box) display, else not*/
    fDefaultColor               = 2,                            /*      2 - when set display prefers multi-bit color, else gray*/
    fActiveBlack                = 3,                            /*      3 - when set black on display must be written, else display is naturally black*/
    fDimMinAt1                  = 4,                            /*      4 - when set should dim backlight to level 1 instead of 0*/
    fBuiltInDetach              = 4,                            /*      4 - when set is built-in (in the box), but detaches*/
    sVidParmDir                 = 126,
    sBkltParmDir                = 140,                          /*directory of backlight tables*/
    stdBkltTblSize              = 36,                           /*size of "standard" 0..31-entry backlight table*/
    sSuperDir                   = 254
};

/* =======================================================================  */
/* sResource types                                                          */
/* =======================================================================  */
enum {
    catBoard                    = 0x0001,                       /*Category for board types.*/
    catTest                     = 0x0002,                       /*Category for test types -- not used much.*/
    catDisplay                  = 0x0003,                       /*Category for display (video) cards.*/
    catNetwork                  = 0x0004,                       /*Category for Networking cards.*/
    catScanner                  = 0x0008,                       /*scanners bring in data somehow*/
    catCPU                      = 0x000A,
    catIntBus                   = 0x000C,
    catProto                    = 0x0011,
    catDock                     = 0x0020,                       /*<Type>*/
    typeBoard                   = 0x0000,
    typeApple                   = 0x0001,
    typeVideo                   = 0x0001,
    typeEtherNet                = 0x0001,
    typeStation                 = 0x0001,
    typeDesk                    = 0x0002,
    typeTravel                  = 0x0003,
    typeDSP                     = 0x0004,
    typeXPT                     = 0x000B,
    typeSIM                     = 0x000C,
    typeDebugger                = 0x0100,
    type68000                   = 0x0002,
    type68020                   = 0x0003,
    type68030                   = 0x0004,
    type68040                   = 0x0005,
    type601                     = 0x0025,
    type603                     = 0x002E,
    typeAppleII                 = 0x0015,                       /*Driver Interface : <id.SW>*/
    drSwMacCPU                  = 0,
    drSwAppleIIe                = 0x0001,
    drSwApple                   = 1,                            /*To ask for or define an Apple-compatible SW device.*/
    drSwMacsBug                 = 0x0104,
    drSwDepewEngineering        = 0x0101,                       /*Driver Interface : <id.SW><id.HW>*/
    drHwTFB                     = 1,                            /*HW ID for the TFB (original Mac II) video card.*/
    drHw3Com                    = 1,                            /*HW ID for the Apple EtherTalk card.*/
    drHwBSC                     = 3,
    drHwGemini                  = 1,
    drHwDeskBar                 = 1,
    drHwHooperDock              = 2,                            /*Hooper's CatDock,TypeDesk,DrSwApple ID; registered with DTS.*/
    drHwATT3210                 = 0x0001,
    drHwBootBug                 = 0x0100,
    drHwMicroDock               = 0x0100,                       /* video hardware id's  - <catDisplay><typVideo>*/
    drHwSTB3                    = 0x0002,                       /* Assigned by Kevin Mellander for STB-3 hardware. */
    drHwSTB                     = drHwSTB3,                     /* (Both STB-3 and STB-4 share the same video hardware.) */
    drHwRBV                     = 0x0018,                       /* IIci Aurora25/16 hw ID */
    drHwJMFB                    = 0x0019,                       /* 4o8/8o24 NuBus card */
    drHwElsie                   = 0x001A,
    drHwTim                     = 0x001B,
    drHwDAFB                    = 0x001C,
    drHwDolphin                 = 0x001D,                       /* 8o24GC NuBus card */
    drHwGSC                     = 0x001E,                       /* (Renamed from GSC drHWDBLite) */
    drHwDAFBPDS                 = 0x001F,
    drHWVSC                     = 0x0020,
    drHwApollo                  = 0x0021,
    drHwSonora                  = 0x0022,
    drHwReserved2               = 0x0023,
    drHwColumbia                = 0x0024,
    drHwCivic                   = 0x0025,
    drHwBrazil                  = 0x0026,
    drHWPBLCD                   = 0x0027,
    drHWCSC                     = 0x0028,
    drHwJET                     = 0x0029,
    drHWMEMCjr                  = 0x002A,
    drHwBoogie                  = 0x002B,                       /* 8o24AC nuBus video card (built by Radius) */
    drHwHPV                     = 0x002C,                       /* High performance Video (HPV) PDS card for original PowerMacs */
    drHwPlanaria                = 0x002D,                       /*PowerMac 6100/7100/8100 PDS AV video*/
    drHwValkyrie                = 0x002E,
    drHwKeystone                = 0x002F,
    drHWATI                     = 0x0055,
    drHwGammaFormula            = 0x0056,                       /* Use for gType of display mgr gamma tables */
                                                                /* other drHW id's for built-in functions*/
    drHwSonic                   = 0x0110,
    drHwMace                    = 0x0114,
    drHwDblExp                  = 0x0001,                       /* CPU board IDs - <catBoard> <typBoard> <0000> <0000>*/
    MIIBoardId                  = 0x0010,                       /*Mac II Board ID*/
    ciVidBoardID                = 0x001F,                       /*Aurora25 board ID*/
    CX16VidBoardID              = 0x0020,                       /*Aurora16 board ID*/
    MIIxBoardId                 = 0x0021,                       /*Mac IIx Board ID*/
    SE30BoardID                 = 0x0022,                       /*Mac SE/30 Board ID*/
    MIIcxBoardId                = 0x0023,                       /*Mac IIcx Board ID*/
    MIIfxBoardId                = 0x0024,                       /*F19 board ID*/
    EricksonBoardID             = 0x0028,
    ElsieBoardID                = 0x0029,
    TIMBoardID                  = 0x002A,
    EclipseBoardID              = 0x002B,
    SpikeBoardID                = 0x0033,
    DBLiteBoardID               = 0x0035,
    ZydecoBrdID                 = 0x0036,
    ApolloBoardID               = 0x0038,
    PDMBrdID                    = 0x0039,
    VailBoardID                 = 0x003A,
    WombatBrdID                 = 0x003B,
    ColumbiaBrdID               = 0x003C,
    CycloneBrdID                = 0x003D,
    CompanionBrdID              = 0x003E,
    DartanianBoardID            = 0x0040,
    DartExtVidBoardID           = 0x0046,
    HookBoardID                 = 0x0047,                       /*Hook internal video board ID*/
    EscherBoardID               = 0x004A,                       /*Board ID for Escher (CSC)*/
    POBoardID                   = 0x004D,                       /*Board ID for Primus/Optimus/Aladdin*/
    TempestBrdID                = 0x0050,                       /*Non-official Board ID for Tempest*/
    BlackBirdBdID               = 0x0058,                       /*Board ID for BlackBird*/
    BBExtVidBdID                = 0x0059,                       /*Board ID for BlackBird built-in external video*/
    YeagerBoardID               = 0x005A,                       /*Board ID for Yeager*/
    BBEtherNetBdID              = 0x005E,                       /*Board ID for BlackBird Ethernet board*/
    TELLBoardID                 = 0x0065,                       /*Board ID for TELL (Valkyrie)*/
    MalcolmBoardID              = 0x065E,                       /*Board ID for Malcolm*/
    AJBoardID                   = 0x065F,                       /*Board ID for AJ*/
    M2BoardID                   = 0x0660,                       /*Board ID for M2*/
    OmegaBoardID                = 0x0661,                       /*Board ID for Omega*/
    TNTBoardID                  = 0x0670,                       /*Board ID for TNT/Alchemy/Hipclipper CPUs (did Nano just make this up?)*/
    HooperBoardID               = 0x06CD,                       /*Board ID for Hooper*/
                                                                /* other board IDs*/
    BoardIDDblExp               = 0x002F,
    DAFBPDSBoardID              = 0x0037,
    MonetBoardID                = 0x0048,
    SacSONIC16BoardID           = 0x004E,
    SacSONIC32BoardID           = 0x004F,                       /* CPU board types - <CatCPU> <Typ680x0> <DrSwMacCPU>*/
    drHWMacII                   = 0x0001,                       /*Mac II hw ID*/
    drHwMacIIx                  = 0x0002,                       /*Mac IIx hw ID*/
    drHWSE30                    = 0x0003,                       /*Mac SE/30 hw ID*/
    drHwMacIIcx                 = 0x0004,                       /*Mac IIcx hw ID*/
    drHWMacIIfx                 = 0x0005,                       /*Mac IIfx hw ID*/
    drHWF19                     = 0x0005,                       /*F19 hw ID*/
    sBlockTransferInfo          = 20,                           /*general slot block xfer info*/
    sMaxLockedTransferCount     = 21,                           /*slot max. locked xfer count*/
    boardId                     = 32,                           /*Board Id*/
    pRAMInitData                = 33,                           /*sPRAM init data*/
    primaryInit                 = 34,                           /*Primary init record*/
    timeOutConst                = 35,                           /*Time out constant*/
    vendorInfo                  = 36,                           /*Vendor information List. See Vendor List, below*/
    boardFlags                  = 37,                           /*Board Flags*/
    secondaryInit               = 38,                           /*Secondary init record/code*/
                                                                /* The following Id's are associated with all CPU sResources.*/
    MajRAMSp                    = 129,                          /*ID of Major RAM space.*/
    MinROMSp                    = 130,                          /*ID of Minor ROM space.*/
    vendorId                    = 1,                            /*Vendor Id*/
    serialNum                   = 2,                            /*Serial number*/
    revLevel                    = 3,                            /*Revision level*/
    partNum                     = 4,                            /*Part number*/
    date                        = 5                             /*Last revision date of the card*/
};

enum {
    testByte                    = 32,                           /*Test byte.*/
    testWord                    = 33,                           /*0021*/
    testLong                    = 34,                           /*Test Long.*/
    testString                  = 35                            /*Test String.*/
};

/* sResource List. Category: Display        Type: Video */
/* The following Id's are common to all Mode sResources in Display_Video */
/* functional sResources. */
enum {
    mBlockTransferInfo          = 5,                            /* slot block xfer info PER MODE */
    mMaxLockedTransferCount     = 6                             /* slot max. locked xfer count PER MODE */
};





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

#endif /* __ROMDEFS__ */

