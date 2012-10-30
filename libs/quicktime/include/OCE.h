/*
     File:       OCE.h
 
     Contains:   Apple Open Collaboration Environment (AOCE) Interfaces.
 
     Version:    Technology: AOCE Toolbox 1.02
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OCE__
#define __OCE__

#ifndef REZ
#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __ALIASES__
#include <Aliases.h>
#endif

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
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

/* All utility routines defined here are callable at interrupt level. */
typedef unsigned short                  OCERecordTypeIndex;
typedef unsigned short                  OCEAttributeTypeIndex;
/* For anyone who absolutely needs a define of the body of the standard record or
attribute type, use these below.  CAUTION!  All the types below are assumed to be
in character set 'smRoman'.  If you try to compare these to some RString or
AttributeType variable, you must take the character set code into account.  Future
standard types may be defined using character sets other than 'smRoman'. */

/* All these standard definitions begin with the Apple symbol (not shown here).

NOTE:  To access these, you must call OCEGetIndRecordType or OCEGetIndAttributeType
with the proper index.  These routines return pointers to the standard type.
This was done so that code fragments (INITs, CDEVs, CSAMs, etc). which cannot
use global data can also use these. */
/* Indices for the standard definitions for certain record types (OCERecordTypeIndex): */
enum {
    kUserRecTypeNum             = 1,                            /* "User" */
    kGroupRecTypeNum            = 2,                            /* "Group" */
    kMnMRecTypeNum              = 3,                            /* "AppleMail(tm) M&M" */
    kMnMForwarderRecTypeNum     = 4,                            /* "AppleMail(tm) Fwdr" */
    kNetworkSpecRecTypeNum      = 5,                            /* "NetworkSpec" */
    kADAPServerRecTypeNum       = 6,                            /* "ADAP Server" */
    kADAPDNodeRecTypeNum        = 7,                            /* "ADAP DNode" */
    kADAPDNodeRepRecTypeNum     = 8,                            /* "ADAP DNode Rep" */
    kServerSetupRecTypeNum      = 9,                            /* "Server Setup" */
    kDirectoryRecTypeNum        = 10,                           /* "Directory" */
    kDNodeRecTypeNum            = 11,                           /* "DNode" */
    kSetupRecTypeNum            = 12,                           /* "Setup" */
    kMSAMRecTypeNum             = 13,                           /* "MSAM" */
    kDSAMRecTypeNum             = 14,                           /* "DSAM" */
    kAttributeValueRecTypeNum   = 15,                           /* "Attribute Value" */
    kBusinessCardRecTypeNum     = 16,                           /* "Business Card" */
    kMailServiceRecTypeNum      = 17,                           /* "Mail Service" */
    kCombinedRecTypeNum         = 18,                           /* "Combined" */
    kOtherServiceRecTypeNum     = 19,                           /* "Other Service" */
    kAFPServiceRecTypeNum       = 20,                           /* "Other Service afps" */
    kFirstOCERecTypeNum         = kUserRecTypeNum,              /* first standard OCE record type */
    kLastOCERecTypeNum          = kAFPServiceRecTypeNum,        /* last standard OCE record type */
    kNumOCERecTypes             = (kLastOCERecTypeNum - kFirstOCERecTypeNum + 1)
};

/* Indices for the standard definitions for certain attribute types (OCEAttributeTypeIndex): */
enum {
    kMemberAttrTypeNum          = 1001,                         /* "Member" */
    kAdminsAttrTypeNum          = 1002,                         /* "Administrators" */
    kMailSlotsAttrTypeNum       = 1003,                         /* "mailslots" */
    kPrefMailAttrTypeNum        = 1004,                         /* "pref mailslot" */
    kAddressAttrTypeNum         = 1005,                         /* "Address" */
    kPictureAttrTypeNum         = 1006,                         /* "Picture" */
    kAuthKeyAttrTypeNum         = 1007,                         /* "auth key" */
    kTelephoneAttrTypeNum       = 1008,                         /* "Telephone" */
    kNBPNameAttrTypeNum         = 1009,                         /* "NBP Name" */
    kQMappingAttrTypeNum        = 1010,                         /* "ForwarderQMap" */
    kDialupSlotAttrTypeNum      = 1011,                         /* "DialupSlotInfo" */
    kHomeNetAttrTypeNum         = 1012,                         /* "Home Internet" */
    kCoResAttrTypeNum           = 1013,                         /* "Co-resident M&M" */
    kFwdrLocalAttrTypeNum       = 1014,                         /* "FwdrLocalRecord" */
    kConnectAttrTypeNum         = 1015,                         /* "Connected To" */
    kForeignAttrTypeNum         = 1016,                         /* "Foreign RLIs" */
    kOwnersAttrTypeNum          = 1017,                         /* "Owners" */
    kReadListAttrTypeNum        = 1018,                         /* "ReadList" */
    kWriteListAttrTypeNum       = 1019,                         /* "WriteList" */
    kDescriptorAttrTypeNum      = 1020,                         /* "Descriptor" */
    kCertificateAttrTypeNum     = 1021,                         /* "Certificate" */
    kMsgQsAttrTypeNum           = 1022,                         /* "MessageQs" */
    kPrefMsgQAttrTypeNum        = 1023,                         /* "PrefMessageQ" */
    kMasterPFAttrTypeNum        = 1024,                         /* "MasterPF" */
    kMasterNetSpecAttrTypeNum   = 1025,                         /* "MasterNetSpec" */
    kServersOfAttrTypeNum       = 1026,                         /* "Servers Of" */
    kParentCIDAttrTypeNum       = 1027,                         /* "Parent CID" */
    kNetworkSpecAttrTypeNum     = 1028,                         /* "NetworkSpec" */
    kLocationAttrTypeNum        = 1029,                         /* "Location" */
    kTimeSvrTypeAttrTypeNum     = 1030,                         /* "TimeServer Type" */
    kUpdateTimerAttrTypeNum     = 1031,                         /* "Update Timer" */
    kShadowsOfAttrTypeNum       = 1032,                         /* "Shadows Of" */
    kShadowServerAttrTypeNum    = 1033,                         /* "Shadow Server" */
    kTBSetupAttrTypeNum         = 1034,                         /* "TB Setup" */
    kMailSetupAttrTypeNum       = 1035,                         /* "Mail Setup" */
    kSlotIDAttrTypeNum          = 1036,                         /* "SlotID" */
    kGatewayFileIDAttrTypeNum   = 1037,                         /* "Gateway FileID" */
    kMailServiceAttrTypeNum     = 1038,                         /* "Mail Service" */
    kStdSlotInfoAttrTypeNum     = 1039,                         /* "Std Slot Info" */
    kAssoDirectoryAttrTypeNum   = 1040,                         /* "Asso. Directory" */
    kDirectoryAttrTypeNum       = 1041,                         /* "Directory" */
    kDirectoriesAttrTypeNum     = 1042,                         /* "Directories" */
    kSFlagsAttrTypeNum          = 1043,                         /* "SFlags" */
    kLocalNameAttrTypeNum       = 1044,                         /* "Local Name" */
    kLocalKeyAttrTypeNum        = 1045,                         /* "Local Key" */
    kDirUserRIDAttrTypeNum      = 1046,                         /* "Dir User RID" */
    kDirUserKeyAttrTypeNum      = 1047,                         /* "Dir User Key" */
    kDirNativeNameAttrTypeNum   = 1048,                         /* "Dir Native Name" */
    kCommentAttrTypeNum         = 1049,                         /* "Comment" */
    kRealNameAttrTypeNum        = 1050,                         /* "Real Name" */
    kPrivateDataAttrTypeNum     = 1051,                         /* "Private Data" */
    kDirTypeAttrTypeNum         = 1052,                         /* "Directory Type" */
    kDSAMFileAliasAttrTypeNum   = 1053,                         /* "DSAM File Alias" */
    kCanAddressToAttrTypeNum    = 1054,                         /* "Can Address To" */
    kDiscriminatorAttrTypeNum   = 1055,                         /* "Discriminator" */
    kAliasAttrTypeNum           = 1056,                         /* "Alias" */
    kParentMSAMAttrTypeNum      = 1057,                         /* "Parent MSAM" */
    kParentDSAMAttrTypeNum      = 1058,                         /* "Parent DSAM" */
    kSlotAttrTypeNum            = 1059,                         /* "Slot" */
    kAssoMailServiceAttrTypeNum = 1060,                         /* "Asso. Mail Service" */
    kFakeAttrTypeNum            = 1061,                         /* "Fake" */
    kInheritSysAdminAttrTypeNum = 1062,                         /* "Inherit SysAdministrators" */
    kPreferredPDAttrTypeNum     = 1063,                         /* "Preferred PD" */
    kLastLoginAttrTypeNum       = 1064,                         /* "Last Login" */
    kMailerAOMStateAttrTypeNum  = 1065,                         /* "Mailer AOM State" */
    kMailerSendOptionsAttrTypeNum = 1066,                       /* "Mailer Send Options" */
    kJoinedAttrTypeNum          = 1067,                         /* "Joined" */
    kUnconfiguredAttrTypeNum    = 1068,                         /* "Unconfigured" */
    kVersionAttrTypeNum         = 1069,                         /* "Version" */
    kLocationNamesAttrTypeNum   = 1070,                         /* "Location Names" */
    kActiveAttrTypeNum          = 1071,                         /* "Active" */
    kDeleteRequestedAttrTypeNum = 1072,                         /* "Delete Requested" */
    kGatewayTypeAttrTypeNum     = 1073,                         /* "Gateway Type" */
    kFirstOCEAttrTypeNum        = kMemberAttrTypeNum,           /* first standard OCE attr type */
    kLastOCEAttrTypeNum         = kGatewayTypeAttrTypeNum,      /* last standard OCE attr type */
    kNumOCEAttrTypes            = (kLastOCEAttrTypeNum - kFirstOCEAttrTypeNum + 1)
};


#endif /* REZ */

/* Standard definitions for certain record types: */

#define  kUserRecTypeBody           "aoce User"
#define  kGroupRecTypeBody          "aoce Group"
#define  kMnMRecTypeBody            "aoce AppleMail  M&M"
#define  kMnMForwarderRecTypeBody   "aoce AppleMail  Fwdr"
#define  kNetworkSpecRecTypeBody    "aoce NetworkSpec"
#define  kADAPServerRecTypeBody     "aoce ADAP Server"
#define  kADAPDNodeRecTypeBody      "aoce ADAP DNode"
#define  kADAPDNodeRepRecTypeBody   "aoce ADAP DNode Rep"
#define  kServerSetupRecTypeBody    "aoce Server Setup"
#define  kDirectoryRecTypeBody      "aoce Directory"
#define  kDNodeRecTypeBody          "aoce DNode"
#define  kSetupRecTypeBody          "aoce Setup"
#define  kMSAMRecTypeBody           "aoce MSAM"
#define  kDSAMRecTypeBody           "aoce DSAM"
#define  kAttributeValueRecTypeBody "aoce Attribute Value"
#define  kBusinessCardRecTypeBody   "aoce Business Card"
#define  kMailServiceRecTypeBody    "aoce Mail Service"
#define  kCombinedRecTypeBody       "aoce Combined"
#define  kOtherServiceRecTypeBody   "aoce Other Service"
#define  kAFPServiceRecTypeBody     "aoce Other Service afps"

/* Standard definitions for certain attribute types: */

#define  kMemberAttrTypeBody        "aoce Member"
#define  kAdminsAttrTypeBody        "aoce Administrators"
#define  kMailSlotsAttrTypeBody     "aoce mailslots"
#define  kPrefMailAttrTypeBody      "aoce pref mailslot"
#define  kAddressAttrTypeBody       "aoce Address"
#define  kPictureAttrTypeBody       "aoce Picture"
#define  kAuthKeyAttrTypeBody       "aoce auth key"
#define  kTelephoneAttrTypeBody     "aoce Telephone"
#define  kNBPNameAttrTypeBody       "aoce NBP Name"
#define  kQMappingAttrTypeBody      "aoce ForwarderQMap"
#define  kDialupSlotAttrTypeBody    "aoce DialupSlotInfo"
#define  kHomeNetAttrTypeBody       "aoce Home Internet"
#define  kCoResAttrTypeBody         "aoce Co-resident M&M"
#define  kFwdrLocalAttrTypeBody     "aoce FwdrLocalRecord"
#define  kConnectAttrTypeBody       "aoce Connected To"
#define  kForeignAttrTypeBody       "aoce Foreign RLIs"
#define  kOwnersAttrTypeBody        "aoce Owners"
#define  kReadListAttrTypeBody      "aoce ReadList"
#define  kWriteListAttrTypeBody     "aoce WriteList"
#define  kDescriptorAttrTypeBody    "aoce Descriptor"
#define  kCertificateAttrTypeBody   "aoce Certificate"
#define  kMsgQsAttrTypeBody         "aoce MessageQs"
#define  kPrefMsgQAttrTypeBody      "aoce PrefMessageQ"
#define  kMasterPFAttrTypeBody      "aoce MasterPF"
#define  kMasterNetSpecAttrTypeBody "aoce MasterNetSpec"
#define  kServersOfAttrTypeBody     "aoce Servers Of"
#define  kParentCIDAttrTypeBody     "aoce Parent CID"
#define  kNetworkSpecAttrTypeBody   "aoce NetworkSpec"
#define  kLocationAttrTypeBody      "aoce Location"
#define  kTimeSvrTypeAttrTypeBody   "aoce TimeServer Type"
#define  kUpdateTimerAttrTypeBody   "aoce Update Timer"
#define  kShadowsOfAttrTypeBody     "aoce Shadows Of"
#define  kShadowServerAttrTypeBody  "aoce Shadow Server"
#define  kTBSetupAttrTypeBody       "aoce TB Setup"
#define  kMailSetupAttrTypeBody     "aoce Mail Setup"
#define  kSlotIDAttrTypeBody        "aoce SlotID"
#define  kGatewayFileIDAttrTypeBody "aoce Gateway FileID"
#define  kMailServiceAttrTypeBody   "aoce Mail Service"
#define  kStdSlotInfoAttrTypeBody   "aoce Std Slot Info"
#define  kAssoDirectoryAttrTypeBody "aoce Asso. Directory"
#define  kDirectoryAttrTypeBody     "aoce Directory"
#define  kDirectoriesAttrTypeBody   "aoce Directories"
#define  kSFlagsAttrTypeBody        "aoce SFlags"
#define  kLocalNameAttrTypeBody     "aoce Local Name"
#define  kLocalKeyAttrTypeBody      "aoce Local Key"
#define  kDirUserRIDAttrTypeBody    "aoce Dir User RID"
#define  kDirUserKeyAttrTypeBody    "aoce Dir User Key"
#define  kDirNativeNameAttrTypeBody "aoce Dir Native Name"
#define  kCommentAttrTypeBody       "aoce Comment"
#define  kRealNameAttrTypeBody      "aoce Real Name"
#define  kPrivateDataAttrTypeBody   "aoce Private Data"
#define  kDirTypeAttrTypeBody       "aoce Directory Type"
#define  kDSAMFileAliasAttrTypeBody "aoce DSAM File Alias"
#define  kCanAddressToAttrTypeBody  "aoce Can Address To"
#define  kDiscriminatorAttrTypeBody "aoce Discriminator"
#define  kAliasAttrTypeBody         "aoce Alias"
#define  kParentMSAMAttrTypeBody    "aoce Parent MSAM"
#define  kParentDSAMAttrTypeBody    "aoce Parent DSAM"
#define  kSlotAttrTypeBody              "aoce Slot"
#define  kAssoMailServiceAttrTypeBody   "aoce Asso. Mail Service"
#define  kFakeAttrTypeBody              "aoce Fake"
#define  kInheritSysAdminAttrTypeBody   "aoce Inherit SysAdministrators"
#define  kPreferredPDAttrTypeBody       "aoce Preferred PD"
#define  kLastLoginAttrTypeBody         "aoce Last Login"
#define  kMailerAOMStateAttrTypeBody    "aoce Mailer AOM State"
#define  kMailerSendOptionsAttrTypeBody "aoce Mailer Send Options"
#define  kJoinedAttrTypeBody            "aoce Joined"
#define  kUnconfiguredAttrTypeBody      "aoce Unconfigured"
#define  kVersionAttrTypeBody           "aoce Version"
#define  kLocationNamesAttrTypeBody     "aoce Location Names"
#define  kActiveAttrTypeBody            "aoce Active"
#define  kDeleteRequestedAttrTypeBody   "aoce Delete Requested"
#define  kGatewayTypeAttrTypeBody       "aoce Gateway Type"
#ifndef REZ
/* Miscellaneous enums: */
enum {
    kRString32Size              = 32,                           /* max size of the body field in RString32 */
    kRString64Size              = 64,                           /* max size of the body field in RString64 */
    kNetworkSpecMaxBytes        = 32,                           /* max size of the body field in NetworkSpec */
    kPathNameMaxBytes           = 1024,                         /* max size of the data field in PackedPathName */
    kDirectoryNameMaxBytes      = 32,                           /* max size of the body field in DirectoryName */
    kAttributeTypeMaxBytes      = 32,                           /* max size of the body field in AttributeType */
    kAttrValueMaxBytes          = 65536L,                       /* max size of any attribute value */
    kRStringMaxBytes            = 256,                          /* max size (in bytes) of the body field of a recordName or recordType */
    kRStringMaxChars            = 128                           /* max size (in chars) of the body field of a recordName or recordType */
};

enum {
    kNULLDNodeNumber            = 0,                            /* Special value meaning none specified */
    kRootDNodeNumber            = 2                             /* DNodeNum corresponding to the root of the tree */
};


/* This enum is used to select the kind of RString in calls such as OCERelRString,
OCEEqualRString, and OCEValidRString.

eGenericSensitive and eGenericInsensitive are enumerators that can be used if you
use RStrings for things other than what you see in this file.  If you want them to
be compared in a case- and diacritical-sensitive manner (c . C . .), use
eGenericSensitive.  If you want them to be compared in a case- and diacritical-
insensitive manner (c = C = .), use eGenericInensitive.
WARNING:  do not use eGenericSensitive and eGenericInsensitive with catalog
names, entity names, pathname parts, entity types, network specs, or attribute
types!  Don't assume that you know how they should be compared!!! */
enum {
    kOCEDirName                 = 0,
    kOCERecordOrDNodeName       = 1,
    kOCERecordType              = 2,
    kOCENetworkSpec             = 3,
    kOCEAttrType                = 4,
    kOCEGenericSensitive        = 5,
    kOCEGenericInsensitive      = 6
};

typedef unsigned short                  RStringKind;
/* Values for the signature field in Discriminator */
enum {
    kDirAllKinds                = 0,
    kDirADAPKind                = FOUR_CHAR_CODE('adap'),
    kDirPersonalDirectoryKind   = FOUR_CHAR_CODE('pdir'),
    kDirDSAMKind                = FOUR_CHAR_CODE('dsam')
};


typedef unsigned long                   OCEDirectoryKind;
/* Values returned by GetDSSpecInfo() */
enum {
    kOCEInvalidDSSpec           = 0x3F3F3F3F,                   /* '????' could not be determined */
    kOCEDirsRootDSSpec          = FOUR_CHAR_CODE('root'),       /* root of all catalogs ("Catalogs" icon) */
    kOCEDirectoryDSSpec         = FOUR_CHAR_CODE('dire'),       /* catalog */
    kOCEDNodeDSSpec             = FOUR_CHAR_CODE('dnod'),       /* d-node */
    kOCERecordDSSpec            = FOUR_CHAR_CODE('reco'),       /* record */
    kOCEentnDSSpec              = FOUR_CHAR_CODE('entn'),       /* extensionType is 'entn' */
    kOCENOTentnDSSpec           = FOUR_CHAR_CODE('not ')        /* extensionType is not 'entn' */
};


/* Values for AttributeTag */
enum {
    typeRString                 = FOUR_CHAR_CODE('rstr'),
    typePackedDSSpec            = FOUR_CHAR_CODE('dspc'),
    typeBinary                  = FOUR_CHAR_CODE('bnry')
};


/* Bit flag corresponding to the canContainRecords bit.  Use it like this:
    if (foo & kCanContainRecords)
        then this dNode can contain records!
kForeignNode is used to indicate nodes in the name hierarchy that correspond to
foreign catalogs (meaning ADAP sees no clusters or DNodes beneath it, but
mail routers might be able to route to clusters beneath it. */
enum {
    kCanContainRecordsBit       = 0,
    kForeignNodeBit             = 1
};

/* DirNodeKind */
enum {
    kCanContainRecords          = 1L << kCanContainRecordsBit,
    kForeignNode                = 1L << kForeignNodeBit
};

typedef unsigned long                   DirNodeKind;


/**** Toolbox Control ****/
/* We will have a version number and attributes for toolboxes off the aa5e trap
and the S&F server trap.

This includes the OCE toolbox and S&F Server.  [Note: the S&F server will
change to ONLY service ServerGateway calls -it will then be necessary to run
it co-resident with an OCE toolbox].

The high order word will represent the S&F Server version number.  The low
order word will represent the OCE toolbox version number.  These will be zero
until the component is up and running.  It is not possible to know these
a-priori. Note: there will not be a seperate version numbers for each component
in the OCE toolbox or S&F server.

The above is consistent with the standard System 7.0 usage of Gestalt.

The oce tb attribute gestaltOCETBPresent implies the existence of OCE on a
machine.

The OCE TB attribute gestaltOCETBAvailable implies the availablity of OCE calls.

The attribute gestaltOCESFServerAvailable implies the availablity of OCE calls
available through the S&F server. This are essentially the server gateway calls.

Any (future) remaining OCE attributes may not be established correctly until
the attribute gestaltOCETBAvailable is set.

*/

/*  Constants used for Transitions. */
enum {
    ATTransIPMStart             = FOUR_CHAR_CODE('ipms'),
    ATTransIPMShutdown          = FOUR_CHAR_CODE('ipmd'),
    ATTransDirStart             = FOUR_CHAR_CODE('dirs'),
    ATTransDirShutdown          = FOUR_CHAR_CODE('dird'),
    ATTransAuthStart            = FOUR_CHAR_CODE('auts'),
    ATTransAuthShutdown         = FOUR_CHAR_CODE('autd'),
    ATTransSFStart              = FOUR_CHAR_CODE('s&fs'),
    ATTransSFShutdown           = FOUR_CHAR_CODE('s&fd')
};



/* Some definitions for time-related parameters: */
/* Interpreted as UTC seconds since 1/1/1904 */
typedef unsigned long                   UTCTime;
/* seconds EAST of Greenwich */
typedef long                            UTCOffset;
/* This is the same as the ScriptManager script. */
typedef short                           CharacterSet;
/**** RString ****/

/* struct RString is a maximum-sized structure.  Allocate one of these and it will
hold any valid RString. */

struct RString {
    CharacterSet                    charSet;
    unsigned short                  dataLength;
    Byte                            body[256];                  /* place for characters */
};
typedef struct RString                  RString;
/* struct ProtoRString is a minimum-sized structure.  Use this for a variable-length RString. */

struct ProtoRString {
    CharacterSet                    charSet;
    unsigned short                  dataLength;
};
typedef struct ProtoRString             ProtoRString;

typedef RString *                       RStringPtr;
typedef RStringPtr *                    RStringHandle;
typedef ProtoRString *                  ProtoRStringPtr;

struct RString64 {
    CharacterSet                    charSet;
    unsigned short                  dataLength;
    Byte                            body[64];
};
typedef struct RString64                RString64;

struct RString32 {
    CharacterSet                    charSet;
    unsigned short                  dataLength;
    Byte                            body[32];
};
typedef struct RString32                RString32;
/* Standard definitions for the entity type field and attribute type
have been moved to the end of the file. */

/* Copies str1 to str2.  str2Length is the size of str2, excluding header.
A memFull error will be returned if that is not as large as str1->dataLength. */
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OCECopyRString                  (const RString *        str1,
                                 RString *              str2,
                                 unsigned short         str2Length)                         THREEWORDINLINE(0x303C, 0x0308, 0xAA5C);


/*    Make an RString from a C string.  If the c string is bigger than rStrLength,
only rStrLength bytes will be copied. (rStrLength does not include the header size) */
EXTERN_API( void )
OCECToRString                   (const char *           cStr,
                                 CharacterSet           charSet,
                                 RString *              rStr,
                                 unsigned short         rStrLength)                         THREEWORDINLINE(0x303C, 0x0339, 0xAA5C);


/*    Make an RString from a Pascal string.  If the Pascal string is bigger than rStrLength,
only rStrLength bytes will be copied. (rStrLength does not include the header size) */
EXTERN_API( void )
OCEPToRString                   (ConstStr255Param       pStr,
                                 CharacterSet           charSet,
                                 RString *              rStr,
                                 unsigned short         rStrLength)                         THREEWORDINLINE(0x303C, 0x033A, 0xAA5C);


/*    Make a Pascal string from an RString.  It's up to you to check the char set of
the RString, or if the length of the RString is greater than 255 (the Pascal string's
length will simply be the lower byte of the RString's length).  The StringPtr that is
returned will point directly into the RString (no memory will be allocated). */
EXTERN_API( StringPtr )
OCERToPString                   (const RString *        rStr)                               THREEWORDINLINE(0x303C, 0x033B, 0xAA5C);


/*    Check the relative equality of two RStrings.  Determines if str1 is greater than,
equal to, or less than str2.  Result types for OCERelRString are defined in <OSUtils.h>
(same as for RelString). */
EXTERN_API( short )
OCERelRString                   (const void *           str1,
                                 const void *           str2,
                                 RStringKind            kind)                               THREEWORDINLINE(0x303C, 0x032D, 0xAA5C);


/*  Check for equality of two RStrings. Returns true if equal. */
EXTERN_API( Boolean )
OCEEqualRString                 (const void *           str1,
                                 const void *           str2,
                                 RStringKind            kind)                               THREEWORDINLINE(0x303C, 0x0316, 0xAA5C);


/*  Check the validity of an RString.  Returns true if the RString is valid */
EXTERN_API( Boolean )
OCEValidRString                 (const void *           str,
                                 RStringKind            kind)                               THREEWORDINLINE(0x303C, 0x0338, 0xAA5C);


/**** CreationID ****/
#endif  /* CALL_NOT_IN_CARBON */


struct CreationID {
    unsigned long                   source;                     /* Fields definitions and usage are not defined */
    unsigned long                   seq;
};
typedef struct CreationID               CreationID;

typedef CreationID                      AttributeCreationID;
typedef CreationID *                    CreationIDPtr;

/* Returns a pointer to a null CreationID . */
#if CALL_NOT_IN_CARBON
EXTERN_API( const CreationID *)
OCENullCID                      (void)                                                      THREEWORDINLINE(0x303C, 0x0344, 0xAA5C);


/* Returns a pointer to a special CreationID used within the PathFinder. */
EXTERN_API( const CreationID *)
OCEPathFinderCID                (void)                                                      THREEWORDINLINE(0x303C, 0x033C, 0xAA5C);


/* Sets the CreationID to a null value. */
EXTERN_API( void )
OCESetCreationIDtoNull          (CreationID *           cid)                                THREEWORDINLINE(0x303C, 0x032E, 0xAA5C);


/* Copies the value of cid1 to cid2. */
EXTERN_API( void )
OCECopyCreationID               (const CreationID *     cid1,
                                 CreationID *           cid2)                               THREEWORDINLINE(0x303C, 0x0300, 0xAA5C);


/* Check the equality of two CreationIDs. */
EXTERN_API( Boolean )
OCEEqualCreationID              (const CreationID *     cid1,
                                 const CreationID *     cid2)                               THREEWORDINLINE(0x303C, 0x030C, 0xAA5C);


/**** NetworkSpec ****/
/* For the record, a NetworkSpec is an RString with a smaller maximum size.
I don't just typedef it to an RString, because I want the definition of the NetworkSpec
struct to contain the max length.  But it should be possible to typecast any
NetworkSpec to an RString and use all the RString utilities on it. */
#endif  /* CALL_NOT_IN_CARBON */


struct NetworkSpec {
    CharacterSet                    charSet;
    unsigned short                  dataLength;
    Byte                            body[32];                   /* always fixed at the max size */
};
typedef struct NetworkSpec              NetworkSpec;
typedef NetworkSpec *                   NetworkSpecPtr;

/**** PackedPathName ****/
/* struct PackedPathName is a maximum-sized structure.  Allocate one of
these and it will hold any valid packed pathname. */

struct PackedPathName {
    unsigned short                  dataLength;                 /* dataLength excludes the space for the dataLength field */
    Byte                            data[1022];
};
typedef struct PackedPathName           PackedPathName;
/* struct ProtoPackedPathName is a minimum-sized structure.  Use this
for a variable-length packed PathName. */

struct ProtoPackedPathName {
    unsigned short                  dataLength;                 /* dataLength excludes the space for the dataLength field */
                                                                /* Followed by data */
};
typedef struct ProtoPackedPathName      ProtoPackedPathName;

typedef PackedPathName *                PackedPathNamePtr;
typedef ProtoPackedPathName *           ProtoPackedPathNamePtr;
/*
Copy the contents of path1 to path2.  path2Length is the size of path2, and must
be large enough to hold a copy of path1.  A memFull error will be returned if that
is not the case.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OCECopyPackedPathName           (const PackedPathName * path1,
                                 PackedPathName *       path2,
                                 unsigned short         path2Length)                        THREEWORDINLINE(0x303C, 0x0304, 0xAA5C);


/*
Returns true if packed path pointer is nil, or is of zero length, or is of
length 2 and nParts of zero.
*/
EXTERN_API( Boolean )
OCEIsNullPackedPathName         (const PackedPathName * path)                               THREEWORDINLINE(0x303C, 0x031D, 0xAA5C);


/*
OCEUnpackPathName breaks apart the path into its component RStrings, writing string
pointers into the array 'parts', which the client asserts can hold as many as
'nParts' elements. The number of parts actually found is returned.  Strings are
placed in the array in order from lowest to highest.  The first pathName element
beneath the root appears last.  NOTE THAT THE UNPACKED STRUCT CONTAINS POINTERS INTO
THE PACKED STRUCT - DON'T DELETE OR REUSE THE PACKED STRUCT UNTIL YOU ARE FINISHED
WITH THE UNPACKED STRUCT AS WELL
*/
EXTERN_API( unsigned short )
OCEUnpackPathName               (const PackedPathName * path,
                                 RString *const         parts[],
                                 unsigned short         nParts)                             THREEWORDINLINE(0x303C, 0x0330, 0xAA5C);


/*
OCEPackedPathNameSize computes the number of bytes of memory needed to hold a
PackedPathName manufactured from the array of parts.  This length
includes the length of the length field of PackedPathName, so it
is safe to do a NewPtr (OCEPackedPathNameSize(...)).
*/
EXTERN_API( unsigned short )
OCEPackedPathNameSize           (const RStringPtr       parts[],
                                 unsigned short         nParts)                             THREEWORDINLINE(0x303C, 0x0328, 0xAA5C);


/* OCEDNodeNameCount returns the number of RStrings contained within the path. */
EXTERN_API( unsigned short )
OCEDNodeNameCount               (const PackedPathName * path)                               THREEWORDINLINE(0x303C, 0x032C, 0xAA5C);


/*
OCEPackPathName packs the parts into the storage provided as 'path'.  path must be
large enough to hold the packed pathname.  A memFull error will be returned if
pathLength is too small.  parts[0] should contain the deepest pathName element,
and parts[nParts - 1] should contain the name of the first pathName element beneath
the root. 
*/
EXTERN_API( OSErr )
OCEPackPathName                 (const RStringPtr       parts[],
                                 unsigned short         nParts,
                                 PackedPathName *       path,
                                 unsigned short         pathLength)                         THREEWORDINLINE(0x303C, 0x0323, 0xAA5C);


/*
Check the equality of two packed paths.
*/
EXTERN_API( Boolean )
OCEEqualPackedPathName          (const PackedPathName * path1,
                                 const PackedPathName * path2)                              THREEWORDINLINE(0x303C, 0x0311, 0xAA5C);


/*
OCEValidPackedPathName checks that the packed PathName is internally consistent.
Returns true if it's ok.
*/
EXTERN_API( Boolean )
OCEValidPackedPathName          (const PackedPathName * path)                               THREEWORDINLINE(0x303C, 0x0334, 0xAA5C);


/**** DirDiscriminator ****/
#endif  /* CALL_NOT_IN_CARBON */


struct DirDiscriminator {
    OCEDirectoryKind                signature;
    unsigned long                   misc;
};
typedef struct DirDiscriminator         DirDiscriminator;
/* Copies the value of disc1 to disc2. */
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
OCECopyDirDiscriminator         (const DirDiscriminator * disc1,
                                 DirDiscriminator *     disc2)                              THREEWORDINLINE(0x303C, 0x0301, 0xAA5C);


/* Check the equality of two DirDiscriminators. */
EXTERN_API( Boolean )
OCEEqualDirDiscriminator        (const DirDiscriminator * disc1,
                                 const DirDiscriminator * disc2)                            THREEWORDINLINE(0x303C, 0x030D, 0xAA5C);

/*
This structure is called RLI because it really contains all the info you
need to locate a record within the entire name space.  It contains four fields.
The first two are the name of the catalog and a catalog discriminator.  These
two fields are used to indicate to which catalog a given record belongs.  The
discriminator is used to distinguish between two different catalogs that have
the same name.

The other two fields in the RLI structure are used to indicate a particular
catalog node within the catalog specified by the directoryName and
discriminator fields.  These fields are exactly analagous to the dirID and
pathname used in HFS.  It is possible to specify a dNode just by dNodeNumber
(pathname is nil), or just by pathname (dNodeNumber is set to kNULLDNodeNumber),
or by a combination of the two.  The latter is called a 'partial pathname', and
while it is valid in the Catalog Manager API, it is not supported by ADAP
catalogs in Release 1.

Note that the path parameter does not include the catalog name, but holds
the names of all the nodes on the path to the desired catalog node, starting
with the catalog node and working its way up the tree.
*/

/**** RLI ****/

#endif  /* CALL_NOT_IN_CARBON */


struct DirectoryName {
    CharacterSet                    charSet;
    unsigned short                  dataLength;
    Byte                            body[32];                   /* space for catalog name */
};
typedef struct DirectoryName            DirectoryName;
typedef DirectoryName *                 DirectoryNamePtr;
/* Catalog node number */

typedef unsigned long                   DNodeNum;

struct RLI {
    DirectoryNamePtr                directoryName;              /* pointer to the name of the catalog root */
    DirDiscriminator                discriminator;              /* used to discriminate between dup catalog names */
    DNodeNum                        dNodeNumber;                /* number of the node */
    PackedPathNamePtr               path;                       /* old-style RLI */
};
typedef struct RLI                      RLI;
typedef RLI *                           RLIPtr;
/*
Create a new RLI from the catalog name, discriminator, DNode number, and
PackedPathName.  You must allocate the storage for the RLI and pass in a pointer
to it.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
OCENewRLI                       (RLI *                  newRLI,
                                 const DirectoryName *  dirName,
                                 DirDiscriminator *     discriminator,
                                 DNodeNum               dNodeNumber,
                                 const PackedPathName * path)                               THREEWORDINLINE(0x303C, 0x031F, 0xAA5C);


/*
Duplicate the contents of rli1 to rli2.  No errors are returned. This
simply copies the pointers to the catalog name and path, wiping out any pointer
that you might have had in there.
*/
EXTERN_API( void )
OCEDuplicateRLI                 (const RLI *            rli1,
                                 RLI *                  rli2)                               THREEWORDINLINE(0x303C, 0x030B, 0xAA5C);


/*
Copy the contents of rli1 to rli2.  rli2 must already contain
pointers to structures large enough to hold copies of the corresponding
fields from rli1.  A memFull error will be returned if that is not the case.
So if you allocate a brand new empty destination, you must at least set up
its length fields.
*/
EXTERN_API( OSErr )
OCECopyRLI                      (const RLI *            rli1,
                                 RLI *                  rli2)                               THREEWORDINLINE(0x303C, 0x0307, 0xAA5C);


/*
Check the equality of two RLIs.  This will take into account differences
in the case and diacriticals of the directoryName and the PathName.
NOTE THAT THIS WILL FAIL IF rli1 CONTAINS A DNODENUMBER AND A NIL PATHNAME,
AND rli2 CONTAINS kNULLDNodeNumber AND A NON-NIL PATHNAME.  IN OTHER WORDS,
THE TWO rlis MUST BE OF THE SAME FORM TO CHECK FOR EQUALITY.
The one exception is that if the pathname is nil, a dNodeNumber of zero and
kRootDNodeNumber will be treated as equal.
*/
EXTERN_API( Boolean )
OCEEqualRLI                     (const RLI *            rli1,
                                 const RLI *            rli2)                               THREEWORDINLINE(0x303C, 0x0315, 0xAA5C);


/*
Check the validity of an RLI.  This checks that the catalog name length
is within bounds, and the packed pathname (if specified) is valid.
*/
EXTERN_API( Boolean )
OCEValidRLI                     (const RLI *            theRLI)                             THREEWORDINLINE(0x303C, 0x0337, 0xAA5C);

/**** PackedRLI ****/
/*
struct PackedRLI is a maximum-sized structure.  Allocate one of
these and it will hold any valid packed pathname.
*/
#endif  /* CALL_NOT_IN_CARBON */

enum {
    kRLIMaxBytes                = (sizeof(RString) + sizeof(DirDiscriminator) + sizeof(DNodeNum) + kPathNameMaxBytes)
};


struct PackedRLI {
    unsigned short                  dataLength;                 /* dataLength excludes the space for the dataLength field */
    Byte                            data[1296];
};
typedef struct PackedRLI                PackedRLI;
/*
struct ProtoPackedRLI is a minimum-sized structure.  Use this
for a variable-length packed RLI.
*/

struct ProtoPackedRLI {
    unsigned short                  dataLength;                 /* dataLength excludes the space for the dataLength field */
                                                                /* Followed by data */
};
typedef struct ProtoPackedRLI           ProtoPackedRLI;

typedef PackedRLI *                     PackedRLIPtr;
typedef ProtoPackedRLI *                ProtoPackedRLIPtr;
/*
Copy the contents of prli1 to prli2.  prli2Length is the size of prli2, and must
be large enough to hold a copy of prli1.  A memFull error will be returned if that
is not the case.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OCECopyPackedRLI                (const PackedRLI *      prli1,
                                 PackedRLI *            prli2,
                                 unsigned short         prli2Length)                        THREEWORDINLINE(0x303C, 0x0305, 0xAA5C);


/*
OCEUnpackRLI breaks apart the prli into its components, writing pointers into
the rli structure.  NOTE THAT THE UNPACKED STRUCT CONTAINS POINTERS INTO THE
PACKED STRUCT - DON'T DELETE OR REUSE THE PACKED STRUCT UNTIL YOU ARE FINISHED
WITH THE UNPACKED STRUCT AS WELL
*/
EXTERN_API( void )
OCEUnpackRLI                    (const PackedRLI *      prli,
                                 RLI *                  theRLI)                             THREEWORDINLINE(0x303C, 0x0331, 0xAA5C);


/*
OCEPackedRLISize computes the number of bytes of memory needed to hold a
PackedRLI manufactured from an RLI.  This length
includes the length of the length field of PackedRLI, so it
is safe to do a NewPtr (OCEPackedRLISize(...)).
*/
EXTERN_API( unsigned short )
OCEPackedRLISize                (const RLI *            theRLI)                             THREEWORDINLINE(0x303C, 0x032A, 0xAA5C);


/*
OCEPackRLI packs the RLI into the storage provided as 'prli'.  prli must be
large enough to hold the packed RLI.  A memFull error will be returned if
prliLength is too small.
*/
EXTERN_API( OSErr )
OCEPackRLI                      (const RLI *            theRLI,
                                 PackedRLI *            prli,
                                 unsigned short         prliLength)                         THREEWORDINLINE(0x303C, 0x0324, 0xAA5C);


/*
OCEPackedRLIPartsSize computes the number of bytes of memory needed to hold a
PackedRLI manufactured from the parts of an RLI.  This length
includes the length of the length field of PackedRLI, so it
is safe to do a NewPtr (OCEPackedRLIPartsSize(...)).
*/
EXTERN_API( unsigned short )
OCEPackedRLIPartsSize           (const DirectoryName *  dirName,
                                 const RStringPtr       parts[],
                                 unsigned short         nParts)                             THREEWORDINLINE(0x303C, 0x0329, 0xAA5C);

/*
OCEPackRLIParts packs the parts of an RLI into the storage provided as 'prli'.
prli must be large enough to hold the packed RLI.  A memFull error will be returned
if prliLength is too small.
*/
EXTERN_API( OSErr )
OCEPackRLIParts                 (const DirectoryName *  dirName,
                                 const DirDiscriminator * discriminator,
                                 DNodeNum               dNodeNumber,
                                 const RStringPtr       parts[],
                                 unsigned short         nParts,
                                 PackedRLI *            prli,
                                 unsigned short         prliLength)                         THREEWORDINLINE(0x303C, 0x0325, 0xAA5C);

/*
Check the equality of two packed prlis.
*/
EXTERN_API( Boolean )
OCEEqualPackedRLI               (const PackedRLI *      prli1,
                                 const PackedRLI *      prli2)                              THREEWORDINLINE(0x303C, 0x0313, 0xAA5C);


/*
Check the validity of a packed RLI.  This checks that the catalog name length
is within bounds, and the packed pathname (if specified) is valid.
*/
EXTERN_API( Boolean )
OCEValidPackedRLI               (const PackedRLI *      prli)                               THREEWORDINLINE(0x303C, 0x0336, 0xAA5C);

/*
If this packed RLI describes a Personal Catalog, this call will return a pointer
to an alias record that can be used to find the actual file.  Otherwise, it returns nil.
*/
EXTERN_API( AliasPtr )
OCEExtractAlias                 (const PackedRLI *      prli)                               THREEWORDINLINE(0x303C, 0x0318, 0xAA5C);

/*
This call returns a pointer to a packed RLI that represents the "Catalogs" icon, or
the root of all catalogs.  It is used in the CollabPack.
*/
EXTERN_API( const PackedRLI *)
OCEGetDirectoryRootPackedRLI    (void)                                                      THREEWORDINLINE(0x303C, 0x0346, 0xAA5C);

/**** LocalRecordID ****/
#endif  /* CALL_NOT_IN_CARBON */


struct LocalRecordID {
    CreationID                      cid;
    RStringPtr                      recordName;
    RStringPtr                      recordType;
};
typedef struct LocalRecordID            LocalRecordID;
typedef LocalRecordID *                 LocalRecordIDPtr;
/* Create a LocalRecordID from a name, type, and CreationID */
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
OCENewLocalRecordID             (const RString *        recordName,
                                 const RString *        recordType,
                                 const CreationID *     cid,
                                 LocalRecordID *        lRID)                               THREEWORDINLINE(0x303C, 0x031E, 0xAA5C);

/*
Copy LocalRecordID lRID1 to LocalRecordID lRID2.  lRID2 must already contain
pointers to RString structures large enough to hold copies of the corresponding
fields from lRID1.  A memFull error will be returned if that is not the case.
So if you allocate a brand new empty destination, you must at least set up
its length field.
*/
EXTERN_API( OSErr )
OCECopyLocalRecordID            (const LocalRecordID *  lRID1,
                                 LocalRecordID *        lRID2)                              THREEWORDINLINE(0x303C, 0x0302, 0xAA5C);


/*
Check the equality of two local RIDs.
*/
EXTERN_API( Boolean )
OCEEqualLocalRecordID           (const LocalRecordID *  lRID1,
                                 const LocalRecordID *  lRID2)                              THREEWORDINLINE(0x303C, 0x030F, 0xAA5C);

/**** ShortRecordID ****/
#endif  /* CALL_NOT_IN_CARBON */


struct ShortRecordID {
    PackedRLIPtr                    rli;
    CreationID                      cid;
};
typedef struct ShortRecordID            ShortRecordID;
typedef ShortRecordID *                 ShortRecordIDPtr;
/* Create a ShortRecordID from an RLI struct and a CreationID */
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
OCENewShortRecordID             (const PackedRLI *      theRLI,
                                 const CreationID *     cid,
                                 ShortRecordIDPtr       sRID)                               THREEWORDINLINE(0x303C, 0x0321, 0xAA5C);


/*
Copy ShortRecordID sRID1 to ShortRecordID sRID2.  sRID2 must already contain
pointers to structures large enough to hold copies of the corresponding
fields from sRID1.  A memFull error will be returned if that is not the case.
So if you allocate a brand new empty destination, you must at least set up
its length fields.
*/
EXTERN_API( OSErr )
OCECopyShortRecordID            (const ShortRecordID *  sRID1,
                                 ShortRecordID *        sRID2)                              THREEWORDINLINE(0x303C, 0x030A, 0xAA5C);


/*
Check the equality of two short RIDs.
*/
EXTERN_API( Boolean )
OCEEqualShortRecordID           (const ShortRecordID *  sRID1,
                                 const ShortRecordID *  sRID2)                              THREEWORDINLINE(0x303C, 0x0317, 0xAA5C);

/**** RecordID ****/
#endif  /* CALL_NOT_IN_CARBON */


struct RecordID {
    PackedRLIPtr                    rli;                        /* pointer to a packed rli structure */
    LocalRecordID                   local;
};
typedef struct RecordID                 RecordID;
typedef RecordID *                      RecordIDPtr;
/*    Create a RecordID from a packed RLI struct and a LocalRecordID.
This doesn't allocate any new space; the RecordID points to the same
packed RLI struct and the same name and type RStrings. */
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
OCENewRecordID                  (const PackedRLI *      theRLI,
                                 const LocalRecordID *  lRID,
                                 RecordID *             rid)                                THREEWORDINLINE(0x303C, 0x0320, 0xAA5C);


/*
Copy RecordID RID1 to RecordID RID2.  RID2 must already contain
pointers to structures large enough to hold copies of the corresponding
fields from lRID1.  A memFull error will be returned if that is not the case.
So if you allocate a brand new empty destination, you must at least set up
its length fields.
*/
EXTERN_API( OSErr )
OCECopyRecordID                 (const RecordID *       rid1,
                                 const RecordID *       rid2)                               THREEWORDINLINE(0x303C, 0x0309, 0xAA5C);


/*  Check the equality of two RIDs. */
EXTERN_API( Boolean )
OCEEqualRecordID                (const RecordID *       rid1,
                                 const RecordID *       rid2)                               THREEWORDINLINE(0x303C, 0x0314, 0xAA5C);


/**** PackedRecordID ****/
/*
struct PackedRecordID is a maximum-sized structure.  Allocate one of
these and it will hold any valid packed RecordID.
*/
#endif  /* CALL_NOT_IN_CARBON */

enum {
    kPackedRecordIDMaxBytes     = (kPathNameMaxBytes + sizeof(DNodeNum) + sizeof(DirDiscriminator) + sizeof(CreationID) + (3 * sizeof(RString)))
};


struct PackedRecordID {
    unsigned short                  dataLength;                 /* dataLength excludes the space for the dataLength field */
    Byte                            data[1824];
};
typedef struct PackedRecordID           PackedRecordID;
/*
struct ProtoPackedRecordID is a minimum-sized structure.  Use this
for a variable-length packed RecordID.
*/

struct ProtoPackedRecordID {
    unsigned short                  dataLength;                 /* dataLength excludes the space for the dataLength field */
                                                                /* Followed by data */
};
typedef struct ProtoPackedRecordID      ProtoPackedRecordID;

typedef PackedRecordID *                PackedRecordIDPtr;
typedef ProtoPackedRecordID *           ProtoPackedRecordIDPtr;
/*
Copy PackedRecordID pRID1 to PackedRecordID pRID2.  pRID2 must already contain
pointers to structures large enough to hold copies of the corresponding
fields from pRID1.  A memFull error will be returned if that is not the case.
pRID2Length is the number of bytes that can be put into pRID2, not counting the
packed RecordID header.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OCECopyPackedRecordID           (const PackedRecordID * pRID1,
                                 const PackedRecordID * pRID2,
                                 unsigned short         pRID2Length)                        THREEWORDINLINE(0x303C, 0x0306, 0xAA5C);


/*
Create a RecordID from a PackedRecordID.
NOTE THAT THE UNPACKED STRUCT CONTAINS POINTERS INTO THE PACKED STRUCT - DON'T DELETE
OR REUSE THE PACKED STRUCT UNTIL YOU ARE FINISHED WITH THE UNPACKED STRUCT AS WELL
*/
EXTERN_API( void )
OCEUnpackRecordID               (const PackedRecordID * pRID,
                                 RecordID *             rid)                                THREEWORDINLINE(0x303C, 0x0332, 0xAA5C);


/*
Create a PackedRecordID from a RecordID.  pRID must be large enough to contain
the packed RecordID.  A memFull error will be returned if that is not the case.
packedRecordIDLength is the number of bytes that can be put into pRID, not
counting the header.
*/
EXTERN_API( OSErr )
OCEPackRecordID                 (const RecordID *       rid,
                                 PackedRecordID *       pRID,
                                 unsigned short         packedRecordIDLength)               THREEWORDINLINE(0x303C, 0x0326, 0xAA5C);


/*
Compute the number of bytes of memory needed to hold a RecordID when packed. This
length includes the length of the length field of PackedRecordID, so it
is safe to do a NewPtr (OCEPackedRecordIDSize(...)).
*/
EXTERN_API( unsigned short )
OCEPackedRecordIDSize           (const RecordID *       rid)                                THREEWORDINLINE(0x303C, 0x032B, 0xAA5C);


/*
Check the equality of two packed RIDs.
*/
EXTERN_API( Boolean )
OCEEqualPackedRecordID          (const PackedRecordID * pRID1,
                                 const PackedRecordID * pRID2)                              THREEWORDINLINE(0x303C, 0x0312, 0xAA5C);


/* OCEValidPackedRecordID checks the validity of a packed record ID. */
EXTERN_API( Boolean )
OCEValidPackedRecordID          (const PackedRecordID * pRID)                               THREEWORDINLINE(0x303C, 0x0335, 0xAA5C);


/**** DSSpec ****/
#endif  /* CALL_NOT_IN_CARBON */


struct DSSpec {
    RecordID *                      entitySpecifier;
    OSType                          extensionType;
    unsigned short                  extensionSize;
    void *                          extensionValue;
};
typedef struct DSSpec                   DSSpec;
typedef DSSpec *                        DSSpecPtr;
/*
struct PackedDSSpec is NOT a maximum-sized structure.  Allocate one of
these and it will hold any valid packed RecordID, but not necessarily any additional
data.
*/

enum {
    kPackedDSSpecMaxBytes       = (sizeof(PackedRecordID) + sizeof(OSType) + sizeof(UInt16))
};


struct PackedDSSpec {
    unsigned short                  dataLength;                 /* dataLength excludes the space for the dataLength field */
    Byte                            data[1832];
};
typedef struct PackedDSSpec             PackedDSSpec;
typedef PackedDSSpec *                  PackedDSSpecPtr;
typedef PackedDSSpecPtr *               PackedDSSpecHandle;
/*
struct ProtoPackedDSSpec is a minimum-sized structure.  Use this
for a variable-length packed DSSpec.
*/

struct ProtoPackedDSSpec {
    unsigned short                  dataLength;                 /* dataLength excludes the space for the dataLength field */
                                                                /* Followed by data */
};
typedef struct ProtoPackedDSSpec        ProtoPackedDSSpec;
typedef ProtoPackedDSSpec *             ProtoPackedDSSpecPtr;
/*
Copy PackedDSSpec pdss1 to PackedDSSpec pdss2.  pdss2 must already contain
pointers to structures large enough to hold copies of the corresponding
fields from pdss1.  A memFull error will be returned if that is not the case.
pdss2Length is the number of bytes that can be put into pdss2, not counting the
packed DSSpec header.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OCECopyPackedDSSpec             (const PackedDSSpec *   pdss1,
                                 const PackedDSSpec *   pdss2,
                                 unsigned short         pdss2Length)                        THREEWORDINLINE(0x303C, 0x0303, 0xAA5C);


/*
Create a DSSpec from a PackedDSSpec.
NOTE THAT THE UNPACKED STRUCT CONTAINS POINTERS INTO THE PACKED STRUCT - DON'T DELETE
OR REUSE THE PACKED STRUCT UNTIL YOU ARE FINISHED WITH THE UNPACKED STRUCT AS WELL.
A pointer to the extension is returned in dss->extensionValue, and the length of that
extension is returned in dss->extensionSize.  If there is no extension, dss->extensionValue will
be set to nil.  This routine will unpack the RecordID (if any) into rid, unpack the rest
into dss, and set dss->entitySpecifier to rid.
*/
EXTERN_API( void )
OCEUnpackDSSpec                 (const PackedDSSpec *   pdss,
                                 DSSpec *               dss,
                                 RecordID *             rid)                                THREEWORDINLINE(0x303C, 0x032F, 0xAA5C);


/*
Create a PackedDSSpec from a DSSpec.  pdss must be large enough to
contain the packed RecordID and any extension.  A memFull error will be returned if that
is not the case.  pdssLength is the number of bytes that can be put into pdss,
not counting the header.
*/
EXTERN_API( OSErr )
OCEPackDSSpec                   (const DSSpec *         dss,
                                 PackedDSSpec *         pdss,
                                 unsigned short         pdssLength)                         THREEWORDINLINE(0x303C, 0x0322, 0xAA5C);


/*
Compute the number of bytes of memory needed to hold a DSSpec when packed. This
length includes the length of the length field of PackedDSSpec, so it
is safe to do a NewPtr (OCEPackedDSSpecSize(...)).
*/
EXTERN_API( unsigned short )
OCEPackedDSSpecSize             (const DSSpec *         dss)                                THREEWORDINLINE(0x303C, 0x0327, 0xAA5C);


/*    Check the equality of two DSSpecs.  This compares all fields, even the
extension (unless extensionSize == 0).  The extensions are compared in a case-insensitive and
diacrit-insensitive manner. */
EXTERN_API( Boolean )
OCEEqualDSSpec                  (const DSSpec *         pdss1,
                                 const DSSpec *         pdss2)                              THREEWORDINLINE(0x303C, 0x030E, 0xAA5C);


/*    Check the equality of two PackedDSSpecs.  This compares all fields, even the
extension (unless extensionSize == 0).  The extensions are compared in a case-insensitive and
diacrit-insensitive manner. */
EXTERN_API( Boolean )
OCEEqualPackedDSSpec            (const PackedDSSpec *   pdss1,
                                 const PackedDSSpec *   pdss2)                              THREEWORDINLINE(0x303C, 0x0310, 0xAA5C);


/*
Check the validity of a PackedDSSpec.  If extensionType is
'entn', pdss must contain a valid entitySpecifier.  For all other extensionTypes, a nil
entitySpecifier is valid, but if non-nil, it will be checked for validity.  No check
is made on the extension.
*/
EXTERN_API( Boolean )
OCEValidPackedDSSpec            (const PackedDSSpec *   pdss)                               THREEWORDINLINE(0x303C, 0x0333, 0xAA5C);


/*
Return info about a DSSpec.  This routine does not check validity.  If the
DSSpec has no extension, we determine whether it represents the root of all
catalogs, a single catalog, a DNode, or a Record.  Else it is invalid.
If the DSSpec has an extension, we simply return the extension type.
*/
EXTERN_API( OSType )
OCEGetDSSpecInfo                (const DSSpec *         spec)                               THREEWORDINLINE(0x303C, 0x0319, 0xAA5C);


/* OCEGetExtensionType returns the extensionType imbedded in the PackedDSSpec. */
EXTERN_API( OSType )
OCEGetExtensionType             (const PackedDSSpec *   pdss)                               THREEWORDINLINE(0x303C, 0x031C, 0xAA5C);

/*
OCEStreamPackedDSSpec streams (flattens) a catalog object a little at a time by
calling the DSSpecStreamer routine that you provide.
*/
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( OSErr , DSSpecStreamerProcPtr )(void *buffer, unsigned long count, Boolean eof, long userData);
typedef STACK_UPP_TYPE(DSSpecStreamerProcPtr)                   DSSpecStreamerUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(DSSpecStreamerUPP)
    NewDSSpecStreamerUPP           (DSSpecStreamerProcPtr   userRoutine);

    EXTERN_API(void)
    DisposeDSSpecStreamerUPP       (DSSpecStreamerUPP       userUPP);

    EXTERN_API(OSErr)
    InvokeDSSpecStreamerUPP        (void *                  buffer,
                                    unsigned long           count,
                                    Boolean                 eof,
                                    long                    userData,
                                    DSSpecStreamerUPP       userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppDSSpecStreamerProcInfo = 0x000037E0 };                /* pascal 2_bytes Func(4_bytes, 4_bytes, 1_byte, 4_bytes) */
    #define NewDSSpecStreamerUPP(userRoutine)                       (DSSpecStreamerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDSSpecStreamerProcInfo, GetCurrentArchitecture())
    #define DisposeDSSpecStreamerUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define InvokeDSSpecStreamerUPP(buffer, count, eof, userData, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppDSSpecStreamerProcInfo, (buffer), (count), (eof), (userData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewDSSpecStreamerProc(userRoutine)                      NewDSSpecStreamerUPP(userRoutine)
#define CallDSSpecStreamerProc(userRoutine, buffer, count, eof, userData) InvokeDSSpecStreamerUPP(buffer, count, eof, userData, userRoutine)

typedef DSSpecStreamerUPP               DSSpecStreamer;
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OCEStreamPackedDSSpec           (const DSSpec *         dss,
                                 DSSpecStreamer         stream,
                                 long                   userData,
                                 unsigned long *        actualCount)                        THREEWORDINLINE(0x303C, 0x033D, 0xAA5C);

/**** AttributeType ****/
/*
For the record, an AttributeType is an RString with a smaller maximum size.
I don't just typedef it to an RString, because I want the definition of the AttributeType
struct to contain the max length, because I need to include it in the Attribute struct
below.  But it should be possible to typecast any AttributeType to an RString and use
all the RString utilities on it.
*/
#endif  /* CALL_NOT_IN_CARBON */


struct AttributeType {
    CharacterSet                    charSet;
    unsigned short                  dataLength;
    Byte                            body[32];                   /* always fixed at the max size */
};
typedef struct AttributeType            AttributeType;
typedef AttributeType *                 AttributeTypePtr;
/* Miscellaneous defines:  (these cannot be made into enums) */
enum {
    kMinPackedRStringLength     = (sizeof(ProtoRString))
};

enum {
    kMinPackedRLISize           = (sizeof(ProtoPackedRLI) + sizeof(DirDiscriminator) + sizeof(DNodeNum) + kMinPackedRStringLength + sizeof(ProtoPackedPathName))
};

/**** AttributeValue ****/
/* same class as is used in AppleEvents */

typedef DescType                        AttributeTag;

struct AttributeValue {
    AttributeTag                    tag;
    unsigned long                   dataLength;
    void *                          bytes;
};
typedef struct AttributeValue           AttributeValue;
typedef AttributeValue *                AttributeValuePtr;
/**** Attribute ****/

struct Attribute {
    AttributeType                   attributeType;
    AttributeCreationID             cid;
    AttributeValue                  value;
};
typedef struct Attribute                Attribute;
typedef Attribute *                     AttributePtr;
#if CALL_NOT_IN_CARBON
EXTERN_API( RString *)
OCEGetIndRecordType             (OCERecordTypeIndex     stringIndex)                        THREEWORDINLINE(0x303C, 0x031B, 0xAA5C);

EXTERN_API( AttributeType *)
OCEGetIndAttributeType          (OCEAttributeTypeIndex  stringIndex)                        THREEWORDINLINE(0x303C, 0x031A, 0xAA5C);

#endif  /* CALL_NOT_IN_CARBON */

enum {
    _oceTBDispatch              = 0xAA5E
};

/****************************************************************************************
   PLEASE NOTE! ROUTINES HAVE MOVED TO THIS HEADER!
 
   OCESizePackedRecipient, OCEPackRecipient, OCEUnpackRecipient, OCEStreamRecipient,
   OCEGetRecipientType, and OCESetRecipientType have moved to the OCE header file.
   The OCEMessaging header includes the OCE header, so no changes to your code are
   required.

****************************************************************************************/

typedef DSSpec                          OCERecipient;
enum {
    kOCESizePackedRecipient     = 830,
    kOCEPackRecipient           = 831,
    kOCEUnpackRecipient         = 832,
    kOCEStreamRecipient         = 833,
    kOCEGetRecipientType        = 834,
    kOCESetRecipientType        = 835
};

/*
Compute the space that a OCERecipient would take if it were in packed
form.  [Note: does NOT even pad extensionSize, so you may get an odd #back out]
Safe to pass dereferenced handle(s).
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( unsigned short )
OCESizePackedRecipient          (const OCERecipient *   rcpt)                               THREEWORDINLINE(0x303C, 0x033E, 0xAA5C);


/*
Take an OCERecipient (scatter) and (gather) stream into the specified
buffer.  It is assumed that there is sufficient space in the buffer (that is
OCESizePackedRecipient).  Safe to pass dereferenced handle(s).
*/
EXTERN_API( unsigned short )
OCEPackRecipient                (const OCERecipient *   rcpt,
                                 void *                 buffer)                             THREEWORDINLINE(0x303C, 0x033F, 0xAA5C);

/*
Take a packed OCERecipient and cast a the OCERecipient frame over it. Returns
amBadDestId if it doesn't look like an OCERecipient. Safe to pass dereferenced
handle(s).
*/
EXTERN_API( OSErr )
OCEUnpackRecipient              (const void *           buffer,
                                 OCERecipient *         rcpt,
                                 RecordID *             entitySpecifier)                    THREEWORDINLINE(0x303C, 0x0340, 0xAA5C);

/*
Take an OCERecipient (scatter) and (gather) stream using the specified
function.  Safe to pass dereferenced handle(s).  If streamer function returns
OCEError OCEStreamRecipient stops execution and passes the error back to the caller
*/
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API( OSErr , OCERecipientStreamerProcPtr )(void *buffer, unsigned long count, Boolean eof, long userData);
typedef STACK_UPP_TYPE(OCERecipientStreamerProcPtr)             OCERecipientStreamerUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(OCERecipientStreamerUPP)
    NewOCERecipientStreamerUPP     (OCERecipientStreamerProcPtr userRoutine);

    EXTERN_API(void)
    DisposeOCERecipientStreamerUPP    (OCERecipientStreamerUPP userUPP);

    EXTERN_API(OSErr)
    InvokeOCERecipientStreamerUPP    (void *                buffer,
                                    unsigned long           count,
                                    Boolean                 eof,
                                    long                    userData,
                                    OCERecipientStreamerUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppOCERecipientStreamerProcInfo = 0x000037E0 };          /* pascal 2_bytes Func(4_bytes, 4_bytes, 1_byte, 4_bytes) */
    #define NewOCERecipientStreamerUPP(userRoutine)                 (OCERecipientStreamerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppOCERecipientStreamerProcInfo, GetCurrentArchitecture())
    #define DisposeOCERecipientStreamerUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define InvokeOCERecipientStreamerUPP(buffer, count, eof, userData, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppOCERecipientStreamerProcInfo, (buffer), (count), (eof), (userData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewOCERecipientStreamerProc(userRoutine)                NewOCERecipientStreamerUPP(userRoutine)
#define CallOCERecipientStreamerProc(userRoutine, buffer, count, eof, userData) InvokeOCERecipientStreamerUPP(buffer, count, eof, userData, userRoutine)
typedef OCERecipientStreamerUPP         OCERecipientStreamer;
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
OCEStreamRecipient              (const OCERecipient *   rcpt,
                                 OCERecipientStreamer   stream,
                                 long                   userData,
                                 unsigned long *        actualCount)                        THREEWORDINLINE(0x303C, 0x0341, 0xAA5C);

/* Get the OCERecipient's extensionType. Safe to pass dereferenced handle(s).*/
EXTERN_API( OSType )
OCEGetRecipientType             (const CreationID *     cid)                                THREEWORDINLINE(0x303C, 0x0342, 0xAA5C);


/*
Set the OCERecipient's extensionType in the specified cid.  (Note: we do NOT
check for a nil pointer).  If the extensionType is 'entn', the cid is assumed
to be "valid" and is not touched.  Note: to properly handle non 'entn''s this
routine must and will zero the high long (source) of the cid! Safe to pass
dereferenced handle(s).
*/
EXTERN_API( void )
OCESetRecipientType             (OSType                 extensionType,
                                 CreationID *           cid)                                THREEWORDINLINE(0x303C, 0x0343, 0xAA5C);

/****************************************************************************************
   PLEASE NOTE! ROUTINES HAVE MOVED TO THIS HEADER!
 
   OCEGetAccessControlDSSpec and its corresponding data type and constants have
   moved to the OCE header from OCEAuthDir. The OCEAuthDir header includes the OCE
   header, so no changes to your code are required.
   
****************************************************************************************/
/* access categories bit numbers */
#endif  /* CALL_NOT_IN_CARBON */

enum {
    kThisRecordOwnerBit         = 0,
    kFriendsBit                 = 1,
    kAuthenticatedInDNodeBit    = 2,
    kAuthenticatedInDirectoryBit = 3,
    kGuestBit                   = 4,
    kMeBit                      = 5
};

/* Values of CategoryMask */
enum {
    kThisRecordOwnerMask        = 1,
    kFriendsMask                = 2,
    kAuthenticatedInDNodeMask   = 4,
    kAuthenticatedInDirectoryMask = 8,
    kGuestMask                  = 16,
    kMeMask                     = 32
};

typedef unsigned long                   CategoryMask;
/*
pass kThisRecordOwnerMask, kFriendsMask, kAuthenticatedInDNodeMask, kAuthenticatedInDirectoryMask,
kGuestMask, or kMeMask to this routine, and it will return a pointer to a
DSSpec that can be used in the Get or Set Access Controls calls.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( DSSpec *)
OCEGetAccessControlDSSpec       (CategoryMask           categoryBitMask)                    THREEWORDINLINE(0x303C, 0x0345, 0xAA5C);




#endif /* REZ */
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

#endif /* __OCE__ */

