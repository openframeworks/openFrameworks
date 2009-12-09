/*
     File:       OCEErrors.h
 
     Contains:   Apple Open Collaboration Environment Error Interfaces.
 
     Version:    Technology: AOCE Toolbox 1.02
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OCEERRORS__
#define __OCEERRORS__

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

/*
Allocated ranges:
    AOCE General Errors                     -1500 to -1539
    AOCE Authentication Errors              -1540 to -1609
    AOCE Catalog Errors                     -1610 to -1680
    
    AOCE Standard Mail Errors               -1900 to -1949
    AOCE Standard Catalog Errors            -1950 to -1969
    AOCE Digital Signature Errors           -1970 to -1999

    AOCE Templates Errors                   -15000 to -15039
    AOCE Mail Errors                        -15040 to -15089
    AOCE Messaging Errors                   -15090 to -15169
    Reserved for future                     -15170 to -15199
*/

/* AOCE General Errors (-1500 to -1539) */
enum {
    kOCEToolboxNotOpen          = -1500,                        /* Toolbox not open; perhaps its closing */
    kOCEInvalidCommand          = -1501,                        /* Request code passed to a trap call is invalid */
    kOCEInvalidRef              = -1502,                        /* a reference to an object is obsolete or invalid */
    kOCEBufferTooSmall          = -1503,                        /* the buffer supplied is too small for the task */
    kOCEVersionErr              = -1504,                        /* some manner of software version mismatch */
    kOCESyncAsyncErr            = -1505,                        /* Op tried synchronously may only be done async, or vice versa */
    kOCEInternalErr             = -1506,                        /* the OCE toolbox has an internal error */
    kOCENotAnOwner              = -1507,                        /* this operation can only be done by the object's owner */
    kOCENotImplemented          = -1508,                        /* Feature not implemented */
    kOCENotAuthenticated        = -1509,                        /* The user must be authenticated */
    kOCEAlreadyExists           = -1510,                        /* The object being added already exists */
    kOCEDoesntExist             = -1511,                        /* the object being referenced doesn't exist or cannot be found */
    kOCEConnectionErr           = -1512,                        /* Unable to open or maintain a network transport */
    kOCEConnectionClosed        = -1513,                        /* Connect has closed */
    kOCEInvalidRecipient        = -1514,                        /* Invalid recipient/address for this command */
    kOCEInvalidIndex            = -1515,                        /* invalid index (out of range) */
    kOCERefIsClosing            = -1516,                        /* Object references is being closed, cannot complete command */
    kOCEHasPendingMsgs          = -1517,
    kOCEInvalidConfiguration    = -1518,
    kOCERequiredServicesNotAvailable = -1519,
    kOCEServerInactive          = -1520,
    kOCEMoreDiskSpaceNeeded     = -1521,                        /* Inadequate disk space to safely complete operation */
    kOCEParamErr                = paramErr                      /* invalid parameter passed to toolbox */
};

/* AOCE Authentication Errors (-1540 to -1609) */
enum {
    kOCEReadAccessDenied        = -1540,
    kOCEWriteAccessDenied       = -1541,
    kOCEAccessRightsInsufficient = -1542,                       /* stream needs to be authenticated, or not authorized, or someone other than agent trying to TPFC, or problem in server-to-server authentication */
    kOCEUnsupportedCredentialsVersion = -1543,                  /* don't know how to read that version of credentials */
    kOCECredentialsProblem      = -1544,                        /* couldn't successfully decrypt credentials */
    kOCECredentialsImmature     = -1545,                        /* current time < first valid time */
    kOCECredentialsExpired      = -1546,                        /* current time > expiry time */
    kOCEProxyImmature           = -1547,                        /* current time < proxy start time */
    kOCEProxyExpired            = -1548,                        /* current time > proxy expiry time */
    kOCEDisallowedRecipient     = -1549,                        /* Recipient not specified in proxy */
    kOCENoKeyFound              = -1550,                        /* found no key for entity in question */
    kOCEPrincipalKeyNotFound    = -1551,                        /* couldn't decode proxy because principal has no key */
    kOCERecipientKeyNotFound    = -1552,                        /* recipient has no key */
    kOCEAgentKeyNotFound        = -1553,                        /* in TPFC, agent has no key */
    kOCEKeyAlreadyRegistered    = -1554,                        /* entity already has a key */
    kOCEMalformedKey            = -1555,                        /* key is of an unknown encryption method, or key doesn't match password */
    kOCEUndesirableKey          = -1556,                        /* password too short, key too simple, or trying to change cluster key to the same value */
    kOCEWrongIdentityOrKey      = -1557,                        /* In CheckIdentity, name doesn't exist or key is wrong */
    kOCEInitiatorKeyProblem     = -1558,                        /* either we couldn't find the initiator's key or we found it but couldn't successfully decrypt instructions or proxy */
    kOCEBadEncryptionMethod     = -1559,                        /* the specified encryption method is not supported */
    kOCELocalIdentityDoesNotExist = -1560,                      /* LocalIdentity has to be setup */
    kOCELocalAuthenticationFail = -1561,                        /* LocalIdentity Login has to be done for this operation */
    kOCELocalIdentitySetupExists = -1562,                       /* LocalIdentity setup exists, use change instead of setup */
    kOCEDirectoryIdentitySetupExists = -1563,                   /* DirectoryIdentity setup exists, use change instead of setup */
    kOCEDirectoryIdentitySetupDoesNotExist = -1564,             /* DirectoryIdentity setup does not exist */
    kOCENotLocalIdentity        = -1565,                        /* Use LocalIdentity calls instead! */
    kOCENoMoreIDs               = -1566,                        /* identity table is full */
    kOCEUnknownID               = -1567,                        /* identity passed is not valid */
    kOCEOperationDenied         = -1568,                        /* LocalID operation denied */
    kOCEAmbiguousMatches        = -1569,                        /* ambiguous matches found in resolving CIDs (more than 1 CID) */
    kOCENoASDSPWorkSpace        = -1570,                        /* No ASDSP workspace passed for Open Secure Stream */
    kOCEAuthenticationTrouble   = -1571                         /* generic authentication problem */
};

/* AOCE Catalog Errors (-1610 to -1680) */
enum {
    kOCENotLocal                = -1610,                        /* the server does not serve the requested dNode */
    kOCETooBusy                 = -1611,                        /* server cannot complete call at this time */
    kOCEDatabaseFull            = -1612,                        /* disk is full */
    kOCETargetDirectoryInaccessible = -1613,                    /* Catalog server not responding */
    kOCEBogusArgs               = -1614,                        /* Args not formatted correctly on the wire */
    kOCENoSuchDNode             = -1615,                        /* the requested dNode does not exist */
    kOCEDNodeUnavailable        = -1616,                        /* could not find any servers that serve the requested dNode */
    kOCEBadRecordID             = -1617,                        /* invalid RecordID (usually name and/or type don't match cid) */
    kOCENoSuchRecord            = -1618,                        /* the specified record does not exist */
    kOCENoSuchAttributeValue    = -1619,                        /* The attribute value that was passed in does not exist */
    kOCENoSuchPseudonym         = -1620,                        /* the specified pseudonym does not exist */
    kOCEAttributeValueTooBig    = -1621,                        /* self-explanatory */
    kOCETypeExists              = -1622,                        /* The type already exists in the record */
    kOCEMoreData                = -1623,                        /* only a warning - buffer not big enough */
    kOCERefNumBad               = -1624,                        /* RefNum is not valid */
    kOCEStreamCreationErr       = -1625,                        /* problem creating stream */
    kOCEOperationNotSupported   = -1626,                        /* the specified catalog does not support this operation */
    kOCEPABNotOpen              = -1627,                        /* specified Personal Catalog is not open to make the operation */
    kOCEDSAMInstallErr          = -1628,                        /* specified CSAM could not be installed */
    kOCEDirListFullErr          = -1629,                        /* catalog list is full, try removing an entry */
    kOCEDirectoryNotFoundErr    = -1630,                        /* catalog was not found in the list */
    kOCEAbortNotSupportedForThisCall = -1631,                   /* this call cannot be aborted */
    kOCEAborted                 = -1632,                        /* this call was aborted */
    kOCEOCESetupRequired        = -1633,                        /* LocalIdentity Setup is required */
    kOCEDSAMRecordNotFound      = -1634,                        /* CSAM Record not found */
    kOCEDSAMNotInstantiated     = -1635,                        /* CSAM is not instantiated */
    kOCEDSAMRecordExists        = -1636,                        /* CSAM record already exists */
    kOCELengthError             = -1637,                        /* supplied buffer was too small */
    kOCEBadStartingRecord       = -1638,                        /* Lookup starting record is not within range */
    kOCEBadStartingAttribute    = -1639,                        /* Lookup starting attribute is not within range */
    kOCEMoreAttrValue           = -1640,                        /* attribute value could not fit into buffer (even though it was the only value) */
    kOCENoDupAllowed            = -1641,
    kOCENoSuchAttributeType     = -1642,
    kOCEMiscError               = -1643,                        /* some other error */
    kOCENoSuchIcon              = -1644,                        /* no matching icon from GetDirectoryIcon */
    kOCERLIsDontMatch           = -1645,                        /* RLIs do not match in lookup */
    kOCEDirectoryCorrupt        = -1646                         /* Serious disk fill corruption problem */
};

/* AOCE Standard Mail Errors (-1900 to -1949) */
enum {
    kSMPNotEnoughMemoryForAllRecips = -1900,
    kSMPCopyInProgress          = -1901,
    kSMPMailerNotInitialized    = -1902,
    kSMPShouldNotAddContent     = -1903,
    kSMPMailboxNotFound         = -1904,
    kSMPNoNextLetter            = -1905,
    kSMPHasOpenAttachments      = -1906,
    kSMPFinderNotRunning        = -1907,
    kSMPCommandDisabled         = -1908,
    kSMPNoMailerInWindow        = -1909,
    kSMPNoSuchAddress           = -1910,
    kSMPMailerAlreadyInWindow   = -1911,
    kSMPMailerUneditable        = -1912,
    kSMPNoMatchingBegin         = -1913,
    kSMPCannotSendReceivedLetter = -1914,
    kSMPIllegalForDraftLetter   = -1915,
    kSMPMailerCannotExpandOrContract = -1916,
    kSMPMailerAlreadyExpandedOrContracted = -1917,
    kSMPIllegalComponent        = -1918,
    kSMPMailerAlreadyNotTarget  = -1919,
    kSMPComponentIsAlreadyTarget = -1920,
    kSMPRecordDoesNotContainAddress = -1921,
    kSMPAddressAlreadyInList    = -1922,
    kSMPIllegalSendFormats      = -1923,
    kSMPInvalidAddressString    = -1924,
    kSMPSubjectTooBig           = -1925,
    kSMPParamCountErr           = -1926,                        /*  enclosure count != 1, while sendEnclosure Only == true. */
    kSMPTooManyPages            = -1927,                        /*  Exceeding the image page limit. */
    kSMPTooManyEnclosures       = -1928
};

/* AOCE Standard Catalog Errors (-1950 to -1969) */
enum {
    kSDPNoSearchText            = -1950,
    kSDPTooManyLoginAttempts    = -1951,
    kSDPNoSelection             = -1952,
    kSDPPersonalDirectoryRepairFailed = -1953
};

/* AOCE Digital Signature Errors (-1970 to -1999) */
enum {
    kSIGOperationIncompatibleErr = -1970,                       /* context in use for different type of operation */
    kSIGCertificateQueryDenied  = -1971,                        /* can't query certificates with this context */
    kSIGVerifyFailedErr         = -1972,                        /* verification failed */
    kSIGInvalidCredentialErr    = -1973,                        /* verified OK but credential out of date,  either pending or expired */
    kSIGIndexErr                = -1974,                        /* the index given is outside the range of allowable values */
    kSIGSignerErr               = -1975,                        /* problem with the signer or signature itself */
    kSIGPasswordErr             = -1976,                        /* password is incorrect */
    kSIGInternalsErr            = -1977,                        /* an internal error occurred like a bad digest, context or signature */
    kSIGToolboxNotPresentErr    = -1978,                        /* for client services (ie servers) to report back - not retruned by the TB */
    kSIGContextPrepareErr       = -1979,                        /* context either prepared with VerifyPrepare, SignPrepare or DigestPrepare already or has been corrupted */
    kSIGNoDigestErr             = -1980,                        /* no digest in the signature */
    kSIGConversionErr           = -1981,                        /* unabled to convert an attribute to Mac format */
    kSIGSignerNotValidErr       = -1982,                        /* Signer has either expired or is pending and can not sign */
    kSIGNoSignature             = -1983                         /* standard file signature not found */
};

/* AOCE Templates Errors (-15000 to -15039) */
enum {
    kDETInvalidTargetAspectName = -15000,                       /* Could not locate an aspect by name */
    kDETInvalidTargetItemNumber = -15001,                       /* Item number is target invalid (out of range 1..<number of children>) */
    kDETInvalidTargetFromNonAspect = -15002,                    /* Target selection relative to non-aspect object which requires an aspect */
    kDETInvalidTargetDSSpec     = -15003,                       /* Target DSSpec could not be resolved */
    kDETUnknownTargetSelector   = -15004,                       /* Unknown selector type for target specification */
    kDETInvalidTarget           = -15005,                       /* Target specification was invalid (for reasons other than those above) */
    kDETTargetNotAnAspect       = -15006,                       /* Specified target object was not an aspect */
    kDETInvalidCommandItemNumber = -15007,                      /* Command item number invalid (out of range 1..<number of items in selection>) */
    kDETUnableToGetCommandItemSpec = -15008,                    /* Unable to retrieve item specification (internal error, possibly out of memory) */
    kDETRequestedTypeUnavailable = -15009,                      /* Command item could not be represented in the form requested */
    kDETInvalidDSSpec           = -15010,                       /* Could not resolve DSSpec (other than in target; see error above for targets) */
    kDETUnableToAccessProperty  = -15011,                       /* Property could not be found and/or created */
    kDETInfoPageNotOpen         = -15012,                       /* Info-page not open; request requires an open info-page */
    kDETNoSuchView              = -15013,                       /* No view found with specified property number */
    kDETCouldNotAddMenuItem     = -15014,                       /* Could not add item to dynamic menu */
    kDETCouldNotRemoveMenuItem  = -15015,                       /* Could not remove item from dynamic menu */
    kDETCouldNotFindMenuItem    = -15016,                       /* Could not find menu item */
    kDETCouldNotFindCustomView  = -15017,                       /* Could not find custom view in info-page */
    kDETInvalidReqFunction      = -15018,                       /* Invalid call-back request function */
    kDETInvalidCallBack         = -15019,                       /* Invalid call-back (for reasons other than those above) */
    kDETPropertyBusy            = -15020                        /* Edit is open on the property being set */
};

/* AOCE Mail Errors (-15040 to -15089) */
enum {
    kMailInvalidOrder           = -15040,                       /* This object has already been written to and cannot be written again */
                                                                /* handle non-zero offsets for now */
    kMailInvalidSeqNum          = -15041,                       /* Invalid letter sequence o refers to an non-existent letter */
    kMailAttrNotInHdr           = -15042,                       /* attribute does not exist in this letter header */
    kMailHdrAttrMissing         = -15043,                       /* required attribute not written into letter header */
    kMailBadEnclLengthErr       = -15044,                       /* incorrect number of bytes has been written after memForm of PutEnclosure */
    kMailInvalidRequest         = -15045,                       /* cannot putAttr for report, putRcptReport for letter */
    kMailInvalidPostItVersion   = -15046,                       /* cannot create new postit with this version */
    kMailNotASlotInQ            = -15047,                       /* queueID does not refer to a slot's incoming queue */
    kMailLtrNotOpen             = -15048,                       /* letter is not open, probably not cached in inQ */
    kMailMissingMailCat         = -15049,                       /* trying to create message in inq, with no postIt */
    kMailCannotWriteMailCat     = -15050,                       /* cannot update a mail catentry */
    kMailTooManyTagsPerLetter   = -15051,                       /* cannot add more than kMaxTagsPerLetter tags */
    kMailGwyDead                = -15052,                       /* MSAM died */
    kMailIgnoredErr             = -15053,                       /* ePPC message was ignored */
    kMailLengthErr              = -15054,                       /* ePPC message did not complete */
    kMailTooManyErr             = -15055,                       /* Too many = request outstanding, */
    kMailNoMSAMErr              = -15056,                       /* no MSAMs found in mailbox! or for command */
    kMailCancelled              = -15057,
    kMailSlotSuspended          = -15058,
    kMailMSAMSuspended          = -15059,
    kMailBadSlotInfo            = -15060,
    kMailMalformedContent       = -15061,                       /* a mailed structure is malformed */
    kMailNoSuchSlot             = -15062,                       /* no such slot */
    kMailBadLtrInfo             = -15063,                       /* bad info there, can't change */
    kMailSetFailed              = -15064,                       /* could not set letter info */
    kMailBadConfig              = -15065,                       /* MSAM has bad config info */
    kMailBadMSAM                = -15066,                       /* bad MSAM: not usable = reason unspec, */
    kMailCannotLkupServerSlots  = -15067,                       /* cannot lookup adas for server mail slots */
    kMailBadState               = -15068                        /* the state specifiy is not in any of defined */
};

/* AOCE Messaging Errors (-15090 to -15169) */
enum {
    kIPMCantCreateIPMCatEntry   = -15090,
    kIPMInvalidMsgType          = -15091,                       /* bad msg hint */
    kIPMInvalidProcHint         = -15092,                       /* bad proc hint */
    kIPMInvalidOffset           = -15093,                       /* bad offset for read/write */
    kIPMUpdateCatFailed         = -15094,
    kIPMMsgTypeReserved         = -15095,
    kIPMNotInABlock             = -15096,
    kIPMNestedMsgOpened         = -15097,
    kIPMA1HdrCorrupt            = -15098,
    kIPMCorruptDataStructures   = -15099,
    kIPMAbortOfNestedMsg        = -15100,
    kIPMBlockIsNotNestedMsg     = -15101,
    kIPMCacheFillError          = -15102,
    kIPMInvalidSender           = -15103,
    kIPMNoRecipientsYet         = -15104,
    kIPMInvalidFilter           = -15105,
    kIPMAttrNotInHdr            = -15106,
    kIPMBlkNotFound             = -15107,
    kIPMStreamErr               = -15108,                       /* Error on stream (should this ever go out?) */
    kIPMPortClosed              = -15109,                       /* Port closed (should this ever go out?) */
    kIPMBinBusy                 = -15110,
    kIPMCorruptedBin            = -15111,
    kIPMBadQName                = -15112,
    kIPMEndOfBin                = -15113,
    kIPMBinNeedsConversion      = -15114,
    kIPMMgrInternalErr          = -15115,
    kIPMEltBusy                 = -15116,
    kIPMEltClosedNotDeleted     = -15117,
    kIPMBadContext              = -15118,
    kIPMContextIsClosing        = -15119,
    kIPMeoQ                     = -15120,
    kIPMQOutOfSpace             = -15121,
    kIPMEltNotFound             = -15122,
    kIPMBinOutOfSpace           = -15123,
    kIPMeoCatEntry              = -15124,
    kIPMeoElt                   = -15125,
    kIPMQBusy                   = -15126,
    kIPMNotSameBin              = -15127,
    kIPMNoMoreServices          = -15128,
    kIPMLookupAttrTooBig        = -15129,                       /* attribute in lookup is too big */
    kIPMUnknownUAM              = -15130,
    kIPMUnsupportedLogin        = -15131,
    kIPMLogInUnknown            = -15132,
    kIPMNoMoreFilters           = -15133,
    kIPMCouldNotReadInfoReply   = -15134,
    kIPMInvalidUser             = -15135,
    kIPMNoMoreSessions          = -15136,
    kIPMNilUserProc             = -15137,
    kIPMBadCatSize              = -15138,
    kIPMIncompleteMsg           = -15139,
    kIPMNoResponsibleTunnel     = -15140,
    kIPMAccessDenied            = -15141,
    kIPMReplyParamUnderrun      = -15142,                       /* Too few reply parameters */
    kIPMReplyParamOverrun       = -15143,                       /* Too many reply parameters */
    kIPMRequestParamUnderrun    = -15144,                       /* Too few request parameters */
    kIPMOutOfSync               = -15145,                       /* Protocol sync. error */
    kIPMNoAttrsFound            = -15146,                       /* couldn't find any attrs in lookup */
    kIPMWrongAttrTag            = -15147,                       /* wrong attr tag in adas lookup */
    kIPMNullCID                 = -15148,                       /* null cid in enumerate */
    kIPMBadMailSlotAttrVal      = -15149                        /* invalid mailslot attr value */
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

#endif /* __OCEERRORS__ */

