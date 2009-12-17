/*
     File:       TextEncodingConverter.h
 
     Contains:   Text Encoding Conversion Interfaces.
 
     Version:    Technology: Mac OS 9.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TEXTENCODINGCONVERTER__
#define __TEXTENCODINGCONVERTER__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __TEXTCOMMON__
#include <TextCommon.h>
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

typedef OSType                          TECPluginSignature;
typedef UInt32                          TECPluginVersion;
/* plugin signatures */
enum {
    kTECSignature               = FOUR_CHAR_CODE('encv'),
    kTECUnicodePluginSignature  = FOUR_CHAR_CODE('puni'),
    kTECJapanesePluginSignature = FOUR_CHAR_CODE('pjpn'),
    kTECChinesePluginSignature  = FOUR_CHAR_CODE('pzho'),
    kTECKoreanPluginSignature   = FOUR_CHAR_CODE('pkor')
};


/* converter object reference */
typedef struct OpaqueTECObjectRef*      TECObjectRef;
typedef struct OpaqueTECSnifferObjectRef*  TECSnifferObjectRef;
typedef OSType                          TECPluginSig;

struct TECConversionInfo {
    TextEncoding                    sourceEncoding;
    TextEncoding                    destinationEncoding;
    UInt16                          reserved1;
    UInt16                          reserved2;
};
typedef struct TECConversionInfo        TECConversionInfo;
/* return number of encodings types supported by user's configuraton of the encoding converter */
EXTERN_API( OSStatus )
TECCountAvailableTextEncodings  (ItemCount *            numberEncodings);

/* fill in an array of type TextEncoding passed in by the user with types of encodings the current configuration of the encoder can handle. */
EXTERN_API( OSStatus )
TECGetAvailableTextEncodings    (TextEncoding           availableEncodings[],
                                 ItemCount              maxAvailableEncodings,
                                 ItemCount *            actualAvailableEncodings);

/* return number of from-to encoding conversion pairs supported  */
EXTERN_API( OSStatus )
TECCountDirectTextEncodingConversions (ItemCount *      numberOfEncodings);

/* fill in an array of type TextEncodingPair passed in by the user with types of encoding pairs the current configuration of the encoder can handle. */
EXTERN_API( OSStatus )
TECGetDirectTextEncodingConversions (TECConversionInfo  availableConversions[],
                                 ItemCount              maxAvailableConversions,
                                 ItemCount *            actualAvailableConversions);

/* return number of encodings a given encoding can be converter into */
EXTERN_API( OSStatus )
TECCountDestinationTextEncodings (TextEncoding          inputEncoding,
                                 ItemCount *            numberOfEncodings);

/* fill in an array of type TextEncodingPair passed in by the user with types of encodings pairs the current configuration of the encoder can handle. */
EXTERN_API( OSStatus )
TECGetDestinationTextEncodings  (TextEncoding           inputEncoding,
                                 TextEncoding           destinationEncodings[],
                                 ItemCount              maxDestinationEncodings,
                                 ItemCount *            actualDestinationEncodings);

/* get info about a text encoding */
EXTERN_API( OSStatus )
TECGetTextEncodingInternetName  (TextEncoding           textEncoding,
                                 Str255                 encodingName);

EXTERN_API( OSStatus )
TECGetTextEncodingFromInternetName (TextEncoding *      textEncoding,
                                 ConstStr255Param       encodingName);

/* create/dispose converters */
EXTERN_API( OSStatus )
TECCreateConverter              (TECObjectRef *         newEncodingConverter,
                                 TextEncoding           inputEncoding,
                                 TextEncoding           outputEncoding);

EXTERN_API( OSStatus )
TECCreateConverterFromPath      (TECObjectRef *         newEncodingConverter,
                                 const TextEncoding     inPath[],
                                 ItemCount              inEncodings);

EXTERN_API( OSStatus )
TECDisposeConverter             (TECObjectRef           newEncodingConverter);

/* convert text encodings */
EXTERN_API( OSStatus )
TECClearConverterContextInfo    (TECObjectRef           encodingConverter);

EXTERN_API( OSStatus )
TECConvertText                  (TECObjectRef           encodingConverter,
                                 ConstTextPtr           inputBuffer,
                                 ByteCount              inputBufferLength,
                                 ByteCount *            actualInputLength,
                                 TextPtr                outputBuffer,
                                 ByteCount              outputBufferLength,
                                 ByteCount *            actualOutputLength);

EXTERN_API( OSStatus )
TECFlushText                    (TECObjectRef           encodingConverter,
                                 TextPtr                outputBuffer,
                                 ByteCount              outputBufferLength,
                                 ByteCount *            actualOutputLength);

/* one-to-many routines */
EXTERN_API( OSStatus )
TECCountSubTextEncodings        (TextEncoding           inputEncoding,
                                 ItemCount *            numberOfEncodings);

EXTERN_API( OSStatus )
TECGetSubTextEncodings          (TextEncoding           inputEncoding,
                                 TextEncoding           subEncodings[],
                                 ItemCount              maxSubEncodings,
                                 ItemCount *            actualSubEncodings);

EXTERN_API( OSStatus )
TECGetEncodingList              (TECObjectRef           encodingConverter,
                                 ItemCount *            numEncodings,
                                 Handle *               encodingList);

EXTERN_API( OSStatus )
TECCreateOneToManyConverter     (TECObjectRef *         newEncodingConverter,
                                 TextEncoding           inputEncoding,
                                 ItemCount              numOutputEncodings,
                                 const TextEncoding     outputEncodings[]);

EXTERN_API( OSStatus )
TECConvertTextToMultipleEncodings (TECObjectRef         encodingConverter,
                                 ConstTextPtr           inputBuffer,
                                 ByteCount              inputBufferLength,
                                 ByteCount *            actualInputLength,
                                 TextPtr                outputBuffer,
                                 ByteCount              outputBufferLength,
                                 ByteCount *            actualOutputLength,
                                 TextEncodingRun        outEncodingsBuffer[],
                                 ItemCount              maxOutEncodingRuns,
                                 ItemCount *            actualOutEncodingRuns);

EXTERN_API( OSStatus )
TECFlushMultipleEncodings       (TECObjectRef           encodingConverter,
                                 TextPtr                outputBuffer,
                                 ByteCount              outputBufferLength,
                                 ByteCount *            actualOutputLength,
                                 TextEncodingRun        outEncodingsBuffer[],
                                 ItemCount              maxOutEncodingRuns,
                                 ItemCount *            actualOutEncodingRuns);

/* international internet info */
EXTERN_API( OSStatus )
TECCountWebTextEncodings        (RegionCode             locale,
                                 ItemCount *            numberEncodings);

EXTERN_API( OSStatus )
TECGetWebTextEncodings          (RegionCode             locale,
                                 TextEncoding           availableEncodings[],
                                 ItemCount              maxAvailableEncodings,
                                 ItemCount *            actualAvailableEncodings);

EXTERN_API( OSStatus )
TECCountMailTextEncodings       (RegionCode             locale,
                                 ItemCount *            numberEncodings);

EXTERN_API( OSStatus )
TECGetMailTextEncodings         (RegionCode             locale,
                                 TextEncoding           availableEncodings[],
                                 ItemCount              maxAvailableEncodings,
                                 ItemCount *            actualAvailableEncodings);

/* examine text encodings */
EXTERN_API( OSStatus )
TECCountAvailableSniffers       (ItemCount *            numberOfEncodings);

EXTERN_API( OSStatus )
TECGetAvailableSniffers         (TextEncoding           availableSniffers[],
                                 ItemCount              maxAvailableSniffers,
                                 ItemCount *            actualAvailableSniffers);

EXTERN_API( OSStatus )
TECCreateSniffer                (TECSnifferObjectRef *  encodingSniffer,
                                 TextEncoding           testEncodings[],
                                 ItemCount              numTextEncodings);

EXTERN_API( OSStatus )
TECSniffTextEncoding            (TECSnifferObjectRef    encodingSniffer,
                                 TextPtr                inputBuffer,
                                 ByteCount              inputBufferLength,
                                 TextEncoding           testEncodings[],
                                 ItemCount              numTextEncodings,
                                 ItemCount              numErrsArray[],
                                 ItemCount              maxErrs,
                                 ItemCount              numFeaturesArray[],
                                 ItemCount              maxFeatures);

EXTERN_API( OSStatus )
TECDisposeSniffer               (TECSnifferObjectRef    encodingSniffer);

EXTERN_API( OSStatus )
TECClearSnifferContextInfo      (TECSnifferObjectRef    encodingSniffer);

#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
TECSetBasicOptions              (TECObjectRef           encodingConverter,
                                 OptionBits             controlFlags);

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

#endif /* __TEXTENCODINGCONVERTER__ */

