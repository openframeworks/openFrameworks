/*
     File:       Printing.h
 
     Contains:   Print Manager Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PRINTING__
#define __PRINTING__

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
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
    kFirstPageMin               = 1,                            /* min value for first page */
    kLastPageMax                = 32767                         /* max value for last page */
};

enum {
    iPFMaxPgs                   = 128,
    iPrPgFract                  = 120,                          /*Page scale factor. ptPgSize (below) is in units of 1/iPrPgFract*/
    iPrPgFst                    = 1,                            /*Page range constants*/
    iPrPgMax                    = 9999,
    iPrRelease                  = 3,                            /*Current version number of the code.*/
    iPrSavPFil                  = -1,
    iPrAbort                    = 0x0080,
    iPrDevCtl                   = 7,                            /*The PrDevCtl Proc's ctl number*/
    lPrReset                    = 0x00010000,                   /*The PrDevCtl Proc's CParam for reset*/
    lPrLineFeed                 = 0x00030000,
    lPrLFStd                    = 0x0003FFFF,                   /*The PrDevCtl Proc's CParam for std paper advance*/
    lPrLFSixth                  = 0x0003FFFF,
    lPrPageEnd                  = 0x00020000,                   /*The PrDevCtl Proc's CParam for end page*/
    lPrDocOpen                  = 0x00010000,
    lPrPageOpen                 = 0x00040000,
    lPrPageClose                = 0x00020000,
    lPrDocClose                 = 0x00050000,
    iFMgrCtl                    = 8,                            /*The FMgr's Tail-hook Proc's ctl number*/
    iMscCtl                     = 9,                            /*The FMgr's Tail-hook Proc's ctl number*/
    iPvtCtl                     = 10                            /*The FMgr's Tail-hook Proc's ctl number*/
};

#define sPrDrvr ".Print"
/*  Error Codes moved to Errors.(hap) */
enum {
    pPrGlobals                  = 0x00000944,                   /*The PrVars lo mem area:*/
    bDraftLoop                  = 0,
    bSpoolLoop                  = 1,
    bUser1Loop                  = 2,
    bUser2Loop                  = 3,
    fNewRunBit                  = 2,
    fHiResOK                    = 3,
    fWeOpenedRF                 = 4,                            /*Driver constants */
    iPrBitsCtl                  = 4,
    lScreenBits                 = 0,
    lPaintBits                  = 1,
    lHiScreenBits               = 0x00000002,                   /*The Bitmap Print Proc's Screen Bitmap param*/
    lHiPaintBits                = 0x00000003,                   /*The Bitmap Print Proc's Paint [sq pix] param*/
    iPrIOCtl                    = 5,
    iPrEvtCtl                   = 6,                            /*The PrEvent Proc's ctl number*/
    lPrEvtAll                   = 0x0002FFFD,                   /*The PrEvent Proc's CParam for the entire screen*/
    lPrEvtTop                   = 0x0001FFFD,                   /*The PrEvent Proc's CParam for the top folder*/
    iPrDrvrRef                  = -3
};

enum {
    getRslDataOp                = 4,
    setRslOp                    = 5,
    draftBitsOp                 = 6,
    noDraftBitsOp               = 7,
    getRotnOp                   = 8,
    NoSuchRsl                   = 1,
    OpNotImpl                   = 2,                            /*the driver doesn't support this opcode*/
    RgType1                     = 1
};

enum {
    feedCut                     = 0,
    feedFanfold                 = 1,
    feedMechCut                 = 2,
    feedOther                   = 3
};

typedef SInt8                           TFeed;
enum {
    scanTB                      = 0,
    scanBT                      = 1,
    scanLR                      = 2,
    scanRL                      = 3
};

typedef SInt8                           TScan;
/* A Rect Ptr */
typedef Rect *                          TPRect;
typedef CALLBACK_API( void , PrIdleProcPtr )(void );
typedef CALLBACK_API( void , PItemProcPtr )(DialogPtr theDialog, short item);
typedef STACK_UPP_TYPE(PrIdleProcPtr)                           PrIdleUPP;
typedef STACK_UPP_TYPE(PItemProcPtr)                            PItemUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(PrIdleUPP)
    NewPrIdleUPP                   (PrIdleProcPtr           userRoutine);

    EXTERN_API(PItemUPP)
    NewPItemUPP                    (PItemProcPtr            userRoutine);

    EXTERN_API(void)
    DisposePrIdleUPP               (PrIdleUPP               userUPP);

    EXTERN_API(void)
    DisposePItemUPP                (PItemUPP                userUPP);

    EXTERN_API(void)
    InvokePrIdleUPP                (PrIdleUPP               userUPP);

    EXTERN_API(void)
    InvokePItemUPP                 (DialogPtr               theDialog,
                                    short                   item,
                                    PItemUPP                userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppPrIdleProcInfo = 0x00000000 };                        /* pascal no_return_value Func() */
    enum { uppPItemProcInfo = 0x000002C0 };                         /* pascal no_return_value Func(4_bytes, 2_bytes) */
    #define NewPrIdleUPP(userRoutine)                               (PrIdleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPrIdleProcInfo, GetCurrentArchitecture())
    #define NewPItemUPP(userRoutine)                                (PItemUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPItemProcInfo, GetCurrentArchitecture())
    #define DisposePrIdleUPP(userUPP)                               DisposeRoutineDescriptor(userUPP)
    #define DisposePItemUPP(userUPP)                                DisposeRoutineDescriptor(userUPP)
    #define InvokePrIdleUPP(userUPP)                                CALL_ZERO_PARAMETER_UPP((userUPP), uppPrIdleProcInfo)
    #define InvokePItemUPP(theDialog, item, userUPP)                CALL_TWO_PARAMETER_UPP((userUPP), uppPItemProcInfo, (theDialog), (item))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewPrIdleProc(userRoutine)                              NewPrIdleUPP(userRoutine)
#define NewPItemProc(userRoutine)                               NewPItemUPP(userRoutine)
#define CallPrIdleProc(userRoutine)                             InvokePrIdleUPP(userRoutine)
#define CallPItemProc(userRoutine, theDialog, item)             InvokePItemUPP(theDialog, item, userRoutine)
#if !OPAQUE_TOOLBOX_STRUCTS

struct TPrPort {
    GrafPort                        gPort;                      /*The Printer's graf port.*/
    QDProcs                         gProcs;                     /*..and its procs*/
    long                            lGParam1;                   /*16 bytes for private parameter storage.*/
    long                            lGParam2;
    long                            lGParam3;
    long                            lGParam4;
    Boolean                         fOurPtr;                    /*Whether the PrPort allocation was done by us.*/
    Boolean                         fOurBits;                   /*Whether the BitMap allocation was done by us.*/
};
typedef struct TPrPort                  TPrPort;

typedef TPrPort *                       TPPrPort;
#else
typedef struct OpaqueTPPrPort*          TPPrPort;
#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

typedef TPPrPort                        TPPrPortRef;
/* Printing Graf Port. All printer imaging, whether spooling, banding, etc, happens "thru" a GrafPort.
  This is the "PrPeek" record. */

struct TPrInfo {
    short                           iDev;                       /*Font mgr/QuickDraw device code*/
    short                           iVRes;                      /*Resolution of device, in device coordinates*/
    short                           iHRes;                      /*..note: V before H => compatable with Point.*/
    Rect                            rPage;                      /*The page (printable) rectangle in device coordinates.*/
};
typedef struct TPrInfo                  TPrInfo;

typedef TPrInfo *                       TPPrInfo;
/* Print Info Record: The parameters needed for page composition. */

struct TPrStl {
    short                           wDev;
    short                           iPageV;
    short                           iPageH;
    SInt8                           bPort;
    TFeed                           feed;
};
typedef struct TPrStl                   TPrStl;

typedef TPrStl *                        TPPrStl;

struct TPrXInfo {
    short                           iRowBytes;
    short                           iBandV;
    short                           iBandH;
    short                           iDevBytes;
    short                           iBands;
    SInt8                           bPatScale;
    SInt8                           bUlThick;
    SInt8                           bUlOffset;
    SInt8                           bUlShadow;
    TScan                           scan;
    SInt8                           bXInfoX;
};
typedef struct TPrXInfo                 TPrXInfo;

typedef TPrXInfo *                      TPPrXInfo;

struct TPrJob {
    short                           iFstPage;                   /*Page Range.*/
    short                           iLstPage;
    short                           iCopies;                    /*No. copies.*/
    SInt8                           bJDocLoop;                  /*The Doc style: Draft, Spool, .., and ..*/
    Boolean                         fFromUsr;                   /*Printing from an User's App (not PrApp) flag*/
    PrIdleUPP                       pIdleProc;                  /*The Proc called while waiting on IO etc.*/
    StringPtr                       pFileName;                  /*Spool File Name: NIL for default.*/
    short                           iFileVol;                   /*Spool File vol, set to 0 initially*/
    SInt8                           bFileVers;                  /*Spool File version, set to 0 initially*/
    SInt8                           bJobX;                      /*An eXtra byte.*/
};
typedef struct TPrJob                   TPrJob;

typedef TPrJob *                        TPPrJob;
/* Print Job: Print "form" for a single print request. */

struct TPrint {
    short                           iPrVersion;                 /*(2) Printing software version*/
    TPrInfo                         prInfo;                     /*(14) the PrInfo data associated with the current style.*/
    Rect                            rPaper;                     /*(8) The paper rectangle [offset from rPage]*/
    TPrStl                          prStl;                      /*(8)  This print request's style.*/
    TPrInfo                         prInfoPT;                   /*(14)  Print Time Imaging metrics*/
    TPrXInfo                        prXInfo;                    /*(16)  Print-time (expanded) Print info record.*/
    TPrJob                          prJob;                      /*(20) The Print Job request (82)  Total of the above; 120-82 = 38 bytes needed to fill 120*/
    short                           printX[19];                 /*Spare to fill to 120 bytes!*/
};
typedef struct TPrint                   TPrint;

typedef TPrint *                        TPPrint;
typedef TPPrint *                       THPrint;

struct TPrStatus {
    short                           iTotPages;                  /*Total pages in Print File.*/
    short                           iCurPage;                   /*Current page number*/
    short                           iTotCopies;                 /*Total copies requested*/
    short                           iCurCopy;                   /*Current copy number*/
    short                           iTotBands;                  /*Total bands per page.*/
    short                           iCurBand;                   /*Current band number*/
    Boolean                         fPgDirty;                   /*True if current page has been written to.*/
    Boolean                         fImaging;                   /*Set while in band's DrawPic call.*/
    THPrint                         hPrint;                     /*Handle to the active Printer record*/
    TPPrPort                        pPrPort;                    /*Ptr to the active PrPort*/
    PicHandle                       hPic;                       /*Handle to the active Picture*/
};
typedef struct TPrStatus                TPrStatus;

typedef TPrStatus *                     TPPrStatus;
typedef TPPrStatus                      TPPrStatusRef;

/* Print Status: Print information during printing. */

struct TPfPgDir {
    short                           iPages;
    long                            iPgPos[129];                /*ARRAY [0..iPfMaxPgs] OF LONGINT*/
};
typedef struct TPfPgDir                 TPfPgDir;

typedef TPfPgDir *                      TPPfPgDir;
typedef TPPfPgDir *                     THPfPgDir;
/* PicFile = a TPfHeader followed by n QuickDraw Pics (whose PicSize is invalid!) */
/* This is the Printing Dialog Record. Only used by folks appending their own
   DITLs to the print dialogs.  Print Dialog: The Dialog Stream object. */
#if !OPAQUE_TOOLBOX_STRUCTS

struct TPrDlg {
    DialogRecord                    Dlg;                        /*The Dialog window*/
    ModalFilterUPP                  pFltrProc;                  /*The Filter Proc.*/
    PItemUPP                        pItemProc;                  /*The Item evaluating proc.*/
    THPrint                         hPrintUsr;                  /*The user's print record.*/
    Boolean                         fDoIt;
    Boolean                         fDone;
    long                            lUser1;                     /*Four longs for apps to hang global data.*/
    long                            lUser2;                     /*Plus more stuff needed by the particular*/
    long                            lUser3;                     /*printing dialog.*/
    long                            lUser4;
};
typedef struct TPrDlg                   TPrDlg;

typedef TPrDlg *                        TPPrDlg;
#else
typedef struct OpaqueTPPrDlg*           TPPrDlg;
#endif  /* !OPAQUE_TOOLBOX_STRUCTS */

typedef TPPrDlg                         TPPrDlgRef;
typedef CALLBACK_API( TPPrDlgRef , PDlgInitProcPtr )(THPrint hPrint);
typedef STACK_UPP_TYPE(PDlgInitProcPtr)                         PDlgInitUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(PDlgInitUPP)
    NewPDlgInitUPP                 (PDlgInitProcPtr         userRoutine);

    EXTERN_API(void)
    DisposePDlgInitUPP             (PDlgInitUPP             userUPP);

    EXTERN_API(TPPrDlgRef)
    InvokePDlgInitUPP              (THPrint                 hPrint,
                                    PDlgInitUPP             userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppPDlgInitProcInfo = 0x000000F0 };                      /* pascal 4_bytes Func(4_bytes) */
    #define NewPDlgInitUPP(userRoutine)                             (PDlgInitUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPDlgInitProcInfo, GetCurrentArchitecture())
    #define DisposePDlgInitUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define InvokePDlgInitUPP(hPrint, userUPP)                      (TPPrDlgRef)CALL_ONE_PARAMETER_UPP((userUPP), uppPDlgInitProcInfo, (hPrint))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewPDlgInitProc(userRoutine)                            NewPDlgInitUPP(userRoutine)
#define CallPDlgInitProc(userRoutine, hPrint)                   InvokePDlgInitUPP(hPrint, userRoutine)

struct TGnlData {
    short                           iOpCode;
    short                           iError;
    long                            lReserved;                  /*more fields here depending on call*/
};
typedef struct TGnlData                 TGnlData;

struct TRslRg {
    short                           iMin;
    short                           iMax;
};
typedef struct TRslRg                   TRslRg;

struct TRslRec {
    short                           iXRsl;
    short                           iYRsl;
};
typedef struct TRslRec                  TRslRec;

struct TGetRslBlk {
    short                           iOpCode;
    short                           iError;
    long                            lReserved;
    short                           iRgType;
    TRslRg                          xRslRg;
    TRslRg                          yRslRg;
    short                           iRslRecCnt;
    TRslRec                         rgRslRec[27];
};
typedef struct TGetRslBlk               TGetRslBlk;

struct TSetRslBlk {
    short                           iOpCode;
    short                           iError;
    long                            lReserved;
    THPrint                         hPrint;
    short                           iXRsl;
    short                           iYRsl;
};
typedef struct TSetRslBlk               TSetRslBlk;

struct TDftBitsBlk {
    short                           iOpCode;
    short                           iError;
    long                            lReserved;
    THPrint                         hPrint;
};
typedef struct TDftBitsBlk              TDftBitsBlk;

struct TGetRotnBlk {
    short                           iOpCode;
    short                           iError;
    long                            lReserved;
    THPrint                         hPrint;
    Boolean                         fLandscape;
    SInt8                           bXtra;
};
typedef struct TGetRotnBlk              TGetRotnBlk;
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
PrPurge                         (void)                                                      FOURWORDINLINE(0x2F3C, 0xA800, 0x0000, 0xA8FD);

EXTERN_API( void )
PrNoPurge                       (void)                                                      FOURWORDINLINE(0x2F3C, 0xB000, 0x0000, 0xA8FD);

EXTERN_API( void )
PrOpen                          (void)                                                      FOURWORDINLINE(0x2F3C, 0xC800, 0x0000, 0xA8FD);

EXTERN_API( void )
PrClose                         (void)                                                      FOURWORDINLINE(0x2F3C, 0xD000, 0x0000, 0xA8FD);

EXTERN_API( void )
PrintDefault                    (THPrint                hPrint)                             FOURWORDINLINE(0x2F3C, 0x2004, 0x0480, 0xA8FD);

EXTERN_API( Boolean )
PrValidate                      (THPrint                hPrint)                             FOURWORDINLINE(0x2F3C, 0x5204, 0x0498, 0xA8FD);

EXTERN_API( Boolean )
PrStlDialog                     (THPrint                hPrint)                             FOURWORDINLINE(0x2F3C, 0x2A04, 0x0484, 0xA8FD);

EXTERN_API( Boolean )
PrJobDialog                     (THPrint                hPrint)                             FOURWORDINLINE(0x2F3C, 0x3204, 0x0488, 0xA8FD);

EXTERN_API( TPPrDlgRef )
PrStlInit                       (THPrint                hPrint)                             FOURWORDINLINE(0x2F3C, 0x3C04, 0x040C, 0xA8FD);

EXTERN_API( TPPrDlgRef )
PrJobInit                       (THPrint                hPrint)                             FOURWORDINLINE(0x2F3C, 0x4404, 0x0410, 0xA8FD);

EXTERN_API( void )
PrJobMerge                      (THPrint                hPrintSrc,
                                 THPrint                hPrintDst)                          FOURWORDINLINE(0x2F3C, 0x5804, 0x089C, 0xA8FD);

EXTERN_API( Boolean )
PrDlgMain                       (THPrint                hPrint,
                                 PDlgInitUPP            pDlgInit)                           FOURWORDINLINE(0x2F3C, 0x4A04, 0x0894, 0xA8FD);

EXTERN_API( TPPrPort )
PrOpenDoc                       (THPrint                hPrint,
                                 TPPrPort               pPrPort, /* can be NULL */
                                 Ptr                    pIOBuf)                             FOURWORDINLINE(0x2F3C, 0x0400, 0x0C00, 0xA8FD);

EXTERN_API( void )
PrCloseDoc                      (TPPrPort               pPrPort)                            FOURWORDINLINE(0x2F3C, 0x0800, 0x0484, 0xA8FD);

EXTERN_API( void )
PrOpenPage                      (TPPrPort               pPrPort,
                                 TPRect                 pPageFrame)                         FOURWORDINLINE(0x2F3C, 0x1000, 0x0808, 0xA8FD);

EXTERN_API( void )
PrClosePage                     (TPPrPort               pPrPort)                            FOURWORDINLINE(0x2F3C, 0x1800, 0x040C, 0xA8FD);

EXTERN_API( void )
PrPicFile                       (THPrint                hPrint,
                                 TPPrPort               pPrPort,
                                 Ptr                    pIOBuf,
                                 Ptr                    pDevBuf,
                                 TPPrStatus             prStatus)                           FOURWORDINLINE(0x2F3C, 0x6005, 0x1480, 0xA8FD);

EXTERN_API( short )
PrError                         (void)                                                      FOURWORDINLINE(0x2F3C, 0xBA00, 0x0000, 0xA8FD);

EXTERN_API( void )
PrSetError                      (short                  iErr)                               FOURWORDINLINE(0x2F3C, 0xC000, 0x0200, 0xA8FD);

EXTERN_API( void )
PrGeneral                       (Ptr                    pData)                              FOURWORDINLINE(0x2F3C, 0x7007, 0x0480, 0xA8FD);

EXTERN_API( void )
PrDrvrOpen                      (void)                                                      FOURWORDINLINE(0x2F3C, 0x8000, 0x0000, 0xA8FD);

EXTERN_API( void )
PrDrvrClose                     (void)                                                      FOURWORDINLINE(0x2F3C, 0x8800, 0x0000, 0xA8FD);

EXTERN_API( void )
PrCtlCall                       (short                  iWhichCtl,
                                 long                   lParam1,
                                 long                   lParam2,
                                 long                   lParam3)                            FOURWORDINLINE(0x2F3C, 0xA000, 0x0E00, 0xA8FD);

EXTERN_API( Handle )
PrDrvrDCE                       (void)                                                      FOURWORDINLINE(0x2F3C, 0x9400, 0x0000, 0xA8FD);

EXTERN_API( short )
PrDrvrVers                      (void)                                                      FOURWORDINLINE(0x2F3C, 0x9A00, 0x0000, 0xA8FD);

EXTERN_API( short )
PrLoadDriver                    (void)                                                      FOURWORDINLINE(0x2F3C, 0xD800, 0x0000, 0xA8FD);


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

#endif /* __PRINTING__ */

