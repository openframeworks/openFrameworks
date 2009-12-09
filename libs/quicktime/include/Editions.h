/*
     File:       Editions.h
 
     Contains:   Edition Manager Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1989-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __EDITIONS__
#define __EDITIONS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __ALIASES__
#include <Aliases.h>
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
                                                                /* resource types  */
    rSectionType                = FOUR_CHAR_CODE('sect'),       /* ResType of saved SectionRecords */
                                                                /* Finder types for edition files */
    kPICTEditionFileType        = FOUR_CHAR_CODE('edtp'),
    kTEXTEditionFileType        = FOUR_CHAR_CODE('edtt'),
    ksndEditionFileType         = FOUR_CHAR_CODE('edts'),
    kUnknownEditionFileType     = FOUR_CHAR_CODE('edtu'),
    kPublisherDocAliasFormat    = FOUR_CHAR_CODE('alis'),
    kPreviewFormat              = FOUR_CHAR_CODE('prvw'),
    kFormatListFormat           = FOUR_CHAR_CODE('fmts')
};

enum {
                                                                /* section types */
    stSubscriber                = 0x01,
    stPublisher                 = 0x0A
};

enum {
    sumAutomatic                = 0,                            /* subscriber update mode - Automatically     */
    sumManual                   = 1,                            /* subscriber update mode - Manually */
    pumOnSave                   = 0,                            /* publisher update mode - OnSave            */
    pumManual                   = 1                             /* publisher update mode - Manually */
};

enum {
    kPartsNotUsed               = 0,
    kPartNumberUnknown          = -1
};

enum {
    kPreviewWidth               = 120,
    kPreviewHeight              = 120
};

enum {
                                                                /* bits for formatsMask */
    kPICTformatMask             = 1,
    kTEXTformatMask             = 2,
    ksndFormatMask              = 4
};


enum {
                                                                /* pseudo-item hits for dialogHooks the first is for NewPublisher or NewSubscriber Dialogs */
    emHookRedrawPreview         = 150,                          /* the following are for SectionOptions Dialog */
    emHookCancelSection         = 160,
    emHookGoToPublisher         = 161,
    emHookGetEditionNow         = 162,
    emHookSendEditionNow        = 162,
    emHookManualUpdateMode      = 163,
    emHookAutoUpdateMode        = 164
};


enum {
                                                                /* the refcon field of the dialog record during a modalfilter or dialoghook contains one the following */
    emOptionsDialogRefCon       = FOUR_CHAR_CODE('optn'),
    emCancelSectionDialogRefCon = FOUR_CHAR_CODE('cncl'),
    emGoToPubErrDialogRefCon    = FOUR_CHAR_CODE('gerr')
};

enum {
    kFormatLengthUnknown        = -1
};

/* one byte, stSubscriber or stPublisher */
typedef SignedByte                      SectionType;
/* seconds since 1904 */
typedef unsigned long                   TimeStamp;
/* similar to ResType */
typedef FourCharCode                    FormatType;
/* used in Edition I/O */
typedef Handle                          EditionRefNum;
/* update modes */
/* sumAutomatic, pumSuspend, etc */
typedef short                           UpdateMode;
typedef struct SectionRecord            SectionRecord;
typedef SectionRecord *                 SectionPtr;
typedef SectionPtr *                    SectionHandle;

struct SectionRecord {
    SignedByte                      version;                    /* always 0x01 in system 7.0 */
    SectionType                     kind;                       /* stSubscriber or stPublisher */
    UpdateMode                      mode;                       /* auto or manual */
    TimeStamp                       mdDate;                     /* last change in document */
    long                            sectionID;                  /* app. specific, unique per document */
    long                            refCon;                     /* application specific */
    AliasHandle                     alias;                      /* handle to Alias Record */
    long                            subPart;                    /* which part of container file */
    SectionHandle                   nextSection;                /* for linked list of app's Sections */
    Handle                          controlBlock;               /* used internally */
    EditionRefNum                   refNum;                     /* used internally */
};


struct EditionContainerSpec {
    FSSpec                          theFile;
    ScriptCode                      theFileScript;
    long                            thePart;
    Str31                           thePartName;
    ScriptCode                      thePartScript;
};
typedef struct EditionContainerSpec     EditionContainerSpec;
typedef EditionContainerSpec *          EditionContainerSpecPtr;

struct EditionInfoRecord {
    TimeStamp                       crDate;                     /* date EditionContainer was created */
    TimeStamp                       mdDate;                     /* date of last change */
    OSType                          fdCreator;                  /* file creator */
    OSType                          fdType;                     /* file type */
    EditionContainerSpec            container;                  /* the Edition */
};
typedef struct EditionInfoRecord        EditionInfoRecord;

struct NewPublisherReply {
    Boolean                         canceled;                   /* O */
    Boolean                         replacing;
    Boolean                         usePart;                    /* I */
    SInt8                           filler;
    Handle                          preview;                    /* I */
    FormatType                      previewFormat;              /* I */
    EditionContainerSpec            container;                  /* I/O */
};
typedef struct NewPublisherReply        NewPublisherReply;

struct NewSubscriberReply {
    Boolean                         canceled;                   /* O */
    SignedByte                      formatsMask;
    EditionContainerSpec            container;                  /*I/O*/
};
typedef struct NewSubscriberReply       NewSubscriberReply;

struct SectionOptionsReply {
    Boolean                         canceled;                   /* O */
    Boolean                         changed;                    /* O */
    SectionHandle                   sectionH;                   /* I */
    ResType                         action;                     /* O */
};
typedef struct SectionOptionsReply      SectionOptionsReply;
typedef CALLBACK_API( Boolean , ExpModalFilterProcPtr )(DialogPtr theDialog, EventRecord *theEvent, short itemOffset, short *itemHit, Ptr yourDataPtr);
typedef CALLBACK_API( short , ExpDlgHookProcPtr )(short itemOffset, short itemHit, DialogPtr theDialog, Ptr yourDataPtr);
typedef STACK_UPP_TYPE(ExpModalFilterProcPtr)                   ExpModalFilterUPP;
typedef STACK_UPP_TYPE(ExpDlgHookProcPtr)                       ExpDlgHookUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ExpModalFilterUPP)
    NewExpModalFilterUPP           (ExpModalFilterProcPtr   userRoutine);

    EXTERN_API(ExpDlgHookUPP)
    NewExpDlgHookUPP               (ExpDlgHookProcPtr       userRoutine);

    EXTERN_API(void)
    DisposeExpModalFilterUPP       (ExpModalFilterUPP       userUPP);

    EXTERN_API(void)
    DisposeExpDlgHookUPP           (ExpDlgHookUPP           userUPP);

    EXTERN_API(Boolean)
    InvokeExpModalFilterUPP        (DialogPtr               theDialog,
                                    EventRecord *           theEvent,
                                    short                   itemOffset,
                                    short *                 itemHit,
                                    Ptr                     yourDataPtr,
                                    ExpModalFilterUPP       userUPP);

    EXTERN_API(short)
    InvokeExpDlgHookUPP            (short                   itemOffset,
                                    short                   itemHit,
                                    DialogPtr               theDialog,
                                    Ptr                     yourDataPtr,
                                    ExpDlgHookUPP           userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppExpModalFilterProcInfo = 0x0000FBD0 };                /* pascal 1_byte Func(4_bytes, 4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppExpDlgHookProcInfo = 0x00003EA0 };                    /* pascal 2_bytes Func(2_bytes, 2_bytes, 4_bytes, 4_bytes) */
    #define NewExpModalFilterUPP(userRoutine)                       (ExpModalFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppExpModalFilterProcInfo, GetCurrentArchitecture())
    #define NewExpDlgHookUPP(userRoutine)                           (ExpDlgHookUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppExpDlgHookProcInfo, GetCurrentArchitecture())
    #define DisposeExpModalFilterUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposeExpDlgHookUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define InvokeExpModalFilterUPP(theDialog, theEvent, itemOffset, itemHit, yourDataPtr, userUPP)  (Boolean)CALL_FIVE_PARAMETER_UPP((userUPP), uppExpModalFilterProcInfo, (theDialog), (theEvent), (itemOffset), (itemHit), (yourDataPtr))
    #define InvokeExpDlgHookUPP(itemOffset, itemHit, theDialog, yourDataPtr, userUPP)  (short)CALL_FOUR_PARAMETER_UPP((userUPP), uppExpDlgHookProcInfo, (itemOffset), (itemHit), (theDialog), (yourDataPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewExpModalFilterProc(userRoutine)                      NewExpModalFilterUPP(userRoutine)
#define NewExpDlgHookProc(userRoutine)                          NewExpDlgHookUPP(userRoutine)
#define CallExpModalFilterProc(userRoutine, theDialog, theEvent, itemOffset, itemHit, yourDataPtr) InvokeExpModalFilterUPP(theDialog, theEvent, itemOffset, itemHit, yourDataPtr, userRoutine)
#define CallExpDlgHookProc(userRoutine, itemOffset, itemHit, theDialog, yourDataPtr) InvokeExpDlgHookUPP(itemOffset, itemHit, theDialog, yourDataPtr, userRoutine)

typedef SInt8 FormatIOVerb;
enum {
    ioHasFormat                 = 0,
    ioReadFormat                = 1,
    ioNewFormat                 = 2,
    ioWriteFormat               = 3
};


typedef SInt8 EditionOpenerVerb;
enum {
    eoOpen                      = 0,
    eoClose                     = 1,
    eoOpenNew                   = 2,
    eoCloseNew                  = 3,
    eoCanSubscribe              = 4
};



struct FormatIOParamBlock {
    long                            ioRefNum;
    FormatType                      format;
    long                            formatIndex;
    unsigned long                   offset;
    Ptr                             buffPtr;
    unsigned long                   buffLen;
};
typedef struct FormatIOParamBlock       FormatIOParamBlock;
typedef struct EditionOpenerParamBlock  EditionOpenerParamBlock;
typedef CALLBACK_API( short , FormatIOProcPtr )(FormatIOVerb selector, FormatIOParamBlock *PB);
typedef CALLBACK_API( short , EditionOpenerProcPtr )(EditionOpenerVerb selector, EditionOpenerParamBlock *PB);
typedef STACK_UPP_TYPE(FormatIOProcPtr)                         FormatIOUPP;
typedef STACK_UPP_TYPE(EditionOpenerProcPtr)                    EditionOpenerUPP;

struct EditionOpenerParamBlock {
    EditionInfoRecord               info;
    SectionHandle                   sectionH;
    const FSSpec *                  document;
    OSType                          fdCreator;
    long                            ioRefNum;
    FormatIOUPP                     ioProc;
    Boolean                         success;
    SignedByte                      formatsMask;
};

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(FormatIOUPP)
    NewFormatIOUPP                 (FormatIOProcPtr         userRoutine);

    EXTERN_API(EditionOpenerUPP)
    NewEditionOpenerUPP            (EditionOpenerProcPtr    userRoutine);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppFormatIOProcInfo = 0x00000360 };                      /* pascal 2_bytes Func(1_byte, 4_bytes) */
    enum { uppEditionOpenerProcInfo = 0x00000360 };                 /* pascal 2_bytes Func(1_byte, 4_bytes) */
    #define NewFormatIOUPP(userRoutine)                             (FormatIOUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppFormatIOProcInfo, GetCurrentArchitecture())
    #define NewEditionOpenerUPP(userRoutine)                        (EditionOpenerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppEditionOpenerProcInfo, GetCurrentArchitecture())
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewFormatIOProc(userRoutine)                            NewFormatIOUPP(userRoutine)
#define NewEditionOpenerProc(userRoutine)                       NewEditionOpenerUPP(userRoutine)
/* 
 Section events now arrive in the message buffer using the AppleEvent format.
 The direct object parameter is an aeTemporaryIDParamType ('tid '). The temporary
 ID's type is rSectionType ('sect') and the 32-bit value is a SectionHandle.
 The following is a sample buffer
 
 name       offset  contents
 ----       ------  --------
 header         0   'aevt'
 majorVersion   4   0x01
 minorVersion   6   0x01
 endOfMetaData  8   ';;;;' 
 directObjKey   12  '----' 
 paramType      16  'tid ' 
 paramLength    20  0x0008 
 tempIDType     24  'sect' 
 tempID         28  the SectionHandle <-- this is want you want
*/

enum {
    sectionEventMsgClass        = FOUR_CHAR_CODE('sect'),
    sectionReadMsgID            = FOUR_CHAR_CODE('read'),
    sectionWriteMsgID           = FOUR_CHAR_CODE('writ'),
    sectionScrollMsgID          = FOUR_CHAR_CODE('scrl'),
    sectionCancelMsgID          = FOUR_CHAR_CODE('cncl')
};

enum {
    currentEditionMgrVers       = 0x0011
};


#if TARGET_RT_MAC_CFM
#define InitEditionPack() InitEditionPackVersion(currentEditionMgrVers)
#else
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
InitEditionPack                 (void)                                                      FIVEWORDINLINE(0x3F3C, 0x0011, 0x303C, 0x0100, 0xA82D);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_RT_MAC_CFM */

#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
InitEditionPackVersion          (short                  curEditionMgrVers)                  THREEWORDINLINE(0x303C, 0x0100, 0xA82D);

EXTERN_API( OSErr )
NewSection                      (const EditionContainerSpec * container,
                                 const FSSpec *         sectionDocument, /* can be NULL */
                                 SectionType            kind,
                                 long                   sectionID,
                                 UpdateMode             initalMode,
                                 SectionHandle *        sectionH)                           THREEWORDINLINE(0x303C, 0x0A02, 0xA82D);

EXTERN_API( OSErr )
RegisterSection                 (const FSSpec *         sectionDocument,
                                 SectionHandle          sectionH,
                                 Boolean *              aliasWasUpdated)                    THREEWORDINLINE(0x303C, 0x0604, 0xA82D);

EXTERN_API( OSErr )
UnRegisterSection               (SectionHandle          sectionH)                           THREEWORDINLINE(0x303C, 0x0206, 0xA82D);

EXTERN_API( OSErr )
IsRegisteredSection             (SectionHandle          sectionH)                           THREEWORDINLINE(0x303C, 0x0208, 0xA82D);

EXTERN_API( OSErr )
AssociateSection                (SectionHandle          sectionH,
                                 const FSSpec *         newSectionDocument)                 THREEWORDINLINE(0x303C, 0x040C, 0xA82D);

EXTERN_API( OSErr )
CreateEditionContainerFile      (const FSSpec *         editionFile,
                                 OSType                 fdCreator,
                                 ScriptCode             editionFileNameScript)              THREEWORDINLINE(0x303C, 0x050E, 0xA82D);

EXTERN_API( OSErr )
DeleteEditionContainerFile      (const FSSpec *         editionFile)                        THREEWORDINLINE(0x303C, 0x0210, 0xA82D);

EXTERN_API( OSErr )
OpenEdition                     (SectionHandle          subscriberSectionH,
                                 EditionRefNum *        refNum)                             THREEWORDINLINE(0x303C, 0x0412, 0xA82D);

EXTERN_API( OSErr )
OpenNewEdition                  (SectionHandle          publisherSectionH,
                                 OSType                 fdCreator,
                                 const FSSpec *         publisherSectionDocument, /* can be NULL */
                                 EditionRefNum *        refNum)                             THREEWORDINLINE(0x303C, 0x0814, 0xA82D);

EXTERN_API( OSErr )
CloseEdition                    (EditionRefNum          whichEdition,
                                 Boolean                successful)                         THREEWORDINLINE(0x303C, 0x0316, 0xA82D);

EXTERN_API( OSErr )
EditionHasFormat                (EditionRefNum          whichEdition,
                                 FormatType             whichFormat,
                                 Size *                 formatSize)                         THREEWORDINLINE(0x303C, 0x0618, 0xA82D);

EXTERN_API( OSErr )
ReadEdition                     (EditionRefNum          whichEdition,
                                 FormatType             whichFormat,
                                 void *                 buffPtr,
                                 Size *                 buffLen)                            THREEWORDINLINE(0x303C, 0x081A, 0xA82D);

EXTERN_API( OSErr )
WriteEdition                    (EditionRefNum          whichEdition,
                                 FormatType             whichFormat,
                                 const void *           buffPtr,
                                 Size                   buffLen)                            THREEWORDINLINE(0x303C, 0x081C, 0xA82D);

EXTERN_API( OSErr )
GetEditionFormatMark            (EditionRefNum          whichEdition,
                                 FormatType             whichFormat,
                                 unsigned long *        currentMark)                        THREEWORDINLINE(0x303C, 0x061E, 0xA82D);

EXTERN_API( OSErr )
SetEditionFormatMark            (EditionRefNum          whichEdition,
                                 FormatType             whichFormat,
                                 unsigned long          setMarkTo)                          THREEWORDINLINE(0x303C, 0x0620, 0xA82D);

EXTERN_API( OSErr )
GetEditionInfo                  (SectionHandle          sectionH,
                                 EditionInfoRecord *    editionInfo)                        THREEWORDINLINE(0x303C, 0x0422, 0xA82D);

EXTERN_API( OSErr )
GoToPublisherSection            (const EditionContainerSpec * container)                    THREEWORDINLINE(0x303C, 0x0224, 0xA82D);

EXTERN_API( OSErr )
GetLastEditionContainerUsed     (EditionContainerSpec * container)                          THREEWORDINLINE(0x303C, 0x0226, 0xA82D);

EXTERN_API( OSErr )
GetStandardFormats              (const EditionContainerSpec * container,
                                 FormatType *           previewFormat,
                                 Handle                 preview,
                                 Handle                 publisherAlias,
                                 Handle                 formats)                            THREEWORDINLINE(0x303C, 0x0A28, 0xA82D);

EXTERN_API( OSErr )
GetEditionOpenerProc            (EditionOpenerUPP *     opener)                             THREEWORDINLINE(0x303C, 0x022A, 0xA82D);

EXTERN_API( OSErr )
SetEditionOpenerProc            (EditionOpenerUPP       opener)                             THREEWORDINLINE(0x303C, 0x022C, 0xA82D);

EXTERN_API( OSErr )
CallEditionOpenerProc           (EditionOpenerVerb      selector,
                                 EditionOpenerParamBlock * PB,
                                 EditionOpenerUPP       routine)                            THREEWORDINLINE(0x303C, 0x052E, 0xA82D);

EXTERN_API( OSErr )
CallFormatIOProc                (FormatIOVerb           selector,
                                 FormatIOParamBlock *   PB,
                                 FormatIOUPP            routine)                            THREEWORDINLINE(0x303C, 0x0530, 0xA82D);

EXTERN_API( OSErr )
NewSubscriberDialog             (NewSubscriberReply *   reply)                              THREEWORDINLINE(0x303C, 0x0232, 0xA82D);

EXTERN_API( OSErr )
NewSubscriberExpDialog          (NewSubscriberReply *   reply,
                                 Point                  where,
                                 short                  expansionDITLresID,
                                 ExpDlgHookUPP          dlgHook,
                                 ExpModalFilterUPP      filter,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x303C, 0x0B34, 0xA82D);

EXTERN_API( OSErr )
NewPublisherDialog              (NewPublisherReply *    reply)                              THREEWORDINLINE(0x303C, 0x0236, 0xA82D);

EXTERN_API( OSErr )
NewPublisherExpDialog           (NewPublisherReply *    reply,
                                 Point                  where,
                                 short                  expansionDITLresID,
                                 ExpDlgHookUPP          dlgHook,
                                 ExpModalFilterUPP      filter,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x303C, 0x0B38, 0xA82D);

EXTERN_API( OSErr )
SectionOptionsDialog            (SectionOptionsReply *  reply)                              THREEWORDINLINE(0x303C, 0x023A, 0xA82D);

EXTERN_API( OSErr )
SectionOptionsExpDialog         (SectionOptionsReply *  reply,
                                 Point                  where,
                                 short                  expansionDITLresID,
                                 ExpDlgHookUPP          dlgHook,
                                 ExpModalFilterUPP      filter,
                                 void *                 yourDataPtr)                        THREEWORDINLINE(0x303C, 0x0B3C, 0xA82D);


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

#endif /* __EDITIONS__ */

