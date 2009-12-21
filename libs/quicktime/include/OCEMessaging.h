/*
     File:       OCEMessaging.h
 
     Contains:   Apple Open Collaboration Environment Messaging Interfaces.
 
     Version:    Technology: AOCE Toolbox 1.02
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OCEMESSAGING__
#define __OCEMESSAGING__

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif


#ifndef __DIGITALSIGNATURE__
#include <DigitalSignature.h>
#endif

#ifndef __OCE__
#include <OCE.h>
#endif

#ifndef __OCEAUTHDIR__
#include <OCEAuthDir.h>
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

/******************************************************************************/
/* Definitions common to OCEMessaging and to OCEMail. These relate to addressing,
message ids and priorities, etc. */

/* Values of IPMPriority */
enum {
    kIPMAnyPriority             = 0,                            /* FOR FILTER ONLY */
    kIPMNormalPriority          = 1,
    kIPMLowPriority             = 2,
    kIPMHighPriority            = 3
};

typedef Byte                            IPMPriority;
/* Values of IPMAccessMode */
enum {
    kIPMAtMark                  = 0,
    kIPMFromStart               = 1,
    kIPMFromLEOM                = 2,
    kIPMFromMark                = 3
};

typedef unsigned short                  IPMAccessMode;
enum {
    kIPMUpdateMsgBit            = 4,
    kIPMNewMsgBit               = 5,
    kIPMDeleteMsgBit            = 6
};

/* Values of IPMNotificationType */
enum {
    kIPMUpdateMsgMask           = 1 << kIPMUpdateMsgBit,
    kIPMNewMsgMask              = 1 << kIPMNewMsgBit,
    kIPMDeleteMsgMask           = 1 << kIPMDeleteMsgBit
};

typedef Byte                            IPMNotificationType;
/* Values of IPMSenderTag */
enum {
    kIPMSenderRStringTag        = 0,
    kIPMSenderRecordIDTag       = 1
};

typedef unsigned short                  IPMSenderTag;
enum {
    kIPMFromDistListBit         = 0,
    kIPMDummyRecBit             = 1,
    kIPMFeedbackRecBit          = 2,                            /* should be redirected to feedback queue */
    kIPMReporterRecBit          = 3,                            /* should be redirected to reporter original queue */
    kIPMBCCRecBit               = 4                             /* this recipient is blind to all recipients of message */
};

/* Values of OCERecipientOffsetFlags */
enum {
    kIPMFromDistListMask        = 1 << kIPMFromDistListBit,
    kIPMDummyRecMask            = 1 << kIPMDummyRecBit,
    kIPMFeedbackRecMask         = 1 << kIPMFeedbackRecBit,
    kIPMReporterRecMask         = 1 << kIPMReporterRecBit,
    kIPMBCCRecMask              = 1 << kIPMBCCRecBit
};

typedef Byte                            OCERecipientOffsetFlags;

struct OCECreatorType {
    OSType                          msgCreator;
    OSType                          msgType;
};
typedef struct OCECreatorType           OCECreatorType;
enum {
    kIPMTypeWildCard            = FOUR_CHAR_CODE('ipmw'),
    kIPMFamilyUnspecified       = 0,
    kIPMFamilyWildCard          = 0x3F3F3F3F,                   /* '????' * well known signature */
    kIPMSignature               = FOUR_CHAR_CODE('ipms'),       /* base type * well known message types */
    kIPMReportNotify            = FOUR_CHAR_CODE('rptn'),       /* routing feedback * well known message block types */
    kIPMEnclosedMsgType         = FOUR_CHAR_CODE('emsg'),       /* enclosed (nested) message */
    kIPMReportInfo              = FOUR_CHAR_CODE('rpti'),       /* recipient information */
    kIPMDigitalSignature        = FOUR_CHAR_CODE('dsig')        /* digital signature */
};

/* Values of IPMMsgFormat */
enum {
    kIPMOSFormatType            = 1,
    kIPMStringFormatType        = 2
};


typedef unsigned short                  IPMMsgFormat;
typedef Str32                           IPMStringMsgType;

union TheType {
    OCECreatorType                  msgOSType;
    IPMStringMsgType                msgStrType;
};
typedef union TheType                   TheType;

struct IPMMsgType {
    IPMMsgFormat                    format;                     /* IPMMsgFormat*/
    TheType                         theType;
};
typedef struct IPMMsgType               IPMMsgType;
/*
Following are the known extension values for IPM addresses handled by Apple.
We define the definition of the entn extension below.
*/
enum {
    kOCEalanXtn                 = FOUR_CHAR_CODE('alan'),
    kOCEentnXtn                 = FOUR_CHAR_CODE('entn'),       /* entn = entity name (aka DSSpec) */
    kOCEaphnXtn                 = FOUR_CHAR_CODE('aphn')
};

/*
Following are the specific definitions for the extension for the standard
OCEMail 'entn' addresses.  [Note, the actual extension is formatted as in
IPMEntityNameExtension.]
*/
/* entn extension forms */
enum {
    kOCEAddrXtn                 = FOUR_CHAR_CODE('addr'),
    kOCEQnamXtn                 = FOUR_CHAR_CODE('qnam'),
    kOCEAttrXtn                 = FOUR_CHAR_CODE('attr'),       /* an attribute specification */
    kOCESpAtXtn                 = FOUR_CHAR_CODE('spat')        /* specific attribute */
};

/*
Following are the specific definitions for standard
OCEMail 'aphn' extension value.  

All RStrings here are packed (e.g. truncated to length) and even padded (e.g.
if length odd, then a pad byte (zero) should be introduced before the next field).

The extension value is in the packed form of the following structure:
    RString     phoneNumber;
    RString     modemType;
    Str32       queueuName;

The body of phoneNumber compound RString is in the packed form of the following structure:
    short       subType;
    RString     countryCode;                // used when subType == kOCEUseHandyDial
    RString     areaCode;                   // used when subType == kOCEUseHandyDial
    RString     phone;                      // used when subType == kOCEUseHandyDial
    RString     postFix;                    // used when subType == kOCEUseHandyDial
    RString     nonHandyDialString;         // used when subType == kOCEDontUseHandyDial
*/
/* phoneNumber sub type constants */
enum {
    kOCEUseHandyDial            = 1,
    kOCEDontUseHandyDial        = 2
};

/* FORMAT OF A PACKED FORM RECIPIENT */

struct ProtoOCEPackedRecipient {
    unsigned short                  dataLength;
};
typedef struct ProtoOCEPackedRecipient  ProtoOCEPackedRecipient;
enum {
    kOCEPackedRecipientMaxBytes = (4096 - sizeof(ProtoOCEPackedRecipient))
};


struct OCEPackedRecipient {
    unsigned short                  dataLength;
    Byte                            data[4094];
};
typedef struct OCEPackedRecipient       OCEPackedRecipient;

struct IPMEntnQueueExtension {
    Str32                           queueName;
};
typedef struct IPMEntnQueueExtension    IPMEntnQueueExtension;
/* kOCEAttrXtn */

struct IPMEntnAttributeExtension {
    AttributeType                   attributeName;
};
typedef struct IPMEntnAttributeExtension IPMEntnAttributeExtension;
/* kOCESpAtXtn */

struct IPMEntnSpecificAttributeExtension {
    AttributeCreationID             attributeCreationID;
    AttributeType                   attributeName;
};
typedef struct IPMEntnSpecificAttributeExtension IPMEntnSpecificAttributeExtension;
/* All IPM entn extensions fit within the following */

struct IPMEntityNameExtension {
    OSType                          subExtensionType;
    union {
        IPMEntnSpecificAttributeExtension  specificAttribute;
        IPMEntnAttributeExtension       attribute;
        IPMEntnQueueExtension           queue;
    }                                 u;
};
typedef struct IPMEntityNameExtension   IPMEntityNameExtension;
/* addresses with kIPMNBPXtn should specify this nbp type */
#define kIPMWSReceiverNBPType "\pMsgReceiver" 

struct IPMMsgID {
    unsigned long                   id[4];
};
typedef struct IPMMsgID                 IPMMsgID;

/* Values of IPMHeaderSelector */
enum {
    kIPMTOC                     = 0,
    kIPMSender                  = 1,
    kIPMProcessHint             = 2,
    kIPMMessageTitle            = 3,
    kIPMMessageType             = 4,
    kIPMFixedInfo               = 7
};


typedef Byte                            IPMHeaderSelector;

union TheSender {
    RString                         rString;
    PackedRecordID                  rid;
};
typedef union TheSender                 TheSender;

struct IPMSender {
    IPMSenderTag                    sendTag;
    TheSender                       theSender;
};
typedef struct IPMSender                IPMSender;
/******************************************************************************/
/* Definitions specific to OCEMessaging */

typedef unsigned long                   IPMContextRef;
typedef unsigned long                   IPMQueueRef;
typedef unsigned long                   IPMMsgRef;
typedef unsigned long                   IPMSeqNum;
typedef Str32                           IPMProcHint;
typedef Str32                           IPMQueueName;
typedef CALLBACK_API( void , IPMNoteProcPtr )(IPMQueueRef queue, IPMSeqNum seqNum, IPMNotificationType notificationType, unsigned long userData);
typedef STACK_UPP_TYPE(IPMNoteProcPtr)                          IPMNoteUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(IPMNoteUPP)
    NewIPMNoteUPP                  (IPMNoteProcPtr          userRoutine);

    EXTERN_API(void)
    DisposeIPMNoteUPP              (IPMNoteUPP              userUPP);

    EXTERN_API(void)
    InvokeIPMNoteUPP               (IPMQueueRef             queue,
                                    IPMSeqNum               seqNum,
                                    IPMNotificationType     notificationType,
                                    unsigned long           userData,
                                    IPMNoteUPP              userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppIPMNoteProcInfo = 0x000037C0 };                       /* pascal no_return_value Func(4_bytes, 4_bytes, 1_byte, 4_bytes) */
    #define NewIPMNoteUPP(userRoutine)                              (IPMNoteUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIPMNoteProcInfo, GetCurrentArchitecture())
    #define DisposeIPMNoteUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define InvokeIPMNoteUPP(queue, seqNum, notificationType, userData, userUPP)  CALL_FOUR_PARAMETER_UPP((userUPP), uppIPMNoteProcInfo, (queue), (seqNum), (notificationType), (userData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewIPMNoteProc(userRoutine)                             NewIPMNoteUPP(userRoutine)
#define CallIPMNoteProc(userRoutine, queue, seqNum, notificationType, userData) InvokeIPMNoteUPP(queue, seqNum, notificationType, userData, userRoutine)

struct IPMFixedHdrInfo {
    unsigned short                  version;
    Boolean                         authenticated;
    Boolean                         signatureEnclosed;          /*  digital signature enclosed */
    unsigned long                   msgSize;
    IPMNotificationType             notification;
    IPMPriority                     priority;
    unsigned short                  blockCount;
    unsigned short                  originalRcptCount;          /*       original number of recipients */
    unsigned long                   refCon;                     /*       Client defined data */
    unsigned short                  reserved;
    UTCTime                         creationTime;               /*       Time when it was created */
    IPMMsgID                        msgID;
    OSType                          family;                     /* family this msg belongs (e.g. mail) */
};
typedef struct IPMFixedHdrInfo          IPMFixedHdrInfo;
enum {
    kIPMDeliveryNotificationBit = 0,
    kIPMNonDeliveryNotificationBit = 1,
    kIPMEncloseOriginalBit      = 2,
    kIPMSummaryReportBit        = 3,                            /* modify enclose original to only on error */
    kIPMOriginalOnlyOnErrorBit  = 4
};

enum {
    kIPMNoNotificationMask      = 0x00,
    kIPMDeliveryNotificationMask = 1 << kIPMDeliveryNotificationBit,
    kIPMNonDeliveryNotificationMask = 1 << kIPMNonDeliveryNotificationBit,
    kIPMDontEncloseOriginalMask = 0x00,
    kIPMEncloseOriginalMask     = 1 << kIPMEncloseOriginalBit,
    kIPMImmediateReportMask     = 0x00,
    kIPMSummaryReportMask       = 1 << kIPMSummaryReportBit,
    kIPMOriginalOnlyOnErrorMask = 1 << kIPMOriginalOnlyOnErrorBit,
    kIPMEncloseOriginalOnErrorMask = (kIPMOriginalOnlyOnErrorMask | kIPMEncloseOriginalMask)
};

/* standard Non delivery codes */
enum {
    kIPMNoSuchRecipient         = 0x0001,
    kIPMRecipientMalformed      = 0x0002,
    kIPMRecipientAmbiguous      = 0x0003,
    kIPMRecipientAccessDenied   = 0x0004,
    kIPMGroupExpansionProblem   = 0x0005,
    kIPMMsgUnreadable           = 0x0006,
    kIPMMsgExpired              = 0x0007,
    kIPMMsgNoTranslatableContent = 0x0008,
    kIPMRecipientReqStdCont     = 0x0009,
    kIPMRecipientReqSnapShot    = 0x000A,
    kIPMNoTransferDiskFull      = 0x000B,
    kIPMNoTransferMsgRejectedbyDest = 0x000C,
    kIPMNoTransferMsgTooLarge   = 0x000D
};

/*************************************************************************/
/*
This is the structure that will be returned by enumerate and getmsginfo
This definition is just to give you a template, the position of msgType
is variable since this is a packed structure.  procHint and msgType are
packed and even length padded.

* master message info */

struct IPMMsgInfo {
    IPMSeqNum                       sequenceNum;
    unsigned long                   userData;
    unsigned short                  respIndex;
    SInt8                           padByte;
    IPMPriority                     priority;
    unsigned long                   msgSize;
    unsigned short                  originalRcptCount;
    unsigned short                  reserved;
    UTCTime                         creationTime;
    IPMMsgID                        msgID;
    OSType                          family;                     /* family this msg belongs (e.g. mail) */
    IPMProcHint                     procHint;
    SInt8                           filler2;
    IPMMsgType                      msgType;
};
typedef struct IPMMsgInfo               IPMMsgInfo;

typedef OCECreatorType                  IPMBlockType;

struct IPMTOC {
    IPMBlockType                    blockType;
    long                            blockOffset;
    unsigned long                   blockSize;
    unsigned long                   blockRefCon;
};
typedef struct IPMTOC                   IPMTOC;
/*
The following structure is just to describe the layout of the SingleFilter.
Each field should be packed and word aligned when passed to the IPM ToolBox.
*/

struct IPMSingleFilter {
    IPMPriority                     priority;
    SInt8                           padByte;
    OSType                          family;                     /* family this msg belongs (e.g. mail), '????' for all */
    ScriptCode                      script;                     /* Language Identifier */
    IPMProcHint                     hint;
    SInt8                           filler2;
    IPMMsgType                      msgType;
};
typedef struct IPMSingleFilter          IPMSingleFilter;

struct IPMFilter {
    unsigned short                  count;
    IPMSingleFilter                 sFilters[1];
};
typedef struct IPMFilter                IPMFilter;
/*************************************************************************
Following structures define the "start" of a recipient report block and the
elements of the array respectively.
*/

struct IPMReportBlockHeader {
    IPMMsgID                        msgID;                      /* message id of the original */
    UTCTime                         creationTime;               /* creation time of the report */
};
typedef struct IPMReportBlockHeader     IPMReportBlockHeader;

struct OCERecipientReport {
    unsigned short                  rcptIndex;                  /* index of recipient in original message */
    OSErr                           result;                     /* result of sending letter to this recipient*/
};
typedef struct OCERecipientReport       OCERecipientReport;
/*************************************************************************/
typedef union IPMParamBlock             IPMParamBlock;

typedef IPMParamBlock *                 IPMParamBlockPtr;
typedef CALLBACK_API( void , IPMIOCompletionProcPtr )(IPMParamBlockPtr paramBlock);
/*
    WARNING: IPMIOCompletionProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(IPMIOCompletionProcPtr)               IPMIOCompletionUPP;

struct IPMOpenContextPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMContextRef                   contextRef;                 /* <--  Context reference to be used in further calls*/
};
typedef struct IPMOpenContextPB         IPMOpenContextPB;

typedef IPMOpenContextPB                IPMCloseContextPB;

struct IPMCreateQueuePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    long                            filler1;
    OCERecipient *                  queue;
    AuthIdentity                    identity;                   /* used only if queue is remote */
    PackedRecordID *                owner;                      /* used only if queue is remote */
};
typedef struct IPMCreateQueuePB         IPMCreateQueuePB;
/* For createqueue and deletequeue only queue and identity are used */

typedef IPMCreateQueuePB                IPMDeleteQueuePB;

struct IPMOpenQueuePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMContextRef                   contextRef;
    OCERecipient *                  queue;
    AuthIdentity                    identity;
    IPMFilter *                     filter;
    IPMQueueRef                     newQueueRef;
    IPMNoteUPP                      notificationProc;
    unsigned long                   userData;
    IPMNotificationType             noteType;
    Byte                            padByte;
    long                            reserved;
    long                            reserved2;
};
typedef struct IPMOpenQueuePB           IPMOpenQueuePB;

struct IPMCloseQueuePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMQueueRef                     queueRef;
};
typedef struct IPMCloseQueuePB          IPMCloseQueuePB;

struct IPMEnumerateQueuePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMQueueRef                     queueRef;
    IPMSeqNum                       startSeqNum;
    Boolean                         getProcHint;
    Boolean                         getMsgType;
    short                           filler;
    IPMFilter *                     filter;
    unsigned short                  numToGet;
    unsigned short                  numGotten;
    unsigned long                   enumCount;
    void *                          enumBuffer;                 /* will be packed array of IPMMsgInfo */
    unsigned long                   actEnumCount;
};
typedef struct IPMEnumerateQueuePB      IPMEnumerateQueuePB;

typedef IPMEnumerateQueuePB             IPMChangeQueueFilterPB;

struct IPMDeleteMsgRangePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMQueueRef                     queueRef;
    IPMSeqNum                       startSeqNum;
    IPMSeqNum                       endSeqNum;
    IPMSeqNum                       lastSeqNum;
};
typedef struct IPMDeleteMsgRangePB      IPMDeleteMsgRangePB;


struct IPMOpenMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMQueueRef                     queueRef;
    IPMSeqNum                       sequenceNum;
    IPMMsgRef                       newMsgRef;
    IPMSeqNum                       actualSeqNum;
    Boolean                         exactMatch;
    Byte                            padByte;
    long                            reserved;
};
typedef struct IPMOpenMsgPB             IPMOpenMsgPB;


struct IPMOpenHFSMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    FSSpec *                        hfsPath;
    long                            filler;
    IPMMsgRef                       newMsgRef;
    long                            filler2;
    Byte                            filler3;
    Boolean                         filler4;
    long                            reserved;
};
typedef struct IPMOpenHFSMsgPB          IPMOpenHFSMsgPB;


struct IPMOpenBlockAsMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    unsigned long                   filler;
    IPMMsgRef                       newMsgRef;
    unsigned short                  filler2[7];
    unsigned short                  blockIndex;
};
typedef struct IPMOpenBlockAsMsgPB      IPMOpenBlockAsMsgPB;


struct IPMCloseMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    Boolean                         deleteMsg;
    Boolean                         filler1;
};
typedef struct IPMCloseMsgPB            IPMCloseMsgPB;


struct IPMGetMsgInfoPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    IPMMsgInfo *                    info;
};
typedef struct IPMGetMsgInfoPB          IPMGetMsgInfoPB;


struct IPMReadHeaderPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    unsigned short                  fieldSelector;
    long                            offset;
    unsigned long                   count;
    void *                          buffer;
    unsigned long                   actualCount;
    unsigned short                  filler;
    unsigned long                   remaining;
};
typedef struct IPMReadHeaderPB          IPMReadHeaderPB;


struct IPMReadRecipientPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    unsigned short                  rcptIndex;
    long                            offset;
    unsigned long                   count;
    void *                          buffer;
    unsigned long                   actualCount;
    short                           reserved;                   /* must be zero */
    unsigned long                   remaining;
    unsigned short                  originalIndex;
    OCERecipientOffsetFlags         recipientOffsetFlags;
    Boolean                         filler1;
};
typedef struct IPMReadRecipientPB       IPMReadRecipientPB;

/*
replyQueue works like recipient. [can no longer read it via ReadHeader]
OriginalIndex is meaningless, rcptFlags are used seperately and there are
currently none defined.
*/

typedef IPMReadRecipientPB              IPMReadReplyQueuePB;

struct IPMGetBlkIndexPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    IPMBlockType                    blockType;
    unsigned short                  index;
    unsigned short                  startingFrom;
    IPMBlockType                    actualBlockType;
    unsigned short                  actualBlockIndex;
};
typedef struct IPMGetBlkIndexPB         IPMGetBlkIndexPB;


struct IPMReadMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    IPMAccessMode                   mode;
    long                            offset;
    unsigned long                   count;
    void *                          buffer;
    unsigned long                   actualCount;
    unsigned short                  blockIndex;
    unsigned long                   remaining;
};
typedef struct IPMReadMsgPB             IPMReadMsgPB;

struct IPMVerifySignaturePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    SIGContextPtr                   signatureContext;
};
typedef struct IPMVerifySignaturePB     IPMVerifySignaturePB;

struct IPMNewMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    unsigned long                   filler;
    OCERecipient *                  recipient;
    OCERecipient *                  replyQueue;
    StringPtr                       procHint;
    unsigned short                  filler2;
    IPMMsgType *                    msgType;
    unsigned long                   refCon;
    IPMMsgRef                       newMsgRef;
    unsigned short                  filler3;
    long                            filler4;
    AuthIdentity                    identity;
    IPMSender *                     sender;
    unsigned long                   internalUse;
    unsigned long                   internalUse2;
};
typedef struct IPMNewMsgPB              IPMNewMsgPB;

struct IPMNewHFSMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    FSSpec *                        hfsPath;
    OCERecipient *                  recipient;
    OCERecipient *                  replyQueue;
    StringPtr                       procHint;
    unsigned short                  filler2;
    IPMMsgType *                    msgType;
    unsigned long                   refCon;
    IPMMsgRef                       newMsgRef;
    unsigned short                  filler3;
    long                            filler4;
    AuthIdentity                    identity;
    IPMSender *                     sender;
    unsigned long                   internalUse;
    unsigned long                   internalUse2;
};
typedef struct IPMNewHFSMsgPB           IPMNewHFSMsgPB;

struct IPMNestMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    unsigned short                  filler[9];
    unsigned long                   refCon;
    IPMMsgRef                       msgToNest;
    unsigned short                  filler2;
    long                            startingOffset;
};
typedef struct IPMNestMsgPB             IPMNestMsgPB;


struct IPMNewNestedMsgBlockPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    OCERecipient *                  recipient;
    OCERecipient *                  replyQueue;
    StringPtr                       procHint;
    unsigned short                  filler1;
    IPMMsgType *                    msgType;
    unsigned long                   refCon;
    IPMMsgRef                       newMsgRef;
    unsigned short                  filler2;
    long                            startingOffset;
    AuthIdentity                    identity;
    IPMSender *                     sender;
    unsigned long                   internalUse;
    unsigned long                   internalUse2;
};
typedef struct IPMNewNestedMsgBlockPB   IPMNewNestedMsgBlockPB;


struct IPMEndMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    IPMMsgID                        msgID;
    RString *                       msgTitle;
    IPMNotificationType             deliveryNotification;
    IPMPriority                     priority;
    Boolean                         cancel;
    Byte                            padByte;
    long                            reserved;
    SIGSignaturePtr                 signature;
    Size                            signatureSize;
    SIGContextPtr                   signatureContext;
                                                                /* family this msg belongs (e.g. mail) use kIPMFamilyUnspecified by default */
    OSType                          family;
};
typedef struct IPMEndMsgPB              IPMEndMsgPB;


struct IPMAddRecipientPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    OCERecipient *                  recipient;
    long                            reserved;
};
typedef struct IPMAddRecipientPB        IPMAddRecipientPB;


struct IPMAddReplyQueuePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    long                            filler;
    OCERecipient *                  replyQueue;
};
typedef struct IPMAddReplyQueuePB       IPMAddReplyQueuePB;


struct IPMNewBlockPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    IPMBlockType                    blockType;
    unsigned short                  filler[5];
    unsigned long                   refCon;
    unsigned short                  filler2[3];
    long                            startingOffset;
};
typedef struct IPMNewBlockPB            IPMNewBlockPB;


struct IPMWriteMsgPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    IPMIOCompletionUPP              ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    IPMMsgRef                       msgRef;
    IPMAccessMode                   mode;
    long                            offset;
    unsigned long                   count;
    void *                          buffer;
    unsigned long                   actualCount;
    Boolean                         currentBlock;
    Boolean                         filler1;
};
typedef struct IPMWriteMsgPB            IPMWriteMsgPB;



union IPMParamBlock {
    struct {
        void *                          qLink;
        long                            reservedH1;
        long                            reservedH2;
        IPMIOCompletionUPP              ioCompletion;
        OSErr                           ioResult;
        long                            saveA5;
        short                           reqCode;
    }                                 header;
    IPMOpenContextPB                openContextPB;
    IPMCloseContextPB               closeContextPB;
    IPMCreateQueuePB                createQueuePB;
    IPMDeleteQueuePB                deleteQueuePB;
    IPMOpenQueuePB                  openQueuePB;
    IPMCloseQueuePB                 closeQueuePB;
    IPMEnumerateQueuePB             enumerateQueuePB;
    IPMChangeQueueFilterPB          changeQueueFilterPB;
    IPMDeleteMsgRangePB             deleteMsgRangePB;
    IPMOpenMsgPB                    openMsgPB;
    IPMOpenHFSMsgPB                 openHFSMsgPB;
    IPMOpenBlockAsMsgPB             openBlockAsMsgPB;
    IPMCloseMsgPB                   closeMsgPB;
    IPMGetMsgInfoPB                 getMsgInfoPB;
    IPMReadHeaderPB                 readHeaderPB;
    IPMReadRecipientPB              readRecipientPB;
    IPMReadReplyQueuePB             readReplyQueuePB;
    IPMGetBlkIndexPB                getBlkIndexPB;
    IPMReadMsgPB                    readMsgPB;
    IPMVerifySignaturePB            verifySignaturePB;
    IPMNewMsgPB                     newMsgPB;
    IPMNewHFSMsgPB                  newHFSMsgPB;
    IPMNestMsgPB                    nestMsgPB;
    IPMNewNestedMsgBlockPB          newNestedMsgBlockPB;
    IPMEndMsgPB                     endMsgPB;
    IPMAddRecipientPB               addRecipientPB;
    IPMAddReplyQueuePB              addReplyQueuePB;
    IPMNewBlockPB                   newBlockPB;
    IPMWriteMsgPB                   writeMsgPB;
};

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(IPMIOCompletionUPP)
    NewIPMIOCompletionUPP          (IPMIOCompletionProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeIPMIOCompletionUPP      (IPMIOCompletionUPP      userUPP);

    EXTERN_API(void)
    InvokeIPMIOCompletionUPP       (IPMParamBlockPtr        paramBlock,
                                    IPMIOCompletionUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppIPMIOCompletionProcInfo = 0x00009802 };               /* register no_return_value Func(4_bytes:A0) */
    #define NewIPMIOCompletionUPP(userRoutine)                      (IPMIOCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppIPMIOCompletionProcInfo, GetCurrentArchitecture())
    #define DisposeIPMIOCompletionUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeIPMIOCompletionUPP(__A0, __A1)
    void InvokeIPMIOCompletionUPP(IPMParamBlockPtr paramBlock, IPMIOCompletionUPP userUPP) = 0x4E91;
    #else
        #define InvokeIPMIOCompletionUPP(paramBlock, userUPP)           CALL_ONE_PARAMETER_UPP((userUPP), uppIPMIOCompletionProcInfo, (paramBlock))
    #endif
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewIPMIOCompletionProc(userRoutine)                     NewIPMIOCompletionUPP(userRoutine)
#define CallIPMIOCompletionProc(userRoutine, paramBlock)        InvokeIPMIOCompletionUPP(paramBlock, userRoutine)
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
IPMOpenContext                  (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0400, 0xAA5E);

EXTERN_API( OSErr )
IPMCloseContext                 (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0401, 0xAA5E);

EXTERN_API( OSErr )
IPMNewMsg                       (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0402, 0xAA5E);

EXTERN_API( OSErr )
IPMNewBlock                     (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0404, 0xAA5E);

EXTERN_API( OSErr )
IPMNewNestedMsgBlock            (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0405, 0xAA5E);

EXTERN_API( OSErr )
IPMNestMsg                      (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0406, 0xAA5E);

EXTERN_API( OSErr )
IPMWriteMsg                     (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0407, 0xAA5E);

EXTERN_API( OSErr )
IPMEndMsg                       (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0408, 0xAA5E);

EXTERN_API( OSErr )
IPMOpenQueue                    (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0409, 0xAA5E);

EXTERN_API( OSErr )
IPMCloseQueue                   (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x040A, 0xAA5E);

/* Always synchronous */
EXTERN_API( OSErr )
IPMVerifySignature              (IPMParamBlockPtr       paramBlock)                         FIVEWORDINLINE(0x7000, 0x1F00, 0x3F3C, 0x0422, 0xAA5E);

EXTERN_API( OSErr )
IPMOpenMsg                      (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x040B, 0xAA5E);

EXTERN_API( OSErr )
IPMCloseMsg                     (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x040C, 0xAA5E);

EXTERN_API( OSErr )
IPMReadMsg                      (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x040D, 0xAA5E);

EXTERN_API( OSErr )
IPMReadHeader                   (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x040E, 0xAA5E);

EXTERN_API( OSErr )
IPMOpenBlockAsMsg               (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x040F, 0xAA5E);

EXTERN_API( OSErr )
IPMNewHFSMsg                    (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x041E, 0xAA5E);

EXTERN_API( OSErr )
IPMReadRecipient                (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0410, 0xAA5E);

EXTERN_API( OSErr )
IPMReadReplyQueue               (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0421, 0xAA5E);

EXTERN_API( OSErr )
IPMCreateQueue                  (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0411, 0xAA5E);

EXTERN_API( OSErr )
IPMDeleteQueue                  (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0412, 0xAA5E);

EXTERN_API( OSErr )
IPMEnumerateQueue               (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0413, 0xAA5E);

EXTERN_API( OSErr )
IPMChangeQueueFilter            (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0414, 0xAA5E);

EXTERN_API( OSErr )
IPMDeleteMsgRange               (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0415, 0xAA5E);

EXTERN_API( OSErr )
IPMAddRecipient                 (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0403, 0xAA5E);

EXTERN_API( OSErr )
IPMAddReplyQueue                (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x041D, 0xAA5E);

EXTERN_API( OSErr )
IPMOpenHFSMsg                   (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0417, 0xAA5E);

EXTERN_API( OSErr )
IPMGetBlkIndex                  (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0418, 0xAA5E);

EXTERN_API( OSErr )
IPMGetMsgInfo                   (IPMParamBlockPtr       paramBlock,
                                 Boolean                async)                              THREEWORDINLINE(0x3F3C, 0x0419, 0xAA5E);



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

#endif /* __OCEMESSAGING__ */

