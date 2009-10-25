/*
     File:       OCEStandardMail.h
 
     Contains:   Apple Open Collaboration Environment Standard Mail Interfaces.
 
     Version:    Technology: AOCE Toolbox 1.02
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OCESTANDARDMAIL__
#define __OCESTANDARDMAIL__

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif


#ifndef __OCEAUTHDIR__
#include <OCEAuthDir.h>
#endif

#ifndef __OCEMAIL__
#include <OCEMail.h>
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
    kSMPVersion                 = 1
};

enum {
    kSMPNativeFormatName        = FOUR_CHAR_CODE('natv')
};


struct LetterSpec {
    unsigned long                   spec[3];
};
typedef struct LetterSpec               LetterSpec;
enum {
    typeLetterSpec              = FOUR_CHAR_CODE('lttr')
};

/*  Wildcard used for filtering letter types. */
enum {
    FilterAnyLetter             = FOUR_CHAR_CODE('ltr*'),
    FilterAppleLetterContent    = FOUR_CHAR_CODE('ltc*'),
    FilterImageContent          = FOUR_CHAR_CODE('lti*')
};



struct LetterDescriptor {
    Boolean                         onDisk;
    Boolean                         filler1;
    union {
        FSSpec                          fileSpec;
        LetterSpec                      mailboxSpec;
    }                                 u;
};
typedef struct LetterDescriptor         LetterDescriptor;
/*
SMPPSendAs values.  You may add the following values together to determine how the
file is sent, but you may not set both kSMPSendAsEnclosureMask and kSMPSendFileOnlyMask.  This
will allow you to send the letter as an image so that it will work with fax gateways
and send as an enclosure as well.
*/
enum {
    kSMPSendAsEnclosureBit      = 0,                            /* Appears as letter with enclosures */
    kSMPSendFileOnlyBit         = 1,                            /* Appears as a file in mailbox. */
    kSMPSendAsImageBit          = 2                             /* Content imaged in letter */
};

/* Values of SMPPSendAs */
enum {
    kSMPSendAsEnclosureMask     = 1 << kSMPSendAsEnclosureBit,
    kSMPSendFileOnlyMask        = 1 << kSMPSendFileOnlyBit,
    kSMPSendAsImageMask         = 1 << kSMPSendAsImageBit
};


typedef Byte                            SMPPSendAs;
/* Send Package Structures */
typedef CALLBACK_API( void , SMPDrawImageProcPtr )(long refcon, Boolean inColor);
typedef STACK_UPP_TYPE(SMPDrawImageProcPtr)                     SMPDrawImageUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(SMPDrawImageUPP)
    NewSMPDrawImageUPP             (SMPDrawImageProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeSMPDrawImageUPP         (SMPDrawImageUPP         userUPP);

    EXTERN_API(void)
    InvokeSMPDrawImageUPP          (long                    refcon,
                                    Boolean                 inColor,
                                    SMPDrawImageUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppSMPDrawImageProcInfo = 0x000001C0 };                  /* pascal no_return_value Func(4_bytes, 1_byte) */
    #define NewSMPDrawImageUPP(userRoutine)                         (SMPDrawImageUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSMPDrawImageProcInfo, GetCurrentArchitecture())
    #define DisposeSMPDrawImageUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeSMPDrawImageUPP(refcon, inColor, userUPP)         CALL_TWO_PARAMETER_UPP((userUPP), uppSMPDrawImageProcInfo, (refcon), (inColor))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewSMPDrawImageProc(userRoutine)                        NewSMPDrawImageUPP(userRoutine)
#define CallSMPDrawImageProc(userRoutine, refcon, inColor)      InvokeSMPDrawImageUPP(refcon, inColor, userRoutine)

struct SMPRecipientDescriptor {
    struct SMPRecipientDescriptor * next;                       /*  Q-Link. */
    OSErr                           result;                     /*  result code when using the object. */
    OCEPackedRecipient *            recipient;                  /*  Pointer to a Packed Address. */
    unsigned long                   size;                       /*  length of recipient in bytes. */
    MailRecipient                   theAddress;                 /*  structure points into recipient and theRID. */
    RecordID                        theRID;                     /*  structure points into recipient. */
};
typedef struct SMPRecipientDescriptor   SMPRecipientDescriptor;
typedef SMPRecipientDescriptor *        SMPRecipientDescriptorPtr;

struct SMPEnclosureDescriptor {
    struct SMPEnclosureDescriptor * next;
    OSErr                           result;
    FSSpec                          fileSpec;
    OSType                          fileCreator;                /*  Creator of this enclosure. */
    OSType                          fileType;                   /*  File Type of this enclosure. */
};
typedef struct SMPEnclosureDescriptor   SMPEnclosureDescriptor;
typedef SMPEnclosureDescriptor *        SMPEnclosureDescriptorPtr;

struct SMPLetterPB {
    OSErr                           result;                     /* result of operation */
    RStringPtr                      subject;                    /* RString */
    AuthIdentity                    senderIdentity;             /* Letter is sent from this Identity */
    SMPRecipientDescriptorPtr       toList;                     /* Pointer to linked list */
    SMPRecipientDescriptorPtr       ccList;                     /* Pointer to linked list */
    SMPRecipientDescriptorPtr       bccList;                    /* Pointer to linked list */
    ScriptCode                      script;                     /* Identifier for language */
    Size                            textSize;                   /* length of body data */
    void *                          textBuffer;                 /* body of the letter */
    SMPPSendAs                      sendAs;                     /* Send as Letter,Enclosure,Image */
    Byte                            padByte;
    SMPEnclosureDescriptorPtr       enclosures;                 /* files to be enclosed */
    SMPDrawImageUPP                 drawImageProc;              /* For imaging */
    long                            imageRefCon;                /* For imaging */
    Boolean                         supportsColor;              /* For imaging - set to true if you application supports color imaging */
    Boolean                         filler1;
};
typedef struct SMPLetterPB              SMPLetterPB;
typedef SMPLetterPB *                   SMPLetterPBPtr;
enum {
    kSMPAppMustHandleEventBit   = 0,
    kSMPAppShouldIgnoreEventBit = 1,
    kSMPContractedBit           = 2,
    kSMPExpandedBit             = 3,
    kSMPMailerBecomesTargetBit  = 4,
    kSMPAppBecomesTargetBit     = 5,
    kSMPCursorOverMailerBit     = 6,
    kSMPCreateCopyWindowBit     = 7,
    kSMPDisposeCopyWindowBit    = 8
};

/* Values of SMPMailerResult */
enum {
    kSMPAppMustHandleEventMask  = 1 << kSMPAppMustHandleEventBit,
    kSMPAppShouldIgnoreEventMask = 1 << kSMPAppShouldIgnoreEventBit,
    kSMPContractedMask          = 1 << kSMPContractedBit,
    kSMPExpandedMask            = 1 << kSMPExpandedBit,
    kSMPMailerBecomesTargetMask = 1 << kSMPMailerBecomesTargetBit,
    kSMPAppBecomesTargetMask    = 1 << kSMPAppBecomesTargetBit,
    kSMPCursorOverMailerMask    = 1 << kSMPCursorOverMailerBit,
    kSMPCreateCopyWindowMask    = 1 << kSMPCreateCopyWindowBit,
    kSMPDisposeCopyWindowMask   = 1 << kSMPDisposeCopyWindowBit
};


typedef unsigned long                   SMPMailerResult;
/* Values of SMPMailerComponent*/
enum {
    kSMPOther                   = -1,
    kSMPFrom                    = 32,
    kSMPTo                      = 20,
    kSMPRegarding               = 22,
    kSMPSendDateTime            = 29,
    kSMPAttachments             = 26,
    kSMPAddressOMatic           = 16
};

typedef unsigned long                   SMPMailerComponent;

enum {
    kSMPToAddress               = kMailToBit,
    kSMPCCAddress               = kMailCcBit,
    kSMPBCCAddress              = kMailBccBit
};

typedef MailAttributeID                 SMPAddressType;

enum {
    kSMPUndoCommand             = 0,
    kSMPCutCommand              = 1,
    kSMPCopyCommand             = 2,
    kSMPPasteCommand            = 3,
    kSMPClearCommand            = 4,
    kSMPSelectAllCommand        = 5
};

typedef unsigned short                  SMPEditCommand;
enum {
    kSMPUndoDisabled            = 0,
    kSMPAppMayUndo              = 1,
    kSMPMailerUndo              = 2
};

typedef unsigned short                  SMPUndoState;
/*
SMPSendFormatMask:

Bitfield indicating which combinations of formats are included in,
should be included or, or can be included in a letter.
*/
enum {
    kSMPNativeBit               = 0,
    kSMPImageBit                = 1,
    kSMPStandardInterchangeBit  = 2
};

/* Values of SMPSendFormatMask */
enum {
    kSMPNativeMask              = 1 << kSMPNativeBit,
    kSMPImageMask               = 1 << kSMPImageBit,
    kSMPStandardInterchangeMask = 1 << kSMPStandardInterchangeBit
};

typedef unsigned long                   SMPSendFormatMask;

/*
    Pseudo-events passed to the clients filter proc for initialization and cleanup.
*/
enum {
    kSMPSendOptionsStart        = -1,
    kSMPSendOptionsEnd          = -2
};


/*
SMPSendFormatMask:

Structure describing the format of a letter.  If kSMPNativeMask bit is set, the whichNativeFormat field indicates which of the client-defined formats to use.
*/


struct SMPSendFormat {
    SMPSendFormatMask               whichFormats;
    short                           whichNativeFormat;          /* 0 based */
};
typedef struct SMPSendFormat            SMPSendFormat;



struct SMPLetterInfo {
    OSType                          letterCreator;
    OSType                          letterType;
    RString32                       subject;
    RString32                       sender;
};
typedef struct SMPLetterInfo            SMPLetterInfo;

enum {
    kSMPSave                    = 0,
    kSMPSaveAs                  = 1,
    kSMPSaveACopy               = 2
};


typedef unsigned short                  SMPSaveType;
typedef CALLBACK_API( WindowPtr , FrontWindowProcPtr )(long clientData);
typedef CALLBACK_API( void , PrepareMailerForDrawingProcPtr )(WindowPtr window, long clientData);
typedef STACK_UPP_TYPE(FrontWindowProcPtr)                      FrontWindowUPP;
typedef STACK_UPP_TYPE(PrepareMailerForDrawingProcPtr)          PrepareMailerForDrawingUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(FrontWindowUPP)
    NewFrontWindowUPP              (FrontWindowProcPtr      userRoutine);

    EXTERN_API(PrepareMailerForDrawingUPP)
    NewPrepareMailerForDrawingUPP    (PrepareMailerForDrawingProcPtr userRoutine);

    EXTERN_API(void)
    DisposeFrontWindowUPP          (FrontWindowUPP          userUPP);

    EXTERN_API(void)
    DisposePrepareMailerForDrawingUPP    (PrepareMailerForDrawingUPP userUPP);

    EXTERN_API(WindowPtr)
    InvokeFrontWindowUPP           (long                    clientData,
                                    FrontWindowUPP          userUPP);

    EXTERN_API(void)
    InvokePrepareMailerForDrawingUPP    (WindowPtr          window,
                                    long                    clientData,
                                    PrepareMailerForDrawingUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppFrontWindowProcInfo = 0x000000F0 };                   /* pascal 4_bytes Func(4_bytes) */
    enum { uppPrepareMailerForDrawingProcInfo = 0x000003C0 };       /* pascal no_return_value Func(4_bytes, 4_bytes) */
    #define NewFrontWindowUPP(userRoutine)                          (FrontWindowUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppFrontWindowProcInfo, GetCurrentArchitecture())
    #define NewPrepareMailerForDrawingUPP(userRoutine)              (PrepareMailerForDrawingUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPrepareMailerForDrawingProcInfo, GetCurrentArchitecture())
    #define DisposeFrontWindowUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposePrepareMailerForDrawingUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define InvokeFrontWindowUPP(clientData, userUPP)               (WindowPtr)CALL_ONE_PARAMETER_UPP((userUPP), uppFrontWindowProcInfo, (clientData))
    #define InvokePrepareMailerForDrawingUPP(window, clientData, userUPP)  CALL_TWO_PARAMETER_UPP((userUPP), uppPrepareMailerForDrawingProcInfo, (window), (clientData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewFrontWindowProc(userRoutine)                         NewFrontWindowUPP(userRoutine)
#define NewPrepareMailerForDrawingProc(userRoutine)             NewPrepareMailerForDrawingUPP(userRoutine)
#define CallFrontWindowProc(userRoutine, clientData)            InvokeFrontWindowUPP(clientData, userRoutine)
#define CallPrepareMailerForDrawingProc(userRoutine, window, clientData) InvokePrepareMailerForDrawingUPP(window, clientData, userRoutine)
typedef CALLBACK_API( Boolean , SendOptionsFilterProcPtr )(DialogPtr theDialog, EventRecord *theEvent, short itemHit, long clientData);
typedef STACK_UPP_TYPE(SendOptionsFilterProcPtr)                SendOptionsFilterUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(SendOptionsFilterUPP)
    NewSendOptionsFilterUPP        (SendOptionsFilterProcPtr userRoutine);

    EXTERN_API(void)
    DisposeSendOptionsFilterUPP    (SendOptionsFilterUPP    userUPP);

    EXTERN_API(Boolean)
    InvokeSendOptionsFilterUPP     (DialogPtr               theDialog,
                                    EventRecord *           theEvent,
                                    short                   itemHit,
                                    long                    clientData,
                                    SendOptionsFilterUPP    userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppSendOptionsFilterProcInfo = 0x00003BD0 };             /* pascal 1_byte Func(4_bytes, 4_bytes, 2_bytes, 4_bytes) */
    #define NewSendOptionsFilterUPP(userRoutine)                    (SendOptionsFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSendOptionsFilterProcInfo, GetCurrentArchitecture())
    #define DisposeSendOptionsFilterUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeSendOptionsFilterUPP(theDialog, theEvent, itemHit, clientData, userUPP)  (Boolean)CALL_FOUR_PARAMETER_UPP((userUPP), uppSendOptionsFilterProcInfo, (theDialog), (theEvent), (itemHit), (clientData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewSendOptionsFilterProc(userRoutine)                   NewSendOptionsFilterUPP(userRoutine)
#define CallSendOptionsFilterProc(userRoutine, theDialog, theEvent, itemHit, clientData) InvokeSendOptionsFilterUPP(theDialog, theEvent, itemHit, clientData, userRoutine)

struct SMPMailerState {
    short                           mailerCount;
    short                           currentMailer;
    Point                           upperLeft;
    Boolean                         hasBeenReceived;
    Boolean                         isTarget;
    Boolean                         isExpanded;
    Boolean                         canMoveToTrash;
    Boolean                         canTag;
    Byte                            padByte2;
    unsigned long                   changeCount;
    SMPMailerComponent              targetComponent;
    Boolean                         canCut;
    Boolean                         canCopy;
    Boolean                         canPaste;
    Boolean                         canClear;
    Boolean                         canSelectAll;
    Byte                            padByte3;
    SMPUndoState                    undoState;
    Str63                           undoWhat;
};
typedef struct SMPMailerState           SMPMailerState;


struct SMPSendOptions {
    Boolean                         signWhenSent;
    IPMPriority                     priority;
};
typedef struct SMPSendOptions           SMPSendOptions;
typedef SMPSendOptions *                SMPSendOptionsPtr;
typedef SMPSendOptionsPtr *             SMPSendOptionsHandle;


struct SMPCloseOptions {
    Boolean                         moveToTrash;
    Boolean                         addTag;
    RString32                       tag;
};
typedef struct SMPCloseOptions          SMPCloseOptions;
typedef SMPCloseOptions *               SMPCloseOptionsPtr;

/*----------------------------------------------------------------------------------------
    Send Package Routines
----------------------------------------------------------------------------------------*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
SMPSendLetter                   (SMPLetterPBPtr         theLetter)                          FOURWORDINLINE(0x203C, 0x0002, 0x01F4, 0xAA5D);

EXTERN_API( OSErr )
SMPNewPage                      (OpenCPicParams *       newHeader)                          FOURWORDINLINE(0x203C, 0x0002, 0x0834, 0xAA5D);

EXTERN_API( OSErr )
SMPImageErr                     (void)                                                      FOURWORDINLINE(0x203C, 0x0000, 0x0835, 0xAA5D);

EXTERN_API( OSErr )
SMPResolveToRecipient           (PackedDSSpecPtr        dsSpec,
                                 SMPRecipientDescriptorPtr * recipientList,
                                 AuthIdentity           identity)                           FOURWORDINLINE(0x203C, 0x0006, 0x044C, 0xAA5D);



EXTERN_API( OSErr )
SMPInitMailer                   (long                   mailerVersion)                      FOURWORDINLINE(0x203C, 0x0002, 0x1285, 0xAA5D);

EXTERN_API( OSErr )
SMPGetDimensions                (short *                width,
                                 short *                contractedHeight,
                                 short *                expandedHeight)                     FOURWORDINLINE(0x203C, 0x0006, 0x125C, 0xAA5D);

EXTERN_API( OSErr )
SMPGetTabInfo                   (SMPMailerComponent *   firstTab,
                                 SMPMailerComponent *   lastTab)                            FOURWORDINLINE(0x203C, 0x0004, 0x1274, 0xAA5D);

EXTERN_API( OSErr )
SMPNewMailer                    (WindowPtr              window,
                                 Point                  upperLeft,
                                 Boolean                canContract,
                                 Boolean                initiallyExpanded,
                                 AuthIdentity           identity,
                                 PrepareMailerForDrawingUPP  prepareMailerForDrawingCB,
                                 long                   clientData)                         FOURWORDINLINE(0x203C, 0x000C, 0x125D, 0xAA5D);

EXTERN_API( OSErr )
SMPPrepareToClose               (WindowPtr              window)                             FOURWORDINLINE(0x203C, 0x0002, 0x1287, 0xAA5D);

EXTERN_API( OSErr )
SMPCloseOptionsDialog           (WindowPtr              window,
                                 SMPCloseOptionsPtr     closeOptions)                       FOURWORDINLINE(0x203C, 0x0004, 0x1288, 0xAA5D);

EXTERN_API( OSErr )
SMPTagDialog                    (WindowPtr              window,
                                 RString32 *            theTag)                             FOURWORDINLINE(0x203C, 0x0004, 0x128B, 0xAA5D);

EXTERN_API( OSErr )
SMPDisposeMailer                (WindowPtr              window,
                                 SMPCloseOptionsPtr     closeOptions)                       FOURWORDINLINE(0x203C, 0x0004, 0x125E, 0xAA5D);

EXTERN_API( OSErr )
SMPMailerEvent                  (const EventRecord *    event,
                                 SMPMailerResult *      whatHappened,
                                 FrontWindowUPP         frontWindowCB,
                                 long                   clientData)                         FOURWORDINLINE(0x203C, 0x0008, 0x125F, 0xAA5D);

EXTERN_API( OSErr )
SMPClearUndo                    (WindowPtr              window)                             FOURWORDINLINE(0x203C, 0x0002, 0x1275, 0xAA5D);

EXTERN_API( OSErr )
SMPMailerEditCommand            (WindowPtr              window,
                                 SMPEditCommand         command,
                                 SMPMailerResult *      whatHappened)                       FOURWORDINLINE(0x203C, 0x0005, 0x1260, 0xAA5D);

EXTERN_API( OSErr )
SMPMailerForward                (WindowPtr              window,
                                 AuthIdentity           from)                               FOURWORDINLINE(0x203C, 0x0004, 0x1261, 0xAA5D);

EXTERN_API( OSErr )
SMPMailerReply                  (WindowPtr              originalLetter,
                                 WindowPtr              newLetter,
                                 Boolean                replyToAll,
                                 Point                  upperLeft,
                                 Boolean                canContract,
                                 Boolean                initiallyExpanded,
                                 AuthIdentity           identity,
                                 PrepareMailerForDrawingUPP  prepareMailerForDrawingCB,
                                 long                   clientData)                         FOURWORDINLINE(0x203C, 0x000F, 0x1262, 0xAA5D);

EXTERN_API( OSErr )
SMPGetMailerState               (WindowPtr              window,
                                 SMPMailerState *       itsState)                           FOURWORDINLINE(0x203C, 0x0004, 0x1263, 0xAA5D);

EXTERN_API( OSErr )
SMPSendOptionsDialog            (WindowPtr              window,
                                 Str255                 documentName,
                                 StringPtr              nativeFormatNames[],
                                 unsigned short         nameCount,
                                 SMPSendFormatMask      canSend,
                                 SMPSendFormat *        currentFormat,
                                 SendOptionsFilterUPP   filterProc,
                                 long                   clientData,
                                 SMPSendFormat *        shouldSend,
                                 SMPSendOptionsPtr      sendOptions)                        FOURWORDINLINE(0x203C, 0x0013, 0x1388, 0xAA5D);


EXTERN_API( OSErr )
SMPPrepareCoverPages            (WindowPtr              window,
                                 short *                pageCount)                          FOURWORDINLINE(0x203C, 0x0004, 0x1264, 0xAA5D);

EXTERN_API( OSErr )
SMPDrawNthCoverPage             (WindowPtr              window,
                                 short                  pageNumber,
                                 Boolean                doneDrawingCoverPages)              FOURWORDINLINE(0x203C, 0x0004, 0x1265, 0xAA5D);

EXTERN_API( OSErr )
SMPPrepareToChange              (WindowPtr              window)                             FOURWORDINLINE(0x203C, 0x0002, 0x1289, 0xAA5D);

EXTERN_API( OSErr )
SMPContentChanged               (WindowPtr              window)                             FOURWORDINLINE(0x203C, 0x0002, 0x126F, 0xAA5D);

EXTERN_API( OSErr )
SMPBeginSave                    (WindowPtr              window,
                                 const FSSpec *         diskLetter,
                                 OSType                 creator,
                                 OSType                 fileType,
                                 SMPSaveType            saveType,
                                 Boolean *              mustAddContent)                     FOURWORDINLINE(0x203C, 0x000B, 0x1266, 0xAA5D);

EXTERN_API( OSErr )
SMPEndSave                      (WindowPtr              window,
                                 Boolean                okToSave)                           FOURWORDINLINE(0x203C, 0x0003, 0x1270, 0xAA5D);

EXTERN_API( OSErr )
SMPBeginSend                    (WindowPtr              window,
                                 OSType                 creator,
                                 OSType                 fileType,
                                 SMPSendOptionsPtr      sendOptions,
                                 Boolean *              mustAddContent)                     FOURWORDINLINE(0x203C, 0x000A, 0x1267, 0xAA5D);

EXTERN_API( OSErr )
SMPEndSend                      (WindowPtr              window,
                                 Boolean                okToSend)                           FOURWORDINLINE(0x203C, 0x0003, 0x1271, 0xAA5D);

EXTERN_API( OSErr )
SMPOpenLetter                   (const LetterDescriptor * letter,
                                 WindowPtr              window,
                                 Point                  upperLeft,
                                 Boolean                canContract,
                                 Boolean                initiallyExpanded,
                                 PrepareMailerForDrawingUPP  prepareMailerForDrawingCB,
                                 long                   clientData)                         FOURWORDINLINE(0x203C, 0x000C, 0x1268, 0xAA5D);

EXTERN_API( OSErr )
SMPAddMainEnclosure             (WindowPtr              window,
                                 const FSSpec *         enclosure)                          FOURWORDINLINE(0x203C, 0x0004, 0x127D, 0xAA5D);

EXTERN_API( OSErr )
SMPGetMainEnclosureFSSpec       (WindowPtr              window,
                                 FSSpec *               enclosureDir)                       FOURWORDINLINE(0x203C, 0x0004, 0x127E, 0xAA5D);

EXTERN_API( OSErr )
SMPAddContent                   (WindowPtr              window,
                                 MailSegmentType        segmentType,
                                 Boolean                appendFlag,
                                 void *                 buffer,
                                 unsigned long          bufferSize,
                                 StScrpRec *            textScrap,
                                 Boolean                startNewScript,
                                 ScriptCode             script)                             FOURWORDINLINE(0x203C, 0x000C, 0x127A, 0xAA5D);

EXTERN_API( OSErr )
SMPReadContent                  (WindowPtr              window,
                                 MailSegmentMask        segmentTypeMask,
                                 void *                 buffer,
                                 unsigned long          bufferSize,
                                 unsigned long *        dataSize,
                                 StScrpRec *            textScrap,
                                 ScriptCode *           script,
                                 MailSegmentType *      segmentType,
                                 Boolean *              endOfScript,
                                 Boolean *              endOfSegment,
                                 Boolean *              endOfContent,
                                 long *                 segmentLength,
                                 long *                 segmentID)                          FOURWORDINLINE(0x203C, 0x0019, 0x127B, 0xAA5D);

EXTERN_API( OSErr )
SMPGetFontNameFromLetter        (WindowPtr              window,
                                 short                  fontNum,
                                 Str255                 fontName,
                                 Boolean                doneWithFontTable)                  FOURWORDINLINE(0x203C, 0x0006, 0x127C, 0xAA5D);

EXTERN_API( OSErr )
SMPAddBlock                     (WindowPtr              window,
                                 const OCECreatorType * blockType,
                                 Boolean                append,
                                 void *                 buffer,
                                 unsigned long          bufferSize,
                                 MailBlockMode          mode,
                                 unsigned long          offset)                             FOURWORDINLINE(0x203C, 0x000C, 0x127F, 0xAA5D);

EXTERN_API( OSErr )
SMPReadBlock                    (WindowPtr              window,
                                 const OCECreatorType * blockType,
                                 unsigned short         blockIndex,
                                 void *                 buffer,
                                 unsigned long          bufferSize,
                                 unsigned long          dataOffset,
                                 unsigned long *        dataSize,
                                 Boolean *              endOfBlock,
                                 unsigned long *        remaining)                          FOURWORDINLINE(0x203C, 0x0011, 0x1280, 0xAA5D);

EXTERN_API( OSErr )
SMPEnumerateBlocks              (WindowPtr              window,
                                 unsigned short         startIndex,
                                 void *                 buffer,
                                 unsigned long          bufferSize,
                                 unsigned long *        dataSize,
                                 unsigned short *       nextIndex,
                                 Boolean *              more)                               FOURWORDINLINE(0x203C, 0x000D, 0x1281, 0xAA5D);

EXTERN_API( OSErr )
SMPDrawMailer                   (WindowPtr              window)                             FOURWORDINLINE(0x203C, 0x0002, 0x1269, 0xAA5D);


EXTERN_API( OSErr )
SMPSetSubject                   (WindowPtr              window,
                                 const RString *        text)                               FOURWORDINLINE(0x203C, 0x0004, 0x126B, 0xAA5D);

EXTERN_API( OSErr )
SMPSetFromIdentity              (WindowPtr              window,
                                 AuthIdentity           from)                               FOURWORDINLINE(0x203C, 0x0004, 0x126C, 0xAA5D);

EXTERN_API( OSErr )
SMPAddAddress                   (WindowPtr              window,
                                 SMPAddressType         addrType,
                                 OCEPackedRecipient *   address)                            FOURWORDINLINE(0x203C, 0x0005, 0x126D, 0xAA5D);

EXTERN_API( OSErr )
SMPAddAttachment                (WindowPtr              window,
                                 const FSSpec *         attachment)                         FOURWORDINLINE(0x203C, 0x0004, 0x126E, 0xAA5D);

EXTERN_API( OSErr )
SMPAttachDialog                 (WindowPtr              window)                             FOURWORDINLINE(0x203C, 0x0002, 0x1276, 0xAA5D);


EXTERN_API( OSErr )
SMPExpandOrContract             (WindowPtr              window,
                                 Boolean                expand)                             FOURWORDINLINE(0x203C, 0x0003, 0x1272, 0xAA5D);


EXTERN_API( OSErr )
SMPMoveMailer                   (WindowPtr              window,
                                 short                  dh,
                                 short                  dv)                                 FOURWORDINLINE(0x203C, 0x0004, 0x126A, 0xAA5D);


EXTERN_API( OSErr )
SMPBecomeTarget                 (WindowPtr              window,
                                 Boolean                becomeTarget,
                                 SMPMailerComponent     whichField)                         FOURWORDINLINE(0x203C, 0x0005, 0x1273, 0xAA5D);


EXTERN_API( OSErr )
SMPGetComponentSize             (WindowPtr              window,
                                 unsigned short         whichMailer,
                                 SMPMailerComponent     whichField,
                                 unsigned short *       size)                               FOURWORDINLINE(0x203C, 0x0007, 0x1277, 0xAA5D);


EXTERN_API( OSErr )
SMPGetComponentInfo             (WindowPtr              window,
                                 unsigned short         whichMailer,
                                 SMPMailerComponent     whichField,
                                 void *                 buffer)                             FOURWORDINLINE(0x203C, 0x0007, 0x1278, 0xAA5D);


EXTERN_API( OSErr )
SMPGetListItemInfo              (WindowPtr              window,
                                 unsigned short         whichMailer,
                                 SMPMailerComponent     whichField,
                                 void *                 buffer,
                                 unsigned long          bufferLength,
                                 unsigned short         startItem,
                                 unsigned short *       itemCount,
                                 unsigned short *       nextItem,
                                 Boolean *              more)                               FOURWORDINLINE(0x203C, 0x0010, 0x1279, 0xAA5D);

EXTERN_API( OSErr )
SMPImage                        (WindowPtr              window,
                                 SMPDrawImageUPP        drawImageProc,
                                 long                   imageRefCon,
                                 Boolean                supportsColor)                      FOURWORDINLINE(0x203C, 0x0007, 0x1282, 0xAA5D);

EXTERN_API( OSErr )
SMPGetNextLetter                (OSType *               typesList,
                                 short                  numTypes,
                                 LetterDescriptor *     adjacentLetter)                     FOURWORDINLINE(0x203C, 0x0005, 0x1286, 0xAA5D);

EXTERN_API( OSErr )
SMPGetLetterInfo                (LetterSpec *           mailboxSpec,
                                 SMPLetterInfo *        info)                               FOURWORDINLINE(0x203C, 0x0004, 0x128A, 0xAA5D);



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

#endif /* __OCESTANDARDMAIL__ */

