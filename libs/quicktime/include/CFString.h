/*
     File:       CFString.h
 
     Contains:   CoreFoundation strings
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFSTRING__
#define __CFSTRING__

#ifndef __CFBASE__
#include <CFBase.h>
#endif

#ifndef __CFARRAY__
#include <CFArray.h>
#endif

#ifndef __CFDATA__
#include <CFData.h>
#endif

#ifndef __CFDICTIONARY__
#include <CFDictionary.h>
#endif


#include <stdarg.h>


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
Please note: CFStrings are conceptually an array of Unicode characters.
However, in general, how a CFString stores this array is an implementation
detail. For instance, CFString might choose to use an array of 8-bit characters;
to store its contents; or it might use multiple blocks of memory; or whatever.
Furthermore, the implementation might change depending on the default
system encoding, the user's language, the OS, or even a given release.

What this means is that you should use the following advanced functions with care:

  CFStringGetPascalStringPtr()
  CFStringGetCStringPtr()
  CFStringGetCharactersPtr()

These functions either return the desired pointer quickly, in constant time, or they
return NULL, which indicates you should use some of the other function, as shown 
in this example: 

    Str255 buffer;
    StringPtr ptr = CFStringGetPascalStringPtr(str, encoding);
    if (ptr == NULL) {
    if (CFStringGetPascalString(str, buffer, 256, encoding)) ptr = buffer;
    }

Note that CFStringGetPascalString call might still return NULL --- but that will happen 
in two circumstances only: The conversion from the UniChar contents of CFString
to the specified encoding fails, or the buffer is too small. 

If you need a copy of the buffer in the above example, you might consider simply
calling CFStringGetPascalString() in all cases --- CFStringGetPascalStringPtr()
is simply an optimization.

In addition, the following functions, which create immutable CFStrings from developer
supplied buffers without copying the buffers, might have to actually copy
under certain circumstances (If they do copy, the buffer will be dealt by the
"contentsDeallocator" argument.):

  CFStringCreateWithPascalStringNoCopy()
  CFStringCreateWithCStringNoCopy()
  CFStringCreateWithCharactersNoCopy()

You should of course never depend on the backing store of these CFStrings being
what you provided, and in other no circumstance should you change the contents
of that buffer (given that would break the invariant about the CFString being immutable).

Having said all this, there are actually ways to create a CFString where the backing store
is external, and can be manipulated by the developer or CFString itself:

  CFStringCreateMutableWithExternalCharactersNoCopy()
  CFStringSetExternalCharactersNoCopy()

A "contentsAllocator" is used to realloc or free the backing store by CFString.
kCFAllocatorNull can be provided to assure CFString will never realloc or free the buffer.
Developer can call CFStringSetExternalCharactersNoCopy() to update
CFString's idea of what's going on, if the buffer is changed externally. In these
strings, CFStringGetCharactersPtr() is guaranteed to return the external buffer.

These functions are here to allow wrapping a buffer of UniChar characters in a CFString,
allowing the buffer to passed into CFString functions and also manipulated via CFString
mutation functions. In general, developers should not use this technique for all strings,
as it prevents CFString from using certain optimizations.
*/
/*
    CFStringRef and CFMutableStringRef are defined in CFBase.h
*/

/* Identifier for character encoding; the values are the same as Text Encoding Converter TextEncoding.
*/
typedef UInt32                          CFStringEncoding;
/* Platform-independent built-in encodings; always available on all platforms.
*/
enum {
    kCFStringEncodingInvalidId  = -1L,
    kCFStringEncodingMacRoman   = 0L,
    kCFStringEncodingWindowsLatin1 = 0x0500,                    /* ANSI codepage 1252 */
    kCFStringEncodingISOLatin1  = 0x0201,                       /* ISO 8850 1 */
    kCFStringEncodingNextStepLatin = 0x0B01,                    /* NextStep encoding*/
    kCFStringEncodingASCII      = 0x0600,                       /* 0..127 */
    kCFStringEncodingUnicode    = 0x0100,                       /* kTextEncodingUnicodeDefault  + kTextEncodingDefaultFormat (aka kUnicode16BitFormat) */
    kCFStringEncodingUTF8       = 0x08000100,                   /* kTextEncodingUnicodeDefault + kUnicodeUTF8Format */
    kCFStringEncodingNonLossyASCII = 0x0BFF                     /* 7bit Unicode variants used by YellowBox & Java */
};


/* CFString type ID
*/
EXTERN_API_C( CFTypeID )
CFStringGetTypeID               (void);

/* Macro to allow creation of compile-time constant strings; the argument should be a constant string. This will work for now but we need something better.
*/
#define CFSTR(cStr)  __CFStringMakeConstantString(cStr "")

/*** Immutable string creation functions ***/
/* Functions to create basic immutable strings. The provided allocator is used for all memory activity in these functions.
*/
/* These functions copy the provided buffer into CFString's internal storage.
*/
EXTERN_API_C( CFStringRef )
CFStringCreateWithPascalString  (CFAllocatorRef         alloc,
                                 ConstStringPtr         pStr,
                                 CFStringEncoding       encoding);

EXTERN_API_C( CFStringRef )
CFStringCreateWithCString       (CFAllocatorRef         alloc,
                                 const char *           cStr,
                                 CFStringEncoding       encoding);

EXTERN_API_C( CFStringRef )
CFStringCreateWithCharacters    (CFAllocatorRef         alloc,
                                 const UniChar *        chars,
                                 CFIndex                numChars);

/* These functions try not to copy the provided buffer. The buffer will be 
deallocated with the provided contentsDeallocator when it's no longer needed;
to not free the buffer, specify kCFAllocatorNull here. As usual, NULL means
default allocator.
NOTE: Do not count on these buffers as being used by the string; 
in some cases the CFString might free the buffer and use something else
(for instance if it decides to always use Unicode encoding internally). 
In addition, some encodings are not used internally; in
those cases CFString might also dump the provided buffer and use its own.
*/

EXTERN_API_C( CFStringRef )
CFStringCreateWithPascalStringNoCopy (CFAllocatorRef    alloc,
                                 ConstStringPtr         pStr,
                                 CFStringEncoding       encoding,
                                 CFAllocatorRef         contentsDeallocator);

EXTERN_API_C( CFStringRef )
CFStringCreateWithCStringNoCopy (CFAllocatorRef         alloc,
                                 const char *           cStr,
                                 CFStringEncoding       encoding,
                                 CFAllocatorRef         contentsDeallocator);

EXTERN_API_C( CFStringRef )
CFStringCreateWithCharactersNoCopy (CFAllocatorRef      alloc,
                                 const UniChar *        chars,
                                 CFIndex                numChars,
                                 CFAllocatorRef         contentsDeallocator);

/* Create copies of part or all of the string.
*/
EXTERN_API_C( CFStringRef )
CFStringCreateWithSubstring     (CFAllocatorRef         alloc,
                                 CFStringRef            str,
                                 CFRange                range);

EXTERN_API_C( CFStringRef )
CFStringCreateCopy              (CFAllocatorRef         alloc,
                                 CFStringRef            theString);

/* These functions create a CFString from the provided printf-format and arguments.
*/
EXTERN_API_C( CFStringRef )
CFStringCreateWithFormat        (CFAllocatorRef         alloc,
                                 CFDictionaryRef        formatOptions,
                                 CFStringRef            format,
                                 ...);

EXTERN_API_C( CFStringRef )
CFStringCreateWithFormatAndArguments (CFAllocatorRef    alloc,
                                 CFDictionaryRef        formatOptions,
                                 CFStringRef            format,
                                 va_list                arguments);

/* Functions to create mutable strings. "maxLength", if not 0, is a hard bound on the length of the string. If 0, there is no limit on the length.
*/
EXTERN_API_C( CFMutableStringRef )
CFStringCreateMutable           (CFAllocatorRef         alloc,
                                 CFIndex                maxLength);

EXTERN_API_C( CFMutableStringRef )
CFStringCreateMutableCopy       (CFAllocatorRef         alloc,
                                 CFIndex                maxLength,
                                 CFStringRef            theString);

/* This function creates a mutable string that has a developer supplied and directly editable backing store.
The string will be manipulated within the provided buffer (if any) until it outgrows capacity; then the
externalCharactersAllocator will be consulted for more memory. When the CFString is deallocated, the
buffer will be freed with the externalCharactersAllocator. Provide kCFAllocatorNull here to prevent the buffer
from ever being reallocated or deallocated by CFString. See comments at top of this file for more info.
*/
EXTERN_API_C( CFMutableStringRef )
CFStringCreateMutableWithExternalCharactersNoCopy (CFAllocatorRef  alloc,
                                 UniChar *              chars,
                                 CFIndex                numChars,
                                 CFIndex                capacity,
                                 CFAllocatorRef         externalCharactersAllocator);


/*** Basic accessors for the contents ***/
/* Number of 16-bit Unicode characters in the string.
*/
EXTERN_API_C( CFIndex )
CFStringGetLength               (CFStringRef            theString);

/* Extracting the contents of the string. For obtaining multiple characters, calling
CFStringGetCharacters() is more efficient than multiple calls to CFStringGetCharacterAtIndex().
If the length of the string is not known (so you can't use a fixed size buffer for CFStringGetCharacters()),
another method is to use is CFStringGetCharacterFromInlineBuffer() (see further below).
*/
EXTERN_API_C( UniChar )
CFStringGetCharacterAtIndex     (CFStringRef            theString,
                                 CFIndex                idx);

EXTERN_API_C( void )
CFStringGetCharacters           (CFStringRef            theString,
                                 CFRange                range,
                                 UniChar *              buffer);


/*** Conversion to other encodings ***/

/* These two convert into the provided buffer; they return FALSE if conversion isn't possible
(due to conversion error, or not enough space in the provided buffer). 
These functions do zero-terminate or put the length byte; the provided bufferSize should include
space for this (so pass 256 for Str255). More sophisticated usages can go through CFStringGetBytes().
*/
EXTERN_API_C( Boolean )
CFStringGetPascalString         (CFStringRef            theString,
                                 StringPtr              buffer,
                                 CFIndex                bufferSize,
                                 CFStringEncoding       encoding);

EXTERN_API_C( Boolean )
CFStringGetCString              (CFStringRef            theString,
                                 char *                 buffer,
                                 CFIndex                bufferSize,
                                 CFStringEncoding       encoding);

/* These functions attempt to return in O(1) time the desired format for the string.
Note that although this means a pointer to the internal structure is being returned,
this can't always be counted on. Please see note at the top of the file for more
details.
*/
EXTERN_API_C( ConstStringPtr )
CFStringGetPascalStringPtr      (CFStringRef            theString,
                                 CFStringEncoding       encoding);

/* Be prepared for NULL */
EXTERN_API_C( const char *)
CFStringGetCStringPtr           (CFStringRef            theString,
                                 CFStringEncoding       encoding);

/* Be prepared for NULL */
EXTERN_API_C( const UniChar *)
CFStringGetCharactersPtr        (CFStringRef            theString);

/* Be prepared for NULL */
/* The primitive conversion routine; allows you to convert a string piece at a time
   into a fixed size buffer. Returns number of characters converted. 
   Characters that cannot be converted to the specified encoding are represented
   with the byte specified by lossByte; if lossByte is 0, then lossy conversion
   is not allowed and conversion stops, returning partial results.
   Pass buffer==NULL if you don't care about the converted string (but just the convertability,
   or number of bytes required, indicated by usedBufLen). 
   maxBufLength indicates the maximum number of bytes to generate; it is consulted even
   if buffer is NULL, so pass in INT_MAX if you want to find out the maximum number of bytes.
   Does not zero-terminate. If you want to create Pascal or C string, allow one extra byte at start or end. 
   Setting isExternalRepresentation causes any extra bytes that would allow 
   the data to be made persistent to be included; for instance, the Unicode BOM.
*/
EXTERN_API_C( CFIndex )
CFStringGetBytes                (CFStringRef            theString,
                                 CFRange                range,
                                 CFStringEncoding       encoding,
                                 UInt8                  lossByte,
                                 Boolean                isExternalRepresentation,
                                 UInt8 *                buffer,
                                 CFIndex                maxBufLen,
                                 CFIndex *              usedBufLen);

/* This one goes the other way by creating a CFString from a bag of bytes. 
This is much like CFStringCreateWithPascalString or CFStringCreateWithCString, 
except the length is supplied explicitly. In addition, you can specify whether 
the data is an external format --- that is, whether to pay attention to the 
BOM character (if any) and do byte swapping if necessary
*/
EXTERN_API_C( CFStringRef )
CFStringCreateWithBytes         (CFAllocatorRef         alloc,
                                 const UInt8 *          bytes,
                                 CFIndex                numBytes,
                                 CFStringEncoding       encoding,
                                 Boolean                isExternalRepresentation);

/* Convenience functions String <-> Data. These generate "external" formats, that is, formats that
   can be written out to disk. For instance, if the encoding is Unicode, CFStringCreateFromExternalRepresentation()
   pays attention to the BOM character (if any) and does byte swapping if necessary.
   Similarly CFStringCreateExternalRepresentation() will always include a BOM character if the encoding is
   Unicode. See above for description of lossByte.
*/
EXTERN_API_C( CFStringRef )
CFStringCreateFromExternalRepresentation (CFAllocatorRef  alloc,
                                 CFDataRef              data,
                                 CFStringEncoding       encoding);

/* May return NULL on conversion error */
EXTERN_API_C( CFDataRef )
CFStringCreateExternalRepresentation (CFAllocatorRef    alloc,
                                 CFStringRef            theString,
                                 CFStringEncoding       encoding,
                                 UInt8                  lossByte);

/* May return NULL on conversion error */
/* Hints about the contents of a string
*/
EXTERN_API_C( CFStringEncoding )
CFStringGetSmallestEncoding     (CFStringRef            theString);

/* Result in O(n) time max */
EXTERN_API_C( CFStringEncoding )
CFStringGetFastestEncoding      (CFStringRef            theString);

/* Result in O(1) time max */
/* General encoding info
*/
EXTERN_API_C( CFStringEncoding )
CFStringGetSystemEncoding       (void);

/* The default encoding for the system; untagged 8-bit characters are usually in this encoding */
EXTERN_API_C( CFIndex )
CFStringGetMaximumSizeForEncoding (CFIndex              length,
                                 CFStringEncoding       encoding);

/* Max bytes a string of specified length (in UniChars) will take up if encoded */

/*** Comparison functions. ***/
enum {
                                                                /* Flags used in all find and compare operations */
    kCFCompareCaseInsensitive   = 1,
    kCFCompareBackwards         = 4,                            /* Starting from the end of the string */
    kCFCompareAnchored          = 8,                            /* Only at the specified starting point */
    kCFCompareNonliteral        = 16,                           /* If specified, loose equivalence is performed (o-umlaut == o, umlaut) */
    kCFCompareLocalized         = 32,                           /* User's default locale is used for the comparisons */
    kCFCompareNumerically       = 64                            /* Numeric comparison is used; that is, Foo2.txt < Foo7.txt < Foo25.txt */
};

/* The main comparison routine; compares specified range of the string to another.
   locale == NULL indicates canonical locale
*/
EXTERN_API_C( CFComparisonResult )
CFStringCompareWithOptions      (CFStringRef            string1,
                                 CFStringRef            string2,
                                 CFRange                rangeToCompare,
                                 CFOptionFlags          compareOptions);

/* Comparison convenience suitable for passing as sorting functions.
*/
EXTERN_API_C( CFComparisonResult )
CFStringCompare                 (CFStringRef            string1,
                                 CFStringRef            string2,
                                 CFOptionFlags          compareOptions);

/* Find routines; CFStringFindWithOptions() returns the found range in the CFRange * argument;  You can pass NULL for simple discovery check.
   CFStringCreateArrayWithFindResults() returns an array of CFRange pointers, or NULL if there are no matches.
*/
EXTERN_API_C( Boolean )
CFStringFindWithOptions         (CFStringRef            theString,
                                 CFStringRef            stringToFind,
                                 CFRange                rangeToSearch,
                                 CFOptionFlags          searchOptions,
                                 CFRange *              result);

EXTERN_API_C( CFArrayRef )
CFStringCreateArrayWithFindResults (CFAllocatorRef      alloc,
                                 CFStringRef            theString,
                                 CFStringRef            stringToFind,
                                 CFRange                rangeToSearch,
                                 CFOptionFlags          compareOptions);

/* Find conveniences
*/
EXTERN_API_C( CFRange )
CFStringFind                    (CFStringRef            theString,
                                 CFStringRef            stringToFind,
                                 CFOptionFlags          compareOptions);

EXTERN_API_C( Boolean )
CFStringHasPrefix               (CFStringRef            theString,
                                 CFStringRef            prefix);

EXTERN_API_C( Boolean )
CFStringHasSuffix               (CFStringRef            theString,
                                 CFStringRef            suffix);

/* Find range of bounds of the line(s) that span the indicated range (startIndex, numChars),
   taking into account various possible line separator sequences (CR, CRLF, LF, and Unicode LS, PS).
   All return values are "optional" (provide NULL if you don't want them)
     lineStartIndex: index of first character in line
     lineEndIndex: index of first character of the next line (including terminating line separator characters)
     contentsEndIndex: index of the first line separator character
   Thus, lineEndIndex - lineStartIndex is the number of chars in the line, including the line separators
         contentsEndIndex - lineStartIndex is the number of chars in the line w/out the line separators
*/
EXTERN_API_C( void )
CFStringGetLineBounds           (CFStringRef            theString,
                                 CFRange                range,
                                 CFIndex *              lineBeginIndex,
                                 CFIndex *              lineEndIndex,
                                 CFIndex *              contentsEndIndex);

/*** Exploding and joining strings with a separator string ***/

EXTERN_API_C( CFStringRef )
CFStringCreateByCombiningStrings (CFAllocatorRef        alloc,
                                 CFArrayRef             theArray,
                                 CFStringRef            separatorString);

/* Empty array returns empty string; one element array returns the element */
EXTERN_API_C( CFArrayRef )
CFStringCreateArrayBySeparatingStrings (CFAllocatorRef  alloc,
                                 CFStringRef            theString,
                                 CFStringRef            separatorString);

/* No separators in the string returns array with that string; string == sep returns two empty strings */
/*** Parsing non-localized numbers from strings ***/

EXTERN_API_C( SInt32 )
CFStringGetIntValue             (CFStringRef            str);

/* Skips whitespace; returns 0 on error, MAX or -MAX on overflow */
EXTERN_API_C( double )
CFStringGetDoubleValue          (CFStringRef            str);

/* Skips whitespace; returns 0.0 on error */
/*** MutableString functions ***/
/* CFStringAppend("abcdef", "xxxxx") -> "abcdefxxxxx"
   CFStringDelete("abcdef", 2, 3) -> "abf"
   CFStringReplace("abcdef", 2, 3, "xxxxx") -> "abxxxxxf"
   CFStringReplaceAll("abcdef", "xxxxx") -> "xxxxx"
*/
EXTERN_API_C( void )
CFStringAppend                  (CFMutableStringRef     theString,
                                 CFStringRef            appendedString);

EXTERN_API_C( void )
CFStringAppendCharacters        (CFMutableStringRef     theString,
                                 const UniChar *        chars,
                                 CFIndex                numChars);

EXTERN_API_C( void )
CFStringAppendPascalString      (CFMutableStringRef     theString,
                                 ConstStringPtr         pStr,
                                 CFStringEncoding       encoding);

EXTERN_API_C( void )
CFStringAppendCString           (CFMutableStringRef     theString,
                                 const char *           cStr,
                                 CFStringEncoding       encoding);

EXTERN_API_C( void )
CFStringAppendFormat            (CFMutableStringRef     theString,
                                 CFDictionaryRef        formatOptions,
                                 CFStringRef            format,
                                 ...);

EXTERN_API_C( void )
CFStringAppendFormatAndArguments (CFMutableStringRef    theString,
                                 CFDictionaryRef        formatOptions,
                                 CFStringRef            format,
                                 va_list                arguments);

EXTERN_API_C( void )
CFStringInsert                  (CFMutableStringRef     str,
                                 CFIndex                idx,
                                 CFStringRef            insertedStr);

EXTERN_API_C( void )
CFStringDelete                  (CFMutableStringRef     theString,
                                 CFRange                range);

EXTERN_API_C( void )
CFStringReplace                 (CFMutableStringRef     theString,
                                 CFRange                range,
                                 CFStringRef            replacement);

EXTERN_API_C( void )
CFStringReplaceAll              (CFMutableStringRef     theString,
                                 CFStringRef            replacement);

/* Replaces whole string */
/* This function will make the contents of a mutable CFString point directly at the specified UniChar array.
it works only with CFStrings created with CFStringCreateMutableWithExternalCharactersNoCopy().
This function does not free the previous buffer.
The string will be manipulated within the provided buffer (if any) until it outgrows capacity; then the
externalCharactersAllocator will be consulted for more memory.
See comments at the top of this file for more info.
*/
EXTERN_API_C( void )
CFStringSetExternalCharactersNoCopy (CFMutableStringRef  theString,
                                 UniChar *              chars,
                                 CFIndex                length,
                                 CFIndex                capacity);

/* Works only on specially created mutable strings! */

/* CFStringPad() will pad or cut down a string to the specified size.
   The pad string is used as the fill string; indexIntoPad specifies which character to start with.
     CFStringPad("abc", " ", 9, 0) ->  "abc      "
     CFStringPad("abc", ". ", 9, 1) -> "abc . . ."
     CFStringPad("abcdef", ?, 3, ?) -> "abc"

     CFStringTrim() will trim the specified string from both ends of the string.
     CFStringTrimWhitespace() will do the same with white space characters (tab, newline, etc)
     CFStringTrim("  abc ", " ") -> "abc"
     CFStringTrim("* * * *abc * ", "* ") -> "*abc "
*/
EXTERN_API_C( void )
CFStringPad                     (CFMutableStringRef     theString,
                                 CFStringRef            padString,
                                 CFIndex                length,
                                 CFIndex                indexIntoPad);

EXTERN_API_C( void )
CFStringTrim                    (CFMutableStringRef     theString,
                                 CFStringRef            trimString);

EXTERN_API_C( void )
CFStringTrimWhitespace          (CFMutableStringRef     theString);


EXTERN_API_C( void )
CFStringLowercase               (CFMutableStringRef     theString,
                                 const void *           localeTBD);

EXTERN_API_C( void )
CFStringUppercase               (CFMutableStringRef     theString,
                                 const void *           localeTBD);

EXTERN_API_C( void )
CFStringCapitalize              (CFMutableStringRef     theString,
                                 const void *           localeTBD);


/* This returns availability of the encoding on the system
*/
EXTERN_API_C( Boolean )
CFStringIsEncodingAvailable     (CFStringEncoding       encoding);

/* This function returns list of available encodings.  The returned list is terminated with kCFStringEncodingInvalidId and owned by the system.
*/
EXTERN_API_C( const CFStringEncoding *)
CFStringGetListOfAvailableEncodings (void);

/* Returns name of the encoding
*/
EXTERN_API_C( CFStringRef )
CFStringGetNameOfEncoding       (CFStringEncoding       encoding);

/* ID mapping functions from/to YellowBox NSStringEncoding.  Returns kCFStringEncodingInvalidId if no mapping exists.
*/
EXTERN_API_C( UInt32 )
CFStringConvertEncodingToNSStringEncoding (CFStringEncoding  encoding);

EXTERN_API_C( CFStringEncoding )
CFStringConvertNSStringEncodingToEncoding (UInt32       encoding);

/* ID mapping functions from/to Microsoft Windows codepage (covers both OEM & ANSI).  Returns kCFStringEncodingInvalidId if no mapping exists.
*/
EXTERN_API_C( UInt32 )
CFStringConvertEncodingToWindowsCodepage (CFStringEncoding  encoding);

EXTERN_API_C( CFStringEncoding )
CFStringConvertWindowsCodepageToEncoding (UInt32        codepage);

/* ID mapping functions from/to IANA registery charset names.  Returns kCFStringEncodingInvalidId if no mapping exists.
*/
EXTERN_API_C( CFStringEncoding )
CFStringConvertIANACharSetNameToEncoding (CFStringRef   theString);

EXTERN_API_C( CFStringRef )
CFStringConvertEncodingToIANACharSetName (CFStringEncoding  encoding);


/* The next two functions allow "fast" access to the contents of a string, 
   assuming you are doing sequential or localized accesses. To use, call
   CFStringInitInlineBuffer() with a CFStringInlineBuffer (on the stack, say),
   and a range in the string to look at. Then call CFStringGetCharacterFromInlineBuffer()
   as many times as you want, with a index into that range (relative to the start
   of that range). These are INLINE functions and will end up calling CFString only 
   once in a while, to fill a buffer.  
*/
#define __kCFStringInlineBufferLength 64
typedef struct {
    UniChar buffer[__kCFStringInlineBufferLength];
    CFStringRef theString;
    const UniChar *directBuffer;
    CFRange rangeToBuffer;        /* Range in string to buffer */
    CFIndex bufferedRangeStart;     /* Start of range currently buffered (relative to rangeToBuffer.location) */
    CFIndex bufferedRangeEnd;      /* bufferedRangeStart + number of chars actually buffered */
} CFStringInlineBuffer;

#if defined(CF_INLINE)
CF_INLINE void CFStringInitInlineBuffer(CFStringRef str, CFStringInlineBuffer *buf, CFRange range) {
    buf->theString = str;
    buf->rangeToBuffer = range;
    buf->directBuffer = CFStringGetCharactersPtr(str);
    buf->bufferedRangeStart = buf->bufferedRangeEnd = 0;
}


CF_INLINE UniChar CFStringGetCharacterFromInlineBuffer(CFStringInlineBuffer *buf, CFIndex idx) {
    if (buf->directBuffer) return buf->directBuffer[idx + buf->rangeToBuffer.location];
    if (idx >= buf->bufferedRangeEnd || idx < buf->bufferedRangeStart) {
 if (idx < 0 || idx > buf->rangeToBuffer.length) return 0;
  if ((buf->bufferedRangeStart = idx - 4) < 0) buf->bufferedRangeStart = 0;
  buf->bufferedRangeEnd = buf->bufferedRangeStart + __kCFStringInlineBufferLength;
   if (buf->bufferedRangeEnd > buf->rangeToBuffer.length) buf->bufferedRangeEnd = buf->rangeToBuffer.length;
  CFStringGetCharacters(buf->theString, CFRangeMake(buf->rangeToBuffer.location + buf->bufferedRangeStart, buf->bufferedRangeEnd - buf->bufferedRangeStart), buf->buffer);
    }
    return buf->buffer[idx - buf->bufferedRangeStart];
}

#else
/* If INLINE functions are not available, we do somewhat less powerful macros that work similarly (except be aware that the buf argument ise evaluated multiple times).
*/
#define CFStringInitInlineBuffer(str, buf, range) \
    {(buf)->theString = str; (buf)->rangeToBuffer = range; (buf)->directBuffer = CFStringGetCharactersPtr(str);}

#define CFStringGetCharacterFromInlineBuffer(buf, idx) \
    ((buf)->directBuffer ? (buf)->directBuffer[(idx) + (buf)->rangeToBuffer.location] : CFStringGetCharacterAtIndex((buf)->theString, (idx) + (buf)->rangeToBuffer.location))

#endif /* CF_INLINE */

/* Rest of the stuff in this file is private and should not be used directly
*/
/* For debugging only
   Use CFShow() to printf the description of any CFType;
   Use CFShowStr() to printf detailed info about a CFString
*/
EXTERN_API_C( void )
CFShow                          (CFTypeRef              obj);

EXTERN_API_C( void )
CFShowStr                       (CFStringRef            str);

/* This function is private and should not be used directly
*/
EXTERN_API_C( CFStringRef )
__CFStringMakeConstantString    (const char *           cStr);

/* Private; do not use */

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

#endif /* __CFSTRING__ */

