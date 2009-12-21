/*
     File:       FileSigning.h
 
     Contains:   Apple File Signing Interfaces.
 
     Version:    Technology: 1.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FILESIGNING__
#define __FILESIGNING__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __KEYCHAIN__
#include <Keychain.h>
#endif

#ifndef __CRYPTOMESSAGESYNTAX__
#include <CryptoMessageSyntax.h>
#endif


/* Data structures and types */


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
    kSecOptionProgress          = 1 << 0,
    kSecOptionShowVerifyUI      = 1 << 1,
    kSecOptionNeverShowUI       = 1 << 2,
    kCertUsageReserved          = 0x7FFFFF00                    /* Last 3 bytes reserved for SecOptions flags */
};

typedef UInt32                          SecOptions;
enum {
    kSecSignatureTypeRawPKCS7   = 0,
    kSecSignatureTypeCMS        = 1,
    kSecSignatureTypePGP        = 2                             /* reserved but not implemented */
};

typedef UInt32                          SecSignatureType;
enum {
    kSecTrustPolicyCodeSigning  = 0,
    kSecTrustPolicyPersonalFileSigning = 1
};

typedef UInt32                          SecTrustPolicy;

struct SecProgressCallbackInfo {
    UInt32                          version;
    UInt32                          bytesProcessed;
    UInt32                          totalBytes;
    UInt32                          itemsRemainingToSign;
    UInt32                          totalItemsToSign;
    UInt32                          secondsRemaining;
    UInt32                          secondsElapsed;
    UInt32                          microSecondsPerByte;
    Str255                          fileName;
};
typedef struct SecProgressCallbackInfo  SecProgressCallbackInfo;
typedef CALLBACK_API( OSStatus , SecProgressCallbackProcPtr )(SecProgressCallbackInfo *callbackInfo, void *userContext);
typedef STACK_UPP_TYPE(SecProgressCallbackProcPtr)              SecProgressCallbackUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(SecProgressCallbackUPP)
    NewSecProgressCallbackUPP      (SecProgressCallbackProcPtr userRoutine);

    EXTERN_API(void)
    DisposeSecProgressCallbackUPP    (SecProgressCallbackUPP userUPP);

    EXTERN_API(OSStatus)
    InvokeSecProgressCallbackUPP    (SecProgressCallbackInfo * callbackInfo,
                                    void *                  userContext,
                                    SecProgressCallbackUPP  userUPP);

#else
    enum { uppSecProgressCallbackProcInfo = 0x000003F0 };           /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    #define NewSecProgressCallbackUPP(userRoutine)                  (SecProgressCallbackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSecProgressCallbackProcInfo, GetCurrentArchitecture())
    #define DisposeSecProgressCallbackUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define InvokeSecProgressCallbackUPP(callbackInfo, userContext, userUPP)  (OSStatus)CALL_TWO_PARAMETER_UPP((userUPP), uppSecProgressCallbackProcInfo, (callbackInfo), (userContext))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewSecProgressCallbackProc(userRoutine)                 NewSecProgressCallbackUPP(userRoutine)
#define CallSecProgressCallbackProc(userRoutine, callbackInfo, userContext) InvokeSecProgressCallbackUPP(callbackInfo, userContext, userRoutine)
EXTERN_API( OSStatus )
SecSetProgressCallback          (SecProgressCallbackUPP  callbackProc,
                                 void *                 userContext);

EXTERN_API( OSStatus )
SecRemoveProgressCallback       (SecProgressCallbackUPP  callbackProc);

EXTERN_API( OSStatus )
DefaultSecProgressCallbackProc  (SecProgressCallbackInfo * info,
                                 void *                 userContext);

enum {
    kSecDefaultSignatureResID   = 1
};

EXTERN_API( OSStatus )
SecMacSignFile                  (FSSpec *               theFile,
                                 KCItemRef              signingCertificate,
                                 Handle                 sigH,
                                 SecOptions             options,
                                 SecProgressCallbackUPP  progressProc,
                                 void *                 userContext);

EXTERN_API( OSStatus )
SecMacSignFileSimple            (FSSpec *               theFile,
                                 KCItemRef              signingCertificate,
                                 SecOptions             options);

EXTERN_API( OSStatus )
SecMacLoadSigPound              (FSSpec *               theFile,
                                 Handle *               resHandle);

EXTERN_API( OSStatus )
SecMacRemoveSignature           (FSSpec *               theFile,
                                 SecSignatureType       signatureType,
                                 SInt16                 signatureToRemove);

EXTERN_API( OSStatus )
SecMacRemoveAllSignatures       (FSSpec *               theFile,
                                 SecSignatureType       signatureType);


EXTERN_API( OSStatus )
SecMacVerifyFile                (FSSpec *               theFile,
                                 SecOptions             options,
                                 SecProgressCallbackUPP  progressProc,
                                 void *                 userContext,
                                 SInt16                 signatureToVerify,
                                 CFArrayRef             policyOIDs,
                                 KCVerifyStopOn         stopOn,
                                 SecSignerRef *         signer);

EXTERN_API( OSStatus )
SecMacVerifyFileSimple          (FSSpec *               theFile,
                                 SecOptions             options,
                                 SecTrustPolicy         trustPolicy,
                                 SecSignerRef *         signer);

EXTERN_API( Boolean )
SecMacHasSignature              (FSSpec *               theFile,
                                 SecSignatureType       signatureType);

EXTERN_API( OSStatus )
SecMacFindSignatureToVerify     (FSSpec *               theFile,
                                 SecSignatureType       signatureType,
                                 SInt16 *               signatureToVerify);

EXTERN_API( CFArrayRef )
SecMacGetDefaultPolicyOIDs      (SecTrustPolicy         trustPolicy);


EXTERN_API( OSStatus )
SecMacDisplaySigner             (OSStatus               inputErr,
                                 SecSignerRef           signer,
                                 Boolean                alwaysShowUI,
                                 FSSpec *               theFile);

/* Errors Codes */
enum {
    errSecTooManySigners        = -13839,
    errSecSigningFailed         = -13838,
    errSecCorruptSigPound       = -13837,
    errSecNoSignatureFound      = -13836
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

#endif /* __FILESIGNING__ */

