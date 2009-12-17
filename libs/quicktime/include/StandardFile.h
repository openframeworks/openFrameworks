/*
     File:       StandardFile.h
 
     Contains:   Standard File package Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __STANDARDFILE__
#define __STANDARDFILE__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
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
                                                                /* resource IDs of pre-7.0 get and put dialogs */
    putDlgID                    = -3999,
    getDlgID                    = -4000
};

enum {
                                                                /* item offsets of pre-7.0 get and put dialogs */
    putSave                     = 1,
    putCancel                   = 2,
    putEject                    = 5,
    putDrive                    = 6,
    putName                     = 7,
    getOpen                     = 1,
    getCancel                   = 3,
    getEject                    = 5,
    getDrive                    = 6,
    getNmList                   = 7,
    getScroll                   = 8
};

enum {
                                                                /* resource IDs of 7.0 get and put dialogs */
    sfPutDialogID               = -6043,
    sfGetDialogID               = -6042
};

enum {
                                                                /* item offsets of 7.0 get and put dialogs */
    sfItemOpenButton            = 1,
    sfItemCancelButton          = 2,
    sfItemBalloonHelp           = 3,
    sfItemVolumeUser            = 4,
    sfItemEjectButton           = 5,
    sfItemDesktopButton         = 6,
    sfItemFileListUser          = 7,
    sfItemPopUpMenuUser         = 8,
    sfItemDividerLinePict       = 9,
    sfItemFileNameTextEdit      = 10,
    sfItemPromptStaticText      = 11,
    sfItemNewFolderUser         = 12
};

enum {
                                                                /* pseudo-item hits for use in DlgHook */
    sfHookFirstCall             = -1,
    sfHookCharOffset            = 0x1000,
    sfHookNullEvent             = 100,
    sfHookRebuildList           = 101,
    sfHookFolderPopUp           = 102,
    sfHookOpenFolder            = 103,                          /* the following are only in system 7.0+ */
    sfHookLastCall              = -2,
    sfHookOpenAlias             = 104,
    sfHookGoToDesktop           = 105,
    sfHookGoToAliasTarget       = 106,
    sfHookGoToParent            = 107,
    sfHookGoToNextDrive         = 108,
    sfHookGoToPrevDrive         = 109,
    sfHookChangeSelection       = 110,
    sfHookSetActiveOffset       = 200
};


/* the refcon field of the dialog record during a
 modalfilter or dialoghook contains one of the following */
enum {
    sfMainDialogRefCon          = FOUR_CHAR_CODE('stdf'),
    sfNewFolderDialogRefCon     = FOUR_CHAR_CODE('nfdr'),
    sfReplaceDialogRefCon       = FOUR_CHAR_CODE('rplc'),
    sfStatWarnDialogRefCon      = FOUR_CHAR_CODE('stat'),
    sfLockWarnDialogRefCon      = FOUR_CHAR_CODE('lock'),
    sfErrorDialogRefCon         = FOUR_CHAR_CODE('err ')
};



struct SFReply {
    Boolean                         good;
    Boolean                         copy;
    OSType                          fType;
    short                           vRefNum;
    short                           version;
    StrFileName                     fName;                      /* a Str63 on MacOS */
};
typedef struct SFReply                  SFReply;

struct StandardFileReply {
    Boolean                         sfGood;
    Boolean                         sfReplacing;
    OSType                          sfType;
    FSSpec                          sfFile;
    ScriptCode                      sfScript;
    short                           sfFlags;
    Boolean                         sfIsFolder;
    Boolean                         sfIsVolume;
    long                            sfReserved1;
    short                           sfReserved2;
};
typedef struct StandardFileReply        StandardFileReply;
/* for CustomXXXFile, ActivationOrderListPtr parameter is a pointer to an array of item numbers */

typedef const short *                   ActivationOrderListPtr;
typedef CALLBACK_API( short , DlgHookProcPtr )(short item, DialogPtr theDialog);
typedef CALLBACK_API( Boolean , FileFilterProcPtr )(CInfoPBPtr pb);
/* the following also include an extra parameter of "your data pointer" */
typedef CALLBACK_API( short , DlgHookYDProcPtr )(short item, DialogPtr theDialog, void *yourDataPtr);
typedef CALLBACK_API( Boolean , ModalFilterYDProcPtr )(DialogPtr theDialog, EventRecord *theEvent, short *itemHit, void *yourDataPtr);
typedef CALLBACK_API( Boolean , FileFilterYDProcPtr )(CInfoPBPtr pb, void *yourDataPtr);
typedef CALLBACK_API( void , ActivateYDProcPtr )(DialogPtr theDialog, short itemNo, Boolean activating, void *yourDataPtr);
typedef STACK_UPP_TYPE(DlgHookProcPtr)                          DlgHookUPP;
typedef STACK_UPP_TYPE(FileFilterProcPtr)                       FileFilterUPP;
typedef STACK_UPP_TYPE(DlgHookYDProcPtr)                        DlgHookYDUPP;
typedef STACK_UPP_TYPE(ModalFilterYDProcPtr)                    ModalFilterYDUPP;
typedef STACK_UPP_TYPE(FileFilterYDProcPtr)                     FileFilterYDUPP;
typedef STACK_UPP_TYPE(ActivateYDProcPtr)                       ActivateYDUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(DlgHookUPP)
    NewDlgHookUPP                  (DlgHookProcPtr          userRoutine);

    EXTERN_API(FileFilterUPP)
    NewFileFilterUPP               (FileFilterProcPtr       userRoutine);

    EXTERN_API(DlgHookYDUPP)
    NewDlgHookYDUPP                (DlgHookYDProcPtr        userRoutine);

    EXTERN_API(ModalFilterYDUPP)
    NewModalFilterYDUPP            (ModalFilterYDProcPtr    userRoutine);

    EXTERN_API(FileFilterYDUPP)
    NewFileFilterYDUPP             (FileFilterYDProcPtr     userRoutine);

    EXTERN_API(ActivateYDUPP)
    NewActivateYDUPP               (ActivateYDProcPtr       userRoutine);

    EXTERN_API(void)
    DisposeDlgHookUPP              (DlgHookUPP              userUPP);

    EXTERN_API(void)
    DisposeFileFilterUPP           (FileFilterUPP           userUPP);

    EXTERN_API(void)
    DisposeDlgHookYDUPP            (DlgHookYDUPP            userUPP);

    EXTERN_API(void)
    DisposeModalFilterYDUPP        (ModalFilterYDUPP        userUPP);

    EXTERN_API(void)
    DisposeFileFilterYDUPP         (FileFilterYDUPP         userUPP);

    EXTERN_API(void)
    DisposeActivateYDUPP           (ActivateYDUPP           userUPP);

    EXTERN_API(short)
    InvokeDlgHookUPP               (short                   item,
                                    DialogPtr               theDialog,
                                    DlgHookUPP              userUPP);

    EXTERN_API(Boolean)
    InvokeFileFilterUPP            (CInfoPBPtr              pb,
                                    FileFilterUPP           userUPP);

    EXTERN_API(short)
    InvokeDlgHookYDUPP             (short                   item,
                                    DialogPtr               theDialog,
                                    void *                  yourDataPtr,
                                    DlgHookYDUPP            userUPP);

    EXTERN_API(Boolean)
    InvokeModalFilterYDUPP         (DialogPtr               theDialog,
                                    EventRecord *           theEvent,
                                    short *                 itemHit,
                                    void *                  yourDataPtr,
                                    ModalFilterYDUPP        userUPP);

    EXTERN_API(Boolean)
    InvokeFileFilterYDUPP          (CInfoPBPtr              pb,
                                    void *                  yourDataPtr,
                                    FileFilterYDUPP         userUPP);

    EXTERN_API(void)
    InvokeActivateYDUPP            (DialogPtr               theDialog,
                                    short                   itemNo,
                                    Boolean                 activating,
                                    void *                  yourDataPtr,
                                    ActivateYDUPP           userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppDlgHookProcInfo = 0x000003A0 };                       /* pascal 2_bytes Func(2_bytes, 4_bytes) */
    enum { uppFileFilterProcInfo = 0x000000D0 };                    /* pascal 1_byte Func(4_bytes) */
    enum { uppDlgHookYDProcInfo = 0x00000FA0 };                     /* pascal 2_bytes Func(2_bytes, 4_bytes, 4_bytes) */
    enum { uppModalFilterYDProcInfo = 0x00003FD0 };                 /* pascal 1_byte Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppFileFilterYDProcInfo = 0x000003D0 };                  /* pascal 1_byte Func(4_bytes, 4_bytes) */
    enum { uppActivateYDProcInfo = 0x000036C0 };                    /* pascal no_return_value Func(4_bytes, 2_bytes, 1_byte, 4_bytes) */
    #define NewDlgHookUPP(userRoutine)                              (DlgHookUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDlgHookProcInfo, GetCurrentArchitecture())
    #define NewFileFilterUPP(userRoutine)                           (FileFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppFileFilterProcInfo, GetCurrentArchitecture())
    #define NewDlgHookYDUPP(userRoutine)                            (DlgHookYDUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDlgHookYDProcInfo, GetCurrentArchitecture())
    #define NewModalFilterYDUPP(userRoutine)                        (ModalFilterYDUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppModalFilterYDProcInfo, GetCurrentArchitecture())
    #define NewFileFilterYDUPP(userRoutine)                         (FileFilterYDUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppFileFilterYDProcInfo, GetCurrentArchitecture())
    #define NewActivateYDUPP(userRoutine)                           (ActivateYDUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppActivateYDProcInfo, GetCurrentArchitecture())
    #define DisposeDlgHookUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define DisposeFileFilterUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeDlgHookYDUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define DisposeModalFilterYDUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeFileFilterYDUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeActivateYDUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define InvokeDlgHookUPP(item, theDialog, userUPP)              (short)CALL_TWO_PARAMETER_UPP((userUPP), uppDlgHookProcInfo, (item), (theDialog))
    #define InvokeFileFilterUPP(pb, userUPP)                        (Boolean)CALL_ONE_PARAMETER_UPP((userUPP), uppFileFilterProcInfo, (pb))
    #define InvokeDlgHookYDUPP(item, theDialog, yourDataPtr, userUPP)  (short)CALL_THREE_PARAMETER_UPP((userUPP), uppDlgHookYDProcInfo, (item), (theDialog), (yourDataPtr))
    #define InvokeModalFilterYDUPP(theDialog, theEvent, itemHit, yourDataPtr, userUPP)  (Boolean)CALL_FOUR_PARAMETER_UPP((userUPP), uppModalFilterYDProcInfo, (theDialog), (theEvent), (itemHit), (yourDataPtr))
    #define InvokeFileFilterYDUPP(pb, yourDataPtr, userUPP)         (Boolean)CALL_TWO_PARAMETER_UPP((userUPP), uppFileFilterYDProcInfo, (pb), (yourDataPtr))
    #define InvokeActivateYDUPP(theDialog, itemNo, activating, yourDataPtr, userUPP)  CALL_FOUR_PARAMETER_UPP((userUPP), uppActivateYDProcInfo, (theDialog), (itemNo), (activating), (yourDataPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewDlgHookProc(userRoutine)                             NewDlgHookUPP(userRoutine)
#define NewFileFilterProc(userRoutine)                          NewFileFilterUPP(userRoutine)
#define NewDlgHookYDProc(userRoutine)                           NewDlgHookYDUPP(userRoutine)
#define NewModalFilterYDProc(userRoutine)                       NewModalFilterYDUPP(userRoutine)
#define NewFileFilterYDProc(userRoutine)                        NewFileFilterYDUPP(userRoutine)
#define NewActivateYDProc(userRoutine)                          NewActivateYDUPP(userRoutine)
#define CallDlgHookProc(userRoutine, item, theDialog)           InvokeDlgHookUPP(item, theDialog, userRoutine)
#define CallFileFilterProc(userRoutine, pb)                     InvokeFileFilterUPP(pb, userRoutine)
#define CallDlgHookYDProc(userRoutine, item, theDialog, yourDataPtr) InvokeDlgHookYDUPP(item, theDialog, yourDataPtr, userRoutine)
#define CallModalFilterYDProc(userRoutine, theDialog, theEvent, itemHit, yourDataPtr) InvokeModalFilterYDUPP(theDialog, theEvent, itemHit, yourDataPtr, userRoutine)
#define CallFileFilterYDProc(userRoutine, pb, yourDataPtr)      InvokeFileFilterYDUPP(pb, yourDataPtr, userRoutine)
#define CallActivateYDProc(userRoutine, theDialog, itemNo, activating, yourDataPtr) InvokeActivateYDUPP(theDialog, itemNo, activating, yourDataPtr, userRoutine)
typedef OSType                          SFTypeList[4];
/*
    The GetFile "typeList" parameter type has changed from "SFTypeList" to "ConstSFTypeListPtr".
    For C, this will add "const" and make it an in-only parameter.
    For Pascal, this will require client code to use the @ operator, but make it easier to specify long lists.

    ConstSFTypeListPtr is a pointer to an array of OSTypes.
*/
typedef const OSType *                  ConstSFTypeListPtr;
#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
SFPutFile                       (Point                  where,
                                 ConstStr255Param       prompt, /* can be NULL */
                                 ConstStr255Param       origName,
                                 DlgHookUPP             dlgHook, /* can be NULL */
                                 SFReply *              reply)                              THREEWORDINLINE(0x3F3C, 0x0001, 0xA9EA);

EXTERN_API( void )
SFGetFile                       (Point                  where,
                                 ConstStr255Param       prompt,
                                 FileFilterUPP          fileFilter, /* can be NULL */
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList, /* can be NULL */
                                 DlgHookUPP             dlgHook, /* can be NULL */
                                 SFReply *              reply)                              THREEWORDINLINE(0x3F3C, 0x0002, 0xA9EA);

EXTERN_API( void )
SFPPutFile                      (Point                  where,
                                 ConstStr255Param       prompt, /* can be NULL */
                                 ConstStr255Param       origName,
                                 DlgHookUPP             dlgHook,
                                 SFReply *              reply,
                                 short                  dlgID,
                                 ModalFilterUPP         filterProc) /* can be NULL */       THREEWORDINLINE(0x3F3C, 0x0003, 0xA9EA);

EXTERN_API( void )
SFPGetFile                      (Point                  where,
                                 ConstStr255Param       prompt,
                                 FileFilterUPP          fileFilter, /* can be NULL */
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList, /* can be NULL */
                                 DlgHookUPP             dlgHook, /* can be NULL */
                                 SFReply *              reply,
                                 short                  dlgID,
                                 ModalFilterUPP         filterProc) /* can be NULL */       THREEWORDINLINE(0x3F3C, 0x0004, 0xA9EA);

EXTERN_API( void )
StandardPutFile                 (ConstStr255Param       prompt, /* can be NULL */
                                 ConstStr255Param       defaultName,
                                 StandardFileReply *    reply)                              THREEWORDINLINE(0x3F3C, 0x0005, 0xA9EA);

EXTERN_API( void )
StandardGetFile                 (FileFilterUPP          fileFilter, /* can be NULL */
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList, /* can be NULL */
                                 StandardFileReply *    reply)                              THREEWORDINLINE(0x3F3C, 0x0006, 0xA9EA);

EXTERN_API( void )
CustomPutFile                   (ConstStr255Param       prompt, /* can be NULL */
                                 ConstStr255Param       defaultName,
                                 StandardFileReply *    reply,
                                 short                  dlgID,
                                 Point                  where,
                                 DlgHookYDUPP           dlgHook, /* can be NULL */
                                 ModalFilterYDUPP       filterProc, /* can be NULL */
                                 ActivationOrderListPtr  activeList, /* can be NULL */
                                 ActivateYDUPP          activate, /* can be NULL */
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x3F3C, 0x0007, 0xA9EA);

EXTERN_API( void )
CustomGetFile                   (FileFilterYDUPP        fileFilter, /* can be NULL */
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList, /* can be NULL */
                                 StandardFileReply *    reply,
                                 short                  dlgID,
                                 Point                  where,
                                 DlgHookYDUPP           dlgHook, /* can be NULL */
                                 ModalFilterYDUPP       filterProc, /* can be NULL */
                                 ActivationOrderListPtr  activeList, /* can be NULL */
                                 ActivateYDUPP          activate, /* can be NULL */
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x3F3C, 0x0008, 0xA9EA);

EXTERN_API( OSErr )
StandardOpenDialog              (StandardFileReply *    reply);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
sfpputfile                      (Point *                where,
                                 const char *           prompt,
                                 const char *           origName,
                                 DlgHookUPP             dlgHook, /* can be NULL */
                                 SFReply *              reply,
                                 short                  dlgID,
                                 ModalFilterUPP         filterProc) /* can be NULL */;

EXTERN_API_C( void )
sfgetfile                       (Point *                where,
                                 const char *           prompt,
                                 FileFilterUPP          fileFilter, /* can be NULL */
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList, /* can be NULL */
                                 DlgHookUPP             dlgHook, /* can be NULL */
                                 SFReply *              reply);

EXTERN_API_C( void )
sfpgetfile                      (Point *                where,
                                 const char *           prompt,
                                 FileFilterUPP          fileFilter, /* can be NULL */
                                 short                  numTypes,
                                 ConstSFTypeListPtr     typeList, /* can be NULL */
                                 DlgHookUPP             dlgHook, /* can be NULL */
                                 SFReply *              reply,
                                 short                  dlgID,
                                 ModalFilterUPP         filterProc) /* can be NULL */;

EXTERN_API_C( void )
sfputfile                       (Point *                where,
                                 const char *           prompt,
                                 const char *           origName,
                                 DlgHookUPP             dlgHook, /* can be NULL */
                                 SFReply *              reply);

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

#endif /* __STANDARDFILE__ */

