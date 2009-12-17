/*
     File:       OCEMail.h
 
     Contains:   Apple Open Collaboration Environment OCEMail Interfaces.
 
     Version:    Technology: AOCE Toolbox 1.02
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OCEMAIL__
#define __OCEMAIL__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __TEXTEDIT__
#include <TextEdit.h>
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

#ifndef __OCEMESSAGING__
#include <OCEMessaging.h>
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

/**************************************************************************************/
/* Common Definitions */
/**************************************************************************************/

/* reference to a new or open letter or message */
typedef long                            MailMsgRef;
/* reference to an open msam queue */
typedef long                            MSAMQueueRef;
/* identifies slots managed by a PMSAM */
typedef unsigned short                  MSAMSlotID;
/* reference to an active mailbox */
typedef long                            MailboxRef;
/* identifies slots within a mailbox */
typedef unsigned short                  MailSlotID;
/* identifies a letter in a mailbox */

struct MailSeqNum {
    MailSlotID                      slotID;
    long                            seqNum;
};
typedef struct MailSeqNum               MailSeqNum;
/* A MailBuffer is used to describe a buffer used for an IO operation.
The location of the buffer is pointed to by 'buffer'. 
When reading, the size of the buffer is 'bufferSize' 
and the size of data actually read is 'dataSize'.
When writing, the size of data to be written is 'bufferSize' 
and the size of data actually written is 'dataSize'.
*/

struct MailBuffer {
    long                            bufferSize;
    void *                          buffer;
    long                            dataSize;
};
typedef struct MailBuffer               MailBuffer;
/* A MailReply is used to describe a commonly used reply buffer format.
It contains a count of tuples followed by an array of tuples.
The format of the tuple itself depends on each particular call.
*/

struct MailReply {
    unsigned short                  tupleCount;
                                                                /* tuple[tupleCount] */
};
typedef struct MailReply                MailReply;
/* Shared Memory Communication Area used when Mail Manager sends 
High Level Events to a PMSAM. 
*/

struct SMCA {
    unsigned short                  smcaLength;                 /* includes size of smcaLength field */
    OSErr                           result;
    long                            userBytes;
    union {
        CreationID                      slotCID;                /* for create/modify/delete slot calls */
        long                            msgHint;                /* for kMailEPPCMsgOpened */
    } u;
};
typedef struct SMCA                     SMCA;
/**************************************************************************************/
/* Value of creator and types fields for messages and blocks defined by MailManager */
enum {
    kMailAppleMailCreator       = FOUR_CHAR_CODE('apml'),       /* message and letter block creator */
    kMailLtrMsgType             = FOUR_CHAR_CODE('lttr'),       /* message type of letters, reports */
    kMailLtrHdrType             = FOUR_CHAR_CODE('lthd'),       /* contains letter header */
    kMailContentType            = FOUR_CHAR_CODE('body'),       /* contains content of letter */
    kMailEnclosureListType      = FOUR_CHAR_CODE('elst'),       /* contains list of enclosures */
    kMailEnclosureDesktopType   = FOUR_CHAR_CODE('edsk'),       /* contains desktop mgr info for enclosures */
    kMailEnclosureFileType      = FOUR_CHAR_CODE('asgl'),       /* contains a file enclosure, format is defined by AppleSingle */
    kMailImageBodyType          = FOUR_CHAR_CODE('imag'),       /* contains image of letter */
    kMailMSAMType               = FOUR_CHAR_CODE('gwyi'),       /* contains msam specific information */
    kMailTunnelLtrType          = FOUR_CHAR_CODE('tunl'),       /* used to read a tunnelled message */
    kMailHopInfoType            = FOUR_CHAR_CODE('hopi'),       /* used to read hopInfo for a tunnelled message */
    kMailReportType             = FOUR_CHAR_CODE('rpti'),       /* contains report info */
    kMailFamily                 = FOUR_CHAR_CODE('mail'),       /* Defines family of "mail" msgs: content, header, etc */
    kMailFamilyFile             = FOUR_CHAR_CODE('file')        /* Defines family of "direct display" msgs */
};

/*    
kMailImageBodyType:
    format is struct TPfPgDir - in Printing.h
    *   struct TPfPgDir {
    *       short   pageCount;      - number of pages in the image.
    *       long    iPgPos[129];    - iPgPos[n] is the offset from the start of the block
    *                               - to image of page n.
    *                               - iPgPos[n+1] - iPgPos[n] is the length of page n.

kMailReportType:
Reports have the isReport bit set in MailIndications and contain a block of type kMailReport.
This block has a header, IPMReportBlockHeader,
followed by an array of elements, each of type IPMRecipientReport

Various families used by mail or related msgs
*/


/**************************************************************************************/

typedef unsigned short                  MailAttributeID;
/* Values of MailAttributeID */
/* Message store attributes - stored in the catalog */
/* Will always be present in a letter and have fixed sizes */
enum {
    kMailLetterFlagsBit         = 1,                            /* MailLetterFlags */
                                                                /* Letter attributes - stored in the letter will always be present in a letter and have fixed sizes */
    kMailIndicationsBit         = 3,                            /* MailIndications */
    kMailMsgTypeBit             = 4,                            /* OCECreatorType */
    kMailLetterIDBit            = 5,                            /* MailLetterID */
    kMailSendTimeStampBit       = 6,                            /* MailTime */
    kMailNestingLevelBit        = 7,                            /* MailNestingLevel */
    kMailMsgFamilyBit           = 8,                            /* OSType */
                                                                /* Letter attributes - stored in the letter may be present in a letter and have fixed sizes */
    kMailReplyIDBit             = 9,                            /* MailLetterID */
    kMailConversationIDBit      = 10,                           /* MailLetterID */
                                                                /* Letter attributes - stored in the letter may be present in a letter and have variable length sizes */
    kMailSubjectBit             = 11,                           /* RString */
    kMailFromBit                = 12,                           /* MailRecipient */
    kMailToBit                  = 13,                           /* MailRecipient */
    kMailCcBit                  = 14,                           /* MailRecipient */
    kMailBccBit                 = 15                            /* MailRecipient */
};

typedef unsigned long                   MailAttributeMask;
/* Values of MailAttributeMask */
enum {
    kMailLetterFlagsMask        = 1L << (kMailLetterFlagsBit - 1),
    kMailIndicationsMask        = 1L << (kMailIndicationsBit - 1),
    kMailMsgTypeMask            = 1L << (kMailMsgTypeBit - 1),
    kMailLetterIDMask           = 1L << (kMailLetterIDBit - 1),
    kMailSendTimeStampMask      = 1L << (kMailSendTimeStampBit - 1),
    kMailNestingLevelMask       = 1L << (kMailNestingLevelBit - 1),
    kMailMsgFamilyMask          = 1L << (kMailMsgFamilyBit - 1),
    kMailReplyIDMask            = 1L << (kMailReplyIDBit - 1),
    kMailConversationIDMask     = 1L << (kMailConversationIDBit - 1),
    kMailSubjectMask            = 1L << (kMailSubjectBit - 1),
    kMailFromMask               = 1L << (kMailFromBit - 1),
    kMailToMask                 = 1L << (kMailToBit - 1),
    kMailCcMask                 = 1L << (kMailCcBit - 1),
    kMailBccMask                = 1L << (kMailBccBit - 1)
};

typedef unsigned long                   MailAttributeBitmap;
/**************************************************************************************/
typedef unsigned short                  MailLetterSystemFlags;
/* Values of MailLetterSystemFlags */
/* letter is available locally (either by nature or via cache) */
enum {
    kMailIsLocalBit             = 2
};

enum {
    kMailIsLocalMask            = 1L << kMailIsLocalBit
};

typedef unsigned short                  MailLetterUserFlags;
enum {
    kMailReadBit                = 0,                            /* this letter has been opened */
    kMailDontArchiveBit         = 1,                            /* this letter is not to be archived either because it has already been archived or it should not be archived. */
    kMailInTrashBit             = 2                             /* this letter is in trash */
};

/* Values of MailLetterUserFlags */
enum {
    kMailReadMask               = 1L << kMailReadBit,
    kMailDontArchiveMask        = 1L << kMailDontArchiveBit,
    kMailInTrashMask            = 1L << kMailInTrashBit
};


struct MailLetterFlags {
    MailLetterSystemFlags           sysFlags;
    MailLetterUserFlags             userFlags;
};
typedef struct MailLetterFlags          MailLetterFlags;

struct MailMaskedLetterFlags {
    MailLetterFlags                 flagMask;                   /* flags that are to be set */
    MailLetterFlags                 flagValues;                 /* and their values */
};
typedef struct MailMaskedLetterFlags    MailMaskedLetterFlags;
enum {
    kMailOriginalInReportBit    = 1,
    kMailNonReceiptReportsBit   = 3,
    kMailReceiptReportsBit      = 4,
    kMailForwardedBit           = 5,
    kMailPriorityBit            = 6,
    kMailIsReportWithOriginalBit = 8,
    kMailIsReportBit            = 9,
    kMailHasContentBit          = 10,
    kMailHasSignatureBit        = 11,
    kMailAuthenticatedBit       = 12,
    kMailSentBit                = 13,
    kMailNativeContentBit       = 14,
    kMailImageContentBit        = 15,
    kMailStandardContentBit     = 16
};

/* Values of MailIndications */
enum {
    kMailStandardContentMask    = 1L << (kMailStandardContentBit - 1),
    kMailImageContentMask       = 1L << (kMailImageContentBit - 1),
    kMailNativeContentMask      = 1L << (kMailNativeContentBit - 1),
    kMailSentMask               = 1L << (kMailSentBit - 1),
    kMailAuthenticatedMask      = 1L << (kMailAuthenticatedBit - 1),
    kMailHasSignatureMask       = 1L << (kMailHasSignatureBit - 1),
    kMailHasContentMask         = 1L << (kMailHasContentBit - 1),
    kMailIsReportMask           = 1L << (kMailIsReportBit - 1),
    kMailIsReportWithOriginalMask = 1L << (kMailIsReportWithOriginalBit - 1),
    kMailPriorityMask           = 3L << (kMailPriorityBit - 1),
    kMailForwardedMask          = 1L << (kMailForwardedBit - 1),
    kMailReceiptReportsMask     = 1L << (kMailReceiptReportsBit - 1),
    kMailNonReceiptReportsMask  = 1L << (kMailNonReceiptReportsBit - 1),
    kMailOriginalInReportMask   = 3L << (kMailOriginalInReportBit - 1)
};


typedef unsigned long                   MailIndications;
/* values of the field originalInReport in MailIndications */
enum {
    kMailNoOriginal             = 0,                            /* do not enclose original in reports */
    kMailEncloseOnNonReceipt    = 3                             /* enclose original in non-delivery reports */
};

typedef IPMMsgID                        MailLetterID;

struct MailTime {
    UTCTime                         time;                       /* current UTC(GMT) time */
    UTCOffset                       offset;                     /* offset from GMT */
};
typedef struct MailTime                 MailTime;
/* innermost letter has nestingLevel 0 */

typedef unsigned short                  MailNestingLevel;
typedef OCERecipient                    MailRecipient;
/**************************************************************************************/
enum {
    kMailTextSegmentBit         = 0,
    kMailPictSegmentBit         = 1,
    kMailSoundSegmentBit        = 2,
    kMailStyledTextSegmentBit   = 3,
    kMailMovieSegmentBit        = 4
};

typedef unsigned short                  MailSegmentMask;
/* Values of MailSegmentMask */
enum {
    kMailTextSegmentMask        = 1L << kMailTextSegmentBit,
    kMailPictSegmentMask        = 1L << kMailPictSegmentBit,
    kMailSoundSegmentMask       = 1L << kMailSoundSegmentBit,
    kMailStyledTextSegmentMask  = 1L << kMailStyledTextSegmentBit,
    kMailMovieSegmentMask       = 1L << kMailMovieSegmentBit
};

typedef unsigned short                  MailSegmentType;
/* Values of MailSegmentType */
enum {
    kMailInvalidSegmentType     = 0,
    kMailTextSegmentType        = 1,
    kMailPictSegmentType        = 2,
    kMailSoundSegmentType       = 3,
    kMailStyledTextSegmentType  = 4,
    kMailMovieSegmentType       = 5
};

/**************************************************************************************/
enum {
    kMailErrorLogEntryVersion   = 0x0101,
    kMailMSAMErrorStringListID  = 128,                          /* These 'STR#' resources should be */
    kMailMSAMActionStringListID = 129                           /* in the PMSAM resource fork */
};

typedef unsigned short                  MailLogErrorType;
/* Values of MailLogErrorType */
enum {
    kMailELECorrectable         = 0,
    kMailELEError               = 1,
    kMailELEWarning             = 2,
    kMailELEInformational       = 3
};

typedef short                           MailLogErrorCode;
/* Values of MailLogErrorCode */
enum {
    kMailMSAMErrorCode          = 0,                            /* positive codes are indices into PMSAM defined strings */
    kMailMiscError              = -1,                           /* negative codes are OCE defined */
    kMailNoModem                = -2                            /* modem required, but missing */
};


struct MailErrorLogEntryInfo {
    short                           version;
    UTCTime                         timeOccurred;               /* do not fill in */
    Str31                           reportingPMSAM;             /* do not fill in */
    Str31                           reportingMSAMSlot;          /* do not fill in */
    MailLogErrorType                errorType;
    MailLogErrorCode                errorCode;

    short                           errorResource;              /* resources are valid if */
    short                           actionResource;             /* errorCode = kMailMSAMErrorCode index starts from 1 */
    unsigned long                   filler;
    unsigned short                  filler2;
};
typedef struct MailErrorLogEntryInfo    MailErrorLogEntryInfo;
/**************************************************************************************/

typedef short                           MailBlockMode;
/* Values of MailBlockMode */
enum {
    kMailFromStart              = 1,                            /* write data from offset calculated from */
    kMailFromLEOB               = 2,                            /* start of block, end of block, */
    kMailFromMark               = 3                             /* or from the current mark */
};


struct MailEnclosureInfo {
    StringPtr                       enclosureName;
    CInfoPBPtr                      catInfo;
    StringPtr                       comment;
    void *                          icon;
};
typedef struct MailEnclosureInfo        MailEnclosureInfo;
/**************************************************************************************/
enum {
    kOCESetupLocationNone       = 0,                            /* disconnect state */
    kOCESetupLocationMax        = 8                             /* maximum location value */
};


typedef char                            OCESetupLocation;
/* location state is a bitmask, 0x1=>1st location active, 
 * 0x2 => 2nd, 0x4 => 3rd, etc.
 */
#define MailLocationMask(locationNumber) (1<<((locationNumber)-1))
typedef UInt8                           MailLocationFlags;

struct MailLocationInfo {
    OCESetupLocation                location;
    MailLocationFlags               active;
};
typedef struct MailLocationInfo         MailLocationInfo;
/**************************************************************************************/
/* Definitions for Personal MSAMs */
/**************************************************************************************/
enum {
    kMailEPPCMsgVersion         = 3
};


struct MailEPPCMsg {
    short                           version;
    union {
        SMCA *                          theSMCA;                /* for 'crsl', 'mdsl', 'dlsl', 'sndi', 'msgo', 'admn' */
        long                            sequenceNumber;         /* for 'inqu', 'dlom' */
        MailLocationInfo                locationInfo;           /* for 'locc' */
    }                                 u;
};
typedef struct MailEPPCMsg              MailEPPCMsg;
/* Values of OCE defined High Level Event message classes */
enum {
    kMailEPPCCreateSlot         = FOUR_CHAR_CODE('crsl'),
    kMailEPPCModifySlot         = FOUR_CHAR_CODE('mdsl'),
    kMailEPPCDeleteSlot         = FOUR_CHAR_CODE('dlsl'),
    kMailEPPCShutDown           = FOUR_CHAR_CODE('quit'),
    kMailEPPCMailboxOpened      = FOUR_CHAR_CODE('mbop'),
    kMailEPPCMailboxClosed      = FOUR_CHAR_CODE('mbcl'),
    kMailEPPCMsgPending         = FOUR_CHAR_CODE('msgp'),
    kMailEPPCSendImmediate      = FOUR_CHAR_CODE('sndi'),
    kMailEPPCContinue           = FOUR_CHAR_CODE('cont'),
    kMailEPPCSchedule           = FOUR_CHAR_CODE('sked'),
    kMailEPPCAdmin              = FOUR_CHAR_CODE('admn'),
    kMailEPPCInQUpdate          = FOUR_CHAR_CODE('inqu'),
    kMailEPPCMsgOpened          = FOUR_CHAR_CODE('msgo'),
    kMailEPPCDeleteOutQMsg      = FOUR_CHAR_CODE('dlom'),
    kMailEPPCWakeup             = FOUR_CHAR_CODE('wkup'),
    kMailEPPCLocationChanged    = FOUR_CHAR_CODE('locc')
};


union MailTimer {
    long                            frequency;                  /* how often to connect */
    long                            connectTime;                /* time since midnight */
};
typedef union MailTimer                 MailTimer;
enum {
    kMailTimerOff               = 0,                            /* control is off */
    kMailTimerTime              = 1,                            /* specifies connect time (relative to midnight) */
    kMailTimerFrequency         = 2                             /* specifies connect frequency */
};

typedef Byte                            MailTimerKind;

struct MailTimers {
    MailTimerKind                   sendTimeKind;               /* either kMailTimerTime or kMailTimerFrequency */
    MailTimerKind                   receiveTimeKind;            /* either kMailTimerTime or kMailTimerFrequency */
    MailTimer                       send;
    MailTimer                       receive;
};
typedef struct MailTimers               MailTimers;


struct MailStandardSlotInfoAttribute {
    short                           version;
    MailLocationFlags               active;                     /* active if MailLocationMask(i) is set */
    Byte                            padByte;
    MailTimers                      sendReceiveTimer;
};
typedef struct MailStandardSlotInfoAttribute MailStandardSlotInfoAttribute;
typedef union MSAMParam TempMSAMParam;
typedef CALLBACK_API( void , MSAMIOCompletionProcPtr )(TempMSAMParam *paramBlock);
/*
    WARNING: MSAMIOCompletionProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(MSAMIOCompletionProcPtr)              MSAMIOCompletionUPP;

struct PMSAMGetMSAMRecordPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    CreationID                      msamCID;
};
typedef struct PMSAMGetMSAMRecordPB     PMSAMGetMSAMRecordPB;

struct PMSAMOpenQueuesPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    inQueueRef;
    MSAMQueueRef                    outQueueRef;
    MSAMSlotID                      msamSlotID;
    long                            filler[2];
};
typedef struct PMSAMOpenQueuesPB        PMSAMOpenQueuesPB;

typedef unsigned short                  PMSAMStatus;
/* Values of PMSAMStatus */
enum {
    kPMSAMStatusPending         = 1,                            /* for inQueue and outQueue */
    kPMSAMStatusError           = 2,                            /* for inQueue and outQueue */
    kPMSAMStatusSending         = 3,                            /* for outQueue only */
    kPMSAMStatusCaching         = 4,                            /* for inQueue only */
    kPMSAMStatusSent            = 5                             /* for outQueue only */
};


struct PMSAMSetStatusPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    queueRef;
    long                            seqNum;
    long                            msgHint;                    /* for posting cache error,set this to 0 when report outq status */
    PMSAMStatus                     status;
};
typedef struct PMSAMSetStatusPB         PMSAMSetStatusPB;

struct PMSAMLogErrorPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMSlotID                      msamSlotID;                 /* 0 for PMSAM errors */
    MailErrorLogEntryInfo *         logEntry;
    long                            filler[2];
};
typedef struct PMSAMLogErrorPB          PMSAMLogErrorPB;
/****************************************************************************************/
enum {
    kMailMsgSummaryVersion      = 1
};


struct MailMasterData {
    MailAttributeBitmap             attrMask;                   /* indicates attributes present in MsgSummary */
    MailLetterID                    messageID;
    MailLetterID                    replyID;
    MailLetterID                    conversationID;
};
typedef struct MailMasterData           MailMasterData;
/* Values for addressedToMe in struct MailCoreData */
enum {
    kAddressedAs_TO             = 0x01,
    kAddressedAs_CC             = 0x02,
    kAddressedAs_BCC            = 0x04
};


struct MailCoreData {
    MailLetterFlags                 letterFlags;
    unsigned long                   messageSize;
    MailIndications                 letterIndications;
    OCECreatorType                  messageType;
    MailTime                        sendTime;
    OSType                          messageFamily;
    unsigned char                   reserved;
    unsigned char                   addressedToMe;
    char                            agentInfo[6];               /* 6 bytes of special info [set to zero] */
                                                                /* these are variable length and even padded */
    RString32                       sender;                     /* recipient's entityName (trunc)*/
    RString32                       subject;                    /* subject maybe truncated */
};
typedef struct MailCoreData             MailCoreData;

struct MSAMMsgSummary {
    short                           version;                    /* following flags are defaulted by Toolbox */
    Boolean                         msgDeleted;                 /* true if msg is to be deleted by PMSAM */
    Boolean                         msgUpdated;                 /* true if msgSummary was updated by MailManager */
    Boolean                         msgCached;                  /* true if msg is in the slot's InQueue */
    Byte                            padByte;

    MailMasterData                  masterData;
    MailCoreData                    coreData;
};
typedef struct MSAMMsgSummary           MSAMMsgSummary;
/* PMSAM can put up to 128 bytes of private msg summary data */
enum {
    kMailMaxPMSAMMsgSummaryData = 128
};


struct PMSAMCreateMsgSummaryPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    inQueueRef;
    long                            seqNum;                     /* <- seq of the new message */
    MSAMMsgSummary *                msgSummary;                 /* attributes and mask filled in */
    MailBuffer *                    buffer;                     /* PMSAM specific data to be appended */
};
typedef struct PMSAMCreateMsgSummaryPB  PMSAMCreateMsgSummaryPB;


struct PMSAMPutMsgSummaryPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    inQueueRef;
    long                            seqNum;
    MailMaskedLetterFlags *         letterFlags;                /* if not nil, then set msgStoreFlags */
    MailBuffer *                    buffer;                     /* PMSAM specific data to be overwritten */
};
typedef struct PMSAMPutMsgSummaryPB     PMSAMPutMsgSummaryPB;

struct PMSAMGetMsgSummaryPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    inQueueRef;
    long                            seqNum;
    MSAMMsgSummary *                msgSummary;                 /* if not nil, then read in the msgSummary */
    MailBuffer *                    buffer;                     /* PMSAM specific data to be read */
    unsigned short                  msgSummaryOffset;           /* offset of PMSAM specific data from start of MsgSummary */
};
typedef struct PMSAMGetMsgSummaryPB     PMSAMGetMsgSummaryPB;
/****************************************************************************************/
/* Definitions for Server MSAMs */
/**************************************************************************************/

typedef unsigned short                  SMSAMAdminCode;
/* Values of SMSAMAdminCode */
enum {
    kSMSAMNotifyFwdrSetupChange = 1,
    kSMSAMNotifyFwdrNameChange  = 2,
    kSMSAMNotifyFwdrPwdChange   = 3,
    kSMSAMGetDynamicFwdrParams  = 4
};

typedef unsigned long                   SMSAMSlotChanges;
enum {
    kSMSAMFwdrHomeInternetChangedBit = 0,
    kSMSAMFwdrConnectedToChangedBit = 1,
    kSMSAMFwdrForeignRLIsChangedBit = 2,
    kSMSAMFwdrMnMServerChangedBit = 3
};

/* Values of SMSAMSlotChanges */
enum {
    kSMSAMFwdrEverythingChangedMask = -1,
    kSMSAMFwdrHomeInternetChangedMask = 1L << kSMSAMFwdrHomeInternetChangedBit,
    kSMSAMFwdrConnectedToChangedMask = 1L << kSMSAMFwdrConnectedToChangedBit,
    kSMSAMFwdrForeignRLIsChangedMask = 1L << kSMSAMFwdrForeignRLIsChangedBit,
    kSMSAMFwdrMnMServerChangedMask = 1L << kSMSAMFwdrMnMServerChangedBit
};

/* kSMSAMNotifyFwdrSetupChange */

struct SMSAMSetupChange {
    SMSAMSlotChanges                whatChanged;                /*  --> bitmap of what parameters changed */
    AddrBlock                       serverHint;                 /*  --> try this ADAP server first */
};
typedef struct SMSAMSetupChange         SMSAMSetupChange;
/* kSMSAMNotifyFwdrNameChange */

struct SMSAMNameChange {
    RString                         newName;                    /*  --> msams new name */
    AddrBlock                       serverHint;                 /*  --> try this ADAP server first */
};
typedef struct SMSAMNameChange          SMSAMNameChange;
/* kSMSAMNotifyFwdrPasswordChange */

struct SMSAMPasswordChange {
    RString                         newPassword;                /*  --> msams new password */
    AddrBlock                       serverHint;                 /*  --> try this ADAP server first */
};
typedef struct SMSAMPasswordChange      SMSAMPasswordChange;
/* kSMSAMGetDynamicFwdrParams */

struct SMSAMDynamicParams {
    unsigned long                   curDiskUsed;                /* <--  amount of disk space used by msam */
    unsigned long                   curMemoryUsed;              /* <--  amount of memory used by msam */
};
typedef struct SMSAMDynamicParams       SMSAMDynamicParams;

struct SMSAMAdminEPPCRequest {
    SMSAMAdminCode                  adminCode;
    union {
        SMSAMSetupChange                setupChange;
        SMSAMNameChange                 nameChange;
        SMSAMPasswordChange             passwordChange;
        SMSAMDynamicParams              dynamicParams;
    }                                 u;
};
typedef struct SMSAMAdminEPPCRequest    SMSAMAdminEPPCRequest;

struct SMSAMSetupPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    RecordIDPtr                     serverMSAM;
    RStringPtr                      password;
    OSType                          gatewayType;
    RStringPtr                      gatewayTypeDescription;
    AddrBlock                       catalogServerHint;
};
typedef struct SMSAMSetupPB             SMSAMSetupPB;

struct SMSAMStartupPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    AuthIdentity                    msamIdentity;
    MSAMQueueRef                    queueRef;
};
typedef struct SMSAMStartupPB           SMSAMStartupPB;

struct SMSAMShutdownPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    queueRef;
};
typedef struct SMSAMShutdownPB          SMSAMShutdownPB;
/****************************************************************************************/
/* Definitions for reading and writing MSAM Letters */
/****************************************************************************************/

struct MSAMEnumeratePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    queueRef;
    long                            startSeqNum;
    long                            nextSeqNum;
                                                                /* buffer contains a Mail Reply. Each tuple is a MSAMEnumerateInQReply when enumerating the inQueue MSAMEnumerateOutQReply when enumerating the outQueue */
    MailBuffer                      buffer;
};
typedef struct MSAMEnumeratePB          MSAMEnumeratePB;

struct MSAMEnumerateInQReply {
    long                            seqNum;
    Boolean                         msgDeleted;                 /* true if msg is to be deleted by PMSAM */
    Boolean                         msgUpdated;                 /* true if MsgSummary has been updated by TB */
    Boolean                         msgCached;                  /* true if msg is in the incoming queue */
    Byte                            padByte;
};
typedef struct MSAMEnumerateInQReply    MSAMEnumerateInQReply;

struct MSAMEnumerateOutQReply {
    long                            seqNum;
    Boolean                         done;                       /* true if all responsible recipients have been processed */
    IPMPriority                     priority;
    OSType                          msgFamily;
    long                            approxSize;
    Boolean                         tunnelForm;                 /* true if this letter has to be tunnelled */
    Byte                            padByte;
    NetworkSpec                     nextHop;                    /* valid if tunnelForm is true */
    OCECreatorType                  msgType;
};
typedef struct MSAMEnumerateOutQReply   MSAMEnumerateOutQReply;

struct MSAMDeletePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    queueRef;
    long                            seqNum;
    Boolean                         msgOnly;                    /* only valid for PMSAM & inQueue */
                                                                /* set true to delete message but not msgSummary */
    Byte                            padByte;
                                                                /* only valid for SMSAM & tunnelled messages */
    OSErr                           result;
};
typedef struct MSAMDeletePB             MSAMDeletePB;

struct MSAMOpenPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    queueRef;
    long                            seqNum;
    MailMsgRef                      mailMsgRef;
};
typedef struct MSAMOpenPB               MSAMOpenPB;

struct MSAMOpenNestedPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    MailMsgRef                      nestedRef;
};
typedef struct MSAMOpenNestedPB         MSAMOpenNestedPB;

struct MSAMClosePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
};
typedef struct MSAMClosePB              MSAMClosePB;

struct MSAMGetMsgHeaderPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    IPMHeaderSelector               selector;
    Boolean                         filler1;
    unsigned long                   offset;
    MailBuffer                      buffer;
    unsigned long                   remaining;
};
typedef struct MSAMGetMsgHeaderPB       MSAMGetMsgHeaderPB;
/*    MSAMGetAttributesPB.buffer returned will contain the attribute values of 
    the attributes indicated in responseMask, 
    from the attribute indicated by the least significant bit set
    to the attribute indicated by the most significant bit set.
    Note that recipients - from, to, cc, bcc cannot be read using
    this call. Use GetRecipients to read these. 
*/

struct MSAMGetAttributesPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    MailAttributeBitmap             requestMask;                /* kMailIndicationsBit thru kMailSubjectBit */
    MailBuffer                      buffer;
    MailAttributeBitmap             responseMask;
    Boolean                         more;
    Boolean                         filler1;
};
typedef struct MSAMGetAttributesPB      MSAMGetAttributesPB;
/* attrID value to get resolved recipient list */
enum {
    kMailResolvedList           = 0
};


struct MailOriginalRecipient {
    short                           index;
};
typedef struct MailOriginalRecipient    MailOriginalRecipient;
/* Followed by OCEPackedRecipient */



struct MailResolvedRecipient {
    short                           index;
    short                           recipientFlags;
    Boolean                         responsible;
    Byte                            padByte;
};
typedef struct MailResolvedRecipient    MailResolvedRecipient;
/* Followed by OCEPackedRecipient */


/*    MSAMGetRecipientsPB.buffer contains a Mail Reply. Each tuple is a
    MailOriginalRecipient if getting original recipients 
                            ie the attrID is kMail[From, To, Cc, Bcc]Bit
    MailResolvedRecipient if getting resolved reicpients
                            ie the attrID is kMailResolvedList
    Both tuples are word alligned.  
*/

struct MSAMGetRecipientsPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    MailAttributeID                 attrID;                     /* kMailFromBit thru kMailBccBit */
    unsigned short                  startIndex;                 /* starts at 1 */
    MailBuffer                      buffer;
    unsigned short                  nextIndex;
    Boolean                         more;
    Boolean                         filler1;
};
typedef struct MSAMGetRecipientsPB      MSAMGetRecipientsPB;

struct MSAMGetContentPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    MailSegmentMask                 segmentMask;
    MailBuffer                      buffer;
    StScrpRec *                     textScrap;
    ScriptCode                      script;
    MailSegmentType                 segmentType;
    Boolean                         endOfScript;
    Boolean                         endOfSegment;
    Boolean                         endOfContent;
    Boolean                         filler1;
    long                            segmentLength;              /* NEW: <-  valid first call in a segment */
    long                            segmentID;                  /* NEW: <-> identifier for this segment */
};
typedef struct MSAMGetContentPB         MSAMGetContentPB;

struct MSAMGetEnclosurePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    Boolean                         contentEnclosure;
    Byte                            padByte;
    MailBuffer                      buffer;
    Boolean                         endOfFile;
    Boolean                         endOfEnclosures;
};
typedef struct MSAMGetEnclosurePB       MSAMGetEnclosurePB;

struct MailBlockInfo {
    OCECreatorType                  blockType;
    unsigned long                   offset;
    unsigned long                   blockLength;
};
typedef struct MailBlockInfo            MailBlockInfo;

struct MSAMEnumerateBlocksPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    unsigned short                  startIndex;                 /* starts at 1 */
    MailBuffer                      buffer;
                                                                /*    buffer contains a Mail Reply. Each tuple is a MailBlockInfo */
    unsigned short                  nextIndex;
    Boolean                         more;
    Boolean                         filler1;
};
typedef struct MSAMEnumerateBlocksPB    MSAMEnumerateBlocksPB;

struct MSAMGetBlockPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    OCECreatorType                  blockType;
    unsigned short                  blockIndex;
    MailBuffer                      buffer;
    unsigned long                   dataOffset;
    Boolean                         endOfBlock;
    Byte                            padByte;
    unsigned long                   remaining;
};
typedef struct MSAMGetBlockPB           MSAMGetBlockPB;
/* YOU SHOULD BE USING THE NEW FORM OF MARK RECIPIENTS
 * THIS VERSION IS MUCH SLOWER AND KEPT FOR COMPATIBILITY
 * REASONS.
*/
/* not valid for tunnel form letters */

struct MSAMMarkRecipientsPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    queueRef;
    long                            seqNum;
    MailBuffer                      buffer;                     /*    buffer contains a Mail Reply. Each tuple is an unsigned short, the index of a recipient to be marked. */

};
typedef struct MSAMMarkRecipientsPB     MSAMMarkRecipientsPB;
/* 
 * same as MSAMMarkRecipients except it takes a mailMsgRef instead of 
 * queueRef, seqNum 
*/
/* not valid for tunnel form letters */

struct MSAMnMarkRecipientsPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    MailBuffer                      buffer;                     /*    buffer contains a Mail Reply. Each tuple is an unsigned short, the index of a recipient to be marked. */

};
typedef struct MSAMnMarkRecipientsPB    MSAMnMarkRecipientsPB;
/****************************************************************************************/

struct MSAMCreatePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    queueRef;
    Boolean                         asLetter;                   /* indicate if we should create as letter or msg */
    Boolean                         filler1;
    IPMMsgType                      msgType;                    /* up to application discretion: must be of IPMSenderTag  kIPMOSFormatType for asLetter=true */
    long                            refCon;                     /* for messages only */
    long                            seqNum;                     /* set if creating message in the inQueue */
    Boolean                         tunnelForm;                 /* if true tunnelForm else newForm */
    Boolean                         bccRecipients;              /* true if creating letter with bcc recipients */
    MailMsgRef                      newRef;
};
typedef struct MSAMCreatePB             MSAMCreatePB;

struct MSAMBeginNestedPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    long                            refCon;                     /* for messages only */
    IPMMsgType                      msgType;
};
typedef struct MSAMBeginNestedPB        MSAMBeginNestedPB;

struct MSAMEndNestedPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
};
typedef struct MSAMEndNestedPB          MSAMEndNestedPB;

struct MSAMSubmitPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    Boolean                         submitFlag;
    Byte                            padByte;
    MailLetterID                    msgID;
};
typedef struct MSAMSubmitPB             MSAMSubmitPB;

struct MSAMPutMsgHeaderPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    OCERecipient *                  replyQueue;
    IPMSender *                     sender;
    IPMNotificationType             deliveryNotification;
    IPMPriority                     priority;
};
typedef struct MSAMPutMsgHeaderPB       MSAMPutMsgHeaderPB;

struct MSAMPutAttributePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    MailAttributeID                 attrID;                     /* kMailIndicationsBit thru kMailSubjectBit */
    MailBuffer                      buffer;
};
typedef struct MSAMPutAttributePB       MSAMPutAttributePB;

struct MSAMPutRecipientPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    MailAttributeID                 attrID;                     /* kMailFromBit thru kMailBccBit */
    MailRecipient *                 recipient;
    Boolean                         responsible;                /* valid for server and message msams only */
    Boolean                         filler1;
};
typedef struct MSAMPutRecipientPB       MSAMPutRecipientPB;

struct MSAMPutContentPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;

    MailMsgRef                      mailMsgRef;
    MailSegmentType                 segmentType;
    Boolean                         append;
    Byte                            padByte;
    MailBuffer                      buffer;
    StScrpRec *                     textScrap;
    Boolean                         startNewScript;
    Boolean                         filler1;
    ScriptCode                      script;                     /* valid only if startNewScript is true */
};
typedef struct MSAMPutContentPB         MSAMPutContentPB;

struct MSAMPutEnclosurePB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    Boolean                         contentEnclosure;
    Byte                            padByte;
    Boolean                         hfs;                        /* true => in file system, false => in memory */

    Boolean                         append;
    MailBuffer                      buffer;                     /* Unused if hfs == true */
    FSSpec                          enclosure;
    MailEnclosureInfo               addlInfo;
};
typedef struct MSAMPutEnclosurePB       MSAMPutEnclosurePB;

struct MSAMPutBlockPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    long                            refCon;                     /* for messages only */
    OCECreatorType                  blockType;
    Boolean                         append;
    Boolean                         filler1;
    MailBuffer                      buffer;
    MailBlockMode                   mode;                       /* if blockType is kMailTunnelLtrType or kMailHopInfoType  mode is assumed to be kMailFromMark */
    unsigned long                   offset;
};
typedef struct MSAMPutBlockPB           MSAMPutBlockPB;
/****************************************************************************************/

struct MSAMCreateReportPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MSAMQueueRef                    queueRef;                   /* to distinguish personal and server MSAMs */
    MailMsgRef                      mailMsgRef;
    MailLetterID                    msgID;                      /* kMailLetterIDBit of letter being reported upon */
    MailRecipient *                 sender;                     /* sender of the letter you are creating report on */
};
typedef struct MSAMCreateReportPB       MSAMCreateReportPB;

struct MSAMPutRecipientReportPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailMsgRef                      mailMsgRef;
    short                           recipientIndex;             /* recipient index in the original letter */
    OSErr                           result;                     /* result of sending the recipient */
};
typedef struct MSAMPutRecipientReportPB MSAMPutRecipientReportPB;
/****************************************************************************************/

struct MailWakeupPMSAMPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    CreationID                      pmsamCID;
    MailSlotID                      mailSlotID;
};
typedef struct MailWakeupPMSAMPB        MailWakeupPMSAMPB;

struct MailCreateMailSlotPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailboxRef                      mailboxRef;
    long                            timeout;
    CreationID                      pmsamCID;
    SMCA                            smca;
};
typedef struct MailCreateMailSlotPB     MailCreateMailSlotPB;

struct MailModifyMailSlotPB {
    void *                          qLink;
    long                            reservedH1;
    long                            reservedH2;
    MSAMIOCompletionUPP             ioCompletion;
    OSErr                           ioResult;
    long                            saveA5;
    short                           reqCode;
    MailboxRef                      mailboxRef;
    long                            timeout;
    CreationID                      pmsamCID;
    SMCA                            smca;
};
typedef struct MailModifyMailSlotPB     MailModifyMailSlotPB;

union MSAMParam {
    struct {
        void *                          qLink;
        long                            reservedH1;
        long                            reservedH2;
        MSAMIOCompletionUPP             ioCompletion;
        OSErr                           ioResult;
        long                            saveA5;
        short                           reqCode;
    }                                 header;

    PMSAMGetMSAMRecordPB            pmsamGetMSAMRecord;
    PMSAMOpenQueuesPB               pmsamOpenQueues;
    PMSAMSetStatusPB                pmsamSetStatus;
    PMSAMLogErrorPB                 pmsamLogError;


    SMSAMSetupPB                    smsamSetup;
    SMSAMStartupPB                  smsamStartup;
    SMSAMShutdownPB                 smsamShutdown;

    MSAMEnumeratePB                 msamEnumerate;
    MSAMDeletePB                    msamDelete;

    MSAMOpenPB                      msamOpen;
    MSAMOpenNestedPB                msamOpenNested;
    MSAMClosePB                     msamClose;
    MSAMGetMsgHeaderPB              msamGetMsgHeader;
    MSAMGetAttributesPB             msamGetAttributes;
    MSAMGetRecipientsPB             msamGetRecipients;
    MSAMGetContentPB                msamGetContent;
    MSAMGetEnclosurePB              msamGetEnclosure;
    MSAMEnumerateBlocksPB           msamEnumerateBlocks;
    MSAMGetBlockPB                  msamGetBlock;
    MSAMMarkRecipientsPB            msamMarkRecipients;
    MSAMnMarkRecipientsPB           msamnMarkRecipients;

    MSAMCreatePB                    msamCreate;
    MSAMBeginNestedPB               msamBeginNested;
    MSAMEndNestedPB                 msamEndNested;
    MSAMSubmitPB                    msamSubmit;
    MSAMPutMsgHeaderPB              msamPutMsgHeader;
    MSAMPutAttributePB              msamPutAttribute;
    MSAMPutRecipientPB              msamPutRecipient;
    MSAMPutContentPB                msamPutContent;
    MSAMPutEnclosurePB              msamPutEnclosure;           /* this field is SYSTEM8_DEPRECATED*/
    MSAMPutBlockPB                  msamPutBlock;

    MSAMCreateReportPB              msamCreateReport;           /* Reports and Error Handling Calls */
    MSAMPutRecipientReportPB        msamPutRecipientReport;

    PMSAMCreateMsgSummaryPB         pmsamCreateMsgSummary;
    PMSAMPutMsgSummaryPB            pmsamPutMsgSummary;
    PMSAMGetMsgSummaryPB            pmsamGetMsgSummary;

    MailWakeupPMSAMPB               wakeupPMSAM;
    MailCreateMailSlotPB            createMailSlot;
    MailModifyMailSlotPB            modifyMailSlot;
};
typedef union MSAMParam                 MSAMParam;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(MSAMIOCompletionUPP)
    NewMSAMIOCompletionUPP         (MSAMIOCompletionProcPtr userRoutine);

    EXTERN_API(void)
    DisposeMSAMIOCompletionUPP     (MSAMIOCompletionUPP     userUPP);

    EXTERN_API(void)
    InvokeMSAMIOCompletionUPP      (TempMSAMParam *         paramBlock,
                                    MSAMIOCompletionUPP     userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppMSAMIOCompletionProcInfo = 0x00009802 };              /* register no_return_value Func(4_bytes:A0) */
    #define NewMSAMIOCompletionUPP(userRoutine)                     (MSAMIOCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMSAMIOCompletionProcInfo, GetCurrentArchitecture())
    #define DisposeMSAMIOCompletionUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeMSAMIOCompletionUPP(__A0, __A1)
    void InvokeMSAMIOCompletionUPP(TempMSAMParam * paramBlock, MSAMIOCompletionUPP userUPP) = 0x4E91;
    #else
        #define InvokeMSAMIOCompletionUPP(paramBlock, userUPP)          CALL_ONE_PARAMETER_UPP((userUPP), uppMSAMIOCompletionProcInfo, (paramBlock))
    #endif
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewMSAMIOCompletionProc(userRoutine)                    NewMSAMIOCompletionUPP(userRoutine)
#define CallMSAMIOCompletionProc(userRoutine, paramBlock)       InvokeMSAMIOCompletionUPP(paramBlock, userRoutine)
/* ASYNCHRONOUS ONLY, client must call WaitNextEvent */
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
MailCreateMailSlot              (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7001, 0x1F00, 0x3F3C, 0x052B, 0xAA5E);

/* ASYNCHRONOUS ONLY, client must call WaitNextEvent */
EXTERN_API( OSErr )
MailModifyMailSlot              (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7001, 0x1F00, 0x3F3C, 0x052C, 0xAA5E);

/* ASYNCHRONOUS ONLY, client must call WaitNextEvent */
EXTERN_API( OSErr )
MailWakeupPMSAM                 (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7001, 0x1F00, 0x3F3C, 0x0507, 0xAA5E);

/* Personal MSAM Glue Routines */
EXTERN_API( OSErr )
PMSAMOpenQueues                 (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7000, 0x1F00, 0x3F3C, 0x0500, 0xAA5E);

EXTERN_API( OSErr )
PMSAMSetStatus                  (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0527, 0xAA5E);

/* SYNC ONLY */
EXTERN_API( OSErr )
PMSAMGetMSAMRecord              (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7000, 0x1F00, 0x3F3C, 0x0506, 0xAA5E);

/* Server MSAM Glue Routines */
/* SYNC ONLY */
EXTERN_API( OSErr )
SMSAMSetup                      (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7000, 0x1F00, 0x3F3C, 0x0523, 0xAA5E);

/* SYNC ONLY */
EXTERN_API( OSErr )
SMSAMStartup                    (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7000, 0x1F00, 0x3F3C, 0x0501, 0xAA5E);

EXTERN_API( OSErr )
SMSAMShutdown                   (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0502, 0xAA5E);

/* Get Interface Glue Routines */
EXTERN_API( OSErr )
MSAMEnumerate                   (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0503, 0xAA5E);

EXTERN_API( OSErr )
MSAMDelete                      (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0504, 0xAA5E);

EXTERN_API( OSErr )
MSAMMarkRecipients              (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0505, 0xAA5E);

EXTERN_API( OSErr )
MSAMnMarkRecipients             (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0512, 0xAA5E);

EXTERN_API( OSErr )
MSAMOpen                        (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0508, 0xAA5E);

EXTERN_API( OSErr )
MSAMOpenNested                  (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0509, 0xAA5E);

EXTERN_API( OSErr )
MSAMClose                       (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x050A, 0xAA5E);

EXTERN_API( OSErr )
MSAMGetRecipients               (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x050C, 0xAA5E);

EXTERN_API( OSErr )
MSAMGetAttributes               (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x050B, 0xAA5E);

EXTERN_API( OSErr )
MSAMGetContent                  (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x050D, 0xAA5E);

EXTERN_API( OSErr )
MSAMGetEnclosure                (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x050E, 0xAA5E);

EXTERN_API( OSErr )
MSAMEnumerateBlocks             (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x050F, 0xAA5E);

EXTERN_API( OSErr )
MSAMGetBlock                    (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0510, 0xAA5E);

EXTERN_API( OSErr )
MSAMGetMsgHeader                (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0511, 0xAA5E);

/* Put Interface Glue Routines */
EXTERN_API( OSErr )
MSAMCreate                      (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0514, 0xAA5E);

EXTERN_API( OSErr )
MSAMBeginNested                 (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0515, 0xAA5E);

EXTERN_API( OSErr )
MSAMEndNested                   (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7000, 0x1F00, 0x3F3C, 0x0516, 0xAA5E);

/*  SYNCHRONOUS ONLY */
EXTERN_API( OSErr )
MSAMSubmit                      (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7000, 0x1F00, 0x3F3C, 0x0517, 0xAA5E);

EXTERN_API( OSErr )
MSAMPutAttribute                (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0518, 0xAA5E);

EXTERN_API( OSErr )
MSAMPutRecipient                (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0519, 0xAA5E);

EXTERN_API( OSErr )
MSAMPutContent                  (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x051A, 0xAA5E);

/*  SYNCHRONOUS ONLY */
EXTERN_API( OSErr )
MSAMPutEnclosure                (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7000, 0x1F00, 0x3F3C, 0x051B, 0xAA5E);

EXTERN_API( OSErr )
MSAMPutBlock                    (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x051C, 0xAA5E);

EXTERN_API( OSErr )
MSAMPutMsgHeader                (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x051D, 0xAA5E);

/* Reports and Error Handling Glue Routines */
EXTERN_API( OSErr )
MSAMCreateReport                (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x051F, 0xAA5E);

EXTERN_API( OSErr )
MSAMPutRecipientReport          (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0520, 0xAA5E);

EXTERN_API( OSErr )
PMSAMLogError                   (MSAMParam *            paramBlock)                         FIVEWORDINLINE(0x7000, 0x1F00, 0x3F3C, 0x0521, 0xAA5E);

/* MsgSummary Glue Routines */
EXTERN_API( OSErr )
PMSAMCreateMsgSummary           (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0522, 0xAA5E);

EXTERN_API( OSErr )
PMSAMPutMsgSummary              (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0525, 0xAA5E);

EXTERN_API( OSErr )
PMSAMGetMsgSummary              (MSAMParam *            paramBlock,
                                 Boolean                asyncFlag)                          THREEWORDINLINE(0x3F3C, 0x0526, 0xAA5E);


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

#endif /* __OCEMAIL__ */

