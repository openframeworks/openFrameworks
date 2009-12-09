/*
     File:       DigitalSignature.h
 
     Contains:   Digital Signature Interfaces.
 
     Version:    Technology: AOCE toolbox 1.02
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DIGITALSIGNATURE__
#define __DIGITALSIGNATURE__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
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

/* values of SIGNameAttributeType */
enum {
    kSIGCountryCode             = 0,
    kSIGOrganization            = 1,
    kSIGStreetAddress           = 2,
    kSIGState                   = 3,
    kSIGLocality                = 4,
    kSIGCommonName              = 5,
    kSIGTitle                   = 6,
    kSIGOrganizationUnit        = 7,
    kSIGPostalCode              = 8
};

typedef unsigned short                  SIGNameAttributeType;
/* 
Certificate status codes returned in SIGCertInfo or SIGSignerInfo from
either SIGGetCertInfo or SIGGetSignerInfo respectively. kSIGValid means that
the certificate is currently valid. kSIGPending means the certificate is
currently not valid - but will be.  kSIGExpired means the certificate has
expired. A time is always associated with a SIGCertStatus.  In each case the
time has a specific interpretation.  When the status is kSIGValid the time is
when the certificate will expire. When the status is kSIGPending the time is
when the certificate will become valid. When the status is kSIGExpired the time
is when the certificate expired. In the SIGCertInfo structure, the startDate
and endDate fields hold the appropriate date information.  In the SIGSignerInfo
structure, this information is provided in the certSetStatusTime field. In the
SIGSignerInfo struct, the status time is actually represented by the SIGSignatureStatus
field which can contain any of the types below. NOTE: The only time you will get 
a kSIGInvalid status is when it pertains to a SIGSignatureStatus field and only when
you get a signature that was created after the certificates expiration date, something
we are not allowing on the Mac but that may not be restricted on other platforms. Also, 
it will not be possible to get a kSIGPending value for SIGSignatureStatus on the Mac but
possibly allowed by other platforms.
*/
/* Values for SIGCertStatus or SIGSignatureStatus */
enum {
    kSIGValid                   = 0,                            /* possible for either a SIGCertStatus or SIGSignatureStatus */
    kSIGPending                 = 1,                            /* possible for either a SIGCertStatus or SIGSignatureStatus */
    kSIGExpired                 = 2,                            /* possible for either a SIGCertStatus or SIGSignatureStatus * possible only for a SIGSignatureStatus */
    kSIGInvalid                 = 3
};

typedef unsigned short                  SIGCertStatus;
typedef unsigned short                  SIGSignatureStatus;
/* Number of bytes needed for a digest record when using SIGDigest */
enum {
    kSIGDigestSize              = 16
};

typedef Byte                            SIGDigestData[16];
typedef Byte *                          SIGDigestDataPtr;

struct SIGCertInfo {
    unsigned long                   startDate;                  /* cert start validity date */
    unsigned long                   endDate;                    /* cert end validity date */
    SIGCertStatus                   certStatus;                 /* see comment on SIGCertStatus for definition */
    unsigned long                   certAttributeCount;         /* number of name attributes in this cert */
    unsigned long                   issuerAttributeCount;       /* number of name attributes in this certs issuer */
    Str255                          serialNumber;               /* cert serial number */
};
typedef struct SIGCertInfo              SIGCertInfo;
typedef SIGCertInfo *                   SIGCertInfoPtr;

struct SIGSignerInfo {
    unsigned long                   signingTime;                /* time of signing */
    unsigned long                   certCount;                  /* number of certificates in the cert set */
    unsigned long                   certSetStatusTime;          /* Worst cert status time. See comment on SIGCertStatus for definition */
    SIGSignatureStatus              signatureStatus;            /* The status of the signature. See comment on SIGCertStatus for definition*/
};
typedef struct SIGSignerInfo            SIGSignerInfo;
typedef SIGSignerInfo *                 SIGSignerInfoPtr;

struct SIGNameAttributesInfo {
    Boolean                         onNewLevel;
    Boolean                         filler1;
    SIGNameAttributeType            attributeType;
    ScriptCode                      attributeScript;
    Str255                          attribute;
};
typedef struct SIGNameAttributesInfo    SIGNameAttributesInfo;
typedef SIGNameAttributesInfo *         SIGNameAttributesInfoPtr;

typedef Ptr                             SIGContextPtr;
typedef Ptr                             SIGSignaturePtr;
/*
Certificates are always in order. That is, the signers cert is always 0, the
issuer of the signers cert is always 1 etc... to the number of certificates-1.
You can use this constant for readability in your code.
*/
enum {
    kSIGSignerCertIndex         = 0
};

/*
Call back procedure supplied by developer, return false to cancel the current
process.
*/
typedef CALLBACK_API( Boolean , SIGStatusProcPtr )(void );
typedef STACK_UPP_TYPE(SIGStatusProcPtr)                        SIGStatusUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(SIGStatusUPP)
    NewSIGStatusUPP                (SIGStatusProcPtr        userRoutine);

    EXTERN_API(void)
    DisposeSIGStatusUPP            (SIGStatusUPP            userUPP);

    EXTERN_API(Boolean)
    InvokeSIGStatusUPP             (SIGStatusUPP            userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppSIGStatusProcInfo = 0x00000010 };                     /* pascal 1_byte Func() */
    #define NewSIGStatusUPP(userRoutine)                            (SIGStatusUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSIGStatusProcInfo, GetCurrentArchitecture())
    #define DisposeSIGStatusUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define InvokeSIGStatusUPP(userUPP)                             (Boolean)CALL_ZERO_PARAMETER_UPP((userUPP), uppSIGStatusProcInfo)
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewSIGStatusProc(userRoutine)                           NewSIGStatusUPP(userRoutine)
#define CallSIGStatusProc(userRoutine)                          InvokeSIGStatusUPP(userRoutine)
/*
Resource id's of standard signature icon suite, all sizes and colors are available.
*/
enum {
    kSIGSignatureIconResID      = -16797,
    kSIGValidSignatureIconResID = -16799,
    kSIGInvalidSignatureIconResID = -16798
};

/* ------------------------------- CONTEXT CALLS ------------------------------- 
To use the Digital Signature toolbox you will need a SIGContextPtr.  To create
a SIGContextPtr you simply call SIGNewContext and it will create and initialize
a context for you.  To free the memory occupied by the context and invalidate
its internal data, call SIGDisposeContext. An initialized context has no notion
of the type of operation it will be performing however, once you call
SIGSignPrepare SIGVerifyPrepare, or SIGDigestPrepare, the contexts operation
type is set and to switch  to another type of operation will require creating a
new context. Be sure to pass the same context to corresponding toolbox calls
(ie SIGSignPrepare, SIGProcessData, SIGSign)  in other words mixing lets say
signing and verify calls with the same context is not allowed.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
SIGNewContext                   (SIGContextPtr *        context)                            FOURWORDINLINE(0x203C, 0x0002, 0x076C, 0xAA5D);

EXTERN_API( OSErr )
SIGDisposeContext               (SIGContextPtr          context)                            FOURWORDINLINE(0x203C, 0x0002, 0x076D, 0xAA5D);


/* ------------------------------- SIGNING CALLS ------------------------------- 
Once you have created a SIGContextPtr, you create a signature by calling
SIGSignPrepare once, followed by n calls to SIGProcessData, followed by one call
toRcpt SIGSign. To create another signature on different data but for the same
signer, don't dispose of the context and call SIGProcessData for the new data
followed by a call SIGSign again. In this case the signer will not be prompted
for their signer and password again as it was already provided.  Once you call
SIGDisposeContext, all signer information will be cleared out of the context and
the signer will be re-prompted.  The signer file FSSpecPtr should be set to nil
if you want the toolbox to use the last signer by default or prompt for a signer
if none exists.  The prompt parameter can be used to pass a string to be displayed
in the dialog that prompts the user for their password.  If the substring "^1"
(without the quotes) is in the prompt string, then the toolbox will replace it
with the name of the signer from the signer selected by the user.  If an empty
string is passed, the following default string will be sent to the toolbox
"\pSigning as ^1.".  You can call any of the utility routines after SIGSignPrepare
or SIGSign to get information about the signer or certs.
*/
EXTERN_API( OSErr )
SIGSignPrepare                  (SIGContextPtr          context,
                                 const FSSpec *         signerFile,
                                 ConstStr255Param       prompt,
                                 Size *                 signatureSize)                      FOURWORDINLINE(0x203C, 0x0008, 0x076E, 0xAA5D);

EXTERN_API( OSErr )
SIGSign                         (SIGContextPtr          context,
                                 SIGSignaturePtr        signature,
                                 SIGStatusUPP           statusProc)                         FOURWORDINLINE(0x203C, 0x0006, 0x076F, 0xAA5D);


/* ------------------------------- VERIFYING CALLS ------------------------------- 
Once you have created a SIGContextPtr, you verify a signature by calling
SIGVerifyPrepare  once, followed by n calls to SIGProcessData, followed by one
call to SIGVerify. Check the return code from SIGVerify to see if the signature
verified or not (noErr is returned on  success otherwise the appropriate error
code).  Upon successfull verification, you can call any of the utility routines
toRcpt find out who signed the data.
*/
EXTERN_API( OSErr )
SIGVerifyPrepare                (SIGContextPtr          context,
                                 SIGSignaturePtr        signature,
                                 Size                   signatureSize,
                                 SIGStatusUPP           statusProc)                         FOURWORDINLINE(0x203C, 0x0008, 0x0770, 0xAA5D);

EXTERN_API( OSErr )
SIGVerify                       (SIGContextPtr          context)                            FOURWORDINLINE(0x203C, 0x0002, 0x0771, 0xAA5D);

/* ------------------------------ DIGESTING CALLS ------------------------------ 
Once you have created a SIGContextPtr, you create a digest by calling
SIGDigestPrepare once, followed by n calls to SIGProcessData, followed by one
call to SIGDigest.  You can dispose of the context after SIGDigest as the
SIGDigestData does not reference back into it.  SIGDigest returns the digest in
digest.
*/
EXTERN_API( OSErr )
SIGDigestPrepare                (SIGContextPtr          context)                            FOURWORDINLINE(0x203C, 0x0002, 0x0772, 0xAA5D);

EXTERN_API( OSErr )
SIGDigest                       (SIGContextPtr          context,
                                 SIGDigestData          digest)                             FOURWORDINLINE(0x203C, 0x0004, 0x0773, 0xAA5D);


/* ------------------------------ PROCESSING DATA ------------------------------ 
To process data during a digest, sign, or verify operation call SIGProcessData
as many times as necessary and with any sized blocks of data.  The data needs to
be processed in the same order during corresponding sign and verify operations
but does not need to be processed in the same sized chunks (i.e., the toolbox
just sees it as a continuous bit stream).
*/
EXTERN_API( OSErr )
SIGProcessData                  (SIGContextPtr          context,
                                 const void *           data,
                                 Size                   dataSize)                           FOURWORDINLINE(0x203C, 0x0006, 0x0774, 0xAA5D);


/* ------------------------------- UTILITY CALLS ------------------------------- 
Given a context that has successfully performed a verification SIGShowSigner
will  display a modal dialog with the entire distinguished name of the person
who signed the data. the prompt (if supplied) will appear at the top of the
dialog.  If no prompt is specified, the default prompt "\pVerification
Successfull." will appear.

Given a context that has been populated by calling SIGSignPrepare, SIGSign or a
successful SIGVerify, you can make the remaining utility calls:

SIGGetSignerInfo will return the SignerInfo record.  The certCount can be used
toRcpt index into the certificate set when calling SIGGetCertInfo,
SIGGetCertNameAttributes or SIGGetCertIssuerNameAttributes. The signingTime is
only defined if the call is made after SIGSign  or SIGVerify. The certSetStatus
will tell you the best status of the entire certificate set while
certSetStatusTime will correspond to the time associated with that status (see
definitions above).

SIGGetCertInfo will return the SIGCertInfo record when given a valid index into
the cert set in  certIndex.  Note: The cert at index kSIGSignerCertIndex is
always the signers certificate.  The  serial number, start date and end date
are there should you wish to display that info.  The  certAttributeCount and
issuerAttributeCount provide the number of parts in the name of that certificate
or that certificates issuer respectively.  You use these numbers to index into
either SIGGetCertNameAttributes or SIGGetCertIssuerNameAttributes to retrieve
the name. The certStatus will tell you the status of the certificate while
certStatusTime will correspond to the time associated with that status (see
definitions above).

SIGGetCertNameAttributes and SIGGetCertIssuerNameAttributes return name parts
of the certificate at  certIndex and attributeIndex.  The newLevel return value
tells you wether the name attribute returned is at the same level in the name
hierarchy as the previous attribute.  The type return value tells you  the type
of attribute returned. nameAttribute is the actual string containing the name
attribute.   So, if you wanted to display the entire distinguished name of the
person who's signature was just validated you could do something like this;

    (...... variable declarations and verification code would preceed this sample ......)

    error = SIGGetCertInfo(verifyContext, kSIGSignerCertIndex, &certInfo);
    HandleErr(error);

    for (i = 0; i <= certInfo.certAttributeCount-1; i++)
        {
        error = SIGGetCertNameAttributes(
            verifyContext, kSIGSignerCertIndex, i, &newLevel, &type, theAttribute);
        HandleErr(error);
        DisplayNamePart(theAttribute, type, newLevel);
        }
*/
EXTERN_API( OSErr )
SIGShowSigner                   (SIGContextPtr          context,
                                 ConstStr255Param       prompt)                             FOURWORDINLINE(0x203C, 0x0004, 0x0775, 0xAA5D);

EXTERN_API( OSErr )
SIGGetSignerInfo                (SIGContextPtr          context,
                                 SIGSignerInfo *        signerInfo)                         FOURWORDINLINE(0x203C, 0x0004, 0x0776, 0xAA5D);

EXTERN_API( OSErr )
SIGGetCertInfo                  (SIGContextPtr          context,
                                 unsigned long          certIndex,
                                 SIGCertInfo *          certInfo)                           FOURWORDINLINE(0x203C, 0x0006, 0x0777, 0xAA5D);

EXTERN_API( OSErr )
SIGGetCertNameAttributes        (SIGContextPtr          context,
                                 unsigned long          certIndex,
                                 unsigned long          attributeIndex,
                                 SIGNameAttributesInfo * attributeInfo)                     FOURWORDINLINE(0x203C, 0x0008, 0x0778, 0xAA5D);

EXTERN_API( OSErr )
SIGGetCertIssuerNameAttributes  (SIGContextPtr          context,
                                 unsigned long          certIndex,
                                 unsigned long          attributeIndex,
                                 SIGNameAttributesInfo * attributeInfo)                     FOURWORDINLINE(0x203C, 0x0008, 0x0779, 0xAA5D);



/* --------------------------- FILE SIGN & VERIFY CALLS -------------------------- 
These calls allow you to detect the presence of a standard signtaure in a file as 
well as sign and verify files in a standard way.  An example of this is the Finder, 
which uses these calls to allow the user to "drop sign" a file.

To detect if a file is signed in the standard way, pass the FSSpec of the file to SIGFileIsSigned.
A result of noErr means the file is in fact signed, otherwise, a kSIGNoSignature error will
be returned.

Once you have created a SIGContextPtr, you can make calls to either sign or verify a file in
a standard way: 

To sign a file, call SIGSignPrepare followed by 'n' number of calls to SIGSignFile,
passing it the file spec for each file you wish to sign in turn.  You supply the context, the signature 
size that was returned from SIGSignPrepare and an optional call back proc.  The call will take care of all
the processing of data and affixing the signature to the file. If a signature already exists in the file, 
it is replaced with the newly created signature.

To verify a file that was signed using SIGSignFile, call SIGVerifyFile passing it a new context and 
the file spec.  Once this call has completed, if the verification is successfull, you can pass the context 
to SIGShowSigner to display the name of the person who signed the file.
*/
EXTERN_API( OSErr )
SIGFileIsSigned                 (const FSSpec *         fileSpec)                           FOURWORDINLINE(0x203C, 0x0002, 0x09C4, 0xAA5D);

EXTERN_API( OSErr )
SIGSignFile                     (SIGContextPtr          context,
                                 Size                   signatureSize,
                                 const FSSpec *         fileSpec,
                                 SIGStatusUPP           statusProc)                         FOURWORDINLINE(0x203C, 0x0008, 0x09C5, 0xAA5D);

EXTERN_API( OSErr )
SIGVerifyFile                   (SIGContextPtr          context,
                                 const FSSpec *         fileSpec,
                                 SIGStatusUPP           statusProc)                         FOURWORDINLINE(0x203C, 0x0006, 0x09C6, 0xAA5D);

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

#endif /* __DIGITALSIGNATURE__ */

